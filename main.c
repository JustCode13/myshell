#include "include/prompt.h"
#include "include/test.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {

    if (test_prompt() != 0) {
        return -1;
    }

    return 0;
}
