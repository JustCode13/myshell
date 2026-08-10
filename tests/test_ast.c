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

static ASTNode *create_command(void) { return ast_create_node(NODE_COMMAND); }

static void test_create_node(void) {
    ASTNode *node = ast_create_node(NODE_COMMAND);

    bool passed = node != NULL && node->type == NODE_COMMAND &&
                  node->left == NULL && node->right == NULL &&
                  node->next == NULL && node->command.argv == NULL &&
                  node->command.argc == 0 && node->command.redirects == NULL;

    test_result("Create command node", passed);

    ast_destroy(node);
}

static void test_create_all_nodes(void) {
    bool passed = true;

    for (NodeType type = NODE_COMMAND; type <= NODE_SUBSHELL; type++) {
        ASTNode *node = ast_create_node(type);

        if (node == NULL || node->type != type) {
            passed = false;
            ast_destroy(node);
            break;
        }

        ast_destroy(node);
    }

    test_result("Create all node types", passed);
}
