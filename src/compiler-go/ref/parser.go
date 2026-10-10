// Recursive-descent parser of .ref files into the model.
//
// It accepts every construct of REM's grammar (REFParser.jj) and of the
// interpreter's importer (parse/grammars/aterm.orig), which adds SWRULE
// (SWITCH/NOSWITCH), String(n) lexems and dccall/dkcall. Where the grammars
// allow a choice that carries no information (a section ending with `end` or
// `,`, a block or rule ending with `end` or `.`, the final `end` after the
// query, `one(r)` for `one(r.nil)`), the model keeps nothing and the printer
// writes the form the exporter writes.

package ref

import (
	"fmt"
	"math"
)

// Parse parses a whole .ref file.
func Parse(src []byte) (*Program, error) {
	var prog *Program
	err := run(src, func(p *parser) { prog = p.program() })
	if err != nil {
		return nil, err
	}
	return prog, nil
}

// ParseTerm parses a single term (FSYM(...), VAR(...)...), for tests and
// tools.
func ParseTerm(src string) (Term, error) {
	var t Term
	err := run([]byte(src), func(p *parser) { t = p.term(); p.eof() })
	return t, err
}

// ParseStrategy parses a strategy body s1 ; s2 ; ..., for tests and tools.
func ParseStrategy(src string) (StrategySeq, error) {
	var s StrategySeq
	err := run([]byte(src), func(p *parser) { s = p.seq(); p.eof() })
	return s, err
}

// ParseWheres parses a list of conditions w1. w2. ... nil, for tests and
// tools.
func ParseWheres(src string) ([]Where, error) {
	var w []Where
	err := run([]byte(src), func(p *parser) { w = p.wheres(); p.eof() })
	return w, err
}

// parseError carries an *Error through panics inside the parser.
type parseError struct{ err *Error }

func run(src []byte, f func(*parser)) (err error) {
	toks, err := Tokenize(src)
	if err != nil {
		return err
	}
	defer func() {
		if r := recover(); r != nil {
			pe, ok := r.(parseError)
			if !ok {
				panic(r)
			}
			err = pe.err
		}
	}()
	f(&parser{toks: toks})
	return nil
}

type parser struct {
	toks []Token
	pos  int
}

func (p *parser) peek() (Token, bool) {
	return p.peekAt(0)
}

func (p *parser) peekAt(k int) (Token, bool) {
	if p.pos+k < len(p.toks) {
		return p.toks[p.pos+k], true
	}
	return Token{}, false
}

func (p *parser) fail(msg string) {
	line, col := 1, 1
	if p.pos < len(p.toks) {
		line, col = p.toks[p.pos].Line, p.toks[p.pos].Col
	} else if len(p.toks) > 0 {
		t := p.toks[len(p.toks)-1]
		line, col = t.Line, t.Col
	}
	found := "end of file"
	if t, ok := p.peek(); ok {
		found = "`" + t.String() + "`"
	}
	panic(parseError{&Error{Line: line, Col: col, Message: msg + ", found " + found}})
}

func (p *parser) isWord(w string) bool {
	t, ok := p.peek()
	return ok && t.Kind == TokWord && t.Text == w
}

func (p *parser) isPunct(c string) bool {
	t, ok := p.peek()
	return ok && t.Kind == TokPunct && t.Text == c
}

func (p *parser) word(w string) {
	if !p.isWord(w) {
		p.fail("expected `" + w + "`")
	}
	p.pos++
}

func (p *parser) punct(c string) {
	if !p.isPunct(c) {
		p.fail("expected `" + c + "`")
	}
	p.pos++
}

// open reads `KEYWORD (`: REM's tokens such as RULE( and one(.
func (p *parser) open(w string) {
	p.word(w)
	p.punct("(")
}

func (p *parser) eof() {
	if p.pos < len(p.toks) {
		p.fail("expected end of file")
	}
}

// int is REM's readInt: ["-"] INTEGER.
func (p *parser) int() int64 {
	if p.isPunct("-") {
		if t, ok := p.peekAt(1); ok && t.Kind == TokInt && t.Int >= 0 {
			p.pos += 2
			return -t.Int
		}
		p.fail("expected an integer")
	}
	t, ok := p.peek()
	if !ok || t.Kind != TokInt {
		p.fail("expected an integer")
	}
	p.pos++
	return t.Int
}

func (p *parser) i32() int32 {
	n := p.int()
	if n < math.MinInt32 || n > math.MaxInt32 {
		p.pos--
		p.fail(fmt.Sprintf("integer %d out of range", n))
	}
	return int32(n)
}

