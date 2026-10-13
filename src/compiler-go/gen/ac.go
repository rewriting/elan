package gen

import (
	"fmt"
	"maps"
	"slices"
	"strings"

	"elan/ir"
)

// AC matching (S7c tasks 1 and 2): the code of a CondAC condition, and of
// a CondEker condition (Eker's matcher, rt.Eker: the patterns of wheres
// with an AC operator and the parts of AC levels that REM abstracts into
// such wheres, ACAbs). The subject
// is a canonical AC term (its elements sorted, rt.Compare); the pattern
// has element patterns, matched one element each, in source order, each
// by a loop over the elements (each distinct element once: rt.FirstOcc),
// then the variables of the AC level, given the elements left over (the
// rest): one variable of multiplicity m takes the rest divided by m
// (rt.DivAC), or, modulo an extension, each value from the largest down
// (rt.Down); two variables take REM's counter order (rt.Split). A
// variable bound to several elements is normalised by the function of the
// AC operator (the rest of a normal form can be reducible: and(x, t) on
// and(a, a, t) with and(x, x) => x). A variable that also occurs elsewhere
// in the pattern (a CondSame) is compared as it is (the elements, as REM
// and the interpreter compare it), normalised only once a solution passes
// the comparisons; when its other occurrence is bound first, it takes that
// value at once (rt.TakeAC) instead of enumerating the sub-multisets.
//
// The conditions after the matching that give at most one solution, bind
// nothing and take no rewrite step (non-linearity checks, ifs and local
// assignments of builtin operations) are written as soon as the variables
// they read are bound, in their order (the matching is pruned: efib's
// compute rule tests n1 == n2 + 1 before choosing the third element); the
// others after the whole match, as REM evaluates them (once per solution:
// the same rewrite steps). Every failure falls through to the next
// iteration: the solutions are tried in order, each one passed to the
// rest of the rule. F(x, t1, ..., tm) at the top of an unlabelled rule,
// x read by no condition, is matched as REM matches it: x takes the rest,
// without an extension (restTaker).

// extension is the extension of a rule applied modulo an extension: the
// AC operator and the Go variable of the elements left over.
type extension struct {
	op   ir.OpID
	rest string
}

// patternVars adds the variables of p to into.
func patternVars(p *ir.Pattern, into map[ir.VarID]bool) {
	switch p.Kind {
	case ir.PatVar:
		into[p.Var] = true
	case ir.PatOp, ir.PatAC:
		for i := range p.Args {
			patternVars(&p.Args[i], into)
		}
		for _, v := range p.Vars {
			into[v.Var] = true
		}
		if p.Abs != nil {
			// (the variables of the abstracted part are bound by the
			// CondEker after the AC matching)
			into[p.Abs.Var] = true
			for _, v := range p.Abs.EkerVars {
				into[v] = true
			}
		}
	}
}

// exprVars adds the variables of x to into.
func exprVars(x *ir.Expr, into map[ir.VarID]bool) {
	if x.Kind == ir.ExprVar {
		into[x.Var] = true
	}
	for i := range x.Args {
		exprVars(&x.Args[i], into)
	}
}

// stepFree tells whether evaluating x takes no rewrite step: it applies
// no operator with rules (constructors, injections, booleans and builtin
// operations only).
func (e *emitter) stepFree(x *ir.Expr) bool {
	if x.Kind == ir.ExprOp && e.ir.Ops[x.Op].Kind == ir.Defined {
		return false
	}
	for i := range x.Args {
		if !e.stepFree(&x.Args[i]) {
			return false
		}
	}
	return true
}

// condReads tells whether the condition c reads the variable v.
func condReads(c *ir.Cond, v ir.VarID) bool {
	vars := map[ir.VarID]bool{}
	c.Walk(func(c *ir.Cond) {
		switch c.Kind {
		case ir.CondSame:
			vars[c.B] = true
			vars[c.A] = true
		case ir.CondAC, ir.CondEker:
			vars[c.A] = true
		case ir.CondChoose:
		default:
			exprVars(&c.Expr, vars)
		}
	})
	return vars[v]
}

