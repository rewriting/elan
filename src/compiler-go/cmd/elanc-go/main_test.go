package main

import (
	"errors"
	"os"
	"os/exec"
	"path/filepath"
	"slices"
	"strings"
	"testing"
)

func TestMain(m *testing.M) {
	if os.Getenv("ELANC_GO_TEST_MAIN") == "1" {
		main()
		os.Exit(0)
	}
	if os.Getenv("ELANC_GO_TEST_PANIC") == "1" {
		guarded(func() { panic("boom") })
		os.Exit(0)
	}
	os.Exit(m.Run())
}

// A panic of the compiler exits with 1 (a failure) and a message, not with
// Go's 2 (which the driver uses for "not supported").
func TestInternalErrorExitsWith1(t *testing.T) {
	exe, err := os.Executable()
	if err != nil {
		t.Fatal(err)
	}
	cmd := exec.Command(exe)
	cmd.Args[0] = "elanc-go"
	cmd.Env = append(os.Environ(), "ELANC_GO_TEST_PANIC=1")
	out, err := cmd.CombinedOutput()
	ee, ok := err.(*exec.ExitError)
	if !ok || ee.ExitCode() != 1 || !strings.HasPrefix(string(out), "elanc-go: internal error: boom\n") {
		t.Fatalf("%v\n%s", err, out)
	}
}

// The command line of the bench (run_tests.py, kind R) and of elanc.
func TestRemOptionsAreIgnored(t *testing.T) {
	o, err := parseArgs([]string{"-b", "-nosplit", "-quiet", "prog"})
	if err != nil || o.lgi != "prog" || !slices.Equal(o.elanOptions, []string{"-b"}) ||
		!slices.Equal(o.ignored, []string{"-nosplit", "-quiet"}) {
		t.Fatalf("%+v %v", o, err)
	}
	o, err = parseArgs([]string{"-strategy", "0", "-O2", "-split", "-bar", "-optimiseChoicePoint", "p", "s.spc"})
	if err != nil || o.lgi != "p" || o.spec != "s.spc" || !slices.Equal(o.elanOptions, []string{"-b"}) ||
		!slices.Equal(o.ignored, []string{"-strategy 0", "-O2", "-split", "-bar", "-optimiseChoicePoint"}) {
		t.Fatalf("%+v %v", o, err)
	}
}

func TestElanOptionsArePassed(t *testing.T) {
	o, err := parseArgs([]string{"--trace", "2", "-nosplit", "-x", "-o", "out", "--gocache", "c", "p"})
	if err != nil || !slices.Equal(o.elanOptions, []string{"-b", "--trace", "2", "-x"}) ||
		o.output != "out" || o.gocache != "c" || !slices.Equal(o.ignored, []string{"-nosplit"}) {
		t.Fatalf("%+v %v", o, err)
	}
}

func TestBadCommandLines(t *testing.T) {
	if _, err := parseArgs([]string{"-o"}); err == nil || err == errUsage {
		t.Fatalf("missing argument: %v", err)
	}
	if _, err := parseArgs(nil); err != errUsage {
		t.Fatalf("no program: %v", err)
	}
	if _, err := parseArgs([]string{"a", "b", "c"}); err != errUsage {
		t.Fatalf("three programs: %v", err)
	}
	if _, err := parseArgs([]string{"-help"}); err != errHelp {
		t.Fatalf("help: %v", err)
	}
}

// The go command: the one CMake found, recorded at the installation in
// <prefix>/share/elan-go/go-command, else go from PATH.
func TestDefaultGoCommand(t *testing.T) {
	pre := t.TempDir()
	if g := defaultGo(pre); g != "go" {
		t.Fatalf("no record: %q", g)
	}
	dir := filepath.Join(pre, "share", "elan-go")
	if err := os.MkdirAll(dir, 0o755); err != nil {
		t.Fatal(err)
	}
	goExe := filepath.Join(pre, "my go", "go")
	if err := os.MkdirAll(filepath.Dir(goExe), 0o755); err != nil {
		t.Fatal(err)
	}
	if err := os.WriteFile(filepath.Join(dir, "go-command"), []byte(goExe+"\n"), 0o644); err != nil {
		t.Fatal(err)
	}
	if g := defaultGo(pre); g != "go" {
		t.Fatalf("recorded go missing: %q", g)
	}
	if err := os.WriteFile(goExe, []byte("#!/bin/sh\n"), 0o755); err != nil {
		t.Fatal(err)
	}
	if g := defaultGo(pre); g != goExe {
		t.Fatalf("recorded go: %q, want %q", g, goExe)
	}
}

