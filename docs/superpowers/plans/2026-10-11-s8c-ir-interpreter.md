# S8c — The IR interpreter (`interp`, `elan-go run`): Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking. Delegated tasks, verified by the controller; one whole-branch review at the end.

**Goal:** `elan-go run [options] prog.ref` runs a program from its `.ref` file by interpreting the IR of `elanc-go` (`src/compiler-go/ir`) on top of `elanrt`, with the command line, output, rewrite steps and exit statuses of the program `elanc-go` builds from the same file — checked on every existing `elanc-go` suite by running both and comparing them (spec S8, criterion 4), with the interpreter's run times reported next to the C++ interpreter's and `elanc-go`'s (criterion 5).

**Architecture:** A new package `elan/interp` mirrors `gen` file by file (`norm.go` ↔ `gen.go`, `ac.go` ↔ `ac.go`, `strat.go` ↔ `strat.go`, `traverse.go` ↔ `traverse.go`, `meta.go` ↔ `meta.go`): what `gen` writes as Go source, `interp` builds once per IR as data and closures and runs with the same `elanrt` calls, every value an `rt.Term` (the native families are a code generation choice, ignored: `interp` always takes `gen`'s boxed path). The decisions that change results or steps and that live in `gen` today (which variables are bound, which conditions are hoisted into an AC match, when the rest of an AC level is normalised...) move to `ir` first, so both back ends read the same answers (Task 1). `cmd/elan-go` reads the `.ref`, lowers it with `ir.Lower` (so the refusals are `elanc-go`'s, status 2), loads it into `interp` and calls `elanrt`'s driver. Tests: `elanc-go --interp` writes an `a.out` that runs `elan-go run` instead of building; a test helper `elanc-interp` builds both and installs a differential `a.out` that runs the compiled program and the interpreter on the same input and fails (status 99) on any difference of stdout (the `rewrite_step` line included), stderr, exit status or written files — so every existing script, run with `ELANC=elanc-interp`, checks the interpreter against compiled code and against the script's own oracles at once.

**Tech Stack:** Go >= 1.24 (standard library only, offline: `GOTOOLCHAIN=local GOFLAGS=-mod=mod GOPROXY=off GOWORK=off`), CMake, POSIX sh / Python test scripts (`tests/compiler-rs`, `tests/compiler`, `tests/legacy-bench`), `.ref` files written by `elan --cexport`.

**Spec:** `docs/superpowers/specs/2026-10-11-s8-go-front-end-design.md` (S8c in §8; criteria 4 and 5 in §3; S8-6, S8-7, S8-15, S8-17 in §6; §10 D1, D6), with `docs/superpowers/specs/2026-10-11-s7-go-compiler-design.md` (G2-G4: representation, runtime, depth guard) and the S7a-S7e plans (the semantics `gen` implements, their rulings and findings).

## Global Constraints

