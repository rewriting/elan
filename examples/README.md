# Examples of the user manual

Runnable copies of the examples of the ELAN user manual
(`docs/manual/manual.pdf`, sources `legacy/elan3/doc/ElanExamples/`). Each
directory `<name>/` holds the logic description `<name>.lgi`, the modules it
needs (library modules come from the installed library), the specification
`.spc` if the logic has one, the queries `input.inp`, the output
`expected.out` of the 2004 reference interpreter, and a `README.md`.

| Example | What it shows | Manual section |
|---|---|---|
| [poly1](poly1/) | sorts, mixfix operators, unlabelled rules, normalisation | 1.2 A very simple example |
| [poly2](poly2/) | specification file, parameterised module, `FOR EACH`, AC operators | 1.3 A more generic example |
| [poly3](poly3/) | labelled rules, strategies, `where` | 1.4 An extended example |
| [poly4](poly4/) | `factorize`/`expand` rules under a strategy, AC operators | not in the manual |
| [quick](quick/) | `where` with a pattern, `choose try` | 3.4.3 Choose-Try |
| [testStrat](testStrat/) | elementary strategies `dc`, `dk` | 3.5.1, 3.7.4 |
| [stratFail](stratFail/) | `repeat*` and failure | 3.5.1 Strategy iterators |
| [iterRepeat](iterRepeat/) | `repeat+` over a non-deterministic rule | 3.5.1 Strategy iterators |
| [normLambda](normLambda/) | `normalise` strategy (lambda calculus) | 3.5.1 The normalize strategy |
| [map](map/) | defined strategies, `[.]` rules, parameterised labels | 3.5.2 Defined strategies |
| [normalizeFirst](normalizeFirst/) | the query is normalised before the strategy | 3.6.1 Rewrite rules on terms |
| [simpleList](simpleList/) | parameterised module instantiated in the `.lgi` | 3.7.3 Parameterised modules |
| [unification](unification/) | two-part specification, generated modules | 3.7.4 LGI modules |

## Running

From an example's directory, with the modern interpreter (`make install`
puts it in `build/install/bin/elan`; it finds its library by itself):

    elan -b <name>.lgi [<spec>.spc] < input.inp    # batch: one query per `... end`
    elan <name>.lgi [<spec>.spc]                   # interactive

The 2004 reference build is used the same way after `source reference/env.sh`.
In batch mode each result is printed as `<term> end` and the results of a
query are closed by ` end end` / `end`. `make check` runs every example and
requires the modern output to be identical to `expected.out`.

## Not included

These `.lgi` files of `ElanExamples` are not runnable as examples:

| File | Reason |
|---|---|
| `concStrat.lgi` | ELAN 1 syntax (`modules` instead of `import`): parse error; the `conc` strategy is only a fragment (`testConc.eln`) |
| `testWhere.lgi` | ELAN 1 syntax (`modules`, `op ... endop`, `strategy ... end of strategy`): parse error |
| `verySimple.lgi`, `verySimpleLogic.lgi` | ELAN 1 syntax (`modules`, `bodies`, `repeat ... endrepeat`): parse error |
| `ruleWithWhere.lgi` | fragment: the strategy `strat` is not defined ("unresolved strategy reference") |
| `simplePair.lgi` | no term can be written: the sorts `X` and `Y` have no constructors |
| `simplePairSelectors.lgi` | the selector syntax `( first:X second:Y )` is not accepted by the 2004 interpreter (parse error) |
| `queens.lgi` | runs, but its only query (`queens end`) enumerates the 92 solutions in about 3.4 s per interpreter, too slow for `make check` |
| `robot.lgi` | needs `labyrint.eln` and `loop.eln`; with them, the 2004 reference build (like the modern one) returns no result for any start state (e.g. `state(0,0)`), whereas the historical `legacy/elan3/applications/Robot/SAMPLES/robot.out` shows paths; the bench case `applications/Robot` is a known baseline FAIL |
