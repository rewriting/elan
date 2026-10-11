// Command gen converts the LR(1) tables that MACC generates for the
// interpreter (mparser.cc, ldparser.cc, ppexparser.cc in the build's
// src/interpreter/generated) and the reserved-word preload (tabofident.cc)
// into Go: front/lr/tables_gen.go and front/tables/keywords_gen.go.
//
//	go run ./front/lr/gen -in BUILD/src/interpreter/generated -out .   (from src/compiler-go)
//
// Every construct of the C files that it does not recognise is an error
// naming the file and line (spec S8, decision S8-4).
package main

import (
	"flag"
	"fmt"
	"go/format"
	"os"
	"path/filepath"
	"regexp"
	"strconv"
	"strings"
)

// The parsers, in the order of the generated file.
var parsers = []struct{ file, name, goName string }{
	{"mparser.cc", "modgram", "ModGram"},
	{"ldparser.cc", "ldmodgram", "LdModGram"},
	{"ppexparser.cc", "ppexgram", "PpexGram"},
}

const (
	tablesOut   = "front/lr/tables_gen.go"
	keywordsOut = "front/tables/keywords_gen.go"
)

type action struct {
	sym             int32
	symText         string // the C text of the symbol, for the comment
	kind            string // Go name of the kind
	num, left, leng int32
}

type state struct{ pr, npr, ak, nak int }

type table struct {
	file, name, goName, sttab string
	actions                   []action // with the sentinel
	gotos                     [][2]int32
	states                    []state
}

type srcError struct {
	file string
	line int
	msg  string
}

func (e *srcError) Error() string { return fmt.Sprintf("%s:%d: %s", e.file, e.line, e.msg) }

func lineOf(src []byte, off int) int { return 1 + strings.Count(string(src[:off]), "\n") }

var (
	reDefine = regexp.MustCompile(`(?m)^#define[ \t]+(\w+)[ \t]+(.*?)[ \t]*$`)
	reRW     = regexp.MustCompile(`^\(-(\d+)\+BOFIDENT\)$`)
	reInt    = regexp.MustCompile(`^-?\d+$`)
	reSep    = regexp.MustCompile(`^[\s,]*`)
	reAppr   = regexp.MustCompile(`^\{\s*(-'(?:\\.|[^'\\])'|-?\d+|[A-Za-z_]\w*)\s*,\s*([A-Za-z_]\w*)\s*,\s*\{\s*(-?\d+)\s*(?:,\s*(-?\d+)\s*,\s*(-?\d+)\s*)?\}\s*\}`)
	reApak   = regexp.MustCompile(`^\{\s*(-?\d+)\s*,\s*(-?\d+)\s*\}`)
	reSttab  = regexp.MustCompile(`^\{\s*&appr\[(\d+)\]\s*,\s*(\d+)\s*,\s*&apak\[(\d+)\]\s*,\s*(\d+)\s*\}`)
)

var kinds = map[string]string{"reduction": "Reduction", "accept": "Accept", "shift": "Shift", "srerr": "SRErr", "rrerr": "RRErr"}
var kindValues = map[string]string{"reduction": "0", "accept": "1", "shift": "2", "srerr": "3", "rrerr": "4"}

// charLit is the value of a C character literal body (between the quotes).
func charLit(s string) (int32, bool) {
	if len(s) == 1 && s != `\` && s != "'" {
		return int32(s[0]), true
	}
	switch s {
	case `\'`:
		return '\'', true
	case `\"`:
		return '"', true
	case `\\`:
		return '\\', true
	case `\n`:
		return '\n', true
	case `\t`:
		return '\t', true
	}
	return 0, false
}

// arrayBody finds `NAME[]={ ... };` and returns the offset of its body and
// the body (without the outer braces).
func arrayBody(file string, src []byte, name string) (int, string, error) {
	re := regexp.MustCompile(regexp.QuoteMeta(name) + `\[\]\s*=\s*\{`)
	loc := re.FindIndex(src)
	if loc == nil {
		return 0, "", &srcError{file, 1, "no array " + name}
	}
	end := strings.Index(string(src[loc[1]:]), "};")
	if end < 0 {
		return 0, "", &srcError{file, lineOf(src, loc[0]), "array " + name + " not closed by };"}
	}
	return loc[1], string(src[loc[1] : loc[1]+end]), nil
}

