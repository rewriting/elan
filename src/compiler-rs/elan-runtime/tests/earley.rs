//! The query parser (a port of elanc-go's earley package): the tests of
//! `src/compiler-go/elanrt/earley`, on the tables of examples/poly1
//! compiled by REM (`data/poly1.tables`) and REM's messages on the same
//! inputs (`data/poly1.rem.json`).

use elan_runtime::earley::{self, Builder, Error, RhsLexem, Rule, Tables};
use elan_runtime::{ident_index, init_idents};

/// The tables of examples/poly1 (its gram[], tabIdent and tabSort); the
/// table of identifiers is set.
fn poly1() -> Tables {
    let src = include_str!("data/poly1.tables");
    let mut idents: Vec<(i32, String)> = Vec::new();
    let mut sorts: Vec<(i32, &'static str)> = Vec::new();
    let mut rules = Vec::new();
    for line in src.lines() {
        let f: Vec<&str> = line.splitn(3, ' ').collect();
        match f[0] {
            "rule" => {
                let w: Vec<i32> = f[1].split(',').map(|x| x.parse().unwrap()).collect();
                let rhs: Vec<RhsLexem> = (0..w[4] as usize)
                    .map(|i| RhsLexem(w[5 + 2 * i] as u8, w[6 + 2 * i]))
                    .collect();
                rules.push(Rule {
                    info: w[1],
                    symbol: w[2],
                    sort: w[3],
                    rhs: Box::leak(rhs.into_boxed_slice()),
                });
            }
            "ident" => idents.push((f[1].parse().unwrap(), f[2].to_string())),
            "sort" => sorts.push((f[1].parse().unwrap(), Box::leak(f[2].to_string().into_boxed_str()))),
            _ => {}
        }
    }
    let entries: Vec<(i32, &str)> = idents.iter().map(|(i, n)| (*i, n.as_str())).collect();
    init_idents(&entries);
    tables(rules, sorts)
}

fn tables(rules: Vec<Rule>, sorts: Vec<(i32, &'static str)>) -> Tables {
    Tables {
        rules: Box::leak(rules.into_boxed_slice()),
        sort_names: Box::leak(sorts.into_boxed_slice()),
        intern_ident: 220,
        intern_int: 19,
        intern_string: 351,
    }
}

/// Builds the terms in prefix form: symbol(args).
struct Prefix(Tables);

impl Builder for Prefix {
    type T = String;
    fn op(&mut self, r: usize, a: Vec<String>) -> String {
        let mut s = self.0.rules[r].symbol.to_string();
        if !a.is_empty() {
            s += &format!("({})", a.join(","));
        }
        s
    }
    fn int(&mut self, n: i32) -> String {
        format!("#{n}")
    }
    fn ident(&mut self, i: i32) -> String {
        format!("id{i}")
    }
    fn str(&mut self, s: &[u8]) -> String {
        format!("{:?}", String::from_utf8_lossy(s))
    }
}

fn parse(tab: &Tables, sort: i32, src: &str) -> (Result<String, Error>, String) {
    let mut stderr = Vec::new();
    let mut r = src.as_bytes();
    let res = earley::parse(&mut r, "standard input", tab, sort, &mut stderr)
        .and_then(|tr| Ok(earley::build(&tr, &mut Prefix(*tab))?));
    (res, String::from_utf8(stderr).unwrap())
}

const POLY_SORT: i32 = 452;

/// The terms built (priorities, associativity, injections, overloading
/// resolved by priorities).
#[test]
fn parse_poly1() {
    let tab = poly1();
    // 318 X, 319 poly <- variable, 320 poly <- int, 321 +, 322 * (poly);
    // 300 int <- builtinInt, 301 302 303 + - * (int), 67 number
    for (src, want) in [
        ("X end", "319(318)"),
        ("X+X*X end", "321(319(318),322(319(318),319(318)))"),
        ("X*X+X end", "321(322(319(318),319(318)),319(318))"),
        // assocRight
        ("X+X+X end", "321(319(318),321(319(318),319(318)))"),
        // int's - is assocLeft
        ("3 - 2 - 1 end", "320(302(302(300(#3),300(#2)),300(#1)))"),
        ("deriv(X)", "323(319(318))"),
        ("X end junk", "319(318)"),
    ] {
        let (got, stderr) = parse(&tab, POLY_SORT, src);
        assert_eq!(got, Ok(want.to_string()), "{src}");
        assert_eq!(stderr, "", "{src}");
    }
}

/// The strings of a JSON array of objects with string and number fields
/// (data/poly1.rem.json): the objects as (key, value) lists.
fn json_objects(src: &str) -> Vec<Vec<(String, String)>> {
    let mut out = Vec::new();
    let mut cur: Vec<(String, String)> = Vec::new();
    let mut key: Option<String> = None;
    let mut it = src.chars().peekable();
    while let Some(c) = it.next() {
        match c {
            '{' => cur = Vec::new(),
            '}' => out.push(std::mem::take(&mut cur)),
            '"' => {
                let mut s = String::new();
                while let Some(c) = it.next() {
                    match c {
                        '"' => break,
                        '\\' => match it.next().unwrap() {
                            'n' => s.push('\n'),
                            't' => s.push('\t'),
                            'u' => {
                                let h: String = (0..4).map(|_| it.next().unwrap()).collect();
                                s.push(char::from_u32(u32::from_str_radix(&h, 16).unwrap()).unwrap());
                            }
                            c => s.push(c),
                        },
                        c => s.push(c),
                    }
                }
                match key.take() {
                    None => key = Some(s),
                    Some(k) => cur.push((k, s)),
                }
            }
            '0'..='9' | '-' => {
                let mut s = c.to_string();
                while matches!(it.peek(), Some('0'..='9')) {
                    s.push(it.next().unwrap());
                }
                cur.push((key.take().unwrap(), s));
            }
            _ => {}
        }
    }
    out
}

/// The messages (stderr) of REM's compiled poly1 on the same inputs, byte
/// for byte, and its statuses (an error: status 1).
#[test]
fn parse_poly1_as_rem() {
    let tab = poly1();
    let cases = json_objects(include_str!("data/poly1.rem.json"));
    assert!(cases.len() > 10);
    for c in cases {
        let get = |k: &str| c.iter().find(|(x, _)| x == k).unwrap().1.clone();
        let (input, status, mut want) = (get("input"), get("status"), get("stderr"));
        // rulings (module comment): REM reads past its arrays there
        if input == "deriv(X" {
            want = "\n[error] :file=standard input\tline=0\tpos=8\n[earley] syntax error on symbol 'ENDOFLSTREAM'\n\
                    \tone of ('+' '*' ')') expected\n\tin context deriv ( X  >>>ENDOFLSTREAM<<< ENDOFLSTREAM \n"
                .to_string();
        } else if input == "\"a\" end" {
            want = want.replace("(null)", "\"a\"");
        }
        let (got, stderr) = parse(&tab, POLY_SORT, &input);
        assert_eq!(stderr, want, "{input}");
        match got {
            Ok(_) => assert_eq!(status, "0", "{input}"),
            Err(Error::Syntax) => assert_ne!(status, "0", "{input}"),
            Err(e) => panic!("{input}: {e:?}"),
        }
    }
}

/// An ambiguity that the priorities do not solve: REM's warning, the
/// first parse chosen (1+2*3 of int injected into poly).
#[test]
fn ambiguity() {
    let (got, stderr) = parse(&poly1(), POLY_SORT, "1+2*3 end");
    assert_eq!(got, Ok("320(301(300(#1),303(300(#2),300(#3))))".to_string()));
    assert_eq!(
        stderr,
        "\n[warning]earley: ambiguity can't be solved in string:\n 1 + 2 * 3\nbetween operators :\n\
         \t @ \t : (int)poly\t pri 0 code 320;\n\t @ '+'  @ \t : (poly poly)poly\t assocRight \t pri 1 code 321;\n"
    );
}

/// A lexical error and a term too long stop with status 1.
#[test]
fn fatal() {
    let tab = poly1();
    match parse(&tab, POLY_SORT, "X // no end").0 {
        Err(Error::Fatal(f)) => assert!(f.status == 1 && f.msg.contains("comment throw end"), "{f:?}"),
        r => panic!("{r:?}"),
    }
    let long = "(".repeat(earley::MAX_LEXEMS) + " end";
    match parse(&tab, POLY_SORT, &long).0 {
        Err(Error::Fatal(f)) => assert_eq!(
            f.msg,
            format!(
                "term is longer then MAXLENNTERM={} lexems\n\tsorry, FATAL !!!\n",
                earley::MAX_LEXEMS
            )
        ),
        r => panic!("{r:?}"),
    }
}

/// A user rule whose code is one of the semantic actions of the parser
/// (67-70, 81, 82) is an operator like any other, the actions applying
/// only to the standard rules and to the `1:` rules of the .ref (s ::=
/// intern int, intern ident, intern string).
#[test]
fn user_builtin_codes() {
    init_idents(&[]);
    let id = |s: &str| ident_index(s).0;
    let (a, foo, bar) = (id("a"), id("foo"), id("bar"));
    let rhs = |v: Vec<RhsLexem>| -> &'static [RhsLexem] { Box::leak(v.into_boxed_slice()) };
    let r = |symbol, v| Rule { sort: 7, symbol, info: 0, rhs: rhs(v) };
    let rules = vec![
        r(500, vec![RhsLexem(0, a)]),
        r(70, vec![RhsLexem(0, foo), RhsLexem(2, '(' as i32), RhsLexem(1, 7), RhsLexem(2, ')' as i32)]),
        r(67, vec![RhsLexem(0, bar), RhsLexem(2, '(' as i32), RhsLexem(1, 7), RhsLexem(2, ')' as i32)]),
        r(67, vec![RhsLexem(1, 19)]),
    ];
    let tab = tables(rules, vec![(7, "s")]);
    for (src, want) in [
        ("foo(a) end", "70(500)"),
        ("bar(a) end", "67(500)"),
        ("bar(foo(3)) end", "67(70(#3))"),
    ] {
        assert_eq!(parse(&tab, 7, src).0, Ok(want.to_string()), "{src}");
    }
}

fn lexems(src: &str) -> Result<String, earley::Fatal> {
    let mut r = src.as_bytes();
    Ok(earley::lexems(&mut r)?.0.join("|"))
}

#[test]
fn lexemes_read() {
    init_idents(&[]);
    ident_index("end");
    for (src, want) in [
        ("f(x_1, y2) end", "f|(|x_1|,|y2|)|end"),
        ("2147483647 2147483648 4294967297", "2147483647|2147483648|1"),
        (r#""a\"b\\c\nd\te" "x\qy""#, r#""a\"b\\c\nd\te"|"x\\qy""#),
        ("a+-*(", "a|+|-|*|("),
        ("`a` `\\n`", "97|10"),
        ("a // comment\n b /* c\n */ c", "a|b|c"),
        ("0 x", "'\\0'|x"),
        ("\\n", "\n"),
    ] {
        assert_eq!(lexems(src), Ok(want.to_string()), "{src:?}");
    }
    assert_eq!(lexems(&"a".repeat(60)), Ok("a".repeat(49)));
    // bytes that are not ASCII letters are characters
    assert_eq!(earley::lexems(&mut "é".as_bytes()).unwrap().0, ["\u{c3}", "\u{a9}"]);
    assert_eq!(elan_runtime::ident_name(ident_index("x_1")), "x_1");
}

/// The messages and positions of REM (examples/poly1 compiled by elanc,
/// the same inputs).
#[test]
fn lexical_errors() {
    for (src, want) in [
        ("a // no newline", "\n[error] :file=standard input\tline=0\tpos=16\n[lexan] comment throw end of input stream\n"),
        ("/* no end", "\n[error] :file=standard input\tline=0\tpos=10\n[lexan] comment throw end of input stream\n"),
        ("\"no end", "\nstring constant too long\n"),
        ("`ab`", "\nwrong string constant\n"),
    ] {
        match lexems(src) {
            Err(f) => assert!(f.msg == want && f.status == 1, "{src:?}: {f:?}"),
            r => panic!("{src:?}: {r:?}"),
        }
    }
}
