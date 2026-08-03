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

static int lex_operator(Lexer *lexer) {
    if (lexer == NULL) {
        return -1;
    }

    char current_char = lexer->input[lexer->position];
    char *current_char_add = lexer->input + lexer->position;
    char next_char = lexer->input[lexer->position + 1];

    TokenType token;
    size_t length;

    if (current_char == '\0' || isspace(current_char) ||
        !is_operator_char(current_char)) {
        return -1;
    }

    switch (current_char) {
    case ('&'): {
        if (next_char == '&') {
            token = TOKEN_AND;
            length = 2;
        }

        token = TOKEN_BACKGROUND;
        length = 1;
    }
    case ('<'): {
        if (next_char == '<') {
            token = TOKEN_HEREDOC;
            length = 2;
        }
        token = TOKEN_REDIR_IN;
        length = 1;
    }
    case ('>'): {
        if (next_char == '>') {
            token = TOKEN_APPEND;
            length = 2;
        }
        token = REDIR_HEREDOC;
        length = 1;
    }
    case ('|'): {
        if (next_char == '||') {
            token = TOKEN_OR;
            length = 2;
        }
        token = TOKEN_PIPE;
        length = 1;
    }
    case (';'): {
        token = TOKEN_SEMICOLON;
        length = 1;
    }

    default:
        return -1;
    }

    return 0;
}
