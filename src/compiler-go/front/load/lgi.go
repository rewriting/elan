package load

import (
	"fmt"

	"elan/front/lex"
	"elan/front/lr"
)

// The stop predicates of the terms of the .lgi and .spc (ldmain.cc).
func (L *loader) qendofin(l lex.Lexem) bool   { return l == L.idLex("end") || l.IsEOS() }
func (L *loader) qchendofin(l lex.Lexem) bool { return l == L.idLex("start") || l.IsEOS() }
func (L *loader) axchendofin(l lex.Lexem) bool {
	return l == L.idLex("end") || l == L.idLex("part") || l.IsEOS()
}
func (L *loader) sendofin(l lex.Lexem) bool {
	return l == L.idLex("end") || l == lex.CharLex(']') || l.IsEOS()
}

// popmn takes the module name built by mkmodname (actmn), as the actions
// that CFRE(actmn) do.
func (L *loader) popmn() string {
	if len(L.actmn) == 0 {
		L.internal("no module name at an .lgi action")
	}
	s := L.actmn[len(L.actmn)-1]
	L.actmn = nil
	return s
}

// ldsemact (ldmain.cc): the actions of ldmodgram that name, import and open
// things; the terms are consumed lexically.
func (L *loader) ldsemact(f *lex.Stream, n int, l lex.Lexem) int {
	switch n {
	case 0: // initial action
		L.oldaxiomname = nil
		L.actmn = nil
	case 1: // the name of a part
		L.axiomname = L.c.Alfsy(l)
		if L.oldaxiomname != nil {
			L.handleaxiom()
		}
	case 2: // name of the LPL
		if L.modname != L.c.Alfsy(l) {
			f.Oerr(" name of logic doesn't correspond to name of file\n")
			L.d.Fail()
		}
	case 4: // a module to import
		if len(L.actmn) == 0 {
			L.internal("no module name at .lgi action 4")
		}
		L.importmodInf(L.actmn[len(L.actmn)-1], f)
		name := L.axiomname
		L.oldaxiomname = &name
		L.actmn = nil
	case 5: // the last specification description part
		L.axiomname = "end"
		L.handleaxiom()
	case 6: // query without check with
		L.importmodInf("bool", f)
	case 9: // sort of "result of" part for the query
		L.qresulttypei = L.typet.AddStr(L.popmn())
	case 10: // "start with" for the query: the term up to `end`
		skipTerm(f, L.qendofin)
	case 11: // "check with" for the query: up to `start`
		skipTerm(f, L.qchendofin)
	case 13: // sort of a "query of sort" or of a spec. part
		L.sortname = L.popmn()
		L.sourcetypei = L.typet.AddStr(L.sortname)
	case 14: // start with [S] query
		L.loadStratMod(f, L.sourcetypei, L.qresulttypei)
		skipTerm(f, L.sendofin) // the strategy
		f.Ilex()                // skip ']'
		skipTerm(f, L.qendofin) // the query term
	case 15: // specification description: the .spc
		L.openSpec()
	case 16:
		L.actmn = mkmodname1(L.actmn, L.c.Alfsy(l))
	case 17:
		L.actmn = L.mkmodname(L.actmn, mkmodname2)
	case 18:
		L.actmn = L.mkmodname(L.actmn, mkmodname3)
	case 23:
		L.actmn = L.mkmodname(L.actmn, mkmodname4)
	case 24:
		L.actmn = L.mkmodname(L.actmn, mkmodname5)
	case 19: // "check with" of a spec. part: up to `end` or `part`
		skipTerm(f, L.axchendofin)
	}
	return lr.NormCont
}

// loadStratMod is load_strat_mod (load/strategy.cc): strat[X] or
// tcstrat[X,Y] (the name in a char[IDLEN], snprintf), read by readmodules
// at the current depth when not loaded.
func (L *loader) loadStratMod(f *lex.Stream, sou, res int) {
	var m string
	if sou == res {
		m = fmt.Sprintf("strat[%s]", L.typet.Ide(sou))
	} else {
		m = fmt.Sprintf("tcstrat[%s,%s]", L.typet.Ide(sou), L.typet.Ide(res))
	}
	if len(m) > 49 {
		m = m[:49]
	}
	if !L.imp.Member(m) {
		L.readmodules(f, m)
	}
}

// openSpec is action 15: the .spc name, the stream, handheadofspec.
func (L *loader) openSpec() {
	if L.specname == "" {
		L.d.Errs("front/load: a specification file is needed (usage)\n")
		L.d.Fail()
	}
	specname := L.specname
	ind := len(specname) - 4
	if ind < 0 {
		ind = 0
	}
	var specsource string
	if ind >= len(specname) || specname[ind] != '.' {
		specsource = specname + ".spc"
	} else {
		specsource = specname
		specname = specname[:ind]
	}
	// elimination of A in A/toto.spc
	for i := min(ind, len(specname)-1); i >= 0; i-- {
		if specname[i] == '/' {
			specname = specname[i+1:]
			break
		}
	}
	L.specstream = lex.New(L.c, lex.Open(L.c, specsource), true)
	L.handheadofspec(specname)
}

// handheadofspec: `specification NAME`.
func (L *loader) handheadofspec(specname string) {
	s := L.specstream
	if s.Fulex() != L.idLex("specification") {
		s.Oerr()
		L.d.Errs("[handheadofspec.paxmmain] bad head of specification\n")
		L.d.Errs("\t'specification' expected\n")
		L.d.Fail()
	}
	s.Ilex()
	fl := s.Fulex()
	if !fl.IsIdent() {
		s.Oerr()
		L.d.Errs("[handheadofspec.paxmmain] bad head of specification\n")
		L.d.Errs("\tname of specification expected\n")
	}
	if fl != L.idLex(specname) {
		s.Oerr("name of specification doesn't match the name of file\n")
	}
	s.Ilex()
}

// handleaxiom: the body of the previous part, read from the .spc up to the
// name of the next part (RWmtfin), and its generated module (a name in the
// import table).
func (L *loader) handleaxiom() {
	old := *L.oldaxiomname
	s := L.specstream
	if L.imp.Member(old) {
		s.Oerr()
		L.d.Errs("\t name of axiom '", old, "' clash with a name of module\n")
		L.d.Fail()
	}
	fl := s.Ilex()
	if fl == L.idLex(old) {
		fin := L.idLex(L.axiomname) // RWmtfin.cridlex(finalsym)
		skipTerm(s, func(l lex.Lexem) bool { return l == fin || l.IsEOS() })
		L.globgr[L.imp.AddStr(old)] = true // genaxiommodule: a grammar
		L.typet.AddStr(L.sortname)
	} else {
		s.Oerr("bad name of specification part\n")
		L.d.Errs("\t '", old, "' expected\n")
		L.d.Fail()
	}
}