// hoist writes the conditions from *next on that can be written now: those
// of [emitter.simpleCond] whose variables are not pending and that take no
// rewrite step, in order (it stops at the first other one). It returns the
// number of ifs opened. A condition that can take steps waits for the
// whole match, as in REM (it evaluates the conditions once per solution of
// the whole AC match: the rewrite steps are counted as REM counts them);
// the hoisted ones only prune the enumeration.
func (e *emitter) hoist(r *ir.Rule, next *int, pending map[ir.VarID]bool, used map[ir.VarID]bool) int {
	opens := 0
	for ; *next < len(r.Conds); *next++ {
		c := &r.Conds[*next]
		vars := map[ir.VarID]bool{}
		switch c.Kind {
		case ir.CondSame:
			vars[c.A], vars[c.B] = true, true
		case ir.CondIf, ir.CondLet:
			if !e.stepFree(&c.Expr) {
				return opens
			}
			exprVars(&c.Expr, vars)
		default:
			return opens
		}
		for v := range vars {
			if pending[v] {
				return opens
			}
		}
		opens += e.simpleCond(c, r, used)
	}
	return opens
}

// acValue is the normal form of the AC term of the multiset m (a Go
// expression of type []rt.Term) of the AC operator o.
func (e *emitter) acValue(o ir.OpID, m string) string {
	op := &e.ir.Ops[o]
	if op.Kind == ir.Defined {
		return fmt.Sprintf("rt.NormAC(%s, %s)", m, acFun(op))
	}
	return fmt.Sprintf("rt.MkAC(%s, %s)", symName(op), m)
}

// acMatch writes the conditions of r from j on that are CondACs (the AC
// patterns of a left-hand side or of a where, outer patterns first, a
// nested one after the pattern whose element binds its subject), then the
// rest of the rule for each solution. Each pattern's element loops are
// written first, then the patterns that follow (nested ones, then
// siblings), and the variables of the AC level of each pattern innermost,
// in reverse order: a nested pattern prunes the choice of its element
// before the variables of the outer level are enumerated (and(x,
// xor(y, z)): no enumeration of x for an element that is not a xor).
func (e *emitter) acMatch(r *ir.Rule, j int, used map[ir.VarID]bool, depth int) {
	end := j
	pending := map[ir.VarID]bool{}
	for end < len(r.Conds) && r.Conds[end].Kind == ir.CondAC {
		patternVars(&r.Conds[end].Pat, pending)
		end++
	}
	next := end
	later := map[ir.VarID]bool{}
	for k := end; k < len(r.Conds); k++ {
		r.Conds[k].Walk(func(c *ir.Cond) { bindsOf(c, later) })
	}
	saved := e.acLater
	e.acLater = later
	e.acLevel(r, j, end, &next, pending, used, func() { e.conds(r, next, used, depth) })
	e.acLater = saved
}

// lazyAC is an AC-level variable v that occurs twice, bound to the
// elements ms (a Go variable of type []rt.Term) of the AC operator op,
// whose normal form is not written yet.
type lazyAC struct {
	v  ir.VarID
	op ir.OpID
	ms string
}

// forceLazy writes the normal forms of the variables of e.lazy that `reads`
// selects (nil: all of them), before the condition or the right-hand side
// that reads them: the candidates that the equality of a CondSame rejects
// are never normalised (their rewrite steps not counted, as in REM), nor
// is a variable that the condition does not read (a partner bound by a
// later where, a hoisted condition of another variable).
func (e *emitter) forceLazy(r *ir.Rule, reads func(ir.VarID) bool) {
	var keep []lazyAC
	for _, l := range e.lazy {
		if reads != nil && !reads(l.v) {
			keep = append(keep, l)
			continue
		}
		e.line("%s := %s", r.Vars[l.v].Name, e.acValue(l.op, l.ms))
	}
	e.lazy = keep
}

// condReader is forceLazy's reads for the condition c.
func condReader(c *ir.Cond) func(ir.VarID) bool {
	return func(v ir.VarID) bool { return condReads(c, v) }
}

