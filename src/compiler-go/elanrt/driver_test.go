package elanrt

import (
	"bytes"
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
		Main("Nat", func(s *Session) {
			for i := range 3 {
				Step()
				s.Result(nat(i))
			}
		})
	}
	helpers["fatal"] = func() {
		Main("Nat", func(s *Session) {
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
	// a run-time error: the results printed before, then the message
	out, errs, st = runHelper(t, "fatal", nil, "-noInput", "-quiet")
	if st != 1 || out != "\nresult = s(o)\n" || errs != "elanrt: no rule for f(o)\n" {
		t.Fatalf("fatal: status %d, out %q, err %q", st, out, errs)
	}
}
