// Printer of a Program back to .ref text, in the layout of the interpreter's
// writer (ref/ref.cc, driver/ldmain.cc) except for the comments (/*i name*/
// before the rules, /**/ in strategy calls), which carry no information.

package ref

import (
	"fmt"
	"strings"
)

// Print prints a program as .ref text: the bytes of the exporter's file,
// comments aside.
func Print(p *Program) []byte {
	var o strings.Builder
	table(&o, "Identifiers", p.Identifiers)
	sorts := make([]TableEntry, len(p.Sorts))
	for i, s := range p.Sorts {
		sorts[i] = TableEntry{Index: s.Index, Name: s.Name}
	}
	table(&o, "Sorts", sorts)
	table(&o, "Modules", p.Modules)
	table(&o, "RuleNames", p.RuleNames)
	table(&o, "StrategyNames", p.StrategyNames)
	for _, d := range p.Grammars.Dumps() {
		grammarDump(&o, d)
	}
	for i := range p.Rules {
		rule(&o, &p.Rules[i])
		o.WriteString(" end\n")
	}
	o.WriteString("EndDef end\n")
	for _, s := range p.Strategies {
		fmt.Fprintf(&o, "STRATEGY(%d,%d,%d,", s.Name, s.Sort, s.Module)
		seq(&o, s.Body)
		o.WriteString(") \nend\n")
	}
	o.WriteString("EndDef end\n")
	q := &p.Query
	fmt.Fprintf(&o, "QUERY(%d,%d,%d,", q.Sort, q.ResultSort, q.Strategy.Value())
	term(&o, q.StartWith)
	o.WriteByte(',')
	term(&o, q.CheckWith)
	o.WriteString(") end \n")
	return []byte(o.String())
}

// PrintTerm prints one term as the writer does.
func PrintTerm(t Term) string {
	var o strings.Builder
	term(&o, t)
	return o.String()
}

// PrintStrategy prints a strategy body as the writer does.
func PrintStrategy(s StrategySeq) string {
	var o strings.Builder
	seq(&o, s)
	return o.String()
}

func table(o *strings.Builder, heading string, t []TableEntry) {
	o.WriteString(heading)
	o.WriteByte('\n')
	for _, e := range t {
		fmt.Fprintf(o, "%d:\"%s\".\n", e.Index, e.Name)
	}
	o.WriteString("nil\nend\n\n")
}

func b2i(b bool) int {
	if b {
		return 1
	}
	return 0
}

func grammarDump(o *strings.Builder, d []SortGrammar) {
	for _, g := range d {
		fmt.Fprintf(o, "GrammarForSort %d:%d:\n", g.Sort, b2i(g.Builtin))
		for _, op := range g.Operators {
			fmt.Fprintf(o, "%d:%d:%d:%d:%d:%d:%d:", op.Flag, op.Symbol, op.Priority,
				op.Syntax, op.Semantic, op.Infos, op.Defstrat.Encode())
			for _, l := range op.Rhs {
				lexem(o, l)
				o.WriteByte('.')
			}
			o.WriteString("nil:")
			for _, s := range op.LocalStrategies {
				fmt.Fprintf(o, "%d.", s)
			}
			o.WriteString("nil.\n")
		}
		o.WriteString("nil end\n")
	}
	o.WriteString("EndDef end\n")
}

func lexem(o *strings.Builder, l Lexem) {
	if l.Kind == LexBlank {
		o.WriteString("Blank")
		return
	}
	fmt.Fprintf(o, "%s(%d)", l.Kind, l.N)
}

func term(o *strings.Builder, t Term) {
	switch t := t.(type) {
	case Var:
		fmt.Fprintf(o, " VAR(%d,%d)", t.Index, t.Sort)
	case EVar:
		fmt.Fprintf(o, " EVAR(%d,%d)", t.Index, t.Sort)
	case Int:
		fmt.Fprintf(o, "INT(%d)", t.Value)
	case Ident:
		fmt.Fprintf(o, "IDENT(%d)", t.Index)
	case Str:
		o.WriteString("STRING(")
		codes(o, t.Codes)
		o.WriteByte(')')
	case Fsym:
		if _, isInt := oneInt(t.Args); t.Symbol == 144 && isInt {
			// a strategy call of --cexport: FSYM(/**/INT(s).nil,144), with
			// neither the space before the argument nor the one before the
			// symbol (ref/ref.cc)
			o.WriteString("FSYM(")
			term(o, t.Args[0])
			o.WriteString(".nil,144)")
			return
		}
		o.WriteString("FSYM(")
		sep := byte(' ')
		for _, a := range t.Args {
			o.WriteByte(sep)
			sep = '.'
			term(o, a)
		}
		fmt.Fprintf(o, "%cnil, %d)", sep, t.Symbol)
	default:
		panic(fmt.Sprintf("ref: unknown term %T", t))
	}
}

