package earley

import (
	"bytes"
	"encoding/json"
	"errors"
	"fmt"
	"os"
	"strconv"
	"strings"
	"testing"
)

// poly1 are the tables of examples/poly1 compiled by REM (testdata/
// poly1.tables: its gram[], tabIdent and tabSort).
func poly1(t *testing.T) *Tables {
	t.Helper()
	src, err := os.ReadFile("testdata/poly1.tables")
	if err != nil {
		t.Fatal(err)
	}
	id := newIdents()
	sorts := map[int32]string{}
	var rules []Rule
	for _, line := range strings.Split(string(src), "\n") {
		f := strings.SplitN(line, " ", 3)
		switch f[0] {
		case "rule":
			var w []int32
			for _, x := range strings.Split(f[1], ",") {
				n, _ := strconv.Atoi(x)
				w = append(w, int32(n))
			}
			r := Rule{Info: w[1], Symbol: w[2], Sort: w[3]}
			for i := 0; i < int(w[4]); i++ {
				r.Rhs = append(r.Rhs, RhsLexem{Kind: uint8(w[5+2*i]), Code: w[6+2*i]})
			}
			rules = append(rules, r)
		case "ident":
			n, _ := strconv.Atoi(f[1])
			id.names[int32(n)], id.index[f[2]] = f[2], int32(n)
		case "sort":
			n, _ := strconv.Atoi(f[1])
			sorts[int32(n)] = f[2]
		}
	}
	return id.tables(rules, sorts)
}

// prefix builds the terms in prefix form: symbol(args), {AC elements}.
type prefix struct{ t *Tables }

func (p prefix) Op(r int, a []string) string {
	s := strconv.Itoa(int(p.t.Rules[r].Symbol))
	if len(a) > 0 {
		s += "(" + strings.Join(a, ",") + ")"
	}
	return s
}
func (p prefix) AC(r int, e []string) string {
	return strconv.Itoa(int(p.t.Rules[r].Symbol)) + "{" + strings.Join(e, ",") + "}"
}
func (prefix) Int(n int32) string   { return "#" + strconv.Itoa(int(n)) }
func (prefix) Ident(i int32) string { return "id" + strconv.Itoa(int(i)) }
func (prefix) Str(s string) string  { return strconv.Quote(s) }
func (p prefix) IsAC(r int) bool    { return p.t.Rules[r].Symbol == 999 }

func parse(t *testing.T, tab *Tables, sort int32, src string) (string, string, error) {
	t.Helper()
	var stderr bytes.Buffer
	tr, err := Parse(strings.NewReader(src), tab, sort, &stderr)
	if err != nil {
		return "", stderr.String(), err
	}
	return Build[string](tr, prefix{tab}), stderr.String(), nil
}

const polySort = 452

// TestParsePoly1: terms built (priorities, associativity, injections,
// overloading resolved by priorities).
func TestParsePoly1(t *testing.T) {
	tab := poly1(t)
	// 318 X, 319 poly <- variable, 320 poly <- int, 321 +, 322 * (poly);
	// 300 int <- builtinInt, 301 302 303 + - * (int), 67 number
	for _, c := range []struct{ src, want string }{
		{"X end", "319(318)"},
		{"X+X*X end", "321(319(318),322(319(318),319(318)))"},
		{"X*X+X end", "321(322(319(318),319(318)),319(318))"},
		// assocRight
		{"X+X+X end", "321(319(318),321(319(318),319(318)))"},
		// int's - is assocLeft
		{"3 - 2 - 1 end", "320(302(302(300(#3),300(#2)),300(#1)))"},
		{"deriv(X)", "323(319(318))"},
		{"X end junk", "319(318)"},
	} {
		got, stderr, err := parse(t, tab, polySort, c.src)
		if err != nil || got != c.want || stderr != "" {
			t.Errorf("%q: %s %q %v, want %s", c.src, got, stderr, err, c.want)
		}
	}
}

