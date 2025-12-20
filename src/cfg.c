#include "cfg.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct CFG_Ctx {
    CFG *cfg;
    Parser *parser;
};

static CFG_Node build_node(size_t id) {
    CFG_Node node = {
        .id = id,
        .edge_count = 0,
    };

    return node;
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
        counter += 1;
        counter = count_nodes(node->as.child.left, counter);
        break;
    default:
        break;
    }
    return counter;
}


static void build_cfg_impl(CFG *cfg, AST_Node *node, size_t *counter, int *pred) {
    switch (node->type) {
    case NODE_SKIP:
        /* TODO: like the assign */
        break;
    case NODE_ASSIGN:

        /* Create the assign node */
        cfg->nodes[*counter] = build_node(*counter);
        cfg->nodes[*counter].edges[0].src = *counter;
        cfg->nodes[*counter].edges[0].dst = -1;
        cfg->nodes[*counter].edges[0].type = EDGE_ASSIGN;
        cfg->nodes[*counter].edges[0].as.assign = node;

        /* If there is a predecessor node we need to wire an edge to this new node */
        if (*pred >= 0) {
            size_t prev_node_edge_count = cfg->nodes[*pred].edge_count;
            if (prev_node_edge_count == 2) {
                assert(0 && "UNREACHABLE");
            }

            cfg->nodes[*pred].edges[prev_node_edge_count].dst = *counter;
            cfg->nodes[*pred].edge_count++;
        }

        *pred = *counter;
        *counter += 1;
        break;
    case NODE_SEQ:
        build_cfg_impl(cfg, node->as.child.left, counter, pred);
        build_cfg_impl(cfg, node->as.child.right, counter, pred);
        break;
    case NODE_IF:
        // printf("if (P%zu) b then\n", counter++);
        // counter = build_cfg_impl(cfg, node->as.child.left, counter);
        // printf("(P%zu) else\n", counter++);
        // counter = build_cfg_impl(cfg, node->as.child.right, counter);
        // printf("fi\n");
        break;
    case NODE_WHILE:
        /* Loop invariant Node */
        cfg->nodes[*counter] = build_node(*counter);

        cfg->nodes[*counter].edges[0].src = *counter;
        cfg->nodes[*counter].edges[0].dst = -1;
        cfg->nodes[*counter].edges[0].type = EDGE_GUARD;
        cfg->nodes[*counter].edges[0].as.guard.condition = node;
        cfg->nodes[*counter].edges[0].as.guard.val = true;

        cfg->nodes[*counter].edges[1].src = *counter;
        cfg->nodes[*counter].edges[1].dst = -1;
        cfg->nodes[*counter].edges[1].type = EDGE_GUARD;
        cfg->nodes[*counter].edges[1].as.guard.condition = node;
        cfg->nodes[*counter].edges[1].as.guard.val = false;

        /* If there is a predecessor node we need to wire an edge to this new node */
        if (*pred >= 0) {
            size_t prev_node_edge_count = cfg->nodes[*pred].edge_count;
            if (prev_node_edge_count == 2) {
                assert(0 && "UNREACHABLE");
            }

            cfg->nodes[*pred].edges[prev_node_edge_count].dst = *counter;
            cfg->nodes[*pred].edge_count++;
        }

        const size_t loop_inv = *counter;
        *pred = *counter;
        *counter += 1;

        build_cfg_impl(cfg, node->as.child.left, counter, pred);

        /* Wire the last added node (the predecessor) to the loop invariant node */
        cfg->nodes[*pred].edges[0].dst = loop_inv;
        cfg->nodes[*pred].edge_count++;

        /* The predecessor of a statement after the while is the loop invariant */
        *pred = loop_inv;
        break;
    default:
        break;
    }
}

static void build_cfg(CFG *cfg, AST_Node *root) {
    size_t counter = 0;
    int pred = -1;
    build_cfg_impl(cfg, root, &counter, &pred);

    /* Wire to the last node */
    if (pred >= 0) {
        size_t prev_node_edge_count = cfg->nodes[pred].edge_count;
        if (prev_node_edge_count == 2) {
            assert(0 && "UNREACHABLE");
        }

        cfg->nodes[pred].edges[prev_node_edge_count].dst = counter;
        cfg->nodes[pred].edge_count++;
    }

}

void cfg_print_graphviz(CFG *cfg) {
    printf("nodes = %zu\n", cfg->count);
    printf("digraph G {\n");
    printf("\tnode [shape=circle]\n\n");
    for (size_t i = 0; i < cfg->count; ++i) {
        CFG_Node node = cfg->nodes[i];
        for (size_t j = 0; j < node.edge_count; ++j) {
            printf("\t%zu -> %zu", node.edges[j].src, node.edges[j].dst);
            switch (node.edges[j].type) {
            case EDGE_ASSIGN:
                const char *var = node.edges[j].as.assign->as.child.left->as.var.str;
                size_t var_len = node.edges[j].as.assign->as.child.left->as.var.len;
                printf(" [label=\"ASSIGN %.*s\"]\n", (int)var_len, var);
                break;
            case EDGE_GUARD:
                printf(" [label=\"GUARD\"]\n");
                break;
            default:
                printf("\n");
                break;
            }
        }
    }
    printf("}\n");
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
        build_cfg(ctx->cfg, root);
        cfg_print_graphviz(ctx->cfg);
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