- Branch `s8c-ir-interp`, from `main` **after the S7e merge** (the interpreter mirrors `gen` and `elanrt` as S7e leaves them: I/O builtins, arrays, `builtinHashTerm`, tail-call loops, the query's operators no longer boxing native families). If S8c must start earlier, branch from the tip of `s7e-go-switch` and rebase on `main` once S7e is merged. S8c is independent of S8a/S8b (its input is a `.ref` written by `elan`); the files it shares with them are `src/compiler-go/go.mod` and `cmd/elan-go` (a subcommand table: S8a/S8b add their entries).
- Go >= 1.24, standard library only, offline. Module `elan` requires `elan/elanrt` through `replace elan/elanrt => ./elanrt` (S8-6); `elanrt` stays a separate module, installed for generated programs.
- Nothing in `legacy/` changes; never read credential files (`Sophia/Admin/passwd-def*`). `elan`, `elanc-rem` (REM) and the code `gen` generates do not change: Task 1 is checked by byte-identical generated `main.go` files on every suite program; later tasks touch `gen` only to call a function moved to `ir`.
- **The oracle is compiled code.** For every program of every suite, `elan-go run` and the `elanc-go` build give the same stdout (the `rewrite_step` line included; the timing lines of a run without `-quiet` excepted), the same stderr, the same exit status and the same files written. A difference is a failure; it may be kept only when listed in `tests/interp/differs.tsv` (`<suite>/<program>\t<reason>`) and approved by the user case by case (the rule of §10 D3 applied to S8c). None is planned.
- A construct `gen` does not compile is refused by `elan-go run` in the same way: `ir.Lower`'s `Unsupported` (status 2, `elan-go: <prog>: not supported ...`), and at run time the same `rt.Unsupported` messages (a query with an operator of a `QueryUnsupported` rule; an untyped `call` with `UntypedWhy`).
- A construct added to `gen` or `ir` afterwards is added to `interp` in the same commit (spec §9, "two semantics of the IR"); `make check` runs both (Task 12).
- Disk: the worktree builds in its own `build/` (`make install` → `build/install`); work directories of the scripts are git-ignored and reused; the go build cache is shared (`ELANC_GO_CACHE`). Keep the volume's free space above 5 GiB (`df -h .` before a full bench run: the bench work directory with kind V adds one copy of each case directory per run, removed by the launcher).
- `make check` green at every commit (`make install` first: `make` alone does not reinstall `elanc-go`/`elan-go`); `bench_parity.py --kind G` unchanged.
- Commit messages end with `Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>`. No push, no merge without the controller.

## Review Focus

1. **Rewrite steps.** `interp` counts a step exactly where `gen` emits `rt.Step()` (after the conditions, before the right-hand side), evaluates what `gen` evaluates and nothing more: an AC variable that the rule does not use is not normalised (`rt.NormAC` takes steps), a hoisted condition is step-free (`StepFree`), a `restTaker` variable is bound after the conditions and merged into the right-hand side (`lateBind`), Eker values only for used variables, `CondLet` evaluated even when unused (`_ = expr`). Checked by the `rewrite_step` line of every suite (programs/ac has REM's counts).
2. **Order of results.** Decision trees in source order; labels: every rule, or with `first` the results of the first rule that has one (`found`); `dk`/`dc`/`one` over labels and over strategies with `rt.DcChoice`/`rt.OneChoice` exactly as `gen/strat.go` combines them (`l1(t, d.K, true) || !d.Found && l2(...)`); AC loops (`rt.FirstOcc`, `rt.Rest`, `rt.DivAC`, `rt.Split`, `rt.Down`, extensions); Eker's order; choose branches then the rest.
3. **Depth.** `rt.Guard`/`Unguard` at the points `gen` guards (recursive operators, labels, strategies, traversals); every Guard has its Unguard on every path (`Main` panics otherwise); deterministic `repeat*`/`iterate*` and tail calls are loops (`rt.RepeatStarDet`, `rt.IterateStarDet`, the `again` loop of `ir.IR.TailCalls`), not recursions; the frame budget of Task 4 holds (the `*6` programs of `test_depth.sh`, 10^6 levels, end with status 0, the `*8` ones with status 3, as compiled code).
4. **The differential runner cannot hide a difference.** In `ELANC=elanc-interp` mode a script never runs the compiled binary alone; the launcher compares status, stdout, stderr and the files of the directory; a failing comparison exits 99 and prints both sides. Its own tests (Task 3) include a fake `elan-go` that differs in each of the four ways.
5. **Task 1 is a pure move.** Generated code byte-identical on every suite program and on the `gen` unit-test programs; `gen` reads the moved functions from `ir`.
6. **Boxed path equals native path.** Every builtin on boxed values (stuck arguments → stuck term; `and`/`or`/`not` on a stuck argument count it false; `Total` operators; partial string operations; `b2i` order of booleans in `<`...); equality of arrays by contents (`rt.ArrayEq` on `builtinArray[...]` sorts) in `CondSame` and `==`; injection tests (`is_<inj>`: `OtherStuck`); results printed by `Session.Result` of the boxed term equal `ResultInt`/`ResultBool`/`ResultStr`/`ResultIdent` (Task 2 test).

## Investigation findings (2026-10-11)

Read at `s7e-go-switch` c3a05b8 (S7e tasks 1-9 done: the base of S8c after the S7e merge). Nothing built for this plan.

### 1. What `gen` writes, construct by construct (the specification of `interp`)

`gen` is 3 442 lines without tests at S7e (`gen.go`, `ac.go`, `strat.go`, `traverse.go`, `meta.go`). Per construct, the Go it writes and the `elanrt` it calls:

| construct | `gen` (function) | runtime calls | `interp` file |
|---|---|---|---|
| runtime symbols | `symbols`: the `Variant` operators in operator order, `rt.DeclareSym(rt.Symbol{Name, Arity, AC, Code: ref code, Print})`, panic if out of order | `DeclareSym`, `Arg/Ident/Char/Num` | `load.go` |
| identifiers, query tables | `mainFunc`, `query`: `rt.InitIdents`, `rt.SetQuery(&rt.QueryParser{...})` with `queryOp`, `queryAC`, `queryIsAC`, `queryUnsupported` | `InitIdents`, `SetQuery`, `ApplyAC`, `Unsupported`, `earley.Rule` | `load.go`, `query.go` |
| operator functions | `opFunc`: constructor (`stuck`: `Mk0..3`, `Mk`, `MkAC2`), injection (identity), boolean constant, builtin (`builtinBody`), defined (decision tree, then stuck term or `panic("<fun>: no rule applies to a total operator")`), AC defined (`op(a0,a1) = op_ac(rt.MergeAC(sym,a0,a1))`; `op_ac(e)`: `a0 := rt.ACTerm(sym,e)`, `rt.Normal(a0)` → return, `steps := rt.Steps`, tree, `rt.NormalForm(a0, steps)`); `rt.Guard()` at entry and `rt.Unguard()` at each return when `Op.Recursive` | `Mk*`, `MergeAC`, `ACTerm`, `Normal`, `NormalForm`, `Guard`, `Unguard` | `norm.go` |
| expressions | `expr`: variable; literals (`MkInt`/`MkStr`/`MkIdent` when boxed); injection = its argument; `rt.True/False`; an AC operator with more than two flattened arguments (`acArgs`) → `rt.ApplyAC(sym, acFun, args...)`; else the operator's function on the arguments' values (innermost) | `ApplyAC` | `norm.go` |
| decision trees | `decision`, `symSwitch` (switch on `.Sym`, `children` binds `Args[i]`), `familySwitch` (chain of `headTest`s: ints, strings, idents by value, `rt.True/False`, injections always true when native else `is_<inj>` = not one of `OtherStuck`), `tryRule` (bindings, conditions, then the next tree node: "falls"); a `Fail` or missing default falls to the stuck term | `IsInt`, `IsStr`, `StrOf`, `IsIdent` | `decision.go` |
| conditions | `conds`, `simpleCond` (`CondSame`: `eqExpr` — `rt.Eq`, `rt.ArrayEq` on `builtinArray[` sorts; `CondIf`: `== rt.True`; `CondLet`: evaluated even when unused), `CondMatch` (`patternCheck`: bool constants, injections, `if false` for a non-`Variant` operator, `.Sym ==` then arguments), `apply` (`CondApply`: the strategy's CPS with the rest of the rule in its continuation; in a normalisation function the first result for which the rule applies, `res`/`ok`), `choose` (`CondChoose`: the rest of the rule a closure of the variables bound by every branch, branch after branch); the right-hand side: `rt.Step()`, then the value (or `rt.ExtendAC` modulo an extension), returned (normalisation) or given to `k` with `found = true` (label) | `Step`, `Eq`, `ArrayEq`, `ExtendAC` | `cond.go` |
| AC matching | `acMatch`/`acLevel`/`acVars` (element loops with `rt.FirstOcc`, `hoist` of step-free simple conditions as soon as their variables are bound, `rt.Rest`, one variable: `rt.DivAC` or the whole subject, two: `rt.NewSplit` or `rt.NewDown`, extensions `rt.NewDown(rest,1)`, abstracted part `rt.ACSubject`), `restTaker` + `lateBind`, `ekerMatch` (`rt.NewEker(pat, n, subject)`, `Value(i, acNorm)`, `ekerNever` → no solution), `acValue` (`rt.NormAC` for a defined operator, `rt.MkAC` else) | `FirstOcc`, `Rest`, `DivAC`, `NewSplit`, `NewDown`, `ACSubject`, `NormAC`, `MkAC`, `NewEker`, `EkVar/EkInt/EkStr/EkOp/EkAC/EkItem` | `ac.go` |
| labels | `labels`: `func(a0 T, k rt.K[T], first bool) bool`, `rt.Guard()`/`defer rt.Unguard()`, `found`, after each rule `if first && found { return false }` | `Guard`, `Unguard` | `strat.go` |
| strategies | `strategies`, `seqExpr`, `stratExpr`, `dc`, `one`: id, fail, call (another sort: a traversal function), META (another sort: `false`), labels of the sort only (`labelsAt`), `dk` as `||`, `dc` as `rt.NewDc` + `!d.Found &&`, `one` as `rt.NewOne(k).Pass(...)`, `repeat*`/`iterate*` (`rt.RepeatStar`, `rt.IterateStar`, deterministic: `rt.RepeatStarDet`/`rt.IterateStarDet` with `rt.DetStep`), tail calls (`ir.IR.TailCalls`) as a loop with `again`, a `rt.NewOne(k)` and the states reset each turn | `NewDc`, `NewOne`, `RepeatStar(Det)`, `IterateStar(Det)`, `DetStep` | `strat.go` |
| traversals | `trav*` (on demand per (node, sort)): `rt.TAll/TOne/TSome(t, k, normTerm, sel)`, `rewrite` = body `||` `rt.TOne(..., sel(fun))`, `rt.NormIn`, `rt.OuterStep`, `rt.NormTop`; on a native sort the constant cases (`k(t)`, `false`...); selectors by the operators that can be at the top (`Sort.Variants`, or `Family.Stuck`), AC: the first function for every element | `TAll`, `TOne`, `TSome`, `NormIn`, `OuterStep`, `NormTop`, `MkAny` | `traverse.go` |
| META | `metas`, `metaApply`, `metaCase`, `metaEval`, `metaLabels` (codes 128-144 of `Meta_apply`/`Meta_strat`; errors `[META-built] META strategy over wrong term`, `term2strategy internal error`, `[META] call of an unknown strategy`, `term2labels internal error`) | `Fatal`, `Unsupported` | `meta.go` |
| `normTerm` | the normal form of `sym(args)` by the function of a non-constructor `Variant` operator (AC: its `op_ac`), else `rt.MkAny` (for `rt.Replace` and the traversals) | `MkAny` | `norm.go` |
| main | `rt.Main(querySort, usesQuery, func(s, q) {...})`: the start term (query variable bound), `s.Result*` of its normal form or of each result of the start strategy | `Main`, `Session.Result*` | `load.go` |

`gen` itself refuses nothing (no error path besides `go/format`): every compile-time refusal is `ir.Lower`'s, every run-time one is in the generated code (`queryUnsupported`, `UntypedWhy`) or in `elanrt` (`Main`: `-REFInput` etc.). Sharing `ir.Lower` gives `interp` the same refusals.

### 2. Decisions that live in `gen` and change results or steps

Not only the shape of the code: `usedVars` decides whether an AC variable is normalised (`bind`, `lateBind`, Eker `Value`), `hoist` with `stepFree` decides when a condition is evaluated in an AC match (pruning; a condition with steps waits for the whole match, as REM), `restTaker` decides whether a rest variable gets an extension, `acVars`'s `need` whether the rest is computed, `bound`/`bindsOf` which variables survive a choose, `labelsAt` which labels apply at a sort in a traversal, `otherStuck` the injection tests, `ekerNever` an impossible Eker pattern, `opOf`/`evalOf` the META dispatch, `acArgs` the flattening of AC expressions, the `builtinArray[` prefix of `eqExpr`. Re-deriving them in `interp` would make a second copy to keep in step; Task 1 moves them to `ir` (exported, documented, tested once).

### 3. `elanrt` as an interpreter's runtime

- Every runtime function the generated code calls takes `rt.Term`s or natives and is usable as is; the generic CPS helpers (`rt.K[T]`, `rt.Strat[T]`, `Dk`, `NewDc`, `NewOne`, `RepeatStarDet`...) work with `T = rt.Term`.
- `rt.Main` reads `os.Args[1:]` and ends the process (`Exit`): `elan-go run [options] prog.ref` needs the arguments passed in (`MainArgs`, Task 2). `ParseOptions` stops at the first argument that is not an option, so `elan-go run -noInput -quiet prog.ref` gives `Rest = [prog.ref]`.
- Global state: `Syms` (symbol table, appended by `DeclareSym`), the unique table (`table.go`, with `flagNF` marks of normal forms), `Steps`, identifiers, the string side table, `queryParser`, I/O files, the printer, depth counters. One program per process (as `elan-go run`) needs nothing; in-process unit tests of several programs need a `Reset` (Task 2) that also purges the unique table (a stale `flagNF` mark of another program's AC term would skip a normalisation). S8d (several snapshots in one process) will register symbols append-only (S8-7): Task 4 keys the registration by (ref code, profile) so that S8d can reuse it.

### 4. Depth budget (the main risk)

`elanrt/depth.go`: 1 GiB stack ceiling (`MaxStack` 2e9, stacks are powers of two), stop at 3/4 of it. Compiled code: 96 bytes per level of an innermost normalisation building a list, 48 on native ints, 160 for CPS `iterate*`; 10^6 levels of each succeed. `test_depth.sh` (20 programs) expects status 0 at 10^6 levels (`count6`, `deeplist6`, `ndrec6`, `iternd6`, `iterdet6`, `repdet6`, `tail6` (with `maxrss 64`), `taildc6`, `tailone6`, `tailstop6`, `where6`, `acloop6`) and status 3 at 10^7-10^8. So the interpreter must stay under ~800 bytes of Go stack per ELAN level (768 MiB / 10^6) on every recursion shape, about 5-8 times compiled code: a naive tree walker (evalExpr → callOp → op closure → decision walker → tryRule → each condition → evalExpr) is at that limit. Task 4 measures it first and sets the design rules (a decision tree walked by a loop, runs of simple conditions in a loop, arity-specialised calls); a shape that cannot meet it is reported to the user with figures (a listed difference needs the user's approval).

### 5. Test infrastructure

- `tests/compiler-rs/test_normalise.sh`, `test_strategies.sh` (`-d strategy_terms|ac|io|arrays`), `test_depth.sh`, `test_query.sh` take `ELANC=elanc-go|elanc-rs`; they skip `<name>.no-$ELANC` programs (and `no-$ELANC` directories), `test_depth.sh` runs the programs whose `.expect` lists the compiler on a `compilers` line, `test_query.sh` accepts only `elanc-go`; they read `$ELANC: build time` from the log; `test_normalise.sh` runs `go vet` on the generated module for `elanc-go`.
- `tests/legacy-bench/run_tests.py`: `COMPILER_KINDS = {"R": "elanc-rs", "G": "elanc-go"}`, `jtest` cases expanded to J, JO, R, G; outputs compared with the J snapshot without the `rewrite_step` line. `bench_parity.py --kind G` checks against `bench_g_expected.tsv` and `bench_g_differs.tsv`. `tests/compiler/test_benchthesis.sh` has a kind G (`BENCHTHESIS_KINDS=G`).
- Suite sizes (programs): normalise 29, strategies 96, strategy_terms 100, ac 48, io 12, arrays 17, depth 20, query 11 (152 queries), refused 6; bench G 175 cases (145 built and run, S7e: PASS 78, PASS~ 1, PASS≈ 53, FAIL 13), BenchThesis G 22. `make check-go` runs them all (8 min for the whole `make check` on the M4 Max).
- The only `.no-elanc-go` program is `programs/refused/ac` (elanc-rs's refusal); the interpreter has `elanc-go`'s refusals, so the scripts in interpreter mode honour `.no-elanc-go` too.
- `gen`'s unit tests build their programs with helpers (`newProg`, `op`, `rule`, `peano`, `builtins`, `wherePatterns`...) private to `gen_test.go`; Task 1 moves them to `internal/irtest` so that `interp`'s tests run the same programs and compare with the built ones.

## File structure

| file | role |
|---|---|
| `src/compiler-go/ir/backend.go` (+ `backend_test.go`) | the decisions shared by `gen` and `interp` (finding 2), moved from `gen` |
| `src/compiler-go/internal/irtest/irtest.go` | builders of small `ref.Program`s (from `gen/gen_test.go`), used by `gen` and `interp` tests |
| `src/compiler-go/elanrt/driver.go` | `MainArgs`; `Reset` (new `reset.go`) |
| `src/compiler-go/interp/interp.go` | package doc, `Program`, `Load`, `Main` |
| `interp/load.go` | symbol registration, identifiers, query tables |
| `interp/norm.go`, `decision.go`, `cond.go`, `builtin.go` | operator functions, expressions, decision trees, conditions, builtins (↔ `gen/gen.go`) |
| `interp/ac.go` | AC operators, AC matching, Eker (↔ `gen/ac.go`) |
| `interp/strat.go`, `traverse.go`, `meta.go`, `query.go` | labels and strategies, traversals, META, query (↔ `gen`) |
| `interp/*_test.go` | unit tests; `parity_test.go`: each `irtest` program interpreted and built, outputs compared |
| `src/compiler-go/cmd/elan-go/main.go` (+ `main_test.go`) | `elan-go run [options] prog.ref`; subcommand table |
| `src/compiler-go/cmd/elanc-go/main.go` | `--interp`, `--elan-go FILE` |
| `src/compiler-go/go.mod`, `CMakeLists.txt` | `require`/`replace elan/elanrt`; build and install `elan-go` |
| `tests/interp/elanc-interp` | the differential build (both programs, a comparing `a.out`) |
| `tests/interp/test_elanc_interp.sh` | tests of the helper with fake programs |
| `tests/interp/differs.tsv` | approved differences (empty) |
| `tests/compiler-rs/test_{normalise,strategies,depth,query}.sh`, `bench_parity.py`, `bench_perf.sh`, `tests/legacy-bench/run_tests.py`, `tests/compiler/test_benchthesis.sh` | `ELANC=elanc-interp`, kind V, interpreter timings |
| `Makefile` | `check-interp` (in `check` when Go >= 1.24) |
| `README.md`, `docs/followups.md`, spec S8 §8 | documentation |

## Tasks

### Task 1: The shared decisions move to `ir`; the test builders to `internal/irtest`

No change of behaviour: `gen` emits the same bytes.

**Files:** create `ir/backend.go`, `ir/backend_test.go`, `internal/irtest/irtest.go`; modify `gen/gen.go`, `gen/ac.go`, `gen/strat.go`, `gen/traverse.go`, `gen/meta.go`, `gen/*_test.go`.

**Interfaces (produces, in `ir`):**
- `func UsedVars(r *Rule) map[VarID]bool` (the variables the rule reads; `usedVars`), `func PatternVars(p *Pattern, into map[VarID]bool)`, `func ExprVars(x *Expr, into map[VarID]bool)`, `func CondReads(c *Cond, v VarID) bool`, `func Bound(c *Cond) map[VarID]bool`, `func BindsOf(c *Cond, into map[VarID]bool)`;
- `func (p *IR) StepFree(x *Expr) bool`; `func (p *IR) Hoistable(c *Cond, pending map[VarID]bool) bool` (the test of one step of `hoist`: a `CondSame`, or a step-free `CondIf`/`CondLet`, whose variables are not pending; `hoist` keeps its loop and stops at the first false);
- `func RestTaker(r *Rule, p *Pattern) bool`; `func NeedsRest(p *Pattern, used map[VarID]bool) bool` (`acVars`'s `need`); `func ACArgs(x *Expr) []*Expr` (the flattened arguments of an AC operator at the top);
- `func (p *IR) LabelsAt(ls []LabelID, s SortID) []LabelID`; `func (p *IR) OtherStuck(inj OpID) []OpID`; `func (p *IR) EkerNever(e *EkerPat) (OpID, bool)`; `func (p *IR) SelectorOps(s SortID) []OpID` (`Sort.Variants`, or `Family.Stuck` for a family); `func (p *IR) IsArraySort(s SortID) bool`; `func (p *IR) MetaOpOf(code int32, s SortID) OpID` and `func (p *IR) EvalOf(s SortID) *MetaEval` (`opOf`, `evalOf`).
- Produces (`internal/irtest`): `type Prog`, `New()`, `(*Prog).Op`, `Rule`, `Program() *ref.Program`, and the programs of `gen`'s tests (`Peano`, `Builtins`, `WherePatterns`, the AC, Eker, strategy, META and traversal programs of `ac_test.go`, `strat_test.go`, `meta_test.go`), each with the output `gen`'s test expects.

- [ ] **Step 1: Reference outputs.** With the current tree: `make install`, then a scratch script (not committed) runs `elanc-go --no-build` on every `.lgi` of `tests/compiler-rs/programs/*` (with its `.spc`, copying the directory's `.eln`, as the scripts do) and on the bench G cases (`run_tests.py --kinds G --list` gives the case directories), and stores `sha256` of each `.elan-go.<prog>/main.go` in `tests/compiler-rs/work/gen-identity/before.tsv` (git-ignored); `go test ./gen/` — Expected: PASS.
- [ ] **Step 2: Failing tests.** `ir/backend_test.go`: each moved function on hand-made rules (`RestTaker` true for `F(x, t1)` at the top of an unlabelled rule whose conditions do not read `x`, false when one does; `Hoistable` false for a condition calling a defined operator; `Bound` of a choose whose branches bind {x, y} and {x} is {x}; `UsedVars` of a rule whose right-hand side ignores an AC variable excludes it) — Expected: FAIL (undefined).
- [ ] **Step 3: Move** the functions (bodies unchanged, `e.ir.` receivers become `p.`), `gen` calls them; move the builders to `internal/irtest`, `gen`'s tests use them.
- [ ] **Step 4: Check.** `go vet ./... && go test ./...` (module `elan`) — Expected: PASS; the scratch script again into `after.tsv`; `cmp before.tsv after.tsv` — Expected: no difference.
- [ ] **Step 5: Commit** (`ir: the decisions shared by the back ends move from gen to ir; test programs in internal/irtest (S8c task 1)`).

### Task 2: `elanrt` for an interpreter

**Files:** modify `elanrt/driver.go`, create `elanrt/reset.go`, `elanrt/reset_test.go`, `elanrt/driver_test.go` (cases added).

**Interfaces (produces):**
- `func MainArgs(args []string, querySort string, usesQuery bool, program func(s *Session, query Term))` — `Main` with the arguments given; `Main` becomes `MainArgs(os.Args[1:], ...)` (generated programs unchanged).
- `func Reset()` — back to the state before a program: `Syms` to the builtin symbols, the unique table purged, `Steps` 0, identifiers, strings side table, query parser, I/O files (stdin/stdout/stderr only), printer state, depth counters. Documented as for tests and for an interpreter loading another program in the same process; never called by generated code.

- [ ] **Step 1: Failing tests.** `driver_test.go`: `MainArgs` in a subprocess (the test binary re-run with an env flag, as the existing driver tests do) with `[]string{"-noInput", "-quiet", "x.ref"}`: options parsed, `Rest == ["x.ref"]`, output `\nresult = 1\n\nrewrite_step = 0\n`, status 0. `reset_test.go`: declare a symbol, make an AC normal form (`Intern` sets its `flagNF`), `Reset`, declare another symbol at the same index: `Mk` of the same shape is a new node without the mark; `Steps == 0`. Printing equivalence: for v in {0, -7, 2147483647}, `Result(MkInt(v))` writes what `ResultInt(v)` writes; the same for `MkBool`, `MkStr` (with and without `Quoted`), `MkIdent` after `InitIdents` (spacing flag included: two results in a row) — Expected: FAIL (undefined) for MainArgs/Reset, PASS for printing (if not, fix `print.go` first in its own commit and add the case to `gen`'s tests).
- [ ] **Step 2: Implement.**
- [ ] **Step 3:** `cd src/compiler-go/elanrt && go vet ./... && go test ./...`; `make install && make check-go` — Expected: PASS, `bench_parity.py --kind G` unchanged.
- [ ] **Step 4: Commit** (`elanrt: MainArgs and Reset, for the IR interpreter (S8c task 2)`).

### Task 3: `elan-go run`, `elanc-go --interp`, the differential runner and the switch of the scripts

The tools first, with an interpreter that refuses everything: every later task runs the suites through them.

**Files:** create `cmd/elan-go/main.go`, `main_test.go`, `interp/interp.go` (stub); modify `go.mod`, `CMakeLists.txt` (`elan_go` target, `install(PROGRAMS ... elan-go)`), `cmd/elanc-go/main.go` (+ test); create `tests/interp/elanc-interp`, `tests/interp/test_elanc_interp.sh`, `tests/interp/differs.tsv`, `tests/interp/README.md`; modify the four `tests/compiler-rs/test_*.sh`, `run_tests.py`, `bench_parity.py`, `test_benchthesis.sh`, `Makefile` (`check-interp`, not yet in `check`), `.gitignore` (work directories).

**Interfaces:**
- `elan-go run [options] prog.ref`: reads the file (`ref.Parse`), `ir.Lower(p, <base name>)` — `*ir.Unsupported`: `elan-go: <prog>: <message>` on stderr, status 2; other errors status 1 —, `interp.Load(ir)`, `rt.MainArgs(args, ...)`. `elan-go` alone or another subcommand: usage on stderr, status 1 (the front end subcommands come with S8a/S8b: a table `map[string]func([]string) int`).
- `interp` (stub): `type Program struct`, `func Load(p *ir.IR) (*Program, error)`, `func (p *Program) QuerySort() string`, `UsesQuery() bool`, `Run(s *rt.Session, query rt.Term)`; the stub's `Load` returns an `*ir.Unsupported` "the IR interpreter (S8c)".
- `elanc-go --interp [--elan-go FILE]`: export and lowering as usual (same refusals, same statuses), no generation, no build; the output (`a.out` or `-o`) is a shell script `exec '<elan-go>' run "$@" '<abs .elan-go.<prog>/<prog>.ref>'` (paths single-quoted); `elan-go` = `--elan-go`, else `<prefix>/bin/elan-go`, else from `PATH`. Documented in the usage as a way to run a program without `go build`.
- `tests/interp/elanc-interp [elanc-go options] prog [spec]` (refuses `-o`/`-output`: status 1): `elanc-go "$@"` (status ≠ 0: returned as is, no `a.out`); `a.out` → `a.out.go`; `elanc-go --interp "$@"` (a failure here is an internal error, status 1, message); `a.out` → `a.out.interp`; writes `a.out`, the differential launcher: stdin saved to a temporary file; a copy of the current directory (without `.elan-go.*`, `a.out*`, `gocache`) in a temporary directory, where `a.out.go "$@"` runs; `a.out.interp "$@"` runs in place; both read the saved stdin; compared: exit status, stdout (lines `total time` and `average speed` removed), stderr, and the two directories (`diff -r`, the same exclusions). Equal: the interpreter's stdout and stderr written, its status returned. Different: the interpreter's stdout written, then on stderr `elanc-interp: the interpreter differs from compiled code (<status|stdout|stderr|files>)` and the `diff` of each differing part, status 99. Temporary files removed (trap).
- Scripts: `ELANC=elanc-interp` puts `tests/interp` first in `PATH`, uses `elanc-go`'s build cache and work directory suffix (`-interp`), skips `.no-elanc-interp` **and** `.no-elanc-go` programs, takes the `compilers` lines naming `elanc-go` (`test_depth.sh`) unless they name `!elanc-interp` (an approved exception), no `go vet` step, `test_query.sh` accepts it. `run_tests.py`: kind V = `elanc-interp` (`COMPILER_KINDS`, `jtest` → J, JO, R, G, V; `PATH` with `tests/interp`). `bench_parity.py --kind V`: expected statuses and allowed differences are G's (`bench_g_expected.tsv`, `bench_g_differs.tsv`): V must equal G. `test_benchthesis.sh`: kind V. `make check-interp`: every G step of `check-go` with `ELANC=elanc-interp` / `--kind V` / `BENCHTHESIS_KINDS=V`.

- [ ] **Step 1: Failing tests.** `cmd/elan-go/main_test.go` (builds the command in `TestMain`): no argument → status 1 and usage; `run` of a missing file → status 1; `run` of a `.ref` written from `irtest.Peano()` (the `ref` printer) → status 2, `not supported` (the stub). `cmd/elanc-go/main_test.go`: `--interp --from-ref peano.ref` writes an executable `a.out` whose text names `elan-go run`. `tests/interp/test_elanc_interp.sh` (no ELAN install needed: fake `elanc-go` and `elan-go` scripts in a temporary `PATH`): identical fakes → the launcher passes stdout, stderr and status 0 through, stdin replayed to both (a fake echoing stdin); fakes differing by status, by stdout, by stderr, by a file written → status 99 and the part named; a `rewrite_step` difference → 99; timing lines differing only → equal; `-o x` → status 1; `elanc-go` status 2 → 2 and no `a.out`. — Expected: FAIL (nothing exists).
- [ ] **Step 2: Implement** the command, the option, the helper, the switches.
- [ ] **Step 3: Run.** `go test ./cmd/...`; `sh tests/interp/test_elanc_interp.sh` — Expected: PASS. `make install && ELANC=elanc-interp tests/compiler-rs/test_normalise.sh build/install peano` — Expected: FAIL with `not supported (the IR interpreter (S8c))` (the stub) — the RED state of every suite until the tasks below; `ELANC=elanc-go` runs unchanged (`make check-go` green).
- [ ] **Step 4: Commit** (`elan-go run, elanc-go --interp and the differential runner elanc-interp; the scripts take ELANC=elanc-interp, kind V (S8c task 3)`).

### Task 4: Normalisation (operators, expressions, decision trees, simple conditions) and the frame budget

**Files:** `interp/interp.go`, `load.go`, `norm.go`, `decision.go`, `cond.go`, `interp_test.go`, `parity_test.go`.

**Interfaces:**
- `Load` registers the `Variant` operators in operator order through a registry keyed by (ref code, argument sort names, result sort name) that returns the existing symbol for a known key with the same printing form (an error otherwise: S8d decides; one program never meets it), initialises the identifiers (`rt.InitIdents`), builds `ops []opFun` (`type opFun func(a []rt.Term) rt.Term`, by kind as `opFunc`), `syms []int32` (OpID → runtime symbol, -1), `symOp []ir.OpID` (runtime symbol → OpID, for `normTerm` and the selectors).
- A rule is executed on an environment `[]rt.Term` (one slot per `Rule.Vars`); a decision tree is walked by a loop (no recursion per node) over precomputed occurrence slots (`children`: argument i of the head's term; the child of an injection is the term itself); a rule's conditions are compiled at load time into steps: runs of simple conditions (`CondSame`, `CondIf`, `CondLet`, `CondMatch` with `patternCheck`) executed in a loop, the backtracking ones (Tasks 6-8) as CPS closures `func(env []rt.Term, rest func() bool) bool` (`rest` = the conditions after it and the right-hand side; true = stop: a normalisation keeps its result).
- `Run`: binds the query variable (`Start.UsesQuery`), `s.Result(normal form of Start.Term)` (strategy start: Task 6).

- [ ] **Step 1: Failing tests.** `interp_test.go` (in process: `rt.Reset`, `Load`, `Run` with a session writing to a buffer): `irtest.Peano()` → `\nresult = s(s(o))\n\nrewrite_step = 2\n`; `WherePatterns()` and the decision-tree programs of `gen`'s tests with their expected outputs; a total operator with no applicable rule → panic naming the function (as `gen`). `parity_test.go` (skipped with `-short`): each `irtest` program interpreted and built with `gen` (the existing `module`/`run` helpers, moved to `irtest`), outputs equal. Frame budget: `BenchmarkDepth`-style tests (`depth_test.go`) measuring `rt.MaxDepth` and the stack size (`runtime/metrics`, as `elanrt/depth_test.go`) for `count(n)` (innermost recursion on ints), `len(gen(n))` (list) and a recursion through a `where x := () t` — record bytes per level. Suite: `ELANC=elanc-interp tests/compiler-rs/test_normalise.sh build/install` on the programs without builtins on strings/terms, AC, strategies or queries (list in the task's results) and `test_depth.sh build/install count6 deeplist6 count8 twophase` — Expected: FAIL (stub).
- [ ] **Step 2: Implement.**
- [ ] **Step 3: Run** the same — Expected: PASS; bytes per level recorded (target ≤ 400, hard limit 700). Above the target: reduce before going on (arity-specialised `opFun1/2/3` without a slice; the argument values in a fixed-size array; the decision loop and the simple conditions in the operator's frame) and measure again; above the hard limit after that: stop and report to the controller with the figures (a depth difference needs the user's approval).
- [ ] **Step 4: Commit** (`interp: normalisation by decision trees, simple conditions, the frame budget measured (S8c task 4)`).

### Task 5: Builtins

**Files:** `interp/builtin.go`, `builtin_test.go`.

Every `BuiltinOp` as `builtinBody` on boxed values: the value operations (`rt.IntOf`/`BoolOf`/`StrValue`/`IdentOf`; a stuck argument → the stuck term; `Total` → `rt.MustInt`... with the operator's function name), `rt.Div`, `rt.Mod`, int32 wrap, comparisons of booleans as C ints (`b2i`), `BoolToInt`, `IntToBool`, the partial string operations (stuck when undefined), `and`/`or`/`not` (`== rt.True`), `==`/`!=` on terms (`rt.Eq`, `rt.ArrayEq` on array sorts), the operations on terms (`IntArgs` checked; `rt.Replace` with `normTerm`, `Occurs`, `SyntacticMatch`, `SyntacticUnify`, `WriteTerm`, `ReadTerm` with the result sort's `.ref` index, arrays, `HashTerm`), I/O (`Getc`, `Putc`, `Open`, `Close`, `Flush`, the `Pid` injection). `normTerm` as `normTermFunc`.

- [ ] **Step 1: Failing tests.** `builtin_test.go`: `irtest.Builtins()` and `gen`'s `TestBuiltinsNativeAndBoxed`, `TestTotalBuiltinReadsBoxedValuesChecked` programs (outputs equal to the built ones in `parity_test.go`); a table test per `BuiltinOp` on values and on a stuck argument. Suites: `test_normalise.sh` (all), `test_strategies.sh -d io`, `-d arrays` (the programs without strategies) with `ELANC=elanc-interp` — Expected: FAIL.
- [ ] **Step 2: Implement**; **Step 3: Run** — Expected: PASS (io: the files written compared by the launcher and by the script).
- [ ] **Step 4: Commit** (`interp: builtins (S8c task 5)`).

### Task 6: Labels, strategies, `where p := (S) t`

**Files:** `interp/strat.go`, `strat_test.go`; `cond.go` (`CondApply`).

**Interfaces:** `type labelFun func(t rt.Term, k rt.K[rt.Term], first bool) bool` per label (`rt.Guard`, deferred `Unguard`, `found`, `if first && found { return false }` after each rule); `func (p *Program) strat(s *ir.Strat, sort ir.SortID) rt.Strat[rt.Term]` and `func (p *Program) seq(seq []ir.Strat, sort ir.SortID) rt.Strat[rt.Term]` built once per (node, sort) (memo), mirroring `seqExpr`/`stratExpr` (id;s = s, s;id = s; the states of `dc`/`one` (`rt.NewDc`, `rt.NewOne`) made per application, which equals `gen`'s hoisting); named strategies with `rt.Guard`; `ir.IR.TailCalls` → the `again` loop (`rt.NewOne(k)`, states fresh each turn); deterministic `repeat*`/`iterate*` with `rt.DetStep`. `CondApply`: in a normalisation the first result for which the rest of the rule holds; in a label each one to `k`. `Run` with `Start.Strategy`: each result printed.

- [ ] **Step 1: Failing tests.** The programs of `gen/strat_test.go` (`TestStrategiesRun`, `TestDcWithoutResult`, `TestTurns`, tail loops) through `irtest`, in `parity_test.go`; a test that a tail-recursive strategy at 10^6 turns keeps `rt.MaxDepth` bounded (as `TestTailLoopsAllocateOnce` checks allocations). Suites with `ELANC=elanc-interp`: `test_strategies.sh` (96), `-d io`, `-d arrays` (all), `test_depth.sh` (the strategy programs: `ndrec*`, `iter*`, `repdet6`, `tail*`, `where*`, `resdeep8`) — Expected: FAIL.
- [ ] **Step 2: Implement**; **Step 3: Run** — Expected: PASS; bytes per level of `ndrec`, `iternd`, `where` recorded (same limits as Task 4).
- [ ] **Step 4: Commit** (`interp: labels, strategies and where p := (S) t (S8c task 6)`).

### Task 7: `choose try ... end`

**Files:** `interp/cond.go` (`CondChoose`), `cond_test.go`.

As `choose`: the rest of the rule (conditions after `end`, right-hand side) shared by the branches, on the variables bound by every branch (`ir.Bound`); branch after branch, each solution of a branch running the rest; a normalisation keeps the first; a label gives every result; under `dc` the interpreter's semantics (the S7d plan, Task 2: not REM's).

- [ ] **Step 1: Failing tests.** `gen`'s `TestChooseLinear` program and the choose programs (`choose_*` of `programs/strategies`, by name in the results) — Expected: FAIL.
- [ ] **Step 2: Implement**; **Step 3: Run** `test_normalise.sh` and `test_strategies.sh` in full with `ELANC=elanc-interp` — Expected: PASS.
- [ ] **Step 4: Commit** (`interp: choose/try (S8c task 7)`).

### Task 8: AC operators, AC matching, Eker

**Files:** `interp/ac.go`, `ac_test.go`; `norm.go` (AC operator functions, `rt.ApplyAC` in expressions).

As `gen/ac.go`, through the `ir` functions of Task 1: AC operator functions (`MergeAC` → the function over a multiset: `ACTerm`, `Normal`, `steps`, tree, `NormalForm`); `ApplyAC` for more than two flattened arguments (`ir.ACArgs`); a run of `CondAC` matched by nested element loops (outer patterns first, nested ones, siblings, the variables of each AC level innermost in reverse order) with `ir.Hoistable` conditions at each binding point; `acVars` (`NeedsRest`, `DivAC`, `Split`, `Down`, extensions, `ACSubject` for an abstracted part); `RestTaker`/`lateBind` (the rest merged into the right-hand side by `ExtendAC` when the variable is an argument of the AC operator at its top); `rt.ExtendAC` modulo an extension; `CondEker` (`rt.EkPat` built once per condition at load, `rt.NewEker`, `Value(i, acNorm)` for used variables only; `EkerNever` → no solution).

- [ ] **Step 1: Failing tests.** `gen`'s `TestACProgramRuns`, `TestEkerProgramRuns` programs in `parity_test.go`; `ELANC=elanc-interp tests/compiler-rs/test_strategies.sh build/install -d ac` (48; `rem-steps`: the step counts compared with REM's too), `test_depth.sh acloop6` — Expected: FAIL.
- [ ] **Step 2: Implement**; **Step 3: Run** — Expected: PASS.
- [ ] **Step 4: Commit** (`interp: AC operators, AC matching and Eker's matcher (S8c task 8)`).

### Task 9: Traversals

**Files:** `interp/traverse.go`, `traverse_test.go`.

As `gen/traverse.go`: per (node, sort), memoised: `tall`/`tone`/`tsome` (`rt.TAll`/`TOne`/`TSome` with `normTerm` and a selector), `rewrite` (body at the top `||` `rt.TOne` with the selector of the traversal itself), `normin` (`rt.NormIn`), `normout` (`rt.NormTop` of `rt.OuterStep`), the step one(labels of the sort); selectors `rt.Sel` from `ir.SelectorOps` (an AC symbol: the first argument's function for every element); a named strategy called at another sort compiled for that sort; on a family sort with no stuck operator, the constant cases of `travExpr` (`k(t)`, `false`, `NormTop`); `META` at another sort: no result.

- [ ] **Step 1: Failing tests.** `gen`'s `TestTraversals` program; the traversal programs of `programs/strategies` (`trav_*`, `runtimetraversal*`) — Expected: FAIL.
- [ ] **Step 2: Implement**; **Step 3: Run** `test_strategies.sh` in full — Expected: PASS.
- [ ] **Step 4: Commit** (`interp: traversals (S8c task 9)`).

### Task 10: META and strategy terms

**Files:** `interp/meta.go`, `meta_test.go`.

As `gen/meta.go`: per `MetaApply`, the `meta_apply` terms of its sort (`MetaList`, `MetaAll`, `MetaNth`; any other term: `rt.Fatal("[META-built] META strategy over wrong term")`); per `MetaEval`, the evaluation of `Strategy[X]` terms (codes 131-144: sequences, `id`, `call "name":X` by `MetaCall.Typed`/`Untyped` (`rt.Unsupported(UntypedWhy)`), `dc`/`dk` over strategies and over labels named by strings (`MetaLabel`), `repeat`, `iterate`) and its errors, as the interpreter's.

- [ ] **Step 1: Failing tests.** `gen`'s `TestMetaRuns`, `TestMetaLabelsNamedByStrings` programs; `ELANC=elanc-interp test_strategies.sh build/install -d strategy_terms` (100) — Expected: FAIL.
- [ ] **Step 2: Implement**; **Step 3: Run** — Expected: PASS.
- [ ] **Step 4: Commit** (`interp: META and the strategy terms (S8c task 10)`).

### Task 11: Query input

**Files:** `interp/query.go`, `query_test.go`.

`Load` with `Start.Query`: `rt.SetQuery(&rt.QueryParser{Rules: <earley.Rule of each ir.GrammarRule>, Sort, SortNames, Intern*, Op, AC, IsAC})`: `Op(r, a)` the operator function of `QueryRule.Op` (an AC operator through `AC`), `QueryUmin` → `rt.MkInt(-a[0].Int)`, `QueryUnsupported` → `rt.Unsupported("a query with the operator of grammar rule <r>: <Why>")`; `AC(r, e)` = `rt.ApplyAC(sym, acFun, e...)`; `IsAC`. The query variable bound to the term read; `-noInput` with `UsesQuery`: `rt.MainArgs` prints REM's two lines.

- [ ] **Step 1: Failing tests.** `gen`'s `TestQueryParserCode` program run with a query on stdin (subprocess through `elan-go run`); `ELANC=elanc-interp tests/compiler-rs/test_query.sh build/install` (49 programs, 152 queries, syntax errors and EOF included) — Expected: FAIL.
- [ ] **Step 2: Implement**; **Step 3: Run** — Expected: PASS (stderr of the syntax errors compared by the launcher).
- [ ] **Step 4: Commit** (`interp: query input (S8c task 11)`).

### Task 12: Every suite, the bench and BenchThesis (criterion 4); `check-interp` in `make check`

**Files:** `Makefile` (`check-interp` in `check` when Go >= 1.24, after `check-go`), `tests/compiler-rs/bench_parity.py`, `tests/interp/differs.tsv`, `docs/followups.md`.

- [ ] **Step 1: Run** `make check-interp`: `test_normalise.sh`, `test_strategies.sh` (and `-d strategy_terms|ac|io|arrays`), `test_depth.sh`, `test_query.sh` with `ELANC=elanc-interp`; `bench_parity.py build/install --kind V` (every G case run by both: statuses equal to G's, no `PARITY:` line, no status 99); `BENCHTHESIS_KINDS=V tests/compiler/test_benchthesis.sh build/install` (22) — Expected: PASS. A failure is fixed in `interp` (with a unit test reproducing it), never by a listed difference without the user's approval.
- [ ] **Step 2:** `make check` (macOS; `make check-sanitize` unaffected: no C++ change) — Expected: green; the time added by `check-interp` recorded.
- [ ] **Step 3: Commit** (`tests: the IR interpreter equals compiled code on every suite, check-interp in make check (S8c task 12)`).

### Task 13: Performance (criterion 5)

**Files:** `tests/compiler-rs/bench_perf.sh` (`--interp`), the plan's results.

`bench_perf.sh PREFIX [N] --interp`: for each case, besides the compiled program, `elan-go run -noInput -quiet <.ref>` (the `.ref` of the build) and the C++ interpreter on the same program (`elan -b <lgi>` with the query `true end`, the start term ignoring it, as `test_strategies.sh` runs it; a time limit of 60 s per run, `>60` printed), best of N real times, RSS, steps (the interpreter's must equal compiled code's); a column `V/I` (interpreter over C++ interpreter). Also the load time of `elan-go run` (read + lower + load; `elan-go run` with `ELAN_STATS=1` prints it on stderr, added here) on the largest programs (BenchThesis `MinelaComp`, `examples/unification`) against the 0.5 s target of the loading criterion (informative for S8c; required in S8d).

- [ ] **Step 1: Run** `tests/compiler-rs/bench_perf.sh build/install 5 --interp` and the bench V timing summary (`run_tests.py --kinds V` times per case vs I) — Expected: the table; target V ≤ I on every case (S8-17).
- [ ] **Step 2:** For a miss: profile (`ELAN_CPUPROFILE`), try the cheap leads (allocation of environments, closures per application, map lookups at run time), each measured alone with a keep/drop rule as S7e; a remaining miss recorded in the results with its figures (not a failure, §10 D6).
- [ ] **Step 3: Commit** (`tests: bench_perf --interp, the IR interpreter's times next to the C++ interpreter's (S8c task 13)`).

### Task 14: Documentation, review, merge

- [ ] `README.md` (`elan-go run`, `elanc-go --interp`, exit statuses), `docs/followups.md` (the S8c differences if any, the misses of Task 13), spec S8 §8 (S8c done, figures), `tests/interp/README.md` (the differential runner, how to read a 99).
- [ ] Whole-branch review (Review Focus 1-6); fixes; `make check` green; merge on the controller's decision.
- [ ] **Commit** (`docs: S8c, the IR interpreter (S8c task 14)`).

## Acceptance (S8c)

- `elan-go run [options] prog.ref` gives the stdout (`rewrite_step` included), stderr, exit status and files of the program `elanc-go` builds, on every program of `test_normalise.sh`, `test_strategies.sh` (strategies, strategy_terms, ac, io, arrays), `test_depth.sh`, `test_query.sh`, the bench G cases (kind V) and BenchThesis (kind V); `tests/interp/differs.tsv` empty or every line approved by the user.
- The same refusals (status 2, same messages) as `elanc-go`.
- `make check` runs `check-interp` when Go >= 1.24 and is green; `elan` and the generated code unchanged.
- The interpreter's times next to the C++ interpreter's and `elanc-go`'s recorded (`bench_perf.sh --interp`), misses recorded.

## Risks

- **Frame budget** (finding 4): an interpreter frame chain per ELAN level above ~768 bytes turns status 0 into status 3 at 10^6 levels. Mitigation: measured in Tasks 4 and 6 before going on, design rules given (loops for trees and simple conditions, arity-specialised calls), escalation with figures.
- **Drift between `gen` and `interp`** (spec §9). Mitigation: the decisions in `ir` (Task 1), files mirrored function by function with `gen`'s names in comments, the differential runner on every suite in `make check`, the same-commit rule.
- **Steps counted differently**: an evaluation `gen` skips (an unused AC variable, a non-hoisted condition) done by `interp`. Mitigation: Review Focus 1, `rewrite_step` compared everywhere (not only where REM's counts are known).
- **Performance** below the C++ interpreter on some programs (an interpreter of a compiled scheme still allocates an environment per rule try and a closure per CPS step). Mitigation: Task 13's leads; a miss is recorded (D6).
- **Global state of `elanrt`** in in-process tests. Mitigation: `rt.Reset` (Task 2) with the unique table purged; `elan-go run` is one program per process.
- **Test time**: `check-interp` runs each suite program twice (compiled and interpreted) and copies its directory. Mitigation: the build cache is shared with `check-go`; the time recorded in Task 12; `ELANC=elanc-interp` can run one suite or one program by name.
- **Side effects of the I/O programs** run twice: the compiled run happens in a copy of the directory, the files of both compared (Task 3).
- **Parallel S8a/S8b**: `go.mod` and `cmd/elan-go` touched by both. Mitigation: S8c introduces the `replace` and the subcommand table; S8a/S8b rebase on it.

## Results

(filled in task by task: tests RED → GREEN, figures, rulings)
