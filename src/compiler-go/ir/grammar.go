package ir

import (
	"fmt"

	"elan/ref"
)

// The tables of the query parser (S7d task 5): the grammar rules of the
// .ref file as REM writes them in the generated C (int gram[],
// rem/parser/REFParser.jj SymbolDeclaration), read at run time by the
// Earley parser of elanrt/earley as REM's runtime reads gram[]
// (earley/runtimeInit.cc grammarInit).

// GrammarRule is a rule of the query grammar: Sort ::= Rhs, building
// Symbol.
type GrammarRule struct {
	// Sort is the left-hand side (a sort index of the .ref file).
	Sort int32
	// Symbol is the function symbol code (the rule number of the parser).
	Symbol int32
	// Info is the priority word: priority + syntax bits * 4096 (bits 12-13
	// the associativity, RLEFTASSOC/RRIGHTASSOC, then built-in strategy and
	// printable form).
	Info int32
	// Rhs is the right-hand side, blanks left out.
	Rhs []GrammarLexem
}

// Kinds of GrammarLexem (REM's codes: runtimeInit.cc IDENTCODE...).
const (
	GramIdent  uint8 = 0
	GramSort   uint8 = 1
	GramChar   uint8 = 2
	GramNumber uint8 = 3
)

// GrammarLexem is a lexem of a right-hand side: an identifier (Code: its
// index), a sort (an argument), a character or a number.
type GrammarLexem struct {
	Kind uint8
	Code int32
}

// Grammar returns the grammar rules of p in REM's gram[] order: the three
// dumps (Both, GlobalOnly, TopOnly) in file order. A String lexem, which
// REM does not read ("Wrong lexem in REF file"), is an error.
func Grammar(p *ref.Program) ([]GrammarRule, error) {
	var rules []GrammarRule
	for _, dump := range p.Grammars.Dumps() {
		for _, sg := range dump {
			for _, op := range sg.Operators {
				r := GrammarRule{Sort: int32(sg.Sort), Symbol: int32(op.Symbol),
					Info: op.Priority + int32(op.Syntax)*4096}
				for _, l := range op.Rhs {
					var k uint8
					switch l.Kind {
					case ref.LexBlank:
						continue
					case ref.LexIdent:
						k = GramIdent
					case ref.LexType:
						k = GramSort
					case ref.LexChar:
						k = GramChar
					case ref.LexNum:
						k = GramNumber
					default:
						return nil, fmt.Errorf("grammar rule of symbol %d: lexem %s(%d)", op.Symbol, l.Kind, l.N)
					}
					r.Rhs = append(r.Rhs, GrammarLexem{Kind: k, Code: int32(l.N)})
				}
				rules = append(rules, r)
			}
		}
	}
	return rules, nil
}
