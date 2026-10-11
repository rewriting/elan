package ir

import (
	"cmp"
	"fmt"
	"maps"
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
//  7. Strategies (S7b, as elanc-rs S6b): the named strategies reachable
//     from the query (`start with (S)`) and from the `where x := (S) t` of
//     the reachable rules, the labels they use (all the labelled rules of a
//     localised rule name, in source order, a decision tree over the whole
//     left-hand side), and which of them give at most one result (greatest
//     fixpoint, [StrategyDet]). A labelled rule whose left-hand side has an
//     operator that no term of the program contains is not compiled
//     ([lowerer.lhsPossible]). Strategy terms (strat[X]) need nothing more:
//     their sorts are ordinary sorts, their evaluation the rules and
//     strategies of strat[X] in the .ref file (see the package comment,
//     "Strategy terms"). `call "name":X` terms are resolved by name and sort
//     ([lowerer.resolveCall]); META gets a function per sort and an
//     evaluator of the Strategy[X] terms per X ([builder.metas]).

// Unsupported is a construct of a later stage: Stage is S7c (AC) or S7d
// (query input and the rest). The compiler exits with status 2.
type Unsupported struct {
	Stage string
	What  string
}

func (e *Unsupported) Error() string {
	if e.Stage == NotPlanned {
		return "not supported (" + NotPlanned + "): " + e.What
	}
	return "not supported yet (" + e.Stage + "): " + e.What
}

// NotPlanned is the stage of a refusal kept by design (S7d task 7): a
// construct no program can reach, one that the oracles do not compute
// (or compute differently), or one outside the compiled language (the
// reasons in docs/followups.md, "elanc-go: kept refusals").
const NotPlanned = "not planned"

// builtinRefusal is the refusal of a builtin operation code that elanc-go
// does not compile, by family of codes (the library's declarations).
func builtinRefusal(code int32, decl string) error {
	switch {
	case 30 <= code && code <= 33:
		return unsupported(NotPlanned, "builtin operation code %d (%s): the term order of cmp[X] (REM has none: its C does not compile; the interpreter's term::tcmp orders function symbols by its own indices and compares AC terms by matching)", code, decl)
	case 34 <= code && code <= 78:
		return unsupported(NotPlanned, "builtin operation code %d (%s): doubles (REM has none)", code, decl)
	case 90 <= code && code <= 112:
		return unsupported(NotPlanned, "builtin operation code %d (%s): a command of the interpreter's top level (Query[I,O,P])", code, decl)
	case 113 <= code && code <= 123:
		return unsupported("S7e", "builtin operation code %d (%s): input/output (stdio, files; REM's fun_113..123; no program of the tests uses them)", code, decl)
	case 160 <= code && code <= 174:
		return unsupported(NotPlanned, "builtin operation code %d (%s): a call of the ELAN system (meta-level: parsing, .ref of a program; REM has none)", code, decl)
	case 200 <= code && code <= 203:
		return unsupported("S7e", "builtin operation code %d (%s): arrays (REM's fun_200..203; the interpreter uses the codes 300-303: no oracle for the library's declarations)", code, decl)
	case code == 210:
		return unsupported(NotPlanned, "builtin operation code %d (%s): the hash of a term (REM's value depends on its representation)", code, decl)
	case code == 215:
		return unsupported(NotPlanned, "builtin operation code %d (%s): equality of molecules (a builtin of one application of REM)", code, decl)
	}
	return unsupported("S7e", "builtin operation code %d (%s)", code, decl)
}

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
	ttIdent // an identifier literal (i: its index)
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
	case "ident":
		return KindIdent, true
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
	printForm     map[profile][]ref.Lexem // the last grammar rule of each profile
	rulesBySymbol map[ref.SymbolCode][]*ref.Rule
	// rulesByLabel are the labelled rules of each localised label, in file
	// order.
	rulesByLabel map[ref.RuleNameIndex][]*ref.Rule
	intSort      optSort
	strSort      optSort
	boolSort     optSort
	identSort    optSort
}

// profile is an operator: its symbol, the sorts of its arguments and its
// result (a symbol is shared by the operators of several profiles, e.g.
// == of eq[X] on bool and on builtinInt).
type profile struct {
	symbol ref.SymbolCode
	args   string // the argument sorts, as text (a comparable key)
	result ref.SortIndex
}

func profileOf(symbol ref.SymbolCode, args []ref.SortIndex, result ref.SortIndex) profile {
	return profile{symbol, fmt.Sprint(args), result}
}