// sameOf are the variables that a CondSame of r compares with v.
func sameOf(r *ir.Rule, v ir.VarID) []ir.VarID {
	var ws []ir.VarID
	for i := range r.Conds {
		r.Conds[i].Walk(func(c *ir.Cond) {
			if c.Kind != ir.CondSame {
				return
			}
			switch v {
			case c.A:
				ws = append(ws, c.B)
			case c.B:
				ws = append(ws, c.A)
			}
		})
	}
	return ws
}

// readsBeyondSame tells whether v is read by r other than by its
// CondSames (by a condition or the right-hand side).
func readsBeyondSame(r *ir.Rule, v ir.VarID) bool {
	vars := map[ir.VarID]bool{}
	for i := range r.Conds {
		r.Conds[i].Walk(func(c *ir.Cond) {
			switch c.Kind {
			case ir.CondSame, ir.CondChoose:
			case ir.CondAC, ir.CondEker:
				vars[c.A] = true
			default:
				exprVars(&c.Expr, vars)
			}
		})
	}
	exprVars(&r.Rhs, vars)
	return vars[v]
}

// acLevel writes the CondAC i (its element loops), then the next ones up
// to end, then the variables of its AC level, then k.
func (e *emitter) acLevel(r *ir.Rule, i, end int, next *int, pending, used map[ir.VarID]bool, k func()) {
	if i == end {
		k()
		return
	}
	c := &r.Conds[i]
	p := &c.Pat
	op := &e.ir.Ops[p.Op]
	subject := r.Vars[c.A].Name
	n := e.next()
	el := fmt.Sprintf("e%d", n)
	opens := 0
	open := func(format string, a ...any) {
		e.open(format, a...)
		opens++
	}
	k0 := len(p.Args)
	least := k0
	for _, v := range p.Vars {
		least += v.Mult
	}
	how := ""
	if p.Ext {
		how = " (modulo an extension)"
	}
	e.line("// AC matching of %s%s: %d element pattern(s), %d variable(s) of the AC level",
		subject, how, k0, len(p.Vars))
	if p.Abs != nil {
		least++
	}
	if restTaker(r, p) {
		// (the rest can be empty during the match, as in REM)
		least = k0
	}
	if len(p.Vars) == 0 && p.Abs == nil && !p.Ext {
		open("if %s.Sym == %s && len(%s.Args) == %d {", subject, symName(op), subject, k0)
	} else {
		open("if %s.Sym == %s && len(%s.Args) >= %d {", subject, symName(op), subject, least)
	}
	e.line("%s := %s.Args", el, subject)
	opens += e.hoist(r, next, pending, used)
	var pos []string
	for i := range p.Args {
		a := &p.Args[i]
		ix := fmt.Sprintf("i%d_%d", n, i)
		open("for %s := range %s {", ix, el)
		taken := "nil"
		if len(pos) > 0 {
			taken = "[]int{" + strings.Join(pos, ", ") + "}"
		}
		e.open("if !rt.FirstOcc(%s, %s, %s) {", el, ix, taken)
		e.line("continue // taken, or the same element as one before")
		e.close("}")
		pos = append(pos, ix)
		m, _ := e.patternCheck(a, el+"["+ix+"]", op.Result, r, used)
		opens += m
		bound := map[ir.VarID]bool{}
		patternVars(a, bound)
		for v := range bound {
			delete(pending, v)
		}
		opens += e.hoist(r, next, pending, used)
	}
	e.acLevel(r, i+1, end, next, pending, used, func() {
		e.acVars(r, c, el, pos, n, next, pending, used, k)
	})
	for range opens {
		e.close("}")
	}
}

