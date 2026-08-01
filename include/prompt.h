#ifndef PROMPT_H
#define PROMPT_H

#include "shell.h"
#include <stddef.h>

#define INITIAL_INPUT_SIZE 64

int prompt_initialize(ShellContext *ctx);
// It initializes the prompt only field with default character '>' to display
// if error returns -1

int prompt_read_line(char **buffer, size_t *length);
// reads a line, character by character with maxium INITIAL_INPUT_SIZE
// characters, and stores the length in length and read line in buffer if error
// returns -1

void prompt_update(ShellContext *ctx);
// gets currnt_working_directory with getcwd and updates prompt and store cwd in
// ctx cwd if error just returns

int test_rc(char *output);
// just a normal function to test read_characters
// not use of this function WORTHLESS

const char *prompt_get(const ShellContext *ctx);
// return the prompt from ctx, but you can't update it

#endif // !PROMPT_H
