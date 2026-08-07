#include "../include/parser.h"
#include "../include/ast.h"

#include <stdlib.h>

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

static int parse_redirections(Parser *parser, Command *command) {
    if (parser == NULL || command == NULL) {
        return -1;
    }

    const Lexer *lexer = parser->lexer;

    while (lexer->tokens[parser->current].type != TOKEN_END) {

        Token *current_token = &lexer->tokens[parser->current];
        Token *next_token = &lexer->tokens[parser->current + 1];

        if (command->redirects != NULL) {
            command->redirects->next = shell_malloc(sizeof(Redirect));

            if (command->redirects->next) {
                return -1;
            }
        } else {
            command->redirects = shell_malloc(sizeof(Redirect));

            if (command->redirects == NULL) {
                return -1;
            }
        }

        switch (current_token) {
        case (TOKEN_REDIR_IN): {
            if (next_token->type == TOKEN_WORD) {
                
            }
        }
        }
    }
}
