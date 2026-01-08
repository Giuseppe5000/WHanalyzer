# WHanalyzer - A static analyzer for the While Language

## Overview
TODO

## Requirements
- C99-compatible compiler.
- Make (optional, you can just put all *.c files into the compiler).

## How to build
Just run
``` bash
$ make
```
And you'll find an executable file named `cli` (it has an integrated help, just run `$ ./cli`).

## Abstract Domain
There is only one abstract domain: the **Parametric Interval** $\text{Int}_{m,n}$.

Given $m, n \in \mathbb{Z} \cup \lbrace -\infty, +\infty \rbrace$,\
the domain is defined as:

$\text{Int}_{m,n} \triangleq \lbrace\varnothing, \mathbb{Z} \rbrace \cup \lbrace [k, k] \mid k \in \mathbb{Z} \rbrace \cup \lbrace [a, b] \mid a < b, [a, b] \subseteq [m, n] \rbrace \cup \lbrace (-\infty, k] \mid k \in [m, n] \rbrace \cup \lbrace [k, +\infty) \mid k \in [m, n] \rbrace$

Some notes about $m, n$:
  - With $(m, n) = (-\infty, +\infty)$ the domain will become the standard interval domain (Int/Box).
  - With $m > n$ the domain will become the constant propagation domain.

## While Language grammar
```
numerals:
    n ∈ Num (integers)

variables:
    x ∈ Var

arithmetic expressions:
    a ∈ Aexp
    a ::= n | x | a1 + a2 | a1 - a2 | a1 * a2 | a1 / a2

boolean expressions:
    b ∈ Bexp
    b ::= true | false | a1 = a2 | a1 <= a2 | !b | b1 & b2

statements:
    S ∈ Stm
    S ::= x := a | skip | S1;S2 | if b then S1 else S2 fi | while b do S done

```

## TODO

Strange issue: if the first node (program point) is a while loop, then the worklist algo will not update the loop invariant program point.
This because the definition of the transfer function says that the first program point is always the same.
So the question is: should I do the transfer function also for the first program point?
- In this case there is a fix to do in `abstract_transfer_function`, because it assumes that there is at least one predecessor (because all program points excepts the first has one).
- Otherwise I could make impossible to have a while loop as first program point (like by adding a skip instr in the CFG creation) and mantain all the rest unchanged.

Note: this problem isn't a real problem where there isn't an init file, in that case P0 (the while) will remain TOP but it is correct.\
The problem exists when a init config is set, in that case the while should not remain the same.

- [ ] Test and check correctness of arithmetic ops in the domain.
- [ ] Implement abstract tests in `abstract_interval_state_exec_command` (There is a TODO in the code).

Other things:
- [ ] Check TODOs in the code.
- [ ] Check comments.
- [ ] Styling and const correctness.
- [ ] Complete README.



