#include "../include/ast.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static void destroy_redirects(Redirect *head) {
    if (head == NULL) {
        return;
    }

    Redirect *current = head;

    while (current != NULL) {
        Redirect *next = current->next;

        free(current->target);
        free(current);

        current = next;
    }
}

static void print_indent(int depth) {
    int spaces = depth * 4;

    for (int counter = 0; counter < spaces; counter++) {
        printf(" ");
    }
}

ASTNode *ast_create_node(NodeType type) {
    if (type < NODE_COMMAND || type > NODE_SUBSHELL) {
        return NULL;
    }

    ASTNode *node = shell_malloc(sizeof(ASTNode));

    if (node == NULL) {
        return NULL;
    }

    node->type = type;

    node->command = (Command){0};

    node->next = NULL;
    node->left = NULL;
    node->right = NULL;

    return node;
}

void ast_destroy(ASTNode *root) {
    if (root == NULL) {
        return;
    }

    ast_destroy(root->left);

    ast_destroy(root->right);

    Command *command = &root->command;

    if (command->argv != NULL) {
        for (size_t i = 0; i < command->argc; i++) {
            free(command->argv[i]);
        }

        free(command->argv);
    }

    if (command->redirects != NULL) {
        destroy_redirects(command->redirects);
    }

    ast_destroy(root->next);
    free(root);

    return;
}

void ast_print(const ASTNode *root, int depth) {
    if (root == NULL) {
        return;
    }

    print_indent(depth);

    switch (root->type) {
    case NODE_COMMAND:
        printf("NODE_COMMAND\n");
        break;

    case NODE_PIPELINE:
        printf("NODE_PIPELINE\n");
        break;

    case NODE_SEQUENCE:
        printf("NODE_SEQUENCE\n");
        break;

    case NODE_AND:
        printf("NODE_AND\n");
        break;

    case NODE_OR:
        printf("NODE_OR\n");
        break;

    case NODE_BACKGROUND:
        printf("NODE_BACKGROUND\n");
        break;

    case NODE_SUBSHELL:
        printf("NODE_SUBSHELL\n");
        break;

    default:
        printf("UNKNOWN_NODE\n");
        break;
    }

    if (root->left) {
        ast_print(root->left, depth + 1);
    }

    if (root->right) {
        ast_print(root->right, depth + 1);
    }
}