// acVars writes the variables of the AC level of the CondAC c (the
// elements of its subject el not at the positions pos) and the
// extension, then k. With an extension and two variables or an
// abstracted part, the elements given to the variables (the core) are
// enumerated from the largest multiset down (the first core is all the
// elements: no extension), the others being the extension, then the
// variables take the core as without extension: the interpreter's
// semantics (the rule, then its extension rule); REM never matches such
// a rule without an extension (the S7c plan, Task 2 findings).
func (e *emitter) acVars(r *ir.Rule, c *ir.Cond, el string, pos []string, n int, next *int, pending, used map[ir.VarID]bool, k func()) {
	p := &c.Pat
	opens := 0
	open := func(format string, a ...any) {
		e.open(format, a...)
		opens++
	}
	need := p.Ext || p.Abs != nil || len(p.Vars) == 2 || len(p.Vars) == 1 && (p.Vars[0].Mult > 1 || used[p.Vars[0].Var])
	rest := el
	if len(pos) > 0 && need {
		rest = fmt.Sprintf("r%d", n)
		e.line("%s := rt.Rest(%s, %s)", rest, el, strings.Join(pos, ", "))
	}
	// a variable that occurs twice (a CondSame) at the level of an
	// operator with rules: compared as it is (REM and the interpreter
	// compare the elements, not their normal form: G(b, c) is not a with
	// G(b, c) => a), normalised only when a solution passes the
	// comparisons (forceLazy): normalising each candidate cost steps,
	// and exponential time (each one a normalisation of the operator,
	// enumerating its own candidates; programs/ac nl_*)
	sym := symName(&e.ir.Ops[p.Op])
	isRaw := func(v ir.VarID) bool {
		return e.ir.Ops[p.Op].Kind == ir.Defined && (p.Abs == nil || v != p.Abs.Var) && len(sameOf(r, v)) > 0
	}
	// known is the value of another occurrence of v already bound (the
	// value v must have), or ""
	known := func(v ir.VarID) string {
		if !isRaw(v) {
			return ""
		}
		for _, w := range sameOf(r, v) {
			if w == v || pending[w] || e.acLater[w] {
				continue
			}
			if x, ok := e.raw[w]; ok {
				return x
			}
			return r.Vars[w].Name
		}
		return ""
	}
	savedRaw, savedLazy := e.raw, e.lazy
	bind := func(v ir.VarID, m string) {
		switch {
		case isRaw(v):
			name := r.Vars[v].Name
			e.line("%s_ms := %s", name, m)
			e.line("%s_raw := rt.ACSubject(%s, %s_ms)", name, sym, name)
			raw := maps.Clone(e.raw)
			if raw == nil {
				raw = map[ir.VarID]string{}
			}
			raw[v] = name + "_raw"
			e.raw = raw
			if readsBeyondSame(r, v) {
				e.lazy = append(slices.Clip(e.lazy), lazyAC{v: v, op: p.Op, ms: name + "_ms"})
			}
		case p.Abs != nil && v == p.Abs.Var:
			// the subject of Eker's matcher: the elements as they are (a
			// part of a term being normalised can be reducible; the
			// interpreter matches it as it is, REM normalises it first),
			// a transient node: it never enters the unique table (AC
			// terms enter it only as normal forms), and the values the
			// matcher gives are elements or normal forms (acNorm)
			e.line("%s := rt.ACSubject(%s, %s)", r.Vars[v].Name, symName(&e.ir.Ops[p.Op]), m)
		case used[v]:
			e.line("%s := %s", r.Vars[v].Name, e.acValue(p.Op, m))
		}
	}
	saved, savedLate := e.ext, e.late
	ext := p.Ext
	if ext && (len(p.Vars) == 2 || p.Abs != nil) {
		d := fmt.Sprintf("d%d", n)
		open("for %s := rt.NewDown(%s, 1); %s.Next(); {", d, rest, d)
		core, x := fmt.Sprintf("core%d", n), fmt.Sprintf("ext%d", n)
		e.line("%s, %s := %s.X(), %s.Rest()", core, x, d, d)
		e.ext = &extension{p.Op, x}
		rest, ext = core, false
	}
	// the variables of the AC level, then the abstracted variable
	vars := p.Vars
	if p.Abs != nil {
		vars = append(slices.Clone(vars), ir.ACVar{Var: p.Abs.Var, Mult: 1})
	}
	switch len(vars) {
	case 0:
		if ext {
			e.ext = &extension{p.Op, rest}
		}
	case 1:
		v := vars[0]
		switch {
		case p.Abs != nil:
			open("if len(%s) > 0 {", rest)
			bind(v.Var, rest)
		case restTaker(r, p):
			// the rest, bound after the conditions (lateBind)
			e.late = &lateRest{v: v.Var, op: p.Op, rest: rest}
		case ext && known(v.Var) != "":
			// the value of its other occurrence: one solution at most
			x := fmt.Sprintf("ext%d", n)
			w := fmt.Sprintf("rt.Elems(%s, %s)", sym, known(v.Var))
			open("if %s, ok := rt.TakeAC(%s, %s, %d); ok {", x, rest, w, v.Mult)
			bind(v.Var, w)
			e.ext = &extension{p.Op, x}
		case ext:
			d := fmt.Sprintf("d%d", n)
			open("for %s := rt.NewDown(%s, %d); %s.Next(); {", d, rest, v.Mult, d)
			bind(v.Var, d+".X()")
			x := fmt.Sprintf("ext%d", n)
			e.line("%s := %s.Rest()", x, d)
			e.ext = &extension{p.Op, x}
		case v.Mult == 1 && len(pos) == 0:
			// the whole subject (already a normal form)
			if used[v.Var] {
				e.line("%s := %s", r.Vars[v.Var].Name, r.Vars[c.A].Name)
			}
		case v.Mult == 1:
			bind(v.Var, rest)
		case used[v.Var]:
			x := fmt.Sprintf("xs%d", n)
			open("if %s, ok := rt.DivAC(%s, %d); ok {", x, rest, v.Mult)
			bind(v.Var, x)
		default:
			open("if _, ok := rt.DivAC(%s, %d); ok {", rest, v.Mult)
		}
	case 2:
		x, y := vars[0], vars[1]
		if x.Mult == 1 && y.Mult == 1 && (known(x.Var) != "" || known(y.Var) != "") {
			// the value of the other occurrence of one of them: one
			// solution at most
			if known(x.Var) == "" {
				x, y = y, x
			}
			ys, w := fmt.Sprintf("ys%d", n), fmt.Sprintf("rt.Elems(%s, %s)", sym, known(x.Var))
			open("if %s, ok := rt.TakeAC(%s, %s, 1); ok && len(%s) > 0 {", ys, rest, w, ys)
			bind(x.Var, w)
			bind(y.Var, ys)
			break
		}
		if x.Mult == 1 && y.Mult == 1 {
			sp := fmt.Sprintf("s%d", n)
			open("for %s := rt.NewSplit(%s); %s.Next(); {", sp, rest, sp)
			bind(x.Var, sp+".X()")
			bind(y.Var, sp+".Y()")
			break
		}
		// x^n U y (REM's maximal extract), y^m U X with X the rest
		// n copies of y (x^n U y^m)
		d, ys := fmt.Sprintf("d%d", n), fmt.Sprintf("ys%d", n)
		open("for %s := rt.NewDown(%s, %d); %s.Next(); {", d, rest, x.Mult, d)
		e.line("%s := %s.Rest()", ys, d)
		if y.Mult == 1 {
			open("if len(%s) > 0 {", ys)
		} else {
			ys1 := ys
			if used[y.Var] {
				ys = fmt.Sprintf("yd%d", n)
				open("if %s, ok := rt.DivAC(%s, %d); len(%s) > 0 && ok {", ys, ys1, y.Mult, ys1)
			} else {
				open("if _, ok := rt.DivAC(%s, %d); len(%s) > 0 && ok {", ys1, y.Mult, ys1)
			}
		}
		bind(x.Var, d+".X()")
		bind(y.Var, ys)
	}
	for _, v := range vars {
		delete(pending, v.Var)
	}
	opens += e.hoist(r, next, pending, used)
	k()
	e.ext, e.late = saved, savedLate
	e.raw, e.lazy = savedRaw, savedLazy
	for range opens {
		e.close("}")
	}
}

