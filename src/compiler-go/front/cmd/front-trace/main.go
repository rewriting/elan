// Command front-trace prints the lexem traces of the Go front end in the
// format of the C++ harness elan-lextrace (tests/front/README.md):
//
//	front-trace lex FILE
//	front-trace load [--tree] [--trace FILE] [--refuse] [elan options] prog.lgi [spec]
//
// `lex` reads FILE as elan-lextrace does: a .lgi or .spc (any file but a
// .eln) through the plain lexer, '_' part of identifiers, one line per lexem,
// `eos` last, "@inline <chars>" after a lexem during whose reading an inline
// was read; warnings on (elan's default). The library lookup needs ELANLIB
// (and SECONDELANLIB, default ..): the Go front end has no compiled-in prefix.
// A .eln is read through the preprocessor (macro.Lex) with the standalone
// evaluator macro.Refuse: a FOR EACH or IF condition is refused where the
// standalone C++ would need a grammar.
//
// `load` is the loader skeleton (front/load) on elan's options and files:
// --tree prints the module tree (`handling module`/`end of`, as elan
// without -b; -b still suppresses it), --trace FILE writes the lexem trace
// of every module (the harness's load mode), --refuse refuses FOR EACH/IF
// (macro.Refuse) instead of reading them without evaluation (macro.Mark).
package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"

	"elan/front/diag"
	"elan/front/lex"
	"elan/front/load"
	"elan/front/macro"
	"elan/front/tables"
)

func main() {
	os.Exit(run(os.Args[1:]))
}

func run(args []string) int {
	if len(args) == 2 && args[0] == "lex" {
		return lexFile(args[1])
	}
	if len(args) >= 1 && args[0] == "load" {
		return loadProgram(args[1:])
	}
	fmt.Fprintln(os.Stderr, "usage: front-trace lex FILE\n       front-trace load [--tree] [--trace FILE] [--refuse] [elan options] prog.lgi [spec]")
	return 2
}

func lexFile(name string) int {
	lib, err := lex.NewLib(os.Getenv("ELANLIB"), "", os.Getenv("SECONDELANLIB"), false)
	if err != nil {
		fmt.Fprintln(os.Stderr, "front-trace:", err)
		return 2
	}
	out := bufio.NewWriter(os.Stdout)
	defer out.Flush()
	d := &diag.D{Out: out, Err: os.Stderr, Warnings: true}
	c := &lex.Context{D: d, Ident: tables.NewTabOfIdent(d), Lib: lib}
	return diag.Run(func() {
		if strings.HasSuffix(name, ".eln") {
			macro.Lex(c, lex.Open(c, name), name, macro.Refuse{}, out)
			return
		}
		s := lex.New(c, lex.Open(c, name), true)
		for {
			l := s.Ilex()
			lex.TraceLexem(out, c, l)
			if in := s.LastInline(); in != nil {
				lex.TraceInline(out, in)
			}
			if l.IsEOS() {
				return
			}
		}
	})
}

func loadProgram(args []string) int {
	tree, refuse, trace := false, false, ""
	for len(args) > 0 {
		switch args[0] {
		case "--tree":
			tree = true
		case "--refuse":
			refuse = true
		case "--trace":
			if len(args) < 2 {
				fmt.Fprintln(os.Stderr, "front-trace: --trace needs a file")
				return 2
			}
			trace = args[1]
			args = args[1:]
		default:
			goto opts
		}
		args = args[1:]
	}
opts:
	o, files, err := load.ParseArgs(args)
	if err != nil {
		fmt.Fprintln(os.Stderr, "front-trace:", err)
		return 2
	}
	if !tree {
		o.Batch = true
	}
	out := bufio.NewWriter(os.Stdout)
	defer out.Flush()
	o.Out, o.Err = out, os.Stderr
	o.Eval = macro.Mark{}
	if refuse {
		o.Eval = macro.Refuse{}
	}
	if trace != "" {
		f, err := os.Create(trace)
		if err != nil {
			fmt.Fprintln(os.Stderr, "front-trace:", err)
			return 2
		}
		defer f.Close()
		w := bufio.NewWriter(f)
		defer w.Flush()
		o.TraceTo = w
	}
	spec := ""
	if len(files) == 2 {
		spec = files[1]
	}
	return load.Load(o, files[0], spec)
}
