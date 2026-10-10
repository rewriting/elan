# Follow-ups

Known issues and deferred work, collected from the reviews of each
sub-project (S1–S5b). Each item says where it comes from.

## Behaviour (needs tests against the interpreter or the reference)

- **`elanc -strategy 2`** (REM `Flags.strat`, option not listed in the
  usage): the generated C does not compile with current compilers
  (`StrategyEval.genEvalLab`/`genEvalDstr` call `str_ruleN`/`str_dstrN`,
  which are not declared: `-Wint-conversion` errors), and
  `genEvalSem` DC/IFTOE still store `allocStable` (an index) in an `int *`
  (the 3.6 bug fixed in `str_eval2`). `-strategy 1` (strategy terms
  evaluated by `str_eval`/`str_eval2`) works and is tested
  (`tests/compiler/test_runtime_strategies.sh`); the `if b then S1 else S2
  fi` case of `str_eval2` (DS_IFTE) is not covered by it.
- **`robot` example** (`legacy/elan3/doc/ElanExamples/robot`,
  `applications/Robot`): no result with either interpreter, although the 1997
  reference output shows paths (S4).
- **`Compiler.2.1/BenchThesis` in the legacy bench**: the 30 J/JO cases (and
  the I/A cases) of `legacy/` still report `ERROR`/`FAIL` there (ELAN 2.1
  syntax: `rewrite` is a keyword in ELAN 3; `legacy/` is not modified). The
  ported copies in `tests/compiler/benchthesis/` (identifier `rewrite`
  renamed `rewriting`) run in `make check` (`test_benchthesis.sh`, 64 cases,
  reference outputs as oracle, modern = reference byte for byte). Remaining
  differences with the 2000 outputs: printing, `rewrite_step` counts
  (+7–14 %), `group` (2000 output made with another `group.spc`) and the
  binding order of MinelaComp `iappend` (see
  `tests/compiler/benchthesis/README.md`).
- **The rtmisc.cc `STRSUBSTR` builtin falls through into `STRSPN`** for other
  argument sorts — kept as in 2004, probably unintended (S2).
- **`trace_backup`/`trace_recover`** (`-coq -proofterm` compiled programs)
  are not safe under backtracking (slots reused after `trace_recover`); and
  `norm_4` now returns `t` where 2004 returned an undefined value (S5b).
- **Very long names (> ~1000 characters)**: `snprintf` truncates where
  `sprintf` overflowed; only `attach_type_modu` fails loudly (S2).
- **`main_coq_skeleton.c`** (`elanc -coq`) does not compile with current
  compilers (pre-ANSI multi-line string); `elanc -noC` links a non-existent
  `-lelan` (S5b).

## Build, tests, tooling

- **CI**: the Linux job failed at the ASan+UBSan step on GitHub (x86_64) while
  passing in local arm64 Docker; on hold at the user's request (S2).
- `ELAN_CC`/`ELAN_CXX` mean both the build compilers and the generated
  Makefiles' defaults (environment override) (S5a).
- `-lfl` (Linux, generated programs) is not checked by CMake (S5a).
- Installs are not relocatable (absolute paths in `elanc`, `elan`) (S5a).
- `CMAKE_BUILD_TYPE` changes the runtime library flags (S5a).
- `check_limits.py` does not strip `/* */` blocks or `#if` branches and skips
  names missing from one copy (S3b review).
- Very deep inputs are now bounded only by the C stack (recursive term
  functions, module loading), not by fixed limits with a message (S3b).
- Static library `elan_core`: relies on every member being referenced and on
  archive order for static initialisation (S3a).
- `check_deps.py` strips `/*...*/` even inside string literals (S3a).
- C code generation is still referenced from the driver (`genmaintfile`) and
  `compiledefs.h` from term/grammar/partial (listed exceptions) (S3a).
- Runner: an example with several `.spc` aborts the bench; a spec named
  `no.spc` collides with the "no" sentinel (S4).
- 325 raw-type `javac -Xlint` warnings in REM (S5b).

## Planned sub-projects

- **S3b — interpreter internals** (spec `2026-10-10-s3b-internals-design.md`):
  done: goldens of what the bench does not see, contract guards, no silent
  overflow, dynamic stacks, parser state grouped (`LoaderState ld`,
  `RefParserState rp`, module frames), fatal errors as `ElanFatal`, `term`
  unit test (`tests/unit/test_fatal.cc`). Kept by decision: hashed table
  sizes and codes (REF contract), manual term ownership (documented in
  `term/termdefs.h`). Left: `interr()` still `abort()`s; the C AC matcher
  exits by itself on its own errors (out of memory); `impmoduli` and
  `withrhs` stay separate globals until lex/parse stop reaching into the
  loader.
- **cpl redesign** (backtracking without copying the C stack), which would
  also allow ASan for the compiled programs.
