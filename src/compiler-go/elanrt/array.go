package elanrt

import (
	"io"
	"strconv"
)

// Arrays (S7e task 7): REM's builtinArray[X] (common/builtinArray.eln,
// codes 200-203; src/compiler/runtime/termBase.c term_newArray,
// term_getArray, term_setArray, term_getLength). An array is a node of
// the symbol [SymArray] outside the unique table: two new are two arrays,
// and an array is mutated in place by set (the mutation is not undone by
// backtracking, as in REM). Its Int is its size as given to new (REM
// prints it in its errors), its Args the elements, its Hash a number of
// its own (a node holding it is shared by the array's identity, and sees
// its later mutations). Two arrays are equal ([ArrayEq], the == of an
// array sort) when they are the same or have the same size and equal
// elements (as the interpreter's terms new(...); REM's == finds any two
// arrays equal); [Eq] compares them by identity.
//
// REM's errors, kept: get or set out of range, and get, set or length on
// a term that is not an array (a stuck term of the sort), print an error
// on the standard output and stop the program (status 1); printing an
// array (a result, or write) prints `print_f_prefix Array not yet
// implemented` and stops the same way. new with a negative size gives an
// array of no element whose errors report that size (REM: its allocation
// fails, then the first get reports the size).

// arrays counts the arrays made (their hashes).
var arrays uint64

// NewArray returns a new array of n elements init (fun_200).
func NewArray(n int32, init Term) Term {
	arrays++
	e := make([]Term, max(n, 0))
	for i := range e {
		e[i] = init
	}
	Created++
	return &Node{Sym: SymArray, Int: n, Hash: mix(arrays*0x9E3779B97F4A7C15)&hashMask | 1, Args: e}
}

// IsArray tells whether t is an array.
func IsArray(t Term) bool { return t.Sym == SymArray }

// arrayError stops the program with REM's message on the standard output.
func arrayError(msg string) {
	Out.WriteString(msg + "\n")
	Exit(1)
}

// array checks that t is an array (REM: `<op> error: symb = <code>`).
func array(t Term, op string) {
	if t.Sym != SymArray {
		arrayError(op + " error: symb = " + strconv.Itoa(int(Syms[t.Sym].Code)))
	}
}

// inRange checks that i is an index of the array t.
func inRange(t Term, i int32, op string) {
	if i < 0 || i >= t.Int {
		arrayError(op + " error: size = " + strconv.Itoa(int(t.Int)) + "\tn = " + strconv.Itoa(int(i)))
	}
}

// ArrayGet returns the element i of the array t (fun_201).
func ArrayGet(t Term, i int32) Term {
	array(t, "getArray")
	inRange(t, i, "getArray")
	return t.Args[i]
}

// ArraySet sets the element i of the array t to x and returns t
// (fun_202).
func ArraySet(t Term, i int32, x Term) Term {
	array(t, "setArray")
	inRange(t, i, "setArray")
	t.Args[i] = x
	return t
}

// ArrayLength is the size of the array t (fun_203).
func ArrayLength(t Term) int32 {
	array(t, "getLength")
	return t.Int
}

// ArrayEq is the equality of the terms of an array sort (== and the
// non-linear patterns there; [Eq] elsewhere, kept small enough to be
// inlined): two arrays are equal when they have the same size and equal
// elements (arrays compared the same way); other terms as [Eq].
func ArrayEq(a, b Term) bool {
	if a == b {
		return true
	}
	if a.Sym != SymArray || b.Sym != SymArray || a.Int != b.Int {
		return Eq(a, b)
	}
	for i := range a.Args {
		if !ArrayEq(a.Args[i], b.Args[i]) {
			return false
		}
	}
	return true
}

// arrayPrintMsg is REM's message (print_f_prefix, termCommon.c).
const arrayPrintMsg = "print_f_prefix Array not yet implemented\n"

// arrayPrintedStop stops the program after the printer w met an array:
// what w holds goes to out, REM's message to the standard output,
// status 1 (REM's printer).
func arrayPrintedStop(w *Writer, out io.Writer) {
	out.Write(w.Take())
	Out.WriteString(arrayPrintMsg)
	Exit(1)
}
