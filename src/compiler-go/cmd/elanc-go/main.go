// Command elanc-go compiles an ELAN program to a native program through Go
// (spec docs/superpowers/specs/2026-10-11-s7-go-compiler-design.md).
//
// Usage:
//
//	elanc-go [options] program[.lgi] [specification[.spc]]
//
// Steps:
//
//  1. `elan --cexport .elan-go.<prog>/<prog>.ref -b [elan options]
//     <dir>/<prog>.lgi [spec]`, as elanc (src/compiler/scripts/elanc.src)
//     runs it: the REM options of elanc are accepted and ignored (-v lists
//     them), other options this driver does not know are passed to elan
//     (--trace, -t, --elanlib, --secondlib, -l, --output take an argument);
//  2. the .ref file is read (package elan/ref), lowered to the IR (elan/ir)
//     and emitted as a Go module in .elan-go.<prog>/ (the work directory, in
//     the current directory, as REM's .elan.<prog>/): go.mod (module
//     elanprog, the runtime elan/elanrt by a replace directive) and main.go
//     (elan/gen);
//  3. `go build -trimpath` offline (GOTOOLCHAIN=local, GOFLAGS=-mod=mod,
//     GOPROXY=off, GOWORK=off) with a shared build cache (GOCACHE), so that
//     the runtime is compiled once, not per program: --gocache DIR, else
//     $ELANC_GO_CACHE, else $XDG_CACHE_HOME/elanc-go or
//     $HOME/.cache/elanc-go; -trimpath keeps the build independent of the
//     work directory (the same program compiled from another directory
//     reuses the cache);
//  4. the program is copied to ./a.out (-o/-output NAME); that file is
//     removed first, so that a refusal or a failure leaves none.
//
// The runtime module: --runtime DIR, else $ELAN_GO_RUNTIME, else the
// installation (<prefix>/share/elan-go/elanrt, <prefix> being the parent of
// the directory of this executable). The interpreter: --elan FILE, else
// <prefix>/bin/elan, else elan from PATH. The go command: --go FILE, else
// go from PATH.
//
// --from-ref FILE compiles an existing .ref file (no export; the program
// name is still taken from the .lgi argument).
//
// Exit status: 0 on success, 2 when the program uses a construct that is
// not supported yet (later stages of S7), 1 on any other failure (an
// internal error included). -v prints
// the commands and the build time; --no-build stops after writing the
// module.
//
// The generated programs exit with 0 on success, 1 on a run-time error (or
// -help), 2 on a construct supported only partly met at run time, 3 when
// the recursion is too deep (elanrt); on 1, 2 and 3 the results printed
// before are written first.
package main

import (
	"bytes"
	"errors"
	"fmt"
	"io"
	"os"
	"os/exec"
	"path/filepath"
	"runtime/debug"
	"slices"
	"strings"
	"time"

	"elan/gen"
	"elan/ir"
	"elan/ref"
)

const usage = `usage: elanc-go [options] program[.lgi] [specification[.spc]]
options:
  -o FILE, -output FILE  the program built (default a.out)
  --gocache DIR          go build cache shared by the programs
                         (default $ELANC_GO_CACHE, else ~/.cache/elanc-go)
  --runtime DIR          the elanrt module (default $ELAN_GO_RUNTIME,
                         else <prefix>/share/elan-go/elanrt)
  --elan FILE            the ELAN interpreter (default <prefix>/bin/elan)
  --go FILE              the go command (default go)
  --from-ref FILE        compile this .ref file instead of exporting the program
  --no-build             write the Go module only
  -v                     print the commands and the build time
  the REM options of elanc (-nosplit, -quiet, -strategy NAME...) are ignored;
  other options are passed to elan (as elanc does)
exit status: 0 success, 2 construct not supported yet, 1 other failure.
the generated program exits with 0 success, 1 run-time error, 2 construct
  supported only partly met at run time, 3 recursion too deep (the results
  printed before an error are kept)`

// remOptions are the REM options without argument of elanc
// (src/compiler/scripts/elanc.src, with -ba*): accepted and ignored, as
// -strategy NAME; -output NAME is -o.
var remOptions = []string{
	"-O", "-O2", "-fast", "-nocode", "-nosplit", "-split", "-lib",
	"-choicePointDebug", "-noOptimiseChoicePoint", "-optimiseChoicePoint",
	"-verbose", "-debug", "-coq", "-proofterm", "-color", "-noColor",
	"-withGoto", "-oldVariableAffectation", "-onlyC", "-noC", "-quiet",
}

type options struct {
	output      string
	gocache     string
	runtime     string
	elan        string
	goCmd       string
	fromRef     string
	build       bool
	verbose     bool
	elanOptions []string
	ignored     []string // REM options (elanc.src)
	lgi         string
	spec        string
}

