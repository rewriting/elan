package elanrt

import (
	"bufio"
	"bytes"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

// The I/O builtins (S7e task 6) as REM's fun_113..122: files numbered
// from 3 (0, 1, 2: the standard streams), the first free number, C's
// signed chars, -1 at the end of a file, REM's message when a file cannot
// be opened.

// closeAll closes the files a test left open (numbers 3 and above).
func closeAll() {
	for i := 3; i < MaxFile; i++ {
		if files[i] != nil {
			Close(int32(i))
		}
	}
}

func TestFilesWrittenThenRead(t *testing.T) {
	defer closeAll()
	path := filepath.Join(t.TempDir(), "f.txt")
	w := Open(path, "w")
	if w != 3 {
		t.Fatalf("open: %d, want 3 (the first free file)", w)
	}
	if c := Putc(w, 65); c != 65 {
		t.Fatalf("putc: %d", c)
	}
	if c := Putc(w, 0xE9+256); c != 0xE9+256 {
		t.Fatalf("putc returns its argument: %d", c)
	}
	f := Mk2(NewSym("f", 2), MkInt(1), Mk0(symO))
	if r := WriteTerm(w, f); r != f {
		t.Fatalf("write returns its term: %v", r)
	}
	if r := Flush(w); r != w {
		t.Fatalf("flush: %d", r)
	}
	if r := Close(w); r != w {
		t.Fatalf("close: %d", r)
	}
	got, err := os.ReadFile(path)
	if err != nil || string(got) != "A\xe9f(1,o)" {
		t.Fatalf("file %q, %v", got, err)
	}
	r := Open(path, "r")
	if r != 3 {
		t.Fatalf("open again: %d, want 3 (closed, free again)", r)
	}
	var cs []int32
	for range 5 {
		cs = append(cs, Getc(r))
	}
	// \xe9 is negative (C's char on the reference platforms of REM)
	if want := []int32{65, -23, 'f', '(', '1'}; !equalInts(cs, want) {
		t.Fatalf("getc: %v, want %v", cs, want)
	}
	for range 4 {
		Getc(r)
	}
	if c := Getc(r); c != -1 {
		t.Fatalf("getc at the end: %d", c)
	}
	// a second file gets the next number; appending
	a := Open(path, "a")
	if a != 4 {
		t.Fatalf("open a second file: %d", a)
	}
	Putc(a, 'Z')
	Close(a)
	Close(r)
	if got, _ := os.ReadFile(path); string(got) != "A\xe9f(1,o)Z" {
		t.Fatalf("appended: %q", got)
	}
	// read and write
	rw := Open(path, "r+")
	if c := Getc(rw); c != 'A' {
		t.Fatalf("r+: getc %d", c)
	}
	Putc(rw, 'B')
	if c := Getc(rw); c != 'f' {
		t.Fatalf("r+: getc after putc %d", c)
	}
	Close(rw)
	if got, _ := os.ReadFile(path); string(got) != "ABf(1,o)Z" {
		t.Fatalf("r+: %q", got)
	}
}

func equalInts(a, b []int32) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if a[i] != b[i] {
			return false
		}
	}
	return true
}

func TestOpenFailures(t *testing.T) {
	defer closeAll()
	var msgs bytes.Buffer
	old := ioStderr
	ioStderr = &msgs
	defer func() { ioStderr = old }()
	dir := t.TempDir()
	missing := filepath.Join(dir, "none", "x.txt")
	if r := Open(missing, "r"); r != -1 {
		t.Fatalf("open of a missing file: %d", r)
	}
	if r := Open(filepath.Join(dir, "y.txt"), "x"); r != -1 {
		t.Fatalf("open with mode x: %d", r)
	}
	want := "Error in open_file(" + missing + ",r)\nError in open_file(" + filepath.Join(dir, "y.txt") + ",x)\n"
	if msgs.String() != want {
		t.Fatalf("messages %q, want %q", msgs.String(), want)
	}
}

