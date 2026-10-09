# Contributing

The goal is *executable heritage*: historical ELAN programs must keep running
with their original behaviour. Every change is checked against the reference
build of 2004.

## Rules

1. `legacy/` is never modified.
2. `reference/patches/` is frozen; a portability fix of the reference is a
   new numbered patch documented in `reference/README.md`.
3. Every commit touching `src/` keeps `make check` green.
4. A test may improve, never regress. When a fix changes an output on purpose,
   commit it on its own: refresh the snapshot and baseline
   (`tests/legacy-bench/run_tests.py --save-baseline --save-snapshots`),
   explain the fix in the message, and add a case to `tests/regression/`.
5. Platform-specific snapshot differences go to
   `tests/legacy-bench/platform-exceptions.tsv` with their analysis; there is
   only one set of snapshots.
6. Commit messages state the behaviour change ("Behaviour change: none" when
   there is none).
7. The interpreter compiles as strict C++17/C17 with `-Wall -Wextra -Werror`
   under GCC and Clang, and the bench runs clean under ASan+UBSan
   (`make check-sanitize`). The compiler sources (`src/compiler/`) are still
   built with the pre-standard flags of the reference. A warning or a sanitizer report is fixed at its
   root cause, never silenced, unless the tool is provably wrong (then say why
   in a comment and in the commit).

8. Module dependencies follow `tests/architecture/check_deps.py` (`make
   check-arch`). Never add an entry to `allowed-exceptions.txt` to make a new
   include pass: restructure instead. Removing entries is always welcome.
9. New or changed interpreter behaviour gets a unit test in `tests/unit/`
   (characterisation tests first when touching old code).

## Before pushing

    make check            # always
    make check-sanitize   # when touching src/
    make check-reference  # when touching reference/ or the bench runner
