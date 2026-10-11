package lex

import (
	"bytes"
	"os"
	"path/filepath"
	"strconv"
	"strings"
	"testing"

	"elan/front/diag"
	"elan/front/tables"
)

const fixtures = "../../../../tests/front/fixtures"

func newCtx(warnings bool) (*Context, *bytes.Buffer, *bytes.Buffer) {
	var out, errb bytes.Buffer
	d := &diag.D{Out: &out, Err: &errb, Warnings: warnings}
	return &Context{D: d, Ident: tables.NewTabOfIdent(d)}, &out, &errb
}

// lexAll reads src (named name) to the end with a plain lstream and returns
// the trace in the harness format (with @inline lines), stderr and status.
func lexAll(src, name string, underID, warnings bool) (string, string, int) {
	c, out, errb := newCtx(warnings)
	st := diag.Run(func() {
		s := New(c, FromBytes(name, []byte(src)), underID)
		for {
			l := s.Ilex()
			TraceLexem(out, c, l)
			if in := s.LastInline(); in != nil {
				TraceInline(out, in)
			}
			if l.IsEOS() {
				break
			}
		}
	})
	return out.String(), errb.String(), st
}

// noPos removes the positions of id and str lines.
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

func check(t *testing.T, src string, underID bool, want string) {
	t.Helper()
	got, errs, st := lexAll(src, "t.eln", underID, true)
	if noPos(got) != want || errs != "" || st != 0 {
		t.Fatalf("%q:\ngot  %q (stderr %q, status %d)\nwant %q", src, noPos(got), errs, st, want)
	}
}

func TestLexemEncoding(t *testing.T) {
	if NoLexem != -256 || BlankLexem != -257 || IdentLexem != -259 || BofIdent != -260 ||
		StringLexem != -3260 || BofString != -3261 || JustNumber != -13261 || BofTypes != -13262 {
		t.Fatal("constants")
	}
	if l := IdLex(646); !l.IsIdent() || !l.IsRealID() || l.IdVal() != 646 || l != -906 {
		t.Fatal("IdLex")
	}
	if l := CharLex('('); !l.IsChar() || l.CharVal() != '(' || l.IsNum() {
		t.Fatal("CharLex")
	}
	if l := NumLex(0); !l.IsNum() || !l.IsRealNum() || l.NumVal() != 0 || l.IsChar() {
		t.Fatal("NumLex")
	}
	if l := TypeLex(3); l.Terminal() || l.TypeVal() != 3 {
		t.Fatal("TypeLex")
	}
	if !JustNumber.IsNum() || JustNumber.IsRealNum() || !IdentLexem.IsIdent() || IdentLexem.IsRealID() ||
		!StringLexem.IsString() || StringLexem.IsRealString() || !Lexem(BofString-5).IsRealString() ||
		Lexem(BofString-5).StrIndex() != 5 || !NoLexem.IsEOS() || !BlankLexem.IsBlank() {
		t.Fatal("special lexems")
	}
	c, _, _ := newCtx(false)
	for l, want := range map[Lexem]string{NoLexem: "ENDOFLSTREAM", BlankLexem: "BLANKLEXEM",
		IdentLexem: "IDENTIFIER", JustNumber: "NUMBER", StringLexem: "???", TypeLex(7): "sort(7)",
		NumLex(42): "42", CharLex('x'): "x", IdLex(646): "module", IdLex(0): "(null)"} {
		if got := c.Alfsy(l); got != want {
			t.Errorf("Alfsy(%d) = %q, want %q", l, got, want)
		}
	}
	// erralfsy: lexems 0 .. -32 as '\n' (the number 0 too: a C quirk)
	if c.ErrAlfsy(CharLex(10)) != `'\10'` || c.ErrAlfsy(NumLex(0)) != `'\0'` || c.ErrAlfsy(CharLex('x')) != "x" {
		t.Fatal("ErrAlfsy")
	}
}

