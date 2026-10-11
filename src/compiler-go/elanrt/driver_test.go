package elanrt

import (
	"bytes"
	"compress/gzip"
	"io"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

// Exact output of the C main program (-noInput -quiet), including the
// spacing flag kept across results (legacy bench enum: "result =  s(o)").
func TestSessionOutputAsCompiledPrograms(t *testing.T) {
	o := ParseOptions([]string{"-noInput", "-quiet"})
	if !o.NoInput || !o.Quiet || o.Help {
		t.Fatalf("options %+v", o)
	}
	var out bytes.Buffer
	s := NewSession(o, &out)
	steps := Steps
	Steps = 0
	defer func() { Steps = steps }()
	for i := range 3 {
		Step()
		s.Result(nat(i))
	}
	s.ResultInt(-5)
	s.ResultBool(true)
	s.ResultStr("a b")
	s.Finish()
	want := "\nresult = o\n\nresult =  s(o)\n\nresult = s(s(o))\n\nresult = -5\n\nresult = true\n\nresult = a b\n\nrewrite_step = 3\n"
	if out.String() != want {
		t.Fatalf("got %q\nwant %q", out.String(), want)
	}
}

func TestSessionWithoutQuietPrintsTheTime(t *testing.T) {
	var out bytes.Buffer
	s := NewSession(ParseOptions([]string{"-noInput"}), &out)
	s.Result(Mk0(symNil))
	s.Finish()
	if !strings.Contains(out.String(), "\nresult = nil\n\nrewrite_step = ") ||
		!strings.Contains(out.String(), "\ntotal time    = ") {
		t.Fatalf("got %q", out.String())
	}
}

func TestOptions(t *testing.T) {
	o := ParseOptions([]string{"-quiet", "-trace", "3", "-unknown", "-h", "rest", "-noInput"})
	if !o.Quiet || !o.Help || o.NoInput || len(o.Rest) != 2 || o.Rest[0] != "rest" {
		t.Fatalf("options %+v", o)
	}
	var out bytes.Buffer
	s := NewSession(ParseOptions([]string{"-noOutput"}), &out)
	s.ResultInt(5)
	s.Finish()
	if s.Results() != 1 || !strings.HasPrefix(out.String(), "\nrewrite_step = ") {
		t.Fatalf("-noOutput: %q", out.String())
	}
}

func init() {
	helpers["main"] = func() {
		Main("Nat", false, func(s *Session, _ Term) {
			for i := range 3 {
				Step()
				s.Result(nat(i))
			}
		})
	}
	helpers["usesquery"] = func() {
		Main("Nat", true, func(s *Session, q Term) {
			s.Result(Mk1(symS, q))
		})
	}
	helpers["panic"] = func() {
		Main("Nat", false, func(s *Session, _ Term) {
			s.Result(nat(1))
			panic("plus_320: no rule applies to a total operator")
		})
	}
	helpers["fatal"] = func() {
		Main("Nat", false, func(s *Session, _ Term) {
			s.Result(nat(1))
			Fatal("elanrt: no rule for f(o)")
		})
	}
}

// The real main program: output bytes and exit statuses.
func TestMainProgram(t *testing.T) {
	out, errs, st := runHelper(t, "main", nil, "-noInput", "-quiet")
	if st != 0 || errs != "" || out != "\nresult = o\n\nresult =  s(o)\n\nresult = s(s(o))\n\nrewrite_step = 3\n" {
		t.Fatalf("-noInput -quiet: status %d, out %q, err %q", st, out, errs)
	}
	out, _, st = runHelper(t, "main", nil, "-noInput")
	if st != 0 || !strings.Contains(out, "\nrewrite_step = 3\ntotal time    = ") {
		t.Fatalf("-noInput: status %d, out %q", st, out)
	}
	out, _, st = runHelper(t, "main", nil, "-help")
	if st != 1 || out != Help {
		t.Fatalf("-help: status %d, out %q", st, out)
	}
	out, errs, st = runHelper(t, "main", nil)
	if st != 2 || out != "Enter a query term of sort 'Nat'\n" || !strings.Contains(errs, "use -noInput") {
		t.Fatalf("query: status %d, out %q, err %q", st, out, errs)
	}
	// a start term that uses the query, run with -noInput: REM's message
	// (Query.java), status 0, the program not run
	for _, args := range [][]string{{"-noInput", "-quiet"}, {"-noInput"}} {
		out, errs, st = runHelper(t, "usesquery", nil, args...)
		if st != 0 || errs != "" || out != "No ground term is defined\nDo not use -noInput in this case\n" {
			t.Fatalf("query under %v: status %d, out %q, err %q", args, st, out, errs)
		}
	}
	out, errs, st = runHelper(t, "usesquery", nil, "-quiet")
	if st != 2 || out != "Enter a query term of sort 'Nat'\n" || !strings.Contains(errs, "use -noInput") {
		t.Fatalf("query: status %d, out %q, err %q", st, out, errs)
	}
	// a run-time error: the results printed before, then the message
	out, errs, st = runHelper(t, "fatal", nil, "-noInput", "-quiet")
	if st != 1 || out != "\nresult = s(o)\n" || errs != "elanrt: no rule for f(o)\n" {
		t.Fatalf("fatal: status %d, out %q, err %q", st, out, errs)
	}
	// a panic (a bug of the compiler or the runtime): the same, not Go's
	// exit status 2 ("not supported yet")
	out, errs, st = runHelper(t, "panic", nil, "-noInput", "-quiet")
	if st != 1 || out != "\nresult = s(o)\n" ||
		!strings.HasPrefix(errs, "elanrt: internal error: plus_320: no rule applies to a total operator\n") {
		t.Fatalf("panic: status %d, out %q, err %q", st, out, errs)
	}
}

func init() {
	helpers["unbalanced"] = func() {
		Main("Nat", false, func(s *Session, _ Term) {
			s.Result(nat(1))
			Guard() // without its Unguard: a bug of the generated code
		})
	}
}

// rt.Main checks that every Guard had its Unguard: an unbalanced depth at
// the end of the program is an internal error (status 1), the results
// printed before kept.
func TestMainDepthBalance(t *testing.T) {
	out, errs, st := runHelper(t, "unbalanced", nil, "-noInput", "-quiet")
	if st != 1 || out != "\nresult = s(o)\n" ||
		!strings.HasPrefix(errs, "elanrt: internal error: guarded depth 1 at the end of the program\n") {
		t.Fatalf("status %d, out %q, err %q", st, out, errs)
	}
}

func init() {
	helpers["profiled"] = func() {
		Main("Nat", false, func(s *Session, _ Term) {
			s.Result(nat(1))
			switch os.Getenv("ELANRT_END") {
			case "fatal":
				Fatal("elanrt: no rule for f(o)")
			case "toodeep":
				TooDeep()
			case "exit":
				Exit(0)
			}
		})
	}
}

// The profiles asked for (ELAN_CPUPROFILE, ELAN_MEMPROFILE) are complete
// whatever the end of the program: normal, a run-time error, a recursion
// too deep, an exit.
func TestProfilesAtEveryExit(t *testing.T) {
	for _, c := range []struct {
		end    string
		status int
	}{{"", 0}, {"fatal", 1}, {"toodeep", DepthExit}, {"exit", 0}} {
		dir := t.TempDir()
		cpu, mem := filepath.Join(dir, "cpu.prof"), filepath.Join(dir, "mem.prof")
		_, errs, st := runHelper(t, "profiled", []string{"ELANRT_END=" + c.end, "ELAN_CPUPROFILE=" + cpu, "ELAN_MEMPROFILE=" + mem}, "-noInput", "-quiet")
		if st != c.status {
			t.Fatalf("%q: status %d, want %d (%s)", c.end, st, c.status, errs)
		}
		for _, f := range []string{cpu, mem} {
			b, err := os.ReadFile(f)
			if err == nil {
				var z *gzip.Reader
				if z, err = gzip.NewReader(bytes.NewReader(b)); err == nil {
					_, err = io.ReadAll(z)
				}
			}
			if err != nil {
				t.Errorf("%q: %s: %v", c.end, filepath.Base(f), err)
			}
		}
	}
}
