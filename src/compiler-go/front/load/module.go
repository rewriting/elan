package load

import (
	"strings"

	"elan/front/lex"
	"elan/front/lr"
	"elan/front/macro"
)

// warner is a stream the loader warns on: the .lgi lstream or a module's
// mlstream (f in the C++).
type warner interface {
	Owarn(parts ...string)
}

// odsek writes the indentation of the module tree: n+1 spaces.
func odsek(n int) string { return strings.Repeat(" ", n+1) }

// importmodInf is importmod_inf (msemact.cc): the module read one level
// deeper when not loaded; a module still being read is a cycle (a warning
// on the importing stream).
func (L *loader) importmodInf(name string, f warner) {
	if !L.imp.Member(name) {
		L.stacki++
		L.readmodules(f, name)
		L.stacki--
	}
	if !L.globgr[L.imp.Index(name)] {
		f.Owarn("there is probably a cycle in imports of module ", name, "\n")
	}
}

// readmodules (msemact.cc): name in the import table, its arguments, the
// file <name>.eln through the preprocessor, modgram up to the first action
// of the module body, then the rest of the module drained (S8b reads it).
// Not done in S8a, by design: trclos/strconc after a strat[...] or
// tcstrat[...] module, explimpl, symbappl, visibilities, grammars.
func (L *loader) readmodules(f warner, name string) {
	if L.imp.Member(name) {
		f.Owarn("[readmodules] int.err.\n")
		L.d.Fail()
	}
	L.globgr[L.imp.AddStr(name)] = false
	L.actargmodname, L.actarglist = divideonargs(name)
	modsou := L.actargmodname + ".eln"
	cs := lex.Open(L.c, modsou)
	s := macro.New(L.c, L.mst, cs, modsou, L.o.Eval, L.trace)
	for len(L.frames) <= L.stacki {
		L.frames = append(L.frames, "")
	}
	L.frames[L.stacki] = name // setmodname
	switch lr.Parse(lr.ModGram, s, s.Ilex, L.ltol, func(n int, l lex.Lexem) int { return L.semact(s, n, l) }) {
	case lr.Rejected:
		L.d.Fail()
	case lr.Suspended:
		for !s.Fulex().IsEOS() {
			s.Ilex()
		}
		L.endOf()
	}
	s.Close()
	L.globgr[L.imp.Index(name)] = true
}

// endOf is action 144's tree line.
func (L *loader) endOf() {
	if !L.o.Batch {
		L.d.Outs(odsek(L.stacki<<1), "end of ", L.frames[L.stacki], "\n")
	}
}

// bodyCodes are the actions that start a module body: the sorts (28, 29),
// `operators` (335), `stratop` (336), `rules` (150), `strategies` (360).
var bodyCodes = map[int]bool{28: true, 29: true, 335: true, 336: true, 150: true, 360: true}

// semact: the module actions met before the body (msemact.cc semact1);
// the first body action suspends the parse.
func (L *loader) semact(f *macro.Stream, n int, l lex.Lexem) int {
	switch {
	case n == 0: // begin
		if !L.o.Batch {
			L.d.Outs(odsek(L.stacki<<1), "handling module ", L.frames[L.stacki], "\n")
		}
		L.actimp = nil
	case n == -2 || n >= 700:
	case n == 169: // module name
		if L.actargmodname != L.c.Alfsy(l) {
			f.Owarn("\n[semact] name of module doesn't correspond with the name of file\n")
			return lr.ErrorIm
		}
	case n == 188: // a formal parameter: a macro of the actual one
		if len(L.actarglist) == 0 {
			f.Oerr("number of formal and actual parameters for module ", L.frames[L.stacki], " doesn't correspond\n")
			return lr.HandErrorIm
		}
		f.AddMac(l.IdVal(), L.paramLexems(L.actarglist[0]))
		L.actarglist = L.actarglist[1:]
	case n == 189:
		if len(L.actarglist) != 0 {
			f.Oerr("number of actual and formal parameters for module ")
			f.Oerr(L.frames[L.stacki], " doesn't correspond\n")
			return lr.HandErrorIm
		}
	case n == 202, n == 140, n == 141, n == 201: // visibilities, aliases: S8b
	case n == 185:
		L.actimp = mkmodname1(L.actimp, L.c.Alfsy(l))
	case n == 186:
		L.actimp = L.mkmodname(L.actimp, mkmodname2)
	case n == 187:
		L.actimp = L.mkmodname(L.actimp, mkmodname3)
	case n == 172:
		L.actimp = L.mkmodname(L.actimp, mkmodname4)
	case n == 173:
		L.actimp = L.mkmodname(L.actimp, mkmodname5)
	case n == 8: // module in import
		if len(L.actimp) == 0 {
			L.internal("no module name at action 8")
		}
		L.importmodInf(L.actimp[len(L.actimp)-1], f)
		L.actimp = nil
	case n == 144: // end of a module without body
		L.endOf()
	case bodyCodes[n]:
		return lr.Suspend
	default:
		L.internal("action %d before the module body (S8b)", n)
	}
	return lr.NormCont
}

// paramLexems is action 188's macro body: a number, or the argument split
// at [ ] , < - > | into identifiers and characters.
func (L *loader) paramLexems(arg string) []lex.Lexem {
	if intArg(arg) {
		return []lex.Lexem{lex.NumLex(atoi(arg))}
	}
	var b []lex.Lexem
	var ss []byte
	flush := func() {
		if len(ss) > 0 {
			b = append(b, L.idLex(string(ss)))
			ss = ss[:0]
		}
	}
	for i := 0; i < len(arg); i++ {
		switch c := arg[i]; c {
		case '[', ']', ',', '<', '-', '>', '|':
			flush()
			b = append(b, lex.CharLex(int(c)))
		default:
			ss = append(ss, c)
		}
	}
	flush()
	return b
}
