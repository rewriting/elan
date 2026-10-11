package lex

import "elan/front/diag"

// Buffer is lbuffer: a FIFO of lexems (the C++ keeps chunks of CHUNKSIZE;
// the order is all that shows).
type Buffer struct{ l []Lexem }

// Put is lbuffer::put.
func (b *Buffer) Put(l Lexem) { b.l = append(b.l, l) }

// Get is lbuffer::get: the first lexem; empty is a fatal internal error.
func (b *Buffer) Get(d *diag.D) Lexem {
	if len(b.l) == 0 {
		d.Errs("[lbuffer::get] try to get from empty buff, internall error\n")
		d.Fail()
	}
	l := b.l[0]
	b.l = b.l[1:]
	if len(b.l) == 0 {
		b.l = nil
	}
	return l
}

// IsEmpty is lbuffer::isempty.
func (b *Buffer) IsEmpty() bool { return len(b.l) == 0 }

// Clear is lbuffer::clear.
func (b *Buffer) Clear() { b.l = nil }

// Copy is lbuffer::copy: into becomes a copy of b.
func (b *Buffer) Copy(into *Buffer) { into.l = append([]Lexem(nil), b.l...) }

// Append is lbuffer::append: b's lexems are put at the end of to.
func (b *Buffer) Append(to *Buffer) { to.l = append(to.l, b.l...) }

// Lexems are the lexems of the buffer, in order (not a copy).
func (b *Buffer) Lexems() []Lexem { return b.l }
