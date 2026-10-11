package gen

import (
	"fmt"
	"strings"

	"elan/ir"
)

// Traversals: tall, tone, tsome, rewrite, normin, normout (S7d task 6;
// elanrt/traverse.go for their semantics and the rulings).
//
// A traversal applies a strategy to the arguments of a term, which may be
// of other sorts than the term (h(nat, int)): REM is untyped and applies
// the same code to every argument; elanc-go compiles the strategy for
// each sort it meets, where the labelled rules of another sort do not
// apply (an ELAN rule applies to the terms of its sort), a named strategy
// is compiled again for that sort, and the traversals go on into the
// arguments of the arguments. The functions are made on demand (a work
// list, emitted after the strategies):
//
//   - the traversal at a sort: `func NAME(t T, k rt.K[T]) bool`, calling
//     elanrt (rt.TAll, rt.TOne, rt.TSome, rt.NormIn, rt.OuterStep,
//     rt.NormTop) with a selector;
//   - its body (or a named strategy) at a sort: as a strategy function;
//   - the step of normin/normout at a sort: one(labels of that sort);
//   - a selector `func NAME(sym int32, i int) rt.Strat[rt.Term]`: the
//     function of argument i of a term of symbol sym, by the operator's
//     profile;
//   - an adapter of a function on native values to rt.Terms (boxed).

// The kinds of traversal functions.
const (
	travFun   = iota // the traversal node at a sort
	travBody         // its body at a sort
	travCall         // a named strategy at another sort than its own
	travStep         // the step of normin/normout: one(its labels of the sort)
	travOuter        // one step of normout (leftmost-outermost)
	travSel          // a selector of the functions of role for the arguments
	travBox          // a native function on boxed terms
)

type travKey struct {
	kind int
	node *ir.Strat
	call ir.StratID
	sort ir.SortID
	// role is the kind of the functions a selector returns
	role int
	// inner is the native function of an adapter
	inner string
}

// trav is the name of the function of key (queued when new).
func (e *emitter) trav(key travKey) string {
	if e.travNames == nil {
		e.travNames = map[travKey]string{}
		e.travNodes = map[*ir.Strat]int{}
	}
	if n, ok := e.travNames[key]; ok {
		return n
	}
	p := e.ir
	sort := ir.OpBaseName([]string{p.Sorts[key.sort].Name})
	node := func() int {
		n, ok := e.travNodes[key.node]
		if !ok {
			n = len(e.travNodes) + 1
			e.travNodes[key.node] = n
		}
		return n
	}
	var name string
	switch key.kind {
	case travFun:
		what := map[ir.StratKind]string{ir.StTAll: "tall", ir.StTOne: "tone", ir.StTSome: "tsome",
			ir.StRewrite: "rewrite", ir.StNormIn: "normin", ir.StNormOut: "normout"}[key.node.Kind]
		name = fmt.Sprintf("%s%d_%s", what, node(), sort)
	case travBody:
		name = fmt.Sprintf("body%d_%s", node(), sort)
	case travCall:
		name = p.Strategies[key.call].Fun + "_" + sort
	case travStep:
		name = fmt.Sprintf("step%d_%s", node(), sort)
	case travOuter:
		name = fmt.Sprintf("outer%d_%s", node(), sort)
	case travSel:
		role := map[int]string{travFun: "fun", travBody: "body", travOuter: "outer"}[key.role]
		name = fmt.Sprintf("sel%d%s_%s", node(), role, sort)
	case travBox:
		name = "boxed_" + key.inner
	}
	for e.travUsed[name] {
		name += "_"
	}
	if e.travUsed == nil {
		e.travUsed = map[string]bool{}
	}
	e.travUsed[name] = true
	e.travNames[key] = name
	e.travQueue = append(e.travQueue, key)
	return name
}

// labelsAt are the labels of ls whose rules are of sort s.
func (e *emitter) labelsAt(ls []ir.LabelID, s ir.SortID) []ir.LabelID {
	var v []ir.LabelID
	for _, l := range ls {
		if e.ir.Labels[l].Sort == s {
			v = append(v, l)
		}
	}
	return v
}

