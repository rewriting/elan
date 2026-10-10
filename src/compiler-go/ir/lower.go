package ir

import (
	"cmp"
	"fmt"
	"slices"
	"strconv"
	"strings"

	"elan/ref"
)

// From the .ref model to the IR (S7a: free operators, unlabelled rules,
// builtins, the start term), as elanc-rs's lower.rs (S6a).
//
//  1. Operators. Every grammar rule declares a symbol with a profile
//     (argument sorts from its Type(n) lexems, result sort from its
//     GrammarForSort block). Builtin codes below 300 are overloaded, so an
//     operator is a (symbol, arguments, result) triple. Its printing form is
//     the last grammar rule of the symbol in the file, as the compiled C
//     runtime chooses it (findTextForm, src/compiler/runtime/termOut.c).
//  2. Typing of terms: a symbol is resolved to the first profile that fits
//     the sorts of its arguments and the expected sort.
//  3. Reachability from the start term: the operators of the start term,
//     then those of the rules (left-hand sides, conditions, right-hand
//     sides) of every reachable operator. Only reachable sorts and operators
//     are compiled; constructs that S7a does not support are refused only
//     when reachable ([Unsupported]).
//  4. Representation of the sorts: builtin families, sorts of terms.
//  5. Totality fixpoint: an operator is total when its unconditional rules
//     cover every normal form of its arguments, a normal form being a
//     constructor or a non-total operator at the top (greatest fixpoint: all
//     total at first, until nothing changes). Non-total operators get a
//     runtime symbol for their stuck terms.
//  6. Decision trees of the defined operators (matching.go), and the call
//     graph (recursive operators guard their depth).

// Unsupported is a construct of a later stage: Stage is S7b (strategies),
// S7c (AC) or S7d (query input and the rest). The compiler exits with
// status 2.
type Unsupported struct {
	Stage string
	What  string
}

func (e *Unsupported) Error() string { return "not supported yet (" + e.Stage + "): " + e.What }

// Invalid is a .ref file that is not what the compiler expects (a bug or a
// misread field).
type Invalid struct{ Msg string }

func (e *Invalid) Error() string { return "invalid program: " + e.Msg }

func unsupported(stage, format string, a ...any) error {
	return &Unsupported{Stage: stage, What: fmt.Sprintf(format, a...)}
}

func invalid(format string, a ...any) error {
	return &Invalid{Msg: fmt.Sprintf(format, a...)}
}

// mismatch is a typing failure that lets the typing try another profile.
type mismatch struct{ msg string }

func (m *mismatch) Error() string { return m.msg }

// callSymbol is the symbol of a strategy call in a term (Call,
// term/codes.h).
const callSymbol ref.SymbolCode = 144

// cand is an operator profile of the .ref file (a candidate when typing).
type cand struct {
	symbol   ref.SymbolCode
	args     []ref.SortIndex
	result   ref.SortIndex
	semantic int32
	infos    int32
	defstrat ref.DefStrat
	// decl are the lexems of the first grammar rule of this profile.
	decl []ref.Lexem
}

type ttKind int

const (
	ttOp ttKind = iota
	ttVar
	ttEVar
	ttInt
	ttStr
)

// tt is a typed term: operators resolved to candidates.
type tt struct {
	kind  ttKind
	cand  int
	args  []tt
	index uint32 // variable
	sort  ref.SortIndex
	i     int32
	s     string
}

type optSort struct {
	s  ref.SortIndex
	ok bool
}

func builtinKind(name string) (BuiltinKind, bool) {
	switch name {
	case "builtinInt":
		return KindInt, true
	case "bool":
		return KindBool, true
	case "builtinString":
		return KindStr, true
	}
	return 0, false
}

// decodeString decodes a STRING(...) term: C chars (signed bytes), kept as
// bytes; a string is a C string: it stops at a NUL byte.
func decodeString(cs []int32) string {
	b := make([]byte, 0, len(cs))
	for _, c := range cs {
		if byte(c) == 0 {
			break
		}
		b = append(b, byte(c))
	}
	return string(b)
}

// Lower builds the IR of a program; name is the program name.
func Lower(p *ref.Program, name string) (*IR, error) {
	return newLowerer(p).run(name)
}

type lowerer struct {
	p             *ref.Program
	cands         []cand
	bySymbol      map[ref.SymbolCode][]int
	printForm     map[ref.SymbolCode][]ref.Lexem
	rulesBySymbol map[ref.SymbolCode][]*ref.Rule
	intSort       optSort
	strSort       optSort
	boolSort      optSort
}

