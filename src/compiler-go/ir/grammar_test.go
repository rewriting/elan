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

// TestQueryRules: when the start term uses the query, the operators that
// a query can build (the grammar rules reachable from the query's sort)
// are compiled, even those the start term does not reach; one that
// cannot be compiled is refused at run time only (QueryUnsupported), and
// the rules of other sorts are not used.
func TestQueryRules(t *testing.T) {
	p := peano()
	p.start(f(320, v(0, sNat), f(318)), sNat)
	p.op(sNat, 321, "double", []uint32{sNat}, 0, 0)
	p.rule(sNat, 1, f(321, v(0, sNat)), f(320, v(0, sNat), v(0, sNat)))
	p.op(sNat, 322, "io", []uint32{sNat}, 115, 0) // a builtin not supported
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
			if r.Kind != QueryUnsupported || !strings.Contains(r.Why, "115") {
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

// queryOnly is ints() with operators that only a query can contain (S7e
// task 2): a constant a : int (a second constructor of int) and, in the
// builtinInt grammar, btoi(@) : (bool) builtinInt (code 25), with p(@) :
// (int) bool partial (p(0) => true only, so a bool can be stuck); the
// query is of sort int, the start term 1 + 1 is ground.
func queryOnly() *prog {
	p := ints()
	p.op(sInts, 303, "a", nil, 0, 0)
	p.op(sInt, 25, "btoi", []uint32{sBool}, 25, 0)
	p.op(sBool, 304, "p", []uint32{sInts}, 0, 0)
	p.rule(sBool, 0, f(304, f(300, ref.Int{Value: 0})), f(1))
	one := f(300, ref.Int{Value: 1})
	p.start(f(301, one, one), sInts)
	p.p.Query.Sort = sInts
	return p
}

// queryRuleOf is the query rule of the grammar rule of symbol sym.
func queryRuleOf(t *testing.T, ir *IR, sym int32) QueryRule {
	t.Helper()
	q := ir.Start.Query
	for i, g := range q.Grammar {
		if g.Symbol == sym {
			return q.Rules[i]
		}
	}
	t.Fatalf("no grammar rule of symbol %d", sym)
	return QueryRule{}
}

// TestQueryOnlyOperatorsKeepIntNative: when the start term does not use
// the query, the query is parsed but not built (S7e task 13): no query
// operator is compiled (every rule QueryNone, none refused), so an
// operator that only the query reaches does not box a family native in
// the program.
func TestQueryOnlyOperatorsKeepIntNative(t *testing.T) {
	ir := queryOnly().lower(t)
	for _, n := range []string{"int", "builtinInt"} {
		s, _ := sortNamed(t, ir, n)
		if k, ok := ir.Native(s); !ok || k != KindInt {
			t.Errorf("%s is boxed", n)
		}
	}
	if ir.Start.Query == nil {
		t.Fatal("no query parser")
	}
	for i, r := range ir.Start.Query.Rules {
		if r.Kind != QueryNone {
			t.Errorf("rule %d (symbol %d): %+v", i, ir.Start.Query.Grammar[i].Symbol, r)
		}
	}
	for _, op := range ir.Ops {
		if op.Symbol == 303 || op.Symbol == 304 || op.Symbol == 25 {
			t.Errorf("%s is compiled", op.Decl)
		}
	}
}

// TestQueryUsedKeepsBoxing: when the start term uses the query, the
// query's operators are compiled as before (S7d task 5): int is boxed.
func TestQueryUsedKeepsBoxing(t *testing.T) {
	p := queryOnly()
	p.start(f(301, v(0, sInts), f(300, ref.Int{Value: 1})), sInts)
	ir := p.lower(t)
	s, _ := sortNamed(t, ir, "int")
	if _, ok := ir.Native(s); ok {
		t.Error("int is native")
	}
	if r := queryRuleOf(t, ir, 303); r.Kind != QueryOp {
		t.Errorf("a: %+v", r)
	}
	if r := queryRuleOf(t, ir, 25); r.Kind != QueryOp {
		t.Errorf("btoi: %+v", r)
	}
}

// TestReadSortsAreParsed: the sorts of the terms a program reads
// (read_builtinIO, code 120, S7e task 6) are parsed with the query's
// grammar: their operators are compiled even when the query's sort
// cannot contain them, as for a query that the start term uses.
func TestReadSortsAreParsed(t *testing.T) {
	p := peano()
	p.p.Query.Sort = sBool
	p.op(sNat, 330, "rd", []uint32{sInt, sNat}, 120, 0)
	p.start(f(330, ref.Int{Value: 3}, f(318)), sNat)
	ir := p.lower(t)
	q := ir.Start.Query
	if q == nil || q.Sort != sBool {
		t.Fatalf("%+v", q)
	}
	for i, g := range q.Grammar {
		if g.Symbol == 319 || g.Symbol == 320 {
			if r := q.Rules[i]; r.Kind != QueryOp || ir.Ops[r.Op].Symbol != g.Symbol {
				t.Errorf("rule %d (symbol %d): %+v", i, g.Symbol, r)
			}
		}
	}
	if rd := opBySymbol(t, ir, 330); rd.Kind != Builtin || rd.Builtin != IORead || !rd.Total {
		t.Errorf("rd: %+v", rd)
	}
}
