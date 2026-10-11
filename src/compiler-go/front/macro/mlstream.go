// Package macro is the interpreter's macro preprocessor in Go (S8a): the
// stream mlstream (src/interpreter/lex/mlstream.cc), its macros and action
// stack, collident, the iteration forms, the constant expressions (ppexgram)
// and the FOR EACH / IF forms, whose evaluation goes through an Evaluator
// (the five C++ call sites that need the loader). A port, function by
// function, the C++ names in comments; the quirks are kept.
package macro

import (
	"fmt"

	"elan/front/lex"
	"elan/front/tables"
)

// The kinds of action (mlstream.h).
const (
	simIterAction    = 0 // SIMITERACTION: X~N, {X}~N
	incrIterAction   = 1 // INCRITERACTION: {X}_I=A...B
	nextMatchAction  = 2 // NEXTMATCHACTION: FOR EACH, IF
	idIterAction     = 3 // IDITERACTION: X_A,...,X_B
	simpleCopyAction = 4 // SIMPLECOPYACTION: a macro body, a re-read buffer
)

// action is macactelem.
type action struct {
	currentb  lex.Buffer
	lastLexem lex.Lexem
	which     int
	i         int32 // simiter.i
	b, e      int32 // incriter, iditer
	mname     int   // incriter.mname
	ch        int   // iditer.ch (a C char: signed)
	sol       Solutions
	varNames  []int // nextmatch.varnames
	delivered bool  // the trace hooks: a body lexem was read from it
	actionbuf lex.Buffer
}

// State holds the file-static variables of mlstream.cc shared by every
// stream of one load: the FOR EACH variables, cexpresult, the evaluation
// stack of ppexsemact (never reset), and the trace hooks' depth and pending
// markers.
type State struct {
	varTypeTab  [tables.MaxNofVar]string
	varNames    [tables.MaxNofVar]lex.Lexem
	wasAffected [tables.MaxNofVar]byte
	varTabI     int
	varNamesI   int
	cexpResult  int32
	evalStack   [tables.MaxDeepConstExp]int32
	evalStackI  int

	rwFor, rwEach, rwSuch, rwThat, rwAnd, rwAndIf, rwIf lex.Lexem

	depth    int      // lextrace_depth
	pending  []string // lextrace_pending
	returned int      // lextrace_returned: Ilex calls completed
	deferred []deferredMarker
}

// deferredMarker is a marker that becomes pending when `returned` reaches
// at (lextrace_deferred).
type deferredMarker struct {
	at int
	m  string
}

// NewState is a fresh preprocessor state (one per load).
func NewState() *State { return &State{} }

// Stream is mlstream: a lexer (NONUNDERID) with macro expansion.
type Stream struct {
	*lex.Stream
	st     *State
	ev     Evaluator
	tr     Tracer
	mflex  lex.Lexem
	mactab *tables.MiTab
	macb   [tables.MaxNofMac][]lex.Lexem
	stack  []*action // macactstack (elements stay in place)
	stacki int       // macactstacki
}

// New is mlstream(char *name) over cs: the base lexer's priming read, the
// seven words of the preprocessor added to tabofident, `@begin name`, then
// the priming ilex, not traced.
func New(c *lex.Context, st *State, cs *lex.CharStream, name string, ev Evaluator, tr Tracer) *Stream {
	s := &Stream{Stream: lex.New(c, cs, false), st: st, ev: ev, tr: tr}
	s.mactab = tables.NewMiTab(c.D, tables.MaxNofMac)
	id := func(w string) lex.Lexem { return lex.IdLex(c.Ident.AddStr(w)) }
	st.rwFor = id("FOR")
	st.rwEach = id("EACH")
	st.rwSuch = id("SUCH")
	st.rwThat = id("THAT")
	st.rwAnd = id("AND")
	st.rwAndIf = id("ANDIF")
	st.rwIf = id("IF")
	if tr != nil {
		tr.Begin(name)
	}
	st.depth++
	s.Ilex()
	st.depth--
	return s
}

// Close is ~mlstream: `@end`.
func (s *Stream) Close() {
	if s.tr != nil {
		s.tr.End()
	}
}

// Raw is the base lexer (lstream).
func (s *Stream) Raw() *lex.Stream { return s.Stream }

func (s *Stream) d() interface {
	Errs(...string)
	Fail()
} {
	return s.Ctx().D
}

func ch(c byte) lex.Lexem { return lex.Lexem(-int32(int8(c))) } // lexem == (char)c

// Fulex is mlstream::fulex.
func (s *Stream) Fulex() lex.Lexem { return s.mflex }

