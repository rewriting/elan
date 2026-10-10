//! Recursive-descent parser of `.ref` files into the [`crate::model`].
//!
//! It accepts every construct of REM's grammar (`REFParser.jj`) and of the
//! interpreter's importer (`parse/grammars/aterm.orig`), which adds `SWRULE`
//! (`SWITCH`/`NOSWITCH`), `String(n)` lexems and `dccall`/`dkcall`. Where the
//! grammars allow a choice that carries no information (a section ending with
//! `end` or `,`, a block or rule ending with `end` or `.`, the final `end`
//! after the query, `one(r)` for `one(r.nil)`), the model keeps nothing and
//! the printer writes the form the exporter writes.

use crate::lexer::{tokenize, Error, Tok, Token};
use crate::model::*;

/// Parses a whole `.ref` file.
pub fn parse_bytes(src: &[u8]) -> Result<Program, Error> {
    let toks = tokenize(src)?;
    let mut p = Parser { toks, pos: 0 };
    let prog = p.program()?;
    Ok(prog)
}

/// Parses a whole `.ref` file given as text (each `char` must be below 256,
/// i.e. the Latin-1 decoding of the file; ASCII files qualify).
pub fn parse(src: &str) -> Result<Program, Error> {
    parse_bytes(&latin1_bytes(src))
}

pub(crate) fn latin1_bytes(s: &str) -> Vec<u8> {
    s.chars()
        .map(|c| if (c as u32) < 256 { c as u8 } else { b'?' })
        .collect()
}

/// Parses a single term (`FSYM(...)`, `VAR(...)`...), for tests and tools.
pub fn parse_term(src: &str) -> Result<Term, Error> {
    let mut p = Parser {
        toks: tokenize(&latin1_bytes(src))?,
        pos: 0,
    };
    let t = p.term()?;
    p.eof()?;
    Ok(t)
}

/// Parses a strategy body `s1 ; s2 ; ...`, for tests and tools.
pub fn parse_strategy(src: &str) -> Result<StrategySeq, Error> {
    let mut p = Parser {
        toks: tokenize(&latin1_bytes(src))?,
        pos: 0,
    };
    let s = p.seq()?;
    p.eof()?;
    Ok(s)
}

/// Parses a list of conditions `w1. w2. ... nil`, for tests and tools.
pub fn parse_wheres(src: &str) -> Result<Vec<Where>, Error> {
    let mut p = Parser {
        toks: tokenize(&latin1_bytes(src))?,
        pos: 0,
    };
    let w = p.wheres()?;
    p.eof()?;
    Ok(w)
}

struct Parser {
    toks: Vec<Token>,
    pos: usize,
}

type R<T> = Result<T, Error>;

