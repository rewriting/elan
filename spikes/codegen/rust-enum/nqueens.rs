#![allow(dead_code)]
// "Generated" code for module nqueens (nqueens.eln, nqueens.lgi), terms as
// typed enums. Query: N ; start with (queens_strat) queens(query).

#[macro_use]
mod runtime;
use runtime::*;
use std::fmt;

// ---- sorts -------------------------------------------------------------------
// int, bool: builtins -> i64, bool.

/// sort qint. Only constructor: the injection `@ : (int) qint`. No recursive
/// occurrence and only scalar fields: an unboxed value type (no hash-consing).
#[derive(Clone, Copy, PartialEq, Eq, Hash)]
pub enum QInt {
    At(i64), // @ : (int) qint
}

/// sort list (every operator of sort list is a variant, defined ones too).
#[derive(PartialEq, Eq, Hash)]
pub enum List {
    Nil,                // nil : list
    Cons(i64, L),       // @.@ : (int list) list
    GenerateList(i64),  // generate_list(@) : (int) list
    Queens2(i64, i64),  // queens(@,@) : (int int) list
    Queens1(i64),       // queens(@) : (int) list
}
pub type L = H<List>;

impl Sort for List {
    unique_table!(List);
    fn children_into(self, out: &mut Vec<L>) {
        if let List::Cons(_, l) = self {
            out.push(l)
        }
    }
}

impl fmt::Display for QInt {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        let QInt::At(x) = self;
        write!(f, "{}", x)
    }
}
impl fmt::Display for List {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            List::Nil => write!(f, "nil"),
            List::Cons(x, l) => write!(f, "{}.{}", x, l),
            List::GenerateList(n) => write!(f, "generate_list({})", n),
            List::Queens2(n, m) => write!(f, "queens({},{})", n, m),
            List::Queens1(n) => write!(f, "queens({})", n),
        }
    }
}

// ---- constants -------------------------------------------------------------
thread_local! {
    static C_NIL: L = H::new(List::Nil);
}
fn nil() -> L {
    C_NIL.with(|c| c.clone())
}

// ---- constructors / defined functions (innermost normalisation) --------

// constructor @.@
fn cons(x: i64, l: L) -> L {
    H::new(List::Cons(x, l))
}

// q2i(@) : rules for int
fn q2i(v0: QInt) -> i64 {
    // [] q2i(n) => n
    let QInt::At(n) = v0;
    step();
    n
}

// generate_list(@) : rules for list
fn generate_list(v0: i64) -> L {
    match v0 {
        // [] generate_list(0) => nil
        0 => {
            step();
            nil()
        }
        // [] generate_list(n) => n . generate_list(n-1)
        n => {
            step();
            cons(n, generate_list(n - 1))
        }
    }
}

// queens(@,@): no unlabelled rule, builds the term
fn queens2(v0: i64, v1: i64) -> L {
    H::new(List::Queens2(v0, v1))
}

// queens(@) : rules for list
fn queens1(v0: i64) -> L {
    // [] queens(n) => queens(n,n)
    let n = v0;
    step();
    queens2(n, n)
}

// noattack(@,@,@) : rules for bool
fn noattack(v0: i64, v1: i64, v2: &L) -> bool {
    match &**v2 {
        // [] noattack(diff,d,nil) => true
        List::Nil => {
            step();
            true
        }
        // [] noattack(diff,d,p.l) => d!=p and d-p!=diff and p-d!=diff
        //                            and noattack(diff+1,d,l)
        List::Cons(p, l) => {
            let (diff, d, p) = (v0, v1, *p);
            step();
            // builtin `and` is strict (innermost): `&`, not `&&`
            (d != p) & (d - p != diff) & (p - d != diff) & noattack(diff + 1, d, l)
        }
        // bool is a builtin type: a stuck noattack(...) has no representation
        _ => panic!("noattack: no rule applies to {}", v2),
    }
}

// ---- labelled rules (CPS) ------------------------------------------------

// [range_rule] x => x-1 if x > 1          (rules for qint)
fn r_range_rule(t: &QInt, k: K<QInt>) -> bool {
    let QInt::At(x) = *t;
    if !(x > 1) {
        return false;
    }
    step();
    k(&QInt::At(x - 1))
}

// [queens_0] queens(0,size) => nil
fn r_queens_0(t: &L, k: K<L>) -> bool {
    let List::Queens2(0, _size) = **t else { return false };
    step();
    k(&nil())
}

// [queens_n] queens(n,size) => x . ql
//    if n>0
//    where ql:=(queens_strat) queens(n-1,size)
//    where xx:=(range) size
//    where x:=()q2i(xx)
//    if noattack(1,x,ql)
fn r_queens_n(t: &L, k: K<L>) -> bool {
    let List::Queens2(n, size) = **t else { return false };
    if !(n > 0) {
        return false;
    }
    s_queens_strat(&queens2(n - 1, size), &mut |ql| {
        s_range(&QInt::At(size), &mut |xx| {
            let x = q2i(*xx);
            if !noattack(1, x, ql) {
                return false;
            }
            step();
            k(&cons(x, ql.clone()))
        })
    })
}

// ---- strategies ------------------------------------------------------------

// [] range => iterate*(dc(range_rule))
fn s_range(t: &QInt, k: K<QInt>) -> bool {
    iterate_star(t, k, s_range_1)
}
// dc(range_rule)
fn s_range_1(t: &QInt, k: K<QInt>) -> bool {
    dc(t, k, &[r_range_rule])
}

// [] queens_strat => dk(queens_0, queens_n)
fn s_queens_strat(t: &L, k: K<L>) -> bool {
    dk(t, k, &[r_queens_0, r_queens_n])
}

// [] det_queens_strat => first one(queens_0, queens_n)
fn s_det_queens_strat(t: &L, k: K<L>) -> bool {
    first_one(t, k, &[r_queens_0, r_queens_n])
}

// ---- main (from nqueens.lgi) ---------------------------------------------

fn main() {
    let n = arg_i64();
    run_big_stack(move || {
        let t = queens1(n);
        let mut out = String::new();
        s_queens_strat(&t, &mut |r| {
            use std::fmt::Write;
            let _ = writeln!(out, "result = {}", r);
            false
        });
        print!("{}", out);
        println!("rewrite_step = {}", steps());
    });
}
