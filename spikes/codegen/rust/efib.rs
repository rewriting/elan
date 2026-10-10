#![allow(dead_code)]
// "Generated" code for module efib (efib.eln, efib.lgi).
// Query: go(N) ; start with () query.

mod runtime;
use runtime::*;

// ---- symbol table -------------------------------------------------------
const U: u32 = FIRST_USER_SYM; //              @ U @ : (Space Space) Space (AC)
const EMPTY: u32 = FIRST_USER_SYM + 1; //      empty : Space
const FIB: u32 = FIRST_USER_SYM + 2; //        Fib[farg=@,val=@]
const COMPUTE: u32 = FIRST_USER_SYM + 3; //    Compute[question=@,answer=@]
const UNDEF: u32 = FIRST_USER_SYM + 4; //      UNDEF : eInt
const OCCURSFIB: u32 = FIRST_USER_SYM + 5;
const GO: u32 = FIRST_USER_SYM + 6;
const RESULT: u32 = FIRST_USER_SYM + 7;
// (@ : (builtinInt) eInt, @ : (Object) Space, @ : (Fib) Object,
//  @ : (Compute) Object are injections: no node)

static SYMS: [Sym; 11] = [
    Sym { name: "<int>", arity: 0, kind: Kind::Int },
    Sym { name: "true", arity: 0, kind: Kind::Prefix },
    Sym { name: "false", arity: 0, kind: Kind::Prefix },
    Sym { name: "U", arity: 2, kind: Kind::AC(Some(" U ")) },
    Sym { name: "empty", arity: 0, kind: Kind::Prefix },
    Sym { name: "Fib", arity: 2, kind: Kind::Record(&["farg", "val"]) },
    Sym { name: "Compute", arity: 2, kind: Kind::Record(&["question", "answer"]) },
    Sym { name: "UNDEF", arity: 0, kind: Kind::Prefix },
    Sym { name: "occursFib", arity: 2, kind: Kind::Prefix },
    Sym { name: "go", arity: 1, kind: Kind::Prefix },
    Sym { name: "result", arity: 2, kind: Kind::Prefix },
];

// ---- constructors / defined functions -----------------------------------

// AC constructor @ U @ (no rules): flatten + sort
fn u(rt: &mut Rt, a: Term, b: Term) -> Term {
    rt.ac2(U, a, b)
}

// constructor Fib[farg=@,val=@]
fn fib(rt: &mut Rt, n: Term, v: Term) -> Term {
    rt.mk2(FIB, n, v)
}

// occursFib(@,@) : rules for bool
fn occurs_fib(rt: &mut Rt, v0: Term, v1: Term) -> Term {
    // [] occursFib(S U Fib[farg=n,val=v],n) => true
    if rt.sym(v0) == U {
        let len = rt.arity(v0);
        for i0 in 0..len {
            if !rt.ac_first_occ(v0, i0, &[]) {
                continue;
            }
            let e0 = rt.arg(v0, i0);
            if rt.sym(e0) != FIB {
                continue;
            }
            let n = rt.arg(e0, 0);
            if n != v1 {
                continue; // non-linear n
            }
            // S := rest, non-empty since len >= 2; unused in the rhs
            rt.steps += 1;
            return rt.t_true;
        }
    }
    // [] occursFib(S,n) => false
    rt.steps += 1;
    rt.t_false
}

// result(@,@) : rules for eInt
fn result(rt: &mut Rt, v0: Term, v1: Term) -> Term {
    // [] result(S U Fib[farg=n,val=v],n) => v
    if rt.sym(v0) == U {
        let len = rt.arity(v0);
        for i0 in 0..len {
            if !rt.ac_first_occ(v0, i0, &[]) {
                continue;
            }
            let e0 = rt.arg(v0, i0);
            if rt.sym(e0) != FIB {
                continue;
            }
            if rt.arg(e0, 0) != v1 {
                continue;
            }
            let v = rt.arg(e0, 1);
            rt.steps += 1;
            return v;
        }
    }
    rt.mk2(RESULT, v0, v1)
}

