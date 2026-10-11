package macro

import (
	"fmt"

	"elan/front/lex"
	"elan/front/lr"
	"elan/front/tables"
)

// constexpexp: a number, or `(` expression `)` parsed by ppexgram (read
// with Pilex); mflex is left on the number or on the `)`.
func (s *Stream) constexpexp() int32 {
	if s.mflex.IsNum() {
		return s.mflex.NumVal()
	}
	if s.mflex == ch('(') {
		if lr.Parse(lr.PpexGram, s, s.Pilex, lr.CexLexToLex, s.ppexsemact) != lr.Accepted {
			s.d().Fail()
		}
		res := s.st.cexpResult
		s.expect(')')
		return res
	}
	s.Oerr()
	s.d().Errs("[preprocessor] constant or '(' expected")
	s.BeforeMess(s.mflex)
	s.d().Fail()
	return 0
}

// deeperr: the evaluation stack is full.
func (s *Stream) deeperr() {
	s.Oerr()
	s.d().Errs(fmt.Sprintf("[mlstream.deeperr] evalstacki overflowed over MAXDEEPCONSTEXP = %d, fatal\n", tables.MaxDeepConstExp))
	s.d().Fail()
}

func b2i(b bool) int32 {
	if b {
		return 1
	}
	return 0
}

// ppexsemact: the semantics of ppexgram, an evaluation stack of C ints.
func (s *Stream) ppexsemact(n int, l lex.Lexem) int {
	st := s.st
	es := &st.evalStack
	// binary pops the right operand and applies f to the top.
	binary := func(f func(a, b int32) int32) {
		st.evalStackI--
		es[st.evalStackI-1] = f(es[st.evalStackI-1], es[st.evalStackI])
	}
	switch n {
	case 1:
		st.evalStackI--
		st.cexpResult = es[st.evalStackI]
		return lr.AcceptIm
	case 2:
		if st.evalStackI >= tables.MaxDeepConstExp {
			s.deeperr()
		}
		es[st.evalStackI] = l.NumVal()
		st.evalStackI++
	case 3:
		binary(func(a, b int32) int32 { return a + b })
	case 4:
		binary(func(a, b int32) int32 { return a - b })
	case 5:
		binary(func(a, b int32) int32 { return a * b })
	case 6:
		binary(func(a, b int32) int32 { return a / b })
	case 7:
		binary(func(a, b int32) int32 { return a % b })
	case 8:
		es[st.evalStackI-1] = -es[st.evalStackI-1]
	case 9:
		binary(func(a, b int32) int32 { return b2i(a != 0 && b != 0) })
	case 10:
		binary(func(a, b int32) int32 { return b2i(a == b) })
	case 11:
		binary(func(a, b int32) int32 { return b2i(a != b) })
	case 12:
		binary(func(a, b int32) int32 { return b2i(a <= b) })
	case 13:
		binary(func(a, b int32) int32 { return b2i(a >= b) })
	case 14:
		binary(func(a, b int32) int32 { return b2i(a > b) })
	case 15:
		binary(func(a, b int32) int32 { return b2i(a < b) })
	case 16:
		if st.evalStackI >= tables.MaxDeepConstExp {
			s.deeperr()
		}
		es[st.evalStackI] = int32(l.IdVal())
		st.evalStackI++
	}
	return lr.NormCont
}
