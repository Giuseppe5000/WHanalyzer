#include "parser.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

struct Parser {
    Lexer *lex;
    AST_Node *root;
};

Parser *parser_init(const char *src) {
    Parser *parser = xmalloc(sizeof(Parser));
    parser->lex = lex_init(src);
    parser->root = NULL;

    return parser;
}

void parser_free(Parser *parser) {
    lex_free(parser->lex);
    /* TODO: free AST */
    free(parser);
}


/* ============================= Recursive descent parser ============================= */
/* https://en.wikipedia.org/wiki/Recursive_descent_parser */

static AST_Node *parse_stmt(Parser *parser);

/* Alloc a zero initialized AST node */
static AST_Node *create_node(enum Node_Type type) {
    AST_Node *node = xmalloc(sizeof(AST_Node));
    memset(node, 0, sizeof(AST_Node));
    node->type = type;
    return node;
}

static void expect(Token t, enum Token_Type type) {
    if (t.type != type) {
        fprintf(stderr, "[ERROR]: Expected type %d but found %d while parsing.", type, t.type);
        exit(1);
    }
}

static AST_Node *parse_aexp(Parser *parser) {
    assert(0 && "TODO");
}

static AST_Node *parse_bexp(Parser *parser) {
    assert(0 && "TODO");
}

static AST_Node *parse_atom_stmt(Parser *parser) {
    Token t = lex_next(parser->lex);

    /* Assignment */
    if (t.type == TOKEN_VAR) {

        /* Variable leaf node */
        AST_Node *var_node = create_node(NODE_VAR);
        var_node->as.var.str = t.as.str.data;
        var_node->as.var.len = t.as.str.len;

        /* Assing symbol (:=) */
        t = lex_next(parser->lex);
        expect(t, TOKEN_ASSIGN);

        /* Aexp */
        AST_Node *node = create_node(NODE_ASSIGN);
        node->as.child.left = var_node;
        node->as.child.right = parse_aexp(parser);

        return node;
    }

    /* Skip */
    if (t.type == TOKEN_SKIP) {
        AST_Node *skip_node = create_node(NODE_SKIP);
        return skip_node;
    }

    /* If stmt */
    if (t.type == TOKEN_IF) {
        AST_Node *if_node = create_node(NODE_IF);

        /* Condition (b) */
        if_node->as.child.condition = parse_bexp(parser);

        /* Then symbol */
        t = lex_next(parser->lex);
        expect(t, TOKEN_THEN);

        /* S1 */
        if_node->as.child.left = parse_stmt(parser);

        /* Else symbol */
        t = lex_next(parser->lex);
        expect(t, TOKEN_ELSE);

        /* S2 */
        if_node->as.child.right = parse_stmt(parser);

        return if_node;
    }

    /* While stmt */
    if (t.type == TOKEN_WHILE) {
        AST_Node *while_node = create_node(NODE_WHILE);

        /* Condition (b) */
        while_node->as.child.condition = parse_bexp(parser);

        /* Do symbol */
        t = lex_next(parser->lex);
        expect(t, TOKEN_DO);

        /* S */
        while_node->as.child.left = parse_stmt(parser);

        return while_node;
    }

    fprintf(stderr, "[ERROR]: Unexpected token\n");
    exit(1);
}

/* Parse the sequence statements */
static AST_Node *parse_stmt(Parser *parser) {
    AST_Node *left = parse_atom_stmt(parser);

    Token t = lex_next(parser->lex);
    if (t.type == TOKEN_SEMICOL) {
        AST_Node *node = create_node(NODE_SEQ);
        node->as.child.left = left;
        node->as.child.right = parse_stmt(parser);
        return node;
    }

    return left;
}

AST_Node *parser_parse(Parser *parser) {
    parser->root = parse_stmt(parser);
    return parser->root;
}

/* ==================================================================================== */