// restTaker tells whether the AC pattern p of rule r is matched as REM
// matches F(x, t1, ..., tm) at the top of an unlabelled rule when no
// condition reads x (RewriteRule.java transformation3_2 adds no extension
// variable then: x is not "constrained"): the element patterns t1, ...,
// tm, then x takes the rest, without an extension (the extensions the
// interpreter would try after it give the same result: the conditions do
// not read x). As in REM's code, the rest can be empty during the match:
// the conditions are evaluated (their rewrite steps counted), then the
// rule fails before its right-hand side ("Avoid an empty instance"; the
// interpreter's x is never empty either).
func restTaker(r *ir.Rule, p *ir.Pattern) bool {
	if !p.Ext || p.Abs != nil || len(p.Vars) != 1 || p.Vars[0].Mult != 1 || len(p.Args) == 0 {
		return false
	}
	x := p.Vars[0].Var
	return !slices.ContainsFunc(r.Conds, func(c ir.Cond) bool { return c.Kind != ir.CondAC && condReads(&c, x) })
}

// lateRest is the variable of a [restTaker] pattern: v takes the elements
// rest of the AC operator op.
type lateRest struct {
	v    ir.VarID
	op   ir.OpID
	rest string
}

// lateBind writes, after the conditions of r, the test that the rest of
// e.late is not empty (an if, left open) and the binding of its variable;
// it returns the right-hand side when it builds it (the variable an
// argument of the AC operator at its top and nowhere else: the rest is
// merged with the other arguments, normalised as a whole, as REM does:
// the rest alone is not normalised, programs/ac steps_if3), else "".
// Elsewhere the variable is the normal form of the rest (REM normalises
// it too: steps_rest3).
func (e *emitter) lateBind(r *ir.Rule, used map[ir.VarID]bool) string {
	l := e.late
	e.open("if len(%s) > 0 { // the variable of the AC level is not empty", l.rest)
	op := &e.ir.Ops[l.op]
	if x := &r.Rhs; x.Kind == ir.ExprOp && x.Op == l.op && varCount(x, l.v) == 1 {
		direct := false
		elems := acElems(x, func(a *ir.Expr) bool {
			if a.Kind == ir.ExprVar && a.Var == l.v {
				direct = true
				return true
			}
			return false
		})
		if direct {
			args, mults := e.acGroup(elems, r.Vars)
			if mults != "" {
				return fmt.Sprintf("rt.ExtendACm(%s, %s, %s, %s, %s)", symName(op), acFun(op), l.rest, mults, strings.Join(args, ", "))
			}
			return fmt.Sprintf("rt.ExtendAC(%s, %s, %s, %s)", symName(op), acFun(op), l.rest, strings.Join(args, ", "))
		}
	}
	if used[l.v] {
		e.line("%s := %s", r.Vars[l.v].Name, e.acValue(l.op, l.rest))
	}
	return ""
}

