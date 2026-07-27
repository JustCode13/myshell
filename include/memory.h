#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

void *shell_malloc(size_t size);

void *shell_realloc(void *ptr, size_t size);

void shell_free(void *ptr);

size_t memory_allocated_bytes(void);

#endif // !MEMORY_H