//! Unit tests of the lexer, parser and printer on small inputs, including the
//! constructs that no export of the bench contains (SWRULE, dccall, String
//! lexems, traversal strategies, negative integers, strings, `call` terms).

use elan_ref::lexer::{tokenize, Tok};
use elan_ref::*;

fn toks(s: &str) -> Vec<Tok> {
    tokenize(s.as_bytes())
        .unwrap()
        .into_iter()
        .map(|t| t.tok)
        .collect()
}

#[test]
fn lexer_skips_white_space_and_comments() {
    assert_eq!(
        toks("/*1106 R:selection*/\nRULE(\n-1, // a line comment\n 3)"),
        vec![
            Tok::Word("RULE".into()),
            Tok::Punct('('),
            Tok::Int(-1),
            Tok::Punct(','),
            Tok::Int(3),
            Tok::Punct(')')
        ]
    );
}

#[test]
fn lexer_words_numbers_names() {
    assert_eq!(
        toks("repeat*( iterate*(x_1 ; 12:\"a b\".- 4"),
        vec![
            Tok::Word("repeat*".into()),
            Tok::Punct('('),
            Tok::Word("iterate*".into()),
            Tok::Punct('('),
            Tok::Word("x_1".into()),
            Tok::Punct(';'),
            Tok::Int(12),
            Tok::Punct(':'),
            Tok::Str("a b".into()),
            Tok::Punct('.'),
            Tok::Punct('-'),
            Tok::Int(4),
        ]
    );
}

#[test]
fn lexer_names_are_verbatim() {
    // names are not escaped by the writer: a quote, a backslash, a newline
    assert_eq!(toks("1:\"\"\".2:\"\\\".3:\"\n\"."), {
        let s = |x: &str| Tok::Str(x.into());
        vec![
            Tok::Int(1),
            Tok::Punct(':'),
            s("\""),
            Tok::Punct('.'),
            Tok::Int(2),
            Tok::Punct(':'),
            s("\\"),
            Tok::Punct('.'),
            Tok::Int(3),
            Tok::Punct(':'),
            s("\n"),
            Tok::Punct('.'),
        ]
    });
    // Latin-1 bytes are kept, one char per byte
    let t = tokenize(b"1:\"\xe9t\xe9\".").unwrap();
    assert_eq!(t[2].tok, Tok::Str("\u{e9}t\u{e9}".into()));
}

#[test]
fn lexer_errors_have_positions() {
    let e = tokenize(b"RULE(\n  @").unwrap_err();
    assert_eq!((e.line, e.col), (2, 3));
    assert!(tokenize(b"/* open").is_err());
    assert!(tokenize(b"1:\"open").is_err());
}

#[test]
fn terms() {
    let t = parse_term(
        "FSYM( VAR(0,440).INT(-3).IDENT(97).STRING(104.-23.nil).FSYM( nil, 300).nil, 301)",
    )
    .unwrap();
    assert_eq!(
        t,
        Term::Fsym {
            args: vec![
                Term::Var {
                    index: 0,
                    sort: 440
                },
                Term::Int(-3),
                Term::Ident(97),
                Term::Str(vec![104, -23]),
                Term::Fsym {
                    args: vec![],
                    symbol: 300
                },
            ],
            symbol: 301
        }
    );
    // as the writer: a blank after `FSYM(`, then the blank that starts a VAR
    assert_eq!(
        print_term(&t),
        "FSYM(  VAR(0,440).INT(-3).IDENT(97).STRING(104.-23.nil).FSYM( nil, 300).nil, 301)"
    );
    assert_eq!(
        parse_term(" EVAR(2,7)").unwrap(),
        Term::EVar { index: 2, sort: 7 }
    );
    assert_eq!(parse_term("STRING(nil)").unwrap(), Term::Str(vec![]));
}

#[test]
fn call_term_of_cexport() {
    // --cexport writes a strategy call as FSYM(/**/INT(s).nil,144)
    let t = parse_term("FSYM(/**/INT(377).nil,144)").unwrap();
    assert_eq!(
        t,
        Term::Fsym {
            args: vec![Term::Int(377)],
            symbol: 144
        }
    );
}

#[test]
fn bad_terms() {
    assert!(parse_term("FSYM( nil 300)").is_err());
    assert!(parse_term("VAR(-1,3)").is_err());
    assert!(parse_term("FOO(1)").is_err());
    let e = parse_term("FSYM( VAR(0,1).\n nil, x)").unwrap_err();
    assert_eq!(e.line, 2, "{e}");
}

