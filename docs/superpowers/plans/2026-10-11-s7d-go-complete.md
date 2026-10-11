# S7d — Completeness of the Go compiler: Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking. Delegated tasks, verified by the controller; one whole-branch review at the end.

**Goal:** `elanc-go` compiles and runs every program of the bench that `elanc` compiles (no case UNSUPPORTED, none worse than J), the BenchThesis programs (with a G kind of `test_benchthesis.sh`) and programs that read their query from stdin.

**Architecture:** The remaining constructs are added where they belong in the existing pipeline (`ref` → `ir` lowering → `gen` emission → `elanrt`): term-level builtins (`replace`, `occurs`, syntactic matching) and identifiers as runtime functions and a new node kind; `choose/try` as a new condition kind; query input as a Go port of the compiled runtime's Earley parser (`src/compiler/earley`, lexer included) in `elanrt`, driven by grammar tables that `gen` emits from the three grammar dumps of the `.ref` file, exactly as REM emits `gram[]`; traversals and `normin`/`normout` as new strategy kinds following REM (the interpreter does not interpret them).

**Tech Stack:** Go >= 1.24 (standard library only, offline), the `.ref` export of `elan --cexport`, shell/Python test scripts of `tests/compiler-rs`, `tests/compiler`, `tests/legacy-bench`.

**Spec:** `docs/superpowers/specs/2026-10-11-s7-go-compiler-design.md` (S7d in §5; G1-G9), with the S6d list of `docs/s6-rust-status.md`.

## Global Constraints

- Branch `s7d-go-complete`. Go >= 1.24, standard library only, offline (`GOTOOLCHAIN=local GOFLAGS=-mod=mod GOPROXY=off GOWORK=off`).
- Nothing in `legacy/` changes; never read credential files. Nothing in `elan`, `elanc`/REM, `elanc-rs` changes (a bug found there: own commit, regression case, `docs/followups.md`); the exporter (`src/interpreter/ref/ref.cc`) is out of scope.
- Oracles: the interpreter for semantics (spec §2.3: "Results equal to the interpreter's (order included) on the dedicated tests; output byte-identical to REM's where REM is right"); REM for the output format and for the constructs the interpreter does not interpret (traversals, `normin`/`normout`, `rewrite`: the interpreter prints "Traversal is not intepreted" and fails, `rewrite/stateofexecution.cc:873-879`). Every case where elanc-go deliberately differs from REM or from the interpreter is listed with its reason (`.no-rem`, `.no-interpreter`, `bench_g_differs.tsv`).
- Generated programs: same command line, output format and exit statuses (0 ok, 1 run-time error, 2 construct not supported, 3 recursion too deep). A program whose query is read at run time behaves as REM's (`termIn.c` `termParser`, query mode 2): prompt `Enter a query term of sort 'S'` (also with `-quiet`), the term parsed up to `end`, an empty line, the query normalised, the start term built with `query` bound to it, the start strategy applied.
- `bench_parity.py --kind G` may only improve (`tests/compiler-rs/bench_g_expected.tsv`); `make check` green at every commit.
- Each task ends with `make check-go` green (`make install` first: `make` alone does not reinstall `build/install/bin/elanc-go`).

## Review Focus

1. Query parsing equal to REM's and the interpreter's on mixfix operators with priorities and associativity (`3*X*X + 2*X + 7`), overloaded operators of several sorts, injections (`@ : (builtinInt) int`), strings with escapes, negative numbers, identifiers not in the program, AC operators (the term read is flattened and sorted): Task 5 `programs/query/*` and the examples.
2. A syntax error in the query: REM's message (`[error] :file=standard input line=1 pos=0`, `[earley] syntax error on symbol ')'`, the expected lexems, the context) and exit status 1, never a Go panic: Task 5 `syntax_*` cases.
3. Empty stdin or a query without `end` (EOF): REM stops with the parser's error and status 1; elanc-go the same, no hang: Task 5 `eof` case.
4. `-noInput` on a program whose start term uses `query`: REM prints `No ground term is defined` / `Do not use -noInput in this case` and exits 0 (bench `Compiler.3.2/Test` ident1): Task 3.
5. Term builtins on AC terms and identifiers (`replace x by y in t` where t has an AC subterm: flattened again and renormalised, `term_rec_replace`): Task 1 `replace_ac`.

## Investigation findings (2026-10-11)

Measured on `main` at e8c0846 (`make install`, Apple M4 Max).

**Bench G.** `python3 tests/compiler-rs/bench_parity.py build/install --kind G`: `PASS=74 PASS~=1 PASS≈=46 FAIL=9 UNSUPPORTED=15 ERROR=15 NOLGI=30 NOREF=15` (J baseline: 145 cases compiled by elanc = PASS 68, PASS~ 1, PASS≈ 63, FAIL 13; ERROR 15 = `Compiler.2.1/BenchThesis` in ELAN 2.1 syntax, `rewrite` is a keyword: elanc fails too). The 15 UNSUPPORTED (the refusal printed by elanc-go, first one only):

| cases | refusal | J status |
|---|---|---|
| `Compiler.{2.1,3.0,3.2}/Test` bimatch (3) | builtin operation code 191 (`builtinSyntacticMatching(@,@,@,@) : (term term term term) term`) | PASS≈ |
| `Compiler.{2.1,3.0,3.2,4.0}/Test` rep (4) | builtin operation code 16 (`replace@by@in@ : (Foo Foo Foo) Foo`) | PASS≈ |
| `Compiler.{2.1,3.0,3.2,4.0}/Test` choice2 (4) | choose/try conditions in a condition of a rule for `g1(@)` | PASS |
| `Compiler.{2.1,3.0,4.0}/Test` ident1 (3) | builtin identifiers (sort ident) | FAIL |
| `Compiler.3.2/Test` ident1 (1) | query input: the start term uses the query (`start with () query`, run with `-noInput`) | FAIL |

A probe (a scratch copy of `ir/lower.go` where every S7d refusal is logged and skipped) finds no other refusal in these four programs.

**What elanc-go cannot run yet, by missing feature** (bench, BenchThesis, `tests/compiler`, `examples/`):

