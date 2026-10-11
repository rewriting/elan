package elanrt

import (
	"errors"
	"os"

	"elan/elanrt/earley"
)

// Reading the query (S7d task 5), as REM's compiled programs
// (runtime/termIn.c termParser, query mode 2): without -noInput, [Main]
// prints the prompt, parses a term of the query's sort from the standard
// input up to `end` (package earley, REM's Earley parser), prints a
// newline, then runs the program with the term, normalised. A syntax
// error: REM's message on the standard error, status 1.

// QueryParser is the query parser of a program: its grammar (the tables
// of earley.Tables) and how the term of each grammar rule is built (the
// generated functions).
type QueryParser struct {
	Rules []earley.Rule
	// Sort is the query's sort (the start symbol)
	Sort      int32
	SortNames map[int32]string
	// InternIdent, InternInt, InternString: the sorts of the standard
	// rules (earley.Tables)
	InternIdent, InternInt, InternString int32
	// Op is the normal form of the operator of grammar rule r applied to
	// a, AC that of the AC operator of rule r applied to the elements e;
	// IsAC tells whether the operator of rule r is AC
	Op   func(r int, a []Term) Term
	AC   func(r int, e []Term) Term
	IsAC func(r int) bool
}

var queryParser *QueryParser

// SetQuery sets the query parser of the program (before [Main]).
func SetQuery(q *QueryParser) { queryParser = q }

// queryBuilder builds the terms of the query (earley.Builder).
type queryBuilder struct{ q *QueryParser }

func (b queryBuilder) Op(r int, a []Term) Term { return b.q.Op(r, a) }
func (b queryBuilder) AC(r int, e []Term) Term { return b.q.AC(r, e) }
func (b queryBuilder) IsAC(r int) bool         { return b.q.IsAC(r) }
func (queryBuilder) Int(n int32) Term          { return MkInt(n) }
func (queryBuilder) Ident(i int32) Term        { return MkIdent(i) }
func (queryBuilder) Str(s string) Term         { return MkStr(s) }

// readQuery reads the query from the standard input, as REM's
// EarleyParser: the term built and normalised, then a newline; an error
// stops the program.
func readQuery(q *QueryParser) Term {
	t := &earley.Tables{Rules: q.Rules, IdentIndex: IdentIndex, IdentName: IdentName,
		SortName:    func(s int32) string { return q.SortNames[s] },
		InternIdent: q.InternIdent, InternInt: q.InternInt, InternString: q.InternString}
	fatal := func(err error) {
		FlushOutput()
		var f *earley.Fatal
		if errors.As(err, &f) {
			os.Stderr.WriteString(f.Msg)
			Exit(f.Status)
		}
		Exit(1)
	}
	tree, err := earley.Parse(os.Stdin, t, q.Sort, os.Stderr)
	if err != nil {
		fatal(err)
	}
	query := func() Term {
		defer func() {
			if x := recover(); x != nil {
				f, ok := x.(*earley.Fatal)
				if !ok {
					panic(x)
				}
				fatal(f)
			}
		}()
		return earley.Build[Term](tree, queryBuilder{q})
	}()
	Out.WriteString("\n")
	return query
}
