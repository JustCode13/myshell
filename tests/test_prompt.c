#include "../include/memory.h"
#include "../include/prompt.h"
#include "../include/shell.h"
#include <stddef.h>
#include <stdio.h>

int test_prompt(void) {

    ShellContext *ctx = shell_malloc(sizeof(ShellContext));

    ctx->interactive = true;
    ctx->last_status = 0;
    ctx->jobs = NULL;

    if (prompt_initialize(ctx) != 0) {
        free(ctx);
        return -1;
    }

    printf("default prompt is %s\n", ctx->prompt);

    char *buffer = shell_malloc(INITIAL_INPUT_SIZE);
    size_t length = 0;

    if (prompt_read_line(&buffer, &length) != 0) {
        free(ctx);
        free(buffer);
        return -1;
    }

    printf("you typed: %s\n", buffer);

    prompt_update(ctx);

    printf("updated prompt is %s\n", ctx->prompt);

    const char *prompt = prompt_get(ctx);

    printf("prompt from get_prompt: %s\n", prompt);

    return 0;
}