func oneInt(args []Term) (Int, bool) {
	if len(args) != 1 {
		return Int{}, false
	}
	i, ok := args[0].(Int)
	return i, ok
}

func codes(o *strings.Builder, cs []int32) {
	for _, c := range cs {
		fmt.Fprintf(o, "%d.", c)
	}
	o.WriteString("nil")
}

func rule(o *strings.Builder, r *Rule) {
	if _, plain := r.Body.(Plain); plain {
		o.WriteString("\nRULE(\n")
	} else {
		o.WriteString("\nSWRULE(\n")
	}
	if r.Name.Valid {
		fmt.Fprintf(o, "%d,", r.Name.Index)
	} else {
		o.WriteString("-1,\n")
	}
	fmt.Fprintf(o, "%d,%d,%d,%d,%d,", r.Sort, r.Module, r.Infos, r.WhichMatch, r.Nvars)
	term(o, r.Lhs)
	o.WriteString(",\n")
	switch b := r.Body.(type) {
	case Plain:
		term(o, b.Rhs)
		o.WriteString(",\n")
		wheres(o, b.Wheres)
	case Switch:
		switch_(o, b)
	default:
		panic(fmt.Sprintf("ref: unknown rule body %T", b))
	}
	o.WriteString(")\n\n")
}

func wheres(o *strings.Builder, ws []Where) {
	for _, w := range ws {
		where(o, w)
		o.WriteString(".\n")
	}
	o.WriteString("nil")
}

func where(o *strings.Builder, w Where) {
	switch w := w.(type) {
	case Try:
		o.WriteString("TRY(\n")
		for _, b := range w.Branches {
			wheres(o, b)
			o.WriteByte('.')
		}
		o.WriteString("nil)\n")
	case If:
		o.WriteString("IFF(")
		term(o, w.Cond)
		o.WriteByte(')')
	case Match:
		o.WriteString("PWHERE(")
		term(o, w.Pattern)
		fmt.Fprintf(o, ",%d,%d,", w.Sort, w.Strategy.Value())
		term(o, w.Rhs)
		o.WriteByte(')')
	case Assign:
		o.WriteString("WHERE(")
		// the writer prints the variable without the leading blank of terms
		o.WriteString(strings.TrimLeft(PrintTerm(w.Lhs), " \t\n\f\r"))
		fmt.Fprintf(o, ",%d,", w.Strategy.Value())
		term(o, w.Rhs)
		o.WriteByte(')')
	default:
		panic(fmt.Sprintf("ref: unknown condition %T", w))
	}
}

func switch_(o *strings.Builder, s Switch) {
	switch s := s.(type) {
	case NoSwitch:
		o.WriteString("NOSWITCH(")
		wheres(o, s.Wheres)
		o.WriteByte(',')
		term(o, s.Result)
		o.WriteByte(')')
	case SwitchCases:
		o.WriteString("SWITCH(")
		wheres(o, s.Wheres)
		o.WriteByte(',')
		for _, b := range s.Branches {
			term(o, b.Test)
			o.WriteByte(',')
			switch_(o, b.Body)
			o.WriteByte('.')
		}
		o.WriteString("nil)")
	default:
		panic(fmt.Sprintf("ref: unknown switch %T", s))
	}
}

func seq(o *strings.Builder, s StrategySeq) {
	for i, e := range s {
		if i > 0 {
			o.WriteString(" ; ")
		}
		strat(o, e)
	}
}

func strat(o *strings.Builder, s StrategyExpr) {
	switch {
	case s.Op == SId:
		o.WriteString(" id ")
	case s.Op == SFail:
		o.WriteString(" fail ")
	case s.Op == SMeta:
		o.WriteString("META")
	case s.Op.IsRules():
		fmt.Fprintf(o, "%s(", s.Op)
		for _, r := range s.Rules {
			fmt.Fprintf(o, "%d.", r)
		}
		o.WriteString("nil)")
	case s.Op.IsStrats():
		fmt.Fprintf(o, "%s(", s.Op)
		for i, b := range s.Alts {
			if i > 0 {
				o.WriteString(" , ")
			}
			seq(o, b)
		}
		o.WriteByte(')')
	case s.Op.IsUnary():
		fmt.Fprintf(o, "%s(", s.Op)
		seq(o, s.Body)
		o.WriteByte(')')
	case s.Op == SCall:
		fmt.Fprintf(o, "call(%d)", s.Call)
	default: // dccall, dkcall
		fmt.Fprintf(o, "%s(", s.Op)
		codes(o, s.Name)
		fmt.Fprintf(o, ",%d,%d)", s.Max, s.Sort)
	}
}