// go(@) : rules for eInt
fn go(rt: &mut Rt, v0: Term) -> Term {
    // [] go(n) => result(S,n)
    //    where S:=(loop) empty U Fib[farg=0,val=1] U Fib[farg=1,val=1]
    //                          U Fib[farg=n,val=UNDEF]
    let n = v0;
    let t0 = rt.c(EMPTY);
    let c0 = rt.int(0);
    let c1 = rt.int(1);
    let t1 = fib(rt, c0, c1);
    let t2 = u(rt, t0, t1);
    let t3 = fib(rt, c1, c1);
    let t4 = u(rt, t2, t3);
    let t5 = rt.c(UNDEF);
    let t6 = fib(rt, n, t5);
    let t7 = u(rt, t4, t6);
    // where in an unlabelled rule: first result of the strategy
    let mut res = None;
    s_loop(rt, t7, &mut |rt, s| {
        rt.steps += 1;
        res = Some(result(rt, s, n));
        true
    });
    match res {
        Some(r) => r,
        None => rt.mk1(GO, v0),
    }
}

// ---- labelled rules (CPS) ------------------------------------------------

// [rec1] S U Fib[farg=n,val=UNDEF]
//     => S U Fib[farg=n,val=UNDEF] U Fib[farg=n-1,val=UNDEF]
//        if n > 2  if not(occursFib(S,n-1))
fn r_rec1(rt: &mut Rt, t: Term, k: K) -> bool {
    if rt.sym(t) != U {
        return false;
    }
    let len = rt.arity(t);
    for i0 in 0..len {
        if !rt.ac_first_occ(t, i0, &[]) {
            continue;
        }
        let e0 = rt.arg(t, i0);
        if rt.sym(e0) != FIB || rt.arg(e0, 1) != rt.c(UNDEF) {
            continue;
        }
        let n = rt.arg(e0, 0);
        let c2 = rt.int(2);
        if rt.gt(n, c2) != rt.t_true {
            continue;
        }
        let s = rt.ac_rest(U, t, &[i0]).unwrap();
        let c1 = rt.int(1);
        let t0 = rt.sub(n, c1);
        let t1 = occurs_fib(rt, s, t0);
        if rt.not(t1) != rt.t_true {
            continue;
        }
        rt.steps += 1;
        let t2 = rt.c(UNDEF);
        let t3 = fib(rt, n, t2);
        let t4 = u(rt, s, t3);
        let t5 = rt.sub(n, c1);
        let t6 = fib(rt, t5, t2);
        let r = u(rt, t4, t6);
        if k(rt, r) {
            return true;
        }
    }
    false
}

// [rec2] S U Fib[farg=n,val=UNDEF]
//     => S U Fib[farg=n,val=UNDEF] U Fib[farg=n-2,val=UNDEF]
//        if n > 3  if not(occursFib(S,n-2))
fn r_rec2(rt: &mut Rt, t: Term, k: K) -> bool {
    if rt.sym(t) != U {
        return false;
    }
    let len = rt.arity(t);
    for i0 in 0..len {
        if !rt.ac_first_occ(t, i0, &[]) {
            continue;
        }
        let e0 = rt.arg(t, i0);
        if rt.sym(e0) != FIB || rt.arg(e0, 1) != rt.c(UNDEF) {
            continue;
        }
        let n = rt.arg(e0, 0);
        let c3 = rt.int(3);
        if rt.gt(n, c3) != rt.t_true {
            continue;
        }
        let s = rt.ac_rest(U, t, &[i0]).unwrap();
        let c2 = rt.int(2);
        let t0 = rt.sub(n, c2);
        let t1 = occurs_fib(rt, s, t0);
        if rt.not(t1) != rt.t_true {
            continue;
        }
        rt.steps += 1;
        let t2 = rt.c(UNDEF);
        let t3 = fib(rt, n, t2);
        let t4 = u(rt, s, t3);
        let t5 = rt.sub(n, c2);
        let t6 = fib(rt, t5, t2);
        let r = u(rt, t4, t6);
        if k(rt, r) {
            return true;
        }
    }
    false
}

