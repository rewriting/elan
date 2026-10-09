# poly2

Generic polynomials over a set of variables given by a *specification* (`someVariables.spc`, `Vars X.Y.Z.nil`): parameterised module `poly2[Vars]`, the `FOR EACH` preprocessor, associative-commutative (AC) `+` and `*`, a conditional rule.

Run it from this directory (which `elan`: see `../README.md`):

    elan -b poly2.lgi someVariables.spc < input.inp     # batch: the queries of input.inp
    elan poly2.lgi someVariables.spc                    # interactive: type a term followed by `end`

Queries (1): the query of section 1.3. `expected.out` is the output of the 2004 reference
interpreter; `make check` compares the modern interpreter with it byte for byte.

Manual: section 1.3 "A more generic example" (`docs/manual/manual.pdf`); source `legacy/elan3/doc/ElanExamples/`.