// TestParsePoly1AsREM: the messages (stderr) of REM's compiled poly1 on
// the same inputs (testdata/poly1.rem.json), byte for byte, and its
// statuses (an error: status 1).
func TestParsePoly1AsREM(t *testing.T) {
	tab := poly1(t)
	src, err := os.ReadFile("testdata/poly1.rem.json")
	if err != nil {
		t.Fatal(err)
	}
	var cases []struct {
		Input  string
		Status int
		Stderr string
	}
	if err := json.Unmarshal(src, &cases); err != nil {
		t.Fatal(err)
	}
	// rulings (package comment): REM reads past its arrays there
	differs := map[string]string{
		"deriv(X": "\n[error] :file=standard input\tline=0\tpos=8\n[earley] syntax error on symbol 'ENDOFLSTREAM'\n" +
			"\tone of ('+' '*' ')') expected\n\tin context deriv ( X  >>>ENDOFLSTREAM<<< ENDOFLSTREAM \n",
		`"a" end`: "",
	}
	for _, c := range cases {
		_, stderr, err := parse(t, tab, polySort, c.Input)
		want := c.Stderr
		if d, ok := differs[c.Input]; ok {
			if d == "" {
				want = strings.ReplaceAll(want, "(null)", `"a"`)
			} else {
				want = d
			}
		}
		if stderr != want {
			t.Errorf("%q: stderr\n%s\nREM:\n%s", c.Input, stderr, want)
		}
		if (err != nil) != (c.Status != 0) {
			t.Errorf("%q: error %v, REM status %d", c.Input, err, c.Status)
		}
		if err != nil && !errors.Is(err, ErrSyntax) {
			t.Errorf("%q: %v", c.Input, err)
		}
	}
}

// TestAmbiguity: an ambiguity that the priorities do not solve: REM's
// warning, the first parse chosen (1+2*3 of int injected into poly).
func TestAmbiguity(t *testing.T) {
	got, stderr, err := parse(t, poly1(t), polySort, "1+2*3 end")
	want := "\n[warning]earley: ambiguity can't be solved in string:\n 1 + 2 * 3\nbetween operators :\n" +
		"\t @ \t : (int)poly\t pri 0 code 320;\n\t @ '+'  @ \t : (poly poly)poly\t assocRight \t pri 1 code 321;\n"
	if err != nil || stderr != want || got != "320(301(300(#1),303(300(#2),300(#3))))" {
		t.Errorf("%s %v\n%s", got, err, stderr)
	}
}

// TestFatal: a lexical error and a term too long stop with status 1.
func TestFatal(t *testing.T) {
	tab := poly1(t)
	_, _, err := parse(t, tab, polySort, "X // no end")
	var f *Fatal
	if !errors.As(err, &f) || f.Status != 1 || !strings.Contains(f.Msg, "comment throw end") {
		t.Errorf("%v", err)
	}
	_, _, err = parse(t, tab, polySort, strings.Repeat("(", MaxLexems)+" end")
	if !errors.As(err, &f) || f.Msg != fmt.Sprintf("term is longer then MAXLENNTERM=%d lexems\n\tsorry, FATAL !!!\n", MaxLexems) {
		t.Errorf("%v", err)
	}
}

// TestAC: the arguments of an AC operator of the same symbol are
// flattened before it is built.
func TestAC(t *testing.T) {
	id := newIdents("end")
	a := id.add("a")
	// s ::= s + s (999, AC) | a
	rules := []Rule{
		{Sort: 7, Symbol: 999, Info: 0, Rhs: []RhsLexem{{RhsSort, 7}, {RhsChar, '+'}, {RhsSort, 7}}},
		{Sort: 7, Symbol: 500, Rhs: []RhsLexem{{RhsIdent, a}}},
	}
	tab := id.tables(rules, map[int32]string{7: "s"})
	got, stderr, err := parse(t, tab, 7, "a + a + a end")
	if err != nil || got != "999{500,500,500}" {
		t.Errorf("%s %v %s", got, err, stderr)
	}
}

