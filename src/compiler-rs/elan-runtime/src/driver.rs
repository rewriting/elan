//! The main program of a generated crate, with the command line and the
//! output of today's compiled programs (`src/compiler/runtime/main_skeleton.c`):
//!
//! ```text
//! $ ./a.out -noInput -quiet
//!
//! result = 4.7.9.2.6.1.3.5.8.10.nil
//!
//! result = ...
//!
//! rewrite_step = 3426635
//! ```
//!
//! Each result is `"\nresult = " term "\n"`; at the end `"\nrewrite_step =
//! N\n"`, then, without `-quiet`, `total time    = S sec` and `average speed
//! = R rwr/sec` (here wall-clock time; C used `getrusage` user time).
//! Options are read while arguments start with `-`; unknown ones are
//! ignored, as in C; `-help`/`-h` prints the options and exits with 1.
//!
//! Exit statuses: 0 success, 1 run-time error ([`fatal`], `-help`), 2
//! construct not supported ([`unsupported`]), 3 recursion too deep
//! ([`crate::depth`]). On an error (1, 2, 3) the results printed before
//! are written first ([`flush_output`]).

use std::cell::Cell;
use std::io::Write;
use std::sync::{Mutex, MutexGuard, TryLockError};
use std::time::Instant;

use crate::earley;
use crate::print::{Print, Writer};

thread_local! {
    static STEPS: Cell<u64> = const { Cell::new(0) };
}

/// Count one rewrite step (a rule application).
#[inline(always)]
pub fn step() {
    STEPS.with(|c| c.set(c.get() + 1))
}

/// Rewrite steps counted on this thread.
pub fn steps() -> u64 {
    STEPS.with(|c| c.get())
}

/// Command-line options of a generated program.
#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub struct Options {
    /// `-noInput`: no query is read; the start term of the `.lgi` is used.
    pub no_input: bool,
    /// `-quiet`: no timing lines.
    pub quiet: bool,
    /// `-noOutput`: results are not printed.
    pub no_output: bool,
    /// `-help` / `-h`.
    pub help: bool,
    /// Arguments after the options.
    pub rest: Vec<String>,
}

impl Options {
    /// Parse the arguments (without the program name).
    pub fn parse<I: IntoIterator<Item = String>>(args: I) -> Options {
        let mut o = Options::default();
        let mut it = args.into_iter().peekable();
        while let Some(a) = it.peek() {
            if !a.starts_with('-') {
                break;
            }
            let a = it.next().unwrap();
            match a.as_str() {
                "-noInput" => o.no_input = true,
                "-quiet" => o.quiet = true,
                "-noOutput" => o.no_output = true,
                "-help" | "-h" => o.help = true,
                // options taking a value in the C program
                "-trace" | "-sort=" | "-strategy=" | "-filequery" => {
                    it.next();
                }
                _ => {}
            }
        }
        o.rest = it.collect();
        o
    }

    pub fn from_env() -> Options {
        Options::parse(std::env::args().skip(1))
    }
}

/// The options understood, as printed by `-help`.
pub const HELP: &str = "  Options:\n    -noInput\n    -noOutput\n    -quiet\n    -help\n";

/// The output of a run: results, then statistics.
pub struct Session<W: Write> {
    pub options: Options,
    pub writer: Writer,
    out: W,
    start: Instant,
    results: u64,
}

impl<W: Write> Session<W> {
    pub fn new(options: Options, out: W) -> Session<W> {
        Session {
            options,
            writer: Writer::new(),
            out,
            start: Instant::now(),
            results: 0,
        }
    }

    /// Print one result: `\nresult = <term>\n`.
    pub fn result<P: Print + ?Sized>(&mut self, t: &P) {
        self.results += 1;
        if self.options.no_output {
            return;
        }
        self.writer.raw("\nresult = ");
        t.print(&mut self.writer);
        self.writer.raw("\n");
        let s = self.writer.take();
        let _ = self.out.write_all(&s);
    }

    /// Number of results printed so far.
    pub fn results(&self) -> u64 {
        self.results
    }

