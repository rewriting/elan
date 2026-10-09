# iterRepeat

Strategy iterators on a non-deterministic rule family: `repeat+(dk(extract))` enumerates the elements of a list; it fails (no result) on `1`.

Run it from this directory (which `elan`: see `../README.md`):

    elan -b iterRepeat.lgi < input.inp     # batch: the queries of input.inp
    elan iterRepeat.lgi                    # interactive: type a term followed by `end`

Queries (2): the terms `1` and `element(1.2.3)` of the table of section 3.5.1 (start strategy `allRepP`). `expected.out` is the output of the 2004 reference
interpreter; `make check` compares the modern interpreter with it byte for byte.

Manual: section 3.5.1, "Strategy iterators" (`docs/manual/manual.pdf`); source `legacy/elan3/doc/ElanExamples/`.