// entries splits an array body into entries matched by re; anything else is
// an error at its line.
func entries(file string, src []byte, off int, body string, re *regexp.Regexp, what string) ([][]string, error) {
	var out [][]string
	pos := 0
	for {
		pos += len(reSep.FindString(body[pos:]))
		if pos >= len(body) {
			return out, nil
		}
		m := re.FindStringSubmatch(body[pos:])
		if m == nil {
			frag := body[pos:]
			if i := strings.IndexAny(frag, "}\n"); i >= 0 {
				frag = frag[:i+1]
			}
			return nil, &srcError{file, lineOf(src, off+pos), fmt.Sprintf("unrecognised %s entry %q", what, strings.TrimSpace(frag))}
		}
		out = append(out, m)
		pos += len(m[0])
	}
}

func atoi32(s string) int32 {
	n, _ := strconv.ParseInt(s, 10, 32)
	return int32(n)
}

func parseParserFile(path, name, goName string) (*table, error) {
	src, err := os.ReadFile(path)
	if err != nil {
		return nil, err
	}
	return parseParser(filepath.Base(path), src, name, goName)
}

// parseParser reads one generated parser source.
func parseParser(file string, src []byte, name, goName string) (*table, error) {
	t := &table{file: file, name: name, goName: goName}
	syms := map[string]int32{}
	for _, m := range reDefine.FindAllSubmatchIndex(src, -1) {
		key, val := string(src[m[2]:m[3]]), string(src[m[4]:m[5]])
		switch {
		case key == "STTAB":
			t.sttab = val
		case strings.HasPrefix(key, "RW"):
			rw := reRW.FindStringSubmatch(val)
			if rw == nil {
				return nil, &srcError{file, lineOf(src, m[0]), "unrecognised reserved-word define " + key + " " + val}
			}
			syms[key] = -atoi32(rw[1]) - 260 // BOFIDENT
		case key == "EOFINPUT" || key == "NUMBER" || key == "IDENTIFIER" || key == "DEFAULT":
			if !reInt.MatchString(val) {
				return nil, &srcError{file, lineOf(src, m[0]), "unrecognised define " + key + " " + val}
			}
			syms[key] = atoi32(val)
		case kinds[key] != "":
			if val != kindValues[key] {
				return nil, &srcError{file, lineOf(src, m[0]), "action " + key + " is " + val}
			}
		}
	}
	if t.sttab == "" {
		return nil, &srcError{file, 1, "no #define STTAB"}
	}
	for _, k := range []string{"EOFINPUT", "NUMBER", "IDENTIFIER", "DEFAULT"} {
		if _, ok := syms[k]; !ok {
			return nil, &srcError{file, 1, "no #define " + k}
		}
	}

	off, body, err := arrayBody(file, src, "appr")
	if err != nil {
		return nil, err
	}
	ms, err := entries(file, src, off, body, reAppr, "appr")
	if err != nil {
		return nil, err
	}
	pos := 0
	for _, m := range ms {
		line := lineOf(src, off+strings.Index(body[pos:], m[0])+pos)
		pos += strings.Index(body[pos:], m[0]) + len(m[0])
		a := action{symText: m[1], num: atoi32(m[3])}
		switch {
		case strings.HasPrefix(m[1], "-'"):
			c, ok := charLit(m[1][2 : len(m[1])-1])
			if !ok {
				return nil, &srcError{file, line, "unrecognised character literal " + m[1]}
			}
			a.sym = -c
		case reInt.MatchString(m[1]):
			a.sym, a.symText = atoi32(m[1]), ""
		default:
			v, ok := syms[m[1]]
			if !ok {
				return nil, &srcError{file, line, "unknown symbol " + m[1]}
			}
			a.sym = v
		}
		if a.kind = kinds[m[2]]; a.kind == "" {
			return nil, &srcError{file, line, "unknown action " + m[2]}
		}
		if m[4] != "" {
			a.left, a.leng = atoi32(m[4]), atoi32(m[5])
		}
		t.actions = append(t.actions, a)
	}
	if n := len(t.actions); n == 0 || t.actions[n-1].sym != 0 || t.actions[n-1].kind != "Shift" {
		return nil, &srcError{file, lineOf(src, off), "appr does not end with the sentinel {0,shift,{0}}"}
	}

	off, body, err = arrayBody(file, src, "apak")
	if err != nil {
		return nil, err
	}
	if ms, err = entries(file, src, off, body, reApak, "apak"); err != nil {
		return nil, err
	}
	for _, m := range ms {
		t.gotos = append(t.gotos, [2]int32{atoi32(m[1]), atoi32(m[2])})
	}
	if n := len(t.gotos); n == 0 || t.gotos[n-1] != [2]int32{0, 0} {
		return nil, &srcError{file, lineOf(src, off), "apak does not end with the sentinel {0,0}"}
	}

	off, body, err = arrayBody(file, src, "STTAB")
	if err != nil {
		return nil, err
	}
	if ms, err = entries(file, src, off, body, reSttab, "STTAB"); err != nil {
		return nil, err
	}
	for i, m := range ms {
		s := state{int(atoi32(m[1])), int(atoi32(m[2])), int(atoi32(m[3])), int(atoi32(m[4]))}
		if s.pr+s.npr > len(t.actions)-1 || s.ak+s.nak > len(t.gotos)-1 { // sentinels excluded
			return nil, &srcError{file, lineOf(src, off), fmt.Sprintf("state %d beyond the arrays", i)}
		}
		t.states = append(t.states, s)
	}
	return t, nil
}