// index reads a non-negative index.
func (p *parser) index() uint32 {
	n := p.int()
	if n < 0 || n > math.MaxUint32 {
		p.pos--
		p.fail(fmt.Sprintf("expected a non-negative index, got %d", n))
	}
	return uint32(n)
}

// optIndex reads an optional index: -1 is None.
func (p *parser) optIndex() OptIndex {
	t, ok := p.peek()
	if p.isPunct("-") || (ok && t.Kind == TokInt && t.Int < 0) {
		n := p.int()
		if n == -1 {
			return None
		}
		p.pos--
		p.fail(fmt.Sprintf("expected an index or -1, got %d", n))
	}
	return Some(p.index())
}

// endOrComma reads `( end | , )` after a section.
func (p *parser) endOrComma() {
	if !p.isWord("end") && !p.isPunct(",") {
		p.fail("expected `end` or `,`")
	}
	p.pos++
}

// endOrDot reads `( end | . )` after a block, a rule or a strategy.
func (p *parser) endOrDot() {
	if !p.isWord("end") && !p.isPunct(".") {
		p.fail("expected `end` or `.`")
	}
	p.pos++
}

func (p *parser) nil() bool {
	if p.isWord("nil") {
		p.pos++
		return true
	}
	return false
}

// ------------------------------------------------------------ sections

func (p *parser) program() *Program {
	prog := &Program{}
	p.word("Identifiers")
	prog.Identifiers = p.table()
	p.endOrComma()
	p.word("Sorts")
	sortTable := p.table()
	p.endOrComma()
	p.word("Modules")
	prog.Modules = p.table()
	p.endOrComma()
	p.word("RuleNames")
	prog.RuleNames = p.table()
	p.endOrComma()
	p.word("StrategyNames")
	prog.StrategyNames = p.table()
	p.endOrComma()
	prog.Grammars.Both = p.grammarDump()
	p.endOrComma()
	prog.Grammars.GlobalOnly = p.grammarDump()
	p.endOrComma()
	prog.Grammars.TopOnly = p.grammarDump()
	p.endOrComma()
	for !p.isWord("EndDef") {
		prog.Rules = append(prog.Rules, p.rule())
	}
	p.word("EndDef")
	p.endOrComma()
	for !p.isWord("EndDef") {
		prog.Strategies = append(prog.Strategies, p.strategyDef())
	}
	p.word("EndDef")
	p.endOrComma()
	prog.Query = p.query()
	if p.isWord("end") {
		p.pos++
	}
	p.eof()
	for _, e := range sortTable {
		builtin := false
		for _, d := range prog.Grammars.Dumps() {
			for _, g := range d {
				if g.Sort == e.Index && g.Builtin {
					builtin = true
				}
			}
		}
		prog.Sorts = append(prog.Sorts, Sort{Index: e.Index, Name: e.Name, Builtin: builtin})
	}
	return prog
}

// table reads `( n : "name" . )* nil`.
func (p *parser) table() []TableEntry {
	var v []TableEntry
	for !p.nil() {
		index := p.index()
		p.punct(":")
		t, ok := p.peek()
		if !ok || t.Kind != TokStr {
			p.fail("expected a quoted name")
		}
		p.pos++
		p.punct(".")
		v = append(v, TableEntry{Index: index, Name: t.Text})
	}
	return v
}

// grammarDump reads `( GrammarForSort s : b : (op .)* nil (end|.) )* EndDef`.
func (p *parser) grammarDump() []SortGrammar {
	var v []SortGrammar
	for p.isWord("GrammarForSort") {
		p.pos++
		sort := p.index()
		p.punct(":")
		b := p.int()
		p.punct(":")
		var ops []Operator
		for !p.nil() {
			ops = append(ops, p.operator())
			p.punct(".")
		}
		p.endOrDot()
		v = append(v, SortGrammar{Sort: sort, Builtin: b == 1, Operators: ops})
	}
	p.word("EndDef")
	return v
}

func (p *parser) operator() Operator {
	var f [7]int64
	for k := range f {
		f[k] = p.int()
		p.punct(":")
	}
	var rhs []Lexem
	for !p.nil() {
		rhs = append(rhs, p.lexem())
		p.punct(".")
	}
	p.punct(":")
	var locals []int32
	for !p.nil() {
		locals = append(locals, p.i32())
		p.punct(".")
	}
	small := func(v int64) int32 {
		if v < math.MinInt32 || v > math.MaxInt32 {
			p.fail(fmt.Sprintf("integer %d out of range", v))
		}
		return int32(v)
	}
	return Operator{
		Flag:            small(f[0]),
		Symbol:          small(f[1]),
		Priority:        small(f[2]),
		Syntax:          SyntaxFlags(small(f[3])),
		Semantic:        small(f[4]),
		Infos:           small(f[5]),
		Defstrat:        DecodeDefStrat(f[6]),
		Rhs:             rhs,
		LocalStrategies: locals,
	}
}

