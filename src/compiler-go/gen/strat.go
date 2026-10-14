package gen

import (
	"fmt"
	"regexp"
	"slices"
	"strings"

	"elan/ir"
)

// Labels and strategies (S7b, spec G4; the design of elanc-rs's S6b).
//
// A strategy applied to a term t passes each of its results, in ELAN's
// order, to a continuation k (rt.K[T]: true means stop); it returns true
// when k asked to stop. The body of a named strategy is one Go expression
// of type bool; the closures it needs (the rest of a sequence, the step of
// a repeat* or an iterate*) and the state of a dc or a one are declared
// before it ("hoisted": declarations only, evaluated in order before the
// expression).

// labels writes the functions of the labels.
func (e *emitter) labels() {
	if len(e.ir.Labels) == 0 {
		return
	}
	e.line("// ==== labelled rules ====")
	e.line("")
	for i := range e.ir.Labels {
		l := &e.ir.Labels[i]
		e.fresh = 0
		ty := e.goType(l.Sort)
		e.line("// %s: label %s, each result of its rules in order (with first: the", l.Fun, comment(l.Name))
		e.line("// results of the first rule that has one).")
		e.line("//")
		for _, r := range l.Rules {
			e.line("//\t%s", comment(r.Text))
		}
		for _, s := range l.Skipped {
			e.line("//\tskipped: %s", comment(s))
		}
		e.open("func %s(a0 %s, k rt.K[%s], first bool) bool {", l.Fun, ty, ty)
		e.line("rt.Guard()")
		e.line("defer rt.Unguard()")
		if hasTry(l.Tree) {
			e.line("found := false // a rule gave a result")
		}
		e.labelMode = true
		if e.decision(l.Tree, l.Rules) {
			e.line("return false")
		}
		e.labelMode = false
		e.close("}")
		e.line("")
	}
}

func hasTry(d *ir.Decision) bool {
	switch d.Kind {
	case ir.Try:
		return true
	case ir.Switch:
		for _, c := range d.Cases {
			if hasTry(c.Then) {
				return true
			}
		}
		return d.Default != nil && hasTry(d.Default) || d.Next != nil && hasTry(d.Next)
	}
	return false
}

// strategies writes the functions of the named strategies.
func (e *emitter) strategies() {
	p := e.ir
	if len(p.Strategies) == 0 {
		return
	}
	e.line("// ==== strategies ====")
	e.line("")
	total := p.TotalStrategies()
	for i := range p.Strategies {
		st := &p.Strategies[i]
		e.fresh = 0
		e.tail = p.TailCalls(i, total)
		ty := e.goType(st.Sort)
		e.line("// %s: strategy %s,", st.Fun, comment(st.Name))
		e.line("//")
		e.line("//\t%s", comment(p.SeqText(st.Body)))
		if st.Det {
			e.line("//")
			e.line("// At most one result.")
		}
		e.open("func %s(t %s, k rt.K[%s]) bool {", st.Fun, ty, ty)
		e.line("rt.Guard()")
		e.line("defer rt.Unguard()")
		var hoist []string
		x := ""
		e.used = map[string]bool{}
		if len(e.tail) > 0 {
			x = e.seqExpr(&hoist, st.Body, st.Sort, "t", "one.K")
			if !e.used["again"] {
				// the tail calls are never reached (after a fail): no loop
				e.tail, e.fresh, hoist = nil, 0, nil
			}
		}
		if len(e.tail) == 0 {
			x := e.seqExpr(&hoist, st.Body, st.Sort, "t", "k")
			e.lines(hoist)
			e.line("return %s", x)
		} else {
			e.line("// tail-recursive: a call of %s in tail position (again)", comment(ir.BaseName(st.Name)))
			e.line("// starts the loop again on its term; the closures and the states of")
			e.line("// the choices are made once, the states reset at each turn")
			e.line("var next %s", ty)
			e.line("tail := false")
			e.open("again := func(r %s) bool {", ty)
			e.line("next, tail = r, true")
			e.line("return true")
			e.close("}")
			e.line("one := rt.NewOne(k) // the result, at most one")
			e.lines(hoist)
			e.open("for {")
			e.line("tail = false")
			e.line("one.Reset()")
			for _, h := range hoist {
				if m := stateDecl.FindStringSubmatch(h); m != nil {
					e.line("%s.Reset()", m[1])
				}
			}
			e.line("_ = %s", x)
			e.open("if !tail {")
			e.line("return one.Pass(false)")
			e.close("}")
			e.line("t = next")
			e.close("}")
		}
		e.close("}")
		e.line("")
		e.tail, e.used = nil, nil
	}
}