// varCount is the number of occurrences of the variable v in x.
func varCount(x *ir.Expr, v ir.VarID) int {
	n := 0
	if x.Kind == ir.ExprVar && x.Var == v {
		n++
	}
	for i := range x.Args {
		n += varCount(&x.Args[i], v)
	}
	return n
}

// ekerMatch writes the CondEker j: each solution of Eker's matcher (the
// oracles' order), then the rest of the rule.
func (e *emitter) ekerMatch(r *ir.Rule, j int, used map[ir.VarID]bool, depth int) {
	c := &r.Conds[j]
	if o, ok := e.ekerNever(c.Eker); ok {
		e.line("_ = %s", r.Vars[c.A].Name)
		e.open("if false { // no solution: %s is never a normal form", comment(e.ir.Ops[o].Decl))
		for _, v := range c.EkerVars {
			if used[v] {
				e.line("var %s %s", r.Vars[v].Name, e.goType(r.Vars[v].Sort))
			}
		}
		e.conds(r, j+1, used, depth)
		e.close("}")
		return
	}
	n := e.next()
	m := fmt.Sprintf("ek%d", n)
	e.ekerPats = append(e.ekerPats, e.ekerExpr(c.Eker))
	e.line("// Eker's AC matcher, pattern %s (the oracles' order of solutions)", comment(e.ekerText(c.Eker, r, c.EkerVars)))
	e.open("for %s := rt.NewEker(ekerPat%d, %d, %s); %s.Next(); {", m, len(e.ekerPats)-1, len(c.EkerVars), r.Vars[c.A].Name, m)
	for i, v := range c.EkerVars {
		if used[v] {
			e.line("%s := %s", r.Vars[v].Name, e.unbox(r.Vars[v].Sort, fmt.Sprintf("%s.Value(%d, acNorm)", m, i)))
		}
	}
	e.conds(r, j+1, used, depth)
	e.close("}")
}

