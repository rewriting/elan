// Package load is the loader skeleton of the Go front end (S8a): the .lgi
// reader (ldmodgram with the actions of driver/ldmain.cc ldsemact that name,
// import and open things), the module lookup and import graph
// (load/msemact.cc readmodules, importmod_inf), each module read with
// modgram up to the end of its import section, its parameters becoming
// macros, then drained through the preprocessor: the module body is S8b's.
// The terms the C++ reads with Earley (spec parts, `start with`,
// `check with`) are consumed lexically with the C++ stop predicates.
// A port, the C++ names in comments.
package load

import (
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"

	"elan/front/diag"
	"elan/front/lex"
	"elan/front/lr"
	"elan/front/macro"
	"elan/front/tables"
)

// Options are the subset of elan's options of spec S8-12, and the front
// end's own: the evaluator of FOR EACH/IF and the trace.
type Options struct {
	ElanLib    string // $ELANLIB
	ElanLibOpt string // --elanlib
	SecondLib  string // -l/--secondlib, else $SECONDELANLIB
	Quote      bool   // --quote
	Batch      bool   // -b: no module tree
	Warnings   bool   // on, unless -w, -q, -b
	Out, Err   io.Writer
	Eval       macro.Evaluator
	// TraceTo receives the lexem trace (the harness's format), nil: none.
	TraceTo io.Writer
}

// ParseArgs reads elan's options (driver/ldmain.cc elan_main_body):
// --elanlib D, -l/--secondlib D, --quote/--noquote, -w, -q, -b,
// --export F/--cexport F (accepted, ignored in S8a), --MAXLENNTERM n
// (accepted); the rest are the files (prog.lgi [spec]).
func ParseArgs(args []string) (Options, []string, error) {
	o := Options{Warnings: true, ElanLib: os.Getenv("ELANLIB"), SecondLib: os.Getenv("SECONDELANLIB")}
	i := 0
	arg := func() (string, error) {
		if i+1 >= len(args) {
			return "", fmt.Errorf("option %s needs an argument", args[i])
		}
		i++
		return args[i], nil
	}
	for ; i < len(args) && strings.HasPrefix(args[i], "-"); i++ {
		var err error
		switch args[i] {
		case "-w", "--warningsoff", "-q", "--quiet":
			o.Warnings = false
		case "-b", "--batch":
			o.Batch, o.Warnings = true, false
		case "--quote":
			o.Quote = true
		case "--noquote":
			o.Quote = false
		case "--elanlib":
			o.ElanLibOpt, err = arg()
		case "-l", "--secondlib":
			o.SecondLib, err = arg()
		case "--export", "--cexport":
			_, err = arg()
		case "--MAXLENNTERM":
			var n string
			if n, err = arg(); err == nil {
				_, err = strconv.Atoi(n)
			}
		default:
			err = fmt.Errorf("unknown option '%s'", args[i])
		}
		if err != nil {
			return o, nil, err
		}
	}
	files := args[i:]
	if len(files) != 1 && len(files) != 2 {
		return o, nil, fmt.Errorf("usage: [options] prog.lgi [spec]")
	}
	return o, files, nil
}

// loader is the state of one load: the C++ globals it needs.
type loader struct {
	o      Options
	d      *diag.D
	c      *lex.Context
	mst    *macro.State
	mrwt   *tables.MiTab
	ltol   func(lex.Lexem) lex.Lexem
	typet  *tables.StringTab
	imp    *tables.StringTab // import
	globgr []bool            // importglobgr[i] != NULL
	stacki int
	frames []string // modframes[i].modname
	trace  macro.Tracer

	// module header state (ld.*)
	actimp        []string // ld.actimp
	actargmodname string
	actarglist    []string

	// .lgi state (ldmain.cc statics)
	modname      string // the .lgi name without its extension
	specname     string // argv[i+1]
	specstream   *lex.Stream
	actmn        []string
	axiomname    string
	oldaxiomname *string
	sortname     string
	sourcetypei  int
	qresulttypei int
}

// Load is elan_main up to the end of the .lgi parse: the status (0, or 1
// after a fatal error; 70 for an internal error of the skeleton).
func Load(o Options, lgi, spec string) int {
	d := &diag.D{Out: o.Out, Err: o.Err, Warnings: o.Warnings}
	return diag.Run(func() {
		lib, err := lex.NewLib(o.ElanLib, o.ElanLibOpt, o.SecondLib, o.Quote)
		if err != nil {
			d.Errs("front/load: ", err.Error(), "\n")
			panic(diag.Exit{Status: 2})
		}
		L := &loader{o: o, d: d, specname: spec}
		L.c = &lex.Context{D: d, Ident: tables.NewTabOfIdent(d), Lib: lib}
		L.mst = macro.NewState()
		L.mrwt = tables.NewMRWT(d)
		L.ltol = lr.LexToModLex(L.c, L.mrwt)
		L.typet = tables.NewTypeT(d)
		L.imp = tables.NewImport(d)
		L.globgr = make([]bool, tables.MaxNofImports)
		L.frames = []string{""}
		if o.TraceTo != nil {
			L.trace = macro.NewTraceWriter(o.TraceTo, L.c)
		}
		L.main(lgi)
	})
}

// internal is an error of the skeleton itself (not of the program).
func (L *loader) internal(format string, a ...any) {
	L.d.Errs(fmt.Sprintf("front/load: "+format+"\n", a...))
	panic(diag.Exit{Status: 70})
}

// main is elan_main_body from the .lgi name to the end of ldsyntan.
func (L *loader) main(lgi string) {
	// modname: after the last '/'; no extension: .lgi added
	name := lgi
	if k := strings.LastIndexByte(name, '/'); k >= 0 {
		name = name[k+1:]
	}
	ind := len(name) - 4
	if ind < 0 {
		ind = 0
	}
	modsource := name
	if ind >= len(name) || name[ind] != '.' {
		modsource = name + ".lgi"
		L.modname = name
	} else {
		L.modname = name[:ind]
	}
	L.imp.AddStr(modsource) // lgimoduli
	L.setstandards()
	L.crStandModules()
	ff := lex.New(L.c, lex.Open(L.c, modsource), true)
	if lr.Parse(lr.LdModGram, ff, ff.Ilex, L.ltol, func(n int, l lex.Lexem) int { return L.ldsemact(ff, n, l) }) != lr.Accepted {
		L.d.Fail()
	}
}

// setstandards inserts the words of ldmain.cc setstandards into tabofident
// (the sorts are typet's initial ones).
func (L *loader) setstandards() {
	for _, w := range []string{"if", "where", "end", "start", "query", "case", "then",
		"switch", "otherwise", "choose", "try", "result", "part", "spart", "END", "specification"} {
		L.c.Ident.AddStr(w)
	}
}

// crStandModules: the four built-in modules, already loaded.
func (L *loader) crStandModules() {
	for _, m := range []string{"anyInteger", "anyIdentifier", "anyString", "anyQuotedIdentifier"} {
		L.globgr[L.imp.AddStr(m)] = true
	}
}

// idLex is lexem::cridlex(const char*).
func (L *loader) idLex(s string) lex.Lexem { return lex.IdLex(L.c.Ident.AddStr(s)) }

// skipTerm is what an Earley parse consumes from f on a valid text
// (earley.cc 533-566): at least one lexem, then up to the first look-ahead
// on which stop holds.
func skipTerm(f lr.Source, stop func(lex.Lexem) bool) {
	for {
		f.Ilex()
		if stop(f.Fulex()) {
			return
		}
	}
}
