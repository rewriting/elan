//! Unit tests of the IR building (`lower`): sort representations,
//! totality, rule order in the decision trees, refusals.
//! Programs are built in memory with the `.ref` model.

use elan_ref::*;
use elanc_rs::ir::*;
use elanc_rs::lower::{lower, Error};

const INT: u32 = 58; // builtinInt
const BOOL: u32 = 428;
const NAT: u32 = 323;
const INTS: u32 = 331; // int (injection of builtinInt)
const SET: u32 = 400;

struct P(Program);

fn f(symbol: i32, args: Vec<Term>) -> Term {
    Term::Fsym { args, symbol }
}
fn v(index: u32, sort: u32) -> Term {
    Term::Var { index, sort }
}

impl P {
    fn new() -> P {
        let sort = |index, name: &str, builtin| Sort {
            index,
            name: name.into(),
            builtin,
        };
        let mut p = P(Program {
            identifiers: vec![],
            sorts: vec![
                sort(INT, "builtinInt", true),
                sort(INTS, "int", false),
                sort(NAT, "nat", false),
                sort(SET, "set", false),
                sort(BOOL, "bool", true),
            ],
            modules: vec![],
            rule_names: vec![],
            strategy_names: vec![],
            grammars: Grammars {
                both: vec![],
                global_only: vec![],
                top_only: vec![],
            },
            rules: vec![],
            strategies: vec![],
            query: Query {
                sort: NAT,
                result_sort: NAT,
                strategy: None,
                start_with: f(318, vec![]),
                check_with: f(1, vec![]),
            },
        });
        p.op(BOOL, 1, "true", &[], 0, 0);
        p.op(BOOL, 0, "false", &[], 0, 0);
        p
    }

    /// Declare `name(@,...)` (or the injection `@` when `name` is empty).
    fn op(&mut self, sort: u32, symbol: i32, name: &str, args: &[u32], semantic: i32, infos: i32) {
        let mut rhs = vec![];
        if !name.is_empty() {
            let id = self.0.identifiers.len() as u32 + 1;
            self.0.identifiers.push(TableEntry {
                index: id,
                name: name.into(),
            });
            rhs.push(Lexem::Ident(id));
        }
        if !name.is_empty() && !args.is_empty() {
            rhs.push(Lexem::Char('(' as i32));
        }
        for (i, a) in args.iter().enumerate() {
            if i > 0 {
                rhs.push(Lexem::Char(',' as i32));
            }
            rhs.push(Lexem::Type(*a));
        }
        if !name.is_empty() && !args.is_empty() {
            rhs.push(Lexem::Char(')' as i32));
        }
        let o = Operator {
            flag: 3,
            symbol,
            priority: 0,
            syntax: SyntaxFlags(8),
            semantic,
            infos,
            defstrat: DefStrat::None,
            rhs,
            local_strategies: vec![],
        };
        let b = &mut self.0.grammars.both;
        match b.iter_mut().find(|g| g.sort == sort) {
            Some(g) => g.operators.push(o),
            None => b.push(SortGrammar {
                sort,
                builtin: sort == INT || sort == BOOL,
                operators: vec![o],
            }),
        }
    }

    fn rule(&mut self, sort: u32, nvars: u32, lhs: Term, rhs: Term, wheres: Vec<Where>) {
        self.0.rules.push(Rule {
            name: None,
            sort,
            module: 1,
            infos: 1,
            which_match: 1,
            nvars,
            lhs,
            body: RuleBody::Plain { rhs, wheres },
        });
    }

    fn start(&mut self, t: Term, sort: u32) {
        self.0.query.start_with = t;
        self.0.query.result_sort = sort;
    }

    fn lower(&self) -> Result<Ir, Error> {
        lower(&self.0, "test")
    }
}

/// nat: o, s, plus with the two usual rules.
fn peano() -> P {
    let mut p = P::new();
    p.op(NAT, 318, "o", &[], 0, 0);
    p.op(NAT, 319, "s", &[NAT], 0, 0);
    p.op(NAT, 320, "plus", &[NAT, NAT], 0, 0);
    p.rule(
        NAT,
        1,
        f(320, vec![v(0, NAT), f(318, vec![])]),
        v(0, NAT),
        vec![],
    );
    p.rule(
        NAT,
        2,
        f(320, vec![v(0, NAT), f(319, vec![v(1, NAT)])]),
        f(319, vec![f(320, vec![v(0, NAT), v(1, NAT)])]),
        vec![],
    );
    p.start(f(320, vec![f(318, vec![]), f(318, vec![])]), NAT);
    p
}

