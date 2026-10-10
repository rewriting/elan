// Lexer of .ref files.
//
// Tokens: integers (a `-` glued to the digits gives a negative integer; a
// detached `-` is a punctuation token, since REM's readInt also accepts
// `- 1`), quoted names, words (RULE, nil, repeat*...), and the punctuation
// `: . , ( ) ;`. White space, /* ... */ and // ... comments are skipped (the
// export writes comments such as /*1106 R:selection*/ before the rules and
// /**/ in strategy calls).
//
// The input is bytes: names are kept as the raw bytes.

package ref

import (
	"bytes"
	"fmt"
	"strconv"
)

// TokKind is the kind of a token.
type TokKind uint8

const (
	// TokInt is an integer, possibly negative (in Token.Int).
	TokInt TokKind = iota
	// TokStr is a quoted name, without its quotes (in Token.Text).
	TokStr
	// TokWord is a word: letters, digits, _, optionally ending with *
	// (in Token.Text).
	TokWord
	// TokPunct is one of `: . , ( ) ;`, or a `-` not glued to its digits
	// (in Token.Text).
	TokPunct
)

// Token is a token and its position.
type Token struct {
	// Kind says which of Int and Text is used.
	Kind TokKind
	// Int is the value of a TokInt.
	Int int64
	// Text is the name, word or punctuation character.
	Text string
	// Line is the 1-based line of the first character.
	Line int
	// Col is the 1-based column of the first character.
	Col int
}

// String shows the token as in the file.
func (t Token) String() string {
	switch t.Kind {
	case TokInt:
		return strconv.FormatInt(t.Int, 10)
	case TokStr:
		return `"` + t.Text + `"`
	}
	return t.Text
}

func (t Token) same(u Token) bool {
	return t.Kind == u.Kind && t.Int == u.Int && t.Text == u.Text
}

// Error is a lexical or syntax error, with its position.
type Error struct {
	// Line is the 1-based line.
	Line int
	// Col is the 1-based column.
	Col int
	// Message says what is wrong.
	Message string
}

func (e *Error) Error() string { return fmt.Sprintf("%d:%d: %s", e.Line, e.Col, e.Message) }

// isSpace is Rust's u8::is_ascii_whitespace (no vertical tab).
func isSpace(b byte) bool {
	return b == ' ' || b == '\t' || b == '\n' || b == '\f' || b == '\r'
}

func isDigit(b byte) bool { return b >= '0' && b <= '9' }

func isAlpha(b byte) bool { return (b|0x20) >= 'a' && (b|0x20) <= 'z' }

// Tokenize splits src into tokens.
//
// A quoted name ends at the first `"` followed by `.`: the writer does not
// escape names, and a name is always followed by `.` (n:"name".), so this
// reads back names containing `"` or `\` (REM's Java lexer would treat `\`
// as an escape).
func Tokenize(src []byte) ([]Token, error) {
	var toks []Token
	i, line, col := 0, 1, 1
	n := len(src)
	step := func(j int) {
		for _, b := range src[i:j] {
			if b == '\n' {
				line++
				col = 1
			} else {
				col++
			}
		}
		i = j
	}
	at := func(k int) byte {
		if k < n {
			return src[k]
		}
		return 0
	}
	for i < n {
		b := src[i]
		l0, c0 := line, col
		fail := func(m string) ([]Token, error) {
			return nil, &Error{Line: l0, Col: c0, Message: m}
		}
		switch {
		case isSpace(b):
			step(i + 1)
		case b == '/' && at(i+1) == '*':
			p := bytes.Index(src[i+2:], []byte("*/"))
			if p < 0 {
				return fail("unterminated comment")
			}
			step(i + 2 + p + 2)
		case b == '/' && at(i+1) == '/':
			p := bytes.IndexByte(src[i:], '\n')
			if p < 0 {
				step(n)
			} else {
				step(i + p)
			}
		case b == '"':
			j := i + 1
			for {
				if j >= n {
					return fail("unterminated name")
				}
				if src[j] == '"' && at(j+1) == '.' {
					break
				}
				j++
			}
			toks = append(toks, Token{Kind: TokStr, Text: string(src[i+1 : j]), Line: l0, Col: c0})
			step(j + 1)
		case isDigit(b) || (b == '-' && i+1 < n && isDigit(src[i+1])):
			j := i + 1
			for j < n && isDigit(src[j]) {
				j++
			}
			text := string(src[i:j])
			v, err := strconv.ParseInt(text, 10, 64)
			if err != nil {
				return fail(fmt.Sprintf("bad integer %s: %v", text, err.(*strconv.NumError).Err))
			}
			toks = append(toks, Token{Kind: TokInt, Int: v, Line: l0, Col: c0})
			step(j)
		case isAlpha(b) || b == '_':
			j := i + 1
			for j < n && (isAlpha(src[j]) || isDigit(src[j]) || src[j] == '_') {
				j++
			}
			if j < n && src[j] == '*' {
				j++ // repeat*, iterate*
			}
			toks = append(toks, Token{Kind: TokWord, Text: string(src[i:j]), Line: l0, Col: c0})
			step(j)
		case bytes.IndexByte([]byte(":.,();-"), b) >= 0:
			toks = append(toks, Token{Kind: TokPunct, Text: string(b), Line: l0, Col: c0})
			step(i + 1)
		default:
			return fail(fmt.Sprintf("unexpected character %s", strconv.QuoteRune(rune(b))))
		}
	}
	return toks, nil
}
