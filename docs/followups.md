# Follow-ups

Known issues and deferred work, collected from the reviews of each
sub-project (S1–S5b). Each item says where it comes from.

## Behaviour (needs tests against the interpreter or the reference)

- **Local alias as printing form** (`aliasprint`): REM (`termOut.c
  findTextForm`) and elanc-rs print an operator through a local alias
  declared after it (`cat(a,b)` for `a ++ b`, `hh(a,b)` instead of the
  global alias `hh2`); the interpreter prints the grammar rule marked
  Printable, and elanc now follows it.
- **`call "name":X` in compiled programs** (`ref.cc`, review of branch
  `compiled-start-term`): the `.ref` export resolves the name with any sort
  (the sort printed with the shared `call` symbol, code 144, is the last
  instance declared); a compiled program with two `strat[]` instances may
  still run the `eval` of another sort, where the interpreter now uses the
  sort of the term (`meta.cc`, robot example).
- **`if b then S1 else S2 fi` compiled with `-strategy 2`** tests
  `GgetSymb(cond)` (any term other than `false` is true), where the
  interpreter fails on a condition that is neither `true` nor `false`.
- **Compiler tests**: a case whose interpreter and compiled outputs are
  both empty passes (`test_start_term.sh`, `test_runtime_strategies.sh`);
  the level-2 marker is the same for every StrategyEval path; no AC,
  string, `-commandLine`, `-REFInput` or `-coq` start-term case.
- **`Compiler.2.1/BenchThesis` in the legacy bench**: the 30 J/JO cases (and
  the I/A cases) of `legacy/` still report `ERROR`/`FAIL` there (ELAN 2.1
  syntax: `rewrite` is a keyword in ELAN 3; `legacy/` is not modified). The
  ported copies in `tests/compiler/benchthesis/` (identifier `rewrite`
  renamed `rewriting`) run in `make check` (`test_benchthesis.sh`, 64 cases,
  reference outputs as oracle, modern = reference byte for byte). Remaining
  differences with the 2000 outputs: printing, `rewrite_step` counts
  (+7–14 %), `group` (2000 output made with another `group.spc`) and the
  binding order of MinelaComp `iappend` (see
  `tests/compiler/benchthesis/README.md`).
- **Integer literals above 2^31-1** wrap modulo 2^32 when read; one that
  wraps to a negative value (e.g. `2147483648`) is then taken by the lexer
  for a lexem of another kind (`sort(...)`), and the query is rejected
  (interpreter `lex/lstream.cc`, compiled runtime `earley/lstream.cc`).
  elanc-go's query parser (S7d task 5) keeps it a number (the value
  wrapped): `programs/query/bigint` (`.no-rem`, `.no-interpreter`).
- **`call "name":X` with a name of another sort only** (interpreter,
  found by S6b): `set_of(call "two":state, st(1))`, where only `two:int`
  exists, applies the int strategy to a state and prints the ill-sorted
  `cons_state(plus(st(1),1),nil)` (`tests/compiler-rs/programs/refused/callother`);
  elanc-rs refuses it.
- **Tail-call loops may terminate where the interpreter does not**
  (elanc-go S7b, elanc-rs S6b; `ir.TailCalls`, `Ir::tail_calls`): the loop
  of a deterministic strategy calling itself in tail position stops the
  search after its single result, so the alternatives that would run after
  it are never started. `me => first(dc(dec) ; me, iterate*(spin ; fail))`
  with `spin => dc(inc) ; spin`, on 3: both compilers print 0 and exit;
  `elan -b` prints 0 then loops for ever (the `iterate*` of the outer
  calls never ends). Same results, in the same order; documented, not
  changed (the interpreter's search after the last result is unobservable
  except by non-termination).
