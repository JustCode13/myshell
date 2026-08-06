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
