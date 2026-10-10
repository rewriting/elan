//! `elanc-rs`: compiles an ELAN program to a native program through Rust.
//!
//! ```text
//! elanc-rs [options] program[.lgi] [specification[.spc]]
//! ```
//!
//! 1. `elan --cexport .elan-rs.<prog>/<prog>.ref -b [elan options]
//!    <dir>/<prog>.lgi [spec]`, as `elanc` (`src/compiler/scripts/elanc.src`)
//!    runs it: options this driver does not know are passed to `elan`
//!    (`--trace`, `-t`, `--elanlib`, `--secondlib`, `-l`, `--output` take an
//!    argument);
//! 2. the `.ref` file is read, lowered to the IR and emitted as a cargo
//!    crate in `.elan-rs.<prog>/` (the work directory, in the current
//!    directory, as REM's `.elan.<prog>/`);
//! 3. `cargo build --release --offline` with a **shared target directory**,
//!    so that the runtime crate is compiled once, not per program:
//!    `--target-dir DIR`, else `$ELANC_RS_TARGET_DIR`, else
//!    `$XDG_CACHE_HOME/elanc-rs/target` or `$HOME/.cache/elanc-rs/target`;
//! 4. the program is copied to `./a.out` (`-o`/`-output NAME`); that file
//!    is removed first, so that a refusal or a failure leaves none.
//!
//! The runtime crate: `--runtime DIR`, else `$ELAN_RS_RUNTIME`, else the
//! installation (`<prefix>/share/elan-rs/elan-runtime`, `<prefix>` being
//! the parent of the directory of this executable). The interpreter:
//! `--elan FILE`, else `<prefix>/bin/elan`, else `elan` from `PATH`.
//!
//! `--from-ref FILE` compiles an existing `.ref` file (no export; the
//! program name is still taken from the `.lgi` argument).
//!
//! The generated `main.rs` is passed through `rustfmt` when it is
//! installed (readability only).
//!
//! Exit status: 0 on success, 2 when the program uses a construct that is
//! not supported yet (later stages of S6), 1 on any other failure. `-v`
//! prints the commands and the build time; `--no-build` stops after
//! writing the crate.

use std::io::Write;
use std::path::{Path, PathBuf};
use std::process::{exit, Command, Stdio};
use std::time::Instant;

use elanc_rs::lower;

const USAGE: &str = "usage: elanc-rs [options] program[.lgi] [specification[.spc]]
options:
  -o FILE, -output FILE  the program built (default a.out)
  --target-dir DIR       cargo target directory shared by the programs
                         (default $ELANC_RS_TARGET_DIR, else ~/.cache/elanc-rs/target)
  --runtime DIR          the elan-runtime crate (default $ELAN_RS_RUNTIME,
                         else <prefix>/share/elan-rs/elan-runtime)
  --elan FILE            the ELAN interpreter (default <prefix>/bin/elan)
  --from-ref FILE        compile this .ref file instead of exporting the program
  --no-build             write the crate only
  -v                     print the commands and the build time
  other options are passed to elan (as elanc does)";

struct Options {
    output: String,
    target_dir: Option<PathBuf>,
    runtime: Option<PathBuf>,
    elan: Option<PathBuf>,
    from_ref: Option<PathBuf>,
    build: bool,
    verbose: bool,
    elan_options: Vec<String>,
    lgi: String,
    spec: Option<String>,
}

fn fail(msg: impl AsRef<str>) -> ! {
    eprintln!("elanc-rs: {}", msg.as_ref());
    exit(1)
}

fn parse_args(args: Vec<String>) -> Options {
    let mut output = "a.out".to_string();
    let (mut target_dir, mut runtime, mut elan, mut from_ref) = (None, None, None, None);
    let (mut build, mut verbose) = (true, false);
    let mut elan_options = vec!["-b".to_string()];
    let mut positional = Vec::new();
    let mut it = args.into_iter();
    while let Some(a) = it.next() {
        let mut value = |name: &str| {
            it.next()
                .unwrap_or_else(|| fail(format!("{name}: missing argument")))
        };
        match a.as_str() {
            "-o" | "-output" => output = value(&a),
            "--target-dir" => target_dir = Some(PathBuf::from(value(&a))),
            "--runtime" => runtime = Some(PathBuf::from(value(&a))),
            "--elan" => elan = Some(PathBuf::from(value(&a))),
            "--from-ref" => from_ref = Some(PathBuf::from(value(&a))),
            "--no-build" => build = false,
            "-v" => verbose = true,
            "-h" | "-help" | "--help" => {
                println!("{USAGE}");
                exit(0)
            }
            // elan options with an argument (elanc.src)
            "--trace" | "-t" | "--elanlib" | "--secondlib" | "-l" | "--output" => {
                let v = value(&a);
                elan_options.push(a);
                elan_options.push(v);
            }
            "-b" => {} // always passed
            _ if a.starts_with('-') => elan_options.push(a),
            _ => positional.push(a),
        }
    }
    let spec = if positional.len() == 2 {
        positional.pop()
    } else {
        None
    };
    if positional.len() != 1 {
        eprintln!("{USAGE}");
        exit(1)
    }
    Options {
        output,
        target_dir,
        runtime,
        elan,
        from_ref,
        build,
        verbose,
        elan_options,
        lgi: positional.pop().unwrap(),
        spec,
    }
}

