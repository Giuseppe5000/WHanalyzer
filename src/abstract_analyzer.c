#include "../include/abstract_analyzer.h"
#include "lang/cfg.h"
#include "lang/parser.h"
#include "utils.h"
#include "domain/abstract_interval_domain.h"

#include <stdio.h>
#include <stdlib.h>

/*
TODO: Wrap every point of the CFG with an abstract state.
P0 is initialized with top and other points to bottom.

Then apply the worklist algorithm and return the fixpoint.
*/

typedef void Abstract_State;

struct While_Analyzer {
    AST_Node *ast;
    CFG *cfg;
    Abstract_State **state;
    char *src;
    struct {
        Abstract_State *(*state_init_bottom) (const char **var_names, size_t var_count);
        Abstract_State *(*state_init_top) (const char **var_names, size_t var_count);
        Abstract_State (*state_free)(Abstract_State *s);

        Abstract_State *(*exec_command) (const Abstract_State *s, const AST_Node *command);
        bool (*abstract_state_leq) (const Abstract_State *s1, const Abstract_State *s2);
        Abstract_State *(*U) (const Abstract_State *s1, const Abstract_State *s2); /* Union */
        Abstract_State *(*widening) (const Abstract_State *s1, const Abstract_State *s2);
        Abstract_State *(*narrowing) (const Abstract_State *s1, const Abstract_State *s2);
    } func;
};


/* =================== Parametric interval domain Int(m,n) wrappers =================== */
Abstract_State* state_init_bottom_abstract_int(const char **var_names, size_t var_count) {
    return (Abstract_State *)abstract_int_state_init_bottom(var_names, var_count);
}
/* TODO */
/* ==================================================================================== */

While_Analyzer *while_analyzer_init_parametric_interval(const char *src_path) {
    /* Open source file */
    FILE *fp = fopen(src_path, "r");

    if (fp == NULL) {
        fprintf(stderr, "[ERROR]: File %s not found.\n", src_path);
        exit(1);
    }

    /* Getting file size */
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    /* Copy the text in a buffer */
    char *src = xmalloc((file_size + 1)*sizeof(char));
    fread(src, file_size, 1, fp);
    src[file_size] = '\0';
    fclose(fp);

    /* Init analyzer */
    While_Analyzer *wa = xmalloc(sizeof(While_Analyzer));
    wa->src = src;

    /* Lexer */
    Lexer *lex = lex_init(src);

    /* AST */
    wa->ast = parser_parse(lex);
    lex_free(lex);

    /* Get CFG */
    wa->cfg = cfg_get(wa->ast);

    /* TODO: get all variable names from CFG */
    wa->state = NULL; // xmalloc...

    /* TODO: link all domain functions */
    wa->func.state_init_bottom = state_init_bottom_abstract_int;

    return wa;
}

// void while_analyzer_exec(While_Analyzer *wa) {
// }

void while_analyzer_free(While_Analyzer *wa) {
    free(wa->src);
    parser_free_ast(wa->ast);
    cfg_free(wa->cfg);
    free(wa->state);
    free(wa);
}
