#include "cfg.h"
#include <stdbool.h>
#include <stdio.h>

static size_t cfg_print_points_impl(AST_Node *node, size_t counter) {
    switch (node->type) {
    case NODE_ASSIGN:
    case NODE_SKIP:
        printf("(P%zu)\n", counter++);
        parser_print_ast(node);
        break;
    case NODE_SEQ:
        counter = cfg_print_points_impl(node->as.child.left, counter);
        counter = cfg_print_points_impl(node->as.child.right, counter);
        break;
    case NODE_IF:
        printf("if (P%zu) b then\n", counter++);
        counter = cfg_print_points_impl(node->as.child.left, counter);
        printf("(P%zu) else\n", counter++);
        counter = cfg_print_points_impl(node->as.child.right, counter);
        printf("fi\n");
        break;
    case NODE_WHILE:
        printf("while (P%zu) b do\n", counter++);
        counter = cfg_print_points_impl(node->as.child.left, counter);
        printf("(P%zu) done\n", counter++);
        break;
    default:
        break;
    }
    return counter;
}

static void cfg_print_points(AST_Node *node) {
    size_t count = cfg_print_points_impl(node, 1);
    printf("(P%zu)\n", count);
}

CFG *cfg_init(AST_Node *root) {
    cfg_print_points(root);
    return NULL;
}
