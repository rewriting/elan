//! Printing terms the way the compiled C programs do
//! (`src/compiler/runtime/termOut.c`, `printTextFormTerm`).
//!
//! Term printing is generated per sort ([`Print`]): for each operator, the
//! tokens of its mixfix declaration in order, an argument printed by a
//! recursive call. The [`Writer`] reproduces the C token spacing: a space is
//! written before an identifier or a character token that starts with a
//! letter or a digit when the previous token ended with one (`alnumF`).
//! Integers and strings neither test nor set that flag (`termOut`), and the
//! flag is **not reset between results** (a global in C): after a result
//! ending with `o`, the next result `s(o)` prints as ` s(o)` (legacy bench,
//! e.g. `Compiler.4.0/Test` `enum`: `result =  s(o)`). Keep one `Writer` for
//! the whole run to get the same output.

use crate::builtin::{int_print, Int};

/// Generated per sort: print a term with the mixfix syntax of its
/// operators.
pub trait Print {
    fn print(&self, w: &mut Writer);
}

impl<T: crate::Sort + Print> Print for crate::H<T> {
    #[inline]
    fn print(&self, w: &mut Writer) {
        (**self).print(w)
    }
}

impl Print for Int {
    fn print(&self, w: &mut Writer) {
        w.int(*self)
    }
}

impl Print for bool {
    fn print(&self, w: &mut Writer) {
        w.ident(crate::builtin::bool_print(*self))
    }
}

impl Print for crate::builtin::Str {
    fn print(&self, w: &mut Writer) {
        w.string(self)
    }
}

/// A value of a builtin sort that can be stuck prints as its value or as
/// its stuck term.
impl<V: Print, S: Print> Print for crate::builtin::Builtin<V, S> {
    fn print(&self, w: &mut Writer) {
        match self {
            crate::builtin::Builtin::Val(v) => v.print(w),
            crate::builtin::Builtin::Stuck(s) => s.print(w),
        }
    }
}

/// How builtin strings are printed (see [`Writer::string`]).
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum StringStyle {
    /// `"abc"`, as the interpreter (default).
    Quoted,
    /// `abc`, as the compiled C runtime.
    Raw,
}

/// A text sink with the C printer's token-spacing state.
pub struct Writer {
    buf: String,
    alnum: bool,
    pub string_style: StringStyle,
}

#[inline]
fn letter_or_digit(c: char) -> bool {
    c.is_ascii_alphanumeric()
}

impl Default for Writer {
    fn default() -> Self {
        Writer::new()
    }
}

impl Writer {
    pub fn new() -> Writer {
        Writer {
            buf: String::new(),
            alnum: false,
            string_style: StringStyle::Quoted,
        }
    }

    /// An identifier token of a mixfix declaration (C `IDENTCODE`).
    pub fn ident(&mut self, s: &str) {
        let (Some(first), Some(last)) = (s.chars().next(), s.chars().last()) else {
            return;
        };
        if self.alnum && letter_or_digit(first) {
            self.buf.push(' ');
        }
        self.buf.push_str(s);
        self.alnum = letter_or_digit(last);
    }

    /// A one-character token (C `CHARCODE`): `(`, `,`, `.`, ...
    pub fn ch(&mut self, c: char) {
        if self.alnum && letter_or_digit(c) {
            self.buf.push(' ');
        }
        self.buf.push(c);
        self.alnum = letter_or_digit(c);
    }

    /// A number token of a mixfix declaration (C `NUMCODE`): a space
    /// whenever the previous token ended alphanumerically.
    pub fn num_code(&mut self, n: i64) {
        if self.alnum {
            self.buf.push(' ');
        }
        self.buf.push_str(&n.to_string());
        self.alnum = true;
    }

    /// A builtin int (low 32 bits); the spacing flag is left unchanged.
    pub fn int(&mut self, n: Int) {
        self.buf.push_str(&int_print(n).to_string());
    }

    /// A builtin string (style [`Writer::string_style`]); the spacing flag
    /// is left unchanged. Quoted strings escape `"` and `\` with `\`.
    pub fn string(&mut self, s: &str) {
        match self.string_style {
            StringStyle::Raw => self.buf.push_str(s),
            StringStyle::Quoted => {
                self.buf.push('"');
                for c in s.chars() {
                    if c == '"' || c == '\\' {
                        self.buf.push('\\');
                    }
                    self.buf.push(c);
                }
                self.buf.push('"');
            }
        }
    }

    /// Text outside the term syntax (`result = `): no spacing, flag kept.
    pub fn raw(&mut self, s: &str) {
        self.buf.push_str(s);
    }

    /// `name(a1,...,an)`: prefix layout with the C tokens.
    pub fn prefix(&mut self, name: &str, args: &[&dyn Print]) {
        self.ident(name);
        if args.is_empty() {
            return;
        }
        self.ch('(');
        for (i, a) in args.iter().enumerate() {
            if i > 0 {
                self.ch(',');
            }
            a.print(self);
        }
        self.ch(')');
    }

    /// The elements of an AC multiset, separated by `sep` tokens (for an
    /// operator `@ op @`: `a op b op c`).
    pub fn sep_list<P: Print>(&mut self, items: &[P], sep: &str) {
        for (i, a) in items.iter().enumerate() {
            if i > 0 {
                self.ident(sep);
            }
            a.print(self);
        }
    }

    /// The text written since the last [`Writer::take`].
    pub fn as_str(&self) -> &str {
        &self.buf
    }

    /// Take the text (the spacing flag is kept).
    pub fn take(&mut self) -> String {
        std::mem::take(&mut self.buf)
    }

    /// Reset the spacing flag (a fresh printer).
    pub fn reset(&mut self) {
        self.alnum = false;
    }
}

/// A term as a string, with a fresh printer (tests, error messages).
pub fn to_text<P: Print + ?Sized>(t: &P) -> String {
    let mut w = Writer::new();
    t.print(&mut w);
    w.take()
}