impl Parser {
    fn peek(&self) -> Option<&Tok> {
        self.toks.get(self.pos).map(|t| &t.tok)
    }
    fn peek_at(&self, k: usize) -> Option<&Tok> {
        self.toks.get(self.pos + k).map(|t| &t.tok)
    }
    fn err<T>(&self, msg: impl Into<String>) -> R<T> {
        let (line, col) = match self.toks.get(self.pos).or(self.toks.last()) {
            Some(t) => (t.line, t.col),
            None => (1, 1),
        };
        let found = match self.peek() {
            Some(t) => format!("`{t}`"),
            None => "end of file".to_string(),
        };
        Err(Error {
            line,
            col,
            message: format!("{}, found {}", msg.into(), found),
        })
    }
    fn is_word(&self, w: &str) -> bool {
        matches!(self.peek(), Some(Tok::Word(x)) if x == w)
    }
    fn is_punct(&self, c: char) -> bool {
        self.peek() == Some(&Tok::Punct(c))
    }
    fn word(&mut self, w: &str) -> R<()> {
        if self.is_word(w) {
            self.pos += 1;
            Ok(())
        } else {
            self.err(format!("expected `{w}`"))
        }
    }
    fn punct(&mut self, c: char) -> R<()> {
        if self.is_punct(c) {
            self.pos += 1;
            Ok(())
        } else {
            self.err(format!("expected `{c}`"))
        }
    }
    /// `KEYWORD (`: REM's tokens such as `RULE(` and `one(`.
    fn open(&mut self, w: &str) -> R<()> {
        self.word(w)?;
        self.punct('(')
    }
    fn eof(&self) -> R<()> {
        match self.peek() {
            None => Ok(()),
            Some(_) => self.err("expected end of file"),
        }
    }
    /// REM's `readInt`: `["-"] INTEGER`.
    fn int(&mut self) -> R<i64> {
        if self.is_punct('-') {
            if let Some(Tok::Int(n)) = self.peek_at(1) {
                let n = *n;
                if n >= 0 {
                    self.pos += 2;
                    return Ok(-n);
                }
            }
            return self.err("expected an integer");
        }
        match self.peek() {
            Some(Tok::Int(n)) => {
                let n = *n;
                self.pos += 1;
                Ok(n)
            }
            _ => self.err("expected an integer"),
        }
    }
    fn i32(&mut self) -> R<i32> {
        let n = self.int()?;
        match i32::try_from(n) {
            Ok(v) => Ok(v),
            Err(_) => {
                self.pos -= 1;
                self.err(format!("integer {n} out of range"))
            }
        }
    }
    /// A non-negative index.
    fn index(&mut self) -> R<u32> {
        let n = self.int()?;
        match u32::try_from(n) {
            Ok(v) => Ok(v),
            Err(_) => {
                self.pos -= 1;
                self.err(format!("expected a non-negative index, got {n}"))
            }
        }
    }
    /// An optional index: `-1` is `None`.
    fn opt_index(&mut self) -> R<Option<u32>> {
        if self.is_punct('-') || matches!(self.peek(), Some(Tok::Int(n)) if *n < 0) {
            let n = self.int()?;
            if n == -1 {
                return Ok(None);
            }
            self.pos -= 1;
            return self.err(format!("expected an index or -1, got {n}"));
        }
        self.index().map(Some)
    }
    /// `( end | , )` after a section.
    fn end_or_comma(&mut self) -> R<()> {
        if self.is_word("end") || self.is_punct(',') {
            self.pos += 1;
            Ok(())
        } else {
            self.err("expected `end` or `,`")
        }
    }
    /// `( end | . )` after a block, a rule or a strategy.
    fn end_or_dot(&mut self) -> R<()> {
        if self.is_word("end") || self.is_punct('.') {
            self.pos += 1;
            Ok(())
        } else {
            self.err("expected `end` or `.`")
        }
    }
    fn nil(&mut self) -> bool {
        if self.is_word("nil") {
            self.pos += 1;
            true
        } else {
            false
        }
    }

    // ------------------------------------------------------------ sections

    fn program(&mut self) -> R<Program> {
        self.word("Identifiers")?;
        let identifiers = self.table()?;
        self.end_or_comma()?;
        self.word("Sorts")?;
        let sort_table = self.table()?;
        self.end_or_comma()?;
        self.word("Modules")?;
        let modules = self.table()?;
        self.end_or_comma()?;
        self.word("RuleNames")?;
        let rule_names = self.table()?;
        self.end_or_comma()?;
        self.word("StrategyNames")?;
        let strategy_names = self.table()?;
        self.end_or_comma()?;
        let both = self.grammar_dump()?;
        self.end_or_comma()?;
        let global_only = self.grammar_dump()?;
        self.end_or_comma()?;
        let top_only = self.grammar_dump()?;
        self.end_or_comma()?;
        let mut rules = Vec::new();
        while !self.is_word("EndDef") {
            rules.push(self.rule()?);
        }
        self.word("EndDef")?;
        self.end_or_comma()?;
        let mut strategies = Vec::new();
        while !self.is_word("EndDef") {
            strategies.push(self.strategy_def()?);
        }
        self.word("EndDef")?;
        self.end_or_comma()?;
        let query = self.query()?;
        if self.is_word("end") {
            self.pos += 1;
        }
        self.eof()?;
        let grammars = Grammars {
            both,
            global_only,
            top_only,
        };
        let sorts = sort_table
            .into_iter()
            .map(|e| {
                let builtin = [&grammars.both, &grammars.global_only, &grammars.top_only]
                    .iter()
                    .any(|d| d.iter().any(|g| g.sort == e.index && g.builtin));
                Sort {
                    index: e.index,
                    name: e.name,
                    builtin,
                }
            })
            .collect();
        Ok(Program {
            identifiers,
            sorts,
            modules,
            rule_names,
            strategy_names,
            grammars,
            rules,
            strategies,
            query,
        })
    }

