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

/* List of language keywords with associated type */
typedef struct {
    const char *keyword;
    enum Token_Type type;
} Keyword_Token;

const Keyword_Token keywords[] = {
    { .keyword = "+"     , .type = TOKEN_PLUS    },
    { .keyword = "-"     , .type = TOKEN_MINUS   },
    { .keyword = "*"     , .type = TOKEN_MULT    },
    { .keyword = "/"     , .type = TOKEN_DIV     },
    { .keyword = "true"  , .type = TOKEN_TRUE    },
    { .keyword = "false" , .type = TOKEN_FALSE   },
    { .keyword = "="     , .type = TOKEN_EQ      },
    { .keyword = "<="    , .type = TOKEN_LEQ     },
    { .keyword = "!"     , .type = TOKEN_NOT     },
    { .keyword = "&"     , .type = TOKEN_AND     },
    { .keyword = ":="    , .type = TOKEN_ASSIGN  },
    { .keyword = "skip"  , .type = TOKEN_SKIP    },
    { .keyword = ";"     , .type = TOKEN_SEMICOL },
    { .keyword = "if"    , .type = TOKEN_IF      },
    { .keyword = "then"  , .type = TOKEN_THEN    },
    { .keyword = "else"  , .type = TOKEN_ELSE    },
    { .keyword = "while" , .type = TOKEN_WHILE   },
    { .keyword = "do"    , .type = TOKEN_DO      },
};

const size_t keywords_len = sizeof(keywords) / sizeof(keywords[0]);

Lexer *lex_init(const char *src) {
    Lexer *lex = xmalloc(sizeof(Lexer));
    lex->src = src;
    lex->cursor = src;
    return lex;
}

static void skip_space(Lexer *lex) {
    while (isspace(*lex->cursor)) {
        lex->cursor++;
    }
}

/*
Check if 'kt.keyword' is pointed by the lexer cursor.
If yes then returns the token associated with that keyword and updates the cursor.
Otherwise returns a token with EOF type.
*/
static Token check_keyword(Lexer *lex, Keyword_Token kt) {
    Token t = {0};

    if (strncmp(lex->cursor, kt.keyword, strlen(kt.keyword)) == 0) {
        t.type = kt.type;
        t.as.str.data = lex->cursor;
        t.as.str.len = strlen(kt.keyword);

        lex->cursor += t.as.str.len;
    }

    return t;
}

Token lex_next(Lexer *lex) {
    Token t = {0};

    skip_space(lex);

    /* EOF */
    if (*lex->cursor == '\0') {
        return t;
    }

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

    /* Keywords */
    for (size_t i = 0; i < keywords_len; ++i) {
        Token t_keyword = check_keyword(lex, keywords[i]);

        if (t_keyword.type != TOKEN_EOF) {
            return t_keyword;
        }
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

    assert(0 && "UNREACHABLE");
}

void lex_free(Lexer *lex) {
    free(lex);
}
