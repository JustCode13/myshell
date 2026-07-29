#include "../include/string_builder.h"
#include "../include/memory.h"

#include <stddef.h>
#include <stdlib.h>

static int ensure_capacity(StringBuilder *builder, size_t required) {
    if (builder == NULL || required == 0) {
        return -1;
    }

    size_t remaining_size = builder->capacity - builder->length;

    if (required > remaining_size) {
        size_t new_capacity = builder->length + required;
        char *buffer = shell_realloc(builder->buffer, new_capacity);

        if (buffer == NULL) {
            return -1;
        }

        builder->buffer = buffer;
        builder->capacity = new_capacity;
    }

    return 0;
}

int sb_initialize(StringBuilder *builder, size_t capacity) {
    if (builder == NULL || capacity == 0) {
        return -1;
    }

    builder->buffer = shell_malloc(capacity);

    if (builder->buffer == NULL) {
        return -1;
    }

    builder->capacity = capacity;
    builder->length = 0;
    builder->buffer[0] = '\0';

    return 0;
}

int sb_append_char(StringBuilder *builder, char value) {
    if (builder == NULL || value == NULL) {
        return -1;
    }
}