const header = "// Code generated by front/lr/gen from %s (MACC); DO NOT EDIT.\n\n"

// genTables writes tables_gen.go for the given tables.
func genTables(tabs []*table) ([]byte, error) {
	var b strings.Builder
	var files []string
	for _, t := range tabs {
		files = append(files, t.file)
	}
	fmt.Fprintf(&b, header, strings.Join(files, ", "))
	b.WriteString("package lr\n")
	for _, t := range tabs {
		na, ng := len(t.actions)-1, len(t.gotos)-1
		fmt.Fprintf(&b, "\n// %s is the LR(1) table of %s (%s in %s): %d states,\n// %d actions, %d gotos (sentinels not counted).\n",
			t.goName, t.name, t.sttab, t.file, len(t.states), na, ng)
		fmt.Fprintf(&b, "var %s = &Table{Name: %q, States: %sStates[:]}\n", t.goName, t.name, t.name)
		fmt.Fprintf(&b, "\nvar %sActions = [...]Action{\n", t.name)
		for i, a := range t.actions {
			comment := a.symText
			if i == len(t.actions)-1 {
				comment = "sentinel"
			}
			fmt.Fprintf(&b, "\t{%d, %s, %d, %d, %d},", a.sym, a.kind, a.num, a.left, a.leng)
			if comment != "" {
				b.WriteString(" // " + comment)
			}
			b.WriteString("\n")
		}
		fmt.Fprintf(&b, "}\n\nvar %sGotos = [...]Goto{\n", t.name)
		for i, g := range t.gotos {
			fmt.Fprintf(&b, "\t{%d, %d},", g[0], g[1])
			if i == len(t.gotos)-1 {
				b.WriteString(" // sentinel")
			}
			b.WriteString("\n")
		}
		fmt.Fprintf(&b, "}\n\nvar %sStates = [...]State{\n", t.name)
		for i, s := range t.states {
			fmt.Fprintf(&b, "\t{%sActions[%d:%d], %sGotos[%d:%d]}, // %d\n", t.name, s.pr, s.pr+s.npr, t.name, s.ak, s.ak+s.nak, i)
		}
		b.WriteString("}\n")
	}
	return format.Source([]byte(b.String()))
}

var (
	reTabOfIdent = regexp.MustCompile(`(?s)stringtab\s+tabofident\s*\(\s*(\d+)\s*,(.*?)\bNULL\s*\)\s*;`)
	reMRWT       = regexp.MustCompile(`(?s)mitab\s+mrwt\s*\(\s*MRWTSIZE\s*,(.*?)\bNOINT\s*\)\s*;`)
	reStr        = regexp.MustCompile(`^"([A-Za-z0-9_]*)"$`)
)

