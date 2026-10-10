package ir

import (
	"strconv"
	"strings"
)

// Go identifiers for ELAN operators (as elanc-rs's names.rs).
//
// ELAN names may contain any character (`list[int]`, `@ + @`); the
// generated names keep the letters and digits and name the other characters
// (`+` is plus), so that the generated code stays readable. Function names
// carry the symbol code (plus_320), so they never are Go keywords or
// predeclared names; uniqueness is ensured by [unique].

// charName is a name for a character of an operator's syntax; "" for the
// parentheses and commas of the prefix syntax.
func charName(c byte) string {
	switch c {
	case '+':
		return "plus"
	case '-':
		return "minus"
	case '*':
		return "star"
	case '/':
		return "slash"
	case '%':
		return "percent"
	case '&':
		return "amp"
	case '|':
		return "bar"
	case '.':
		return "dot"
	case '<':
		return "lt"
	case '>':
		return "gt"
	case '=':
		return "eq"
	case '!':
		return "bang"
	case '~':
		return "tilde"
	case '^':
		return "caret"
	case ':':
		return "colon"
	case ';':
		return "semi"
	case '#':
		return "hash"
	case '$':
		return "dollar"
	case '?':
		return "q"
	case '\'':
		return "quote"
	case '"':
		return "dquote"
	case '\\':
		return "bslash"
	case '`':
		return "bquote"
	case '[':
		return "lb"
	case ']':
		return "rb"
	case '{':
		return "lc"
	case '}':
		return "rc"
	case '@':
		return "at"
	case '(', ')', ',':
		return ""
	}
	return "c"
}

func isAlnum(c byte) bool {
	return 'a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || '0' <= c && c <= '9'
}

// sanitise keeps the ASCII letters and digits of s, other bytes become _.
func sanitise(s string) string {
	b := []byte(s)
	for i, c := range b {
		if !isAlnum(c) {
			b[i] = '_'
		}
	}
	return string(b)
}

// OpBaseName is the base name of an operator from the texts of its syntax
// tokens (identifiers and characters, arguments left out): fact(@) gives
// fact, @ + @ gives plus, @ (an injection) gives inj.
func OpBaseName(tokens []string) string {
	var parts []string
	for _, t := range tokens {
		if len(t) == 1 && !isAlnum(t[0]) {
			if n := charName(t[0]); n != "" {
				parts = append(parts, n)
			}
			continue
		}
		parts = append(parts, sanitise(t))
	}
	s := strings.Trim(strings.Join(parts, "_"), "_")
	switch {
	case s == "":
		return "inj"
	case s[0] >= '0' && s[0] <= '9':
		return "n" + s
	}
	return s
}

// unique returns name, or name_2, name_3... if already used; it records
// the result.
func unique(used map[string]bool, name string) string {
	n := name
	for i := 2; used[n]; i++ {
		n = name + "_" + strconv.Itoa(i)
	}
	used[n] = true
	return n
}