fn op_named<'a>(ir: &'a Ir, name: &str) -> &'a OpIr {
    ir.ops.iter().find(|o| o.name == name).unwrap()
}

fn sort_named<'a>(ir: &'a Ir, name: &str) -> &'a SortIr {
    ir.sorts.iter().find(|s| s.name == name).unwrap()
}

#[test]
fn total_operator_has_no_variant() {
    let ir = peano().lower().unwrap();
    let plus = op_named(&ir, "plus");
    assert!(plus.kind.total(), "plus covers o and s");
    assert!(plus.variant.is_none());
    let Repr::Enum { variants } = &sort_named(&ir, "nat").repr else {
        panic!()
    };
    let names: Vec<&str> = variants.iter().map(|&o| ir.ops[o].name.as_str()).collect();
    assert_eq!(names, ["o", "s"]);
    // the start term's operators only: bool is not reachable
    assert!(ir.sorts.iter().all(|s| s.name != "bool"));
}

#[test]
fn recursive_operators_are_the_cycles_of_the_call_graph() {
    // plus calls itself; double(x) => plus(x, x) does not; ev(s(x)) =>
    // od(x), od(s(x)) => ev(x): a cycle of two
    let mut p = peano();
    p.op(NAT, 321, "double", &[NAT], 0, 0);
    p.op(NAT, 322, "ev", &[NAT], 0, 0);
    p.op(NAT, 324, "od", &[NAT], 0, 0);
    p.rule(
        NAT,
        3,
        f(321, vec![v(0, NAT)]),
        f(320, vec![v(0, NAT), v(0, NAT)]),
        vec![],
    );
    p.rule(
        NAT,
        4,
        f(322, vec![f(319, vec![v(0, NAT)])]),
        f(324, vec![v(0, NAT)]),
        vec![],
    );
    p.rule(
        NAT,
        5,
        f(324, vec![f(319, vec![v(0, NAT)])]),
        f(322, vec![v(0, NAT)]),
        vec![],
    );
    p.start(
        f(
            320,
            vec![f(321, vec![f(318, vec![])]), f(322, vec![f(318, vec![])])],
        ),
        NAT,
    );
    let ir = p.lower().unwrap();
    let rec = ir.recursive_ops();
    let is_rec = |name: &str| rec[ir.ops.iter().position(|o| o.name == name).unwrap()];
    assert!(is_rec("plus"));
    assert!(is_rec("ev") && is_rec("od"));
    assert!(!is_rec("double") && !is_rec("s") && !is_rec("o"));
}

#[test]
fn another_constructor_makes_an_operator_partial() {
    let mut p = peano();
    p.op(NAT, 321, "inf", &[], 0, 0);
    // inf is reachable from the start term
    p.start(f(320, vec![f(321, vec![]), f(318, vec![])]), NAT);
    let ir = p.lower().unwrap();
    let plus = op_named(&ir, "plus");
    assert!(!plus.kind.total(), "plus(x, inf) has no rule");
    assert_eq!(plus.variant.as_deref(), Some("Plus"));
}

#[test]
fn rules_are_tried_in_source_order() {
    // f(x) => o if c(x)  ;  f(s(x)) => x  ;  f(o) => s(o)
    let mut p = P::new();
    p.op(NAT, 318, "o", &[], 0, 0);
    p.op(NAT, 319, "s", &[NAT], 0, 0);
    p.op(NAT, 320, "f", &[NAT], 0, 0);
    p.op(BOOL, 321, "c", &[NAT], 0, 0);
    p.rule(
        NAT,
        1,
        f(320, vec![v(0, NAT)]),
        f(318, vec![]),
        vec![Where::If(f(321, vec![v(0, NAT)]))],
    );
    p.rule(
        NAT,
        1,
        f(320, vec![f(319, vec![v(0, NAT)])]),
        v(0, NAT),
        vec![],
    );
    p.rule(
        NAT,
        0,
        f(320, vec![f(318, vec![])]),
        f(319, vec![f(318, vec![])]),
        vec![],
    );
    p.start(f(320, vec![f(318, vec![])]), NAT);
    let ir = p.lower().unwrap();
    let OpKind::Defined { tree, total, .. } = &op_named(&ir, "f").kind else {
        panic!()
    };
    assert!(*total, "the unconditional rules cover o and s");
    // the conditional rule first, then a switch on the argument
    let Decision::Try { rule: 0, next, .. } = tree else {
        panic!("{tree:?}")
    };
    let Decision::Switch {
        occ,
        cases,
        default,
        ..
    } = &**next
    else {
        panic!()
    };
    assert_eq!(occ, &vec![0]);
    assert!(default.is_none(), "o and s: complete");
    let rules: Vec<RuleId> = cases
        .iter()
        .map(|(_, d)| match d {
            Decision::Try { rule, .. } => *rule,
            d => panic!("{d:?}"),
        })
        .collect();
    assert_eq!(rules, [1, 2], "cases in order of appearance");
}

