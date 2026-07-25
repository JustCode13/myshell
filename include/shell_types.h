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

typedef struct Token {
    TokenType token;

    char *text;

    size_t length;

    size_t line;

    size_t column;
} Token;


typedef struct Redirect {
    RedirectType type;

    int fd;

    char *target;

    Redirect *next; 
} Redirect;

typedef struct Command {
    char **argv;

    size_t argc;

    Redirect *redirects;

    bool background;
} Command;

typedef struct ASTNode ASTNode;

typedef struct Job {
    pid_t pgid;

    int job_id;

    JobState state;

    char *command_line;

    struct Job *next;
} Job;

typedef int (*BuiltInFunction)(int argc, char **argv);

#endif // !SHELL_TYPES_H