package earley

import (
	"io"
	"strconv"
	"strings"
)

// Tables are the grammar of a program and its tables of names.
type Tables struct {
	// Rules are the grammar rules, in the order of REM's gram[] (the three
	// grammar dumps of the .ref file, in file order).
	Rules []Rule
	// IdentIndex gives the index of an identifier read, added to the table
	// of identifiers when it is new (tabofident.addstr); IdentName the
	// name of an index.
	IdentIndex func(name string) int32
	IdentName  func(i int32) string
	// SortName is the name of a sort (typet), for the warnings.
	SortName func(s int32) string
	// InternIdent, InternInt, InternString are the sorts of the standard
	// rules (REM's addstandards: 220, 19 and 351, the positions of their
	// names in the interpreter's table of sorts).
	InternIdent, InternInt, InternString int32
}

// Rule is a grammar rule: Sort ::= Rhs, building the symbol Symbol; Info
// is the priority word (priority + associativity bits * 4096).
type Rule struct {
	Sort, Symbol, Info int32
	Rhs                []RhsLexem
}

// Kinds of RhsLexem (REM's gram[] codes).
const (
	RhsIdent  = 0
	RhsSort   = 1
	RhsChar   = 2
	RhsNumber = 3
)

// RhsLexem is a lexem of a right-hand side: Kind RhsIdent (Code: the
// index of the identifier), RhsSort, RhsChar or RhsNumber.
type RhsLexem struct {
	Kind uint8
	Code int32
}

// Priority word (commondefs.h).
const (
	rnoprior     = 0
	rprioritymsk = 0o7777
	rleftassoc   = 0o10000
	rrightassoc  = 0o20000
	rassocmsk    = 0o70000
	standpri     = 4000
)

// Semantic actions of the standard rules and of the rules building
// numbers, identifiers and strings (term/codes.h).
const (
	numToTerm    = 67 // NUMTOTERM
	identToTerm  = 68 // IDENTTOTERM
	numRule      = 69 // NUMRULE
	identRule    = 70 // IDENTRULE
	stringToTerm = 81 // STRINGTOTERM
	stringRule   = 82 // STRINGRULE
)

// MaxLexems is MAXLENNTERM, the longest query (in lexems).
const MaxLexems = 50000

// grule is a rule of the parser (struct sgrammrule).
type grule struct {
	priority   int32
	rulenumber int32
	leftside   Lexem
	rside      []Lexem // ended by NoLexem
	index      int     // in Tables.Rules; -1 for a standard rule
	arity      int
	// action: rulenumber is a semantic action of the parser (esemact),
	// only for the standard rules and the `1:` rules of the .ref (s ::=
	// intern int code 67, intern ident 68, intern string 81); a user
	// operator with such a code is an operator like any other
	action bool
}

// sit is a situation [rule, pos, i] (struct sitlist).
type sit struct {
	rule *grule
	pos  int
	i    int
}

// psym are the situations of a set whose next symbol is symbol (struct
// psymlist), in the order they were added (REM's nextwss chain is the
// reverse).
type psym struct {
	symbol Lexem
	sits   []*sit
}

// ssits is a set of situations (struct ssits): the situations in the
// order they were added, and by next symbol in increasing order.
type ssits struct {
	actlex Lexem
	sits   []*sit
	syms   []*psym
}

// grammar is REM's class grammar with the state of a parse.
type grammar struct {
	nontt map[int32][]*grule // the rules of each nonterminal, in order
	t     *Tables
	f     *lstream

	sitset      []ssits
	sitseti     int
	startsymbol Lexem
	stderr      io.Writer
	tree        *Tree
}

