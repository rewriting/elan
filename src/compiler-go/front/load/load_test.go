package load

import (
	"bytes"
	"fmt"
	"os"
	"path/filepath"
	"reflect"
	"regexp"
	"strings"
	"testing"

	"elan/front/macro"
)

// The expected outputs of testdata/{diamond,params,cycle,spec} are the C++'s:
// tree.expected = the `handling module`/`end of` lines of
// `elan --export /dev/null ARGS` (no -b), trace.expected =
// `elan-lextrace load trace.expected -b ARGS` with the positions of id/str
// lines removed, both run in the program's directory with ELANLIB set to an
// installed ELAN (the programs carry their own bool.eln, so the library is
// not read). stratlgi's tree is written by hand (its strat.eln is the fake
// library's, testdata/lib).

const corpus = "../../../../tests/front/errors/load"

// copyDir copies the files of src into dst.
func copyDir(t *testing.T, src, dst string) {
	t.Helper()
	if err := os.MkdirAll(dst, 0o755); err != nil {
		t.Fatal(err)
	}
	ents, err := os.ReadDir(src)
	if err != nil {
		t.Fatal(err)
	}
	for _, e := range ents {
		if e.IsDir() {
			continue
		}
		b, err := os.ReadFile(filepath.Join(src, e.Name()))
		if err != nil {
			t.Fatal(err)
		}
		if err := os.WriteFile(filepath.Join(dst, e.Name()), b, 0o644); err != nil {
			t.Fatal(err)
		}
	}
}

// run loads the program of dir (args from dir/args) in a private copy
// whose parent is empty, with elanlib as the library, the Go tree printed
// unless -b; it returns stdout, stderr, the trace and the status.
func run(t *testing.T, dir, elanlib string, tree bool) (string, string, string, int) {
	t.Helper()
	abs, _ := filepath.Abs(dir)
	w := filepath.Join(t.TempDir(), filepath.Base(dir))
	copyDir(t, abs, w)
	t.Chdir(w)
	t.Setenv("SECONDELANLIB", "")
	b, err := os.ReadFile("args")
	if err != nil {
		t.Fatal(err)
	}
	var out, errb, tr bytes.Buffer
	o, files, err := ParseArgs(strings.Fields(string(b)))
	if err != nil {
		t.Fatal(err)
	}
	o.ElanLib = elanlib
	o.Out, o.Err = &out, &errb
	o.Eval = macro.Mark{}
	o.TraceTo = &tr
	if tree {
		o.Batch = false
	}
	spec := ""
	if len(files) > 1 {
		spec = files[1]
	}
	st := Load(o, files[0], spec)
	return out.String(), errb.String(), tr.String(), st
}

var posRE = regexp.MustCompile(`(?m)^(id|str) -?\d+ `)

func noPos(s string) string { return posRE.ReplaceAllString(s, "$1 ") }

func readFile(t *testing.T, name string) string {
	t.Helper()
	b, err := os.ReadFile(name)
	if err != nil {
		t.Fatal(err)
	}
	return string(b)
}

func TestPrograms(t *testing.T) {
	lib, _ := filepath.Abs("testdata/lib")
	for _, c := range []string{"diamond", "params", "cycle", "spec", "specimport"} {
		t.Run(c, func(t *testing.T) {
			tree := readFile(t, filepath.Join("testdata", c, "tree.expected"))
			trace := readFile(t, filepath.Join("testdata", c, "trace.expected"))
			out, errs, tr, st := run(t, filepath.Join("testdata", c), lib, true)
			if st != 0 || out != tree {
				t.Fatalf("status %d stderr %q\ntree\n%s\nwant\n%s", st, errs, out, tree)
			}
			if noPos(tr) != trace {
				t.Fatalf("trace\n%s\nwant\n%s", tr, trace)
			}
			cycle := ""
			if c == "cycle" {
				cycle = "\n[warning] :file=cb.eln\tline=3\tpos=0\nthere is probably a cycle in imports of module ca\n"
			}
			if errs != cycle {
				t.Fatalf("stderr %q, want %q", errs, cycle)
			}
		})
	}
}

// start with [s]: strat[s] loaded from the library by readmodules at
// stacki 0 (one space), after the imports of the query and before bool.
func TestStratLgi(t *testing.T) {
	lib, _ := filepath.Abs("testdata/lib")
	want := readFile(t, "testdata/stratlgi/tree.expected")
	out, errs, _, st := run(t, "testdata/stratlgi", lib, true)
	if st != 0 || out != want || errs != "" {
		t.Fatalf("status %d stderr %q\ntree\n%s\nwant\n%s", st, errs, out, want)
	}
}

