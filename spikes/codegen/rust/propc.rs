#![allow(dead_code)]
// "Generated" code for module propc (propc.eln, propc{1,2,3}.lgi).
// Query: K in {1,2,3} ; start with () qK.

mod runtime;
use runtime::*;

// ---- symbol table -------------------------------------------------------
const T: u32 = FIRST_USER_SYM; //          t : Prop
const F: u32 = FIRST_USER_SYM + 1; //      f : Prop
const AND: u32 = FIRST_USER_SYM + 2; //    and(@,@) (AC)
const XOR: u32 = FIRST_USER_SYM + 3; //    xor(@,@) (AC)
const OR: u32 = FIRST_USER_SYM + 4;
const IFF: u32 = FIRST_USER_SYM + 5;
const NOT: u32 = FIRST_USER_SYM + 6;
const IMPLIES: u32 = FIRST_USER_SYM + 7;
const A1: u32 = FIRST_USER_SYM + 8; //     a1 .. a18 : Prop
const A2: u32 = A1 + 1;
const A3: u32 = A1 + 2;
const A4: u32 = A1 + 3;
const A5: u32 = A1 + 4;
const A6: u32 = A1 + 5;
const A7: u32 = A1 + 6;
const A8: u32 = A1 + 7;
const A9: u32 = A1 + 8;
const A10: u32 = A1 + 9;
const A11: u32 = A1 + 10;
const A12: u32 = A1 + 11;
const A13: u32 = A1 + 12;
const A14: u32 = A1 + 13;
const A15: u32 = A1 + 14;
const A16: u32 = A1 + 15;
const A17: u32 = A1 + 16;
const A18: u32 = A1 + 17;
const Q1: u32 = A1 + 18;
const Q2: u32 = A1 + 19;
const Q3: u32 = A1 + 20;

macro_rules! cst {
    ($n:expr) => {
        Sym { name: $n, arity: 0, kind: Kind::Prefix }
    };
}
static SYMS: [Sym; 32] = [
    Sym { name: "<int>", arity: 0, kind: Kind::Int },
    cst!("true"),
    cst!("false"),
    cst!("t"),
    cst!("f"),
    Sym { name: "and", arity: 2, kind: Kind::AC(None) },
    Sym { name: "xor", arity: 2, kind: Kind::AC(None) },
    Sym { name: "or", arity: 2, kind: Kind::Prefix },
    Sym { name: "iff", arity: 2, kind: Kind::Prefix },
    Sym { name: "not", arity: 1, kind: Kind::Prefix },
    Sym { name: "implies", arity: 2, kind: Kind::Prefix },
    cst!("a1"), cst!("a2"), cst!("a3"), cst!("a4"), cst!("a5"), cst!("a6"),
    cst!("a7"), cst!("a8"), cst!("a9"), cst!("a10"), cst!("a11"), cst!("a12"),
    cst!("a13"), cst!("a14"), cst!("a15"), cst!("a16"), cst!("a17"), cst!("a18"),
    cst!("q1"), cst!("q2"), cst!("q3"),
];

// ---- AC symbol and: flatten + sort, then its rules -----------------------

fn and(rt: &mut Rt, x: Term, y: Term) -> Term {
    let t = rt.ac2(AND, x, y);
    and_rules(rt, t)
}

// re-normalise a variable bound to an AC rest (it may be an and-term)
fn and_renorm(rt: &mut Rt, x: Term) -> Term {
    if rt.sym(x) == AND {
        and_rules(rt, x)
    } else {
        x
    }
}

