#include "../include/ast.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

static int tests_run = 0;
static int tests_passed = 0;

static void test_result(const char *name, bool passed) {
    tests_run++;

    if (passed) {
        tests_passed++;
        printf("[PASS] %s\n", name);
    } else {
        printf("[FAIL] %s\n", name);
    }
}