// installed is the library of the error corpus (some cases read bool.eln
// from it): $ELANLIB, else the worktree's build/install.
func installed(t *testing.T) string {
	if l := os.Getenv("ELANLIB"); l != "" {
		return l
	}
	l, _ := filepath.Abs("../../../../build/install")
	if _, err := os.Stat(filepath.Join(l, "share/elanlib/common/bool.eln")); err != nil {
		t.Skip("no installed ELAN library: set ELANLIB")
	}
	return l
}

// earleyCut keeps the expected text up to the `[earley] syntax error` line
// (Earley's expected symbols are S8b's).
func earleyCut(s string) string {
	i := strings.Index(s, "[earley] syntax error on symbol ")
	if i < 0 {
		return s
	}
	j := strings.Index(s[i:], "\n")
	return s[:i+j+1]
}

// The load cases of the error corpus give the C++'s stdout, stderr and
// status (elan -b).
func TestErrorCorpus(t *testing.T) {
	lib := installed(t)
	abs, _ := filepath.Abs(corpus)
	dirs, _ := filepath.Glob(filepath.Join(abs, "*"))
	n := 0
	for _, d := range dirs {
		if _, err := os.Stat(filepath.Join(d, "args")); err != nil {
			continue
		}
		n++
		t.Run(filepath.Base(d), func(t *testing.T) {
			want := readFile(t, filepath.Join(d, "expected"))
			out, errs, _, st := run(t, d, lib, false)
			got := fmt.Sprintf("%s@@stderr\n%s@@status %d\n", out, errs, st)
			if _, err := os.Stat(filepath.Join(d, "earley")); err == nil {
				ws := want[strings.LastIndex(want, "@@status"):]
				want, got = earleyCut(want)+ws, earleyCut(got)+got[strings.LastIndex(got, "@@status"):]
			}
			if got != want {
				t.Fatalf("got\n%s\nwant\n%s", got, want)
			}
		})
	}
	if n < 15 {
		t.Fatalf("only %d cases", n)
	}
}

func TestDivideOnArgs(t *testing.T) {
	for _, c := range []struct {
		in   string
		name string
		args []string
	}{
		{"a[b[c,d],e]", "a", []string{"b[c,d]", "e"}},
		{"list[pair[a,b]]", "list", []string{"pair[a,b]"}},
		{"m", "m", nil},
		{"m[1]", "m", []string{"1"}},
		{"t[<a->b>,c]", "t", []string{"<a->b>", "c"}},
	} {
		n, a := divideonargs(c.in)
		if n != c.name || !reflect.DeepEqual(a, c.args) {
			t.Errorf("divideonargs(%q) = %q %q", c.in, n, a)
		}
	}
}

func TestParseArgs(t *testing.T) {
	o, files, err := ParseArgs([]string{"-b", "--export", "/dev/null", "--elanlib", "/x", "-l", "lib", "--quote", "--MAXLENNTERM", "9", "p.lgi", "s.spc"})
	if err != nil || !o.Batch || o.Warnings || o.ElanLibOpt != "/x" || o.SecondLib != "lib" || !o.Quote || !reflect.DeepEqual(files, []string{"p.lgi", "s.spc"}) {
		t.Fatalf("%+v %q %v", o, files, err)
	}
	if o, _, _ := ParseArgs([]string{"p.lgi"}); o.Batch || !o.Warnings {
		t.Fatalf("defaults %+v", o)
	}
	if _, _, err := ParseArgs([]string{"--nosuch", "p.lgi"}); err == nil {
		t.Fatal("unknown option accepted")
	}
}

// front-trace load --refuse: an IF condition is refused (macro.Refuse).
func TestRefuse(t *testing.T) {
	lib, _ := filepath.Abs("testdata/lib")
	abs, _ := filepath.Abs("testdata/refuse")
	t.Chdir(abs)
	var out, errb bytes.Buffer
	o, files, _ := ParseArgs([]string{"-b", "p.lgi"})
	o.ElanLib, o.Out, o.Err, o.Eval = lib, &out, &errb, macro.Refuse{}
	if st := Load(o, files[0], ""); st != 1 || !strings.HasSuffix(errb.String(), "[preprocessor] FOR EACH and IF are not evaluated before S8d\n") {
		t.Fatalf("status %d stderr %q", st, errb.String())
	}
	errb.Reset()
	o.Eval = macro.Mark{}
	if st := Load(o, files[0], ""); st != 0 || errb.Len() != 0 {
		t.Fatalf("Mark: status %d stderr %q", st, errb.String())
	}
}