| feature | programs |
|---|---|
| term builtins 16 `replace`, 17 `occurs`, 191 syntactic matching (175 unification is its sibling) — polymorphic operations on sorts of terms (`(X Y) bool`), today also refused as "builtin operation on sorts that are not builtin" | rep, bimatch; BenchThesis FastCompletion (`occurs@in@` on 6 profiles, `replace@by@in@` on 2) and MinelaComp (16, 17); examples/unification |
| choose/try | choice2; FastCompletion; examples/quick |
| identifiers (sort `ident`, literals `Ident(n)`, codes 14/15 `==`/`!=`, 177 `ident2string`) | ident1 (4); FastCompletion, MinelaComp (sort `identifier` of the library: `noquote/identifier.eln`) |
| query input (stdin) | ident1 3.2 (only the `-noInput` message); MinelaComp compiled (`rewritingC.lgi`, 14 cases of `test_benchthesis.sh`, query `[append(cons(a,nil),cons(b,nil)) , proofnil] end`); `tests/compiler/test_start_term.sh` (7 programs x 3 queries + MinelaComp `rewriting` peano/append), `test_runtime_strategies.sh` (19 programs x 3 queries); all 13 `examples/` |
| traversals tall/tone/tsome/rewrite, normin/normout | no bench case; `tests/compiler-rs/programs/refused/traversal`, `runtimetraversal` (`tone` in a strategy term: `eval_TONE` of `strat[X]`) |
| labels named by strings in Meta_strat (codes 133-136), `switch/case` rule bodies (SWRULE: REM's `REFParser.jj` does not read them), builtin operations of native sorts under AC | no bench case, no program of `tests/compiler`; `programs/refused/acbuiltin` |

So: **J/JO** (the jtest cases, all run with `-noInput`, stdin `/dev/null`, `run_tests.py:278,288`) need no query parser: they need Tasks 1-3. **BenchThesis**: the 30 FastCompletion cases (15 specs, J and JO) run `./a.out -noInput -quiet` (`start with (s_commande) sat`, ground) and need Tasks 1-3; the 14 MinelaComp cases (7 specs) read their query from stdin and need Task 5. BenchThesis has no elanc-go kind today (`test_benchthesis.sh` knows J, JO, I).

**How the compiled programs read queries today (REM).** `main_skeleton.c` parses the options (`-noInput` = query mode 0, default 2), `termIn.c` `termParser`: mode 0 calls `main_query()` (the ground start term; when the start term uses `query`, REM generates `printf("No ground term is defined\n"); printf("Do not use -noInput in this case\n"); exit(0);`, `rem/compiler/Query.java:85`); mode 2 prints `Enter a query term of sort '%s'\n` (`tabSort[earleyQuerySort]`), calls `EarleyParser("")` (`earleyCall(stdin, sort)` then `computeTerm`, then `printf("\n")`), normalises the term, `main_query_start(res)` (the start term with `query` bound, `Query.genStartTerm`), then the start strategy (`strTab[earleyQueryStrategy]`). A syntax error prints the Earley error on stdout and exits 1 (REM run on `examples/poly1`: `[error] :file=standard input line=1 pos=0` / `[earley] syntax error on symbol ')'` / `one of ('NUMBER' 'umin_builtinInt' ... '-' '(') expected` / `in context deriv ( 3 * X * X +  >>>)<<< end`).
The parser is driven by `int gram[]`, which REM writes while reading the `.ref` file (`rem/parser/REFParser.jj` `SymbolDeclaration`): for every operator of the three grammar dumps (Both, GlobalOnly, TopOnly, in file order) `1, priority + assoc*4096, symbol, sort, n` then `n` pairs (kind 0 identifier / 1 sort / 2 char / 3 number, code). `runtimeInit.cc` `addstandards` adds the three standard rules (`intern ident` 220 ← IDENT, `intern int` 19 ← NUMBER, `intern string` 351 ← STRING, priority 4000), `grammarInit` the others; the semantic actions (`esemact`) record a prefix list of symbol codes, idents, numbers and strings that `computeTerm` (`termIn.c`) turns into a term (an AC symbol: its arguments added one by one to a flattened, sorted term). Sizes: `src/compiler/earley` 4138 lines (earley.cc 675, grammar.cc 312, lstream.cc 312, ichstream.cc 139, lbuffer.cc 70, stringtab.cc 171, runtimeInit.cc 263; the rest headers and allocators), copied from the interpreter's parser (`interpreter/parse/earley.cc` 737 lines, 94 lines differ). Grammar size: the MinelaComp export (`rewritingC` + `append`) has 151 `GrammarForSort` blocks, 702 grammar rules; FastCompletion `ans` 127 KB of `.ref`, 339 rules, 42 strategies.

**Decision: port the Earley parser to Go, driven by tables emitted from the `.ref` grammars; do not generate a parser.** Evidence:
- The `.ref` grammars are mixfix grammars with overloading across sorts (`@ == @` on bool, builtinInt, ident...), injection chains (`intern int` → builtinInt → int), priorities and associativity resolved *during* the Earley completion (`grammar::solveconflicts`, `solveconflict2`, masks `RPRIORITYMSK`/`RASSOCMSK`) and ambiguity warnings when they do not decide; they are not LR(k) or LL(k), so a generated LR/LL/PEG parser would need its own disambiguation and would accept or reject other inputs, with other error messages. A parser "generated from the grammar" that keeps these semantics is an Earley parser with precomputed tables, i.e. the port.
- The behaviour to reproduce (which queries parse, how ties resolve, the error text and its list of expected lexems) is defined by ~1200 lines of parser and lexer C++, which the port follows function by function: it can be tested differentially, query by query, against REM's compiled programs and the interpreter (same `.ref`, same input).
- The tables are small (700 rules) and parsing happens once per run: no performance concern. Emitting the tables keeps the runtime independent of the program (one `elanrt` package), as REM's `gram[]`.

**Other findings.**
- REM prints an identifier value as `IDENT(n)` (its index in `tabofident`): the J snapshot of ident1 2.1/3.0/4.0 is `result = IDENT(454)`; the interpreter prints `toto`. elanc-go follows the interpreter, so its output deliberately differs from the J snapshot (bench_parity needs an allowed-difference list, Task 3). Identifier indices are hash positions (`base/stringtab.cc`: sum of the characters modulo 3000, collisions probed by steps of 211); REM orders identifiers under AC symbols by index (`Gterm_cmp`), so new identifiers read from a query get the same hash position as in REM (seeded with the program's `Identifiers` table).
- bimatch: REM gives `a.nil` (matching `f(var(1))` against `f(a)` with the variable list `var(1).nil`), the interpreter `fail`; the 2000 output (`SAMPLES/bimatch.out`: `.(a,nil)`) agrees with REM: the interpreter is wrong here (to analyse in Task 1: likely `term::equal` on the int argument of `var(1)`); elanc-go follows REM, the program gets a `.no-interpreter`, `docs/followups.md` an entry.
- `replace` (REM `term_rec_replace`) replaces every occurrence of t2 by t3 in t1 (equality modulo sharing, ints/idents/strings left as they are, AC arguments re-added with their multiplicity) and renormalises the copied path; `occurs a in b` is `term_occur(b, a)`.
- elanrt today: `Main` refuses a query at run time (`driver.go:224-230`: prompt, "reading a query is not supported yet", status 2); `ir.(*lowerer).run` refuses a start term containing the query variable (`lower.go:483`).

**Rulings (Tasks 1-3, decided from both oracles; programs in `tests/compiler-rs/programs/normalise` and `programs/strategies/choose_*`).**
- Ruling: syntactic matching (191) follows REM (`term_syntacticMatching`) — the interpreter's `term::equalMatch` ends its function-symbol case with `break` and returns 0, so it fails on every non-variable, non-constant pattern (`g(a,b)` against `g(a,b)` gives `fail`), while REM and the 2000 output agree (`bimatch`: `a.nil`) — cost if wrong: none for the bench (J is REM's); `bimatch`/`synmatch` `.no-interpreter`. Within it: a variable takes any subject (the interpreter's order: REM tests the tags before the variables and refuses ints, identifiers and AC terms there), strings match equal strings (REM's `return strcmp(...)` is inverted), AC terms match only equal terms ("only for empty theories", manual), the result list is rebuilt without normalisation (REM's `term_buildResult`, the interpreter's `buildResult`) — cost if wrong: a REM-only difference on programs no oracle runs correctly.
- Ruling: syntactic unification (175) follows the interpreter (`equalUnify`, `buildMGUResult`: the lower variable bound to the higher, no occurs check, a value followed through bound variables but not substituted inside: `g(v1,a)` / `g(f(v2),v2)` gives `f(v2).a.nil`) — REM has no implementation (`fun_175: TODO`, the C does not compile) — cost if wrong: `synunify` (`.no-rem`).
- Ruling: `replace`/`occurs` (16, 17) follow REM's `term_rec_replace`/`term_occur` (copied path renormalised bottom-up, unchanged subterms shared, the new term not searched) and, for AC terms, the interpreter: an AC sub-multiset is an occurrence (`a U b` in `a U b U c`; `stermreplace` replaces it and searches the rest; REM looks at the elements only) — spec §2.3 (results as the interpreter's) and the interpreter's explicit AC code; no bench or BenchThesis program applies them to AC terms (FastCompletion: lists) — cost if wrong: one branch of `rt.Replace`/`rt.Occurs`, `replace_ac.rem` (REM's line). A builtin string as the container: REM's (equal strings occur; the interpreter's `occursin` never finds anything in a string, but `occur[X,Y]` wants sorts of terms).
- Ruling: `btoi`/`itob` (25, 26) are C's `b ? 1 : 0` and `n != 0` — neither oracle runs them (the interpreter stops: "not yet implemented in the interpreted version"; REM's `fun_25`/`fun_26` return their argument and the program crashes, status 139) — cost if wrong: `btoi.expected` (new in `test_normalise.sh`: the expected line of a program no oracle runs).
- Ruling: a value builtin declared on sorts of terms (`@ pl @ : (t t) t code 3`) is a constructor (the interpreter leaves `a pl a`; REM adds addresses: `r(26382544)`); `and`/`or`/`not`/`==` declared with a result that is not a bool stay refused — cost if wrong: `builtinterms` (`.no-rem`).
- Ruling: `choose try b1 try b2 ... end` makes each branch an alternative of the rest of the rule (branch i, then the conditions after `end` and the right-hand side; when that fails or its solutions are exhausted, branch i+1), not "the first branch whose conditions hold, committed to" as this plan expected — both oracles do so (the interpreter's TRYCHOICE appends the branch to the rest and backtracks into the next branch, `rewrite/stateofexecution.cc`; REM's `Choice.java` sets a choice point per branch): `cafter(3)` = 30 (the condition after `end` fails on branch 1), `(sdk) 1` gives the results of both branches, both oracles agree on every normalisation and dk/one case (`programs/strategies/choose_*`) — cost if wrong: the emission of `CondChoose` (each branch followed by a copy of the rest of the rule).
- Ruling: under `dc`, a labelled rule with a choose gives the results of all its branches (the interpreter: 2 3 12 13 for `(sdc) 1`), not only those of the first branch that has one (REM, `Choice.java` `isDcApply`: 2 3) — spec §2.3; dc keeps the results of the first *rule* that has one, and the branches belong to one rule — cost if wrong: `choose_dc`, `choose_dc2` (`.no-rem`); no bench case uses choose under dc (choice2 is a normalisation).
- Ruling: an identifier prints by its name (the interpreter: `toto`; REM prints `IDENT(454)`, its index, `termOut.c`) — spec §2.3, a name is the value; REM's form is an artefact of its tagged representation — cost if wrong: `bench_g_differs.tsv` (ident1 2.1/3.0/4.0, J FAIL either way) and `rem_idents.py` (test_normalise.sh compares REM's output with the names put back, so the rest of the line, AC order included, is still compared byte for byte).
- Ruling: an identifier is its index in the `.ref` Identifiers table (REM's numbering), compared and ordered by index after the ints and before the strings and the terms (REM's `Gterm_cmp` compares two tagged identifiers by index; it has no defined order between an identifier and another kind) — the order of AC arguments is visible (`idents_ac`: `k(ab)U k(zeta)U k(alpha)U k(beta)U c`, as REM) — cost if wrong: `rt.Compare`'s kind ranks. A name absent from the table gets the free position of REM's stringtab (sum of the bytes as signed C chars modulo 3000, probed by 211; `rt.IdentIndex`, Task 5's queries): REM's runtime parser re-adds the program's names in index order (`tabofidentInit`), so a program whose own names collided could give REM other positions — to check in Task 5 against REM.
- Ruling: under `-noInput`, a start term that uses the query prints REM's two lines (`No ground term is defined`, `Do not use -noInput in this case`) and exits 0, `-quiet` or not, without running anything (`Query.java`) — the interpreter has no `-noInput` — cost if wrong: `noinput_query.stdout` (new in `test_normalise.sh`: the whole expected output, compared with elanc-go's and REM's).
- Ruling: an identifier inside an AC pattern matched by Eker's matcher is refused (`S7e`): REM's order of a tagged identifier among the items of its matchers is not established and no program has one — cost if wrong: a refusal.
- Interfaces: `rt.IdentIndex(name)` (not `rt.Ident`: the name is the print item of an identifier token); `rt.Main(querySort, usesQuery, func(s, query))`; symbols of the program from `rt.FirstSym` (the builtin symbol `ident` is code 4).
- Found by Task 3 (elanc-go bug, fixed): `!=` of `eq[X]` (code 19) on a sort with native values was emitted `!a0 == a1`, i.e. `(!a0) == a1` in Go: a wrong result on bool, a Go compile error on ints and identifiers (`programs/normalise/neqnative`).
- `rt.Replace` takes the dispatcher as `func(sym int32, args []Term) Term` (`rt.Norm`, the generated `normTerm`: the function of each operator that can be stuck, else the term), not `func(T) T`: an AC term must not enter the unique table before it is normalised (G2), so the elements are passed, not a node.

**Results (Tasks 1-3).** Bench G (`bench_parity.py --kind G`): before `PASS=74 PASS~=1 PASS≈=46 FAIL=9 UNSUPPORTED=15`; after Task 1 `PASS≈=53 UNSUPPORTED=8` (rep, bimatch PASS≈); after Task 2 `PASS=78 UNSUPPORTED=4` (choice2 PASS); after Task 3 `PASS=78 PASS~=1 PASS≈=53 FAIL=13 UNSUPPORTED=0` (ident1 FAIL as J; 2.1/3.0/4.0 print `toto`, listed in `bench_g_differs.tsv`; 3.2 prints J's `-noInput` message), no case worse than J. New programs: `normalise/` termops, replace_ac, bimatch, synmatch, synunify, btoi, builtinterms (moved from `refused/`), idents, idents_ac, ident2string, noinput_query, neqnative; `strategies/` choose_* (11); unit tests `elanrt/termops_test.go`, `ident_test.go`, the `-noInput` cases of `driver_test.go`.

**Results (Task 4).** `BENCHTHESIS_KINDS=G tests/compiler/test_benchthesis.sh build/install`: the 15 FastCompletion cases ok at once (output equal to J's, `rewrite_step` equal to J's in all 15: elanc-go tries the same redexes in the same order), no refusal, nothing to fix; the 7 MinelaComp cases `FAIL (a.out status 2)` (query input), skipped in `check-go` by `BENCHTHESIS_G_SKIP` (Makefile) until Task 5. `test_benchthesis.sh` gets `BENCHTHESIS_KINDS` (default `J JO I`), `BENCHTHESIS_SKIP` (printed case names) and the kind G (`elanc-go -v -b`, go cache `tests/compiler/work/gocache`; oracle `expected/J-<out>.out` without its `rewrite_step` line; the result line gives the time, the `go build` time and the steps of G and J). Compile times of `ans` (Apple M4 Max; `main.go` 229 KB, 339 rules): an unchanged program 0.15 s, an empty cache 1.55 s — under the 2 s target; each specification gives another Go module (the 1.5 s first written here for a new program with the runtime cached measured a `go build` outside elanc-go, whose cache keys differ: the runtime was compiled again; Task 5 measures 0.35 s). Run times (`-noInput -quiet`): p8 0.88 s (REM 5.39 s), furtin 0.04 s (0.38), taussky 0.03 s (0.21), the others at most 0.02 s (REM up to 0.11 s); table in `tests/compiler/benchthesis/README.md`.

**Results (Task 5).** The Earley parser of REM's compiled programs is ported to Go, function by function (`src/compiler-go/elanrt/earley`: `lex.go` = ichstream.cc + lstream.cc, `earley.go` = earley.cc + grammar.cc + runtimeInit.cc, `build.go` = esemact + termIn.c computeTerm), with REM's lexem encoding (the order of the lexems orders the expected lexems of an error message). `ir.Grammar` gives REM's gram[] (tested against the gram[] of three REM programs, `ir/testdata/*.gram`); the IR records, per grammar rule, how a query builds its term (`ir.Query`, `QueryRule`); `gen` emits the tables (`grammar`, `sortNames`) and `queryOp`/`queryAC`/`queryIsAC`; `rt.SetQuery` + `rt.Main` read the query without `-noInput` (prompt, parse, `\n`, the term normalised as it is built, the start term, the strategy; a syntax error: REM's message on stderr, status 1; `-sort=`, `-strategy=`, `-REFInput`, `-commandLine`: status 2). Tests (RED then GREEN): `ir/grammar_test.go` (TestGrammarAsREM, TestQueryRules), `gen` TestQueryParserCode, `elanrt/earley` (lexer: tokens, limits, REM's lexical errors and positions; parser: priorities, associativity, injections, overloading, the 17 inputs of `testdata/poly1.rem.json` whose stderr is REM's byte for byte, the ambiguity warning, the MAXLENNTERM limit, AC flattening), `elanrt/query_test.go` (TestMainQuery: prompt, `-quiet`, empty input, lexical error, refused options). The lexer and parser tests were written after their code (the port was written first, function by function, then checked against REM's runs); every other test was seen failing first. `tests/compiler-rs/test_query.sh`: 46 programs, 142 queries, each run separately, stdout byte-identical to REM's (`rewrite_step` excluded), the same stderr and status, results equal to the interpreter's (AC terms canonical, `programs/query/canon`): `tests/compiler/programs/start_term` (7 × 3), `runtime_strategies` (19 × 3), the 13 `examples/` (every query of `input.inp`), `programs/query/` assoc (priorities, left/right/no associativity, injections of int and string, overloading of `f` on two sorts, AC), overload (numbers, negative numbers, `` `a` ``), strings (escapes), ident, newident (identifiers absent from the program under an AC operator: REM's order), errors (syntax errors, unknown identifier, empty input, EOF without `end`, `end` alone, comments and inlines), collide and bigint (`.no-rem`). BenchThesis G: 22 ok (the 7 MinelaComp cases read their query; steps equal to J's), `BENCHTHESIS_SKIP` removed from `check-go`, `test_query.sh` added. Bench G as saved (`PASS=78 PASS~=1 PASS≈=53 FAIL=13 UNSUPPORTED=0`). Compile times with elanc-go (Apple M4 Max, go build cache holding the runtime): `ans` (FastCompletion, `main.go` 390 KB with the query tables) 0.33-0.38 s, `rewritingC` (MinelaComp, 375-575 KB) 0.35-0.37 s for a new program (`go build` 0.27-0.29 s), 0.10-0.13 s unchanged, 1.7 s with an empty cache; run times p8 0.88 s (REM 5.39 s), MinelaComp primes8 0.05 s (REM 0.29 s).

**Rulings (Task 5).**
- Ruling: the identifiers of the program keep their `.ref` indices at run time, and a name read from a query that the table does not have gets REM's stringtab position in that table (the open point of Task 3) — REM's runtime rebuilds the table by re-adding the names in index order (`tabofidentInit`), which swaps two names of one hash position whose probe sequence wraps around: `programs/query/collide` (anagrams of hash 2900, the second at 111) gives `r(false,true)` with REM, `r(true,false)` with the interpreter and elanc-go (the query names the first one); without such a collision, REM's positions are these (`newident`, `assoc`: AC orders equal to REM's) — cost if wrong: `collide.no-rem`, `rem_idents.py --query` (which places the query's names the same way).
- Ruling: at the end of the input before a complete term, the syntax error is reported at the end (`'ENDOFLSTREAM'`, the lexems expected after the last one) — REM advances its completed situations past the end of their rule there (`earleyaddnewsits(NOLEXEM)`: an extra round, then `'\0'` read from uninitialised memory in the list) — cost if wrong: the text of the message (status 1 either way); the `deriv(X` case of TestParsePoly1AsREM.
- Ruling: a string lexem in an error message prints `STRING` (any string) or the string read, quoted — REM prints `(null)`: it reads them past its table of identifiers (`lexem::alfsy`) — cost if wrong: `remstrings` in `test_query.sh` (REM's `(null)` compared with ours mapped).
- Ruling: a number above 2^31-1 stays a number lexem (its value wrapped), as the plan said — both oracles reject it (`sort(2147470386)`, the lexem becomes a nonterminal: followups) — cost if wrong: `programs/query/bigint` (`.no-rem`, `.no-interpreter`, `.expected`).
- Ruling: the term of a query is built bottom-up by the functions of its operators (each operator normalised when its arguments are), the arguments of an AC operator flattened first when they have its symbol (`term_add_onf_term`) — REM builds the whole term then normalises it (innermost, the same redexes) — evidence: every query of `test_query.sh` equal to REM and the interpreter, MinelaComp's steps equal to J's — cost if wrong: `rewrite_step` counts.
- Ruling: the operators a query can contain (the grammar rules of the sorts reachable from the query's sort, the three dumps as REM's gram[]) are compiled in every program, a query being read without `-noInput`; one whose compilation is refused is refused when a query contains it (`queryUnsupported`, status 2), and when the whole set stops the program from building, the program is compiled as before with every query operator refused at run time — so no `-noInput` program gets worse (bench G unchanged) — cost if wrong: a query refused at run time instead of a program refused at compile time.
- Ruling: the syntax errors, lexical errors and ambiguity warnings go to stderr — REM writes them on `sterr` (the plan said stdout; REM run on poly1); REM's runtime prints the ambiguity warnings (`in_runtime` is 0), so does elanc-go (`1+2*3` of poly1 between `@ : (int) poly` and `@ + @`) — cost if wrong: none (byte-identical to REM).
- An empty input is no query for the interpreter (it stops silently): `test_query.sh` compares it with REM only (`<EOF>`).
- Interfaces (differences from the plan): `ir.Grammar(p) ([]GrammarRule, error)` (a String lexem, which REM does not read, is an error); `earley.Tables` takes the identifier table as functions (`IdentIndex`, `IdentName`: elanrt's, so the indices of the parser and of the terms are one table) and `SortName`; `earley.Parse(r, t, sort, stderr)`; `earley.Builder[T]` is generic (no elanrt import: elanrt imports earley) and receives the grammar rule (`Op(r, args)`, `AC(r, elems)`, `IsAC(r)`), whose operator profile the IR knows (REM keeps the symbol only and dispatches on tags); `rt.QueryParser` + `rt.SetQuery`, `rt.Main` unchanged.

## File structure

| file | responsibility |
|---|---|
| `src/compiler-go/ir/ir.go` | `BuiltinOp` gets `TermReplace`, `TermOccurs`, `TermMatch`, `TermUnify`, `IdentEq`, `IdentNe`, `IdentToString`, `BoolToInt`, `IntToBool`; `CondKind` gets `CondChoose`; `StratKind` gets `StTAll`, `StTOne`, `StTSome`, `StRewrite`, `StNormIn`, `StNormOut`; `Start` gets `UsesQuery`, `Grammar` |
| `src/compiler-go/ir/lower.go` | lowering of the new constructs (refusals removed one by one) |
| `src/compiler-go/ir/grammar.go` (new) | the parser tables from `ref.Grammars` (REM's `gram[]` order) |
| `src/compiler-go/gen/gen.go`, `gen/strat.go` | emission: builtin calls, choose/try, traversals, query start, tables |
| `src/compiler-go/elanrt/termops.go` (new) | `Replace`, `Occurs`, `SyntacticMatch`, `SyntacticUnify` |
| `src/compiler-go/elanrt/ident.go` (new) | identifier table (`InitIdents`, `Ident`, `IdentName`, REM hash) |
| `src/compiler-go/elanrt/earley/` (new package `elanrt/earley`) | lexer (`lex.go`), grammar and Earley (`earley.go`), errors (`errors.go`), term building (`build.go`) |
| `src/compiler-go/elanrt/driver.go` | query mode: read, normalise, start term, strategy; the `-noInput` message |
| `tests/compiler-rs/programs/builtins/`, `programs/query/`, `programs/strategies/` (new cases) | dedicated programs |
| `tests/compiler-rs/test_query.sh` (new) | compiled programs reading their query, against the interpreter and REM |
| `tests/compiler-rs/bench_parity.py`, `bench_g_differs.tsv` (new) | allowed differences from the J snapshot |
| `tests/compiler/test_benchthesis.sh` | kind G (elanc-go) |
| `Makefile` | `check-go` runs the new scripts |

---

### Task 1: Term builtins (replace, occurs, syntactic matching and unification), btoi/itob

Bench: rep (4) and bimatch (3) leave UNSUPPORTED (15 → 8).

**Files:**
- Modify: `src/compiler-go/ir/ir.go` (`builtinCodes`, `BuiltinOp`, `NeedsValues`), `src/compiler-go/ir/lower.go:1188-1207` (the "builtin operation on sorts that are not builtin" check: term builtins are allowed on any sort), `src/compiler-go/gen/gen.go` (calls)
- Create: `src/compiler-go/elanrt/termops.go`, `src/compiler-go/elanrt/termops_test.go`
- Create: `tests/compiler-rs/programs/normalise/termops.{lgi,eln}`, `replace_ac.{lgi,eln}`, `bimatch.{lgi,eln,no-interpreter}`, `btoi.{lgi,eln}`
- Modify: `tests/compiler-rs/programs/refused/builtinterms.*` (see Step 6)

**Interfaces:**
- Produces: `ir.TermReplace` (code 16, args (t1 t2 t3) → `replace t1 by t2 in t3`), `ir.TermOccurs` (17), `ir.TermMatch` (191), `ir.TermUnify` (175), `ir.BoolToInt` (25), `ir.IntToBool` (26); all with `NeedsValues() == false` except 25/26.
- Produces (elanrt): `func Replace(t, old, new T, norm func(T) T) T` (the copied path renormalised by `norm`, the generated normalisation dispatcher), `func Occurs(a, b T) bool` (a occurs in b), `func SyntacticMatch(pat, subj, vars, fail T) T`, `func SyntacticUnify(pat, subj, vars, fail T) T`.

- [x] **Step 1: Write the failing tests.** `termops_test.go`: `Replace` on a free term (`f(a, g(a))`, a → b gives `f(b, g(b))`, unchanged subterms shared: same pointer), on an AC term (`a U b U a` with a → c: `c U c U b` in canonical order), ints and strings left; `Occurs(a, f(g(a)))` true, `Occurs(f(a), a)` false; `SyntacticMatch(f(var(1)), f(a), cons(var(1), nil), fail)` = `cons(a, nil)`, no match → `fail`, non-linear `f(var(1),var(1))` vs `f(a,b)` → `fail`. Programs: `termops.lgi` (start term `go` whose rules use replace, occurs, both on a sort with an AC operator), `replace_ac.lgi` (Review Focus 5: the replaced term has an AC subterm that a rule of the AC operator rewrites after the replacement), `bimatch.lgi` (copy of the bench program; `.no-interpreter`: "the interpreter returns fail, REM and the 2000 output `.(a,nil)` agree on a.nil", after Step 2's analysis), `btoi.lgi` (`btoi(true) + itob(0)` cases).
- [x] **Step 2: Analyse the bimatch difference.** Run `echo 'a end' | elan -b bimatch.lgi` (prints `fail`) and REM; read `interpreter/term/term.cc:1828` (`syntacticMatching`, `equalMatch`) and `compiler/runtime/builtinMatching.c`; write the cause in `bimatch.no-interpreter` and a `docs/followups.md` entry (interpreter bug, not fixed here).
- [x] **Step 3: Run the tests to see them fail.**
  Run: `cd src/compiler-go/elanrt && go test -run 'Replace|Occurs|Syntactic' ./...` — Expected: FAIL (undefined: Replace).
  Run: `ELANC=elanc-go tests/compiler-rs/test_normalise.sh build/install termops replace_ac bimatch btoi` — Expected: FAIL `refused by elanc-go: not supported yet (S7d): builtin operation code 16`.
- [x] **Step 4: Implement.** elanrt functions as REM's (`termCommon.c:777` `term_rec_replace`, `term_occur`, `builtinMatching.c`; equality by `rt.Eq`); IR: term builtins accepted on any sort; gen: a call `rt.Replace(a0, a1, a2, normT)` where `normT` normalises a term of any sort (the generated dispatcher used by META; add it if only per sort exists).
- [x] **Step 5: Run the tests.** Same commands — Expected: PASS; `go vet` clean.
- [x] **Step 6: builtinterms.** A value builtin (`NeedsValues`) declared on sorts of terms (`@ pl @ : (t t) t code 3`) is compiled as a constructor (the interpreter leaves `a pl a`); move `programs/refused/builtinterms` to `programs/normalise/` (keep `.no-elanc-rs`).
- [x] **Step 7: Bench.** `python3 tests/compiler-rs/bench_parity.py build/install --kind G` — Expected: `UNSUPPORTED=8`, rep PASS≈ (×4), bimatch PASS≈ (×3), no `PARITY:` line; then `--save`.
- [x] **Step 8: Commit.**
```bash
git add src/compiler-go tests/compiler-rs docs/followups.md
git commit -m "elanc-go: replace, occurs, syntactic matching and unification, btoi/itob (S7d task 1)

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
```

### Task 2: choose/try conditions

Bench: choice2 (4) leaves UNSUPPORTED (8 → 4).

**Files:**
- Modify: `src/compiler-go/ir/ir.go` (`CondChoose` with `Branches [][]Cond`), `ir/lower.go:894` (`ref.Try`) and `:2132` (rule lowering), `gen/gen.go` and `gen/strat.go` (emission of conditions)
- Create: `tests/compiler-rs/programs/strategies/choose_*.lgi` + `choose.eln`

**Interfaces:**
- Consumes: the condition emission of S7b (`Cond`, backtracking continuations).
- Produces: `ir.CondChoose` (`Cond.Branches [][]Cond`).

- [x] **Step 1: Establish the semantics with both oracles.** Programs (one per case, start term ground): first branch succeeds; first fails, second succeeds; all fail (the rule fails, next rule tried); a branch with a where binding a variable used by the right-hand side; a branch whose where has several solutions under `dk` (labelled rule: are later solutions of the same branch tried? are later branches tried after the first branch gave a result?); conditions after `end`; nested choose. Run `elan -b` and REM (`elanc -b -nosplit -quiet`, `./a.out -noInput -quiet`) on each; write the rule in the `CondChoose` doc comment (expected from `rewrite/` and REM `StrategyChooseRule.java`: the first branch whose conditions all hold is committed to, its solutions enumerated, the others not tried).
- [x] **Step 2: Run to see them fail.** `ELANC=elanc-go tests/compiler-rs/test_strategies.sh build/install choose_first choose_second choose_none choose_where choose_dk choose_nested` — Expected: FAIL `not supported yet (S7d): choose/try conditions`.
- [x] **Step 3: Implement** the lowering (each branch's wheres typed and lowered as the rule's conditions, variables bound in a branch available after `end`) and the emission (try branch i: on its first solution commit, else branch i+1, else fail).
- [x] **Step 4: Run** the same command — Expected: PASS (results equal to the interpreter's, `result =` lines byte-identical to REM's).
- [x] **Step 5: Bench.** Expected `UNSUPPORTED=4`, choice2 PASS (×4); `--save`.
- [x] **Step 6: Commit** (`elanc-go: choose/try conditions (S7d task 2)`, with the Co-Authored-By line).

### Task 3: Identifiers; start terms using the query under -noInput

Bench: ident1 (4) leaves UNSUPPORTED (4 → 0).

**Files:**
- Create: `src/compiler-go/elanrt/ident.go`, `ident_test.go`
- Modify: `ir/lower.go:314` (`ref.Ident`), `:483` (start term with the query), `:522-526` (sort `ident`), `ir/ir.go` (`IdentEq` 14, `IdentNe` 15, `IdentToString` 177; a builtin family `ident`: native `int32` index), `gen/gen.go` (identifier table, patterns on identifier literals, the `$x` variables), `elanrt/print.go` (an ident prints its name), `elanrt/driver.go` (`Main` gets the query mode)
- Create: `tests/compiler-rs/programs/normalise/idents.{lgi,eln}`, `ident2string.{lgi,eln}`, `programs/normalise/noinput_query.{lgi,eln}` (start term `s(query)`)
- Create: `tests/compiler-rs/bench_g_differs.tsv`; modify `tests/compiler-rs/bench_parity.py`
- Modify: `tests/compiler-rs/programs/refused/queryinput.lgi` (no longer refused at compile time: delete it; its run-time behaviour is tested by Step 1)

**Interfaces:**
- Produces (elanrt): `func InitIdents(names map[int32]string)` (the program's `Identifiers` table), `func Ident(name string) int32` (existing index, else the REM hash position: sum of bytes mod 3000, probe +211 mod 3000), `func IdentName(i int32) string`.
- Produces (ir): `Start.UsesQuery bool`, `Start.QueryVar int` (the variable bound to the query); `elanrt.Main(querySort string, usesQuery bool, program func(s *Session))`, the start function taking the query term (`func(s *Session, query rt.T)`; nil under `-noInput` when `!usesQuery`).

- [x] **Step 1: Write the failing tests.** `ident_test.go`: `Ident` of a name of the table returns its index; of a new name the stringtab position (cases computed by hand: `"toto"` absent from an empty table → (116+111+116+111) mod 3000 = 454, matching ident1's `IDENT(454)`; a collision probes by 211). Programs: `idents.lgi` (identifier literals in patterns and right-hand sides, `==`/`!=` on idents, an identifier under an AC operator: order by index as REM), `ident2string.lgi`, `noinput_query.lgi`: `./a.out -noInput -quiet` must print exactly `No ground term is defined\nDo not use -noInput in this case\n` and exit 0 (REM's output, Review Focus 4); without `-noInput` (stdin `1 end`) status 2 and "reading a query is not supported yet" until Task 5.
- [x] **Step 2: Run to see them fail.** `go test -run Ident ./...` (elanrt) — FAIL undefined; `ELANC=elanc-go tests/compiler-rs/test_normalise.sh build/install idents ident2string noinput_query` — FAIL `builtin identifiers (sort ident)` / `query input`.
- [x] **Step 3: Implement** (idents as a native builtin family like builtinInt, boxed in nodes with a distinct tag so `rt.Compare` orders them by index after ints, as `Gterm_cmp`); the start function binds the query variable; under `-noInput` with `UsesQuery` the driver prints REM's two lines and exits 0.
- [x] **Step 4: Run** the same commands — Expected: PASS.
- [x] **Step 5: Allowed differences from J.** `bench_g_differs.tsv`: `<G case id>\t<reason>` for `Compiler.{2.1,3.0,4.0}/Test::G::ident1:no:-:ident1` ("REM prints identifiers as IDENT(n), the interpreter and elanc-go by name: toto"); `bench_parity.py` accepts SNAP-DIFF for a listed case (the status rule still applies) and reports a listed case that is no longer SNAP-DIFF ("stale entry"). Test: `python3 tests/compiler-rs/bench_parity.py build/install --kind G` — Expected: `UNSUPPORTED=0`, ident1 2.1/3.0/4.0 FAIL (J FAIL), 3.2 FAIL with J's output, no `PARITY:` line; `--save`.
- [x] **Step 6: Commit** (`elanc-go: identifiers, start terms using the query under -noInput (S7d task 3)`).

### Task 4: BenchThesis with elanc-go (kind G), FastCompletion

**Files:**
- Modify: `tests/compiler/test_benchthesis.sh` (kind G), `tests/compiler/benchthesis/README.md`, `Makefile` (`check-go`)

**Interfaces:**
- Produces: `BENCHTHESIS_KINDS` (default `J JO I`; `G` alone in `check-go`); kind G runs `elanc-go -b LGI SPC` (shared `--gocache`, as `test_strategies.sh`), then the same command as J; its oracle is `expected/J-<out>.out` with the `rewrite_step = N` line removed on both sides (the count depends on the compilation scheme, as in the bench); the G steps are written to the result line for information.

- [x] **Step 1: Add kind G** with the 15 FastCompletion cases (`G FastCompletion ans <spc> - j<spc>`) and the 7 MinelaComp cases, which fail until Task 5 (`G MinelaComp rewritingC ...`).
- [x] **Step 2: Run.** `BENCHTHESIS_KINDS=G tests/compiler/test_benchthesis.sh build/install` — Expected: 15 `ok` (FastCompletion), 7 `FAIL ... (a.out status 2)` (MinelaComp: query input). If a FastCompletion case is refused or differs, fix it in this task (new refusal: implement or explain; difference: compare with the interpreter's `I-i<spc>.out` and REM's).
- [x] **Step 3: Measure** each case: `elanc-go` compile time (cold and warm cache; spec target under 2 s warm) and run time against REM's (README "Timings": p8 6 s); write the table in the README.
- [x] **Step 4: Wire `check-go`** to run `BENCHTHESIS_KINDS=G tests/compiler/test_benchthesis.sh` with a `BENCHTHESIS_SKIP` list for the 7 MinelaComp G cases (removed in Task 5).
- [x] **Step 5: Run** `make check-go` — Expected: green.
- [x] **Step 6: Commit** (`tests: BenchThesis compiled by elanc-go, kind G (S7d task 4)`).

### Task 5: Query input — Earley parser port

**Files:**
- Create: `src/compiler-go/ir/grammar.go`, `ir/grammar_test.go`
- Create: `src/compiler-go/elanrt/earley/{lex.go,earley.go,errors.go,build.go}` and tests
- Modify: `src/compiler-go/gen/gen.go` (emit the tables), `elanrt/driver.go` (query mode)
- Create: `tests/compiler-rs/test_query.sh`, `tests/compiler-rs/programs/query/*`
- Modify: `Makefile` (`check-go`), `tests/compiler/test_benchthesis.sh` (remove `BENCHTHESIS_SKIP`)

**Interfaces:**
- Consumes: `ref.Program.Grammars` (Both, GlobalOnly, TopOnly), `ref.Operator` (`Symbol`, `Priority`, `Syntax`, `Rhs []Lexem`), `ref.Program.Sorts`, `Identifiers`; Task 3's `rt.Ident`.
- Produces (ir): `type GrammarRule struct { Sort int32; Symbol int32; Info int32 /* priority + assoc<<12 */; Rhs []GrammarLexem }`, `type GrammarLexem struct { Kind uint8 /* 0 ident, 1 sort, 2 char, 3 number */; Code int32 }`, `func Grammar(p *ref.Program) []GrammarRule` (REM's `gram[]`: the three dumps in file order); `ir.Start.QuerySort`, `QueryStrategy`.
- Produces (elanrt/earley): `type Tables struct { Rules []Rule; Idents map[int32]string; Sorts map[int32]string; InternIdent, InternInt, InternString int32 /* looked up by name: "intern ident", "intern int", "intern string" */ }`, `func Parse(r io.Reader, t *Tables, sort int32) (Tree, error)` (the error's text is REM's message), `type Builder interface { Op(sym int32, args []rt.T) rt.T; Int(int32) rt.T; Ident(int32) rt.T; Str([]byte) rt.T }`, `func Build(tr Tree, b Builder) rt.T` (AC symbols flattened by the generated constructors).

- [x] **Step 1: Grammar tables test.** `grammar_test.go`: on `tests/golden` `.ref` files, `ir.Grammar` equals REM's `gram[]` for the same program (generate REM's C once with `elanc -b -nosplit` for 3 golden programs, extract `int gram[]` from `.elan.<prog>/*.c`, check it in as `ir/testdata/<prog>.gram`). Run `go test -run Grammar ./ir` — FAIL undefined.
- [x] **Step 2: Implement `ir.Grammar`**; the test passes.
- [x] **Step 3: Lexer tests** (`lex_test.go`): identifiers, numbers (2147483647; 2147483648 wraps as the interpreter's reader, but stays a number lexem — `docs/followups.md` "Integer literals above 2^31-1"), strings with escapes, special characters one per lexem, `end`, EOF; port `lex/lstream.cc`/`earley/lstream.cc`, `lbuffer.cc`, `ichstream.cc`.
- [x] **Step 4: Earley tests** (`earley_test.go`) on hand-written tables: priorities (`a + b * c`), left/right associativity, an injection chain, overloading resolved by sort, an ambiguity (REM's warning text on stderr), a syntax error (REM's message, byte for byte, from a REM run on the same grammar), EOF without `end`. Port `earley.cc` and `grammar.cc` function by function (`earleycall`, `addtosit`, `completesit`, `solveconflicts`, `solveconflict2`, `earleysecondpass`, `oearleyerr`), keeping the C names in comments.
- [x] **Step 5: Driver.** Query mode as REM (`termIn.c` mode 2): prompt (also with `-quiet`), parse, `\n`, normalise, start term, start strategy; parse error: message on stdout, status 1. `-strategy=`/`-sort=`/`-REFInput`/`-commandLine` stay unsupported (exit 2 with a message).
- [x] **Step 6: test_query.sh** (`ELANC` as the other scripts): for each program and each query, the compiled program's whole stdout must be byte-identical to REM's (`elanc -b -nosplit -quiet`, `./a.out -quiet < query`, `rewrite_step` line excluded unless `rem-steps`), its `result =` lines equal to the interpreter's results (` t end` lines of `elan -b`). Programs: `tests/compiler/programs/start_term/*` (queries 5, 7, 2147483647), `runtime_strategies/*` (3, 95, 200), `examples/*` (each query of `input.inp` run separately; the specification passed), `programs/query/`: `mixfix` (poly1's `3*X*X + 2*X + 7`), `assoc`, `overload`, `inject`, `strings`, `negative`, `newident` (an identifier absent from the program, under an AC operator: REM's order), `acquery` (a query `c U a U b`), `syntax_paren` and `syntax_unknown` (errors, status 1; Review Focus 2), `eof` (empty stdin; Review Focus 3).
- [x] **Step 7: Run.** `ELANC=elanc-go tests/compiler-rs/test_query.sh build/install` — Expected: all ok (each exception documented by a `.no-rem`/`.no-interpreter`). `BENCHTHESIS_KINDS=G tests/compiler/test_benchthesis.sh build/install` — Expected: 22 ok (MinelaComp included); remove `BENCHTHESIS_SKIP`; add `test_query.sh` to `check-go`.
- [x] **Step 8: Commit** (`elanc-go: query input, Earley parser ported from the compiled runtime (S7d task 5)`).

### Task 6: Traversals, rewrite, normin/normout

**Files:**
- Modify: `ir/ir.go` (`StTAll`, `StTOne`, `StTSome`, `StRewrite`, `StNormIn`, `StNormOut`), `ir/lower.go:786`, `gen/strat.go`, `elanrt/strat.go`
- Move: `tests/compiler-rs/programs/refused/traversal.lgi`, `runtimetraversal.lgi`, `later.eln`, `laterstrat.eln` to `programs/strategies/` (each with `.no-interpreter`: "the interpreter does not interpret traversals: `Traversal is not intepreted`, stateofexecution.cc:873"); new `programs/strategies/trav_*.lgi`

**Interfaces:**
- Produces (elanrt): `func TAll(t T, s func(T, K) bool, k K) bool`, `TOne`, `TSome` (on the arguments of t, AC arguments with their multiplicities as REM's `genericGetMult`, the rebuilt term normalised by `specialApply`), `Rewrite`, `NormIn`, `NormOut`.

- [ ] **Step 1: Semantics from REM** (`rem/compiler/StrategyTraversalAll.java`, `...One`, `...Some`, `StrategyRewriteStep.java`, `StrategyNormInRule.java`, `StrategyNormOutRule.java`): write each in the doc comment of its elanrt function (results and their order; tall with a non-deterministic sub-strategy; tone: first argument where s succeeds? all?; constants; AC arguments).
- [ ] **Step 2: Programs** `trav_all`, `trav_all_fail`, `trav_one`, `trav_some`, `trav_ac`, `trav_const`, `rewrite_step`, `normin`, `normout`, `runtimetraversal` (`[tone(inc)]3`); run `ELANC=elanc-go tests/compiler-rs/test_strategies.sh build/install trav_all ...` — Expected: FAIL `strategy tall`.
- [ ] **Step 3: Implement**; Step 2's command — Expected: PASS (byte-identical to REM).
- [ ] **Step 4: Commit** (`elanc-go: traversals, rewrite, normin/normout as REM (S7d task 6)`).

### Task 7: The remaining refusals

**Files:** `ir/lower.go` (each remaining `unsupported("S7d", ...)`), `gen/meta.go`, `tests/compiler-rs/programs/{strategy_terms,refused}`, `docs/followups.md`

- [ ] **Step 1: Inventory.** `grep -n 'unsupported("S7d"' src/compiler-go/ir/*.go`; for each: an oracle (interpreter, REM) and a program, or a reason to keep the refusal.
- [ ] **Step 2: Labels named by strings in Meta_strat** (codes 133-136, `ms_*` programs of `programs/strategy_terms`): implement as the interpreter (`meta/meta.cc` term2strategy); test with `-d strategy_terms`.
- [ ] **Step 3: switch/case rule bodies** (SWRULE; REM cannot read them: `.no-rem`): implement as the interpreter; `programs/normalise/switch.lgi`.
- [ ] **Step 4: Kept refusals, renamed with a stage that says why** (`unsupported("S7e", ...)` or a "not planned" stage the scripts accept): builtin operations of native sorts under AC (`refused/acbuiltin`: strings under AC make both oracles fail), doubles (codes 34-78: REM has none), I/O and arrays (113-123, 200-203), meta/ref (160-174), `call` of another sort (`refused/callother`, interpreter bug), `call` whose name is a variable (exporter fatal). Each listed in `docs/followups.md`.
- [ ] **Step 5: Run** `make check-go` — green; commit (`elanc-go: labels by strings in META, switch rules; the kept refusals (S7d task 7)`).

### Task 8: Bench, review, documentation

- [ ] **Step 1:** `python3 tests/compiler-rs/bench_parity.py build/install --kind G --save` — Expected: `UNSUPPORTED=0`, every case at J's status or better, the only SNAP-DIFFs those of `bench_g_differs.tsv`.
- [ ] **Step 2:** `make check` green (Go, Rust, compiler, golden, bench I/A/J/JO).
- [ ] **Step 3:** Results in this plan (one "Task N results" paragraph per task, as S7c), spec §5 S7d marked done with its decisions (query parser: Earley port), `docs/followups.md` updated (bimatch interpreter bug, identifier printing in REM, kept refusals).
- [ ] **Step 4:** Whole-branch review; fixes, each with its test; merge into main.

## Acceptance (S7d)

- Bench: every case elanc compiles, elanc-go compiles, status no worse than J (`bench_parity.py --kind G`: UNSUPPORTED 0), output equal to J's snapshot except `bench_g_differs.tsv`.
- J/JO: the G kind runs every jtest program of the bench (J and JO compile the same programs; elanc-go has no `-optimiseChoicePoint`).
- BenchThesis: `BENCHTHESIS_KINDS=G tests/compiler/test_benchthesis.sh` 22 cases ok (15 FastCompletion with `-noInput`, 7 MinelaComp reading their query), in `make check-go`.
- Query input: `test_query.sh` green (start_term, runtime_strategies, examples, programs/query).
- `make check` green.

## Risks

- The Earley port reproduces REM's error messages and ambiguity resolution only if it follows the C++ closely; differences show up as `test_query.sh` failures on error cases. Mitigation: port function by function, tables tested against REM's `gram[]`.
- Identifier indices: REM's AC order and `IDENT(n)` depend on the hash positions; a new identifier read from a query must get REM's position (Task 3 test) or AC orders differ.
- choose/try and traversal semantics are known only from the oracles' code; where the interpreter and REM disagree (traversals: the interpreter does not run them), REM is the only oracle.
- BenchThesis compile time (339-399 rules, 700 grammar rules emitted as tables) may exceed the 2 s target; measured in Task 4, optimisation left to S7e.
- bimatch: following REM against the interpreter is justified by the 2000 output only; if Task 1's analysis shows the interpreter right, the case goes to `bench_g_differs.tsv` instead.