// getmflex: the old first lexem; the next one read from the action stack or
// the base lexer, blanks skipped; 0 when it comes from a simple copy.
func (s *Stream) getmflex() (lex.Lexem, int) {
	l := s.mflex
	if s.stacki == 0 {
		for {
			s.mflex = s.Stream.Ilex()
			if !s.mflex.IsBlank() {
				break
			}
		}
		return l, 1
	}
	var r int
	for {
		r = s.buffilex()
		if !s.mflex.IsBlank() {
			break
		}
	}
	return l, r
}

// macrotest: mflex a macro is replaced by its body.
func (s *Stream) macrotest() bool {
	if s.mflex.IsIdent() && s.mactab.Member(s.mflex.IdVal()) {
		if len(s.macb[s.mactab.Posid]) == 0 {
			s.Oerr("[preprocessor] non defined macro '", s.Ctx().Alfsy(s.mflex), "' used")
			s.BeforeMess(s.mflex)
			s.d().Fail()
		}
		s.mflex = lex.BlankLexem
		var b lex.Buffer
		for _, l := range s.macb[s.mactab.Posid] {
			b.Put(l)
		}
		s.addSimpleCopy(&b)
		return true
	}
	return false
}

// Pilex is mlstream::pilex: the next lexem with macros expanded but no
// form (the ILEXEM of ppexgram).
func (s *Stream) Pilex() lex.Lexem {
	l, r := s.getmflex()
	if r != 0 {
		s.macrotest()
	}
	return l
}

// Ilex is mlstream::ilex: the next lexem, forms expanded, identifiers
// joined; traced when it is the outermost call.
func (s *Stream) Ilex() lex.Lexem {
	s.st.depth++
	l := s.Pilex()
	if s.macroexp() {
		s.collident()
		for s.macroexp() {
			s.collident()
		}
	} else {
		s.collident()
	}
	s.returnedOne()
	s.st.depth--
	if s.st.depth == 0 {
		if s.tr != nil {
			s.tr.Lexem(l)
			for _, m := range s.st.pending {
				s.tr.Marker(m)
			}
		}
		s.st.pending = s.st.pending[:0]
	}
	return l
}

// returnedOne is lextrace_returned_one: an Ilex call completed, the
// deferred markers that are due become pending.
func (s *Stream) returnedOne() {
	st := s.st
	st.returned++
	k := 0
	for _, d := range st.deferred {
		if d.at <= st.returned {
			s.mark(d.m)
		} else {
			st.deferred[k] = d
			k++
		}
	}
	st.deferred = st.deferred[:k]
}

func (s *Stream) mark(m string) {
	if s.tr != nil {
		s.st.pending = append(s.st.pending, m)
	}
}

// collpilex is mlstream::collpilex.
func (s *Stream) collpilex() lex.Lexem {
	l := s.Pilex()
	s.collident()
	return l
}

// collident joins an identifier followed by `_` and an identifier or a
// number (a_b, x_1), and expands the joined name when it is a macro.
func (s *Stream) collident() {
	if !s.mflex.IsIdent() {
		return
	}
	var tmpb lex.Buffer
	coll := false
	fl := s.Pilex()
	tmpb.Put(fl)
	colide := s.Ctx().Alfsy(fl)
	for s.mflex == ch('_') {
		coll = true
		colide += "_"
		s.Pilex()
		if s.mflex.IsNum() || s.mflex.IsIdent() {
			colide += s.Ctx().Alfsy(s.mflex)
			s.Pilex()
		}
		tmpb.Get(s.Ctx().D)
		tmpb.Put(lex.IdLex(s.Ctx().Ident.AddStr(colide)))
	}
	s.addSimpleCopy(&tmpb)
	if coll {
		s.macrotest()
	}
}

// AddMac is mlstream::addmac: a redefinition warns (the position only with
// warnings on, the message always, as the C++).
func (s *Stream) AddMac(name int, body []lex.Lexem) {
	if s.mactab.Member(name) {
		s.Owarn()
		s.d().Errs("[mlstream] redefinition of macro ", s.Ctx().Ident.Ide(name), "\n")
	}
	i := s.mactab.AddN(name)
	s.macb[i] = append([]lex.Lexem(nil), body...)
}

// DeleteMac is mlstream::deletemac.
func (s *Stream) DeleteMac(name int) {
	if !s.mactab.Member(name) {
		s.Oerr("try to delete non existing macro, int.err.\n")
		s.d().Fail()
	}
	s.mactab.RemoveNonP(s.mactab.Posid)
	s.macb[s.mactab.Posid] = nil
}

// addactionprelim: the next element of the stack (reused when it exists).
func (s *Stream) push() *action {
	if s.stacki >= len(s.stack) {
		s.stack = append(s.stack, &action{})
	}
	a := s.stack[s.stacki]
	a.currentb.Clear()
	a.lastLexem = s.mflex
	return a
}

