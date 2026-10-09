# Follow-ups

Known issues and deferred work, collected from the reviews of each
sub-project (S1–S5b). Each item says where it comes from.

## Behaviour (needs tests against the interpreter or the reference)

- **Compiled DC/ONE and IFTOE strategies evaluated at run time** (`str_eval2`,
  `src/compiler/runtime/streval.c`): stop with "not supported by the compiler"
  (S5b). ELAN 3.6 crashed there; the intended semantics (try the next
  alternative / the else branch) is to be implemented with tests comparing the
  compiled program with the interpreter. Construct: a strategy *term*
  evaluated at run time (REM `StrategyEval.genEval`, non-built-in strategy).
- **`robot` example** (`legacy/elan3/doc/ElanExamples/robot`,
  `applications/Robot`): no result with either interpreter, although the 1997
  reference output shows paths (S4).
- **Builtin codes 200–215 collide with symbols of large programs compiled
  with REM** (`Compiler.2.1/BenchThesis`, 30 bench cases ERROR) (S1).
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
- Static library `elan_core`: relies on every member being referenced and on
  archive order for static initialisation (S3a).
- `check_deps.py` strips `/*...*/` even inside string literals (S3a).
- C code generation is still referenced from the driver (`genmaintfile`) and
  `compiledefs.h` from term/grammar/partial (listed exceptions) (S3a).
- Runner: an example with several `.spc` aborts the bench; a spec named
  `no.spc` collides with the "no" sentinel (S4).
- 325 raw-type `javac -Xlint` warnings in REM (S5b).

## Planned sub-projects

- **S3b — interpreter internals**: fixed-size global tables, ~70 parser-state
  globals, `exit()` on every error, term ownership; then a `term` unit test
  (needs a loaded symbol table).
- **cpl redesign** (backtracking without copying the C stack), which would
  also allow ASan for the compiled programs.
