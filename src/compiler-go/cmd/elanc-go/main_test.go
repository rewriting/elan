package main

import (
	"os"
	"os/exec"
	"path/filepath"
	"slices"
	"strings"
	"testing"
)

func TestMain(m *testing.M) {
	if os.Getenv("ELANC_GO_TEST_PANIC") == "1" {
		guarded(func() { panic("boom") })
		os.Exit(0)
	}
	os.Exit(m.Run())
}

// A panic of the compiler exits with 1 (a failure) and a message, not with
// Go's 2 (which the driver uses for "not supported yet").
func TestInternalErrorExitsWith1(t *testing.T) {
	exe, err := os.Executable()
	if err != nil {
		t.Fatal(err)
	}
	cmd := exec.Command(exe)
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
	o, err = parseArgs([]string{"-strategy", "foo", "-O2", "-split", "-bar", "-noC", "p", "s.spc"})
	if err != nil || o.lgi != "p" || o.spec != "s.spc" || !slices.Equal(o.elanOptions, []string{"-b"}) ||
		!slices.Equal(o.ignored, []string{"-strategy foo", "-O2", "-split", "-bar", "-noC"}) {
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
