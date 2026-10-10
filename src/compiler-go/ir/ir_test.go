package ir

// Unit tests of the IR building (Lower): sort representations, totality,
// rule order in the decision trees, refusals, as elanc-rs's tests/ir.rs.
// Programs are built in memory with the .ref model.

import (
	"errors"
	"reflect"
	"slices"
	"strings"
	"testing"

	"elan/ref"
)

const (
	sInt  = 58 // builtinInt
	sBool = 428
	sNat  = 323
	sInts = 331 // int (injection of builtinInt)
	sSet  = 400
	sStr  = 77 // builtinString
)

type prog struct{ p ref.Program }

func f(symbol int32, args ...ref.Term) ref.Term { return ref.Fsym{Args: args, Symbol: symbol} }
func v(index, sort uint32) ref.Term             { return ref.Var{Index: index, Sort: sort} }

func newProg() *prog {
	p := &prog{p: ref.Program{
		Sorts: []ref.Sort{
			{Index: sInt, Name: "builtinInt", Builtin: true},
			{Index: sInts, Name: "int"},
			{Index: sNat, Name: "nat"},
			{Index: sSet, Name: "set"},
			{Index: sBool, Name: "bool", Builtin: true},
			{Index: sStr, Name: "builtinString", Builtin: true},
		},
		Query: ref.Query{Sort: sNat, ResultSort: sNat, StartWith: f(318), CheckWith: f(1)},
	}}
	p.op(sBool, 1, "true", nil, 0, 0)
	p.op(sBool, 0, "false", nil, 0, 0)
	return p
}

// op declares name(@,...) (or the injection @ when name is empty).
func (p *prog) op(sort uint32, symbol int32, name string, args []uint32, semantic, infos int32) {
	var rhs []ref.Lexem
	if name != "" {
		id := uint32(len(p.p.Identifiers) + 1)
		p.p.Identifiers = append(p.p.Identifiers, ref.TableEntry{Index: id, Name: name})
		rhs = append(rhs, ref.Lexem{Kind: ref.LexIdent, N: int64(id)})
	}
	if name != "" && len(args) > 0 {
		rhs = append(rhs, ref.Lexem{Kind: ref.LexChar, N: '('})
	}
	for i, a := range args {
		if i > 0 {
			rhs = append(rhs, ref.Lexem{Kind: ref.LexChar, N: ','})
		}
		rhs = append(rhs, ref.Lexem{Kind: ref.LexType, N: int64(a)})
	}
	if name != "" && len(args) > 0 {
		rhs = append(rhs, ref.Lexem{Kind: ref.LexChar, N: ')'})
	}
	o := ref.Operator{Flag: 3, Symbol: symbol, Syntax: ref.Printable, Semantic: semantic, Infos: infos, Rhs: rhs}
	b := &p.p.Grammars.Both
	for i := range *b {
		if (*b)[i].Sort == sort {
			(*b)[i].Operators = append((*b)[i].Operators, o)
			return
		}
	}
	*b = append(*b, ref.SortGrammar{Sort: sort, Builtin: sort == sInt || sort == sBool || sort == sStr,
		Operators: []ref.Operator{o}})
}

func (p *prog) rule(sort, nvars uint32, lhs, rhs ref.Term, wheres ...ref.Where) {
	p.p.Rules = append(p.p.Rules, ref.Rule{Name: ref.None, Sort: sort, Module: 1, Infos: 1,
		WhichMatch: 1, Nvars: nvars, Lhs: lhs, Body: ref.Plain{Rhs: rhs, Wheres: wheres}})
}

func (p *prog) start(t ref.Term, sort uint32) {
	p.p.Query.StartWith = t
	p.p.Query.ResultSort = sort
}

func (p *prog) lower(t *testing.T) *IR {
	t.Helper()
	ir, err := Lower(&p.p, "test")
	if err != nil {
		t.Fatalf("Lower: %v", err)
	}
	return ir
}

// peano: nat with o, s, plus and the two usual rules.
func peano() *prog {
	p := newProg()
	p.op(sNat, 318, "o", nil, 0, 0)
	p.op(sNat, 319, "s", []uint32{sNat}, 0, 0)
	p.op(sNat, 320, "plus", []uint32{sNat, sNat}, 0, 0)
	p.rule(sNat, 1, f(320, v(0, sNat), f(318)), v(0, sNat))
	p.rule(sNat, 2, f(320, v(0, sNat), f(319, v(1, sNat))), f(319, f(320, v(0, sNat), v(1, sNat))))
	p.start(f(320, f(318), f(318)), sNat)
	return p
}

