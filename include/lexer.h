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

// initializes the lexer with the default values and allocates memory for tokens
// with LEXER_INITIAL_CAPACITY

int lexer_tokenize(Lexer *lexer);

// creates tokens of inputs for operators and normal words with the help of
// is_operator_char, lex_operator and lex_word static functions

const Token *lexer_peek(const Lexer *lexer, size_t index);

// returns the tokens on the given index

void lexer_destroy(Lexer *lexer);

// frees every token and destroys the entire lexer

const char *token_type_name(TokenType type);

// return the token type in short and in string format

#endif // !LEXER_H