#[test]
fn non_linear_rules_are_conditions() {
    // eq(x, x) => true ; eq(x, y) => false
    let mut p = peano();
    p.op(BOOL, 330, "eq", &[NAT, NAT], 0, 0);
    p.rule(
        BOOL,
        1,
        f(330, vec![v(0, NAT), v(0, NAT)]),
        f(1, vec![]),
        vec![],
    );
    p.rule(
        BOOL,
        2,
        f(330, vec![v(0, NAT), v(1, NAT)]),
        f(0, vec![]),
        vec![],
    );
    p.start(f(330, vec![f(318, vec![]), f(318, vec![])]), BOOL);
    let ir = p.lower().unwrap();
    let OpKind::Defined { rules, total, .. } = &op_named(&ir, "eq").kind else {
        panic!()
    };
    assert_eq!(rules[0].conds, vec![Cond::Same(0, 1)]);
    assert!(!rules[0].unconditional());
    assert!(*total, "the linear rule covers everything");
    // bool: native (true/false only)
    let b = sort_named(&ir, "bool");
    let Repr::Family(fid) = b.repr else { panic!() };
    assert!(ir.families[fid].stuck.is_none());
    assert_eq!(ir.families[fid].kind, BuiltinKind::Bool);
}

/// builtinInt and int with its injection and `+`, as in int.eln.
fn ints() -> P {
    let mut p = P::new();
    p.op(INT, 3, "", &[INT, INT], 3, 0); // @ + @ on builtinInt, code 3
    p.op(INTS, 300, "", &[INT], 0, 0); // @ : (builtinInt) int
    p.op(INTS, 301, "plus", &[INTS, INTS], 0, 0);
    // @(x) + @(y) => @(z) where z := () x + y
    p.rule(
        INTS,
        3,
        f(301, vec![f(300, vec![v(0, INT)]), f(300, vec![v(1, INT)])]),
        f(300, vec![v(2, INT)]),
        vec![Where::Assign {
            lhs: v(2, INT),
            strategy: None,
            rhs: f(3, vec![v(0, INT), v(1, INT)]),
        }],
    );
    p.op(INTS, 302, "g", &[NAT], 0, 0);
    p.op(NAT, 318, "o", &[], 0, 0);
    p.op(NAT, 319, "s", &[NAT], 0, 0);
    p.rule(
        INTS,
        0,
        f(302, vec![f(318, vec![])]),
        f(300, vec![Term::Int(0)]),
        vec![],
    );
    p
}

#[test]
fn int_is_native_when_nothing_can_be_stuck() {
    let mut p = ints();
    let one = f(300, vec![Term::Int(1)]);
    p.start(f(301, vec![one.clone(), one]), INTS);
    let ir = p.lower().unwrap();
    let int = sort_named(&ir, "int");
    let Repr::Family(fid) = int.repr else {
        panic!("{:?}", int.repr)
    };
    let fam = &ir.families[fid];
    assert_eq!(fam.kind, BuiltinKind::Int);
    assert_eq!(ir.sorts[fam.base].name, "builtinInt");
    assert!(fam.stuck.is_none());
    let inj = ir.ops.iter().find(|o| o.symbol == 300).unwrap();
    assert!(matches!(inj.kind, OpKind::Injection));
    assert!(op_named(&ir, "plus").kind.total());
}

