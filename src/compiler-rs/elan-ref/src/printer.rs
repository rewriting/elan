//! Printer of a [`Program`] back to `.ref` text, in the layout of the
//! interpreter's writer (`ref/ref.cc`, `driver/ldmain.cc`) except for the
//! comments (`/*i name*/` before the rules, `/**/` in strategy calls), which
//! carry no information.

use crate::model::*;
use std::fmt::Write;

/// Prints a program as `.ref` text. Names are the Latin-1 decoding of the
/// file's bytes: use [`print_bytes`] to get the file back byte for byte.
pub fn print(p: &Program) -> String {
    let mut o = String::new();
    table(&mut o, "Identifiers", &p.identifiers);
    let sorts: Vec<TableEntry> = p
        .sorts
        .iter()
        .map(|s| TableEntry {
            index: s.index,
            name: s.name.clone(),
        })
        .collect();
    table(&mut o, "Sorts", &sorts);
    table(&mut o, "Modules", &p.modules);
    table(&mut o, "RuleNames", &p.rule_names);
    table(&mut o, "StrategyNames", &p.strategy_names);
    grammar_dump(&mut o, &p.grammars.both);
    grammar_dump(&mut o, &p.grammars.global_only);
    grammar_dump(&mut o, &p.grammars.top_only);
    for r in &p.rules {
        rule(&mut o, r);
        o.push_str(" end\n");
    }
    o.push_str("EndDef end\n");
    for s in &p.strategies {
        let _ = write!(o, "STRATEGY({},{},{},", s.name, s.sort, s.module);
        seq(&mut o, &s.body);
        o.push_str(") \nend\n");
    }
    o.push_str("EndDef end\n");
    let q = &p.query;
    let _ = write!(o, "QUERY({},{},{},", q.sort, q.result_sort, opt(q.strategy));
    term(&mut o, &q.start_with);
    o.push(',');
    term(&mut o, &q.check_with);
    o.push_str(") end \n");
    o
}

/// [`print`], encoded back to the bytes of the file (Latin-1).
pub fn print_bytes(p: &Program) -> Vec<u8> {
    crate::parser::latin1_bytes(&print(p))
}

/// Prints one term as the writer does.
pub fn print_term(t: &Term) -> String {
    let mut o = String::new();
    term(&mut o, t);
    o
}

/// Prints a strategy body as the writer does.
pub fn print_strategy(s: &[StrategyExpr]) -> String {
    let mut o = String::new();
    seq(&mut o, s);
    o
}

fn opt(i: Option<u32>) -> i64 {
    i.map_or(-1, i64::from)
}

fn table(o: &mut String, heading: &str, t: &[TableEntry]) {
    o.push_str(heading);
    o.push('\n');
    for e in t {
        let _ = writeln!(o, "{}:\"{}\".", e.index, e.name);
    }
    o.push_str("nil\nend\n\n");
}

fn grammar_dump(o: &mut String, d: &[SortGrammar]) {
    for g in d {
        let _ = writeln!(o, "GrammarForSort {}:{}:", g.sort, g.builtin as i32);
        for op in &g.operators {
            let _ = write!(
                o,
                "{}:{}:{}:{}:{}:{}:{}:",
                op.flag,
                op.symbol,
                op.priority,
                op.syntax.bits(),
                op.semantic,
                op.infos,
                op.defstrat.encode()
            );
            for l in &op.rhs {
                lexem(o, l);
                o.push('.');
            }
            o.push_str("nil:");
            for s in &op.local_strategies {
                let _ = write!(o, "{s}.");
            }
            o.push_str("nil.\n");
        }
        o.push_str("nil end\n");
    }
    o.push_str("EndDef end\n");
}

fn lexem(o: &mut String, l: &Lexem) {
    let _ = match l {
        Lexem::Type(n) => write!(o, "Type({n})"),
        Lexem::Char(n) => write!(o, "Char({n})"),
        Lexem::Num(n) => write!(o, "Num({n})"),
        Lexem::String(n) => write!(o, "String({n})"),
        Lexem::Ident(n) => write!(o, "Ident({n})"),
        Lexem::Blank => write!(o, "Blank"),
    };
}

fn term(o: &mut String, t: &Term) {
    match t {
        Term::Var { index, sort } => {
            let _ = write!(o, " VAR({index},{sort})");
        }
        Term::EVar { index, sort } => {
            let _ = write!(o, " EVAR({index},{sort})");
        }
        Term::Int(n) => {
            let _ = write!(o, "INT({n})");
        }
        Term::Ident(n) => {
            let _ = write!(o, "IDENT({n})");
        }
        Term::Str(codes) => {
            o.push_str("STRING(");
            codes_(o, codes);
            o.push(')');
        }
        Term::Fsym { args, symbol } => {
            o.push_str("FSYM(");
            let mut sep = ' ';
            for a in args {
                o.push(sep);
                sep = '.';
                term(o, a);
            }
            let _ = write!(o, "{sep}nil, {symbol})");
        }
    }
}