func fail(format string, a ...any) {
	fmt.Fprintf(os.Stderr, "elanc-go: "+format+"\n", a...)
	os.Exit(1)
}

func parseArgs(args []string) (options, error) {
	o := options{output: "a.out", build: true, elanOptions: []string{"-b"}}
	var positional []string
	for i := 0; i < len(args); i++ {
		a := args[i]
		value := func() (string, error) {
			if i+1 >= len(args) {
				return "", fmt.Errorf("%s: missing argument", a)
			}
			i++
			return args[i], nil
		}
		var v string
		var err error
		switch {
		case a == "-o" || a == "-output":
			o.output, err = value()
		case a == "--gocache":
			o.gocache, err = value()
		case a == "--runtime":
			o.runtime, err = value()
		case a == "--elan":
			o.elan, err = value()
		case a == "--go":
			o.goCmd, err = value()
		case a == "--from-ref":
			o.fromRef, err = value()
		case a == "--no-build":
			o.build = false
		case a == "-v":
			o.verbose = true
		case a == "-h" || a == "-help" || a == "--help":
			return o, errHelp
		case slices.Contains([]string{"--trace", "-t", "--elanlib", "--secondlib", "-l", "--output"}, a):
			// elan options with an argument (elanc.src)
			if v, err = value(); err == nil {
				o.elanOptions = append(o.elanOptions, a, v)
			}
		case a == "-b": // always passed
		case a == "-strategy":
			if v, err = value(); err == nil {
				o.ignored = append(o.ignored, a+" "+v)
			}
		case slices.Contains(remOptions, a) || strings.HasPrefix(a, "-ba"):
			o.ignored = append(o.ignored, a)
		case strings.HasPrefix(a, "-"):
			o.elanOptions = append(o.elanOptions, a)
		default:
			positional = append(positional, a)
		}
		if err != nil {
			return o, err
		}
	}
	if len(positional) == 2 {
		o.spec = positional[1]
		positional = positional[:1]
	}
	if len(positional) != 1 {
		return o, errUsage
	}
	o.lgi = positional[0]
	return o, nil
}

var (
	errHelp  = errors.New("help")
	errUsage = errors.New("usage")
)

// prefix is the parent of the directory of this executable.
func prefix() string {
	exe, err := os.Executable()
	if err != nil {
		return ""
	}
	if r, err := filepath.EvalSymlinks(exe); err == nil {
		exe = r
	}
	return filepath.Dir(filepath.Dir(exe))
}

// defaultCache is the shared build cache: $ELANC_GO_CACHE, else
// $XDG_CACHE_HOME/elanc-go, else $HOME/.cache/elanc-go.
func defaultCache() string {
	if d := os.Getenv("ELANC_GO_CACHE"); d != "" {
		return d
	}
	if d := os.Getenv("XDG_CACHE_HOME"); d != "" {
		return filepath.Join(d, "elanc-go")
	}
	if d := os.Getenv("HOME"); d != "" {
		return filepath.Join(d, ".cache", "elanc-go")
	}
	return ""
}

// writeIfChanged writes path unless it already has this content.
func writeIfChanged(path string, content []byte) {
	if old, err := os.ReadFile(path); err == nil && bytes.Equal(old, content) {
		return
	}
	if err := os.WriteFile(path, content, 0o644); err != nil {
		fail("cannot write %s: %v", path, err)
	}
}

func show(verbose bool, cmd *exec.Cmd) {
	if verbose {
		fmt.Fprintln(os.Stderr, strings.Join(cmd.Args, " "))
	}
}

func isFile(p string) bool {
	st, err := os.Stat(p)
	return err == nil && st.Mode().IsRegular()
}

// split gives the directory and the name of dir/name.ext.
func split(file, ext string) (string, string) {
	return filepath.Dir(file), strings.TrimSuffix(filepath.Base(file), ext)
}

func copyFile(from, to string) error {
	in, err := os.Open(from)
	if err != nil {
		return err
	}
	defer in.Close()
	out, err := os.OpenFile(to, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0o755)
	if err != nil {
		return err
	}
	if _, err := io.Copy(out, in); err != nil {
		out.Close()
		return err
	}
	return out.Close()
}

func main() { guarded(compile) }

// guarded runs f; a panic (a bug of the compiler) is a failure, exit status
// 1 with the error and its stack, not Go's exit status 2, which says "not
// supported yet".
func guarded(f func()) {
	defer func() {
		if r := recover(); r != nil {
			fmt.Fprintf(os.Stderr, "elanc-go: internal error: %v\n%s", r, debug.Stack())
			os.Exit(1)
		}
	}()
	f()
}

