// Comparison of .ref files modulo white space and comments, for the
// round-trip tests. Deliberately independent of the lexer: a token is a
// quoted name (up to the `"` followed by `.`, see Tokenize), a run of
// letters, digits, _, * and -, or any other single character.

package ref

import (
	"bytes"
	"fmt"
	"strings"
)

// CompareTokens returns the tokens of src, white space and /* */, //
// comments removed.
func CompareTokens(src []byte) [][]byte {
	word := func(b byte) bool {
		return isAlpha(b) || isDigit(b) || b == '_' || b == '*' || b == '-'
	}
	var v [][]byte
	i := 0
	for i < len(src) {
		b := src[i]
		switch {
		case isSpace(b):
			i++
		case bytes.HasPrefix(src[i:], []byte("/*")):
			if p := bytes.Index(src[i+2:], []byte("*/")); p >= 0 {
				i = i + 2 + p + 2
			} else {
				i = len(src)
			}
		case bytes.HasPrefix(src[i:], []byte("//")):
			for i < len(src) && src[i] != '\n' {
				i++
			}
		case b == '"':
			j := i + 1
			for j < len(src) && !(src[j] == '"' && j+1 < len(src) && src[j+1] == '.') {
				j++
			}
			end := min(j+1, len(src))
			v = append(v, src[i:end])
			i = end
		case word(b):
			j := i
			for j < len(src) && word(src[j]) {
				j++
			}
			v = append(v, src[i:j])
			i = j
		default:
			v = append(v, src[i:i+1])
			i++
		}
	}
	return v
}

// FirstDifference returns "" when a and b have the same tokens, else a
// description of the first difference (token position and a few tokens of
// context).
func FirstDifference(a, b []byte) string {
	ta, tb := CompareTokens(a), CompareTokens(b)
	n := 0
	for n < len(ta) && n < len(tb) && bytes.Equal(ta[n], tb[n]) {
		n++
	}
	if n == len(ta) && n == len(tb) {
		return ""
	}
	ctx := func(t [][]byte) string {
		lo, hi := max(n-8, 0), min(n+8, len(t))
		s := make([]string, 0, hi-lo)
		for _, x := range t[lo:hi] {
			s = append(s, strings.ToValidUTF8(string(x), "\uFFFD"))
		}
		return strings.Join(s, " ")
	}
	return fmt.Sprintf("token %d of %d/%d:\n  original: %s\n  printed:  %s",
		n, len(ta), len(tb), ctx(ta), ctx(tb))
}
