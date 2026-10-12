# ELAN

ELAN is a rewriting-logic language and environment developed at LORIA (Nancy)
from 1994 to 2004: computations are rewrite rules, and *strategies* control how
and where they are applied, including non-deterministic search with
backtracking. This repository preserves the original system and revives it.

## Layout

| Path | Purpose |
|------|---------|
| `legacy/` | verbatim archive of the historical sources (never edited; a few files renamed `x-case-*`, see `CASE-RENAMES.tsv`) |
| `reference/` | rebuild of ELAN 3 (2004) from `legacy/` with minimal portability patches; the behavioural reference |
| `tests/legacy-bench/` | the historical test bench (778 tests), its baseline and reference snapshots |
| `src/interpreter/` | the modern interpreter, one directory per module (see below) |
| `src/compiler-go/` | the compiler `elanc` (also `elanc-go`): ELAN to Go, its runtime module `elanrt` |
| `src/compiler/` | REM, the compiler of 2004, installed as `elanc-rem`: Java C generator, C runtime, Earley parser, choice points (`cpl`) |
| `src/compiler-rs/` | `elanc-rs`, the compiler in Rust (frozen, built only with `-DELAN_RUST=ON`) |
| `src/lib/elanlib/` | the standard library |
| `examples/` | runnable examples of the manual (checked by `make check`) |
| `docs/manual/` | the ELAN 3.6 user manual (LaTeX sources, PDF) |
| `docs/ref-format.md` | the `.ref` export format and the constants it depends on (REF contract) |
| `tests/unit/`, `tests/architecture/` | C++ unit tests of the modules; module dependency rules, compiler copies of the limits |
| `tests/regression/` | targeted regression tests (one directory per case) |
| `tests/golden/` | golden tests: exported `.ref` files, `-d` dump, statistics, error messages and exit status |
| `ci/`, `.github/workflows/` | Linux image for local checks, CI on Ubuntu and macOS |

## Requirements

macOS on Apple Silicon (other Unix systems should only need path changes).
The modern interpreter only needs the Xcode command line tools (system Clang)
and CMake; the compiler `elanc` needs Go >= 1.24 (when building ELAN and
when compiling a program: it builds the programs with the `go` found by
CMake); REM (`elanc-rem`) and its tests need a JDK and the Boehm GC (bison
and flex come with the command line tools); the 2004 reference system
(`make reference`) is built with Homebrew GCC:

```sh
xcode-select --install   # system Clang (cc/c++), bison, flex
brew install cmake go openjdk bdw-gc   # interpreter, elanc, elanc-rem: make, make check
brew install gcc libtool               # reference system
```

Without Go (or with Go older than 1.24), the build still succeeds: `elanc`
is then REM's driver (`elanc-rem`), with a CMake message.

### File names and case

The repository works on case-insensitive file systems (the macOS and Windows
defaults). The few files of the imported archives (`legacy/`) and of the
manual's examples whose names differed only by case from another file are
stored with an `x-case-` prefix, contents unchanged, and listed in
`CASE-RENAMES.tsv`; the legacy bench gives them back their names in its
private copy of an application, `tests/restore_case.sh DEST` rebuilds the
original layout, and `tests/check_case.sh` (run by `make check`) refuses new
collisions. Only the 2004 reference system (`make reference`) needs a
case-sensitive volume, for its build tree and install (`reference/README.md`).

## Getting started

```sh
make install PREFIX=$HOME/.local        # build and install elan, elanc, elanc-rem + the library
export PATH=$HOME/.local/bin:$PATH
cd examples/poly1
elan poly1.lgi                          # interactive: type  deriv(X) end
elan -b poly1.lgi < input.inp           # batch: the queries of input.inp
```

`poly1` (section 1.2 of the manual) differentiates polynomials:

```
$ cat input.inp
deriv(X) end
deriv(3*X*X + 2*X + 7) end
$ elan -b poly1.lgi < input.inp
 1 end
 ...
 0*X*X+3*1*X+X*1+0*X+2*1+0 end
```

