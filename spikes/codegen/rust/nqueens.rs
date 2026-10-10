#![allow(dead_code)]
// "Generated" code for module nqueens (nqueens.eln, nqueens.lgi).
// Query: N ; start with (queens_strat) queens(query).

mod runtime;
use runtime::*;

// ---- symbol table -------------------------------------------------------
const NIL: u32 = FIRST_USER_SYM; //            nil : list
const CONS: u32 = FIRST_USER_SYM + 1; //       @.@ : (int list) list
const A: u32 = FIRST_USER_SYM + 2; //          a : int
const GENERATE_LIST: u32 = FIRST_USER_SYM + 3;
const QUEENS2: u32 = FIRST_USER_SYM + 4; //    queens(@,@)
const QUEENS1: u32 = FIRST_USER_SYM + 5; //    queens(@)
const NOATTACK: u32 = FIRST_USER_SYM + 6;
const Q2I: u32 = FIRST_USER_SYM + 7;
// (@ : (int) qint is an injection: no node)

static SYMS: [Sym; 11] = [
    Sym { name: "<int>", arity: 0, kind: Kind::Int },
    Sym { name: "true", arity: 0, kind: Kind::Prefix },
    Sym { name: "false", arity: 0, kind: Kind::Prefix },
    Sym { name: "nil", arity: 0, kind: Kind::Prefix },
    Sym { name: ".", arity: 2, kind: Kind::Infix(".") },
    Sym { name: "a", arity: 0, kind: Kind::Prefix },
    Sym { name: "generate_list", arity: 1, kind: Kind::Prefix },
    Sym { name: "queens", arity: 2, kind: Kind::Prefix },
    Sym { name: "queens", arity: 1, kind: Kind::Prefix },
    Sym { name: "noattack", arity: 3, kind: Kind::Prefix },
    Sym { name: "q2i", arity: 1, kind: Kind::Prefix },
];

// ---- constructors / defined functions (innermost normalisation) --------

// constructor @.@
fn cons(rt: &mut Rt, x: Term, l: Term) -> Term {
    rt.mk2(CONS, x, l)
}

// q2i(@) : rules for int
fn q2i(rt: &mut Rt, v0: Term) -> Term {
    // [] q2i(n) => n
    {
        let n = v0;
        rt.steps += 1;
        return n;
    }
}

// generate_list(@) : rules for list
fn generate_list(rt: &mut Rt, v0: Term) -> Term {
    // [] generate_list(0) => nil
    let c0 = rt.int(0);
    if v0 == c0 {
        rt.steps += 1;
        return rt.c(NIL);
    }
    // [] generate_list(n) => n . generate_list(n-1)
    {
        let n = v0;
        rt.steps += 1;
        let c1 = rt.int(1);
        let t0 = rt.sub(n, c1);
        let t1 = generate_list(rt, t0);
        return cons(rt, n, t1);
    }
}

// queens(@,@): no unlabelled rule, builds the term
fn queens2(rt: &mut Rt, v0: Term, v1: Term) -> Term {
    rt.mk2(QUEENS2, v0, v1)
}

// queens(@) : rules for list
fn queens1(rt: &mut Rt, v0: Term) -> Term {
    // [] queens(n) => queens(n,n)
    {
        let n = v0;
        rt.steps += 1;
        return queens2(rt, n, n);
    }
}

// noattack(@,@,@) : rules for bool
fn noattack(rt: &mut Rt, v0: Term, v1: Term, v2: Term) -> Term {
    // [] noattack(diff,d,nil) => true
    if v2 == rt.c(NIL) {
        rt.steps += 1;
        return rt.t_true;
    }
    // [] noattack(diff,d,p.l) => d!=p and d-p!=diff and p-d!=diff
    //                            and noattack(diff+1,d,l)
    if rt.sym(v2) == CONS {
        let diff = v0;
        let d = v1;
        let p = rt.arg(v2, 0);
        let l = rt.arg(v2, 1);
        rt.steps += 1;
        let t0 = rt.ne(d, p);
        let t1 = rt.sub(d, p);
        let t2 = rt.ne(t1, diff);
        let t3 = rt.and(t0, t2);
        let t4 = rt.sub(p, d);
        let t5 = rt.ne(t4, diff);
        let t6 = rt.and(t3, t5);
        let c1 = rt.int(1);
        let t7 = rt.add(diff, c1);
        let t8 = noattack(rt, t7, d, l);
        return rt.and(t6, t8);
    }
    rt.mk3(NOATTACK, v0, v1, v2)
}

