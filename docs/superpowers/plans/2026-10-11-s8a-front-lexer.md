# S8a — Tables, lexer, macro preprocessor, LR driver (`front/...`, `elan-lextrace`): Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking. Delegated tasks, verified by the controller; one whole-branch review at the end.

**Goal:** the first layer of the Go front end of S8: the interpreter's hash tables (`front/tables`), its lexer (`front/lex`), its macro preprocessor without evaluation (`front/macro`: every form of spec §2.2; `FOR EACH`/`IF` headers and bodies recognised, their evaluation delegated to an evaluator interface that S8d implements), the LR(1) driver with the MACC tables converted from the build's generated C by a generator (`front/lr`, tables checked in), and a loader skeleton (`front/load`: the `.lgi` reader, module file lookup, module headers and import sections, the import graph). Checked against a C++ harness `elan-lextrace` built on `elan_core` (the lexem stream of a file, or of every module a program loads, `FOR EACH` regions marked) and against the "handling module" trace that `elan` prints without `-b` (spec S8, §8 S8a).

**Architecture:** a port, function by function (S8-3), with the C++ names in comments. `front/tables` holds `stringtab`/`mitab` and the static contents of `tabofident`, `mrwt`, `typet`, `import`, `builtinmodules`. `front/lex` holds the lexem encoding, `ichstream` (positions, library lookup S8-14) and `lstream` (the lexer). `front/macro` holds `mlstream` (macros, action stack, `collident`, the iteration forms, constant expressions through the `ppexgram` LR tables, the `FOR EACH`/`IF` headers) and calls an `Evaluator` at the five C++ call sites that need the loader (`addvars`, `addvarrule`, the strategy lookup, `earleycall`, `addforeach`); S8a ships two evaluators: `Refuse` (an error) and `Mark` (a condition is consumed lexically exactly as Earley consumes it, zero solutions, `@foreach` markers in a trace). `front/lr` is `include.parser` in Go over generated tables. `front/load` runs `ldmodgram` on the `.lgi` with the `ldsemact` actions that name, import and open things (the terms the C++ reads with Earley are consumed lexically by the same stop predicates), and reads each module with `modgram` up to the end of its import section (module parameters become macros, imports load depth first), then *drains* the rest of the module text through the preprocessor: the body is S8b's. A command `front-trace` (under `front/cmd`) prints the same traces as the harness.

**Tech Stack:** Go >= 1.24 (standard library only, offline: `GOTOOLCHAIN=local GOFLAGS=-mod=mod GOPROXY=off GOWORK=off`), C++17 (the harness, linked with `elan_core`), CMake, POSIX sh and Python 3 test scripts.

**Spec:** `docs/superpowers/specs/2026-10-11-s8-go-front-end-design.md` (§2.2 forms, §2.4 numbering, §4 packages, §8 S8a; decisions S8-2, S8-3, S8-4, S8-9, S8-10, S8-11, S8-14; §10 D2), `docs/ref-format.md` (tables), the C++ sources `src/interpreter/{base,lex,parse/grammars}`, `load/msemact.cc` (header and import actions), `driver/ldmain.cc` (`ldsemact`, options, library paths).

## Global Constraints

- Branch `s8a-front-lex`, from `main` (d9c73a8 or later). **S8a does not depend on S7e nor on S8c** and must merge in any order with them: it adds packages under `src/compiler-go/front/` only (no change to `go.mod`, no import of `elan/elanrt`, no `cmd/elan-go`), the harness (`tests/front/lextrace/`, a target in `src/interpreter/CMakeLists.txt`, `#ifdef ELAN_LEXTRACE` hooks in `src/interpreter/lex/mlstream.cc`), test scripts under `tests/front/`, and a `check-front` target in the `Makefile` (the only file S7e/S8c also touch: one target, one word in `check:`; resolve by hand on rebase). It may import `elan/ref` (on `main` since S7a) in tests only.
- **`elan` does not change** (S8-11): the hooks are compiled only into `elan-lextrace` (its own compilation of `mlstream.cc` with `-DELAN_LEXTRACE`); `tests/front/check_lextrace_hooks.sh` proves that `mlstream.cc` preprocessed without the macro is token-identical to `main`'s, and `make check-golden` stays green.
- **The oracle is the C++.** Standalone streams compared exactly (lexem codes, i.e. hash positions, included), stderr and exit status included; loader streams and trees compared exactly except the three exclusions of finding 6, each counted and printed by the comparison script. A difference is a failure; it may be kept only when listed in `tests/front/differs.tsv` with its reason and approved by the user case by case (S8-16). None is planned.
- A port: same limits (`MAXNOFMAC` 50, `MAXNOFVAR` 49, `MAXDEEPCONSTEXP` 10, `IDLEN` 50, `STRLEN` 1000, `MAXNOFSTRING` 10 000, table sizes), same error texts, streams and statuses, same order of side effects on the tables (an identifier is inserted when the C++ inserts it, read-ahead included). C quirks are kept and tested (finding 4), never "fixed".
- Go: standard library only, offline; `go vet ./front/... && go test ./front/...` (module `elan`, from `src/compiler-go`) green at every commit; `make check-unit check-golden` green at every commit that touches C++ or CMake; full `make check` at Tasks 9 and 10.
- Nothing in `legacy/` changes (it is read: bench programs); never read credential files (`Sophia/Admin/passwd-def*`).
- Disk: the worktree builds in its own `build/`; work directories `tests/front/work/` (git-ignored by `tests/front/.gitignore`), removed on success; a full loader run writes ~2 M trace lines per side (~60 MB), streamed and deleted per program. Keep the volume's free space above 5 GiB.
- Commit messages end with `Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>`. No push, no merge without the controller.

## Review Focus

1. **`elan` unchanged.** The hooks are pure output under `#ifdef ELAN_LEXTRACE` and a runtime switch; no member is added to `mlstream` (its layout is shared with code compiled without the macro); `check_lextrace_hooks.sh` and the goldens pass; the harness cannot silently link the archive's `mlstream.o` (it references `lextrace_out`, defined only in its own object).
2. **Order of side effects.** `tabofident` insertions in the C++ order: read-ahead of one raw lexem in `lstream` and of `mflex` in `mlstream`, the seven reserved words of the preprocessor inserted by every `mlstream` constructor after the base constructor's first read, `collident` joined names, macro parameter lexems (action 188), spec part names (`RWmtfin.cridlex`), `stringconstants` numbering. The standalone traces compare positions: one misplaced insertion shows as a code difference.
3. **Faithful quirks** (finding 4) each covered by a unit test naming the C++ line.
4. **Lexical consumption equals the C++ consumption.** Where the C++ calls Earley (spec parts, `start with`, `check with`, `FOR EACH` conditions), S8a consumes `do { ilex } while (!stop(fulex))` (earley.cc 533-566: at least one lexem, stop lexem left in the stream); the module reader suspends the LR parse only at the first action of a module body (finding 5) and fails loudly on any other unexpected action code.
5. **The comparison cannot hide a difference.** `tracecmp.py` removes only: lexems strictly inside `@foreach-begin`/`@foreach-end` on the C++ side, `@begin strconc.eln` subtrees (C++ side), and the position/index fields in loader mode; `test_tracecmp.py` shows that any other difference (a lexem, a marker, a begin/end, an extra Go subtree, stderr, status) fails.
6. **Independence** (Global Constraints): `git diff main --stat` touches only the listed paths.

## Investigation findings (2026-10-11)

Read at `s8-spec` 2dca657 (= `main` d9c73a8 + the S8 docs). The interpreter was built in the s8 worktree (`build/`, `-DELAN_COMPILER=OFF -DELAN_RUST=OFF -DELAN_GO=OFF`), and a prototype of the harness (the hooks of Task 1 applied to a scratch copy of `mlstream.cc`, linked with `libelan_core.a`) was run on the whole corpus; nothing was committed.

### 1. Sizes

| part | lines | content |
|---|---|---|
| `lex/lstream.cc` | 385 | lexer: comments `//`, `/* */`, inlines `/# #/`, strings, back-quoted chars, `\n`/`\t` identifiers, identifiers, numbers |
| `lex/mlstream.cc` | 820 | preprocessor, `ppexsemact` (constant expressions) |
| `lex/lbuffer.cc`, `lexem.h`, `lstream.h`, `mlstream.h` | 174 + 162 + 123 + 134 | lexem buffers (chunks of 10), encoding |
| `base/stringtab.cc`, `mitab.cc`, `ichstream.cc` | 174 + 166 + 180 | hash tables, character stream, library lookup |
| `parse/grammars/include.parser` | 107 | the LR(1) driver (`PARSER`, `syerror`) |
| generated `mparser.cc` | 1 521 | `modgram`: 646 states, 2 728 actions, 1 141 gotos, 151 semantic codes < 700 |
| generated `ldparser.cc` | 356 | `ldmodgram`: 90 states, 186 actions, 82 gotos, codes 1-24 (not 8) |
| generated `ppexparser.cc` | 344 | `ppexgram`: 50 states, 203 actions, 24 gotos, codes 1-16 |
| generated `tabofident.cc`, `commtokens.h` | 25 + 88 | 90 reserved words (80 distinct: 10 are in two grammars) preloaded into `tabofident` (size 3 000) and `mrwt` (`mitab`, 300; occurrence counts 1 or 2); 84 `#define RW<word> (-pos+BOFIDENT)` |