#[test]
fn wheres() {
    let w = parse_wheres(
        "IFF(FSYM( nil, 300)).\nWHERE(VAR(1,440),-1, VAR(0,440)).\nPWHERE(FSYM( VAR(2,5).nil, 301),5,140, VAR(0,5)).\n\
         TRY(\nIFF(INT(1)).\nnil.nil.nil)\n.\nnil",
    )
    .unwrap();
    assert_eq!(
        w,
        vec![
            Where::If(Term::Fsym {
                args: vec![],
                symbol: 300
            }),
            Where::Assign {
                lhs: Term::Var {
                    index: 1,
                    sort: 440
                },
                strategy: None,
                rhs: Term::Var {
                    index: 0,
                    sort: 440
                }
            },
            Where::Match {
                pattern: Term::Fsym {
                    args: vec![Term::Var { index: 2, sort: 5 }],
                    symbol: 301
                },
                sort: 5,
                strategy: Some(140),
                rhs: Term::Var { index: 0, sort: 5 }
            },
            Where::Try(vec![vec![Where::If(Term::Int(1))], vec![]]),
        ]
    );
    assert!(parse_wheres("WHERE(VAR(1,440),-2, VAR(0,440)).nil").is_err());
}

#[test]
fn strategies() {
    use StrategyExpr as S;
    let s = parse_strategy(
        "one(5.7.nil) ; dc(nil) ; dk(9) ; normin(1.nil) ; normout(2.nil) ; \
         ONE( id  , fail  ; META) ; DC(call(3)) ; DK(dk(4.nil)) ; repeat*(iterate*(tall(tone(tsome(rewrite( id )))))) ; \
         dccall(97.98.nil,4,440) ; dkcall(nil,0,1)",
    )
    .unwrap();
    assert_eq!(
        s,
        vec![
            S::OneRules(vec![5, 7]),
            S::DcRules(vec![]),
            S::DkRules(vec![9]), // REM's short form for a single rule
            S::NormIn(vec![1]),
            S::NormOut(vec![2]),
            S::OneStrats(vec![vec![S::Id], vec![S::Fail, S::Meta]]),
            S::DcStrats(vec![vec![S::Call(3)]]),
            S::DkStrats(vec![vec![S::DkRules(vec![4])]]),
            S::Repeat(vec![S::Iterate(vec![S::TAll(vec![S::TOne(vec![
                S::TSome(vec![S::Rewrite(vec![S::Id])])
            ])])])]),
            S::DcCall {
                name: vec![97, 98],
                max: 4,
                sort: 440
            },
            S::DkCall {
                name: vec![],
                max: 0,
                sort: 1
            },
        ]
    );
    // printed as the writer does, the short form `dk(9)` becomes `dk(9.nil)`
    let printed = print_strategy(&s);
    assert!(
        printed.starts_with("one(5.7.nil) ; dc(nil) ; dk(9.nil) ; "),
        "{printed}"
    );
    assert!(printed.contains("ONE( id  ,  fail  ; META)"), "{printed}");
    assert_eq!(parse_strategy(&printed).unwrap(), s);
    assert!(parse_strategy("repeat(id)").is_err());
    assert!(parse_strategy("one()").is_err());
}

#[test]
fn defstrat_packing() {
    // FSYM_FLAG(300,301), LAB_FLAG(302,5), DSTR_FLAG(7,8), APPLY_FLAG(9)
    let cases = [
        (
            0x100_0000 | 300 << 12 | 301,
            DefStrat::Fsym { f1: 300, f2: 301 },
        ),
        (0x200_0000 | 302 << 12 | 5, DefStrat::Lab { f: 302, lab: 5 }),
        (0x400_0000 | 7 << 12 | 8, DefStrat::Dstr { f: 7, lab: 8 }),
        (0x800_0000 | 9, DefStrat::Apply { x: 9 }),
        (0, DefStrat::None),
    ];
    for (w, d) in cases {
        assert_eq!(DefStrat::decode(w), d);
        assert_eq!(d.encode(), w);
    }
    // FSYM_FLAG(-1,5): the shifted -1 overwrites the kind bits, kept raw
    let w = (0x100_0000u32 | (u32::MAX << 12).wrapping_add(5)) as i32 as i64;
    assert_eq!(DefStrat::decode(w), DefStrat::Raw(w));
    assert_eq!(DefStrat::decode(w).encode(), w);
}

#[test]
fn syntax_flags() {
    let f = SyntaxFlags(8 + 2);
    assert!(f.printable() && f.right_assoc() && !f.left_assoc() && !f.builtin_strategy());
    assert!(SyntaxFlags(4).builtin_strategy());
}

/// A small program with every section and the constructs of the
/// interpreter's importer that REM does not read.
const SMALL: &str = r#"Identifiers
97:"a".
nil
end

Sorts
32:"ident".
440:"term".
nil
end

Modules
68:"m".
nil
end

RuleNames
1555:"r0:term/m!GL".
nil
end

StrategyNames
376:"s0:term/m".
nil
end

