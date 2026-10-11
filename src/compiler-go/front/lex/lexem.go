// Package lex is the interpreter's lexer in Go (S8a): the lexem encoding
// (lex/lexem.h, base/constants.h), the character streams with the library
// lookup (base/ichstream.cc) and the lexer (lex/lstream.cc, lex/lbuffer.cc).
// A port, function by function, the C++ names in comments; the quirks are
// kept (spec S8, decision S8-3; plan S8a finding 4).
package lex

import (
	"fmt"
	"io"

	"elan/front/diag"
	"elan/front/tables"
)

// Lexem is a lexem: one int (base/constants.h).
//
//	>= 0                     a number (its value)
//	-1 .. -255               a character (minus its code)
//	NoLexem, BlankLexem, IdentLexem   end of stream, blank, any identifier
//	BofIdent-i               identifier i (position in tabofident)
//	StringLexem              any string
//	BofString-i              string constant i
//	JustNumber               any number
//	BofTypes-i               sort i (position in typet), a nonterminal
//
// S8b unifies it with elanrt/earley.Lexem (same values); S8a keeps its own
// type to stay independent.
type Lexem int32

const (
	NoLexem     Lexem = -256                            // NOLEXEM
	BlankLexem  Lexem = -257                            // BLANKLEXEM
	IdentLexem  Lexem = -259                            // IDENT
	BofIdent    Lexem = -260                            // BOFIDENT
	StringLexem Lexem = BofIdent - tables.MaxNofIdent   // STRING, -3260
	BofString   Lexem = StringLexem - 1                 // BOFSTRING, -3261
	JustNumber  Lexem = BofString - tables.MaxNofString // JUSTNUMBER, -13261
	BofTypes    Lexem = JustNumber - 1                  // BOFTYPES, -13262
)

// IdLex is cridlex(n): identifier at position n of tabofident.
func IdLex(pos int) Lexem { return BofIdent - Lexem(pos) }

// NumLex is crnumlex(n).
func NumLex(n int32) Lexem { return Lexem(n) }

// CharLex is crcharlex(c).
func CharLex(c int) Lexem { return Lexem(-c) }

// TypeLex is crtypelex(n): sort at position n of typet.
func TypeLex(n int) Lexem { return BofTypes - Lexem(n) }

func (l Lexem) Terminal() bool  { return l > BofTypes }    // terminal
func (l Lexem) IsEOS() bool     { return l == NoLexem }    // isendofstream
func (l Lexem) IsBlank() bool   { return l == BlankLexem } // isblankk
func (l Lexem) IsIdent() bool   { return l.IsRealID() || l == IdentLexem }
func (l Lexem) IsRealID() bool  { return l <= BofIdent && l > BofIdent-tables.MaxNofIdent }
func (l Lexem) IdVal() int      { return int(BofIdent - l) }
func (l Lexem) IsNum() bool     { return l >= 0 || l == JustNumber }
func (l Lexem) IsRealNum() bool { return l >= 0 }
func (l Lexem) NumVal() int32   { return int32(l) } // the raw value, as numval()
func (l Lexem) IsChar() bool    { return l < 0 && l >= -255 }
func (l Lexem) CharVal() int    { return int(-l) }
func (l Lexem) IsString() bool  { return l.IsRealString() || l == StringLexem }
func (l Lexem) IsRealString() bool {
	return l <= BofString && l > BofString-tables.MaxNofString
}
func (l Lexem) StrIndex() int { return int(BofString - l) }
func (l Lexem) TypeVal() int  { return int(BofTypes - l) }

// Context is the lexical state shared by every stream of one load:
// tabofident, the string constants (stringconstants, numbered globally in
// order of creation) and the library paths.
type Context struct {
	D       *diag.D
	Ident   *tables.StringTab
	Strings []string
	Lib     Lib
}

