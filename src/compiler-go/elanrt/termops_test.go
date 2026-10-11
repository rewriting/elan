package elanrt

import "testing"

// Symbols of the tests of the term builtins: constants a, b, c, d, free
// f(@,@), g(@) with the rule g(g(x)) => x (applied by normG), var(@) and
// the AC symbol U of common_test.go.
var (
	symTa   = NewSym("a", 0)
	symTb   = NewSym("b", 0)
	symTc   = NewSym("c", 0)
	symTd   = NewSym("d", 0)
	symTf   = NewSym("f", 2)
	symTg   = NewSym("g", 1)
	symTvar = NewSym("var", 1)
	symFail = NewSym("fail", 0)
)

var (
	ta, tb, tc, td = Mk0(symTa), Mk0(symTb), Mk0(symTc), Mk0(symTd)
	tnil, tfail    = Mk0(symNil), Mk0(symFail)
)

func tf(x, y Term) Term   { return Mk2(symTf, x, y) }
func tg(x Term) Term      { return Mk1(symTg, x) }
func tv(i int32) Term     { return Mk1(symTvar, MkInt(i)) }
func cons(x, l Term) Term { return Mk2(symCons, x, l) }
func tu(xs ...Term) Term  { return MkAC(symU, Flatten(symU, xs...)) }

// normG is a normalisation dispatcher (the generated normTerm): g(g(x))
// => x, c U c => d (modulo an extension); the other terms are built.
func normG(sym int32, args []Term) Term {
	switch sym {
	case symTg:
		if args[0].Sym == symTg {
			return args[0].Args[0]
		}
	case symU:
		e := args
		if i := Find(e, tc); i >= 0 && Mult(e, i) >= 2 {
			rest := Rest(e, i, i+1)
			return NormAC(Insert(rest, td), func(e []Term) Term { return normG(symU, e) })
		}
	}
	return MkAny(sym, args)
}

func TestReplaceOnFreeTerms(t *testing.T) {
	x := tf(ta, tg(ta))
	if got := Replace(x, ta, tb, normG); got != tf(tb, tg(tb)) {
		t.Fatalf("replace a by b in %s = %s", Format(x), Format(got))
	}
	// unchanged subterms are shared: the same node
	y := tf(tb, tg(ta))
	if got := Replace(y, ta, tc, normG); got.Args[0] != y.Args[0] || got != tf(tb, tg(tc)) {
		t.Fatalf("replace a by c in %s = %s", Format(y), Format(got))
	}
	if got := Replace(y, td, tc, normG); got != y {
		t.Fatalf("nothing replaced: %s, not the same node", Format(got))
	}
	// the whole term
	if got := Replace(ta, ta, tb, normG); got != tb {
		t.Fatalf("replace a by b in a = %s", Format(got))
	}
	// the copied path renormalised (bottom-up): g(a) -> g(g(b)) -> b
	if got := Replace(tg(ta), ta, tg(tb), normG); got != tb {
		t.Fatalf("replace a by g(b) in g(a) = %s", Format(got))
	}
	// no replacement inside the new term
	if got := Replace(tf(ta, ta), ta, tf(ta, ta), normG); got != tf(tf(ta, ta), tf(ta, ta)) {
		t.Fatalf("replace a by f(a,a) in f(a,a) = %s", Format(got))
	}
	// ints and strings are left as they are, unless equal to the old term
	s := Mk2(symTf, MkInt(7), MkStr("x"))
	if got := Replace(s, MkInt(7), MkInt(8), normG); !Eq(got.Args[0], MkInt(8)) || got.Args[1] != s.Args[1] {
		t.Fatalf("replace 7 by 8 in %s = %s", Format(s), Format(got))
	}
	if got := Replace(s, MkStr("x"), MkStr("y"), normG); StrOf(got.Args[1]) != "y" {
		t.Fatalf("replace x by y in %s = %s", Format(s), Format(got))
	}
}

func TestReplaceOnACTerms(t *testing.T) {
	// a U b U a, a -> c: c U c U b, in canonical order, then c U c => d
	x := tu(ta, tb, ta)
	got := Replace(x, ta, tc, func(sym int32, args []Term) Term { return MkAny(sym, args) })
	if got != tu(tc, tc, tb) || Format(got) != "U(b,U(c,c))" && Format(got) != "b U c U c" {
		t.Fatalf("replace a by c in %s = %s", Format(x), Format(got))
	}
	if got := Replace(x, ta, tc, normG); got != tu(tb, td) {
		t.Fatalf("replace a by c in %s, renormalised = %s", Format(x), Format(got))
	}
	// a new element with the AC symbol is flattened: b -> b U c in a U b
	if got := Replace(tu(ta, tb), tb, tu(tb, tc), normG); got != tu(ta, tb, tc) {
		t.Fatalf("replace b by b U c in a U b = %s", Format(got))
	}
	// an AC sub-multiset is an occurrence (the interpreter's
	// stermreplace): a U b in a U b U c, and its repetitions
	if got := Replace(tu(ta, tb, tc), tu(ta, tb), td, normG); got != tu(tc, td) {
		t.Fatalf("replace a U b by d in a U b U c = %s", Format(got))
	}
	if got := Replace(tu(ta, tb, ta, tb, tc), tu(ta, tb), tb, normG); got != tu(tb, tb, tc) {
		t.Fatalf("replace a U b by b in a U a U b U b U c = %s", Format(got))
	}
	if got := Replace(tu(ta, ta, tb, tb), tu(ta, tb), tc, normG); got != td {
		t.Fatalf("replace a U b by c in a U a U b U b = %s, want d (c U c => d)", Format(got))
	}
	// not a sub-multiset: the elements one by one
	if got := Replace(tu(ta, tc), tu(ta, tb), td, normG); got != tu(ta, tc) {
		t.Fatalf("replace a U b by d in a U c = %s", Format(got))
	}
	// under a free symbol
	if got := Replace(tg(tu(ta, tb, tc)), tu(ta, tc), tb, normG); got != tg(tu(tb, tb)) {
		t.Fatalf("replace a U c by b in g(a U b U c) = %s", Format(got))
	}
}