    /// `( n : "name" . )* nil`
    fn table(&mut self) -> R<Vec<TableEntry>> {
        let mut v = Vec::new();
        while !self.nil() {
            let index = self.index()?;
            self.punct(':')?;
            let name = match self.peek() {
                Some(Tok::Str(s)) => s.clone(),
                _ => return self.err("expected a quoted name"),
            };
            self.pos += 1;
            self.punct('.')?;
            v.push(TableEntry { index, name });
        }
        Ok(v)
    }

    /// `( GrammarForSort s : b : (op .)* nil (end|.) )* EndDef`
    fn grammar_dump(&mut self) -> R<Vec<SortGrammar>> {
        let mut v = Vec::new();
        while self.is_word("GrammarForSort") {
            self.pos += 1;
            let sort = self.index()?;
            self.punct(':')?;
            let b = self.int()?;
            self.punct(':')?;
            let mut operators = Vec::new();
            while !self.nil() {
                operators.push(self.operator()?);
                self.punct('.')?;
            }
            self.end_or_dot()?;
            v.push(SortGrammar {
                sort,
                builtin: b == 1,
                operators,
            });
        }
        self.word("EndDef")?;
        Ok(v)
    }

    fn operator(&mut self) -> R<Operator> {
        let mut f = [0i64; 7];
        for x in f.iter_mut() {
            *x = self.int()?;
            self.punct(':')?;
        }
        let small = |p: &Self, v: i64| -> R<i32> {
            i32::try_from(v).or_else(|_| p.err(format!("integer {v} out of range")))
        };
        let mut rhs = Vec::new();
        while !self.nil() {
            rhs.push(self.lexem()?);
            self.punct('.')?;
        }
        self.punct(':')?;
        let mut local_strategies = Vec::new();
        while !self.nil() {
            local_strategies.push(self.i32()?);
            self.punct('.')?;
        }
        Ok(Operator {
            flag: small(self, f[0])?,
            symbol: small(self, f[1])?,
            priority: small(self, f[2])?,
            syntax: SyntaxFlags(small(self, f[3])?),
            semantic: small(self, f[4])?,
            infos: small(self, f[5])?,
            defstrat: DefStrat::decode(f[6]),
            rhs,
            local_strategies,
        })
    }

    fn lexem(&mut self) -> R<Lexem> {
        if self.is_word("Blank") {
            self.pos += 1;
            return Ok(Lexem::Blank);
        }
        let w = match self.peek() {
            Some(Tok::Word(w)) => w.clone(),
            _ => return self.err("expected a lexem"),
        };
        let mk: fn(i64) -> Option<Lexem> = match w.as_str() {
            "Type" => |n| u32::try_from(n).ok().map(Lexem::Type),
            "Char" => |n| i32::try_from(n).ok().map(Lexem::Char),
            "Num" => |n| i32::try_from(n).ok().map(Lexem::Num),
            "String" => |n| i32::try_from(n).ok().map(Lexem::String),
            "Ident" => |n| u32::try_from(n).ok().map(Lexem::Ident),
            _ => return self.err("expected a lexem"),
        };
        self.open(&w)?;
        let n = self.int()?;
        let l = match mk(n) {
            Some(l) => l,
            None => {
                self.pos -= 1;
                return self.err(format!("bad {w} value {n}"));
            }
        };
        self.punct(')')?;
        Ok(l)
    }

    // ------------------------------------------------------------ terms

    pub(crate) fn term(&mut self) -> R<Term> {
        let w = match self.peek() {
            Some(Tok::Word(w)) => w.clone(),
            _ => return self.err("expected a term"),
        };
        match w.as_str() {
            "FSYM" => {
                self.open("FSYM")?;
                let mut args = Vec::new();
                while !self.nil() {
                    args.push(self.term()?);
                    self.punct('.')?;
                }
                self.punct(',')?;
                let symbol = self.i32()?;
                self.punct(')')?;
                Ok(Term::Fsym { args, symbol })
            }
            "VAR" | "EVAR" => {
                self.open(&w)?;
                let index = self.index()?;
                self.punct(',')?;
                let sort = self.index()?;
                self.punct(')')?;
                Ok(if w == "VAR" {
                    Term::Var { index, sort }
                } else {
                    Term::EVar { index, sort }
                })
            }
            "INT" => {
                self.open("INT")?;
                let n = self.int()?;
                self.punct(')')?;
                Ok(Term::Int(n))
            }
            "IDENT" => {
                self.open("IDENT")?;
                let n = self.index()?;
                self.punct(')')?;
                Ok(Term::Ident(n))
            }
            "STRING" => {
                self.open("STRING")?;
                let codes = self.codes()?;
                self.punct(')')?;
                Ok(Term::Str(codes))
            }
            _ => self.err("expected a term"),
        }
    }

