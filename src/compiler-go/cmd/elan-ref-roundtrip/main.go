// Command elan-ref-roundtrip checks the .ref reader and printer (package
// elan/ref), as the roundtrip example of the Rust crate elan-ref.
//
// `elan-ref-roundtrip FILE...` parses each .ref file, prints it back and
// checks that the tokens are the same (white space and comments aside) and
// that the printed file parses to the same model. It prints `ok   FILE` or
// `FAIL FILE: reason` and exits with 1 when a file fails.
//
// `elan-ref-roundtrip --print FILE` prints the file as the printer writes it.
package main

import (
	"bytes"
	"errors"
	"fmt"
	"os"
	"reflect"

	"elan/ref"
)

func check(path string) error {
	src, err := os.ReadFile(path)
	if err != nil {
		return fmt.Errorf("cannot read: %v", err)
	}
	prog, err := ref.Parse(src)
	if err != nil {
		return fmt.Errorf("parse error at %v", err)
	}
	out := ref.Print(prog)
	if d := ref.FirstDifference(src, out); d != "" {
		return fmt.Errorf("printed file differs, %s", d)
	}
	// the printed file must parse to the same model
	again, err := ref.Parse(out)
	if err != nil {
		return fmt.Errorf("reparse error at %v", err)
	}
	if !reflect.DeepEqual(again, prog) {
		return errors.New("the printed file parses to another model")
	}
	return nil
}

func main() {
	files := os.Args[1:]
	if len(files) == 2 && files[0] == "--print" {
		src, err := os.ReadFile(files[1])
		if err != nil {
			fmt.Fprintf(os.Stderr, "%v\n", err)
			os.Exit(2)
		}
		p, err := ref.Parse(src)
		if err != nil {
			fmt.Fprintf(os.Stderr, "%s:%v\n", files[1], err)
			os.Exit(1)
		}
		os.Stdout.Write(ref.Print(p))
		return
	}
	if len(files) == 0 {
		fmt.Fprintln(os.Stderr, "usage: elan-ref-roundtrip FILE.ref...  |  elan-ref-roundtrip --print FILE.ref")
		os.Exit(2)
	}
	failed := 0
	var out bytes.Buffer
	for _, f := range files {
		if err := check(f); err != nil {
			fmt.Fprintf(&out, "FAIL %s: %v\n", f, err)
			failed++
		} else {
			fmt.Fprintf(&out, "ok   %s\n", f)
		}
	}
	os.Stdout.Write(out.Bytes())
	if failed > 0 {
		os.Exit(1)
	}
}
