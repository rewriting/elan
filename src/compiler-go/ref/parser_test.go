// Unit tests of the lexer, parser and printer on small inputs, including the
// constructs that no export of the bench contains (SWRULE, dccall, String
// lexems, traversal strategies, negative integers, strings, call terms).
// Port of src/compiler-rs/elan-ref/tests/parser.rs.

package ref_test

import (
	"errors"
	"fmt"
	"reflect"
	"strings"
	"testing"

	. "elan/ref"
)

type tok struct {
	kind TokKind
	val  any
}

func ti(n int64) tok  { return tok{TokInt, n} }
func ts(s string) tok { return tok{TokStr, s} }
func tw(s string) tok { return tok{TokWord, s} }
func tp(s string) tok { return tok{TokPunct, s} }
func plain(t Token) tok {
	if t.Kind == TokInt {
		return tok{t.Kind, t.Int}
	}
	return tok{t.Kind, t.Text}
}

func toks(t *testing.T, s string) []tok {
	t.Helper()
	v, err := Tokenize([]byte(s))
	if err != nil {
		t.Fatal(err)
	}
	var out []tok
	for _, x := range v {
		out = append(out, plain(x))
	}
	return out
}

func eq(t *testing.T, got, want any) {
	t.Helper()
	if !reflect.DeepEqual(got, want) {
		t.Fatalf("\ngot:  %#v\nwant: %#v", got, want)
	}
}

// must returns v, and panics (failing the test) on an error.
func must[T any](v T, err error) T {
	if err != nil {
		panic(err)
	}
	return v
}

func TestLexerSkipsWhiteSpaceAndComments(t *testing.T) {
	eq(t, toks(t, "/*1106 R:selection*/\nRULE(\n-1, // a line comment\n 3)"),
		[]tok{tw("RULE"), tp("("), ti(-1), tp(","), ti(3), tp(")")})
}

func TestLexerWordsNumbersNames(t *testing.T) {
	eq(t, toks(t, "repeat*( iterate*(x_1 ; 12:\"a b\".- 4"),
		[]tok{tw("repeat*"), tp("("), tw("iterate*"), tp("("), tw("x_1"), tp(";"),
			ti(12), tp(":"), ts("a b"), tp("."), tp("-"), ti(4)})
}

func TestLexerNamesAreVerbatim(t *testing.T) {
	// names are not escaped by the writer: a quote, a backslash, a newline
	eq(t, toks(t, "1:\"\"\".2:\"\\\".3:\"\n\"."),
		[]tok{ti(1), tp(":"), ts("\""), tp("."), ti(2), tp(":"), ts("\\"), tp("."),
			ti(3), tp(":"), ts("\n"), tp(".")})
	// Latin-1 bytes are kept as they are
	v := must(Tokenize([]byte("1:\"\xe9t\xe9\".")))
	eq(t, v[2].Text, "\xe9t\xe9")
}

func TestLexerErrorsHavePositions(t *testing.T) {
	_, err := Tokenize([]byte("RULE(\n  @"))
	var e *Error
	if !errors.As(err, &e) {
		t.Fatalf("no error: %v", err)
	}
	eq(t, [2]int{e.Line, e.Col}, [2]int{2, 3})
	if _, err := Tokenize([]byte("/* open")); err == nil {
		t.Fatal("unterminated comment accepted")
	}
	if _, err := Tokenize([]byte("1:\"open")); err == nil {
		t.Fatal("unterminated name accepted")
	}
}

func TestTerms(t *testing.T) {
	term := must(ParseTerm(
		"FSYM( VAR(0,440).INT(-3).IDENT(97).STRING(104.-23.nil).FSYM( nil, 300).nil, 301)"))
	eq(t, term, Term(Fsym{
		Args: []Term{
			Var{Index: 0, Sort: 440},
			Int{Value: -3},
			Ident{Index: 97},
			Str{Codes: []int32{104, -23}},
			Fsym{Symbol: 300},
		},
		Symbol: 301,
	}))
	// as the writer: a blank after FSYM(, then the blank that starts a VAR
	eq(t, PrintTerm(term),
		"FSYM(  VAR(0,440).INT(-3).IDENT(97).STRING(104.-23.nil).FSYM( nil, 300).nil, 301)")
	eq(t, must(ParseTerm(" EVAR(2,7)")), Term(EVar{Index: 2, Sort: 7}))
	eq(t, must(ParseTerm("STRING(nil)")), Term(Str{}))
}

