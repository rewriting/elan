# unification

Syntactic unification as a logic with a two-part specification (`Vars` and `Ops` in `simplesig.spc`) from which ELAN generates the signature modules; the solved form is a conjunction of equations.

`unification.eln` and `constraint.eln` are not in `ElanExamples`: they are copied from `legacy/elan3/applications/Unification/` (the logic description is the one of the manual).

Run it from this directory (which `elan`: see `../README.md`):

    elan -b unification.lgi simplesig.spc < input.inp     # batch: the queries of input.inp
    elan unification.lgi simplesig.spc                    # interactive: type a term followed by `end`

Queries (2): the query commented in `legacy/elan3/applications/Unification/simplesig.spc`, plus an invented one. `expected.out` is the output of the 2004 reference
interpreter; `make check` compares the modern interpreter with it byte for byte.

Manual: section 3.7.4 "LGI modules" (`docs/manual/manual.pdf`); logic description from `legacy/elan3/doc/ElanExamples/`.