* `examples/` — 13 runnable examples of the manual, each with its queries
  and expected output (`examples/README.md`);
* `docs/manual/manual.pdf` — the ELAN 3.6 user manual (99 pages, rebuilt from
  the 2003 LaTeX sources with `make manual`);
* `legacy/elan3/applications/` — larger historical applications (completion,
  unification, constraint solving, ...).

## Compiling ELAN programs

`elanc` compiles a program to a native executable `./a.out`, through Go (the
compiler of `src/compiler-go/`, also installed as `elanc-go`; since S7e it
is what `elanc` designates, REM being `elanc-rem`):

```sh
cd legacy/elan/sources/Compiler.4.0/Test   # or a copy of it
elanc enum                                 # enum.lgi [spec.spc]: builds ./a.out (work directory .elan-go.enum/)
./a.out -noInput                           # runs the start term of the program
echo 'enum(o,s(o)) end' | ./a.out          # or reads the queries on the standard input
```

The command line is REM's: `elanc [elan options] [compiler options]
program[.lgi] [specification[.spc]]`. `elanc` exports the program with
`elan --cexport` (options it does not know, such as `-b`, `-t N`, `-l DIR`,
`--elanlib DIR`, are passed to `elan`), translates it to a Go module and
builds it with `go build` (offline, with a build cache shared by the
programs: `--gocache DIR`, default `$ELANC_GO_CACHE`, else
`~/.cache/elanc-go`). `-o NAME` or `-output NAME` names the program
(default `a.out`); `-v` prints the commands; `elanc -help` lists the
options. The first program compiled with an empty cache takes ~1.7 s (the
runtime is compiled once), the others 0.1-0.4 s. `elanc` exits with 0 on
success, 2 when the program uses a construct that is not supported
(`docs/followups.md`, "elanc-go: kept refusals") or an option that only REM
has, 1 on any other failure; a refusal or a failure leaves no `a.out`. The
programs it generates print their results as REM's, have the options
`-noInput`, `-noOutput`, `-quiet`, `-help`, and exit with:

| status | meaning |
|---|---|
| 0 | success |
| 1 | run-time error (message on the standard error), or `-help` |
| 2 | a construct supported only partly, met at run time |
| 3 | recursion too deep: the stack is nearly exhausted, the program stops cleanly (`elan: recursion too deep ...`) |

On an error (1, 2, 3) the results printed before it are still written to
the standard output.

On the 145 programs of the bench that both compile, `elanc` gives REM's
results and rewrite steps; programs running longer than 20 ms run 1.6-7x
faster than with REM's choice-point optimisation (`elanc-rem -O`: propc2
0.24 s → 0.08 s, BenchThesis p8 1.33 s → 0.81 s; 7-38x faster than REM
without it), the
compile time is 0.11 s instead of 0.19 s (median, REM's `elanc` + `make`);
the programs use more memory (median 5.4 MB instead of 2.9 MB) and are
~3.3 MB executables (REM's: 0.2 MB). Details:
`docs/measures/2026-10-12-rem-O/README.md` (times) and
`docs/superpowers/plans/2026-10-11-s7e-go-switch.md`.

### Differences from REM's driver (`elanc-rem`)

The common command line is the same (program, specification, `-b` and the
other `elan` options, `-output NAME`); REM's options without effect on the
result are accepted and ignored (`-nosplit`, `-split`, `-quiet`, `-O`,
`-O2`, `-fast`, `-optimiseChoicePoint`, `-noOptimiseChoicePoint`,
`-verbose`, `-color`, `-withGoto`, `-strategy 0`, ...). The differences:

* `elanc` builds `./a.out` itself: there is no `<prog>.make` to run with
  `make -f`, no `.elan.<prog>/` C sources (the Go module is in
  `.elan-go.<prog>/`). A script that runs `make -f prog.make` after `elanc`
  calls `elanc-rem` instead.
* REM's options that change the result or the files written are refused
  (status 2, `elanc: option -coq is REM's: use elanc-rem`): `-coq`,
  `-proofterm`, `-lib`, `-debug`, `-onlyC`, `-noC`, `-nocode`,
  `-strategy 1`, `-strategy 2`.
