package ir

import (
	"cmp"
	"slices"
)

// Eker's AC matcher (S7c task 2). Both oracles match the patterns of
// wheres that contain an AC operator with Eker's matcher (REM:
// Term.java genOneToOneMatching, the whole pattern; the interpreter: every
// AC pattern), and REM also the AC patterns of left-hand sides that it
// rewrites into wheres (several variables at an AC level of three items
// or more, RewriteRule.java transformation3_1). The runtime's port of the
// matcher (rt.Eker) gives the solutions in the oracles' order provided
// that the pattern is given as REM gives it: its AC levels as their
// distinct items in REM's order with their multiplicities, its variables
// numbered depth first in that order. The interpreter numbers them in the
// same order on the programs tried (the S7c plan, Task 2 findings).

// remNode is a pattern in REM's ordered normal form (Term.java
// orderedNormalForm, insertOrderedNormalForm, cmp).
type remNode struct {
	t *tt
	// rank: 0 a value (int, string), 1 a variable, 2 a symbol
	// (SymbolValue < SymbolVariable < SymbolCode in SymbolCode.cmp)
	rank int
	code int64 // the int, the variable's number, the symbol's .ref code
	ac   bool
	args []*remNode
	// mults are the multiplicities of the items (args) of an AC level
	mults []int
	// dups are the items equal to this one merged into it (insert)
	dups []*remNode
}

// remOf is the ordered normal form of t: the arguments of an AC operator
// flattened, grouped (equal items: one item, its multiplicity) and sorted
// in REM's order.
func (rl *ruleLowerer) remOf(t *tt) *remNode {
	switch t.kind {
	case ttInt:
		return &remNode{t: t, rank: 0, code: int64(t.i)}
	case ttStr:
		return &remNode{t: t, rank: 0, code: 1 << 40}
	case ttVar:
		return &remNode{t: t, rank: 1, code: rl.varCode(t)}
	case ttIdent:
		if rl.err == nil {
			rl.err = unsupported(NotPlanned, "an identifier in an AC pattern (REM's order of identifiers among the items of its matchers not established; no program has one)")
		}
		return &remNode{t: t, rank: 0, code: 1<<41 + int64(t.i)}
	}
	if rl.isInjection(t.cand) {
		// (no node: the value itself, as in the runtime)
		return rl.remOf(&t.args[0])
	}
	k := &rl.b.l.cands[t.cand]
	n := &remNode{t: t, rank: 2, code: int64(k.symbol)}
	if k.infos != 2 {
		for i := range t.args {
			n.args = append(n.args, rl.remOf(&t.args[i]))
		}
		return n
	}
	n.ac = true
	var flatten func(x *tt)
	flatten = func(x *tt) {
		if x.kind == ttOp && x.cand == t.cand {
			for i := range x.args {
				flatten(&x.args[i])
			}
			return
		}
		n.insert(rl.remOf(x), 1)
	}
	flatten(t)
	return n
}

// varCode is REM's code of the variable occurrence t: its number, or
// after the linearisation (rl.useLin), the larger number REM gives to a
// repeated occurrence (rl.linCode).
func (rl *ruleLowerer) varCode(t *tt) int64 {
	if rl.useLin {
		if c, ok := rl.linCode[t]; ok {
			return c
		}
	}
	return int64(t.index)
}

// linearise numbers the variable occurrences of the left-hand side lhs as
// REM's linearisation does (Term.java rlinearise, on the ordered normal
// form, depth first, before the transformations): an occurrence of a
// variable already met gets a new number, larger than the rule's ones, in
// the order met (equal items of an AC level are one occurrence).
func (rl *ruleLowerer) linearise(lhs *tt) {
	rl.linCode = map[*tt]int64{}
	seen := map[uint32]bool{}
	next := int64(rl.rule.Nvars)
	var group func(ns []*remNode)
	group = func(ns []*remNode) {
		for i := 0; i < len(ns); i++ {
			ns = append(ns, ns[i].dups...)
		}
		if ns[0].rank == 1 {
			idx := ns[0].t.index
			if !seen[idx] {
				seen[idx] = true
				return
			}
			for _, n := range ns {
				rl.linCode[n.t] = next
			}
			next++
			return
		}
		for i := range ns[0].args {
			var as []*remNode
			for _, n := range ns {
				if i < len(n.args) {
					as = append(as, n.args[i])
				}
			}
			group(as)
		}
	}
	var walk func(t *tt)
	walk = func(t *tt) {
		switch {
		case t.kind == ttVar:
			group([]*remNode{{t: t, rank: 1}})
		case t.kind == ttOp && rl.b.l.cands[t.cand].infos == 2:
			group([]*remNode{rl.remOf(t)})
		default:
			for i := range t.args {
				walk(&t.args[i])
			}
		}
	}
	walk(lhs)
}

