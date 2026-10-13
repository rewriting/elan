//! The lexems and the lexer of the query parser: REM's `lstream.cc` and
//! `ichstream.cc` (`src/compiler/earley`), as elanc-go's `earley/lex.go`.

use std::io::{BufRead, Write};

use super::Fatal;
use crate::ident::{ident_index, ident_name, Ident};

/// A lexem in REM's encoding (class lexem, `commondefs.h`): the order of
/// the lexems orders the lists of the parser, hence the list of the
/// expected lexems of an error message. A number is its value (an
/// unsigned 32-bit number: REM's int wraps above 2^31-1 and then stops
/// being a number lexem; here it stays one).
pub type Lexem = i64;

/// NOLEXEM: end of a rule, end of the input.
pub const NO_LEXEM: Lexem = -256;
/// BLANKLEXEM.
pub const BLANK_LEXEM: Lexem = -257;
/// IDENT: any identifier (standard rule).
pub const IDENT_LEXEM: Lexem = -259;
/// BOFIDENT: identifier n is BOFIDENT-n.
pub const BOF_IDENT: Lexem = -260;
/// MAXNOFIDENT, MAXNOFSTRING.
pub const MAX_NOF_IDENT: Lexem = 3000;
pub const MAX_NOF_STRING: usize = 10000;
/// STRING: any string.
pub const STRING_LEXEM: Lexem = BOF_IDENT - MAX_NOF_IDENT;
/// BOFSTRING: string i is BOFSTRING-i.
pub const BOF_STRING: Lexem = BOF_IDENT - MAX_NOF_IDENT - 1;
/// JUSTNUMBER: any number.
pub const JUST_NUMBER: Lexem = BOF_STRING - MAX_NOF_STRING as Lexem;
/// BOFTYPES: sort n is BOFTYPES-n.
pub const BOF_TYPES: Lexem = JUST_NUMBER - 1;

/// The lexem of identifier n (`cridlex`).
pub fn ident_lex(n: i32) -> Lexem {
    BOF_IDENT - n as Lexem
}

/// The lexem of sort n (`crtypelex`).
pub fn type_lex(n: i32) -> Lexem {
    BOF_TYPES - n as Lexem
}

/// The lexem of the character c (`crcharlex`).
pub fn char_lex(c: i32) -> Lexem {
    -(c as Lexem)
}

/// The lexem of the number n (`crnumlex`).
pub fn num_lex(n: u32) -> Lexem {
    n as Lexem
}

pub fn terminal(l: Lexem) -> bool {
    l > BOF_TYPES
}
pub fn nonterminal(l: Lexem) -> bool {
    l <= BOF_TYPES
}
pub fn is_end_of_stream(l: Lexem) -> bool {
    l == NO_LEXEM
}
pub fn is_blank(l: Lexem) -> bool {
    l == BLANK_LEXEM
}
pub fn is_real_id(l: Lexem) -> bool {
    l <= BOF_IDENT && l > BOF_IDENT - MAX_NOF_IDENT
}
pub fn is_real_string(l: Lexem) -> bool {
    l <= BOF_STRING && l > BOF_STRING - MAX_NOF_STRING as Lexem
}
pub fn is_real_num(l: Lexem) -> bool {
    l >= 0
}
pub fn idval(l: Lexem) -> i32 {
    (BOF_IDENT - l) as i32
}
pub fn typeval(l: Lexem) -> i32 {
    (BOF_TYPES - l) as i32
}
pub fn numval(l: Lexem) -> i32 {
    l as u32 as i32
}

/// The end of the input (`EOF`).
const EOF: i32 = -1;

/// A stream of characters with its position (`ichstream.cc`).
pub struct Ichstream<'r> {
    r: &'r mut dyn BufRead,
    /// The next character (EOF: -1).
    fchar: i32,
    name: String,
    line: usize,
    pos: usize,
}

