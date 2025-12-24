#include "src/lang/lexer.h"
#include "src/lang/parser.h"
#include "src/lang/cfg.h"


#include <stdio.h>
#include <stdlib.h>

int main(void) {
    const char *src_path = "example.while";

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
    char *src = malloc((file_size + 1)*sizeof(char));
    if (src == NULL) {
        fprintf(stderr, "[ERROR]: OOM when allocating %zu bytes.\n", (file_size + 1)*sizeof(char));
        exit(1);

    }
    fread(src, file_size, 1, fp);
    src[file_size] = '\0';
    fclose(fp);

    /* Lexer */
    Lexer *lex = lex_init(src);

    /* AST */
    AST_Node *ast = parser_parse(lex);
    lex_free(lex);

    /* CFG */
    CFG *cfg = cfg_get(ast);
    parser_free_ast(ast);
    cfg_print_graphviz(cfg);
    cfg_free(cfg);

    free(src);
    return 0;
}
