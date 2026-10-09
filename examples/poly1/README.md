# poly1

Derivative of simple polynomials in one variable `X`: sorts, mixfix operators with `assocLeft`/priorities, and unlabelled rules applied by the built-in leftmost-innermost normalisation (empty strategy `()`).

Run it from this directory (which `elan`: see `../README.md`):

    elan -b poly1.lgi < input.inp     # batch: the queries of input.inp
    elan poly1.lgi                    # interactive: type a term followed by `end`

Queries (2): the two queries of section 1.2. `expected.out` is the output of the 2004 reference
interpreter; `make check` compares the modern interpreter with it byte for byte.

Manual: section 1.2 "A very simple example" (`docs/manual/manual.pdf`); source `legacy/elan3/doc/ElanExamples/`.
