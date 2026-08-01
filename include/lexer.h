#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stddef.h>

#include "shell_types.h"

#define LEXER_INITIAL_CAPACITY 64

typedef struct Lexer {
    const char *input; // the full command the user has typed

    size_t position; // the current char position which will update constantly

    size_t line; // tells which line currently we are on

    size_t column; // tells which column we are on

    Token *tokens; // all tokens array in a command

    size_t count; // number of valid tokens currently stored

    size_t capacity; // maximum tokens currenntly fit inside array
} Lexer;

int lexer_initialize(Lexer *lexer, const char *input);

int lexer_tokenize(Lexer *lexer);

const Token *lexer_peek(const Lexer *lexer, size_t index);

void lexer_destroy(Lexer *lexer);

const char *token_type_name(TokenType type);

#endif // !LEXER_H
