# ELAN

ELAN is a rewriting-logic language and environment developed at LORIA (Nancy)
from 1994 to 2004: computations are rewrite rules, and *strategies* control how
and where they are applied, including non-deterministic search with
backtracking. This repository preserves the original system and revives it.

## Layout

| Path | Purpose |
|------|---------|
| `legacy/` | verbatim archive of the historical sources (never edited) |
| `reference/` | rebuild of ELAN 3 (2004) from `legacy/` with minimal portability patches; the behavioural reference |
| `tests/legacy-bench/` | the historical test bench (778 tests), its baseline and reference snapshots |
| `src/interpreter/` | the modern interpreter, one directory per module (see below) |
| `src/compiler/` | the compiler `elanc`: REM (Java, C generator), C runtime, Earley parser, choice points (`cpl`) |
| `src/lib/elanlib/` | the standard library |
| `examples/` | runnable examples of the manual (checked by `make check`) |
| `docs/manual/` | the ELAN 3.6 user manual (LaTeX sources, PDF) |
| `tests/unit/`, `tests/architecture/` | C++ unit tests of the modules; module dependency rules |
| `tests/regression/` | targeted regression tests (one directory per case) |
| `tests/golden/` | golden tests: exported `.ref` files, `-d` dump, statistics, error messages and exit status |
| `ci/`, `.github/workflows/` | Linux image for local checks, CI on Ubuntu and macOS |

## Requirements

macOS on Apple Silicon (other Unix systems should only need path changes).
The modern interpreter only needs the Xcode command line tools (system Clang)
and CMake; the compiler also needs a JDK and the Boehm GC (bison and flex come
with the command line tools); the 2004 reference system (`make reference`) is
built with Homebrew GCC:

```sh
xcode-select --install   # system Clang (cc/c++), bison, flex
brew install cmake openjdk bdw-gc   # modern interpreter and compiler: make, make check
brew install gcc libtool            # reference system
```

### Case-sensitive file system

Some files of `legacy/` (and of the manual's example copies in `docs/manual/`)
differ only by the case of their name (`any.eln` / `Any.eln`, `Robot.lgi` /
`robot.lgi`, ...), and the default macOS file system is case-insensitive.
Work in a case-sensitive
APFS volume:

```sh
hdiutil create -size 20g -type SPARSE -fs "Case-sensitive APFS" -volname elan ~/github/elan.sparseimage
mkdir -p ~/github/elan
hdiutil attach ~/github/elan.sparseimage -mountpoint ~/github/elan -nobrowse
cd ~/github/elan && git clone https://github.com/rewriting/elan .
```

After a reboot, only the `hdiutil attach` line is needed. This is only needed
for the repository (because of `legacy/`): the installed interpreter and its
library work on any file system.

## Getting started

```sh
make install PREFIX=$HOME/.local        # build and install elan, elanc + the library
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

`elanc` compiles a program to C and generates a Makefile that builds a native
executable `a.out` (linked with the runtime libraries installed next to
`elan`):

```sh
cd legacy/elan/sources/Compiler.4.0/Test   # or a copy of it
elanc -nosplit -quiet enum               # enum.lgi [spec.spc]: writes enum.make and .elan.enum/
make -f enum.make                          # compiles .elan.enum/*.c, copies a.out here
./a.out -noInput                           # runs the query of the program
```

`elanc` first exports the program with `elan --cexport enum.ref` (options it
does not know, such as `-b`, are passed to `elan`), then runs the C
generator REM (options such as `-optimiseChoicePoint`, `-debug`, `-quiet`,
`-output name`; `elanc` alone lists them). The generated Makefile
uses the C/C++ compilers and the Boehm GC of the build (`ELAN_CC`,
`ELAN_CXX`, `GC_PREFIX`, which the environment can override; `ELAN_SANITIZE`
holds the sanitizer options in a `make check-sanitize` build). `elanc` and
REM exit with a non-zero status on errors. The ATerm runtime of 2003
(`-aterm`) is not supported.

## Quick start (development)

```sh
make                 # build the interpreter and the compiler (CMake, into build/)
make check           # unit tests, architecture, golden tests, 368 bench tests + 13 examples, 410 compiled tests
make check-compiler  # only the compiled tests (elanc, J/JO kinds of the bench)
make check-sanitize  # same under ASan+UBSan (UBSan only on macOS; compiled programs: UBSan)
make install PREFIX=$HOME/.local   # any file system
make reference       # build the 2004 reference system (interpreter + compiler)
make check-reference # full bench (778 tests) against the reference
```

The modern interpreter and the compiler's libraries (C runtime, Earley parser,
cpl) build with GCC and Clang as C17/C++17 with `-Wall -Wextra -Werror`;
the default compilers are the system Clang (`cc`/`c++`) on macOS and
`gcc`/`g++` on Linux; the programs generated by `elanc` are built with the
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

`parser/` is the parser-table generator `macc`, `acmatcher/` the AC matcher.
Everything except `driver/main.cc` is the library `elan_core`, which the unit
tests (`tests/unit/test_*.cc`, using `tests/unit/check.h`) link against: add a
file `tests/unit/test_<module>.cc` and `make check` runs it.

## License

ELAN is distributed under the GNU General Public License (see the headers of
the legacy sources).