    /// Print the statistics and flush.
    pub fn finish(&mut self) {
        let n = steps();
        let _ = write!(self.out, "\nrewrite_step = {}\n", n);
        if !self.options.quiet {
            let t = self.start.elapsed().as_secs_f64();
            let _ = writeln!(self.out, "total time    = {:.3} sec", t);
            if t > 0.0 {
                let _ = writeln!(
                    self.out,
                    "average speed = {} rwr/sec",
                    (n as f64 / t) as i64
                );
            }
        }
        let _ = self.out.flush();
    }

    /// The underlying output (tests).
    pub fn into_output(self) -> W {
        self.out
    }
}

/// A run-time error of the program (the interpreter's `interr()`): the
/// results printed before are flushed ([`flush_output`]), then the message
/// on the standard error, exit status 1.
pub fn fatal(msg: &str) -> ! {
    flush_output();
    eprintln!("{msg}");
    std::process::exit(1)
}

/// The bytes written to [`ProgramOut`] and not yet to the standard output.
static OUT_BUF: Mutex<Vec<u8>> = Mutex::new(Vec::new());
/// [`ProgramOut`] writes its buffer when it holds this many bytes.
const OUT_CAP: usize = 1 << 16;

fn out_buf() -> MutexGuard<'static, Vec<u8>> {
    OUT_BUF.lock().unwrap_or_else(|p| p.into_inner())
}

fn drain(buf: &mut Vec<u8>) -> std::io::Result<()> {
    let mut o = std::io::stdout().lock();
    let r = o.write_all(buf).and_then(|_| o.flush());
    buf.clear();
    r
}

/// The standard output of a generated program ([`main_program`]): buffered
/// in a global buffer, so that [`fatal`] and [`crate::depth::too_deep`],
/// which exit from anywhere in the program, can still write the results
/// printed before ([`flush_output`]). A result is written in one call
/// ([`Session::result`]): the buffer never holds part of one.
pub struct ProgramOut;

impl Write for ProgramOut {
    fn write(&mut self, b: &[u8]) -> std::io::Result<usize> {
        let mut buf = out_buf();
        buf.extend_from_slice(b);
        if buf.len() >= OUT_CAP {
            drain(&mut buf)?;
        }
        Ok(b.len())
    }

    fn flush(&mut self) -> std::io::Result<()> {
        drain(&mut out_buf())
    }
}

/// Write what [`ProgramOut`] holds to the standard output (before an exit
/// on an error). Does nothing if the buffer is being written (a run-time
/// error cannot happen inside [`ProgramOut::write`]; never blocks).
pub fn flush_output() {
    let mut buf = match OUT_BUF.try_lock() {
        Ok(b) => b,
        Err(TryLockError::Poisoned(p)) => p.into_inner(),
        Err(TryLockError::WouldBlock) => return,
    };
    let _ = drain(&mut buf);
}

/// A construct that the compiler supports only partly met at run time:
/// the message on the standard error, exit status 2 (as a refusal of
/// `elanc-rs`).
pub fn unsupported(msg: &str) -> ! {
    flush_output();
    eprintln!("elan-runtime: not supported yet: {msg}");
    std::process::exit(2)
}

/// Stack size of the thread that runs a program: innermost normalisation
/// and continuations recurse (spec D3); the memory is reserved, not used.
/// Its recursion is bounded ([`crate::depth`]).
pub const BIG_STACK: usize = 1 << 30;

/// Run `f` on a thread with a stack of `size` bytes and return its result;
/// a panic of `f` is propagated.
pub fn run_with_stack<R: Send + 'static, F: FnOnce() -> R + Send + 'static>(
    size: usize,
    f: F,
) -> R {
    let h = std::thread::Builder::new()
        .stack_size(size)
        .spawn(f)
        .expect("spawn the main thread");
    match h.join() {
        Ok(r) => r,
        Err(e) => std::panic::resume_unwind(e),
    }
}

/// The query parser of a program (S6d): its grammar, the query's sort
/// (the start symbol) and how a query is built.
pub struct Query<Q> {
    pub tables: &'static earley::Tables,
    pub sort: i32,
    /// Builds the term of a derivation, normalised (`None`: the query's
    /// sort is not compiled, a query is parsed but not built).
    pub build: Option<fn(&earley::Tree) -> Result<Q, earley::Fatal>>,
    /// When the start term does not use the query: whether the operator
    /// of a grammar rule is not compiled (a query containing it is parsed
    /// but not built).
    pub uncompiled: Option<fn(usize) -> bool>,
}

