#include "include/abstract_analyzer.h"
#include <stdio.h>
#include <string.h>

void print_help(char **argv) {
    fprintf(stderr, "Usage: %s COMMAND [ARGS]\n\n", argv[0]);

    fprintf(stderr, "Commands:\n");
    fprintf(stderr, "  cfg             Control Flow Graph utils.\n");
    fprintf(stderr, "  analyze         Static analysis.\n\n");

    fprintf(stderr, "Run '%s COMMAND' for more information on a specific command.\n", argv[0]);
}

void print_help_cfg(char **argv) {
    fprintf(stderr, "Usage: %s cfg SOURCE\n\n", argv[0]);
    fprintf(stderr, "Dump the Control Flow Graph of the SOURCE (Graphviz format).\n\n");

    fprintf(stderr, "Arguments:\n");
    fprintf(stderr, "  SOURCE          Path to the source file (While language).\n");
}

void print_help_analyze(char **argv) {
    fprintf(stderr, "Usage: %s analyze DOMAIN [ARGS]\n\n", argv[0]);
    fprintf(stderr, "Run the static analysis on the SOURCE using the selected abstract domain.\n\n");

    fprintf(stderr, "Arguments:\n");
    fprintf(stderr, "  DOMAIN          Abstract domain (available domains are listed below).\n\n");

    fprintf(stderr, "Domains:\n");
    fprintf(stderr, "  p_interval      Parametric Interval domain.\n\n");

    fprintf(stderr, "Run '%s analyze DOMAIN' for more information on a specific domain.\n", argv[0]);
}

void print_help_analyze_p_interval(char **argv) {
    fprintf(stderr, "Usage: %s analyze p_interval [OPTIONS] SOURCE\n\n", argv[0]);
    fprintf(stderr, "TODO: Parametric interval exaplain.\n\n");

    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -m INT          Lower bound of the domain (default: -INF).\n");
    fprintf(stderr, "  -n INT          Upper bound of the domain (default: +INF).\n");
    fprintf(stderr, "  -wdelay N       Number of steps to wait before applying widening (default: disabled).\n");
    fprintf(stderr, "  -dsteps N       Number of descending steps (narrowing) (default: 0).\n\n");

    fprintf(stderr, "Arguments:\n");
    fprintf(stderr, "  SOURCE          Path to the source file (While language).\n");
}

const char *get_opt(const char *opt, size_t i, int argc, char **argv) {
    return NULL; /* TODO */
}

int64_t parse_int64(const char *c) {
    return 0; /* TODO */
}

size_t parse_size(const char *c) {
    return 0; /* TODO */
}

void handle_cfg_cmd(int argc, char **argv) {

    /* Help handling */
    if (argc == 2) {
        print_help_cfg(argv);
    }
    else if (argc == 3) {
        const char *src_path = argv[2];
        While_Analyzer_Opt opt = {0};
        While_Analyzer *wa = while_analyzer_init(src_path, &opt);
        while_analyzer_cfg_dump(wa, stdout);
        while_analyzer_free(wa);
    }
    else {
        print_help_cfg(argv);
    }
}

void handle_analyze_cmd(int argc, char **argv) {

    /* Help handling */
    if (argc == 2) {
        print_help_analyze(argv);
    }
    else if (argc == 3) {
        if (strcmp(argv[2], "p_interval") == 0) {
            print_help_analyze_p_interval(argv);
        }
        else {
            print_help_analyze(argv);
        }
    }
    /* Parametric interval handling */
    else if (strcmp(argv[2], "p_interval") == 0) {
        While_Analyzer_Opt opt = {
            .type = WHILE_ANALYZER_PARAMETRIC_INTERVAL,
            .as = {
                .parametric_interval = {
                    .m = INT64_MIN,
                    .n = INT64_MAX,
                },
            },
        };

        While_Analyzer_Exec_Opt exec_opt = {
            .widening_delay = SIZE_MAX,
            .descending_steps = 0,
        };

        const char *src_path = argv[3];

        /* get_opt return NULL if not found or the pointer to the start of the flag */
        const char *m = get_opt("m", 4, argc, argv);
        const char *n = get_opt("n", 4, argc, argv);
        const char *wdelay = get_opt("wdelay", 4, argc, argv);
        const char *dsteps = get_opt("dsteps", 4, argc, argv);

        if (m != NULL) {
            opt.as.parametric_interval.m = parse_int64(m);
        }
        if (n != NULL) {
            opt.as.parametric_interval.n = parse_int64(n);
        }
        if (wdelay != NULL) {
            exec_opt.widening_delay = parse_size(wdelay);
        }
        if (dsteps != NULL) {
            exec_opt.descending_steps = parse_size(dsteps);
        }

        While_Analyzer *wa = while_analyzer_init(src_path, &opt);
        while_analyzer_exec(wa, &exec_opt);
        while_analyzer_states_dump(wa, stdout);
        while_analyzer_free(wa);
    }
    else {
        print_help_analyze(argv);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        print_help(argv);
        return 0;
    }

    if (strcmp(argv[1], "cfg") == 0) {
        handle_cfg_cmd(argc, argv);
    }
    else if (strcmp(argv[1], "analyze") == 0) {
        handle_analyze_cmd(argc, argv);
    }
    else {
        print_help(argv);
    }
}
