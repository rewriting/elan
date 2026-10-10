// ELAN codegen spike -- Rust runtime for terms as typed algebraic data types.
//
// Each ELAN sort is a Rust enum (one variant per operator). A shared term of
// sort T is an `H<T>`: a reference-counted pointer to a `Node<T>` that is
// hash-consed through a per-sort unique table holding `Weak` pointers, so a
// term nobody references is freed by reference counting. Equality of terms is
// pointer equality; every node carries a stable creation id (one global
// counter shared by all sorts) used for hashing children and for ordering
// the elements of AC multisets.
//
// What is left in the runtime: hash-consing (`H`, `Table`, `Sort`), AC
// multiset helpers, the CPS strategy combinators, the step counter, and the
// driver. Pattern matching and printing are generated per sort.

#![allow(dead_code)]

use std::cell::{Cell, RefCell};
use std::hash::{Hash, Hasher};
use std::mem::ManuallyDrop;
use std::ops::Deref;
use std::rc::{Rc, Weak};
use std::thread::LocalKey;

// ----------------------------------------------------------------------
// Sorts and hash-consed handles
// ----------------------------------------------------------------------

/// Implemented (generated) for every hash-consed sort enum.
/// `Eq`/`Hash` are derived: children are `H<_>`, compared by pointer and
/// hashed by id, so both are shallow (O(arity)).
pub trait Sort: Eq + Hash + Sized + 'static {
    /// The unique table of this sort (declare it with `unique_table!(Type)`).
    fn table() -> &'static LocalKey<RefCell<Table<Self>>>;
    /// Move the children *of the same sort* out of a node being freed, so
    /// that freeing a deep term is a loop, not a recursion (see `H::drop`).
    fn children_into(self, _out: &mut Vec<H<Self>>) {}
}

/// Body of `Sort::table` for a generated sort: one thread-local table.
#[macro_export]
macro_rules! unique_table {
    ($t:ty) => {
        fn table() -> &'static ::std::thread::LocalKey<::std::cell::RefCell<Table<$t>>> {
            thread_local! {
                static TABLE: ::std::cell::RefCell<Table<$t>> =
                    const { ::std::cell::RefCell::new(Table::new()) };
            }
            &TABLE
        }
    };
}

pub struct Node<T> {
    id: u64,
    v: T,
}

/// A shared (hash-consed) term of sort T.
pub struct H<T: Sort>(ManuallyDrop<Rc<Node<T>>>);

thread_local! {
    static NEXT_ID: Cell<u64> = const { Cell::new(0) };
    static STEPS: Cell<u64> = const { Cell::new(0) };
}

impl<T: Sort> H<T> {
    /// The unique term with this top constructor and these (shared) children.
    #[inline]
    pub fn new(v: T) -> H<T> {
        H(ManuallyDrop::new(T::table().with(|t| t.borrow_mut().intern(v))))
    }
    #[inline(always)]
    pub fn id(&self) -> u64 {
        self.0.id
    }
    #[inline]
    fn into_rc(self) -> Rc<Node<T>> {
        let mut me = ManuallyDrop::new(self);
        // SAFETY: `me` is never used or dropped again.
        unsafe { ManuallyDrop::take(&mut me.0) }
    }
}

impl<T: Sort> Drop for H<T> {
    /// Freeing a term frees its children; for a deep term (a long list) a
    /// recursive drop would overflow the stack, so the same-sort children of
    /// a node being freed are pushed on an explicit stack instead.
    #[inline]
    fn drop(&mut self) {
        // SAFETY: self.0 is not used after this.
        let rc = unsafe { ManuallyDrop::take(&mut self.0) };
        if Rc::strong_count(&rc) != 1 {
            return; // just a decrement
        }
        drop_unique(rc);
    }
}

