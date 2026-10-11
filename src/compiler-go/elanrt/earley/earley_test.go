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
