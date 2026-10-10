# Rust spike: NOTES

The files are `runtime.rs` (the runtime library), `nqueens.rs`, `efib.rs` and `propc.rs` (the "generated" code), plus `Cargo.toml`, `build.sh` and `tools/anf_q.py`. `anf_q.py` turns the big right-hand sides of `q1`/`q2`/`q3` into A-normal form, the way a back-end would. The build uses std only, offline.

## Build

`./build.sh` runs `cargo clean` (only this directory's `./target`), then `cargo build --release --offline` (opt-level 3, the default 16 codegen units, no LTO, no overflow checks). It copies the binaries to `bin/`.

- Clean release build of the three binaries: **0.26 s real, 1.25 s user** (rustc 1.74.1, Apple Silicon).
- Each binary includes `runtime.rs` as a module (`mod runtime;`). That lets rustc inline the runtime into the generated code without `#[inline]` annotations across crates.

## Results (all verified)

| run | result | rewrite_step | real (s) | user (s) | max RSS (MB) |
|---|---|---|---|---|---|
| nqueens 8  | 92 solutions, order identical to oracle   | 128949   | 0.00 | 0.00 | 2.3 |
| nqueens 10 | 724 solutions, order identical to oracle  | 3426635  | 0.02 | 0.02 | 3.7 |
| nqueens 11 | 2680 solutions, order identical to oracle | 19286638 | 0.14 | 0.14 | 10.1 |
| efib 50    | 11011074 | 3721  | 0.00 | 0.00 | 3.0 |
| efib 100   | 35084101 | 14946 | 0.00 | 0.00 | 8.0 |
| propc 1    | t | 20546  | 0.00 | 0.00 | 4.0 |
| propc 2    | t | 322083 | 0.06 | 0.05 | 24.9 |
| propc 3    | t | 810879 | 0.32 | 0.31 | 90.2 |

- Each run was measured once with `/usr/bin/time -l`.
- For nqueens, the `result = ...` lines of `out{8,10,11}.txt` were checked with `cmp` against ours (same lines, same order).
- The `rewrite_step` counts are identical to the Go version's for every run, which suggests both do the same work. efib 50 also gives 3721, the figure in the comment at the end of `efib.eln`.

## Line counts

| file | lines | non-blank, non-comment |
|---|---|---|
| runtime.rs | 588 | 472 |
| nqueens.rs (generated) | 215 | 162 |
| efib.rs (generated) | 305 | 253 |
| propc.rs (generated) | 575 | 530 (≈350 of them are the A-normal-form `let`s of q1..q3) |

## Design

- **Terms.** A term is a `u32` index into `Vec<Node>`; a node is `{sym: u32, len: u32, data: u64}`. `data` is an offset into a shared `Vec<u32>` of arguments, or the value of an `INT` node.
  - One open-addressing unique table (linear probing, FxHash-like mixing, grown at 50% load) does the hash-consing.
  - Nothing is ever freed. Symbols 0/1/2 are the runtime's `int`, `true` and `false`. Constants are built once at start-up (`rt.c(SYM)`).
- **Builtins.** Integers are hash-consed nodes, with no small-integer cache. Every `n-1` probes the table, including the literal `1`, which is looked up at each use: literals are not hoisted.
  - Builtin predicates return the terms `true`/`false`, and conditions compare with `rt.t_true`.
  - Builtin bool `and` is strict (innermost), so `noattack` always recurses to the end of the list, as REM's innermost normalisation does.
- **AC terms.** AC terms are flattened, with arguments sorted by id and duplicates kept.
  - `ac2` merges the two already-sorted element lists.
  - Helpers: `ac_first_occ` (iterate over the distinct elements, skipping removed positions), `ac_mult`, `ac_find` (binary search), and `ac_rest`. `ac_rest` returns `None` if empty, the element itself if one remains, else a new AC term.
  - There is no general AC matcher.
- **AC rules (same choices as the Go version and REM):**
  - `and(x,x,R) => and(x,R)` and `xor(x,x,R) => xor(R,f)`.
  - In `and(x,t)`, `and(x,f)`, `xor(x,f)` and `and(x,xor(y,z))`, `x` is the whole rest of the multiset. In the last rule, `y` is the first xor element and `z` is the rest of the xor.
  - A rest bound to a variable that is returned directly is re-normalised (`and_renorm` / `xor_renorm`). A rest passed to `and`/`xor` is normalised by that call.
  - Pattern variables that are never used (efib `occursFib`'s `S`) are not built.
- **efib `compute`.** It enumerates the three elements with nested loops. Each condition is tested as soon as its variables are bound (`n1 == n2+1` after the second element). `v1+v2 % 1000000` is `v1 + (v2 % 1000000)`.
- **Strategies (CPS).**
  - `type K<'a> = &'a mut dyn FnMut(&mut Rt, Term) -> bool`; a strategy is a `fn(&mut Rt, Term, K) -> bool`.
  - The combinators `dk`, `dc`, `first_one`, `one`, `iterate_star`, `repeat_star` and `repeat_star_det` are runtime functions that take the sub-strategies as function pointers. The compiler emits one named `fn` per strategy sub-expression, e.g. `s_range` = iterate*(`s_range_1`), and `s_range_1` = dc(`r_range_rule`).
  - Labelled rules are `fn r_<label>(rt, t, k)`. AC matching loops call `k` for every match, so a rule can yield several results.
  - `where x := (S) u` in a labelled rule becomes a nested closure. In an unlabelled rule (efib `go`), it takes the first result.
  - `repeat*(first one(...))` is compiled as a loop (`repeat_star_det`) because its argument is deterministic. The general CPS `repeat_star` exists but is not used.
- **Stack.** `main` runs the program on a thread with a 1 GiB stack (reserved, not touched), so that deep CPS/innermost recursion is safe. The default 8 MB would probably have been enough here.

## Where Rust forced a different shape (and the cost for a code generator)

1. **No global mutable runtime.** A `static mut` arena needs `unsafe` everywhere; `thread_local!` + `RefCell` costs a borrow check per access and panics on re-entrant borrows. So every generated function takes `rt: &mut Rt` explicitly.
   - Continuations *receive* `rt` as a parameter instead of capturing it, because a closure that captured `&mut rt` would block any use of `rt` until the closure is dropped. Hence the `FnMut(&mut Rt, Term)` signature.
   - Cost for the generator: mechanical (one extra parameter everywhere), but it must be designed in from the start.
2. **A-normal form is mandatory.** `rt.and(rt.ne(d,p), x)` and `f(rt, rt.c(A1))` do not compile: two-phase borrows only cover a shared use inside an autoref'd method call, not a nested `&mut` use or an explicit reborrow argument. Every non-trivial subterm, and even constant lookups used as arguments, must be bound with `let` first. That is why q1..q3 take ≈350 lines of `let`s; `tools/anf_q.py` does the flattening.
   - Cost: low for a compiler, which already produces temporaries as REM does with `sv[i]`, but it rules out the simple "print the rhs as a nested expression" back-end.
   - The workaround of putting `rt` *last* in the parameter list (`f(g(x, rt), rt)`) does compile, but looks odd.
3. **Closures and lifetimes.**
   - Nested `where` clauses become nested `&mut |rt, x| { ... }` closures that capture the outer `k` by unique borrow. This works without annotations because each closure lives only for the duration of one call.
   - The `dc` combinator needs a `found` flag mutated inside the closure and read after the call. That is fine because the temporary closure is dropped before the read.
   - Closure parameter types are inferred from the `K` coercion, so the generator never writes types in closures.
   - `&mut dyn FnMut` means one indirect call per continuation; generics would monomorphise and inline, but recursive strategies like `iterate*` would then need dyn somewhere anyway.
4. **Borrowing the argument array.** Generated code cannot hold a slice `&rt.args[..]` while calling anything that takes `&mut rt` (hash-consing may reallocate the vector). So AC matching works on positions (`rt.arg(t, i)`), and the runtime moves its scratch buffer out of `rt` (`std::mem::take`) while it fills it.
   - This is safe and cheap, but it is a design constraint that C does not have (in C the reallocation bug would just be latent).
5. Overall verbosity is close to C's. The only noise is `rt` everywhere, the ANF `let`s, and `!= rt.t_true` tests. The code compiled on the first attempt, with no lifetime annotations in the generated code.

## Deviations from the brief / comparability caveats

- **Build options.** I used the cargo `--release` defaults (16 codegen units, no LTO). The runtime is a module of each binary, not a separate crate, so inlining across runtime and generated code is possible. C and Go get the equivalent through header inlining and their compilers.
- **`rewrite_step`.** It counts user-rule applications, including unlabelled rules such as `q2i`, `not` and `occursFib` (both its rules). Builtin int/bool operations are not counted. The counts equal the Go version's but not REM's (nqueens 8: REM 648617, here 128949).
- **Large stack.** The 1 GiB thread stack is reserved address space only; RSS counts only touched pages.
- **Timing resolution.** `/usr/bin/time` resolution is 10 ms. Most runs are below that, so only nqueens 11 and propc 2/3 compare meaningfully.
- **Memory measurements.**
  - Max RSS includes the never-freed arena: there is no GC.
  - The initial unique table (64 Ki slots) and growth by doubling with a full rehash give stepwise RSS jumps.
  - The `Vec` growth policy doubles capacity, so peak RSS can be up to about 2x the live data during a reallocation.
  - This is comparable to the C arena build, not to C+Boehm or Go.
- **Output buffering.** nqueens collects the `result = ...` lines in a `String` and prints them once (Rust's stdout is line-buffered). Unbuffered `println!` per solution would add syscalls for 2680 lines.
- **Unused code.** `generate_list`, `det_queens_strat` and `Compute` are emitted or declared but unused (`#![allow(dead_code)]`). The efib labelled rules `init1`/`init2` are not used by any strategy and were *not* written, which makes efib.rs about 50 lines shorter than a compiler's output.