fn and_rules(rt: &mut Rt, v0: Term) -> Term {
    let len = rt.arity(v0);
    // [] and(x, x) => x        compiled with extension: and(x,x,S$) => and(x,S$)
    for i0 in 0..len {
        if !rt.ac_first_occ(v0, i0, &[]) || rt.ac_mult(v0, i0) < 2 {
            continue;
        }
        let x = rt.arg(v0, i0);
        rt.steps += 1;
        return match rt.ac_rest(AND, v0, &[i0, i0 + 1]) {
            None => x,
            Some(s) => and(rt, x, s),
        };
    }
    // [] and(x, t) => x
    if let Some(i0) = rt.ac_find(v0, rt.c(T)) {
        let x = rt.ac_rest(AND, v0, &[i0]).unwrap();
        rt.steps += 1;
        return and_renorm(rt, x);
    }
    // [] and(x, f) => f
    if rt.ac_find(v0, rt.c(F)).is_some() {
        rt.steps += 1;
        return rt.c(F);
    }
    // [] and(x, xor(y, z)) => xor(and(x, y), and(x, z))
    for i0 in 0..len {
        if !rt.ac_first_occ(v0, i0, &[]) {
            continue;
        }
        let e0 = rt.arg(v0, i0);
        if rt.sym(e0) != XOR {
            continue;
        }
        let x = rt.ac_rest(AND, v0, &[i0]).unwrap();
        // xor(y, z) against the xor-term e0: first split y = one element
        let y = rt.arg(e0, 0);
        let z = rt.ac_rest(XOR, e0, &[0]).unwrap();
        rt.steps += 1;
        let t0 = and(rt, x, y);
        let t1 = and(rt, x, z);
        return xor(rt, t0, t1);
    }
    v0
}

// ---- AC symbol xor ----------------------------------------------------------

fn xor(rt: &mut Rt, x: Term, y: Term) -> Term {
    let t = rt.ac2(XOR, x, y);
    xor_rules(rt, t)
}

fn xor_renorm(rt: &mut Rt, x: Term) -> Term {
    if rt.sym(x) == XOR {
        xor_rules(rt, x)
    } else {
        x
    }
}

fn xor_rules(rt: &mut Rt, v0: Term) -> Term {
    let len = rt.arity(v0);
    // [] xor(x, x) => f        compiled with extension: xor(x,x,S$) => xor(S$,f)
    for i0 in 0..len {
        if !rt.ac_first_occ(v0, i0, &[]) || rt.ac_mult(v0, i0) < 2 {
            continue;
        }
        rt.steps += 1;
        return match rt.ac_rest(XOR, v0, &[i0, i0 + 1]) {
            None => rt.c(F),
            Some(s) => {
                let c_f = rt.c(F);
                xor(rt, s, c_f)
            }
        };
    }
    // [] xor(x, f) => x
    if let Some(i0) = rt.ac_find(v0, rt.c(F)) {
        let x = rt.ac_rest(XOR, v0, &[i0]).unwrap();
        rt.steps += 1;
        return xor_renorm(rt, x);
    }
    v0
}

// ---- free defined symbols ---------------------------------------------------

// [] not(x) => xor(x, t)
fn not(rt: &mut Rt, x: Term) -> Term {
    rt.steps += 1;
    let c_t = rt.c(T);
    xor(rt, x, c_t)
}

// [] implies(x, y) => not(xor(x, and(x, y)))
fn implies(rt: &mut Rt, x: Term, y: Term) -> Term {
    rt.steps += 1;
    let t0 = and(rt, x, y);
    let t1 = xor(rt, x, t0);
    not(rt, t1)
}

// [] or(x, y) => xor(and(x, y), xor(x, y))
fn or(rt: &mut Rt, x: Term, y: Term) -> Term {
    rt.steps += 1;
    let t0 = and(rt, x, y);
    let t1 = xor(rt, x, y);
    xor(rt, t0, t1)
}

// [] iff(x, y) => not(xor(x, y))
fn iff(rt: &mut Rt, x: Term, y: Term) -> Term {
    rt.steps += 1;
    let t0 = xor(rt, x, y);
    not(rt, t0)
}

// ---- constants with rules (rhs in A-normal form, emitted by tools/anf_q.py)

