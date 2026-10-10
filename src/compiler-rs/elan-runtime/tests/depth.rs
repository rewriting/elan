//! The stack guard (`elan_runtime::depth`): a recursion on a thread whose
//! limit is set sees it before the end of its stack. (One test
//! in its own binary: the limit is global to the process.)
use elan_runtime::depth::{past_limit, set_limit, too_deep_message, MARGIN};
use elan_runtime::driver::run_with_stack;

/// Recurse until the guard's check says stop; the depth reached.
#[inline(never)]
fn dive(n: u64) -> u64 {
    let pad = [n as u8; 256];
    if past_limit() {
        return n;
    }
    std::hint::black_box(&pad);
    dive(n + 1) + std::hint::black_box(0)
}

#[test]
fn the_limit_is_reached_before_the_end_of_the_stack() {
    assert!(!past_limit(), "no limit before set_limit");
    let size = 64 << 20;
    let depth = run_with_stack(size, move || {
        set_limit(size);
        dive(0)
    });
    // frames of at least 256 bytes, not more than a few KB: the limit is
    // about size - MARGIN below the start of the thread
    assert!((depth * 256) < size as u64, "depth {depth}");
    assert!((depth * 4096) > (size - MARGIN) as u64, "depth {depth}");
    assert!(too_deep_message(size).starts_with("elan: recursion too deep (stack of 64 MiB"));
}
