#include "../include/parser.h"
#include "../include/ast.h"

#include <stdlib.h>
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

    free(parser->error);

    parser->error = NULL;
    parser->lexer = NULL;

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
            current_redirect->target = next_token->text;

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
    if (parser == NULL) {
        return NULL;
    }

    Lexer *lexer = parser->lexer;

    if (lexer == NULL) {
        return NULL;
    }

    TokenType type = lexer->tokens[parser->current].type;

    if (type == TOKEN_SEMICOLON || type == TOKEN_AND || type == TOKEN_PIPE ||
        type == TOKEN_OR || type == TOKEN_END) {

        parser->error = "Invalid first token";
        return NULL;
    }

    if (parser->current >= lexer->count) {
        parser->error = "Parser token index out of bounds";
        return NULL;
    }

    ASTNode *node = shell_malloc(sizeof(ASTNode));

    if (node == NULL) {
        parser->error = "Unable to allocate AST node";

        return NULL;
    }

    node->type = NODE_COMMAND;
    node->left = NULL;
    node->right = NULL;
    node->next = NULL;

    node->command.argv = NULL;
    node->command.argc = 0;
    node->command.redirects = NULL;
    node->command.background = false;

    while (lexer->tokens[parser->current].type == TOKEN_WORD) {
        char **new_argv = shell_realloc(
            node->command.argv, (node->command.argc + 2) * sizeof(*new_argv));

        if (new_argv == NULL) {
            parser->error = "Unable to allocate argument vector";

            ast_destroy(node);

            return NULL;
        }

        node->command.argv = new_argv;
        node->command.argv[node->command.argc] =
            lexer->tokens[parser->current].text;
        node->command.argc += 1;
        node->command.argv[node->command.argc] = NULL;

        parser->current += 1;
    }

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

    Lexer *lexer = parser->lexer;

    if (lexer == NULL) {
        return NULL;
    }

    ASTNode *left_node = parse_command(parser);

    if (left_node != NULL) {
        return NULL;
    }

    while (lexer->tokens[parser->current].type == TOKEN_PIPE) {
        Token *current_token = &lexer->tokens[parser->current];

        if (current_token == NULL) {
            ast_destroy(left_node);
            return NULL;
        }

        parser->current += 1;
        // now the current will point to the next TOKEN_WORD token

        ASTNode *right_node = parse_command(parser);

        if (right_node == NULL) {
            ast_destroy(left_node);

            return NULL;
        }

        ASTNode *pipeline_node = shell_malloc(sizeof(ASTNode));

        if (pipeline_node == NULL) {
            ast_destroy(left_node);
            ast_destroy(right_node);

            return NULL;
        }

        pipeline_node->command.argv = NULL;
        pipeline_node->command.argc = 0;
        pipeline_node->command.redirects = NULL;
        pipeline_node->command.background = false;

        pipeline_node->type = NODE_PIPELINE;
        pipeline_node->left = left_node;
        pipeline_node->right = right_node;

        left_node->type = NODE_PIPELINE;
    }
}
