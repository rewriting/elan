package main

import (
	"slices"
	"testing"
)

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