// helpers run in a subprocess (common_test.go): the standard streams
func init() {
	helpers["io-stdout"] = func() {
		Main("Nat", false, func(s *Session, _ Term) {
			Putc(1, 'H')
			Putc(1, 300) // (char)300 = ','
			Putc(1, '\n')
			Putc(2, 'E')
			// the printer's spacing flag is the results' (a global in C)
			WriteTerm(1, Mk0(symO))
			s.Result(Mk0(symO))
		})
	}
	helpers["io-stdin"] = func() {
		Main("Nat", false, func(s *Session, _ Term) {
			s.ResultInt(Getc(0))
			s.ResultInt(Getc(0))
			s.ResultInt(Getc(0))
			s.ResultInt(Getc(1)) // a stream not open for reading: EOF
		})
	}
	helpers["io-close-stdout"] = func() {
		Main("Nat", false, func(s *Session, _ Term) {
			Putc(1, 'A')
			Close(1)
			s.Result(Mk0(symO))
		})
	}
	helpers["io-bad-pid"] = func() {
		Main("Nat", false, func(s *Session, _ Term) {
			Putc(1, 'A')
			Getc(7)
		})
	}
	helpers["io-at-exit"] = func() {
		Main("Nat", false, func(s *Session, _ Term) {
			w := Open(os.Getenv("ELANRT_FILE"), "w")
			Putc(w, 'Q')
		})
	}
}

func TestStandardStreams(t *testing.T) {
	out, errs, st := runHelper(t, "io-stdout", nil, "-noInput", "-quiet")
	if st != 0 || out != "H,\no\nresult =  o\n\nrewrite_step = 0\n" || errs != "E" {
		t.Fatalf("status %d, out %q, err %q", st, out, errs)
	}
	out, _, st = runHelperIn(t, "io-stdin", nil, "xy", "-noInput", "-quiet")
	if want := "\nresult = 120\n\nresult = 121\n\nresult = -1\n\nresult = -1\n\nrewrite_step = 0\n"; st != 0 || out != want {
		t.Fatalf("stdin: status %d, out %q", st, out)
	}
	// REM's fclose(stdout): nothing more is printed
	out, _, st = runHelper(t, "io-close-stdout", nil, "-noInput", "-quiet")
	if st != 0 || out != "A" {
		t.Fatalf("close(stdout): status %d, out %q", st, out)
	}
}

// A file number that is not open: a run-time error (REM: undefined
// behaviour, a segmentation fault).
func TestFileNotOpen(t *testing.T) {
	out, errs, st := runHelper(t, "io-bad-pid", nil, "-noInput", "-quiet")
	if st != 1 || out != "A" || errs != "getc_builtinStdio: file 7 is not open\n" {
		t.Fatalf("status %d, out %q, err %q", st, out, errs)
	}
}

// The files left open are flushed at the end of the program (C's exit).
func TestFilesFlushedAtExit(t *testing.T) {
	path := filepath.Join(t.TempDir(), "q.txt")
	if _, errs, st := runHelper(t, "io-at-exit", []string{"ELANRT_FILE=" + path}, "-noInput", "-quiet"); st != 0 {
		t.Fatalf("status %d: %s", st, errs)
	}
	if got, err := os.ReadFile(path); err != nil || string(got) != "Q" {
		t.Fatalf("file %q, %v", got, err)
	}
}

// Putc and WriteTerm on the standard output go through Out, in order with
// the results.
func TestStdoutIsOut(t *testing.T) {
	var buf bytes.Buffer
	old := Out
	Out = bufio.NewWriter(&buf)
	defer func() { Out = old }()
	Putc(1, 'x')
	WriteTerm(1, MkStr("a b"))
	Out.Flush()
	if got := buf.String(); !strings.HasPrefix(got, "xa b") {
		t.Fatalf("%q", got)
	}
}
