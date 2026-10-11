package macro

import (
	"elan/front/lex"
	"elan/front/tables"
)

// Var is a FOR EACH variable: its name, its sort (parsetype's text, e.g.
// `list[int]`) and its index (i - bvarnamesi in the C++).
type Var struct {
	Name  lex.Lexem
	Sort  string
	Index int
}

// Term is a parsed condition term (the evaluator's own representation).
type Term any

// Cond is one condition of a FOR EACH / IF: Var -1 for a boolean condition
// (IF, ANDIF, AND IF), else the index of the variable affected; Strategy 0
// for `()`.
type Cond struct {
	Var      int
	Strategy lex.Lexem
	Term     Term
}

// ForEach is a FOR EACH / IF form after its header (the wherelist of the
// C++): its variables, its conditions and its body.
type ForEach struct {
	Vars  []Var
	Conds []Cond
	Body  []lex.Lexem
	If    bool
}

// Solutions enumerates the instances of the variables, one lexem list per
// variable (isWhereBacktrackNextSol and term::writetobuf).
type Solutions interface {
	Next() ([][]lex.Lexem, bool)
}

// Evaluator does what the C++ preprocessor asks of the loader, at its five
// call sites (mlstream.cc macroexp, addforeach).
type Evaluator interface {
	// AddVars is addvars' check of the sort in typet; false: the
	// preprocessor prints `[preprocessor] unknown type <sort> used`.
	AddVars(s *Stream, sort string) bool
	// AddVarRules is actgram()->addvarrule for each variable.
	AddVarRules(s *Stream, vars []Var)
	// Strategy is strategyindex_refs + conform_strategies + assign_one_ref
	// for `V := (S) t`.
	Strategy(s *Stream, v Var, name lex.Lexem)
	// Term is actgram()->earleycall(this, sort, pppendofin): v nil for a
	// boolean condition; false: fatal, the evaluator printed its error.
	Term(s *Stream, v *Var, stop func(lex.Lexem) bool) (Term, bool)
	// DeleteVarRules is actgram()->deleterule for each variable.
	DeleteVarRules(s *Stream, vars []Var)
	// Solutions is addforeach's wherelist and isWhereBacktrackNextSol.
	Solutions(s *Stream, f *ForEach) Solutions
}

// Refuse is the evaluator of a module read standalone (front-trace lex,
// front-trace load --refuse): what the C++ does without a loader up to its
// first Earley call. A sort is checked in the built-in typet (the seven
// sorts of load/module.cc: a standalone typet has nothing else), the
// variable rules are not needed before Earley, and a condition term — where
// the standalone C++ needs a grammar — is refused:
// `[preprocessor] FOR EACH and IF are not evaluated before S8d`, fatal.
type Refuse struct{}

func (Refuse) AddVars(s *Stream, sort string) bool {
	return tables.NewTypeT(s.Ctx().D).Member(sort)
}
func (Refuse) AddVarRules(*Stream, []Var)       {}
func (Refuse) Strategy(*Stream, Var, lex.Lexem) {}
func (Refuse) Term(s *Stream, v *Var, stop func(lex.Lexem) bool) (Term, bool) {
	s.Oerr("[preprocessor] FOR EACH and IF are not evaluated before S8d\n")
	return nil, false
}
func (Refuse) DeleteVarRules(*Stream, []Var)         {}
func (Refuse) Solutions(*Stream, *ForEach) Solutions { return none{} }

// Mark reads FOR EACH and IF without evaluating them: every sort is
// accepted, a condition term is consumed lexically as Earley consumes it
// (earley.cc 533-566: at least one lexem, read with the stream's Ilex until
// the stop predicate holds on the look-ahead), and there is no solution
// (the trace has the two markers and no body).
type Mark struct{}

func (Mark) AddVars(*Stream, string) bool     { return true }
func (Mark) AddVarRules(*Stream, []Var)       {}
func (Mark) Strategy(*Stream, Var, lex.Lexem) {}

// Term: a term that would start on its stop lexem (`IF c ANDIF c2`, whose
// second condition the C++ parses from ANDIF on) is Earley's syntax error on
// its first lexem; S8a prints the part of oearleyerr that does not need the
// grammar (the expected symbols are S8b's) and fails.
func (Mark) Term(s *Stream, v *Var, stop func(lex.Lexem) bool) (Term, bool) {
	if stop(s.Fulex()) {
		l := s.Ilex()
		s.Oerr()
		s.d().Errs("[earley] syntax error on symbol '", s.Ctx().ErrAlfsy(l), "'\n")
		return nil, false
	}
	var t []lex.Lexem
	for {
		t = append(t, s.Ilex())
		if stop(s.Fulex()) {
			break
		}
	}
	return t, true
}

func (Mark) DeleteVarRules(*Stream, []Var)         {}
func (Mark) Solutions(*Stream, *ForEach) Solutions { return none{} }

type none struct{}

func (none) Next() ([][]lex.Lexem, bool) { return nil, false }
