package lr

import (
	"fmt"

	"elan/front/lex"
	"elan/front/tables"
)

// Source is the stream a parser reads (lstream &f in include.parser):
// lex.Stream, and the preprocessor's stream, which overrides Ilex and
// BeforeMess (virtual in the C++).
type Source interface {
	Fulex() lex.Lexem
	Ilex() lex.Lexem
	Oerr(parts ...string)
	BeforeMess(lex.Lexem)
	Ctx() *lex.Context
}

// Return codes of a semantics (base/constants.h), and the Go-only Suspend.
const (
	ErrorIm     = 1 // ERRORIM
	AcceptIm    = 2 // ACCEPTIM
	NormCont    = 3 // NORMCONT
	HandErrorIm = 4 // HANDERRORIM
	// Suspend stops the parse at once, no further read: the loader skeleton
	// stops a module before its body (S8a); S8b removes its use.
	Suspend = 100
)

// Result is PARSER's result: 0 (syntax or semantic error), 1, or Suspended.
type Result int

const (
	Rejected  Result = 0
	Accepted  Result = 1
	Suspended Result = 2
)

// SatSts is SATSTS, the size of the parser stack.
const SatSts = 1500

// LexToModLex is lextomodlex (load/msemact.cc): a reserved word (in mrwt)
// stays itself, any other identifier becomes IDENTIFIER, a number NUMBER, a
// string STRING; the rest is unchanged.
func LexToModLex(c *lex.Context, mrwt *tables.MiTab) func(lex.Lexem) lex.Lexem {
	return func(l lex.Lexem) lex.Lexem {
		switch {
		case l.IsIdent():
			if mrwt.Member(l.IdVal()) {
				return l
			}
			return lex.IdentLexem
		case l.IsNum():
			return lex.JustNumber
		case l.IsString():
			return lex.StringLexem
		}
		return l
	}
}

// CexLexToLex is cexlextolex (lex/mlstream.cc): a number becomes NUMBER.
func CexLexToLex(l lex.Lexem) lex.Lexem {
	if l.IsNum() {
		return lex.JustNumber
	}
	return l
}

// SymName is maccsymtolex + alfsy: the printing form of a table symbol.
func SymName(c *lex.Context, sym int32) string { return c.Alfsy(lex.Lexem(sym)) }

// errcommon (include.parser): the next lexem, read with the stream's ilex,
// and the lexems before.
func errcommon(f Source) {
	s := f.Ilex()
	f.Ctx().D.Errs("\ton symbol '", f.Ctx().Alfsy(s), "'")
	f.BeforeMess(s)
}

// syerror (include.parser).
func syerror(t *Table, f Source, st int) {
	f.Oerr()
	c := f.Ctx()
	c.D.Errs("[PARSER] syntax error ")
	errcommon(f)
	c.D.Errs("\tone of (")
	for _, a := range t.States[st].Pr {
		c.D.Errs(" '", SymName(c, a.Sym), "' ")
	}
	c.D.Errs(") expected \n")
}

// Parse is PARSER (include.parser), line by line, from state 0: t the
// table, f the stream, ilexem the ILEXEM of the grammar (ilex, or pilex for
// ppexgram), ltol the lexem mapping, sem the semantics SEMACT(n, lastl):
// called with 0 first, with -2 on accept and with the code of each
// reduction, its result is ignored for 0 and -2.
func Parse(t *Table, f Source, ilexem func() lex.Lexem, ltol func(lex.Lexem) lex.Lexem, sem func(n int, l lex.Lexem) int) Result {
	var stack [SatSts]int
	istack := 0
	state := 0
	lastl := lex.NoLexem // lastl.crendofstreamlex()
	ls := ltol(f.Fulex())
	sem(0, lastl)
	for {
		pr := t.States[state].Pr
		j := len(pr)
		if j == 0 {
			syerror(t, f, state)
			return Rejected
		}
		// FFOUND: the first entry on ls, else the last one
		k := 0
		for k < j-1 && ls != lex.Lexem(pr[k].Sym) {
			k++
		}
		p := pr[k]
		if ls != lex.Lexem(p.Sym) {
			if pr[0].Sym == Default {
				p = pr[0]
			} else {
				syerror(t, f, state)
				return Rejected
			}
		}
		switch p.Kind {
		case Accept:
			sem(-2, lastl)
			return Accepted
		case SRErr, RRErr:
			f.Oerr("[PARSER] syntax error\n")
			c := f.Ctx()
			c.D.Errs("\ton symbol '", c.Alfsy(lastl), "' ")
			c.D.Errs(fmt.Sprintf("in state %d;\n", state))
			kind := "reduce"
			if p.Kind == SRErr {
				kind = "shift"
			}
			c.D.Errs("\t there was an ", kind, "/reduce conflict in this place")
			return Rejected
		case Shift:
			if istack >= SatSts {
				f.Oerr("[PARSER] stack overflow ")
				return Rejected
			}
			stack[istack] = state
			istack++
			state = int(p.Num)
			lastl = ilexem()
			ls = ltol(f.Fulex())
		case Reduction:
			switch sem(int(p.Num), lastl) {
			case ErrorIm:
				f.Oerr()
				f.Ctx().D.Errs("[PARSER] semaction error ")
				errcommon(f)
				return Rejected
			case AcceptIm:
				return Accepted
			case HandErrorIm:
				return Rejected
			case Suspend:
				return Suspended
			}
			ls = ltol(f.Fulex())
			istack -= int(p.Len)
			state = stack[istack]
			istack++
			// RFOUND: the first goto on the left side, else the last one
			// (the entry after the state's gotos when it has none, as the
			// C pointer reads it)
			ak := t.States[state].Ak
			if len(ak) == 0 {
				ak = ak[:1]
			}
			g := 0
			for g < len(ak)-1 && p.Left != ak[g].Sym {
				g++
			}
			state = int(ak[g].To)
		default:
			// the C switch has no default: the loop goes on in the same state
		}
	}
}
