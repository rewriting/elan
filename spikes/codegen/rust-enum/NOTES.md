# Rust spike, second version: terms as typed enums (NOTES)

Follow-up of `../rust/`. There, a term is a `u32` index into an untyped arena
that is never freed. Here every ELAN sort is a Rust `enum`, and a shared term
is an `Rc` to a hash-consed node. Terms that nobody references are freed by
reference counting. Same programs, same model (innermost normalisation,
AC canonical form, CPS strategies), same parity choices as `../rust/NOTES.md`,
no memoisation.

Files:

- `runtime.rs`: hash-consing, AC multiset helpers, CPS combinators, step
  counter, driver.
- `nqueens.rs`, `propc.rs`: the "generated" code.
- `tools/q_nested.py`: emits the q1..q3 right-hand sides.
- `droptest.rs`: freeing a long list.
- `build.sh`, `measure.sh`, `results.tsv`.

Everything is std only and builds offline with rustc 1.74.1.

## Build

`./build.sh` runs `cargo clean` on this directory's `./target`, then
`cargo build --release --offline`, and copies `nqueens`, `propc` and
`droptest` to `bin/`. The command lines are the same as in the first version
(`bin/nqueens 11`, `bin/propc 3`).

There is one cargo feature, `immortal`, which never frees a term (each new
node is leaked once). It is only used for the comparisons below and is not
part of the build.

## Results (same machine, same session, best of 5, `/usr/bin/time -l`)

| case | first version (u32 arena) real / user / RSS | enum + Rc + weak table real / user / RSS | enum, `immortal` real / RSS |
|---|---|---|---|
| nqueens 11 | 0.13 / 0.13 s / 10.1 MB | **0.03 / 0.03 s / 2.5 MB** | – |
| nqueens 12 | 0.85 / 0.84 s / 40.3 MB | **0.20 / 0.20 s / 3.3 MB** | 0.24 s / 105 MB |
| propc 2 | 0.06 / 0.05 s / 24.9 MB | 0.10 / 0.10 s / **5.7 MB** | 0.09 s / 66 MB |
| propc 3 | 0.29 / 0.28 s / 90.2 MB | 0.40 / 0.40 s / **15.5 MB** | 0.40 s / 286 MB |

How the numbers were taken:

- Run `measure.sh` to get the first two columns (raw data in `results.tsv`).
- The `immortal` column is best of 3.
- RSS is in MiB.

Clean release build (nqueens + propc, cold `target`, 3 runs each):

| | real | user |
|---|---|---|
| first version | 0.25 s | 0.77 s |
| enum version | 0.66 s | 1.40 s |

Per binary, nqueens takes 0.22 s against 0.21 s. propc takes **0.66 s against 0.25 s**. With `-C panic=abort`, propc builds in 0.48 s; see "Difficulties" for why.

### Correctness

- **nqueens.** For 8, 10, 11 and 12, the output is byte-identical to `../rust/bin/nqueens`: the same `result` lines in the same order, and the same `rewrite_step` (8: 128949; 10: 3426635; 11: 19286638; 12: 116583341).
- **propc.** q1, q2 and q3 all give `result = t`.
  - Their `rewrite_step` counts are **20514 / 322083 / 810867**, where the first version gives 20546 / 322083 / 810879. So q1 is -32, q2 is equal and q3 is -12.
  - **Explanation.** AC multisets are sorted by node creation id. In the arena, a term keeps its id forever. Here, an intermediate term that is freed and later rebuilt gets a *new, larger* id. Rebuilt terms are mostly unnormalised `and`/`xor` merges and AC rests.
  - The new id changes the order inside some multisets. The rule `and(x, xor(y,z))` takes the first xor element (by id) as the redex and its first element as `y`. A different order therefore picks a different, equally valid distribution path, with a slightly different number of steps.
  - **Check.** With `--features immortal`, terms are never freed, so ids are stable, and the counts are exactly 20546 / 322083 / 810879. Everything else is identical.
- **Deep drop.** `bin/droptest 10000000` builds a 10M-cell list on the main thread (default 8 MB stack) and drops it: "dropped, live = 0".
  - `bin/droptest 1000000 naive` uses the same sort without the generated `children_into`. It overflows the stack ("thread 'main' has overflowed its stack").
  - This is not a theoretical concern: the iterative drop is required.