func newLowerer(p *ref.Program) *lowerer {
	l := &lowerer{
		p:             p,
		bySymbol:      map[ref.SymbolCode][]int{},
		printForm:     map[ref.SymbolCode][]ref.Lexem{},
		rulesBySymbol: map[ref.SymbolCode][]*ref.Rule{},
	}
	for _, r := range p.Operators() {
		o := r.Op
		var args []ref.SortIndex
		for _, x := range o.Rhs {
			if x.Kind == ref.LexType {
				args = append(args, ref.SortIndex(x.N))
			}
		}
		l.printForm[o.Symbol] = o.Rhs
		ids := l.bySymbol[o.Symbol]
		if slices.ContainsFunc(ids, func(i int) bool {
			return slices.Equal(l.cands[i].args, args) && l.cands[i].result == r.Grammar.Sort
		}) {
			continue
		}
		l.bySymbol[o.Symbol] = append(ids, len(l.cands))
		l.cands = append(l.cands, cand{
			symbol: o.Symbol, args: args, result: r.Grammar.Sort,
			semantic: o.Semantic, infos: o.Infos, defstrat: o.Defstrat, decl: o.Rhs,
		})
	}
	for i := range p.Rules {
		r := &p.Rules[i]
		// labelled rules: strategies only (S7b); the visibility (infos,
		// global or local) is ignored for normalisation, as REM does
		if r.Name.Valid {
			continue
		}
		if f, ok := r.Lhs.(ref.Fsym); ok {
			l.rulesBySymbol[f.Symbol] = append(l.rulesBySymbol[f.Symbol], r)
		}
	}
	named := func(n string) optSort {
		for _, s := range p.Sorts {
			if s.Name == n {
				return optSort{s.Index, true}
			}
		}
		return optSort{}
	}
	l.intSort, l.strSort, l.boolSort = named("builtinInt"), named("builtinString"), named("bool")
	return l
}

func (l *lowerer) sortName(s ref.SortIndex) string {
	if x := l.p.Sort(s); x != nil {
		return x.Name
	}
	return fmt.Sprintf("sort%d", s)
}

func (l *lowerer) ident(i uint32) string {
	if s, ok := l.p.Identifier(i); ok {
		return s
	}
	return fmt.Sprintf("ident%d", i)
}

func lexChar(n int64) string { return string([]byte{byte(n)}) }

func endsAlnum(s string) bool { return s != "" && isAlnum(s[len(s)-1]) }

// candDecl is `name(@,@) : (s1 s2) s`, for messages and comments.
func (l *lowerer) candDecl(c *cand) string {
	var s strings.Builder
	for _, x := range c.decl {
		switch x.Kind {
		case ref.LexType:
			s.WriteByte('@')
		case ref.LexChar:
			s.WriteString(lexChar(x.N))
		case ref.LexNum, ref.LexString:
			s.WriteString(strconv.FormatInt(x.N, 10))
		case ref.LexIdent:
			if endsAlnum(s.String()) {
				s.WriteByte(' ')
			}
			s.WriteString(l.ident(uint32(x.N)))
		case ref.LexBlank:
			s.WriteByte(' ')
		}
	}
	var args []string
	for _, a := range c.args {
		args = append(args, l.sortName(a))
	}
	text := strings.TrimSpace(s.String())
	if len(args) == 0 {
		return text + " : " + l.sortName(c.result)
	}
	return text + " : (" + strings.Join(args, " ") + ") " + l.sortName(c.result)
}

// ---------------------------------------------------------------------------
// Typing

func (l *lowerer) typeTerm(t ref.Term, expected optSort) (tt, error) {
	check := func(s ref.SortIndex, what string) error {
		if expected.ok && expected.s != s {
			return &mismatch{fmt.Sprintf("%s of sort %s where %s is expected", what, l.sortName(s), l.sortName(expected.s))}
		}
		return nil
	}
	switch t := t.(type) {
	case ref.Var:
		if err := check(t.Sort, "variable"); err != nil {
			return tt{}, err
		}
		return tt{kind: ttVar, index: t.Index, sort: t.Sort}, nil
	case ref.EVar:
		return tt{kind: ttEVar}, nil
	case ref.Int:
		if !l.intSort.ok {
			return tt{}, invalid("integer without builtinInt")
		}
		if err := check(l.intSort.s, "integer"); err != nil {
			return tt{}, err
		}
		return tt{kind: ttInt, i: int32(t.Value)}, nil
	case ref.Str:
		if !l.strSort.ok {
			return tt{}, invalid("string without builtinString")
		}
		if err := check(l.strSort.s, "string"); err != nil {
			return tt{}, err
		}
		return tt{kind: ttStr, s: decodeString(t.Codes)}, nil
	case ref.Ident:
		return tt{}, unsupported("S7d", "builtin identifiers (sort ident)")
	case ref.Fsym:
		if t.Symbol == callSymbol && len(t.Args) == 1 {
			if n, ok := t.Args[0].(ref.Int); ok {
				name := "?"
				if n.Value >= 0 {
					if s, ok := l.p.StrategyName(uint32(n.Value)); ok {
						name = s
					}
				}
				return tt{}, unsupported("S7b", "strategy call %s in a term", name)
			}
		}
		why := fmt.Sprintf("symbol %d has no profile", t.Symbol)
		for _, c := range l.bySymbol[t.Symbol] {
			k := &l.cands[c]
			if len(k.args) != len(t.Args) {
				why = fmt.Sprintf("symbol %d: arity", t.Symbol)
				continue
			}
			if expected.ok && expected.s != k.result {
				why = fmt.Sprintf("symbol %d of sort %s where %s is expected",
					t.Symbol, l.sortName(k.result), l.sortName(expected.s))
				continue
			}
			typed := make([]tt, 0, len(t.Args))
			ok := true
			for i, a := range t.Args {
				x, err := l.typeTerm(a, optSort{k.args[i], true})
				if m, isM := err.(*mismatch); isM {
					why, ok = m.msg, false
					break
				} else if err != nil {
					return tt{}, err
				}
				typed = append(typed, x)
			}
			if ok {
				return tt{kind: ttOp, cand: c, args: typed}, nil
			}
		}
		return tt{}, &mismatch{why}
	}
	return tt{}, invalid("unknown term %T", t)
}

