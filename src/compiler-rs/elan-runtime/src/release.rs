//! Iterative freeing of shared terms (spec D5).
//!
//! Rust's default drop of a deep term (a list of 10^6 cells) recurses once
//! per level and overflows the stack. The spike (`spikes/codegen/rust-enum`)
//! made it iterative with a generated `children_into` per sort that moved the
//! *same-sort* children onto a local stack; children of another sort were
//! dropped recursively, so mutually recursive sorts (A contains B contains
//! A) still recursed once per level.
//!
//! Here the machinery is generic and needs no generated code:
//!
//! * One **per-thread queue** of pending nodes, type-erased without
//!   allocation: an entry is the node pointer (`Rc::into_raw`) and the
//!   monomorphised function that frees a node of its sort (16 bytes).
//! * When the last `H` of a node is dropped ([`release`]) and no release is
//!   running on this thread, the node is freed *here*: its value is dropped
//!   normally, so every child `H` (of any sort, directly in a field, in an
//!   AC multiset, in a `Box`, a `Vec`...) runs its own `Drop`. A child that
//!   is still shared is just decremented; a child whose count reaches zero
//!   sees that a release is running and is **pushed onto the queue** instead
//!   of being freed. Then the queue is drained, entry by entry, the same way.
//! * Stack depth is therefore one node, whatever the shape of the term and
//!   the nesting of sorts; the queue holds at most the number of pending
//!   unique children (1 for a list, n for an n-element AC multiset).
//!
//! The generated code only derives its enums; no per-sort drop code (the
//! spike's `children_into`) is needed. Contract: a sort enum must not hold a
//! term through a type whose `Drop` interns terms or calls user code.
//!
//! Limits:
//!
//! * A `Drop` impl of a *non-term* value stored in a node runs during the
//!   drain; it must not intern terms (generated values never do).
//! * A panic inside a drop during the drain resets the "running" flag; the
//!   nodes still queued are leaked, not freed twice.
//! * At thread exit, once the queue's thread-local is destroyed, a node
//!   whose last reference is dropped (from another thread-local's
//!   destructor, e.g. a table of constants) is leaked instead of freed: the
//!   memory is returned by the exit itself.

use std::cell::{Cell, RefCell};
use std::rc::Rc;

use crate::term::{Node, Sort};

struct Pending {
    ptr: *const (),
    free: unsafe fn(*const ()),
}

thread_local! {
    static RUNNING: Cell<bool> = const { Cell::new(false) };
    static QUEUE: RefCell<Vec<Pending>> = const { RefCell::new(Vec::new()) };
    static MAX_QUEUE: Cell<usize> = const { Cell::new(0) };
}

/// Capacity kept by the queue after a drain (a larger one is shrunk).
const KEEP_CAPACITY: usize = 1 << 12;

/// Free a node of sort `T` whose pointer came from `Rc::into_raw`.
unsafe fn free_node<T: Sort>(p: *const ()) {
    // SAFETY: `p` was produced by `Rc::into_raw` on an `Rc<Node<T>>` and is
    // consumed exactly once.
    drop(Rc::from_raw(p as *const Node<T>));
}

struct Running;
impl Drop for Running {
    fn drop(&mut self) {
        RUNNING.with(|r| r.set(false));
    }
}

/// Drop the last reference `rc` to a node.
#[inline(never)]
pub(crate) fn release<T: Sort>(rc: Rc<Node<T>>) {
    if RUNNING.with(|r| r.get()) {
        let p = Pending {
            ptr: Rc::into_raw(rc) as *const (),
            free: free_node::<T>,
        };
        // If the queue is already destroyed (thread exit), the node leaks.
        let _ = QUEUE.try_with(|q| q.borrow_mut().push(p));
        return;
    }
    if QUEUE.try_with(|_| ()).is_err() {
        // Thread exit: the queue is gone. Leak rather than recurse.
        std::mem::forget(rc);
        return;
    }
    RUNNING.with(|r| r.set(true));
    let _guard = Running;
    drop(rc); // frees the node; its unique children are queued
    let mut max = 0;
    loop {
        let next = QUEUE.with(|q| {
            let mut q = q.borrow_mut();
            max = max.max(q.len());
            q.pop()
        });
        match next {
            // SAFETY: each entry is consumed once, by its own free function.
            Some(p) => unsafe { (p.free)(p.ptr) },
            None => break,
        }
    }
    QUEUE.with(|q| {
        let mut q = q.borrow_mut();
        if q.capacity() > KEEP_CAPACITY {
            q.shrink_to(KEEP_CAPACITY);
        }
    });
    MAX_QUEUE.with(|m| m.set(m.get().max(max)));
}

/// Largest number of nodes waiting in the release queue so far on this
/// thread (tests: it stays 1 when a list is freed).
pub fn max_release_queue() -> usize {
    MAX_QUEUE.with(|m| m.get())
}