#[test]
fn int_is_wrapped_when_an_operator_can_be_stuck() {
    // g(s(o)) has no rule: g(s(o)) + 1 stays
    let mut p = ints();
    let g = f(302, vec![f(319, vec![f(318, vec![])])]);
    p.start(f(301, vec![g, f(300, vec![Term::Int(1)])]), INTS);
    let ir = p.lower().unwrap();
    let Repr::Family(fid) = sort_named(&ir, "int").repr else {
        panic!()
    };
    let stuck = ir.families[fid].stuck.as_ref().expect("wrapped");
    let names: Vec<&str> = stuck
        .variants
        .iter()
        .map(|&o| ir.ops[o].name.as_str())
        .collect();
    assert_eq!(names, ["plus", "g"], "int + over a stuck g, and g");
    // builtinInt + only sees values (no builtinInt term is stuck)
    assert!(ir
        .ops
        .iter()
        .find(|o| o.symbol == 300)
        .unwrap()
        .variant
        .is_none());
    let b3 = ir.ops.iter().find(|o| o.symbol == 3).unwrap();
    assert!(b3.kind.total());
}

#[test]
fn unsupported_constructs_are_refused_when_reachable() {
    let mut p = peano();
    p.op(SET, 340, "u", &[SET, SET], 0, 2); // AC
    p.op(SET, 341, "single", &[NAT], 0, 0);
    // not reachable: fine
    assert!(p.lower().is_ok());
    p.start(
        f(
            340,
            vec![f(341, vec![f(318, vec![])]), f(341, vec![f(318, vec![])])],
        ),
        SET,
    );
    match p.lower() {
        Err(Error::Unsupported { stage: "S6c", what }) => assert!(what.contains("AC"), "{what}"),
        r => panic!("{r:?}"),
    }
    // a start term with the query (S6d): the query variable; every
    // operator of the query grammar is compiled
    let mut p = peano();
    p.start(f(319, vec![v(0, NAT)]), NAT);
    let ir = p.lower().unwrap();
    assert!(ir.start.uses_query && ir.start.vars.len() == 1);
    let id = |n: &str| ir.ops.iter().position(|o| o.name == n).unwrap();
    assert_eq!(
        ir.start.term,
        Expr::Op {
            op: id("s"),
            args: vec![Expr::Var(0)]
        }
    );
    // o, s and plus may be in the query: compiled; the rules of bool are
    // not (a query of sort nat has none)
    let rules = &ir.start.query.rules;
    for n in ["o", "s", "plus"] {
        assert!(rules.contains(&QueryRule::Op(id(n))), "{n}: {rules:?}");
    }
    assert!(rules.contains(&QueryRule::None));
    // a traversal (S6d); a strategy of the strategy interpreter strat[X]
    // is compiled as any other (S6b task 2)
    let mut p = peano();
    p.0.query.strategy = Some(3);
    p.strategy(3, vec![StrategyExpr::TAll(vec![StrategyExpr::Id])]);
    match p.lower() {
        Err(Error::Unsupported { stage: "S6d", what }) => assert!(what.contains("tall"), "{what}"),
        r => panic!("{r:?}"),
    }
    p.0.strategies[0].body = vec![StrategyExpr::Id];
    assert!(p.lower().is_ok());
    p.0.modules.push(TableEntry {
        index: 2,
        name: "strat[nat]".into(),
    });
    p.0.strategies[0].module = 2;
    assert!(p.lower().is_ok());
}

impl P {
    fn strategy(&mut self, name: u32, body: Vec<StrategyExpr>) {
        self.0.strategies.push(StrategyDef {
            name,
            sort: NAT,
            module: 1,
            body,
        });
        self.0.strategy_names.push(TableEntry {
            index: name,
            name: format!("st{name}:nat/m"),
        });
    }

    fn labelled(&mut self, label: u32, nvars: u32, lhs: Term, rhs: Term, wheres: Vec<Where>) {
        self.rule(NAT, nvars, lhs, rhs, wheres);
        self.0.rules.last_mut().unwrap().name = Some(label);
        if !self.0.rule_names.iter().any(|e| e.index == label) {
            self.0.rule_names.push(TableEntry {
                index: label,
                name: format!("r{label}:nat/m!GL"),
            });
        }
    }
}

