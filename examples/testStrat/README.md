# testStrat

Elementary strategies: `dc` (don't care choose) over `dk` (don't know choose); the same strategy gives one result on `a` (`c`) and all results on `b` (`e`, `f`).

Run it from this directory (which `elan`: see `../README.md`):

    elan -b testStrat.lgi < input.inp     # batch: the queries of input.inp
    elan testStrat.lgi                    # interactive: type a term followed by `end`

Queries (2): the terms `a` and `b` discussed in section 3.5.1. `expected.out` is the output of the 2004 reference
interpreter; `make check` compares the modern interpreter with it byte for byte.

Manual: section 3.5.1 "Elementary strategies syntax" (module) and 3.7.4 "LGI modules" (logic description) (`docs/manual/manual.pdf`); source `legacy/elan3/doc/ElanExamples/`.