// StringLex is lexem::crstringlex(char*): a new string constant. The C
// string ends at its first NUL byte (strdup).
func (c *Context) StringLex(s string) Lexem {
	for i := 0; i < len(s); i++ {
		if s[i] == 0 {
			s = s[:i]
			break
		}
	}
	if len(c.Strings) >= tables.MaxNofString {
		c.D.Errs("\ntoo many string constants\n")
		c.D.Fail()
	}
	c.Strings = append(c.Strings, s)
	return BofString - Lexem(len(c.Strings)-1)
}

// stringConstant is string_constant (lstream.cc): out of range is interr().
func (c *Context) stringConstant(i int) string {
	if i < 0 || i >= len(c.Strings) {
		c.D.Errs(fmt.Sprintf("[lexem] string constant %d out of range\n", i))
		c.D.Errs("internal error\n")
		c.D.Fail()
	}
	return c.Strings[i]
}

// Alfsy is lexem::alfsy: the printing form of a lexem in messages.
func (c *Context) Alfsy(l Lexem) string {
	switch {
	case !l.Terminal():
		return fmt.Sprintf("sort(%d)", l.TypeVal())
	case l == BlankLexem:
		return "BLANKLEXEM"
	case l == NoLexem:
		return "ENDOFLSTREAM"
	case l == IdentLexem:
		return "IDENTIFIER"
	case l == JustNumber:
		return "NUMBER"
	case l == StringLexem:
		return "???"
	case l.IsRealString():
		return c.stringConstant(l.StrIndex())
	case l >= 0:
		return fmt.Sprintf("%d", int32(l))
	case l <= BofIdent:
		if !c.Ident.Used(l.IdVal()) {
			return "(null)" // fprintf("%s", NULL)
		}
		return c.Ident.Ide(l.IdVal())
	default:
		return string([]byte{byte(-l)}) // alfs[0] = -lex (a char)
	}
}

// ErrAlfsy is lexem::erralfsy: lexems 0 .. -32 as '\n' (the number 0 too).
func (c *Context) ErrAlfsy(l Lexem) string {
	if l <= 0 && l >= -32 {
		return fmt.Sprintf("'\\%d'", -int32(l))
	}
	return c.Alfsy(l)
}

// escape writes s escaped as the trace format does: \\ \n \t \" and other
// bytes < 32 or >= 127 as \ooo (lextrace_chars in mlstream.cc).
func escape(w io.Writer, s []byte) {
	var b []byte
	for _, ch := range s {
		switch {
		case ch == '\\':
			b = append(b, '\\', '\\')
		case ch == '\n':
			b = append(b, '\\', 'n')
		case ch == '\t':
			b = append(b, '\\', 't')
		case ch == '"':
			b = append(b, '\\', '"')
		case ch < 32 || ch >= 127:
			b = append(b, '\\', '0'+ch>>6, '0'+(ch>>3)&7, '0'+ch&7)
		default:
			b = append(b, ch)
		}
	}
	w.Write(b)
}

// TraceLexem writes one line of the harness's trace (lextrace_lexem in
// mlstream.cc, tests/front/README.md).
func TraceLexem(w io.Writer, c *Context, l Lexem) {
	switch {
	case l.IsEOS():
		io.WriteString(w, "eos\n")
		return
	case l >= 0:
		fmt.Fprintf(w, "num %d\n", int32(l))
		return
	case l.IsChar():
		fmt.Fprintf(w, "char %d\n", l.CharVal())
		return
	case l.IsRealID():
		fmt.Fprintf(w, "id %d ", l.IdVal())
		escape(w, []byte(c.Ident.Ide(l.IdVal())))
	case l.IsRealString():
		fmt.Fprintf(w, "str %d ", l.StrIndex())
		escape(w, []byte(c.stringConstant(l.StrIndex())))
	default:
		fmt.Fprintf(w, "other %d\n", int32(l))
		return
	}
	io.WriteString(w, "\n")
}

// TraceInline writes the harness's `@inline <chars>` line for an inline
// buffer of character lexems.
func TraceInline(w io.Writer, in []Lexem) {
	b := make([]byte, len(in))
	for i, l := range in {
		b[i] = byte(l.CharVal())
	}
	io.WriteString(w, "@inline ")
	escape(w, b)
	io.WriteString(w, "\n")
}