// newGrammar is addstandards then grammarInit.
func newGrammar(t *Tables) *grammar {
	g := &grammar{nontt: map[int32][]*grule{}, t: t}
	g.addrul(TypeLex(t.InternIdent), []Lexem{IdentLexem}, standpri, identRule, -1).action = true
	g.addrul(TypeLex(t.InternInt), []Lexem{JustNumber}, standpri, numRule, -1).action = true
	g.addrul(TypeLex(t.InternString), []Lexem{StringLexem}, standpri, stringRule, -1).action = true
	intern := map[int32]int32{numToTerm: t.InternInt, identToTerm: t.InternIdent, stringToTerm: t.InternString}
	for i, r := range t.Rules {
		var rs []Lexem
		for _, x := range r.Rhs {
			switch x.Kind {
			case RhsIdent:
				rs = append(rs, IdentLex(x.Code))
			case RhsChar:
				rs = append(rs, CharLex(x.Code))
			case RhsSort:
				rs = append(rs, TypeLex(x.Code))
			case RhsNumber:
				rs = append(rs, NumLex(uint32(x.Code)))
			}
		}
		gr := g.addrul(TypeLex(r.Sort), rs, r.Info, r.Symbol, i)
		if s, ok := intern[r.Symbol]; ok && len(r.Rhs) == 1 && r.Rhs[0].Kind == RhsSort && r.Rhs[0].Code == s {
			gr.action = true
		}
	}
	return g
}

// addrul appends a rule to the rules of its left-hand side.
func (g *grammar) addrul(leftside Lexem, body []Lexem, priority, num int32, index int) *grule {
	r := &grule{priority: priority, rulenumber: num, leftside: leftside,
		rside: append(body, NoLexem), index: index}
	for _, l := range body {
		if l.nonterminal() {
			r.arity++
		}
	}
	n := leftside.typeval()
	g.nontt[n] = append(g.nontt[n], r)
	return r
}

// found is FOUND: the situations of set s whose next symbol is sy, nil if
// none.
func (s *ssits) found(sy Lexem) *psym {
	for _, p := range s.syms {
		if !(sy > p.symbol) {
			if p.symbol == sy {
				return p
			}
			return nil
		}
	}
	return nil
}

// inisitset initialises sitset[sitseti].
func (g *grammar) inisitset() {
	if g.sitseti == len(g.sitset) {
		g.sitset = append(g.sitset, ssits{})
	}
	g.sitset[g.sitseti] = ssits{actlex: NoLexem}
}

// addtosit appends the situation [(rule, position), i] to
// sitset[sitseti], unless it is there; true when added.
func (g *grammar) addtosit(rule *grule, position, i int) bool {
	set := &g.sitset[g.sitseti]
	sy := rule.rside[position]
	p := set.found(sy)
	if p != nil {
		for _, s := range p.sits {
			if s.rule == rule && s.pos == position && s.i == i {
				return false
			}
		}
	}
	s := &sit{rule: rule, pos: position, i: i}
	set.sits = append(set.sits, s)
	if p == nil {
		// PLACE: before the first symbol not smaller than sy
		k := 0
		for k < len(set.syms) && sy > set.syms[k].symbol {
			k++
		}
		p = &psym{symbol: sy}
		set.syms = append(set.syms, nil)
		copy(set.syms[k+1:], set.syms[k:])
		set.syms[k] = p
	}
	p.sits = append(p.sits, s)
	return true
}

// completesit completes the current set with predictions and
// completions.
func (g *grammar) completesit() {
	added := map[int32]bool{}
	set := &g.sitset[g.sitseti]
	for n := 0; n < len(set.sits); n++ {
		p := set.sits[n]
		l := p.rule.rside[p.pos]
		switch {
		case l.nonterminal():
			i := l.typeval()
			if !added[i] {
				added[i] = true
				for _, gr := range g.nontt[i] {
					g.addtosit(gr, 0, g.sitseti)
				}
			}
		case l.isBlank():
			g.addtosit(p.rule, p.pos+1, p.i)
		case l.isEndOfStream():
			if ps := g.sitset[p.i].found(p.rule.leftside); ps != nil {
				// (the nextwss chain: the last added first)
				for k := len(ps.sits) - 1; k >= 0; k-- {
					p2 := ps.sits[k]
					g.addtosit(p2.rule, p2.pos+1, p2.i)
				}
			}
		}
		set = &g.sitset[g.sitseti]
	}
}