impl<'r> Ichstream<'r> {
    pub fn new(r: &'r mut dyn BufRead, name: &str) -> Ichstream<'r> {
        let mut s = Ichstream {
            r,
            fchar: 1,
            name: name.to_string(),
            line: 0,
            pos: 0,
        };
        // commonopen
        s.ich();
        s
    }

    fn read_byte(&mut self) -> Option<u8> {
        let b = match self.r.fill_buf() {
            Ok(buf) if !buf.is_empty() => buf[0],
            _ => return None,
        };
        self.r.consume(1);
        Some(b)
    }

    /// The next character; the one after it is read.
    fn ich(&mut self) -> i32 {
        let c = self.fchar;
        if c != EOF {
            self.fchar = match self.read_byte() {
                Some(b) => b as i32,
                None => EOF,
            };
            if self.fchar == '\n' as i32 {
                self.line += 1;
                self.pos = 0;
            } else {
                self.pos += 1;
            }
        }
        c
    }

    /// The next character, which stays in the stream.
    fn fuch(&self) -> i32 {
        self.fchar
    }

    fn blankskip(&mut self) {
        while self.fchar <= ' ' as i32 && self.fchar != EOF {
            self.ich();
        }
    }
}

/// Identifier length limit (IDLEN) and string constant limit (STRLEN).
const ID_LEN: usize = 50;
const STR_LEN: usize = 1000;

fn isalpha(c: i32) -> bool {
    (b'a' as i32..=b'z' as i32).contains(&c) || (b'A' as i32..=b'Z' as i32).contains(&c)
}
fn isdigit(c: i32) -> bool {
    (b'0' as i32..=b'9' as i32).contains(&c)
}

/// The identifier lexem of a name (added to the table of identifiers
/// when new: `tabofident.addstr`); the bytes are Latin-1 characters.
fn ident_named(b: &[u8]) -> Lexem {
    let name: String = b.iter().map(|&c| c as char).collect();
    ident_lex(ident_index(&name).0)
}

/// The stream of lexems of the parser (`lstream.cc`), with one lexem of
/// lookahead (`flex`).
pub struct Lstream<'r> {
    flex: Lexem,
    istr: Ichstream<'r>,
    /// The string constants read (`stringconstants`).
    strings: Vec<Vec<u8>>,
    /// An inline definition was read (`lastinline != NULL`).
    inline: bool,
    /// The warnings (`sterr`).
    pub stderr: Vec<u8>,
}

