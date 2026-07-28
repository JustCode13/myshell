#ifndef MEMORY_H
#define MEMORY_H

#include "shell_types.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void *shell_malloc(size_t size);

void *shell_realloc(void *ptr, size_t size);

void shell_free(void *ptr);

size_t memory_allocated_bytes(void);

size_t memory_allocated_count(void);

size_t memory_freed_count(void);

#endif // !MEMORY_H
