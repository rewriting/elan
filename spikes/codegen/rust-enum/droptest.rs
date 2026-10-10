// Dropping a long hash-consed list on the main thread (default 8 MB stack).
// usage: droptest N [naive]
//   default: sort with the generated `children_into` (iterative free)
//   naive:   same sort without it (recursive free through Drop) -> overflows
#[macro_use]
mod runtime;
use runtime::*;

#[derive(PartialEq, Eq, Hash)]
enum List { Nil, Cons(i64, H<List>) }
impl Sort for List {
    unique_table!(List);
    fn children_into(self, out: &mut Vec<H<List>>) {
        if let List::Cons(_, l) = self { out.push(l) }
    }
}

#[derive(PartialEq, Eq, Hash)]
enum NList { Nil, Cons(i64, H<NList>) }
impl Sort for NList {
    unique_table!(NList);
}

fn main() {
    let n: i64 = std::env::args().nth(1).and_then(|s| s.parse().ok()).unwrap_or(10_000_000);
    let naive = std::env::args().nth(2).as_deref() == Some("naive");
    if naive {
        let mut l = H::new(NList::Nil);
        for i in 0..n { l = H::new(NList::Cons(i, l)); }
        println!("built {} cells (naive), live = {}", n, live_terms::<NList>());
        drop(l);
    } else {
        let mut l = H::new(List::Nil);
        for i in 0..n { l = H::new(List::Cons(i, l)); }
        println!("built {} cells, live = {}", n, live_terms::<List>());
        drop(l);
        println!("dropped, live = {}", live_terms::<List>());
    }
}