/// Labels: all the rules of a label in source order, the left-hand side
/// as one pattern; labelled rules do not normalise; determinism.
#[test]
fn labels_and_strategies() {
    let mut p = peano();
    p.op(NAT, 321, "a", &[], 0, 0);
    // [r7] a => s(o)  [r8] x => s(x)  [r7] s(x) => x  [r7] a => o
    p.labelled(7, 0, f(321, vec![]), f(319, vec![f(318, vec![])]), vec![]);
    p.labelled(8, 1, v(0, NAT), f(319, vec![v(0, NAT)]), vec![]);
    p.labelled(7, 1, f(319, vec![v(0, NAT)]), v(0, NAT), vec![]);
    p.labelled(7, 0, f(321, vec![]), f(318, vec![]), vec![]);
    // st3 = dk(r8, r7) ; st4      st4 = repeat*(one(r7))
    // st5 = dc(r7) ; [r9] o => o where x := (st3) o
    p.strategy(
        3,
        vec![StrategyExpr::DkRules(vec![8, 7]), StrategyExpr::Call(4)],
    );
    p.strategy(
        4,
        vec![StrategyExpr::Repeat(vec![StrategyExpr::OneRules(vec![7])])],
    );
    p.strategy(5, vec![StrategyExpr::DcRules(vec![7, 9])]);
    p.labelled(
        9,
        1,
        f(318, vec![]),
        f(318, vec![]),
        vec![Where::Assign {
            lhs: v(0, NAT),
            strategy: Some(3),
            rhs: f(318, vec![]),
        }],
    );
    p.0.query.strategy = Some(5);
    p.start(f(321, vec![]), NAT);
    let ir = p.lower().unwrap();
    // a has labelled rules only: a constructor
    assert!(op_named(&ir, "a").variant.is_some());
    let names: Vec<&str> = ir.labels.iter().map(|l| l.name.as_str()).collect();
    assert_eq!(names, ["r7:nat/m!GL", "r8:nat/m!GL", "r9:nat/m!GL"]);
    let r7 = &ir.labels[0];
    assert_eq!(r7.rules.len(), 3);
    assert!(
        r7.rules[0].text.starts_with("[r7] a =>"),
        "{}",
        r7.rules[0].text
    );
    assert!(r7.rules.iter().all(|r| r.lhs.len() == 1));
    let names: Vec<&str> = ir.strategies.iter().map(|s| s.name.as_str()).collect();
    assert_eq!(names, ["st3:nat/m", "st4:nat/m", "st5:nat/m"]);
    assert_eq!(
        ir.strategies[0].body,
        vec![
            Strategy::Rules {
                choice: Choice::Dk,
                labels: vec![1, 0]
            },
            Strategy::Call(1)
        ]
    );
    assert_eq!(ir.start.strategy, Some(2));
    // st3: dk over 4 rules; st4: repeat* of one; st5: dc of r7 (3 rules
    // without where) and r9 (a where with st3)
    let det: Vec<bool> = ir.strategies.iter().map(|s| s.det).collect();
    assert_eq!(det, [false, true, false]);
    assert!(ir.labels[0].rules_det && !ir.labels[2].rules_det);
    assert!(matches!(
        ir.labels[2].rules[0].conds.as_slice(),
        [Cond::Apply {
            strategy: 0,
            pattern: Pattern::Var(_),
            ..
        }]
    ));
    // the emitted code: CPS functions, dk as ||, a loop for the
    // deterministic repeat*, each result of the start strategy printed
    let main = elanc_rs::emit::emit_crate(&ir, "/rt").main_rs;
    for needle in [
        "pub fn rl_r7_7(a0: &H<Nat>, k: K<H<Nat>>, first: bool) -> bool {",
        "(rl_r8_8(t, k2, false) || rl_r7_7(t, k2, false))",
        "// repeat* of a deterministic strategy: a loop",
        "if first && found { return false; }",
        "st_st5_5(&t, &mut |r: &H<Nat>| -> bool { s.result(r); false });",
    ] {
        assert!(main.contains(needle), "{needle} not in:\n{main}");
    }
}

