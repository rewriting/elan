//! Builtins: the interpreter's 32-bit int semantics (values checked with
//! `elan -b` on macOS arm64; see `src/builtin.rs`), strings, stuck terms.
mod common;
use common::*;
use elan_runtime::builtin::*;
use elan_runtime::print::Writer;

#[test]
fn int_arithmetic_wraps_at_32_bits_as_the_interpreter() {
    let max = int_lit(2147483647);
    let min = int_sub(int_sub(0, max), 1);
    assert_eq!(min, -2147483648);
    // 65536*65536 == 0 is true in the interpreter
    assert_eq!(int_mul(65536, 65536), 0);
    // f(65536) = n*n + 2147483647 + 1
    assert_eq!(int_add(int_add(int_mul(65536, 65536), max), 1), -2147483648);
    // 0 - 2147483647 - 1 - 1
    assert_eq!(int_sub(min, 1), 2147483647);
    // 65536 * 65536 * 3 + 1
    assert_eq!(int_add(int_mul(int_mul(65536, 65536), 3), 1), 1);
    // 2147483647 * 2
    assert_eq!(int_mul(max, 2), -2);
    // literal 4294967297 reads as 1
    assert_eq!(int_lit(4294967297), 1);
    assert_eq!(int_neg(min), min);
}

#[test]
fn int_division_as_the_interpreter() {
    let min = -2147483648;
    assert_eq!(int_div(-7, 2), -3);
    assert_eq!(int_rem(-7, 3), -1);
    assert_eq!(int_div(-50, 7) + int_rem(-50, 7), -8);
    assert_eq!(int_add(int_div(min, 7), int_rem(min, 7)), -306783380);
    assert_eq!(int_div(5, 0), 0);
    assert_eq!(int_rem(5, 0), 5);
    assert_eq!(int_rem(-5, 0), -5);
    assert_eq!(int_div(min, -1), min);
    assert_eq!(int_rem(min, -1), 0);
    assert_eq!(int_and(12, 10), 8);
    assert_eq!(int_or(12, 10), 14);
}

#[test]
fn int_prints_low_32_bits() {
    // tests/compiler/test_elanc.sh compiled_int_prints_low_32_bits
    assert_eq!(int_print(65536 * 65536), 0);
    assert_eq!(int_print(int_mul(65536, 65536)), 0);
    assert_eq!(int_print(1 << 31), -2147483648);
    let mut w = Writer::new();
    w.int(4294967296 + 7);
    w.ch(' ');
    w.ident(bool_print(true));
    assert_eq!(w.take(), "7 true");
}

#[test]
fn strings_as_the_interpreter() {
    assert_eq!(str_len("abc"), 3);
    assert_eq!(&*str_cat("ab", "c d"), "abc d");
    assert_eq!(str_cmp("a", "d"), -3);
    assert_eq!(str_cmp("da", "a"), 3);
    assert_eq!(str_cmp("ab", "ab"), 0);
    assert_eq!(str_cmp("ab", "a"), 98);
    assert_eq!(str_spn("aab", "a"), 2);
    assert_eq!(&*str_of_char(65), "A");
    assert_eq!(str_substr("abcd", 1, 2).as_deref(), Some("bc"));
    // out of range: stuck (the interpreter prints string_substr("abc",1,5))
    assert_eq!(str_substr("abc", 1, 5), None);
    assert_eq!(str_index("abc", 1), Some(98));
    assert_eq!(str_index("abc", 3), None);
    assert_eq!(str_modif("abc", 0, 120).as_deref(), Some("xbc"));
    assert_eq!(str_modif("abc", -1, 120), None);
    let mut w = Writer::new();
    w.string("a\"b");
    assert_eq!(w.take(), "\"a\\\"b\"");
    w.string_style = elan_runtime::print::StringStyle::Raw;
    w.string("abc d");
    assert_eq!(w.take(), "abc d");
}

// A builtin result sort that can be stuck: `f : (int) int` with no rule
// for some arguments, and `+` applied to a stuck argument.
#[derive(PartialEq, Eq, Hash, Debug)]
enum IntStuck {
    F(I),
    Plus(I, I),
}
elan_runtime::impl_sort!(IntStuck);
type I = Builtin<Int, elan_runtime::H<IntStuck>>;

fn plus(a: &I, b: &I) -> I {
    lift2(
        a,
        b,
        |x, y| Some(int_add(x, y)),
        || elan_runtime::H::new(IntStuck::Plus(a.clone(), b.clone())),
    )
}
fn f(a: &I) -> I {
    // [] f(n) => n*2 if n > 0 ; stuck otherwise
    lift1(
        a,
        |x| if x > 0 { Some(int_mul(x, 2)) } else { None },
        || elan_runtime::H::new(IntStuck::F(a.clone())),
    )
}

#[test]
fn stuck_builtin_terms() {
    on_thread(8 << 10, || {
        let one: I = Builtin::Val(1);
        assert_eq!(plus(&f(&one), &one), Builtin::Val(3));
        let st = plus(&f(&Builtin::Val(0)), &one);
        let Builtin::Stuck(s) = &st else { panic!() };
        let IntStuck::Plus(Builtin::Stuck(g), Builtin::Val(1)) = &**s else {
            panic!()
        };
        assert!(matches!(&**g, IntStuck::F(Builtin::Val(0))));
        assert_eq!(st.val(), None);
        // stuck terms are shared terms
        assert_eq!(plus(&f(&Builtin::Val(0)), &one), st);
    })
}
