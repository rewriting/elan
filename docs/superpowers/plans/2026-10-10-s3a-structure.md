# S3a — Structure: Implementation Plan

> **For agentic workers:** inline execution (superpowers:executing-plans); heavy mechanical tasks may be delegated to a subagent and verified by the controller.

**Goal:** module directories with enforced dependencies, dead code removed, C codegen isolated, header cycle broken, core library + unit tests — behaviour unchanged.

**Spec:** `docs/superpowers/specs/2026-10-10-s3a-structure-design.md`

## Global Constraints

- Branch `s3a-structure`. Every commit: `make check` (Clang) 0/0; before merge also GCC (`make BUILD=build-gcc ELAN_CC=gcc-16 ELAN_CXX=g++-16 PREFIX=$PWD/build-gcc/install check`) and `make check-sanitize` 0/0. macOS only (CI on hold, user's instruction).
- `git mv` for moves; no renames of classes/functions; generated parser sources must stay byte-identical (compare `build/src/interpreter/generated` before/after when touching the generator inputs or `gen-parsers.sh`).
- Commit messages: behaviour change line + `Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>`.

## Review Focus

1. Moving files must not change which definitions are linked (e.g. two `mallo.cc`/`tools` with the same symbol names) → `nm` of the elan binary: same set of defined symbols before/after Tasks 3–4 (except deleted dead code).
2. Removing "dead" code must be proven dead (no reference left, link succeeds, bench 0/0).
3. The architecture test must fail on a real violation (test with a deliberately bad include).
4. Unit tests must exercise real code (link `elan_core`), not copies.
5. `make check` must still fail on regressions/snapshot diffs (unchanged runner).

---

### Task 1: Remove dead code (D6)
- [ ] `strat`, `Any`, `Slist` classes (strategy.h ~120–230, strategy.cc ~38–206): delete; build (Clang+GCC) — a remaining use is a compile error.
- [ ] mallo.cc chunk allocator (`init_alloc`, `allocator`, `intern_alloc`, `intern_free` and their statics): delete; keep `allo`/`fre`.
- [ ] gen-parsers.sh: stop generating `rtabofident.cc`/`rcommtokens.h` if unused by the linked parsers (check `reduceparser.cc` includes `rcommtokens.h`: it does → keep `rcommtokens.h`, drop only `rtabofident.cc`); other generated files byte-identical.
- [ ] gccmatch.c: delete the mangled-name shims if no symbol references them (`nm` of elan).
- [ ] `make check` 0/0; commit "src: remove dead code (behaviour change: none)".

### Task 2: Isolate C code generation (D7)
- [ ] Move `term::termtype`, `isofbuiltintype` (termcompile.cc) into term.cc (or a new `term/termtype.cc`).
- [ ] Move `strategy::compile` and the `trsystem::compile*`/`gen*` blocks (trsystem.cc ~478–764, ~1638–2090) into `trsystem_compile.cc`; declarations unchanged.
- [ ] Build, `make check` 0/0; commit.

### Task 3: Module directories (D1, D3, D4)
- [ ] Create `src/interpreter/{base,lex,parse,term,match,rewrite,load,meta,ref,peval,command,compile,driver}`; `git mv` each file per the spec mapping (aterm.cc → ref/ref.cc; grammar inputs *.t, include.parser, *parser.h, toaterm, aterm.* → parse/grammars/; mtok*.cc → parse/tools/).
- [ ] CMake: `elan_core` static library (all module sources + generated parsers, except `driver/main.cc`), `elan` = `driver/main.cc` + `elan_core`; include directories = all module dirs (transitional). `main` → `elan_main` in ldmain (driver/ldmain.cc); `driver/main.cc` calls it.
- [ ] Generated sources byte-identical; `nm -U` defined symbols of elan identical except `main`/`elan_main`; `make check` 0/0; commit.

### Task 4: Headers (spec goal 2)
- [ ] Break the cycle `rtdatas.h` ↔ `compiledefs.h` (forward declarations; compiledefs.h only included by compile/ and where its types are really used).
- [ ] Split `commondefs.h` into `base/streams.h` (ichstream/ochstream + extern streams), `lex/lexem.h`, `lex/lstream.h`, `parse/grammar.h` (sgrammrule, grammar, Earley structs), `base/alloc.h` (NNEW/AALLOS/CFRE…); move runtime prototypes it declares to `rewrite/` headers; `commondefs.h` becomes an umbrella including them (removed where not needed).
- [ ] Build GCC+Clang, `make check` 0/0; commit per step.

### Task 5: Architecture test (D2)
- [ ] `tests/architecture/check_deps.py`: maps each source/header to its module by directory, parses `#include "..."`, resolves to a file, fails if the edge is not allowed by D2 and not listed in `allowed-exceptions.txt` (`from-file -> to-header  # reason`). Unit tests (stdlib unittest) first: allowed edge passes, forbidden edge fails, listed exception passes, unknown header ignored.
- [ ] Generate the initial exceptions list from the current tree; give each a reason; `make check` runs the script; verify a deliberately bad include fails.
- [ ] Commit.

### Task 6: Unit tests (D5)
- [ ] `tests/unit/check.h` (CHECK/CHECK_EQ macros, failure count, exit status); CMake: one executable per `tests/unit/test_*.cc`, linked with `elan_core`, registered with CTest; `make check` runs `ctest --output-on-failure` in `$(BUILD)`.
- [ ] First tests (RED first by asserting expected values, then they pass against real code): `stringtab` (addstr/member/index/removestr), `mitab`, `lexem` encodings (crcharlex/cridlex/crnumlex/isendofstream/alfsy), `lstream` lexing of a small string, `term` construction + printing of a constant via a minimal symbol table if feasible without loading modules (else defer and say so).
- [ ] Commit; README/CONTRIBUTING: module map, dependency rules, how to add a unit test.