// insert adds an item to an AC level (insertOrderedNormalForm: before the
// first greater item; an equal item gets one more occurrence).
func (n *remNode) insert(x *remNode, mult int) {
	for i, y := range n.args {
		switch c := remCmp(y, x); {
		case c == 0:
			n.mults[i] += mult
			y.dups = append(y.dups, x)
			return
		case c > 0:
			n.args = slices.Insert(n.args, i, x)
			n.mults = slices.Insert(n.mults, i, mult)
			return
		}
	}
	n.args = append(n.args, x)
	n.mults = append(n.mults, mult)
}

// remCmp is REM's order of patterns (Term.cmp).
func remCmp(a, b *remNode) int {
	if c := cmp.Or(cmp.Compare(a.rank, b.rank), cmp.Compare(a.code, b.code)); c != 0 {
		return c
	}
	if a.rank == 0 && a.t.kind == ttStr {
		return cmp.Compare(a.t.s, b.t.s)
	}
	if !a.ac {
		for i := 0; i < len(a.args) && i < len(b.args); i++ {
			if c := remCmp(a.args[i], b.args[i]); c != 0 {
				return c
			}
		}
		return cmp.Compare(len(a.args), len(b.args))
	}
	for i := 0; i < len(a.args) && i < len(b.args); i++ {
		if c := cmp.Or(remCmp(a.args[i], b.args[i]), cmp.Compare(a.mults[i], b.mults[i])); c != 0 {
			return c
		}
	}
	return cmp.Compare(len(a.args), len(b.args))
}

// containsAC tells whether t has an AC operator.
func (rl *ruleLowerer) containsAC(t *tt) bool {
	if t.kind == ttOp && rl.b.l.cands[t.cand].infos == 2 {
		return true
	}
	return slices.ContainsFunc(t.args, func(a tt) bool { return rl.containsAC(&a) })
}

// ekerCond is the CondEker matching the term of variable subject against
// the pattern n (in REM's form); the variables of the pattern get their
// variables of the rule (an equality condition in same for one already
// bound, as REM renames them).
func (rl *ruleLowerer) ekerCond(subject VarID, n *remNode, same *[]Cond) Cond {
	num := map[uint32]int{}
	var order []*tt
	var number func(n *remNode)
	number = func(n *remNode) {
		if n.rank == 1 {
			if _, ok := num[n.t.index]; !ok {
				num[n.t.index] = len(order)
				order = append(order, n.t)
			}
			return
		}
		for _, a := range n.args {
			number(a)
		}
	}
	number(n)
	pat := rl.ekerPat(n, num)
	c := Cond{Kind: CondEker, A: subject, Eker: &pat}
	for _, t := range order {
		c.EkerVars = append(c.EkerVars, rl.pattern(t, same).Var)
	}
	return c
}

func (rl *ruleLowerer) ekerPat(n *remNode, num map[uint32]int) EkerPat {
	switch n.t.kind {
	case ttInt:
		return EkerPat{Kind: PatInt, Int: n.t.i}
	case ttStr:
		return EkerPat{Kind: PatStr, Str: n.t.s}
	case ttVar:
		return EkerPat{Kind: PatVar, Var: num[n.t.index]}
	}
	o := rl.b.opOf[n.t.cand]
	p := EkerPat{Kind: PatOp, Op: o}
	if n.ac {
		p.Kind, p.Mults = PatAC, n.mults
	}
	for _, a := range n.args {
		p.Args = append(p.Args, rl.ekerPat(a, num))
	}
	return p
}

// wherePattern is the pattern of a where: a pattern with an AC operator
// is matched as a whole by Eker's matcher (as REM does): a fresh variable,
// and the CondEker to add after the where's condition.
func (rl *ruleLowerer) wherePattern(t *tt, same *[]Cond) (Pattern, *Cond) {
	if !rl.containsAC(t) {
		return rl.pattern(t, same), nil
	}
	s := rl.b.sortOf[rl.b.l.sortOf(t)]
	if _, ok := rl.b.ir.Native(s); ok {
		rl.refuse("a where pattern with an AC operator, of a builtin sort (its value is native: only a stuck term could match; no program of the bench has one)")
	}
	v := rl.freshVar("w", s)
	c := rl.ekerCond(v, rl.remOf(t), same)
	return Pattern{Kind: PatVar, Var: v}, &c
}

// ACOps are the AC operators of the pattern (each once, in order).
func (p *EkerPat) ACOps() []OpID {
	var out []OpID
	var walk func(p *EkerPat)
	walk = func(p *EkerPat) {
		if p.Kind == PatAC && !slices.Contains(out, p.Op) {
			out = append(out, p.Op)
		}
		for i := range p.Args {
			walk(&p.Args[i])
		}
	}
	walk(p)
	return out
}

// isInjection tells whether the operator of candidate c is the injection
// of a builtin sort into another sort (no node: its value is the
// injected value; the kinds of the operators are set after the rules are
// lowered, the families before).
func (rl *ruleLowerer) isInjection(c int) bool {
	ir := rl.b.ir
	op := &ir.Ops[rl.b.opOf[c]]
	f := ir.Sorts[op.Result].Family
	return f >= 0 && ir.Families[f].Base != op.Result && len(op.Args) == 1 &&
		len(op.Print) == 1 && op.Print[0] == (PrintItem{Kind: PArg, Arg: 0})
}