func TestCallTermOfCexport(t *testing.T) {
	// --cexport writes a strategy call as FSYM(/**/INT(s).nil,144)
	eq(t, must(ParseTerm("FSYM(/**/INT(377).nil,144)")),
		Term(Fsym{Args: []Term{Int{Value: 377}}, Symbol: 144}))
	eq(t, PrintTerm(Fsym{Args: []Term{Int{Value: 377}}, Symbol: 144}), "FSYM(INT(377).nil,144)")
}

func TestBadTerms(t *testing.T) {
	for _, s := range []string{"FSYM( nil 300)", "VAR(-1,3)", "FOO(1)"} {
		if _, err := ParseTerm(s); err == nil {
			t.Errorf("%q accepted", s)
		}
	}
	_, err := ParseTerm("FSYM( VAR(0,1).\n nil, x)")
	var e *Error
	if !errors.As(err, &e) || e.Line != 2 {
		t.Fatalf("%v", err)
	}
}

func TestWheres(t *testing.T) {
	w := must(ParseWheres(
		"IFF(FSYM( nil, 300)).\nWHERE(VAR(1,440),-1, VAR(0,440)).\nPWHERE(FSYM( VAR(2,5).nil, 301),5,140, VAR(0,5)).\n" +
			"TRY(\nIFF(INT(1)).\nnil.nil.nil)\n.\nnil"))
	eq(t, w, []Where{
		If{Cond: Fsym{Symbol: 300}},
		Assign{Lhs: Var{Index: 1, Sort: 440}, Strategy: None, Rhs: Var{Index: 0, Sort: 440}},
		Match{
			Pattern:  Fsym{Args: []Term{Var{Index: 2, Sort: 5}}, Symbol: 301},
			Sort:     5,
			Strategy: Some(140),
			Rhs:      Var{Index: 0, Sort: 5},
		},
		Try{Branches: [][]Where{{If{Cond: Int{Value: 1}}}, nil}},
	})
	if _, err := ParseWheres("WHERE(VAR(1,440),-2, VAR(0,440)).nil"); err == nil {
		t.Fatal("-2 accepted as a strategy")
	}
}

func TestStrategies(t *testing.T) {
	s := must(ParseStrategy(
		"one(5.7.nil) ; dc(nil) ; dk(9) ; normin(1.nil) ; normout(2.nil) ; " +
			"ONE( id  , fail  ; META) ; DC(call(3)) ; DK(dk(4.nil)) ; repeat*(iterate*(tall(tone(tsome(rewrite( id )))))) ; " +
			"dccall(97.98.nil,4,440) ; dkcall(nil,0,1)"))
	id := StrategyExpr{Op: SId}
	un := func(op StrategyOp, b StrategyExpr) StrategyExpr {
		return StrategyExpr{Op: op, Body: StrategySeq{b}}
	}
	eq(t, s, StrategySeq{
		{Op: SOneRules, Rules: []uint32{5, 7}},
		{Op: SDcRules},
		{Op: SDkRules, Rules: []uint32{9}}, // REM's short form for a single rule
		{Op: SNormIn, Rules: []uint32{1}},
		{Op: SNormOut, Rules: []uint32{2}},
		{Op: SOneStrats, Alts: []StrategySeq{{id}, {{Op: SFail}, {Op: SMeta}}}},
		{Op: SDcStrats, Alts: []StrategySeq{{{Op: SCall, Call: 3}}}},
		{Op: SDkStrats, Alts: []StrategySeq{{{Op: SDkRules, Rules: []uint32{4}}}}},
		un(SRepeat, un(SIterate, un(STAll, un(STOne, un(STSome, un(SRewrite, id)))))),
		{Op: SDcCall, Name: []int32{97, 98}, Max: 4, Sort: 440},
		{Op: SDkCall, Max: 0, Sort: 1},
	})
	// printed as the writer does, the short form dk(9) becomes dk(9.nil)
	printed := PrintStrategy(s)
	if !strings.HasPrefix(printed, "one(5.7.nil) ; dc(nil) ; dk(9.nil) ; ") {
		t.Fatal(printed)
	}
	if !strings.Contains(printed, "ONE( id  ,  fail  ; META)") {
		t.Fatal(printed)
	}
	eq(t, must(ParseStrategy(printed)), s)
	for _, bad := range []string{"repeat(id)", "one()"} {
		if _, err := ParseStrategy(bad); err == nil {
			t.Errorf("%q accepted", bad)
		}
	}
}

