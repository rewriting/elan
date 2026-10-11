package macro

import (
	"bytes"
	"fmt"
	"os"
	"path/filepath"
	"reflect"
	"strings"
	"testing"

	"elan/front/diag"
	"elan/front/lex"
	"elan/front/tables"
)

const fixtures = "../../../../tests/front/fixtures"

func newCtx(warnings bool) (*lex.Context, *bytes.Buffer, *bytes.Buffer) {
	var out, errb bytes.Buffer
	d := &diag.D{Out: &out, Err: &errb, Warnings: warnings}
	return &lex.Context{D: d, Ident: tables.NewTabOfIdent(d)}, &out, &errb
}

// render is the harness's .expected format for a module text read to its end.
func render(name string, src []byte, ev Evaluator) string {
	c, out, errb := newCtx(true)
	st := diag.Run(func() { Lex(c, lex.FromBytes(name, src), name, ev, out) })
	return fmt.Sprintf("%s@@stderr\n%s@@status %d\n", out, errb, st)
}

// Every .eln fixture equals the harness output (stdout with positions,
// stderr, status), with the standalone evaluator of `front-trace lex`.
func TestFixtures(t *testing.T) {
	files, _ := filepath.Glob(filepath.Join(fixtures, "*.eln"))
	n := 0
	for _, f := range files {
		exp, err := os.ReadFile(f + ".expected")
		if err != nil {
			continue
		}
		src, _ := os.ReadFile(f)
		n++
		if got := render(filepath.Base(f), src, Refuse{}); got != string(exp) {
			t.Errorf("%s:\n--- got\n%s--- want\n%s", filepath.Base(f), got, exp)
		}
	}
	if n < 40 {
		t.Fatalf("only %d .eln fixtures", n)
	}
}

// stream opens a stream on src with the Mark evaluator; its trace in tr.
func stream(t *testing.T, src string, ev Evaluator) (*Stream, *bytes.Buffer, *bytes.Buffer, *bytes.Buffer) {
	t.Helper()
	c, out, errb := newCtx(true)
	var tr bytes.Buffer
	var s *Stream
	if st := diag.Run(func() {
		s = New(c, NewState(), lex.FromBytes("t.eln", []byte(src)), "t.eln", ev, NewTraceWriter(&tr, c))
	}); st != 0 {
		t.Fatalf("open: status %d, %s", st, errb)
	}
	return s, &tr, out, errb
}

func drain(s *Stream) int {
	return diag.Run(func() {
		for !s.Ilex().IsEOS() {
		}
		s.Close()
	})
}

func noPos(trace string) string {
	var b strings.Builder
	for _, l := range strings.SplitAfter(trace, "\n") {
		f := strings.SplitN(l, " ", 3)
		if len(f) == 3 && (f[0] == "id" || f[0] == "str") {
			l = f[0] + " " + f[2]
		}
		b.WriteString(l)
	}
	return b.String()
}

func id(c *lex.Context, w string) int { return c.Ident.AddStr(w) }

// A macro defined by AddMac (action 188 of a module parameter) is expanded;
// a redefinition warns (the position only with warnings on, the message
// always); deleted once after two definitions it stays a macro with an empty
// body (quirk 8): `non defined macro`.
func TestAddMac(t *testing.T) {
	s, tr, _, errb := stream(t, "module m X X end", Mark{})
	c := s.Ctx()
	x := id(c, "X")
	s.AddMac(x, []lex.Lexem{lex.NumLex(3), lex.CharLex('+'), lex.NumLex(1)})
	if st := drain(s); st != 0 || errb.Len() != 0 {
		t.Fatalf("status %d %s", st, errb)
	}
	// the priming ilex has read `module` (and the raw lexer `m`): both X
	// are looked up after the definition.
	if got, want := noPos(tr.String()), "@begin t.eln\nid module\nid m\nnum 3\nchar 43\nnum 1\nnum 3\nchar 43\nnum 1\nid end\neos\n@end\n"; got != want {
		t.Fatalf("got %q want %q", got, want)
	}

	s, _, _, errb = stream(t, "module m X end", Mark{})
	s.AddMac(x, []lex.Lexem{lex.NumLex(1)})
	s.AddMac(x, []lex.Lexem{lex.NumLex(2)})
	if want := "\n[warning] :file=t.eln\tline=0\tpos=11\n[mlstream] redefinition of macro X\n"; errb.String() != want {
		t.Fatalf("redefinition: %q, want %q", errb.String(), want)
	}
	s.DeleteMac(x)
	errb.Reset()
	st := drain(s)
	if want := "\n[error] :file=t.eln\tline=1\tpos=0\n[preprocessor] non defined macro 'X' used\n\t before  X end ENDOFLSTREAM \n"; st != 1 || !strings.HasPrefix(errb.String(), "\n[error] :") || !strings.Contains(errb.String(), "[preprocessor] non defined macro 'X' used\n") {
		t.Fatalf("status %d stderr %q, want like %q", st, errb.String(), want)
	}

	// warnings off: the position is not printed, the message is
	c2, _, errb2 := newCtx(false)
	s2 := New(c2, NewState(), lex.FromBytes("t.eln", []byte("a")), "t.eln", Mark{}, nil)
	s2.AddMac(id(c2, "X"), nil)
	s2.AddMac(id(c2, "X"), nil)
	if errb2.String() != "[mlstream] redefinition of macro X\n" {
		t.Fatalf("warnings off: %q", errb2.String())
	}
	// deleting a macro twice after one definition
	st = diag.Run(func() { s2.DeleteMac(id(c2, "X")); s2.DeleteMac(id(c2, "X")); s2.DeleteMac(id(c2, "X")) })
	if st != 1 || !strings.Contains(errb2.String(), "try to delete non existing macro, int.err.\n") {
		t.Fatalf("delete: %d %q", st, errb2.String())
	}
}