func compile() {
	o, err := parseArgs(os.Args[1:])
	switch {
	case err == errHelp:
		fmt.Println(usage)
		os.Exit(0)
	case err == errUsage:
		fmt.Fprintln(os.Stderr, usage)
		os.Exit(1)
	case err != nil:
		fail("%v", err)
	}
	lgiDir, prog := split(o.lgi, ".lgi")
	pre := prefix()
	if o.verbose && len(o.ignored) > 0 {
		fmt.Fprintln(os.Stderr, "elanc-go: REM options ignored: "+strings.Join(o.ignored, " "))
	}

	// a previous program must not survive a refusal or a failure
	if err := os.Remove(o.output); err != nil && !errors.Is(err, os.ErrNotExist) {
		fail("cannot remove %s: %v", o.output, err)
	}

	// the work directory
	work, err := filepath.Abs(".elan-go." + prog)
	if err != nil {
		fail("%v", err)
	}
	if err := os.MkdirAll(work, 0o755); err != nil {
		fail("cannot create %s: %v", work, err)
	}

	// 1. export
	refFile := o.fromRef
	if refFile == "" {
		elan := o.elan
		if elan == "" {
			if p := filepath.Join(pre, "bin", "elan"); isFile(p) {
				elan = p
			} else {
				elan = "elan"
			}
		}
		refFile = filepath.Join(work, prog+".ref")
		os.Remove(refFile) // a stale .ref must not be compiled
		args := append([]string{"--cexport", refFile}, o.elanOptions...)
		args = append(args, filepath.Join(lgiDir, prog+".lgi"))
		if o.spec != "" {
			d, n := split(o.spec, ".spc")
			args = append(args, filepath.Join(d, n+".spc"))
		}
		cmd := exec.Command(elan, args...)
		cmd.Stdout, cmd.Stderr = os.Stdout, os.Stderr
		show(o.verbose, cmd)
		if err := cmd.Run(); err != nil {
			fail("%s --cexport failed (%v)", elan, err)
		}
	}
	src, err := os.ReadFile(refFile)
	if err != nil {
		fail("cannot read %s: %v", refFile, err)
	}

	// 2. generate
	runtime := o.runtime
	if runtime == "" {
		runtime = os.Getenv("ELAN_GO_RUNTIME")
	}
	if runtime == "" {
		if d := filepath.Join(pre, "share", "elan-go", "elanrt"); isFile(filepath.Join(d, "go.mod")) {
			runtime = d
		} else {
			fail("the elanrt module is not found: use --runtime DIR")
		}
	}
	if runtime, err = filepath.Abs(runtime); err != nil {
		fail("runtime %s: %v", runtime, err)
	}
	if !isFile(filepath.Join(runtime, "go.mod")) {
		fail("runtime %s: no go.mod", runtime)
	}
	p, err := ref.Parse(src)
	if err != nil {
		fail("%s: cannot read the .ref file: %v", prog, err)
	}
	program, err := ir.Lower(p, prog)
	var unsup *ir.Unsupported
	switch {
	case errors.As(err, &unsup):
		fmt.Fprintf(os.Stderr, "elanc-go: %s: %v\n", prog, err)
		os.Exit(2)
	case err != nil:
		fail("%s: %v", prog, err)
	}
	mainGo, err := gen.Main(program)
	writeIfChanged(filepath.Join(work, "go.mod"), []byte(gen.GoMod(prog, runtime)))
	writeIfChanged(filepath.Join(work, "main.go"), mainGo)
	if err != nil {
		fail("%s: %v (see %s)", prog, err, filepath.Join(work, "main.go"))
	}
	if !o.build {
		return
	}

	// 3. build
	cache := o.gocache
	if cache == "" {
		cache = defaultCache()
	}
	if cache == "" {
		cache = filepath.Join(work, "gocache")
	}
	if cache, err = filepath.Abs(cache); err != nil {
		fail("%v", err)
	}
	goCmd := o.goCmd
	if goCmd == "" {
		goCmd = "go"
	}
	bin := filepath.Join(work, "elanprog")
	cmd := exec.Command(goCmd, "build", "-trimpath", "-o", bin, ".")
	cmd.Dir = work
	cmd.Env = append(os.Environ(), "GOTOOLCHAIN=local", "GOFLAGS=-mod=mod", "GOPROXY=off",
		"GOWORK=off", "GOCACHE="+cache)
	cmd.Stdout, cmd.Stderr = os.Stderr, os.Stderr
	if o.verbose {
		fmt.Fprintf(os.Stderr, "cd %s && GOCACHE=%s %s\n", work, cache, strings.Join(cmd.Args, " "))
	}
	t0 := time.Now()
	if err := cmd.Run(); err != nil {
		fail("go build failed (%v), module in %s", err, work)
	}
	if o.verbose {
		fmt.Fprintf(os.Stderr, "elanc-go: build time %.2f s\n", time.Since(t0).Seconds())
	}

	// 4. the program
	if err := copyFile(bin, o.output); err != nil {
		fail("cannot copy %s to %s: %v", bin, o.output, err)
	}
}