func (l *lowerer) typeTop(t ref.Term, expected optSort, ctx string) (tt, error) {
	x, err := l.typeTerm(t, expected)
	if m, ok := err.(*mismatch); ok {
		return tt{}, invalid("%s: cannot type: %s", ctx, m.msg)
	}
	return x, err
}

func (l *lowerer) sortOf(t *tt) ref.SortIndex {
	switch t.kind {
	case ttOp:
		return l.cands[t.cand].result
	case ttVar:
		return t.sort
	case ttInt:
		return l.intSort.s
	case ttStr:
		return l.strSort.s
	}
	panic("sort of an extension variable")
}

func containsVar(t *tt) bool {
	if t.kind == ttVar {
		return true
	}
	return slices.ContainsFunc(t.args, func(a tt) bool { return containsVar(&a) })
}

func containsEVar(t *tt) bool {
	if t.kind == ttEVar {
		return true
	}
	return slices.ContainsFunc(t.args, func(a tt) bool { return containsEVar(&a) })
}

// ---------------------------------------------------------------------------
// The whole program

type reach struct {
	cands []int
	seen  map[int]bool
	todo  []int
	sorts map[ref.SortIndex]bool
}

func (r *reach) reach(c int) {
	if !r.seen[c] {
		r.seen[c] = true
		r.cands = append(r.cands, c)
		r.todo = append(r.todo, c)
	}
}

func (l *lowerer) run(name string) (*IR, error) {
	q := &l.p.Query
	// `start with (S) t`: only normalisation (or the identity) in S7a
	if q.Strategy.Valid {
		d := l.p.Strategy(q.Strategy.Index)
		identity := d != nil && len(d.Body) == 1 && d.Body[0].Op == ref.SId
		if !identity {
			n, _ := l.p.StrategyName(q.Strategy.Index)
			return nil, unsupported("S7b", "strategies: start with (%s)", n)
		}
	}
	start, err := l.typeTop(q.StartWith, optSort{q.ResultSort, true}, "start term")
	if err != nil {
		return nil, err
	}
	if containsVar(&start) {
		return nil, unsupported("S7d", "query input: the start term uses the query")
	}

	r := &reach{seen: map[int]bool{}, sorts: map[ref.SortIndex]bool{}}
	l.visit(&start, r)
	for len(r.todo) > 0 {
		c := r.todo[len(r.todo)-1]
		r.todo = r.todo[:len(r.todo)-1]
		if err := l.expand(c, r); err != nil {
			return nil, err
		}
	}
	sorts := slices.Sorted(func(yield func(ref.SortIndex) bool) {
		for s := range r.sorts {
			if !yield(s) {
				return
			}
		}
	})
	for _, s := range sorts {
		n := l.sortName(s)
		if n == "ident" || strings.HasPrefix(n, "intern ") {
			return nil, unsupported("S7d", "builtin sort %s", n)
		}
	}
	return (&builder{l: l, reach: r}).build(name, &start, q, sorts)
}

func (l *lowerer) visit(t *tt, r *reach) {
	switch t.kind {
	case ttOp:
		r.reach(t.cand)
		for i := range t.args {
			l.visit(&t.args[i], r)
		}
	case ttVar:
		r.sorts[t.sort] = true
	case ttInt:
		r.sorts[l.intSort.s] = true
	case ttStr:
		r.sorts[l.strSort.s] = true
	}
}

// strategyTermCode tells whether a semantic code (its absolute value) is
// that of a strategy term constructor, not of a builtin operation: the
// strategy constructors of strsig[X]/strat[X] (179..190, 205..208, DS_*
// in term/codes.h), of Meta_strat[X] (131..147) and meta_apply (128..130).
// The interpreter has no reduction for them; their terms are strategies
// (S7b), as in elanc-rs (builtin_semantic, lower.rs).
func strategyTermCode(code int32) bool {
	return 128 <= code && code <= 147 || 179 <= code && code <= 190 || 205 <= code && code <= 208
}