// ---- labelled rules (CPS) ------------------------------------------------

// [range_rule] x => x-1 if x > 1          (rules for qint)
fn r_range_rule(rt: &mut Rt, t: Term, k: K) -> bool {
    let x = t;
    let c1 = rt.int(1);
    if rt.gt(x, c1) != rt.t_true {
        return false;
    }
    rt.steps += 1;
    let r = rt.sub(x, c1);
    k(rt, r)
}

// [queens_0] queens(0,size) => nil
fn r_queens_0(rt: &mut Rt, t: Term, k: K) -> bool {
    if rt.sym(t) != QUEENS2 {
        return false;
    }
    let c0 = rt.int(0);
    if rt.arg(t, 0) != c0 {
        return false;
    }
    rt.steps += 1;
    let r = rt.c(NIL);
    k(rt, r)
}

// [queens_n] queens(n,size) => x . ql
//    if n>0
//    where ql:=(queens_strat) queens(n-1,size)
//    where xx:=(range) size
//    where x:=()q2i(xx)
//    if noattack(1,x,ql)
fn r_queens_n(rt: &mut Rt, t: Term, k: K) -> bool {
    if rt.sym(t) != QUEENS2 {
        return false;
    }
    let n = rt.arg(t, 0);
    let size = rt.arg(t, 1);
    let c0 = rt.int(0);
    if rt.gt(n, c0) != rt.t_true {
        return false;
    }
    let c1 = rt.int(1);
    let t0 = rt.sub(n, c1);
    let u0 = queens2(rt, t0, size);
    s_queens_strat(rt, u0, &mut |rt, ql| {
        s_range(rt, size, &mut |rt, xx| {
            let x = q2i(rt, xx);
            let c1 = rt.int(1);
            if noattack(rt, c1, x, ql) != rt.t_true {
                return false;
            }
            rt.steps += 1;
            let r = cons(rt, x, ql);
            k(rt, r)
        })
    })
}

// ---- strategies ------------------------------------------------------------

// [] range => iterate*(dc(range_rule))
fn s_range(rt: &mut Rt, t: Term, k: K) -> bool {
    iterate_star(rt, t, k, s_range_1)
}
// dc(range_rule)
fn s_range_1(rt: &mut Rt, t: Term, k: K) -> bool {
    dc(rt, t, k, &[r_range_rule])
}

// [] queens_strat => dk(queens_0, queens_n)
fn s_queens_strat(rt: &mut Rt, t: Term, k: K) -> bool {
    dk(rt, t, k, &[r_queens_0, r_queens_n])
}

// [] det_queens_strat => first one(queens_0, queens_n)
fn s_det_queens_strat(rt: &mut Rt, t: Term, k: K) -> bool {
    first_one(rt, t, k, &[r_queens_0, r_queens_n])
}

// ---- main (from nqueens.lgi) ---------------------------------------------

fn main() {
    let n = arg_i64();
    run_big_stack(move || {
        let mut rt = Rt::new(&SYMS);
        let _ = (generate_list as fn(&mut Rt, Term) -> Term, s_det_queens_strat as Strat, A);
        let q = rt.int(n);
        let t = queens1(&mut rt, q);
        let mut out = String::new();
        s_queens_strat(&mut rt, t, &mut |rt, r| {
            out.push_str("result = ");
            out.push_str(&rt.show(r));
            out.push('\n');
            false
        });
        print!("{}", out);
        println!("rewrite_step = {}", rt.steps);
    });
}
