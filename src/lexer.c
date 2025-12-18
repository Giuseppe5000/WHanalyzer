#include "lexer.h"
#include "utils.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

struct Lexer {
    const char *src;
    const char *cursor;
};

Lexer *lex_init(const char *src) {
    Lexer *lex = xmalloc(sizeof(Lexer));
    lex->src = src;
    lex->cursor = src;
    return lex;
}

void skip_space(Lexer *lex) {
    while (isspace(*lex->cursor)) {
        lex->cursor++;
    }
}

Token lex_next(Lexer *lex) {
    Token t = {0};

    skip_space(lex);

    /* Parse numbers (integers) */
    if (isdigit(*lex->cursor)) {
        const char *start = lex->cursor;

        /* Skip the number */
        while (isdigit(*lex->cursor)) {
            lex->cursor++;
        }

        t.type = TOKEN_NUM;
        t.as.num = atoi(start);
        return t;
    }

    /* Variables (alphanum and start with alpha) */
    if (isalpha(*lex->cursor)) {
        const char *start = lex->cursor;

        while (isalnum(*lex->cursor)) {
            lex->cursor++;
        }

        t.type = TOKEN_VAR;
        t.as.str.data = start;
        t.as.str.len = lex->cursor - start;
        return t;
    }

    /* Single character symbols */
    if (strchr("+-*/=!&;", *lex->cursor)) {
        switch(*lex->cursor) {
            case '+':
                t.type = TOKEN_PLUS;
                break;
            case '-':
                t.type = TOKEN_MINUS;
                break;
            case '*':
                t.type = TOKEN_MULT;
                break;
            case '/':
                t.type = TOKEN_DIV;
                break;
            case '=':
                t.type = TOKEN_EQ;
                break;
            case '!':
                t.type = TOKEN_NOT;
                break;
            case '&':
                t.type = TOKEN_AND;
                break;
            case ';':
                t.type = TOKEN_SEMICOL;
                break;
            default:
                assert(0 && "UNREACHABLE");
        }

        t.as.str.data = lex->cursor;
        t.as.str.len = 1;
        return t;
    }

    return t;
}

void lex_free(Lexer *lex) {
    free(lex);
}