/// The `main` of a generated program: parse the options, then run
/// `program` on a big-stack thread with a session on the standard output.
/// `program` computes and prints the results (`session.result(&t)`); the
/// statistics are printed after it returns. `idents` is the table of
/// identifiers of the program ([`crate::init_idents`]).
///
/// Without `-noInput`, the program prints the prompt (`query_sort` is the
/// sort named there), reads a term of the query's sort from the standard
/// input up to `end` ([`crate::earley`], REM's Earley parser), prints a
/// newline, then runs `program` with the term, normalised (REM's
/// `termParser`, query mode 2, as elanc-go's `Main`). A syntax error: REM's
/// message on the standard error, status 1. `uses_query`: the start term
/// uses the query; under `-noInput`, the program then prints REM's message
/// (`rem/compiler/Query.java`) and exits with status 0 without running.
pub fn main_program<Q: 'static, F>(
    query_sort: &'static str,
    idents: &'static [(i32, &'static str)],
    uses_query: bool,
    query: Option<Query<Q>>,
    program: F,
) where
    F: FnOnce(&mut Session<ProgramOut>, Option<Q>) + Send + 'static,
{
    let options = Options::from_env();
    if options.help {
        print!("{}", HELP);
        std::process::exit(1);
    }
    let code = run_with_stack(BIG_STACK, move || {
        crate::depth::set_limit(BIG_STACK);
        crate::init_idents(idents);
        let mut s = Session::new(options, ProgramOut);
        if s.options.no_input && uses_query {
            let _ = write!(
                s.out,
                "No ground term is defined\nDo not use -noInput in this case\n"
            );
            let _ = s.out.flush();
            return 0;
        }
        let mut q = None;
        if !s.options.no_input {
            let _ = writeln!(s.out, "Enter a query term of sort '{}'", query_sort);
            let _ = s.out.flush();
            let Some(query) = query else {
                eprintln!("elan-runtime: reading a query is not supported yet: the program has no query grammar; use -noInput");
                return 2;
            };
            q = read_query(&query, uses_query);
            let _ = s.out.write_all(b"\n");
        }
        program(&mut s, q);
        s.finish();
        0
    });
    std::process::exit(code);
}

/// Read the query from the standard input: the term built and normalised
/// (`None` when it is not built: its sort is not compiled, or the start
/// term does not use it and it contains an operator that is not
/// compiled); an error stops the program.
fn read_query<Q>(query: &Query<Q>, uses_query: bool) -> Option<Q> {
    let mut stderr = Vec::new();
    let stdin = std::io::stdin();
    let mut input = stdin.lock();
    let tree = earley::parse(&mut input, "standard input", query.tables, query.sort, &mut stderr);
    let _ = std::io::stderr().write_all(&stderr);
    let stop = |e: earley::Error| -> ! {
        flush_output();
        match e {
            earley::Error::Syntax => std::process::exit(1),
            earley::Error::Fatal(f) => {
                eprint!("{}", f.msg);
                std::process::exit(f.status)
            }
        }
    };
    let tree = tree.unwrap_or_else(|e| stop(e));
    let build = query.build?;
    if !uses_query {
        if let Some(u) = query.uncompiled {
            let mut finder = UncompiledFinder(u);
            if earley::build(&tree, &mut finder).unwrap_or_else(|f| stop(f.into())) {
                return None;
            }
        }
    }
    Some(build(&tree).unwrap_or_else(|f| stop(f.into())))
}

/// Finds an operator that is not compiled in a derivation: a term is true
/// when it contains one.
struct UncompiledFinder(fn(usize) -> bool);

impl earley::Builder for UncompiledFinder {
    type T = bool;
    fn op(&mut self, rule: usize, args: Vec<bool>) -> bool {
        (self.0)(rule) || args.contains(&true)
    }
    fn int(&mut self, _: i32) -> bool {
        false
    }
    fn ident(&mut self, _: i32) -> bool {
        false
    }
    fn str(&mut self, _: &[u8]) -> bool {
        false
    }
}