#[inline(never)]
fn drop_unique<T: Sort>(rc: Rc<Node<T>>) {
    let mut stack: Vec<H<T>> = Vec::new();
    let mut cur = Some(rc);
    while let Some(rc) = cur {
        if let Ok(node) = Rc::try_unwrap(rc) {
            node.v.children_into(&mut stack);
        }
        cur = stack.pop().map(H::into_rc);
    }
}

impl<T: Sort> Clone for H<T> {
    #[inline(always)]
    fn clone(&self) -> Self {
        H(ManuallyDrop::new(Rc::clone(&self.0)))
    }
}
impl<T: Sort> PartialEq for H<T> {
    #[inline(always)]
    fn eq(&self, o: &Self) -> bool {
        Rc::ptr_eq(&self.0, &o.0)
    }
}
impl<T: Sort> Eq for H<T> {}
impl<T: Sort> Hash for H<T> {
    #[inline(always)]
    fn hash<S: Hasher>(&self, s: &mut S) {
        s.write_u64(self.0.id)
    }
}
/// Total order on shared terms: creation id (used to sort AC multisets).
impl<T: Sort> PartialOrd for H<T> {
    #[inline(always)]
    fn partial_cmp(&self, o: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(o))
    }
}
impl<T: Sort> Ord for H<T> {
    #[inline(always)]
    fn cmp(&self, o: &Self) -> std::cmp::Ordering {
        self.0.id.cmp(&o.0.id)
    }
}
impl<T: Sort> Deref for H<T> {
    type Target = T;
    #[inline(always)]
    fn deref(&self) -> &T {
        &self.0.v
    }
}
impl<T: Sort + std::fmt::Display> std::fmt::Display for H<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        self.0.v.fmt(f)
    }
}

// ----------------------------------------------------------------------
// Unique table: open addressing, linear probing, Weak entries.
// Dead entries (term freed) are reused by insertions and purged when the
// table is rebuilt; a rebuild doubles the table only if live entries need it.
// ----------------------------------------------------------------------

struct Slot<T> {
    hash: u64,
    w: Option<Weak<Node<T>>>,
}

pub struct Table<T> {
    slots: Vec<Slot<T>>,
    used: usize, // non-empty slots, live or dead
}

const MIN_CAP: usize = 1 << 12;

#[derive(Default)]
struct Fx(u64);
impl Hasher for Fx {
    #[inline(always)]
    fn finish(&self) -> u64 {
        self.0 ^ (self.0 >> 29)
    }
    #[inline(always)]
    fn write(&mut self, b: &[u8]) {
        for &x in b {
            self.write_u64(x as u64)
        }
    }
    #[inline(always)]
    fn write_u8(&mut self, x: u8) {
        self.write_u64(x as u64)
    }
    #[inline(always)]
    fn write_u32(&mut self, x: u32) {
        self.write_u64(x as u64)
    }
    #[inline(always)]
    fn write_usize(&mut self, x: usize) {
        self.write_u64(x as u64)
    }
    #[inline(always)]
    fn write_u64(&mut self, x: u64) {
        self.0 = (self.0.rotate_left(5) ^ x).wrapping_mul(0x517c_c1b7_2722_0a95)
    }
}

impl<T: Sort> Table<T> {
    pub const fn new() -> Table<T> {
        Table { slots: Vec::new(), used: 0 }
    }

    fn intern(&mut self, v: T) -> Rc<Node<T>> {
        if (self.used + 1) * 2 > self.slots.len() {
            self.rebuild();
        }
        let mut fx = Fx(0x9e37_79b9);
        v.hash(&mut fx);
        let h = fx.finish();
        let mask = self.slots.len() - 1;
        let mut i = h as usize & mask;
        let mut free = usize::MAX;
        loop {
            let s = &self.slots[i];
            match &s.w {
                None => break,
                Some(w) => {
                    if s.hash == h {
                        if let Some(rc) = w.upgrade() {
                            if rc.v == v {
                                return rc;
                            }
                        } else if free == usize::MAX {
                            free = i;
                        }
                    } else if free == usize::MAX && w.strong_count() == 0 {
                        free = i;
                    }
                }
            }
            i = (i + 1) & mask;
        }
        let id = NEXT_ID.with(|c| {
            let id = c.get();
            c.set(id + 1);
            id
        });
        let rc = Rc::new(Node { id, v });
        // feature "immortal": never free a term (arena-like, for comparison)
        #[cfg(feature = "immortal")]
        std::mem::forget(Rc::clone(&rc));
        if free == usize::MAX {
            free = i;
            self.used += 1;
        }
        self.slots[free] = Slot { hash: h, w: Some(Rc::downgrade(&rc)) };
        rc
    }