// The macro table holds MaxNofMac-1 = 49 macros.
func TestMacroOverflow(t *testing.T) {
	c, _, errb := newCtx(true)
	s := New(c, NewState(), lex.FromBytes("t.eln", []byte("a")), "t.eln", Mark{}, nil)
	st := diag.Run(func() {
		for i := 0; i < 49; i++ {
			s.AddMac(id(c, fmt.Sprintf("M%d", i)), nil)
		}
	})
	if st != 0 {
		t.Fatalf("49 macros: %d %s", st, errb)
	}
	st = diag.Run(func() { s.AddMac(id(c, "M49"), nil) })
	if st != 1 || errb.String() != "[addn] ERROR, mi tab overflowed over 50\n int.err.\n" {
		t.Fatalf("50th macro: %d %q", st, errb)
	}
}

// recEval is Mark, recording what the preprocessor asks.
type recEval struct {
	Mark
	log   []string
	terms [][]lex.Lexem
}

func (r *recEval) AddVars(s *Stream, sort string) bool {
	r.log = append(r.log, "sort "+sort)
	return true
}
func (r *recEval) AddVarRules(s *Stream, vars []Var) {
	for _, v := range vars {
		r.log = append(r.log, fmt.Sprintf("var %s:%s %d", s.Ctx().Alfsy(v.Name), v.Sort, v.Index))
	}
}
func (r *recEval) Strategy(s *Stream, v Var, name lex.Lexem) {
	r.log = append(r.log, fmt.Sprintf("strategy %s for %s", s.Ctx().Alfsy(name), s.Ctx().Alfsy(v.Name)))
}
func (r *recEval) Term(s *Stream, v *Var, stop func(lex.Lexem) bool) (Term, bool) {
	t, ok := r.Mark.Term(s, v, stop)
	if ok {
		var w []string
		for _, l := range t.([]lex.Lexem) {
			w = append(w, s.Ctx().Alfsy(l))
		}
		who := "IF"
		if v != nil {
			who = s.Ctx().Alfsy(v.Name)
		}
		r.log = append(r.log, fmt.Sprintf("term %s: %s (stop %s)", who, strings.Join(w, " "), s.Ctx().Alfsy(s.Fulex())))
	}
	return t, ok
}
func (r *recEval) DeleteVarRules(s *Stream, vars []Var) {
	for _, v := range vars {
		r.log = append(r.log, "delete "+s.Ctx().Alfsy(v.Name))
	}
}
func (r *recEval) Solutions(s *Stream, f *ForEach) Solutions {
	var b []string
	for _, l := range f.Body {
		b = append(b, s.Ctx().Alfsy(l))
	}
	r.log = append(r.log, fmt.Sprintf("foreach if=%v vars=%d conds=%d body: %s", f.If, len(f.Vars), len(f.Conds), strings.Join(b, " ")))
	return none{}
}

func TestForEachMark(t *testing.T) {
	ev := &recEval{}
	s, tr, _, errb := stream(t, "module m a FOR EACH x:int SUCH THAT x:=(s) f(1) :{ g(x) } b end", ev)
	if st := drain(s); st != 0 {
		t.Fatalf("status %d %s", st, errb)
	}
	want := "@begin t.eln\nid module\nid m\nid a\n@foreach-begin\n@foreach-end\nid b\nid end\neos\n@end\n"
	if got := noPos(tr.String()); got != want {
		t.Fatalf("trace %q\nwant %q", got, want)
	}
	log := []string{"sort int", "var x:int 0", "strategy s for x", "term x: f ( 1 ) (stop :)", "delete x",
		"foreach if=false vars=1 conds=1 body: g ( x )"}
	if !reflect.DeepEqual(ev.log, log) {
		t.Fatalf("log %q\nwant %q", ev.log, log)
	}
}