func (s *Stream) addSimpleCopy(b *lex.Buffer) {
	a := s.push()
	a.which = simpleCopyAction
	a.i = 1
	b.Copy(&a.actionbuf)
	s.stacki++
	s.Pilex()
}

func (s *Stream) addSimIter(n int32, b *lex.Buffer) {
	a := s.push()
	a.which = simIterAction
	a.i = n
	b.Copy(&a.actionbuf)
	s.stacki++
	s.Pilex()
}

func (s *Stream) addIdIter(b, e int32, c int, buf *lex.Buffer) {
	a := s.push()
	a.which = idIterAction
	a.b, a.e, a.ch = b, e, c
	buf.Copy(&a.actionbuf)
	s.stacki++
	s.Pilex()
}

func (s *Stream) addIncrIter(mn int, b, e int32, buf *lex.Buffer) {
	a := s.push()
	a.which = incrIterAction
	a.b, a.e, a.mname = b, e, mn
	buf.Copy(&a.actionbuf)
	s.AddMac(mn, nil)
	s.stacki++
	s.Pilex()
}

// addforeach: the variables become empty macros, the body is the action.
func (s *Stream) addForEach(buf *lex.Buffer, sol Solutions, bvarnamesi int) {
	a := s.push()
	a.which = nextMatchAction
	a.sol = sol
	a.delivered = false
	n := s.st.varTabI - bvarnamesi
	a.varNames = make([]int, n)
	for i := 0; i < n; i++ {
		a.varNames[i] = s.st.varNames[i+bvarnamesi].IdVal()
		s.AddMac(a.varNames[i], nil)
	}
	buf.Copy(&a.actionbuf)
	s.stacki++
	s.Pilex()
}

func (s *Stream) deleteForEachMacs(names []int) {
	for i := len(names) - 1; i >= 0; i-- {
		s.DeleteMac(names[i])
	}
}

// pop ends the top action: mflex is its last lexem (read on when blank).
func (s *Stream) pop(a *action) int {
	s.mflex = a.lastLexem
	s.stacki--
	if s.mflex.IsBlank() {
		s.Pilex() // pilex(l) with l aliasing mflex: mflex is the new lexem
	}
	return 0
}

// buffilex reads mflex from the top action.
func (s *Stream) buffilex() int {
	a := s.stack[s.stacki-1]
	for a.currentb.IsEmpty() {
		switch a.which {
		case simIterAction, simpleCopyAction:
			if a.i <= 0 {
				return s.pop(a)
			}
			a.i--
			a.actionbuf.Copy(&a.currentb)
		case idIterAction:
			if a.b > a.e {
				return s.pop(a)
			}
			a.actionbuf.Copy(&a.currentb)
			a.currentb.Put(ch('_'))
			a.currentb.Put(lex.NumLex(a.b))
			if a.b != a.e {
				a.currentb.Put(lex.CharLex(a.ch))
			}
			a.b++
		case incrIterAction:
			if a.b > a.e {
				s.DeleteMac(a.mname)
				return s.pop(a)
			}
			a.actionbuf.Copy(&a.currentb)
			s.DeleteMac(a.mname)
			s.AddMac(a.mname, []lex.Lexem{lex.NumLex(a.b)}) // ADDNUMMAC
			a.b++
		case nextMatchAction:
			vals, ok := a.sol.Next()
			if !ok {
				// the end is found while reading one lexem ahead, during
				// the Ilex call that returns the body's next-to-last lexem:
				// when the action delivered a lexem, the marker becomes
				// pending after the next completed Ilex call, at any depth
				// (lextrace_foreach_end)
				if a.delivered {
					a.delivered = false
					if s.tr != nil {
						s.st.deferred = append(s.st.deferred, deferredMarker{s.st.returned + 2, "@foreach-end"})
					}
				} else {
					s.mark("@foreach-end")
				}
				s.deleteForEachMacs(a.varNames)
				return s.pop(a)
			}
			s.deleteForEachMacs(a.varNames)
			for i, n := range a.varNames {
				s.AddMac(n, vals[i])
			}
			a.actionbuf.Copy(&a.currentb)
		default:
			s.d().Errs(fmt.Sprintf("[mlstream::buffilex] unknown macro action #%d internal error !!!\n", a.which))
		}
	}
	s.mflex = a.currentb.Get(s.Ctx().D)
	if a.which == nextMatchAction {
		a.delivered = true
	}
	if a.which != simpleCopyAction {
		return 1
	}
	return 0
}

func (s *Stream) insteaderr() { s.d().Errs(" instead of ", s.Ctx().Alfsy(s.mflex)) }