fn codes_(o: &mut String, codes: &[i32]) {
    for c in codes {
        let _ = write!(o, "{c}.");
    }
    o.push_str("nil");
}

fn rule(o: &mut String, r: &Rule) {
    o.push_str(match r.body {
        RuleBody::Switch(_) => "\nSWRULE(\n",
        RuleBody::Plain { .. } => "\nRULE(\n",
    });
    match r.name {
        Some(n) => {
            let _ = write!(o, "{n},");
        }
        None => o.push_str("-1,\n"),
    }
    let _ = write!(
        o,
        "{},{},{},{},{},",
        r.sort, r.module, r.infos, r.which_match, r.nvars
    );
    term(o, &r.lhs);
    o.push_str(",\n");
    match &r.body {
        RuleBody::Switch(s) => switch(o, s),
        RuleBody::Plain { rhs, wheres: w } => {
            term(o, rhs);
            o.push_str(",\n");
            wheres(o, w);
        }
    }
    o.push_str(")\n\n");
}

fn wheres(o: &mut String, ws: &[Where]) {
    for w in ws {
        where_(o, w);
        o.push_str(".\n");
    }
    o.push_str("nil");
}

fn where_(o: &mut String, w: &Where) {
    match w {
        Where::Try(branches) => {
            o.push_str("TRY(\n");
            for b in branches {
                wheres(o, b);
                o.push('.');
            }
            o.push_str("nil)\n");
        }
        Where::If(t) => {
            o.push_str("IFF(");
            term(o, t);
            o.push(')');
        }
        Where::Match {
            pattern,
            sort,
            strategy,
            rhs,
        } => {
            o.push_str("PWHERE(");
            term(o, pattern);
            let _ = write!(o, ",{sort},{},", opt(*strategy));
            term(o, rhs);
            o.push(')');
        }
        Where::Assign { lhs, strategy, rhs } => {
            o.push_str("WHERE(");
            // the writer prints the variable without the leading blank of terms
            let mut l = String::new();
            term(&mut l, lhs);
            o.push_str(l.trim_start());
            let _ = write!(o, ",{},", opt(*strategy));
            term(o, rhs);
            o.push(')');
        }
    }
}

fn switch(o: &mut String, s: &Switch) {
    match s {
        Switch::NoSwitch { wheres: w, result } => {
            o.push_str("NOSWITCH(");
            wheres(o, w);
            o.push(',');
            term(o, result);
            o.push(')');
        }
        Switch::Switch {
            wheres: w,
            branches,
        } => {
            o.push_str("SWITCH(");
            wheres(o, w);
            o.push(',');
            for b in branches {
                term(o, &b.test);
                o.push(',');
                switch(o, &b.body);
                o.push('.');
            }
            o.push_str("nil)");
        }
    }
}

fn seq(o: &mut String, s: &[StrategyExpr]) {
    for (i, e) in s.iter().enumerate() {
        if i > 0 {
            o.push_str(" ; ");
        }
        strat(o, e);
    }
}

fn strat(o: &mut String, s: &StrategyExpr) {
    use StrategyExpr as S;
    let rules = |o: &mut String, kw: &str, l: &[u32]| {
        let _ = write!(o, "{kw}(");
        for r in l {
            let _ = write!(o, "{r}.");
        }
        o.push_str("nil)");
    };
    let strats = |o: &mut String, kw: &str, l: &[StrategySeq]| {
        let _ = write!(o, "{kw}(");
        for (i, b) in l.iter().enumerate() {
            if i > 0 {
                o.push_str(" , ");
            }
            seq(o, b);
        }
        o.push(')');
    };
    let unary = |o: &mut String, kw: &str, b: &[StrategyExpr]| {
        let _ = write!(o, "{kw}(");
        seq(o, b);
        o.push(')');
    };
    match s {
        S::Id => o.push_str(" id "),
        S::Fail => o.push_str(" fail "),
        S::Meta => o.push_str("META"),
        S::OneRules(l) => rules(o, "one", l),
        S::DcRules(l) => rules(o, "dc", l),
        S::DkRules(l) => rules(o, "dk", l),
        S::NormIn(l) => rules(o, "normin", l),
        S::NormOut(l) => rules(o, "normout", l),
        S::OneStrats(l) => strats(o, "ONE", l),
        S::DcStrats(l) => strats(o, "DC", l),
        S::DkStrats(l) => strats(o, "DK", l),
        S::Repeat(b) => unary(o, "repeat*", b),
        S::Iterate(b) => unary(o, "iterate*", b),
        S::TAll(b) => unary(o, "tall", b),
        S::TOne(b) => unary(o, "tone", b),
        S::TSome(b) => unary(o, "tsome", b),
        S::Rewrite(b) => unary(o, "rewrite", b),
        S::Call(n) => {
            let _ = write!(o, "call({n})");
        }
        S::DcCall { name, max, sort } | S::DkCall { name, max, sort } => {
            o.push_str(if matches!(s, S::DcCall { .. }) {
                "dccall("
            } else {
                "dkcall("
            });
            codes_(o, name);
            let _ = write!(o, ",{max},{sort})");
        }
    }
}
