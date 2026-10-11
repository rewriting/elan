package gen

import (
	"fmt"
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
// and(a, a, t) with and(x, x) => x).
//
// The conditions after the matching that give at most one solution and
// bind nothing (non-linearity checks, ifs, local assignments) are written
// as soon as the variables they read are bound, in their order (the
// matching is pruned: efib's compute rule tests n1 == n2 + 1 before
// choosing the third element); the others after the whole match. Every
// failure falls through to the next iteration: the solutions are tried in
// order, each one passed to the rest of the rule.

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

// hoist writes the conditions from *next on that can be written now: those
// of [emitter.simpleCond] whose variables are not pending, in order (it
// stops at the first other one). It returns the number of ifs opened.
func (e *emitter) hoist(r *ir.Rule, next *int, pending map[ir.VarID]bool, used map[ir.VarID]bool) int {
	opens := 0
	for ; *next < len(r.Conds); *next++ {
		c := &r.Conds[*next]
		vars := map[ir.VarID]bool{}
		switch c.Kind {
		case ir.CondSame:
			vars[c.A], vars[c.B] = true, true
		case ir.CondIf, ir.CondLet:
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
	e.acLevel(r, j, end, &next, pending, used, func() { e.conds(r, next, used, depth) })
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
	bind := func(v ir.VarID, m string) {
		switch {
		case p.Abs != nil && v == p.Abs.Var:
			// the subject of Eker's matcher: the elements as they are (a
			// part of a term being normalised can be reducible; the
			// interpreter matches it as it is, REM normalises it first)
			e.line("%s := rt.MkAC(%s, %s)", r.Vars[v].Name, symName(&e.ir.Ops[p.Op]), m)
		case used[v]:
			e.line("%s := %s", r.Vars[v].Name, e.acValue(p.Op, m))
		}
	}
	saved := e.ext
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
	e.ext = saved
	for range opens {
		e.close("}")
	}
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