// expand checks a newly reached operator, then reaches what its rules use.
func (l *lowerer) expand(c int, r *reach) error {
	k := &l.cands[c]
	r.sorts[k.result] = true
	for _, a := range k.args {
		r.sorts[a] = true
	}
	switch k.infos {
	case 0:
	case 2:
		return unsupported("S7c", "AC operator %s", l.candDecl(k))
	default:
		return unsupported("S7c", "commutative operator %s", l.candDecl(k))
	}
	if k.defstrat.Kind != ref.DSNone {
		return unsupported("S7b", "operator with a default strategy %s", l.candDecl(k))
	}
	if l.boolSort.ok && k.result == l.boolSort.s {
		// true and false are values of every bool computation
		for _, sym := range []ref.SymbolCode{0, 1} {
			for _, i := range l.bySymbol[sym] {
				if l.cands[i].result == k.result && len(l.cands[i].args) == 0 {
					r.reach(i)
					break
				}
			}
		}
	}
	rules, err := l.rulesOf(c)
	if err != nil {
		return err
	}
	if k.semantic != 0 {
		code := k.semantic
		if code < 0 {
			code = -code
		}
		if strategyTermCode(code) {
			return unsupported("S7b", "strategy term operator %s (code %d)", l.candDecl(k), k.semantic)
		}
		if _, ok := BuiltinFromCode(code); !ok {
			return unsupported("S7d", "builtin operation code %d (%s)", code, l.candDecl(k))
		}
		if len(rules) > 0 {
			return unsupported("S7d", "rules for the builtin operation %s", l.candDecl(k))
		}
		return nil
	}
	for _, rl := range rules {
		if err := l.checkRule(rl.rule); err != nil {
			return err
		}
		l.visit(&rl.lhs, r)
		if containsEVar(&rl.lhs) {
			continue
		}
		body := rl.rule.Body.(ref.Plain)
		for _, w := range body.Wheres {
			ts, err := l.whereTerms(w, k)
			if err != nil {
				return err
			}
			for i := range ts {
				l.visit(&ts[i], r)
			}
		}
		rhs, err := l.typeTop(body.Rhs, optSort{rl.rule.Sort, true}, "rule for "+l.candDecl(k))
		if err != nil {
			return err
		}
		if containsEVar(&rhs) {
			return invalid("extension variable in a right-hand side (%s)", l.candDecl(k))
		}
		l.visit(&rhs, r)
	}
	return nil
}

func (l *lowerer) checkRule(r *ref.Rule) error {
	if r.WhichMatch == 0 {
		return unsupported("S7c", "rule with AC matching")
	}
	if _, ok := r.Body.(ref.Plain); !ok {
		return unsupported("S7d", "rule with a switch/case right-hand side")
	}
	return nil
}

func varSort(t ref.Term) optSort {
	if v, ok := t.(ref.Var); ok {
		return optSort{v.Sort, true}
	}
	return optSort{}
}

// whereTerms are the typed terms of a condition (patterns included).
func (l *lowerer) whereTerms(w ref.Where, k *cand) ([]tt, error) {
	ctx := "condition of a rule for " + l.candDecl(k)
	switch w := w.(type) {
	case ref.If:
		t, err := l.typeTop(w.Cond, l.boolSort, ctx)
		return []tt{t}, err
	case ref.Assign:
		if w.Strategy.Valid {
			n, _ := l.p.StrategyName(w.Strategy.Index)
			return nil, unsupported("S7b", "where with a strategy (%s) in %s", n, ctx)
		}
		r, err := l.typeTop(w.Rhs, varSort(w.Lhs), ctx)
		if err != nil {
			return nil, err
		}
		lt, err := l.typeTop(w.Lhs, optSort{l.sortOf(&r), true}, ctx)
		return []tt{lt, r}, err
	case ref.Match:
		if w.Strategy.Valid {
			n, _ := l.p.StrategyName(w.Strategy.Index)
			return nil, unsupported("S7b", "where with a strategy (%s) in %s", n, ctx)
		}
		pt, err := l.typeTop(w.Pattern, optSort{w.Sort, true}, ctx)
		if err != nil {
			return nil, err
		}
		r, err := l.typeTop(w.Rhs, optSort{w.Sort, true}, ctx)
		return []tt{pt, r}, err
	case ref.Try:
		return nil, unsupported("S7d", "choose/try conditions in %s", ctx)
	}
	return nil, invalid("unknown condition %T", w)
}

type typedRule struct {
	rule *ref.Rule
	lhs  tt
}

// rulesOf are the unlabelled rules whose left-hand side has operator c at
// the top, in file order, with their typed left-hand side.
func (l *lowerer) rulesOf(c int) ([]typedRule, error) {
	k := &l.cands[c]
	var v []typedRule
	for _, r := range l.rulesBySymbol[k.symbol] {
		if r.Sort != k.result {
			continue
		}
		t, err := l.typeTerm(r.Lhs, optSort{r.Sort, true})
		if m, ok := err.(*mismatch); ok {
			// a rule with the symbol at the top that fits no profile:
			// refused rather than silently ignored
			return nil, invalid("a rule of sort %s for symbol %d (%s): cannot type its left-hand side: %s",
				l.sortName(r.Sort), k.symbol, l.candDecl(k), m.msg)
		} else if err != nil {
			return nil, err
		}
		// else: a rule of another profile of the symbol (overloading)
		if t.kind == ttOp && t.cand == c {
			v = append(v, typedRule{r, t})
		}
	}
	return v, nil
}

// ---------------------------------------------------------------------------
// Building the IR from the reachable part

type preKind struct {
	kind    OpKind
	bool    bool
	builtin BuiltinOp
}

type builder struct {
	l      *lowerer
	reach  *reach
	opOf   map[int]OpID
	sortOf map[ref.SortIndex]SortID
}