func (p *parser) lexem() Lexem {
	if p.isWord("Blank") {
		p.pos++
		return Lexem{Kind: LexBlank}
	}
	t, ok := p.peek()
	if !ok || t.Kind != TokWord {
		p.fail("expected a lexem")
	}
	var kind LexemKind
	unsigned := false
	switch t.Text {
	case "Type":
		kind, unsigned = LexType, true
	case "Char":
		kind = LexChar
	case "Num":
		kind = LexNum
	case "String":
		kind = LexString
	case "Ident":
		kind, unsigned = LexIdent, true
	default:
		p.fail("expected a lexem")
	}
	p.open(t.Text)
	n := p.int()
	if (unsigned && (n < 0 || n > math.MaxUint32)) ||
		(!unsigned && (n < math.MinInt32 || n > math.MaxInt32)) {
		p.pos--
		p.fail(fmt.Sprintf("bad %s value %d", t.Text, n))
	}
	p.punct(")")
	return Lexem{Kind: kind, N: n}
}

// ------------------------------------------------------------ terms

func (p *parser) term() Term {
	t, ok := p.peek()
	if !ok || t.Kind != TokWord {
		p.fail("expected a term")
	}
	switch t.Text {
	case "FSYM":
		p.open("FSYM")
		var args []Term
		for !p.nil() {
			args = append(args, p.term())
			p.punct(".")
		}
		p.punct(",")
		symbol := p.i32()
		p.punct(")")
		return Fsym{Args: args, Symbol: symbol}
	case "VAR", "EVAR":
		p.open(t.Text)
		index := p.index()
		p.punct(",")
		sort := p.index()
		p.punct(")")
		if t.Text == "VAR" {
			return Var{Index: index, Sort: sort}
		}
		return EVar{Index: index, Sort: sort}
	case "INT":
		p.open("INT")
		n := p.int()
		p.punct(")")
		return Int{Value: n}
	case "IDENT":
		p.open("IDENT")
		n := p.index()
		p.punct(")")
		return Ident{Index: n}
	case "STRING":
		p.open("STRING")
		codes := p.codes()
		p.punct(")")
		return Str{Codes: codes}
	}
	p.fail("expected a term")
	return nil
}

// codes reads `( n . )* nil`: character codes.
func (p *parser) codes() []int32 {
	var v []int32
	for !p.nil() {
		v = append(v, p.i32())
		p.punct(".")
	}
	return v
}

// ------------------------------------------------------------ rules

func (p *parser) rule() Rule {
	var sw bool
	switch {
	case p.isWord("SWRULE"):
		sw = true
	case p.isWord("RULE"):
	default:
		p.fail("expected `RULE`, `SWRULE` or `EndDef`")
	}
	p.pos++
	p.punct("(")
	var r Rule
	r.Name = p.optIndex()
	p.punct(",")
	r.Sort = p.index()
	p.punct(",")
	r.Module = p.i32()
	p.punct(",")
	r.Infos = p.i32()
	p.punct(",")
	r.WhichMatch = p.i32()
	p.punct(",")
	r.Nvars = p.index()
	p.punct(",")
	r.Lhs = p.term()
	p.punct(",")
	if sw {
		r.Body = p.switch_()
	} else {
		rhs := p.term()
		p.punct(",")
		r.Body = Plain{Rhs: rhs, Wheres: p.wheres()}
	}
	p.punct(")")
	p.endOrDot()
	return r
}

// wheres reads `( where . )* nil`.
func (p *parser) wheres() []Where {
	var v []Where
	for !p.nil() {
		v = append(v, p.where())
		p.punct(".")
	}
	return v
}

