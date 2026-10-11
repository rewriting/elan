package ir

import (
	"os"
	"strconv"
	"strings"
	"testing"

	"elan/ref"
)

// gramWords flattens rules into REM's int gram[] (rem/parser/REFParser.jj
// SymbolDeclaration): per rule 1, info, symbol, sort, n, then n pairs.
func gramWords(rules []GrammarRule) []int32 {
	var w []int32
	for _, r := range rules {
		w = append(w, 1, r.Info, r.Symbol, r.Sort, int32(len(r.Rhs)))
		for _, l := range r.Rhs {
			w = append(w, int32(l.Kind), l.Code)
		}
	}
	return w
}

// TestGrammarAsREM: ir.Grammar gives REM's gram[] for the same .ref file
// (testdata/<prog>.gram: the gram[] of the C that elanc -b -nosplit wrote
// for legacy Compiler.4.0/Test/<prog>, whose export is the golden
// C40-<prog>.cref).
func TestGrammarAsREM(t *testing.T) {
	for _, prog := range []string{"enum", "choice1", "propc1"} {
		src, err := os.ReadFile("../../../tests/golden/expected/C40-" + prog + ".cref")
		if err != nil {
			t.Fatal(err)
		}
		p, err := ref.Parse(src)
		if err != nil {
			t.Fatal(err)
		}
		gram, err := os.ReadFile("testdata/" + prog + ".gram")
		if err != nil {
			t.Fatal(err)
		}
		var want []int32
		for _, f := range strings.FieldsFunc(string(gram), func(r rune) bool { return r == ',' || r == '\n' || r == ' ' }) {
			n, err := strconv.Atoi(f)
			if err != nil {
				t.Fatal(err)
			}
			want = append(want, int32(n))
		}
		rules, err := Grammar(p)
		if err != nil {
			t.Fatal(err)
		}
		got := gramWords(rules)
		if len(got) != len(want) {
			t.Errorf("%s: %d words, REM %d", prog, len(got), len(want))
			continue
		}
		for i := range got {
			if got[i] != want[i] {
				t.Errorf("%s: word %d is %d, REM %d", prog, i, got[i], want[i])
				break
			}
		}
	}
}

// TestQueryRules: the operators that a query can build (the grammar rules
// reachable from the query's sort) are compiled even when the start term
// does not use them; one that cannot be compiled is refused at run time
// only (QueryUnsupported), and the rules of other sorts are not used.
func TestQueryRules(t *testing.T) {
	p := peano()
	p.op(sNat, 321, "double", []uint32{sNat}, 0, 0)
	p.rule(sNat, 1, f(321, v(0, sNat)), f(320, v(0, sNat), v(0, sNat)))
	p.op(sNat, 322, "io", []uint32{sNat}, 113, 0) // a builtin not supported
	p.op(sSet, 341, "single", []uint32{sNat}, 0, 0)
	ir := p.lower(t)
	q := ir.Start.Query
	if q == nil || q.Sort != sNat || len(q.Rules) != len(q.Grammar) {
		t.Fatalf("%+v", q)
	}
	for i, g := range q.Grammar {
		r := q.Rules[i]
		switch g.Symbol {
		case 318, 319, 320, 321:
			if r.Kind != QueryOp || ir.Ops[r.Op].Symbol != g.Symbol {
				t.Errorf("rule %d (symbol %d): %+v", i, g.Symbol, r)
			}
		case 322:
			if r.Kind != QueryUnsupported || !strings.Contains(r.Why, "113") {
				t.Errorf("io: %+v", r)
			}
		default:
			if r.Kind != QueryNone {
				t.Errorf("rule %d (symbol %d): %+v", i, g.Symbol, r)
			}
		}
	}
	for i, g := range q.Grammar {
		if op := &ir.Ops[q.Rules[i].Op]; g.Symbol == 321 && (op.Name != "double" || len(op.Rules) != 1) {
			t.Errorf("double is compiled with its rule: %+v", op)
		}
	}
}

// TestQueryRulesParserCodes: a user operator whose code is a semantic
// action of the parser (67-70, 81, 82: REM's parser builds another term
// for it) is refused when a query uses it; the `1:` rules of the .ref (s
// ::= intern int code 67) stay built by the parser.
func TestQueryRulesParserCodes(t *testing.T) {
	const sIntern = 19
	p := peano()
	p.p.Sorts = append(p.p.Sorts, ref.Sort{Index: sIntern, Name: "intern int", Builtin: true})
	for _, c := range []int32{67, 68, 69, 70, 81, 82} {
		p.op(sNat, c, "c"+strconv.Itoa(int(c)), []uint32{sNat}, c, 0)
	}
	p.p.Grammars.TopOnly = append(p.p.Grammars.TopOnly, ref.SortGrammar{Sort: sNat, Operators: []ref.Operator{
		{Flag: 1, Symbol: 67, Priority: 4000, Rhs: []ref.Lexem{{Kind: ref.LexType, N: sIntern}}}}})
	ir := p.lower(t)
	q := ir.Start.Query
	n := 0
	for i, g := range q.Grammar {
		r := q.Rules[i]
		switch {
		case g.Symbol == 67 && len(g.Rhs) == 1:
			if r.Kind != QueryNone {
				t.Errorf("nat ::= intern int: %+v", r)
			}
		case g.Symbol >= 67 && g.Symbol <= 70 || g.Symbol == 81 || g.Symbol == 82:
			n++
			if r.Kind != QueryUnsupported || !strings.Contains(r.Why, "code "+strconv.Itoa(int(g.Symbol))) {
				t.Errorf("c%d: %+v", g.Symbol, r)
			}
		}
	}
	if n != 6 {
		t.Errorf("%d user rules", n)
	}
}
