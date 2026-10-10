//! Printing and the main program's output.
mod common;
use common::*;
use elan_runtime::print::{to_text, Writer};
use elan_runtime::{step, Options, Session};

#[test]
fn mixfix_printing() {
    on_thread(8 << 10, || {
        assert_eq!(to_text(&nat(2)), "s(s(o))");
        let l = cons(4, &cons(7, &nil()));
        assert_eq!(to_text(&l), "4.7.nil");
        assert_eq!(to_text(&cons(65536 * 65536, &nil())), "0.nil");
        assert_eq!(
            to_text(&elan_runtime::H::new(Nat::Plus(nat(1), z()))),
            "s(o)+o"
        );
    })
}

#[test]
fn token_spacing_as_the_c_printer() {
    let mut w = Writer::new();
    w.ident("if");
    w.ident("x"); // alnum after alnum: a space
    w.ch('+');
    w.ident("y");
    w.int(3); // ints neither test nor set the flag
    w.ident("z");
    w.ch('(');
    w.num_code(2);
    w.ident("w");
    assert_eq!(w.take(), "if x+y3 z(2 w");
}

/// Exact output of the C main program (`-noInput -quiet`), including the
/// spacing flag kept across results (legacy bench `enum`: `result =  s(o)`).
#[test]
fn session_output_as_compiled_programs() {
    on_thread(8 << 10, || {
        let o = Options::parse(["-noInput", "-quiet"].map(String::from));
        assert!(o.no_input && o.quiet && !o.help);
        let mut s = Session::new(o, Vec::new());
        for i in 0..3 {
            step();
            s.result(&nat(i));
        }
        s.finish();
        let out = String::from_utf8(s.into_output()).unwrap();
        assert_eq!(
            out,
            "\nresult = o\n\nresult =  s(o)\n\nresult = s(s(o))\n\nrewrite_step = 3\n"
        );
    })
}

#[test]
fn session_without_quiet_prints_the_time() {
    on_thread(8 << 10, || {
        let mut s = Session::new(Options::parse(["-noInput".to_string()]), Vec::new());
        s.result(&nil());
        s.finish();
        let out = String::from_utf8(s.into_output()).unwrap();
        assert!(
            out.starts_with("\nresult = nil\n\nrewrite_step = 0\ntotal time    = "),
            "{out}"
        );
    })
}

#[test]
fn options() {
    let o = Options::parse(
        [
            "-quiet", "-trace", "3", "-unknown", "-h", "rest", "-noInput",
        ]
        .map(String::from),
    );
    assert!(o.quiet && o.help && !o.no_input);
    assert_eq!(o.rest, vec!["rest".to_string(), "-noInput".to_string()]);
    let o = Options::parse(["-noOutput".to_string()]);
    let mut s = Session::new(o, Vec::new());
    s.result(&5i64);
    assert_eq!(s.results(), 1);
    s.finish();
    assert!(String::from_utf8(s.into_output())
        .unwrap()
        .starts_with("\nrewrite_step = "));
}