// searchendsit is SEARCHENDSIT: the situations of set seti at the end of
// their rule that satisfy cond, in the order they were added.
func (g *grammar) searchendsit(seti int, cond func(*sit) bool) []*sit {
	var r []*sit
	if ps := g.sitset[seti].found(NoLexem); ps != nil {
		for _, s := range ps.sits {
			if cond(s) {
				r = append(r, s)
			}
		}
	}
	return r
}

// solveconflicts removes from sis the situations that another one, from
// the same position, makes useless: the same operator profile (an
// overloaded symbol declared twice), else the one of higher priority
// value when both have a priority.
func solveconflicts(sis []*sit) []*sit {
	i1 := 0
	for i1 < len(sis) {
		restart := false
		n1 := sis[i1].rule
		p1 := n1.priority & rprioritymsk
		i2 := i1 + 1
		for i2 < len(sis) {
			n2 := sis[i2].rule
			p2 := n2.priority & rprioritymsk
			if sis[i1].i == sis[i2].i {
				ok := false
				if n1.rulenumber == n2.rulenumber {
					// compare the argument sorts, then the result sort
					ok = true
					for j := 0; ok && n1.rside[j] != NoLexem && n2.rside[j] != NoLexem; j++ {
						if l1, l2 := n1.rside[j], n2.rside[j]; l1.nonterminal() && l2.nonterminal() {
							ok = l1.typeval() == l2.typeval()
						}
					}
					ok = ok && n1.leftside.typeval() == n2.leftside.typeval()
				}
				if ok {
					sis = append(sis[:i2], sis[i2+1:]...)
					continue
				}
				// Marian's version
				if p1 != rnoprior && p2 != rnoprior {
					if p1 < p2 {
						sis = append(sis[:i2], sis[i2+1:]...)
						continue
					}
					sis = append(sis[:i1], sis[i1+1:]...)
					restart = true // goto cont1
					break
				}
			}
			i2++
		}
		if !restart {
			i1++
		}
	}
	return sis
}

// solveconflict2 solves the associativity conflicts of the situations sis
// of an argument of rule r: when r is left associative, its own
// situations are removed, else (right associative) only its situation
// starting first is kept.
func solveconflict2(sis []*sit, r *grule) []*sit {
	rp := r.priority & rassocmsk
	if rp == 0 || len(sis) < 2 {
		return sis
	}
	var minis *sit
	for _, s := range sis {
		if s.rule.rulenumber == r.rulenumber && (minis == nil || s.i < minis.i) {
			minis = s
		}
	}
	if minis == nil {
		return sis
	}
	if rp == rleftassoc {
		var kept []*sit
		for _, s := range sis {
			if s.rule.rulenumber != r.rulenumber {
				kept = append(kept, s)
			}
		}
		return kept
	}
	return []*sit{minis}
}

func (g *grammar) firsttestcond(fins *sit) bool {
	return fins.i == 0 && fins.rule.leftside == g.startsymbol
}

// earleyaddnewsits adds the situations of the previous set that read lex;
// false if none. (A rule is not read past its end: REM does it at the end
// of the input, see the package comment.)
func (g *grammar) earleyaddnewsits(lex Lexem) bool {
	if lex == NoLexem {
		return false
	}
	ps := g.sitset[g.sitseti-1].found(lex)
	if ps == nil {
		return false
	}
	for k := len(ps.sits) - 1; k >= 0; k-- {
		s := ps.sits[k]
		g.addtosit(s.rule, s.pos+1, s.i)
	}
	return true
}

// isEnd is qendofin: the end of a query is `end` or the end of the input.
func (g *grammar) isEnd(l Lexem) bool {
	return l == IdentLex(g.t.IdentIndex("end")) || l.isEndOfStream()
}

