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
    TOKEN_BACKGROUND,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END,
} TokenType;

typedef enum NodeType {
    NODE_COMMAND,
    NODE_PIPELINE,
    NODE_SEQUENCE,
    NODE_AND,
    NODE_OR,
    NODE_BACKGROUND,
    NODE_SUBSHELL
} NodeType;

typedef enum RedirectType {
    REDIR_INPUT,
    REDIR_OUTPUT,
    REDIR_APPEND,
    REDIR_HEREDOC,
} RedirectType;

typedef enum JobState {
    JOB_RUNNING,
    JOB_STOPPED,
    JOB_DONE,
} JobState;

#endif // !SHELL_TYPES_H