package tables

import (
	"bytes"
	"os"
	"path/filepath"
	"strings"
	"testing"

	"elan/front/diag"
	"elan/ref"
)

func newD() (*diag.D, *bytes.Buffer) {
	var err bytes.Buffer
	return &diag.D{Out: &bytes.Buffer{}, Err: &err}, &err
}

// tests/unit/test_base_lex.cc, same values.
func TestHashIsCharSumModuloSize(t *testing.T) {
	if h := Hash("ab", 100); h != 95 {
		t.Fatalf("Hash(ab) = %d", h)
	}
	d, _ := newD()
	st := NewStringTab(d, 100)
	if p := st.AddStr("ab"); p != 95 {
		t.Fatalf("ab at %d", p)
	}
	if p := st.AddStr("ba"); p != 6 { // same sum: next probe (95+211)%100
		t.Fatalf("ba at %d", p)
	}
}

// Quirk 5 (finding 4): -fsigned-char, the hash sums signed chars (C %).
func TestHashSignedChars(t *testing.T) {
	// 'a' + (char)0xE9 = 97 - 23 = 74
	if h := Hash("a\xe9", 3000); h != 74 {
		t.Fatalf("Hash = %d", h)
	}
	// a sum of exactly -3000 is 0 in C (no out-of-bounds index)
	s := strings.Repeat("\x88", 25) // 25 * -120 = -3000
	if h := Hash(s, 3000); h != 0 {
		t.Fatalf("Hash = %d", h)
	}
	defer func() {
		if r := recover(); r == nil || !strings.Contains(r.(string), `"\xe9"`) {
			t.Fatalf("recovered %v", r)
		}
	}()
	Hash("\xe9", 3000) // -23: the C++ indexes out of bounds
}

func TestStringTab(t *testing.T) {
	d, _ := newD()
	st := NewStringTab(d, 100, "x", "y")
	i := st.AddStr("foo")
	if st.AddStr("foo") != i || !st.Member("foo") || st.Posid != i || st.Index("foo") != i {
		t.Fatal("add twice / member / index")
	}
	if st.Ide(i) != "foo" || st.Len() != 3 {
		t.Fatalf("Ide %q Len %d", st.Ide(i), st.Len())
	}
	if st.Member("nope") || st.Index("nope") != -1 {
		t.Fatal("nope is a member")
	}
	st.RemoveStr(i)
	if st.Member("foo") || st.Len() != 2 {
		t.Fatal("RemoveStr")
	}
	var got []string
	st.Each(func(i int, s string) { got = append(got, s) })
	if strings.Join(got, ",") != "x,y" { // positions 20 ('x'=120), 21
		t.Fatalf("Each %v", got)
	}
}

func TestStringTabOverflow(t *testing.T) {
	d, errb := newD()
	st := NewStringTab(d, 4)
	st.AddStr("a")
	st.AddStr("b")
	st.AddStr("c")
	if st.Len() != 3 {
		t.Fatal("3 names")
	}
	status := diag.Run(func() { st.AddStr("d") })
	if status != 1 || errb.String() != "[addstr] ERROR, string tab overflowed over 4!!! \t !!!!!!!!!\n" {
		t.Fatalf("status %d stderr %q", status, errb.String())
	}
}

func TestMiTabCounts(t *testing.T) {
	d, errb := newD()
	m := NewMiTab(d, 50, 7, 8, 0, 9) // the C varargs stop at the first value <= 0
	if !m.Member(7) || !m.Member(8) || m.Member(9) {
		t.Fatal("init")
	}
	m.AddN(5)
	m.AddN(5)
	m.RemoveN(5)
	if !m.Member(5) {
		t.Fatal("5 removed after one RemoveN of two AddN")
	}
	m.RemoveN(5)
	if m.Member(5) {
		t.Fatal("5 still a member")
	}
	if st := diag.Run(func() { m.RemoveN(5) }); st != 1 ||
		errb.String() != "[removen] trying to remove non ex. number\nint.err.\n" {
		t.Fatalf("status %d stderr %q", st, errb.String())
	}
}

