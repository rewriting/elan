package lex

import (
	"errors"
	"os"
	"path/filepath"
)

// EOF is EOFICHSTR (EOF of stdio).
const EOF = -1

// Lib holds the library prefixes of the lookup (base/commondefs.cc perslib,
// elanlibcommon, elanlibqnq, elanlibstrat, elanlibref).
type Lib struct{ Pers, Common, QNQ, Strat, Ref string }

// NewLib sets the prefixes as elan_main does (driver/ldmain.cc): elanlib is
// --elanlib (elanlibOpt) or else $ELANLIB, plus "/"; the four library
// directories are string concatenations (no path cleaning: a double / stays
// and shows in error positions); perslib is -l/--secondlib or
// $SECONDELANLIB (resolved by the caller), else "..", plus "/". The C++
// falls back on its compiled-in PREFIX when no elanlib is given; S8a has
// none, so that is an error.
func NewLib(elanlib, elanlibOpt, perslib string, quote bool) (Lib, error) {
	if elanlibOpt != "" {
		elanlib = elanlibOpt
	}
	if elanlib == "" {
		return Lib{}, errors.New("no library: set ELANLIB or give --elanlib (the Go front end has no compiled-in prefix)")
	}
	elanlib += "/"
	qnq := "share/elanlib/noquote/"
	if quote {
		qnq = "share/elanlib/quote/"
	}
	if perslib == "" {
		perslib = ".."
	}
	return Lib{
		Pers:   perslib + "/",
		Common: elanlib + "share/elanlib/common/",
		QNQ:    elanlib + qnq,
		Strat:  elanlib + "share/elanlib/strategy/",
		Ref:    elanlib + "share/elanlib/ref/",
	}, nil
}

// CharStream is ichstream: a character stream with one character of
// look-ahead (fchar); line and pos count the look-ahead character.
type CharStream struct {
	data      []byte
	next      int // index of the next byte to read into fchar
	fchar     int
	line, pos int
	name      string
}

// readExact is fopen_exact: the file only if its name matches exactly, also
// on a case-insensitive file system (a directory that cannot be listed keeps
// the open's answer).
func readExact(path string) ([]byte, bool) {
	b, err := os.ReadFile(path)
	if err != nil {
		if !isDirRead(path) {
			return nil, false
		}
		b = nil
	}
	dir, base := filepath.Split(path)
	if dir == "" {
		dir = "."
	}
	ents, err := os.ReadDir(dir)
	if err != nil {
		return b, true
	}
	for _, e := range ents {
		if e.Name() == base {
			return b, true
		}
	}
	return nil, false
}

// isDirRead: fopen(dir, "r") succeeds in C and every fgetc is EOF; Go's
// ReadFile fails on a directory. Treat it as an empty file.
func isDirRead(path string) bool {
	fi, err := os.Stat(path)
	return err == nil && fi.IsDir()
}

// Open is ichstream(const char*): the name as given (plain fopen, a
// case-insensitive match accepted), then perslib, the common, noquote/quote,
// strategy and ref libraries, exact case there (spec S8, S8-14). Not found:
// `[error] can't open input file 'NAME'`, fatal.
func Open(c *Context, name string) *CharStream {
	if b, err := os.ReadFile(name); err == nil || isDirRead(name) {
		return FromBytes(name, b)
	}
	for _, prefix := range []string{c.Lib.Pers, c.Lib.Common, c.Lib.QNQ, c.Lib.Strat, c.Lib.Ref} {
		fn := prefix + name // addsuffix
		if b, ok := readExact(fn); ok {
			return FromBytes(fn, b)
		}
	}
	c.D.Errs("[error] can't open input file '" + name + "'\n\tfatal\n")
	c.D.Fail()
	return nil
}

// FromBytes is a stream over b named name (commonopen): generated modules
// (spec S8-9) and tests.
func FromBytes(name string, b []byte) *CharStream {
	s := &CharStream{data: b, name: name, fchar: 1}
	s.Ich()
	return s
}

// Ich is ichstream::ich: the look-ahead character, and the next one read.
func (s *CharStream) Ich() int {
	c := s.fchar
	if c != EOF {
		if s.next < len(s.data) {
			s.fchar = int(s.data[s.next])
			s.next++
		} else {
			s.fchar = EOF
		}
		if s.fchar == '\n' {
			s.line++
			s.pos = 0
		} else {
			s.pos++
		}
	}
	return c
}

// Fuch is ichstream::fuch: the look-ahead character.
func (s *CharStream) Fuch() int { return s.fchar }

// BlankSkip is ichstream::blankskip: skips bytes <= ' '.
func (s *CharStream) BlankSkip() {
	for s.fchar <= ' ' && s.fchar != EOF {
		s.Ich()
	}
}

func (s *CharStream) ActLine() int         { return s.line }
func (s *CharStream) ActPos() int          { return s.pos }
func (s *CharStream) ActName() string      { return s.name }
func (s *CharStream) SetPosition(l, p int) { s.line, s.pos = l, p }