func TestComments(t *testing.T) {
	check(t, "// a line\na /* b\n c */ d /**/ e // f\n", true, "id a\nid d\nid e\neos\n")
	check(t, "/*/ x */ y", true, "id y\neos\n") // "/*/" does not close
}

func TestCommentToEndOfFile(t *testing.T) {
	for _, c := range []struct{ src, err string }{
		{"a /* never", "\n[error] :file=t.eln\tline=0\tpos=11\n[lexan] comment throw end of input stream\n"},
		{"a // no newline", "\n[error] :file=t.eln\tline=0\tpos=16\n[lexan] comment throw end of input stream\n"},
		{"a /# never", "\n[error] :file=t.eln\tline=0\tpos=11\n[lexan] inline throw end of input stream\n"},
	} {
		out, errs, st := lexAll(c.src, "t.eln", true, true)
		if st != 1 || errs != c.err || out != "" { // the error comes while reading ahead of a
			t.Errorf("%q: out %q stderr %q status %d", c.src, out, errs, st)
		}
	}
}

func TestInlines(t *testing.T) {
	src := "a /# one #/ /# two #/ b"
	out, errs, st := lexAll(src, "t.eln", true, true)
	if noPos(out) != "id a\n@inline  two \nid b\neos\n" || st != 0 ||
		errs != "\n[warning] :file=t.eln\tline=0\tpos=14\n\ttwo succesives inline definitions,\n\tprevious one will be lost\n" {
		t.Fatalf("out %q stderr %q", out, errs)
	}
	if _, errs, _ := lexAll(src, "t.eln", true, false); errs != "" {
		t.Fatalf("warning without Warnings: %q", errs)
	}
	check(t, "a /#x#/ b", true, "id a\n@inline x\nid b\neos\n")
}

func TestStrings(t *testing.T) {
	check(t, `"a\"b\\c\nd\te\q" "" "x"`, true, "str a\\\"b\\\\c\\nd\\te\\\\q\nstr \nstr x\neos\n")
	c, out, _ := newCtx(true)
	s := New(c, FromBytes("t", []byte(`"one" "two"`)), true)
	if a, b := s.Ilex(), s.Ilex(); a.StrIndex() != 0 || b.StrIndex() != 1 || c.Strings[1] != "two" {
		t.Fatalf("string numbering %d %d", a, b)
	}
	_ = out
	// a NUL byte ends the C string (strdup)
	check(t, "\"ab\x00cd\" z", true, "str ab\nid z\neos\n")
}

func TestStringLength(t *testing.T) {
	check(t, `"`+strings.Repeat("a", 999)+`"`, true, "str "+strings.Repeat("a", 999)+"\neos\n")
	for _, src := range []string{`"` + strings.Repeat("a", 1000) + `"`, "\"abc\n"} {
		out, errs, st := lexAll(src, "t.eln", true, true)
		if out != "" || errs != "\nstring constant too long\n" || st != 1 {
			t.Fatalf("out %q stderr %q status %d", out, errs, st)
		}
	}
}

func TestTooManyStrings(t *testing.T) {
	src := strings.Repeat(`"s" `, tables.MaxNofString+1)
	out, errs, st := lexAll(src, "t.eln", true, true)
	if st != 1 || errs != "\ntoo many string constants\n" || strings.Count(out, "\n") != tables.MaxNofString-1 {
		t.Fatalf("stderr %q status %d lines %d", errs, st, strings.Count(out, "\n"))
	}
}

func TestBackQuotes(t *testing.T) {
	check(t, "`a` `\\n` `\\t` `\\\\` `\\\"` `1`", true, "num 97\nnum 10\nnum 9\nnum 92\nnum 34\nnum 49\neos\n")
	out, errs, st := lexAll("x `ab`", "t.eln", true, true)
	if noPos(out) != "" || errs != "\nwrong string constant\n" || st != 1 {
		t.Fatalf("out %q stderr %q status %d", out, errs, st)
	}
}

