//! AC multisets: canonical form and helpers.
mod common;
use common::*;
use elan_runtime::ac::{self, AcSet, Rest};
use elan_runtime::print::to_text;
use elan_runtime::{created_terms, H};

fn ids(s: &[P]) -> Vec<u64> {
    s.iter().map(|x| x.id()).collect()
}

#[test]
fn canonical_form_is_independent_of_grouping_and_order() {
    on_thread(8 << 10, || {
        let (a, b, c) = (atom(1), atom(2), atom(3));
        let x = and(&and(&a, &b), &c);
        let y = and(&a, &and(&b, &c));
        let z = and(&c, &and(&b, &a));
        assert!(x == y && y == z);
        let Prop::And(s) = &*x else { panic!() };
        assert_eq!(ids(s), vec![a.id(), b.id(), c.id()]);
        // flattening only applies to the AC symbol itself
        let n = not(&and(&a, &b));
        let Prop::And(s) = &*and(&n, &c) else {
            panic!()
        };
        assert_eq!(s.len(), 2);
        assert_eq!(to_text(&*x), "and(p1,p2,p3)");
    })
}

#[test]
fn sorted_by_creation_id_duplicates_kept() {
    on_thread(8 << 10, || {
        let c = atom(30); // created first: smallest id
        let a = atom(10);
        let b = atom(20);
        let x = and(&and(&a, &b), &and(&c, &a));
        let Prop::And(s) = &*x else { panic!() };
        assert_eq!(ids(s), vec![c.id(), a.id(), a.id(), b.id()]);
        assert_eq!(to_text(&*x), "and(p30,p10,p10,p20)");
        assert_eq!(s.count(&a), 2);
        assert_eq!(s.count(&c), 1);
        assert_eq!(s.count(&atom(99)), 0);
        assert_eq!(s.find(&a), Some(1));
        assert_eq!(s.find(&b), Some(3));
        let d: Vec<(usize, u64, usize)> = s.distinct().map(|(i, e, m)| (i, e.id(), m)).collect();
        assert_eq!(d, vec![(0, c.id(), 1), (1, a.id(), 2), (3, b.id(), 1)]);
        // from_vec sorts; merge_all and flat_all agree
        let v = AcSet::from_vec(vec![b.clone(), a.clone(), c.clone(), a.clone()]);
        assert!(*v == **s);
        let f = ac::flat_all(&[and(&a, &b), c.clone(), a.clone()], and_view);
        assert!(*f == **s);
        let i = AcSet::from_vec(vec![c.clone(), b.clone()])
            .insert(&a)
            .insert(&a);
        assert!(*i == **s);
    })
}

#[test]
fn remove_and_rest() {
    on_thread(8 << 10, || {
        let (a, b, c) = (atom(1), atom(2), atom(3));
        let s = AcSet::from_vec(vec![a.clone(), b.clone(), b.clone(), c.clone()]);
        // remove one occurrence of b: one b is left
        let Some(Rest::Many(r)) = s.remove_one(&b) else {
            panic!()
        };
        assert_eq!(ids(&r), vec![a.id(), b.id(), c.id()]);
        assert!(s.remove_one(&atom(9)).is_none());
        // rest of positions
        let Rest::One(x) = s.rest(&[0, 1, 3]) else {
            panic!()
        };
        assert!(x == b);
        assert!(s.rest(&[3, 2, 1, 0]).is_empty());
        let Rest::Many(r) = s.rest(&[2, 0]) else {
            panic!()
        };
        assert_eq!(ids(&r), vec![b.id(), c.id()]);
        // a rest as a term
        let mk = |m| H::new(Prop::And(m));
        assert!(s.rest(&[0, 1]).into_term(mk).unwrap() == and(&b, &c));
        assert!(s.rest(&[0, 1, 2]).into_term(mk).unwrap() == c);
        assert!(s.rest(&[0, 1, 2, 3]).into_term(mk).is_none());
    })
}

#[test]
fn first_occurrence_among_remaining_positions() {
    on_thread(8 << 10, || {
        let (a, b) = (atom(1), atom(2));
        let s = AcSet::from_vec(vec![a.clone(), a.clone(), a.clone(), b.clone()]);
        // distinct remaining elements when position 0 is taken: 1 (a), 3 (b)
        let firsts: Vec<usize> = (0..s.len())
            .filter(|&j| ac::ac_first_occ(&s, j, &[0]))
            .collect();
        assert_eq!(firsts, vec![1, 3]);
        let firsts: Vec<usize> = (0..s.len())
            .filter(|&j| ac::ac_first_occ(&s, j, &[]))
            .collect();
        assert_eq!(firsts, vec![0, 3]);
        assert_eq!(ac::ac_mult(&s, 0), 3);
        assert_eq!(ac::ac_mult(&s, 1), 2);
    })
}

#[test]
fn lookup_by_parts_before_building() {
    on_thread(8 << 10, || {
        let (a, b, c) = (atom(1), atom(2), atom(3));
        let ab = and(&a, &b);
        let (x, y) = (ab.clone(), c.clone());
        let (ea, eb) = (ac::elems(&x, and_view), ac::elems(&y, and_view));
        let h = ac::ac_hash_merge(TAG_AND, ea, eb);
        assert_eq!(h, ac::ac_hash(TAG_AND, &AcSet::merge(ea, eb)));
        let probe = |h| {
            H::find(
                h,
                |v: &Prop| matches!(v, Prop::And(s) if ac::ac_eq_merge(s, ea, eb)),
            )
        };
        assert!(probe(h).is_none());
        let abc = and_lookup(&ab, &c);
        let n = created_terms();
        // now found without building: no node created, same pointer
        assert!(probe(h).unwrap() == abc);
        assert!(and_lookup(&ab, &c) == abc);
        assert!(and(&c, &ab) == abc);
        assert_eq!(created_terms(), n);
    })
}

#[test]
fn ac_terms_are_freed() {
    on_thread(8 << 10, || {
        let base = elan_runtime::live_terms();
        {
            let atoms: Vec<P> = (0..1000).map(atom).collect();
            let mut x = and(&atoms[0], &atoms[1]);
            for e in &atoms[2..] {
                x = and(&x, e);
            }
            let Prop::And(s) = &*x else { panic!() };
            assert_eq!(s.len(), 1000);
        }
        assert_eq!(elan_runtime::live_terms(), base);
    })
}
