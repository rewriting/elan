package elanrt

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"runtime"
	"runtime/debug"
	"runtime/pprof"
	"strconv"
	"strings"
	"sync"
	"time"
)

// The main program of a generated program, with the command line and the
// output of the compiled programs (src/compiler/runtime/main_skeleton.c,
// elanc-rs elan-runtime/src/driver.rs):
//
//	$ ./a.out -noInput -quiet
//
//	result = 4.7.9.2.6.1.3.5.8.10.nil
//
//	rewrite_step = 3426635
//
// Each result is "\nresult = " term "\n"; at the end "\nrewrite_step =
// N\n", then, without -quiet, "total time    = S sec" and "average speed =
// R rwr/sec" (wall-clock time; C used the user time). Options are read
// while the arguments start with -; unknown ones are ignored, as in C;
// -help/-h prints the options and exits with 1.
//
// Exit statuses: 0 success, 1 run-time error ([Fatal], -help, a panic of
// the program), 2 construct not supported ([Unsupported]), 3 recursion too
// deep ([TooDeep]). Every exit writes the buffered output first ([Exit]).

// Steps is the number of rewrite steps (rule applications).
var Steps int64

// Step counts one rewrite step.
func Step() { Steps++ }

// Options are the command-line options of a generated program.
type Options struct {
	NoInput bool // -noInput: no query is read, the start term of the .lgi is used
	// Other is an option asking for another query mode or sort, which
	// elanc-go does not support (-REFInput, -commandLine, -sort=,
	// -strategy=; the last query mode given counts, as in C)
	Other    string
	Quiet    bool     // -quiet: no timing lines
	NoOutput bool     // -noOutput: results are not printed
	Help     bool     // -help, -h
	Rest     []string // the arguments after the options
}

// ParseOptions parses the arguments (without the program name).
func ParseOptions(args []string) Options {
	var o Options
	i := 0
	mode, eval := "", ""
	for ; i < len(args) && strings.HasPrefix(args[i], "-"); i++ {
		switch args[i] {
		case "-noInput":
			mode = args[i]
		case "-REFInput", "-commandLine":
			mode = args[i]
		case "-sort=", "-strategy=":
			eval = args[i]
			i++
		case "-quiet":
			o.Quiet = true
		case "-noOutput":
			o.NoOutput = true
		case "-help", "-h":
			o.Help = true
		case "-trace", "-filequery":
			i++ // options taking a value in the C program
		}
	}
	o.NoInput = mode == "-noInput"
	if !o.NoInput {
		o.Other = mode
		if eval != "" {
			o.Other = eval
		}
	}
	if i < len(args) {
		o.Rest = args[i:]
	}
	return o
}

// Help is the text printed by -help.
const Help = "  Options:\n    -noInput\n    -noOutput\n    -quiet\n    -help\n"

// Out is the program's buffered standard output. Write to it only from the
// program goroutine; [Exit] flushes it.
var Out = bufio.NewWriterSize(os.Stdout, 1<<16)

// FlushOutput writes what Out holds to the standard output (and what the
// files opened by the program hold, as C's exit).
func FlushOutput() {
	flushFiles()
	Out.Flush()
}

// Exit flushes the output and ends the program with the status code.
func Exit(code int) {
	FlushOutput()
	endProfiles()
	os.Exit(code)
}

// profiles ends the profiles of the run (ELAN_CPUPROFILE, ELAN_MEMPROFILE),
// set by [Main]: every end of the program writes them complete (a normal
// end, [Exit], [Fatal], [Unsupported], [TooDeep]).
var (
	profiles     = func() {}
	profilesOnce sync.Once
)

// endProfiles writes the profiles of the run, once.
func endProfiles() { profilesOnce.Do(profiles) }

// Fatal is a run-time error of the program (the interpreter's interr()):
// the output so far, then the message on the standard error, status 1.
func Fatal(msg string) {
	FlushOutput()
	fmt.Fprintln(os.Stderr, msg)
	endProfiles()
	os.Exit(1)
}

