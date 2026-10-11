# BenchThesis programs ported to ELAN 3

`FastCompletion/` and `MinelaComp/` are the two applications of
`legacy/elan/sources/Compiler.2.1/BenchThesis/` (ELAN 2.1, December 2000),
the benchmarks of the thesis on the ELAN compiler. They do not parse in
ELAN 3, so the legacy bench reports their 30 J/JO cases (and their I/A
cases) as `ERROR`/`FAIL`. Here they are ported to ELAN 3 with the smallest
possible change and run by `../test_benchthesis.sh` (`make check`,
`make check-compiler`).

* `FastCompletion`: completion of equational theories (Knuth-Bendix with
  LPO, `ans.lgi` compiled with the `start with (s_commande) sat` query,
  `ans_completion.lgi` interpreted); 15 specifications (`*.spc`).
* `MinelaComp`: a rewriting engine written in ELAN, building proof terms
  (`rewriting.lgi`); 6 specifications.

Copied from legacy: the `.eln`, `.lgi`, `.spc` files, `tst_all` and
`SAMPLES/` (the 2000 outputs, unchanged). Not copied: generated files
(`.c`, `.h`, `.make`, `.ref`), timing scripts, `Sample/` and `Examples/`
(older copies of the specifications), `CVS/`.

## The port

The first commit of the port copies the files verbatim, so
`git log -p -- tests/compiler/benchthesis` shows the whole port.

### 1. `rewrite` is a keyword in ELAN 3

`rewrite` is in the keyword table of the ELAN 3 lexer (`tabofident`); ELAN 2.1
programs used it as a module name, an operator name and a rule label. Every
occurrence of the identifier is renamed `rewriting`; the module file
`FastCompletion/rewrite.eln` becomes `rewriting.eln`. Nothing else changes
(rule and strategy semantics, specifications, queries).

FastCompletion, `rewriting.eln` (was `rewrite.eln`):

    before: module rewrite[vars,fss,prec]
            rewrite(@,@)        : (term equation) term;
            [rewrite] rewrite(s,l->r)  => s[theta(r)] at omega
    after:  module rewriting[vars,fss,prec]
            rewriting(@,@)      : (term equation) term;
            [rewriting] rewriting(s,l->r)  => s[theta(r)] at omega

FastCompletion, `ans_completion.eln`:

    before: import global ... rewrite[vars,fss,prec]
            where v:=(dk(rewrite)) rewrite(t,rrwr)       (3 times)
    after:  import global ... rewriting[vars,fss,prec]
            where v:=(dk(rewriting)) rewriting(t,rrwr)

MinelaComp, `rewriting.eln` (the module was already called `rewriting`):

    before: rewrite(@,@)        : (term rwrule) rewrite_state;
            [] rewrite(s,rw)    => [s,nil,label(rw),rw2rb(rw),identity]:proofnil
            [rewrite]  [s,pi] => ...  where rs:=(rewrite_state) rewrite(s,rwr)
            [.] s_rewrite => repeat*(dc one(rewrite))
    after:  rewriting(@,@)      : (term rwrule) rewrite_state;
            [] rewriting(s,rw)  => [s,nil,label(rw),rw2rb(rw),identity]:proofnil
            [rewriting]  [s,pi] => ... where rs:=(rewrite_state) rewriting(s,rwr)
            [.] s_rewrite => repeat*(dc one(rewriting))

No other ELAN 2 to ELAN 3 change was needed: with this renaming, the
programs load and run on the 2004 reference and on the modern system.

### 2. `MinelaComp/rewritingC.lgi` (added, for the compiled cases)

`rewriting.lgi` reads a `term` and starts with
`start with (s_rewrite) [query,proofnil]`. A compiled program that reads its
query (no `-noInput`) applies the start strategy to the query itself
(REM `Query.genStrategyCall`): the start term is used only by `-noInput`,
when it is ground. The 2000 compiled tests (`ctest`, inputs
`SAMPLES/c*.inp` = `[t , proofnil] end`) therefore used the variant left in
a comment in `rewriting.lgi`; `rewritingC.lgi` is that variant:

    before: LPL rewriting description
            query of sort term  //pair[term,proofterm]
            start with (s_rewrite) [query,proofnil]
    after:  LPL rewritingC description
            query of sort pair[term,proofterm]  // was: term
            start with (s_rewrite) query  // was: [query,proofnil]

Since then, the compiler applies the start term as the interpreter does
(branch `compiled-start-term`): the compiled `rewriting.lgi` gives the
interpreter's results (`tests/compiler/test_start_term.sh`). `rewritingC.lgi`
is kept: its outputs are those of the 2004 compiled programs.

## The cases (64)

| kind | app | program | queries | expected | historical |
|---|---|---|---|---|---|
| J, JO | FastCompletion | `ans` + spec | `-noInput` (`sat`) | `expected/J-j<spec>.out` | `SAMPLES/j<spec>.out` |
| I | FastCompletion | `ans_completion` + spec | `SAMPLES/sat.inp` | `expected/I-i<spec>.out` | `SAMPLES/i<spec>.out` |
| J, JO | MinelaComp | `rewritingC` + spec | `SAMPLES/c<x>.inp` | `expected/J-c<x>.out` | `SAMPLES/c<x>.out` |
| I | MinelaComp | `rewriting` + spec | `SAMPLES/i<x>.inp` | `expected/I-i<x>.out` | `SAMPLES/i<x>.out` |
| G | FastCompletion | `ans` + spec | `-noInput` (`sat`) | `expected/J-j<spec>.out`, `rewrite_step` excluded | |
| G | MinelaComp | `rewritingC` + spec | `SAMPLES/c<x>.inp` | `expected/J-c<x>.out`, `rewrite_step` excluded | |