// [compute] S U Fib[farg=n1,val=v1] U Fib[farg=n2,val=v2] U Fib[farg=n,val=UNDEF]
//        => S U Fib[farg=n1,val=v1] U Fib[farg=n2,val=v2]
//             U Fib[farg=n,val=v1+v2 % 1000000]
//           if n1 == n2 + 1  if n == n1 + 1
// (each condition is tested as soon as its variables are bound)
fn r_compute(rt: &mut Rt, t: Term, k: K) -> bool {
    if rt.sym(t) != U {
        return false;
    }
    let len = rt.arity(t);
    if len < 4 {
        return false; // three elements + non-empty S
    }
    for i0 in 0..len {
        if !rt.ac_first_occ(t, i0, &[]) {
            continue;
        }
        let e0 = rt.arg(t, i0);
        if rt.sym(e0) != FIB || rt.sym(rt.arg(e0, 1)) != INT {
            continue;
        }
        let n1 = rt.arg(e0, 0);
        let v1 = rt.arg(e0, 1);
        for i1 in 0..len {
            if !rt.ac_first_occ(t, i1, &[i0]) {
                continue;
            }
            let e1 = rt.arg(t, i1);
            if rt.sym(e1) != FIB || rt.sym(rt.arg(e1, 1)) != INT {
                continue;
            }
            let n2 = rt.arg(e1, 0);
            let v2 = rt.arg(e1, 1);
            let c1 = rt.int(1);
            let t0 = rt.add(n2, c1);
            if rt.eq(n1, t0) != rt.t_true {
                continue;
            }
            for i2 in 0..len {
                if !rt.ac_first_occ(t, i2, &[i0, i1]) {
                    continue;
                }
                let e2 = rt.arg(t, i2);
                if rt.sym(e2) != FIB || rt.arg(e2, 1) != rt.c(UNDEF) {
                    continue;
                }
                let n = rt.arg(e2, 0);
                let t1 = rt.add(n1, c1);
                if rt.eq(n, t1) != rt.t_true {
                    continue;
                }
                let s = rt.ac_rest(U, t, &[i0, i1, i2]).unwrap();
                rt.steps += 1;
                let t2 = fib(rt, n1, v1);
                let t3 = u(rt, s, t2);
                let t4 = fib(rt, n2, v2);
                let t5 = u(rt, t3, t4);
                let c1000000 = rt.int(1000000);
                let t6 = rt.rem(v2, c1000000);
                let t7 = rt.add(v1, t6);
                let t8 = fib(rt, n, t7);
                let r = u(rt, t5, t8);
                if k(rt, r) {
                    return true;
                }
            }
        }
    }
    false
}

// ---- strategies ------------------------------------------------------------

// [] loop => repeat*(first one(rec1, rec2, compute))
fn s_loop(rt: &mut Rt, t: Term, k: K) -> bool {
    repeat_star_det(rt, t, k, s_loop_1)
}
// first one(rec1, rec2, compute)
fn s_loop_1(rt: &mut Rt, t: Term, k: K) -> bool {
    first_one(rt, t, k, &[r_rec1, r_rec2, r_compute])
}

// ---- main (from efib.lgi) --------------------------------------------------

fn main() {
    let n = arg_i64();
    run_big_stack(move || {
        let mut rt = Rt::new(&SYMS);
        let q = rt.int(n);
        let r = go(&mut rt, q);
        println!("result = {}", rt.show(r));
        println!("rewrite_step = {}", rt.steps);
    });
}