### Reading the numbers

- **nqueens is 4x faster and uses 3 to 12 times less memory.**
  - This is mostly a gain from typed representation of builtins, not from `match` itself. `int` is a plain `i64` field, and `qint` (only the injection `@ : (int) qint`) is an unboxed `Copy` enum.
  - So `x-1`, `d-p != diff` and the `range` iteration do no table probes and no allocations. The only hash-consed nodes are the list cells and `queens(n,size)`.
  - The arena version hash-consed every integer, a probe per arithmetic result. That version could get some of this gain by unboxing ints in its node words, but the typed version gets it for free from the sort declarations.
  - RSS stays flat (3.3 MB for nqueens 12) because the backtracking discards the partial boards, and reference counting frees them at once.
- **propc is 1.4x slower (0.40 against 0.29 s) but uses about 6x less memory.**
  - The `immortal` build runs at the same speed (0.40 s) and needs 286 MB. So freeing (refcount reaching zero, `drop_unique`, weak-entry purge) costs nothing measurable, and it saves 95% of the memory.
  - The slowdown comes from the representation of a node. An AC node is two allocations: the `Rc` box (48 bytes) and the `Box<[P]>` of elements. Every element of a merge or AC rest is an `Rc` clone, which writes to the *element's* node (a cache miss). The arena copied `u32`s without touching the elements.
  - About 30% of the interns are hits (propc 3: 615k hits, 1.49M nodes created). For each hit, the freshly built multiset is cloned, then dropped (n increments, n decrements, malloc+free).
  - A profile of propc 3 (`sample`) shows where the worker thread's time goes:
    - `Table::intern` (hash, probe, compare) ≈ 35%;
    - `ac_rest` ≈ 16% (mostly refcount increments);
    - `drop_unique` ≈ 11%;
    - malloc/free ≈ 12%.
  - Making `ac_rest` preallocate took propc 3 from 0.47 to 0.40 s.
  - Not done: looking up a multiset *by borrowed slice* before building it, to avoid the clone/drop on hits. It needs a hash that can be computed from the parts, so a generated `hash_parts` per variant. It is the obvious next step.

## Design

### Sorts and hash-consing over several enums

```rust
pub trait Sort: Eq + Hash + Sized + 'static {
    fn table() -> &'static LocalKey<RefCell<Table<Self>>>; // unique_table!(Self)
    fn children_into(self, out: &mut Vec<H<Self>>) {}       // same-sort children
}
pub struct H<T: Sort>(ManuallyDrop<Rc<Node<T>>>);           // a shared term
struct Node<T> { id: u64, v: T }
```

- **One enum per sort.** The generator emits one enum per sort, with one variant per operator. That includes defined operators (`generate_list`, `queens(@)`, `not`, `q1`...), because a stuck call must stay representable. It derives `PartialEq, Eq, Hash`.
  - Children are `H<Sort>`, `i64` (int), or an unboxed value enum.
  - Because `H` compares by `Rc::ptr_eq` and hashes by id, the derived `Eq`/`Hash` are shallow, O(arity). The generic table needs nothing else from a sort.
- **One unique table per sort**, `Table<T>`, in a `thread_local!` declared by the `unique_table!(T)` macro inside the `Sort` impl. Generic code cannot declare a generic thread-local, so each sort gets its own. `H::new(v)` borrows that sort's table and interns `v`.
  - The table is open addressing with linear probing. A slot is `(hash: u64, Option<Weak<Node<T>>>)`, 16 bytes.
  - A lookup compares the stored hash first. It touches the node only on a hash match, through `Weak::upgrade`.
  - A dead entry (strong count 0) is reused by the next insertion in its probe chain.
  - When used slots exceed 1/2, the table is *rebuilt*. Live entries are reinserted; dead `Weak`s are dropped, which deallocates their boxes. The capacity is the smallest power of 2 ≥ 4096 holding live entries at load ≤ 1/4, so the table can shrink as well as grow.
  - Freeing never touches the table: dead entries are purged lazily. So `Drop` cannot re-enter a `RefCell` borrow, even when the drop happens inside `intern` (dropping the candidate value after a hit).
