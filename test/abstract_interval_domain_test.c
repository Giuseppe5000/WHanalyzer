#include "../src/domain/abstract_interval_domain.c"
#include "../src/common.h"
#include <assert.h>
#include <stdio.h>

void interval_leq_test(void) {
    // STD intervals
    Interval i1 = {
        .type = INTERVAL_STD,
        .a = 10,
        .b = 12,
    };

    Interval i2 = {
        .type = INTERVAL_STD,
        .a = 10,
        .b = 20,
    };

    assert(interval_leq(i1, i2) == true);

    // i1 and i2 BOTTOM
    i1.type = INTERVAL_BOTTOM;
    i2.type = INTERVAL_BOTTOM;
    assert(interval_leq(i1, i2));

    // i1 BOTTOM
    i1.type = INTERVAL_BOTTOM;
    i2 = (Interval) {
        .type = INTERVAL_STD,
        .a = 10,
        .b = 20,
    };
    assert(interval_leq(i1, i2) == true);

    // i2 BOTTOM
    i1 = (Interval) {
        .type = INTERVAL_STD,
        .a = 10,
        .b = 20,
    };
    i2.type = INTERVAL_BOTTOM;
    assert(interval_leq(i1, i2) == false);
}

void interval_create_test(void) {
    // m,n integers
    int64_t m = -10;
    int64_t n = 10;
    Variables vars = {0};
    Constants c = {0};
    Abstract_Interval_Ctx *ctx = abstract_interval_ctx_init(m, n, vars, c);
    Interval i = {0};

    i = interval_create(ctx, 3, 2);
    assert(i.type == INTERVAL_BOTTOM);

    i = interval_create(ctx, INTERVAL_MIN_INF, INTERVAL_PLUS_INF);
    assert(i.type == INTERVAL_STD);

    i = interval_create(ctx, 2, 2);
    assert(i.type == INTERVAL_STD && i.a == i.b && i.a == 2);

    i = interval_create(ctx, -11, -11);
    assert(i.type == INTERVAL_STD && i.a == i.b && i.a == -11);

    i = interval_create(ctx, 100, 100);
    assert(i.type == INTERVAL_STD && i.a == i.b && i.a == 100);

    i = interval_create(ctx, INTERVAL_MIN_INF, 9);
    assert(i.type == INTERVAL_STD);

    i = interval_create(ctx, 9, INTERVAL_PLUS_INF);
    assert(i.type == INTERVAL_STD);

    i = interval_create(ctx, -6, 7);
    assert(i.type == INTERVAL_STD);

    i = interval_create(ctx, -20, 7);
    assert(i.type == INTERVAL_STD && i.a == INTERVAL_MIN_INF && i.b == 7);

    // Try to create (-INF, -INF) / (INF,INF)
    i = interval_create(ctx, INTERVAL_MIN_INF, INTERVAL_MIN_INF);
    assert(i.type == INTERVAL_STD && i.a == INTERVAL_MIN_INF && i.b == INTERVAL_PLUS_INF);

    i = interval_create(ctx, INTERVAL_PLUS_INF, INTERVAL_PLUS_INF);
    assert(i.type == INTERVAL_STD && i.a == INTERVAL_MIN_INF && i.b == INTERVAL_PLUS_INF);

    abstract_interval_ctx_free(ctx);

    // m,n infinite
    m = INTERVAL_MIN_INF;
    n = INTERVAL_PLUS_INF;
    ctx = abstract_interval_ctx_init(m, n, vars, c);

    i = interval_create(ctx, 3, 2);
    assert(i.type == INTERVAL_BOTTOM);

    i = interval_create(ctx, INTERVAL_MIN_INF, INTERVAL_PLUS_INF);
    assert(i.type == INTERVAL_STD);

    i = interval_create(ctx, 2, 2);
    assert(i.type == INTERVAL_STD && i.a == i.b && i.a == 2);

    i = interval_create(ctx, -11, -11);
    assert(i.type == INTERVAL_STD && i.a == i.b && i.a == -11);

    i = interval_create(ctx, 11, 11);
    assert(i.type == INTERVAL_STD && i.a == i.b && i.a == 11);

    i = interval_create(ctx, INTERVAL_MIN_INF, 9);
    assert(i.type == INTERVAL_STD);

    i = interval_create(ctx, 9, INTERVAL_PLUS_INF);
    assert(i.type == INTERVAL_STD);

    i = interval_create(ctx, -6, 7);
    assert(i.type == INTERVAL_STD);

    // Try to create (-INF, -INF) / (INF,INF)
    i = interval_create(ctx, INTERVAL_MIN_INF, INTERVAL_MIN_INF);
    assert(i.type == INTERVAL_STD && i.a == INTERVAL_MIN_INF && i.b == INTERVAL_PLUS_INF);

    i = interval_create(ctx, INTERVAL_PLUS_INF, INTERVAL_PLUS_INF);
    assert(i.type == INTERVAL_STD && i.a == INTERVAL_MIN_INF && i.b == INTERVAL_PLUS_INF);

    abstract_interval_ctx_free(ctx);
}

