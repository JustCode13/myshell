#include "../include/process.h"
#include "../include/memory.h"
#include "../include/redirect.h"

#include <stddef.h>
#include <stdint.h>
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