func TestForEachNestedAndConditions(t *testing.T) {
	ev := &recEval{}
	src := "module m FOR EACH x,y:list[pair[int,bool]]; z:int SUCH THAT x:=() a AND y:=(t) b ANDIF c AND IF d AND z:=() e :" +
		"{ FOR EACH w:int SUCH THAT w:=() x :{ w } } q end"
	s, tr, _, errb := stream(t, src, ev)
	if st := drain(s); st != 0 {
		t.Fatalf("status %d %s", st, errb)
	}
	want := "@begin t.eln\nid module\nid m\n@foreach-begin\n@foreach-end\nid q\nid end\neos\n@end\n"
	if got := noPos(tr.String()); got != want {
		t.Fatalf("trace %q\nwant %q", got, want)
	}
	log := []string{"sort list[pair[int,bool]]", "sort int",
		"var x:list[pair[int,bool]] 0", "var y:list[pair[int,bool]] 1", "var z:int 2",
		"term x: a (stop AND)", "strategy t for y", "term y: b (stop ANDIF)", "term IF: c (stop AND)",
		"term IF: d (stop AND)", "term z: e (stop :)", "delete z", "delete y", "delete x",
		"foreach if=false vars=3 conds=5 body: FOR EACH w : int SUCH THAT w : = ( ) x : { w }"}
	if !reflect.DeepEqual(ev.log, log) {
		t.Fatalf("log %q\nwant %q", ev.log, log)
	}
}

func TestIfMark(t *testing.T) {
	ev := &recEval{}
	s, tr, _, errb := stream(t, "module m IF verbose == 1 : { a } b end", ev)
	verbose := id(s.Ctx(), "verbose")
	s.AddMac(verbose, []lex.Lexem{lex.NumLex(1)})
	if st := drain(s); st != 0 {
		t.Fatalf("status %d %s", st, errb)
	}
	if got, want := noPos(tr.String()), "@begin t.eln\nid module\nid m\n@foreach-begin\n@foreach-end\nid b\nid end\neos\n@end\n"; got != want {
		t.Fatalf("trace %q want %q", got, want)
	}
	// verbose (a module parameter) is expanded in the condition
	if want := []string{"term IF: 1 = = 1 (stop :)", "foreach if=true vars=0 conds=1 body: a"}; !reflect.DeepEqual(ev.log, want) {
		t.Fatalf("log %q want %q", ev.log, want)
	}
}

// `IF c ANDIF c2`: the C++ parses the second condition from ANDIF on
// (iffirst is never reset for IF): Earley's syntax error on ANDIF.
func TestIfAndIf(t *testing.T) {
	s, _, _, errb := stream(t, "module m IF c ANDIF d : { a } end", Mark{})
	if st := drain(s); st != 1 || errb.String() != "\n[error] :file=t.eln\tline=0\tpos=26\n[earley] syntax error on symbol 'ANDIF'\n" {
		t.Fatalf("status %d stderr %q", st, errb)
	}
}

func TestNonAffected(t *testing.T) {
	s, _, _, errb := stream(t, "module m FOR EACH x,y:int SUCH THAT x:=() a :{ b } end", Mark{})
	if st := drain(s); st != 1 || !strings.Contains(errb.String(), "[preprocessor] non affected variable y in FOR EACH\n\t before  ") {
		t.Fatalf("status %d stderr %q", st, errb)
	}
}

func TestRefuse(t *testing.T) {
	s, _, _, errb := stream(t, "module m IF c : { a } end", Refuse{})
	if st := drain(s); st != 1 || !strings.HasSuffix(errb.String(), "[preprocessor] FOR EACH and IF are not evaluated before S8d\n") {
		t.Fatalf("status %d stderr %q", st, errb)
	}
	// Refuse checks the sort in the built-in typet, as the standalone C++
	s, _, _, errb = stream(t, "module m FOR EACH x:int SUCH THAT x:=() a :{ b } end", Refuse{})
	if st := drain(s); st != 1 || !strings.Contains(errb.String(), "[preprocessor] unknown type int used") {
		t.Fatalf("status %d stderr %q", st, errb)
	}
}

// A solution instantiates the variables as macros in the body, the body is
// read once per solution, the macros are deleted at the end.
type twoSols struct {
	Mark
	n int
}

func (e *twoSols) Solutions(s *Stream, f *ForEach) Solutions {
	return &listSols{l: [][][]lex.Lexem{{{lex.NumLex(1)}}, {{lex.NumLex(2), lex.CharLex('+')}}}}
}

type listSols struct{ l [][][]lex.Lexem }

func (l *listSols) Next() ([][]lex.Lexem, bool) {
	if len(l.l) == 0 {
		return nil, false
	}
	v := l.l[0]
	l.l = l.l[1:]
	return v, true
}

// @foreach-end is pending during the call that returns the body's
// next-to-last lexem: the stream reads one lexem ahead (macroexp, collident),
// as the C++ (programs/foreach: `@foreach-end` before the body's last `;`).
func TestForEachSolutions(t *testing.T) {
	s, tr, _, errb := stream(t, "module m FOR EACH x:int SUCH THAT x:=() a :{ f(x) } x end", &twoSols{})
	if st := drain(s); st != 0 {
		t.Fatalf("status %d %s", st, errb)
	}
	want := "@begin t.eln\nid module\nid m\n@foreach-begin\nid f\nchar 40\nnum 1\nchar 41\nid f\nchar 40\nnum 2\nchar 43\n@foreach-end\nchar 41\nid x\nid end\neos\n@end\n"
	if got := noPos(tr.String()); got != want {
		t.Fatalf("trace %q\nwant %q", got, want)
	}
}
