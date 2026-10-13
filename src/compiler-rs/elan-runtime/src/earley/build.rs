//! The term of a derivation: REM's semantic actions (`esemact`,
//! `earley/runtimeInit.cc`) record the derivation in reverse order in
//! `earleyRes`/`earleyKind`/`earleyString`, which `computeTerm`
//! (`runtime/termIn.c`) reads back from the end. As elanc-go's
//! `earley/build.go`.

use super::lex::{idval, is_real_string, numval, Lexem};
use super::{Fatal, Grammar, IDENT_RULE, IDENT_TO_TERM, NUM_RULE, NUM_TO_TERM};
use super::{STRING_RULE, STRING_TO_TERM};

/// DEFAULTRULE: an entry of an operator (`res`: its symbol).
const DEFAULT_RULE: i32 = 0;

/// EARLEYSIZE, the size of the derivation.
pub const EARLEY_SIZE: usize = 100000;

/// The rule of an operator entry: its index in [`super::Tables::rules`]
/// (`None` for a standard rule) and its arity (REM keeps its symbol only).
#[derive(Clone, Copy, Debug)]
struct RuleRef {
    index: Option<usize>,
    arity: usize,
}

/// A derivation (`earleyRes`, `earleyKind`, `earleyString`, `earleyPos`).
#[derive(Default, Debug)]
pub struct Tree {
    res: Vec<i32>,
    kind: Vec<i32>,
    strs: Vec<Vec<u8>>,
    rules: Vec<Option<RuleRef>>,
    pos: usize,
    last_ident: i32,
    last_num: i32,
    last_str: Vec<u8>,
}

impl Tree {
    /// Make entry `pos` exist (the arrays of REM have EARLEYSIZE entries).
    fn at(&mut self) {
        while self.res.len() <= self.pos {
            self.res.push(0);
            self.kind.push(0);
            self.strs.push(Vec::new());
            self.rules.push(None);
        }
    }
}

impl<'r, 't> Grammar<'r, 't> {
    /// Record the semantic action of `rule`, `lex` being the lexem at the
    /// end of its derivation.
    pub(super) fn esemact(&mut self, rule: usize, lex: Lexem) -> Result<(), Fatal> {
        let r = &self.rules[rule];
        let tr = &mut self.tree;
        if tr.pos == EARLEY_SIZE {
            return Err(Fatal {
                msg: "[esemact] Not enough memory...\n".to_string(),
                status: 255,
            });
        }
        tr.at();
        let p = tr.pos;
        // an operator unless an action
        let num = if r.action { r.rulenumber } else { -1 };
        match num {
            IDENT_RULE => {
                tr.last_ident = idval(lex);
                tr.kind[p] = IDENT_RULE;
            }
            NUM_RULE => {
                tr.last_num = numval(lex);
                tr.kind[p] = NUM_RULE;
            }
            NUM_TO_TERM => {
                tr.res[p] = tr.last_num;
                tr.pos += 1;
            }
            IDENT_TO_TERM => {
                tr.res[p] = tr.last_ident;
                tr.pos += 1;
            }
            STRING_RULE => {
                tr.last_str = self.f.stringval(lex).to_vec();
                tr.kind[p] = STRING_RULE;
                tr.strs[p] = tr.last_str.clone();
            }
            STRING_TO_TERM => {
                if is_real_string(lex) {
                    tr.last_str = self.f.stringval(lex).to_vec();
                }
                tr.strs[p] = tr.last_str.clone();
                tr.pos += 1;
            }
            _ => {
                tr.kind[p] = DEFAULT_RULE;
                tr.res[p] = r.rulenumber;
                tr.rules[p] = Some(RuleRef {
                    index: r.index,
                    arity: r.arity,
                });
                tr.pos += 1;
            }
        }
        Ok(())
    }
}

/// Builds the terms of a query: [`Builder::op`] is the normal form of the
/// operator of grammar rule `rule` (an index in
/// [`super::Tables::rules`]) applied to `args`; `int`, `ident` and `str`
/// the values read.
pub trait Builder {
    type T;
    fn op(&mut self, rule: usize, args: Vec<Self::T>) -> Self::T;
    fn int(&mut self, n: i32) -> Self::T;
    fn ident(&mut self, i: i32) -> Self::T;
    fn str(&mut self, s: &[u8]) -> Self::T;
}

/// A term as `computeTerm` builds it, before its normalisation.
struct Raw {
    rule: Option<RuleRef>,
    kind: i32,
    res: i32,
    s: Vec<u8>,
    args: Vec<Raw>,
}

impl Tree {
    /// The term ending at `*ppos`.
    fn compute_term(&self, ppos: &mut usize) -> Result<Raw, Fatal> {
        if *ppos == 0 {
            return Err(Fatal::new("computeTerm: the derivation is empty\n"));
        }
        *ppos -= 1;
        let p = *ppos;
        let mut t = Raw {
            rule: self.rules[p],
            kind: self.kind[p],
            res: self.res[p],
            s: self.strs[p].clone(),
            args: Vec::new(),
        };
        match t.kind {
            DEFAULT_RULE => {
                let Some(r) = t.rule else {
                    return Err(Fatal::new("computeTerm: an operator without its rule\n"));
                };
                for _ in 0..r.arity {
                    t.args.push(self.compute_term(ppos)?);
                }
            }
            NUM_RULE | IDENT_RULE | STRING_RULE => {}
            _ => return Err(Fatal::new("computeTerm: case not matched\n")),
        }
        Ok(t)
    }
}

/// The normal form of the term of the derivation `tr` (REM: computeTerm,
/// then normalise): built bottom-up, each operator by `b`.
pub fn build<B: Builder>(tr: &Tree, b: &mut B) -> Result<B::T, Fatal> {
    let mut pos = tr.pos;
    let raw = tr.compute_term(&mut pos)?;
    build_raw(raw, b)
}

fn build_raw<B: Builder>(t: Raw, b: &mut B) -> Result<B::T, Fatal> {
    match t.kind {
        NUM_RULE => return Ok(b.int(t.res)),
        IDENT_RULE => return Ok(b.ident(t.res)),
        STRING_RULE => return Ok(b.str(&t.s)),
        _ => {}
    }
    let index = t
        .rule
        .and_then(|r| r.index)
        .ok_or_else(|| Fatal::new("computeTerm: a standard rule as an operator\n"))?;
    let mut args = Vec::with_capacity(t.args.len());
    for a in t.args {
        args.push(build_raw(a, b)?);
    }
    Ok(b.op(index, args))
}
