# map

Defined (user-programmed) strategies: `map(s)` applies a strategy to every element of a list, with explicit `[.]` strategy rules and rules parameterised by an integer (`ass(n)`, `add(n)`, `sub(n)`); `start 2` gives all 3x3 combinations.

Run it from this directory (which `elan`: see `../README.md`):

    elan -b map.lgi < input.inp     # batch: the queries of input.inp
    elan map.lgi                    # interactive: type a term followed by `end`

Queries (1): invented (the manual shows the module but no query). `expected.out` is the output of the 2004 reference
interpreter; `make check` compares the modern interpreter with it byte for byte.

Manual: section 3.5.2 "Defined strategies" (`docs/manual/manual.pdf`); source `legacy/elan3/doc/ElanExamples/`.
