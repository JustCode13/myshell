#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include <cstddef>
#include <stddef.h>
#include <stdbool.h>

#define STRING_BUILDER_INITIAL_SIZE 128

typedef struct StringBuilder {
    char *buffer;
    
    size_t length;

    size_t capacity;
} StringBuilder ;

int sb_initialize(StringBuilder *builder, size_t capacity);

int sb_append_char(StringBuilder *builder, char value);

int sb_append_string(StringBuilder *builder, const char *text);

char sb_duplicate(StringBuilder *builder);

void sb_clear(StringBuilder *builder);

void sb_destroy(StringBuilder *builder);

#endif // !STRING_BUILDER_H
