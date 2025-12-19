#include "src/lang/parser.h"

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
    char *src = malloc(file_size*sizeof(char));
    if (src == NULL) {
        fprintf(stderr, "[ERROR]: OOM when allocating %zu bytes.\n", (file_size + 1)*sizeof(char));
        exit(1);

    }
    fread(src, file_size, 1, fp);
    src[file_size] = '\0';
    fclose(fp);

    Parser *parser = parser_init(src);

    AST_Node *root = parser_parse(parser);

    parser_print_ast(root);

    parser_free(parser);
    free(src);
    return 0;
}
