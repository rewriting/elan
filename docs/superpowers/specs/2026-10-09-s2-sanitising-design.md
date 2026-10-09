# S2 — Sanitising the modern interpreter

Date: 2026-10-09 · Status: **done** (decided autonomously, the user asked to
proceed without stopping; to be reviewed after the fact).

Deviations found while implementing: the interpreter never used iostreams
(no `elan_std.h` needed, `compat/` deleted); ASan hangs at startup on
macOS 27 / Apple Clang 17, so `check-sanitize` is UBSan-only on macOS and
ASan+UBSan on Linux; ASan's stack-use-after-return detector is off (100x
slower on the recursive interpreter, no report found when enabled).

## 1. Context

S1 delivered `src/interpreter/` (legacy sources + one patch), a CMake build,
`make check` (368 interpreter tests: 0 regressions, 0 snapshot differences) and
CI on Ubuntu 24.04 and macOS 15. The code is still compiled in pre-standard
modes: `-std=gnu++98 -fpermissive -w`, `-std=gnu89 -w -fcommon`, with
`compat/iostream.h` shims, and only with GCC.

The overall strategy (approach A, oriented to prepare B) asks S2 to remove
undefined behaviour and compatibility crutches so that the code can be read as a
specification and later split into modules (S3).

## 2. Goal and success criteria

At the end of S2:

1. The C++ code compiles with `-std=c++17` (no `-fpermissive`, no `compat/`),
   the C code with `-std=c17` (no `-fcommon`), both with
   `-Wall -Wextra -Werror`, with **GCC and Clang**.
2. A sanitizer build (`-fsanitize=address,undefined`) runs the whole interpreter
   bench with **no sanitizer report**.
3. Code under dead configuration macros is removed.
4. `make check` stays at 0 regressions; snapshots change only for documented,
   intended fixes (CONTRIBUTING rule 4), each with a regression case.
5. CI builds with GCC and Clang and runs the sanitizer bench on Linux.

## 3. Decisions

| # | Decision | Reason |
|---|----------|--------|
| D1 | Dead `#if` branches are removed with `unifdef`, using the configuration of the 2003 build: defined `ALPHA COMMAND PEM BORO ANYS SYMBS EARLEY TO_BE_DISTRIBUTED META PEVAL`; undefined every other project option (`RUNTIME GCMEM STORM DEBUG DEBUG2 DEBUG3 VISIGRAPH MEMORY PICALC BLABLA LEX_BLABLABLA CONCUR_BLABLA SUN __DECCXX PURIFY JUNK JUNK_CODE JUNK_1006 TO_BE_REMOVED ORIGINAL_VERSION OLD_HISTORY OLDHISTORY ONLY_FOR_DEBUG WASDONE MARIANS NO_MORE_SWITCH RSWITCH VRSION1901 ONE_HISTORY HISTORY BINS`), each checked not to be `#define`d in the sources (`DEEP` is, in `acmatcher/matcher/tools.h`, so it is left alone). The inputs of the parser generator (`include.parser`, `*parser.h`, `*.t`) are processed too, since they end up in the generated sources. The `-D` options then disappear from the build. | `unifdef` is exact (it evaluates the conditions like the preprocessor), so behaviour cannot change; the old variants remain in `legacy/`. |
| D2 | `<iostream.h>`-style headers are replaced by the standard headers; the code uses `std::` names through a single project header (`elan_std.h`) with explicit `using` declarations, not `using namespace std` in every file. | Removes `compat/`; keeps the diff small and the names explicit. |
| D3 | `-fsigned-char` is kept. | The code relies on a signed `char` in places that are hard to find exhaustively (S1 found module visibilities); changing types is S3 work, with module-level tests. The flag is supported by GCC and Clang. |
| D4 | Generated parser tables (macc output) are exempt from `-Wmissing-field-initializers` only (per-file option). | The tables are generated; the generator is not modernised in S2. |
| D5 | The sanitizer build is a CMake option `ELAN_SANITIZE=ON` and a `make check-sanitize` target (separate build dir `build-san/`). | Normal builds stay unaffected; CI runs it on Linux (ASan on macOS arm64 also works locally). |
| D6 | Each sanitizer report is fixed at its root cause. If a fix changes an output, the snapshot and baseline are refreshed in a commit of its own, with a case in `tests/regression/`. | CONTRIBUTING rules 3–4. |
| D7 | The ATerm-related code (`aterm.cc`, `--export`/`--import`) is **kept** in S2. | The user noted that ATerms were an unused experiment; but `--export` produces the REF files the compiler (REM) consumes, and the `A` tests use them. Whether to drop it is decided in S3 with the module split. |
| D8 | The C files keep their K&R style where valid C17; only what `-std=c17 -Wall -Wextra -Werror` rejects is changed. | Minimal diff; reading the 1990s code stays easy. |

## 4. Out of scope

Module split, replacing fixed-size tables, the custom allocator and `macc`
(S3); the library and examples (S4); the compiler (S5).