/// `<prefix>`: the parent of the directory of this executable.
fn prefix() -> Option<PathBuf> {
    let exe = std::env::current_exe().ok()?.canonicalize().ok()?;
    Some(exe.parent()?.parent()?.to_path_buf())
}

fn env_path(name: &str) -> Option<PathBuf> {
    std::env::var_os(name)
        .filter(|v| !v.is_empty())
        .map(PathBuf::from)
}

fn default_target_dir() -> Option<PathBuf> {
    env_path("ELANC_RS_TARGET_DIR")
        .or_else(|| env_path("XDG_CACHE_HOME").map(|c| c.join("elanc-rs/target")))
        .or_else(|| env_path("HOME").map(|h| h.join(".cache/elanc-rs/target")))
}

/// A short stable hash (FNV-1a) of a text, for unique binary names in the
/// shared target directory.
fn fnv(s: &str) -> u32 {
    let mut h: u32 = 0x811c_9dc5;
    for b in s.bytes() {
        h ^= b as u32;
        h = h.wrapping_mul(0x0100_0193);
    }
    h
}

/// Write `path` unless it already has this content (cargo then has nothing
/// to rebuild).
fn write_if_changed(path: &Path, content: &str) {
    if std::fs::read(path).ok().as_deref() == Some(content.as_bytes()) {
        return;
    }
    std::fs::write(path, content)
        .unwrap_or_else(|e| fail(format!("cannot write {}: {e}", path.display())));
}

/// The generated code formatted by `rustfmt` when it is installed (only
/// for reading: the code is already indented), else unchanged.
fn rustfmt(src: String) -> String {
    let child = Command::new("rustfmt")
        .args(["--edition", "2021", "--emit", "stdout"])
        .stdin(Stdio::piped())
        .stdout(Stdio::piped())
        .stderr(Stdio::null())
        .spawn();
    let Ok(mut child) = child else { return src };
    let mut stdin = child.stdin.take().unwrap();
    let input = src.clone();
    let writer = std::thread::spawn(move || stdin.write_all(input.as_bytes()));
    let out = child.wait_with_output();
    match (writer.join(), out) {
        (Ok(Ok(())), Ok(o)) if o.status.success() && !o.stdout.is_empty() => {
            String::from_utf8(o.stdout).unwrap_or(src)
        }
        _ => src,
    }
}

fn show(verbose: bool, cmd: &Command) {
    if verbose {
        let mut s = cmd.get_program().to_string_lossy().into_owned();
        for a in cmd.get_args() {
            s.push(' ');
            s.push_str(&a.to_string_lossy());
        }
        eprintln!("{s}");
    }
}

/// `dir/name.ext` -> (`dir`, `name`), `dir` being `.` when empty.
fn split(file: &str, ext: &str) -> (PathBuf, String) {
    let p = Path::new(file);
    let name = p
        .file_name()
        .map(|f| f.to_string_lossy().trim_end_matches(ext).to_string())
        .unwrap_or_else(|| fail(format!("{file}: not a file name")));
    let dir = p
        .parent()
        .filter(|d| !d.as_os_str().is_empty())
        .unwrap_or(Path::new("."))
        .to_path_buf();
    (dir, name)
}

