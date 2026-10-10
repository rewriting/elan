//! AC multisets (spec D4, D5).
//!
//! The variant of an AC operator holds an [`AcSet<T>`]: the flattened
//! arguments, **sorted by creation id**, duplicates kept, at least two
//! elements in a term (a rest of 0 or 1 element is not an AC term, see
//! [`Rest`]). Flattening needs to know which variant is the AC operator, so
//! the helpers take a `view`: the generated function returning the multiset
//! of a term if its top symbol is that operator (`|p| match p { Prop::And(s)
//! => Some(s), _ => None }`).
//!
//! The order by id is deterministic for a program and its input, but it
//! depends on which terms were alive when a term was built (a rebuilt term
//! gets a new id): output that must not depend on it (printing an AC term)
//! must not rely on it.

use std::ops::Deref;

use crate::term::{Fx, Sort, H};
use std::hash::Hasher;

/// A canonical multiset of shared terms: sorted by id, duplicates kept.
#[derive(PartialEq, Eq, Hash)]
pub struct AcSet<T: Sort>(Box<[H<T>]>);

impl<T: Sort> Clone for AcSet<T> {
    fn clone(&self) -> Self {
        AcSet(self.0.clone())
    }
}

impl<T: Sort + std::fmt::Debug> std::fmt::Debug for AcSet<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        f.debug_list().entries(self.0.iter()).finish()
    }
}

impl<T: Sort> Deref for AcSet<T> {
    type Target = [H<T>];
    #[inline(always)]
    fn deref(&self) -> &[H<T>] {
        &self.0
    }
}

impl<T: Sort> AcSet<T> {
    /// From any elements (sorted here).
    pub fn from_vec(mut v: Vec<H<T>>) -> AcSet<T> {
        v.sort_unstable();
        AcSet(v.into_boxed_slice())
    }

    /// From elements already sorted by id (checked in debug builds).
    pub fn from_sorted(v: Vec<H<T>>) -> AcSet<T> {
        debug_assert!(
            v.windows(2).all(|w| w[0] <= w[1]),
            "AcSet::from_sorted: not sorted"
        );
        AcSet(v.into_boxed_slice())
    }

    /// Merge two sorted element lists (already flattened).
    pub fn merge(a: &[H<T>], b: &[H<T>]) -> AcSet<T> {
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
        AcSet(out.into_boxed_slice())
    }

    /// Merge any number of sorted element lists.
    pub fn merge_all(parts: &[&[H<T>]]) -> AcSet<T> {
        let mut v: Vec<H<T>> = Vec::with_capacity(parts.iter().map(|p| p.len()).sum());
        for p in parts {
            v.extend_from_slice(p);
        }
        // a stable merge of sorted runs: sort is adaptive on runs
        v.sort();
        AcSet(v.into_boxed_slice())
    }

    /// This multiset with one more occurrence of `e` (after its equals).
    pub fn insert(&self, e: &H<T>) -> AcSet<T> {
        AcSet::merge(&self.0, std::slice::from_ref(e))
    }

    pub fn as_slice(&self) -> &[H<T>] {
        &self.0
    }

    pub fn into_vec(self) -> Vec<H<T>> {
        self.0.into_vec()
    }

    /// Position of the first occurrence of `e`, if present (binary search).
    #[inline]
    pub fn find(&self, e: &H<T>) -> Option<usize> {
        ac_find(&self.0, e)
    }

    /// Multiplicity of `e`.
    pub fn count(&self, e: &H<T>) -> usize {
        match self.find(e) {
            Some(i) => ac_mult(&self.0, i),
            None => 0,
        }
    }

    /// Distinct elements in order: `(position of the first occurrence,
    /// element, multiplicity)`.
    pub fn distinct(&self) -> Distinct<'_, T> {
        Distinct { s: &self.0, i: 0 }
    }

    /// The multiset without the element at `pos`.
    pub fn remove_at(&self, pos: usize) -> Rest<T> {
        ac_rest(&self.0, &[pos])
    }

    /// The multiset without one occurrence of `e` (None if absent).
    pub fn remove_one(&self, e: &H<T>) -> Option<Rest<T>> {
        self.find(e).map(|i| self.remove_at(i))
    }

    /// The multiset without the elements at the positions `removed`.
    pub fn rest(&self, removed: &[usize]) -> Rest<T> {
        ac_rest(&self.0, removed)
    }
}

/// Iterator of [`AcSet::distinct`].
pub struct Distinct<'a, T: Sort> {
    s: &'a [H<T>],
    i: usize,
}

impl<'a, T: Sort> Iterator for Distinct<'a, T> {
    type Item = (usize, &'a H<T>, usize);
    fn next(&mut self) -> Option<Self::Item> {
        if self.i >= self.s.len() {
            return None;
        }
        let i = self.i;
        let m = ac_mult(self.s, i);
        self.i += m;
        Some((i, &self.s[i], m))
    }
}

/// What is left of a multiset after removing elements.
pub enum Rest<T: Sort> {
    Empty,
    One(H<T>),
    Many(AcSet<T>),
}

impl<T: Sort> Rest<T> {
    /// The rest as a term: `None` if empty, the element if one, else
    /// `mk(multiset)` (the generated constructor of the AC variant, not
    /// normalised).
    pub fn into_term(self, mk: impl FnOnce(AcSet<T>) -> H<T>) -> Option<H<T>> {
        match self {
            Rest::Empty => None,
            Rest::One(x) => Some(x),
            Rest::Many(s) => Some(mk(s)),
        }
    }

