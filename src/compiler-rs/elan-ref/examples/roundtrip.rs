//! `roundtrip FILE...`: parses each `.ref` file, prints it back and checks
//! that the tokens are the same (white space and comments aside). Prints
//! `ok FILE` or `FAIL FILE: reason`; exits with 1 when a file fails.
//! `roundtrip --print FILE`: prints the file as the printer writes it.
use std::process::ExitCode;

fn check(path: &str) -> Result<(), String> {
    let src = std::fs::read(path).map_err(|e| format!("cannot read: {e}"))?;
    let prog = elan_ref::parse_bytes(&src).map_err(|e| format!("parse error at {e}"))?;
    let out = elan_ref::print_bytes(&prog);
    if let Some(d) = elan_ref::normalise::first_difference(&src, &out) {
        return Err(format!("printed file differs, {d}"));
    }
    // the printed file must parse to the same model
    let again = elan_ref::parse_bytes(&out).map_err(|e| format!("reparse error at {e}"))?;
    if again != prog {
        return Err("the printed file parses to another model".into());
    }
    Ok(())
}

fn main() -> ExitCode {
    let files: Vec<String> = std::env::args().skip(1).collect();
    if files.len() == 2 && files[0] == "--print" {
        let src = match std::fs::read(&files[1]) {
            Ok(s) => s,
            Err(e) => {
                eprintln!("{}: {e}", files[1]);
                return ExitCode::from(2);
            }
        };
        return match elan_ref::parse_bytes(&src) {
            Ok(p) => {
                use std::io::Write;
                let _ = std::io::stdout().write_all(&elan_ref::print_bytes(&p));
                ExitCode::SUCCESS
            }
            Err(e) => {
                eprintln!("{}:{e}", files[1]);
                ExitCode::from(1)
            }
        };
    }
    if files.is_empty() {
        eprintln!("usage: roundtrip FILE.ref...");
        return ExitCode::from(2);
    }
    let mut failed = 0;
    for f in &files {
        match check(f) {
            Ok(()) => println!("ok   {f}"),
            Err(e) => {
                println!("FAIL {f}: {e}");
                failed += 1;
            }
        }
    }
    if failed > 0 {
        ExitCode::from(1)
    } else {
        ExitCode::SUCCESS
    }
}
