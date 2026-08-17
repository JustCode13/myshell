#include "../include/lexer.h"

#include <stddef.h>
#include <stdio.h>

static const char *expected_token_name(TokenType type) {
    return token_type_name(type);
}

int test_lexer(void) {
    const char *input = "echo \"hello, 5 & 7 world\"";

    Lexer lexer;

    if (lexer_initialize(&lexer, input) != 0) {
        printf("FAIL: lexer_initialize()\n");
        return -1;
    }

    if (lexer_tokenize(&lexer) != 0) {
        printf("FAIL: lexer_tokenize()\n");
        lexer_destroy(&lexer);
        return -1;
    }

    printf("Input: %s\n\n", input);

    printf("Tokens: %zu\n", lexer.count);

    for (size_t i = 0; i < lexer.count; i++) {
        const Token *token = lexer_peek(&lexer, i);

        if (token == NULL) {
            printf("FAIL: lexer_peek(%zu) returned NULL\n", i);
            lexer_destroy(&lexer);
            return -1;
        }

        printf("[%zu] %-10s text=\"%s\" length=%zu line=%zu column=%zu\n", i,
               expected_token_name(token->type),
               token->text != NULL ? token->text : "", token->length,
               token->line, token->column);
    }

    const Token *last = lexer_peek(&lexer, lexer.count - 1);

    if (last == NULL || last->type != TOKEN_END) {
        printf("\nFAIL: TOKEN_END is missing\n");
        lexer_destroy(&lexer);
        return -1;
    }

    printf("\nPASS: lexer produced valid tokens and TOKEN_END\n");

    lexer_destroy(&lexer);

    return 0;
}