func TestDefstratPacking(t *testing.T) {
	// FSYM_FLAG(300,301), LAB_FLAG(302,5), DSTR_FLAG(7,8), APPLY_FLAG(9)
	cases := []struct {
		w int64
		d DefStrat
	}{
		{0x100_0000 | 300<<12 | 301, DefStrat{Kind: DSFsym, Hi: 300, Lo: 301}},
		{0x200_0000 | 302<<12 | 5, DefStrat{Kind: DSLab, Hi: 302, Lo: 5}},
		{0x400_0000 | 7<<12 | 8, DefStrat{Kind: DSDstr, Hi: 7, Lo: 8}},
		{0x800_0000 | 9, DefStrat{Kind: DSApply, X: 9}},
		{0, DefStrat{Kind: DSNone}},
	}
	for _, c := range cases {
		eq(t, DecodeDefStrat(c.w), c.d)
		eq(t, c.d.Encode(), c.w)
	}
	// FSYM_FLAG(-1,5): the shifted -1 overwrites the kind bits, kept raw
	m := ^uint32(0)
	w := int64(int32(0x100_0000 | (m<<12 + 5)))
	eq(t, DecodeDefStrat(w), DefStrat{Kind: DSRaw, Word: w})
	eq(t, DecodeDefStrat(w).Encode(), w)
}

func TestSyntaxFlags(t *testing.T) {
	f := SyntaxFlags(8 + 2)
	if !(f.Printable() && f.RightAssoc() && !f.LeftAssoc() && !f.BuiltinStrategy()) {
		t.Fatal(f)
	}
	if !SyntaxFlags(4).BuiltinStrategy() {
		t.Fatal("4")
	}
}

// small is a small program with every section and the constructs of the
// interpreter's importer that REM does not read.
const small = `Identifiers
97:"a".
nil
end

Sorts
32:"ident".
440:"term".
nil
end

Modules
68:"m".
nil
end

RuleNames
1555:"r0:term/m!GL".
nil
end

StrategyNames
376:"s0:term/m".
nil
end

GrammarForSort 32:1:
3:5:0:8:0:0:0:Ident(97).nil:nil.
nil end
GrammarForSort 440:0:
3:300:0:8:0:0:16789805:Ident(97).Blank.Char(40).Num(1).String(2).Type(32).nil:1.2.nil.
3:301:10:9:-12:2:0:Type(440).Char(43).Type(440).nil:nil.
nil end
EndDef end
GrammarForSort 440:0:
nil end
EndDef end
EndDef end
/*1555 r0:term*/
RULE(
1555,440,68,1,0,1, VAR(0,440),
FSYM( nil, 300),
IFF(FSYM( nil, 5)).
nil)

 end

SWRULE(
-1,
440,68,2,1,2,FSYM( VAR(0,440).VAR(1,440).nil, 301),
SWITCH(WHERE(VAR(0,440),376, VAR(1,440)).
nil,FSYM( nil, 5),NOSWITCH(nil, VAR(0,440)).INT(1),SWITCH(nil,nil).nil))

 end
EndDef end
STRATEGY(376,440,68,dk(1555.nil) ; repeat*( id ))
end
EndDef end
QUERY(440,440,376,FSYM( nil, 300),FSYM( nil, 5)) end
`