// genKeywords reads tabofident.cc and writes keywords_gen.go.
func genKeywords(file string, src []byte) ([]byte, error) {
	m := reTabOfIdent.FindSubmatchIndex(src)
	if m == nil {
		return nil, &srcError{file, 1, "no `stringtab tabofident(N, ..., NULL);`"}
	}
	if string(src[m[2]:m[3]]) != "3000" {
		return nil, &srcError{file, lineOf(src, m[0]), "tabofident size " + string(src[m[2]:m[3]])}
	}
	var words []string
	for _, w := range strings.Split(string(src[m[4]:m[5]]), ",") {
		w = strings.TrimSpace(w)
		if w == "" {
			continue
		}
		s := reStr.FindStringSubmatch(w)
		if s == nil {
			return nil, &srcError{file, lineOf(src, m[4]), "unrecognised reserved word " + w}
		}
		words = append(words, s[1])
	}
	r := reMRWT.FindSubmatchIndex(src)
	if r == nil {
		return nil, &srcError{file, 1, "no `mitab mrwt(MRWTSIZE, ..., NOINT);`"}
	}
	var nums []string
	for _, w := range strings.Split(string(src[r[2]:r[3]]), ",") {
		w = strings.TrimSpace(w)
		if w == "" {
			continue
		}
		if !reInt.MatchString(w) || strings.HasPrefix(w, "-") {
			return nil, &srcError{file, lineOf(src, r[2]), "unrecognised mrwt value " + w}
		}
		nums = append(nums, w)
	}
	var b strings.Builder
	fmt.Fprintf(&b, header, file)
	b.WriteString("package tables\n\n")
	fmt.Fprintf(&b, "// reservedWords are the reserved words of ldmodgram, modgram and ppexgram\n// (%d, in this order, some twice), preloaded into tabofident.\nvar reservedWords = []string{\n", len(words))
	for _, w := range words {
		fmt.Fprintf(&b, "\t%q,\n", w)
	}
	fmt.Fprintf(&b, "}\n\n// mrwtInit are their positions in tabofident, the initial contents of\n// mrwt (%d; the C list ends with NOINT).\nvar mrwtInit = []int{\n", len(nums))
	for i := 0; i < len(nums); i += 10 {
		j := min(i+10, len(nums))
		fmt.Fprintf(&b, "\t%s,\n", strings.Join(nums[i:j], ", "))
	}
	b.WriteString("}\n")
	return format.Source([]byte(b.String()))
}

// generate reads the generated directory and returns the two Go files,
// keyed by their path relative to src/compiler-go.
func generate(dir string) (map[string][]byte, error) {
	var tabs []*table
	for _, p := range parsers {
		t, err := parseParserFile(filepath.Join(dir, p.file), p.name, p.goName)
		if err != nil {
			return nil, err
		}
		tabs = append(tabs, t)
	}
	tg, err := genTables(tabs)
	if err != nil {
		return nil, err
	}
	src, err := os.ReadFile(filepath.Join(dir, "tabofident.cc"))
	if err != nil {
		return nil, err
	}
	kg, err := genKeywords("tabofident.cc", src)
	if err != nil {
		return nil, err
	}
	return map[string][]byte{tablesOut: tg, keywordsOut: kg}, nil
}

func main() {
	in := flag.String("in", "", "the build's src/interpreter/generated directory")
	out := flag.String("out", ".", "src/compiler-go")
	flag.Parse()
	if *in == "" || flag.NArg() != 0 {
		fmt.Fprintln(os.Stderr, "usage: gen -in BUILD/src/interpreter/generated [-out src/compiler-go]")
		os.Exit(2)
	}
	files, err := generate(*in)
	if err != nil {
		fmt.Fprintln(os.Stderr, "gen:", err)
		os.Exit(1)
	}
	for rel, b := range files {
		if err := os.WriteFile(filepath.Join(*out, rel), b, 0o644); err != nil {
			fmt.Fprintln(os.Stderr, "gen:", err)
			os.Exit(1)
		}
	}
}
