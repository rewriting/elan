package macro

import (
	"fmt"

	"elan/front/lex"
	"elan/front/tables"
)

// pppendofin: the end of a condition term.
func (s *Stream) pppendofin(l lex.Lexem) bool {
	st := s.st
	return l.IsEOS() || l == ch(':') || l == st.rwAnd || l == st.rwAndIf || l == st.rwIf
}

// addvarid: a FOR EACH variable.
func (s *Stream) addvarid(l lex.Lexem) {
	st := s.st
	if st.varNamesI >= tables.MaxNofVar {
		s.d().Errs("\n too much variables in 'FOR EACH'\n")
		s.d().Errs(fmt.Sprintf("more then MAXNOFVAR=%d\n", tables.MaxNofVar))
		s.d().Fail()
	}
	st.wasAffected[st.varNamesI] = 'n'
	st.varNames[st.varNamesI] = l
	st.varNamesI++
}

// addvars: the sort of the variables read since the last call.
func (s *Stream) addvars(sort string) {
	if !s.ev.AddVars(s, sort) {
		s.Oerr("[preprocessor] unknown type ", sort, " used")
		s.BeforeMess(s.mflex)
		s.d().Fail()
	}
	st := s.st
	for ; st.varTabI < st.varNamesI; st.varTabI++ {
		st.varTypeTab[st.varTabI] = sort
	}
}

// parsetype: a sort name, `s` or `s[a,b[c]]`.
func (s *Stream) parsetype() string {
	s.expectident()
	fl := s.collpilex()
	n := s.Ctx().Alfsy(fl)
	if s.mflex == ch('[') {
		s.collpilex()
		n += "[" + s.parsetype()
		for s.mflex == ch(',') {
			s.collpilex()
			n += "," + s.parsetype()
		}
		s.expect(']')
		s.collpilex()
		n += "]"
	}
	return n
}

func (s *Stream) vars(from, to int) []Var {
	st := s.st
	var v []Var
	for i := from; i < to; i++ {
		v = append(v, Var{Name: st.varNames[i], Sort: st.varTypeTab[i], Index: i - from})
	}
	return v
}

// forEach is the FOR EACH / IF branch of macroexp (fl is FOR, mflex EACH;
// or fl is IF).
func (s *Stream) forEach(fl lex.Lexem) {
	st := s.st
	var tmpb lex.Buffer
	bvarnamesi := st.varNamesI // for the case of nested FOR EACH
	st.varNamesI = st.varTabI
	bvartabi := st.varTabI
	iffirst := false
	f := &ForEach{If: fl == st.rwIf}

	if fl != st.rwIf {
		for {
			fl = s.Ilex()
			if s.mflex != ch(':') {
				s.expectident()
				fl = s.Ilex()
				s.addvarid(fl)
				for s.mflex == ch(',') {
					s.Ilex()
					s.expectident()
					fl = s.Ilex()
					s.addvarid(fl)
				}
			}
			s.expect(':')
			s.collpilex()
			s.addvars(s.parsetype())
			if s.mflex != ch(';') {
				break
			}
		}
		s.ev.AddVarRules(s, s.vars(bvarnamesi, st.varTabI))
		if s.mflex != st.rwSuch {
			s.Oerr("\tSUCH expected ")
			s.insteaderr()
			s.BeforeMess(s.mflex)
			s.d().Fail()
		}
		s.collpilex()
		if s.mflex != st.rwThat {
			s.Oerr("\tTHAT expected ")
			s.insteaderr()
			s.BeforeMess(s.mflex)
			s.d().Fail()
		}
	} else {
		iffirst = true
	}
	f.Vars = s.vars(bvarnamesi, st.varTabI)
	for {
		c := Cond{Var: -1}
		if !iffirst {
			iffirst = false
			fl = s.collpilex()
		}
		if fl == st.rwAnd && s.mflex == st.rwIf {
			fl = s.collpilex()
		}
		var v *Var
		if fl == st.rwAndIf || fl == st.rwIf {
			// IFVARN, the sort bool
		} else {
			s.expectident()
			fl = s.collpilex()
			i := bvarnamesi
			for i < st.varTabI && st.varNames[i] != fl {
				i++
			}
			if i >= st.varTabI {
				s.Oerr("[preprocesor] variable name expected after SUCH THAT or AND instead of ", s.Ctx().Alfsy(fl))
				s.BeforeMess(s.mflex)
				s.d().Fail()
			}
			st.wasAffected[i] = 'y'
			c.Var = i - bvarnamesi
			v = &f.Vars[c.Var]
			s.expect(':')
			s.collpilex()
			s.expect('=')
			s.collpilex()
			s.expect('(')
			s.collpilex()
			if s.mflex.IsIdent() {
				c.Strategy = s.collpilex()
				s.ev.Strategy(s, *v, c.Strategy)
			}
			s.expect(')')
			s.collpilex()
		}
		t, ok := s.ev.Term(s, v, s.pppendofin)
		if !ok {
			s.d().Fail()
		}
		c.Term = t
		f.Conds = append(f.Conds, c)
		if !(s.mflex == st.rwAnd || s.mflex == st.rwAndIf || s.mflex == st.rwIf) {
			break
		}
	}
	for i := st.varTabI - 1; i >= bvarnamesi; i-- {
		if st.wasAffected[i] == 'n' {
			s.Oerr("[preprocessor] non affected variable ", s.Ctx().Alfsy(st.varNames[i]), " in FOR EACH")
			s.BeforeMess(s.mflex)
			s.d().Fail()
		}
		s.ev.DeleteVarRules(s, []Var{f.Vars[i-bvarnamesi]})
	}
	s.expect(':')
	s.Pilex()
	s.expect('{')
	s.fillmacbuf(&tmpb)
	f.Body = append([]lex.Lexem(nil), tmpb.Lexems()...)
	s.mflex = lex.BlankLexem
	s.mark("@foreach-begin")
	s.addForEach(&tmpb, s.ev.Solutions(s, f), bvarnamesi)
	st.varTabI = bvartabi
	st.varNamesI = bvarnamesi
}
