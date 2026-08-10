#include "../include/parser.h"
#include "../include/ast.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// static ASTNode *parse_sequence(Parser *parser) {
//     if (parser == NULL) {
//         return NULL;
//     }
//
// }
//

int parser_initialize(Parser *parser, const Lexer *lexer) {
    if (parser == NULL || lexer == NULL) {
        return -1;
    }

    parser->lexer = lexer;
    parser->current = 0;
    parser->panic_mode = false;
    parser->error = NULL;

    return 0;
}

const char *parser_last_error(const Parser *parser) {
    if (parser == NULL) {
        return NULL;
    }

    return parser->error;
}

void parser_destroy(Parser *parser) {
    if (parser == NULL) {
        return;
    }

    parser->error = NULL;
    parser->lexer = NULL;
    parser->current = 0;
    parser->panic_mode = false;

    return;
}

/**
 * Append a new redirection to a command's redirection list.
 *
 * @param command Command receiving the new redirection.
 * @return Pointer to the newly allocated redirection, or NULL on failure.
 */
static Redirect *append_redirect(Command *command) {
    if (command == NULL) {
        return NULL;
    }

    Redirect **link = &command->redirects;

    while (*link != NULL) {
        link = &(*link)->next;
    }

    *link = shell_malloc(sizeof(**link));
    if (*link == NULL) {
        return NULL;
    }

    (*link)->next = NULL;

    return *link;
}

/**
 * Parse redirection operators and their targets into a command.
 *
 * Supported redirections:
 *   <  file   - stdin input
 *   >  file   - stdout output
 *   >> file   - stdout append
 *   << word   - stdin heredoc
 *
 * @param parser Parser containing the token stream and current position.
 * @param command Command receiving the parsed redirections.
 * @return 0 on success, or -1 on invalid input/allocation failure.
 */
static int parse_redirections(Parser *parser, Command *command) {
    if (parser == NULL || command == NULL || parser->lexer == NULL) {
        return -1;
    }

    const Lexer *lexer = parser->lexer;

    while (lexer->tokens[parser->current].type != TOKEN_END) {

        if (parser->current + 1 >= lexer->count) {
            return -1;
        }

        const Token *current_token = &lexer->tokens[parser->current];

        const Token *next_token = &lexer->tokens[parser->current + 1];

        Redirect *current_redirect = NULL;

        switch (current_token->type) {
        case TOKEN_REDIR_IN:
            if (next_token->type != TOKEN_WORD) {
                return -1;
            }

            current_redirect = append_redirect(command);
            if (current_redirect == NULL) {
                return -1;
            }

            current_redirect->type = REDIR_INPUT;
            current_redirect->fd = 0;

            size_t length = next_token->length;

            char *text = shell_malloc(length + 1);

            if (text == NULL) {
                return -1;
            }

            memcpy(text, next_token->text, length + 1);

            current_redirect->target = text;

            parser->current += 2;
            break;

        case TOKEN_REDIR_OUT:
            if (next_token->type != TOKEN_WORD) {
                return -1;
            }

            current_redirect = append_redirect(command);
            if (current_redirect == NULL) {
                return -1;
            }

            current_redirect->type = REDIR_OUTPUT;
            current_redirect->fd = 1;
            current_redirect->target = next_token->text;

            parser->current += 2;
            break;

        case TOKEN_HEREDOC:
            if (next_token->type != TOKEN_WORD) {
                return -1;
            }

            current_redirect = append_redirect(command);
            if (current_redirect == NULL) {
                return -1;
            }

            current_redirect->type = REDIR_HEREDOC;
            current_redirect->fd = 0;
            current_redirect->target = next_token->text;

            parser->current += 2;
            break;

        case TOKEN_APPEND:
            if (next_token->type != TOKEN_WORD) {
                return -1;
            }

            current_redirect = append_redirect(command);
            if (current_redirect == NULL) {
                return -1;
            }

            current_redirect->type = REDIR_APPEND;
            current_redirect->fd = 1;
            current_redirect->target = next_token->text;

            parser->current += 2;
            break;

        default:
            return 0;
        }
    }

    return 0;
}

static ASTNode *parse_command(Parser *parser) {
    /*
     * Validate the parser itself.
     */
    if (parser == NULL) {
        return NULL;
    }

    /*
     * The parser must have a lexer.
     */
    const Lexer *lexer = parser->lexer;

    if (lexer == NULL) {
        return NULL;
    }

    /*
     * Make sure the current token index is valid.
     */
    if (parser->current >= lexer->count) {
        parser->error = "Parser token index out of bounds";
        return NULL;
    }

    /*
     * A command must not start with an operator.
     *
     * Examples of invalid command starts:
     *
     *     ;
     *     &&
     *     ||
     *     |
     *     END
     */
    TokenType type = lexer->tokens[parser->current].type;

    if (type == TOKEN_SEMICOLON || type == TOKEN_AND || type == TOKEN_PIPE ||
        type == TOKEN_OR || type == TOKEN_END) {

        parser->error = "Invalid first token";
        return NULL;
    }

    /*
     * Allocate the AST node for the command.
     */
    ASTNode *node = ast_create_node(NODE_COMMAND);

    if (node == NULL) {
        parser->error = "Unable to allocate AST node";
        return NULL;
    }

    /*
     * Parse command arguments.
     *
     * Example:
     *
     *     echo hello world
     *
     * becomes:
     *
     *     argv[0] = "echo"
     *     argv[1] = "hello"
     *     argv[2] = "world"
     *     argv[3] = NULL
     */
    while (parser->current < lexer->count &&
           lexer->tokens[parser->current].type == TOKEN_WORD) {

        const Token *token = &lexer->tokens[parser->current];

        /*
         * Allocate space for:
         *
         *     existing arguments
         *     + new argument
         *     + NULL terminator
         */
        size_t new_count = node->command.argc + 2;

        char **new_argv =
            shell_realloc(node->command.argv, new_count * sizeof(*new_argv));

        if (new_argv == NULL) {
            parser->error = "Unable to allocate argument vector";

            ast_destroy(node);

            return NULL;
        }

        /*
         * Update argv only after realloc succeeds.
         */
        node->command.argv = new_argv;

        /*
         * Allocate memory for the argument string.
         *
         * +1 is for the terminating '\0'.
         */
        size_t length = token->length;

        char *argument = shell_malloc(length + 1);

        if (argument == NULL) {
            parser->error = "Unable to allocate argument";

            ast_destroy(node);

            return NULL;
        }

        /*
         * Copy the token text into our own memory.
         */
        memcpy(argument, token->text, length);

        /*
         * Explicitly terminate the string.
         */
        argument[length] = '\0';

        /*
         * Store the argument.
         */
        node->command.argv[node->command.argc] = argument;

        /*
         * Increase argument count.
         */
        node->command.argc++;

        /*
         * Keep argv NULL-terminated.
         */
        node->command.argv[node->command.argc] = NULL;

        /*
         * Move to the next token.
         */
        parser->current++;
    }

    /*
     * Parse redirections belonging to this command.
     */
    if (parse_redirections(parser, &node->command) != 0) {
        ast_destroy(node);
        return NULL;
    }

    return node;
}

