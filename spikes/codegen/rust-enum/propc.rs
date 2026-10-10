#![allow(dead_code)]
// "Generated" code for module propc (propc.eln, propc{1,2,3}.lgi), terms as
// typed enums. Query: K in {1,2,3} ; start with () qK.

#[macro_use]
mod runtime;
use runtime::*;
use std::fmt;

// ---- sort Prop ---------------------------------------------------------------
#[derive(PartialEq, Eq, Hash)]
pub enum Prop {
    T,                // t : Prop
    F,                // f : Prop
    And(Ms<Prop>),    // and(@,@) (AC): sorted multiset, >= 2 elements
    Xor(Ms<Prop>),    // xor(@,@) (AC)
    Or(P, P),         // or(@,@)
    Iff(P, P),        // iff(@,@)
    Not(P),           // not(@)
    Implies(P, P),    // implies(@,@)
    A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18,
    Q1, Q2, Q3,
}
pub type P = H<Prop>;

impl Sort for Prop {
    unique_table!(Prop);
    fn children_into(self, out: &mut Vec<P>) {
        match self {
            Prop::And(s) | Prop::Xor(s) => out.extend(s.into_vec()),
            Prop::Or(x, y) | Prop::Iff(x, y) | Prop::Implies(x, y) => {
                out.push(x);
                out.push(y)
            }
            Prop::Not(x) => out.push(x),
            _ => {}
        }
    }
}

fn show_args(f: &mut fmt::Formatter, name: &str, xs: &[P]) -> fmt::Result {
    write!(f, "{}(", name)?;
    for (i, x) in xs.iter().enumerate() {
        if i > 0 {
            write!(f, ",")?;
        }
        write!(f, "{}", x)?;
    }
    write!(f, ")")
}

impl fmt::Display for Prop {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        use Prop::*;
        let c = match self {
            And(s) => return show_args(f, "and", s),
            Xor(s) => return show_args(f, "xor", s),
            Or(x, y) => return write!(f, "or({},{})", x, y),
            Iff(x, y) => return write!(f, "iff({},{})", x, y),
            Not(x) => return write!(f, "not({})", x),
            Implies(x, y) => return write!(f, "implies({},{})", x, y),
            T => "t", F => "f",
            A1 => "a1", A2 => "a2", A3 => "a3", A4 => "a4", A5 => "a5", A6 => "a6",
            A7 => "a7", A8 => "a8", A9 => "a9", A10 => "a10", A11 => "a11", A12 => "a12",
            A13 => "a13", A14 => "a14", A15 => "a15", A16 => "a16", A17 => "a17", A18 => "a18",
            Q1 => "q1", Q2 => "q2", Q3 => "q3",
        };
        f.write_str(c)
    }
}

// ---- constants: built once, in declaration order (fixes their ids) ----------
struct Consts {
    t: P, f: P,
    a: [P; 18],
}
thread_local! {
    static C: Consts = {
        use Prop::*;
        let t = H::new(T);
        let f = H::new(F);
        let a = [A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15, A16, A17, A18]
            .map(H::new);
        Consts { t, f, a }
    };
}
fn c_t() -> P { C.with(|c| c.t.clone()) }
fn c_f() -> P { C.with(|c| c.f.clone()) }
fn c_a1() -> P { C.with(|c| c.a[0].clone()) }
fn c_a2() -> P { C.with(|c| c.a[1].clone()) }
fn c_a3() -> P { C.with(|c| c.a[2].clone()) }
fn c_a4() -> P { C.with(|c| c.a[3].clone()) }
fn c_a5() -> P { C.with(|c| c.a[4].clone()) }
fn c_a6() -> P { C.with(|c| c.a[5].clone()) }
fn c_a7() -> P { C.with(|c| c.a[6].clone()) }
fn c_a8() -> P { C.with(|c| c.a[7].clone()) }
fn c_a9() -> P { C.with(|c| c.a[8].clone()) }
fn c_a10() -> P { C.with(|c| c.a[9].clone()) }
fn c_a11() -> P { C.with(|c| c.a[10].clone()) }

