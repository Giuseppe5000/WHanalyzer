#ifndef WHILE_AI_ABSTRACT_INTERVAL_DOM_
#define WHILE_AI_ABSTRACT_INTERVAL_DOM_

#include "../lang/parser.h"
#include <stdint.h>
#include <stddef.h>

typedef struct Interval Interval;

/*
Set the parameters for Int(m,n) and the number of variables (so the number of intervals for state).
This function MUST be called before all the other functions.

[NOTE]: This function is not thread safe.
*/
void abstract_int_configure(int64_t m, int64_t n, size_t var_count);

/*
Create an array of Abstract Interval states in the domain of parametric intervals (m,n).

For example, if we call 'abstract_int_configure(-100, 100, 10)' then
'abstract_int_state_init(1)' will return an array of 10 intervals,
'abstract_int_state_init(2)' will return an array of 20 intervals, and so on.

It is useful to create multiple abstract states in one hit for a better cache locality.

[NOTE]: This function sets all intervals in the states to bottom.

The domain of parametric intervals is defined as the union of this sets:
    { BOTTOM, TOP }
    { [k,k] | k ∈ Z }
    { [a, b] | a < b, [a, b] ⊆ [m, n] }
    { (-INF, k] | k ∈ [m, n] }
    { [k, +INF) | k ∈ [m, n] }
*/
Interval *abstract_int_state_init(size_t count);

/* Helper functions to set all intervals of a state to bottom/top */
void abstract_int_state_set_bottom(Interval *s);
void abstract_int_state_set_top(Interval *s);

/* Free the abstract state */
void abstract_int_state_free(Interval *s);

/* Abstract commands */
Interval *abstract_int_state_exec_command(const Interval *s, const AST_Node *command);

/* Compare */
bool abstract_int_state_leq(const Interval *s1, const Interval *s2);

/* Union */
Interval *abstract_int_state_union(const Interval *s1, const Interval *s2);

/* Widening */
Interval *abstract_int_state_widening(const Interval *s1, const Interval *s2);

/* Narrowing */
Interval *abstract_int_state_narrowing(const Interval *s1, const Interval *s2);

#endif  /* WHILE_AI_ABSTRACT_INTERVAL_DOM_ */