    /// `( n . )* nil`: character codes.
    fn codes(&mut self) -> R<Vec<i32>> {
        let mut v = Vec::new();
        while !self.nil() {
            v.push(self.i32()?);
            self.punct('.')?;
        }
        Ok(v)
    }

    // ------------------------------------------------------------ rules

    fn rule(&mut self) -> R<Rule> {
        let sw = if self.is_word("SWRULE") {
            true
        } else if self.is_word("RULE") {
            false
        } else {
            return self.err("expected `RULE`, `SWRULE` or `EndDef`");
        };
        self.pos += 1;
        self.punct('(')?;
        let name = self.opt_index()?;
        self.punct(',')?;
        let sort = self.index()?;
        self.punct(',')?;
        let module = self.i32()?;
        self.punct(',')?;
        let infos = self.i32()?;
        self.punct(',')?;
        let which_match = self.i32()?;
        self.punct(',')?;
        let nvars = self.index()?;
        self.punct(',')?;
        let lhs = self.term()?;
        self.punct(',')?;
        let body = if sw {
            RuleBody::Switch(self.switch()?)
        } else {
            let rhs = self.term()?;
            self.punct(',')?;
            let wheres = self.wheres()?;
            RuleBody::Plain { rhs, wheres }
        };
        self.punct(')')?;
        self.end_or_dot()?;
        Ok(Rule {
            name,
            sort,
            module,
            infos,
            which_match,
            nvars,
            lhs,
            body,
        })
    }

    /// `( where . )* nil`
    fn wheres(&mut self) -> R<Vec<Where>> {
        let mut v = Vec::new();
        while !self.nil() {
            v.push(self.where_()?);
            self.punct('.')?;
        }
        Ok(v)
    }

    fn where_(&mut self) -> R<Where> {
        if self.is_word("IFF") {
            self.open("IFF")?;
            let t = self.term()?;
            self.punct(')')?;
            Ok(Where::If(t))
        } else if self.is_word("WHERE") {
            self.open("WHERE")?;
            let lhs = self.term()?;
            self.punct(',')?;
            let strategy = self.opt_index()?;
            self.punct(',')?;
            let rhs = self.term()?;
            self.punct(')')?;
            Ok(Where::Assign { lhs, strategy, rhs })
        } else if self.is_word("PWHERE") {
            self.open("PWHERE")?;
            let pattern = self.term()?;
            self.punct(',')?;
            let sort = self.index()?;
            self.punct(',')?;
            let strategy = self.opt_index()?;
            self.punct(',')?;
            let rhs = self.term()?;
            self.punct(')')?;
            Ok(Where::Match {
                pattern,
                sort,
                strategy,
                rhs,
            })
        } else if self.is_word("TRY") {
            self.open("TRY")?;
            let mut branches = Vec::new();
            // REM: ( LOOKAHEAD(2) wheres . )* nil ) -- an empty branch is `nil .`
            while !(self.is_word("nil") && self.peek_at(1) == Some(&Tok::Punct(')'))) {
                branches.push(self.wheres()?);
                self.punct('.')?;
            }
            self.word("nil")?;
            self.punct(')')?;
            Ok(Where::Try(branches))
        } else {
            self.err("expected `IFF`, `WHERE`, `PWHERE`, `TRY` or `nil`")
        }
    }

    fn switch(&mut self) -> R<Switch> {
        if self.is_word("NOSWITCH") {
            self.open("NOSWITCH")?;
            let wheres = self.wheres()?;
            self.punct(',')?;
            let result = self.term()?;
            self.punct(')')?;
            Ok(Switch::NoSwitch { wheres, result })
        } else if self.is_word("SWITCH") {
            self.open("SWITCH")?;
            let wheres = self.wheres()?;
            self.punct(',')?;
            let mut branches = Vec::new();
            while !self.nil() {
                let test = self.term()?;
                self.punct(',')?;
                let body = self.switch()?;
                self.punct('.')?;
                branches.push(Branch { test, body });
            }
            self.punct(')')?;
            Ok(Switch::Switch { wheres, branches })
        } else {
            self.err("expected `SWITCH` or `NOSWITCH`")
        }
    }

    // ------------------------------------------------------------ strategies