// Quirk 4: \n and \t outside strings are the identifiers "newline" and "tab".
func TestNewlineTabIdentifiers(t *testing.T) {
	got, _, _ := lexAll(`\n \t \x`, "t", true, true)
	if got != "id 10 \\n\nid 9 \\t\nchar 92\nid 120 x\neos\n" {
		t.Fatalf("got %q", got)
	}
}

// Quirk 2: identifiers keep their first 49 characters.
func TestLongIdentifier(t *testing.T) {
	id := "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567"
	check(t, id+" z", true, "id "+id[:49]+"\nid z\neos\n")
}

func TestUnderscore(t *testing.T) {
	check(t, "a_b c_1 _d e__f", true, "id a_b\nid c_1\nchar 95\nid d\nid e__f\neos\n")
	check(t, "a_b", false, "id a\nchar 95\nid b\neos\n")
}

// Quirk 1: numbers wrap modulo 2^32 into a C int.
func TestNumbers(t *testing.T) {
	check(t, "0 2147483647 2147483648 4294967295 4294967296 007", true,
		"num 0\nnum 2147483647\nother -2147483648\nchar 1\nnum 0\nnum 7\neos\n")
}

func TestHighBytes(t *testing.T) {
	check(t, "\xe9 \"caf\xe9\"", true, "char 233\nstr caf\\351\neos\n")
}

// Positions: the look-ahead counters of ichstream (fixtures/pos.lgi:
// "line=2 pos=14", the harness's values).
func TestPositions(t *testing.T) {
	src, err := os.ReadFile(filepath.Join(fixtures, "pos.lgi"))
	if err != nil {
		t.Fatal(err)
	}
	_, errs, _ := lexAll(string(src), "pos.lgi", true, true)
	if !strings.Contains(errs, "file=pos.lgi\tline=2\tpos=14\n") {
		t.Fatalf("stderr %q", errs)
	}
}

// The .lgi and .spc fixtures equal their harness output (stdout, stderr,
// status); the plain-lexer errors of the .eln fixtures give the harness's
// stderr and status (their stdout is the preprocessor's: task 7).
func TestFixtures(t *testing.T) {
	files, _ := filepath.Glob(filepath.Join(fixtures, "*.expected"))
	n := 0
	for _, e := range files {
		name := strings.TrimSuffix(filepath.Base(e), ".expected")
		src, err := os.ReadFile(filepath.Join(fixtures, name))
		if err != nil {
			t.Fatal(err)
		}
		want, _ := os.ReadFile(e)
		wout, rest, _ := strings.Cut(string(want), "@@stderr\n")
		werr, wst, _ := strings.Cut(rest, "@@status ")
		eln := strings.HasSuffix(name, ".eln")
		out, errs, st := lexAll(string(src), name, !eln, true)
		if eln {
			if wst != "1\n" || !lexerError(werr) {
				continue
			}
			out = wout
		}
		got := out + "@@stderr\n" + errs + "@@status " + strconv.Itoa(st) + "\n"
		if got != string(want) {
			t.Errorf("%s:\ngot\n%s\nwant\n%s", name, got, want)
		}
		n++
	}
	if n < 10 {
		t.Fatalf("only %d fixtures compared", n)
	}
}

func lexerError(s string) bool {
	for _, m := range []string{"[lexan]", "string constant too long", "wrong string constant"} {
		if strings.Contains(s, m) {
			return true
		}
	}
	return false
}

func TestNewLib(t *testing.T) {
	for _, c := range []struct {
		env, opt, pers string
		quote          bool
		want           Lib
	}{
		{"/p", "", "", false, Lib{"../", "/p/share/elanlib/common/", "/p/share/elanlib/noquote/", "/p/share/elanlib/strategy/", "/p/share/elanlib/ref/"}},
		{"/p/", "", "", true, Lib{"../", "/p//share/elanlib/common/", "/p//share/elanlib/quote/", "/p//share/elanlib/strategy/", "/p//share/elanlib/ref/"}},
		{"/p", "q", "d", false, Lib{"d/", "q/share/elanlib/common/", "q/share/elanlib/noquote/", "q/share/elanlib/strategy/", "q/share/elanlib/ref/"}},
	} {
		got, err := NewLib(c.env, c.opt, c.pers, c.quote)
		if err != nil || got != c.want {
			t.Errorf("NewLib(%q,%q,%q) = %+v, %v", c.env, c.opt, c.pers, got, err)
		}
	}
	if _, err := NewLib("", "", "", false); err == nil {
		t.Fatal("no library prefix accepted")
	}
}