func opNamed(t *testing.T, ir *IR, name string) *Op {
	t.Helper()
	for i := range ir.Ops {
		if ir.Ops[i].Name == name {
			return &ir.Ops[i]
		}
	}
	t.Fatalf("no operator %s", name)
	return nil
}

func sortNamed(t *testing.T, ir *IR, name string) (SortID, *Sort) {
	t.Helper()
	for i := range ir.Sorts {
		if ir.Sorts[i].Name == name {
			return i, &ir.Sorts[i]
		}
	}
	t.Fatalf("no sort %s", name)
	return 0, nil
}

func names(ir *IR, ops []OpID) []string {
	var v []string
	for _, o := range ops {
		v = append(v, ir.Ops[o].Name)
	}
	return v
}

func TestTotalOperatorHasNoVariant(t *testing.T) {
	ir := peano().lower(t)
	plus := opNamed(t, ir, "plus")
	if !plus.Total || plus.Variant {
		t.Fatalf("plus covers o and s: total %v, variant %v", plus.Total, plus.Variant)
	}
	_, nat := sortNamed(t, ir, "nat")
	if got := names(ir, nat.Variants); !slices.Equal(got, []string{"o", "s"}) {
		t.Fatalf("variants of nat: %v", got)
	}
	// the start term's operators only: bool is not reachable
	for _, s := range ir.Sorts {
		if s.Name == "bool" {
			t.Fatal("bool is reachable")
		}
	}
	if !plus.Recursive || opNamed(t, ir, "s").Recursive {
		t.Fatal("plus calls itself, s calls nothing")
	}
}

func TestAnotherConstructorMakesAnOperatorPartial(t *testing.T) {
	p := peano()
	p.op(sNat, 321, "inf", nil, 0, 0)
	// inf is reachable from the start term
	p.start(f(320, f(321), f(318)), sNat)
	ir := p.lower(t)
	plus := opNamed(t, ir, "plus")
	if plus.Total || !plus.Variant {
		t.Fatal("plus(x, inf) has no rule: plus can be stuck")
	}
	_, nat := sortNamed(t, ir, "nat")
	if got := names(ir, nat.Variants); !slices.Equal(got, []string{"o", "s", "plus", "inf"}) {
		t.Fatalf("variants of nat: %v", got)
	}
}

func TestRulesAreTriedInSourceOrder(t *testing.T) {
	// f(x) => o if c(x)  ;  f(s(x)) => x  ;  f(o) => s(o)
	p := newProg()
	p.op(sNat, 318, "o", nil, 0, 0)
	p.op(sNat, 319, "s", []uint32{sNat}, 0, 0)
	p.op(sNat, 320, "f", []uint32{sNat}, 0, 0)
	p.op(sBool, 321, "c", []uint32{sNat}, 0, 0)
	p.rule(sNat, 1, f(320, v(0, sNat)), f(318), ref.If{Cond: f(321, v(0, sNat))})
	p.rule(sNat, 1, f(320, f(319, v(0, sNat))), v(0, sNat))
	p.rule(sNat, 0, f(320, f(318)), f(319, f(318)))
	p.start(f(320, f(318)), sNat)
	ir := p.lower(t)
	fop := opNamed(t, ir, "f")
	if fop.Kind != Defined || !fop.Total {
		t.Fatal("the unconditional rules cover o and s")
	}
	// the conditional rule first, then a switch on the argument
	d := fop.Tree
	if d.Kind != Try || d.Rule != 0 {
		t.Fatalf("first: %+v", d)
	}
	sw := d.Next
	if sw.Kind != Switch || !slices.Equal(sw.Occ, Occ{0}) || sw.Default != nil {
		t.Fatalf("then a complete switch on the argument: %+v", sw)
	}
	var rules []RuleID
	for _, c := range sw.Cases {
		if c.Then.Kind != Try {
			t.Fatalf("case %+v", c)
		}
		rules = append(rules, c.Then.Rule)
	}
	if !slices.Equal(rules, []RuleID{1, 2}) {
		t.Fatalf("cases in order of appearance: %v", rules)
	}
	// c has no rule: a constructor of bool, so bool is boxed
	if k, ok := ir.Native(ir.Ops[fop.Rules[0].Conds[0].Expr.Op].Result); ok {
		t.Fatalf("bool with a constructor c is native %v", k)
	}
}

