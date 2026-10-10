package elanrt

import "strconv"

// Printing terms the way the compiled C programs do
// (src/compiler/runtime/termOut.c, printTextFormTerm; elanc-rs
// elan-runtime/src/print.rs).
//
// A symbol prints the tokens of its mixfix declaration in order
// ([Symbol.Print]; the prefix layout name(a1,...,an) by default), an
// argument by printing the argument. The [Writer] reproduces the C token
// spacing: a space is written before an identifier or a one-character
// token that starts with a letter or a digit when the previous token ended
// with one. Ints and strings neither test nor set that flag, and the flag is
// not reset between results (a global in C): after a result ending with o,
// the next result s(o) prints as " s(o)" (legacy bench Compiler.4.0/Test
// enum: "result =  s(o)"). Keep one Writer for the whole run.
//
// Bytes: the output is bytes, as the C runtime's. Identifiers and tokens
// are given as bytes (the generator writes the Latin-1 characters of the
// .ref one byte each); builtin strings are written as they are (raw, as
// the compiled C programs; the interpreter quotes them: [Writer.Quoted]).

// ItemKind is the kind of a [PrintItem].
type ItemKind uint8

const (
	ItemArg   ItemKind = iota // an argument
	ItemIdent                 // an identifier token (C IDENTCODE)
	ItemChar                  // a one-character token (C CHARCODE): ( , . + ...
	ItemNum                   // a number token of a declaration (C NUMCODE)
)

// PrintItem is a token of a symbol's layout.
type PrintItem struct {
	Kind ItemKind
	Arg  int    // ItemArg: the argument's position
	Text string // ItemIdent, ItemChar, ItemNum: the bytes of the token
}

// Arg is the layout item of the i-th argument.
func Arg(i int) PrintItem { return PrintItem{Kind: ItemArg, Arg: i} }

// Ident is the layout item of an identifier token.
func Ident(s string) PrintItem { return PrintItem{Kind: ItemIdent, Text: s} }

// Char is the layout item of a one-character token.
func Char(c byte) PrintItem { return PrintItem{Kind: ItemChar, Text: string([]byte{c})} }

// Num is the layout item of a number token.
func Num(n int64) PrintItem { return PrintItem{Kind: ItemNum, Text: strconv.FormatInt(n, 10)} }

// prefixLayout is name(a1,...,an).
func prefixLayout(name string, arity int) []PrintItem {
	items := []PrintItem{Ident(name)}
	if arity == 0 {
		return items
	}
	items = append(items, Char('('))
	for i := range arity {
		if i > 0 {
			items = append(items, Char(','))
		}
		items = append(items, Arg(i))
	}
	return append(items, Char(')'))
}

// Writer is a byte buffer with the C printer's token-spacing state.
type Writer struct {
	buf   []byte
	alnum bool // the last token ended with a letter or a digit
	// Quoted prints strings as the interpreter does ("a b", with \" and
	// \\); false (the default) prints them raw, as the compiled C programs.
	Quoted bool
}

func isAlnum(c byte) bool {
	return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || '0' <= c && c <= '9'
}

// Ident writes an identifier token.
func (w *Writer) Ident(s string) {
	if s == "" {
		return
	}
	if w.alnum && isAlnum(s[0]) {
		w.buf = append(w.buf, ' ')
	}
	w.buf = append(w.buf, s...)
	w.alnum = isAlnum(s[len(s)-1])
}

// Char writes a one-character token.
func (w *Writer) Char(c byte) {
	if w.alnum && isAlnum(c) {
		w.buf = append(w.buf, ' ')
	}
	w.buf = append(w.buf, c)
	w.alnum = isAlnum(c)
}

// NumCode writes a number token of a declaration: a space whenever the
// previous token ended with a letter or a digit.
func (w *Writer) NumCode(s string) {
	if w.alnum {
		w.buf = append(w.buf, ' ')
	}
	w.buf = append(w.buf, s...)
	w.alnum = true
}

// Int writes a builtin int; the spacing flag is unchanged.
func (w *Writer) Int(v int32) { w.buf = strconv.AppendInt(w.buf, int64(v), 10) }