func (p *parser) where() Where {
	switch {
	case p.isWord("IFF"):
		p.open("IFF")
		t := p.term()
		p.punct(")")
		return If{Cond: t}
	case p.isWord("WHERE"):
		p.open("WHERE")
		lhs := p.term()
		p.punct(",")
		s := p.optIndex()
		p.punct(",")
		rhs := p.term()
		p.punct(")")
		return Assign{Lhs: lhs, Strategy: s, Rhs: rhs}
	case p.isWord("PWHERE"):
		p.open("PWHERE")
		pat := p.term()
		p.punct(",")
		sort := p.index()
		p.punct(",")
		s := p.optIndex()
		p.punct(",")
		rhs := p.term()
		p.punct(")")
		return Match{Pattern: pat, Sort: sort, Strategy: s, Rhs: rhs}
	case p.isWord("TRY"):
		p.open("TRY")
		var branches [][]Where
		// REM: ( LOOKAHEAD(2) wheres . )* nil ) -- an empty branch is `nil .`
		for {
			next, ok := p.peekAt(1)
			if p.isWord("nil") && ok && next.Kind == TokPunct && next.Text == ")" {
				break
			}
			branches = append(branches, p.wheres())
			p.punct(".")
		}
		p.word("nil")
		p.punct(")")
		return Try{Branches: branches}
	}
	p.fail("expected `IFF`, `WHERE`, `PWHERE`, `TRY` or `nil`")
	return nil
}

func (p *parser) switch_() Switch {
	switch {
	case p.isWord("NOSWITCH"):
		p.open("NOSWITCH")
		w := p.wheres()
		p.punct(",")
		result := p.term()
		p.punct(")")
		return NoSwitch{Wheres: w, Result: result}
	case p.isWord("SWITCH"):
		p.open("SWITCH")
		w := p.wheres()
		p.punct(",")
		var branches []Branch
		for !p.nil() {
			test := p.term()
			p.punct(",")
			body := p.switch_()
			p.punct(".")
			branches = append(branches, Branch{Test: test, Body: body})
		}
		p.punct(")")
		return SwitchCases{Wheres: w, Branches: branches}
	}
	p.fail("expected `SWITCH` or `NOSWITCH`")
	return nil
}

// ------------------------------------------------------------ strategies

func (p *parser) strategyDef() StrategyDef {
	if !p.isWord("STRATEGY") {
		p.fail("expected `STRATEGY` or `EndDef`")
	}
	p.open("STRATEGY")
	var s StrategyDef
	s.Name = p.index()
	p.punct(",")
	s.Sort = p.index()
	p.punct(",")
	s.Module = p.i32()
	p.punct(",")
	s.Body = p.seq()
	p.punct(")")
	p.endOrDot()
	return s
}

// seq reads `expr ( ; expr )*`.
func (p *parser) seq() StrategySeq {
	v := StrategySeq{p.strat()}
	for p.isPunct(";") {
		p.pos++
		v = append(v, p.strat())
	}
	return v
}

// ruleList reads REM's RuleNames: `r [ . ( r . )* nil ]`, plus the empty
// list `nil` that the exporter writes when no rule of a label is visible.
func (p *parser) ruleList() []RuleNameIndex {
	var v []RuleNameIndex
	if p.nil() {
		return v
	}
	v = append(v, p.index())
	if p.isPunct(".") {
		p.pos++
		for !p.nil() {
			v = append(v, p.index())
			p.punct(".")
		}
	}
	return v
}

// seqList reads `body ( , body )*`.
func (p *parser) seqList() []StrategySeq {
	v := []StrategySeq{p.seq()}
	for p.isPunct(",") {
		p.pos++
		v = append(v, p.seq())
	}
	return v
}

var strategyOps = func() map[string]StrategyOp {
	m := map[string]StrategyOp{}
	for i, k := range strategyKeywords {
		m[k] = StrategyOp(i)
	}
	return m
}()

func (p *parser) strat() StrategyExpr {
	t, ok := p.peek()
	if !ok || t.Kind != TokWord {
		p.fail("expected a strategy")
	}
	op, known := strategyOps[t.Text]
	if !known {
		p.fail("expected a strategy")
	}
	if op == SId || op == SFail || op == SMeta {
		p.pos++
		return StrategyExpr{Op: op}
	}
	p.open(t.Text)
	s := StrategyExpr{Op: op}
	switch {
	case op.IsRules():
		s.Rules = p.ruleList()
	case op.IsStrats():
		s.Alts = p.seqList()
	case op.IsUnary():
		s.Body = p.seq()
	case op == SCall:
		s.Call = p.index()
	default: // dccall, dkcall
		s.Name = p.codes()
		p.punct(",")
		s.Max = p.i32()
		p.punct(",")
		s.Sort = p.index()
	}
	p.punct(")")
	return s
}

// ------------------------------------------------------------ query

func (p *parser) query() Query {
	p.open("QUERY")
	var q Query
	q.Sort = p.index()
	p.punct(",")
	q.ResultSort = p.index()
	p.punct(",")
	q.Strategy = p.optIndex()
	p.punct(",")
	q.StartWith = p.term()
	p.punct(",")
	q.CheckWith = p.term()
	p.punct(")")
	return q
}