void interval_union_test(void) {
    // m,n integers
    int64_t m = -10;
    int64_t n = 10;
    Variables vars = {0};
    Constants c = {0};
    Abstract_Interval_Ctx *ctx = abstract_interval_ctx_init(m, n, vars, c);
    Interval i1 = {0};
    Interval i2 = {0};
    Interval i_union = {0};

    i1 = interval_create(ctx, 1, -1); // Bottom
    i2 = interval_create(ctx, 1, -1); // Bottom
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_BOTTOM);

    i1 = interval_create(ctx, 1, -1); // Bottom
    i2 = interval_create(ctx, 0, 2);  // [0, 2]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == i2.a && i_union.b == i2.b);

    i1 = interval_create(ctx, 0, 2);  // [0, 2]
    i2 = interval_create(ctx, 1, -1); // Bottom
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == i1.a && i_union.b == i1.b);

    i1 = interval_create(ctx, INTERVAL_MIN_INF, INTERVAL_PLUS_INF); // Top
    i2 = interval_create(ctx, INTERVAL_MIN_INF, INTERVAL_PLUS_INF); // Top
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == i1.a && i_union.b == i1.b);

    i1 = interval_create(ctx, INTERVAL_MIN_INF, INTERVAL_PLUS_INF); // Top
    i2 = interval_create(ctx, 1, -1); // Bottom
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == i1.a && i_union.b == i1.b);

    i1 = interval_create(ctx, 1, -1); // Bottom
    i2 = interval_create(ctx, INTERVAL_MIN_INF, INTERVAL_PLUS_INF); // Top
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == i2.a && i_union.b == i2.b);

    i1 = interval_create(ctx, INTERVAL_MIN_INF, INTERVAL_PLUS_INF); // Top
    i2 = interval_create(ctx, 0, 2);  // [0, 2]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == i1.a && i_union.b == i1.b);

    i1 = interval_create(ctx, 0, 2);  // [0, 2]
    i2 = interval_create(ctx, INTERVAL_MIN_INF, INTERVAL_PLUS_INF); // Top
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == i2.a && i_union.b == i2.b);

    i1 = interval_create(ctx, -2, 0);  // [-2, 0]
    i2 = interval_create(ctx, 1, 2);   // [1, 2]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == i1.a && i_union.b == i2.b);

    i1 = interval_create(ctx, -1, 0);  // [-2, 0]
    i2 = interval_create(ctx, 1, 2);   // [1, 2]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == i1.a && i_union.b == i2.b);

    i1 = interval_create(ctx, 1, 2);  // [1, 2]
    i2 = interval_create(ctx, -2, 0); // [-2, 0]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == i2.a && i_union.b == i1.b);

    i1 = interval_create(ctx, INTERVAL_MIN_INF, 5); // (-INF, 5]
    i2 = interval_create(ctx, 1, 2); // [1, 2]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == i1.a && i_union.b == i1.b);

    i1 = interval_create(ctx, INTERVAL_MIN_INF, 5); // (-INF, 5]
    i2 = interval_create(ctx, 1, 8); // [1, 8]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == i1.a && i_union.b == i2.b);

    i1 = interval_create(ctx, -1, INTERVAL_PLUS_INF); // [-1, +INF)
    i2 = interval_create(ctx, 1, 2); // [1, 2]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == i1.a && i_union.b == i1.b);

    i1 = interval_create(ctx, -1, INTERVAL_PLUS_INF); // [-1, +INF)
    i2 = interval_create(ctx, -5, 8); // [-5, 8]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == i2.a && i_union.b == i1.b);

    i1 = interval_create(ctx, -1, INTERVAL_PLUS_INF); // [-1, +INF)
    i2 = interval_create(ctx, INTERVAL_MIN_INF, 8); // (-INF, 8]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == i2.a && i_union.b == i1.b);

    i1 = interval_create(ctx, 100, 100);  // [100, 100]
    i2 = interval_create(ctx, 1, 2);      // [1, 2]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == i2.a && i_union.b == INTERVAL_PLUS_INF);

    i1 = interval_create(ctx, -100, -100);  // [-100, -100]
    i2 = interval_create(ctx, 1, 2);        // [1, 2]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == INTERVAL_MIN_INF && i_union.b == i2.b);

    i1 = interval_create(ctx, -100, -100);  // [-100, -100]
    i2 = interval_create(ctx, -101, -101);  // [-101, -101]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == INTERVAL_MIN_INF && i_union.b == m);

    i1 = interval_create(ctx, 100, 100);  // [100, 100]
    i2 = interval_create(ctx, 101, 101);  // [101, 101]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == n && i_union.b == INTERVAL_PLUS_INF);

    i1 = interval_create(ctx, -100, -100);  // [-100, -100]
    i2 = interval_create(ctx, 101, 101);    // [101, 101]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == INTERVAL_MIN_INF && i_union.b == INTERVAL_PLUS_INF);

    i1 = interval_create(ctx, -1, INTERVAL_PLUS_INF); // [-1, +INF)
    i2 = interval_create(ctx, -100, -100);            // [-100, -100]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == INTERVAL_MIN_INF && i_union.b == INTERVAL_PLUS_INF);

    i1 = interval_create(ctx, -1, INTERVAL_PLUS_INF); // [-1, +INF)
    i2 = interval_create(ctx, 100, 100);              // [100, 100]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == i1.a && i_union.b == INTERVAL_PLUS_INF);

    i1 = interval_create(ctx, INTERVAL_MIN_INF, 8); // (-INF, 8]
    i2 = interval_create(ctx, -100, -100);          // [-100, -100]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == INTERVAL_MIN_INF && i_union.b == i1.b);

    i1 = interval_create(ctx, INTERVAL_MIN_INF, 8); // (-INF, 8]
    i2 = interval_create(ctx, 100, 100);            // [100, 100]
    i_union = interval_union(ctx, i1, i2);
    assert(i_union.type == INTERVAL_STD && i_union.a == INTERVAL_MIN_INF && i_union.b == INTERVAL_PLUS_INF);

    abstract_interval_ctx_free(ctx);
}

