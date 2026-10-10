//! Strategies in continuation-passing style (spec D3).
//!
//! A strategy applied to `t` calls its continuation `k(&r)` once per result
//! `r`, in ELAN's order. `k` returns `true` to **stop**: the strategy then
//! returns `true` at once, without computing further results (a `one`, a
//! `first one`, the end of the program). A strategy that returns `false`
//! has delivered all its results (possibly none: failure).
//!
//! Terms are passed by reference: a continuation clones a result only when
//! it stores it. The combinators are generic over the term type `T` and over
//! the strategy type `S` (a `fn`, a closure or a `dyn Fn`); the generated
//! code calls them or inlines its own loops (S6b extends this set).
//!
//! Orders (checked against the interpreter, `elan -b`, on a rule set with
//! two results per step): `iterate*` is depth first, the term itself first
//! (1, 10, 100, 101, 11, 110, 111); `repeat*` yields the leaves of the same
//! tree in the same order (100, 101, 110, 111).

/// A success continuation: called once per result; `true` means stop.
pub type K<'a, T> = &'a mut dyn FnMut(&T) -> bool;

/// A strategy as a function pointer.
pub type Strat<T> = for<'a> fn(&T, K<'a, T>) -> bool;

/// A strategy as a trait object (strategies with parameters, closures).
pub type DynStrat<'s, T> = &'s dyn for<'a> Fn(&T, K<'a, T>) -> bool;

/// `id`: the term itself.
#[inline]
pub fn id<T: ?Sized>(t: &T, k: K<T>) -> bool {
    k(t)
}

/// `fail`: no result.
#[inline]
pub fn fail<T: ?Sized>(_t: &T, _k: K<T>) -> bool {
    false
}

/// `s1 ; s2`: s2 applied to every result of s1.
#[inline]
pub fn seq<T, S1, S2>(t: &T, k: K<T>, s1: &S1, s2: &S2) -> bool
where
    S1: Fn(&T, K<T>) -> bool + ?Sized,
    S2: Fn(&T, K<T>) -> bool + ?Sized,
{
    s1(t, &mut |r| s2(r, k))
}

/// `dk(S1,...,Sn)`: all results of every Si, in order.
pub fn dk<T>(t: &T, k: K<T>, ss: &[DynStrat<T>]) -> bool {
    for s in ss {
        if s(t, k) {
            return true;
        }
    }
    false
}

/// `first(S1,...,Sn)` (and `dc`, which the compiled code implements as
/// `first`): all results of the first Si that has at least one.
pub fn first<T>(t: &T, k: K<T>, ss: &[DynStrat<T>]) -> bool {
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

/// The first result of `s` applied to `t`, if any (the search stops there).
pub fn first_result<T: Clone, S>(t: &T, s: &S) -> Option<T>
where
    S: Fn(&T, K<T>) -> bool + ?Sized,
{
    let mut res = None;
    s(t, &mut |r| {
        res = Some(r.clone());
        true
    });
    res
}

/// All results of `s` applied to `t`, in order (tests, small searches).
pub fn all_results<T: Clone, S>(t: &T, s: &S) -> Vec<T>
where
    S: Fn(&T, K<T>) -> bool + ?Sized,
{
    let mut v = Vec::new();
    s(t, &mut |r| {
        v.push(r.clone());
        false
    });
    v
}

/// `first one(S1,...,Sn)` (and `dc one`): one result, from the first Si
/// that has one.
pub fn first_one<T: Clone>(t: &T, k: K<T>, ss: &[DynStrat<T>]) -> bool {
    for s in ss {
        if let Some(r) = first_result(t, *s) {
            return k(&r);
        }
    }
    false
}

/// `one(S)`: the first result of S.
pub fn one<T: Clone, S>(t: &T, k: K<T>, s: &S) -> bool
where
    S: Fn(&T, K<T>) -> bool + ?Sized,
{
    match first_result(t, s) {
        Some(r) => k(&r),
        None => false,
    }
}

/// `iterate*(S)`: t, then `iterate*(S)` of each result of S(t), depth first.
pub fn iterate_star<T, S>(t: &T, k: K<T>, s: &S) -> bool
where
    S: Fn(&T, K<T>) -> bool + ?Sized,
{
    if k(t) {
        return true;
    }
    s(t, &mut |r| iterate_star(r, k, s))
}

/// `iterate+(S)`: `iterate*(S)` of each result of S(t) (t itself excluded).
pub fn iterate_plus<T, S>(t: &T, k: K<T>, s: &S) -> bool
where
    S: Fn(&T, K<T>) -> bool + ?Sized,
{
    s(t, &mut |r| iterate_star(r, k, s))
}

/// `repeat*(S)`: the terms reached from t on which S fails, depth first
/// (t itself if S fails on t).
pub fn repeat_star<T, S>(t: &T, k: K<T>, s: &S) -> bool
where
    S: Fn(&T, K<T>) -> bool + ?Sized,
{
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

/// `repeat+(S)`: as `repeat*(S)`, but S must apply at least once.
pub fn repeat_plus<T, S>(t: &T, k: K<T>, s: &S) -> bool
where
    S: Fn(&T, K<T>) -> bool + ?Sized,
{
    s(t, &mut |r| repeat_star(r, k, s))
}

/// `repeat*(S)` for a deterministic S given as a step function (`None`:
/// S fails): a loop, no recursion.
pub fn repeat_star_det<T, F>(t: T, mut step: F) -> T
where
    F: FnMut(&T) -> Option<T>,
{
    let mut t = t;
    while let Some(r) = step(&t) {
        t = r;
    }
    t
}
