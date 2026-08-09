#include "../include/ast.h"
#include "../include/test.h"

#include <stdio.h>
#include <stdlib.h>

int test_parser(void) {

    const char *input = "echo hello world";

    Lexer lexer = {0};
    Parser parser = {0};

    if (lexer_initialize(&lexer, input) != 0) {
        fprintf(stderr, "Lexer Initialize Failed\n");
        return EXIT_FAILURE;
    }
}