void interval_intersect_test(void) {
    // m,n integers
    int64_t m = -10;
    int64_t n = 10;
    Variables vars = {0};
    Constants c = {0};
    Abstract_Interval_Ctx *ctx = abstract_interval_ctx_init(m, n, vars, c);
    Interval i1 = {0};
    Interval i2 = {0};
    Interval i_intersect = {0};

    i1 = interval_create(ctx, 1, -1); // Bottom
    i2 = interval_create(ctx, 5, 10); // [5, 10]
    i_intersect = interval_intersect(ctx, i1, i2);
    assert(i_intersect.type == INTERVAL_BOTTOM);

    i1 = interval_create(ctx, 5, 10); // [5, 10]
    i2 = interval_create(ctx, 1, -1); // Bottom
    i_intersect = interval_intersect(ctx, i1, i2);
    assert(i_intersect.type == INTERVAL_BOTTOM);

    i1 = interval_create(ctx, 0, 2);   // [0, 2]
    i2 = interval_create(ctx, 5, 10);  // [5, 10]
    i_intersect = interval_intersect(ctx, i1, i2);
    assert(i_intersect.type == INTERVAL_BOTTOM);

    i1 = interval_create(ctx, -10, -5); // [-10, -5]
    i2 = interval_create(ctx, 0, 5);    // [0, 5]
    i_intersect = interval_intersect(ctx, i1, i2);
    assert(i_intersect.type == INTERVAL_BOTTOM);

    i1 = interval_create(ctx, 0, 5);   // [0, 5]
    i2 = interval_create(ctx, 3, 8);   // [3, 8]
    i_intersect = interval_intersect(ctx, i1, i2);
    assert(i_intersect.type == INTERVAL_STD);
    assert(i_intersect.a == 3);
    assert(i_intersect.b == 5);

    i1 = interval_create(ctx, 3, 8);   // [3, 8]
    i2 = interval_create(ctx, 0, 5);   // [0, 5]
    i_intersect = interval_intersect(ctx, i1, i2);
    assert(i_intersect.type == INTERVAL_STD);
    assert(i_intersect.a == 3);
    assert(i_intersect.b == 5);

    i1 = interval_create(ctx, 0, 10);  // [0, 10]
    i2 = interval_create(ctx, 2, 5);   // [2, 5]
    i_intersect = interval_intersect(ctx, i1, i2);
    assert(i_intersect.type == INTERVAL_STD);
    assert(i_intersect.a == 2);
    assert(i_intersect.b == 5);

    i1 = interval_create(ctx, 0, 3);   // [0, 3]
    i2 = interval_create(ctx, 3, 6);   // [3, 6]
    i_intersect = interval_intersect(ctx, i1, i2);
    assert(i_intersect.type == INTERVAL_STD);
    assert(i_intersect.a == 3);
    assert(i_intersect.b == 3);

    i1 = interval_create(ctx, INTERVAL_MIN_INF, 5);  // (-INF, 5]
    i2 = interval_create(ctx, 0, INTERVAL_PLUS_INF); // [0, +INF)
    i_intersect = interval_intersect(ctx, i1, i2);
    assert(i_intersect.type == INTERVAL_STD);
    assert(i_intersect.a == 0);
    assert(i_intersect.b == 5);

    i1 = interval_create(ctx, INTERVAL_MIN_INF, INTERVAL_PLUS_INF); // Top
    i2 = interval_create(ctx, -2, 2); // [-2, 2]
    i_intersect = interval_intersect(ctx, i1, i2);
    assert(i_intersect.type == INTERVAL_STD);
    assert(i_intersect.a == -2);
    assert(i_intersect.b == 2);

    abstract_interval_ctx_free(ctx);
}