func TestOccurs(t *testing.T) {
	cases := []struct {
		a, b Term
		want bool
	}{
		{ta, tf(tg(ta), tb), true},
		{tf(ta, tb), ta, false},
		{ta, ta, true},
		{MkInt(3), tf(MkInt(3), ta), true},
		{MkInt(3), tf(MkInt(4), ta), false},
		{MkStr("x"), tg(MkStr("x")), true},
		{tb, tu(ta, tb, tc), true},
		{td, tu(ta, tb), false},
		// a sub-multiset of an AC term (the interpreter's occursin)
		{tu(ta, tb), tu(ta, tb, tc), true},
		{tu(ta, tb), tg(tu(tc, tb, ta)), true},
		{tu(ta, td), tu(ta, tb, tc), false},
	}
	for _, c := range cases {
		if got := Occurs(c.a, c.b); got != c.want {
			t.Errorf("occurs %s in %s = %v, want %v", Format(c.a), Format(c.b), got, c.want)
		}
	}
}

func TestSyntacticMatch(t *testing.T) {
	vars := cons(tv(1), tnil)
	cases := []struct {
		pat, subj, vars, want Term
	}{
		// bimatch: f(var(1)) against f(a)
		{tg(tv(1)), tg(ta), vars, cons(ta, tnil)},
		{tg(tv(1)), tb, vars, tfail},
		// non-linear
		{tf(tv(1), tv(1)), tf(ta, tb), vars, tfail},
		{tf(tv(1), tv(1)), tf(ta, ta), vars, cons(ta, tnil)},
		// a variable that is not a leaf, two variables, in list order
		{tf(tg(tb), tv(2)), tf(tc, ta), cons(tv(2), cons(tg(tb), tnil)), cons(ta, cons(tc, tnil))},
		// no variable: nil when equal, else fail
		{tf(ta, tb), tf(ta, tb), tnil, tnil},
		{tf(ta, tb), tf(ta, tc), tnil, tfail},
		// a variable takes ints, strings and AC terms; ints and strings
		// match equal values
		{tv(1), MkInt(5), vars, cons(MkInt(5), tnil)},
		{tg(tv(1)), tg(tu(ta, tb)), vars, cons(tu(ta, tb), tnil)},
		{tf(MkInt(2), MkStr("s")), tf(MkInt(2), MkStr("s")), tnil, tnil},
		{tf(MkInt(2), MkStr("s")), tf(MkInt(2), MkStr("t")), tnil, tfail},
		// AC terms match only when equal (empty theories)
		{tu(ta, tb), tu(tb, ta), tnil, tnil},
		{tu(tv(1), tb), tu(ta, tb), vars, tfail},
	}
	for _, c := range cases {
		if got := SyntacticMatch(c.pat, c.subj, c.vars, tfail); !Eq(got, c.want) {
			t.Errorf("match %s against %s (vars %s) = %s, want %s",
				Format(c.pat), Format(c.subj), Format(c.vars), Format(got), Format(c.want))
		}
	}
}

func TestSyntacticUnify(t *testing.T) {
	v1, v2 := tv(1), tv(2)
	vs := cons(v1, cons(v2, tnil))
	// the interpreter's results (equalUnify, buildMGUResult): a variable
	// is replaced by the term it is bound to, followed through variables,
	// not inside it
	cases := []struct {
		pat, subj, vars, want Term
	}{
		{tf(v1, tb), tf(ta, v2), vs, cons(ta, cons(tb, tnil))},
		{tf(v1, v1), tf(ta, tb), cons(v1, tnil), tfail},
		{tf(v1, v2), tf(v2, tg(ta)), vs, cons(tg(ta), cons(tg(ta), tnil))},
		{tf(v1, ta), tf(tg(v2), v2), vs, cons(tg(v2), cons(ta, tnil))},
		{v1, v2, vs, cons(v2, cons(v2, tnil))},
		{tg(v1), tg(v1), cons(v1, tnil), cons(v1, tnil)},
		{tf(v2, v1), tf(tg(v1), ta), vs, cons(ta, cons(tg(v1), tnil))},
	}
	for _, c := range cases {
		if got := SyntacticUnify(c.pat, c.subj, c.vars, tfail); !Eq(got, c.want) {
			t.Errorf("unify %s and %s (vars %s) = %s, want %s",
				Format(c.pat), Format(c.subj), Format(c.vars), Format(got), Format(c.want))
		}
	}
}