fn main() {
    let o = parse_args(std::env::args().skip(1).collect());
    let (lgi_dir, prog) = split(&o.lgi, ".lgi");
    let spec = o.spec.as_ref().map(|s| {
        let (d, n) = split(s, ".spc");
        d.join(format!("{n}.spc"))
    });
    let prefix = prefix();

    // a previous program must not survive a refusal or a failure
    match std::fs::remove_file(&o.output) {
        Err(e) if e.kind() != std::io::ErrorKind::NotFound => {
            fail(format!("cannot remove {}: {e}", o.output))
        }
        _ => {}
    }

    // the work directory
    let work = PathBuf::from(format!(".elan-rs.{prog}"));
    std::fs::create_dir_all(work.join("src"))
        .unwrap_or_else(|e| fail(format!("cannot create {}: {e}", work.display())));
    let work = work
        .canonicalize()
        .unwrap_or_else(|e| fail(format!("{}: {e}", work.display())));

    // 1. export
    let elan = o
        .elan
        .clone()
        .or_else(|| {
            prefix
                .as_ref()
                .map(|p| p.join("bin/elan"))
                .filter(|p| p.is_file())
        })
        .unwrap_or_else(|| PathBuf::from("elan"));
    let ref_file = match &o.from_ref {
        Some(f) => f.clone(),
        None => {
            let ref_file = work.join(format!("{prog}.ref"));
            let _ = std::fs::remove_file(&ref_file); // a stale .ref must not be compiled
            let mut cmd = Command::new(&elan);
            cmd.arg("--cexport")
                .arg(&ref_file)
                .args(&o.elan_options)
                .arg(lgi_dir.join(format!("{prog}.lgi")));
            if let Some(s) = &spec {
                cmd.arg(s);
            }
            show(o.verbose, &cmd);
            match cmd.status() {
                Ok(s) if s.success() => {}
                Ok(s) => fail(format!("{} --cexport failed ({s})", elan.display())),
                Err(e) => fail(format!("cannot run {}: {e}", elan.display())),
            }
            ref_file
        }
    };
    let src = std::fs::read(&ref_file)
        .unwrap_or_else(|e| fail(format!("cannot read {}: {e}", ref_file.display())));

    // 2. generate
    let runtime = o
        .runtime
        .clone()
        .or_else(|| env_path("ELAN_RS_RUNTIME"))
        .or_else(|| {
            prefix
                .as_ref()
                .map(|p| p.join("share/elan-rs/elan-runtime"))
                .filter(|p| p.join("Cargo.toml").is_file())
        })
        .unwrap_or_else(|| fail("the elan-runtime crate is not found: use --runtime DIR"));
    let runtime = runtime
        .canonicalize()
        .unwrap_or_else(|e| fail(format!("runtime {}: {e}", runtime.display())));
    let bin = format!(
        "elan-{}-{:08x}",
        prog.chars()
            .map(|c| if c.is_ascii_alphanumeric() {
                c.to_ascii_lowercase()
            } else {
                '_'
            })
            .collect::<String>(),
        fnv(&work.to_string_lossy())
    );
    let krate = match elanc_rs::compile_ref(&src, &prog, &bin, &runtime.to_string_lossy()) {
        Ok(k) => k,
        Err(e @ lower::Error::Unsupported { .. }) => {
            eprintln!("elanc-rs: {prog}: {e}");
            exit(2)
        }
        Err(e) => fail(format!("{prog}: {e}")),
    };
    write_if_changed(&work.join("Cargo.toml"), &krate.cargo_toml);
    write_if_changed(&work.join("src/main.rs"), &rustfmt(krate.main_rs));
    if !o.build {
        return;
    }

    // 3. build
    let target = o
        .target_dir
        .clone()
        .or_else(default_target_dir)
        .unwrap_or_else(|| work.join("target"));
    let mut cmd = Command::new(env_path("CARGO").unwrap_or_else(|| "cargo".into()));
    cmd.args(["build", "--release", "--offline"])
        .arg("--manifest-path")
        .arg(work.join("Cargo.toml"))
        .arg("--target-dir")
        .arg(&target);
    if !o.verbose {
        cmd.arg("--quiet");
    }
    show(o.verbose, &cmd);
    let t0 = Instant::now();
    match cmd.status() {
        Ok(s) if s.success() => {}
        Ok(s) => fail(format!(
            "cargo build failed ({s}), crate in {}",
            work.display()
        )),
        Err(e) => fail(format!("cannot run cargo: {e}")),
    }
    if o.verbose {
        eprintln!("elanc-rs: build time {:.2} s", t0.elapsed().as_secs_f64());
    }

    // 4. the program
    let built = target.join("release").join(&bin);
    std::fs::copy(&built, &o.output).unwrap_or_else(|e| {
        fail(format!(
            "cannot copy {} to {}: {e}",
            built.display(),
            o.output
        ))
    });
}
