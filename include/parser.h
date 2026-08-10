#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "shell_types.h"
#include <stddef.h>

typedef struct Parser {
    const Lexer *lexer;

    size_t current;

    bool panic_mode;

    char *error;
} Parser;

ASTNode *parser_parse(Parser *parser);

//

int parser_initialize(Parser *parser, const Lexer *lexer);

// initializes the parse with default values and lexer

void parser_destroy(Parser *parser);

//

const char *parser_last_error(const Parser *parser);

// returns the last parser error

#endif // !PARSER_H