    #[cold]
    fn rebuild(&mut self) {
        let live = self
            .slots
            .iter()
            .filter(|s| s.w.as_ref().map_or(false, |w| w.strong_count() > 0))
            .count();
        let mut cap = MIN_CAP;
        while cap < live * 4 {
            cap *= 2;
        }
        let old = std::mem::replace(
            &mut self.slots,
            (0..cap).map(|_| Slot { hash: 0, w: None }).collect(),
        );
        self.used = 0;
        let mask = cap - 1;
        for s in old {
            if let Some(w) = s.w {
                if w.strong_count() > 0 {
                    let mut i = s.hash as usize & mask;
                    while self.slots[i].w.is_some() {
                        i = (i + 1) & mask;
                    }
                    self.slots[i] = Slot { hash: s.hash, w: Some(w) };
                    self.used += 1;
                }
                // a dead Weak is dropped here: its box is deallocated
            }
        }
    }

    pub fn live(&self) -> usize {
        self.slots
            .iter()
            .filter(|s| s.w.as_ref().map_or(false, |w| w.strong_count() > 0))
            .count()
    }
}

/// Number of live terms of sort T (diagnostics).
pub fn live_terms<T: Sort>() -> usize {
    T::table().with(|t| t.borrow().live())
}

// ----------------------------------------------------------------------
// Step counter
// ----------------------------------------------------------------------

#[inline(always)]
pub fn step() {
    STEPS.with(|c| c.set(c.get() + 1))
}
pub fn steps() -> u64 {
    STEPS.with(|c| c.get())
}

// ----------------------------------------------------------------------
// AC multisets: a variant of an AC symbol holds `Box<[H<T>]>`, n >= 2
// elements sorted by creation id, duplicates kept.
// ----------------------------------------------------------------------

pub type Ms<T> = Box<[H<T>]>;

/// Merge two sorted element lists (flattening is done by the caller, which
/// passes the elements of an argument that has the same AC symbol).
pub fn ac_merge<T: Sort>(a: &[H<T>], b: &[H<T>]) -> Ms<T> {
    let mut out = Vec::with_capacity(a.len() + b.len());
    let (mut i, mut j) = (0, 0);
    while i < a.len() && j < b.len() {
        if a[i] <= b[j] {
            out.push(a[i].clone());
            i += 1;
        } else {
            out.push(b[j].clone());
            j += 1;
        }
    }
    out.extend_from_slice(&a[i..]);
    out.extend_from_slice(&b[j..]);
    out.into_boxed_slice()
}

/// Is position j the first occurrence of its element among the positions
/// not in `removed`? (iterate over the distinct elements of a multiset)
#[inline]
pub fn ac_first_occ<T: Sort>(s: &[H<T>], j: usize, removed: &[usize]) -> bool {
    if removed.contains(&j) {
        return false;
    }
    let mut p = j;
    while p > 0 {
        p -= 1;
        if s[p] != s[j] {
            return true;
        }
        if !removed.contains(&p) {
            return false;
        }
    }
    true
}

/// Multiplicity of the element at position j (a first occurrence).
#[inline]
pub fn ac_mult<T: Sort>(s: &[H<T>], j: usize) -> usize {
    let mut m = 1;
    while j + m < s.len() && s[j + m] == s[j] {
        m += 1;
    }
    m
}

