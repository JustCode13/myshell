#ifndef SHELL_H
#define SHELL_H

#include "shell_types.h"
#include <stdbool.h>

typedef struct ShellContext {
    bool interactive;

    int last_status;

    Job *jobs;

    char cwd[SHELL_MAX_PATH];

    char prompt[SHELL_PROMPT_MAX];
} ShellContext;

int shell_initialize(ShellContext *ctx);

int shell_run(ShellContext *ctx);

void shell_shutdown(ShellContext *ctx);

void shell_set_exit_status(ShellContext *ctx, int status);

#endif // !SHELL_H
