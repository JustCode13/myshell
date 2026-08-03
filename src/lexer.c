#include "../include/lexer.h"
#include "../include/memory.h"
#include "../include/string_builder.h"

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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

    char operator_start = lexer->input[lexer->position];
    char *operator_start_add = lexer->input + lexer->position;
    char next_operator = lexer->input[lexer->position + 1];

    TokenType type;
    size_t length;

    if (operator_start == '\0' || isspace(operator_start) ||
        !is_operator_char(operator_start)) {
        return -1;
    }

    switch (operator_start) {
    case ('&'): {
        if (next_operator == '&') {
            type = TOKEN_AND;
            length = 2;
        } else {
            type = TOKEN_BACKGROUND;
            length = 1;
        }
        break;
    }
    case ('<'): {
        if (next_operator == '<') {
            type = TOKEN_HEREDOC;
            length = 2;
        } else {
            type = TOKEN_REDIR_IN;
            length = 1;
        }
        break;
    }
    case ('>'): {
        if (next_operator == '>') {
            type = TOKEN_APPEND;
            length = 2;
        } else {
            type = TOKEN_HEREDOC;
            length = 1;
        }
        break;
    }
    case ('|'): {
        if (next_operator == "||") {
            type = TOKEN_OR;
            length = 2;
        } else {
            type = TOKEN_PIPE;
            length = 1;
        }
        break;
    }
    case (';'): {
        type = TOKEN_SEMICOLON;
        length = 1;
        break;
    }

    default:
        return -1;
    }

    if (append_token(lexer, type, (const char *)operator_start_add, length) !=
        0) {
        return -1;
    }

    lexer->position += length;

    return 0;
}

static int append_token(Lexer *lexer, TokenType type, const char *text,
                        size_t length) {
    if (lexer == NULL || text == NULL || length == 0) {
        return -1;
    }

    return 0;
}