func (b *builder) build(name string, start *tt, q *ref.Query, sortIndices []ref.SortIndex) (*IR, error) {
	l := b.l
	b.opOf = map[int]OpID{}
	b.sortOf = map[ref.SortIndex]SortID{}
	ir := &IR{Program: name}
	for _, s := range sortIndices {
		b.sortOf[s] = len(ir.Sorts)
		ir.Sorts = append(ir.Sorts, Sort{Index: s, Name: l.sortName(s), Family: -1})
	}
	// operators, by symbol code then profile
	cs := slices.Clone(b.reach.cands)
	slices.SortFunc(cs, func(x, y int) int {
		a, c := &l.cands[x], &l.cands[y]
		return cmp.Or(cmp.Compare(a.symbol, c.symbol), cmp.Compare(a.result, c.result), slices.Compare(a.args, c.args))
	})
	usedFuns := map[string]bool{}
	for _, c := range cs {
		b.opOf[c] = len(ir.Ops)
		k := &l.cands[c]
		base := OpBaseName(b.lexemTexts(k.decl))
		op := Op{
			Symbol: k.symbol,
			Name:   base,
			Fun:    unique(usedFuns, fmt.Sprintf("%s_%d", base, k.symbol)),
			Result: b.sortOf[k.result],
			Print:  b.printItems(k),
			Decl:   l.candDecl(k),
			Kind:   Constructor,
		}
		for _, a := range k.args {
			op.Args = append(op.Args, b.sortOf[a])
		}
		ir.Ops = append(ir.Ops, op)
	}

	// pre-kinds
	builtinOf := func(s SortID) (BuiltinKind, bool) { return builtinKind(ir.Sorts[s].Name) }
	pre := make([]preKind, len(ir.Ops))
	for i, c := range cs {
		k := &l.cands[c]
		rules, err := l.rulesOf(c)
		if err != nil {
			return nil, err
		}
		bk, isB := builtinOf(ir.Ops[i].Result)
		switch {
		case k.semantic != 0:
			code := k.semantic
			if code < 0 {
				code = -code
			}
			op, _ := BuiltinFromCode(code)
			pre[i] = preKind{kind: Builtin, builtin: op}
		case len(rules) > 0:
			pre[i] = preKind{kind: Defined}
		case isB && bk == KindBool && len(k.args) == 0 && (k.symbol == 0 || k.symbol == 1):
			pre[i] = preKind{kind: BoolConst, bool: k.symbol == 1}
		default:
			pre[i] = preKind{kind: Constructor}
		}
	}

	// families: builtin sorts, and the sorts whose only constructor is a
	// transparent injection of a builtin sort
	familyOf := map[SortID]FamilyID{}
	for s := range ir.Sorts {
		if bk, ok := builtinOf(s); ok {
			familyOf[s] = len(ir.Families)
			ir.Families = append(ir.Families, Family{Kind: bk, Base: s, Members: []SortID{s}})
		}
	}
	for s := range ir.Sorts {
		if _, ok := familyOf[s]; ok {
			continue
		}
		var ctors []OpID
		for o := range ir.Ops {
			if ir.Ops[o].Result == s && pre[o].kind == Constructor {
				ctors = append(ctors, o)
			}
		}
		if len(ctors) != 1 {
			continue
		}
		op := &ir.Ops[ctors[0]]
		if len(op.Args) != 1 {
			continue
		}
		if _, ok := builtinOf(op.Args[0]); ok && len(op.Print) == 1 && op.Print[0] == (PrintItem{Kind: PArg, Arg: 0}) {
			f := familyOf[op.Args[0]]
			ir.Families[f].Members = append(ir.Families[f].Members, s)
			familyOf[s] = f
			pre[ctors[0]] = preKind{kind: Injection}
		}
	}
	for s, f := range familyOf {
		ir.Sorts[s].Family = f
	}

	// a builtin operation computes a value of a builtin sort, from values
	// when it needs them: declared on sorts of terms (`@ pl @ : (t t) t
	// code 3`, the interpreter leaves the term as it is), it is not
	// compiled
	for i, c := range cs {
		if pre[i].kind != Builtin {
			continue
		}
		op := &ir.Ops[i]
		_, ok := familyOf[op.Result]
		for _, a := range op.Args {
			if _, isF := familyOf[a]; !isF && pre[i].builtin.NeedsValues() {
				ok = false
			}
		}
		if !ok {
			return nil, unsupported("S7d", "builtin operation %s on sorts that are not builtin (%s)",
				pre[i].builtin, l.candDecl(&l.cands[c]))
		}
	}

	// rules
	rules := make([][]Rule, len(ir.Ops))
	skipped := make([][]string, len(ir.Ops))
	for i, c := range cs {
		if pre[i].kind != Defined {
			continue
		}
		trs, err := l.rulesOf(c)
		if err != nil {
			return nil, err
		}
		for _, tr := range trs {
			rl := &ruleLowerer{b: b, rule: tr.rule, varOf: map[uint32]VarID{}}
			r, ok, err := rl.lower(&tr.lhs)
			if err != nil {
				return nil, err
			}
			if ok {
				rules[i] = append(rules[i], r)
			} else {
				skipped[i] = append(skipped[i], rl.render(&tr.lhs)+
					" (extension variable in the left-hand side: not compiled, as REM)")
			}
		}
	}

	// totality fixpoint
	total := make([]bool, len(ir.Ops))
	for i := range total {
		total[i] = true
	}
	sig := &signatures{ops: ir.Ops, pre: pre, familyOf: familyOf, families: ir.Families}
	for {
		sig.total = total
		next := make([]bool, len(ir.Ops))
		for o := range ir.Ops {
			switch pre[o].kind {
			case Defined:
				var rows [][]Pattern
				for _, r := range rules[o] {
					if r.Unconditional() {
						rows = append(rows, r.Lhs)
					}
				}
				next[o] = exhaustive(rows, ir.Ops[o].Args, sig)
			case Builtin:
				bo := pre[o].builtin
				next[o] = !bo.Partial()
				if bo.NeedsValues() {
					for _, a := range ir.Ops[o].Args {
						if !sig.valuesOnly(a) {
							next[o] = false
						}
					}
				}
			default:
				next[o] = true
			}
		}
		if slices.Equal(next, total) {
			break
		}
		total = next
	}
	sig.total = total

	// variants (constructors and operators that can be stuck), decision
	// trees, kinds
	for o := range ir.Ops {
		op := &ir.Ops[o]
		op.Kind, op.Bool, op.Builtin = pre[o].kind, pre[o].bool, pre[o].builtin
		op.Total = total[o]
		if sig.hasVariant(o) {
			op.Variant = true
			if f, ok := familyOf[op.Result]; ok {
				ir.Families[f].Stuck = append(ir.Families[f].Stuck, o)
			} else {
				ir.Sorts[op.Result].Variants = append(ir.Sorts[op.Result].Variants, o)
			}
		}
		if op.Kind == Defined {
			tree, never := decisionTree(rules[o], op.Args, sig)
			op.Rules, op.Tree, op.Skipped = rules[o], tree, skipped[o]
			for _, r := range never {
				op.Skipped = append(op.Skipped, rules[o][r].Text+
					" (never applies: an operator of its left-hand side is always reduced)")
			}
		}
	}
	markRecursive(ir)

	startExpr, err := b.expr(start, func(uint32) (VarID, bool) { return 0, false })
	if err != nil {
		return nil, err
	}
	ir.Start = Start{QuerySort: l.sortName(q.Sort), Sort: b.sortOf[l.sortOf(start)], Term: startExpr}
	return ir, nil
}

