//! CPS strategy helpers: order of results and "stop" semantics, against
//! the interpreter (`elan -b`) on
//!
//! ```text
//! [a] n => n*10   if n < 100
//! [b] n => n*10+1 if n < 100
//! [c] n => n      if n > 100
//! ```
//! start term 1: iterate*(dk(a,b)) = 1 10 100 101 11 110 111;
//! repeat*(dk(a,b)) = repeat+(dk(a,b)) = dk(a,b);dk(a,b) = 100 101 110 111;
//! iterate+(dk(a,b)) = 10 100 101 11 110 111; first(a,b) = 10;
//! first(c,b) = 11; dc one(dk(a,b)) = first one(dk(a,b)) = 10.
use elan_runtime::strat::*;
use elan_runtime::Int;

fn a(t: &Int, k: K<Int>) -> bool {
    *t < 100 && k(&(t * 10))
}
fn b(t: &Int, k: K<Int>) -> bool {
    *t < 100 && k(&(t * 10 + 1))
}
fn c(t: &Int, k: K<Int>) -> bool {
    *t > 100 && k(t)
}
fn dk_ab(t: &Int, k: K<Int>) -> bool {
    dk(t, k, &[&a, &b])
}

fn results(s: impl Fn(&Int, K<Int>) -> bool) -> Vec<Int> {
    all_results(&1, &s)
}

#[test]
fn orders_of_results_match_the_interpreter() {
    assert_eq!(
        results(|t, k| iterate_star(t, k, &dk_ab)),
        vec![1, 10, 100, 101, 11, 110, 111]
    );
    assert_eq!(
        results(|t, k| iterate_plus(t, k, &dk_ab)),
        vec![10, 100, 101, 11, 110, 111]
    );
    assert_eq!(
        results(|t, k| repeat_star(t, k, &dk_ab)),
        vec![100, 101, 110, 111]
    );
    assert_eq!(
        results(|t, k| repeat_plus(t, k, &dk_ab)),
        vec![100, 101, 110, 111]
    );
    assert_eq!(
        results(|t, k| seq(t, k, &dk_ab, &dk_ab)),
        vec![100, 101, 110, 111]
    );
    assert_eq!(results(dk_ab), vec![10, 11]);
    assert_eq!(results(|t, k| first(t, k, &[&a, &b])), vec![10]);
    assert_eq!(results(|t, k| first(t, k, &[&c, &b])), vec![11]);
    assert_eq!(results(|t, k| first(t, k, &[&c, &dk_ab])), vec![10, 11]);
    assert_eq!(results(|t, k| first_one(t, k, &[&c, &dk_ab])), vec![10]);
    assert_eq!(results(|t, k| one(t, k, &dk_ab)), vec![10]);
    assert_eq!(results(id), vec![1]);
    assert_eq!(results(fail), Vec::<Int>::new());
    // S fails at once: repeat* gives the term, repeat+ and iterate+ nothing
    assert_eq!(results(|t, k| repeat_star(t, k, &c)), vec![1]);
    assert_eq!(results(|t, k| repeat_plus(t, k, &c)), Vec::<Int>::new());
    assert_eq!(results(|t, k| iterate_plus(t, k, &c)), Vec::<Int>::new());
    assert_eq!(results(|t, k| first(t, k, &[&c, &fail])), Vec::<Int>::new());
}

#[test]
fn stop_ends_the_search() {
    let mut seen = Vec::new();
    let stopped = iterate_star(
        &1,
        &mut |r| {
            seen.push(*r);
            seen.len() == 3
        },
        &dk_ab,
    );
    assert!(stopped);
    assert_eq!(seen, vec![1, 10, 100]);
    // a strategy that delivers all its results returns false
    assert!(!dk_ab(&1, &mut |_| false));
    // stop propagates through dk, seq, first, repeat*
    assert!(dk(&1, &mut |_| true, &[&a, &b]));
    assert!(seq(&1, &mut |r| *r == 101, &dk_ab, &dk_ab));
    assert!(first(&1, &mut |_| true, &[&c, &b]));
    let mut n = 0;
    assert!(repeat_star(
        &1,
        &mut |_| {
            n += 1;
            n == 2
        },
        &dk_ab
    ));
    assert_eq!(n, 2);
    assert_eq!(first_result(&1, &dk_ab), Some(10));
    assert_eq!(first_result(&1, &c), None);
}

#[test]
fn deterministic_repeat_is_a_loop() {
    // repeat*(first one(a)) from 1: 1 -> 10 -> 100
    assert_eq!(repeat_star_det(1, |t| first_result(t, &a)), 100);
    // a long deterministic loop does not recurse
    assert_eq!(
        repeat_star_det(0i64, |t| if *t < 10_000_000 { Some(t + 1) } else { None }),
        10_000_000
    );
}

#[test]
fn strategies_over_shared_terms() {
    // CPS over H<_>: results are borrowed; cloned only when kept
    use elan_runtime::{impl_sort, H};
    #[derive(PartialEq, Eq, Hash, Debug)]
    enum Nat {
        Z,
        S(H<Nat>),
    }
    impl_sort!(Nat);
    fn pred(t: &H<Nat>, k: K<H<Nat>>) -> bool {
        match &**t {
            Nat::S(x) => k(x),
            Nat::Z => false,
        }
    }
    let mut x = H::new(Nat::Z);
    for _ in 0..5 {
        x = H::new(Nat::S(x));
    }
    let r = all_results(&x, &|t: &H<Nat>, k: K<H<Nat>>| repeat_star(t, k, &pred));
    assert_eq!(r, vec![H::new(Nat::Z)]);
    assert_eq!(
        all_results(&x, &|t: &H<Nat>, k: K<H<Nat>>| iterate_star(t, k, &pred)).len(),
        6
    );
}
