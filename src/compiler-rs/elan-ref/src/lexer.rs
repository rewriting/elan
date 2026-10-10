//! Lexer of `.ref` files.
//!
//! Tokens: integers (a `-` glued to the digits gives a negative integer; a
//! detached `-` is a punctuation token, since REM's `readInt` also accepts
//! `- 1`), quoted names, words (`RULE`, `nil`, `repeat*`...), and the
//! punctuation `: . , ( ) ;`. White space, `/* ... */` and `// ...` comments
//! are skipped (the export writes comments such as `/*1106 R:selection*/`
//! before the rules and `/**/` in strategy calls).
//!
//! The input is bytes: names are decoded as Latin-1 (one `char` per byte).

use std::fmt;

/// A token kind.
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Tok {
    /// An integer, possibly negative.
    Int(i64),
    /// A quoted name, without its quotes.
    Str(String),
    /// A word: letters, digits, `_`, optionally ending with `*`.
    Word(String),
    /// One of `: . , ( ) ;`, or a `-` not glued to its digits.
    Punct(char),
}

impl fmt::Display for Tok {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Tok::Int(n) => write!(f, "{n}"),
            Tok::Str(s) => write!(f, "\"{s}\""),
            Tok::Word(w) => write!(f, "{w}"),
            Tok::Punct(c) => write!(f, "{c}"),
        }
    }
}

/// A token and its position (1-based line and column).
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Token {
    pub tok: Tok,
    pub line: u32,
    pub col: u32,
}

/// A lexical or syntax error, with its position.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Error {
    pub line: u32,
    pub col: u32,
    pub message: String,
}

impl fmt::Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}:{}: {}", self.line, self.col, self.message)
    }
}

impl std::error::Error for Error {}

/// Splits `src` into tokens.
///
/// A quoted name ends at the first `"` followed by `.`: the writer does not
/// escape names, and a name is always followed by `.` (`n:"name".`), so this
/// reads back names containing `"` or `\` (REM's Java lexer would treat `\`
/// as an escape).
pub fn tokenize(src: &[u8]) -> Result<Vec<Token>, Error> {
    let mut toks = Vec::new();
    let (mut i, mut line, mut col) = (0usize, 1u32, 1u32);
    let n = src.len();
    // advance over src[i..j], updating line/col
    let step = |i: &mut usize, j: usize, line: &mut u32, col: &mut u32| {
        for &b in &src[*i..j] {
            if b == b'\n' {
                *line += 1;
                *col = 1;
            } else {
                *col += 1;
            }
        }
        *i = j;
    };
    while i < n {
        let b = src[i];
        let (l0, c0) = (line, col);
        let err = |m: String| Error {
            line: l0,
            col: c0,
            message: m,
        };
        if b.is_ascii_whitespace() {
            let j = i + 1;
            step(&mut i, j, &mut line, &mut col);
        } else if b == b'/' && src.get(i + 1) == Some(&b'*') {
            let end = find(src, i + 2, b"*/").ok_or_else(|| err("unterminated comment".into()))?;
            step(&mut i, end + 2, &mut line, &mut col);
        } else if b == b'/' && src.get(i + 1) == Some(&b'/') {
            let end = src[i..]
                .iter()
                .position(|&c| c == b'\n')
                .map_or(n, |p| i + p);
            step(&mut i, end, &mut line, &mut col);
        } else if b == b'"' {
            let mut j = i + 1;
            loop {
                match src.get(j) {
                    None => return Err(err("unterminated name".into())),
                    Some(b'"') if src.get(j + 1) == Some(&b'.') => break,
                    _ => j += 1,
                }
            }
            let s: String = src[i + 1..j].iter().map(|&c| c as char).collect();
            toks.push(Token {
                tok: Tok::Str(s),
                line: l0,
                col: c0,
            });
            step(&mut i, j + 1, &mut line, &mut col);
        } else if b.is_ascii_digit()
            || (b == b'-' && src.get(i + 1).map_or(false, u8::is_ascii_digit))
        {
            let mut j = i + 1;
            while j < n && src[j].is_ascii_digit() {
                j += 1;
            }
            let text = std::str::from_utf8(&src[i..j]).unwrap();
            let v = text
                .parse::<i64>()
                .map_err(|e| err(format!("bad integer {text}: {e}")))?;
            toks.push(Token {
                tok: Tok::Int(v),
                line: l0,
                col: c0,
            });
            step(&mut i, j, &mut line, &mut col);
        } else if b.is_ascii_alphabetic() || b == b'_' {
            let mut j = i + 1;
            while j < n && (src[j].is_ascii_alphanumeric() || src[j] == b'_') {
                j += 1;
            }
            if j < n && src[j] == b'*' {
                j += 1; // repeat*, iterate*
            }
            let w = std::str::from_utf8(&src[i..j]).unwrap().to_string();
            toks.push(Token {
                tok: Tok::Word(w),
                line: l0,
                col: c0,
            });
            step(&mut i, j, &mut line, &mut col);
        } else if b":.,();-".contains(&b) {
            toks.push(Token {
                tok: Tok::Punct(b as char),
                line: l0,
                col: c0,
            });
            let j = i + 1;
            step(&mut i, j, &mut line, &mut col);
        } else {
            return Err(err(format!("unexpected character {:?}", b as char)));
        }
    }
    Ok(toks)
}

fn find(hay: &[u8], from: usize, needle: &[u8]) -> Option<usize> {
    hay.get(from..)?
        .windows(needle.len())
        .position(|w| w == needle)
        .map(|p| from + p)
}
