// Round trip of the golden exports tests/golden/expected/*.ref (elan
// --export) and *.cref (elan --cexport): 28 programs, 56 files. Port of
// src/compiler-rs/elan-ref/tests/goldens.rs.

package ref_test

import (
	"bytes"
	"os"
	"path/filepath"
	"reflect"
	"slices"
	"strings"
	"testing"

	. "elan/ref"
)

const goldenDir = "../../../tests/golden/expected"

func goldenFiles(t *testing.T) []string {
	t.Helper()
	entries, err := os.ReadDir(goldenDir)
	if err != nil {
		t.Fatal(err)
	}
	var v []string
	for _, e := range entries {
		if ext := filepath.Ext(e.Name()); ext == ".ref" || ext == ".cref" {
			v = append(v, filepath.Join(goldenDir, e.Name()))
		}
	}
	slices.Sort(v)
	return v
}

// stripComments is the file without its /* */ comments.
func stripComments(t *testing.T, src []byte) []byte {
	var out []byte
	for i := 0; i < len(src); {
		if bytes.HasPrefix(src[i:], []byte("/*")) {
			p := bytes.Index(src[i+2:], []byte("*/"))
			if p < 0 {
				t.Fatal("unterminated comment")
			}
			i += 2 + p + 2
		} else {
			out = append(out, src[i])
			i++
		}
	}
	return out
}

func load(t *testing.T, name string) *Program {
	t.Helper()
	src, err := os.ReadFile(filepath.Join(goldenDir, name))
	if err != nil {
		t.Fatal(err)
	}
	return must(Parse(src))
}

func TestAllGoldensRoundTrip(t *testing.T) {
	files := goldenFiles(t)
	if len(files) != 56 {
		t.Fatalf("%d files, want 56: 28 programs, a .ref and a .cref each", len(files))
	}
	var failures []string
	for _, f := range files {
		src, err := os.ReadFile(f)
		if err != nil {
			t.Fatal(err)
		}
		prog, err := Parse(src)
		if err != nil {
			failures = append(failures, f+": parse error "+err.Error())
			continue
		}
		out := Print(prog)
		// the contract: same tokens, white space and comments aside
		if d := FirstDifference(src, out); d != "" {
			failures = append(failures, f+": "+d)
			continue
		}
		// stronger, true of the current writer: same bytes once comments
		// are removed
		if !bytes.Equal(stripComments(t, src), out) {
			failures = append(failures, f+": layout differs from the writer's")
		}
		if again, err := Parse(out); err != nil || !reflect.DeepEqual(again, prog) {
			failures = append(failures, f+": reparse gives another model")
		}
	}
	if len(failures) > 0 {
		t.Fatal(strings.Join(failures, "\n"))
	}
}

// callIndex is the strategy index held by the first call (symbol 144) of t.
func callIndex(t Term) (int64, bool) {
	f, ok := t.(Fsym)
	if !ok {
		return 0, false
	}
	if f.Symbol == 144 {
		if len(f.Args) == 1 {
			if i, ok := f.Args[0].(Int); ok {
				return i.Value, true
			}
		}
		return 0, false
	}
	for _, a := range f.Args {
		if s, ok := callIndex(a); ok {
			return s, true
		}
	}
	return 0, false
}

// call "ab":int exports the index of ab in StrategyNames, although ab and ba
// (equal character sums, the same hash bucket) are in another order in the
// table of references: until 2026 the export gave the index of the
// reference, that of ba here (tests/golden/programs/call-anagram).
func TestCallAnagramModel(t *testing.T) {
	p := load(t, "call-anagram.cref")
	s, ok := callIndex(p.Query.StartWith)
	if !ok {
		t.Fatal("no call in the start term")
	}
	name, _ := p.StrategyName(uint32(s))
	eq(t, name, "ab:int/anagram")
}

func TestChoice1Model(t *testing.T) {
	p := load(t, "C40-choice1.cref")
	// tables
	str := func(s string, _ bool) string { return s }
	eq(t, str(p.Identifier(97)), "a")
	eq(t, str(p.Module(68)), "choice1")
	eq(t, str(p.RuleName(1555)), "r0:term/choice1!GL")
	eq(t, str(p.StrategyName(376)), "s0:term/choice1")
	// built-in sorts come from the GrammarForSort flags
	var builtin []string
	for _, s := range p.Sorts {
		if s.Builtin {
			builtin = append(builtin, s.Name)
		}
	}
	if !slices.Contains(builtin, "builtinInt") {
		t.Fatal(builtin)
	}
	if p.Sort(440).Builtin {
		t.Fatal("term is not built-in")
	}
	eq(t, p.Sort(440).Name, "term")
	// first grammar rule: 3:3:500:1:3:0:0:Type(58).Char(43).Type(58).nil:nil.
	g := p.Grammars.Both[0]
	eq(t, []any{g.Sort, g.Builtin}, []any{uint32(58), true})
	op := g.Operators[0]
	eq(t, []int32{op.Flag, op.Symbol, op.Priority, op.Semantic, op.Infos}, []int32{3, 3, 500, 3, 0})
	if !op.Syntax.LeftAssoc() || op.Syntax.Printable() {
		t.Fatal(op.Syntax)
	}
	eq(t, op.Defstrat, DefStrat{Kind: DSNone})
	eq(t, op.Rhs, []Lexem{{LexType, 58}, {LexChar, 43}, {LexType, 58}})
	// RULE(1555,440,68,1,1,2, VAR(0,440), VAR(1,440), WHERE(VAR(1,440),140, VAR(0,440)).nil)
	var r *Rule
	for i := range p.Rules {
		if p.Rules[i].Name == Some(1555) {
			r = &p.Rules[i]
			break
		}
	}
	if r == nil {
		t.Fatal("no rule 1555")
	}
	eq(t, []any{r.Sort, r.Module, r.Infos, r.WhichMatch, r.Nvars},
		[]any{uint32(440), int32(68), int32(1), int32(1), uint32(2)})
	eq(t, r.Lhs, Term(Var{Index: 0, Sort: 440}))
	body, ok := r.Body.(Plain)
	if !ok {
		t.Fatalf("%#v", r.Body)
	}
	eq(t, body.Rhs, Term(Var{Index: 1, Sort: 440}))
	eq(t, len(body.Wheres), 1)
	if len(p.Strategies) == 0 {
		t.Fatal("no strategies")
	}
	if _, ok := p.PrintingForm(318); !ok {
		t.Fatal("no printing form of 318")
	}
}