// TestUserBuiltinCodes: a user rule whose code is one of the semantic
// actions of the parser (67-70, 81, 82) is an operator like any other
// (built by the builder, which refuses it), the actions applying only to
// the standard rules and to the `1:` rules of the .ref (s ::= intern int,
// intern ident, intern string).
func TestUserBuiltinCodes(t *testing.T) {
	id := newIdents("end", "(", ")")
	a, foo, bar := id.add("a"), id.add("foo"), id.add("bar")
	rules := []Rule{
		{Sort: 7, Symbol: 500, Rhs: []RhsLexem{{RhsIdent, a}}},
		{Sort: 7, Symbol: identRule, Rhs: []RhsLexem{{RhsIdent, foo}, {RhsChar, '('}, {RhsSort, 7}, {RhsChar, ')'}}},
		{Sort: 7, Symbol: numToTerm, Rhs: []RhsLexem{{RhsIdent, bar}, {RhsChar, '('}, {RhsSort, 7}, {RhsChar, ')'}}},
		{Sort: 7, Symbol: numToTerm, Rhs: []RhsLexem{{RhsSort, 19}}},
	}
	tab := id.tables(rules, map[int32]string{7: "s"})
	for _, c := range []struct{ src, want string }{
		{"foo(a) end", "70(500)"},
		{"bar(a) end", "67(500)"},
		{"bar(foo(3)) end", "67(70(#3))"},
	} {
		func() {
			defer func() {
				if x := recover(); x != nil {
					t.Errorf("%q: panic %v", c.src, x)
				}
			}()
			got, stderr, err := parse(t, tab, 7, c.src)
			if err != nil || got != c.want {
				t.Errorf("%q: %s %v %s, want %s", c.src, got, err, stderr, c.want)
			}
		}()
	}
}

// TestComputeTermNoRule: an operator entry without its rule stops with a
// Fatal (not a nil dereference).
func TestComputeTermNoRule(t *testing.T) {
	tr := &Tree{res: []int32{70}, kind: []int32{defaultRule}, str: []string{""}, rule: []*grule{nil}, pos: 1}
	defer func() {
		x := recover()
		if f, ok := x.(*Fatal); !ok || f.Status != 1 {
			t.Errorf("%v, want a Fatal", x)
		}
	}()
	Build[string](tr, prefix{&Tables{}})
}

// counting is prefix, counting the operators built.
type counting struct {
	prefix
	ops *int
}

func (c counting) Op(r int, a []string) string { *c.ops++; return c.prefix.Op(r, a) }

// TestACRepeatedElements: the equal elements (modulo AC) of an AC term
// are built once each, as REM merges them into a multiplicity before
// normalising (the rewrite steps of a query G(g(a), G(g(a), g(a))) are
// those of one g(a)).
func TestACRepeatedElements(t *testing.T) {
	id := newIdents("end")
	a, b, g := id.add("a"), id.add("b"), id.add("g")
	// s ::= s + s (999, AC) | a | b | g ( s )
	rules := []Rule{
		{Sort: 7, Symbol: 999, Info: 0, Rhs: []RhsLexem{{RhsSort, 7}, {RhsChar, '+'}, {RhsSort, 7}}},
		{Sort: 7, Symbol: 500, Rhs: []RhsLexem{{RhsIdent, a}}},
		{Sort: 7, Symbol: 501, Rhs: []RhsLexem{{RhsIdent, b}}},
		{Sort: 7, Symbol: 502, Rhs: []RhsLexem{{RhsIdent, g}, {RhsChar, '('}, {RhsSort, 7}, {RhsChar, ')'}}},
		// the same operator written another way: one grammar rule each
		{Sort: 7, Symbol: 502, Rhs: []RhsLexem{{RhsIdent, g}, {RhsChar, '['}, {RhsSort, 7}, {RhsChar, ']'}}},
	}
	tab := id.tables(rules, map[int32]string{7: "s"})
	for _, tc := range []struct {
		src, want string
		ops       int
	}{
		{"g(a) + g[a] + g(a) end", "999{502(500),502(500),502(500)}", 2},
		{"g(a) + g(a) + g(a) end", "999{502(500),502(500),502(500)}", 2},
		{"g(a) + g(b) + g(a) + b end", "999{502(500),502(501),502(500),501}", 5},
		// modulo AC: g(a + b) and g(b + a) are one element
		{"g(a + b) + g(b + a) end", "999{502(999{500,501}),502(999{500,501})}", 3},
		// the arguments of other operators are built each time
		{"g(g(a) + g(a)) end", "502(999{502(500),502(500)})", 3},
	} {
		var stderr bytes.Buffer
		tr, err := Parse(strings.NewReader(tc.src), tab, 7, &stderr)
		if err != nil {
			t.Fatalf("%s: %v %s", tc.src, err, stderr.String())
		}
		ops := 0
		if got := Build[string](tr, counting{prefix{tab}, &ops}); got != tc.want || ops != tc.ops {
			t.Errorf("%s: %s with %d operators built, want %s with %d", tc.src, got, ops, tc.want, tc.ops)
		}
	}
}