impl<'r> Lstream<'r> {
    pub fn new(istr: Ichstream<'r>) -> Result<Lstream<'r>, Fatal> {
        let mut f = Lstream {
            flex: NO_LEXEM,
            istr,
            strings: Vec::new(),
            inline: false,
            stderr: Vec::new(),
        };
        f.ilex()?;
        Ok(f)
    }

    /// The next lexem, which stays in the stream.
    pub fn fulex(&self) -> Lexem {
        self.flex
    }

    /// The next lexem; the one after it is read (a lexical error is a
    /// [`Fatal`]).
    pub fn ilex(&mut self) -> Result<Lexem, Fatal> {
        let l = self.flex;
        self.flex = self.next_lexem()?;
        Ok(l)
    }

    fn next_lexem(&mut self) -> Result<Lexem, Fatal> {
        let s = &mut self.istr;
        s.blankskip();
        let mut c = s.ich();
        let mut fc = s.fuch();
        // comments and inlines
        while c == '/' as i32 && (fc == '*' as i32 || fc == '/' as i32 || fc == '#' as i32) {
            if fc == '/' as i32 {
                while fc != '\n' as i32 {
                    self.istr.ich();
                    fc = self.istr.fuch();
                    if fc == EOF {
                        return Err(self.fail("[lexan] comment throw end of input stream\n"));
                    }
                }
            } else if fc == '#' as i32 {
                if self.inline {
                    self.owarn("\ttwo succesives inline definitions,\n\tprevious one will be lost\n");
                }
                self.inline = true;
                self.istr.ich();
                c = self.istr.ich();
                fc = self.istr.fuch();
                while c != '#' as i32 || fc != '/' as i32 {
                    c = self.istr.ich();
                    fc = self.istr.fuch();
                    if fc == EOF {
                        return Err(self.fail("[lexan] inline throw end of input stream\n"));
                    }
                }
            } else {
                self.istr.ich();
                c = self.istr.ich();
                fc = self.istr.fuch();
                while c != '*' as i32 || fc != '/' as i32 {
                    c = self.istr.ich();
                    fc = self.istr.fuch();
                    if fc == EOF {
                        return Err(self.fail("[lexan] comment throw end of input stream\n"));
                    }
                }
            }
            self.istr.ich();
            self.istr.blankskip();
            c = self.istr.ich();
            fc = self.istr.fuch();
        }
        let s = &mut self.istr;
        if c == '"' as i32 {
            let mut b: Vec<u8> = Vec::new();
            c = s.ich();
            fc = s.fuch();
            while c != '"' as i32 {
                if c == '\\' as i32 && fc == 'n' as i32 {
                    b.push(b'\n');
                    s.ich();
                } else if c == '\\' as i32 && fc == 't' as i32 {
                    b.push(b'\t');
                    s.ich();
                } else if c == '\\' as i32 && fc == '\\' as i32 {
                    b.push(b'\\');
                    s.ich();
                } else if c == '\\' as i32 && fc == '"' as i32 {
                    b.push(b'"');
                    s.ich();
                } else {
                    b.push(c as u8);
                }
                if b.len() >= STR_LEN {
                    return Err(Fatal::new("\nstring constant too long\n"));
                }
                c = s.ich();
                fc = s.fuch();
            }
            // (mstrdup: a C string stops at a NUL byte)
            if let Some(i) = b.iter().position(|&x| x == 0) {
                b.truncate(i);
            }
            return self.crstringlex(b);
        }
        if c == '`' as i32 {
            c = s.ich();
            fc = s.fuch();
            let n = if c == '\\' as i32 && fc == 'n' as i32 {
                s.ich();
                '\n' as i32
            } else if c == '\\' as i32 && fc == 't' as i32 {
                s.ich();
                '\t' as i32
            } else if c == '\\' as i32 && fc == '\\' as i32 {
                s.ich();
                '\\' as i32
            } else if c == '\\' as i32 && fc == '"' as i32 {
                s.ich();
                '"' as i32
            } else {
                c
            };
            c = s.ich();
            if c != '`' as i32 {
                return Err(Fatal::new("\nwrong string constant\n"));
            }
            return Ok(num_lex(n as u32));
        }
        // BORO's hacks
        if c == '\\' as i32 {
            if fc == 'n' as i32 {
                s.ich();
                return Ok(ident_named(b"\n"));
            }
            if fc == 't' as i32 {
                s.ich();
                return Ok(ident_named(b"\t"));
            }
        }
        // identifier
        if isalpha(c) {
            let mut ide = vec![c as u8];
            while isalpha(fc) || isdigit(fc) || fc == '_' as i32 {
                let tc = s.ich();
                // (the first IDLEN-1 characters)
                if ide.len() < ID_LEN - 1 {
                    ide.push(tc as u8);
                }
                fc = s.fuch();
            }
            return Ok(ident_named(&ide));
        }
        // number: modulo 2^32
        if isdigit(c) {
            let mut u = (c - '0' as i32) as u32;
            while isdigit(fc) {
                c = s.ich();
                fc = s.fuch();
                u = u.wrapping_mul(10).wrapping_add((c - '0' as i32) as u32);
            }
            return Ok(num_lex(u));
        }
        if c == EOF {
            return Ok(NO_LEXEM);
        }
        Ok(char_lex(c))
    }

    /// The lexem of a new string constant.
    fn crstringlex(&mut self, s: Vec<u8>) -> Result<Lexem, Fatal> {
        if self.strings.len() >= MAX_NOF_STRING {
            return Err(Fatal::new("\ntoo many string constants\n"));
        }
        self.strings.push(s);
        Ok(BOF_STRING - (self.strings.len() as Lexem - 1))
    }

    /// The string of a string lexem.
    pub fn stringval(&self, l: Lexem) -> &[u8] {
        &self.strings[(BOF_STRING - l) as usize]
    }

    /// `oerr`'s line: `\n[error] :file=NAME\tline=L\tpos=P\n`.
    pub fn position(&self, kind: &str) -> String {
        format!(
            "\n[{kind}] :file={}\tline={}\tpos={}\n",
            self.istr.name, self.istr.line, self.istr.pos
        )
    }

    /// `oerr(msg)` then `failexit`.
    fn fail(&self, msg: &str) -> Fatal {
        Fatal::new(&format!("{}{msg}", self.position("error")))
    }

    /// `owarn(msg)` (warnings are always on in REM's runtime).
    fn owarn(&mut self, msg: &str) {
        let w = format!("{}{msg}", self.position("warning"));
        let _ = self.stderr.write_all(w.as_bytes());
    }

    /// The text of a lexem (`lexem::alfsy`).
    pub fn alfsy(&self, l: Lexem) -> String {
        if nonterminal(l) {
            format!("sort({})", typeval(l))
        } else if l == BLANK_LEXEM {
            "BLANKLEXEM".to_string()
        } else if l == NO_LEXEM {
            "ENDOFLSTREAM".to_string()
        } else if l == IDENT_LEXEM {
            "IDENTIFIER".to_string()
        } else if l == JUST_NUMBER {
            "NUMBER".to_string()
        } else if l >= 0 {
            l.to_string()
        } else if l == STRING_LEXEM {
            // (REM: tabofident.ide(3000), past its table)
            "STRING".to_string()
        } else if is_real_string(l) {
            quote(self.stringval(l))
        } else if l <= BOF_IDENT {
            ident_name(Ident(idval(l)))
        } else {
            ((-l) as u8 as char).to_string()
        }
    }

    /// `alfsy`, the characters (and numbers) 0 to 32 as `'\N'`.
    pub fn erralfsy(&self, l: Lexem) -> String {
        if (-32..=0).contains(&l) {
            format!("'\\{}'", -l)
        } else {
            self.alfsy(l)
        }
    }
}

/// A string in double quotes, as Go's `strconv.Quote` writes the usual
/// ones (`"` and `\` escaped, `\n`, `\t`; other control bytes `\xNN`).
fn quote(s: &[u8]) -> String {
    let mut q = String::from("\"");
    for &c in s {
        match c {
            b'"' => q.push_str("\\\""),
            b'\\' => q.push_str("\\\\"),
            b'\n' => q.push_str("\\n"),
            b'\t' => q.push_str("\\t"),
            0x20..=0x7e => q.push(c as char),
            _ => q.push_str(&format!("\\x{c:02x}")),
        }
    }
    q.push('"');
    q
}
