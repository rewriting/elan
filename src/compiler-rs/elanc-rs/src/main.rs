//! `elanc-rs`: compiles an ELAN program (its `.ref` export) to a Rust crate.

fn main() {
    let args: Vec<String> = std::env::args().skip(1).collect();
    if args.is_empty() {
        eprintln!("usage: elanc-rs [options] program[.lgi] [spec]");
        std::process::exit(1);
    }
    eprintln!("elanc-rs: not implemented yet");
    std::process::exit(1);
}