// ---- AC symbol and: flatten + merge, then its rules ------------------------

/// elements of p seen as an argument of and (flattening)
fn and_elems(p: &P) -> &[P] {
    match &**p {
        Prop::And(s) => s,
        _ => std::slice::from_ref(p),
    }
}
fn xor_elems(p: &P) -> &[P] {
    match &**p {
        Prop::Xor(s) => s,
        _ => std::slice::from_ref(p),
    }
}
/// an AC rest as a term (not normalised)
fn and_rest(r: Rest<Prop>) -> Option<P> {
    match r {
        Rest::Empty => None,
        Rest::One(x) => Some(x),
        Rest::Many(s) => Some(H::new(Prop::And(s))),
    }
}
fn xor_rest(r: Rest<Prop>) -> Option<P> {
    match r {
        Rest::Empty => None,
        Rest::One(x) => Some(x),
        Rest::Many(s) => Some(H::new(Prop::Xor(s))),
    }
}

fn and(x: &P, y: &P) -> P {
    let t = H::new(Prop::And(ac_merge(and_elems(x), and_elems(y))));
    and_rules(&t).unwrap_or(t)
}

// re-normalise a variable bound to an AC rest (it may be an and-term)
fn and_renorm(x: P) -> P {
    match and_rules(&x) {
        Some(r) => r,
        None => x,
    }
}

/// The unlabelled rules of and on an and-term; None if none applies.
fn and_rules(v0: &P) -> Option<P> {
    let Prop::And(s) = &**v0 else { return None };
    // [] and(x, x) => x        compiled with extension: and(x,x,S$) => and(x,S$)
    for i0 in 0..s.len() {
        if !ac_first_occ(s, i0, &[]) || ac_mult(s, i0) < 2 {
            continue;
        }
        let x = &s[i0];
        step();
        return Some(match and_rest(ac_rest(s, &[i0, i0 + 1])) {
            None => x.clone(),
            Some(r) => and(x, &r),
        });
    }
    // [] and(x, t) => x
    if let Some(i0) = ac_find(s, &c_t()) {
        let x = and_rest(ac_rest(s, &[i0])).unwrap();
        step();
        return Some(and_renorm(x));
    }
    // [] and(x, f) => f
    if ac_find(s, &c_f()).is_some() {
        step();
        return Some(c_f());
    }
    // [] and(x, xor(y, z)) => xor(and(x, y), and(x, z))
    for i0 in 0..s.len() {
        if !ac_first_occ(s, i0, &[]) {
            continue;
        }
        let Prop::Xor(e0) = &*s[i0] else { continue };
        let x = and_rest(ac_rest(s, &[i0])).unwrap();
        // xor(y, z) against the xor-term: y = its first element, z = the rest
        let y = &e0[0];
        let z = xor_rest(ac_rest(e0, &[0])).unwrap();
        step();
        return Some(xor(&and(&x, y), &and(&x, &z)));
    }
    None
}

// ---- AC symbol xor ----------------------------------------------------------

fn xor(x: &P, y: &P) -> P {
    let t = H::new(Prop::Xor(ac_merge(xor_elems(x), xor_elems(y))));
    xor_rules(&t).unwrap_or(t)
}

fn xor_renorm(x: P) -> P {
    match xor_rules(&x) {
        Some(r) => r,
        None => x,
    }
}