// The options of REM that change the result or the files written (S7e task
// 11): refused under any name, with a message naming elanc-rem.
func TestRemOnlyOptionsAreRefused(t *testing.T) {
	cases := [][]string{{"-coq"}, {"-proofterm"}, {"-lib"}, {"-debug"}, {"-onlyC"}, {"-noC"},
		{"-nocode"}, {"-strategy", "1"}, {"-strategy", "2"}}
	for _, c := range cases {
		_, err := parseArgs(append(append([]string{"-nosplit"}, c...), "prog"))
		var r *remOnlyError
		if !errors.As(err, &r) || r.option != strings.Join(c, " ") {
			t.Fatalf("%v: %v", c, err)
		}
		if msg := err.Error(); msg != "option "+strings.Join(c, " ")+" is REM's: use elanc-rem" {
			t.Fatalf("%v: message %q", c, msg)
		}
	}
}

// The command exits with 2 and names elanc-rem; its name in the messages is
// the one it was invoked by (elanc after the switch).
func TestRemOnlyOptionExitsWith2(t *testing.T) {
	exe, err := os.Executable()
	if err != nil {
		t.Fatal(err)
	}
	for _, c := range [][]string{{"-coq"}, {"-strategy", "2"}} {
		cmd := exec.Command(exe, append(c, "enum")...)
		cmd.Args[0] = "elanc"
		cmd.Dir = t.TempDir()
		cmd.Env = append(os.Environ(), "ELANC_GO_TEST_MAIN=1")
		if err := os.WriteFile(filepath.Join(cmd.Dir, "a.out"), nil, 0o755); err != nil {
			t.Fatal(err)
		}
		out, err := cmd.CombinedOutput()
		ee, ok := err.(*exec.ExitError)
		want := "elanc: option " + strings.Join(c, " ") + " is REM's: use elanc-rem\n"
		if !ok || ee.ExitCode() != 2 || string(out) != want {
			t.Fatalf("%v: %v\n%q, want %q", c, err, out, want)
		}
		if _, err := os.Stat(filepath.Join(cmd.Dir, "a.out")); !errors.Is(err, os.ErrNotExist) {
			t.Fatalf("%v: a previous a.out survives the refusal (%v)", c, err)
		}
	}
}

func TestUsageNamesTheCommand(t *testing.T) {
	for _, name := range []string{"elanc", "elanc-go"} {
		if u := usageText(name); !strings.HasPrefix(u, "usage: "+name+" [options] program[.lgi]") ||
			!strings.Contains(u, "use elanc-rem") {
			t.Fatalf("%s: %s", name, u)
		}
	}
}

// bin/elanc is a symlink to elanc-go (ELAN_ELANC=go): the prefix is the one
// of the file it designates.
func TestPrefixThroughASymlink(t *testing.T) {
	tmp, err := filepath.EvalSymlinks(t.TempDir())
	if err != nil {
		t.Fatal(err)
	}
	pre := filepath.Join(tmp, "pre")
	bin := filepath.Join(pre, "bin")
	other := filepath.Join(tmp, "other", "bin")
	for _, d := range []string{bin, other} {
		if err := os.MkdirAll(d, 0o755); err != nil {
			t.Fatal(err)
		}
	}
	if err := os.WriteFile(filepath.Join(bin, "elanc-go"), []byte("x"), 0o755); err != nil {
		t.Fatal(err)
	}
	if err := os.Symlink("elanc-go", filepath.Join(bin, "elanc")); err != nil {
		t.Fatal(err)
	}
	if err := os.Symlink(filepath.Join(bin, "elanc"), filepath.Join(other, "elanc")); err != nil {
		t.Fatal(err)
	}
	for _, exe := range []string{filepath.Join(bin, "elanc-go"), filepath.Join(bin, "elanc"),
		filepath.Join(other, "elanc")} {
		if p := prefixOf(exe); p != pre {
			t.Fatalf("%s: %q, want %q", exe, p, pre)
		}
	}
}