void interval_plus_test(void) {
    // m,n integers
    int64_t m = -10;
    int64_t n = 10;
    Variables vars = {0};
    Constants c = {0};
    Abstract_Interval_Ctx *ctx = abstract_interval_ctx_init(m, n, vars, c);
    Interval i1 = {0};
    Interval i2 = {0};
    Interval i_plus = {0};

    i1 = interval_create(ctx, 1, -1); // Bottom
    i2 = interval_create(ctx, 1, -1); // Bottom
    i_plus = interval_plus(ctx, i1, i2);
    assert(i_plus.type == INTERVAL_BOTTOM);

    i1 = interval_create(ctx, 5, 10); // [5, 10]
    i2 = interval_create(ctx, 1, -1); // Bottom
    i_plus = interval_plus(ctx, i1, i2);
    assert(i_plus.type == INTERVAL_BOTTOM);

    i1 = interval_create(ctx, 1, -1); // Bottom
    i2 = interval_create(ctx, 5, 10); // [5, 10]
    i_plus = interval_plus(ctx, i1, i2);
    assert(i_plus.type == INTERVAL_BOTTOM);

    i1 = interval_create(ctx, 2, 4); // [2, 4]
    i2 = interval_create(ctx, 5, 6); // [5, 6]
    i_plus = interval_plus(ctx, i1, i2);
    assert(i_plus.type == INTERVAL_STD);
    assert(i_plus.a == 7);
    assert(i_plus.b == 10);

    // Overapproximations

    i1 = interval_create(ctx, 2, 6); // [2, 6]
    i2 = interval_create(ctx, 5, 6); // [5, 6]
    i_plus = interval_plus(ctx, i1, i2);
    assert(i_plus.type == INTERVAL_STD);
    assert(i_plus.a == 7);
    assert(i_plus.b == INTERVAL_PLUS_INF);

    i1 = interval_create(ctx, 10, 10); // [10, 10]
    i2 = interval_create(ctx, 5, 6);   // [5, 6]
    i_plus = interval_plus(ctx, i1, i2);
    assert(i_plus.type == INTERVAL_STD);
    assert(i_plus.a == n);
    assert(i_plus.b == INTERVAL_PLUS_INF);

    abstract_interval_ctx_free(ctx);
}

