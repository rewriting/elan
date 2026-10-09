# quick

Quicksort with a `where` whose left-hand side is a *pattern* (`[s,l] := ()pivot(...)`) and the `choose try` construction.

Run it from this directory (which `elan`: see `../README.md`):

    elan -b quick.lgi < input.inp     # batch: the queries of input.inp
    elan quick.lgi                    # interactive: type a term followed by `end`

Queries (1): adapted from the list commented out in `quick.lgi` (the manual shows the module but no query). `expected.out` is the output of the 2004 reference
interpreter; `make check` compares the modern interpreter with it byte for byte.

Manual: section 3.4.3 "Choose-Try" (`docs/manual/manual.pdf`); source `legacy/elan3/doc/ElanExamples/`.
