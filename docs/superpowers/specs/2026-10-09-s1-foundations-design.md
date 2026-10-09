# S1 — Foundations of the modern ELAN interpreter

Date: 2026-10-09 · Status: draft, awaiting review

## 1. Context and intent

ELAN (LORIA, 1994–2004) is preserved in this repository as:

* `legacy/` — the verbatim historical sources (never edited);
* `reference/` — ELAN 3 rebuilt from `legacy/` with five minimal portability
  patches; it is the **behavioural oracle**;
* `tests/legacy-bench/` — the 778 historical tests (`itest`, `aitest`,
  `jtest`), with a status baseline and exact snapshots of the reference outputs.

**Goal of the overall project (agreed):** *executable heritage* — historical
ELAN programs must keep running unchanged; fidelity to the original behaviour
comes first; the language does not evolve.

**Strategy (agreed):** progressive modernisation of the existing code
("approach A"), oriented to make a later rewrite ("approach B", possibly module
by module) possible: tests, removal of undefined behaviour, module boundaries
and documented semantics are favoured over internal polish.

**Scope order (agreed):** interpreter first; the compiler (`elanc`: REM in Java,
C runtime, cpl) stays provided by `reference/` until a later sub-project.

**Platforms (agreed):** macOS arm64 and Linux (x86_64, arm64), with continuous
integration on GitHub Actions (the repository is public, so CI is free).

### Sub-projects

| # | Sub-project | Outcome |
|---|-------------|---------|
| **S1** | **Foundations** (this spec) | modern build, `make check`, CI on two platforms; interpreter code unchanged |
| S2 | Sanitising | strict C++17 without compatibility flags, zero warnings, ASan/UBSan clean on the whole bench, dead `#ifdef` code removed |
| S3 | Modules | explicit module boundaries and interfaces (lexing/parsing, terms, matching/AC, rules/strategies, printing, I/O), unit tests, standard containers instead of fixed tables and the custom allocator |
| S4 | Library and examples | `elanlib` without case-colliding files, organised `examples/`, converted manual |
| S5 | Compiler | decided later: modernise REM + runtime, or rewrite them without stack copying |

Each sub-project gets its own spec → plan → implementation cycle.

## 2. Goal and success criterion of S1

Provide the safety net that makes every later code change verifiable, without
changing the interpreter's code.

S1 is done when:

1. `make && make check` builds the modern interpreter from `src/` with CMake
   and replays the interpreter tests of the bench (kinds `I` and `A`,
   368 tests) against it, on macOS arm64 and Linux x86_64;
2. the result is **0 regressions against `baseline.tsv` and 0 snapshot
   differences**, except platform differences listed in a documented
   exceptions file (§6);
3. the reference build (`reference/build.sh`) also builds on Linux, and on both
   platforms it passes the full bench (778 tests) with 0 regressions and
   0 snapshot differences (same exception rule);
4. CI runs (1)–(3) on every push and pull request.

## 3. Repository layout after S1

```
elan/
├── legacy/                 unchanged
├── reference/              build.sh made portable (macOS + Linux)
├── src/
│   └── interpreter/        interpreter sources (initially identical to reference)
│       ├── src/            C++ sources of `elan`
│       ├── parser/         `macc`, the parser-table generator (C)
│       ├── acmatcher/      AC matcher library (C, flex/bison)
│       └── compat/         <iostream.h> shims (removed in S2)
├── src/lib/elanlib/        standard library (copy of the elan3 library)
├── tests/
│   ├── legacy-bench/       oracle (unchanged except runner fixes)
│   └── regression/         new targeted tests (empty in S1, format defined here)
├── CMakeLists.txt
├── Makefile                façade: make, make install, make check, make reference
├── docs/
└── .github/workflows/ci.yml
```

## 4. Source import

The first commit of `src/interpreter/` copies, without modification, the
interpreter sources as built by the reference: `legacy/elan3/src/elan-interpreter`
(`src/`, `parser/`, `acmatcher/`) with `reference/patches/01-interpreter-missing-return.patch`
applied, and `reference/compat/`. Generated files, objects, CVS directories and
autotools files are not imported. `git diff` between this commit and `legacy/`
therefore shows exactly the patch, and every later change to the interpreter is
visible in the history of `src/`.

The library is copied from `legacy/elan3/src/elan-library/elanlib` to
`src/lib/elanlib/` unchanged (both `any.eln` and `Any.eln` are kept; S4 deals
with the collision).

## 5. Build

### CMake (replaces autotools)

The `CMakeLists.txt` reproduces the steps of the 2003 `Makefile.am` files:

1. build the host tools: `macc` (from `parser/`), `mtokdef` and `mabident`
   (from `src/`), and the AC-matching library `libmatch` of `acmatcher/matcher/`
   (the stand-alone `match` program of `acmatcher/`, built with flex and bison,
   is not used by `elan` and is not built);
2. generate the parser tables with them, exactly as the `$(generated_c)` rule
   of `src/Makefile.am` does (`ppexgram.t`, `modgram.t`, `ldmodgram.t`,
   `aterm.*` → `*parsertab.cc`, `tabofident.cc`, `commtokens.h`, ...) as
   CMake custom commands with explicit inputs/outputs;
