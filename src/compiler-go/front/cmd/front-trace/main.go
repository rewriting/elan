// Command front-trace prints the lexem traces of the Go front end in the
// format of the C++ harness elan-lextrace (tests/front/README.md):
//
//	front-trace lex FILE
//	front-trace load [--tree] [--trace FILE] [--refuse] [elan options] prog.lgi [spec]
//
// S8a, task 2: a stub; the subcommands come with tasks 5, 7 and 8.
package main

import (
	"fmt"
	"os"
)

func main() {
	fmt.Fprintln(os.Stderr, "front-trace: not implemented (S8a)")
	os.Exit(3)
}
