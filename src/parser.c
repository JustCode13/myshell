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

    if (lexer->tokens[0].type == TOKEN_SEMICOLON ||
        lexer->tokens[0].type == TOKEN_AND ||
        lexer->tokens[0].type == TOKEN_PIPE ||
        lexer->tokens[0].type == TOKEN_OR ||
        lexer->tokens[0].type == TOKEN_END) {

        parser->error = "Invalid first token";

        return NULL;
    }
}
