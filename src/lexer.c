#include "../include/lexer.h"
#include "../include/string_builder.h"

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static bool is_operator_char(char c) {
    switch (c) {
    case '&':
        return true;

    case '|':
        return true;

    case '>':
        return true;

    case '<':
        return true;

    case ';':
        return true;

    default:
        return false;
    }
}

static int append_token(Lexer *lexer, TokenType Type, const char *text,
                        size_t length);

static int lex_word(Lexer *lexer) {
    if (lexer == NULL || lexer->input == NULL) {
        return -1;
    }

    size_t start_position = lexer->position;
    const char *start_address = lexer->input + lexer->position;

    while (lexer->input[lexer->position] != '\0' ||
           !isspace((unsigned char)lexer->input[lexer->position]) ||
           !is_operator_char(lexer->input[lexer->position])) {
        lexer->position++;
    }

    size_t length = lexer->position - start_position;

    if (append_token(lexer, TOKEN_WORD, start_address, length) != 0) {
        return -1;
    }

    return 0;
}