// ekerNever finds an operator of p that is never a normal form (the
// pattern matches nothing).
func (e *emitter) ekerNever(p *ir.EkerPat) (ir.OpID, bool) {
	if p.Kind == ir.PatOp || p.Kind == ir.PatAC {
		if op := &e.ir.Ops[p.Op]; !op.Variant && op.Kind != ir.BoolConst && op.Kind != ir.Injection {
			return p.Op, true
		}
	}
	for i := range p.Args {
		if o, ok := e.ekerNever(&p.Args[i]); ok {
			return o, true
		}
	}
	return 0, false
}

// ekerExpr is the Go expression of an Eker pattern (rt.EkPat).
func (e *emitter) ekerExpr(p *ir.EkerPat) string {
	if p.Kind == ir.PatOp && e.ir.Ops[p.Op].Kind == ir.Injection {
		return e.ekerExpr(&p.Args[0])
	}
	switch p.Kind {
	case ir.PatVar:
		return fmt.Sprintf("rt.EkVar(%d)", p.Var)
	case ir.PatInt:
		return "rt.EkInt(" + goInt(p.Int) + ")"
	case ir.PatStr:
		return "rt.EkStr(" + goString(p.Str) + ")"
	}
	op := &e.ir.Ops[p.Op]
	sym := symName(op)
	if op.Kind == ir.BoolConst {
		sym = "rt.SymFalse"
		if op.Bool {
			sym = "rt.SymTrue"
		}
	}
	var args []string
	for i := range p.Args {
		a := e.ekerExpr(&p.Args[i])
		if p.Kind == ir.PatAC {
			a = fmt.Sprintf("rt.EkItem{P: %s, Mult: %d}", a, p.Mults[i])
		}
		args = append(args, a)
	}
	if p.Kind == ir.PatAC {
		return "rt.EkAC(" + sym + ", " + strings.Join(args, ", ") + ")"
	}
	return "rt.EkOp(" + strings.Join(append([]string{sym}, args...), ", ") + ")"
}

// ekerText is an Eker pattern for comments (variables by their names,
// repeated items with ^n).
func (e *emitter) ekerText(p *ir.EkerPat, r *ir.Rule, vars []ir.VarID) string {
	switch p.Kind {
	case ir.PatVar:
		return r.Vars[vars[p.Var]].Name
	case ir.PatInt:
		return goInt(p.Int)
	case ir.PatStr:
		return goString(p.Str)
	}
	var args []string
	for i := range p.Args {
		a := e.ekerText(&p.Args[i], r, vars)
		if p.Kind == ir.PatAC && p.Mults[i] > 1 {
			a += fmt.Sprintf("^%d", p.Mults[i])
		}
		args = append(args, a)
	}
	name := e.ir.Ops[p.Op].Name
	if len(args) == 0 {
		return name
	}
	return name + "(" + strings.Join(args, ", ") + ")"
}

// ekerDecls writes the patterns of Eker's matcher and acNorm, the normal
// form of an AC term given by the matcher.
func (e *emitter) ekerDecls() {
	if len(e.ekerPats) == 0 {
		return
	}
	e.line("// The patterns of Eker's AC matcher (variables numbered as REM numbers them).")
	e.open("var (")
	for i, p := range e.ekerPats {
		e.line("ekerPat%d = %s", i, p)
	}
	e.close(")")
	e.line("")
	e.line("// acNorm is the normal form of the AC term of sym over the elements es")
	e.line("// (sorted, two or more), a value given by Eker's matcher.")
	e.open("func acNorm(sym int32, es []rt.Term) rt.Term {")
	var cases []ir.OpID
	for o := range e.ir.Ops {
		if op := &e.ir.Ops[o]; op.AC && op.Variant && op.Kind == ir.Defined {
			cases = append(cases, o)
		}
	}
	if len(cases) > 0 {
		e.open("switch sym {")
		for _, o := range cases {
			e.line("case %s:", symName(&e.ir.Ops[o]))
			e.indent++
			e.line("return %s", e.acValue(o, "es"))
			e.indent--
		}
		e.close("}")
	}
	e.line("return rt.MkAC(sym, es)")
	e.close("}")
	e.line("")
}