    pub fn len(&self) -> usize {
        match self {
            Rest::Empty => 0,
            Rest::One(_) => 1,
            Rest::Many(s) => s.len(),
        }
    }

    pub fn is_empty(&self) -> bool {
        matches!(self, Rest::Empty)
    }
}

// ----------------------------------------------------------------------
// Flattening
// ----------------------------------------------------------------------

/// The elements of `x` seen as an argument of the AC operator of `view`:
/// its multiset if `x` has that operator on top, else `x` alone.
#[inline]
pub fn elems<T: Sort>(x: &H<T>, view: impl Fn(&T) -> Option<&AcSet<T>>) -> &[H<T>] {
    match view(x) {
        Some(s) => s,
        None => std::slice::from_ref(x),
    }
}

/// The flattened multiset of `op(x, y)` for the AC operator of `view`.
#[inline]
pub fn flat2<T: Sort>(
    x: &H<T>,
    y: &H<T>,
    view: impl Fn(&T) -> Option<&AcSet<T>> + Copy,
) -> AcSet<T> {
    AcSet::merge(elems(x, view), elems(y, view))
}

/// The flattened multiset of `op(x1, ..., xn)`.
pub fn flat_all<T: Sort>(xs: &[H<T>], view: impl Fn(&T) -> Option<&AcSet<T>> + Copy) -> AcSet<T> {
    let parts: Vec<&[H<T>]> = xs.iter().map(|x| elems(x, view)).collect();
    AcSet::merge_all(&parts)
}

// ----------------------------------------------------------------------
// Slice helpers for generated matching code (positions `removed` are the
// elements already taken by the pattern).
// ----------------------------------------------------------------------

/// Is position `j` the first occurrence of its element among the positions
/// not in `removed`? (iterate over the distinct remaining elements)
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

/// Multiplicity of the element at position `j`, counted from `j` (the full
/// multiplicity when `j` is its first occurrence).
#[inline]
pub fn ac_mult<T: Sort>(s: &[H<T>], j: usize) -> usize {
    let mut m = 1;
    while j + m < s.len() && s[j + m] == s[j] {
        m += 1;
    }
    m
}

/// Position of the first occurrence of `e`, if present.
#[inline]
pub fn ac_find<T: Sort>(s: &[H<T>], e: &H<T>) -> Option<usize> {
    let i = s.partition_point(|x| x.id() < e.id());
    if i < s.len() && s[i] == *e {
        Some(i)
    } else {
        None
    }
}

/// The elements of `s` not at the positions `removed` (any order, no
/// duplicates).
pub fn ac_rest<T: Sort>(s: &[H<T>], removed: &[usize]) -> Rest<T> {
    match s.len() - removed.len() {
        0 => Rest::Empty,
        1 => Rest::One(
            (0..s.len())
                .find(|i| !removed.contains(i))
                .map(|i| s[i].clone())
                .unwrap(),
        ),
        n => {
            let mut v = Vec::with_capacity(n);
            for (i, e) in s.iter().enumerate() {
                if !removed.contains(&i) {
                    v.push(e.clone());
                }
            }
            Rest::Many(AcSet(v.into_boxed_slice()))
        }
    }
}

/// A hash of an AC node computable from its parts (`tag` distinguishes the
/// AC operators of a sort): for a generated [`Sort::node_hash`] that wants
/// lookups by parts ([`H::find`]) before building a merge.
#[inline]
pub fn ac_hash<T: Sort>(tag: u64, elems: &[H<T>]) -> u64 {
    let mut h = Fx::default();
    h.write_u64(tag);
    h.write_usize(elems.len());
    for e in elems {
        h.write_u64(e.id());
    }
    h.finish()
}

/// Hash of the merge of two sorted element lists, without building it:
/// equals `ac_hash(tag, &AcSet::merge(a, b))`.
pub fn ac_hash_merge<T: Sort>(tag: u64, a: &[H<T>], b: &[H<T>]) -> u64 {
    let mut h = Fx::default();
    h.write_u64(tag);
    h.write_usize(a.len() + b.len());
    let (mut i, mut j) = (0, 0);
    while i < a.len() || j < b.len() {
        if j >= b.len() || (i < a.len() && a[i] <= b[j]) {
            h.write_u64(a[i].id());
            i += 1;
        } else {
            h.write_u64(b[j].id());
            j += 1;
        }
    }
    h.finish()
}

/// Does `s` equal the merge of `a` and `b`? (lookup by parts)
pub fn ac_eq_merge<T: Sort>(s: &[H<T>], a: &[H<T>], b: &[H<T>]) -> bool {
    if s.len() != a.len() + b.len() {
        return false;
    }
    let (mut i, mut j) = (0, 0);
    for x in s {
        let y = if j >= b.len() || (i < a.len() && a[i] <= b[j]) {
            i += 1;
            &a[i - 1]
        } else {
            j += 1;
            &b[j - 1]
        };
        if x != y {
            return false;
        }
    }
    true
}