* `-O2` (REM: no rewrite counter) is ignored: the program counts its steps.
* `elanc` prints nothing on success (REM's driver echoes its commands and
  REM's banner unless `-quiet`; `-v` prints `elanc`'s); with no argument it
  prints its usage on the standard error and exits with 1 (REM: 0).
* The generated programs do not have REM's `-REFInput`, `-commandLine`,
  `-sort=`, `-strategy=` (status 2 when the query is read) and ignore
  `-REFOutput`, `-internalOutput`, `-debug`, `-trace`, `-filequery`.
* A few constructs that REM compiles are refused (status 2, "not supported
  (not planned)", each with its reason in `docs/followups.md`): AC patterns
  REM transforms before matching, an identifier in an AC pattern, AC
  operators of builtin sorts, rules for a builtin operation. Conversely,
  `elanc` fixes REM's bugs listed there (traversals, deep recursion, ...).
* Go >= 1.24 is needed at compile time instead of a JDK, the Boehm GC and
  a C compiler.

To make `elanc` REM's driver again: `make install CMAKE_FLAGS=-DELAN_ELANC=rem`
(the CMake variable `ELAN_ELANC`, `go` by default, makes `bin/elanc` a
symlink to `elanc-go` or `elanc-rem`; `go` falls back to `rem` when
`elanc-go` is not built). The ELAN 3.6 manual of 2003 (`docs/manual/`)
describes REM: its `elanc` is today's `elanc-rem`.

### REM: `elanc-rem`

`elanc-rem` (REM, the compiler of 2004) compiles a program to C and
generates a Makefile that builds a native executable `a.out` (linked with
the runtime libraries installed next to `elan`):

```sh
elanc-rem -nosplit -quiet enum            # enum.lgi [spec.spc]: writes enum.make and .elan.enum/
make -f enum.make                         # compiles .elan.enum/*.c, copies a.out here
./a.out -noInput                          # runs the query of the program
```

`elanc-rem` first exports the program with `elan --cexport enum.ref`
(options it does not know, such as `-b`, are passed to `elan`), then runs
the C generator REM (options such as `-optimiseChoicePoint`, `-debug`,
`-quiet`, `-output name`; `elanc-rem` alone lists them). The generated
Makefile uses the C/C++ compilers and the Boehm GC of the build (`ELAN_CC`,
`ELAN_CXX`, `GC_PREFIX`, which the environment can override; `ELAN_SANITIZE`
holds the sanitizer options in a `make check-sanitize` build). The tests
call it by this name, whatever `elanc` is. `elanc-rem` and REM exit with a
non-zero status on errors. The ATerm runtime of 2003 (`-aterm`) is not
supported. With `-strategy 1`, the strategy terms of `strat[X]`
(`elanlib/strategy/strat.eln`) are evaluated at run time by the C runtime
(`str_eval`) instead of the compiled `eval` rules; with `-strategy 2`, REM
compiles to C the strategy terms known at compile time (both are compared
with the interpreter by `tests/compiler/test_runtime_strategies.sh`).

### `elanc-rs`

`elanc-rs` (built only with `-DELAN_RUST=ON`, e.g.
`make install CMAKE_FLAGS=-DELAN_RUST=ON`, and cargo; frozen, see
`docs/s6-rust-status.md`)
compiles a program through Rust instead: `elanc-rs -b enum` exports the
program, writes a cargo crate in `.elan-rs.enum/` and builds `./a.out`
(`elanc-rs -help` lists the options). Its statuses and those of its
programs are `elanc`'s.

## Quick start (development)

```sh
make                 # build the interpreter and the compiler (CMake, into build/)
make check           # unit tests, architecture, golden tests, 368 bench tests + 13 examples; elanc: its tests, 160 bench cases, 22 BenchThesis cases; elanc-rem: 410 compiled tests, 64 BenchThesis cases
make check-compiler  # only the compiled tests of REM (tests/compiler, J/JO kinds of the bench)
make check-go        # only the tests of elanc (Go): go test, normalisation, strategies, bench G, queries
make check-elanc     # what bin/elanc is (ELAN_ELANC); make check-elanc-rem: the same with -DELAN_ELANC=rem
make check-sanitize  # same under ASan+UBSan (UBSan only on macOS; compiled programs: UBSan)
make install PREFIX=$HOME/.local   # any file system
make reference       # build the 2004 reference system (interpreter + compiler)
make check-reference # full bench (778 tests) against the reference
```

The modern interpreter and the compiler's libraries (C runtime, Earley parser,
cpl) build with GCC and Clang as C17/C++17 with `-Wall -Wextra -Werror`;
the default compilers are the system Clang (`cc`/`c++`) on macOS and
`gcc`/`g++` on Linux; the programs generated by `elanc-rem` are built with the
same compilers (the generated C itself keeps the pre-standard
`-std=gnu89 -w -fcommon` of 2004). Override them with, e.g.,
`make BUILD=build-gcc ELAN_CC=gcc-16 ELAN_CXX=g++-16 PREFIX=$PWD/build-gcc/install check`
(changing the compilers, `PREFIX` or the CMake options recreates the build
directory automatically). `make reference` uses Homebrew's
`gcc-16`/`g++-16` on macOS unless `ELAN_CC`/`ELAN_CXX` are set. Linux needs
`build-essential clang libclang-rt-18-dev cmake bison flex libfl-dev default-jdk-headless libgc-dev automake tcsh python3`;
`ci/Dockerfile.linux` reproduces the Linux CI job locally. See
`CONTRIBUTING.md` for the rules every change follows.