fn xor_rules(v0: &P) -> Option<P> {
    let Prop::Xor(s) = &**v0 else { return None };
    // [] xor(x, x) => f        compiled with extension: xor(x,x,S$) => xor(S$,f)
    for i0 in 0..s.len() {
        if !ac_first_occ(s, i0, &[]) || ac_mult(s, i0) < 2 {
            continue;
        }
        step();
        return Some(match xor_rest(ac_rest(s, &[i0, i0 + 1])) {
            None => c_f(),
            Some(r) => xor(&r, &c_f()),
        });
    }
    // [] xor(x, f) => x
    if let Some(i0) = ac_find(s, &c_f()) {
        let x = xor_rest(ac_rest(s, &[i0])).unwrap();
        step();
        return Some(xor_renorm(x));
    }
    None
}

// ---- free defined symbols ---------------------------------------------------

// [] not(x) => xor(x, t)
fn not(x: &P) -> P {
    step();
    xor(x, &c_t())
}

// [] implies(x, y) => not(xor(x, and(x, y)))
fn implies(x: &P, y: &P) -> P {
    step();
    not(&xor(x, &and(x, y)))
}

// [] or(x, y) => xor(and(x, y), xor(x, y))
fn or(x: &P, y: &P) -> P {
    step();
    xor(&and(x, y), &xor(x, y))
}

// [] iff(x, y) => not(xor(x, y))
fn iff(x: &P, y: &P) -> P {
    step();
    not(&xor(x, y))
}

// ---- constants with rules (rhs emitted by tools/q_nested.py) ----------------

// [] q1 => <rhs, 103 subterms>
fn q1() -> P {
    step();
    implies(
        &and(
            &iff(
                &iff(
                    &or(&c_a1(), &c_a2()),
                    &or(
                        &not(&c_a3()),
                        &iff(&xor(&c_a4(), &c_a5()), &not(&not(&not(&c_a6()))))
                    )
                ),
                &not(
                    &and(
                        &and(&c_a7(), &c_a8()),
                        &not(
                            &xor(
                                &xor(&or(&c_a9(), &and(&c_a10(), &c_a11())), &c_a2()),
                                &and(
                                    &and(&c_a11(), &xor(&c_a2(), &iff(&c_a5(), &c_a5()))),
                                    &xor(&xor(&c_a7(), &c_a7()), &iff(&c_a9(), &c_a4()))
                                )
                            )
                        )
                    )
                )
            ),
            &implies(
                &iff(
                    &iff(
                        &or(&c_a1(), &c_a2()),
                        &or(
                            &not(&c_a3()),
                            &iff(&xor(&c_a4(), &c_a5()), &not(&not(&not(&c_a6()))))
                        )
                    ),
                    &not(
                        &and(
                            &and(&c_a7(), &c_a8()),
                            &not(
                                &xor(
                                    &xor(&or(&c_a9(), &and(&c_a10(), &c_a11())), &c_a2()),
                                    &and(
                                        &and(
                                            &c_a11(),
                                            &xor(&c_a2(), &iff(&c_a5(), &c_a5()))
                                        ),
                                        &xor(
                                            &xor(&c_a7(), &c_a7()),
                                            &iff(&c_a9(), &c_a4())
                                        )
                                    )
                                )
                            )
                        )
                    )
                ),
                &not(
                    &and(
                        &implies(
                            &and(&c_a1(), &c_a2()),
                            &not(
                                &xor(
                                    &or(
                                        &or(
                                            &xor(
                                                &implies(
                                                    &and(&c_a3(), &c_a4()),
                                                    &implies(&c_a5(), &c_a6())
                                                ),
                                                &or(&c_a7(), &c_a8())
                                            ),
                                            &xor(&iff(&c_a9(), &c_a10()), &c_a11())
                                        ),
                                        &xor(&xor(&c_a2(), &c_a2()), &c_a7())
                                    ),
                                    &iff(
                                        &or(&c_a4(), &c_a9()),
                                        &xor(&not(&c_a6()), &c_a6())
                                    )
                                )
                            )
                        ),
                        &not(&iff(&not(&c_a11()), &not(&c_a9())))
                    )
                )
            )
        ),
        &not(
            &and(
                &implies(
                    &and(&c_a1(), &c_a2()),
                    &not(
                        &xor(
                            &or(
                                &or(
                                    &xor(
                                        &implies(
                                            &and(&c_a3(), &c_a4()),
                                            &implies(&c_a5(), &c_a6())
                                        ),
                                        &or(&c_a7(), &c_a8())
                                    ),
                                    &xor(&iff(&c_a9(), &c_a10()), &c_a11())
                                ),
                                &xor(&xor(&c_a2(), &c_a2()), &c_a7())
                            ),
                            &iff(&or(&c_a4(), &c_a9()), &xor(&not(&c_a6()), &c_a6()))
                        )
                    )
                ),
                &not(&iff(&not(&c_a11()), &not(&c_a9())))
            )
        )
    )
}