// earley is the first pass: the sets of situations of the input up to
// the end of a term of sort starts; the start situations (solved), nil
// after a syntax error (reported).
func (g *grammar) earley(starts Lexem) []*sit {
	f := g.f
	var lex, le Lexem
	g.startsymbol = starts
	g.sitseti = 0
	g.inisitset()
	for _, r := range g.nontt[starts.typeval()] {
		g.addtosit(r, 0, 0)
	}
	g.completesit()
	f.fulex(&lex)
	for {
		for {
			g.sitseti++
			if g.sitseti >= MaxLexems {
				panic(&Fatal{Msg: "term is longer then MAXLENNTERM=" + strconv.Itoa(MaxLexems) +
					" lexems\n\tsorry, FATAL !!!\n", Status: 1})
			}
			g.inisitset()
			g.sitset[g.sitseti].actlex = lex
			sadded := g.earleyaddnewsits(lex)
			// identifiers, numbers and strings in general
			if lex.isRealID() {
				le = IdentLexem
				sadded = g.earleyaddnewsits(le) || sadded
			}
			if lex.isRealNum() {
				le = JustNumber
				sadded = g.earleyaddnewsits(le) || sadded
			}
			if lex.isRealString() {
				le = StringLexem
				sadded = g.earleyaddnewsits(le) || sadded
			}
			if !sadded {
				g.oearleyerr(g.sitseti - 1)
				return nil
			}
			g.completesit()
			f.ilex(&lex)
			f.fulex(&lex)
			if g.isEnd(lex) {
				break
			}
		}
		// the start situations; none: continue (the end was a lexem)
		chooselist := solveconflicts(g.searchendsit(g.sitseti, g.firsttestcond))
		if len(chooselist) > 0 {
			return chooselist
		}
	}
}

// testcond: the complete situation fins derives the k-th lexem xk of the
// situation acts.
func (g *grammar) testcond(fins, acts *sit, k int, xk Lexem) bool {
	if fins.rule.leftside != xk {
		return false
	}
	if ps := g.sitset[fins.i].found(xk); ps != nil {
		for k2 := len(ps.sits) - 1; k2 >= 0; k2-- {
			s := ps.sits[k2]
			if acts.rule.rulenumber == s.rule.rulenumber && s.pos == k && s.i == acts.i {
				return true
			}
		}
	}
	return false
}

// earleysecprec is the second pass on the situations si (the first one is
// chosen, a warning when there are others) ending at set j: the
// derivations of its arguments, right to left, then its semantic action
// (the derivation in reverse order).
func (g *grammar) earleysecprec(si []*sit, j int) {
	if len(si) > 1 {
		g.oambigwarning(si)
	}
	s0 := si[0]
	k, l := s0.pos-1, j
	for k >= 0 {
		xk := s0.rule.rside[k]
		if xk.terminal() {
			k--
			if !xk.isBlank() {
				l--
			}
			continue
		}
		ss := g.searchendsit(l, func(s *sit) bool { return g.testcond(s, s0, k, xk) })
		ss = solveconflicts(ss)
		ss = solveconflict2(ss, s0.rule)
		if len(ss) == 0 {
			panic(&Fatal{Msg: "[earleysecprec] int.err\n", Status: 1})
		}
		g.earleysecprec(ss, l)
		k--
		l = ss[0].i
	}
	g.esemact(s0.rule, g.sitset[j].actlex)
}

// oearleyerr writes the syntax error at the lexem after set sit.
func (g *grammar) oearleyerr(sit int) {
	f := g.f
	var lex Lexem
	var b strings.Builder
	f.ilex(&lex)
	b.WriteString(f.position("error"))
	b.WriteString("[earley] syntax error on symbol '" + f.erralfsy(lex) + "'")
	b.WriteString("\n\tone of (")
	first := true
	for _, ps := range g.sitset[sit].syms {
		if ps.symbol.terminal() && !ps.symbol.isEndOfStream() {
			if first {
				b.WriteString("'")
			} else {
				b.WriteString(" '")
			}
			b.WriteString(f.erralfsy(ps.symbol) + "'")
			first = false
		}
	}
	b.WriteString(") expected")
	b.WriteString("\n\tin context ")
	for i := max(1, sit-8); i <= sit; i++ {
		b.WriteString(f.erralfsy(g.sitset[i].actlex) + " ")
	}
	b.WriteString(" >>>" + f.erralfsy(lex) + "<<< ")
	f.fulex(&lex)
	b.WriteString(f.erralfsy(lex) + " ")
	for i := 0; i < 8 && !g.isEnd(lex); i++ {
		f.ilex(&lex)
		f.fulex(&lex)
		b.WriteString(f.erralfsy(lex) + " ")
	}
	b.WriteString("\n")
	io.WriteString(g.stderr, b.String())
}

