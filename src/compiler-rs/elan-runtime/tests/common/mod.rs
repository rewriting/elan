//! A hand-written "generated-like" module, as `elanc-rs` would emit it:
//!
//! ```text
//! sort Nat   : z, s(@), plus(@,@)          (plus defined: stuck if no rule)
//! sort List  : nil, @.@ : (int List) List  (self-recursive)
//! sort A, B  : a_end, a(@,@) : (int B) A ; b(@) : (A) B  (mutually recursive)
//! sort Prop  : t, f, p(@) : (int) Prop, not(@), and(@,@) (AC)
//! ```
#![allow(dead_code)]

use elan_runtime::ac::{self, AcSet};
use elan_runtime::print::{Print, Writer};
use elan_runtime::{fx_hash, impl_sort, unique_table, Int, Sort, H};

// ---- Nat ---------------------------------------------------------------
#[derive(PartialEq, Eq, Hash, Debug)]
pub enum Nat {
    Z,
    S(H<Nat>),
    Plus(H<Nat>, H<Nat>),
}
impl_sort!(Nat);
pub type N = H<Nat>;

pub fn z() -> N {
    H::new(Nat::Z)
}
pub fn s(x: &N) -> N {
    H::new(Nat::S(x.clone()))
}
pub fn nat(n: usize) -> N {
    let mut x = z();
    for _ in 0..n {
        x = s(&x);
    }
    x
}

impl Print for Nat {
    fn print(&self, w: &mut Writer) {
        match self {
            Nat::Z => w.ident("o"),
            Nat::S(x) => w.prefix("s", &[x]),
            Nat::Plus(x, y) => {
                x.print(w);
                w.ch('+');
                y.print(w)
            }
        }
    }
}

// ---- List (self-recursive) ----------------------------------------------
#[derive(PartialEq, Eq, Hash, Debug)]
pub enum List {
    Nil,
    Cons(Int, H<List>),
}
impl_sort!(List);
pub type L = H<List>;

pub fn nil() -> L {
    H::new(List::Nil)
}
pub fn cons(x: Int, l: &L) -> L {
    H::new(List::Cons(x, l.clone()))
}
/// [n-1, ..., 1, 0] built iteratively.
pub fn list(n: usize) -> L {
    let mut l = nil();
    for i in 0..n {
        l = H::new(List::Cons(i as Int, l));
    }
    l
}

impl Print for List {
    fn print(&self, w: &mut Writer) {
        match self {
            List::Nil => w.ident("nil"),
            List::Cons(x, l) => {
                w.int(*x);
                w.ch('.');
                l.print(w)
            }
        }
    }
}

// ---- A, B (mutually recursive) -------------------------------------------
#[derive(PartialEq, Eq, Hash, Debug)]
pub enum A {
    End,
    A(Int, H<B>),
}
#[derive(PartialEq, Eq, Hash, Debug)]
pub enum B {
    B(H<A>),
}
impl_sort!(A);
impl_sort!(B);

/// a(n-1, b(a(n-2, b(... a(0, b(end))))))
pub fn chain(n: usize) -> H<A> {
    let mut x = H::new(A::End);
    for i in 0..n {
        let b = H::new(B::B(x));
        x = H::new(A::A(i as Int, b));
    }
    x
}

// ---- Prop (AC and) ---------------------------------------------------------
#[derive(PartialEq, Eq, Hash, Debug)]
pub enum Prop {
    T,
    F,
    P(Int),
    Not(H<Prop>),
    And(AcSet<Prop>),
}
pub type P = H<Prop>;

pub const TAG_AND: u64 = 1;

impl Sort for Prop {
    unique_table!(Prop);
    // lookups by parts on `and` (ac::ac_hash), default hash elsewhere
    fn node_hash(&self) -> u64 {
        match self {
            Prop::And(s) => ac::ac_hash(TAG_AND, s),
            other => fx_hash(other),
        }
    }
}

pub fn and_view(p: &Prop) -> Option<&AcSet<Prop>> {
    match p {
        Prop::And(s) => Some(s),
        _ => None,
    }
}
pub fn atom(i: Int) -> P {
    H::new(Prop::P(i))
}
pub fn not(x: &P) -> P {
    H::new(Prop::Not(x.clone()))
}
/// and(x, y), flattened (not normalised)
pub fn and(x: &P, y: &P) -> P {
    H::new(Prop::And(ac::flat2(x, y, and_view)))
}
/// and(x, y), looking the merge up before building it.
pub fn and_lookup(x: &P, y: &P) -> P {
    let (a, b) = (ac::elems(x, and_view), ac::elems(y, and_view));
    let h = ac::ac_hash_merge(TAG_AND, a, b);
    match H::find(h, |v| matches!(v, Prop::And(s) if ac::ac_eq_merge(s, a, b))) {
        Some(t) => t,
        None => H::new(Prop::And(AcSet::merge(a, b))),
    }
}

impl Print for Prop {
    fn print(&self, w: &mut Writer) {
        match self {
            Prop::T => w.ident("t"),
            Prop::F => w.ident("f"),
            Prop::P(i) => {
                w.ident("p");
                w.int(*i)
            }
            Prop::Not(x) => w.prefix("not", &[x]),
            Prop::And(s) => {
                w.ident("and");
                w.ch('(');
                for (i, x) in s.iter().enumerate() {
                    if i > 0 {
                        w.ch(',');
                    }
                    x.print(w);
                }
                w.ch(')')
            }
        }
    }
}

/// Run `f` on a fresh thread (fresh thread-locals) with a stack of `kb` KiB.
pub fn on_thread<R: Send + 'static>(kb: usize, f: impl FnOnce() -> R + Send + 'static) -> R {
    elan_runtime::driver::run_with_stack(kb * 1024, f)
}