// [] q2 => <rhs, 103 subterms>
fn q2() -> P {
    step();
    implies(
        &and(
            &not(
                &and(
                    &xor(&c_a1(), &xor(&or(&c_a2(), &c_a3()), &c_a4())),
                    &xor(
                        &iff(
                            &xor(
                                &not(&c_a5()),
                                &or(
                                    &xor(&iff(&c_a6(), &c_a7()), &iff(&c_a8(), &c_a9())),
                                    &and(&c_a10(), &c_a9())
                                )
                            ),
                            &iff(
                                &not(&not(&c_a2())),
                                &implies(&or(&c_a9(), &c_a6()), &or(&c_a10(), &c_a5()))
                            )
                        ),
                        &not(&or(&c_a9(), &implies(&not(&c_a8()), &or(&c_a4(), &c_a9()))))
                    )
                )
            ),
            &implies(
                &not(
                    &and(
                        &xor(&c_a1(), &xor(&or(&c_a2(), &c_a3()), &c_a4())),
                        &xor(
                            &iff(
                                &xor(
                                    &not(&c_a5()),
                                    &or(
                                        &xor(
                                            &iff(&c_a6(), &c_a7()),
                                            &iff(&c_a8(), &c_a9())
                                        ),
                                        &and(&c_a10(), &c_a9())
                                    )
                                ),
                                &iff(
                                    &not(&not(&c_a2())),
                                    &implies(&or(&c_a9(), &c_a6()), &or(&c_a10(), &c_a5()))
                                )
                            ),
                            &not(
                                &or(
                                    &c_a9(),
                                    &implies(&not(&c_a8()), &or(&c_a4(), &c_a9()))
                                )
                            )
                        )
                    )
                ),
                &not(
                    &implies(
                        &implies(
                            &and(
                                &or(&c_a1(), &xor(&xor(&c_a2(), &c_a3()), &not(&c_a4()))),
                                &not(&xor(&c_a5(), &and(&c_a6(), &c_a7())))
                            ),
                            &implies(
                                &xor(&implies(&c_a8(), &c_a9()), &c_a10()),
                                &xor(&and(&c_a4(), &or(&c_a4(), &c_a1())), &c_a2())
                            )
                        ),
                        &or(
                            &or(
                                &xor(&or(&c_a4(), &c_a7()), &c_a2()),
                                &and(&c_a8(), &c_a1())
                            ),
                            &not(&not(&not(&c_a6())))
                        )
                    )
                )
            )
        ),
        &not(
            &implies(
                &implies(
                    &and(
                        &or(&c_a1(), &xor(&xor(&c_a2(), &c_a3()), &not(&c_a4()))),
                        &not(&xor(&c_a5(), &and(&c_a6(), &c_a7())))
                    ),
                    &implies(
                        &xor(&implies(&c_a8(), &c_a9()), &c_a10()),
                        &xor(&and(&c_a4(), &or(&c_a4(), &c_a1())), &c_a2())
                    )
                ),
                &or(
                    &or(&xor(&or(&c_a4(), &c_a7()), &c_a2()), &and(&c_a8(), &c_a1())),
                    &not(&not(&not(&c_a6())))
                )
            )
        )
    )
}

