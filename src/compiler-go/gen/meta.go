package gen

import (
	"fmt"

	"elan/ir"
)

// META (S7b task 2, as elanc-rs's S6b): the strategy of set_of
// (Meta_apply[X]) and the evaluation of the Strategy[X] terms
// (Meta_strat[X]), the one construct of the strategy terms that the
// interpreter evaluates with builtins (strbuiltins,
// rewrite/stateofexecution.cc; term2strategy, meta/meta.cc).
//
//   - per sort META is applied to, `func meta_<sort>(t T, k rt.K[T]) bool`:
//     a switch over the meta_apply terms (ir.MetaOp);
//   - per X, `func ev_Strategy_X(s rt.Term, t T, typed bool, k rt.K[T])
//     bool`: the strategy denoted by s applied to t, with its helpers for
//     the Strateg[X] terms (id, call, dc, dk, repeat, iterate) and the
//     lists of strategies of dc and dk.

// The codes of the Meta_strat[X] operators (symbol = code).
const (
	codeStrategiesOne  = 131 // @ : (Strategy[X]) Strategies[X]
	codeStrategiesMore = 132 // @ , @ : (Strategy[X] Strategies[X]) Strategies[X]
	codeDc             = 137 // dc(@) : (Strategies[X]) Strateg[X]
	codeDk             = 138 // dk(@)
	codeRepeat         = 139 // repeat @ endrepeat : (Strategy[X]) Strateg[X]
	codeIterate        = 140 // iterate @ enditerate
	codeStrategyOne    = 141 // @ : (Strateg[X]) Strategy[X]
	codeStrategySeq    = 142 // @ @ : (Strateg[X] Strategy[X]) Strategy[X]
	codeId             = 143 // id : Strateg[X]
	codeCall           = 144 // call @':'X : (builtinString) Strateg[X]
)

// The run-time errors of META, as the interpreter's.
const (
	metaWrongTerm = "[META-built] META strategy over wrong term"
	metaInternal  = "term2strategy internal error"
	metaUnknown   = "[META] call of an unknown strategy"
)

// opOf is the operator with symbol code and result sort s that can be at
// the top of a term (a runtime symbol), -1 if none.
func (e *emitter) opOf(code int32, s ir.SortID) ir.OpID {
	for o := range e.ir.Ops {
		op := &e.ir.Ops[o]
		if op.Symbol == code && op.Result == s && op.Variant {
			return o
		}
	}
	return -1
}

// evalOf is the evaluator of the terms of sort Strategy[X] st, nil if none.
func (e *emitter) evalOf(st ir.SortID) *ir.MetaEval {
	for i := range e.ir.MetaEvals {
		if e.ir.MetaEvals[i].Strategy == st {
			return &e.ir.MetaEvals[i]
		}
	}
	return nil
}

// metas writes the META functions, the evaluators and their helpers.
func (e *emitter) metas() {
	p := e.ir
	if len(p.MetaApplies) == 0 && len(p.MetaEvals) == 0 {
		return
	}
	e.line("// ==== META (Meta_apply, Meta_strat) ====")
	e.line("")
	for i := range p.MetaApplies {
		e.metaApply(&p.MetaApplies[i])
	}
	for i := range p.MetaEvals {
		e.metaEval(&p.MetaEvals[i])
	}
	e.line("// metaFatal stops the program with a run-time error of META, as the")
	e.line("// interpreter (status 1).")
	e.open("func metaFatal(msg string) bool {")
	e.line("rt.Fatal(msg)")
	e.line("return false")
	e.close("}")
	e.line("")
	e.line("// metaInt is the value of a builtinInt argument of a meta_apply term.")
	e.open("func metaInt(x rt.Term) int {")
	e.open("if !rt.IsInt(x) {")
	e.line("metaFatal(%q)", metaWrongTerm)
	e.close("}")
	e.line("return int(x.Int)")
	e.close("}")
	e.line("")
	e.line("// metaStr is the value of the builtinString argument of a call.")
	e.open("func metaStr(x rt.Term) string {")
	e.open("if !rt.IsStr(x) {")
	e.line("metaFatal(%q)", metaInternal)
	e.close("}")
	e.line("return rt.StrOf(x)")
	e.close("}")
	e.line("")
}

// metaApply writes the function of META on a sort.
func (e *emitter) metaApply(m *ir.MetaApply) {
	p := e.ir
	ty := e.goType(m.Sort)
	e.line("// %s: the strategy META on %s: the meta_apply terms (strbuiltins,", m.Fun, comment(p.Sorts[m.Sort].Name))
	e.line("// rewrite/stateofexecution.cc); another term is a run-time error.")
	e.open("func %s(t %s, k rt.K[%s]) bool {", m.Fun, ty, ty)
	e.line("rt.Guard()")
	e.line("defer rt.Unguard()")
	if _, native := p.Native(m.Sort); !native {
		e.line("switch t.Sym {")
		for o := range p.Ops {
			op := &p.Ops[o]
			if op.Meta == ir.MetaNone || op.Result != m.Sort || !op.Variant {
				continue
			}
			ev := e.evalOf(op.Args[0])
			if ev == nil {
				continue
			}
			e.line("case %s: // %s", symName(op), comment(op.Decl))
			e.indent++
			e.metaCase(op, ev)
			e.indent--
		}
		e.line("}")
	}
	e.line("return metaFatal(%q)", metaWrongTerm)
	e.close("}")
	e.line("")
}

