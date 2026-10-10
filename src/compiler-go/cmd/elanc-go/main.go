// Command elanc-go compiles an ELAN program (its .ref export) to a Go program.
package main

import (
	"fmt"
	"os"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Fprintln(os.Stderr, "usage: elanc-go [options] program[.lgi] [specification[.spc]]")
		os.Exit(1)
	}
	fmt.Fprintln(os.Stderr, "elanc-go: not implemented yet")
	os.Exit(1)
}