void interval_minus_test(void) {
    // m,n integers
    int64_t m = -10;
    int64_t n = 10;
    Variables vars = {0};
    Constants c = {0};
    Abstract_Interval_Ctx *ctx = abstract_interval_ctx_init(m, n, vars, c);
    Interval i1 = {0};
    Interval i2 = {0};
    Interval i_minus = {0};

    i1 = interval_create(ctx, 1, -1); // Bottom
    i2 = interval_create(ctx, 1, -1); // Bottom
    i_minus = interval_minus(ctx, i1, i2);
    assert(i_minus.type == INTERVAL_BOTTOM);

    i1 = interval_create(ctx, 5, 10); // [5, 10]
    i2 = interval_create(ctx, 1, -1); // Bottom
    i_minus = interval_minus(ctx, i1, i2);
    assert(i_minus.type == INTERVAL_BOTTOM);

    i1 = interval_create(ctx, 1, -1); // Bottom
    i2 = interval_create(ctx, 5, 10); // [5, 10]
    i_minus = interval_minus(ctx, i1, i2);
    assert(i_minus.type == INTERVAL_BOTTOM);

    i1 = interval_create(ctx, 2, 4); // [2, 4]
    i2 = interval_create(ctx, 5, 6); // [5, 6]
    i_minus = interval_minus(ctx, i1, i2);
    assert(i_minus.type == INTERVAL_STD);
    assert(i_minus.a == -4);
    assert(i_minus.b == -1);

    // Overapproximations

    i1 = interval_create(ctx, -1, 6); // [-1, 6]
    i2 = interval_create(ctx, 5, 10); // [5, 10]
    i_minus = interval_minus(ctx, i1, i2);
    assert(i_minus.type == INTERVAL_STD);
    assert(i_minus.a == INTERVAL_MIN_INF);
    assert(i_minus.b == 1);

    i1 = interval_create(ctx, -10, -10); // [-10, -10]
    i2 = interval_create(ctx, 5, 6);     // [5, 6]
    i_minus = interval_minus(ctx, i1, i2);
    assert(i_minus.type == INTERVAL_STD);
    assert(i_minus.a == INTERVAL_MIN_INF);
    assert(i_minus.b == m);

    abstract_interval_ctx_free(ctx);
}

void interval_mult_test(void) {
    // m,n integers
    int64_t m = -10;
    int64_t n = 10;
    Variables vars = {0};
    Constants c = {0};
    Abstract_Interval_Ctx *ctx = abstract_interval_ctx_init(m, n, vars, c);
    Interval i1 = {0};
    Interval i2 = {0};
    Interval i_mult = {0};

    i1 = interval_create(ctx, 1, -1); // Bottom
    i2 = interval_create(ctx, 1, -1); // Bottom
    i_mult = interval_mult(ctx, i1, i2);
    assert(i_mult.type == INTERVAL_BOTTOM);

    i1 = interval_create(ctx, 5, 10); // [5, 10]
    i2 = interval_create(ctx, 1, -1); // Bottom
    i_mult = interval_mult(ctx, i1, i2);
    assert(i_mult.type == INTERVAL_BOTTOM);

    i1 = interval_create(ctx, 1, -1); // Bottom
    i2 = interval_create(ctx, 5, 10); // [5, 10]
    i_mult = interval_mult(ctx, i1, i2);
    assert(i_mult.type == INTERVAL_BOTTOM);

    i1 = interval_create(ctx, 2, 3); // [2, 3]
    i2 = interval_create(ctx, 2, 3); // [2, 3]
    i_mult = interval_mult(ctx, i1, i2);
    assert(i_mult.type == INTERVAL_STD);
    assert(i_mult.a == 4);
    assert(i_mult.b == 9);

    i1 = interval_create(ctx, -3, -2); // [-3, -2]
    i2 = interval_create(ctx, 2, 4);   // [2, 4]
    i_mult = interval_mult(ctx, i1, i2);
    assert(i_mult.type == INTERVAL_STD);
    assert(i_mult.a == INTERVAL_MIN_INF);
    assert(i_mult.b == -4);

    i1 = interval_create(ctx, -3, -2); // [-3, -2]
    i2 = interval_create(ctx, -3, -2); // [-3, -2]
    i_mult = interval_mult(ctx, i1, i2);
    assert(i_mult.type == INTERVAL_STD);
    assert(i_mult.a == 4);
    assert(i_mult.b == 9);

    i1 = interval_create(ctx, -2, 2); // [-2, 2]
    i2 = interval_create(ctx, -2, 2); // [-2, 2]
    i_mult = interval_mult(ctx, i1, i2);
    assert(i_mult.type == INTERVAL_STD);
    assert(i_mult.a == -4);
    assert(i_mult.b == 4);

    i1 = interval_create(ctx, 10, 10); // [10, 10]
    i2 = interval_create(ctx, 10, 10); // [10, 10]
    i_mult = interval_mult(ctx, i1, i2);
    assert(i_mult.type == INTERVAL_STD);
    assert(i_mult.a == 100);
    assert(i_mult.b == 100);

    // Overapproximations

    i1 = interval_create(ctx, 3, 4); // [3, 4]
    i2 = interval_create(ctx, 3, 3); // [3, 3]
    i_mult = interval_mult(ctx, i1, i2);
    assert(i_mult.type == INTERVAL_STD);
    assert(i_mult.a == 9);
    assert(i_mult.b == INTERVAL_PLUS_INF);

    i1 = interval_create(ctx, -5, 5); // [-5, 5]
    i2 = interval_create(ctx, 0, 0);  // [0, 0]
    i_mult = interval_mult(ctx, i1, i2);
    assert(i_mult.type == INTERVAL_STD);
    assert(i_mult.a == 0);
    assert(i_mult.b == 0);

    abstract_interval_ctx_free(ctx);
}

