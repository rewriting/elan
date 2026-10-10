//! `elanc-rs`: compiles an ELAN program (its `.ref` export) to a Rust
//! crate (spec `docs/superpowers/specs/2026-10-10-s6-rust-compiler-design.md`).
//!
//! model ([`elan_ref`]) → [`lower`] → [`ir`] → [`emit`] (Rust source); the
//! driver (`main.rs`) runs `elan --cexport`, writes the crate and builds it
//! with cargo.

pub mod emit;
pub mod ir;
pub mod lower;
pub mod matching;
pub mod names;

/// Compile the `.ref` text of program `name` to a crate (named after its
/// content, [`emit::package_name`]).
pub fn compile_ref(
    src: &[u8],
    name: &str,
    runtime_path: &str,
) -> Result<emit::Crate, lower::Error> {
    let p = elan_ref::parse_bytes(src)
        .map_err(|e| lower::Error::Invalid(format!("cannot read the .ref file: {e}")))?;
    let ir = lower::lower(&p, name)?;
    Ok(emit::emit_crate(&ir, runtime_path))
}