func (b *builder) lexemTexts(ls []ref.Lexem) []string {
	var v []string
	for _, x := range ls {
		switch x.Kind {
		case ref.LexIdent:
			v = append(v, b.l.ident(uint32(x.N)))
		case ref.LexChar:
			v = append(v, lexChar(x.N))
		case ref.LexNum:
			v = append(v, strconv.FormatInt(x.N, 10))
		}
	}
	return v
}

// printItems is the printing form: the last grammar rule of the symbol (C
// runtime); the operator's own declaration if that rule has another arity.
func (b *builder) printItems(k *cand) []PrintItem {
	arity := func(ls []ref.Lexem) int {
		n := 0
		for _, x := range ls {
			if x.Kind == ref.LexType {
				n++
			}
		}
		return n
	}
	ls := k.decl
	if f, ok := b.l.printForm[k.symbol]; ok && arity(f) == len(k.args) {
		ls = f
	}
	var items []PrintItem
	i := 0
	for _, x := range ls {
		switch x.Kind {
		case ref.LexType:
			items = append(items, PrintItem{Kind: PArg, Arg: i})
			i++
		case ref.LexChar:
			items = append(items, PrintItem{Kind: PChar, Char: byte(x.N)})
		case ref.LexNum:
			items = append(items, PrintItem{Kind: PNum, Num: x.N})
		case ref.LexIdent:
			items = append(items, PrintItem{Kind: PIdent, Text: b.l.ident(uint32(x.N))})
		}
	}
	return items
}

// expr is the expression of a typed term; varOf maps model variables.
func (b *builder) expr(t *tt, varOf func(uint32) (VarID, bool)) (Expr, error) {
	switch t.kind {
	case ttVar:
		v, ok := varOf(t.index)
		if !ok {
			return Expr{}, invalid("variable %d used before being bound", t.index)
		}
		return Expr{Kind: ExprVar, Var: v}, nil
	case ttOp:
		e := Expr{Kind: ExprOp, Op: b.opOf[t.cand]}
		for i := range t.args {
			a, err := b.expr(&t.args[i], varOf)
			if err != nil {
				return Expr{}, err
			}
			e.Args = append(e.Args, a)
		}
		return e, nil
	case ttInt:
		return Expr{Kind: ExprInt, Int: t.i}, nil
	case ttStr:
		return Expr{Kind: ExprStr, Str: t.s}, nil
	}
	return Expr{}, invalid("extension variable in an expression")
}

// signatures are the possible heads of the normal forms of each sort,
// during and after the totality analysis.
type signatures struct {
	ops      []Op
	pre      []preKind
	total    []bool
	familyOf map[SortID]FamilyID
	families []Family
}

// hasVariant tells whether a normal form with o at the top exists (a
// constructor, or an operator that can be stuck). Injections and booleans
// are values.
func (s *signatures) hasVariant(o OpID) bool {
	switch s.pre[o].kind {
	case Constructor:
		return true
	case Injection, BoolConst:
		return false
	}
	return !s.total[o]
}

