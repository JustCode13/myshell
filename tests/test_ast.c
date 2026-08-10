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

static void test_invalid_node_type(void) {
    bool passed = ast_create_node((NodeType)-1) == NULL &&
                  ast_create_node((NodeType)(NODE_SUBSHELL + 1)) == NULL;

    test_result("Reject invalid node types", passed);
}

static void test_validation(void) {
    bool passed = true;

    ASTNode *command = create_command();

    if (command == NULL || !ast_validate(command)) {
        passed = false;
    }

    ast_destroy(command);

    NodeType binary_types[] = {NODE_PIPELINE, NODE_SEQUENCE, NODE_AND, NODE_OR};

    for (size_t i = 0; i < 4 && passed; i++) {
        ASTNode *node = ast_create_node(binary_types[i]);
        ASTNode *left = create_command();
        ASTNode *right = create_command();

        if (node == NULL || left == NULL || right == NULL) {
            passed = false;
        } else {
            node->left = left;
            node->right = right;

            if (!ast_validate(node)) {
                passed = false;
            }
        }

        ast_destroy(node);
        if (node == NULL) {
            ast_destroy(left);
            ast_destroy(right);
        }
    }

    NodeType unary_types[] = {NODE_BACKGROUND, NODE_SUBSHELL};

    for (size_t i = 0; i < 2 && passed; i++) {
        ASTNode *node = ast_create_node(unary_types[i]);
        ASTNode *child = create_command();

        if (node == NULL || child == NULL) {
            passed = false;
        } else {
            node->left = child;

            if (!ast_validate(node)) {
                passed = false;
            }
        }

        ast_destroy(node);
        if (node == NULL) {
            ast_destroy(child);
        }
    }

    test_result("Validate valid AST nodes", passed);
}

static void test_invalid_nodes(void) {
    bool passed = true;

    ASTNode *pipeline = ast_create_node(NODE_PIPELINE);

    if (pipeline == NULL || ast_validate(pipeline)) {
        passed = false;
    }

    ast_destroy(pipeline);

    ASTNode *command = create_command();
    ASTNode *child = create_command();

    if (command == NULL || child == NULL) {
        passed = false;
        ast_destroy(command);
        ast_destroy(child);
    } else {
        command->left = child;

        if (ast_validate(command)) {
            passed = false;
        }

        ast_destroy(command);
    }

    test_result("Reject invalid AST nodes", passed);
}

static void test_destroy(void) {
    ASTNode *root = ast_create_node(NODE_PIPELINE);

    if (root == NULL) {
        test_result("Destroy AST tree", false);
        return;
    }

    root->left = create_command();
    root->right = create_command();

    bool passed = root->left != NULL && root->right != NULL;

    ast_destroy(root);

    test_result("Destroy AST tree", passed);
}

int test_ast(void) {
    test_create_all_nodes();
    test_invalid_node_type();
    test_validation();
    test_invalid_nodes();
    test_destroy();

    printf("\n");
    printf("Tests: %d | Passed: %d | Failed: %d\n", tests_run, tests_passed,
           tests_run - tests_passed);

    return tests_run == tests_passed ? 0 : 1;
}