// travFuncs writes the traversal functions (and those they need).
func (e *emitter) travFuncs() {
	if len(e.travQueue) == 0 {
		return
	}
	e.line("// ==== traversals ====")
	e.line("")
	for len(e.travQueue) > 0 {
		key := e.travQueue[0]
		e.travQueue = e.travQueue[1:]
		switch key.kind {
		case travSel:
			e.travSelFunc(key)
		case travBox:
			e.travBoxFunc(key)
		default:
			e.travStratFunc(key)
		}
	}
}

// travStratFunc writes a function of strategy type at key.sort.
func (e *emitter) travStratFunc(key travKey) {
	p := e.ir
	name := e.travNames[key]
	z := key.sort
	ty := e.goType(z)
	_, native := p.Native(z)
	e.fresh, e.tail, e.used = 0, nil, map[string]bool{}
	var hoist []string
	x := ""
	var doc string
	switch key.kind {
	case travCall:
		st := &p.Strategies[key.call]
		doc = fmt.Sprintf("strategy %s on terms of sort %s (in a traversal): %s",
			st.Name, p.Sorts[z].Name, p.SeqText(st.Body))
		x = e.seqExpr(&hoist, st.Body, z, "t", "k")
	case travBody:
		doc = fmt.Sprintf("%s on terms of sort %s (in %s)", p.SeqText(key.node.Body), p.Sorts[z].Name,
			p.SeqText([]ir.Strat{*key.node}))
		x = e.seqExpr(&hoist, key.node.Body, z, "t", "k")
	case travStep:
		ls := e.labelsAt(key.node.Labels, z)
		doc = fmt.Sprintf("a step of %s on terms of sort %s: the first result of its labels of the sort",
			p.SeqText([]ir.Strat{*key.node}), p.Sorts[z].Name)
		x = "false"
		if len(ls) > 0 {
			x = e.stratExpr(&hoist, &ir.Strat{Kind: ir.StRules, Choice: ir.One, Labels: ls}, z, "t", "k")
		}
	case travOuter:
		doc = fmt.Sprintf("a step of %s on terms of sort %s, leftmost-outermost",
			p.SeqText([]ir.Strat{*key.node}), p.Sorts[z].Name)
		step := e.trav(travKey{kind: travStep, node: key.node, sort: z})
		if native {
			x = step + "(t, k)"
		} else {
			e.normTerm = true
			x = fmt.Sprintf("rt.OuterStep(t, k, normTerm, %s, %s)",
				e.trav(travKey{kind: travSel, role: travOuter, node: key.node, sort: z}), step)
		}
	case travFun:
		doc = fmt.Sprintf("%s on terms of sort %s", p.SeqText([]ir.Strat{*key.node}), p.Sorts[z].Name)
		x = e.travExpr(key.node, z, native)
	}
	e.line("// %s: %s.", name, comment(doc))
	e.open("func %s(t %s, k rt.K[%s]) bool {", name, ty, ty)
	e.line("rt.Guard()")
	e.line("defer rt.Unguard()")
	e.lines(hoist)
	e.line("return %s", x)
	e.close("}")
	e.line("")
	e.used = nil
}

