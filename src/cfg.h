#ifndef WHILE_AI_CFG_
#define WHILE_AI_CFG_

#include "lang/parser.h"

enum Edge_Type {
    EDGE_ASSIGN,
    EDGE_GUARD,
    EDGE_SKIP,
};

typedef struct CFG_Node CFG_Node;
typedef struct CFG_Edge CFG_Edge;

struct CFG_Edge {
    size_t src;
    size_t dst;
    enum Edge_Type type;
    union {
        AST_Node *assign;
        struct {
            AST_Node *condition;
            bool val;
        } guard;
    } as;
};

struct CFG_Node{
    size_t id;

    /*
    Array of edges that *starts* from this point.
    The size is fixed because one node can have at maximum 2 edges in output (if case).
    */
    CFG_Edge edges[2];
    size_t edge_count;
};

typedef struct {
    size_t count;
    CFG_Node *nodes;
} CFG;

typedef struct CFG_Ctx CFG_Ctx;

/* Init the CFG context using the src */
CFG_Ctx *cfg_init(const char *src);

/* Construct and returns the CFG */
CFG *cfg_get(CFG_Ctx *ctx);

/*
Free the CFG context.
The CFG returned by 'cfg_get' is also freed.
*/
void cfg_free(CFG_Ctx *ctx);

#endif /* WHILE_AI_CFG_ */
