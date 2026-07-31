#include "../include/prompt.h"
#include "../include/memory.h"

#include <limits.h>
#include <stddef.h>
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
