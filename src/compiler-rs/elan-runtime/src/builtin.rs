//! Builtin sorts: int, bool, string (spec D2: native values).
//!
//! # int
//!
//! An ELAN `int` is an `i64` in the generated code, but its **semantics are
//! the interpreter's 32-bit C `int`**: `src/interpreter/rewrite/rtmisc.cc`
//! (`standardreduction`, cases `PLUS`..`UNMIN`) computes
//! `t.subterm(0)->head() + t.subterm(1)->head()` on `int` heads, and the
//! number lexer stores literals in an `int` (`crnumlex(atoi(..))`). Checked
//! with the interpreter (`elan -b`, macOS arm64, 2026-10-10):
//!
//! | expression | interpreter |
//! |---|---|
//! | `65536 * 65536` | 0 (`65536*65536 == 0` is `true`) |
//! | `65536 * 65536 + 2147483647 + 1` | -2147483648 |
//! | `0 - 2147483647 - 1 - 1` | 2147483647 |
//! | literal `4294967297` | 1 |
//! | `- (0-2147483647-1)` | -2147483648 |
//! | `(0-7) / 2`, `(0-7) % 3` | -3, -1 (C truncation) |
//! | `5 / 0`, `5 % 0`, `(0-5) % 0` | 0, 5, -5 (arm64 `sdiv`; SIGFPE on x86) |
//! | `(0-2147483647-1) / (0-1)`, `% (0-1)` | -2147483648, 0 |
//!
//! So every operation wraps to 32 bits (two's complement, as C `int` does
//! in practice; the interpreter is not built with `-ftrapv`), and an `Int`
//! value always lies in the `i32` range. Division by zero follows arm64
//! (quotient 0, remainder the dividend) rather than trapping. The compiled
//! C runtime (`src/compiler/runtime/builtin.h`, 63-bit tagged `long`)
//! differs on intermediate overflow and only prints the low 32 bits
//! (`termOut.c`, `(int)GgetInt`, test `compiled_int_prints_low_32_bits`);
//! [`int_print`] keeps that rule, which is the identity on wrapped values.
//!
//! # bool
//!
//! `bool`; `and`/`or` are strict (both arguments are normalised first, as in
//! innermost rewriting), printed `true`/`false`.
//!
//! # string
//!
//! [`Str`] = `Rc<str>` (cheap clone, structural equality). The primitives of
//! `builtinString.eln` mirror `rtmisc.cc` (`STRLENGTH`..`STRNG`); the partial
//! ones return `None` where the interpreter leaves the term unreduced.
//! Printing: the interpreter quotes strings (`"abc d"`), the compiled C
//! runtime prints them raw (`abc d`, `termCommon.c`); see
//! [`crate::print::Writer::string`].
//!
//! # Stuck terms of builtin sorts
//!
//! A defined operator whose result sort is builtin (`f : (int) int`,
//! `string_substr(s,i,j)` out of range) has no value when no rule applies:
//! the term stays `f(3)`, and the interpreter prints it. Design: the
//! generator keeps the native type for a builtin sort when no operator of
//! that sort can be stuck (it proves every rule set total, e.g. a final
//! variable rule, or only builtin constructors), and otherwise uses
//! [`Builtin<V, S>`] for that sort: `Val(v)` or `Stuck(s)`, where `S` is a
//! generated hash-consed sort (`H<IntStuck>`) with one variant per operator
//! that can be stuck **and** one per builtin operator applied to a stuck
//! argument (`f(3) + 1` stays `f(3) + 1`, as `two_int_args` fails in the
//! interpreter). [`lift1`]/[`lift2`] apply a builtin operation or build the
//! stuck term.

use std::rc::Rc;

/// An ELAN `int` value (always in the `i32` range, see the module doc).
pub type Int = i64;

/// An ELAN builtin string.
pub type Str = Rc<str>;

#[inline(always)]
fn w(x: i64) -> i64 {
    x as i32 as i64
}

/// An integer literal as the interpreter reads it (low 32 bits).
#[inline]
pub fn int_lit(x: i64) -> Int {
    w(x)
}
#[inline]
pub fn int_add(a: Int, b: Int) -> Int {
    w(a.wrapping_add(b))
}
#[inline]
pub fn int_sub(a: Int, b: Int) -> Int {
    w(a.wrapping_sub(b))
}
#[inline]
pub fn int_mul(a: Int, b: Int) -> Int {
    (a as i32).wrapping_mul(b as i32) as i64
}
/// C truncating division; `x / 0 = 0` (arm64), `MIN / -1 = MIN`.
#[inline]
pub fn int_div(a: Int, b: Int) -> Int {
    if b as i32 == 0 {
        0
    } else {
        (a as i32).wrapping_div(b as i32) as i64
    }
}
/// C remainder (sign of the dividend); `x % 0 = x` (arm64), `MIN % -1 = 0`.
#[inline]
pub fn int_rem(a: Int, b: Int) -> Int {
    if b as i32 == 0 {
        w(a)
    } else {
        (a as i32).wrapping_rem(b as i32) as i64
    }
}
#[inline]
pub fn int_neg(a: Int) -> Int {
    (a as i32).wrapping_neg() as i64
}
#[inline]
pub fn int_and(a: Int, b: Int) -> Int {
    w(a & b)
}
#[inline]
pub fn int_or(a: Int, b: Int) -> Int {
    w(a | b)
}
/// The printed value of an int: its low 32 bits as a signed C `int`.
#[inline]
pub fn int_print(a: Int) -> i32 {
    a as i32
}

