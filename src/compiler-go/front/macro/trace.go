package macro

import (
	"io"

	"elan/front/lex"
)

// Tracer receives the trace hooks of mlstream.cc (ELAN_LEXTRACE): a stream
// opened (before its priming read) and closed, each lexem returned by an
// outermost Ilex, and the markers that became pending during that call.
type Tracer interface {
	Begin(name string)
	Lexem(l lex.Lexem)
	Marker(m string)
	End()
}

type traceWriter struct {
	w io.Writer
	c *lex.Context
}

// NewTraceWriter writes the harness's format (tests/front/README.md).
func NewTraceWriter(w io.Writer, c *lex.Context) Tracer { return &traceWriter{w, c} }

func (t *traceWriter) Begin(name string) { io.WriteString(t.w, "@begin "+name+"\n") }
func (t *traceWriter) Lexem(l lex.Lexem) { lex.TraceLexem(t.w, t.c, l) }
func (t *traceWriter) Marker(m string)   { io.WriteString(t.w, m+"\n") }
func (t *traceWriter) End()              { io.WriteString(t.w, "@end\n") }

// Lex reads the module text of cs (named name) to its end with a fresh
// preprocessor state, as `elan-lextrace lex` does: the trace on w (each
// lexem, its markers, then `@inline <chars>` when an inline was read during
// that call), `@end` last.
func Lex(c *lex.Context, cs *lex.CharStream, name string, ev Evaluator, w io.Writer) {
	s := New(c, NewState(), cs, name, ev, NewTraceWriter(w, c))
	for {
		l := s.Ilex()
		if in := s.LastInline(); in != nil {
			lex.TraceInline(w, in)
		}
		if l.IsEOS() {
			break
		}
	}
	s.Close()
}