- **Ids.** One global creation counter is shared by all sorts. The id hashes children and orders AC multisets. With 64 bits it never wraps.
- **Unboxed sorts.** A sort whose constructors carry only builtins and that is not recursive (`qint`) is emitted as a `Copy` enum with no hash-consing. Equality is structural, so it is still O(1) here.
- **Builtins.** `int` is `i64` and `bool` is `bool`. The builtin `and` stays strict (`&`, not `&&`), as in the first version, so `noattack` still walks the whole list and the step counts match.
- **Constants** are built once per thread (`thread_local!`), in declaration order, so their ids are fixed (propc: `t, f, a1..a18`). `c_t()` is a refcount increment.
- **Freeing deep terms.** `impl Drop for H<T>` checks whether this is the last reference.
  - If it is not, the drop is a plain decrement.
  - If it is, `drop_unique` loops: `Rc::try_unwrap` the node, move its same-sort children (generated `children_into`, by destructuring the owned enum) onto an explicit stack, and repeat.
  - Children of *other* sorts are dropped normally and run their own loop. So stack depth is bounded by the nesting of sorts, not by term depth.
  - This is the only place with `unsafe`: two `ManuallyDrop::take` calls, which take the `Rc` out of an `H` in `drop`/`into_rc`.

### AC multisets

`And(Ms<Prop>)` with `Ms<T> = Box<[H<T>]>`: n ≥ 2 elements, sorted by **creation id**, duplicates kept.

- **Flattening is generated per symbol.** `and_elems(p)` gives the element slice of `p` if `p` is an `And`, else `slice::from_ref(p)`. Then `ac_merge` merges two sorted slices.
- **Runtime helpers.** `ac_first_occ` and `ac_mult` iterate over distinct elements. `ac_find` is a binary search on id. `ac_rest` returns `Rest::{Empty, One(x), Many(ms)}`; the generated `and_rest`/`xor_rest` wrap `Many` in the right variant.
- **Why creation id.** The order is deterministic: same program, same input, same order. The Rc address would vary between runs (ASLR, malloc). A structural order would be history-independent, but comparisons would be O(size).
  - The price: with freeing, the id of a term depends on whether an equal term was alive when it was built. A normal form is still a correct normal form, but the printed order of a non-trivial AC normal form, and the step count, can differ from the never-freeing version (see propc above).
  - If a compiler needs history-independent output, it should order AC arguments structurally when it *prints*, or keep a structural order everywhere.

### Continuations and strategies

```rust
pub type K<'a, T> = &'a mut dyn FnMut(&T) -> bool;  // typed by sort
pub type Strat<T> = fn(&T, K<T>) -> bool;
```

This is the same CPS as the first version. Two things change:

- There is no `rt` parameter.
- Terms are passed **by reference**, both to strategies and to continuations.

The combinators (`dk`, `dc`, `first_one`, `one`, `iterate_star`, `repeat_star`, `repeat_star_det`) are generic over the sort. A labelled rule is an ordinary function:

```rust
// [queens_n] queens(n,size) => x . ql  if n>0  where ql:=(queens_strat) queens(n-1,size)
//            where xx:=(range) size  where x:=()q2i(xx)  if noattack(1,x,ql)
fn r_queens_n(t: &L, k: K<L>) -> bool {
    let List::Queens2(n, size) = **t else { return false };
    if !(n > 0) { return false; }
    s_queens_strat(&queens2(n - 1, size), &mut |ql| {
        s_range(&QInt::At(size), &mut |xx| {
            let x = q2i(*xx);
            if !noattack(1, x, ql) { return false; }
            step();
            k(&cons(x, ql.clone()))
        })
    })
}
```

- A strategy result is borrowed by the continuation. It is cloned only when it is stored into a new term (`ql.clone()` above), so backtracking does no refcount traffic.
- Defined functions take `&P` and return an owned `P`. Constructors take owned children.
- The step counter is a `thread_local!` `Cell`.

## Line counts

The first figure is total lines; the second excludes blank and comment lines.