Kind G (the Go compiler `elanc-go`, spec S7) is not in the default kinds:
`BENCHTHESIS_KINDS=G ../test_benchthesis.sh PREFIX` (run by `make check-go`)
compiles with `elanc-go` (go build cache `tests/compiler/work/gocache`, or
`$ELANC_GO_CACHE`) and runs the same commands as J. Its oracle is J's
expected output without the `rewrite_step` line (the count depends on the
compilation scheme, as in the bench); the result line shows the time,
elanc-go's `go build` time and the steps of both (FastCompletion: equal to
J's in all 15 cases).

FastCompletion: the 15 specifications of the `jtest` lines of `tst_all`
(KNZ86 Zeh89 curry exa79 exa80 exa85 expf expoclos furtin gdiv group p2 p8
sample taussky) compiled with and without `-optimiseChoicePoint` (the 30 J/JO
cases of the legacy bench), and interpreted (the 14 with an `i*.out`).
MinelaComp: the `ctest` lines compiled (append congruence or peano primes4
primes8 sample) and the `itest` lines interpreted.

The expected outputs are the stdout of the 2004 reference
(`reference/install`, compiled programs built with Homebrew GCC 16),
compared exactly, `rewrite_step` included. They were written by
`BENCHTHESIS_SAVE=1 ../test_benchthesis.sh <repo>/reference/install`. The
modern system gives byte-identical outputs for all 64 cases (J and JO give
identical outputs too).

## Comparison with the 2000 outputs

With the weak "atoms" comparison of `tests/legacy-bench/run_tests.py`
(identifiers and numbers in order, statistics lines ignored):

| cases | status | difference |
|---|---|---|
| FastCompletion J/JO (28 of 30) | atoms-equal | the 2000 compiler printed `.(->(m(,)(I,(var()([](0)))),...` (prefix forms of the mixfix operators), now `cons_equation(m(I,var(0))->...`; `rewrite_step` is 7–14 % higher today (`group`: +51 %, see below) (e.g. curry 32503 → 36305, p8 20039097 → 21607279), not compared by atoms |
| FastCompletion J/JO `group` (2) | different | same completed system, rules in another order: `SAMPLES/jgroup.out` was produced with the version of `group.spc` in `Sample/` (two more axioms, `x=m(x,o)` and `o=m(x,i(x))`, commented out in `group.spc`); with that file the reference prints the historical order |
| FastCompletion I (14) | atoms-equal | lists printed `a.b.nil` in 2000, now `cons_equation(a,cons_equation(b,nil))` |
| MinelaComp I congruence, or (2) | exact | |
| MinelaComp I peano, primes4, sample (3) | atoms-equal | `a.nil` vs `cons_proofterm(a,nil)` |
| MinelaComp I append (1) | different | the substitution is printed `x->a o y->nil o z->...` instead of `z->... o x->a o y->nil` in 2000 (order of the substitution's bindings) |
| MinelaComp J/JO congruence, or (4) | exact (result line) | the compiled program prints `result = t` and statistics; the 2000 `ctest` output is the bare term |
| MinelaComp J/JO peano, sample (4) | atoms-equal (result line) | as for I |
| MinelaComp J/JO append, primes4, primes8 (6) | atoms-equal (result line) once the 2000 printing `x->ao  y->nilo  z->...` (no space before the composition `o`) is split | |

So every difference with 2000 is a printing difference, except `group`
(different input file) and the binding order of `iappend`.

## Timings

Apple M-series, `make check` build: the 64 cases take about 10 s in parallel
(`BENCHTHESIS_JOBS`, default the number of CPUs); the reference takes 22 s
(its interpreter is slower: `furtin` 14 s, `taussky` 9 s). The longest
compiled case is `p8` (about 20 million rewrite steps, 6 s with the
reference and with the modern compiler; 47 s for the 64 cases with `BENCHTHESIS_JOBS=1`). No case is skipped.

Kind G, FastCompletion (Apple M4 Max, 2026-10-11, S7d task 4). Compiling
`ans` with `elanc-go` (export, Go emission of a 229 KB `main.go`, `go
build`): 1.5 s when the program is new (`go build` 1.48 s with the runtime
in the cache, 1.55 s with an empty cache: the runtime compiles in parallel
with the program), 0.15 s when the same Go module (program and specification: each
specification gives another module) was built before. Under the 2 s target of the spec (S7 §2.4).
The 22 G cases take 7 s in parallel (each compilation then takes about
5 s, the 22 `go build`s competing for the CPUs). Run times (`./a.out
-noInput -quiet`, second run, s):

| spec | steps | elanc-go | REM |
|---|---|---|---|
| KNZ86 | 23485 | 0.00 | 0.01 |
| Zeh89 | 44605 | 0.00 | 0.01 |
| curry | 36305 | 0.00 | 0.01 |
| exa79 | 28398 | 0.00 | 0.01 |
| exa80 | 185672 | 0.01 | 0.05 |
| exa85 | 367736 | 0.02 | 0.09 |
| expf | 534549 | 0.00 | 0.10 |
| expoclos | 145667 | 0.00 | 0.03 |
| furtin | 1630386 | 0.04 | 0.38 |
| gdiv | 244890 | 0.01 | 0.06 |
| group | 181752 | 0.01 | 0.05 |
| p2 | 417760 | 0.02 | 0.11 |
| p8 | 21607279 | 0.88 | 5.39 |
| sample | 2842 | 0.00 | 0.00 |
| taussky | 795094 | 0.03 | 0.21 |

The steps are those of J (elanc-go tries the same redexes in the same
order).
