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

/// A byte sink with the C printer's token-spacing state. The output is
/// bytes, as the C runtime's: builtin strings are written as they are, and
/// the identifiers of the `.ref` (Latin-1 characters) one byte per
/// character ([`Writer::ident`]).
pub struct Writer {
    buf: Vec<u8>,
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
            buf: Vec::new(),
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
            self.buf.push(b' ');
        }
        for c in s.chars() {
            self.push_char(c);
        }
        self.alnum = letter_or_digit(last);
    }

    /// A one-character token (C `CHARCODE`): `(`, `,`, `.`, ...
    pub fn ch(&mut self, c: char) {
        if self.alnum && letter_or_digit(c) {
            self.buf.push(b' ');
        }
        self.push_char(c);
        self.alnum = letter_or_digit(c);
    }

    /// A number token of a mixfix declaration (C `NUMCODE`): a space
    /// whenever the previous token ended alphanumerically.
    pub fn num_code(&mut self, n: i64) {
        if self.alnum {
            self.buf.push(b' ');
        }
        self.buf.extend_from_slice(n.to_string().as_bytes());
        self.alnum = true;
    }

    /// A builtin int (low 32 bits); the spacing flag is left unchanged.
    pub fn int(&mut self, n: Int) {
        self.buf
            .extend_from_slice(int_print(n).to_string().as_bytes());
    }

    /// A builtin string (style [`Writer::string_style`]); the spacing flag
    /// is left unchanged. Quoted strings escape `"` and `\` with `\`.
    /// The bytes are written as they are.
    pub fn string(&mut self, s: &[u8]) {
        match self.string_style {
            StringStyle::Raw => self.buf.extend_from_slice(s),
            StringStyle::Quoted => {
                self.buf.push(b'"');
                for &c in s {
                    if c == b'"' || c == b'\\' {
                        self.buf.push(b'\\');
                    }
                    self.buf.push(c);
                }
                self.buf.push(b'"');
            }
        }
    }

    /// Text outside the term syntax (`result = `): no spacing, flag kept.
    pub fn raw(&mut self, s: &str) {
        self.buf.extend_from_slice(s.as_bytes());
    }

    /// A character of an identifier or a token: one byte when it is
    /// Latin-1 (the `.ref` decoding), else its UTF-8 bytes.
    fn push_char(&mut self, c: char) {
        if (c as u32) < 256 {
            self.buf.push(c as u32 as u8);
        } else {
            let mut b = [0; 4];
            self.buf.extend_from_slice(c.encode_utf8(&mut b).as_bytes());
        }
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

    /// The bytes written since the last [`Writer::take`].
    pub fn as_bytes(&self) -> &[u8] {
        &self.buf
    }

    /// Take the bytes written (the spacing flag is kept).
    pub fn take(&mut self) -> Vec<u8> {
        std::mem::take(&mut self.buf)
    }

    /// Reset the spacing flag (a fresh printer).
    pub fn reset(&mut self) {
        self.alnum = false;
    }
}

/// A term as a string, with a fresh printer (tests, error messages; bytes
/// that are not UTF-8 are replaced, see [`Writer::take`] for the bytes).
pub fn to_text<P: Print + ?Sized>(t: &P) -> String {
    let mut w = Writer::new();
    t.print(&mut w);
    String::from_utf8_lossy(&w.take()).into_owned()
}
