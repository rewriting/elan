# poly4

Variant of poly3 with `factorize`/`expand` labelled rules and a `simplify` strategy over AC `+` and `*`.

Run it from this directory (which `elan`: see `../README.md`):

    elan -b poly4.lgi someVariables.spc < input.inp     # batch: the queries of input.inp
    elan poly4.lgi someVariables.spc                    # interactive: type a term followed by `end`

Queries (1): invented: the poly2/poly3 query. `expected.out` is the output of the 2004 reference
interpreter; `make check` compares the modern interpreter with it byte for byte.

Manual: not shown in the built manual (the module also appears in
`docs/manual/changes.tex`, which `manual.tex` does not include); source
`legacy/elan3/doc/ElanExamples/`.