/// Position of element e, if present (binary search on the id order).
#[inline]
pub fn ac_find<T: Sort>(s: &[H<T>], e: &H<T>) -> Option<usize> {
    s.binary_search_by_key(&e.id(), |x| x.id()).ok()
}

/// The rest of a multiset without the positions in `removed`.
pub enum Rest<T: Sort> {
    Empty,
    One(H<T>),
    Many(Ms<T>),
}

pub fn ac_rest<T: Sort>(s: &[H<T>], removed: &[usize]) -> Rest<T> {
    match s.len() - removed.len() {
        0 => Rest::Empty,
        1 => Rest::One(
            (0..s.len()).find(|i| !removed.contains(i)).map(|i| s[i].clone()).unwrap(),
        ),
        n => {
            let mut v = Vec::with_capacity(n);
            for (i, e) in s.iter().enumerate() {
                if !removed.contains(&i) {
                    v.push(e.clone());
                }
            }
            Rest::Many(v.into_boxed_slice())
        }
    }
}

// ----------------------------------------------------------------------
// Strategy combinators (CPS), generic over the sort.
// A strategy calls k(&r) for each result r; true from k means "stop".
// ----------------------------------------------------------------------

pub type K<'a, T> = &'a mut dyn FnMut(&T) -> bool;
pub type Strat<T> = fn(&T, K<T>) -> bool;

/// dk(S1,...,Sn): all results of every Si, in order.
pub fn dk<T>(t: &T, k: K<T>, ss: &[Strat<T>]) -> bool {
    for s in ss {
        if s(t, k) {
            return true;
        }
    }
    false
}

/// dc(S1,...,Sn): all results of the first Si that has at least one.
pub fn dc<T>(t: &T, k: K<T>, ss: &[Strat<T>]) -> bool {
    for s in ss {
        let mut found = false;
        let stop = s(t, &mut |r| {
            found = true;
            k(r)
        });
        if stop || found {
            return stop;
        }
    }
    false
}

/// The first result of s applied to t, if any.
pub fn first_result<T: Clone>(t: &T, s: Strat<T>) -> Option<T> {
    let mut res = None;
    s(t, &mut |r| {
        res = Some(r.clone());
        true
    });
    res
}

/// first one(S1,...,Sn): exactly one result, from the first Si that has one.
pub fn first_one<T: Clone>(t: &T, k: K<T>, ss: &[Strat<T>]) -> bool {
    for &s in ss {
        if let Some(r) = first_result(t, s) {
            return k(&r);
        }
    }
    false
}

/// one(S): the first result of S.
pub fn one<T: Clone>(t: &T, k: K<T>, s: Strat<T>) -> bool {
    match first_result(t, s) {
        Some(r) => k(&r),
        None => false,
    }
}

/// iterate*(S): t, then for each result r of S(t), iterate*(S)(r).
pub fn iterate_star<T>(t: &T, k: K<T>, s: Strat<T>) -> bool {
    if k(t) {
        return true;
    }
    s(t, &mut |r| iterate_star(r, k, s))
}

/// repeat*(S), general: the terms reached when S fails.
pub fn repeat_star<T>(t: &T, k: K<T>, s: Strat<T>) -> bool {
    let mut any = false;
    let stop = s(t, &mut |r| {
        any = true;
        repeat_star(r, k, s)
    });
    if stop {
        return true;
    }
    if !any {
        return k(t);
    }
    false
}

/// repeat*(S) when S is deterministic: a loop.
pub fn repeat_star_det<T: Clone>(t: &T, k: K<T>, s: Strat<T>) -> bool {
    let mut t = t.clone();
    while let Some(r) = first_result(&t, s) {
        t = r;
    }
    k(&t)
}

// ----------------------------------------------------------------------
// Driver
// ----------------------------------------------------------------------

/// Run f on a thread with a large stack (deep CPS / innermost recursion).
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
