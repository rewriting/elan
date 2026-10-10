# Spike: target language of a modernised ELAN compiler (C, Rust, Go)

Question: what does the generated code cost in each target language, with a
backtracking model that does **not** copy the C stack (no cpl)? We hand-write,
in C, Rust and Go, the code a new compiler would generate for three
representative programs, and measure it against the current compiler
(REM + cpl, 2004 design).

The code is **throwaway**: it answers a question. It must still be honest:
it is what a compiler would emit mechanically from the rules, not a
hand-optimised algorithm (no bitboards for queens, no special-casing of
the input). Every language follows the same model below, so that the
differences measure the language, not the design.

## Programs (`programs/`)

| Program | ELAN source | What it exercises | Sizes measured |
|---|---|---|---|
| nqueens | `nqueens.eln`, `nqueens.lgi` (query: N) | non-determinism: `dk`, `iterate*(dc(...))`, strategy in a `where`, conditions; all solutions | N = 8, 10, 11 |
| efib | `efib.eln`, `efib.lgi` (query: `go(N)`) | AC matching over a multiset (`S U Fib[...] U Fib[...] U Fib[...]`) with conditions and non-linear constraints, `repeat*(first one(...))` | N = 50, 100 |
| propc | `propc.eln`, `propc{1,2,3}.lgi` (start terms q1, q2, q3) | AC normalisation (`and`, `xor` AC; `and(x,x)`, `xor(x,x)`, distributivity), innermost normalisation, sharing | q1, q2, q3 |

Read the `.eln` files: they are the specification. Expected results (the
current compiler, `work-current/`):

- nqueens: one `result = ...` line per solution, **in the same order** as
  `work-current/nqueens/out{8,10,11}.txt` (92, 724, 2680 solutions). The
  order follows from the strategy semantics (`dk` tries its alternatives in
  order, `iterate*` yields the 0th, 1st, ... iterate).
- efib: `go(50)` = 11011074, `go(100)` = 35084101.
- propc: q1, q2, q3 all normalise to `t`.

Print results like the current programs: `result = <term>` (nqueens lists
as `2.4.6.1.3.5.nil`), then `rewrite_step = <n>` (the number of rule
applications in your implementation; it need not equal the current one).

## Execution model (the same in the three languages)

**Terms.** Immutable, **maximally shared** (hash-consing: one unique table;
structural equality = pointer/id equality). A node has a symbol code and
arguments. Builtin integers are term nodes holding the value. Free (non-AC)
symbols: fixed arity. AC symbols (`and`, `xor`, `U`): **flattened canonical
form**: one node with n >= 2 arguments, sorted by a total order on
(shared) terms (e.g. unique id), duplicates kept (multiset).

**Normalisation.** As REM does: every defined symbol `f` has a generated
function `f(args...)` that builds the term from normalised arguments and
applies the unlabelled rules of `f` (innermost), returning the normal form.
Constructors just build (hash-cons). AC symbol functions flatten, sort,
then apply their rules. Rule order = source order.

**AC matching.** Generated per pattern, with the runtime giving helpers
(iterate over the distinct elements of a multiset, remove an element,
build the rest `S` as an AC term or the neutral `empty`). A pattern with
several AC elements (efib `compute`) enumerates element choices with
backtracking, conditions pruning. No general-purpose AC matcher.

**Strategies and backtracking: success continuations (CPS).** A strategy
compiles to a function `s(t, k)` that calls `k(r)` for each result `r` in
order; `k` returns whether to **stop** (a `one`/`first`/`dc` needs one
result) and `s` returns it upward. So:

- labelled rule `[l]`: match, conditions, `where`, then `k(rhs)`;
- `dk(S1,...,Sn)`: call each `Si(t, k)` in order (all results);
- `dc(S1,...)` / `first(S1,...)`: the first `Si` with at least one result
  gives all its results (`dc`), or `first one`: exactly one result;
- `one(S)`: the first result of `S`;
- `iterate*(S)`: `k(t)`, then each result `r` of `S(t)` continues with
  `iterate*(S)(r)` (depth first, in order);
- `repeat*(S)`: apply `S` while it has a result (here only with
  `first one`, so deterministic: a loop); the result is the last term;
- `where x := (S) u`: call `S(u, k')` where `k'` continues with the rest of
  the rule; `where x := () u`: plain normalisation.

C: a continuation is a function pointer plus an environment pointer
(struct on the C stack, no heap allocation per call). Rust: `&mut dyn
FnMut(Term) -> bool` (or generics if the code stays mechanical). Go:
`func(Term) bool`. Deep recursion is acceptable for these sizes.

**Memory.**

- C: two builds of the same code: (a) Boehm GC (`/opt/homebrew/opt/bdw-gc`,
  as today), (b) a bump arena that never frees (to see the GC cost).
- Rust: an arena of nodes indexed by `u32` (never frees); std only.
- Go: the Go GC; std only.
Report the peak memory of every run.

**Constraints.** No third-party libraries (C: libc + Boehm; Rust: std;
Go: std). Rust 1.74 (`rustc`/`cargo` offline), Go 1.27, Apple Clang.
Release builds: C `-O2`, Rust `--release` (opt-level 3), Go default.

## Layout and deliverables

`spikes/codegen/<lang>/` with:

- `runtime.*` (or a module): terms, hash-consing, AC canonical form, AC
  helpers, printing — written once, as a real runtime library would be;
- one file per program with the "generated" code (`nqueens.*`, `efib.*`,
  `propc.*`), written as REM would emit it (one function per symbol, per
  labelled rule, per strategy), with a short comment per function naming
  the ELAN rule or strategy it comes from;
- `build.sh`: builds the binaries into `spikes/codegen/<lang>/bin/`
  (`nqueens`, `efib`, `propc` taking the query on the command line:
  `nqueens 10`, `efib 100`, `propc 3`; C: also `*-arena`);
- `NOTES.md`: design choices, anything that deviates from this brief and
  why, line counts (runtime vs generated), difficulties met in the language.

Do not modify anything outside `spikes/codegen/<lang>/`. Do not commit.
Check every result against the expected outputs above before reporting.