| file | rust-enum | first version (`../rust`) |
|---|---|---|
| runtime | **517 / 403** | 588 / 472 |
| nqueens (generated) | 209 / 146 | 215 / 162 |
| propc (generated) | 595 / 542 | 575 / 530 |
| of which q1..q3 right-hand sides | ≈320 (nested expressions, one argument per line when long) | ≈350 (A-normal-form `let`s) |

Where the lines moved:

- **The runtime shrank by 15%.** It lost the arena, node layout, symbol table, generic printer, integer/bool builtins and `rt` accessors. It gained the generic weak unique table, `H` (Clone/Eq/Hash/Ord/Deref/Drop) and the iterative drop. AC helpers and combinators are about the same size.
- **The generated code did not shrink.** Each sort now carries its enum, a `Sort` impl (`children_into`), a `Display` impl (about 25 lines for `Prop`), constant accessors and the AC flattening/rest adapters. These replace the symbol table (`SYMS`) and the runtime's generic printer.
  - The rule code itself is shorter and closer to the source. For example, `not(&xor(x, &and(x, y)))` replaces three `let`s.

## What `match` handles natively, and what is still generated by hand

**Native:**

- **One level of a free pattern**, including literals and nested `i64` literals: `List::Queens2(0, _)`, `List::Cons(p, l)`, `match v0 { 0 => .., n => .. }`.
- **Exhaustiveness checking per sort.** The compiler forced the `_ =>` arm of `noattack`.
- **Bindings by reference** into the node: `p`, `l` are `&i64` and `&L`, with no copies.
- **`let ... else`** for single-pattern labelled rules.
- Rule order is source order, as `match` tries arms in order.
- Builtin comparisons and arithmetic on `i64`.

**Still generated by hand:**

- **Nested patterns.** `box`/deref patterns are not stable. A pattern like `f(g(x), y)` needs one `match` per `H` level (`match &**a { G(x) => ...}`). The generator must split a pattern into a decision tree at every shared-term boundary, as REM's matching automaton does. The first version had the same problem with every argument.
- **Non-linear patterns** (`and(x,x)`): equality tests (`==` is pointer equality).
- **Conditions and `where`**, as before: `if` and nested closures.
- **Everything AC**:
  - flattening (`and_elems`),
  - merge,
  - iteration over distinct elements,
  - multiplicity,
  - search,
  - rest-building,
  - the extension variable of `and(x,x)`,
  - re-normalisation of a rest bound to a returned variable.

  `match` only dispatches on the variant of an element (`let Prop::Xor(e0) = &*s[i0] else { continue }`).
- **Fallthrough.** "No rule applies → build the term" is `None` from `*_rules` → `unwrap_or(t)`.

## Difficulties for a code generator

1. **Borrowing: much easier than the first version.** There is no `rt` to thread, so there is no A-normal form. Nested right-hand sides compile as written, e.g. `xor(&and(&x, y), &and(&x, &z))`.
   - Rust evaluates arguments left to right, so terms are created in the same order as the first version's ANF. That matters for ids: with `immortal`, the step counts are identical.
   - Temporaries live until the end of the statement. In q1..q3 all intermediate results stay alive during the whole expression, exactly like the first version's `let`s.
   - No lifetime annotation was needed in generated code. Everything compiled on the first attempt except the `unique_table!` macro: it cannot name `Self` inside a nested `static`, so it takes the type.
2. **Rc clones.** The rule is mechanical:
   - parameters and continuation arguments are `&T`;
   - a constructor argument taken from a variable is `.clone()`d;
   - a fresh result is moved;
   - constants are cloned from a thread-local.

   A generator can emit `.clone()` whenever a variable is stored into a constructor. The optimiser does not remove the refcount write, so redundant clones cost one memory write each. The real cost is AC: cloning n elements per merge or rest.
