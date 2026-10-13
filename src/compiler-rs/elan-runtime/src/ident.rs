//! Identifiers (sort `ident`). An identifier is its index in the table of
//! identifiers of the program: the `Identifiers` table of the `.ref` file,
//! whose indices are the positions of REM's stringtab
//! (`src/compiler/earley/stringtab.cc`: the sum of the characters modulo
//! 3000, a collision probed by steps of 211), so that a name gets the
//! index REM gives it and identifiers compare as REM compares them (by
//! index). The generated program sets the table ([`init_idents`]) before
//! running; the query parser adds the new names it reads
//! ([`ident_index`]). As `elanrt/ident.go` of elanc-go.

use std::cell::RefCell;
use std::collections::HashMap;

use crate::print::{Print, Writer};

/// An identifier: its index in the table of identifiers.
#[derive(Clone, Copy, PartialEq, Eq, Hash, Debug, PartialOrd, Ord)]
pub struct Ident(pub i32);

/// The size of REM's table of identifiers (`tabofident`).
const TABLE_SIZE: i32 = 3000;

#[derive(Default)]
struct Table {
    names: HashMap<i32, String>,
    indices: HashMap<String, i32>,
}

thread_local! {
    static TABLE: RefCell<Table> = RefCell::new(Table::default());
}

/// Set the table of identifiers: the program's, (index, name) pairs.
pub fn init_idents(entries: &[(i32, &str)]) {
    TABLE.with(|t| {
        let mut t = t.borrow_mut();
        *t = Table::default();
        for &(i, n) in entries {
            t.names.insert(i, n.to_string());
            t.indices.insert(n.to_string(), i);
        }
    })
}

/// The index of the identifier `name`: its index in the table, else the
/// free position REM's stringtab gives it (added to the table).
pub fn ident_index(name: &str) -> Ident {
    TABLE.with(|t| {
        let mut t = t.borrow_mut();
        if let Some(&i) = t.indices.get(name) {
            return Ident(i);
        }
        // (C chars: signed bytes)
        let sum: i32 = name.bytes().map(|b| b as i8 as i32).sum();
        let mut pos = sum % TABLE_SIZE;
        if pos < 0 {
            pos += TABLE_SIZE; // (out of REM's table: undefined there)
        }
        let mut n = 0;
        while t.names.contains_key(&pos) {
            if n == TABLE_SIZE {
                crate::fatal("elan-runtime: the table of identifiers is full");
            }
            n += 1;
            pos = (pos + 211) % TABLE_SIZE;
        }
        t.names.insert(pos, name.to_string());
        t.indices.insert(name.to_string(), pos);
        Ident(pos)
    })
}

/// The name of an identifier (`IDENT(i)` if the table has none).
pub fn ident_name(i: Ident) -> String {
    TABLE.with(|t| {
        t.borrow()
            .names
            .get(&i.0)
            .cloned()
            .unwrap_or_else(|| format!("IDENT({})", i.0))
    })
}

/// An identifier prints as its name (an identifier token).
impl Print for Ident {
    fn print(&self, w: &mut Writer) {
        w.ident(&ident_name(*self))
    }
}
