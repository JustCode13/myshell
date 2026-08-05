#include "../include/lexer.h"
#include "../include/memory.h"
#include "../include/prompt.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int test_lexer(void) {
    Lexer *lexer = shell_malloc(sizeof(Lexer));

    ShellContext *ctx = shell_malloc(sizeof(ShellContext));

    if (lexer == NULL || ctx == NULL) {
        free(lexer);
        free(ctx);
        return -1;
    }

    if (prompt_initialize(ctx) != 0) {
        free(lexer);
        free(ctx);
        return -1;
    }

    prompt_update(ctx);

    char *buffer = NULL;
    size_t length = 0;

    while (1) {

        printf("%s", ctx->prompt);
        fflush(stdout);

        if (prompt_read_line(&buffer, &length) != 0) {
            shell_free(buffer);
            shell_free(lexer);
            shell_free(ctx);
            return -1;
        }

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        if (lexer_initialize(lexer, (const char *)buffer) != 0) {
            shell_free(buffer);
            shell_free(lexer);
            shell_free(ctx);
            printf("lexer_initialize\n");
            return -1;
        }

        if (lexer_tokenize(lexer) == -1) {
            shell_free(buffer);
            shell_free(lexer);
            shell_free(ctx);
            printf("lexer_tokenize\n");
            return -1;
        }

        for (size_t i = 0; i < lexer->count; i++) {
            const Token *token = lexer_peek(lexer, i);

            if (token == NULL) {
                shell_free(buffer);
                shell_free(lexer);
                shell_free(ctx);
                return -1;
            }

            const char *token_name = token_type_name(token->type);

            printf("Token Type: %d, %s\n", token->type, token_name);
            printf("Text: %.*s\n", (int)token->length, token->text);
            printf("Length: %zu\n", token->length);
            printf("Line: %zu\n", token->line);
            printf("Column: %zu\n", token->column);

            printf(
                "--------------------------------------------------------\n");
        }
    }

    shell_free(buffer);
    shell_free(lexer);
    shell_free(ctx);

    return 0;
}
