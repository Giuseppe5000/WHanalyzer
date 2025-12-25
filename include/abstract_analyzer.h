#ifndef WHILE_AI_ANALYZER_
#define WHILE_AI_ANALYZER_

#include <stdint.h>

/* TODO: external interface */

typedef struct While_Analyzer While_Analyzer;

While_Analyzer *while_analyzer_init_parametric_interval(const char *src_path, int64_t m, int64_t n);

void while_analyzer_exec(While_Analyzer *wa);

void while_analyzer_free(While_Analyzer *wa);

#endif /* WHILE_AI_ANALYZER_ */
