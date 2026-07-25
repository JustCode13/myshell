#ifndef SHELL_TYPES_H
#define SHELL_TYPES_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#define SHELL_MAX_ARG 256
#define SHELL_MAX_PATH 4096
#define SHELL_MAX_PIPES 64
#define SHELL_PROMPT_MAX 128

typedef enum TokenType {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_SEMICOLON,
    TOKEN_BACKWARD,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END,
} TokenType;



#endif // !SHELL_TYPES_H