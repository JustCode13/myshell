#include "../include/prompt.h"
#include "../include/memory.h"

#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int read_character(char *output) {
    if (output == NULL) {
        return -1;
    }

    int read_bytes = read(STDIN_FILENO, output, 1);

    if (read_bytes != 1) {
        return -1;
    }

    return 0;
}

int test_rc(char *output) {
    int result = read_character(output);
    return result;
}

static int expand_input_buffer(char **buffer, size_t *capacity) {

    if (buffer == NULL || *buffer == NULL || capacity == NULL) {
        return -1;
    }

    if (*capacity == 0) {
        return -1;
    }

    if (*capacity > SIZE_MAX / 2) {
        return -1;
    }

    size_t new_capacity = (*capacity) * 2;

    char *new_buffer = shell_realloc(*buffer, new_capacity);

    if (new_buffer == NULL) {
        return -1;
    }

    *buffer = new_buffer;
    *capacity = new_capacity;

    return 0;
}
