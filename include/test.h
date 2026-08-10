#ifndef TEST_H
#define TEST_H
#include <stddef.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"

int test_memory(void);
int test_prompt(void);
int test_lexer(void);
int test_parser(void);
int test_ast(void);

#endif // !TEST_H