func TestNonLinearRulesAreConditions(t *testing.T) {
	// eq(x, x) => true ; eq(x, y) => false
	p := peano()
	p.op(sBool, 330, "eq", []uint32{sNat, sNat}, 0, 0)
	p.rule(sBool, 1, f(330, v(0, sNat), v(0, sNat)), f(1))
	p.rule(sBool, 2, f(330, v(0, sNat), v(1, sNat)), f(0))
	p.start(f(330, f(318), f(318)), sBool)
	ir := p.lower(t)
	eq := opNamed(t, ir, "eq")
	if !reflect.DeepEqual(eq.Rules[0].Conds, []Cond{{Kind: CondSame, A: 0, B: 1}}) {
		t.Fatalf("conditions %+v", eq.Rules[0].Conds)
	}
	if eq.Rules[0].Unconditional() || !eq.Total {
		t.Fatal("the linear rule covers everything")
	}
	// bool: native (true/false only)
	b, _ := sortNamed(t, ir, "bool")
	if k, ok := ir.Native(b); !ok || k != KindBool {
		t.Fatal("bool is not native")
	}
}

// ints: builtinInt and int with its injection and +, as in int.eln.
func ints() *prog {
	p := newProg()
	p.op(sInt, 3, "", []uint32{sInt, sInt}, 3, 0) // @ + @ on builtinInt, code 3
	p.op(sInts, 300, "", []uint32{sInt}, 0, 0)    // @ : (builtinInt) int
	p.op(sInts, 301, "plus", []uint32{sInts, sInts}, 0, 0)
	// @(x) + @(y) => @(z) where z := () x + y
	p.rule(sInts, 3, f(301, f(300, v(0, sInt)), f(300, v(1, sInt))), f(300, v(2, sInt)),
		ref.Assign{Lhs: v(2, sInt), Strategy: ref.None, Rhs: f(3, v(0, sInt), v(1, sInt))})
	p.op(sInts, 302, "g", []uint32{sNat}, 0, 0)
	p.op(sNat, 318, "o", nil, 0, 0)
	p.op(sNat, 319, "s", []uint32{sNat}, 0, 0)
	p.rule(sInts, 0, f(302, f(318)), f(300, ref.Int{Value: 0}))
	return p
}

func opBySymbol(t *testing.T, ir *IR, sym int32) *Op {
	t.Helper()
	for i := range ir.Ops {
		if ir.Ops[i].Symbol == sym {
			return &ir.Ops[i]
		}
	}
	t.Fatalf("no symbol %d", sym)
	return nil
}

func TestIntIsNativeWhenNothingCanBeStuck(t *testing.T) {
	p := ints()
	one := f(300, ref.Int{Value: 1})
	p.start(f(301, one, one), sInts)
	ir := p.lower(t)
	s, _ := sortNamed(t, ir, "int")
	fam := ir.FamilyOf(s)
	if fam == nil || fam.Kind != KindInt || ir.Sorts[fam.Base].Name != "builtinInt" || fam.Boxed() {
		t.Fatalf("family of int: %+v", fam)
	}
	if k, ok := ir.Native(s); !ok || k != KindInt {
		t.Fatal("int is not native")
	}
	if opBySymbol(t, ir, 300).Kind != Injection {
		t.Fatal("@ : (builtinInt) int is not an injection")
	}
	if !opNamed(t, ir, "plus").Total {
		t.Fatal("plus is not total")
	}
}

func TestIntIsBoxedWhenAnOperatorCanBeStuck(t *testing.T) {
	// g(s(o)) has no rule: g(s(o)) + 1 stays
	p := ints()
	g := f(302, f(319, f(318)))
	p.start(f(301, g, f(300, ref.Int{Value: 1})), sInts)
	ir := p.lower(t)
	s, _ := sortNamed(t, ir, "int")
	fam := ir.FamilyOf(s)
	if !fam.Boxed() {
		t.Fatal("int is native")
	}
	if got := names(ir, fam.Stuck); !slices.Equal(got, []string{"plus", "g"}) {
		t.Fatalf("int + over a stuck g, and g: %v", got)
	}
	// builtinInt + only sees values (no builtinInt term is stuck)
	if opBySymbol(t, ir, 300).Variant {
		t.Fatal("the injection has a variant")
	}
	if b3 := opBySymbol(t, ir, 3); !b3.Total || b3.Variant {
		t.Fatal("builtinInt + is not total")
	}
}

