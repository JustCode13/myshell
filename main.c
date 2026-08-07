#include "include/ast.h"
#include "include/prompt.h"
#include "include/test.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    if (use_indent(5) != 0) {
        return -1;
    }

    printf("Hello\n");

    return 0;
}
