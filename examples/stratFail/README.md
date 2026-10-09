# stratFail

Strategy iterators and failure: `repeat*(dc(a2b))` returns `b` from `a` and also from `b` (zero iteration = identity), unlike `repeat+`.

Run it from this directory (which `elan`: see `../README.md`):

    elan -b stratFail.lgi < input.inp     # batch: the queries of input.inp
    elan stratFail.lgi                    # interactive: type a term followed by `end`

Queries (2): the terms `a` and `b` of the table of section 3.5.1 (start strategy `repeatS`). `expected.out` is the output of the 2004 reference
interpreter; `make check` compares the modern interpreter with it byte for byte.

Manual: section 3.5.1, "Strategy iterators" (`docs/manual/manual.pdf`); source `legacy/elan3/doc/ElanExamples/`.