func newLowerer(p *ref.Program) *lowerer {
	l := &lowerer{
		p:             p,
		bySymbol:      map[ref.SymbolCode][]int{},
		printForm:     map[profile][]ref.Lexem{},
		rulesBySymbol: map[ref.SymbolCode][]*ref.Rule{},
		rulesByLabel:  map[ref.RuleNameIndex][]*ref.Rule{},
	}
	for _, r := range p.Operators() {
		o := r.Op
		var args []ref.SortIndex
		for _, x := range o.Rhs {
			if x.Kind == ref.LexType {
				args = append(args, ref.SortIndex(x.N))
			}
		}
		l.printForm[profileOf(o.Symbol, args, r.Grammar.Sort)] = o.Rhs
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
		// labelled rules: strategies only; the visibility (infos, global
		// or local) is ignored for normalisation, as REM does
		if r.Name.Valid {
			l.rulesByLabel[r.Name.Index] = append(l.rulesByLabel[r.Name.Index], r)
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
	l.identSort = named("ident")
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
		if !l.identSort.ok {
			return tt{}, invalid("identifier without the sort ident")
		}
		if err := check(l.identSort.s, "identifier"); err != nil {
			return tt{}, err
		}
		return tt{kind: ttIdent, i: int32(t.Index)}, nil
	case ref.Fsym:
		if t.Symbol == callSymbol && len(t.Args) == 1 {
			if n, ok := t.Args[0].(ref.Int); ok {
				// call "name":X (`call @':'X : (builtinString)
				// Strateg[X]`): the export replaces the string by the index
				// of the strategy it finds for the name, any sort
				// (ref/ref.cc); the term holds the name again, the
				// strategy is chosen with the sort when it is applied
				// ([lowerer.resolveCall]). -1: no strategy has this name
				if n.Value < 0 {
					return tt{}, unsupported(NotPlanned, "call \"name\":X of a name that no strategy has "+
						"(the export writes -1: the name is lost; the interpreter stops: `unknown strategy in call used`; REM crashes)")
				}
				full, ok := l.p.StrategyName(uint32(n.Value))
				if !ok {
					return tt{}, invalid("call of strategy %d: no name", n.Value)
				}
				name, _, _ := strings.Cut(full, ":")
				codes := make([]int32, len(name))
				for i := range len(name) {
					codes[i] = int32(int8(name[i]))
				}
				return l.typeTerm(ref.Fsym{Symbol: callSymbol, Args: []ref.Term{ref.Str{Codes: codes}}}, expected)
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
	case ttIdent:
		return l.identSort.s
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
	// the labels and strategies reached, and those still to expand
	labels, labelsTodo         []ref.RuleNameIndex
	strategies, strategiesTodo []ref.StrategyNameIndex
	// pending are the labelled rules of reached labels whose left-hand side
	// cannot match yet ([lowerer.lhsPossible])
	pending []pendingRule
	// calls are the call "name":X terms reached, by name and Strateg[X]
	calls map[callKey]callTargets
	// errs are the refusals met by visit (reported after the others)
	errs []error
}

type callKey struct {
	name    string
	strateg ref.SortIndex
}

// callTargets are the strategies of a call "name":X (see [MetaCall]).
type callTargets struct {
	typed      ref.StrategyNameIndex
	untyped    ref.StrategyNameIndex
	hasUntyped bool   // X is builtinInt or builtinString
	untypedWhy string // the untyped strategy is refused
}

type pendingRule struct {
	rule *ref.Rule
	lhs  tt
	ctx  string
}

func (r *reach) reach(c int) {
	if !r.seen[c] {
		r.seen[c] = true
		r.cands = append(r.cands, c)
		r.todo = append(r.todo, c)
	}
}

func (r *reach) reachLabel(l ref.RuleNameIndex) {
	if !slices.Contains(r.labels, l) {
		r.labels = append(r.labels, l)
		r.labelsTodo = append(r.labelsTodo, l)
	}
}

func (r *reach) reachStrategy(s ref.StrategyNameIndex) {
	if !slices.Contains(r.strategies, s) {
		r.strategies = append(r.strategies, s)
		r.strategiesTodo = append(r.strategiesTodo, s)
	}
}

func pop[T any](v *[]T) T {
	x := (*v)[len(*v)-1]
	*v = (*v)[:len(*v)-1]
	return x
}

func (l *lowerer) run(name string) (*IR, error) {
	q := &l.p.Query
	start, err := l.typeTop(q.StartWith, optSort{q.ResultSort, true}, "start term")
	if err != nil {
		return nil, err
	}

	r := &reach{seen: map[int]bool{}, sorts: map[ref.SortIndex]bool{}, calls: map[callKey]callTargets{}}
	l.visit(&start, r)
	if q.Strategy.Valid {
		r.reachStrategy(q.Strategy.Index)
	}
	if err := l.fixpoint(r); err != nil {
		return nil, err
	}
	sortsOf := func(r *reach) ([]ref.SortIndex, error) {
		sorts := slices.Sorted(maps.Keys(r.sorts))
		for _, s := range sorts {
			n := l.sortName(s)
			if strings.HasPrefix(n, "intern ") {
				return nil, unsupported(NotPlanned, "builtin sort %s (a sort of the parser's grammar)", n)
			}
		}
		return sorts, nil
	}
	if _, err := sortsOf(r); err != nil {
		return nil, err
	}

	// the operators of the query (S7d task 5): those of the grammar rules
	// that a term of the query's sort can contain; each one that cannot
	// be compiled is refused when a query uses it
	grammar, err := Grammar(l.p)
	if err != nil {
		return nil, unsupported(NotPlanned, "query grammar: %v (REM does not read it either)", err)
	}
	qrules := l.queryRules(grammar)
	base := r.clone()
	if !l.reachQuery(r, qrules) {
		r = base.clone()
		for i := range qrules {
			if qrules[i].Kind != QueryOp {
				continue
			}
			snap := r.clone()
			if err := l.reachQueryOp(r, qrules[i].cand); err != nil {
				r = snap
				qrules[i] = queryRule{QueryRule: QueryRule{Kind: QueryUnsupported, Why: err.Error()}}
			}
		}
	}
	sorts, err := sortsOf(r)
	if err == nil {
		var ir *IR
		ir, err = (&builder{l: l, reach: r, query: qrules, grammar: grammar}).build(name, &start, q, sorts)
		if err == nil {
			return ir, nil
		}
	}
	// the program alone, its query refused at run time
	why := err.Error()
	for i := range qrules {
		if qrules[i].Kind == QueryOp || qrules[i].Kind == QueryUmin {
			qrules[i] = queryRule{QueryRule: QueryRule{Kind: QueryUnsupported, Why: why}}
		}
	}
	sorts, err = sortsOf(base)
	if err != nil {
		return nil, err
	}
	return (&builder{l: l, reach: base, query: qrules, grammar: grammar}).build(name, &start, q, sorts)
}

// fixpoint expands what r has reached until nothing new is reached.
func (l *lowerer) fixpoint(r *reach) error {
	for {
		var err error
		if len(r.todo) > 0 {
			err = l.expand(pop(&r.todo), r)
		} else if len(r.labelsTodo) > 0 {
			err = l.expandLabel(pop(&r.labelsTodo), r)
		} else if len(r.strategiesTodo) > 0 {
			err = l.expandStrategy(pop(&r.strategiesTodo), r)
		} else if i := slices.IndexFunc(r.pending, func(p pendingRule) bool { return l.lhsPossible(&p.lhs, r) }); i >= 0 {
			// a labelled rule whose left-hand side has become possible
			p := r.pending[i]
			r.pending = slices.Delete(r.pending, i, i+1)
			err = l.acceptRule(p.rule, &p.lhs, p.ctx, r)
		} else {
			break
		}
		if err != nil {
			return err
		}
	}
	if len(r.errs) > 0 {
		return r.errs[0]
	}
	return nil
}

// clone is a copy of r (to undo what a query operator reaches).
func (r *reach) clone() *reach {
	c := &reach{
		cands: slices.Clone(r.cands), seen: maps.Clone(r.seen), todo: slices.Clone(r.todo),
		sorts: maps.Clone(r.sorts), labels: slices.Clone(r.labels), labelsTodo: slices.Clone(r.labelsTodo),
		strategies: slices.Clone(r.strategies), strategiesTodo: slices.Clone(r.strategiesTodo),
		pending: slices.Clone(r.pending), calls: maps.Clone(r.calls), errs: slices.Clone(r.errs),
	}
	return c
}

// queryRule is a QueryRule with the profile of its operator.
type queryRule struct {
	QueryRule
	cand int
}

// Codes of the query grammar (term/codes.h): the semantic actions of the
// parser (the lexical rules, built by the parser) and INTCONSTUMIN.
const (
	numToTerm      = 67 // NUMTOTERM
	identToTerm    = 68 // IDENTTOTERM
	numRule        = 69 // NUMRULE
	identRule      = 70 // IDENTRULE
	intConstUminus = 79 // INTCONSTUMIN
	stringToTerm   = 81 // STRINGTOTERM
	stringRule     = 82 // STRINGRULE
)

// parserCode tells whether a rule's code is a semantic action of REM's
// parser (esemact).
func parserCode(symbol int32) bool {
	switch symbol {
	case numToTerm, identToTerm, numRule, identRule, stringToTerm, stringRule:
		return true
	}
	return false
}

// lexicalRule tells whether g is a `1:` rule of the .ref, s ::= intern
// int (67), intern ident (68) or intern string (81), built by the parser
// (the only rules of the .ref to which the parser applies its action).
func (l *lowerer) lexicalRule(g *GrammarRule) bool {
	want := map[int32]string{numToTerm: "intern int", identToTerm: "intern ident", stringToTerm: "intern string"}[g.Symbol]
	return want != "" && len(g.Rhs) == 1 && g.Rhs[0].Kind == GramSort && l.sortName(ref.SortIndex(g.Rhs[0].Code)) == want
}

// queryRules says, per grammar rule, how a query builds its term: the
// rules of the sorts that a term of the query's sort can contain (its
// sort, and the sorts of the arguments of their rules) are built by
// their operators.
func (l *lowerer) queryRules(grammar []GrammarRule) []queryRule {
	in := map[int32]bool{int32(l.p.Query.Sort): true}
	for changed := true; changed; {
		changed = false
		for _, g := range grammar {
			if !in[g.Sort] {
				continue
			}
			for _, x := range g.Rhs {
				if x.Kind == GramSort && !in[x.Code] {
					in[x.Code], changed = true, true
				}
			}
		}
	}
	rules := make([]queryRule, len(grammar))
	for i, g := range grammar {
		if !in[g.Sort] || l.lexicalRule(&g) {
			continue
		}
		if parserCode(g.Symbol) {
			// REM's parser applies its action to it (S7d task 8 ruling)
			rules[i].Kind, rules[i].Why = QueryUnsupported,
				fmt.Sprintf("code %d is a semantic action of REM's parser, which builds another term", g.Symbol)
			continue
		}
		if g.Symbol == intConstUminus {
			rules[i].Kind = QueryUmin
			continue
		}
		var args []ref.SortIndex
		lexical := false
		for _, x := range g.Rhs {
			if x.Kind == GramSort {
				args = append(args, ref.SortIndex(x.Code))
				lexical = lexical || strings.HasPrefix(l.sortName(ref.SortIndex(x.Code)), "intern ")
			}
		}
		c := slices.IndexFunc(l.bySymbol[ref.SymbolCode(g.Symbol)], func(c int) bool {
			return slices.Equal(l.cands[c].args, args) && l.cands[c].result == ref.SortIndex(g.Sort)
		})
		switch {
		case lexical:
			rules[i].Kind, rules[i].Why = QueryUnsupported, fmt.Sprintf("code %d on a lexical sort", g.Symbol)
		case c < 0:
			rules[i].Kind, rules[i].Why = QueryUnsupported, fmt.Sprintf("code %d: no operator", g.Symbol)
		default:
			rules[i].Kind, rules[i].cand = QueryOp, l.bySymbol[ref.SymbolCode(g.Symbol)][c]
		}
	}
	return rules
}

// reachQuery reaches the operators of the query rules; false if one of
// them is refused (r is then to be dropped).
func (l *lowerer) reachQuery(r *reach, rules []queryRule) bool {
	for _, q := range rules {
		if q.Kind == QueryOp {
			r.reach(q.cand)
		}
	}
	return l.fixpoint(r) == nil
}

// reachQueryOp reaches one operator of the query.
func (l *lowerer) reachQueryOp(r *reach, c int) error {
	r.reach(c)
	return l.fixpoint(r)
}

func (l *lowerer) visit(t *tt, r *reach) {
	switch t.kind {
	case ttOp:
		r.reach(t.cand)
		if k := &l.cands[t.cand]; k.symbol == callSymbol && len(t.args) == 1 && t.args[0].kind == ttStr {
			if err := l.reachCall(t.args[0].s, k.result, r); err != nil {
				r.errs = append(r.errs, err)
			}
		}
		for i := range t.args {
			l.visit(&t.args[i], r)
		}
	case ttVar:
		r.sorts[t.sort] = true
	case ttInt:
		r.sorts[l.intSort.s] = true
	case ttStr:
		r.sorts[l.strSort.s] = true
	case ttIdent:
		r.sorts[l.identSort.s] = true
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

// builtinCode is the code of a builtin operation (the absolute value of the
// semantic code), false for an operator without one or with the code of a
// strategy term constructor ([strategyTermCode]), which is a constructor.
func builtinCode(semantic int32) (int32, bool) {
	code := semantic
	if code < 0 {
		code = -code
	}
	return code, code != 0 && !strategyTermCode(code)
}

func isBuiltin(semantic int32) bool { _, ok := builtinCode(semantic); return ok }

// expand checks a newly reached operator, then reaches what its rules use.
func (l *lowerer) expand(c int, r *reach) error {
	k := &l.cands[c]
	r.sorts[k.result] = true
	for _, a := range k.args {
		r.sorts[a] = true
	}
	if code := k.semantic; code == 135 || code == 136 || code == -135 || code == -136 {
		// dc/dk("l1" ... "ln"): Strateg[X] over labels named at run
		// time: every label of X can be named
		x, ok := metaParam(l.sortName(k.result), "Strateg")
		if !ok {
			return invalid("%s: not of sort Strateg[X]", l.candDecl(k))
		}
		for _, s := range l.p.Sorts {
			if s.Name != x {
				continue
			}
			for lb := range l.rulesByLabel {
				if l.labelFits(lb, s.Index) {
					r.reachLabel(lb)
				}
			}
		}
	}
	switch k.infos {
	case 0:
	case 2:
		if err := l.checkAC(k); err != nil {
			return err
		}
	default:
		// (the language has no commutative-only operators: never exported)
		return unsupported(NotPlanned, "commutative operator %s (the language has no commutative-only operators)", l.candDecl(k))
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
	if code, ok := builtinCode(k.semantic); ok {
		if _, ok := BuiltinFromCode(code); !ok {
			return builtinRefusal(code, l.candDecl(k))
		}
		if len(rules) > 0 {
			return unsupported("S7e", "rules for the builtin operation %s (no program has one)", l.candDecl(k))
		}
		return nil
	}
	ctx := "rule for " + l.candDecl(k)
	for _, rl := range rules {
		if err := l.acceptRule(rl.rule, &rl.lhs, ctx, r); err != nil {
			return err
		}
	}
	return nil
}

// acceptRule reaches what a rule uses (its left-hand side lhs is typed and
// can match).
func (l *lowerer) acceptRule(rule *ref.Rule, lhs *tt, ctx string, r *reach) error {
	if err := l.checkRule(rule); err != nil {
		return err
	}
	l.visit(lhs, r)
	if containsEVar(lhs) {
		return nil
	}
	body := rule.Body.(ref.Plain)
	for _, w := range body.Wheres {
		ts, err := l.whereTerms(w, ctx, r)
		if err != nil {
			return err
		}
		for i := range ts {
			l.visit(&ts[i], r)
		}
	}
	rhs, err := l.typeTop(body.Rhs, optSort{rule.Sort, true}, ctx)
	if err != nil {
		return err
	}
	if containsEVar(&rhs) {
		return invalid("extension variable in a right-hand side (%s)", ctx)
	}
	l.visit(&rhs, r)
	return nil
}

// alwaysPossible tells whether operator c can be at the top of a term
// without being built by an expression of the program: an injection of a
// builtin sort (the values of the builtin operations), true and false (the
// results of the comparisons). Conservative: keeping a rule is always
// correct.
func (l *lowerer) alwaysPossible(c int) bool {
	k := &l.cands[c]
	builtin := func(s ref.SortIndex) bool { _, ok := builtinKind(l.sortName(s)); return ok }
	return len(k.args) == 1 && builtin(k.args[0]) && !builtin(k.result) ||
		len(k.args) == 0 && (k.symbol == 0 || k.symbol == 1) && l.boolSort.ok && k.result == l.boolSort.s
}

// lhsPossible tells whether the left-hand side of a labelled rule can
// match a term of the program: each of its operators must be reached by
// an expression or by the left-hand side of an unlabelled rule (or be
// [lowerer.alwaysPossible]). The terms of a compiled program are built by
// its start term and by the right-hand sides and conditions of the
// reachable rules (the query is not read, S7d). A labelled rule whose
// left-hand side has an operator that no term contains never applies; it
// is not compiled and what it uses is not reached (e.g. the [TONE] rule of
// strat[X] when no strategy term tone(...) is built). Unlabelled rules are
// all reached with their operator, as in S7a: conservative.
func (l *lowerer) lhsPossible(lhs *tt, r *reach) bool {
	if lhs.kind != ttOp {
		return true
	}
	if !r.seen[lhs.cand] && !l.alwaysPossible(lhs.cand) {
		return false
	}
	for i := range lhs.args {
		if !l.lhsPossible(&lhs.args[i], r) {
			return false
		}
	}
	return true
}

// labelName is the name of a label (r1:nat/m!GL).
func (l *lowerer) labelName(i ref.RuleNameIndex) string {
	if s, ok := l.p.RuleName(i); ok {
		return s
	}
	return fmt.Sprintf("label%d", i)
}

// strategyName is the name of a strategy (go:nat/m).
func (l *lowerer) strategyName(i ref.StrategyNameIndex) string {
	if s, ok := l.p.StrategyName(i); ok {
		return s
	}
	return fmt.Sprintf("strategy%d", i)
}

// labelFits tells whether label i can apply to terms of sort s: a rule
// name of a strategy may resolve to the rules of another sort (they never
// match); a label without rules applies to no term.
func (l *lowerer) labelFits(i ref.RuleNameIndex, s ref.SortIndex) bool {
	rules := l.rulesByLabel[i]
	for _, r := range rules {
		if r.Sort != s {
			return false
		}
	}
	return len(rules) > 0
}

// expandLabel reaches the rules of a newly reached label (now, or when
// their left-hand side can match).
func (l *lowerer) expandLabel(i ref.RuleNameIndex, r *reach) error {
	ctx := "rule " + l.labelName(i)
	for _, rule := range l.rulesByLabel[i] {
		r.sorts[rule.Sort] = true
		lhs, err := l.typeTop(rule.Lhs, optSort{rule.Sort, true}, ctx)
		if err != nil {
			return err
		}
		if l.lhsPossible(&lhs, r) {
			if err := l.acceptRule(rule, &lhs, ctx, r); err != nil {
				return err
			}
		} else {
			r.pending = append(r.pending, pendingRule{rule, lhs, ctx})
		}
	}
	return nil
}

// expandStrategy reaches what the definition of a newly reached strategy
// uses.
func (l *lowerer) expandStrategy(s ref.StrategyNameIndex, r *reach) error {
	name := l.strategyName(s)
	d := l.p.Strategy(s)
	if d == nil {
		return invalid("strategy %s is not defined", name)
	}
	r.sorts[d.Sort] = true
	return l.visitSeq(d.Body, d.Sort, name, r)
}

func (l *lowerer) visitSeq(seq ref.StrategySeq, sort ref.SortIndex, name string, r *reach) error {
	for _, e := range seq {
		switch e.Op {
		case ref.SId, ref.SFail:
		case ref.SOneRules, ref.SDcRules, ref.SDkRules:
			for _, lb := range e.Rules {
				if l.labelFits(lb, sort) {
					r.reachLabel(lb)
				}
			}
		case ref.SOneStrats, ref.SDcStrats, ref.SDkStrats:
			for _, a := range e.Alts {
				if err := l.visitSeq(a, sort, name, r); err != nil {
					return err
				}
			}
		case ref.SRepeat, ref.SIterate, ref.STAll, ref.STOne, ref.STSome, ref.SRewrite:
			// (a traversal applies its body to the arguments of the term,
			// of any sort: the rules of its labels apply to the terms of
			// their sort, the strategies it calls are compiled for each
			// sort they meet when the code is emitted)
			if err := l.visitSeq(e.Body, sort, name, r); err != nil {
				return err
			}
		case ref.SNormIn, ref.SNormOut:
			for _, lb := range e.Rules {
				if l.labelFits(lb, sort) {
					r.reachLabel(lb)
				}
			}
		case ref.SCall:
			if d := l.p.Strategy(e.Call); d != nil && d.Sort != sort {
				return invalid("%s calls %s of another sort", name, l.strategyName(e.Call))
			}
			r.reachStrategy(e.Call)
		case ref.SMeta:
			// the meta_apply terms are reached by the rules that build
			// them; the strategies of their calls by reachCall
		case ref.SDcCall, ref.SDkCall:
			return unsupported(NotPlanned, "strategy %s (external processes, in %s: REM has none)", e.Op, name)
		default:
			return invalid("unknown strategy %v (in %s)", e.Op, name)
		}
	}
	return nil
}

// checkWhereStrategy checks a strategy applied by a where to terms of sort
// s.
func (l *lowerer) checkWhereStrategy(st ref.StrategyNameIndex, s ref.SortIndex, ctx string) error {
	name := l.strategyName(st)
	d := l.p.Strategy(st)
	switch {
	case d == nil:
		return invalid("strategy %s is not defined (%s)", name, ctx)
	case d.Sort != s:
		return invalid("strategy %s of sort %s applied to a term of sort %s (%s)",
			name, l.sortName(d.Sort), l.sortName(s), ctx)
	}
	return nil
}

// checkAC checks an AC operator: two arguments of its result sort, a sort
// of terms (not a builtin family: refused as S7d, see builtinAC), a
// printing form with its two arguments in order.
// builtinAC is the refusal of an AC operator on a builtin sort or a sort
// with an injection of one (not planned, S7d task 7): no program of the
// bench needs it; the oracles disagree (programs/refused/acbuiltin:
// pl(pl(1,2),3) for the interpreter, which does not flatten it, pl(1,pl(2,3))
// for REM), order ints as REM's tagged words in Eker's matcher, and strings
// under an AC symbol are a fatal error of the interpreter and crash REM's
// compiler (the S7c plan).
const builtinAC = "AC operator %s on a builtin sort (the oracles disagree; strings under an AC symbol make both fail)"

func (l *lowerer) checkAC(k *cand) error {
	if len(k.args) != 2 || k.args[0] != k.result || k.args[1] != k.result {
		return unsupported("S7e", "AC operator %s whose arguments are not of its result sort (its terms are flattened: (s s) s; no program of the bench declares one)", l.candDecl(k))
	}
	if _, ok := builtinKind(l.sortName(k.result)); ok || isBuiltin(k.semantic) {
		return unsupported(NotPlanned, builtinAC, l.candDecl(k))
	}
	// (an injection of a builtin sort into the sort makes it a family:
	// checked when the IR is built)
	var args []int64
	for _, x := range l.printForm[profileOf(k.symbol, k.args, k.result)] {
		if x.Kind == ref.LexType {
			args = append(args, x.N)
		}
	}
	if len(args) != 2 {
		return unsupported("S7e", "AC operator %s printed without its two arguments (REM prints an AC term as a right comb of its form; no program of the bench has one)", l.candDecl(k))
	}
	return nil
}

func (l *lowerer) checkRule(r *ref.Rule) error {
	if _, ok := r.Body.(ref.Plain); !ok {
		return unsupported(NotPlanned, "rule with a switch/case right-hand side (SWRULE: the interpreter's parser no longer reads switch, NO_MORE_SWITCH in load/msemact.cc; REM does not read it)")
	}
	return nil
}

func varSort(t ref.Term) optSort {
	if v, ok := t.(ref.Var); ok {
		return optSort{v.Sort, true}
	}
	return optSort{}
}

// whereTerms are the typed terms of a condition (patterns included); the
// strategies of the wheres are reached.
func (l *lowerer) whereTerms(w ref.Where, ruleCtx string, reach *reach) ([]tt, error) {
	ctx := "condition of a " + ruleCtx
	switch w := w.(type) {
	case ref.If:
		t, err := l.typeTop(w.Cond, l.boolSort, ctx)
		return []tt{t}, err
	case ref.Assign:
		r, err := l.typeTop(w.Rhs, varSort(w.Lhs), ctx)
		if err != nil {
			return nil, err
		}
		lt, err := l.typeTop(w.Lhs, optSort{l.sortOf(&r), true}, ctx)
		if err != nil {
			return nil, err
		}
		if w.Strategy.Valid {
			if err := l.checkWhereStrategy(w.Strategy.Index, l.sortOf(&r), ctx); err != nil {
				return nil, err
			}
			reach.reachStrategy(w.Strategy.Index)
		}
		return []tt{lt, r}, nil
	case ref.Match:
		if w.Strategy.Valid {
			if err := l.checkWhereStrategy(w.Strategy.Index, w.Sort, ctx); err != nil {
				return nil, err
			}
			reach.reachStrategy(w.Strategy.Index)
		}
		pt, err := l.typeTop(w.Pattern, optSort{w.Sort, true}, ctx)
		if err != nil {
			return nil, err
		}
		r, err := l.typeTop(w.Rhs, optSort{w.Sort, true}, ctx)
		return []tt{pt, r}, err
	case ref.Try:
		var ts []tt
		for _, br := range w.Branches {
			for _, x := range br {
				t, err := l.whereTerms(x, ruleCtx, reach)
				if err != nil {
					return nil, err
				}
				ts = append(ts, t...)
			}
		}
		return ts, nil
	}
	return nil, invalid("unknown condition %T", w)
}

// resolveCall is the strategy that call "name":X applies to a term of sort
// sort (sort empty: unknown), as the interpreter finds it when it evaluates
// a META strategy (Strategyname_to_ref_index, meta/meta.cc, fixed in commit
// 93ec493): the last strategy (in table order) named exactly
// name:sort/..., else the last named name:... of any sort. Its third step,
// when no strategy has the name: the last strategy whose name starts with
// name (in hash order) gives a sort and a module, but the name stays name,
// still undefined: the interpreter stops with `unknown strategy in call
// used` (status 1). The export (ref/ref.cc, the same lookup without the
// sort) then writes -1, refused by typeTerm; otherwise the index of a
// strategy named exactly name, looked up again here with the sort.
func (l *lowerer) resolveCall(name, sort string) (ref.StrategyNameIndex, bool) {
	entries := slices.Clone(l.p.StrategyNames)
	slices.SortFunc(entries, func(a, b ref.TableEntry) int { return cmp.Compare(a.Index, b.Index) })
	find := func(sort string) (ref.StrategyNameIndex, bool) {
		for i := len(entries) - 1; i >= 0; i-- {
			e := &entries[i]
			rest, ok := strings.CutPrefix(e.Name, name+":")
			if !ok || l.p.Strategy(e.Index) == nil {
				continue
			}
			if sort == "" || strings.HasPrefix(rest, sort+"/") {
				return e.Index, true
			}
		}
		return 0, false
	}
	if sort != "" {
		if s, ok := find(sort); ok {
			return s, true
		}
	}
	return find("")
}

// reachCall reaches the strategies of a call "name":X term (strateg is
// Strateg[X]). The term is applied to terms of sort X (the interpreter
// takes the sort of the head symbol of the term): a term of unknown sort
// only when X is builtinInt or builtinString (a number or a string has no
// symbol, NEW_META_APPLY in rewrite/stateofexecution.cc). The strategy
// found must be of sort X (the interpreter would apply a strategy of
// another sort to the term): refused; for a term of unknown sort, only if
// it happens, at run time, since a stuck term of sort builtinInt has a
// symbol.
func (l *lowerer) reachCall(name string, strateg ref.SortIndex, r *reach) error {
	key := callKey{name, strateg}
	if _, ok := r.calls[key]; ok {
		return nil
	}
	sname := l.sortName(strateg)
	x, ok := metaParam(sname, "Strateg")
	if !ok {
		return invalid("call %s of sort %s, not Strateg[X]", StringText(name), sname)
	}
	var xs optSort
	for _, s := range l.p.Sorts {
		if s.Name == x {
			xs = optSort{s.Index, true}
		}
	}
	if !xs.ok {
		return invalid("call %s:%s: no sort %s", StringText(name), x, x)
	}
	check := func(s ref.StrategyNameIndex, found bool) (ref.StrategyNameIndex, error) {
		if !found {
			return 0, invalid("call %s:%s: no strategy %s", StringText(name), x, name)
		}
		if d := l.p.Strategy(s); d.Sort != xs.s {
			return 0, unsupported(NotPlanned, "call %s:%s finds %s, a strategy of another sort (both oracles apply it to the term: ill-sorted results)",
				StringText(name), x, l.strategyName(s))
		}
		return s, nil
	}
	typed, err := check(l.resolveCall(name, x))
	if err != nil {
		return err
	}
	c := callTargets{typed: typed}
	if k, ok := builtinKind(x); ok && (k == KindInt || k == KindStr) {
		c.hasUntyped = true
		u, err := check(l.resolveCall(name, ""))
		if e, ok := err.(*Unsupported); ok {
			c.untypedWhy = e.What
		} else if err != nil {
			return err
		} else {
			c.untyped = u
			r.reachStrategy(u)
		}
	}
	r.reachStrategy(typed)
	r.calls[key] = c
	return nil
}

// metaParam is X for a sort base[X] (Strateg[X], Strategy[X]).
func metaParam(sort, base string) (string, bool) {
	rest, ok := strings.CutPrefix(sort, base+"[")
	if !ok || !strings.HasSuffix(rest, "]") {
		return "", false
	}
	return rest[:len(rest)-1], true
}

// metaOp is what META does with an operator of semantic code semantic.
func metaOp(semantic int32) MetaOp {
	switch semantic {
	case 128, -128:
		return MetaList
	case 129, -129:
		return MetaAll
	case 130, -130:
		return MetaNth
	}
	return MetaNone
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
	l     *lowerer
	reach *reach
	// query: how each grammar rule is built (QueryOp: by the operator of
	// a reached profile)
	query   []queryRule
	grammar []GrammarRule
	opOf    map[int]OpID
	sortOf  map[ref.SortIndex]SortID
	labelOf map[ref.RuleNameIndex]LabelID     // labels in table order
	stratOf map[ref.StrategyNameIndex]StratID // strategies in table order
	ir      *IR
	// usedFuns are the names of the functions (unique)
	usedFuns map[string]bool
}

// neverBuilt is the reason a labelled rule is not compiled.
const neverBuilt = "(never applies: no term of the program has an operator of its left-hand side)"

func (b *builder) build(name string, start *tt, q *ref.Query, sortIndices []ref.SortIndex) (*IR, error) {
	l := b.l
	b.opOf = map[int]OpID{}
	b.sortOf = map[ref.SortIndex]SortID{}
	b.labelOf = map[ref.RuleNameIndex]LabelID{}
	for i, x := range slices.Sorted(slices.Values(b.reach.labels)) {
		b.labelOf[x] = i
	}
	b.stratOf = map[ref.StrategyNameIndex]StratID{}
	for i, x := range slices.Sorted(slices.Values(b.reach.strategies)) {
		b.stratOf[x] = i
	}
	ir := &IR{Program: name}
	b.ir = ir
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
	b.usedFuns = usedFuns
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
			Meta:   metaOp(k.semantic),
			AC:     k.infos == 2,
		}
		if op.AC {
			op.ACFun = unique(usedFuns, op.Fun+"_ac")
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
		case isBuiltin(k.semantic):
			code, _ := builtinCode(k.semantic)
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
	// transparent injection of a builtin sort. A builtin operation
	// computes a value of a builtin sort from values: declared on sorts of
	// terms (`@ pl @ : (t t) t code 3`), it is a constructor (the
	// interpreter leaves the term as it is; REM adds the addresses); then
	// the families are computed again (one more constructor: a sort may no
	// longer be one), until none changes. The operations on terms
	// (replace, occurs, matching) take and give any sorts.
	var familyOf map[SortID]FamilyID
	for {
		familyOf = b.families(pre)
		demoted := false
		for i := range cs {
			if pre[i].kind != Builtin || !pre[i].builtin.NeedsValues() {
				continue
			}
			op := &ir.Ops[i]
			_, ok := familyOf[op.Result]
			for _, a := range op.Args {
				if _, isF := familyOf[a]; !isF {
					ok = false
				}
			}
			if !ok {
				pre[i] = preKind{kind: Constructor}
				demoted = true
			}
		}
		if !demoted {
			break
		}
	}
	for s, f := range familyOf {
		ir.Sorts[s].Family = f
	}
	for o := range ir.Ops {
		if op := &ir.Ops[o]; op.AC && ir.Sorts[op.Result].Family >= 0 {
			return nil, unsupported(NotPlanned, builtinAC, op.Decl)
		}
	}

	// and, or, not and the term equalities give a bool
	for i, c := range cs {
		if pre[i].kind != Builtin || pre[i].builtin.NeedsValues() || pre[i].builtin.OnTerms() {
			continue
		}
		if _, ok := familyOf[ir.Ops[i].Result]; !ok {
			return nil, unsupported(NotPlanned, "builtin operation %s on sorts that are not builtin (%s): its result is not a bool",
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
		ac := ir.Ops[i].AC
		for ti, tr := range trs {
			rl := &ruleLowerer{b: b, rule: tr.rule, varOf: map[uint32]VarID{}}
			// an AC operator: one column, the whole (canonical) term; the
			// rule is applied modulo an extension when the interpreter
			// exports one for it (the next rule, F(EVAR, lhs))
			rl.extTop = ac && ti+1 < len(trs) && containsEVar(&trs[ti+1].lhs)
			r, ok, err := rl.lower(&tr.lhs, ac)
			if err != nil {
				return nil, err
			}
			if ok {
				rules[i] = append(rules[i], r)
			} else {
				why := " (extension variable in the left-hand side: not compiled, as REM)"
				if ac {
					why = " (the extension of the rule before it: that rule is applied modulo an extension)"
				}
				skipped[i] = append(skipped[i], rl.render(&tr.lhs)+why)
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
				if ir.Ops[o].AC {
					// its rules all match modulo AC (conditions)
					next[o] = false
					continue
				}
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
			cols := op.Args
			if op.AC {
				cols = []SortID{op.Result}
			}
			tree, never := decisionTree(rules[o], cols, sig)
			op.Rules, op.Tree, op.Skipped = rules[o], tree, skipped[o]
			for _, r := range never {
				op.Skipped = append(op.Skipped, rules[o][r].Text+
					" (never applies: an operator of its left-hand side is always reduced)")
			}
		}
	}
	if err := b.strategies(ir, sig, usedFuns); err != nil {
		return nil, err
	}
	if err := b.metas(); err != nil {
		return nil, err
	}
	markRecursive(ir)

	// the query is the variable of the start term (`start with () s(query)`)
	usesQuery := containsVar(start)
	startExpr, err := b.expr(start, func(uint32) (VarID, bool) { return 0, usesQuery })
	if err != nil {
		return nil, err
	}
	ir.Start = Start{QuerySort: l.sortName(q.Sort), Sort: b.sortOf[l.sortOf(start)], Term: startExpr, Strategy: -1,
		UsesQuery: usesQuery}
	if usesQuery {
		qs, ok := b.sortOf[q.Sort]
		if !ok {
			return nil, invalid("the query's sort %s is not reached", l.sortName(q.Sort))
		}
		ir.Start.Vars = []Var{{Name: "query", Sort: qs}}
	}
	// the table of identifiers: the parser of the query reads them (and
	// the identifiers of its grammar rules are their indices)
	for _, e := range l.p.Identifiers {
		ir.Idents = append(ir.Idents, Ident{Index: int32(e.Index), Name: e.Name})
	}
	if qs, ok := b.sortOf[q.Sort]; ok || !slices.ContainsFunc(b.query, func(r queryRule) bool { return r.Kind == QueryOp }) {
		named := func(n string, def int32) int32 {
			for _, s := range l.p.Sorts {
				if s.Name == n {
					return int32(s.Index)
				}
			}
			return def
		}
		iq := &Query{Sort: int32(q.Sort), SortID: -1, Grammar: b.grammar, SortNames: map[int32]string{},
			InternIdent: named("intern ident", 220), InternInt: named("intern int", 19),
			InternString: named("intern string", 351)}
		if ok {
			iq.SortID = qs
		}
		for _, s := range l.p.Sorts {
			iq.SortNames[int32(s.Index)] = s.Name
		}
		for _, r := range b.query {
			qr := r.QueryRule
			if qr.Kind == QueryOp {
				qr.Op = b.opOf[r.cand]
			}
			iq.Rules = append(iq.Rules, qr)
		}
		ir.Start.Query = iq
	}
	if q.Strategy.Valid {
		id := b.stratOf[q.Strategy.Index]
		if st := &ir.Strategies[id]; st.Sort != ir.Start.Sort {
			return nil, invalid("start with (%s): the strategy is of sort %s, the term of sort %s",
				st.Name, ir.Sorts[st.Sort].Name, ir.Sorts[ir.Start.Sort].Name)
		}
		ir.Start.Strategy = id
	}
	return ir, nil
}

// families computes the builtin families (ir.Families) and the family of
// each of their sorts: the builtin sorts, and the sorts whose only
// constructor (pre) is a transparent injection of a builtin sort, marked
// Injection in pre (the marks of a previous computation undone first).
func (b *builder) families(pre []preKind) map[SortID]FamilyID {
	ir := b.ir
	builtinOf := func(s SortID) (BuiltinKind, bool) { return builtinKind(ir.Sorts[s].Name) }
	for o := range pre {
		if pre[o].kind == Injection {
			pre[o] = preKind{kind: Constructor}
		}
	}
	ir.Families = nil
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
	return familyOf
}

// strategies builds the labels and the strategies reached, in table order,
// and their determinism (greatest fixpoint).
func (b *builder) strategies(ir *IR, sig *signatures, usedFuns map[string]bool) error {
	l := b.l
	for _, li := range slices.Sorted(slices.Values(b.reach.labels)) {
		name := l.labelName(li)
		base := BaseName(name)
		rules := l.rulesByLabel[li]
		lb := Label{
			Name:     name,
			Fun:      unique(usedFuns, fmt.Sprintf("rl_%s_%d", OpBaseName([]string{base}), li)),
			Sort:     b.sortOf[rules[0].Sort],
			RulesDet: true,
		}
		ctx := "rule " + name
		for _, r := range rules {
			lhs, err := l.typeTop(r.Lhs, optSort{r.Sort, true}, ctx)
			if err != nil {
				return err
			}
			rl := &ruleLowerer{b: b, rule: r, varOf: map[uint32]VarID{}}
			if !l.lhsPossible(&lhs, b.reach) {
				lb.Skipped = append(lb.Skipped, "["+base+"] "+rl.render(&lhs)+" "+neverBuilt)
				continue
			}
			x, ok, err := rl.lower(&lhs, true)
			if err != nil {
				return err
			}
			if !ok {
				lb.Skipped = append(lb.Skipped, "["+base+"] "+rl.render(&lhs)+
					" (extension variable in the left-hand side: not compiled, as REM)")
				continue
			}
			x.Text = "[" + base + "] " + strings.TrimPrefix(x.Text, "[] ")
			lb.Rules = append(lb.Rules, x)
		}
		tree, never := decisionTree(lb.Rules, []SortID{lb.Sort}, sig)
		for _, r := range never {
			lb.Skipped = append(lb.Skipped, lb.Rules[r].Text+
				" (never applies: an operator of its left-hand side is always reduced)")
		}
		lb.Tree = tree
		ir.Labels = append(ir.Labels, lb)
	}
	for _, si := range slices.Sorted(slices.Values(b.reach.strategies)) {
		name := l.strategyName(si)
		d := l.p.Strategy(si)
		ir.Strategies = append(ir.Strategies, Strategy{
			Name: name,
			Fun:  unique(usedFuns, fmt.Sprintf("st_%s_%d", OpBaseName([]string{BaseName(name)}), si)),
			Sort: b.sortOf[d.Sort],
			Body: b.strategySeq(d.Body, d.Sort),
			Det:  true,
		})
	}
	// determinism: greatest fixpoint
	for changed := true; changed; {
		changed = false
		for i := range ir.Labels {
			lb := &ir.Labels[i]
			det := true
			for _, r := range lb.Rules {
				for i := range r.Conds {
					r.Conds[i].Walk(func(c *Cond) {
						if c.Kind == CondApply && !ir.Strategies[c.Strategy].Det {
							det = false
						}
						// an AC matching can give several solutions, and
						// so can the branches of a choose
						if c.Kind == CondAC || c.Kind == CondEker || c.Kind == CondChoose && len(c.Branches) > 1 {
							det = false
						}
					})
				}
			}
			if det != lb.RulesDet {
				lb.RulesDet, changed = det, true
			}
		}
		for i := range ir.Strategies {
			if det := ir.SeqDet(ir.Strategies[i].Body); det != ir.Strategies[i].Det {
				ir.Strategies[i].Det, changed = det, true
			}
		}
	}
	return nil
}

// strategySeq is a strategy body of the .ref model (checked during
// reachability); the labels that do not fit the sort are left out.
func (b *builder) strategySeq(seq ref.StrategySeq, sort ref.SortIndex) []Strat {
	labels := func(ls []ref.RuleNameIndex) []LabelID {
		var v []LabelID
		for _, x := range ls {
			if b.l.labelFits(x, sort) {
				v = append(v, b.labelOf[x])
			}
		}
		return v
	}
	var out []Strat
	for _, e := range seq {
		var s Strat
		switch e.Op {
		case ref.SId:
			s = Strat{Kind: StId}
		case ref.SFail:
			s = Strat{Kind: StFail}
		case ref.SDkRules, ref.SDcRules, ref.SOneRules:
			c := map[ref.StrategyOp]Choice{ref.SDkRules: Dk, ref.SDcRules: Dc, ref.SOneRules: One}[e.Op]
			s = Strat{Kind: StRules, Choice: c, Labels: labels(e.Rules)}
		case ref.SDkStrats, ref.SDcStrats, ref.SOneStrats:
			c := map[ref.StrategyOp]Choice{ref.SDkStrats: Dk, ref.SDcStrats: Dc, ref.SOneStrats: One}[e.Op]
			s = Strat{Kind: StStrats, Choice: c}
			for _, a := range e.Alts {
				s.Alts = append(s.Alts, b.strategySeq(a, sort))
			}
		case ref.SRepeat:
			s = Strat{Kind: StRepeat, Body: b.strategySeq(e.Body, sort)}
		case ref.SIterate:
			s = Strat{Kind: StIterate, Body: b.strategySeq(e.Body, sort)}
		case ref.STAll, ref.STOne, ref.STSome, ref.SRewrite:
			k := map[ref.StrategyOp]StratKind{ref.STAll: StTAll, ref.STOne: StTOne, ref.STSome: StTSome, ref.SRewrite: StRewrite}[e.Op]
			s = Strat{Kind: k, Body: b.strategySeq(e.Body, sort)}
		case ref.SNormIn:
			s = Strat{Kind: StNormIn, Labels: labels(e.Rules)}
		case ref.SNormOut:
			s = Strat{Kind: StNormOut, Labels: labels(e.Rules)}
		case ref.SCall:
			s = Strat{Kind: StCall, Call: b.stratOf[e.Call]}
		case ref.SMeta:
			s = Strat{Kind: StMeta, Meta: b.metaApply(b.sortOf[sort])}
		default:
			panic("refused during reachability")
		}
		out = append(out, s)
	}
	return out
}

// metaApply is the function of META on sort s (created when first used).
func (b *builder) metaApply(s SortID) int {
	for i, m := range b.ir.MetaApplies {
		if m.Sort == s {
			return i
		}
	}
	b.ir.MetaApplies = append(b.ir.MetaApplies, MetaApply{
		Fun:  unique(b.usedFuns, "meta_"+OpBaseName([]string{b.ir.Sorts[s].Name})),
		Sort: s,
	})
	return len(b.ir.MetaApplies) - 1
}

// metas builds the evaluators of the Strategy[X] terms of the reachable
// meta_apply operators, with the call "name":X terms reached.
func (b *builder) metas() error {
	p := b.ir
	byName := func(n string) SortID {
		return slices.IndexFunc(p.Sorts, func(s Sort) bool { return s.Name == n })
	}
	for o := range p.Ops {
		op := &p.Ops[o]
		if op.Meta == MetaNone || slices.ContainsFunc(p.MetaEvals, func(m MetaEval) bool { return m.Strategy == op.Args[0] }) {
			continue
		}
		sname := p.Sorts[op.Args[0]].Name
		x, ok := metaParam(sname, "Strategy")
		if !ok {
			return invalid("%s: a strategy of sort %s", op.Decl, sname)
		}
		term := byName(x)
		if term < 0 {
			return invalid("%s: sort %s not reached", op.Decl, x)
		}
		base := "ev_" + OpBaseName([]string{sname})
		ev := MetaEval{
			Fun:        unique(b.usedFuns, base),
			StrategFun: unique(b.usedFuns, base+"_step"),
			ListFun:    unique(b.usedFuns, base+"_list"),
			Strategy:   op.Args[0],
			Strateg:    byName("Strateg[" + x + "]"),
			Strategies: byName("Strategies[" + x + "]"),
			Term:       term,
		}
		if ev.Strateg >= 0 {
			for key, c := range b.reach.calls {
				if key.strateg != p.Sorts[ev.Strateg].Index {
					continue
				}
				mc := MetaCall{Name: key.name, Typed: b.stratOf[c.typed], Untyped: -1, UntypedWhy: c.untypedWhy}
				if c.hasUntyped && c.untypedWhy == "" {
					mc.Untyped = b.stratOf[c.untyped]
				}
				ev.Calls = append(ev.Calls, mc)
			}
			slices.SortFunc(ev.Calls, func(a, c MetaCall) int { return strings.Compare(a.Name, c.Name) })
		}
		if b.labelsByString(ev.Strateg) {
			ev.LabelsFun = unique(b.usedFuns, base+"_labels")
			for l := range p.Labels {
				if p.Labels[l].Sort != term {
					continue
				}
				name := InterpreterLabelName(p.Labels[l].Name)
				i := slices.IndexFunc(ev.Labels, func(m MetaLabel) bool { return m.Name == name })
				if i < 0 {
					ev.Labels = append(ev.Labels, MetaLabel{Name: name})
					i = len(ev.Labels) - 1
				}
				ev.Labels[i].Labels = append(ev.Labels[i].Labels, l)
			}
			slices.SortFunc(ev.Labels, func(a, c MetaLabel) int { return strings.Compare(a.Name, c.Name) })
		}
		p.MetaEvals = append(p.MetaEvals, ev)
	}
	return nil
}

// labelsByString tells whether a dc/dk over labels named by strings
// (Meta_strat codes 135, 136) of sort strateg (Strateg[X]) is reached.
func (b *builder) labelsByString(strateg SortID) bool {
	if strateg < 0 {
		return false
	}
	for _, op := range b.ir.Ops {
		// (the symbol of a Meta_strat[X] operator is its code)
		if (op.Symbol == 135 || op.Symbol == 136) && op.Result == strateg {
			return true
		}
	}
	return false
}

// InterpreterLabelName is the name the interpreter gives a label of the
// .ref file (label:sort/module!GL): label:sort, the name a string of a
// Labels[X] term must give (trrules.trruleindex, attach_type).
func InterpreterLabelName(refName string) string {
	colon := strings.Index(refName, ":")
	if colon < 0 {
		return refName
	}
	if slash := strings.Index(refName[colon:], "/"); slash >= 0 {
		return refName[:colon+slash]
	}
	return refName
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

// printItems is the printing form: the last grammar rule of the symbol
// with the operator's profile, as the interpreter prints (an alias
// declared after the operator, `eq_bool(@,@)` after `@ == @`); the rules
// of the symbol with other profiles are not used (eq_builtinInt, declared
// last, is not the form of == on bool; the C runtime of REM, keyed by the
// symbol only, would print it).
func (b *builder) printItems(k *cand) []PrintItem {
	ls := k.decl
	if f, ok := b.l.printForm[profileOf(k.symbol, k.args, k.result)]; ok {
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
	case ttIdent:
		return Expr{Kind: ExprIdent, Int: t.i}, nil
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
// sides; injections and booleans are inlined) and the strategies of its
// `where x := (S) t`; a strategy calls the labels and strategies of its
// body; a label calls what its rules call. Labels and strategies always
// guard their depth; an operator on a cycle through them guards it too.
func markRecursive(ir *IR) {
	nOps, nLabels := len(ir.Ops), len(ir.Labels)
	label := func(l LabelID) int { return nOps + l }
	strat := func(s StratID) int { return nOps + nLabels + s }
	n := nOps + nLabels + len(ir.Strategies)
	calls := make([][]int, n)
	var collect func(from int, e *Expr)
	collect = func(from int, e *Expr) {
		if e.Kind != ExprOp {
			return
		}
		if k := ir.Ops[e.Op].Kind; k != Injection && k != BoolConst {
			calls[from] = append(calls[from], e.Op)
		}
		for i := range e.Args {
			collect(from, &e.Args[i])
		}
	}
	rules := func(from int, rs []Rule) {
		for ri := range rs {
			r := &rs[ri]
			var all []*Cond
			for ci := range r.Conds {
				r.Conds[ci].Walk(func(c *Cond) { all = append(all, c) })
			}
			for _, c := range all {
				if c.Kind != CondSame && c.Kind != CondAC && c.Kind != CondEker && c.Kind != CondChoose {
					collect(from, &c.Expr)
				}
				if c.Kind == CondEker {
					// the values of its variables are normalised by the
					// functions of the AC operators of its pattern
					for _, o := range c.Eker.ACOps() {
						if ir.Ops[o].Kind == Defined {
							calls[from] = append(calls[from], o)
						}
					}
				}
				if c.Kind == CondApply {
					calls[from] = append(calls[from], strat(c.Strategy))
				}
				if c.Kind == CondAC && ir.Ops[c.Pat.Op].Kind == Defined {
					// the values of its variables and the extension are
					// normalised by the operator's function
					calls[from] = append(calls[from], c.Pat.Op)
				}
			}
			collect(from, &r.Rhs)
		}
	}
	for o := range ir.Ops {
		rules(o, ir.Ops[o].Rules)
		if op := &ir.Ops[o]; op.Kind == Builtin && op.Builtin == TermReplace {
			// the copied path is renormalised by the functions of the
			// operators that can be stuck (normTerm)
			for x := range ir.Ops {
				if ir.Ops[x].Variant && ir.Ops[x].Kind != Constructor {
					calls[o] = append(calls[o], x)
				}
			}
		}
	}
	for l := range ir.Labels {
		rules(label(l), ir.Labels[l].Rules)
	}
	var seq func(from int, v []Strat)
	seq = func(from int, v []Strat) {
		for i := range v {
			s := &v[i]
			for _, l := range s.Labels {
				calls[from] = append(calls[from], label(l))
			}
			for _, a := range s.Alts {
				seq(from, a)
			}
			seq(from, s.Body)
			if s.Kind.IsTraversal() {
				// the terms it rebuilds are renormalised by the
				// functions of the operators that can be stuck (normTerm)
				for x := range ir.Ops {
					if ir.Ops[x].Variant && ir.Ops[x].Kind != Constructor {
						calls[from] = append(calls[from], x)
					}
				}
			}
			switch s.Kind {
			case StCall:
				calls[from] = append(calls[from], strat(s.Call))
			case StMeta:
				// META applies the strategies of the call "name":X terms
				for _, ev := range ir.MetaEvals {
					for _, c := range ev.Calls {
						calls[from] = append(calls[from], strat(c.Typed))
						if c.Untyped >= 0 {
							calls[from] = append(calls[from], strat(c.Untyped))
						}
					}
				}
			}
		}
	}
	for i := range ir.Strategies {
		seq(strat(i), ir.Strategies[i].Body)
	}
	index := make([]int, n)
	low := make([]int, n)
	onStack := make([]bool, n)
	var stack []int
	next := 1
	var visit func(v int)
	visit = func(v int) {
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
			var comp []int
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
				if w < nOps {
					ir.Ops[w].Recursive = rec
				}
			}
		}
	}
	for v := range n {
		if index[v] == 0 {
			visit(v)
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
	// extTop: the left-hand side is that of an unlabelled rule of its AC
	// operator, applied modulo an extension (the top AC pattern gets Ext)
	extTop bool
	// acConds are the CondAC conditions of the AC patterns met by pattern
	// and not yet added to conds (outer patterns first)
	acConds []Cond
	// ekerConds are the CondEker conditions of the abstracted parts of
	// those patterns (acAbstract), added after them
	ekerConds []Cond
	// err is the first refusal met by pattern
	err error
	// shared are the variables bound by the branches of the enclosing
	// chooses already lowered: a branch binding the same model variable
	// gets the same variable ([CondChoose])
	shared map[uint32]VarID
}

// bindVar is the variable of the first binding of model variable i (in a
// pattern or a where): the one of another branch of an enclosing choose
// ([CondChoose]), else a new one.
func (rl *ruleLowerer) bindVar(i uint32, s ref.SortIndex) VarID {
	if v, ok := rl.shared[i]; ok {
		return v
	}
	return rl.newVar(i, s)
}

// freshVar is a new variable of the rule (the subject of an AC pattern).
func (rl *ruleLowerer) freshVar(prefix string, s SortID) VarID {
	name := prefix
	for n := 2; slices.ContainsFunc(rl.vars, func(v Var) bool { return v.Name == name }); n++ {
		name = fmt.Sprintf("%s_%d", prefix, n)
	}
	rl.vars = append(rl.vars, Var{Name: name, Sort: s})
	return len(rl.vars) - 1
}

// takeAC moves the pending CondAC conditions, then the CondEker ones, to
// the conditions.
func (rl *ruleLowerer) takeAC() {
	rl.conds = append(rl.conds, rl.acConds...)
	rl.conds = append(rl.conds, rl.ekerConds...)
	rl.acConds, rl.ekerConds = nil, nil
}

// addEker adds the CondEker of a where pattern (if any).
func (rl *ruleLowerer) addEker(ek *Cond) {
	if ek != nil {
		rl.conds = append(rl.conds, *ek)
	}
}

// acPattern is the AC pattern of t (an AC operator applied): its arguments
// flattened (the arguments with the same operator on top spliced), the
// variables among them grouped by name (x U x: one variable of
// multiplicity 2), the others element patterns; at a level of three items
// or more (equal items counted once) with two variables or more, or an
// element pattern repeated, the variables and repeated items are matched
// by Eker's matcher, as REM does (acAbstract).
func (rl *ruleLowerer) acPattern(t *tt, same *[]Cond, ext bool) Pattern {
	n := rl.remOf(t)
	p := Pattern{Kind: PatAC, Op: rl.b.opOf[t.cand], Ext: ext}
	nVars, repeated := 0, false
	for i, a := range n.args {
		if a.rank == 1 {
			nVars++
		} else if n.mults[i] > 1 {
			repeated = true
		}
	}
	if len(n.args) > 2 && (nVars >= 2 || repeated) {
		return rl.acAbstract(t, n, p, same)
	}
	var items []*tt
	var flatten func(x *tt)
	flatten = func(x *tt) {
		if x.kind == ttOp && x.cand == t.cand {
			for i := range x.args {
				flatten(&x.args[i])
			}
			return
		}
		items = append(items, x)
	}
	flatten(t)
	type mv struct {
		x    *tt
		mult int
	}
	var vars []mv
	for _, x := range items {
		if x.kind != ttVar {
			p.Args = append(p.Args, rl.pattern(x, same))
			continue
		}
		if i := slices.IndexFunc(vars, func(v mv) bool { return v.x.index == x.index }); i >= 0 {
			vars[i].mult++
		} else {
			vars = append(vars, mv{x, 1})
		}
	}
	// REM's list_x first (Term.java genDioMatching): the higher
	// multiplicity, else the smaller variable code; x^n U y^m with n, m > 1:
	// REM abstracts the smaller variable (transformation3_1), the other
	// one is list_x
	slices.SortStableFunc(vars, func(a, b mv) int {
		if a.mult > 1 && b.mult > 1 {
			return cmp.Compare(b.x.index, a.x.index)
		}
		return cmp.Or(cmp.Compare(b.mult, a.mult), cmp.Compare(a.x.index, b.x.index))
	})
	for _, v := range vars {
		pv := rl.pattern(v.x, same)
		p.Vars = append(p.Vars, ACVar{Var: pv.Var, Mult: v.mult})
	}
	return p
}

// acAbstract is the AC pattern of t (its level n in REM's form) whose
// variables and repeated items REM matches with Eker's matcher
// (RewriteRule.java transformation3_1, case 2): F(x1^a1, ..., t1, ...,
// s^b, ...) is matched as F(t1, ..., X) where F(x1^a1, ..., s^b, ...) :=()
// X, X a fresh variable; when every item would go, the first one in REM's
// order stays at the level (F(x, y, z): F(x, X) where F(y, z) :=() X).
func (rl *ruleLowerer) acAbstract(t *tt, n *remNode, p Pattern, same *[]Cond) Pattern {
	decl := rl.b.l.candDecl(&rl.b.l.cands[t.cand])
	var list []int
	for i, a := range n.args {
		if a.rank == 1 || n.mults[i] > 1 {
			list = append(list, i)
		}
	}
	keep := -1
	if len(list) == len(n.args) {
		keep, list = list[0], list[1:]
		if n.args[keep].rank != 1 {
			rl.refuse("an AC pattern whose items are all repeated, the first one an element pattern (%s: REM keeps it at the level for its bipartite matcher, not reproduced)", decl)
		}
	}
	abs := &remNode{t: t, rank: 2, code: n.code, ac: true}
	for _, i := range list {
		if rl.containsAC(n.args[i].t) {
			rl.refuse("an AC pattern nested in a repeated element of an AC pattern with several variables (%s: REM first abstracts the nested one into a where, RewriteRule.java transformation2, not reproduced)", decl)
		}
		abs.args = append(abs.args, n.args[i])
		abs.mults = append(abs.mults, n.mults[i])
	}
	// the element patterns left at the level (multiplicity 1), in source
	// order
	var flatten func(x *tt)
	flatten = func(x *tt) {
		if x.kind == ttOp && x.cand == t.cand {
			for i := range x.args {
				flatten(&x.args[i])
			}
			return
		}
		if x.kind != ttVar {
			r := rl.remOf(x)
			if i := slices.IndexFunc(n.args, func(a *remNode) bool { return remCmp(a, r) == 0 }); n.mults[i] == 1 {
				p.Args = append(p.Args, rl.pattern(x, same))
			}
		}
	}
	flatten(t)
	if keep >= 0 {
		pv := rl.pattern(n.args[keep].t, same)
		p.Vars = append(p.Vars, ACVar{Var: pv.Var, Mult: n.mults[keep]})
	}
	x := rl.freshVar("ab", rl.b.ir.Ops[p.Op].Result)
	c := rl.ekerCond(x, abs, same)
	p.Abs = &ACAbs{Var: x, EkerVars: c.EkerVars}
	rl.ekerConds = append(rl.ekerConds, c)
	return p
}

func (rl *ruleLowerer) refuse(format string, a ...any) {
	if rl.err == nil {
		rl.err = unsupported("S7e", format, a...)
	}
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
		var v VarID
		if bound {
			v = rl.newVar(t.index, t.sort)
			*same = append(*same, Cond{Kind: CondSame, A: first, B: v})
		} else {
			v = rl.bindVar(t.index, t.sort)
			rl.varOf[t.index] = v
		}
		return Pattern{Kind: PatVar, Var: v}
	case ttOp:
		if rl.b.l.cands[t.cand].infos == 2 {
			// an AC pattern: a fresh variable here, matched by a CondAC
			// (the outer patterns' conditions first)
			ext := rl.extTop
			rl.extTop = false
			op := rl.b.opOf[t.cand]
			v := rl.freshVar("ac", rl.b.ir.Ops[op].Result)
			slot := len(rl.acConds)
			rl.acConds = append(rl.acConds, Cond{Kind: CondAC, A: v})
			p := rl.acPattern(t, same, ext)
			rl.acConds[slot].Pat = p
			return Pattern{Kind: PatVar, Var: v}
		}
		rl.extTop = false
		p := Pattern{Kind: PatOp, Op: rl.b.opOf[t.cand]}
		for i := range t.args {
			p.Args = append(p.Args, rl.pattern(&t.args[i], same))
		}
		return p
	case ttInt:
		return Pattern{Kind: PatInt, Int: t.i}
	case ttStr:
		return Pattern{Kind: PatStr, Str: t.s}
	case ttIdent:
		return Pattern{Kind: PatIdent, Int: t.i}
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
// The patterns are those of the arguments of the left-hand side (a rule of
// an operator), or with whole the left-hand side itself (a labelled rule).
func (rl *ruleLowerer) lower(lhs *tt, whole bool) (Rule, bool, error) {
	if containsEVar(lhs) {
		return Rule{}, false, nil
	}
	l := rl.b.l
	var same []Cond
	var pats []Pattern
	if whole {
		pats = append(pats, rl.pattern(lhs, &same))
	} else {
		for i := range lhs.args {
			pats = append(pats, rl.pattern(&lhs.args[i], &same))
		}
	}
	rl.takeAC()
	rl.conds = append(rl.conds, same...)
	body := rl.rule.Body.(ref.Plain)
	var texts []string
	if err := rl.wheres(body.Wheres, &texts); err != nil {
		return Rule{}, false, err
	}
	rt, err := l.typeTop(body.Rhs, optSort{rl.rule.Sort, true}, "right-hand side")
	if err != nil {
		return Rule{}, false, err
	}
	rhs, err := rl.expr(&rt)
	if err != nil {
		return Rule{}, false, err
	}
	if rl.err != nil {
		return Rule{}, false, rl.err
	}
	text := "[] " + rl.render(lhs) + " => " + rl.render(&rt)
	for _, c := range texts {
		text += " " + c
	}
	return Rule{Text: text, Vars: rl.vars, Lhs: pats, Conds: rl.conds, Rhs: rhs}, true, nil
}

// wheres lowers the conditions ws of the rule, in order, appending them
// to rl.conds and their text (comments) to texts.
func (rl *ruleLowerer) wheres(ws []ref.Where, texts *[]string) error {
	l := rl.b.l
	for _, w := range ws {
		switch w := w.(type) {
		case ref.If:
			t, err := l.typeTop(w.Cond, l.boolSort, "condition")
			if err != nil {
				return err
			}
			*texts = append(*texts, "if "+rl.render(&t))
			e, err := rl.expr(&t)
			if err != nil {
				return err
			}
			rl.conds = append(rl.conds, Cond{Kind: CondIf, Expr: e})
		case ref.Assign:
			r, err := l.typeTop(w.Rhs, varSort(w.Lhs), "where")
			if err != nil {
				return err
			}
			lt, err := l.typeTop(w.Lhs, optSort{l.sortOf(&r), true}, "where")
			if err != nil {
				return err
			}
			e, err := rl.expr(&r)
			if err != nil {
				return err
			}
			*texts = append(*texts, "where "+rl.render(&lt)+" := ("+rl.strategyText(w.Strategy)+") "+rl.render(&r))
			if w.Strategy.Valid {
				var same []Cond
				p, ek := rl.wherePattern(&lt, &same)
				rl.conds = append(rl.conds, Cond{Kind: CondApply, Pat: p, Strategy: rl.b.stratOf[w.Strategy.Index], Expr: e})
				rl.addEker(ek)
				rl.conds = append(rl.conds, same...)
			} else if _, bound := rl.varOf[lt.index]; lt.kind == ttVar && !bound {
				v := rl.bindVar(lt.index, lt.sort)
				rl.varOf[lt.index] = v
				rl.conds = append(rl.conds, Cond{Kind: CondLet, A: v, Expr: e})
			} else {
				var same []Cond
				p, ek := rl.wherePattern(&lt, &same)
				rl.conds = append(rl.conds, Cond{Kind: CondMatch, Pat: p, Expr: e})
				rl.addEker(ek)
				rl.conds = append(rl.conds, same...)
			}
		case ref.Match:
			r, err := l.typeTop(w.Rhs, optSort{w.Sort, true}, "where")
			if err != nil {
				return err
			}
			pt, err := l.typeTop(w.Pattern, optSort{w.Sort, true}, "where")
			if err != nil {
				return err
			}
			e, err := rl.expr(&r)
			if err != nil {
				return err
			}
			*texts = append(*texts, "where "+rl.render(&pt)+" := ("+rl.strategyText(w.Strategy)+") "+rl.render(&r))
			var same []Cond
			p, ek := rl.wherePattern(&pt, &same)
			if w.Strategy.Valid {
				rl.conds = append(rl.conds, Cond{Kind: CondApply, Pat: p, Strategy: rl.b.stratOf[w.Strategy.Index], Expr: e})
			} else {
				rl.conds = append(rl.conds, Cond{Kind: CondMatch, Pat: p, Expr: e})
			}
			rl.addEker(ek)
			rl.conds = append(rl.conds, same...)
		case ref.Try:
			if err := rl.choose(w, texts); err != nil {
				return err
			}
		default:
			panic("unknown condition")
		}
	}
	return nil
}

// choose lowers `choose try b1 ... end` ([CondChoose]): each branch's
// conditions from the variables bound before it, a model variable bound by
// several branches being one variable; after end, the variables bound by
// every branch are bound.
func (rl *ruleLowerer) choose(w ref.Try, texts *[]string) error {
	before := maps.Clone(rl.varOf)
	outer := rl.shared
	shared := maps.Clone(outer)
	if shared == nil {
		shared = map[uint32]VarID{}
	}
	conds := rl.conds
	var branches [][]Cond
	var everywhere map[uint32]VarID
	var bt []string
	for _, br := range w.Branches {
		rl.varOf, rl.shared, rl.conds = maps.Clone(before), shared, nil
		var t []string
		if err := rl.wheres(br, &t); err != nil {
			return err
		}
		bt = append(bt, "try "+strings.Join(t, " "))
		branches = append(branches, rl.conds)
		bound := map[uint32]VarID{}
		for m, v := range rl.varOf {
			if _, ok := before[m]; !ok {
				shared[m] = v
				bound[m] = v
			}
		}
		if everywhere == nil {
			everywhere = bound
		} else {
			maps.DeleteFunc(everywhere, func(m uint32, _ VarID) bool { _, ok := bound[m]; return !ok })
		}
	}
	rl.varOf, rl.shared = before, outer
	maps.Copy(rl.varOf, everywhere)
	rl.conds = append(conds, Cond{Kind: CondChoose, Branches: branches})
	*texts = append(*texts, "choose "+strings.Join(bt, " ")+" end")
	return nil
}

// strategyText is the name of the strategy of a where (empty for ()).
func (rl *ruleLowerer) strategyText(s ref.OptIndex) string {
	if !s.Valid {
		return ""
	}
	return BaseName(rl.b.l.strategyName(s.Index))
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
	case ttIdent:
		if endsAlnum(out.String()) {
			out.WriteByte(' ')
		}
		out.WriteString(b.l.ident(uint32(t.i)))
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
				const ops = "+-*/=<>&|%!"
				if prev := out.String(); strings.IndexByte(ops, c) >= 0 && len(prev) >= 2 &&
					prev[len(prev)-1] == ' ' && strings.IndexByte(ops, prev[len(prev)-2]) >= 0 {
					// the next character of an operator (!=, ==, <=)
					out.Reset()
					out.WriteString(prev[:len(prev)-1])
					out.WriteByte(c)
					out.WriteByte(' ')
				} else if strings.IndexByte(ops, c) >= 0 {
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
