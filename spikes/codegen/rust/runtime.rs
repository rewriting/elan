// ELAN codegen spike -- Rust runtime library.
//
// Terms are u32 indices into an arena of nodes that is never freed.
// Every node is hash-consed through one open-addressing unique table, so
// structural equality is index equality. AC symbols are stored flattened,
// with their arguments sorted by index (duplicates kept: multisets).
// Builtin integers are nodes of symbol INT carrying their value.
//
// Strategies are compiled in continuation-passing style: a strategy is a
// function `s(rt, t, k) -> bool` that calls `k(rt, r)` for each result r in
// order; a `true` returned by `k` means "stop", and is propagated upwards.

#![allow(dead_code)]

use std::fmt::Write as _;

pub type Term = u32;

/// Continuation: receives each result; returns true to stop the search.
/// The runtime is passed explicitly (not captured), so that nested closures
/// never hold a borrow of it.
pub type K<'a> = &'a mut dyn FnMut(&mut Rt, Term) -> bool;
/// A compiled strategy or labelled rule.
pub type Strat = fn(&mut Rt, Term, K) -> bool;

// Symbol codes reserved by the runtime. Programs number their own symbols
// from FIRST_USER_SYM, and their symbol table starts with these three.
pub const INT: u32 = 0;
pub const TRUE: u32 = 1;
pub const FALSE: u32 = 2;
pub const FIRST_USER_SYM: u32 = 3;

#[derive(Clone, Copy)]
pub enum Kind {
    Int,
    /// constant or prefix function symbol: `f`, `f(a,b)`
    Prefix,
    /// binary infix symbol printed `a<op>b`, e.g. lists `x.l`
    Infix(&'static str),
    /// AC symbol, printed in prefix form `and(a,b,c)` or infix if Some(op)
    AC(Option<&'static str>),
    /// record-like mixfix `Fib[farg=..,val=..]`
    Record(&'static [&'static str]),
}

pub struct Sym {
    pub name: &'static str,
    pub arity: u32,
    pub kind: Kind,
}

#[derive(Clone, Copy)]
struct Node {
    sym: u32,
    len: u32,
    /// offset into `args` (len > 0) or integer value (sym == INT)
    data: u64,
}

const EMPTY_SLOT: u32 = u32::MAX;

pub struct Rt {
    nodes: Vec<Node>,
    args: Vec<Term>,
    table: Vec<u32>,
    mask: usize,
    syms: &'static [Sym],
    consts: Vec<Term>,
    buf: Vec<Term>,
    pub steps: u64,
    pub t_true: Term,
    pub t_false: Term,
}

#[inline(always)]
fn mix(h: u64, x: u64) -> u64 {
    (h.rotate_left(5) ^ x).wrapping_mul(0x517c_c1b7_2722_0a95)
}

#[inline]
fn hash_app(sym: u32, args: &[Term]) -> u64 {
    let mut h = mix(0x9e37_79b9, sym as u64);
    for &a in args {
        h = mix(h, a as u64);
    }
    h ^ (h >> 29)
}

#[inline]
fn hash_int(v: i64) -> u64 {
    let h = mix(mix(0x9e37_79b9, INT as u64), v as u64);
    h ^ (h >> 29)
}

impl Rt {
    pub fn new(syms: &'static [Sym]) -> Rt {
        let cap = 1 << 16;
        let mut rt = Rt {
            nodes: Vec::with_capacity(cap / 2),
            args: Vec::with_capacity(cap),
            table: vec![EMPTY_SLOT; cap],
            mask: cap - 1,
            syms,
            consts: Vec::new(),
            buf: Vec::new(),
            steps: 0,
            t_true: 0,
            t_false: 0,
        };
        // pre-build every constant (arity 0, non-int) once
        for (code, s) in syms.iter().enumerate() {
            let t = if s.arity == 0 && !matches!(s.kind, Kind::Int) {
                rt.mk(code as u32, &[])
            } else {
                EMPTY_SLOT
            };
            rt.consts.push(t);
        }
        rt.t_true = rt.consts[TRUE as usize];
        rt.t_false = rt.consts[FALSE as usize];
        rt
    }

    // ------------------------------------------------------------------
    // Hash-consing
    // ------------------------------------------------------------------

    fn node_hash(&self, id: Term) -> u64 {
        let n = self.nodes[id as usize];
        if n.sym == INT {
            hash_int(n.data as i64)
        } else {
            let o = n.data as usize;
            hash_app(n.sym, &self.args[o..o + n.len as usize])
        }
    }

