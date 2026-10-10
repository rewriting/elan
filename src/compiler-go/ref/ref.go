// Package ref reads and prints the .ref export of ELAN programs
// (docs/ref-format.md); it is the Go port of the Rust crate elan-ref
// (src/compiler-rs/elan-ref), with the same model, semantics and tests.
//
// `elan --cexport F.ref` writes a loaded program (tables, grammars, rules,
// strategies, query) with numbers instead of names. This package reads it
// into a typed [Program] ([Parse]) and prints it back ([Print]): printing a
// parsed file gives the same tokens (comments and white space aside), see
// [FirstDifference] and the tests.
//
// References for the meaning of the fields: REM's grammar
// (src/compiler/rem/parser/REFParser.jj), the interpreter's writer and
// importer (src/interpreter/ref/ref.cc,
// src/interpreter/parse/grammars/aterm.orig).
//
// Names are kept as the raw bytes of the file (a Go string is a byte
// sequence), so printing gives back the same bytes without any decoding.
package ref
