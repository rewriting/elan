# normalizeFirst

The query is first normalised by the unlabelled rules, then the strategy is applied: `(s1)f(a)` has no result because `f(a)` is already rewritten to `a`, on which `r1` fails.

Run it from this directory (which `elan`: see `../README.md`):

    elan -b normalizeFirst.lgi < input.inp     # batch: the queries of input.inp
    elan normalizeFirst.lgi                    # interactive: type a term followed by `end`

Queries (1): the term `f(a)` discussed in section 3.6.1. `expected.out` is the output of the 2004 reference
interpreter; `make check` compares the modern interpreter with it byte for byte.

Manual: section 3.6.1 "Rewrite rules on terms" (`docs/manual/manual.pdf`); source `legacy/elan3/doc/ElanExamples/`.
