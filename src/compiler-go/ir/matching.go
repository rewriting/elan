package ir

import "slices"

// Pattern matching of the left-hand sides (syntactic, S7a; elanc-rs
// matching.rs): the exhaustiveness check of the totality analysis and the
// decision trees.
//
// Both work on pattern matrices (one row per rule, one column per
// position), with the signature of a sort being the heads that its normal
// forms can have ([signatures.heads]): constructors, operators that can be
// stuck, the injection of a builtin family, true/false; the literals of
// builtinInt and builtinString make an infinite signature (only a variable
// covers them).
//
// Decision trees follow Maranget ("Compiling pattern matching to good
// decision trees", 2008) with first-match semantics: a Switch tests the
// head of the first position where the first remaining rule has a
// non-variable pattern; each case keeps, in order, the rules whose pattern
// there has that head or is a variable, so when every rule of a case fails
// (patterns or conditions), no other rule can apply and the term is stuck.
// Rules that test the same position share the test.

// wild is the variable of a wildcard (a column that a rule does not
// constrain).
const wild VarID = -1

var wildPat = &Pattern{Kind: PatVar, Var: wild}

func headOf(p *Pattern) (Head, bool) {
	switch p.Kind {
	case PatOp:
		return Head{Kind: HeadOp, Op: p.Op}, true
	case PatInt:
		return Head{Kind: HeadInt, Int: p.Int}, true
	case PatStr:
		return Head{Kind: HeadStr, Str: p.Str}, true
	case PatIdent:
		return Head{Kind: HeadIdent, Int: p.Int}, true
	}
	return Head{}, false
}

func sameHead(a, b Head) bool { return a == b }

// exhaustive tells whether the rows cover every tuple of normal forms of
// sorts.
func exhaustive(rows [][]Pattern, sorts []SortID, sig *signatures) bool {
	rs := make([][]*Pattern, len(rows))
	for i, r := range rows {
		for j := range r {
			rs[i] = append(rs[i], &r[j])
		}
	}
	return !usefulWildcards(rs, sorts, sig)
}

// usefulWildcards tells whether a row of wildcards is useful after rows,
// i.e. whether some tuple of normal forms is matched by no row.
func usefulWildcards(rows [][]*Pattern, sorts []SortID, sig *signatures) bool {
	if len(sorts) == 0 {
		return len(rows) == 0
	}
	s0, rest := sorts[0], sorts[1:]
	heads, finite := sig.heads(s0)
	used := map[OpID]bool{}
	for _, r := range rows {
		if r[0].Kind == PatOp {
			used[r[0].Op] = true
		}
	}
	all := finite
	for _, h := range heads {
		if !used[h] {
			all = false
		}
	}
	if all {
		for _, h := range heads {
			childSorts := sig.ops[h].Args
			var spec [][]*Pattern
			for _, r := range rows {
				switch {
				case r[0].Kind == PatVar:
					v := make([]*Pattern, 0, len(childSorts)+len(r)-1)
					for range childSorts {
						v = append(v, wildPat)
					}
					spec = append(spec, append(v, r[1:]...))
				case r[0].Kind == PatOp && r[0].Op == h:
					var v []*Pattern
					for i := range r[0].Args {
						v = append(v, &r[0].Args[i])
					}
					spec = append(spec, append(v, r[1:]...))
				}
			}
			s := append(slices.Clone(childSorts), rest...)
			if usefulWildcards(spec, s, sig) {
				return true
			}
		}
		return false
	}
	var def [][]*Pattern
	for _, r := range rows {
		if r[0].Kind == PatVar {
			def = append(def, r[1:])
		}
	}
	return usefulWildcards(def, rest, sig)
}

// possible tells whether a pattern can match some normal form (all its
// operators can be at the top of a normal form).
func possible(p *Pattern, sig *signatures) bool {
	if p.Kind != PatOp {
		return true
	}
	if !sig.possible(p.Op) {
		return false
	}
	for i := range p.Args {
		if !possible(&p.Args[i], sig) {
			return false
		}
	}
	return true
}

type row struct {
	rule     RuleID
	pats     []*Pattern
	bindings []Binding
}

func (r row) clone() row {
	return row{r.rule, slices.Clone(r.pats), slices.Clone(r.bindings)}
}

