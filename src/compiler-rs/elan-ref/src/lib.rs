//! Reader of the `.ref` export of ELAN programs (`docs/ref-format.md`).
//!
//! `elan --cexport F.ref` writes a loaded program (tables, grammars, rules,
//! strategies, query) with numbers instead of names. This crate reads it into
//! a typed [`model::Program`] ([`parse`], [`parse_bytes`]) and prints it back
//! ([`print`], [`print_bytes`]): printing a parsed file gives the same tokens
//! (comments and white space aside), see [`normalise`] and the tests.
//!
//! References for the meaning of the fields: REM's grammar
//! (`src/compiler/rem/parser/REFParser.jj`), the interpreter's writer and
//! importer (`src/interpreter/ref/ref.cc`,
//! `src/interpreter/parse/grammars/aterm.orig`).

pub mod lexer;
pub mod model;
pub mod normalise;
pub mod parser;
pub mod printer;

pub use lexer::Error;
pub use model::*;
pub use parser::{parse, parse_bytes, parse_strategy, parse_term, parse_wheres};
pub use printer::{print, print_bytes, print_strategy, print_term};
