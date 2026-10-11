package earley

import (
	"bytes"
	"errors"
	"strings"
	"testing"
)

// idents is a table of identifiers for the tests: REM's stringtab (sum of
// the bytes modulo 3000, probed by 211).
type idents struct {
	names map[int32]string
	index map[string]int32
}

func newIdents(names ...string) *idents {
	t := &idents{names: map[int32]string{}, index: map[string]int32{}}
	for _, n := range names {
		t.add(n)
	}
	return t
}

func (t *idents) add(name string) int32 {
	if i, ok := t.index[name]; ok {
		return i
	}
	sum := 0
	for i := 0; i < len(name); i++ {
		sum += int(int8(name[i]))
	}
	pos := int32(sum % 3000)
	for {
		if _, used := t.names[pos]; !used {
			break
		}
		pos = (pos + 211) % 3000
	}
	t.names[pos], t.index[name] = name, pos
	return pos
}

func (t *idents) tables(rules []Rule, sorts map[int32]string) *Tables {
	return &Tables{Rules: rules, IdentIndex: t.add, IdentName: func(i int32) string { return t.names[i] },
		SortName:    func(s int32) string { return sorts[s] },
		InternIdent: 220, InternInt: 19, InternString: 351}
}

// lexems reads the lexems of src up to the end of the input.
func lexems(t *testing.T, tab *Tables, src string) (out []string, warn string) {
	t.Helper()
	var stderr bytes.Buffer
	f := newLstream(newIchstream(strings.NewReader(src), "standard input"), tab, &stderr)
	for {
		var l Lexem
		f.ilex(&l)
		if l == NoLexem {
			return out, stderr.String()
		}
		out = append(out, f.erralfsy(l))
		if len(out) > 100 {
			t.Fatal("no end")
		}
	}
}

func TestLexems(t *testing.T) {
	id := newIdents("end")
	tab := id.tables(nil, nil)
	for _, c := range []struct{ src, want string }{
		{"f(x_1, y2) end", "f|(|x_1|,|y2|)|end"},
		{"2147483647 2147483648 4294967297", "2147483647|2147483648|1"},
		{`"a\"b\\c\nd\te" "x\qy"`, `"a\"b\\c\nd\te"|"x\\qy"`},
		{"a+-*(", "a|+|-|*|("},
		{"`a` `\\n`", "97|10"},
		{"a // comment\n b /* c\n */ c", "a|b|c"},
		{"0 x", `'\0'|x`},
		{"\\n", "\n"},
		{strings.Repeat("a", 60), strings.Repeat("a", 49)},
		{"é", "\xc3|\xa9"},
	} {
		got, _ := lexems(t, tab, c.src)
		if s := strings.Join(got, "|"); s != c.want {
			t.Errorf("%q: %q, want %q", c.src, s, c.want)
		}
	}
	if _, ok := id.index["x_1"]; !ok {
		t.Error("the identifiers read are added to the table")
	}
}

// TestLexErrors: the messages and positions of REM (examples/poly1
// compiled by elanc, the same inputs).
func TestLexErrors(t *testing.T) {
	tab := newIdents().tables(nil, nil)
	for _, c := range []struct{ src, want string }{
		{"a // no newline", "\n[error] :file=standard input\tline=0\tpos=16\n[lexan] comment throw end of input stream\n"},
		{"/* no end", "\n[error] :file=standard input\tline=0\tpos=10\n[lexan] comment throw end of input stream\n"},
		{`"no end`, "\nstring constant too long\n"},
		{"`ab`", "\nwrong string constant\n"},
	} {
		func() {
			defer func() {
				x := recover()
				var f *Fatal
				if e, ok := x.(error); !ok || !errors.As(e, &f) {
					t.Errorf("%q: %v, want a Fatal", c.src, x)
					return
				}
				if f.Msg != c.want || f.Status != 1 {
					t.Errorf("%q: %q (%d), want %q", c.src, f.Msg, f.Status, c.want)
				}
			}()
			lexems(t, tab, c.src)
		}()
	}
}
