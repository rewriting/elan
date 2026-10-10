package elanrt

import (
	"bytes"
	"fmt"
	"os"
	"os/exec"
	"testing"
)

// Symbols of a small "generated-like" program:
//
//	sort Nat  : o, s(@), @+@ (defined: stuck if no rule)
//	sort List : nil, @.@ : (int List) List
//	sort Prop : p(@) : (int) Prop, not(@), and(@,@) (AC, prefix), @ U @ (AC, infix)
//	f(@,@,@,@) : arity 4 (arguments not inline)
var (
	symO    = NewSym("o", 0)
	symS    = NewSym("s", 1)
	symPlus = DeclareSym(Symbol{Name: "+", Arity: 2, Print: []PrintItem{Arg(0), Char('+'), Arg(1)}})
	symNil  = NewSym("nil", 0)
	symCons = DeclareSym(Symbol{Name: ".", Arity: 2, Print: []PrintItem{Arg(0), Char('.'), Arg(1)}})
	symP    = NewSym("p", 1)
	symNot  = NewSym("not", 1)
	symAnd  = DeclareSym(Symbol{Name: "and", Arity: 2, AC: true})
	symU    = DeclareSym(Symbol{Name: "U", Arity: 2, AC: true, Print: []PrintItem{Arg(0), Ident("U"), Arg(1)}})
	symF4   = NewSym("f", 4)
)

func nat(n int) Term {
	x := Mk0(symO)
	for range n {
		x = Mk1(symS, x)
	}
	return x
}

// list returns n-1 . ... . 1 . 0 . nil, built iteratively.
func list(n int) Term {
	l := Mk0(symNil)
	for i := range n {
		l = Mk2(symCons, MkInt(int32(i)), l)
	}
	return l
}

func atom(i int) Term    { return Mk1(symP, MkInt(int32(i))) }
func and(a, b Term) Term { return MkAC2(symAnd, a, b) }

// Subprocess tests: the test binary runs itself with ELANRT_HELPER=name
// and the program's arguments; TestMain then runs the helper instead of
// the tests.
var helpers = map[string]func(){}

func TestMain(m *testing.M) {
	if name := os.Getenv("ELANRT_HELPER"); name != "" {
		h, ok := helpers[name]
		if !ok {
			fmt.Fprintln(os.Stderr, "no helper", name)
			os.Exit(99)
		}
		h()
		os.Exit(0)
	}
	os.Exit(m.Run())
}

// runHelper runs a helper in a subprocess with the extra environment env
// and the program arguments args; it returns stdout, stderr and the exit
// status.
func runHelper(t *testing.T, name string, env []string, args ...string) (string, string, int) {
	t.Helper()
	exe, err := os.Executable()
	if err != nil {
		t.Fatal(err)
	}
	cmd := exec.Command(exe, args...)
	cmd.Env = append(append(os.Environ(), "ELANRT_HELPER="+name), env...)
	var out, errb bytes.Buffer
	cmd.Stdout, cmd.Stderr = &out, &errb
	err = cmd.Run()
	status := 0
	if ee, ok := err.(*exec.ExitError); ok {
		status = ee.ExitCode()
	} else if err != nil {
		t.Fatal(err)
	}
	return out.String(), errb.String(), status
}
