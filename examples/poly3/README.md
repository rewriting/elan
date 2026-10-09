# poly3

Same polynomials with labelled rules driven by a strategy (`last_simplify`), local variables computed by `where` with strategies, AC operators.

Run it from this directory (which `elan`: see `../README.md`):

    elan -b poly3.lgi someVariables.spc < input.inp     # batch: the queries of input.inp
    elan poly3.lgi someVariables.spc                    # interactive: type a term followed by `end`

Queries (1): the query of section 1.4. `expected.out` is the output of the 2004 reference
interpreter; `make check` compares the modern interpreter with it byte for byte.

Manual: section 1.4 "An extended example" (`docs/manual/manual.pdf`); source `legacy/elan3/doc/ElanExamples/`.
