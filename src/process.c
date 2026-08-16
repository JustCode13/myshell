#include "../include/process.h"
#include "../include/memory.h"
#include "../include/redirect.h"

#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

static char **build_environment(void) {
    size_t count = 0;

    char *const *source = environ;

    while (source[count] != NULL) {
        count++;
    }

    if (count == SIZE_MAX || count + 2 > SIZE_MAX / sizeof(*source)) {
        return NULL;
    }

    char **env = shell_malloc((count + 1) * sizeof(*env));

    if (env == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < count; i++) {
        env[i] = source[i];
    }

    env[count] = NULL;

    return env;
}

int process_resolve_path(const char *file, char *buffer, size_t capacity) {
    if (file == NULL || buffer == NULL || capacity == 0) {
        return -1;
    }

    if (strchr(file, '/') != NULL) {
        if (sizeof(file) > capacity) {
            return -1;
        }

        int written = snprintf(buffer, capacity, "./%s", file);

        if (written >= 0 && (size_t)written < capacity &&
            access(buffer, X_OK) == 0) {
            return 0;
        }
    } else {
        const char *start = getenv("PATH");

        while (true) {
            const char *end = strchr(start, ':');
            size_t directory_length =
                end != NULL ? (size_t)(end - start) : strlen(start);

            size_t file_length = strlen(file);

            if (directory_length > SIZE_MAX - file_length - 2) {
                return -1;
            }

            size_t required_size = directory_length + file_length + 2;

            if (required_size <= capacity) {

                if (directory_length == 0) {

                    int written = snprintf(buffer, capacity, "./%s", file);

                    if (written >= 0 && (size_t)written < capacity &&
                        access(buffer, X_OK) == 0) {
                        return 0;
                    }
                } else {
                    int written = snprintf(buffer, capacity, "%.*s/%s",
                                           (int)directory_length, start, file);

                    if (written >= 0 && (size_t)written < capacity &&
                        access(buffer, X_OK) == 0) {
                        return 0;
                    }
                }
            }

            if (end == NULL) {
                break;
            }

            start = end + 1;
        }
    }

    return -1;
}