// metaCase is the code of META on the term t, with meta_apply operator op
// at the top.
func (e *emitter) metaCase(op *ir.Op, ev *ir.MetaEval) {
	p := e.ir
	x := ev.Term
	tx := e.goType(x)
	switch op.Meta {
	case ir.MetaAll:
		e.line("// meta_apply(s, t): every result of s on t")
		e.line("return %s(t.Args[0], %s, true, k)", ev.Fun, e.unbox(x, "t.Args[1]"))
	case ir.MetaNth:
		e.line("// meta_apply(s, t, n): result number n of s on t (0 first), if any")
		e.line("n := metaInt(t.Args[2])")
		e.line("var res %s", tx)
		e.line("found := false")
		e.open("if n >= 0 {")
		e.open("%s(t.Args[0], %s, true, func(r %s) bool {", ev.Fun, e.unbox(x, "t.Args[1]"), tx)
		e.open("if n == 0 {")
		e.line("res, found = r, true")
		e.line("return true")
		e.close("}")
		e.line("n--")
		e.line("return false")
		e.close("})")
		e.close("}")
		e.line("return found && k(res)")
	case ir.MetaList:
		e.line("// meta_apply(s, t.nil, from, n) (set_of): the list of the results of s on t,")
		e.line("// from number from (0 first), n of them (all when n is 0)")
		l := op.Args[1]
		var conses, nils []ir.OpID
		for c := range p.Ops {
			o := &p.Ops[c]
			if o.Result != l || !o.Variant {
				continue
			}
			if len(o.Args) == 2 && o.Args[0] == x && o.Args[1] == l {
				conses = append(conses, c)
			} else if len(o.Args) == 0 {
				nils = append(nils, c)
			}
		}
		// the sort of t is unknown for a number or a string (no symbol)
		typed := "true"
		if f := p.FamilyOf(x); f != nil && f.Base == x && f.Kind != ir.KindBool {
			switch {
			case !f.Boxed():
				typed = "false"
			case f.Kind == ir.KindInt:
				typed = "!rt.IsInt(l.Args[0])"
			default:
				typed = "!rt.IsStr(l.Args[0])"
			}
		}
		e.line("from, n := metaInt(t.Args[2]), metaInt(t.Args[3])")
		e.line("all := n == 0")
		e.open("if !all {")
		e.line("n += from")
		e.close("}")
		e.line("l := t.Args[1]")
		for _, c := range conses {
			for _, nl := range nils {
				e.open("if l.Sym == %s && l.Args[1].Sym == %s {", symName(&p.Ops[c]), symName(&p.Ops[nl]))
				e.line("var res []%s", tx)
				e.open("if n > 0 || all {")
				e.open("%s(t.Args[0], %s, %s, func(r %s) bool {", ev.Fun, e.unbox(x, "l.Args[0]"), typed, tx)
				e.line("n--")
				e.open("if from > 0 {")
				e.line("from--")
				e.close("} else {")
				e.indent++
				e.line("res = append(res, r)")
				e.close("}")
				e.line("return !(n > 0 || all)")
				e.close("})")
				e.close("}")
				e.line("acc := %s()", p.Ops[nl].Fun)
				e.open("for i := len(res) - 1; i >= 0; i-- {")
				e.line("acc = %s(res[i], acc)", p.Ops[c].Fun)
				e.close("}")
				e.line("return k(acc)")
				e.close("}")
			}
		}
	}
}

