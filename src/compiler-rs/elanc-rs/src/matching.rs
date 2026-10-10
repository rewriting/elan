//! Pattern matching of the left-hand sides (syntactic, S6a): the
//! exhaustiveness check of the totality analysis and the decision trees.
//!
//! Both work on pattern matrices (one row per rule, one column per
//! position), with the *signature* of a sort being the heads that its
//! normal forms can have ([`Signatures::heads`]): constructors, operators
//! that can be stuck, the injection of a builtin family, `true`/`false`;
//! the literals of `builtinInt` and `builtinString` make an infinite
//! signature (only a variable covers them).
//!
//! Decision trees follow Maranget ("Compiling pattern matching to good
//! decision trees", 2008) with first-match semantics: a `Switch` tests the
//! head of the first position where the first remaining rule has a
//! non-variable pattern; each case keeps, in order, the rules whose
//! pattern there has that head **or is a variable**, so when every rule of
//! a case fails (patterns or conditions), no other rule can apply and the
//! term is stuck. One test per shared node: rules that test the same
//! position share the test.

use std::collections::HashSet;

use crate::ir::*;
use crate::lower::Signatures;

/// A wildcard (a column that a rule does not constrain).
const WILD: VarId = usize::MAX;
static WILD_PAT: Pattern = Pattern::Var(WILD);

fn head_of(p: &Pattern) -> Option<Head> {
    match p {
        Pattern::Var(_) => None,
        Pattern::Op { op, .. } => Some(Head::Op(*op)),
        Pattern::Int(n) => Some(Head::Int(*n)),
        Pattern::Str(s) => Some(Head::Str(s.clone())),
    }
}

fn children(p: &Pattern) -> Vec<&Pattern> {
    match p {
        Pattern::Op { args, .. } => args.iter().collect(),
        _ => vec![],
    }
}

/// Do the rows cover every tuple of normal forms of `sorts`?
pub fn exhaustive(rows: &[Vec<Pattern>], sorts: &[SortId], sig: &Signatures) -> bool {
    let rows: Vec<Vec<&Pattern>> = rows.iter().map(|r| r.iter().collect()).collect();
    !useful_wildcards(rows, sorts, sig)
}

/// Is a row of wildcards useful after `rows`, i.e. is some tuple of normal
/// forms matched by no row?
fn useful_wildcards(rows: Vec<Vec<&Pattern>>, sorts: &[SortId], sig: &Signatures) -> bool {
    let Some((&s0, rest)) = sorts.split_first() else {
        return rows.is_empty();
    };
    let (heads, finite) = sig.heads(s0);
    let used: HashSet<OpId> = rows
        .iter()
        .filter_map(|r| match r[0] {
            Pattern::Op { op, .. } => Some(*op),
            _ => None,
        })
        .collect();
    if finite && heads.iter().all(|h| used.contains(h)) {
        heads.iter().any(|&h| {
            let child_sorts = sig.args(h);
            let spec: Vec<Vec<&Pattern>> = rows
                .iter()
                .filter_map(|r| match r[0] {
                    Pattern::Var(_) => {
                        let mut v = vec![&WILD_PAT; child_sorts.len()];
                        v.extend_from_slice(&r[1..]);
                        Some(v)
                    }
                    Pattern::Op { op, args } if *op == h => {
                        let mut v: Vec<&Pattern> = args.iter().collect();
                        v.extend_from_slice(&r[1..]);
                        Some(v)
                    }
                    _ => None,
                })
                .collect();
            let mut s = child_sorts.to_vec();
            s.extend_from_slice(rest);
            useful_wildcards(spec, &s, sig)
        })
    } else {
        let def: Vec<Vec<&Pattern>> = rows
            .iter()
            .filter(|r| matches!(r[0], Pattern::Var(_)))
            .map(|r| r[1..].to_vec())
            .collect();
        useful_wildcards(def, rest, sig)
    }
}

/// Can a pattern match some normal form (all its operators can be at the
/// top of a normal form)?
fn possible(p: &Pattern, sig: &Signatures) -> bool {
    match p {
        Pattern::Op { op, args } => sig.possible(*op) && args.iter().all(|a| possible(a, sig)),
        _ => true,
    }
}