func TestMiTabOverflowAndPosid(t *testing.T) {
	d, errb := newD()
	m := NewMiTab(d, 3)
	if p := m.AddN(4); p != 1 || m.Posid != 1 {
		t.Fatalf("4 at %d", p)
	}
	if p := m.AddN(7); p != (1+211)%3 { // 7%3 = 1 taken: probe
		t.Fatalf("7 at %d", p)
	}
	if st := diag.Run(func() { m.AddN(9) }); st != 1 ||
		errb.String() != "[addn] ERROR, mi tab overflowed over 3\n int.err.\n" {
		t.Fatalf("status %d stderr %q", st, errb.String())
	}
}

func TestMiTabNoInt(t *testing.T) {
	d, errb := newD()
	m := NewMiTab(d, 10)
	if st := diag.Run(func() { m.AddN(NoInt) }); st != 1 ||
		errb.String() != "[addn] ERROR trying to add NOINT value into mitab\n int. err.\n" {
		t.Fatalf("status %d stderr %q", st, errb.String())
	}
}

func TestInitialTables(t *testing.T) {
	d, _ := newD()
	typet := NewTypeT(d)
	sorts := []string{"bool", "ident", "builtinInt", "builtinString", "intern string", "intern ident", "intern int"}
	if typet.Len() != 7 {
		t.Fatalf("typet Len %d", typet.Len())
	}
	for _, s := range sorts {
		if typet.Index(s) != Hash(s, NTypes) {
			t.Fatalf("%s at %d, hash %d", s, typet.Index(s), Hash(s, NTypes))
		}
	}
	if imp := NewImport(d); imp.Len() != 0 {
		t.Fatal("import not empty")
	}
	bm := NewBuiltinModules(d)
	if bm.Len() != 33 || !bm.Member("builtinEqMolecule") || !bm.Member("Query") || bm.Member("int") {
		t.Fatalf("builtinmodules Len %d", bm.Len())
	}
	if MaxNofIdent != 3000 || NTypes != 500 || MaxNofImports != 200 || MaxNofTrn != 2000 ||
		MaxNofStrat != 500 || MaxNofMac != 50 || MaxNofVar != 49 || MaxDeepConstExp != 10 ||
		MRWTSize != 300 || IDLen != 50 || StrLen != 1000 || MaxNofString != 10000 {
		t.Fatal("limits")
	}
}

// Every entry of the five tables of the golden .ref files is at its hash
// position or reachable from it by +211 steps through occupied entries.
func TestGoldenTablePositions(t *testing.T) {
	files, _ := filepath.Glob("../../../../tests/golden/expected/*.ref")
	if len(files) == 0 {
		t.Fatal("no golden .ref")
	}
	type tab struct {
		name string
		size int
		e    []ref.TableEntry
	}
	n := 0
	for _, f := range files {
		src, err := os.ReadFile(f)
		if err != nil {
			t.Fatal(err)
		}
		p, err := ref.Parse(src)
		if err != nil {
			t.Fatalf("%s: %v", f, err)
		}
		var sorts []ref.TableEntry
		for _, s := range p.Sorts {
			sorts = append(sorts, ref.TableEntry{Index: s.Index, Name: s.Name})
		}
		for _, tb := range []tab{{"identifiers", MaxNofIdent, p.Identifiers}, {"sorts", NTypes, sorts},
			{"modules", MaxNofImports, p.Modules}, {"rule names", MaxNofTrn, p.RuleNames},
			{"strategy names", MaxNofStrat, p.StrategyNames}} {
			used := map[int]bool{}
			for _, e := range tb.e {
				used[int(e.Index)] = true
			}
			for _, e := range tb.e {
				pos, steps := Hash(e.Name, tb.size), 0
				for pos != int(e.Index) {
					if !used[pos] || steps > tb.size {
						t.Fatalf("%s: %s %q at %d, not reachable from %d", filepath.Base(f), tb.name, e.Name, e.Index, Hash(e.Name, tb.size))
					}
					pos, steps = (pos+211)%tb.size, steps+1
				}
				n++
			}
		}
	}
	t.Logf("%d entries checked in %d files", n, len(files))
}