## Interpreter modules

`src/interpreter/` is split into modules; a module may only include headers of
the modules below it (checked by `make check-arch`, existing exceptions listed
with their reason in `tests/architecture/allowed-exceptions.txt`):

| Module | Contents |
|--------|----------|
| `driver` | `main`, command line, query loop |
| `command`, `compile`, `peval`, `ref`, `meta` | command language (`-C`); C code generation (`-c`); partial evaluation; REF export/import/reduce; meta level |
| `load` | `.lgi`/`.eln` semantic actions, module system, global tables |
| `rewrite` | rules, strategies, rewrite system, reduction, strategy interpreter, builtins |
| `match` | matching (syntactic, AC via `acmatcher/`), processes |
| `term` | terms, symbols, printing |
| `parse` | grammars (macc inputs), Earley parser, term building |
| `lex` | lexems, lexer, preprocessor |
| `base` | streams, string/int tables, allocation, options |

Conventions inside the interpreter:

* **Fatal errors** print their message and call `failexit()`
  (`base/fatal.h`). In `elan_main` (and in unit tests, inside a
  `FatalCatcher`) it throws `ElanFatal`; `elan_main` catches it, flushes
  stdout and stderr, kills the subprocesses and returns 1. The forked
  children (`match/process.cc`), the ^C handler, static initialisation and
  destruction exit directly as before.
* **Terms** (`term/termdefs.h`) are non-owning handles to shared nodes,
  managed by hand with reference counters (`incrcount`, `tdelete`); no RAII.
* **Parser state**: the semantic actions of the module parser share one
  `LoaderState ld` (`load/strategy.h`; one instance for every import
  level), the module frames being read are `modframes[0..stacki]`; the
  `.ref` parser has its own `RefParserState rp` (`ref/ref.cc`).

`parser/` is the parser-table generator `macc`, `acmatcher/` the AC matcher.
Everything except `driver/main.cc` is the library `elan_core`, which the unit
tests (`tests/unit/test_*.cc`, using `tests/unit/check.h`) link against: add a
file `tests/unit/test_<module>.cc` and `make check` runs it.

## License

ELAN is distributed under the GNU General Public License (see the headers of
the legacy sources).
