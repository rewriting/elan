package lex

import (
	"fmt"
	"strings"

	"elan/front/tables"
)

// Stream is lstream: the lexer. underID (UNDERID) makes '_' part of
// identifiers (.lgi, .spc); the preprocessor's base stream has it off
// (NONUNDERID) and joins identifiers itself.
type Stream struct {
	c          *Context
	cs         *CharStream
	underID    bool
	flex       Lexem
	lastInline []Lexem
	inline     bool // lastinline != NULL
}

// New is lstream(ichstream*, ...): primes the look-ahead lexem (ilex(flex)).
func New(c *Context, cs *CharStream, underID bool) *Stream {
	s := &Stream{c: c, cs: cs, underID: underID}
	s.Ilex()
	return s
}

// Ctx is the stream's context.
func (s *Stream) Ctx() *Context { return s.c }

// Chars is the character stream.
func (s *Stream) Chars() *CharStream { return s.cs }

// Fulex is lstream::fulex: the look-ahead lexem.
func (s *Stream) Fulex() Lexem { return s.flex }

func isAlpha(c int) bool { return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' } // C locale
func isDigit(c int) bool { return c >= '0' && c <= '9' }

// Ilex is lstream::ilex: returns the look-ahead lexem and reads the next one.
func (s *Stream) Ilex() Lexem {
	l := s.flex
	is := s.cs
	is.BlankSkip()
	c, fc := is.Ich(), is.Fuch()
	// comments and inlines
	for c == '/' && (fc == '*' || fc == '/' || fc == '#') {
		switch fc {
		case '/':
			for fc != '\n' {
				c, fc = is.Ich(), is.Fuch()
				if fc == EOF {
					s.Oerr("[lexan] comment throw end of input stream\n")
					s.c.D.Fail()
				}
			}
		case '#':
			if s.inline {
				s.Owarn("\ttwo succesives inline definitions,\n\tprevious one will be lost\n")
			}
			s.inline, s.lastInline = true, []Lexem{}
			is.Ich()
			c, fc = is.Ich(), is.Fuch()
			for c != '#' || fc != '/' {
				s.lastInline = append(s.lastInline, CharLex(c))
				c, fc = is.Ich(), is.Fuch()
				if fc == EOF {
					s.Oerr("[lexan] inline throw end of input stream\n")
					s.c.D.Fail()
				}
			}
		default:
			is.Ich()
			c, fc = is.Ich(), is.Fuch()
			for c != '*' || fc != '/' {
				c, fc = is.Ich(), is.Fuch()
				if fc == EOF {
					s.Oerr("[lexan] comment throw end of input stream\n")
					s.c.D.Fail()
				}
			}
		}
		is.Ich()
		is.BlankSkip()
		c, fc = is.Ich(), is.Fuch()
	}
	if c == '"' {
		var str []byte
		c, fc = is.Ich(), is.Fuch()
		for c != '"' {
			switch {
			case c == '\\' && fc == 'n':
				str = append(str, '\n')
				is.Ich()
			case c == '\\' && fc == 't':
				str = append(str, '\t')
				is.Ich()
			case c == '\\' && fc == '\\':
				str = append(str, '\\')
				is.Ich()
			case c == '\\' && fc == '"':
				str = append(str, '"')
				is.Ich()
			default:
				str = append(str, byte(c)) // EOF stored as (char)-1, as in C
			}
			if len(str) >= tables.StrLen {
				s.c.D.Errs("\nstring constant too long\n")
				s.c.D.Fail()
			}
			c, fc = is.Ich(), is.Fuch()
		}
		s.flex = s.c.StringLex(string(str))
		return l
	}
	if c == '`' {
		var n int
		c, fc = is.Ich(), is.Fuch()
		switch {
		case c == '\\' && fc == 'n':
			is.Ich()
			n = '\n'
		case c == '\\' && fc == 't':
			is.Ich()
			n = '\t'
		case c == '\\' && fc == '\\':
			is.Ich()
			n = '\\'
		case c == '\\' && fc == '"':
			is.Ich()
			n = '"'
		default:
			n = c
		}
		c = is.Ich()
		if c != '`' {
			s.c.D.Errs("\nwrong string constant\n")
			s.c.D.Fail()
		}
		s.flex = NumLex(int32(n))
		return l
	}
	// BORO's hacks: \n and \t are the identifiers "newline" and "tab"
	if c == '\\' {
		if fc == 'n' {
			is.Ich()
			s.flex = IdLex(s.c.Ident.AddStr("\n"))
			return l
		}
		if fc == 't' {
			is.Ich()
			s.flex = IdLex(s.c.Ident.AddStr("\t"))
			return l
		}
	}
	// identifier: the first IDLEN-1 characters are kept
	if isAlpha(c) {
		ide := []byte{byte(c)}
		for isAlpha(fc) || isDigit(fc) || (fc == '_' && s.underID) {
			tc := is.Ich()
			if len(ide) < tables.IDLen-1 {
				ide = append(ide, byte(tc))
			}
			fc = is.Fuch()
		}
		s.flex = IdLex(s.c.Ident.AddStr(string(ide)))
		return l
	}
	// number: wraps modulo 2^32 into a C int
	if isDigit(c) {
		u := uint32(c - '0')
		for isDigit(fc) {
			c, fc = is.Ich(), is.Fuch()
			u = u*10 + uint32(c-'0')
		}
		s.flex = NumLex(int32(u))
		return l
	}
	if c == EOF {
		s.flex = NoLexem
		return l
	}
	s.flex = CharLex(c)
	return l
}

func (s *Stream) position() string {
	return fmt.Sprintf("file=%s\tline=%d\tpos=%d\n", s.cs.ActName(), s.cs.ActLine(), s.cs.ActPos())
}

// Oerr is lstream::oerr(...): the position line, then the parts.
func (s *Stream) Oerr(parts ...string) {
	s.c.D.Errs("\n[error] :", s.position(), strings.Join(parts, ""))
}

// Owarn is lstream::owarn(...): as Oerr, when warnings are on.
func (s *Stream) Owarn(parts ...string) {
	s.c.D.Warn("\n[warning] :", s.position(), strings.Join(parts, ""))
}

// BeforeMess is lstream::beforemess: the next 7 lexems (or up to the end of
// the stream) after the error, read with Ilex.
func (s *Stream) BeforeMess(l Lexem) {
	s.c.D.Errs("\n\t before  ")
	for n := 0; n < 7 && !l.IsEOS(); n++ {
		l = s.Ilex()
		s.c.D.Errs(s.c.Alfsy(l), " ")
	}
	s.c.D.Errs(" \n")
}

// LastInline is lstream::getlastinlineAndinit: the inline read since the
// last call (nil if none), as character lexems; it resets it.
func (s *Stream) LastInline() []Lexem {
	if !s.inline {
		return nil
	}
	in := s.lastInline
	s.inline, s.lastInline = false, nil
	return in
}

// SetPosition is lstream::setposition.
func (s *Stream) SetPosition(line, pos int) { s.cs.SetPosition(line, pos) }

func (s *Stream) ActLine() int { return s.cs.ActLine() }
func (s *Stream) ActPos() int  { return s.cs.ActPos() }
