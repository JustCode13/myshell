#include "../include/ast.h"
#include "../include/parser.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    const char *input;
    const char *description;
} ParserTest;

static int run_test(const ParserTest *test, size_t number) {
    Lexer lexer = {0};
    Parser parser = {0};
    ASTNode *root = NULL;

    printf("\n");
    printf("TEST %zu: %s\n", number, test->description);
    printf("Input: %s\n", test->input);

    if (lexer_initialize(&lexer, test->input) != 0) {
        printf("RESULT: FAIL - lexer_initialize()\n");
        return EXIT_FAILURE;
    }

    if (lexer_tokenize(&lexer) < 0) {
        printf("RESULT: FAIL - lexer_tokenize()\n");
        lexer_destroy(&lexer);
        return EXIT_FAILURE;
    }

    if (parser_initialize(&parser, &lexer) != 0) {
        printf("RESULT: FAIL - parser_initialize()\n");
        lexer_destroy(&lexer);
        return EXIT_FAILURE;
    }

    root = parser_parse(&parser);

    if (root == NULL) {
        printf("RESULT: FAIL - parser_parse()\n");

        if (parser_last_error(&parser) != NULL) {
            printf("ERROR: %s\n", parser_last_error(&parser));
        }

        parser_destroy(&parser);
        lexer_destroy(&lexer);

        return EXIT_FAILURE;
    }

    if (!ast_validate(root)) {
        printf("RESULT: FAIL - invalid AST\n");

        ast_destroy(root);
        parser_destroy(&parser);
        lexer_destroy(&lexer);

        return EXIT_FAILURE;
    }

    printf("RESULT: PASS\n");
    printf("AST GRAPH:\n");

    ast_print(root, 0);

    ast_destroy(root);
    parser_destroy(&parser);
    lexer_destroy(&lexer);

    return EXIT_SUCCESS;
}

int test_parser(void) {
    const ParserTest tests[] = {

        {"pwd", "Simple command"},

        {"echo hello world", "Command with arguments"},

        {"cat < input.txt", "Input redirection"},

        {"echo hello > output.txt", "Output redirection"},

        {"echo hello >> output.txt", "Append redirection"},

        {"cat << EOF", "Heredoc"},

        {"ls | grep .c", "Pipeline"},

        {"pwd && ls", "Logical AND"},

        {"pwd || ls", "Logical OR"},

        {"pwd ; ls", "Command sequence"},

        {"cat file.txt | grep hello > result.txt", "Pipeline with redirection"},

        {"cat file.txt | grep hello && echo found",
         "Pipeline with logical AND"},

        {"echo hello > out.txt ; cat < out.txt", "Sequence with redirections"},

        {"cat input.txt | grep hello > output.txt && echo done || echo failed "
         "; pwd",
         "Pipeline with redirection, AND, OR and sequence"}};

    size_t test_count = sizeof(tests) / sizeof(tests[0]);
    size_t passed = 0;

    for (size_t i = 0; i < test_count; i++) {
        if (run_test(&tests[i], i + 1) == EXIT_SUCCESS) {
            passed++;
        }
    }

    printf("\n");
    printf("Tests: %zu | Passed: %zu | Failed: %zu\n", test_count, passed,
           test_count - passed);

    return passed == test_count ? EXIT_SUCCESS : EXIT_FAILURE;
}
