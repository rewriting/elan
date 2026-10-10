//! Bounded recursion (spec D3): a program never overflows its stack.
//!
//! Normalisation functions, strategies in continuation-passing style and
//! the printer recurse; their depth depends on the data (a term 10^7 deep,
//! a strategy recursion 10^7 levels deep). A program runs on a thread with
//! a big stack ([`crate::driver::BIG_STACK`], reserved, not used); the
//! functions that can recurse call [`guard`] at their entry, which compares
//! the stack pointer with a limit fixed when the thread starts
//! ([`set_limit`]): the size of the stack minus a margin
//! ([`MARGIN`]). Past the limit the program stops with the message
//!
//! ```text
//! elan: recursion too deep (stack of 1024 MiB exhausted); ...
//! ```
//!
//! on the standard error and the exit status [`DEPTH_EXIT`] (3), instead
//! of a segmentation fault. The margin covers the frames between two
//! guarded calls (closures of the continuations, builtins, the
//! hash-consing table, the iterative release of terms) and the message.
//!
//! The check is one comparison of the stack pointer with a global value
//! (one program thread per process); before [`set_limit`] the limit is 0:
//! never past. The generated code checks at the entry of the strategies,
//! the labels, the `Strategy[X]` evaluators and the normalisation
//! functions of the operators of a cycle of calls (elanc-rs
//! `Ir::recursive_ops`: a check in a small non-recursive hot function
//! costs); the printer checks at each subterm.
//! Deterministic strategies are compiled as loops where they can be
//! (`repeat*`, `iterate*`, tail-recursive named strategies): those never
//! reach the limit.

use std::sync::atomic::{AtomicUsize, Ordering::Relaxed};

/// Exit status of a program stopped by [`too_deep`] (1: run-time error,
/// 2: construct not supported).
pub const DEPTH_EXIT: i32 = 3;

/// The part of the stack kept free under the limit.
pub const MARGIN: usize = 16 << 20;

/// Lowest stack address allowed at a guarded call (0: no limit). One
/// value for the process, not per thread: a thread-local costs a call on
/// macOS at every guarded call (nqueens 12, every operator guarded: 1.02 s
/// with a thread-local, 0.76 s with this global, 0.40 s unguarded); a
/// program runs on one thread.
static LIMIT: AtomicUsize = AtomicUsize::new(0);
/// Size of the stack of the guarded thread (message).
static SIZE: AtomicUsize = AtomicUsize::new(0);

/// The stack pointer, or an address in the current stack frame (the
/// stacks grow downwards on every supported target). Read with an
/// instruction where possible: `std::hint::black_box` on the address of a
/// local is an optimisation barrier (memory clobber; nqueens 12, every
/// operator guarded: 0.76 s instead of 0.67 s).
#[inline(always)]
pub fn stack_address() -> usize {
    #[cfg(target_arch = "aarch64")]
    {
        let sp: usize;
        // SAFETY: reads the stack pointer register; no memory access.
        unsafe {
            std::arch::asm!("mov {}, sp", out(reg) sp, options(nomem, nostack, preserves_flags));
        }
        sp
    }
    #[cfg(target_arch = "x86_64")]
    {
        let sp: usize;
        // SAFETY: reads the stack pointer register; no memory access.
        unsafe {
            std::arch::asm!("mov {}, rsp", out(reg) sp, options(nomem, nostack, preserves_flags));
        }
        sp
    }
    #[cfg(not(any(target_arch = "aarch64", target_arch = "x86_64")))]
    {
        let x = 0u8;
        std::hint::black_box(&x) as *const u8 as usize
    }
}

/// Stop the program if the stack is nearly exhausted ([`too_deep`]).
#[inline(always)]
pub fn guard() {
    if stack_address() < LIMIT.load(Relaxed) {
        too_deep()
    }
}

/// Is the stack past the limit? (The check of [`guard`], tests.)
pub fn past_limit() -> bool {
    stack_address() < LIMIT.load(Relaxed)
}

/// The message of [`too_deep`].
pub fn too_deep_message(size: usize) -> String {
    format!(
        "elan: recursion too deep (stack of {} MiB exhausted); the program stops \
         (exit status {DEPTH_EXIT}). A deterministic formulation (repeat*, first one, \
         a tail-recursive strategy) runs in constant stack.",
        size >> 20
    )
}

/// Write the results printed so far ([`crate::driver::flush_output`]),
/// print the message on the standard error and exit with [`DEPTH_EXIT`].
#[cold]
#[inline(never)]
pub fn too_deep() -> ! {
    let size = SIZE.load(Relaxed);
    // no more guarded calls on this thread: the message has the margin
    LIMIT.store(0, Relaxed);
    crate::driver::flush_output();
    eprintln!("{}", too_deep_message(size));
    std::process::exit(DEPTH_EXIT)
}

/// Fix the limit for the current thread, whose stack has `size` bytes and
/// started just above the caller's frame (the program thread of
/// [`crate::main_program`]). The limit is global to the process: after
/// this call, a guarded call on another thread with a lower stack would
/// stop the process; only one thread calls the program's code.
pub fn set_limit(size: usize) {
    let base = stack_address();
    let used_below = size.saturating_sub(MARGIN);
    LIMIT.store(base.saturating_sub(used_below), Relaxed);
    SIZE.store(size, Relaxed);
}
