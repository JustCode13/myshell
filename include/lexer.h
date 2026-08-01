#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stddef.h>

#include "shell_types.h"

#define LEXER_INITIAL_CAPACITY 64

typedef struct Lexer {
    const char *input;

    size_t position;

    size_t line;

    size_t column;

    Token *tokens;

    size_t count;

    size_t capacity;
} Lexer;

int lexer_initialize(Lexer *lexer, const char *input);

int lexer_tokenize(Lexer *lexer);

const Token *lexer_peek(const Lexer *lexer, size_t index);

void lexer_destroy(Lexer *lexer);

const char *token_type_name(TokenType type);

#endif // !LEXER_H