// metaEval writes the evaluator of the Strategy[X] terms and its helpers.
func (e *emitter) metaEval(ev *ir.MetaEval) {
	p := e.ir
	tx := e.goType(ev.Term)
	params := fmt.Sprintf("(s rt.Term, t %s, typed bool, k rt.K[%s]) bool", tx, tx)
	step := ev.Strateg >= 0
	e.line("// %s: the strategy denoted by s, a term of sort %s, applied to t", ev.Fun, comment(p.Sorts[ev.Strategy].Name))
	e.line("// (term2strategy, meta/meta.cc): a sequence of steps; typed: the sort of t is")
	e.line("// known (false for a number or a string), it chooses the strategy of a call.")
	e.open("func %s%s {", ev.Fun, params)
	e.line("rt.Guard()")
	e.line("defer rt.Unguard()")
	e.line("switch s.Sym {")
	if o := e.opOf(codeStrategyOne, ev.Strategy); o >= 0 && step {
		e.line("case %s: // one step", symName(&p.Ops[o]))
		e.line("\treturn %s(s.Args[0], t, typed, k)", ev.StrategFun)
	}
	if o := e.opOf(codeStrategySeq, ev.Strategy); o >= 0 && step {
		e.line("case %s: // a step, then the rest", symName(&p.Ops[o]))
		e.indent++
		e.line("rest := s.Args[1]")
		e.line("return %s(s.Args[0], t, typed, func(r %s) bool { return %s(rest, r, typed, k) })", ev.StrategFun, tx, ev.Fun)
		e.indent--
	}
	e.line("}")
	e.line("return metaFatal(%q)", metaInternal)
	e.close("}")
	e.line("")
	if !step {
		return
	}
	lists := ev.Strategies >= 0 && (e.opOf(codeDc, ev.Strateg) >= 0 || e.opOf(codeDk, ev.Strateg) >= 0)
	e.line("// %s: the strategy denoted by s, a term of sort %s, applied to t.", ev.StrategFun, comment(p.Sorts[ev.Strateg].Name))
	e.open("func %s%s {", ev.StrategFun, params)
	e.line("rt.Guard()")
	e.line("defer rt.Unguard()")
	e.line("switch s.Sym {")
	if o := e.opOf(codeId, ev.Strateg); o >= 0 {
		e.line("case %s: // id", symName(&p.Ops[o]))
		e.line("\treturn k(t)")
	}
	if o := e.opOf(codeCall, ev.Strateg); o >= 0 {
		e.line("case %s: // call \"name\":%s, by name and sort (meta.cc, commit 93ec493)", symName(&p.Ops[o]), comment(p.Sorts[ev.Term].Name))
		e.indent++
		e.line("switch metaStr(s.Args[0]) {")
		for _, c := range ev.Calls {
			typed := p.Strategies[c.Typed].Fun
			e.line("case %s:", goString(c.Name))
			e.indent++
			switch {
			case c.Untyped >= 0 && c.Untyped != c.Typed:
				e.open("if typed {")
				e.line("return %s(t, k)", typed)
				e.close("}")
				e.line("// a number or a string: the strategy of the name, any sort")
				e.line("return %s(t, k)", p.Strategies[c.Untyped].Fun)
			case c.UntypedWhy != "":
				e.open("if !typed {")
				e.line("rt.Unsupported(%s)", goString(c.UntypedWhy))
				e.close("}")
				e.line("return %s(t, k)", typed)
			default:
				e.line("return %s(t, k)", typed)
			}
			e.indent--
		}
		e.line("}")
		e.line("return metaFatal(%q)", metaUnknown)
		e.indent--
	}
	body := func(f string) string {
		return fmt.Sprintf("func(t %s, k rt.K[%s]) bool { return %s(body, t, typed, k) }", tx, tx, f)
	}
	if o := e.opOf(codeRepeat, ev.Strateg); o >= 0 {
		e.line("case %s: // repeat s endrepeat: repeat*(s)", symName(&p.Ops[o]))
		e.line("\tbody := s.Args[0]")
		e.line("\treturn rt.RepeatStar(t, k, %s)", body(ev.Fun))
	}
	if o := e.opOf(codeIterate, ev.Strateg); o >= 0 {
		e.line("case %s: // iterate s enditerate: iterate*(s)", symName(&p.Ops[o]))
		e.line("\tbody := s.Args[0]")
		e.line("\treturn rt.IterateStar(t, k, %s)", body(ev.Fun))
	}
	if lists {
		if o := e.opOf(codeDc, ev.Strateg); o >= 0 {
			e.line("case %s: // dc(s1, ..., sn): the results of the first si that has one", symName(&p.Ops[o]))
			e.line("\treturn rt.First(t, k, %s(s.Args[0], typed)...)", ev.ListFun)
		}
		if o := e.opOf(codeDk, ev.Strateg); o >= 0 {
			e.line("case %s: // dk(s1, ..., sn): the results of every si", symName(&p.Ops[o]))
			e.line("\treturn rt.Dk(t, k, %s(s.Args[0], typed)...)", ev.ListFun)
		}
	}
	e.line("}")
	e.line("return metaFatal(%q)", metaInternal)
	e.close("}")
	e.line("")
	if !lists {
		return
	}
	one, more := e.opOf(codeStrategiesOne, ev.Strategies), e.opOf(codeStrategiesMore, ev.Strategies)
	e.line("// %s: the strategies of a term of sort %s (s1, ..., sn).", ev.ListFun, comment(p.Sorts[ev.Strategies].Name))
	e.open("func %s(l rt.Term, typed bool) []rt.Strat[%s] {", ev.ListFun, tx)
	e.open("strat := func(s rt.Term) rt.Strat[%s] {", tx)
	e.line("return func(t %s, k rt.K[%s]) bool { return %s(s, t, typed, k) }", tx, tx, ev.Fun)
	e.close("}")
	e.line("var ss []rt.Strat[%s]", tx)
	if more >= 0 {
		e.open("for l.Sym == %s {", symName(&p.Ops[more]))
		e.line("ss = append(ss, strat(l.Args[0]))")
		e.line("l = l.Args[1]")
		e.close("}")
	}
	end := "true" // no list of one strategy: never well formed
	if one >= 0 {
		end = "l.Sym != " + symName(&p.Ops[one])
	}
	e.open("if %s {", end)
	e.line("metaFatal(%q)", metaInternal)
	e.close("}")
	e.line("return append(ss, strat(l.Args[0]))")
	e.close("}")
	e.line("")
}
