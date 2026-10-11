// Package diag holds the output streams of the Go front end and its fatal
// errors (S8a). The C++ writes errors to sterr and exits through failexit()
// (base/fatal.h, load/module.cc), which throws ElanFatal caught by elan_main;
// here Fail panics with Exit and Run recovers it.
package diag

import (
	"fmt"
	"io"
)

// D is the interpreter's output state: stout, sterr and the `warnings`
// option (base/commondefs.cc, default on; -w, -q and -b clear it).
type D struct {
	Out, Err io.Writer
	Warnings bool
}

// Exit is the panic value of a fatal error: the process status.
type Exit struct{ Status int }

// Fail is failexit(): a fatal error, status 1.
func (d *D) Fail() { panic(Exit{Status: 1}) }

// Internal is an internal error of the front end (a state the C++ does not
// define, or a bug of the port, not an error of the program): "where: msg"
// on the error stream, status 70.
func (d *D) Internal(where, format string, a ...any) {
	d.Errs(where, ": ", fmt.Sprintf(format, a...), "\n")
	panic(Exit{Status: 70})
}

// Errs writes its arguments to the error stream (sterr << ...).
func (d *D) Errs(parts ...string) {
	for _, p := range parts {
		io.WriteString(d.Err, p)
	}
}

// Outs writes its arguments to the output stream (stout << ...).
func (d *D) Outs(parts ...string) {
	for _, p := range parts {
		io.WriteString(d.Out, p)
	}
}

// Warn writes its arguments to the error stream when warnings are on.
func (d *D) Warn(parts ...string) {
	if d.Warnings {
		d.Errs(parts...)
	}
}

// Run calls f and returns 0, or the status of the Exit it panicked with
// (elan_main's catch of ElanFatal). Any other panic goes on.
func Run(f func()) (status int) {
	defer func() {
		if r := recover(); r != nil {
			e, ok := r.(Exit)
			if !ok {
				panic(r)
			}
			status = e.Status
		}
	}()
	f()
	return 0
}
