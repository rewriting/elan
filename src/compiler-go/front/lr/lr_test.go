package lr

import (
	"bytes"
	"os"
	"reflect"
	"strings"
	"testing"

	"elan/front/diag"
	"elan/front/lex"
	"elan/front/tables"
)

// sliceSource is a Source over a fixed list of lexems; its messages imitate
// mlstream's (BeforeMess ends with "\n") at a fixed position.
type sliceSource struct {
	c   *lex.Context
	l   []lex.Lexem
	i   int
	pos string
}

func (s *sliceSource) Fulex() lex.Lexem {
	if s.i < len(s.l) {
		return s.l[s.i]
	}
	return lex.NoLexem
}

func (s *sliceSource) Ilex() lex.Lexem {
	l := s.Fulex()
	if s.i < len(s.l) {
		s.i++
	}
	return l
}

func (s *sliceSource) Oerr(parts ...string) {
	s.c.D.Errs("\n[error] :", s.pos)
	s.c.D.Errs(parts...)
}

func (s *sliceSource) BeforeMess(l lex.Lexem) {
	s.c.D.Errs("\n\t before  ")
	for n := 0; n < 7 && !l.IsEOS(); n++ {
		l = s.Ilex()
		s.c.D.Errs(s.c.Alfsy(l), " ")
	}
	s.c.D.Errs("\n")
}

func (s *sliceSource) Ctx() *lex.Context { return s.c }

func newCtx() (*lex.Context, *bytes.Buffer) {
	var out, errb bytes.Buffer
	d := &diag.D{Out: &out, Err: &errb, Warnings: true}
	return &lex.Context{D: d, Ident: tables.NewTabOfIdent(d)}, &errb
}

// ppexLexems reads src with a plain lexer (no '_' in identifiers), as the
// preprocessor's base stream does.
func ppexLexems(c *lex.Context, src string) []lex.Lexem {
	s := lex.New(c, lex.FromBytes("t.eln", []byte(src)), false)
	var ls []lex.Lexem
	for !s.Fulex().IsEOS() {
		ls = append(ls, s.Ilex())
	}
	return ls
}

// ppexCodes parses src with PpexGram and returns the reduction codes < 700.
func ppexCodes(t *testing.T, src string) ([]int, Result, string) {
	t.Helper()
	c, errb := newCtx()
	f := &sliceSource{c: c, l: ppexLexems(c, src), pos: "file=ppexerr.eln\tline=1\tpos=0\n"}
	var codes []int
	var res Result
	st := diag.Run(func() {
		res = Parse(PpexGram, f, f.Ilex, CexLexToLex, func(n int, l lex.Lexem) int {
			if n > 0 && n < 700 {
				codes = append(codes, n)
			}
			if n == 1 {
				return AcceptIm
			}
			return NormCont
		})
	})
	if st != 0 {
		t.Fatalf("%s: status %d", src, st)
	}
	return codes, res, errb.String()
}

func TestPpexArith(t *testing.T) {
	codes, res, errs := ppexCodes(t, "( 1 + 2 * 3 )")
	if want := []int{2, 2, 2, 5, 3, 1}; !reflect.DeepEqual(codes, want) || res != Accepted || errs != "" {
		t.Fatalf("codes %v result %v stderr %q, want %v", codes, res, errs, want)
	}
}

func TestPpexBool(t *testing.T) {
	codes, res, _ := ppexCodes(t, "( 1 == 1 & 2 > 1 )")
	if want := []int{2, 2, 10, 2, 2, 14, 9, 1}; !reflect.DeepEqual(codes, want) || res != Accepted {
		t.Fatalf("codes %v result %v, want %v", codes, res, want)
	}
}

// The message is the harness's for `{ a }~(1 + )` (fixtures/ppexerr.eln).
func TestPpexSyntaxError(t *testing.T) {
	_, res, errs := ppexCodes(t, "( 1 + ) end")
	b, err := os.ReadFile("../../../../tests/front/fixtures/ppexerr.eln.expected")
	if err != nil {
		t.Fatal(err)
	}
	exp := string(b)
	exp = exp[strings.Index(exp, "@@stderr\n")+len("@@stderr\n") : strings.Index(exp, "@@status")]
	if res != Rejected || errs != exp {
		t.Fatalf("result %v\nstderr %q\nwant   %q", res, errs, exp)
	}
}

func modLexems(c *lex.Context, src string) []lex.Lexem {
	return ppexLexems(c, src)
}

