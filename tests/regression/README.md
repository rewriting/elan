# Regression tests

One directory per case, added whenever a bug is fixed or a subtle behaviour is
pinned down. A case is an interpreter (`I`) test run by
`tests/legacy-bench/run_tests.py`:

    <case>/prog.lgi       LPL description (logic name: prog)
    <case>/*.eln          modules it imports
    <case>/input.inp      query terms, as for `elan -b prog.lgi < input.inp`
    <case>/expected.out   expected standard output
    <case>/README         one line: what the case checks and why
    <case>/fixes-2004     (optional) the case tests the fix of a bug of the
                          2004 system: `make check-reference` does not run it
                          (status MODERN-ONLY)