    fn grow(&mut self) {
        let cap = self.table.len() * 2;
        self.table = vec![EMPTY_SLOT; cap];
        self.mask = cap - 1;
        for id in 0..self.nodes.len() as u32 {
            let mut i = self.node_hash(id) as usize & self.mask;
            while self.table[i] != EMPTY_SLOT {
                i = (i + 1) & self.mask;
            }
            self.table[i] = id;
        }
    }

    #[inline]
    fn insert_at(&mut self, slot: usize, n: Node) -> Term {
        let id = self.nodes.len() as Term;
        self.nodes.push(n);
        self.table[slot] = id;
        if self.nodes.len() * 2 > self.table.len() {
            self.grow();
        }
        id
    }

    /// The unique term `sym(args...)` (args already normalised / canonical).
    pub fn mk(&mut self, sym: u32, args: &[Term]) -> Term {
        let mut i = hash_app(sym, args) as usize & self.mask;
        loop {
            let id = self.table[i];
            if id == EMPTY_SLOT {
                break;
            }
            let n = self.nodes[id as usize];
            if n.sym == sym && n.len as usize == args.len() {
                let o = n.data as usize;
                if &self.args[o..o + args.len()] == args {
                    return id;
                }
            }
            i = (i + 1) & self.mask;
        }
        let off = self.args.len() as u64;
        self.args.extend_from_slice(args);
        self.insert_at(i, Node { sym, len: args.len() as u32, data: off })
    }

    #[inline]
    pub fn mk1(&mut self, sym: u32, a: Term) -> Term {
        self.mk(sym, &[a])
    }
    #[inline]
    pub fn mk2(&mut self, sym: u32, a: Term, b: Term) -> Term {
        self.mk(sym, &[a, b])
    }
    #[inline]
    pub fn mk3(&mut self, sym: u32, a: Term, b: Term, c: Term) -> Term {
        self.mk(sym, &[a, b, c])
    }

    /// Pre-built constant of symbol `sym`.
    #[inline(always)]
    pub fn c(&self, sym: u32) -> Term {
        self.consts[sym as usize]
    }

    /// The unique builtin integer term holding `v`.
    pub fn int(&mut self, v: i64) -> Term {
        let mut i = hash_int(v) as usize & self.mask;
        loop {
            let id = self.table[i];
            if id == EMPTY_SLOT {
                break;
            }
            let n = self.nodes[id as usize];
            if n.sym == INT && n.data as i64 == v {
                return id;
            }
            i = (i + 1) & self.mask;
        }
        self.insert_at(i, Node { sym: INT, len: 0, data: v as u64 })
    }

    // ------------------------------------------------------------------
    // Accessors
    // ------------------------------------------------------------------

    #[inline(always)]
    pub fn sym(&self, t: Term) -> u32 {
        self.nodes[t as usize].sym
    }
    #[inline(always)]
    pub fn arity(&self, t: Term) -> usize {
        self.nodes[t as usize].len as usize
    }
    #[inline(always)]
    pub fn arg(&self, t: Term, i: usize) -> Term {
        let n = self.nodes[t as usize];
        self.args[n.data as usize + i]
    }
    #[inline(always)]
    pub fn int_val(&self, t: Term) -> i64 {
        debug_assert!(self.sym(t) == INT);
        self.nodes[t as usize].data as i64
    }
    pub fn node_count(&self) -> usize {
        self.nodes.len()
    }

    // ------------------------------------------------------------------
    // Builtins (int, bool). Ints and booleans stay terms.
    // ------------------------------------------------------------------

    #[inline]
    pub fn bool_t(&self, b: bool) -> Term {
        if b {
            self.t_true
        } else {
            self.t_false
        }
    }
    pub fn add(&mut self, a: Term, b: Term) -> Term {
        let v = self.int_val(a).wrapping_add(self.int_val(b));
        self.int(v)
    }
    pub fn sub(&mut self, a: Term, b: Term) -> Term {
        let v = self.int_val(a).wrapping_sub(self.int_val(b));
        self.int(v)
    }
    pub fn rem(&mut self, a: Term, b: Term) -> Term {
        let v = self.int_val(a) % self.int_val(b);
        self.int(v)
    }
    /// Comparisons on hash-consed ints: equality is index equality.
    #[inline]
    pub fn eq(&self, a: Term, b: Term) -> Term {
        self.bool_t(a == b)
    }
    #[inline]
    pub fn ne(&self, a: Term, b: Term) -> Term {
        self.bool_t(a != b)
    }
    #[inline]
    pub fn gt(&self, a: Term, b: Term) -> Term {
        self.bool_t(self.int_val(a) > self.int_val(b))
    }
    #[inline]
    pub fn and(&self, a: Term, b: Term) -> Term {
        self.bool_t(a == self.t_true && b == self.t_true)
    }
    #[inline]
    pub fn not(&self, a: Term) -> Term {
        self.bool_t(a != self.t_true)
    }

