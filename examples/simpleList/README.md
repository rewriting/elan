# simpleList

Parameterised module `simpleList[X]` instantiated with `int` in the logic description.

Run it from this directory (which `elan`: see `../README.md`):

    elan -b simpleList.lgi < input.inp     # batch: the queries of input.inp
    elan simpleList.lgi                    # interactive: type a term followed by `end`

Queries (1): invented (the manual shows the modules but no query). `expected.out` is the output of the 2004 reference
interpreter; `make check` compares the modern interpreter with it byte for byte.

Manual: section 3.7.3 "Parameterised modules" (`docs/manual/manual.pdf`); source `legacy/elan3/doc/ElanExamples/`.