// [] q1 => <rhs, 103 subterms>
fn q1(rt: &mut Rt) -> Term {
    rt.steps += 1;
    let c_a1 = rt.c(A1);
    let c_a2 = rt.c(A2);
    let c_a3 = rt.c(A3);
    let c_a4 = rt.c(A4);
    let c_a5 = rt.c(A5);
    let c_a6 = rt.c(A6);
    let c_a7 = rt.c(A7);
    let c_a8 = rt.c(A8);
    let c_a9 = rt.c(A9);
    let c_a10 = rt.c(A10);
    let c_a11 = rt.c(A11);
    let t0 = or(rt, c_a1, c_a2);
    let t1 = not(rt, c_a3);
    let t2 = xor(rt, c_a4, c_a5);
    let t3 = not(rt, c_a6);
    let t4 = not(rt, t3);
    let t5 = not(rt, t4);
    let t6 = iff(rt, t2, t5);
    let t7 = or(rt, t1, t6);
    let t8 = iff(rt, t0, t7);
    let t9 = and(rt, c_a7, c_a8);
    let t10 = and(rt, c_a10, c_a11);
    let t11 = or(rt, c_a9, t10);
    let t12 = xor(rt, t11, c_a2);
    let t13 = iff(rt, c_a5, c_a5);
    let t14 = xor(rt, c_a2, t13);
    let t15 = and(rt, c_a11, t14);
    let t16 = xor(rt, c_a7, c_a7);
    let t17 = iff(rt, c_a9, c_a4);
    let t18 = xor(rt, t16, t17);
    let t19 = and(rt, t15, t18);
    let t20 = xor(rt, t12, t19);
    let t21 = not(rt, t20);
    let t22 = and(rt, t9, t21);
    let t23 = not(rt, t22);
    let t24 = iff(rt, t8, t23);
    let t25 = or(rt, c_a1, c_a2);
    let t26 = not(rt, c_a3);
    let t27 = xor(rt, c_a4, c_a5);
    let t28 = not(rt, c_a6);
    let t29 = not(rt, t28);
    let t30 = not(rt, t29);
    let t31 = iff(rt, t27, t30);
    let t32 = or(rt, t26, t31);
    let t33 = iff(rt, t25, t32);
    let t34 = and(rt, c_a7, c_a8);
    let t35 = and(rt, c_a10, c_a11);
    let t36 = or(rt, c_a9, t35);
    let t37 = xor(rt, t36, c_a2);
    let t38 = iff(rt, c_a5, c_a5);
    let t39 = xor(rt, c_a2, t38);
    let t40 = and(rt, c_a11, t39);
    let t41 = xor(rt, c_a7, c_a7);
    let t42 = iff(rt, c_a9, c_a4);
    let t43 = xor(rt, t41, t42);
    let t44 = and(rt, t40, t43);
    let t45 = xor(rt, t37, t44);
    let t46 = not(rt, t45);
    let t47 = and(rt, t34, t46);
    let t48 = not(rt, t47);
    let t49 = iff(rt, t33, t48);
    let t50 = and(rt, c_a1, c_a2);
    let t51 = and(rt, c_a3, c_a4);
    let t52 = implies(rt, c_a5, c_a6);
    let t53 = implies(rt, t51, t52);
    let t54 = or(rt, c_a7, c_a8);
    let t55 = xor(rt, t53, t54);
    let t56 = iff(rt, c_a9, c_a10);
    let t57 = xor(rt, t56, c_a11);
    let t58 = or(rt, t55, t57);
    let t59 = xor(rt, c_a2, c_a2);
    let t60 = xor(rt, t59, c_a7);
    let t61 = or(rt, t58, t60);
    let t62 = or(rt, c_a4, c_a9);
    let t63 = not(rt, c_a6);
    let t64 = xor(rt, t63, c_a6);
    let t65 = iff(rt, t62, t64);
    let t66 = xor(rt, t61, t65);
    let t67 = not(rt, t66);
    let t68 = implies(rt, t50, t67);
    let t69 = not(rt, c_a11);
    let t70 = not(rt, c_a9);
    let t71 = iff(rt, t69, t70);
    let t72 = not(rt, t71);
    let t73 = and(rt, t68, t72);
    let t74 = not(rt, t73);
    let t75 = implies(rt, t49, t74);
    let t76 = and(rt, t24, t75);
    let t77 = and(rt, c_a1, c_a2);
    let t78 = and(rt, c_a3, c_a4);
    let t79 = implies(rt, c_a5, c_a6);
    let t80 = implies(rt, t78, t79);
    let t81 = or(rt, c_a7, c_a8);
    let t82 = xor(rt, t80, t81);
    let t83 = iff(rt, c_a9, c_a10);
    let t84 = xor(rt, t83, c_a11);
    let t85 = or(rt, t82, t84);
    let t86 = xor(rt, c_a2, c_a2);
    let t87 = xor(rt, t86, c_a7);
    let t88 = or(rt, t85, t87);
    let t89 = or(rt, c_a4, c_a9);
    let t90 = not(rt, c_a6);
    let t91 = xor(rt, t90, c_a6);
    let t92 = iff(rt, t89, t91);
    let t93 = xor(rt, t88, t92);
    let t94 = not(rt, t93);
    let t95 = implies(rt, t77, t94);
    let t96 = not(rt, c_a11);
    let t97 = not(rt, c_a9);
    let t98 = iff(rt, t96, t97);
    let t99 = not(rt, t98);
    let t100 = and(rt, t95, t99);
    let t101 = not(rt, t100);
    let t102 = implies(rt, t76, t101);
    t102
}