    // ------------------------------------------------------------------
    // AC canonical form and matching helpers
    // ------------------------------------------------------------------

    /// Build `sym(a, b)` for an AC symbol: flatten both sides, merge their
    /// sorted element lists. The result has >= 2 arguments.
    pub fn ac2(&mut self, sym: u32, a: Term, b: Term) -> Term {
        let mut buf = std::mem::take(&mut self.buf);
        buf.clear();
        let (ao, al) = self.ac_span(sym, a);
        let (bo, bl) = self.ac_span(sym, b);
        let (mut i, mut j) = (0, 0);
        while i < al && j < bl {
            let x = self.elem(a, ao, i);
            let y = self.elem(b, bo, j);
            if x <= y {
                buf.push(x);
                i += 1;
            } else {
                buf.push(y);
                j += 1;
            }
        }
        while i < al {
            buf.push(self.elem(a, ao, i));
            i += 1;
        }
        while j < bl {
            buf.push(self.elem(b, bo, j));
            j += 1;
        }
        let r = self.mk(sym, &buf);
        self.buf = buf;
        r
    }

    // (offset, length) of the element list of `t` seen under AC symbol `sym`;
    // offset usize::MAX means "t itself is the single element".
    #[inline]
    fn ac_span(&self, sym: u32, t: Term) -> (usize, usize) {
        let n = self.nodes[t as usize];
        if n.sym == sym {
            (n.data as usize, n.len as usize)
        } else {
            (usize::MAX, 1)
        }
    }
    #[inline]
    fn elem(&self, t: Term, off: usize, i: usize) -> Term {
        if off == usize::MAX {
            t
        } else {
            self.args[off + i]
        }
    }

    /// Is position `j` of AC term `t` the first occurrence of its element
    /// among the positions not in `removed`? (used to iterate over the
    /// distinct elements of a multiset)
    #[inline]
    pub fn ac_first_occ(&self, t: Term, j: usize, removed: &[usize]) -> bool {
        if removed.contains(&j) {
            return false;
        }
        let e = self.arg(t, j);
        let mut p = j;
        while p > 0 {
            p -= 1;
            if self.arg(t, p) != e {
                return true;
            }
            if !removed.contains(&p) {
                return false;
            }
        }
        true
    }

    /// Multiplicity of the element at position `j` (first occurrence).
    #[inline]
    pub fn ac_mult(&self, t: Term, j: usize) -> usize {
        let e = self.arg(t, j);
        let n = self.arity(t);
        let mut m = 1;
        while j + m < n && self.arg(t, j + m) == e {
            m += 1;
        }
        m
    }

    /// Position of element `e` in AC term `t`, if present.
    pub fn ac_find(&self, t: Term, e: Term) -> Option<usize> {
        let n = self.nodes[t as usize];
        let o = n.data as usize;
        self.args[o..o + n.len as usize].binary_search(&e).ok()
    }

    /// The rest of AC term `t` without the positions in `removed`:
    /// None if empty, the element itself if one remains, else an AC term.
    pub fn ac_rest(&mut self, sym: u32, t: Term, removed: &[usize]) -> Option<Term> {
        let n = self.arity(t);
        let left = n - removed.len();
        if left == 0 {
            return None;
        }
        if left == 1 {
            for i in 0..n {
                if !removed.contains(&i) {
                    return Some(self.arg(t, i));
                }
            }
        }
        let mut buf = std::mem::take(&mut self.buf);
        buf.clear();
        let o = self.nodes[t as usize].data as usize;
        for i in 0..n {
            if !removed.contains(&i) {
                buf.push(self.args[o + i]);
            }
        }
        let r = self.mk(sym, &buf);
        self.buf = buf;
        Some(r)
    }

    // ------------------------------------------------------------------
    // Printing
    // ------------------------------------------------------------------

    pub fn show(&self, t: Term) -> String {
        let mut s = String::new();
        self.show_into(t, &mut s);
        s
    }