// decisionTree is the decision tree of the rules of an operator with
// arguments of sorts args, and the rules that can never apply (an operator
// of their left-hand side never is at the top of a normal form).
func decisionTree(rules []Rule, args []SortID, sig *signatures) (*Decision, []RuleID) {
	var never []RuleID
	var rows []row
	for i := range rules {
		ok := true
		for j := range rules[i].Lhs {
			if !possible(&rules[i].Lhs[j], sig) {
				ok = false
			}
		}
		if !ok {
			never = append(never, i)
			continue
		}
		r := row{rule: i}
		for j := range rules[i].Lhs {
			r.pats = append(r.pats, &rules[i].Lhs[j])
		}
		rows = append(rows, r)
	}
	occs := make([]Occ, len(args))
	for i := range args {
		occs[i] = Occ{i}
	}
	return compile(rows, occs, slices.Clone(args), sig), never
}

func child(o Occ, i int) Occ { return append(slices.Clone(o), i) }

func compile(rows []row, occs []Occ, sorts []SortID, sig *signatures) *Decision {
	if len(rows) == 0 {
		return &Decision{Kind: Fail}
	}
	first := rows[0]
	j := slices.IndexFunc(first.pats, func(p *Pattern) bool { return p.Kind != PatVar })
	if j < 0 {
		// the first rule matches: try it, then the others
		bindings := slices.Clone(first.bindings)
		for i, p := range first.pats {
			if p.Var != wild {
				bindings = append(bindings, Binding{p.Var, occs[i]})
			}
		}
		return &Decision{Kind: Try, Rule: first.rule, Bindings: bindings,
			Next: compile(rows[1:], occs, sorts, sig)}
	}
	var heads []Head
	for _, r := range rows {
		if h, ok := headOf(r.pats[j]); ok && !slices.ContainsFunc(heads, func(x Head) bool { return sameHead(x, h) }) {
			heads = append(heads, h)
		}
	}
	sigHeads, finite := sig.heads(sorts[j])
	complete := finite
	for _, h := range sigHeads {
		if !slices.Contains(heads, Head{Kind: HeadOp, Op: h}) {
			complete = false
		}
	}
	occ := occs[j]

	d := &Decision{Kind: Switch, Occ: occ, Sort: sorts[j]}
	for _, h := range heads {
		var childSorts []SortID
		if h.Kind == HeadOp {
			childSorts = sig.ops[h.Op].Args
		}
		k := len(childSorts)
		var spec []row
		for _, r := range rows {
			p := r.pats[j]
			var repl []*Pattern
			if p.Kind == PatVar {
				for range k {
					repl = append(repl, wildPat)
				}
			} else if ph, _ := headOf(p); sameHead(ph, h) {
				for i := range p.Args {
					repl = append(repl, &p.Args[i])
				}
			} else {
				continue
			}
			r2 := r.clone()
			if p.Kind == PatVar && p.Var != wild {
				r2.bindings = append(r2.bindings, Binding{p.Var, occ})
			}
			r2.pats = slices.Concat(r.pats[:j], repl, r.pats[j+1:])
			spec = append(spec, r2)
		}
		var childOccs []Occ
		for i := range k {
			childOccs = append(childOccs, child(occ, i))
		}
		occs2 := slices.Concat(occs[:j], childOccs, occs[j+1:])
		sorts2 := slices.Concat(sorts[:j], childSorts, sorts[j+1:])
		d.Cases = append(d.Cases, Case{Head: h, Then: compile(spec, occs2, sorts2, sig)})
	}
	if !complete {
		var def []row
		for _, r := range rows {
			p := r.pats[j]
			if p.Kind != PatVar {
				continue
			}
			r2 := r.clone()
			if p.Var != wild {
				r2.bindings = append(r2.bindings, Binding{p.Var, occ})
			}
			r2.pats = slices.Delete(r2.pats, j, j+1)
			def = append(def, r2)
		}
		occs2 := slices.Delete(slices.Clone(occs), j, j+1)
		sorts2 := slices.Delete(slices.Clone(sorts), j, j+1)
		d.Default = compile(def, occs2, sorts2, sig)
	}
	return d
}