// Bool writes true or false (identifier tokens).
func (w *Writer) Bool(b bool) {
	if b {
		w.Ident("true")
	} else {
		w.Ident("false")
	}
}

// Str writes a builtin string (raw, or quoted with [Writer.Quoted]); the
// spacing flag is unchanged.
func (w *Writer) Str(s string) {
	if !w.Quoted {
		w.buf = append(w.buf, s...)
		return
	}
	w.buf = append(w.buf, '"')
	for i := 0; i < len(s); i++ {
		if s[i] == '"' || s[i] == '\\' {
			w.buf = append(w.buf, '\\')
		}
		w.buf = append(w.buf, s[i])
	}
	w.buf = append(w.buf, '"')
}

// Raw writes text outside the term syntax ("result = "): no spacing, the
// flag is unchanged.
func (w *Writer) Raw(s string) { w.buf = append(w.buf, s...) }

// Bytes returns the bytes written since the last Take.
func (w *Writer) Bytes() []byte { return w.buf }

// Take returns the bytes written and empties the buffer (the spacing flag
// is kept).
func (w *Writer) Take() []byte {
	b := w.buf
	w.buf = nil
	return b
}

// Reset clears the spacing flag (a fresh printer).
func (w *Writer) Reset() { w.alnum = false }

// item writes a token.
func (w *Writer) item(it PrintItem) {
	switch it.Kind {
	case ItemIdent:
		w.Ident(it.Text)
	case ItemChar:
		w.Char(it.Text[0])
	case ItemNum:
		w.NumCode(it.Text)
	}
}

// Term writes a term. It iterates on the last argument of each symbol (a
// list a1.a2. ... .nil of any length needs no recursion; the tokens that
// follow a last argument wait in a stack); the other arguments are printed
// by a recursive call, guarded ([Guard]).
func (w *Writer) Term(t Term) {
	var after [][]PrintItem // tokens to write after the current term, innermost last
	for t != nil {
		t = w.head(t, &after)
	}
	for i := len(after) - 1; i >= 0; i-- {
		for _, it := range after[i] {
			w.item(it)
		}
	}
}

// subterm writes a term that is not the last argument of its parent.
func (w *Writer) subterm(t Term) {
	Guard()
	defer Unguard()
	w.Term(t)
}

// head writes t up to its last argument and returns that argument (nil
// when t is written entirely); the tokens after it go to after.
func (w *Writer) head(t Term, after *[][]PrintItem) Term {
	switch t.Sym {
	case SymInt:
		w.Int(t.Int)
		return nil
	case SymString:
		w.Str(StrOf(t))
		return nil
	}
	s := &Syms[t.Sym]
	items := s.Print
	if s.AC && len(t.Args) >= 2 {
		return w.acHead(t.Args, items, after)
	}
	last := lastArgItem(items)
	if last < 0 {
		for _, it := range items {
			w.item(it)
		}
		return nil
	}
	for _, it := range items[:last] {
		if it.Kind == ItemArg {
			w.subterm(t.Args[it.Arg])
		} else {
			w.item(it)
		}
	}
	*after = append(*after, items[last+1:])
	return t.Args[items[last].Arg]
}

// acHead writes the elements e of an AC term with the layout of its binary
// symbol (before @ middle @ after): before e1 middle e2 middle ... en after;
// it returns en.
func (w *Writer) acHead(e []Term, items []PrintItem, after *[][]PrintItem) Term {
	first := -1
	for i, it := range items {
		if it.Kind == ItemArg {
			first = i
			break
		}
	}
	last := lastArgItem(items)
	for _, it := range items[:first] {
		w.item(it)
	}
	for _, x := range e[:len(e)-1] {
		w.subterm(x)
		for _, it := range items[first+1 : last] {
			w.item(it)
		}
	}
	*after = append(*after, items[last+1:])
	return e[len(e)-1]
}

// lastArgItem is the position of the last argument in a layout, or -1.
func lastArgItem(items []PrintItem) int {
	for i := len(items) - 1; i >= 0; i-- {
		if items[i].Kind == ItemArg {
			return i
		}
	}
	return -1
}

// Format returns a term as a string, with a fresh printer (tests, error
// messages).
func Format(t Term) string {
	var w Writer
	w.Term(t)
	return string(w.Bytes())
}
