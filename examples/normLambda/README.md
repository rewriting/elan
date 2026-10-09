# normLambda

The `normalise` strategy: normal form of de Bruijn lambda terms with the labelled rules `beta` and `eta` applied anywhere in the term (`normalise(first one(beta,eta))`).

Run it from this directory (which `elan`: see `../README.md`):

    elan -b normLambda.lgi < input.inp     # batch: the queries of input.inp
    elan normLambda.lgi                    # interactive: type a term followed by `end`

Queries (2): invented (the manual shows the module but no query): two beta-redexes. `expected.out` is the output of the 2004 reference
interpreter; `make check` compares the modern interpreter with it byte for byte.

Manual: section 3.5.1, "The normalize strategy" (`docs/manual/manual.pdf`); source `legacy/elan3/doc/ElanExamples/`.