Each array ends with a sentinel (`{0,shift,{0}}`, `{0,0}`). No shift/reduce or reduce/reduce entry in the three tables; a `DEFAULT` entry, when a state has one, is always its first (330, 30 and 8 states), which is what `PARSER` tests (`pp->sym == DEFAULT`). Codes >= 700 are MACC's numbers for rules without `code` (and 10 048-10 380 for the keyword-as-symbol rules of `sssymbol`); `semact` passes n < 300 to `semact1`, the rest to `semact3`, whose `switch`es ignore the codes they do not list. All three parsers map lexems with `lextomodlex` over the one `mrwt` (so every reserved word of the three grammars is reserved everywhere); `ppexgram` reads with `pilex` (no `macroexp`, no `collident`) and maps with `cexlextolex`; `errcommon` reads with the virtual `ilex`.

Corpus (`src/lib`, `examples`, `tests`, without `work/`): 217 `.eln`, 373 `.lgi`, 29 `.spc` = 619 files. Forms (comments stripped): module parameters 80 files, `{...}_I=A...B` 12, `X_A,...,X_B` 10, `~` 4, `FOR EACH` 18, `IF` 2 (`ref/ref_read.eln`, `ref/ref_write.eln`), inline `/# #/` 0, bytes >= 0x80 3 (`programs/normalise/latin1*`: in strings only).

### 2. The lexer (`lstream`, `ichstream`)

- `.lgi` and `.spc` files are read by a plain `lstream` (`_` is part of an identifier: `UNDERID`); modules by `mlstream`, whose base `lstream` is `NONUNDERID` (`a_b` is `a` `_` `b`, joined again by `collident`).
- `ichstream` reads one character ahead (`fchar`); `line`/`pos` count the look-ahead character (`commonopen` sets `fchar=1` then reads); error positions `file=F\tline=L\tpos=P` are these counters. `blankskip` skips bytes <= ' '. No `setlocale`: `isalpha`/`isdigit` are ASCII.
- Lookup (`ichstream(const char*)`, S8-14): the name as given with plain `fopen` (case-insensitive file systems accepted), then `perslib+name`, `elanlibcommon+name`, `elanlibqnq+name`, `elanlibstrat+name`, `elanlibref+name`, each with `fopen_exact` (exact case). The prefixes are string concatenations (ldmain.cc 1038-1047): `elanlib` = `$ELANLIB` or `PREFIX`, plus `/` (`--elanlib D` gives `D/`), `share/elanlib/{common,noquote|quote,strategy,ref}/`; `perslib` = `-l`/`--secondlib`, else `$SECONDELANLIB`, else `..`, plus `/`. No path cleaning (a double `/` stays, and shows in error positions). Not found: `[error] can't open input file 'NAME'\n\tfatal\n`, status 1.
- Lexems (`base/constants.h`): number >= 0; char `-c` (1..255); `NOLEXEM` -256, `BLANKLEXEM` -257, `IDENT` -259, identifier `-260-pos`, `STRING` -3260, string constant `-3261-i` (i = order of creation, global, at most 10 000), `JUSTNUMBER` -13261, sort `-13262-n`. `elanrt/earley.Lexem` has the same values (S8b unifies the types; S8a defines its own to stay independent).

### 3. The preprocessor (`mlstream`)

- Stream order (mlstream.h): `mflex`, then the action stack top-down (`currentb`, `actionbuf`, `lastlexem` of each), then the raw lexer. Actions: `SIMPLECOPYACTION` (a macro body, a re-read buffer), `SIMITERACTION` (`~`), `INCRITERACTION` (`{}_I=A...B`: the macro I redefined as a number each turn, deleted at the end), `IDITERACTION` (`X_A,...,X_B`), `NEXTMATCHACTION` (`FOR EACH`/`IF`).
- File-static state shared by all streams: `vartypetab`, `varnames`, `vartab`, `wasaffected`, `vartabi`, `varnamesi` (nested `FOR EACH` offsets `bvarnamesi`), `cexpresult`, and `ppexsemact`'s `evalstack`/`evalstacki` (never reset between expressions). Go keeps them in one `macro.State` per load.
- The macro table is a `mitab` of identifier positions with occurrence counts: a redefinition warns (`[mlstream] redefinition of macro X`, when warnings are on) and increments the count; `deletemac` decrements and clears the body, so a macro deleted once after two definitions stays a member with an empty body (`[preprocessor] non defined macro 'X' used`).
- `FOR EACH` header (macroexp, mlstream.cc 555-685): variables and sorts (`parsetype` builds `s[a,b]` names), `addvars` checks the sort in `typet` (`[preprocessor] unknown type T used`), `addvarrule` for each variable (`actgram()`), `SUCH`/`THAT`, then per condition `V := (S) t` or `V := () t`, `AND IF c`, `ANDIF c`, `IF c`: `S` looked up with `trrules.strategyindex_refs(attach_type_mod(S, sort, impmoduli))`, `t` parsed by `earleycall(this, sort, pppendofin)` (stop at end of stream, `:`, `AND`, `ANDIF`, `IF`); the non-affected check; `deleterule`s; `:`; the body by `fillmacbuf`; `addforeach`. The first four and the last need the loader: they are the evaluator's.
- Constant expressions (`constexpexp`): a number, or `(` + `ppexsyntan(*this, 0, cexlextolex)` + `)`; identifiers compare by `tabofident` position (code 16).

### 4. Quirks to keep (each gets a test)