- **`call` whose name is a variable** (`[] mk(s) => call s : int`, review
  of S7b): `elan --cexport` stops with `[fatal] Call type name not found`
  (exporter, `src/interpreter/ref/ref.cc`), so elanc-go and elanc-rs exit
  with status 1 instead of refusing the program (status 2, "not supported
  yet"); the interpreter runs it. To do with the calls by name, S7d (S6d).
- **Interpreter: `builtinSyntacticMatching` fails on every function
  symbol** (found by S7d, bench `bimatch`): `term::equalMatch`
  (`src/interpreter/term/term.cc`) ends the `TNORMFS` case with `break`
  after matching the arguments, so it returns 0: only a variable or a
  constant pattern matches (`f(var(1))` against `f(a)` gives `fail`, even
  `g(a,b)` against `g(a,b)`). REM (`builtinMatching.c`) and the 2000 output
  (`.(a,nil)`) agree on `a.nil`; elanc-go follows them
  (`tests/compiler-rs/programs/normalise/bimatch`, `synmatch`,
  `.no-interpreter`). Not fixed (the interpreter does not change in S7d).
- **REM: term builtins** (found by S7d): `term_equalMatch` compares two
  strings with `return strcmp(...)` (true when they differ) and refuses a
  variable whose value would be an int, an identifier or an AC term (the
  tag tests come before the variable lookup); `builtinSyntacticUnification`
  (175) is not implemented (`fun_175: TODO`: the C does not compile);
  `btoi`/`itob` (25, 26) return their argument (a boolean constant read as
  an int: the program crashes), and the interpreter stops on them ("not yet
  implemented in the interpreted version"); a value builtin declared on a
  sort of terms (`@ pl @ : (t t) t code 3`) adds addresses. elanc-go:
  matching as REM with these fixed, unification as the interpreter, btoi
  1/0 and itob `n != 0`, the value builtin a constructor (S7d plan, Task 1
  rulings).
- **replace/occurs on AC terms differ between the oracles** (S7d): the
  interpreter finds an AC sub-multiset (`a U b` occurs in `a U b U c`,
  `stermreplace`/`occursin`), REM the elements only (`term_rec_replace`,
  `term_occur`); elanc-go follows the interpreter
  (`programs/normalise/replace_ac.rem`).
- **REM prints identifiers as `IDENT(n)`** (`src/compiler/runtime/termOut.c`,
  n the index of the identifier in the `.ref` Identifiers table): the J
  snapshots of `ident1` (Compiler 2.1/3.0/4.0) are `result = IDENT(454)`,
  the interpreter and elanc-go print `toto`
  (`tests/compiler-rs/bench_g_differs.tsv`; `test_normalise.sh` puts the
  names back in REM's output, `rem_idents.py`). REM needs at least one
  rule in a program (`ParseException: Encountered "EndDef"`).
- **REM: the identifiers of a query can get other indices than the
  program's** (found by S7d task 5): the compiled runtime rebuilds its
  table of identifiers by adding the names of the program in index order
  (`src/compiler/earley/runtimeInit.cc` `tabofidentInit`), so two names
  of the same hash position whose probe sequence wraps around the table
  (position + 211 >= 3000) are swapped, and a query naming one gets the
  index of the other (`tests/compiler-rs/programs/query/collide`: REM
  `r(false,true)`, the interpreter and elanc-go `r(true,false)`). The same
  re-adding puts the empty string in position 0 or on its probe chain.
  elanc-go keeps the indices of the `.ref` file.
- **REM: the Earley parser of the compiled programs reads past its
  arrays** (found by S7d task 5): an error message prints a string lexem
  (`STRING`, a string read) through the table of identifiers
  (`lexem::alfsy`, index 3000 and above: `(null)` on macOS); at the end of
  the input before a complete term, the completed situations are advanced
  past the end of their rule (`earleyaddnewsits` on `NOLEXEM`: the next
  error lists `'\0'`, read from uninitialised memory). elanc-go prints
  `STRING` and the string quoted, and reports the error at the end of the
  input (`test_query.sh` maps the first to REM's `(null)`).
- **REM: traversals and normin/normout** (found by S7d task 6;
  `tests/compiler-rs/programs/strategies/trav_*`, `rewrite_*`, `normin_*`,
  each `.no-rem` says which): (1) the strategy after a traversal in a
  sequence gets the last argument's result, not the rebuilt term
  (`StrategyTraversalAll.java` and its siblings leave `v0` there):
  `tall(s) ; id` or a bottom-up `bu => tall(bu) ; first(dk(r), id)` give
  wrong terms, and crash when that argument is an int; (2) tall/tsome on an
  AC term with a non-deterministic strategy keep the elements of the
  previous result (`genericSetSubterm` adds to one term allocated before
  the loop); (3) `rewrite` does not renormalise the term it rebuilds
  (`term_rewriteStep`, no `specialApply`); (4) `normin`/`normout` run the
  step function of the start strategy (`strTab[... + strCall]`, trace.c):
  right only as the start strategy itself, elsewhere one step at the top
  at most; `normout` does not go into AC terms (`normout_ac`, `.no-rem`;
  elanc-go goes into them, `normin_ac` equal to REM); (5) REM is untyped: a rule
  `x => s(x)` applies to an int argument (`h(s(o),s(3))`). elanc-go follows
  REM elsewhere (the S7d plan, Task 6 rulings). The interpreter does not
  interpret traversals at all ("Traversal is not intepreted").
- **REM's compiler crashes on `rewrite(dk(...))`** (found by the S7d
  review): `rewrite` whose body is not a named strategy, e.g.
  `repeat*(rewrite(dk(cyc)))` or `rewrite(dk(cyc)) ; rr`, stops elanc-rem with
  `java.lang.ClassCastException: StrategyDkRule cannot be cast to
  StrategyCall` (`StrategyRewriteStep.compile`, StrategyRewriteStep.java:53):
  REM compiles `rewrite(s)` only for a named s. elanc-go compiles any body
  (`rewrite_*` name their strategy, so REM is the oracle there).
- **normin/normout on cyclic rules do not terminate** (found by the S7d
  review): `normin(cyc)` / `normout(cyc)` with `[cyc] a => b`, `[cyc] b =>
  a` on `g(a,b)` loop forever in elanc-go — a genuine non-termination like
  `repeat*` on such rules: each step succeeds, no depth is consumed (the
  rules rewrite in place), so no stack-depth exit (status 4) stops it.
  REM's normout loops too; REM's normin stops at `g(b,a)` (one rewrite per
  argument: its normin is not a normal form for these rules). Kept as is
  (the rules have no normal form); a step bound would need an option.
- **REM: Meta_strat terms** (`call`, sequences... built as `Strategy[X]`
  terms) give C that does not compile (`fun_143`), found by S6b
  (`tests/compiler-rs/programs/strategy_terms/ms_*`, `.no-rem`).
- **`trace_backup`/`trace_recover`** (`-coq -proofterm` compiled programs)
  are not safe under backtracking (slots reused after `trace_recover`); and
  `norm_4` now returns `t` where 2004 returned an undefined value (S5b).
- **Very long names (> ~1000 characters)**: `snprintf` truncates where
  `sprintf` overflowed; only `attach_type_modu` fails loudly (S2).
- **`main_coq_skeleton.c`** (`elanc-rem -coq`) does not compile with current
  compilers (pre-ANSI multi-line string); `elanc-rem -noC` links a non-existent
  `-lelan` (S5b).

- **elanc-go: I/O builtins** (S7e task 6, codes 113, 114, 116, 118-122
  as REM's fun_113..122, `elanrt/io.go`, `programs/io`): REM's output,
  error output and files byte for byte. Differences from REM: a file
  number that is not open stops the program (status 1, `getc_builtinStdio:
  file 7 is not open`), where REM reads an invalid `FILE *` (a
  segmentation fault, `getc(-1)` after a failed open); getc gives C's
  signed char as REM on macOS and x86 (REM on Linux arm64, whose char is
  unsigned, gives 0-255). The interpreter is not an oracle for most of
  them (its bugs below).
- **elanc-go: arrays and builtinHashTerm** (S7e task 7, codes 200-203
  and 210, `elanrt/array.go`, `hash.go`, `programs/arrays`: the five
  programs of legacy `Compiler.4.0/Work` that REM runs, testArray,
  testVector, testMatrice, testHashSet, testMatrix, give REM's results and
  rewrite steps): REM's errors and output byte for byte (out of range, not
  an array, an array printed: REM's message on stdout, status 1), REM's
  hash values on ints, identifiers, terms and AC terms. Differences from
  REM: `==` on two arrays compares their sizes and elements (REM finds any
  two arrays equal, below); that equality is structural only at the top
  level (`==` and non-linear patterns of an array sort): inside a term,
  arrays compare by identity (a term holding an array is shared by the
  array's identity, so `h(new(1,a)) == h(new(1,a))` is false); the hash of
  a string or of an array is elanc-go's (REM's reads memory). The normal
  form of an AC term that can hold an array is not cached (S7e task 13:
  `set` can make it reducible, `programs/arrays/arr_ac`), and the hash of
  a value of an injection family is that of REM's injection node (task
  13, `hash_nat`, `hash_inj`).
- **REM: arrays and hashes** (found in S7e task 7, not fixed; REM does
  not change in S7e): `==` (code 18) on two arrays is always true
  (`term_notDestructEqual`: equal hashes and the arity of the symbol
  CODE_ARRAY, 0, end the comparison: `new(1,a) == new(3,b)` is true,
  `programs/arrays/arr_neq.no-rem`); `hashTerm` of a string term hashes
  its length and its address read as terms (a value of its memory), of a
  term of 12 arguments or more reads its last arguments at a wrong address
  (`GgetArgument(k+t, i)`); `new(n, x)` with n < 0 asks the GC for a huge
  block (warnings), the first get then reports `size = -1`.
- **REM: bugs found by the bug hunt of 2026-10-13** (differential random
  testing of the interpreter, elanc, elanc-rs and `elanc-rem -O` on about
  8000 programs and queries, plus reviews of AC matching, strategies and
  pattern matching; not fixed, REM does not change outside S10e; elanc
  agrees with the interpreter on each):
  - a term is lost after a `where` fails in a later rule of a symbol whose
    earlier rule did not match: `[] f(a) => a`, `[] f(g(x)) => x where
    (T) g(y) := () x`, start `f(g(a))`: no result line (the others print
    `f(g(a))`); the same with a `where y := (s) x` whose strategy has no
    result;
  - an AC rule with two element patterns and an empty rest: `[] F(g(x),
    F(g(y), z)) => z` on `F(g(a), g(a))` gives no result (the others
    `F(g(a),g(a))`);
  - an AC-bound variable read in a `choose`: `[] f(F(y, x)) => z choose
    try where z := () x end end` on `f(F(d, a))` gives `F(a,d)` (x bound
    to the whole AC term; the interpreter gives `d`, elanc `a`, both
    valid);
  - repeated variables and repeated element patterns under AC (`x^n`
    shapes: `F(x,F(x,y))` at the top, `F(b,F(x,x))`, `F(z,F(z,F(b,b)))`,
    `g(F(x,F(x,F(y,y))))`; `F(g(x),F(g(x),y))`, `F(G(x,y),F(G(x,y),z))`):
    segmentation faults, wrong values, missing or extra solutions,
    `iterate*` that never ends (same class as the `dk_xx`/`g_x2y2*`
    `.no-rem` programs);
  - builtin operations without rules are not evaluated in a query of a
    compiled program (`computeTerm`, `runtime/termIn.c`, only builds the
    term): `and`/`or`/`not`, `==` of `eq[t]`, `occurs`/`replace`, `==` on
    identifiers, `<` on bool: `res((false and maybe))`, `rewrite_step = 0`;
  - a non-linear left-hand side needs the module `eq[T]` imported, else
    the compiled program crashes;
  - REM builds in `./.elan.<prog>/` and its `make` does not notice a
    changed start term: rebuilding in the same directory can leave a stale
    `a.out` (build in a fresh directory).
  Reproductions: the minimal programs of the hunt (`min/rem_wherepat_lost`,
  `min/rem_ac_two_elems_lost`, `min/rem_choose_acvar`, AC `x^n` shapes)
  are to be added as `.no-rem` regression programs of `tests/compiler-rs/
  programs/` when elanc's AC fixes land.
- **Interpreter: I/O builtins** (found in S7e task 6, not fixed; the
  interpreter does not change in S7e): `write` (code 119) crashes with a
  segmentation fault after writing its term (`write(stdout, 5)`, status
  139); code 122 is `flush` in the library (`builtinStdio.eln`) but the
  interpreter's `BUILTERRORPIPCONSTR` (`term/codes.h`), so `flush(f)`
  stays a term; `getc` on a file opened for reading reads after the line
  that the lexer stream made at `open` has buffered (`programs/io/
  io_getfile`: 102 for the first char of `A\351f(...`), and `read` there
  fails (9999); `open` gives `Error(errno)` on a failure, which `stdio`'s
  `iserr([x])` does not recognise (`rb(false,errno([2]))`, REM
  `rb(true,1)`). Regression programs: `programs/io/*.no-interpreter`.
- **elanc (Go): differences from elanc-rem — the kept refusals** (since
  the S7e decision of 2026-10-11, `elanc` is elanc-go and REM is
  `elanc-rem`: what follows is what `elanc` refuses and `elanc-rem`
  compiles, or neither does; the options of REM only are listed in the
  README, "Differences from REM's driver") (S7d task 7, closed in S7e; status 2,
  "not supported (not planned)" — since S7e no refusal names a stage,
  `ir` `TestKeptRefusalsAreNotPlanned` checks it; the inventory of
  `unsupported(` in `src/compiler-go/ir`):
  - not planned — no program can reach it: a commutative-only operator
    (the language has none); a `switch/case` right-hand side (SWRULE: the
    interpreter's parser no longer reads `switch`, NO_MORE_SWITCH in
    `load/msemact.cc`, the manual's section is commented out; REM does not
    read it); a builtin sort `intern ...` of the parser's grammar; a String
    lexem in a grammar (REM does not read it either);
  - not planned — the oracles do not compute it, or disagree: an AC
    operator on a builtin sort (`programs/refused/acbuiltin`: the
    interpreter `pl(pl(1,2),3)`, REM `pl(1,pl(2,3))`; strings under AC
    make both fail); `call "name":X` finding only a strategy of another
    sort (`refused/callother`: both apply it to the term, ill-sorted
    `plus(st(1),1)`); `call "name":X` of an unknown name (`refused/
    callunknown`: the export writes -1 and loses the name; the interpreter
    stops with `unknown strategy in call used`, REM crashes); `and`, `or`,
    `not`, `==` declared with a result that is not a bool; dccall/dkcall
    (external processes, REM has none); builtin codes 30-33 (`cmp[X]`
    term order: REM has none, the interpreter's `term::tcmp` uses its own
    symbol indices), 34-78 (doubles: REM has none), 90-112 (the
    interpreter's top-level commands), 160-174 (calls of the ELAN system),
    215 (equality of molecules, one application's builtin), 115 and 117
    (create a process) and 123 (the interpreter's I/O error constructor
    `Error(e)` of builtinIO): REM has no fun_115, 117, 123 (S7e task 6);
  - not planned (S7e, were "S7e" refusals) — possible, but no program
    of the tests, of the bench or of the library needs it, and each would
    port a piece of REM for no program:
    - AC shapes REM transforms before matching (`refused/acnested`,
      `acallrep`: both oracles agree, e2 and e1; a port of REM's
      bipartite matcher and of `RewriteRule.java` transformation2) and a
      where pattern with an AC operator of a builtin sort (its value is
      native: only a stuck term could match);
    - an identifier in an AC pattern (REM's order of a tagged identifier
      among its matchers' items is not established; no oracle program);
    - an AC operator whose arguments are not of its result sort (REM and
      Eker's matcher flatten `(s s) s` only; no program declares one);
    - rules for a builtin operation (whether the builtin or the rules
      come first in each oracle is not established; no program has one);
    - any other builtin code: no module of the library declares one (REM's
      ASF list codes 192-199, the interpreter's 79-82, 155, 179-190,
      205-208); the I/O codes and the arrays are compiled since S7e tasks
      6-7, builtinHashTerm too;
    - (removed: "an AC operator printed without its two arguments" could
      not happen — a candidate's arguments are those of its printing form.)
  - at run time (status 2): a META call of a number or a string whose
    strategy is of another sort; a query using an operator whose
    compilation was refused (Task 5); the options `-sort=`, `-strategy=`,
    `-REFInput`, `-commandLine` of REM's programs;
  - (removed, S7e task 13: the run-time refusal of task 2 — a query
    containing an operator that only the query reaches and that would
    box a family native in the program, when the start term does not
    use the query.) When the start term does not use the query, the
    query operators that the program reaches are compiled and a query of
    them is built and normalised as in REM (its steps in `rewrite_step`,
    its effects: `programs/query/queryignored`, `.steps`); the others are
    not compiled (the native families stay native) and a query
    containing one is parsed (the same syntax errors and warnings) but
    neither built nor normalised: nothing is refused and the start
    term's result is printed as REM does (`programs/query/queryonly`).
    What differs from REM for such a query only: REM builds and
    normalises it, so its `rewrite_step` counts the query's steps (not
    compared by `test_query.sh` for `queryonly`), and a query whose
    normalisation does not end or stops the program (an array error, a
    write) does so in REM only. The run-time refusals left are worded
    `elanrt: not supported (not planned): ...`.
- **REM: a compiled program that reads a query ignores the start term**
  (found by S9b): `start with (s) f(query)` behaves as `(s) query`. Examples:
  `Features/nqueens`, `longnames`, `tri`, `FastCompletion/cp`,
  `MatchingAC/CompMatchingAC`. elanc and the interpreter evaluate the start
  term. Consequence: no `gtest` for these programs, since the G oracle is
  REM (`tests/apps/category4.md`).
- **`Robot/tictac` with elanc-go** (found by S9b, closed on branch
  `fix-codesize`): it generated a 4 MB `main.go` (121 615 lines, 119 121 in
  the label of `[s]t`) and `go build` took more than 6 min. Cause: the
  Maranget decision trees (`ir/matching.go`) copy the rows with a variable
  at the tested position into every case and the default, so the 9 `moves`
  rules (each tests a different cell) and the 24 non-linear `win` rules
  gave 2^9 paths (7 004 nodes, 4 887 rule blocks for 49 rules; sharing the
  identical sub-trees, a DAG, still leaves 2 124 nodes and 1 560 rule
  blocks, about 38 000 lines). A tree is now kept within a budget of 64 + 8
  nodes per pattern node (the trees of the 175 bench programs that
  elanc-go compiles use at most 0.6 of it: their main.go is unchanged);
  over it, the rules are matched with bit vectors (`compileBits`: one
  traversal tests each pattern position once, ANDing per position and head
  the mask of the compatible rules, then the candidates are tried in source
  order; per subject O(positions visited * ceil(rules/64) + candidates
  tried), code O(pattern nodes * ceil(rules/64) + the rule bodies); elanc-rs
  still builds the whole tree): 3 720 lines (REM's C: 23 035), `go build` 2.2 s with an empty
  cache, 12 position tests per subject; same output as the interpreter and
  REM, `gtest tictac` added; guards `ir.TestWideRulesHaveALinearTree`
  (order of the rules tried for every subject, one test per position),
  `gen.TestWideRulesGiveCodeOfTheirSize`.
- **`.ref` export of `APT/bubleLOG`, `Robot/tictac`, `Strategies/ll`**: the
  file cannot be imported back (`[PARSER] syntax error on symbol 'tone'`,
  before `dk (N . div)`). The modern `elan -b --export` and the reference's
  write the same file (same size) and both fail on the import, so the bug is
  in both exporters; the A test of the bench uses the modern `elan`.

## Performance

- **Depth guard without Unguard** (S7e task 3, tried, not kept): a guard
  reading the stack every 1024 calls (no exit call) gives nqueens 12
  0.705 s instead of 0.741 s (no guard: 0.640 s), short of the target
  0.67 s; kept on the local branch `s7e-task3-guard-tried` (96f18ec, tests
  included). Any check at a function entry costs ~0.5 ns per call there
  (107 M guarded calls); a further gain needs fewer guarded calls (e.g.
  self tail calls of normalisation functions as loops).

- **Weak pointer per node** (S7e task 5, tried, not kept): nodes in slabs
  of 3 with one weak pointer per slab (local branch
  `s7e-task5-slabs-tried`, 65cc5ec) give deeplist3m 1.26 → 0.83 s and
  nqueens 12 0.737 → 0.672 s (-8.8 %, short of the 10 % rule) at 1.34x
  its RSS; larger slabs gain more time (N = 4: -12 %) but keep dead
  nodes with a live slab-mate (N = 4: 1.8x RSS, N = 64: 7x). The weak
  table stays as G2 decided; figures in the S7e plan, Task 5 results.

- **Tail-call loops of elanc-go allocated every turn** (review of S7b):
  done in S7e task 4 (`gen/strat.go`: the closures and the dc/one states
  of a loop are made once per call, the states reset at each turn; tail
  10^7 0.24 → 0.03 s). Left: a ONE/dc inside a continuation closure of the
  loop (`dc(dk(dec) ; dc one(taildc), id)`: `one3` in `k2`) is still made
  at each call of the closure (taildc 10^7 0.13 s, one object per turn).

- **Order of AC solutions: REM's** (bug hunt of the AC matching,
  2026-10-13, fixed 2026-10-14): elanc-go matched the element patterns of
  an AC level in source order and every AC subterm in place; it now
  follows REM (`ir/ir.go` "AC operators", `ir/lower.go` remTransform,
  `ir/eker.go` linearise): element patterns in REM's ordered normal form,
  RewriteRule.java transformation1 (several AC subterms under free
  symbols: all but the last one in wheres matched by Eker's matcher) and
  transformation2 (AC subterms in the level of the remaining AC pattern),
  and the codes of REM's linearisation (a repeated occurrence of a
  variable gets a larger number: the order of the two variables of a
  level, the orders after an abstraction). `programs/ac` ord_*; the bug
  hunt's harness: no order difference with REM left except one shape
  (below). Where REM's order differs from the interpreter's, the tests
  compare with the interpreter as multisets. Left: two or more AC
  subterms in one AC level (`F(G(x,y), F(G(y,z), w))`): REM abstracts
  them in the iteration order of a HashSet of terms (identity hash codes,
  `collectACSubterm`), an order elanc-go cannot reproduce; it takes them
  in the order of the term (bug hunt m3 p13: 7 of 100 queries in another
  order than REM's). Also: the pattern of a where that REM abstracts
  from a level (transformation3_1) is sorted by elanc-go with the codes
  of the linearisation, those of the other where patterns with the codes
  before it (`ir/lower.go` ekerCond of rl.abs): it would differ from REM
  only after a re-sort that depends on the HashSet order above.
  The variables shared by two AC subterms that REM matches apart are
  given to Eker's matcher with their value (`rt.EkPin`, the same
  solutions in the same order, `elanrt` TestEkerPinnedSameOrder): `p(F(x,
  y), G(x, z))` with 12 elements 7.8 s before (0c9d621), 11 s after
  517de33, 0.2 s now (16 elements: > 120 s before, 0.5 s now).
  Left by the review of be69e62 (minor): (1) `extension32`'s
  "constrained" test counts the occurrences of a variable in the
  left-hand side after abstraction, so a variable that occurs again only
  inside an abstracted subterm (`[] K(x, g(G(x,a)), ...)`) is not seen
  constrained and REM's case 3 re-sort is not done (another order only
  when two element patterns swap under the linearisation numbers); count
  on the left-hand side before abstraction. (2) `TestEkerPinnedSameOrder`
  covers one-level patterns only: add a pinned variable in a nested AC
  level (`U(z, g(x))`, `G(x, F(x, y))`) and a pinned AC term of the nested
  symbol. (3) A pin compares the partner's value with the subject's parts
  as they are: right when the subject is in normal form (every real
  program), as the non-linear fix assumes; say so in a comment.
  `programs/ac/ord_ext` is the first AC case where following REM's order
  gives another normal form than the interpreter's (a single
  non-confluent unlabelled AC rule: each takes its first solution;
  `.no-interpreter`): consistent with "the G oracle is REM".

- **Non-linear variable in an AC part given to Eker's matcher** (bug hunt
  of the AC matching, 2026-10-13): a variable that occurs twice, at the
  level of an AC operator with rules and in another pattern, is now
  compared as it is and takes the value of its other occurrence at once
  (`gen/ac.go` acVars, `rt.TakeAC`: `programs/ac` nl_slow, 24 s → 0.08 s
  with 18 elements). Left: when the level has two variables besides an
  element modulo an extension (`G(x, G(w, F(x, y))) => G(y, w)`), REM
  abstracts the part into a where matched by Eker's matcher
  (transformation1/2); elanc-go enumerates the cores (2^n) and Eker's
  values are normalised before the comparison (`ek.Value(i, acNorm)`):
  > 100 s on the start term of nl_slow with 16 elements, where REM takes
  1.9 s (18 elements: REM 18 s, exponential too).

- **Or-parallelism of `dk` in elanc-go** (idea of the user, 2026-10-12, not
  started): run the branches of a `dk` (all results) on goroutines. Gain
  expected only on programs enumerating every solution of a large search
  tree (nqueens, `set_of` over large spaces: up to the number of
  performance cores); none on normalisation (propc, efib, BenchThesis).
  Obstacles in `elanrt` today: (1) order — `Dk` (`strat.go`) gives the
  results to `k` in branch order and `k` may stop (`first`, `one`, `dc`);
  the suites compare outputs and `rewrite_step` byte for byte with REM and
  the interpreter, so each branch must buffer its results for an in-order
  replay, and speculative branches cut by a stop add work and steps (`dc`
  is don't-care in ELAN, so racing would be correct but not
  deterministic); (2) shared state — `Step()` (`Steps++`, `driver.go`),
  the unique table (`table.go`, not thread-safe, and term equality is
  pointer identity, so one table per goroutine is not enough: a concurrent
  sharded table, paid at every node creation, also by sequential
  programs), the depth guard (`depth`/`nextCheck`, per goroutine), I/O
  builtins and arrays (side effects: stay sequential); (3) granularity — a
  `dk` branch is often tens of ns, a goroutine 0.3-1 µs: parallelise near
  the root only (depth cutoff or work stealing); (4) the Go GC already
  uses the other cores (propc 3: user 2.1 s for 0.64 s real). Next step if
  pursued: a spike with an opt-in `-parallel` limited to all-results `dk`,
  ordered replay, per-goroutine step counters summed at the end, a
  sharded unique table; measure nqueens 12 and the overhead on sequential
  programs (a few percent at most, or drop it).

## Build, tests, tooling

- **CI**: the Linux job failed at the ASan+UBSan step on GitHub (x86_64) while
  passing in local arm64 Docker; on hold at the user's request (S2).
- `ELAN_CC`/`ELAN_CXX` mean both the build compilers and the generated
  Makefiles' defaults (environment override) (S5a).
- `-lfl` (Linux, generated programs) is not checked by CMake (S5a).
- Installs are not relocatable (absolute paths in `elanc-rem`, `elan`) (S5a).
- `CMAKE_BUILD_TYPE` changes the runtime library flags (S5a).
- `check_limits.py` does not strip `/* */` blocks or `#if` branches and skips
  names missing from one copy (S3b review).
- Very deep inputs are now bounded only by the C stack (recursive term
  functions, module loading), not by fixed limits with a message (S3b).
- Static library `elan_core`: relies on every member being referenced and on
  archive order for static initialisation (S3a).
- `check_deps.py` strips `/*...*/` even inside string literals (S3a).
- C code generation is still referenced from the driver (`genmaintfile`) and
  `compiledefs.h` from term/grammar/partial (listed exceptions) (S3a).
- Runner: an example with several `.spc` aborts the bench; a spec named
  `no.spc` collides with the "no" sentinel (S4).
- 325 raw-type `javac -Xlint` warnings in REM (S5b).

## Planned sub-projects

- **S3b — interpreter internals** (spec `2026-10-10-s3b-internals-design.md`):
  done: goldens of what the bench does not see, contract guards, no silent
  overflow, dynamic stacks, parser state grouped (`LoaderState ld`,
  `RefParserState rp`, module frames), fatal errors as `ElanFatal`, `term`
  unit test (`tests/unit/test_fatal.cc`). Kept by decision: hashed table
  sizes and codes (REF contract), manual term ownership (documented in
  `term/termdefs.h`). Left: `interr()` still `abort()`s; the C AC matcher
  exits by itself on its own errors (out of memory); `impmoduli` and
  `withrhs` stay separate globals until lex/parse stop reaching into the
  loader.
- **cpl redesign** (backtracking without copying the C stack), which would
  also allow ASan for the compiled programs.