GrammarForSort 32:1:
3:5:0:8:0:0:0:Ident(97).nil:nil.
nil end
GrammarForSort 440:0:
3:300:0:8:0:0:16789805:Ident(97).Blank.Char(40).Num(1).String(2).Type(32).nil:1.2.nil.
3:301:10:9:-12:2:0:Type(440).Char(43).Type(440).nil:nil.
nil end
EndDef end
GrammarForSort 440:0:
nil end
EndDef end
EndDef end
/*1555 r0:term*/
RULE(
1555,440,68,1,0,1, VAR(0,440),
FSYM( nil, 300),
IFF(FSYM( nil, 5)).
nil)

 end

SWRULE(
-1,
440,68,2,1,2,FSYM( VAR(0,440).VAR(1,440).nil, 301),
SWITCH(WHERE(VAR(0,440),376, VAR(1,440)).
nil,FSYM( nil, 5),NOSWITCH(nil, VAR(0,440)).INT(1),SWITCH(nil,nil).nil))

 end
EndDef end
STRATEGY(376,440,68,dk(1555.nil) ; repeat*( id ))
end
EndDef end
QUERY(440,440,376,FSYM( nil, 300),FSYM( nil, 5)) end
"#;

#[test]
fn small_program() {
    let p = parse(SMALL).unwrap();
    assert_eq!(p.identifier(97), Some("a"));
    assert!(p.sort(32).unwrap().builtin);
    assert!(!p.sort(440).unwrap().builtin);
    assert_eq!(p.grammars.both.len(), 2);
    assert_eq!(p.grammars.global_only.len(), 1);
    assert!(p.grammars.global_only[0].operators.is_empty());
    assert!(p.grammars.top_only.is_empty());
    let op = &p.grammars.both[1].operators[0];
    assert_eq!(op.defstrat, DefStrat::Fsym { f1: 3, f2: 301 });
    assert_eq!(
        op.rhs,
        vec![
            Lexem::Ident(97),
            Lexem::Blank,
            Lexem::Char(40),
            Lexem::Num(1),
            Lexem::String(2),
            Lexem::Type(32)
        ]
    );
    assert_eq!(op.local_strategies, vec![1, 2]);
    let ac = &p.grammars.both[1].operators[1];
    assert_eq!((ac.semantic, ac.infos), (-12, 2));
    assert!(ac.syntax.left_assoc() && ac.syntax.printable());
    assert_eq!(p.printing_form(301).unwrap().0.sort, 440);

    assert_eq!(p.rules.len(), 2);
    assert_eq!(p.rules[0].name, Some(1555));
    let sw = &p.rules[1];
    assert_eq!(
        (sw.name, sw.infos, sw.which_match, sw.nvars),
        (None, 2, 1, 2)
    );
    match &sw.body {
        RuleBody::Switch(Switch::Switch { wheres, branches }) => {
            assert_eq!(wheres.len(), 1);
            assert_eq!(branches.len(), 2);
            assert_eq!(
                branches[0].body,
                Switch::NoSwitch {
                    wheres: vec![],
                    result: Term::Var {
                        index: 0,
                        sort: 440
                    }
                }
            );
            assert_eq!(
                branches[1].body,
                Switch::Switch {
                    wheres: vec![],
                    branches: vec![]
                }
            );
        }
        b => panic!("{b:?}"),
    }
    assert_eq!(p.strategy(376).unwrap().body.len(), 2);
    assert_eq!(p.query.strategy, Some(376));
    assert_eq!(
        p.query.check_with,
        Term::Fsym {
            args: vec![],
            symbol: 5
        }
    );

    // printing: same tokens, same model
    let out = print(&p);
    assert_eq!(
        normalise::first_difference(SMALL.as_bytes(), out.as_bytes()),
        None
    );
    assert_eq!(parse(&out).unwrap(), p);
}

#[test]
fn rem_alternatives_are_accepted() {
    // REM accepts `,` after a section, `.` after a block, a rule or a strategy,
    // and no `end` after the query; the printer writes the exporter's form.
    let alt = SMALL
        .replacen("nil\nend\n\nSorts", "nil\n,\nSorts", 1)
        .replacen(
            "nil end\nGrammarForSort 440",
            "nil .\nGrammarForSort 440",
            1,
        )
        .replacen("nil)\n\n end\n\nSWRULE", "nil) .\nSWRULE", 1)
        .replacen(") \nend\nEndDef", ") .\nEndDef", 1)
        .replacen(") end \n", ")\n", 1);
    assert_ne!(alt, SMALL);
    assert_eq!(parse(&alt).unwrap(), parse(SMALL).unwrap());
}

#[test]
fn syntax_errors() {
    let missing_nil = SMALL.replacen("97:\"a\".\nnil", "97:\"a\".\n", 1);
    let e = parse(&missing_nil).unwrap_err();
    assert_eq!(e.line, 4, "{e}");
    assert!(parse(&SMALL.replacen("QUERY(", "QUERY ", 1)).is_err());
    assert!(parse(&format!("{SMALL} junk")).is_err());
    assert!(parse(&SMALL.replacen("EndDef end\nSTRATEGY", "STRATEGY", 1)).is_err());
}