void interval_div_test(void) {
    // m,n integers
    int64_t m = -10;
    int64_t n = 10;
    Variables vars = {0};
    Constants c = {0};
    Abstract_Interval_Ctx *ctx = abstract_interval_ctx_init(m, n, vars, c);
    Interval i1 = {0};
    Interval i2 = {0};
    Interval i_div = {0};

    i1 = interval_create(ctx, 1, -1); // Bottom
    i2 = interval_create(ctx, 1, -1); // Bottom
    i_div = interval_div(ctx, i1, i2);
    assert(i_div.type == INTERVAL_BOTTOM);

    i1 = interval_create(ctx, 1, -1); // Bottom
    i2 = interval_create(ctx, 2, 4);  // [2, 4]
    i_div = interval_div(ctx, i1, i2);
    assert(i_div.type == INTERVAL_BOTTOM);

    i1 = interval_create(ctx, 2, 4);  // [2, 4]
    i2 = interval_create(ctx, 1, -1); // Bottom
    i_div = interval_div(ctx, i1, i2);
    assert(i_div.type == INTERVAL_BOTTOM);

    i1 = interval_create(ctx, 2, 4); // [2, 4]
    i2 = interval_create(ctx, 0, 0); // [0, 0]
    i_div = interval_div(ctx, i1, i2);
    assert(i_div.type == INTERVAL_BOTTOM);

    i1 = interval_create(ctx, 4, 8); // [4, 8]
    i2 = interval_create(ctx, 2, 2); // [2, 2]
    i_div = interval_div(ctx, i1, i2);
    assert(i_div.type == INTERVAL_STD);
    assert(i_div.a == 2);
    assert(i_div.b == 4);

    i1 = interval_create(ctx, 10, 10); // [10, 10]
    i2 = interval_create(ctx, 2, 5);   // [2, 5]
    i_div = interval_div(ctx, i1, i2);
    assert(i_div.type == INTERVAL_STD);
    assert(i_div.a == 2);
    assert(i_div.b == 5);

    i1 = interval_create(ctx, -10, -5); // [-10, -5]
    i2 = interval_create(ctx, 1, 1);    // [1, 1]
    i_div = interval_div(ctx, i1, i2);
    assert(i_div.type == INTERVAL_STD);
    assert(i_div.a == -10);
    assert(i_div.b == -5);

    i1 = interval_create(ctx, 10, 10); // [10, 10]
    i2 = interval_create(ctx, 1, 1);   // [1, 1]
    i_div = interval_div(ctx, i1, i2);
    assert(i_div.type == INTERVAL_STD);
    assert(i_div.a == 10);
    assert(i_div.b == 10);

    i1 = interval_create(ctx, 10, 10); // [10, 10]
    i2 = interval_create(ctx, 2, 2);   // [2, 2]
    i_div = interval_div(ctx, i1, i2);
    assert(i_div.type == INTERVAL_STD);
    assert(i_div.a == 5);
    assert(i_div.a == 5);

    i1 = interval_create(ctx, -5, 5); // [-5, 5]
    i2 = interval_create(ctx, 2, 2);  // [2, 2]
    i_div = interval_div(ctx, i1, i2);
    assert(i_div.type == INTERVAL_STD);
    assert(i_div.a == -2);
    assert(i_div.b == 2);

    abstract_interval_ctx_free(ctx);
}

int main(void) {
    interval_leq_test();
    printf("[TEST PASS]: interval_leq\n");
    interval_create_test();
    printf("[TEST PASS]: interval_create\n");
    interval_union_test();
    printf("[TEST PASS]: interval_union\n");
    interval_intersect_test();
    printf("[TEST PASS]: interval_intersect\n");
    interval_plus_test();
    printf("[TEST PASS]: interval_plus\n");
    interval_minus_test();
    printf("[TEST PASS]: interval_minus\n");
    interval_mult_test();
    printf("[TEST PASS]: interval_mult\n");
    interval_div_test();
    printf("[TEST PASS]: interval_div\n");
    return 0;
}
