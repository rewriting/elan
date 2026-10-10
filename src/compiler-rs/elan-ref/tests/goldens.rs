//! Round trip of the golden exports `tests/golden/expected/*.ref` (`elan
//! --export`) and `*.cref` (`elan --cexport`): 27 programs, 54 files.

use elan_ref::{normalise, parse_bytes, print_bytes, DefStrat, Lexem, Program, RuleBody, Term};
use std::path::PathBuf;

fn golden_files() -> Vec<PathBuf> {
    let dir = PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("../../../tests/golden/expected");
    let mut v: Vec<PathBuf> = std::fs::read_dir(&dir)
        .unwrap_or_else(|e| panic!("{}: {e}", dir.display()))
        .map(|e| e.unwrap().path())
        .filter(|p| matches!(p.extension().and_then(|e| e.to_str()), Some("ref" | "cref")))
        .collect();
    v.sort();
    v
}

/// The file without its `/* */` comments.
fn strip_comments(src: &[u8]) -> Vec<u8> {
    let mut out = Vec::with_capacity(src.len());
    let mut i = 0;
    while i < src.len() {
        if src[i..].starts_with(b"/*") {
            let p = src[i + 2..]
                .windows(2)
                .position(|w| w == b"*/")
                .expect("unterminated comment");
            i += 2 + p + 2;
        } else {
            out.push(src[i]);
            i += 1;
        }
    }
    out
}

fn load(name: &str) -> Program {
    let p = PathBuf::from(env!("CARGO_MANIFEST_DIR"))
        .join("../../../tests/golden/expected")
        .join(name);
    parse_bytes(&std::fs::read(p).unwrap()).unwrap()
}

#[test]
fn all_goldens_round_trip() {
    let files = golden_files();
    assert_eq!(files.len(), 54, "27 programs, a .ref and a .cref each");
    let mut failures = Vec::new();
    for f in &files {
        let src = std::fs::read(f).unwrap();
        let prog = match parse_bytes(&src) {
            Ok(p) => p,
            Err(e) => {
                failures.push(format!("{}: parse error {e}", f.display()));
                continue;
            }
        };
        let out = print_bytes(&prog);
        // the contract: same tokens, white space and comments aside
        if let Some(d) = normalise::first_difference(&src, &out) {
            failures.push(format!("{}: {d}", f.display()));
            continue;
        }
        // stronger, true of the current writer: same bytes once comments are removed
        if strip_comments(&src) != out {
            failures.push(format!("{}: layout differs from the writer's", f.display()));
        }
        if parse_bytes(&out).as_ref() != Ok(&prog) {
            failures.push(format!("{}: reparse gives another model", f.display()));
        }
    }
    assert!(failures.is_empty(), "{}", failures.join("\n"));
}

#[test]
fn choice1_model() {
    let p = load("C40-choice1.cref");
    // tables
    assert_eq!(p.identifier(97), Some("a"));
    assert_eq!(p.module(68), Some("choice1"));
    assert_eq!(p.rule_name(1555), Some("r0:term/choice1!GL"));
    assert_eq!(p.strategy_name(376), Some("s0:term/choice1"));
    // built-in sorts come from the GrammarForSort flags
    let builtin: Vec<&str> = p
        .sorts
        .iter()
        .filter(|s| s.builtin)
        .map(|s| s.name.as_str())
        .collect();
    assert!(builtin.contains(&"builtinInt"), "{builtin:?}");
    assert!(!p.sort(440).unwrap().builtin, "term");
    assert_eq!(p.sort(440).unwrap().name, "term");
    // first grammar rule: 3:3:500:1:3:0:0:Type(58).Char(43).Type(58).nil:nil.
    let g = &p.grammars.both[0];
    assert_eq!((g.sort, g.builtin), (58, true));
    let op = &g.operators[0];
    assert_eq!(
        (op.flag, op.symbol, op.priority, op.semantic, op.infos),
        (3, 3, 500, 3, 0)
    );
    assert!(op.syntax.left_assoc() && !op.syntax.printable());
    assert_eq!(op.defstrat, DefStrat::None);
    assert_eq!(
        op.rhs,
        vec![Lexem::Type(58), Lexem::Char(43), Lexem::Type(58)]
    );
    // RULE(1555,440,68,1,1,2, VAR(0,440), VAR(1,440), WHERE(VAR(1,440),140, VAR(0,440)).nil)
    let r = p.rules.iter().find(|r| r.name == Some(1555)).unwrap();
    assert_eq!(
        (r.sort, r.module, r.infos, r.which_match, r.nvars),
        (440, 68, 1, 1, 2)
    );
    assert_eq!(
        r.lhs,
        Term::Var {
            index: 0,
            sort: 440
        }
    );
    match &r.body {
        RuleBody::Plain { rhs, wheres } => {
            assert_eq!(
                *rhs,
                Term::Var {
                    index: 1,
                    sort: 440
                }
            );
            assert_eq!(wheres.len(), 1);
        }
        b => panic!("{b:?}"),
    }
    assert!(!p.strategies.is_empty());
    assert!(p.printing_form(318).is_some());
}
