package diag

import (
	"bytes"
	"testing"
)

func TestFailIsStatusOne(t *testing.T) {
	var out, err bytes.Buffer
	d := &D{Out: &out, Err: &err}
	st := Run(func() {
		d.Errs("[x] boom\n")
		d.Fail()
		t.Fatal("Fail returned")
	})
	if st != 1 || err.String() != "[x] boom\n" {
		t.Fatalf("status %d, stderr %q", st, err.String())
	}
}

func TestRunNormalReturnIsZero(t *testing.T) {
	if st := Run(func() {}); st != 0 {
		t.Fatalf("status %d", st)
	}
}

func TestRunExitStatus(t *testing.T) {
	if st := Run(func() { panic(Exit{Status: 70}) }); st != 70 {
		t.Fatalf("status %d", st)
	}
}

func TestRunRepanicsOtherPanics(t *testing.T) {
	defer func() {
		if r := recover(); r != "other" {
			t.Fatalf("recovered %v", r)
		}
	}()
	Run(func() { panic("other") })
	t.Fatal("Run swallowed a foreign panic")
}

func TestWarnOnlyWithWarnings(t *testing.T) {
	var err bytes.Buffer
	d := &D{Err: &err}
	d.Warn("a", "b")
	if err.Len() != 0 {
		t.Fatalf("warning written with Warnings off: %q", err.String())
	}
	d.Warnings = true
	d.Warn("a", "b")
	if err.String() != "ab" {
		t.Fatalf("got %q", err.String())
	}
}