// stateDecl is the declaration of the state of a dc or a one (see dc and
// one): a loop resets it at each turn.
var stateDecl = regexp.MustCompile(`^((?:dc|one)\d+) := rt\.New(?:Dc|One)\(`)

// lines writes statements (possibly of several lines: go/format indents
// them).
func (e *emitter) lines(stmts []string) {
	for _, s := range stmts {
		for _, l := range strings.Split(s, "\n") {
			e.line("%s", l)
		}
	}
}

// paren parenthesises a compound expression.
func paren(x string) string {
	if strings.Contains(x, "||") || strings.Contains(x, "&&") {
		return "(" + x + ")"
	}
	return x
}

// seqExpr is a Go expression (of type bool) applying the sequence seq
// (strategies on sort) to the term t with the continuation k (names of Go
// variables); the declarations it needs are appended to hoist.
func (e *emitter) seqExpr(hoist *[]string, seq []ir.Strat, sort ir.SortID, t, k string) string {
	switch {
	case len(seq) == 0:
		return e.use(k) + "(" + t + ")"
	case len(seq) == 1:
		return e.stratExpr(hoist, &seq[0], sort, t, k)
	case seq[0].Kind == ir.StId:
		// id ; s is s (a subslice: the addresses of the tail calls stay)
		return e.seqExpr(hoist, seq[1:], sort, t, k)
	case seq[len(seq)-1].Kind == ir.StId:
		return e.seqExpr(hoist, seq[:len(seq)-1], sort, t, k)
	}
	n := e.next()
	r, kn := fmt.Sprintf("r%d", n), fmt.Sprintf("k%d", n)
	var body []string
	// the names the rest uses count only if its closure is kept
	outer := e.used
	e.used = map[string]bool{}
	rest := e.seqExpr(&body, seq[1:], sort, r, k)
	inner := e.used
	e.used = outer
	at := len(*hoist)
	*hoist = append(*hoist, "")
	first := e.stratExpr(hoist, &seq[0], sort, t, kn)
	if !e.used[kn] {
		// the rest is never reached (fail ; ...): Go refuses an unused
		// variable
		*hoist = append((*hoist)[:at], (*hoist)[at+1:]...)
		return first
	}
	for n := range inner {
		e.used[n] = true
	}
	(*hoist)[at] = fmt.Sprintf("// %s: then %s\n%s := %s", kn, comment(e.ir.SeqText(seq[1:])), kn,
		closure(r+" "+e.goType(sort), body, rest))
	return first
}

// use records that the generated code uses the Go variable name (a
// continuation, again) and returns it: a closure or a loop that nothing uses
// is not emitted, Go refusing an unused variable. (The usage is recorded
// while emitting, not searched for in the code: its comments quote ELAN
// names, which may be again or k1.)
func (e *emitter) use(name string) string {
	e.used[name] = true
	return name
}

// or is the disjunction of xs without the alternatives false (which have
// no result: go vet refuses false || false).
func or(xs []string) string {
	xs = slices.DeleteFunc(slices.Clone(xs), func(x string) bool { return x == "false" })
	if len(xs) == 0 {
		return "false"
	}
	return strings.Join(xs, " || ")
}

// closure is a function literal of the parameters params, made of the
// statements body and the return of ret.
func closure(params string, body []string, ret string) string {
	var b strings.Builder
	b.WriteString("func(" + params + ") bool {\n")
	for _, s := range body {
		b.WriteString(s + "\n")
	}
	b.WriteString("return " + ret + "\n}")
	return b.String()
}