func TestModGramImports(t *testing.T) {
	c, errb := newCtx()
	mrwt := tables.NewMRWT(c.D)
	f := &sliceSource{c: c, l: modLexems(c, "module m import a b[c, d]; end end")}
	var codes []int
	res := Parse(ModGram, f, f.Ilex, LexToModLex(c, mrwt), func(n int, l lex.Lexem) int {
		if n < 700 {
			codes = append(codes, n)
		}
		return NormCont
	})
	want := []int{0, 169, 189, 202, 185, 8, 185, 185, 185, 187, 186, 8, 201, 144, -2}
	if res != Accepted || !reflect.DeepEqual(codes, want) {
		t.Fatalf("result %v codes %v (stderr %q)\nwant %v", res, codes, errb.String(), want)
	}
}

// ldSem consumes the term of `start with () ...` lexically at code 10, up to
// `end` (qendofin), as the loader skeleton does.
func ldParse(t *testing.T, src string, at int, ret int) (Result, []int, *sliceSource, string) {
	t.Helper()
	c, errb := newCtx()
	mrwt := tables.NewMRWT(c.D)
	s := lex.New(c, lex.FromBytes("t.lgi", []byte(src)), true)
	var ls []lex.Lexem
	for !s.Fulex().IsEOS() {
		ls = append(ls, s.Ilex())
	}
	f := &sliceSource{c: c, l: ls, pos: "file=t.lgi\tline=1\tpos=1\n"}
	end := lex.IdLex(c.Ident.Index("end"))
	var codes []int
	var res Result
	diag.Run(func() {
		res = Parse(LdModGram, f, f.Ilex, LexToModLex(c, mrwt), func(n int, l lex.Lexem) int {
			if n < 700 {
				codes = append(codes, n)
			}
			if n == at {
				return ret
			}
			if n == 10 {
				for {
					f.Ilex()
					if x := f.Fulex(); x == end || x.IsEOS() {
						break
					}
				}
			}
			return NormCont
		})
	})
	return res, codes, f, errb.String()
}

const ldSrc = "LPL p description query of sort s result of sort s import a start with () t(x) end"

func TestLdModGram(t *testing.T) {
	res, codes, _, errs := ldParse(t, ldSrc, -100, 0)
	// logicname, mimodule, sortpart, mimodule, qresult, mimodule,
	// imodulelist, qstartwith, queryhandbypart, logicdescription, accept
	want := []int{0, 2, 16, 13, 16, 9, 16, 4, 10, 6, 3, -2}
	if res != Accepted || errs != "" || !reflect.DeepEqual(codes, want) {
		t.Fatalf("result %v codes %v stderr %q, want %v", res, codes, errs, want)
	}
}

func TestSuspend(t *testing.T) {
	res, codes, f, _ := ldParse(t, ldSrc, 2, Suspend)
	if res != Suspended || codes[len(codes)-1] != 2 {
		t.Fatalf("result %v codes %v", res, codes)
	}
	// code 2 (logicname) is reduced on the look-ahead `description`, which
	// has not been read.
	if got := f.c.Alfsy(f.Fulex()); got != "description" {
		t.Fatalf("read past the suspension: look-ahead %q", got)
	}
}

func TestErrorIm(t *testing.T) {
	res, _, _, errs := ldParse(t, ldSrc, 2, ErrorIm)
	if res != Rejected || !strings.Contains(errs, "[PARSER] semaction error \ton symbol 'description'\n\t before  query of sort s result of sort \n") {
		t.Fatalf("result %v stderr %q", res, errs)
	}
}

func TestSyntaxErrorExpected(t *testing.T) {
	// `LPL p descriptio` : one of ( 'description' ) expected.
	res, _, _, errs := ldParse(t, "LPL p query", -100, 0)
	if res != Rejected || !strings.Contains(errs, "[PARSER] syntax error \ton symbol 'query'\n\t before  ENDOFLSTREAM \n\tone of ( 'description' ) expected \n") {
		t.Fatalf("result %v stderr %q", res, errs)
	}
}

func TestSymName(t *testing.T) {
	c, _ := newCtx()
	for sym, want := range map[int32]string{-259: "IDENTIFIER", -13261: "NUMBER", -256: "ENDOFLSTREAM", -40: "(", -571: "end"} {
		if got := SymName(c, sym); got != want {
			t.Errorf("SymName(%d) = %q, want %q", sym, got, want)
		}
	}
	if CexLexToLex(lex.NumLex(7)) != lex.JustNumber || CexLexToLex(lex.CharLex('(')) != lex.CharLex('(') {
		t.Error("CexLexToLex")
	}
}