// possible tells whether o can be at the top of a normal form.
func (s *signatures) possible(o OpID) bool {
	switch s.pre[o].kind {
	case Injection, BoolConst:
		return true
	}
	return s.hasVariant(o)
}

// heads are the heads of the normal forms of sort st, and whether this list
// is complete (false for the literals of builtinInt and builtinString).
func (s *signatures) heads(st SortID) ([]OpID, bool) {
	var v []OpID
	for o := range s.ops {
		if s.ops[o].Result == st && s.possible(o) {
			v = append(v, o)
		}
	}
	finite := true
	if f, ok := s.familyOf[st]; ok && s.families[f].Base == st {
		// literals: infinitely many values; bool: true, false
		finite = s.families[f].Kind == KindBool
	}
	return v, finite
}

// valuesOnly tells whether the normal forms of sort st are values only (no
// stuck term).
func (s *signatures) valuesOnly(st SortID) bool {
	for o := range s.ops {
		if s.ops[o].Result == st && s.hasVariant(o) {
			return false
		}
	}
	if f, ok := s.familyOf[st]; ok && s.families[f].Base != st {
		return s.valuesOnly(s.families[f].Base)
	}
	return true
}

// markRecursive sets Op.Recursive for the operators on a cycle of the call
// graph (Tarjan's strongly connected components). A function calls the
// functions of the operators built by its rules (conditions and right-hand
// sides); injections and booleans are inlined.
func markRecursive(ir *IR) {
	calls := make([][]OpID, len(ir.Ops))
	var collect func(o OpID, e *Expr)
	collect = func(o OpID, e *Expr) {
		if e.Kind != ExprOp {
			return
		}
		if k := ir.Ops[e.Op].Kind; k != Injection && k != BoolConst {
			calls[o] = append(calls[o], e.Op)
		}
		for i := range e.Args {
			collect(o, &e.Args[i])
		}
	}
	for o := range ir.Ops {
		for ri := range ir.Ops[o].Rules {
			r := &ir.Ops[o].Rules[ri]
			for ci := range r.Conds {
				if c := &r.Conds[ci]; c.Kind != CondSame {
					collect(o, &c.Expr)
				}
			}
			collect(o, &r.Rhs)
		}
	}
	index := make([]int, len(ir.Ops))
	low := make([]int, len(ir.Ops))
	onStack := make([]bool, len(ir.Ops))
	var stack []OpID
	next := 1
	var visit func(v OpID)
	visit = func(v OpID) {
		index[v], low[v] = next, next
		next++
		stack = append(stack, v)
		onStack[v] = true
		for _, w := range calls[v] {
			if index[w] == 0 {
				visit(w)
				low[v] = min(low[v], low[w])
			} else if onStack[w] {
				low[v] = min(low[v], index[w])
			}
		}
		if low[v] == index[v] {
			var comp []OpID
			for {
				w := stack[len(stack)-1]
				stack = stack[:len(stack)-1]
				onStack[w] = false
				comp = append(comp, w)
				if w == v {
					break
				}
			}
			rec := len(comp) > 1 || slices.Contains(calls[v], v)
			for _, w := range comp {
				ir.Ops[w].Recursive = rec
			}
		}
	}
	for o := range ir.Ops {
		if index[o] == 0 {
			visit(o)
		}
	}
}

// ---------------------------------------------------------------------------
// Rules

// ruleLowerer lowers one rule: variables, non-linearity, conditions.
type ruleLowerer struct {
	b    *builder
	rule *ref.Rule
	vars []Var
	// varOf maps a model variable to its IR variable (first occurrence).
	varOf map[uint32]VarID
	conds []Cond
}

// newVar is a new variable for model variable i (named x<i> as in the rule
// comments, x<i>_<n> for its other occurrences).
func (rl *ruleLowerer) newVar(i uint32, s ref.SortIndex) VarID {
	name := fmt.Sprintf("x%d", i)
	for n := 2; slices.ContainsFunc(rl.vars, func(v Var) bool { return v.Name == name }); n++ {
		name = fmt.Sprintf("x%d_%d", i, n)
	}
	rl.vars = append(rl.vars, Var{Name: name, Sort: rl.b.sortOf[s]})
	return len(rl.vars) - 1
}

// pattern is the pattern of a typed term; a variable already bound gives a
// fresh variable and an equality condition (same).
func (rl *ruleLowerer) pattern(t *tt, same *[]Cond) Pattern {
	switch t.kind {
	case ttVar:
		first, bound := rl.varOf[t.index]
		v := rl.newVar(t.index, t.sort)
		if bound {
			*same = append(*same, Cond{Kind: CondSame, A: first, B: v})
		} else {
			rl.varOf[t.index] = v
		}
		return Pattern{Kind: PatVar, Var: v}
	case ttOp:
		p := Pattern{Kind: PatOp, Op: rl.b.opOf[t.cand]}
		for i := range t.args {
			p.Args = append(p.Args, rl.pattern(&t.args[i], same))
		}
		return p
	case ttInt:
		return Pattern{Kind: PatInt, Int: t.i}
	case ttStr:
		return Pattern{Kind: PatStr, Str: t.s}
	}
	panic("rules with extension variables are skipped")
}