// stratExpr is the expression of the elementary strategy s (see seqExpr).
func (e *emitter) stratExpr(hoist *[]string, s *ir.Strat, sort ir.SortID, t, k string) string {
	p := e.ir
	switch s.Kind {
	case ir.StId:
		return e.use(k) + "(" + t + ")"
	case ir.StFail:
		return "false"
	case ir.StCall:
		if e.tail[s] {
			return e.use("again") + "(" + t + ")"
		}
		if p.Strategies[s.Call].Sort != sort {
			// in a traversal, on an argument of another sort
			return e.trav(travKey{kind: travCall, call: s.Call, sort: sort}) + "(" + t + ", " + e.use(k) + ")"
		}
		return p.Strategies[s.Call].Fun + "(" + t + ", " + e.use(k) + ")"
	case ir.StMeta:
		if p.MetaApplies[s.Meta].Sort != sort {
			// in a traversal, on an argument of another sort: its terms
			// are not meta_apply terms of the strategy's sort
			return "false"
		}
		return p.MetaApplies[s.Meta].Fun + "(" + t + ", " + e.use(k) + ")"
	case ir.StTAll, ir.StTOne, ir.StTSome, ir.StRewrite, ir.StNormIn, ir.StNormOut:
		return e.trav(travKey{kind: travFun, node: s, sort: sort}) + "(" + t + ", " + e.use(k) + ")"
	case ir.StRules:
		// (in a traversal, the labels of another sort do not apply to
		// its arguments of this sort)
		labels := e.labelsAt(s.Labels, sort)
		if len(labels) == 0 {
			return "false"
		}
		calls := func(k string, first bool) []string {
			var v []string
			for _, l := range labels {
				v = append(v, fmt.Sprintf("%s(%s, %s, %v)", p.Labels[l].Fun, t, k, first))
			}
			return v
		}
		switch s.Choice {
		case ir.Dk:
			return strings.Join(calls(e.use(k), false), " || ")
		case ir.Dc:
			if len(labels) == 1 {
				return calls(e.use(k), true)[0]
			}
			d := e.dc(hoist, k, s)
			return strings.Join(calls(d+".K", true), " || !"+d+".Found && ")
		}
		o := e.one(hoist, k, s)
		return o + ".Pass(" + strings.Join(calls(o+".K", false), " || ") + ")"
	case ir.StStrats:
		if len(s.Alts) == 0 {
			return "false"
		}
		alts := func(k string) []string {
			var v []string
			for _, a := range s.Alts {
				v = append(v, paren(e.seqExpr(hoist, a, sort, t, k)))
			}
			return v
		}
		switch s.Choice {
		case ir.Dk:
			return or(alts(k))
		case ir.Dc:
			if len(s.Alts) == 1 {
				return e.seqExpr(hoist, s.Alts[0], sort, t, k)
			}
			at, wasUsed := len(*hoist), e.used[k]
			d := e.dc(hoist, k, s)
			// (an alternative without result, false, finds nothing: left
			// out; none left: no dc)
			xs := slices.DeleteFunc(alts(d+".K"), func(x string) bool { return x == "false" })
			if len(xs) == 0 {
				*hoist = slices.Delete(*hoist, at, at+1)
				e.used[k] = wasUsed
				return "false"
			}
			return strings.Join(xs, " || !"+d+".Found && ")
		}
		o := e.one(hoist, k, s)
		return o + ".Pass(" + or(alts(o+".K")) + ")"
	case ir.StRepeat, ir.StIterate:
		det := p.SeqDet(s.Body)
		n := e.next()
		step, ts, ks := fmt.Sprintf("step%d", n), fmt.Sprintf("t%d", n), fmt.Sprintf("k%d", n)
		var body []string
		x := e.seqExpr(&body, s.Body, sort, ts, ks)
		ty := e.goType(sort)
		what := "repeat*"
		if s.Kind == ir.StIterate {
			what = "iterate*"
		}
		how := ""
		if det {
			how = " (deterministic: a loop)"
		}
		*hoist = append(*hoist, fmt.Sprintf("// %s: the step of %s(%s)%s\n%s := %s", step, what,
			comment(p.SeqText(s.Body)), how, step, closure(ts+" "+ty+", "+ks+" rt.K["+ty+"]", body, x)))
		e.use(k)
		switch {
		case s.Kind == ir.StRepeat && det:
			return k + "(rt.RepeatStarDet(" + t + ", rt.DetStep(" + step + ")))"
		case s.Kind == ir.StRepeat:
			return "rt.RepeatStar(" + t + ", " + k + ", " + step + ")"
		case det:
			return "rt.IterateStarDet(" + t + ", " + k + ", rt.DetStep(" + step + "))"
		}
		return "rt.IterateStar(" + t + ", " + k + ", " + step + ")"
	}
	panic(fmt.Sprintf("strategy kind %d", s.Kind))
}

// dc declares the state of a dc/DC over several alternatives, whose
// results go to k.
func (e *emitter) dc(hoist *[]string, k string, s *ir.Strat) string {
	d := fmt.Sprintf("dc%d", e.next())
	*hoist = append(*hoist, fmt.Sprintf("%s := rt.NewDc(%s) // %s: the results of the first alternative with one",
		d, e.use(k), comment(e.ir.SeqText([]ir.Strat{*s}))))
	return d
}

// one declares the state of a one/ONE, whose result goes to k.
func (e *emitter) one(hoist *[]string, k string, s *ir.Strat) string {
	o := fmt.Sprintf("one%d", e.next())
	*hoist = append(*hoist, fmt.Sprintf("%s := rt.NewOne(%s) // %s: the first result",
		o, e.use(k), comment(e.ir.SeqText([]ir.Strat{*s}))))
	return o
}