3. compile `elan` with the same definitions
   (`-DALPHA -DCOMMAND -DPEM -DBORO -DANYS -DSYMBS -DEARLEY -DTO_BE_DISTRIBUTED -DMETA -DPEVAL`)
   and `-DPREFIX="<install prefix>"`;
4. install `bin/elan` and `share/elanlib/` (the interpreter looks for the
   library in `$ELANLIB/share/elanlib`, default `PREFIX`).

S1 keeps the compatibility flags of the reference (`-std=gnu++98 -fpermissive -w`,
`-std=gnu89 -fcommon` for C, `-I compat`); removing them is S2.

Compiler selection: the reference was validated with GCC 16 on macOS. The modern
build uses the system's default C/C++ compilers when they build the code
unchanged (Clang on macOS rejects some 1998 constructs today, so CMake defaults
to GCC on macOS through `CMAKE_C_COMPILER`/`CMAKE_CXX_COMPILER` set by the
façade; on Linux the distribution GCC). Any compiler change that alters
outputs is caught by `make check`.

### Makefile façade

| Target | Action |
|--------|--------|
| `make` | configure (`cmake -S . -B build`) and build |
| `make install PREFIX=dir` | install `elan` and `elanlib` |
| `make check` | install into `build/install`, then `tests/legacy-bench/run_tests.py --prefix build/install --kinds I,A` (plus `tests/regression`) |
| `make reference` | `reference/build.sh` |
| `make check-reference` | full bench (778 tests) against `reference/install` |
| `make clean` | remove `build/` |

### `reference/build.sh` on Linux

Platform defaults are selected by `uname`: on macOS the Homebrew paths used
today; on Linux the distribution packages (`gcc`, `g++`, `default-jdk`,
`libgc-dev`, `bison`, `flex`, `automake` for `config.guess`/`config.sub`),
the `/usr` GC prefix and `-ll`/`-lfl` as available. The patch
`04-rem-darwin-makefile` already leaves the Linux branch of the generated
Makefile untouched; if the Linux branch needs fixing, it is done in a new,
documented patch.

## 6. Tests

### Oracle and statuses

`tests/legacy-bench/run_tests.py` (see its README) is the main oracle:
historical statuses compared with `baseline.tsv`, exact outputs compared with
`snapshots/`. New options needed by S1: `--prefix` (exists), a way to add
`tests/regression` as an extra test source, and an exceptions file
(`tests/legacy-bench/platform-exceptions.tsv`: test id, platform, reason) whose
entries do not count as snapshot differences on that platform.

There is a **single set of snapshots** (produced on macOS). On Linux the
reference build is first compared with it; each difference is analysed:
undefined behaviour → recorded as a bug for S2 and listed as an exception with
that reason; genuine platform difference (e.g. formatting of numbers) →
documented exception. No Linux-specific snapshot set is created.

### Improvements are expected

Fixing undefined behaviour (S2 onwards) is likely to make some currently failing
tests pass (the case-sensitive volume alone made 11 more interpreter tests pass).
The runner reports such changes as improvements against the baseline; the
corresponding snapshot then differs on purpose. Rule: **a test may improve,
never regress**. An intended change is committed on its own, updating the
snapshot and baseline, explaining the fix, and adding a case to
`tests/regression/`.

### `tests/regression/` format

One directory per case: `prog.lgi`, its `*.eln`, `input.inp`, `expected.out`,
and a one-line `README` saying what is checked. The runner treats each directory
as an `I` test. Empty in S1.

Unit tests of C++ modules start with S3 (no unit-test framework in S1).

### Case sensitivity

The bench must run on a case-sensitive file system. `run_tests.py` and
`reference/build.sh` refuse to run otherwise (the build already checks). On the
macOS CI runner, a case-sensitive APFS image is created with `hdiutil` and the
work is done inside it.

## 7. Continuous integration

`.github/workflows/ci.yml`, triggered on push and pull request, matrix
`ubuntu-latest` and `macos-latest`:

1. install dependencies (apt / Homebrew);
2. macOS: create and attach a case-sensitive APFS image, check out into it;
3. `make reference` and `make check-reference`;
4. `make` and `make check`;
5. on failure, upload `tests/legacy-bench/results/` as an artifact.

The Homebrew/apt package versions are not pinned (the point of CI is to detect
breakage by new toolchains early); a breakage is then fixed or documented.

## 8. Working rules

* Every commit touching `src/` keeps `make check` green; a commit that changes a
  snapshot is separate and explains why.
* Commit messages state the behaviour change ("none" for almost all of S1/S2).
* `reference/patches/` is frozen; it changes only for a portability bug of the
  reference itself, in a new numbered patch.
* `legacy/` is never modified.

## 9. Out of scope for S1

Any change to the interpreter's C++ code; the compiler (except making
`reference/` build on Linux); reorganising the library and examples; the
manual; unit tests.

## 10. Risks

| Risk | Mitigation |
|------|------------|
| The CMake build differs subtly from autotools (flags, generated tables) | snapshots are exact: any difference in parser tables shows up as output differences; the generated files of both builds can be diffed |
| Linux outputs differ from macOS snapshots | analysed one by one, documented in the exceptions file (§6) |
| GCC/Clang/Homebrew updates break the build | CI detects it; fix or document |
| CI time on macOS runners | the full bench takes ~30 s; the reference build a few minutes |