pub fn bool_print(b: bool) -> &'static str {
    if b {
        "true"
    } else {
        "false"
    }
}

/// `string_strlen(s)` (bytes).
pub fn str_len(s: &str) -> Int {
    w(s.len() as i64)
}
/// `string_strcat(s1, s2)`.
pub fn str_cat(a: &str, b: &str) -> Str {
    let mut s = String::with_capacity(a.len() + b.len());
    s.push_str(a);
    s.push_str(b);
    s.into()
}
/// `s[i]`: the byte at i (C `char`, signed), `None` (stuck) out of range.
pub fn str_index(s: &str, i: Int) -> Option<Int> {
    let b = s.as_bytes();
    if 0 <= i && (i as usize) < b.len() {
        Some(b[i as usize] as i8 as i64)
    } else {
        None
    }
}
/// `s[i <- c]`: s with byte i replaced by c, `None` out of range. (The
/// interpreter modifies the string in place, sharing included; here the
/// result is a new string. A byte that breaks UTF-8 is replaced lossily.)
pub fn str_modif(s: &str, i: Int, c: Int) -> Option<Str> {
    let mut b = s.as_bytes().to_vec();
    if 0 <= i && (i as usize) < b.len() {
        b[i as usize] = c as u8;
        Some(String::from_utf8_lossy(&b).as_ref().into())
    } else {
        None
    }
}
/// `string_substr(s, i, n)`: n bytes from i, `None` unless
/// `0 <= i < len` and `0 <= n` and `i + n <= len`.
pub fn str_substr(s: &str, i: Int, n: Int) -> Option<Str> {
    let l = s.len() as i64;
    if !(0 <= i && i < l && 0 <= n && i + n <= l) {
        return None;
    }
    let b = &s.as_bytes()[i as usize..(i + n) as usize];
    Some(String::from_utf8_lossy(b).as_ref().into())
}
/// `string_strspn(s, accept)` (C `strspn`).
pub fn str_spn(s: &str, accept: &str) -> Int {
    let a = accept.as_bytes();
    w(s.bytes().take_while(|c| a.contains(c)).count() as i64)
}
/// `string_strcmp(a, b)`: as the interpreter on macOS, the difference of
/// the first differing bytes (`strcmp("a","d") = -3`), 0 if equal.
pub fn str_cmp(a: &str, b: &str) -> Int {
    let (a, b) = (a.as_bytes(), b.as_bytes());
    let n = a.len().max(b.len());
    for i in 0..n {
        let x = a.get(i).copied().unwrap_or(0) as i64;
        let y = b.get(i).copied().unwrap_or(0) as i64;
        if x != y {
            return x - y;
        }
    }
    0
}
/// `string_string(c)`: the one-character string of byte c.
pub fn str_of_char(c: Int) -> Str {
    String::from_utf8_lossy(&[c as u8]).as_ref().into()
}

/// A value of a builtin sort that can also be a stuck term (module doc).
#[derive(Clone, PartialEq, Eq, Hash, Debug)]
pub enum Builtin<V, S> {
    Val(V),
    Stuck(S),
}

impl<V: Clone, S> Builtin<V, S> {
    #[inline]
    pub fn val(&self) -> Option<V> {
        match self {
            Builtin::Val(v) => Some(v.clone()),
            Builtin::Stuck(_) => None,
        }
    }
}

/// Apply a unary builtin operation, or build the stuck term
/// (`stuck(a)`) when the argument is stuck or the operation is undefined.
#[inline]
pub fn lift1<A: Clone, V, S, SA>(
    a: &Builtin<A, SA>,
    op: impl FnOnce(A) -> Option<V>,
    stuck: impl FnOnce() -> S,
) -> Builtin<V, S> {
    match a {
        Builtin::Val(x) => match op(x.clone()) {
            Some(v) => Builtin::Val(v),
            None => Builtin::Stuck(stuck()),
        },
        Builtin::Stuck(_) => Builtin::Stuck(stuck()),
    }
}

/// Binary version of [`lift1`].
#[inline]
pub fn lift2<A: Clone, B: Clone, V, S, SA, SB>(
    a: &Builtin<A, SA>,
    b: &Builtin<B, SB>,
    op: impl FnOnce(A, B) -> Option<V>,
    stuck: impl FnOnce() -> S,
) -> Builtin<V, S> {
    match (a, b) {
        (Builtin::Val(x), Builtin::Val(y)) => match op(x.clone(), y.clone()) {
            Some(v) => Builtin::Val(v),
            None => Builtin::Stuck(stuck()),
        },
        _ => Builtin::Stuck(stuck()),
    }
}
