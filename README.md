# Abstract interpreter for the abstract equational semantics X# of While Language

## Overview
TODO

## Abstract Domain

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
- [ ] Lexing doesn't tokenize negative numbers (-x).
- [ ] Check if the CFG struct is good for the fixpoint computation.
- [ ] Implement `cfg_init` for getting the CFG from the AST.
- [ ] Implement the abstract domain and all the operations in a standalone module.
- [ ] Wire the CFG with an abstract state and do the fixpoint computation.