    fn strategy_def(&mut self) -> R<StrategyDef> {
        if !self.is_word("STRATEGY") {
            return self.err("expected `STRATEGY` or `EndDef`");
        }
        self.open("STRATEGY")?;
        let name = self.index()?;
        self.punct(',')?;
        let sort = self.index()?;
        self.punct(',')?;
        let module = self.i32()?;
        self.punct(',')?;
        let body = self.seq()?;
        self.punct(')')?;
        self.end_or_dot()?;
        Ok(StrategyDef {
            name,
            sort,
            module,
            body,
        })
    }

    /// `expr ( ; expr )*`
    fn seq(&mut self) -> R<StrategySeq> {
        let mut v = vec![self.strat()?];
        while self.is_punct(';') {
            self.pos += 1;
            v.push(self.strat()?);
        }
        Ok(v)
    }

    /// REM's `RuleNames`: `r [ . ( r . )* nil ]`, plus the empty list `nil`
    /// that the exporter writes when no rule of a label is visible.
    fn rule_list(&mut self) -> R<Vec<RuleNameIndex>> {
        let mut v = Vec::new();
        if self.nil() {
            return Ok(v);
        }
        v.push(self.index()?);
        if self.is_punct('.') {
            self.pos += 1;
            while !self.nil() {
                v.push(self.index()?);
                self.punct('.')?;
            }
        }
        Ok(v)
    }

    /// `body ( , body )*`
    fn seq_list(&mut self) -> R<Vec<StrategySeq>> {
        let mut v = vec![self.seq()?];
        while self.is_punct(',') {
            self.pos += 1;
            v.push(self.seq()?);
        }
        Ok(v)
    }

    fn strat(&mut self) -> R<StrategyExpr> {
        let w = match self.peek() {
            Some(Tok::Word(w)) => w.clone(),
            _ => return self.err("expected a strategy"),
        };
        use StrategyExpr as S;
        let simple = match w.as_str() {
            "id" => Some(S::Id),
            "fail" => Some(S::Fail),
            "META" => Some(S::Meta),
            _ => None,
        };
        if let Some(s) = simple {
            self.pos += 1;
            return Ok(s);
        }
        let rules: Option<fn(Vec<RuleNameIndex>) -> StrategyExpr> = match w.as_str() {
            "one" => Some(S::OneRules),
            "dc" => Some(S::DcRules),
            "dk" => Some(S::DkRules),
            "normin" => Some(S::NormIn),
            "normout" => Some(S::NormOut),
            _ => None,
        };
        let strats: Option<fn(Vec<StrategySeq>) -> StrategyExpr> = match w.as_str() {
            "ONE" => Some(S::OneStrats),
            "DC" => Some(S::DcStrats),
            "DK" => Some(S::DkStrats),
            _ => None,
        };
        let unary: Option<fn(StrategySeq) -> StrategyExpr> = match w.as_str() {
            "repeat*" => Some(S::Repeat),
            "iterate*" => Some(S::Iterate),
            "tall" => Some(S::TAll),
            "tone" => Some(S::TOne),
            "tsome" => Some(S::TSome),
            "rewrite" => Some(S::Rewrite),
            _ => None,
        };
        let known = rules.is_some()
            || strats.is_some()
            || unary.is_some()
            || matches!(w.as_str(), "call" | "dccall" | "dkcall");
        if !known {
            return self.err("expected a strategy");
        }
        self.open(&w)?;
        let s = if let Some(f) = rules {
            f(self.rule_list()?)
        } else if let Some(f) = strats {
            f(self.seq_list()?)
        } else if let Some(f) = unary {
            f(self.seq()?)
        } else if w == "call" {
            S::Call(self.index()?)
        } else {
            let name = self.codes()?;
            self.punct(',')?;
            let max = self.i32()?;
            self.punct(',')?;
            let sort = self.index()?;
            if w == "dccall" {
                S::DcCall { name, max, sort }
            } else {
                S::DkCall { name, max, sort }
            }
        };
        self.punct(')')?;
        Ok(s)
    }

    // ------------------------------------------------------------ query

    fn query(&mut self) -> R<Query> {
        self.open("QUERY")?;
        let sort = self.index()?;
        self.punct(',')?;
        let result_sort = self.index()?;
        self.punct(',')?;
        let strategy = self.opt_index()?;
        self.punct(',')?;
        let start_with = self.term()?;
        self.punct(',')?;
        let check_with = self.term()?;
        self.punct(')')?;
        Ok(Query {
            sort,
            result_sort,
            strategy,
            start_with,
            check_with,
        })
    }
}