// [] q2 => <rhs, 103 subterms>
fn q2(rt: &mut Rt) -> Term {
    rt.steps += 1;
    let c_a1 = rt.c(A1);
    let c_a2 = rt.c(A2);
    let c_a3 = rt.c(A3);
    let c_a4 = rt.c(A4);
    let c_a5 = rt.c(A5);
    let c_a6 = rt.c(A6);
    let c_a7 = rt.c(A7);
    let c_a8 = rt.c(A8);
    let c_a9 = rt.c(A9);
    let c_a10 = rt.c(A10);
    let t0 = or(rt, c_a2, c_a3);
    let t1 = xor(rt, t0, c_a4);
    let t2 = xor(rt, c_a1, t1);
    let t3 = not(rt, c_a5);
    let t4 = iff(rt, c_a6, c_a7);
    let t5 = iff(rt, c_a8, c_a9);
    let t6 = xor(rt, t4, t5);
    let t7 = and(rt, c_a10, c_a9);
    let t8 = or(rt, t6, t7);
    let t9 = xor(rt, t3, t8);
    let t10 = not(rt, c_a2);
    let t11 = not(rt, t10);
    let t12 = or(rt, c_a9, c_a6);
    let t13 = or(rt, c_a10, c_a5);
    let t14 = implies(rt, t12, t13);
    let t15 = iff(rt, t11, t14);
    let t16 = iff(rt, t9, t15);
    let t17 = not(rt, c_a8);
    let t18 = or(rt, c_a4, c_a9);
    let t19 = implies(rt, t17, t18);
    let t20 = or(rt, c_a9, t19);
    let t21 = not(rt, t20);
    let t22 = xor(rt, t16, t21);
    let t23 = and(rt, t2, t22);
    let t24 = not(rt, t23);
    let t25 = or(rt, c_a2, c_a3);
    let t26 = xor(rt, t25, c_a4);
    let t27 = xor(rt, c_a1, t26);
    let t28 = not(rt, c_a5);
    let t29 = iff(rt, c_a6, c_a7);
    let t30 = iff(rt, c_a8, c_a9);
    let t31 = xor(rt, t29, t30);
    let t32 = and(rt, c_a10, c_a9);
    let t33 = or(rt, t31, t32);
    let t34 = xor(rt, t28, t33);
    let t35 = not(rt, c_a2);
    let t36 = not(rt, t35);
    let t37 = or(rt, c_a9, c_a6);
    let t38 = or(rt, c_a10, c_a5);
    let t39 = implies(rt, t37, t38);
    let t40 = iff(rt, t36, t39);
    let t41 = iff(rt, t34, t40);
    let t42 = not(rt, c_a8);
    let t43 = or(rt, c_a4, c_a9);
    let t44 = implies(rt, t42, t43);
    let t45 = or(rt, c_a9, t44);
    let t46 = not(rt, t45);
    let t47 = xor(rt, t41, t46);
    let t48 = and(rt, t27, t47);
    let t49 = not(rt, t48);
    let t50 = xor(rt, c_a2, c_a3);
    let t51 = not(rt, c_a4);
    let t52 = xor(rt, t50, t51);
    let t53 = or(rt, c_a1, t52);
    let t54 = and(rt, c_a6, c_a7);
    let t55 = xor(rt, c_a5, t54);
    let t56 = not(rt, t55);
    let t57 = and(rt, t53, t56);
    let t58 = implies(rt, c_a8, c_a9);
    let t59 = xor(rt, t58, c_a10);
    let t60 = or(rt, c_a4, c_a1);
    let t61 = and(rt, c_a4, t60);
    let t62 = xor(rt, t61, c_a2);
    let t63 = implies(rt, t59, t62);
    let t64 = implies(rt, t57, t63);
    let t65 = or(rt, c_a4, c_a7);
    let t66 = xor(rt, t65, c_a2);
    let t67 = and(rt, c_a8, c_a1);
    let t68 = or(rt, t66, t67);
    let t69 = not(rt, c_a6);
    let t70 = not(rt, t69);
    let t71 = not(rt, t70);
    let t72 = or(rt, t68, t71);
    let t73 = implies(rt, t64, t72);
    let t74 = not(rt, t73);
    let t75 = implies(rt, t49, t74);
    let t76 = and(rt, t24, t75);
    let t77 = xor(rt, c_a2, c_a3);
    let t78 = not(rt, c_a4);
    let t79 = xor(rt, t77, t78);
    let t80 = or(rt, c_a1, t79);
    let t81 = and(rt, c_a6, c_a7);
    let t82 = xor(rt, c_a5, t81);
    let t83 = not(rt, t82);
    let t84 = and(rt, t80, t83);
    let t85 = implies(rt, c_a8, c_a9);
    let t86 = xor(rt, t85, c_a10);
    let t87 = or(rt, c_a4, c_a1);
    let t88 = and(rt, c_a4, t87);
    let t89 = xor(rt, t88, c_a2);
    let t90 = implies(rt, t86, t89);
    let t91 = implies(rt, t84, t90);
    let t92 = or(rt, c_a4, c_a7);
    let t93 = xor(rt, t92, c_a2);
    let t94 = and(rt, c_a8, c_a1);
    let t95 = or(rt, t93, t94);
    let t96 = not(rt, c_a6);
    let t97 = not(rt, t96);
    let t98 = not(rt, t97);
    let t99 = or(rt, t95, t98);
    let t100 = implies(rt, t91, t99);
    let t101 = not(rt, t100);
    let t102 = implies(rt, t76, t101);
    t102
}

