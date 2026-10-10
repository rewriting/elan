//! Shared terms: sharing, freeing, bounded tables, deep drop.
mod common;
use common::*;
use elan_runtime::release::max_release_queue;
use elan_runtime::term::MIN_CAPACITY;
use elan_runtime::{created_terms, live_terms, live_terms_of, purge, table_stats, H};

#[test]
fn same_structure_is_the_same_node() {
    on_thread(8 << 10, || {
        let x = nat(3);
        let y = nat(3);
        assert!(x == y);
        assert_eq!(x.id(), y.id());
        assert!(std::ptr::eq(&*x, &*y));
        assert_ne!(nat(2), x);
        // two handles + nothing else hold the node
        assert_eq!(H::ref_count(&x), 2);
        // children are shared too: s(s(s(o))) holds s(s(o))
        let Nat::S(c) = &*x else { panic!() };
        assert!(*c == nat(2));
        // a deref'd match binds by reference into the node
        assert!(matches!(&*nat(1), Nat::S(o) if *o == z()));
        // 4 nodes: o, s(o), s(s(o)), s(s(s(o)))
        assert_eq!(live_terms_of::<Nat>(), 4);
    })
}

#[test]
fn ids_are_creation_order_shared_by_all_sorts() {
    on_thread(8 << 10, || {
        let c0 = created_terms();
        let a = z();
        let l = nil();
        let b = s(&a);
        assert_eq!((a.id(), l.id(), b.id()), (c0, c0 + 1, c0 + 2));
        assert!(a < b && l.id() < b.id());
        // a hit creates nothing
        let _a2 = z();
        assert_eq!(created_terms(), c0 + 3);
    })
}

#[test]
fn dropped_terms_are_freed_and_rebuilt_with_a_new_id() {
    on_thread(8 << 10, || {
        let base = live_terms();
        let x = nat(10);
        let id = x.id();
        assert_eq!(live_terms(), base + 11);
        drop(x);
        assert_eq!(live_terms(), base);
        assert_eq!(live_terms_of::<Nat>(), 0);
        // still in the table as dead entries, until purged
        assert!(table_stats::<Nat>().used >= 11);
        purge::<Nat>();
        let st = table_stats::<Nat>();
        assert_eq!((st.used, st.live), (0, 0));
        // rebuilt: a new node, a new id
        let x = nat(10);
        assert!(x.id() > id);
    })
}

#[test]
fn shared_subterm_survives_its_parent() {
    on_thread(8 << 10, || {
        let base = live_terms();
        let two = nat(2);
        let four = {
            let three = s(&two);
            s(&three)
        };
        drop(two);
        assert_eq!(live_terms(), base + 5);
        let Nat::S(three) = &*four else { panic!() };
        let three = three.clone();
        drop(four);
        assert_eq!(live_terms(), base + 4);
        assert!(three == nat(3));
        drop(three);
        assert_eq!(live_terms(), base);
    })
}

#[test]
fn constant_memory_over_a_long_loop() {
    on_thread(8 << 10, || {
        let base = live_terms();
        let mut max_cap = 0;
        let mut max_used = 0;
        let mut acc = 0i64;
        for i in 0..20_000i64 {
            // 50 fresh cells per iteration (the ints differ every time)
            let mut l = nil();
            for j in 0..50 {
                l = cons(i * 100 + j, &l);
            }
            if let List::Cons(x, _) = &*l {
                acc += *x;
            }
            drop(l);
            assert_eq!(live_terms(), base);
            let st = table_stats::<List>();
            max_cap = max_cap.max(st.capacity);
            max_used = max_used.max(st.used);
        }
        assert!(acc > 0);
        // 10^6 terms were built; the table never held more than its minimum
        assert_eq!(max_cap, MIN_CAPACITY);
        assert!(max_used <= MIN_CAPACITY / 2);
        // and a big live set makes it grow, then shrink back once freed
        let big = list(100_000);
        assert!(table_stats::<List>().capacity >= 2 * 100_000);
        drop(big);
        let keep = list(10);
        purge::<List>();
        assert_eq!(table_stats::<List>().capacity, MIN_CAPACITY);
        assert_eq!(table_stats::<List>().live, 11);
        drop(keep);
        assert_eq!(live_terms(), base);
    })
}

#[test]
fn deep_self_recursive_list_dropped_on_a_small_stack() {
    on_thread(2 << 10, || {
        let base = live_terms();
        let l = list(1_000_000);
        assert_eq!(live_terms(), base + 1_000_001);
        drop(l);
        assert_eq!(live_terms(), base);
        // a list frees one cell at a time
        assert!(max_release_queue() <= 1);
    })
}

#[test]
fn deep_mutually_recursive_chain_dropped_on_a_small_stack() {
    on_thread(2 << 10, || {
        let base = live_terms();
        let x = chain(1_000_000);
        assert_eq!(live_terms(), base + 2_000_001);
        assert_eq!(live_terms_of::<B>(), 1_000_000);
        drop(x);
        assert_eq!(live_terms(), base);
        assert_eq!(live_terms_of::<A>(), 0);
        assert_eq!(live_terms_of::<B>(), 0);
        assert!(max_release_queue() <= 1);
    })
}

#[test]
fn deep_ac_and_nested_sorts_dropped_on_a_small_stack() {
    on_thread(2 << 10, || {
        let base = live_terms();
        // not(not(...not(and(p0, ..., p9999))...)), 10^6 deep
        let mut v = Vec::new();
        for i in 0..10_000 {
            v.push(atom(i));
        }
        let mut x = H::new(Prop::And(elan_runtime::AcSet::from_vec(v)));
        for _ in 0..1_000_000 {
            x = H::new(Prop::Not(x));
        }
        drop(x);
        assert_eq!(live_terms(), base);
    })
}

#[test]
fn a_deep_term_shared_by_two_parents_is_freed_once() {
    on_thread(2 << 10, || {
        let base = live_terms();
        let l = list(300_000);
        let x = cons(-1, &l);
        let y = cons(-2, &l);
        drop(l);
        drop(x);
        assert_eq!(live_terms(), base + 300_002);
        drop(y);
        assert_eq!(live_terms(), base);
    })
}

thread_local! {
    static KEEP: std::cell::RefCell<Option<L>> = const { std::cell::RefCell::new(None) };
}

#[test]
fn terms_in_thread_locals_at_thread_exit() {
    // a deep term held by a thread-local destroyed at thread exit
    on_thread(2 << 10, || {
        KEEP.with(|k| *k.borrow_mut() = Some(list(200_000)));
    });
}