// Warning writes a warning of the interpreter on the standard error, as
// is (the output so far first).
func Warning(msg string) {
	FlushOutput()
	fmt.Fprint(os.Stderr, msg)
}

// Unsupported stops a program that meets a construct the compiler
// supports only partly: the message on the standard error, status 2 (as a
// refusal of the compiler).
func Unsupported(msg string) {
	FlushOutput()
	fmt.Fprintln(os.Stderr, "elanrt: not supported yet: "+msg)
	endProfiles()
	os.Exit(2)
}

// Session prints the results and the statistics of a run.
type Session struct {
	Options Options
	W       Writer // the printer, kept for the whole run (spacing flag)
	out     io.Writer
	start   time.Time
	results int
}

// NewSession starts a session writing to out ([Out] in a program).
func NewSession(o Options, out io.Writer) *Session {
	s := &Session{Options: o, out: out, start: time.Now()}
	s.W.onArray = func(w *Writer) { arrayPrintedStop(w, s.out) }
	printer = &s.W // the terms written by the program (WriteTerm) share its spacing state
	return s
}

// Results is the number of results so far.
func (s *Session) Results() int { return s.results }

// Result prints one result: "\nresult = <term>\n".
func (s *Session) Result(t Term) { s.result(func(w *Writer) { w.Term(t) }) }

// ResultInt prints an int result.
func (s *Session) ResultInt(v int32) { s.result(func(w *Writer) { w.Int(v) }) }

// ResultBool prints a bool result.
func (s *Session) ResultBool(b bool) { s.result(func(w *Writer) { w.Bool(b) }) }

// ResultStr prints a string result.
func (s *Session) ResultStr(v string) { s.result(func(w *Writer) { w.Str(v) }) }

// ResultIdent prints an identifier result (its index).
func (s *Session) ResultIdent(i int32) { s.result(func(w *Writer) { w.IdentValue(i) }) }

func (s *Session) result(print func(w *Writer)) {
	s.results++
	if s.Options.NoOutput {
		return
	}
	s.W.Raw("\nresult = ")
	print(&s.W)
	s.W.Raw("\n")
	s.out.Write(s.W.Take())
}

// Finish prints the statistics.
func (s *Session) Finish() {
	fmt.Fprintf(s.out, "\nrewrite_step = %d\n", Steps)
	if s.Options.Quiet {
		return
	}
	secs := time.Since(s.start).Seconds()
	fmt.Fprintf(s.out, "total time    = %.3f sec\n", secs)
	if secs > 0 {
		fmt.Fprintf(s.out, "average speed = %d rwr/sec\n", int64(float64(Steps)/secs))
	}
}

// MaxStack is the maximum stack size [Main] sets: Go caps it at twice its
// default, and stacks are powers of two, so 1 GiB is usable.
const MaxStack = 2_000_000_000

// GCPercent is the GC setting of [Main] when GOGC is not set: the live
// heap of a program with weak unique tables is small, the default (100)
// runs hundreds of collections (spike G2: propc 3, 0.68 s at 100, 0.43 s
// at 400); larger while the live heap is below [MinHeap] (gc.go).
const GCPercent = 400

// Go samples the allocations for its memory profile (one per 512 KiB by
// default), each sample unwinding the stack: with the deep recursions of
// the programs and their allocation rates, a few per cent of the time
// (propc 3: 0.77 -> 0.75 s). Off unless a profile is asked for
// (ELAN_MEMPROFILE).
func init() {
	if os.Getenv("ELAN_MEMPROFILE") == "" {
		runtime.MemProfileRate = 0
	}
}