// travExpr is the body of the traversal node at sort z.
func (e *emitter) travExpr(node *ir.Strat, z ir.SortID, native bool) string {
	sel := func(role int) string {
		return e.trav(travKey{kind: travSel, role: role, node: node, sort: z})
	}
	body := func() string { return e.trav(travKey{kind: travBody, node: node, sort: z}) }
	step := func() string { return e.trav(travKey{kind: travStep, node: node, sort: z}) }
	if native {
		// a native value has no arguments: a constant
		switch node.Kind {
		case ir.StTAll:
			return "k(t)"
		case ir.StTOne, ir.StTSome:
			return "false"
		case ir.StRewrite:
			return body() + "(t, k)"
		case ir.StNormIn:
			return "k(rt.NormTop(t, " + step() + "))"
		}
		return "k(rt.NormTop(t, " + e.trav(travKey{kind: travOuter, node: node, sort: z}) + "))"
	}
	e.normTerm = true
	switch node.Kind {
	case ir.StTAll:
		return "rt.TAll(t, k, normTerm, " + sel(travBody) + ")"
	case ir.StTOne:
		return "rt.TOne(t, k, normTerm, " + sel(travBody) + ")"
	case ir.StTSome:
		return "rt.TSome(t, k, normTerm, " + sel(travBody) + ")"
	case ir.StRewrite:
		// DK(s, tone(rewrite(s))): s at the top, then in the arguments
		return body() + "(t, k) || rt.TOne(t, k, normTerm, " + sel(travFun) + ")"
	case ir.StNormIn:
		return "k(rt.NormIn(t, normTerm, " + sel(travFun) + ", " + step() + "))"
	}
	return "k(rt.NormTop(t, " + e.trav(travKey{kind: travOuter, node: node, sort: z}) + "))"
}

// travSelFunc writes a selector: for each operator that can be at the top
// of a term of key.sort, the function of role for each of its arguments
// (by the sort of the argument).
func (e *emitter) travSelFunc(key travKey) {
	p := e.ir
	name := e.travNames[key]
	z := key.sort
	ops := p.Sorts[z].Variants
	if f := p.FamilyOf(z); f != nil {
		ops = f.Stuck
	}
	type group struct {
		syms []string
		body []string
	}
	var groups []*group
	byBody := map[string]*group{}
	for _, o := range ops {
		op := &p.Ops[o]
		if len(op.Args) == 0 {
			continue
		}
		var funs []string
		for _, a := range op.Args {
			fn := e.trav(travKey{kind: key.role, node: key.node, sort: a})
			if _, ok := p.Native(a); ok {
				fn = e.trav(travKey{kind: travBox, inner: fn, sort: a})
			}
			funs = append(funs, fn)
		}
		var body []string
		same := true
		for _, f := range funs {
			same = same && f == funs[0]
		}
		if same || op.AC {
			body = []string{"return " + funs[0]}
		} else {
			body = append(body, "switch i {")
			for i, f := range funs {
				body = append(body, fmt.Sprintf("case %d:", i), "return "+f)
			}
			body = append(body, "}")
		}
		b := strings.Join(body, "\n")
		g, ok := byBody[b]
		if !ok {
			g = &group{body: body}
			byBody[b] = g
			groups = append(groups, g)
		}
		g.syms = append(g.syms, symName(op))
	}
	e.line("// %s: the strategy applied to argument i of a term of sort %s whose", name, comment(p.Sorts[z].Name))
	e.line("// symbol is sym, compiled for the sort of the argument.")
	if len(groups) == 0 {
		e.open("func %s(int32, int) rt.Strat[rt.Term] {", name)
		e.line("return nil // (no term of the sort has arguments)")
		e.close("}")
		e.line("")
		return
	}
	e.open("func %s(sym int32, i int) rt.Strat[rt.Term] {", name)
	e.line("switch sym {")
	for _, g := range groups {
		e.line("case %s:", strings.Join(g.syms, ", "))
		e.indent++
		e.lines(g.body)
		e.indent--
	}
	e.line("}")
	e.line("panic(\"no argument\")")
	e.close("}")
	e.line("")
}

// travBoxFunc writes the adapter of a function on native values to the
// terms that hold them.
func (e *emitter) travBoxFunc(key travKey) {
	name := e.travNames[key]
	ty := e.goType(key.sort)
	e.line("// %s is %s on the terms holding the values.", name, key.inner)
	e.open("func %s(t rt.Term, k rt.K[rt.Term]) bool {", name)
	e.line("return %s(%s, func(r %s) bool { return k(%s) })", key.inner, e.unbox(key.sort, "t"), ty, e.asTerm(key.sort, "r"))
	e.close("}")
	e.line("")
}
