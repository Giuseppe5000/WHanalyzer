#include "cfg.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct CFG_Ctx {
    CFG *cfg;
    Parser *parser;
};

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

static size_t cfg_print_points(AST_Node *node) {
    size_t count = cfg_print_points_impl(node, 1);
    printf("(P%zu)\n", count);
    return count;
}

static size_t count_nodes(AST_Node *node, size_t counter) {
    switch (node->type) {
    case NODE_ASSIGN:
    case NODE_SKIP:
        counter++;
        break;
    case NODE_SEQ:
        counter = count_nodes(node->as.child.left, counter);
        counter = count_nodes(node->as.child.right, counter);
        break;
    case NODE_IF:
        counter += 2;
        counter = count_nodes(node->as.child.left, counter);
        counter = count_nodes(node->as.child.right, counter);
        break;
    case NODE_WHILE:
        counter += 2;
        counter = count_nodes(node->as.child.left, counter);
        break;
    default:
        break;
    }
    return counter;
}

static CFG *build_cfg(AST_Node *node, size_t counter) {
    /* TODO */
}

CFG_Ctx *cfg_init(const char *src) {
    CFG_Ctx *ctx = xmalloc(sizeof(CFG_Ctx));

    /* Parser init */
    ctx->parser = parser_init(src);

    /* CFG init */
    ctx->cfg = NULL;

    return ctx;
}

CFG *cfg_get(CFG_Ctx *ctx) {
    if (ctx->cfg == NULL) {
        /* Alloc and build */
        AST_Node *root = parser_parse(ctx->parser);
        ctx->cfg = xmalloc(sizeof(CFG));
        ctx->cfg->count = count_nodes(root, 1);
        ctx->cfg->nodes = xmalloc(sizeof(CFG_Node)*(ctx->cfg->count));
        ctx->cfg = build_cfg(root, 1);
    }

    return ctx->cfg;
}

void cfg_free(CFG_Ctx *ctx) {
    parser_free(ctx->parser);
    if (ctx->cfg != NULL) {
        free(ctx->cfg->nodes);
        free(ctx->cfg);
    }
    free(ctx);
}
