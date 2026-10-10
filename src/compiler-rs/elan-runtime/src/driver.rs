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

use std::cell::Cell;
use std::io::Write;
use std::time::Instant;

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

/// Stack size of the thread that runs a program: innermost normalisation
/// and continuations recurse (spec D3); the memory is reserved, not used.
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

/// The `main` of a generated program: parse the options, then run
/// `program` on a big-stack thread with a session on the standard output.
/// `program` computes and prints the results (`session.result(&t)`); the
/// statistics are printed after it returns. `query_sort` is used for the
/// prompt when a query must be read (not supported before S6d: the program
/// then stops with an error, exit status 2).
pub fn main_program<F>(query_sort: &'static str, program: F)
where
    F: FnOnce(&mut Session<std::io::BufWriter<std::io::Stdout>>) + Send + 'static,
{
    let options = Options::from_env();
    if options.help {
        print!("{}", HELP);
        std::process::exit(1);
    }
    let code = run_with_stack(BIG_STACK, move || {
        let out = std::io::BufWriter::new(std::io::stdout());
        let mut s = Session::new(options, out);
        if !s.options.no_input {
            let _ = writeln!(s.out, "Enter a query term of sort '{}'", query_sort);
            let _ = s.out.flush();
            eprintln!("elan-runtime: reading a query is not supported yet; use -noInput");
            return 2;
        }
        program(&mut s);
        s.finish();
        0
    });
    std::process::exit(code);
}