#[derive(Clone)]
struct Row<'r> {
    rule: RuleId,
    pats: Vec<&'r Pattern>,
    bindings: Vec<(VarId, Occ)>,
}

/// The decision tree of the rules of an operator with arguments of sorts
/// `args`, and the rules that can never apply (an operator of their
/// left-hand side never is at the top of a normal form).
pub fn decision_tree(
    rules: &[RuleIr],
    args: &[SortId],
    sig: &Signatures,
) -> (Decision, Vec<RuleId>) {
    let mut never = Vec::new();
    let mut rows = Vec::new();
    for (i, r) in rules.iter().enumerate() {
        if r.lhs.iter().all(|p| possible(p, sig)) {
            rows.push(Row {
                rule: i,
                pats: r.lhs.iter().collect(),
                bindings: vec![],
            });
        } else {
            never.push(i);
        }
    }
    let occs: Vec<Occ> = (0..args.len()).map(|i| vec![i]).collect();
    (compile(rows, occs, args.to_vec(), sig), never)
}

fn compile(rows: Vec<Row>, occs: Vec<Occ>, sorts: Vec<SortId>, sig: &Signatures) -> Decision {
    let Some(first) = rows.first() else {
        return Decision::Fail;
    };
    let Some(j) = first
        .pats
        .iter()
        .position(|p| !matches!(p, Pattern::Var(_)))
    else {
        // the first rule matches: try it, then the others
        let mut bindings = first.bindings.clone();
        for (p, o) in first.pats.iter().zip(&occs) {
            if let Pattern::Var(v) = p {
                if *v != WILD {
                    bindings.push((*v, o.clone()));
                }
            }
        }
        let rule = first.rule;
        let rest = rows[1..].to_vec();
        return Decision::Try {
            rule,
            bindings,
            next: Box::new(compile(rest, occs, sorts, sig)),
        };
    };
    let mut heads: Vec<Head> = Vec::new();
    for r in &rows {
        if let Some(h) = head_of(r.pats[j]) {
            if !heads.contains(&h) {
                heads.push(h);
            }
        }
    }
    let (sig_heads, finite) = sig.heads(sorts[j]);
    let complete = finite && sig_heads.iter().all(|h| heads.contains(&Head::Op(*h)));
    let occ = occs[j].clone();

    let mut cases = Vec::new();
    for h in &heads {
        let child_sorts: Vec<SortId> = match h {
            Head::Op(o) => sig.args(*o).to_vec(),
            _ => vec![],
        };
        let k = child_sorts.len();
        let spec: Vec<Row> = rows
            .iter()
            .filter_map(|r| {
                let mut r2 = r.clone();
                let p = r.pats[j];
                let new: Vec<&Pattern> = match p {
                    Pattern::Var(v) => {
                        if *v != WILD {
                            r2.bindings.push((*v, occ.clone()));
                        }
                        vec![&WILD_PAT; k]
                    }
                    _ if head_of(p).as_ref() == Some(h) => children(p),
                    _ => return None,
                };
                r2.pats.splice(j..j + 1, new);
                Some(r2)
            })
            .collect();
        let mut occs2 = occs.clone();
        occs2.splice(
            j..j + 1,
            (0..k).map(|i| {
                let mut o = occ.clone();
                o.push(i);
                o
            }),
        );
        let mut sorts2 = sorts.clone();
        sorts2.splice(j..j + 1, child_sorts);
        cases.push((h.clone(), compile(spec, occs2, sorts2, sig)));
    }
    let default = if complete {
        None
    } else {
        let def: Vec<Row> = rows
            .iter()
            .filter_map(|r| match r.pats[j] {
                Pattern::Var(v) => {
                    let mut r2 = r.clone();
                    if *v != WILD {
                        r2.bindings.push((*v, occ.clone()));
                    }
                    r2.pats.remove(j);
                    Some(r2)
                }
                _ => None,
            })
            .collect();
        let mut occs2 = occs.clone();
        occs2.remove(j);
        let mut sorts2 = sorts.clone();
        sorts2.remove(j);
        Some(Box::new(compile(def, occs2, sorts2, sig)))
    };
    Decision::Switch {
        occ,
        sort: sorts[j],
        cases,
        default,
    }
}
