package elanrt

import (
	"strings"
	"testing"

	"elan/elanrt/earley"
)

// natQuery is the query parser of nat ::= o | s(nat): o 318, s 319.
func natQuery() *QueryParser {
	InitIdents(map[int32]string{111: "o", 115: "s", 220: "end"})
	return &QueryParser{
		Sort: 323, SortNames: map[int32]string{323: "nat"},
		InternIdent: 220, InternInt: 19, InternString: 351,
		Rules: []earley.Rule{
			{Sort: 323, Symbol: 318, Rhs: []earley.RhsLexem{{Kind: earley.RhsIdent, Code: 111}}},
			{Sort: 323, Symbol: 319, Rhs: []earley.RhsLexem{{Kind: earley.RhsIdent, Code: 115},
				{Kind: earley.RhsChar, Code: '('}, {Kind: earley.RhsSort, Code: 323}, {Kind: earley.RhsChar, Code: ')'}}},
		},
		Op: func(r int, a []Term) Term {
			if r == 0 {
				return Mk0(symO)
			}
			Step()
			return Mk1(symS, a[0])
		},
		IsAC: func(int) bool { return false },
	}
}

func init() {
	helpers["query"] = func() {
		SetQuery(natQuery())
		Main("Nat", true, func(s *Session, q Term) { s.Result(Mk1(symS, q)) })
	}
	helpers["queryignored"] = func() {
		SetQuery(natQuery())
		Main("Nat", false, func(s *Session, _ Term) { s.Result(nat(0)) })
	}
}

// A query read from the standard input (REM's termParser, query mode 2):
// the prompt (also with -quiet), the term parsed up to `end`, a newline,
// then the program with the query; a syntax error: REM's message on
// stderr, status 1; the options that choose another sort or strategy
// are not supported (status 2).
func TestMainQuery(t *testing.T) {
	prompt := "Enter a query term of sort 'Nat'\n"
	out, errs, st := runHelperIn(t, "query", nil, "s(s(o)) end\n", "-quiet")
	if st != 0 || errs != "" || out != prompt+"\n\nresult = s(s(s(o)))\n\nrewrite_step = 2\n" {
		t.Fatalf("status %d, out %q, err %q", st, out, errs)
	}
	// the query is read (and normalised) even when the start term does
	// not use it
	out, errs, st = runHelperIn(t, "queryignored", nil, "s(o)", "-quiet")
	if st != 0 || errs != "" || out != prompt+"\n\nresult = o\n\nrewrite_step = 1\n" {
		t.Fatalf("status %d, out %q, err %q", st, out, errs)
	}
	out, errs, st = runHelperIn(t, "query", nil, "s(o end\n", "-quiet")
	want := "\n[error] :file=standard input\tline=1\tpos=1\n[earley] syntax error on symbol 'end'\n" +
		"\tone of (')') expected\n\tin context s ( o  >>>end<<< ENDOFLSTREAM \n"
	if st != 1 || out != prompt || errs != want {
		t.Fatalf("status %d, out %q, err %q", st, out, errs)
	}
	// empty input: the error, no hang
	out, errs, st = runHelperIn(t, "query", nil, "", "-quiet")
	if st != 1 || out != prompt || !strings.Contains(errs, "syntax error on symbol 'ENDOFLSTREAM'") {
		t.Fatalf("status %d, out %q, err %q", st, out, errs)
	}
	// a lexical error: status 1
	out, errs, st = runHelperIn(t, "query", nil, "s(o) /* end", "-quiet")
	if st != 1 || out != prompt || !strings.Contains(errs, "comment throw end of input stream") {
		t.Fatalf("status %d, out %q, err %q", st, out, errs)
	}
	for _, args := range [][]string{{"-sort=", "nat"}, {"-strategy=", "s:nat"}, {"-REFInput"}, {"-commandLine"}} {
		out, errs, st = runHelperIn(t, "query", nil, "o end", args...)
		if st != 2 || !strings.Contains(errs, "not supported") {
			t.Fatalf("%v: status %d, out %q, err %q", args, st, out, errs)
		}
	}
	// -noInput: the query is not read
	out, _, st = runHelperIn(t, "queryignored", nil, "", "-noInput", "-quiet")
	if st != 0 || out != "\nresult = o\n\nrewrite_step = 0\n" {
		t.Fatalf("status %d, out %q", st, out)
	}
}
