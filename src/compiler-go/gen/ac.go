package gen

import (
	"fmt"
	"strings"

	"elan/ir"
)

// AC matching (S7c task 1): the code of a CondAC condition. The subject
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
		return fmt.Sprintf("rt.NormAC(%s, %s, %s)", symName(op), m, op.Fun)
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
	if len(p.Vars) == 0 && !p.Ext {
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
// extension, then k.
func (e *emitter) acVars(r *ir.Rule, c *ir.Cond, el string, pos []string, n int, next *int, pending, used map[ir.VarID]bool, k func()) {
	p := &c.Pat
	opens := 0
	open := func(format string, a ...any) {
		e.open(format, a...)
		opens++
	}
	need := p.Ext || len(p.Vars) == 2 || len(p.Vars) == 1 && (p.Vars[0].Mult > 1 || used[p.Vars[0].Var])
	rest := el
	if len(pos) > 0 && need {
		rest = fmt.Sprintf("r%d", n)
		e.line("%s := rt.Rest(%s, %s)", rest, el, strings.Join(pos, ", "))
	}
	bind := func(v ir.VarID, m string) {
		if used[v] {
			e.line("%s := %s", r.Vars[v].Name, e.acValue(p.Op, m))
		}
	}
	saved := e.ext
	switch len(p.Vars) {
	case 0:
		if p.Ext {
			e.ext = &extension{p.Op, rest}
		}
	case 1:
		v := p.Vars[0]
		switch {
		case p.Ext:
			d := fmt.Sprintf("d%d", n)
			open("for %s := rt.NewDown(%s, %d); %s.Next(); {", d, rest, v.Mult, d)
			bind(v.Var, d+".X()")
			ext := fmt.Sprintf("ext%d", n)
			e.line("%s := %s.Rest()", ext, d)
			e.ext = &extension{p.Op, ext}
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
		sp := fmt.Sprintf("s%d", n)
		open("for %s := rt.NewSplit(%s); %s.Next(); {", sp, rest, sp)
		bind(p.Vars[0].Var, sp+".X()")
		bind(p.Vars[1].Var, sp+".Y()")
	}
	for _, v := range p.Vars {
		delete(pending, v.Var)
	}
	opens += e.hoist(r, next, pending, used)
	k()
	e.ext = saved
	for range opens {
		e.close("}")
	}
}
