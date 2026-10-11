// Package lr is the LR(1) driver of the interpreter's MACC grammars
// (src/interpreter/parse/grammars/include.parser) and their tables, converted
// from the build's generated mparser.cc, ldparser.cc and ppexparser.cc by
// front/lr/gen into tables_gen.go (checked in; `make front-tables`
// regenerates it, and front/lr/gen's test compares when ELAN_GENERATED_DIR is
// set). Spec S8, decision S8-4.
package lr

// Kind is the action of a table entry (the generated #define reduction ...).
type Kind uint8

const (
	Reduction Kind = 0
	Accept    Kind = 1
	Shift     Kind = 2
	SRErr     Kind = 3
	RRErr     Kind = 4
)

// Default is DEFAULTSYM, the symbol of a state's default action (always its
// first entry when it has one).
const Default = -32000

// Action is one entry of appr: on Sym, Kind; Num is the target state of a
// shift or the semantic code of a reduction, Left the nonterminal and Len the
// length of its right-hand side.
type Action struct {
	Sym  int32
	Kind Kind
	Num  int32
	Left int32
	Len  int32
}

// Goto is one entry of apak: after a reduction to Sym, go to state To.
type Goto struct{ Sym, To int32 }

// State is one entry of STTAB: its actions and gotos. Both are slices of one
// array per table whose capacity runs to the end of that array, as the C
// pointers do (&appr[k]): FFOUND/RFOUND read one entry even when the count is
// 0, and Task 6 reproduces that by reslicing within the capacity.
type State struct {
	Pr []Action
	Ak []Goto
}

// Table is a parser table (STTAB); the last state is MACC's sentinel.
type Table struct {
	Name   string
	States []State
}