1. Numbers wrap modulo 2^32 into a C `int` (lstream.cc 247-256): `4294967295` is lexem -1, i.e. the character 1.
2. Identifiers keep their first 49 characters (`ide[i>=(IDLEN-1)?(IDLEN-1):i++]`, then `ide[i]=0`).
3. A string constant of 1 000 bytes or more: `\nstring constant too long\n`, status 1; only `\n`, `\t`, `\\`, `\"` are escapes.
4. `\n` and `\t` outside strings are the identifiers "newline" and "tab" (BORO's hack).
5. The build uses `-fsigned-char` on every platform: the `stringtab` hash is the sum of *signed* chars modulo the size (C `%`), probing by +211; Go sums `int8(b)`.
6. Errors inside `beforemess` nest: `beforemess` reads 7 lexems with `ilex`, which can fail again (`common/array.eln` standalone prints two `[preprocessor] constant or '(' expected` before exiting).
7. `X_A,...,X_B` with a separator other than `,` (any character lexem) and `{...}_I=A...B` with `B < A` (no turn; I defined then deleted).
8. `mitab` occurrence counts (§3).

### 5. Loading: what S8a must do to reach every module

- `elan_main` → `ldsyntan` on the `.lgi` (plain `lstream`) with `ldsemact`: 1/5 (`handleaxiom`: the previous spec part's body read by Earley from the `.spc` stream up to the next part name, `RWmtfin.cridlex(finalsym)`), 4 (`importmod_inf(name, lgimoduli, RGLOP)`), 6 (`importmod_inf("bool")`), 10 (`start with (S) query`: the term read up to `end`, `qendofin`), 11 (`check with`, up to `start`), 12, 13, 14 (`start with [S] query`: `load_strat_mod` → `strat[X]` or `tcstrat[X,Y]`, the strategy read up to `end`/`]`, `]` skipped, then the term), 15 (open the `.spc`, `handheadofspec`), 16-18/23/24 (`mkmodname*`), 19, 20, 21, 22. The `.lgi` itself is `import.addstr(prog.lgi)`.
- `importmod_inf` → `readmodules` (msemact.cc 878): `divideonargs` (name and actual arguments at bracket depth 1), file `<name>.eln` through the lookup, `mlstream`, `msyntan` with `semact`. Module actions met before the body: 0 (begin: `handling module N`, indented `stacki<<1`+1 spaces by `odsek`, unless `-b`), 169 (name check: `[semact] name of module doesn't correspond with the name of file` + `ERRORIM`), 188 (each formal parameter becomes a macro: a numeric argument as a number lexem, else the argument split at `[ ] , < - > |` into identifier and character lexems; `number of formal and actual parameters for module M doesn't correspond`), 189 (count check), 202, 140, 141, 185-187, 172, 173 (`mkmodname*`), 8 (import: depth first, once per name; cycle: `there is probably a cycle in imports of module M` warning), 201 (end of imports: `fsymtab_remakealias`, S8b). The first body action is one of 28/29 (sort), 335 (`operators`), 336 (`stratop`), 150 (`rules`), 360 (`strategies`); a module with nothing after its imports reaches 144 (`end of N`) directly. **No module is loaded from a module body except** `strconc[A,B,C]` (`trclos` after a `strat[...]`/`tcstrat[...]` module, from the inline codes of its strategies), `explimpl<n>` (`any[X]`) and `symbappl<n>` (`Symbol`).
- Measured with `elan --export /dev/null prog.lgi [spec]` (no `-b`) on the 555 program runs of finding 7: 4 137 `handling module` lines; 101 `strconc[...]` (one per `strat[X]`, always `strconc[X,X,X]`, whose imports `strsig[X,X]` are already loaded), 0 `explimpl`, 0 `symbappl`, 0 `tcstrat`. So the S8a tree equals the C++ tree minus the `strconc` subtrees.
- The stream the C++ consumes equals the stream drained to its end: checked with the prototype on `bool`, `int`, `builtinInt`, `identifier`, `substitution` (consumed by LR + Earley in loader mode = standalone stream, 28-1 121 lexems each).

### 6. The harness, prototyped

Hooks (all in `mlstream.cc`, ~40 lines): `@begin NAME` in the constructor before its priming `ilex`, which is not traced; `@end` in the destructor; in `mlstream::ilex` a global depth counter: a lexem is written only when the outermost call returns (calls from `macroexp`, `FOR EACH` headers and their Earley parses are nested, so not written), followed by the markers that became pending during that call; `@foreach-begin` pending just before `addforeach`, `@foreach-end` when `NEXTMATCHACTION` has no more solution (buffilex). Harness `lex FILE`: a `.eln` through `mlstream` (written by the hook), a `.lgi`/`.spc` through `lstream` (written by the harness with the same function), `eos` last. Harness `load TRACE ELAN-ARGS...`: `elan_main` with the hooks writing to TRACE. Results:
- links with `libelan_core.a` (the harness's `mlstream.o` takes precedence; the archive member is not extracted);
- `lex` on the 619 files: 594 streams, 4 errors (`common/array.eln`, `common/tuple.eln`, `strategy/symbol.eln`, `strategy/carray.eln`: a module parameter used in a constant expression is not defined standalone), 21 files not runnable standalone (`FOR EACH`/`IF` call `actgram()`, `typet`, Earley); 66 630 lines;
- `load` on `examples/unification`: 8 717 lines, 32 module streams, 10 `FOR EACH` regions; 0.01 s; on 555 runs (finding 7): 1 868 591 lines, 415 `FOR EACH` regions, slowest 0.07 s (`HOU_1_18`);
- `ref_read.eln`/`ref_write.eln` (the only `IF` files) cannot be loaded by `elan` (`io.eln` missing from the library): their form gets a C++ oracle through a fixture module (Task 2).

### 7. Program corpus (criterion 1 of the spec, loading only)

`list_j_programs.py`: 175 runs (145 export, 30 `ans` programs refused by the interpreter); golden `ref` cases 28; `examples` 13; `.lgi` of `tests/compiler-rs/programs` and `tests/compiler/programs` 334; BenchThesis 22 cases (`test_benchthesis.sh`: FastCompletion `ans` × 15 specs, MinelaComp `rewritingC` × 7). Programs `elan` rejects (status != 0) are excluded from the loader comparison in S8a (their first error is in a module body, S8b) and listed as `NOLOAD`. S7e adds `tests/compiler-rs/programs/{io,arrays,query}`: taken automatically after rebase.

## File structure

| file | role |
|---|---|
| `src/interpreter/lex/mlstream.cc` | `#ifdef ELAN_LEXTRACE` hooks (Task 1) |
| `src/interpreter/CMakeLists.txt` | target `elan-lextrace` (not installed) |
| `tests/front/lextrace/elan-lextrace.cc` | the harness: `lex FILE`, `load TRACE ARGS...` |
| `tests/front/check_lextrace_hooks.sh` | `mlstream.cc` without the macro = `main`'s (preprocessed) |
| `tests/front/fixtures/` | small `.eln`/`.lgi`/`.spc` files, one per form and quirk, with `<f>.expected` (harness output: stdout, stderr, status) |
| `tests/front/programs/` | fixture programs that `elan` loads: `ifparam` (an `IF` on a numeric parameter), `params` (`tuple[3,...]`, `X_1,...,X_N`, `{}_I`), `foreach` (nested `FOR EACH`), `stratlgi` (`start with [S]`), `cycle` (import cycle warning) |
| `tests/front/list_files.py`, `list_programs.py` | the corpora (finding 1, 7) |
| `tests/front/tracecmp.py`, `test_tracecmp.py` | comparison and its tests |
| `tests/front/test_front_lex.sh`, `test_front_load.sh`, `test_front_errors.sh` | the acceptance scripts |
| `tests/front/errors/{lex,load}/` | error corpus (expected outputs by the harness / `elan -b`) |
| `tests/front/differs.tsv`, `README.md`, `.gitignore` | approved differences (empty), how to run and read |
| `src/compiler-go/front/diag/diag.go` | stderr/stdout, warnings flag, `failexit` as a panic, `Run` |
| `src/compiler-go/front/tables/{stringtab,mitab,initial}.go`, `keywords_gen.go` | tables (Task 3), generated reserved words (Task 4) |
| `src/compiler-go/front/lr/gen/main.go` (+ `gen_test.go`) | the table generator |
| `src/compiler-go/front/lr/{lr,tables_gen}.go` | the driver, generated tables |
| `src/compiler-go/front/lex/{lexem,ichstream,lstream,lbuffer}.go` | lexer |
| `src/compiler-go/front/macro/{mlstream,iter,foreach,ppex,eval,trace}.go` | preprocessor |
| `src/compiler-go/front/load/{load,lgi,module,names}.go` | loader skeleton |
| `src/compiler-go/front/cmd/front-trace/main.go` | `front-trace lex FILE`, `front-trace load [opts] prog.lgi [spec]` |
| `Makefile` | `check-front` (in `check` when Go >= 1.24 is built), `front-tables` (regenerate) |

## Tasks

### Task 1: The harness `elan-lextrace`

**Files:** modify `src/interpreter/lex/mlstream.cc`, `src/interpreter/CMakeLists.txt`; create `tests/front/lextrace/elan-lextrace.cc`, `tests/front/check_lextrace_hooks.sh`, `tests/front/fixtures/*`, `tests/front/test_lextrace_harness.sh`, `tests/front/.gitignore`.

**Interfaces:**
- Hooks, all under `#ifdef ELAN_LEXTRACE`, no member added to any class: `FILE *lextrace_out` (NULL: off); `void lextrace_lexem(FILE *, lexem)` writes one line: `id <pos> <text>`, `num <n>`, `char <c>` (decimal), `str <i> <text>`, `eos`, `other <raw>`; texts escaped (`\\`, `\n`, `\t`, `\"`, bytes < 32 or >= 127 as `\ooo`); the placements of finding 6. The trace stream is flushed by an `atexit` handler (`failexit` exits without unwinding).
- `elan-lextrace lex FILE`: as finding 6; inline buffers: after each lexem the harness calls `getlastinlineAndinit()` and writes `@inline <escaped chars>` when one was read. Exit status and stderr those of the C++ (fatal errors: status 1).
- `elan-lextrace load TRACE [elan options] prog.lgi [spec]`: `elan_main` with `argv[0]` kept; the scripts pass `--export /dev/null -b`.
- CMake: `add_executable(elan-lextrace ${PROJECT_SOURCE_DIR}/tests/front/lextrace/elan-lextrace.cc lex/mlstream.cc)`, `elan_cxx_target`, `ELAN_LEXTRACE` defined, the generated directory included, linked with `elan_core`; built by `make`, not installed (`build/src/interpreter/elan-lextrace`).

- [ ] **Step 1: Failing tests.** `check_lextrace_hooks.sh`: `c++ -E -P` of `mlstream.cc` from `git show main:...` and of the working copy, both without `ELAN_LEXTRACE` and with the include directories of the build, compared — Expected now: PASS (no hook yet), it is the guard. `test_lextrace_harness.sh BUILD`: runs the harness on `tests/front/fixtures/*` and compares with the `.expected` files written by hand for the five smallest fixtures (`ids.lgi`: `LPL a_b description end` → `id <pos> LPL`, `id <pos> a_b`...; `under.eln`: `module m x_y end` → `x` `_` `y` joined to `id x_y`; `num.eln`: `4294967295` → `char 1` (quirk 1); `str.eln`; `macro.eln` with `{ a }~3` → `id a` three times) and checks for `examples/unification` in loader mode: status 0, balanced `@begin`/`@end`, 32 `@begin`, 10 `@foreach-begin`, every `@foreach-begin` followed by its `@foreach-end` — Expected: FAIL (no harness).
- [ ] **Step 2: Implement** the hooks (as the prototype of finding 6), the harness, the target.
- [ ] **Step 3: Run** `make && tests/front/test_lextrace_harness.sh build && tests/front/check_lextrace_hooks.sh && make check-unit check-golden` — Expected: PASS; generate the `.expected` of the other fixtures with the harness and read each one (they are the oracle of Tasks 5-7).
- [ ] **Step 4: Commit** (`tests: elan-lextrace, the lexem-stream harness of the Go front end (S8a task 1)`).

### Task 2: Corpora, comparison, acceptance scripts (RED)

**Files:** create `tests/front/{list_files.py,list_programs.py,tracecmp.py,test_tracecmp.py,test_front_lex.sh,test_front_load.sh,differs.tsv,README.md}`, `tests/front/programs/*`, `src/compiler-go/front/cmd/front-trace/main.go` (stub: prints `front-trace: not implemented (S8a)` on stderr, status 3).

**Interfaces:**
- `list_files.py REPO`: the 619 files, one per line, with a column `standalone|loaded`: `loaded` for a `.eln` containing `FOR EACH` or `IF` (comments stripped).
- `list_programs.py REPO`: `kind\tdir\tlgi\tspc|-` for the corpus of finding 7 plus `tests/front/programs`, deduplicated by (dir, lgi, spc).
- `tracecmp.py --mode lex|load C GO`: lex mode: equal lines; load mode: C++ side lexems inside `@foreach` regions (outermost) and `@begin strconc.eln` subtrees removed, positions dropped from `id`/`str` lines on both sides; on a difference, the first one with 5 lines of context and the module stack (`@begin` names) at that point; on success a summary line `foreach=<n> strconc=<n>`. Status 0/1.
- `test_front_lex.sh [PREFIX]`: for each `standalone` file, harness and `front-trace lex` (built by the script into `tests/front/work/`, as `test_ref_roundtrip.sh` builds its reader) in the file's directory; stdout by `tracecmp.py --mode lex`, stderr with `cmp`, statuses equal; summary `N files: ok, failed`; `loaded` files counted as covered only if their name appears in a loader trace of `test_front_load.sh` (it writes `work/loaded-files.txt`), else listed as `UNCOVERED` and failing.
- `test_front_load.sh PREFIX`: for each program, in a private copy of its directory: `elan --export /dev/null ARGS` (no `-b`) → the lines `^ +(handling module|end of) ` = the C++ tree, status; `elan-lextrace load` → the C++ trace; `front-trace load --tree --trace F ARGS` → the Go tree and trace; status != 0 for `elan`: `NOLOAD` (reported, not compared); else trees compared after removing `strconc` subtrees (counted), traces by `tracecmp.py --mode load`, statuses equal; `xargs -P`, summary line.
- Fixture programs (`tests/front/programs/`), each loaded by `elan` with status 0: `ifparam` (`module m[verbose]` with `IF verbose == 1 : { ... }` and `ANDIF`, imported as `m[1]` and `m[0]`), `params` (`tuple[3,int,bool,int]` from the library, a local module with `X_1,...,X_N` and `{...}_I=1...N` on a parameter, `~`), `foreach` (nested `FOR EACH` on `list[X]` elements), `stratlgi` (`start with [s] query`, loads `strat[int]`), `cycle` (two modules importing each other: the warning; checked without `-b`).

- [ ] **Step 1: Failing tests.** `test_tracecmp.py` (unit, no ELAN needed): identical traces → 0; a lexem differing, missing, extra; a marker missing; different bodies inside `@foreach` → 0 (load) and 1 (lex); a `strconc` subtree on the C++ side only → 0 (load), on the Go side → 1; positions differing → 0 (load), 1 (lex); an extra `@begin` on the Go side → 1 — Expected: FAIL (no script). `elan` loads each fixture program (`elan -b --export /dev/null`, status 0) — Expected: PASS after writing them.
- [ ] **Step 2: Implement** the listers, `tracecmp.py`, the scripts, the stub command.
- [ ] **Step 3: Run** `python3 tests/front/test_tracecmp.py` — Expected: PASS. `tests/front/test_front_lex.sh build/install` and `test_front_load.sh build/install` — Expected: FAIL on every file/program with `not implemented` (the RED state of S8a); record the counts printed (files, programs, `NOLOAD`).
- [ ] **Step 4: Commit** (`tests: corpora, trace comparison and the S8a acceptance scripts, RED (S8a task 2)`).

### Task 3: `front/diag` and `front/tables`

**Files:** create `front/diag/diag.go`, `diag_test.go`, `front/tables/{stringtab.go,mitab.go,initial.go,tables_test.go}`.

**Interfaces:**
- `diag`: `type D struct { Out, Err io.Writer; Warnings bool }`; `func (d *D) Fail()` (`failexit`: panics `Exit{Status: 1}`); `type Exit struct{ Status int }`; `func Run(f func()) (status int)` (recovers `Exit`, re-panics anything else); `func (d *D) Warn(...)` writes only when `Warnings`.
- `tables`: constants `MaxNofIdent` 3000, `NTypes` 500, `MaxNofImports` 200, `MaxNofTrn` 2000, `MaxNofStrat` 500, `MaxNofMac` 50, `MaxNofVar` 49, `MaxDeepConstExp` 10, `MRWTSize` 300, `IDLen` 50, `StrLen` 1000, `MaxNofString` 10000; `func Hash(s string, size int) int` (signed-char sum, C `%`; a negative result — bytes >= 0x80 — panics with the name: the C++ indexes out of bounds there); `type StringTab` with `NewStringTab(d *diag.D, size int, init ...string)`, `Member(s) bool` (sets `Posid`), `Index(s) int`, `AddStr(s) int` (overflow: `[addstr] ERROR, string tab overflowed over N!!! \t !!!!!!!!!\n` then `Fail`), `Ide(i) string`, `RemoveStr(i)`, `Len() int`, `Each(func(i int, s string))` (position order: the `Adump` order); `type MiTab` with `NewMiTab(d, size, init ...int)` (stops at the first value <= 0, as the C varargs), `Member(n) bool`, `AddN(n) int` (counts; overflow `[addn] ERROR, mi tab overflowed over N\n int.err.\n`), `RemoveNonP(p)` (`[removen] trying to remove non ex. number\nint.err.\n`), `RemoveN(n)`, `Posid`; `NewTypeT(d)` (the 7 sorts of module.cc), `NewImport(d)`, `NewBuiltinModules(d)` (64, the 33 names of module.cc in order). `NewTabOfIdent`/`NewMRWT` come with Task 4.

- [ ] **Step 1: Failing tests.** `Hash("ab",100) == 95`, `"ba"` after `"ab"` at 6 (the C++ unit tests of `tests/unit/test_base_lex.cc`, same values); `AddStr` twice same position; `RemoveStr` then `Member` false; overflow of a `StringTab(4)` after 3 names: the message and `diag.Run` status 1; `MiTab`: add 5 twice, `RemoveN(5)` once → still member, twice → not member, three times → the error; `NewTypeT` positions of `bool`... equal `Hash` (no collision) and `Len()==7`; consistency with the goldens: for each `tests/golden/expected/*.ref` parsed with `elan/ref`, every entry of the five tables (sizes 3000, 500, 200, 2000, 500) is at `Hash(name)` or reachable from it by +211 steps through entries of the same table — Expected: FAIL (undefined).
- [ ] **Step 2: Implement.** **Step 3:** `go vet ./front/... && go test ./front/...` — Expected: PASS.
- [ ] **Step 4: Commit** (`front: diag and tables, the interpreter's hash tables in Go (S8a task 3)`).

### Task 4: The table generator and the converted tables

**Files:** create `front/lr/gen/main.go`, `gen_test.go`, `front/lr/tables_gen.go`, `front/tables/keywords_gen.go`, `front/lr/doc.go` (types only, so the generated file compiles); modify `Makefile` (`front-tables`).

**Interfaces:**
- `go run ./front/lr/gen -in DIR -out .` (from `src/compiler-go`; `DIR` = `build/src/interpreter/generated`) reads `mparser.cc`, `ldparser.cc`, `ppexparser.cc` (arrays `appr`, `apak`, `STTAB`; symbols `RWxxx` resolved through `commtokens.h`, `-'c'`, `IDENTIFIER`, `NUMBER`, `EOFINPUT`, `DEFAULT`; `&appr[k]` offsets) and `tabofident.cc` (the preload list in order, the `mrwt` list), and writes `front/lr/tables_gen.go` (`var ModGram, LdModGram, PpexGram = &Table{...}`, one `Action{Sym, Kind, Num, Left, Len}` per entry, states as offset/length into the arrays, keyword symbols as numbers with the word in a comment) and `front/tables/keywords_gen.go` (`var reservedWords = []string{...}` (90, in order), `var mrwtInit = []int{...}`), both `gofmt`ed, headed `// Code generated by front/lr/gen from <file> (MACC); DO NOT EDIT.`; any construct it does not recognise is an error naming the file and line. `make front-tables` runs it.
- `lr` types (doc.go): `type Kind uint8` (`Reduction` 0, `Accept` 1, `Shift` 2, `SRErr` 3, `RRErr` 4), `const Default = -32000`, `type Action struct{ Sym int32; Kind Kind; Num, Left, Len int32 }`, `type Goto struct{ Sym, To int32 }`, `type State struct{ Pr []Action; Ak []Goto }`, `type Table struct{ Name string; States []State }`.
- `tables`: `NewTabOfIdent(d) *StringTab` (3 000, `reservedWords` inserted in order), `NewMRWT(d) *MiTab`.

- [ ] **Step 1: Failing tests.** `gen_test.go`: with `ELAN_GENERATED_DIR` set, the generator's output for that directory equals the checked-in files byte for byte (skipped with a message naming the variable when unset); on a 3-state excerpt of `ppexparser.cc` in `testdata/`, the expected Go text. `tables_test.go`: for each of the 84 `RW` defines of `commtokens.h` (copied to `testdata/commtokens.h`), `NewTabOfIdent().Index(word) == pos`; `NewMRWT` counts 2 for the 10 duplicated words. `lr` sanity: 646/90/50 states; every shift/goto target in range; in every table a `Default` entry is the first of its state — Expected: FAIL.
- [ ] **Step 2: Implement**, generate, check in.
- [ ] **Step 3: Run** `ELAN_GENERATED_DIR=$PWD/../../build/src/interpreter/generated go test ./front/...` (from `src/compiler-go`) — Expected: PASS (with the variable: the up-to-date test runs).
- [ ] **Step 4: Commit** (`front: the MACC tables converted to Go by front/lr/gen, checked in (S8a task 4)`).

### Task 5: `front/lex`

**Files:** create `front/lex/{lexem.go,ichstream.go,lstream.go,lbuffer.go,lex_test.go}`, `testdata/`; modify `front/cmd/front-trace/main.go` (`lex` for `.lgi`/`.spc`).

**Interfaces:**
- `type Lexem int32`; constants `NoLexem`, `BlankLexem`, `IdentLexem` (-259), `BofIdent`, `StringLexem`, `BofString`, `JustNumber`, `BofTypes`; methods `IsEOS`, `IsBlank`, `IsIdent`, `IsRealID`, `IdVal`, `IsNum`, `IsRealNum`, `NumVal`, `IsChar`, `CharVal`, `IsString`, `IsRealString`, `StrIndex`, `TypeVal`, `Terminal`; `IdLex(pos)`, `NumLex(n)`, `CharLex(c)`, `TypeLex(n)`.
- `type Lib struct{ Pers, Common, QNQ, Strat, Ref string }`; `func NewLib(elanlib string, elanlibOpt string, perslib string, quote bool) Lib` (finding 2: concatenations, defaults `..`; `elanlib` empty = error to the caller: S8a has no compiled-in prefix).
- `type Context struct { D *diag.D; Ident *tables.StringTab; Strings []string; Lib Lib }` (one per load; `Strings` = `stringconstants`); `func (c *Context) Alfsy(l Lexem) string`, `ErrAlfsy`; `func (c *Context) StringLex(s string) Lexem` (`crstringlex`, `too many string constants`).
- `type CharStream`: `Open(c *Context, name string) *CharStream` (lookup; fatal message), `FromBytes(name string, b []byte)` (generated modules, S8-9); `Ich() int`, `Fuch() int`, `BlankSkip()`, `ActLine()`, `ActPos()`, `ActName()`, `SetPosition`. EOF is -1 (`EOFICHSTR`).
- `type Stream` (`lstream`): `New(c *Context, cs *CharStream, underID bool) *Stream` (primes `flex`); `Fulex() Lexem`, `Ilex() Lexem` (raw lexer), `Oerr(parts ...string)` (no part: the position line only), `Owarn(...)`, `BeforeMess(s Lexem)` (7 lexems), `LastInline() []Lexem` (`getlastinlineAndinit`), `Ctx() *Context`. `type Buffer` (`lbuffer`): `Put`, `Get` (empty: `[lbuffer::get] try to get from empty buff, internall error\n`, fatal), `IsEmpty`, `Copy`, `Append`, `Clear`.
- `front-trace lex FILE` for `.lgi`/`.spc`: the harness's format (`TraceLexem(w, ctx, l)` in `lex`, shared with `macro`).

- [ ] **Step 1: Failing tests.** `lex_test.go` with expected lexem sequences: comments of both kinds, comment to end of file (message, status 1, position), inline and two successive inlines (warning only when `Warnings`), strings with the four escapes and an unknown one (`\q` kept as two bytes), 999 and 1000 bytes, back-quoted characters and `` `ab` `` (`wrong string constant`), `\n`/`\t` identifiers, identifier of 60 characters (first 49), `a_b` with and without `underID`, `2147483647`, `2147483648` (lexem -2147483648: `other`), `4294967295` (char 1), a byte 0xE9 outside a string (char 233), positions of an error on line 3 (`line`/`pos` as the C++'s look-ahead counters: values taken from the harness on `fixtures/pos.eln`); lookup: a name found in the current directory, in `../`, in each library directory, a case-only match refused in the library (`Any.eln` vs `any.eln`), not found (message); `NewLib` strings for `ELANLIB=/p`, `/p/`, `--elanlib q`, `-l d`. Acceptance: `tests/front/test_front_lex.sh build/install --only lgi,spc` — Expected: FAIL.
- [ ] **Step 2: Implement.** **Step 3: Run** the unit tests and `test_front_lex.sh build/install --only lgi,spc` — Expected: PASS on the 402 `.lgi`/`.spc` files.
- [ ] **Step 4: Commit** (`front: lex, the interpreter's lexer and character streams in Go (S8a task 5)`).

### Task 6: `front/lr`, the driver

**Files:** create `front/lr/lr.go`, `lr_test.go`.

**Interfaces:**
- `type Source interface { Fulex() lex.Lexem; Ilex() lex.Lexem; Oerr(parts ...string); BeforeMess(lex.Lexem); Ctx() *lex.Context }` (`lex.Stream` and `macro.Stream` implement it; `macro` overrides `Ilex`/`BeforeMess`).
- Semantic return codes `ErrorIm` 1, `AcceptIm` 2, `NormCont` 3, `HandErrorIm` 4, and the Go-only `Suspend` 100 (the loader skeleton stops a module before its body; S8b removes its use).
- `type Result int` (`Rejected` 0, `Accepted` 1, `Suspended` 2); `func Parse(t *Table, f Source, ilexem func() lex.Lexem, ltol func(lex.Lexem) lex.Lexem, sem func(n int, l lex.Lexem) int) Result` — `include.parser` line by line: stack of 1 500 (`[PARSER] stack overflow `), `FFOUND`/`RFOUND` as written (a missing symbol takes the last entry), `SEMACT(0)` first, `SEMACT(-2)` on accept, `syerror` (`[PARSER] syntax error ` + `errcommon` with `f.Ilex()`, `\ton symbol 'X'`, `BeforeMess`, `\tone of ( 'a'  'b' ) expected \n`), the semaction error (`[PARSER] semaction error ` + `errcommon`), the srerr/rrerr message.
- `func LexToModLex(c *lex.Context, mrwt *tables.MiTab) func(lex.Lexem) lex.Lexem` (`lextomodlex`), `func CexLexToLex(l lex.Lexem) lex.Lexem` (`cexlextolex`); `func SymName(c *lex.Context, sym int32) string` (`maccsymtolex` + `alfsy`).

- [ ] **Step 1: Failing tests.** With a slice-backed `Source`: `PpexGram` on `( 1 + 2 * 3 )` gives the reduction codes 2 2 2 5 3 1 in that order and `Accepted`; `( 1 == 1 & 2 > 1 )` the codes of `==`, `>`, `&`; `( 1 + )` → `Rejected` and the message equal to the harness's for the same expression inside `{ a }~(1 + )` (`tests/front/fixtures/ppexerr.eln`, its expected stderr); `ModGram` on `module m import a b[c, d]; end end` (lexems through `LexToModLex`): the sequence of codes < 700 recorded by the semantics is 0, 169, 189, 202, 185, 8, 185, 185, 185, 187, 186, 8, 201, 144, -2 (derived from `modgram.t`; a difference is first checked against the grammar, not edited into the test); `LdModGram` on `LPL p description query of sort s result of sort s import a start with () end end`, with a semantics that consumes the term lexically at code 10: `Accepted`; a semantics returning `Suspend` → `Suspended` and no further read; `ErrorIm` → `Rejected` and the semaction message — Expected: FAIL.
- [ ] **Step 2: Implement.** **Step 3:** `go test ./front/...` — Expected: PASS.
- [ ] **Step 4: Commit** (`front: lr, the LR(1) driver of the interpreter's grammars (S8a task 6)`).

### Task 7: `front/macro`, the preprocessor

**Files:** create `front/macro/{mlstream.go,iter.go,foreach.go,ppex.go,eval.go,trace.go,macro_test.go}`, `testdata/`; modify `front/cmd/front-trace/main.go` (`lex` for `.eln`).

**Interfaces:**
- `type State struct` (the file-static variables of finding 3, the `ppexsemact` stack); `func NewState() *State`.
- `type Stream struct` (embeds `*lex.Stream`, `underID` false): `New(c *lex.Context, st *State, cs *lex.CharStream, name string, ev Evaluator, tr Tracer) *Stream` (constructor order: base priming read, the seven words `FOR`, `EACH`, `SUCH`, `THAT`, `AND`, `ANDIF`, `IF` inserted into `tabofident`, `Tracer.Begin(name)`, priming `Ilex` untraced); `Ilex() lex.Lexem` (traced at depth 0), `Pilex()`, `Fulex()`, `AddMac(name int, body []lex.Lexem)`, `DeleteMac(name int)`, `BeforeMess(lex.Lexem)`, `Close()` (`Tracer.End()`).
- `type Tracer interface { Begin(name string); Lexem(l lex.Lexem); Marker(m string); End() }`; `NewTraceWriter(w io.Writer, c *lex.Context) Tracer` (the harness's format; markers are written after the lexem of the depth-0 call during which they became pending).
- `type Evaluator interface` — one method per C++ call site of finding 3:
  - `AddVars(s *Stream, sort string) bool` (`addvars`: false → the macro package prints `[preprocessor] unknown type <sort> used` + `beforemess`, fatal);
  - `AddVarRules(s *Stream, vars []Var)` (`addvarrule` per variable, `Var{Name lex.Lexem; Sort string; Index int}`);
  - `Strategy(s *Stream, v Var, name lex.Lexem)` (`strategyindex_refs` + `conform_strategies` + `assign_one_ref`);
  - `Term(s *Stream, v *Var /* nil: a boolean condition */, stop func(lex.Lexem) bool) (Term, bool)` (`earleycall`; false → fatal, the evaluator has printed its error);
  - `DeleteVarRules(s *Stream, vars []Var)`;
  - `Solutions(s *Stream, f *ForEach) Solutions` (`addforeach` + `isWhereBacktrackNextSol`), `type Solutions interface { Next() ([][]lex.Lexem, bool) }`, `type ForEach struct { Vars []Var; Conds []Cond; Body []lex.Lexem; If bool }`, `type Cond struct { Var int /* -1: IF/ANDIF */; Strategy lex.Lexem /* 0: () */; Term Term }`, `type Term any`.
  - `Refuse{}`: `AddVars` prints `[preprocessor] FOR EACH and IF are not evaluated before S8d` with the stream position and fails; `Mark{}`: accepts every sort, `Term` consumes `do { s.Ilex() } while (!stop(s.Fulex()))` and returns the lexems, `Solutions` returns none.
- Every form of spec §2.2, the errors of finding 3 with their texts and streams, the limits, quirks 6-8.

- [ ] **Step 1: Failing tests.** `macro_test.go`, expected streams taken from the harness on `tests/front/fixtures/*.eln` (Task 1): a macro defined with `AddMac` then used, redefined (warning), deleted twice after two definitions (`non defined macro`); `a_b` joined, `T_I` re-expanded after joining; `{ x }~3`, `X~(2+1)`; `{ f(I) }_I=1...3`, `_I=3...1` (none), `B` a macro; `x_1,...,x_3`, `x_1;...;x_3`, `x_(1+1),...,x_N`; `(1 == 1 & 2 >= 3)`; 11 nested parentheses (`MAXDEEPCONSTEXP`); missing `}` (`macro construction buffer throw end of file`); `{ a } b` (`syntax error in macro construction`); `FOR EACH x:int SUCH THAT x:=(s) f(1) :{ g(x) }` with `Mark`: the stream around it and the markers, the condition lexems `f ( 1 )` given to `Term` with `stop` true on `:`; nested `FOR EACH`; `IF verbose == 1 : { a }` with a macro `verbose`; `SUCH`/`THAT` missing, a variable not affected, an unknown variable after `SUCH THAT`, 50 variables (`too much variables`); `Refuse`'s message; the 51st macro (`mitab overflowed over 50`). Acceptance: `tests/front/test_front_lex.sh build/install` (all standalone files) — Expected: FAIL.
- [ ] **Step 2: Implement.** **Step 3: Run** the unit tests and `test_front_lex.sh build/install` — Expected: PASS on the 598 standalone files (594 streams, 4 errors identical), the 21 `loaded` files reported as pending Task 8.
- [ ] **Step 4: Commit** (`front: macro, the preprocessor without evaluation (S8a task 7)`).

### Task 8: `front/load`, the loader skeleton; `front-trace load`

**Files:** create `front/load/{load.go,lgi.go,module.go,names.go,load_test.go}`, `testdata/`; modify `front/cmd/front-trace/main.go` (`load`).

**Interfaces:**
- `type Options struct { ElanLib string; ElanLibOpt string; SecondLib string; Quote, Batch, Warnings bool; Out, Err io.Writer; Eval macro.Evaluator; Trace macro.Tracer }`; `func ParseArgs(args []string) (Options, []string, error)` (the subset of `elan`'s options of S8-12: `--elanlib`, `-l`/`--secondlib`, `--quote`/`--noquote`, `-w`, `-q`, `-b` (warnings off), `--export F`/`--cexport F` accepted and ignored in S8a, `--MAXLENNTERM n` accepted); `func Load(o Options, lgi, spec string) int` (status; `diag.Run` around everything).
- `.lgi`: `LdModGram` with `ldsemact`'s codes of finding 5; terms read by Earley in the C++ consumed by `macro.Mark`-style lexical skips with the C++ predicates (`qendofin`, `qchendofin`, `sendofin`, `mtendofin` with `finalsym` inserted by `cridlex`, `axchendofin`); the `.spc` opened at 15 (`handheadofspec` messages); bodies of parts skipped at 1/5 (`handleaxiom`: `name of axiom ... clash with a name of module`, `bad name of specification part`); `load_strat_mod` at 14 (`strat[X]` / `tcstrat[X,Y]` from the canonical sort names).
- Modules: `importmod_inf`/`readmodules` (`import` table, `divideonargs`, `mkmodname1-5`, `stacki`, the cycle warning, `handling module`/`end of` with `odsek` when not `Batch`), `ModGram` with the actions of finding 5; the first body action (28, 29, 335, 336, 150, 360) returns `lr.Suspend`, then the stream is drained (`for !Fulex().IsEOS() { Ilex() }`), `end of N` printed, the stream closed; action 144 reached without a body: same end; any other code below 700 not in the module set: internal error `front/load: action N before the module body (S8b)`, status 70. Not done in S8a, by design: `trclos`/`strconc`, `explimpl`, `symbappl`, visibilities, grammars.
- `front-trace load [--tree] [--trace FILE] [--refuse] [elan options] prog.lgi [spec]`: `Eval` = `Mark` (`Refuse` with `--refuse`); `--tree` = not `Batch` for the tree only (the C++ banner is not printed; the scripts compare only tree lines).

- [ ] **Step 1: Failing tests.** `load_test.go` on `testdata/` programs (a library directory of tiny modules): the tree of a diamond import (a module loaded once), a parametrised module (`m[3,int]`: the macros, the stream of its body), a numeric and a structured argument (`list[pair[a,b]]` → `pair [ a , b ]`), the formal/actual count errors (messages, statuses as `elan -b` on the same files: `tests/front/errors/load/*`), the module name mismatch, an unknown module, a cycle (warning text), `start with [s]` loading `strat[int]` from the library, a `.spc` with two parts; `divideonargs("a[b[c,d],e]")` = `a`, [`b[c,d]`, `e`]. Acceptance: `tests/front/test_front_load.sh build/install` and `test_front_lex.sh build/install` (the `loaded` files now covered) — Expected: FAIL.
- [ ] **Step 2: Implement.** **Step 3: Run** both scripts — Expected: PASS: every program `elan` loads has the same tree (101 `strconc` subtrees removed, counted) and the same trace (415+ `@foreach` regions excluded, counted); `NOLOAD` 30 (+ any S7e program refused by `elan`), `UNCOVERED` 0.
- [ ] **Step 4: Commit** (`front: the loader skeleton, .lgi reader, module lookup and import graph; front-trace load (S8a task 8)`).

### Task 9: Error corpus, `make check-front`

**Files:** create `tests/front/errors/{lex,load}/*`, `tests/front/test_front_errors.sh`; modify `Makefile` (`check-front`, in `check`), `tests/front/README.md`.

- `errors/lex/<case>.eln|.lgi|.spc` + `<case>.expected` (harness `lex`: stdout, stderr, status), one per lexer and preprocessor message of findings 2-4 (comment and inline to end of file, string too long, wrong string constant, too many string constants (a generated file of 10 001 strings, made by the script), undefined macro, `'c' expected`, `identifier expected`, constant or `(` expected, `MAXDEEPCONSTEXP`, `}` missing, macro construction syntax, `SUCH`, `THAT`, variable expected, non affected variable, too many variables, `mitab` overflow, `tabofident` overflow (3 000 identifiers, generated)); `errors/load/<case>/` (a program, its `args`, `expected` = `elan -b --export /dev/null` stdout, stderr, status): LR syntax error in a header, in an import list, in a `.lgi`; unknown module; name mismatch; parameter count (both messages); `.spc` head errors; spec part name errors; import table overflow (201 modules, generated). Expected outputs generated by the C++ and checked in (`--save`), read once by the implementer.
- `test_front_errors.sh PREFIX [--save]`: each case through `front-trace` (lex or load), stdout, stderr and status compared.
- `check-front` (when `$(PREFIX)/bin/elanc-go` exists, as `check-go`): `go vet ./front/...`, `ELAN_GENERATED_DIR=$(BUILD)/src/interpreter/generated go test ./front/...`, `check_lextrace_hooks.sh`, `test_lextrace_harness.sh`, `test_tracecmp.py`, `test_front_lex.sh`, `test_front_load.sh`, `test_front_errors.sh`; `check:` gains `check-front`.

- [ ] **Step 1:** write the cases, `--save`, read the expected files — Expected: each one shows its message. **Step 2:** `test_front_errors.sh build/install` — Expected: PASS (fix `front` on a failure, with a unit test). **Step 3:** `make check` — Expected: green; time of `check-front` recorded (target: under 2 minutes).
- [ ] **Step 4: Commit** (`tests: the S8a error corpus and make check-front (S8a task 9)`).

### Task 10: Documentation, review, merge

- [ ] `tests/front/README.md` (the harness, its two modes, the trace format, how to read a `tracecmp` difference, the exclusions and why), spec S8 §8 (S8a done, figures: files, programs, regions, exclusions, timings of `front-trace load` on the largest programs), `docs/followups.md` (S8b: `strconc`/`trclos`, `explimpl`, `symbappl`, Earley in place of the lexical skips, unification of `lex.Lexem` with `elanrt/earley.Lexem`, the loader's prefix default).
- [ ] Whole-branch review (Review Focus 1-6); fixes; `make check` green; merge on the controller's decision.
- [ ] **Commit** (`docs: S8a, tables, lexer, preprocessor and LR driver of the Go front end (S8a task 10)`).

## Acceptance (S8a)

- `front-trace lex` equals `elan-lextrace lex` (stdout with positions, stderr, status) on every `.eln`, `.lgi`, `.spc` of `src/lib`, `examples`, `tests` that the harness can read standalone (598 of 619 at d9c73a8); the 21 others (`FOR EACH`/`IF`) are covered by the loader traces.
- For every program of finding 7 that `elan` loads, `front-trace load` gives the C++ module tree (`handling module`/`end of`, `strconc` subtrees excepted) and the C++ lexem stream of every module (text, `FOR EACH` bodies excepted, markers included).
- The error corpus gives the C++ stdout, stderr and status.
- The converted tables are regenerated from the build and equal the checked-in ones; `elan` unchanged; `make check` green with `check-front`; `tests/front/differs.tsv` empty or approved line by line.

## Risks

- **Hooks in `mlstream.cc`** (a C++ source of `elan` is edited). Mitigation: preprocessed identity without the macro, no class layout change, goldens; the alternative (a copy of `mlstream.cc` in `tests/front`) would drift from the oracle.
- **Read-ahead fidelity**: one look-ahead too many or too few moves an insertion into `tabofident` (numbering, S8b) or shifts an error position. Mitigation: standalone traces compare positions; error corpus compares positions.
- **The loader skeleton guesses where a module body starts.** Mitigation: an explicit set of body codes, a loud internal error otherwise, the whole corpus as test; S8b replaces the suspension by the real actions.
- **Lexical skips** stand for Earley in S8a. Exact for valid programs (Earley consumes up to its stop lexem, at least one lexem); they cannot reproduce Earley's error messages: Earley-level errors are S8b's error family.
- **Exclusions hide something.** Mitigation: only three, counted, tested by `test_tracecmp.py`; `strconc` is the only module loaded outside import sections in the corpus (finding 5).
- **Parallel work** (S7e, S8c): `Makefile` only; `front/` is new; S8c's `cmd/elan-go` gets the front subcommands in S8b.
- **Library default prefix**: `front-trace` needs `ELANLIB` or `--elanlib` (no compiled-in prefix); decided for the installed `elan-go` in S8e.
- **Test time**: ~540 programs × 3 runs (each < 0.1 s) and 619 files × 2; parallel by `xargs -P`; recorded in Task 9.

## Results

(filled in task by task: tests RED → GREEN, figures, rulings)

### Task 1 (harness `elan-lextrace`)

- RED: `test_lextrace_harness.sh build` → `FAIL: .../elan-lextrace not built`; `check_lextrace_hooks.sh` PASS before and after the hooks (the guard), and shown to fail on an unguarded line appended to `mlstream.cc` (`static int lextrace_leak;`), and to require that `-DELAN_LEXTRACE` changes the token stream.
- GREEN: the five hand-written `.expected` (`ids.lgi`, `under.eln`, `num.eln`, `str.eln`, `macro.eln`; positions computed from `tabofident.cc` by hand, signed-char sum + 211 probing) equal the harness output at the first run; `examples/unification` in loader mode: status 0, 8 717 lines (as finding 6), 25 `@begin`, 10 `@foreach-begin`, balanced; `make check-unit check-golden` green (golden: 55 cases, 0 failed).
- 33 fixtures in `tests/front/fixtures/` (28 `.expected` generated with `--save` and read): comments, comment/inline/string to end of file, one and two inlines (warning with position), escapes and `\q`, 999/1000-byte strings, back-quoted characters and `` `ab` ``, `\n`/`\t`/`\x` outside strings, 60-character identifier (49 kept), `a_b` in `.lgi` and `.eln`, numbers (`2147483648` → `other -2147483648`, `4294967295` → `char 1`, `4294967296` → `num 0`), byte 0xE9 in and out of a string, `pos.eln`/`pos.lgi` (warning at `line=2 pos=14`; `@inline` placed differently by the preprocessor's read-ahead), the iteration forms, constant expressions, `ppexerr.eln`.
- Observed C++ behaviours worth a test in Task 7 (oracle, not to be "fixed"): in `iter.eln`, `x_1,...,x_3` right after `{ g }_J=3...1` is **not** expanded (stream `x_1 , . . . , x_3`), while `y_1;...;y_3` and `z_(1+1),...,z_3` are; `e_ f` joins to `e_f`; 11 nested parentheses around one number do **not** overflow `MAXDEEPCONSTEXP` (it bounds the evaluation stack, not the nesting: the Task 7 test needs 11 pending operands, e.g. `1+(1+(...))`).
- **Ruling:** the loader check expects 25 module streams for `examples/unification`, not 32 as written in Task 1 Step 1 — why: the trace has exactly the 8 717 lines and 10 regions of finding 6, and `elan --export /dev/null unification.lgi simplesig.spc` (no `-b`) prints 25 `handling module` lines; 32 was a miscount. The script also cross-checks the `@begin` count against `elan`'s own tree when `$BUILD/install/bin/elan` exists — cost if wrong: none for later tasks (the loader comparison of Task 8 compares trees with `elan`, not with a constant).

### Task 2 (corpora, comparison, acceptance scripts, RED)

- RED → GREEN: `python3 tests/front/test_tracecmp.py` failed (`No module named 'tracecmp'`), then 13 tests OK (identical; a lexem differing, missing, extra; a marker missing; `@foreach` bodies: load 0, lex 1; a body on the Go side: 1; `strconc` on the C++ side: 0 with `strconc=1`, on the Go side: 1; positions: load 0, lex 1; text after a position: 1; extra Go `@begin`: 1; stderr and status; the module stack in the report).
- Fixture programs `tests/front/programs/{ifparam,params,foreach,stratlgi,cycle}`: `elan -b --export /dev/null` status 0 for each; harness traces: `foreach` 2 regions (the inner one once, inside the outer one's single solution), `ifparam` 2 (`ifm[1]`, `ifm[0]`), `stratlgi` loads `strat.eln … strsig.eln strconc.eln`, `cycle` prints the warning without `-b`.
- **Ruling:** `ifparam` has a single `IF` condition, not `IF … ANDIF …` — why: the C++ refuses `IF c ANDIF c2` (`[earley] syntax error on symbol 'ANDIF'`): in `macroexp` (mlstream.cc, the condition loop) `iffirst` is set to false only inside `if (!iffirst)`, so for an `IF` it is never reset and the second condition is parsed from `ANDIF` on; `ANDIF` is exercised in `foreach` (`FOR EACH … SUCH THAT A:=(listExtract) elem(1.2.nil) ANDIF A > 1`), and the `IF … ANDIF` refusal is a case for the error corpus (Task 9) — cost if wrong: none (the library's only `IF` users, `ref_read`/`ref_write`, have one condition).
- Corpora measured: `list_files.py` 619 files = 217 `.eln` (197 `standalone`, 20 `loaded`), 373 `.lgi`, 29 `.spc`; the harness reads all 599 standalone files: 595 streams, 4 errors (`common/array.eln`, `common/tuple.eln`, `strategy/carray.eln`, `strategy/symbol.eln`, as finding 6). **Ruling:** 599 standalone / 20 loaded instead of 598 / 21 — why: with comments and strings stripped only 20 `.eln` contain `FOR EACH` or `IF` (`MinelaComp/preprocessing.eln` has `FOR EACH` in a `//` comment only, and the harness reads it standalone); the acceptance figures of Tasks 5/7 become 402 `.lgi`/`.spc` and 599 standalone (595 + 4) — cost if wrong: none, the classification only decides which mode covers a file and both modes are run.
- `list_programs.py`: 568 programs = bench 175, golden 20 (28 `ref` cases, 8 already in the bench), examples 13, test programs 334, BenchThesis 21 (FastCompletion 15, MinelaComp 6: `primes` is listed twice in `test_benchthesis.sh`), fixtures 5; finding 5's "555" is not reproduced (563 without the fixtures) — the scripts count, they do not assume.
- RED state recorded: `test_front_lex.sh build/install --only lgi,spc` → `402 files: 0 ok, 402 failed` (4.1 s); `test_front_load.sh build/install` → `568 programs: 0 ok, 538 failed, 30 NOLOAD` (the 30 `ans` programs of `BCompletion` and `Compiler.2.1/BenchThesis/FastCompletion`; 21 s on 8 cores).
- `test_front_load.sh` sets `SECONDELANLIB` to the program's original parent directory: the private copy would otherwise change the C++ `..` lookup (finding 2).

### Task 3 (`front/diag`, `front/tables`)

- RED: `go test ./front/...` → build failed (`undefined: Run`, `Exit`, `D`, …). GREEN: `go vet ./front/... && go test ./front/...` pass: 5 `diag` tests, 9 `tables` tests (the C++ unit values `Hash("ab",100) = 95`, `"ba"` at 6; signed chars `Hash("a\xe9") = 74`, a sum of exactly -3000 is position 0 as in C, a negative position panics naming the string; `StringTab(4)` overflow message and status 1; `MiTab` counts, the C varargs stop at the first value <= 0, overflow and `NOINT` messages; `typet` 7 sorts at their hash positions, `builtinmodules` 33 names; the limits).
- Consistency with the goldens: the 7 838 table entries of the 28 `tests/golden/expected/*.ref` (identifiers 3 000, sorts 500, modules 200, rule names 2 000, strategy names 500) are each at `Hash(name)` or reachable from it by +211 steps through occupied entries of the same table.

### Task 4 (the table generator, the converted tables)

- RED: `go test ./front/...` → build failed (`undefined: Shift`, `NewTabOfIdent`, `reservedWords`, …; `front/lr/gen` absent). GREEN: `go vet` and `ELAN_GENERATED_DIR=…/build/src/interpreter/generated go test ./front/...` pass; `TestUpToDate` runs (not skipped) and the regenerated files equal the checked-in ones byte for byte; without the variable it is skipped with a message naming it.
- Generator: the excerpt `testdata/ppex3.cc` (3 states, a `-'\''` literal, a `RW` symbol, `DEFAULT`) gives `testdata/ppex3.golden` (written by hand, then only `gofmt`ed); an unknown symbol, an unknown action and a state beyond the arrays are errors (`x.cc:27: unknown symbol WHAT`, line numbers checked).
- Converted tables: `modgram` 646 states (the sentinel state included, as finding 1 counts them), 2 728 actions, 1 141 gotos, 330 `Default` entries; `ldmodgram` 90 / 186 / 82 / 30; `ppexgram` 50 / 203 / 24 / 8; every `Default` entry first in its state, no conflict entry, every shift and goto target in range. State slices keep the capacity of the C arrays up to their end (FFOUND/RFOUND read one entry even for an empty list: Task 6).
- Reserved words: 90 preloaded in order (80 distinct); each of the **80** `#define RWword (-pos+BOFIDENT)` of `commtokens.h` (copied to `front/tables/testdata/`) is `NewTabOfIdent().Index(word)` (finding 1's "84 `#define RW`" counts the four other defines `EOFINPUT`, `NUMBER`, `IDENTIFIER`, `DEFAULT`); `mrwt` holds the 80 positions, 10 of them counted twice.
- `make front-tables` regenerates both files (`go run ./front/lr/gen -in $(BUILD)/src/interpreter/generated -out .`).

### Task 5 (`front/lex`)

- RED: `go test ./front/lex/` → build failed (`undefined: Context`, `New`, `FromBytes`, …); `test_front_lex.sh build/install --only lgi,spc` → `402 files: 0 ok, 402 failed` (Task 2, the stub). GREEN: 18 `lex` tests pass (encoding and `alfsy`/`erralfsy`, including `erralfsy` of the number 0 printing `'\0'`; comments, `/*/` not closing; comment, line comment and inline to end of file — `pos` is the look-ahead counter, one more than the length since the EOF read counts; two inlines (warning only with `Warnings`); the four escapes and `\q`; a NUL byte ending a string constant (strdup); 999/1 000 bytes and an unterminated string (`string constant too long`); 10 001 strings (`too many string constants` while reading ahead of the 9 999th); back quotes and `` `ab` ``; `\n`/`\t` identifiers; 49 characters kept; `a_b` with and without `underID`; wrap-around numbers; byte 0xE9; the position of `fixtures/pos.lgi`; the lookup in the current directory, `../`, each library directory, the case-only refusal `any.eln` vs `Any.eln`, not found; `NewLib` concatenations; `Buffer` over more than a chunk), plus `TestFixtures`: the 4 `.lgi`/`.spc` fixtures equal their harness `.expected` (stdout with positions, stderr, status) and the 6 lexer-error `.eln` fixtures give the harness's stderr and status.
- Acceptance: `tests/front/test_front_lex.sh build/install --only lgi,spc` → **`402 files: 402 ok, 0 failed`** (373 `.lgi` + 29 `.spc`; streams with positions, stderr and status identical), 3.3 s.
- `front-trace lex FILE` reads `.lgi`/`.spc` (and any non-`.eln` file); a `.eln` answers `needs the preprocessor (S8a task 7)`, status 3; it needs `ELANLIB` (no compiled-in prefix).
- **Ruling:** small interface adjustments to the plan's list — `NewLib` returns `(Lib, error)` (the "error to the caller" made explicit), `Buffer.Get(d *diag.D)` (the fatal message needs the streams), and helpers `TraceInline`, `Stream.Chars`, `StringTab.Used/Size`, `MiTab.Count/Len`, `diag.D.Errs/Outs` — why: the plan's signatures had no way to report these errors; nothing else changes — cost if wrong: a rename in tasks 6-8, none outside `front/`.

### Check after Task 5

`make check` (worktree root, own `build/`): exit 0 — smoke, unit (5/5), golden 55/0 failed, round trip 145 ok / 30 not exported (Rust and Go readers), `go test` of `elan/...` and `elanrt` green (with `front/...`), BenchThesis 22 G + 64 J/JO/I ok, legacy bench I/A and J/JO: 0 regressions, 0 snapshot differences. S8a: `check_lextrace_hooks.sh` ok, `test_lextrace_harness.sh` 33 fixtures + unification ok, `test_tracecmp.py` 13 OK, `ELAN_GENERATED_DIR=… go test ./front/...` ok, `test_front_lex.sh --only lgi,spc` 402/402.
