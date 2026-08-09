#include "../include/lexer.h"
#include "../include/memory.h"
#include "../include/string_builder.h"

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

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

    while (lexer->input[lexer->position] != '\0' &&
           !isspace((unsigned char)lexer->input[lexer->position]) &&
           !is_operator_char(lexer->input[lexer->position])) {
        lexer->position++;
        lexer->column++;
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
    const char *operator_start_add = lexer->input + lexer->position;
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
            type = TOKEN_REDIR_OUT;
            length = 1;
        }
        break;
    }
    case ('|'): {
        if (next_operator == '|') {
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
    if (lexer == NULL) {
        return -1;
    }

    if (type != TOKEN_END && (text == NULL || length == 0)) {
        return -1;
    }

    if (lexer->count >= lexer->capacity - 1) {
        if (lexer->capacity > SIZE_MAX / 2) {
            return -1;
        }

        size_t new_capacity = lexer->capacity * 2;

        if (new_capacity > SIZE_MAX / sizeof(*lexer->tokens)) {
            return -1;
        }

        Token *temp =
            shell_realloc(lexer->tokens, new_capacity * sizeof(*lexer->tokens));

        if (temp == NULL) {
            return -1;
        }

        lexer->tokens = temp;
        lexer->capacity = new_capacity;
    }

    if (length == SIZE_MAX) {
        return -1;
    }

    lexer->tokens[lexer->count].text = shell_malloc(length + 1);
    Token *token = &lexer->tokens[lexer->count];

    if (token->text == NULL) {
        return -1;
    }

    memcpy(token->text, text, length);
    token->text[length] = '\0';

    token->length = length;
    token->line = lexer->line;
    token->column = lexer->column;
    token->type = type;

    lexer->count++;

    return 0;
}

int lexer_initialize(Lexer *lexer, const char *input) {
    if (lexer == NULL || input == NULL) {
        return -1;
    }

    lexer->input = NULL;
    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->tokens = NULL;
    lexer->count = 0;
    lexer->capacity = LEXER_INITIAL_CAPACITY;

    lexer->input = input;

    if (LEXER_INITIAL_CAPACITY > SIZE_MAX / sizeof(Token)) {
        return -1;
    }

    lexer->tokens = shell_malloc(LEXER_INITIAL_CAPACITY * (sizeof(Token)));

    if (lexer->tokens == NULL) {
        return -1;
    }

    return 0;
}

int lexer_tokenize(Lexer *lexer) {
    if (lexer == NULL || lexer->input == NULL || lexer->tokens == NULL) {
        return -1;
    }

    lexer->position = 0;
    lexer->line = 1;
    lexer->column = 1;
    lexer->count = 0;

    while (lexer->input[lexer->position] != '\0') {
        if (isspace((unsigned char)lexer->input[lexer->position])) {
            if (lexer->input[lexer->position] == '\n') {
                lexer->line++;
                lexer->column = 1;
            } else {
                lexer->column++;
            }

            lexer->position++;
            continue;
        } else if (is_operator_char(lexer->input[lexer->position])) {
            if (lex_operator(lexer) != 0) {
                return -1;
            }
        } else {
            if (lex_word(lexer) != 0) {
                return -1;
            }
        }
    }

    if (append_token(lexer, TOKEN_END, NULL, 0) != 0) {
        return -1;
    }

    return 0;
}

const Token *lexer_peek(const Lexer *lexer, size_t index) {
    if (lexer == NULL) {
        return NULL;
    }

    if (index >= lexer->count) {
        return NULL;
    }

    return &lexer->tokens[index];
}

void lexer_destroy(Lexer *lexer) {
    if (lexer == NULL) {
        return;
    }

    for (size_t i = 0; i < lexer->count; i++) {
        free(lexer->tokens[i].text);
    }

    free(lexer->tokens);

    lexer->tokens = NULL;
    lexer->input = NULL;

    lexer->line = 0;
    lexer->column = 0;
    lexer->count = 0;
    lexer->capacity = 0;

    return;
}

const char *token_type_name(TokenType type) {
    switch (type) {
    case TOKEN_WORD:
        return "WORD";
    case TOKEN_PIPE:
        return "PIPE";
    case TOKEN_AND:
        return "AND";
    case TOKEN_OR:
        return "OR";
    case TOKEN_SEMICOLON:
        return "SEMICOLON";
    case TOKEN_BACKGROUND:
        return "BACKGROUND";
    case TOKEN_REDIR_IN:
        return "REDIR_IN";
    case TOKEN_REDIR_OUT:
        return "REDIR_OUT";
    case TOKEN_APPEND:
        return "APPEND";
    case TOKEN_HEREDOC:
        return "HEREDOC";
    case TOKEN_LPAREN:
        return "LPAREN";
    case TOKEN_RPAREN:
        return "RPAREN";
    case TOKEN_END:
        return "END";
    default:
        return "UNKNOWN";
    }
}
