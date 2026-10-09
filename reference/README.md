# Reference build of ELAN 3

`build.sh` rebuilds the 2004 ELAN 3 system from the verbatim sources in
`../legacy`, with the smallest possible set of portability patches, so that it
runs on a current machine (macOS arm64, GCC 16, OpenJDK, Boehm GC 8).

It is the **behavioural reference** for the modern port: the test bench of
`../tests/legacy-bench` records the outputs of this build, and the modern
implementation must reproduce them.

```sh
./build.sh                 # prepare + aterm + cpl + interpreter + compiler + library
source env.sh              # PATH, ELANLIB, JAVA_HOME
elan -b queens.lgi < queens.inp            # interpreter
elanc -nosplit queens && make -f queens.make && ./a.out -noInput   # compiler
```

Requirements: `brew install gcc openjdk bdw-gc bison flex libtool` (macOS), or
`apt install build-essential cmake bison flex libfl-dev default-jdk-headless libgc-dev automake tcsh python3`
(Linux, Ubuntu 24.04), and a **case-sensitive** file system (see the top-level
README). `build.sh` picks its defaults from `uname -s`; `ELAN_CC`, `ELAN_CXX`,
`GC_PREFIX`, `JAVA_HOME` and `AUX` override them.

## What the build does

1. copies `legacy/elan3/src` and `legacy/elan/src/aterm-1.6.5` into `build/`;
2. applies `patches/*.patch`;
3. restores what the 2003 checkout depended on: the `Makefile.in` of cpl (taken
   from the `cpl-0.6.tar.gz` it ships), current `config.guess`/`config.sub`/
   `install-sh`/`depcomp` instead of dangling links to `/sw/share/automake-1.7`,
   and removes the i386 objects left in the tree;
4. builds ATerm, cpl, the interpreter, the compiler (REM in Java + C runtime)
   and the library with pre-standard language modes (`-std=gnu89 -fcommon`,
   `-std=gnu++98 -fpermissive`, `compat/iostream.h`) into `install/`.

The Boehm GC 6.2 shipped with ELAN predates arm64; the current `bdw-gc` is used
instead (the ELAN "patched" GC only differed by its build system).

## Patches

Each patch fixes a construct that was accepted (or happened to work) with the
compilers and 32-bit machines of 2003 and is now rejected or miscompiled.

| Patch | Problem | Symptom without it |
|-------|---------|--------------------|
| `01-interpreter-missing-return` | 8 functions declared `int` end without `return`. Undefined behaviour in C++: GCC ≥ 8 treats the end of the function as unreachable. | `elan --export` loops forever in `grammar::Adump` (writes a 14 GB file) |
| `02-runtime-lvalue-casts` | casts used as lvalues (`(T)x = ...`, `&((T*)p)`), a GNU extension removed in GCC 4; `fsymtab[]` declared with an incomplete element type | the compiler runtime does not compile |
| `03-rem-enum-identifier` | `enum` used as a variable name in the JavaCC-generated REF parser (keyword since Java 5) | REM does not compile |
| `04-rem-darwin-makefile` | the Makefile generated for compiled programs only knows Linux/Cygwin | compiled programs do not link on macOS (`-lfl`, `-static`, libtool, GC paths) |
| `05-cpl-arm64` | (a) `get_sp()` returns the address of a local: GCC folds it to `NULL`; (b) the result of `alloca()` is unused and may be removed; (c) `allocStablePointer()` moved the back-trail by 4 bytes, so the saved stack copies became misaligned and the conservative GC no longer saw the pointers they contain | (a) segfault at the first choice point; (c) non-deterministic results under `-optimiseChoicePoint` (e.g. 21–39 solutions instead of 92 for 8 queens) |
| `06-rem-linux-makefile` | the Linux branch of the Makefile generated for compiled programs links statically and through `libtool`, and compiles the 1990s C in the default (C17) mode | compiled programs do not build on current Linux distributions |

## Known limitations of the reference

These are documented by the test bench rather than fixed here (fixing them is
the job of the modern port):

* the `Compiler.2.1/BenchThesis` programs (ELAN 2.1 syntax: they import a
  module `rewrite`, a keyword in ELAN 3) do not parse; the 2004 `elanc`
  ignored the failed export and compiled an incomplete `.ref`, which shows up
  as builtin code clashes (`fun_202` ...) when the C code is compiled;
* some reference outputs (`SAMPLES/*.out`) were produced with older libraries
  and printers; see `../tests/legacy-bench/README.md`.