// Main is the main function of a generated program: it parses the options,
// sets the GC and the stack limit, and runs program on a new goroutine with
// a session on [Out]; program computes and prints the results
// (s.Result(t)) of the start term, the query given as its second argument;
// the statistics are printed after it returns, then the program exits
// (status 0). querySort is the sort named in the prompt when a query must
// be read (without -noInput: not supported yet, status 2). usesQuery: the
// start term uses the query; under -noInput, the program prints REM's
// message (rem/compiler/Query.java: "No ground term is defined", "Do not
// use -noInput in this case") and exits with status 0 without running;
// else program gets a nil query.
func Main(querySort string, usesQuery bool, program func(s *Session, query Term)) {
	o := ParseOptions(os.Args[1:])
	if o.Help {
		Out.WriteString(Help)
		Exit(1)
	}
	if os.Getenv("GOGC") == "" {
		debug.SetGCPercent(GCPercent)
		startGCTuner()
	}
	SetMaxStack(MaxStack)
	done := make(chan int)
	go func() {
		defer func() {
			// a panic is a bug (of the compiler or the runtime): a run-time
			// error, the results so far kept, not Go's exit status 2
			// (which says "not supported yet")
			if r := recover(); r != nil {
				Fatal(fmt.Sprintf("elanrt: internal error: %v\n%s", r, debug.Stack()))
			}
		}()
		s := NewSession(o, Out)
		if o.NoInput && usesQuery {
			Out.WriteString("No ground term is defined\nDo not use -noInput in this case\n")
			done <- 0
			return
		}
		var query Term
		if !o.NoInput {
			if o.Other != "" {
				Unsupported("the option " + o.Other + " (use -noInput, or give the query on the standard input)")
			}
			fmt.Fprintf(Out, "Enter a query term of sort '%s'\n", querySort)
			FlushOutput()
			if queryParser == nil {
				fmt.Fprintln(os.Stderr, "elanrt: reading a query is not supported yet; use -noInput")
				done <- 2
				return
			}
			query = readQuery(queryParser)
		}
		stop := func() {}
		if f := os.Getenv("ELAN_CPUPROFILE"); f != "" {
			stop = startProfile(f)
		}
		profiles = func() {
			stop()
			if f := os.Getenv("ELAN_MEMPROFILE"); f != "" {
				writeMemProfile(f)
			}
		}
		program(s, query)
		if depth != 0 {
			// every Guard has its Unguard (generated code: a bug otherwise)
			panic(fmt.Sprintf("guarded depth %d at the end of the program", depth))
		}
		endProfiles()
		s.Finish()
		if os.Getenv("ELAN_STATS") == "1" {
			FlushOutput()
			fmt.Fprintln(os.Stderr, "elanrt:", Stats())
		}
		done <- 0
	}()
	Exit(<-done)
}

// startProfile starts a CPU profile written to file (ELAN_CPUPROFILE: `go
// tool pprof a.out file`); the function returned stops it.
func startProfile(file string) func() {
	f, err := os.Create(file)
	if err == nil {
		err = pprof.StartCPUProfile(f)
	}
	if err != nil {
		fmt.Fprintln(os.Stderr, "elanrt: ELAN_CPUPROFILE:", err)
		return func() {}
	}
	return func() {
		pprof.StopCPUProfile()
		f.Close()
	}
}

// writeMemProfile writes the allocations of the run to file
// (ELAN_MEMPROFILE: `go tool pprof -sample_index=alloc_space a.out file`).
func writeMemProfile(file string) {
	f, err := os.Create(file)
	if err == nil {
		err = pprof.Lookup("allocs").WriteTo(f, 0)
		f.Close()
	}
	if err != nil {
		fmt.Fprintln(os.Stderr, "elanrt: ELAN_MEMPROFILE:", err)
	}
}

// Stats is a report of the runtime's counters (ELAN_STATS=1 in a program,
// tests).
func Stats() string {
	return "steps " + strconv.FormatInt(Steps, 10) +
		", nodes created " + strconv.FormatInt(Created, 10) +
		", table hits " + strconv.FormatInt(Hits, 10) +
		", live " + strconv.Itoa(LiveTerms()) +
		", table " + strconv.Itoa(TableSize()) +
		", rebuilds " + strconv.FormatInt(Rebuilds, 10) +
		", forced GCs " + strconv.FormatInt(ForcedGCs, 10) +
		", GC cycles " + strconv.FormatUint(gcCycles(), 10) +
		", GC tunings " + strconv.FormatInt(GCTunings.Load(), 10)
}
