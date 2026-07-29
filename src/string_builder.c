#include "../include/string_builder.h"
#include "../include/memory.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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
    if (builder == NULL) {
        return -1;
    }

    if (ensure_capacity(builder, 2) != 0) {
        return -1;
    }

    builder->buffer[builder->length++] = value;
    builder->buffer[builder->length] = '\0';

    return 0;
}

int sb_append_string(StringBuilder *builder, const char *text) {
    if (builder == NULL || text == NULL) {
        return -1;
    }

    size_t text_length = strlen(text);

    if (ensure_capacity(builder, text_length) != 0) {
        return -1;
    }

    memcpy(builder->buffer + builder->length, text, text_length);

    builder->length += text_length;
    builder->buffer[builder->length] = '\0';

    return 0;
}

char *sb_duplicate(StringBuilder *builder) {
    if (builder == NULL) {
        return NULL;
    }

    char *des_buffer = shell_malloc(builder->length + 1);

    if (des_buffer == NULL) {
        return NULL;
    }

    if (builder->buffer == NULL) {
        return NULL;
    }

    strcpy(des_buffer, builder->buffer);

    return des_buffer;
}

void sb_clear(StringBuilder *builder) {
    if (builder == NULL) {
        return;
    }

    builder->length = 0;
    builder->buffer[0] = '\0';
}