func TestSmallProgram(t *testing.T) {
	p := must(Parse([]byte(small)))
	name, _ := p.Identifier(97)
	eq(t, name, "a")
	if !p.Sort(32).Builtin || p.Sort(440).Builtin {
		t.Fatal("builtin flags")
	}
	eq(t, len(p.Grammars.Both), 2)
	eq(t, len(p.Grammars.GlobalOnly), 1)
	eq(t, len(p.Grammars.GlobalOnly[0].Operators), 0)
	eq(t, len(p.Grammars.TopOnly), 0)
	op := p.Grammars.Both[1].Operators[0]
	eq(t, op.Defstrat, DefStrat{Kind: DSFsym, Hi: 3, Lo: 301})
	eq(t, op.Rhs, []Lexem{{LexIdent, 97}, {LexBlank, 0}, {LexChar, 40}, {LexNum, 1}, {LexString, 2}, {LexType, 32}})
	eq(t, op.LocalStrategies, []int32{1, 2})
	ac := p.Grammars.Both[1].Operators[1]
	eq(t, [2]int32{ac.Semantic, ac.Infos}, [2]int32{-12, 2})
	if !(ac.Syntax.LeftAssoc() && ac.Syntax.Printable()) {
		t.Fatal("syntax of 301")
	}
	pf, ok := p.PrintingForm(301)
	if !ok || pf.Grammar.Sort != 440 {
		t.Fatal("printing form of 301")
	}

	eq(t, len(p.Rules), 2)
	eq(t, p.Rules[0].Name, Some(1555))
	sw := p.Rules[1]
	eq(t, []any{sw.Name, sw.Infos, sw.WhichMatch, sw.Nvars}, []any{None, int32(2), int32(1), uint32(2)})
	cases, ok := sw.Body.(SwitchCases)
	if !ok {
		t.Fatalf("%#v", sw.Body)
	}
	eq(t, len(cases.Wheres), 1)
	eq(t, len(cases.Branches), 2)
	eq(t, cases.Branches[0].Body, Switch(NoSwitch{Result: Var{Index: 0, Sort: 440}}))
	eq(t, cases.Branches[1].Body, Switch(SwitchCases{}))
	eq(t, len(p.Strategy(376).Body), 2)
	eq(t, p.Query.Strategy, Some(376))
	eq(t, p.Query.CheckWith, Term(Fsym{Symbol: 5}))

	// printing: same tokens, same model
	out := Print(p)
	if d := FirstDifference([]byte(small), out); d != "" {
		t.Fatal(d)
	}
	eq(t, must(Parse(out)), p)
}

func TestRemAlternativesAreAccepted(t *testing.T) {
	// REM accepts `,` after a section, `.` after a block, a rule or a
	// strategy, and no `end` after the query; the printer writes the
	// exporter's form.
	alt := strings.Replace(small, "nil\nend\n\nSorts", "nil\n,\nSorts", 1)
	alt = strings.Replace(alt, "nil end\nGrammarForSort 440", "nil .\nGrammarForSort 440", 1)
	alt = strings.Replace(alt, "nil)\n\n end\n\nSWRULE", "nil) .\nSWRULE", 1)
	alt = strings.Replace(alt, ") \nend\nEndDef", ") .\nEndDef", 1)
	alt = strings.Replace(alt, ") end \n", ")\n", 1)
	if alt == small {
		t.Fatal("no replacement")
	}
	eq(t, must(Parse([]byte(alt))), must(Parse([]byte(small))))
}

func TestSyntaxErrors(t *testing.T) {
	missingNil := strings.Replace(small, "97:\"a\".\nnil", "97:\"a\".\n", 1)
	_, err := Parse([]byte(missingNil))
	var e *Error
	if !errors.As(err, &e) || e.Line != 4 {
		t.Fatalf("%v", err)
	}
	for i, bad := range []string{
		strings.Replace(small, "QUERY(", "QUERY ", 1),
		fmt.Sprintf("%s junk", small),
		strings.Replace(small, "EndDef end\nSTRATEGY", "STRATEGY", 1),
	} {
		if _, err := Parse([]byte(bad)); err == nil {
			t.Errorf("case %d accepted", i)
		}
	}
}
