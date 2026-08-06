#ifndef AST_H
#define AST_H

#include "memory.h"
#include "shell_types.h"

typedef struct ASTNode {
    NodeType type;

    Command command;

    struct ASTNode *left;

    struct ASTNode *right;

    struct ASTNode *next;
} ASTNode;

ASTNode *ast_create_node(NodeType type);

void ast_destroy(ASTNode *root);

void ast_print(const ASTNode *root, int depth);

bool ast_validate(const ASTNode *root);

int use_indent(int depth);

#endif // !AST_H