// oambigwarning writes the ambiguity that the priorities and the
// associativity do not solve (the first situation is chosen).
func (g *grammar) oambigwarning(si []*sit) {
	var b strings.Builder
	b.WriteString("\n[warning]earley: ambiguity can't be solved in string:\n")
	for i := 1; i <= g.sitseti; i++ {
		b.WriteString(" " + g.f.alfsy(g.sitset[i].actlex))
	}
	b.WriteString("\n")
	b.WriteString("between operators :\n")
	for _, s := range si {
		b.WriteString("\t")
		g.writegrrule(&b, s.rule)
	}
	io.WriteString(g.stderr, b.String())
}

// writegrrule writes a rule as `'f' '(' @ ')' \t : (s1)s;`.
func (g *grammar) writegrrule(b *strings.Builder, gr *grule) {
	for _, ll := range gr.rside[:len(gr.rside)-1] {
		if ll.nonterminal() {
			b.WriteString(" @ ")
		} else {
			b.WriteString("'" + g.f.alfsy(ll) + "' ")
		}
	}
	b.WriteString("\t : ")
	wast := false
	for _, ll := range gr.rside[:len(gr.rside)-1] {
		if ll.nonterminal() {
			if wast {
				b.WriteString(" ")
			} else {
				b.WriteString("(")
			}
			b.WriteString(g.t.SortName(ll.typeval()))
			wast = true
		}
	}
	if wast {
		b.WriteString(")")
	}
	b.WriteString(g.t.SortName(gr.leftside.typeval()))
	if gr.rulenumber >= 0 {
		switch gr.priority & rassocmsk {
		case rleftassoc:
			b.WriteString("\t assocLeft ")
		case rrightassoc:
			b.WriteString("\t assocRight ")
		}
		b.WriteString("\t pri " + strconv.Itoa(int(gr.priority&rprioritymsk)))
		b.WriteString(" code " + strconv.Itoa(int(gr.rulenumber)))
	} else {
		b.WriteString("\t VAR(" + strconv.Itoa(int(-gr.rulenumber)) + ")")
		b.WriteString("\t (pri " + strconv.Itoa(int(gr.priority&rprioritymsk)) + ")")
	}
	b.WriteString(";\n")
}

// Parse reads a term of sort sort up to `end` (or the end of the input)
// from r, as REM's earleyCall: its derivation, or an error whose text
// (REM's message) has been written on stderr (*SyntaxError), or a *Fatal
// error (not written). The ambiguity warnings are written on stderr.
func Parse(r io.Reader, t *Tables, sort int32, stderr io.Writer) (tree *Tree, err error) {
	defer func() {
		if x := recover(); x != nil {
			f, ok := x.(*Fatal)
			if !ok {
				panic(x)
			}
			tree, err = nil, f
		}
	}()
	g := newGrammar(t)
	g.stderr = stderr
	g.f = newLstream(newIchstream(r, "standard input"), t, stderr)
	choose := g.earley(TypeLex(sort))
	if choose == nil {
		return nil, ErrSyntax
	}
	g.tree = &Tree{}
	g.earleysecprec(choose, g.sitseti)
	return g.tree, nil
}

// ErrSyntax is the error of a query that does not parse (the message is
// written).
var ErrSyntax = &syntaxError{}

type syntaxError struct{}

func (*syntaxError) Error() string { return "syntax error in the query" }