// [] q3 => <rhs, 103 subterms>
fn q3(rt: &mut Rt) -> Term {
    rt.steps += 1;
    let c_a1 = rt.c(A1);
    let c_a2 = rt.c(A2);
    let c_a3 = rt.c(A3);
    let c_a4 = rt.c(A4);
    let c_a5 = rt.c(A5);
    let c_a6 = rt.c(A6);
    let c_a7 = rt.c(A7);
    let c_a8 = rt.c(A8);
    let c_a9 = rt.c(A9);
    let c_a10 = rt.c(A10);
    let c_a11 = rt.c(A11);
    let t0 = or(rt, c_a2, c_a3);
    let t1 = xor(rt, t0, c_a4);
    let t2 = xor(rt, c_a1, t1);
    let t3 = not(rt, c_a5);
    let t4 = iff(rt, c_a6, c_a7);
    let t5 = iff(rt, c_a8, c_a9);
    let t6 = xor(rt, t4, t5);
    let t7 = and(rt, c_a10, c_a11);
    let t8 = or(rt, t6, t7);
    let t9 = xor(rt, t3, t8);
    let t10 = iff(rt, c_a1, c_a2);
    let t11 = and(rt, c_a3, t10);
    let t12 = or(rt, c_a4, t11);
    let t13 = not(rt, c_a4);
    let t14 = not(rt, t13);
    let t15 = implies(rt, t12, t14);
    let t16 = iff(rt, t9, t15);
    let t17 = implies(rt, c_a6, c_a1);
    let t18 = not(rt, c_a1);
    let t19 = implies(rt, t17, t18);
    let t20 = not(rt, c_a9);
    let t21 = xor(rt, t19, t20);
    let t22 = xor(rt, t16, t21);
    let t23 = and(rt, t2, t22);
    let t24 = not(rt, t23);
    let t25 = or(rt, c_a2, c_a3);
    let t26 = xor(rt, t25, c_a4);
    let t27 = xor(rt, c_a1, t26);
    let t28 = not(rt, c_a5);
    let t29 = iff(rt, c_a6, c_a7);
    let t30 = iff(rt, c_a8, c_a9);
    let t31 = xor(rt, t29, t30);
    let t32 = and(rt, c_a10, c_a11);
    let t33 = or(rt, t31, t32);
    let t34 = xor(rt, t28, t33);
    let t35 = iff(rt, c_a1, c_a2);
    let t36 = and(rt, c_a3, t35);
    let t37 = or(rt, c_a4, t36);
    let t38 = not(rt, c_a4);
    let t39 = not(rt, t38);
    let t40 = implies(rt, t37, t39);
    let t41 = iff(rt, t34, t40);
    let t42 = implies(rt, c_a6, c_a1);
    let t43 = not(rt, c_a1);
    let t44 = implies(rt, t42, t43);
    let t45 = not(rt, c_a9);
    let t46 = xor(rt, t44, t45);
    let t47 = xor(rt, t41, t46);
    let t48 = and(rt, t27, t47);
    let t49 = not(rt, t48);
    let t50 = xor(rt, c_a2, c_a3);
    let t51 = not(rt, c_a4);
    let t52 = xor(rt, t50, t51);
    let t53 = or(rt, c_a1, t52);
    let t54 = and(rt, c_a6, c_a7);
    let t55 = xor(rt, c_a5, t54);
    let t56 = not(rt, t55);
    let t57 = and(rt, t53, t56);
    let t58 = implies(rt, c_a8, c_a9);
    let t59 = xor(rt, t58, c_a10);
    let t60 = implies(rt, c_a2, c_a8);
    let t61 = and(rt, c_a11, t60);
    let t62 = xor(rt, t61, c_a8);
    let t63 = implies(rt, t59, t62);
    let t64 = implies(rt, t57, t63);
    let t65 = and(rt, c_a8, c_a9);
    let t66 = or(rt, c_a8, t65);
    let t67 = or(rt, c_a5, t66);
    let t68 = not(rt, c_a2);
    let t69 = implies(rt, t67, t68);
    let t70 = not(rt, c_a7);
    let t71 = or(rt, t69, t70);
    let t72 = not(rt, t71);
    let t73 = implies(rt, t64, t72);
    let t74 = not(rt, t73);
    let t75 = implies(rt, t49, t74);
    let t76 = and(rt, t24, t75);
    let t77 = xor(rt, c_a2, c_a3);
    let t78 = not(rt, c_a4);
    let t79 = xor(rt, t77, t78);
    let t80 = or(rt, c_a1, t79);
    let t81 = and(rt, c_a6, c_a7);
    let t82 = xor(rt, c_a5, t81);
    let t83 = not(rt, t82);
    let t84 = and(rt, t80, t83);
    let t85 = implies(rt, c_a8, c_a9);
    let t86 = xor(rt, t85, c_a10);
    let t87 = implies(rt, c_a2, c_a8);
    let t88 = and(rt, c_a11, t87);
    let t89 = xor(rt, t88, c_a8);
    let t90 = implies(rt, t86, t89);
    let t91 = implies(rt, t84, t90);
    let t92 = and(rt, c_a8, c_a9);
    let t93 = or(rt, c_a8, t92);
    let t94 = or(rt, c_a5, t93);
    let t95 = not(rt, c_a2);
    let t96 = implies(rt, t94, t95);
    let t97 = not(rt, c_a7);
    let t98 = or(rt, t96, t97);
    let t99 = not(rt, t98);
    let t100 = implies(rt, t91, t99);
    let t101 = not(rt, t100);
    let t102 = implies(rt, t76, t101);
    t102
}

// ---- main (from propc{1,2,3}.lgi) -------------------------------------------

fn main() {
    let q = arg_i64();
    run_big_stack(move || {
        let mut rt = Rt::new(&SYMS);
        let r = match q {
            1 => q1(&mut rt),
            2 => q2(&mut rt),
            3 => q3(&mut rt),
            _ => {
                eprintln!("propc: query must be 1, 2 or 3");
                std::process::exit(2)
            }
        };
        println!("result = {}", rt.show(r));
        println!("rewrite_step = {}", rt.steps);
    });
}
