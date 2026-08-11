#ifndef PROCESS_H
#define PROCESS_H

#include "shell.h"
#include "shell_types.h"

#include <stddef.h>
#include <sys/types.h>

int process_spawn(Command *command, pid_t pgid, bool foreground);

int process_exec(Command *command);

int process_wait_group(pid_t pgid, int *status);

int process_wait_signal(pid_t pgid, int *status);

int process_resolve_path(const char *file, char *buffer, size_t capacity);

#endif // !PROCESS_H
