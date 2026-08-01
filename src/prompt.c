#include "../include/prompt.h"
#include "../include/memory.h"

#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int prompt_initialize(ShellContext *ctx) {
    if (ctx == NULL) {
        return -1;
    }

    ctx->prompt[0] = '>';
    ctx->prompt[1] = '\0';

    return 0;
}

int prompt_read_line(char **buffer, size_t *length) {
    if (buffer == NULL || length == NULL) {
        return -1;
    }

    *buffer = NULL;
    *length = 0;

    size_t current_length = 0;
    size_t capacity = INITIAL_INPUT_SIZE;

    char *new_buffer = shell_malloc(capacity);

    if (new_buffer == NULL) {
        return -1;
    }

    char ch;

    while (1) {
        if (current_length == capacity - 1) {
            if (expand_input_buffer(&new_buffer, &capacity) != 0) {
                shell_free(new_buffer);
                return -1;
            }
        }

        if (read_character(&ch) != 0) {
            shell_free(new_buffer);
            return -1;
        }

        if (ch == '\n') {
            break;
        }

        new_buffer[current_length] = ch;
        current_length++;
    }

    new_buffer[current_length] = '\0';

    *buffer = new_buffer;
    *length = current_length;

    return 0;
}

void prompt_update(ShellContext *ctx) {
    if (ctx == NULL) {
        return;
    }

    char cwd[SHELL_MAX_PATH];

    if (getcwd(cwd, SHELL_MAX_PATH) == NULL) {
        return;
    }

    if (snprintf(ctx->prompt, sizeof(ctx->prompt), "%s+>", cwd) >=
        (int)sizeof(ctx->prompt)) {
        return;
    }

    memcpy(ctx->cwd, cwd, sizeof(cwd));
}

const char *prompt_get(const ShellContext *ctx) {
    if (ctx == NULL) {
        return NULL;
    }

    return (const char *)ctx->prompt;
}