    fn show_into(&self, t: Term, out: &mut String) {
        let n = self.nodes[t as usize];
        let sy = &self.syms[n.sym as usize];
        match sy.kind {
            Kind::Int => {
                let _ = write!(out, "{}", n.data as i64);
            }
            Kind::Infix(op) => {
                self.show_into(self.arg(t, 0), out);
                out.push_str(op);
                self.show_into(self.arg(t, 1), out);
            }
            Kind::AC(Some(op)) => {
                for i in 0..n.len as usize {
                    if i > 0 {
                        out.push_str(op);
                    }
                    self.show_into(self.arg(t, i), out);
                }
            }
            Kind::Record(fields) => {
                out.push_str(sy.name);
                out.push('[');
                for i in 0..n.len as usize {
                    if i > 0 {
                        out.push(',');
                    }
                    out.push_str(fields[i]);
                    out.push('=');
                    self.show_into(self.arg(t, i), out);
                }
                out.push(']');
            }
            Kind::Prefix | Kind::AC(None) => {
                out.push_str(sy.name);
                if n.len > 0 {
                    out.push('(');
                    for i in 0..n.len as usize {
                        if i > 0 {
                            out.push(',');
                        }
                        self.show_into(self.arg(t, i), out);
                    }
                    out.push(')');
                }
            }
        }
    }
}

// ----------------------------------------------------------------------
// Strategy combinators (CPS)
// ----------------------------------------------------------------------

/// dk(S1,...,Sn): all results of every Si, in order.
pub fn dk(rt: &mut Rt, t: Term, k: K, ss: &[Strat]) -> bool {
    for s in ss {
        if s(rt, t, k) {
            return true;
        }
    }
    false
}

/// dc(S1,...,Sn): all results of the first Si that has at least one.
pub fn dc(rt: &mut Rt, t: Term, k: K, ss: &[Strat]) -> bool {
    for s in ss {
        let mut found = false;
        let stop = s(rt, t, &mut |rt, r| {
            found = true;
            k(rt, r)
        });
        if stop || found {
            return stop;
        }
    }
    false
}

/// The first result of `s` applied to `t`, if any.
pub fn first_result(rt: &mut Rt, t: Term, s: Strat) -> Option<Term> {
    let mut res = None;
    s(rt, t, &mut |_, r| {
        res = Some(r);
        true
    });
    res
}

/// first one(S1,...,Sn): exactly one result, from the first Si that has one.
pub fn first_one(rt: &mut Rt, t: Term, k: K, ss: &[Strat]) -> bool {
    for &s in ss {
        if let Some(r) = first_result(rt, t, s) {
            return k(rt, r);
        }
    }
    false
}

/// one(S): the first result of S.
pub fn one(rt: &mut Rt, t: Term, k: K, s: Strat) -> bool {
    match first_result(rt, t, s) {
        Some(r) => k(rt, r),
        None => false,
    }
}

/// iterate*(S): t, then for each result r of S(t), iterate*(S)(r).
pub fn iterate_star(rt: &mut Rt, t: Term, k: K, s: Strat) -> bool {
    if k(rt, t) {
        return true;
    }
    s(rt, t, &mut |rt, r| iterate_star(rt, r, k, s))
}

/// repeat*(S), general (non-deterministic S): the terms reached when S fails.
pub fn repeat_star(rt: &mut Rt, t: Term, k: K, s: Strat) -> bool {
    let mut any = false;
    let stop = s(rt, t, &mut |rt, r| {
        any = true;
        repeat_star(rt, r, k, s)
    });
    if stop {
        return true;
    }
    if !any {
        return k(rt, t);
    }
    false
}

/// repeat*(S) when S is deterministic (one / first one): a loop.
pub fn repeat_star_det(rt: &mut Rt, mut t: Term, k: K, s: Strat) -> bool {
    while let Some(r) = first_result(rt, t, s) {
        t = r;
    }
    k(rt, t)
}

// ----------------------------------------------------------------------
// Driver
// ----------------------------------------------------------------------

/// Run `f` on a thread with a large stack (deep CPS / innermost recursion).
pub fn run_big_stack<F: FnOnce() + Send + 'static>(f: F) {
    std::thread::Builder::new()
        .stack_size(1 << 30)
        .spawn(f)
        .expect("spawn")
        .join()
        .expect("join");
}

pub fn arg_i64() -> i64 {
    std::env::args()
        .nth(1)
        .and_then(|s| s.parse().ok())
        .unwrap_or_else(|| {
            eprintln!("usage: prog <int>");
            std::process::exit(2)
        })
}