func TestPartialStringBuiltinBoxesItsResultSort(t *testing.T) {
	// s[i] (code 152) is undefined out of range: builtinInt is boxed
	p := newProg()
	p.op(sInt, 152, "", []uint32{sStr, sInt}, 152, 0)
	p.start(f(152, ref.Str{Codes: []int32{'a', 'b'}}, ref.Int{Value: 5}), sInt)
	ir := p.lower(t)
	idx := opBySymbol(t, ir, 152)
	if idx.Total || !idx.Variant || idx.Builtin != StrIndex {
		t.Fatalf("s[i]: %+v", idx)
	}
	i, _ := sortNamed(t, ir, "builtinInt")
	if _, ok := ir.Native(i); ok {
		t.Fatal("builtinInt is native")
	}
	s, _ := sortNamed(t, ir, "builtinString")
	if _, ok := ir.Native(s); !ok {
		t.Fatal("builtinString is boxed")
	}
}

func unsupportedStage(t *testing.T, p *prog) (string, string) {
	t.Helper()
	_, err := Lower(&p.p, "test")
	var u *Unsupported
	if !errors.As(err, &u) {
		t.Fatalf("not refused: %v", err)
	}
	return u.Stage, u.What
}

func TestUnsupportedConstructsAreRefusedWhenReachable(t *testing.T) {
	p := peano()
	p.op(sSet, 340, "u", []uint32{sSet, sSet}, 0, 2) // AC
	p.op(sSet, 341, "single", []uint32{sNat}, 0, 0)
	// not reachable: fine
	p.lower(t)
	p.start(f(340, f(341, f(318)), f(341, f(318))), sSet)
	if st, what := unsupportedStage(t, p); st != "S7c" || !strings.Contains(what, "AC") {
		t.Fatalf("%s %s", st, what)
	}
	// a start term with the query
	p = peano()
	p.start(f(319, v(0, sNat)), sNat)
	if st, _ := unsupportedStage(t, p); st != "S7d" {
		t.Fatal(st)
	}
	// a strategy
	p = peano()
	p.p.Query.Strategy = ref.Some(3)
	p.p.Strategies = append(p.p.Strategies, ref.StrategyDef{Name: 3, Sort: sNat, Module: 1,
		Body: ref.StrategySeq{{Op: ref.SDkRules, Rules: []uint32{1}}}})
	if st, _ := unsupportedStage(t, p); st != "S7b" {
		t.Fatal(st)
	}
	// `start with (id)` is a normalisation
	p.p.Strategies[0].Body = ref.StrategySeq{{Op: ref.SId}}
	p.lower(t)
	// a strategy call in a term, a where with a strategy
	p = peano()
	p.start(f(320, f(144, ref.Int{Value: 2}), f(318)), sNat)
	if st, _ := unsupportedStage(t, p); st != "S7b" {
		t.Fatal(st)
	}
	p = peano()
	p.op(sNat, 330, "h", []uint32{sNat}, 0, 0)
	p.rule(sNat, 2, f(330, v(0, sNat)), v(1, sNat),
		ref.Assign{Lhs: v(1, sNat), Strategy: ref.Some(4), Rhs: v(0, sNat)})
	p.start(f(330, f(318)), sNat)
	if st, _ := unsupportedStage(t, p); st != "S7b" {
		t.Fatal(st)
	}
	// an unsupported builtin code
	p = peano()
	p.op(sNat, 155, "x", []uint32{sNat}, 155, 0)
	p.start(f(155, f(318)), sNat)
	if st, what := unsupportedStage(t, p); st != "S7d" || !strings.Contains(what, "code 155") {
		t.Fatalf("%s %s", st, what)
	}
	// the codes of strategy terms (strsig/strat, Meta_strat, meta_apply)
	// are strategies (S7b), not builtins
	for _, code := range []int32{128, 147, -180, 179, 190, -205, 208} {
		p = peano()
		p.op(sNat, 156, "x", []uint32{sNat}, code, 0)
		p.start(f(156, f(318)), sNat)
		if st, what := unsupportedStage(t, p); st != "S7b" || !strings.Contains(what, "strategy term") {
			t.Fatalf("code %d: %s %s", code, st, what)
		}
	}
}

