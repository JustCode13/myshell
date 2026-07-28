#include "../include/test.h"

int main(void) {
    if (test_memory() == 0) {
        printf("Worked Successfully!\n");
    }

    return -1;
}