// [] q3 => <rhs, 103 subterms>
fn q3() -> P {
    step();
    implies(
        &and(
            &not(
                &and(
                    &xor(&c_a1(), &xor(&or(&c_a2(), &c_a3()), &c_a4())),
                    &xor(
                        &iff(
                            &xor(
                                &not(&c_a5()),
                                &or(
                                    &xor(&iff(&c_a6(), &c_a7()), &iff(&c_a8(), &c_a9())),
                                    &and(&c_a10(), &c_a11())
                                )
                            ),
                            &implies(
                                &or(&c_a4(), &and(&c_a3(), &iff(&c_a1(), &c_a2()))),
                                &not(&not(&c_a4()))
                            )
                        ),
                        &xor(
                            &implies(&implies(&c_a6(), &c_a1()), &not(&c_a1())),
                            &not(&c_a9())
                        )
                    )
                )
            ),
            &implies(
                &not(
                    &and(
                        &xor(&c_a1(), &xor(&or(&c_a2(), &c_a3()), &c_a4())),
                        &xor(
                            &iff(
                                &xor(
                                    &not(&c_a5()),
                                    &or(
                                        &xor(
                                            &iff(&c_a6(), &c_a7()),
                                            &iff(&c_a8(), &c_a9())
                                        ),
                                        &and(&c_a10(), &c_a11())
                                    )
                                ),
                                &implies(
                                    &or(&c_a4(), &and(&c_a3(), &iff(&c_a1(), &c_a2()))),
                                    &not(&not(&c_a4()))
                                )
                            ),
                            &xor(
                                &implies(&implies(&c_a6(), &c_a1()), &not(&c_a1())),
                                &not(&c_a9())
                            )
                        )
                    )
                ),
                &not(
                    &implies(
                        &implies(
                            &and(
                                &or(&c_a1(), &xor(&xor(&c_a2(), &c_a3()), &not(&c_a4()))),
                                &not(&xor(&c_a5(), &and(&c_a6(), &c_a7())))
                            ),
                            &implies(
                                &xor(&implies(&c_a8(), &c_a9()), &c_a10()),
                                &xor(&and(&c_a11(), &implies(&c_a2(), &c_a8())), &c_a8())
                            )
                        ),
                        &not(
                            &or(
                                &implies(
                                    &or(&c_a5(), &or(&c_a8(), &and(&c_a8(), &c_a9()))),
                                    &not(&c_a2())
                                ),
                                &not(&c_a7())
                            )
                        )
                    )
                )
            )
        ),
        &not(
            &implies(
                &implies(
                    &and(
                        &or(&c_a1(), &xor(&xor(&c_a2(), &c_a3()), &not(&c_a4()))),
                        &not(&xor(&c_a5(), &and(&c_a6(), &c_a7())))
                    ),
                    &implies(
                        &xor(&implies(&c_a8(), &c_a9()), &c_a10()),
                        &xor(&and(&c_a11(), &implies(&c_a2(), &c_a8())), &c_a8())
                    )
                ),
                &not(
                    &or(
                        &implies(
                            &or(&c_a5(), &or(&c_a8(), &and(&c_a8(), &c_a9()))),
                            &not(&c_a2())
                        ),
                        &not(&c_a7())
                    )
                )
            )
        )
    )
}

// ---- main (from propc{1,2,3}.lgi) -------------------------------------------

fn main() {
    let q = arg_i64();
    run_big_stack(move || {
        C.with(|_| ()); // build the constants first, in declaration order
        let r = match q {
            1 => q1(),
            2 => q2(),
            3 => q3(),
            _ => {
                eprintln!("propc: query must be 1, 2 or 3");
                std::process::exit(2)
            }
        };
        println!("result = {}", r);
        println!("rewrite_step = {}", steps());
    });
}