// A builtin operation needs values of builtin sorts: declared on a sort
// of terms (`@ pl @ : (t t) t code 3;`, the interpreter leaves the term),
// it is refused (elanc-go crashed on it).
func TestBuiltinOnASortOfTermsIsRefused(t *testing.T) {
	p := peano()
	p.op(sNat, 3, "pl", []uint32{sNat, sNat}, 3, 0) // + on nat
	p.start(f(3, f(318), f(318)), sNat)
	if st, what := unsupportedStage(t, p); st != "S7d" || !strings.Contains(what, "pl") {
		t.Fatalf("%s %s", st, what)
	}
	// a result of a sort of terms
	p = peano()
	p.op(sNat, 150, "len", []uint32{sStr}, 150, 0)
	p.start(f(150, ref.Str{Codes: []int32{'a'}}), sNat)
	if st, what := unsupportedStage(t, p); st != "S7d" || !strings.Contains(what, "len") {
		t.Fatalf("%s %s", st, what)
	}
	// and, or, not, ==, != take any term: fine on a sort of terms
	p = peano()
	p.op(sBool, 18, "eq", []uint32{sNat, sNat}, 18, 0)
	p.start(f(18, f(318), f(318)), sBool)
	p.lower(t)
}

func TestPrintingFormIsTheLastGrammarRule(t *testing.T) {
	p := peano()
	// an alias declared after: `@ + @` with the same symbol as plus
	g := &p.p.Grammars.Both[1]
	g.Operators = append(g.Operators, ref.Operator{Flag: 3, Symbol: 320, Rhs: []ref.Lexem{
		{Kind: ref.LexType, N: sNat}, {Kind: ref.LexChar, N: '+'}, {Kind: ref.LexType, N: sNat}}})
	ir := p.lower(t)
	want := []PrintItem{{Kind: PArg, Arg: 0}, {Kind: PChar, Char: '+'}, {Kind: PArg, Arg: 1}}
	if got := opNamed(t, ir, "plus").Print; !reflect.DeepEqual(got, want) {
		t.Fatalf("print %+v", got)
	}
}

func TestARuleThatTypesToNoProfileIsAnError(t *testing.T) {
	// plus(x, b) => x with b of sort bool: plus has no profile (nat bool)
	p := peano()
	p.rule(sNat, 2, f(320, v(0, sNat), v(1, sBool)), v(0, sNat))
	_, err := Lower(&p.p, "test")
	var inv *Invalid
	if !errors.As(err, &inv) || !strings.Contains(inv.Msg, "cannot type") || !strings.Contains(inv.Msg, "plus") {
		t.Fatalf("the rule must not be dropped silently: %v", err)
	}
}

func TestMutualRecursionIsGuarded(t *testing.T) {
	// even(s(x)) => odd(x) ; odd(s(x)) => even(x): both on a cycle
	p := peano()
	p.op(sNat, 330, "even", []uint32{sNat}, 0, 0)
	p.op(sNat, 331, "odd", []uint32{sNat}, 0, 0)
	p.op(sNat, 332, "two", []uint32{sNat}, 0, 0)
	p.rule(sNat, 1, f(330, f(319, v(0, sNat))), f(331, v(0, sNat)))
	p.rule(sNat, 1, f(331, f(319, v(0, sNat))), f(330, v(0, sNat)))
	p.rule(sNat, 1, f(332, v(0, sNat)), f(330, f(319, f(319, v(0, sNat)))))
	p.start(f(332, f(318)), sNat)
	ir := p.lower(t)
	if !opNamed(t, ir, "even").Recursive || !opNamed(t, ir, "odd").Recursive || opNamed(t, ir, "two").Recursive {
		t.Fatal("even and odd are on a cycle, two is not")
	}
}

func TestOpBaseNames(t *testing.T) {
	for _, c := range []struct {
		toks []string
		want string
	}{
		{[]string{"fact", "(", ")"}, "fact"},
		{[]string{"+"}, "plus"},
		{nil, "inj"},
		{[]string{"[", "]"}, "lb_rb"},
		{[]string{"string_strlen", "(", ")"}, "string_strlen"},
		{[]string{"1st"}, "n1st"},
		{[]string{"caf\xe9"}, "caf"}, // trailing _ trimmed
	} {
		if got := OpBaseName(c.toks); got != c.want {
			t.Errorf("%q: %s, want %s", c.toks, got, c.want)
		}
	}
	used := map[string]bool{}
	if unique(used, "a") != "a" || unique(used, "a") != "a_2" {
		t.Fatal("unique")
	}
}
