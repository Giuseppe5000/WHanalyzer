#ifndef WHILE_AI_ABSTRACT_DOM_
#define WHILE_AI_ABSTRACT_DOM_

#include <stdint.h>

enum Interval_Type {
    INT_STD,
    INT_BOTTOM,
};

/*
Define the integer interval [a,b].
If a or b are INF, then the value is:
    INT64_MAX for represent infinite.
    INT64_MIN for represent -infinite.
*/
typedef struct {
    enum Interval_Type type;
    int64_t a;
    int64_t b;
} Interval;

/*
Create a parametric interval domain.
Domain is defined as the union of this sets :
    { BOTTOM, TOP }
    { [k,k] | k ∈ Z }
    { [a, b] | a < b, [a, b] ⊆ [m, n] }
    { (-INF, k] | k ∈ [m, n] }
    { [k, +INF) | k ∈ [m, n] }

NOTE: m,n represents -infinite/infinite if they are INT_MIN/INT_MAX.
*/
Interval_Dom *interval_dom_init(int64_t m, int64_t n);

/*
Create an interval beloging to the domain, if a or b.

NOTE: if [a, b] does not belong to the domain dom, an assertion will occurr.
*/
Interval interval_create(Interval_Dom *dom, int64_t a, int64_t b);

/*
TODO: define:
    interval_leq(Interval a, Interval b);
    interval_union(Interval a, Interval b);
    interval_intersect(Interval a, Interval b);

    interval_plus(Interval a, Interval b);
    interval_minus(Interval a, Interval b);
    interval_mult(Interval a, Interval b);
    interval_div(Interval a, Interval b);
*/

/* Free the domain */
interval_dom_free(Interval_Dom *dom);

#endif /* WHILE_AI_ABSTRACT_DOM_ */