static ASTNode *parse_pipeline(Parser *parser) {
    if (parser == NULL) {
        return NULL;
    }

    const Lexer *lexer = parser->lexer;

    if (lexer == NULL) {
        return NULL;
    }

    ASTNode *left_node = parse_command(parser);

    if (left_node == NULL) {
        return NULL;
    }

    ASTNode *pipeline_node = left_node;

    while (parser->current < lexer->count &&
           lexer->tokens[parser->current].type == TOKEN_PIPE) {

        parser->current += 1;
        // now the current will point to the next TOKEN_WORD token

        ASTNode *right_node = parse_command(parser);

        if (right_node == NULL) {
            ast_destroy(left_node);

            return NULL;
        }

        ASTNode *new_pipeline_node = ast_create_node(NODE_PIPELINE);

        if (new_pipeline_node == NULL) {
            ast_destroy(left_node);
            ast_destroy(right_node);

            return NULL;
        }

        new_pipeline_node->type = NODE_PIPELINE;
        new_pipeline_node->left = left_node;
        new_pipeline_node->right = right_node;

        pipeline_node = new_pipeline_node;
        left_node = new_pipeline_node;
    }

    return pipeline_node;
}

static ASTNode *parse_logical(Parser *parser) {
    if (parser == NULL) {
        return NULL;
    }

    ASTNode *left_node = parse_pipeline(parser);

    if (left_node == NULL) {
        return NULL;
    }

    const Lexer *lexer = parser->lexer;

    if (lexer == NULL || lexer->tokens == NULL) {
        ast_destroy(left_node);
        parser->error = "Invalid lexer state";
        return NULL;
    }

    while (parser->current < lexer->count &&
           (lexer->tokens[parser->current].type == TOKEN_AND ||
            lexer->tokens[parser->current].type == TOKEN_OR)) {
        TokenType type = lexer->tokens[parser->current].type;

        parser->current += 1;

        ASTNode *right_node = parse_pipeline(parser);

        if (right_node == NULL) {
            ast_destroy(left_node);
            return NULL;
        }

        ASTNode *new_node =
            ast_create_node(type == TOKEN_AND ? NODE_AND : NODE_OR);

        if (new_node == NULL) {
            ast_destroy(left_node);
            ast_destroy(right_node);
            parser->error = "failed to allocate sequence AST node";
            return NULL;
        }

        new_node->type = (type == TOKEN_AND) ? NODE_AND : NODE_OR;

        new_node->left = left_node;
        new_node->right = right_node;

        left_node = new_node;
    }

    return left_node;
}

static ASTNode *parse_sequence(Parser *parser) {
    if (parser == NULL) {
        return NULL;
    }

    ASTNode *left_node = parse_logical(parser);

    if (left_node == NULL) {
        return NULL;
    }

    const Lexer *lexer = parser->lexer;

    if (lexer == NULL || lexer->tokens == NULL) {
        ast_destroy(left_node);
        parser->error = "Invalid lexer state";
        return NULL;
    }

    while (parser->current < lexer->count &&
           lexer->tokens[parser->current].type == TOKEN_SEMICOLON) {

        parser->current += 1;

        ASTNode *right_node = parse_logical(parser);

        if (right_node == NULL) {
            ast_destroy(left_node);
            return NULL;
        }

        ASTNode *new_node = ast_create_node(NODE_SEQUENCE);

        if (new_node == NULL) {
            ast_destroy(left_node);
            ast_destroy(right_node);
            parser->error = "failed to allocate sequence AST node";
            return NULL;
        }

        new_node->left = left_node;
        new_node->right = right_node;

        left_node = new_node;
    }

    return left_node;
}

ASTNode *parser_parse(Parser *parser) {
    if (parser == NULL) {
        return NULL;
    }

    const Lexer *lexer = parser->lexer;

    if (lexer == NULL || lexer->tokens == NULL) {
        parser->error = "Invalid lexer state";
        return NULL;
    }

    ASTNode *root = parse_sequence(parser);

    if (root == NULL) {
        return NULL;
    }

    if (parser->current < lexer->count &&
        lexer->tokens[parser->current].type != TOKEN_END) {
        parser->error = "Unexpected trailing tokens";
        ast_destroy(root);
        return NULL;
    }

    return root;
}