// The lookup of ichstream(const char*): as given, then perslib, then the
// library directories, exact case there.
func TestLookup(t *testing.T) {
	root := t.TempDir()
	lib := filepath.Join(root, "inst")
	for _, d := range []string{"common", "noquote", "strategy", "ref"} {
		os.MkdirAll(filepath.Join(lib, "share/elanlib", d), 0o755)
		os.WriteFile(filepath.Join(lib, "share/elanlib", d, d+".eln"), []byte(d), 0o644)
	}
	os.WriteFile(filepath.Join(lib, "share/elanlib/common/Any.eln"), []byte("Any"), 0o644)
	os.MkdirAll(filepath.Join(root, "app"), 0o755)
	os.WriteFile(filepath.Join(root, "app/here.eln"), []byte("here"), 0o644)
	os.WriteFile(filepath.Join(root, "up.eln"), []byte("up"), 0o644)
	t.Chdir(filepath.Join(root, "app"))
	l, err := NewLib(lib, "", "", false)
	if err != nil {
		t.Fatal(err)
	}
	read := func(name string) (string, string, string, int) {
		c, _, errb := newCtx(false)
		c.Lib = l
		var text, actName string
		st := diag.Run(func() {
			cs := Open(c, name)
			actName = cs.ActName()
			for ch := cs.Ich(); ch != EOF; ch = cs.Ich() {
				text += string(rune(ch))
			}
		})
		return text, actName, errb.String(), st
	}
	for name, want := range map[string]string{"here.eln": "here", "up.eln": "up", "common.eln": "common",
		"noquote.eln": "noquote", "strategy.eln": "strategy", "ref.eln": "ref"} {
		if got, _, _, st := read(name); got != want || st != 0 {
			t.Errorf("%s: %q status %d", name, got, st)
		}
	}
	if _, an, _, _ := read("up.eln"); an != "../up.eln" {
		t.Errorf("ActName %q", an)
	}
	if _, an, _, _ := read("common.eln"); an != lib+"/share/elanlib/common/common.eln" {
		t.Errorf("ActName %q", an)
	}
	// a case-only match is refused in the library (any.eln vs Any.eln)
	if got, _, errs, st := read("any.eln"); st != 1 || errs != "[error] can't open input file 'any.eln'\n\tfatal\n" {
		t.Errorf("any.eln: %q %q %d", got, errs, st)
	}
	if _, _, errs, st := read("none.eln"); st != 1 || errs != "[error] can't open input file 'none.eln'\n\tfatal\n" {
		t.Errorf("none.eln: %q %d", errs, st)
	}
}

func TestBuffer(t *testing.T) {
	c, _, errb := newCtx(false)
	var b, cp Buffer
	if !b.IsEmpty() {
		t.Fatal("new buffer not empty")
	}
	for i := 0; i < 25; i++ { // more than a chunk of 10
		b.Put(NumLex(int32(i)))
	}
	b.Get(c.D)
	b.Copy(&cp)
	var app Buffer
	app.Put(CharLex('x'))
	b.Append(&app)
	for i := 1; i < 25; i++ {
		if l := cp.Get(c.D); l != NumLex(int32(i)) {
			t.Fatalf("copy %d: %d", i, l)
		}
	}
	if !cp.IsEmpty() || app.Get(c.D) != CharLex('x') || app.Get(c.D) != NumLex(1) {
		t.Fatal("copy/append")
	}
	b.Clear()
	if st := diag.Run(func() { b.Get(c.D) }); st != 1 || errb.String() != "[lbuffer::get] try to get from empty buff, internall error\n" {
		t.Fatalf("status %d stderr %q", st, errb.String())
	}
}