3. **Recursion depth** is unchanged. Innermost normalisation and CPS recurse as before; `run_big_stack` (1 GiB reserved) is kept. Typed enums do not help here.
4. **Drop of deep terms.** The default recursive drop **does overflow** on a 1M-element list (8 MB stack). The generator must emit `children_into` for every recursive sort, and the runtime must route `Drop` through an explicit stack. Mutually recursive sorts (A contains B contains A) would need a shared heterogeneous stack, for example `Vec<Box<dyn Droppable>>`; this spike only handles same-sort recursion.
5. **Builtin result sorts.** A defined operator whose sort is a builtin (`noattack : ... bool`) has no representation when no rule applies: Rust `bool` has no `noattack(...)` value. The spike panics. A real compiler must either prove such functions total, or wrap builtin sorts in an enum with a "stuck term" variant, at a cost on every builtin operation.
6. **Compile time.** The deeply nested q1..q3 expressions create many temporaries that need drop glue and unwinding cleanup paths. propc builds in 0.66 s against 0.25 s, and in 0.48 s with `panic=abort`.
   - On a large program, the generator should emit `panic=abort` and break very large right-hand sides into `let`s. Those `let`s are for compile time, not for the borrow checker.
7. **Thread-locals.** Unique tables, constants and the step counter live in `thread_local!`, one per sort. The runtime is therefore per thread, and terms are `!Send` (`Rc`), so parallel rewriting would need `Arc` and a concurrent table.
   - On macOS, the `const` thread-locals access cheaply: no `tlv_get_addr` call showed up in the profile.
8. **AC order instability under freeing.** See above. It is invisible for confluent, terminating systems as far as normal forms go, but it shows in step counts and in the printed order of AC normal forms.

## Assessment for a real compiler

**For.**

- Memory management is solved without a collector. Hash-consed terms are acyclic by construction, so reference counting plus a weak unique table frees everything: propc 3 drops from 90 MB (arena) and 286 MB (same code, never freeing) to 15 MB, at no time cost.
- Builtins become native: ints are `i64`, injections are unboxed. That alone makes nqueens 4x faster.
- The generated rule code is closer to the source: nested expressions, no ANF, `match` with exhaustiveness checks.
- The type checker catches sort errors in generated code.

**Against.**

- An AC node costs two allocations, and every merge or rest clones its elements. propc is 1.4x slower than the arena, and per live node it is 3 times bigger (immortal: 286 against 90 MB). Lookup-by-parts before building (avoid allocating on the 30% of hits) and a small-vector representation for short multisets would reduce this. An arena-plus-collector in the first version's style remains faster on AC-heavy code.
- Build time grows, and the generator has more to emit per sort:
  - the enum,
  - `Sort`,
  - `children_into`,
  - `Display`,
  - constants,
  - AC adapters,
  - and, below, a parser.

**Reading query terms at run time.**

- With the arena, a query is parsed generically: a symbol table maps names to codes and arities.
- With typed enums, the query must be parsed *into a given sort*. The generator must emit, per sort, a function `parse_<sort>(&Syntax) -> Result<H<Sort>, Error>` that maps each operator name and arity to its variant and recurses on the argument sorts. Overloaded names are resolved by the expected sort, as ELAN's parser does.
  - This is mechanical and table-driven, but it is generated code proportional to the signature.
  - The first version needed only a runtime symbol table.
- A dynamic term (query or `.lgi` input) can then only be built for sorts known at compile time. For ELAN, where the query sort is declared, that is acceptable.

**Generic printing.**

- `Display` is generated per sort: a `match` per variant, with prefix, infix or mixfix layout from the operator declaration.
- Anything *generic over all sorts* needs a uniform view: printing, a debugger, tracing, term size statistics, structural ordering for history-independent AC output. The view would be a `trait Term { fn sym(&self) -> SymId; fn args(&self) -> Vec<&dyn Term> }` generated per sort, or a conversion to an untyped tree.
- The first version had all of this for free from the symbol table.

**Verdict.**

- Typed enums with `Rc` and a weak hash-consing table are a good fit for the **free, non-AC part** of ELAN programs. The gains are clearer code, native builtins, and automatic freeing with bounded memory and no GC.
- On **AC-heavy** code they lose about 1.4x to the never-freeing arena, while beating it by 6x on memory.
- The cost moves into the generator, which must emit per-sort machinery: enum, drop, printer, parser, AC adapters. It must also handle three things the untyped representation hid:
  - stuck terms of builtin sorts;
  - nested patterns through `Rc`;
  - recursion in `Drop`.
- A realistic compiler could keep this representation and recover AC speed with:
  - lookup-by-parts in the unique table;
  - inline small multisets;
  - possibly a per-sort slab allocator instead of `Rc`'s malloc.