// expect is mlstream::expect.
func (s *Stream) expect(c byte) {
	if s.mflex != ch(c) {
		s.Stream.Oerr()
		s.d().Errs("[mlstream] '", string([]byte{c}), "' expected ")
		s.insteaderr()
		s.BeforeMess(s.mflex)
		s.d().Fail()
	}
}

// expectident is mlstream::expectident.
func (s *Stream) expectident() {
	if !s.mflex.IsIdent() {
		s.Oerr("[mlstream] identifier expected")
		s.insteaderr()
		s.BeforeMess(s.mflex)
		s.d().Fail()
	}
}

// fillmacbuf reads a `{ ... }` body (balanced) into tmpb; mflex is the
// closing `}`.
func (s *Stream) fillmacbuf(tmpb *lex.Buffer) {
	i := 0
	s.getmflex()
	for i != 0 || s.mflex != ch('}') {
		fl, _ := s.getmflex()
		if s.mflex.IsEOS() {
			s.Oerr("macro construction buffer throw end of file\n")
			s.d().Errs("\t did you forget a '}' somewhere ?\n")
			s.d().Fail()
		}
		tmpb.Put(fl)
		if fl == ch('{') {
			i++
		} else if fl == ch('}') {
			i--
		}
	}
}

// macroexp expands a form at mflex; false (the lexem re-read) when none.
func (s *Stream) macroexp() bool {
	var tmpb lex.Buffer
	if s.mflex == ch('{') {
		s.fillmacbuf(&tmpb)
		s.Pilex()
		if s.mflex == ch('~') { // {XXX}~N
			s.Pilex()
			n := s.constexpexp()
			s.mflex = lex.BlankLexem
			s.addSimIter(n, &tmpb)
			return true
		} else if s.mflex == ch('_') { // {XXX}_I=N1...N2
			s.Pilex()
			s.expectident()
			name := s.mflex.IdVal()
			s.Pilex()
			s.expect('=')
			s.Pilex()
			b := s.constexpexp()
			s.Pilex()
			s.expect('.')
			s.Pilex()
			s.expect('.')
			s.Pilex()
			s.expect('.')
			s.Pilex()
			e := s.constexpexp()
			s.mflex = lex.BlankLexem
			s.addIncrIter(name, b, e, &tmpb)
			return true
		} else {
			s.Oerr("[mlstream::ilex] syntax error in macro construction")
			s.BeforeMess(s.mflex)
			s.d().Fail()
		}
	}
	fl := s.Pilex()
	tmpb.Put(fl)
	if s.mflex == ch('~') { // X~N
		s.collpilex()
		n := s.constexpexp()
		s.mflex = lex.BlankLexem
		s.addSimIter(n, &tmpb)
		return true
	}
	if (fl == s.st.rwFor && s.mflex == s.st.rwEach) || fl == s.st.rwIf { // FOR EACH, IF
		s.forEach(fl)
		return true
	}
	if fl.IsIdent() && s.mflex == ch('_') { // maybe X_1,...,X_N
		ide := fl
		var idebuf lex.Buffer
		idebuf.Put(ide)
		fl = s.Pilex()
		tmpb.Put(fl)
		if s.mflex.IsNum() || s.mflex == ch('(') {
			bn := s.constexpexp()
			fl = s.Pilex()
			tmpb.Put(fl)
			if s.mflex.IsChar() {
				c := int(int8(s.mflex.CharVal())) // char ch = mflex.charval()
				cl := lex.Lexem(-int32(c))        // mflex == ch
				fl = s.Pilex()
				tmpb.Put(fl)
				fl = s.Pilex()
				tmpb.Put(fl)
				if fl == ch('.') && s.mflex == ch('.') {
					fl = s.Pilex()
					tmpb.Put(fl)
					fl = s.Pilex()
					tmpb.Put(fl)
					if fl == ch('.') && s.mflex == cl {
						fl = s.Pilex()
						tmpb.Put(fl)
						fl = s.Pilex()
						tmpb.Put(fl)
						if fl == ide && s.mflex == ch('_') {
							fl = s.Pilex()
							tmpb.Put(fl)
							if s.mflex.IsNum() || s.mflex == ch('(') { // X_1,...,X_N
								en := s.constexpexp()
								s.mflex = lex.BlankLexem
								s.addIdIter(bn, en, c, &idebuf)
								return true
							}
						}
					}
				}
			}
		}
	}
	s.addSimpleCopy(&tmpb)
	return false
}

// BeforeMess is mlstream::beforemess: 7 lexems read with Ilex.
func (s *Stream) BeforeMess(l lex.Lexem) {
	s.d().Errs("\n\t before  ")
	for n := 0; n < 7 && !l.IsEOS(); n++ {
		l = s.Ilex()
		s.d().Errs(s.Ctx().Alfsy(l), " ")
	}
	s.d().Errs("\n")
}
