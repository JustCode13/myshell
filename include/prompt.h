#ifndef PROMPT_H
#define PROMPT_H

#include "shell.h"
#include <stddef.h>

int prompt_initialize(ShellContext *ctx);

int prompt_read_line(char **buffer, size_t *length);

void prompt_update(ShellContext *ctx);

const char *prompt_get(const ShellContext *ctx);

#endif // !PROMPT_H
