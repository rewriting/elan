# S3a — Structure of the modern interpreter

Date: 2026-10-10 · Status: decided autonomously (the user asked to keep
developing without stopping; verification on macOS only, CI on hold).

## 1. Context

After S2 the interpreter (`src/interpreter/src`, ~38k lines of C++17) builds
cleanly with GCC and Clang and passes the bench under sanitizers, but it is a
flat directory of 1990s files: a god header (`commondefs.h`), a header cycle
(`rtdatas.h` ↔ `compiledefs.h`), C code generation mixed into the rewrite
system (`trsystem.cc`), dead code, and no unit tests. An architecture survey
(2026-10-10) mapped files, globals and coupling; its "hard spots" (fixed-size
global tables indexed from 20 files, ~70 parser-state globals, `exit()` on
every error, re-entrant parser/runtime) cannot be removed safely in one step.

S3 is therefore split:

* **S3a (this spec): structure** — make the architecture visible and enforced,
  without changing behaviour or data structures.
* **S3b (later): internals** — replace fixed tables and global parser state,
  error propagation instead of `exit()`, ownership of terms.

## 2. Goal and success criteria

1. Sources are grouped by module in directories with a documented, *enforced*
   dependency order (an architecture test in `make check` fails on any new
   forbidden `#include`).
2. Dead code is removed; C code generation (`-c` compiler mode) lives only in
   its own module; the header cycle is gone; `commondefs.h` no longer declares
   everything (module headers, with `commondefs.h` kept as a thin umbrella only
   where still needed).
3. The interpreter core is a library (`elan_core`) linked by the `elan`
   executable and by C++ unit tests (`make check` runs them).
4. `make check` (Clang and GCC) and `make check-sanitize` stay at 0/0; no
   snapshot changes.

## 3. Decisions

| # | Decision | Reason |
|---|----------|--------|
| D1 | Modules (directories under `src/interpreter/`): `base` (I/O streams, string/int tables, allocation macros, misc), `lex` (lexem, lexer, lexem buffers, preprocessor `mlstream`), `parse` (macc LR driver inputs, grammar, Earley), `term` (terms, symbol table entries, printing), `match` (syntactic matching, libmatch bridge, process I/O), `rewrite` (rules, strategies, trsystem, reduction, strategy interpreter, builtins, normalisation), `load` (.lgi/.eln semantic actions, module system), `meta` (meta level), `ref` (REF export/import/reduce, formerly "aterm"), `peval` (partial evaluation), `command` (command language), `compile` (C code generation), `driver` (main, CLI, query loop). `parser/` (macc) and `acmatcher/` stay as they are. | Mirrors the survey's natural seams; names say what the code does ("ref" is the REF format, not the ATerm library). |
| D2 | Allowed dependencies (a module may include headers of the modules listed after it): `driver` → all; `command`, `compile`, `peval`, `ref`, `meta` → `load`, `rewrite`, `match`, `term`, `parse`, `lex`, `base`; `load` → `rewrite`, `match`, `term`, `parse`, `lex`, `base`; `rewrite` → `match`, `term`, `parse`, `lex`, `base`; `match` → `term`, `parse`, `lex`, `base`; `term` → `parse`, `lex`, `base`; `parse` → `lex`, `base`; `lex` → `base`. Existing back-edges (e.g. runtime testing the `commands` flag, the preprocessor calling the strategy interpreter, printing through grammar rules) are listed explicitly in `tests/architecture/allowed-exceptions.txt`, each with a reason; the list may only shrink. | Makes coupling visible and ratchets it down instead of pretending it is gone. |
| D3 | Files move with `git mv` (history kept); `#include "x.h"` keep working through include directories during the move; no renaming of classes or functions in S3a. | Reviewable, behaviour-preserving. |
| D4 | `main()` becomes `elan_main()` in the driver; `driver/main.cc` only calls it. Everything else is the static library `elan_core`. | Unit tests link the core without a second `main`. |
| D5 | Unit tests: plain C++ with a small header-only check macro set (`tests/unit/check.h`), one executable per module test file, run by `make check` (CTest). No external framework. | No dependency to vendor or download; enough for module tests. |
| D6 | Dead code removed: `strat`/`Any`/`Slist` (strategy.h/.cc), the unused chunk allocator of `mallo.cc`, generation of `rtabofident.cc`, the g++ 2.x mangled-name shims of `gccmatch.c`. | Never called (survey §8). |
| D7 | `term::termtype()` and `isofbuiltintype()` move from `termcompile.cc` to the `term` module (used by the interpreter); C code generation parts of `trsystem.cc` move to `compile/`. | So that `compile` can be left out of a core build later. |

## 4. Out of scope (S3b and later)

Fixed-size tables, parser-state globals, error handling, term ownership,
replacing macc; library and examples (S4); compiler (S5).
