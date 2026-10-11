package elanrt

import (
	"os"
	"strconv"
	"testing"
)

// Arrays (S7e task 7) as REM's term_newArray, term_getArray,
// term_setArray, term_getLength: mutable, outside the unique table (two
// new are two arrays), REM's errors on stdout with status 1.

func TestArrays(t *testing.T) {
	a, b := Mk0(symO), Mk1(symS, Mk0(symO))
	x := NewArray(3, a)
	if !IsArray(x) || ArrayLength(x) != 3 {
		t.Fatalf("new: %v %d", IsArray(x), ArrayLength(x))
	}
	for i := range int32(3) {
		if ArrayGet(x, i) != a {
			t.Fatalf("get %d", i)
		}
	}
	if r := ArraySet(x, 1, b); r != x {
		t.Fatal("set returns its array")
	}
	if ArrayGet(x, 1) != b || ArrayGet(x, 0) != a {
		t.Fatal("set mutates the array")
	}
	// two new are two arrays
	y := NewArray(3, a)
	if x == y {
		t.Fatal("new shares")
	}
	// a node holding an array: the same array, the same node; it sees the
	// later mutations
	h1, h2 := Mk1(symP, x), Mk1(symP, x)
	if h1 != h2 || Mk1(symP, y) == h1 {
		t.Fatal("sharing of a node holding an array")
	}
	ArraySet(x, 2, b)
	if ArrayGet(h1.Args[0], 2) != b {
		t.Fatal("mutation seen through the node")
	}
	// equality of an array sort: the same array, or the same contents (as
	// the interpreter's terms; REM: always true); Eq: identity
	if !ArrayEq(x, x) || ArrayEq(x, y) {
		t.Fatal("ArrayEq")
	}
	ArraySet(y, 1, b)
	ArraySet(y, 2, b)
	if !ArrayEq(x, y) || ArrayEq(x, NewArray(2, a)) || Eq(x, y) {
		t.Fatal("ArrayEq of contents")
	}
	// arrays of arrays
	if !ArrayEq(NewArray(1, x), NewArray(1, y)) || ArrayEq(NewArray(1, x), NewArray(1, NewArray(3, a))) {
		t.Fatal("ArrayEq of arrays of arrays")
	}
}

func init() {
	helpers["array"] = func() {
		Main("Nat", false, func(s *Session, _ Term) {
			x := NewArray(2, Mk0(symO))
			switch os.Getenv("ELANRT_ARRAY") {
			case "get":
				ArrayGet(x, 5)
			case "set":
				ArraySet(x, -1, x)
			case "stuck":
				ArrayGet(Mk0(symO), 0)
			case "length":
				ArrayLength(Mk0(symO))
			case "print":
				s.Result(Mk1(symS, x))
			case "printed":
				s.Result(ArrayGet(x, 1))
			}
		})
	}
}

// REM's errors: on the standard output, status 1.
func TestArrayErrors(t *testing.T) {
	code := Syms[symO].Code
	for _, c := range []struct{ arg, out string }{
		{"get", "getArray error: size = 2\tn = 5\n"},
		{"set", "setArray error: size = 2\tn = -1\n"},
		{"stuck", "getArray error: symb = " + strconv.Itoa(int(code)) + "\n"},
		{"length", "getLength error: symb = " + strconv.Itoa(int(code)) + "\n"},
		// an array printed: REM's printer stops there
		{"print", "\nresult = s(print_f_prefix Array not yet implemented\n"},
	} {
		out, errs, st := runHelper(t, "array", []string{"ELANRT_ARRAY=" + c.arg}, "-noInput", "-quiet")
		if st != 1 || out != c.out || errs != "" {
			t.Errorf("%s: status %d, out %q, err %q", c.arg, st, out, errs)
		}
	}
	out, _, st := runHelper(t, "array", []string{"ELANRT_ARRAY=printed"}, "-noInput", "-quiet")
	if st != 0 || out != "\nresult = o\n\nrewrite_step = 0\n" {
		t.Errorf("printed: status %d, out %q", st, out)
	}
}