/// Deterministic strategies calling themselves in tail position are loops
/// (spec D3); `iterate*` of a deterministic step is a loop.
#[test]
fn tail_recursive_strategies_are_loops() {
    use StrategyExpr::*;
    let mut p = peano();
    // [r7] s(x) => x   [r8] x => s(x)   [r8] s(x) => x
    p.labelled(7, 1, f(319, vec![v(0, NAT)]), v(0, NAT), vec![]);
    p.labelled(8, 1, v(0, NAT), f(319, vec![v(0, NAT)]), vec![]);
    p.labelled(8, 1, f(319, vec![v(0, NAT)]), v(0, NAT), vec![]);
    // st3 = DC(dc(r7) ; st3, id)        total, a tail call
    // st4 = DC(dk(r8) ; st4, id)        not deterministic: no loop
    // st5 = DC(dc(r7) ; st5, dc(r7))    st5 can fail: not in tail position
    // st6 = DC(dc(r7), dc(r7) ; st6)    last alternative: a tail call
    // st7 = ONE(dk(r8) ; st7, id)       dk(r8) before the call: no loop
    // st9 = iterate*(dc(r7))
    // st10 = DK(st3, st4, st5, st6, st7, st9) (start)
    p.strategy(
        3,
        vec![DcStrats(vec![vec![DcRules(vec![7]), Call(3)], vec![Id]])],
    );
    p.strategy(
        4,
        vec![DcStrats(vec![vec![DkRules(vec![8]), Call(4)], vec![Id]])],
    );
    p.strategy(
        5,
        vec![DcStrats(vec![
            vec![DcRules(vec![7]), Call(5)],
            vec![DcRules(vec![7])],
        ])],
    );
    p.strategy(
        6,
        vec![DcStrats(vec![
            vec![DcRules(vec![7])],
            vec![DcRules(vec![7]), Call(6)],
        ])],
    );
    p.strategy(
        7,
        vec![OneStrats(vec![vec![DkRules(vec![8]), Call(7)], vec![Id]])],
    );
    p.strategy(9, vec![Iterate(vec![DcRules(vec![7])])]);
    p.strategy(
        10,
        vec![DkStrats(
            [3, 4, 5, 6, 7, 9].iter().map(|&c| vec![Call(c)]).collect(),
        )],
    );
    p.0.query.strategy = Some(10);
    p.start(f(318, vec![]), NAT);
    let ir = p.lower().unwrap();
    let names: Vec<&str> = ir.strategies.iter().map(|s| s.name.as_str()).collect();
    assert_eq!(
        names,
        [
            "st3:nat/m",
            "st4:nat/m",
            "st5:nat/m",
            "st6:nat/m",
            "st7:nat/m",
            "st9:nat/m",
            "st10:nat/m"
        ]
    );
    let total = ir.total_strategies();
    assert_eq!(total, [true, true, false, false, true, true, true]);
    let tails: Vec<usize> = (0..ir.strategies.len())
        .map(|i| ir.tail_calls(i, &total).len())
        .collect();
    assert_eq!(tails, [1, 0, 0, 1, 0, 0, 0]);
    let main = elanc_rs::emit::emit_crate(&ir, "/rt").main_rs;
    assert_eq!(
        main.matches("(tail-recursive: a loop)").count(),
        2,
        "{main}"
    );
    for needle in [
        "{ tail.set(Some(r3.clone())); true }",
        "// iterate* of a deterministic strategy: a loop",
        // ONE passes on the stop of a tail call
        "None => stop",
        "pub fn st_st4_4(t: &H<Nat>, k: K<H<Nat>>) -> bool {\n    elan_runtime::guard();",
    ] {
        assert!(main.contains(needle), "{needle} not in:\n{main}");
    }
}

#[test]
fn printing_form_is_the_last_grammar_rule() {
    let mut p = peano();
    // an alias declared after: `@ ++ @` with the same symbol as plus
    p.0.grammars.both[1].operators.push(Operator {
        flag: 3,
        symbol: 320,
        priority: 0,
        syntax: SyntaxFlags(0),
        semantic: 0,
        infos: 0,
        defstrat: DefStrat::None,
        rhs: vec![Lexem::Type(NAT), Lexem::Char('+' as i32), Lexem::Type(NAT)],
        local_strategies: vec![],
    });
    let ir = p.lower().unwrap();
    assert_eq!(
        op_named(&ir, "plus").print,
        vec![PrintItem::Arg(0), PrintItem::Char('+'), PrintItem::Arg(1)]
    );
}

#[test]
fn a_rule_that_types_to_no_profile_is_an_error() {
    // plus(x, b) => x with b of sort bool: plus has no profile (nat bool)
    let mut p = peano();
    p.rule(
        NAT,
        2,
        f(320, vec![v(0, NAT), v(1, BOOL)]),
        v(0, NAT),
        vec![],
    );
    match p.lower() {
        Err(Error::Invalid(m)) => assert!(m.contains("cannot type") && m.contains("plus"), "{m}"),
        r => panic!("the rule must not be dropped silently: {r:?}"),
    }
}