func (rl *ruleLowerer) expr(t *tt) (Expr, error) {
	e, err := rl.b.expr(t, func(i uint32) (VarID, bool) {
		v, ok := rl.varOf[i]
		return v, ok
	})
	if err != nil {
		return Expr{}, invalid("%s in a rule of sort %s", err.(*Invalid).Msg, rl.b.l.sortName(rl.rule.Sort))
	}
	return e, nil
}

// lower is the rule; false when it is not compiled (extension variables).
func (rl *ruleLowerer) lower(lhs *tt) (Rule, bool, error) {
	if containsEVar(lhs) {
		return Rule{}, false, nil
	}
	l := rl.b.l
	var same []Cond
	var pats []Pattern
	for i := range lhs.args {
		pats = append(pats, rl.pattern(&lhs.args[i], &same))
	}
	rl.conds = append(rl.conds, same...)
	body := rl.rule.Body.(ref.Plain)
	var texts []string
	for _, w := range body.Wheres {
		switch w := w.(type) {
		case ref.If:
			t, err := l.typeTop(w.Cond, l.boolSort, "condition")
			if err != nil {
				return Rule{}, false, err
			}
			texts = append(texts, "if "+rl.render(&t))
			e, err := rl.expr(&t)
			if err != nil {
				return Rule{}, false, err
			}
			rl.conds = append(rl.conds, Cond{Kind: CondIf, Expr: e})
		case ref.Assign:
			r, err := l.typeTop(w.Rhs, varSort(w.Lhs), "where")
			if err != nil {
				return Rule{}, false, err
			}
			lt, err := l.typeTop(w.Lhs, optSort{l.sortOf(&r), true}, "where")
			if err != nil {
				return Rule{}, false, err
			}
			e, err := rl.expr(&r)
			if err != nil {
				return Rule{}, false, err
			}
			texts = append(texts, "where "+rl.render(&lt)+" := () "+rl.render(&r))
			if _, bound := rl.varOf[lt.index]; lt.kind == ttVar && !bound {
				v := rl.newVar(lt.index, lt.sort)
				rl.varOf[lt.index] = v
				rl.conds = append(rl.conds, Cond{Kind: CondLet, A: v, Expr: e})
			} else {
				var same []Cond
				p := rl.pattern(&lt, &same)
				rl.conds = append(rl.conds, Cond{Kind: CondMatch, Pat: p, Expr: e})
				rl.conds = append(rl.conds, same...)
			}
		case ref.Match:
			r, err := l.typeTop(w.Rhs, optSort{w.Sort, true}, "where")
			if err != nil {
				return Rule{}, false, err
			}
			pt, err := l.typeTop(w.Pattern, optSort{w.Sort, true}, "where")
			if err != nil {
				return Rule{}, false, err
			}
			e, err := rl.expr(&r)
			if err != nil {
				return Rule{}, false, err
			}
			texts = append(texts, "where "+rl.render(&pt)+" := () "+rl.render(&r))
			var same []Cond
			p := rl.pattern(&pt, &same)
			rl.conds = append(rl.conds, Cond{Kind: CondMatch, Pat: p, Expr: e})
			rl.conds = append(rl.conds, same...)
		default:
			panic("refused during reachability")
		}
	}
	rt, err := l.typeTop(body.Rhs, optSort{rl.rule.Sort, true}, "right-hand side")
	if err != nil {
		return Rule{}, false, err
	}
	rhs, err := rl.expr(&rt)
	if err != nil {
		return Rule{}, false, err
	}
	text := "[] " + rl.render(lhs) + " => " + rl.render(&rt)
	for _, c := range texts {
		text += " " + c
	}
	return Rule{Text: text, Vars: rl.vars, Lhs: pats, Conds: rl.conds, Rhs: rhs}, true, nil
}

// render is the ELAN text of a typed term (variables x<model index>).
func (rl *ruleLowerer) render(t *tt) string {
	var out strings.Builder
	rl.b.render(t, &out)
	return out.String()
}

func (b *builder) render(t *tt, out *strings.Builder) {
	switch t.kind {
	case ttVar:
		if endsAlnum(out.String()) {
			out.WriteByte(' ')
		}
		fmt.Fprintf(out, "x%d", t.index)
	case ttEVar:
		out.WriteString("<evar>")
	case ttInt:
		if endsAlnum(out.String()) {
			out.WriteByte(' ')
		}
		fmt.Fprintf(out, "%d", t.i)
	case ttStr:
		out.WriteString(StringText(t.s))
	case ttOp:
		// the declaration's syntax (the printing form may be an alias)
		i := 0
		for _, x := range b.l.cands[t.cand].decl {
			switch x.Kind {
			case ref.LexType:
				b.render(&t.args[i], out)
				i++
			case ref.LexIdent:
				s := b.l.ident(uint32(x.N))
				if endsAlnum(out.String()) && s != "" && isAlnum(s[0]) {
					out.WriteByte(' ')
				}
				out.WriteString(s)
			case ref.LexChar:
				c := byte(x.N)
				if strings.IndexByte("+-*/=<>&|%", c) >= 0 {
					out.WriteByte(' ')
					out.WriteByte(c)
					out.WriteByte(' ')
				} else {
					out.WriteByte(c)
				}
			case ref.LexNum:
				fmt.Fprintf(out, "%d", x.N)
			}
		}
	}
}
