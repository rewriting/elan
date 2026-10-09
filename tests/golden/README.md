# Golden tests

The bench (`tests/legacy-bench`) compares the standard output of programs
only. These tests pin, byte for byte, what it does not see:

* the `.ref` files written by `elan --export` and `elan --cexport` for 27
  bench programs (applications and contributions whose I/A tests pass,
  `Compiler.4.0/Test` programs whose compiled J tests pass): their numbers are
  stringtab hash positions and symbol codes, read by `elan --import`, by REM
  and by the compiled runtime (see `docs/ref-format.md`);
* the `-d` dump of 5 programs and the `-s`/`-S` statistics of 6 (times and
  speeds normalised);
* the standard output, standard error and exit status of deliberate errors
  (`programs/`: syntax error, missing file or module, undefined sort or
  operator, bad query, too many variables, overflow of the tables of
  identifiers, symbols and string constants, deep imports, nested
  strategies) and of a term deeper than the 2004 term stack.

Files:

| | |
|---|---|
| `cases.tsv` | the cases (format in `run_golden.py`) |
| `expected/` | the goldens: `<id>.ref`, `<id>.cref`, `<id>.txt` |
| `programs/` | the small programs of the `run` cases; `gen.py` generates large ones in the work copy |
| `run_golden.py` | the runner (`make check-golden`, part of `make check`) |
| `test_run_golden.py` | its unit tests |

    python3 tests/golden/run_golden.py [--prefix PREFIX] [--filter REGEX]
    python3 tests/golden/run_golden.py --update     # regenerate the goldens

A golden changes only on purpose: commit the refreshed files with the change
that explains them, state the behaviour change in the message, and update the
table below.

## Origin

| goldens | generated from |
|---|---|
| all (initial set) | commit `96974dc` (S3b design), Apple Clang 17, macOS |