/// A labelled rule whose left-hand side has an operator that no term of
/// the program contains is not compiled, and what only it uses is not
/// reached (strat[X]: the `[TONE]` rule and its traversal); it is compiled
/// when the operator is built. The codes of the strategy constructors of
/// strsig/strat (`code -180`..) make constructors, not builtins.
#[test]
fn labelled_rules_need_their_operators() {
    let mut p = peano();
    p.op(NAT, 321, "a", &[], 0, 0);
    p.op(NAT, 322, "b", &[NAT], -205, 0); // as tone(@): code -205
                                          // [r7] b(x) => x where y := (st4) x     [r7] x => s(x)
                                          // st3 = dk(r7)    st4 = tall(id)        start: (st3) a
    p.labelled(
        7,
        2,
        f(322, vec![v(0, NAT)]),
        v(0, NAT),
        vec![Where::Assign {
            lhs: v(1, NAT),
            strategy: Some(4),
            rhs: v(0, NAT),
        }],
    );
    p.labelled(7, 1, v(0, NAT), f(319, vec![v(0, NAT)]), vec![]);
    p.strategy(3, vec![StrategyExpr::DkRules(vec![7])]);
    p.strategy(4, vec![StrategyExpr::TAll(vec![StrategyExpr::Id])]);
    p.0.query.strategy = Some(3);
    p.start(f(321, vec![]), NAT);
    let ir = p.lower().unwrap();
    assert_eq!(ir.labels[0].rules.len(), 1, "{:?}", ir.labels[0].skipped);
    assert!(ir.labels[0].skipped[0].contains("never applies"));
    assert!(ir.ops.iter().all(|o| o.name != "b"));
    assert_eq!(ir.strategies.len(), 1);
    // b built by the start term: the rule and the traversal are reached
    p.start(f(322, vec![f(321, vec![])]), NAT);
    match p.lower() {
        Err(Error::Unsupported { stage: "S6d", what }) => assert!(what.contains("tall"), "{what}"),
        r => panic!("{r:?}"),
    }
    p.0.strategies[1].body = vec![StrategyExpr::Id];
    let ir = p.lower().unwrap();
    assert_eq!(ir.labels[0].rules.len(), 2);
    assert!(matches!(op_named(&ir, "b").kind, OpKind::Constructor));
}

/// The term builtins on sorts of terms (codes 16, 17) are total
/// operations compiled to walkers: one per pair of sorts that can contain
/// the searched term, the copied path rebuilt by the operators' functions.
#[test]
fn replace_and_occurs_walk_the_sorts_that_can_contain_the_term() {
    let mut p = peano();
    p.op(SET, 341, "single", &[NAT], 0, 0);
    p.op(SET, 342, "empty", &[], 0, 0);
    p.op(SET, 343, "replace", &[NAT, NAT, SET], 16, 0);
    p.op(BOOL, 344, "occurs", &[NAT, SET], 17, 0);
    p.op(SET, 345, "pair", &[SET, BOOL], 0, 0);
    let o = || f(318, vec![]);
    let so = || f(319, vec![o()]);
    p.start(
        f(
            345,
            vec![
                f(343, vec![o(), so(), f(341, vec![so()])]),
                f(344, vec![o(), f(342, vec![])]),
            ],
        ),
        SET,
    );
    let ir = p.lower().unwrap();
    for name in ["replace", "occurs"] {
        assert!(op_named(&ir, name).kind.total(), "{name}");
    }
    let main = elanc_rs::emit::emit_crate(&ir, "/rt").main_rs;
    for needle in [
        "tw_replace__Nat__Set(a2, a0, a1)",
        "pub fn tw_replace__Nat__Set(t: &H<Set>, a: &H<Nat>, b: &H<Nat>) -> H<Set> {",
        "let r0 = tw_replace__Nat__Nat(x0, a, b);",
        "return single_341(&r0);",
        "pub fn tw_replace__Nat__Nat(t: &H<Nat>, a: &H<Nat>, b: &H<Nat>) -> H<Nat> {\n    if t == a { return b.clone(); }",
        "Set::Single(x0) => {\n            tw_occurs__Nat__Nat(a, x0)",
        // a bool cannot contain a nat
        "Set::Pair(x0, x1) => {\n            tw_occurs__Nat__Set(a, x0)\n",
    ] {
        assert!(main.contains(needle), "{needle} not in:\n{main}");
    }
}
