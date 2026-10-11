# tests/front — the Go front end against the C++ (S8a)

The Go front end (`src/compiler-go/front/...`, spec
`docs/superpowers/specs/2026-10-11-s8-go-front-end-design.md`, plan
`docs/superpowers/plans/2026-10-11-s8a-front-lexer.md`) is a port of the
interpreter's lexer, preprocessor and LR driver. Its oracle is the C++ itself,
through a harness.

## The harness `elan-lextrace`

`build/src/interpreter/elan-lextrace` (built by `make`, not installed) is
`tests/front/lextrace/elan-lextrace.cc` linked with `elan_core` and with its
own compilation of `src/interpreter/lex/mlstream.cc` under `-DELAN_LEXTRACE`:
the trace hooks there are compiled into the harness only, never into `elan`
(decision D2 of the user; `check_lextrace_hooks.sh` proves that `mlstream.cc`
preprocessed without the macro is token-identical to `main`'s).

    elan-lextrace lex FILE                                # one file
    elan-lextrace load TRACE [elan options] prog.lgi [spec]   # elan_main, every module

`lex`: a `.eln` through `mlstream` (the preprocessor, `_` not part of
identifiers, then joined by `collident`), any other file (`.lgi`, `.spc`)
through a plain `lstream`; stderr and exit status are the C++'s (a fatal
error exits with 1). `FOR EACH`/`IF` need the loader (`actgram()`, `typet`,
Earley): files that use them are read only in `load` mode.

## Trace format

One line per lexem returned by the outermost `mlstream::ilex` (calls nested
in the preprocessor — macro bodies, `FOR EACH` headers, their Earley parses —
are not written):

| line | lexem |
|---|---|
| `id <pos> <text>` | identifier, `<pos>` its position in `tabofident` |
| `num <n>` | number (>= 0) |
| `char <c>` | character, decimal code |
| `str <i> <text>` | string constant number `<i>` |
| `eos` | end of stream |
| `other <raw>` | anything else, raw code (e.g. `2147483648` wraps to -2147483648) |
| `@begin NAME` / `@end` | an `mlstream` opened (before its priming read, not traced) / closed |
| `@foreach-begin` / `@foreach-end` | a `FOR EACH`/`IF` expansion starts / has no more solution |
| `@inline <chars>` | (`lex` mode) an inline `/# ... #/` was read while reading the previous lexem |

Texts are escaped: `\\`, `\n`, `\t`, `\"`, other bytes < 32 or >= 127 as
`\ooo`. A marker is written after the lexem of the outermost call during
which it became pending.

## Scripts

| script | what |
|---|---|
| `check_lextrace_hooks.sh [BUILD]` | the guard of D2 |
| `test_lextrace_harness.sh [BUILD] [--save]` | the harness on `fixtures/*` (`<f>.expected`: stdout, `@@stderr`, stderr, `@@status N`) and on `examples/unification` in loader mode |
| `python3 test_tracecmp.py` | unit tests of `tracecmp.py` |
| `test_front_lex.sh PREFIX [--only lgi,spc]` | every file of `list_files.py`: harness and `front-trace lex` compared exactly |
| `test_front_load.sh PREFIX` | every program of `list_programs.py`: module tree of `elan` (no `-b`) and harness trace against `front-trace load` |

`tracecmp.py --mode lex|load C GO`: `lex` compares line for line; `load`
removes, and counts, only (1) on the C++ side, the lines strictly inside an
outermost `@foreach` region (S8a does not evaluate `FOR EACH`: its evaluator
gives no solution, so the Go side has the two markers and no body), (2) on
the C++ side, the `@begin strconc.eln` subtrees (`strconc[X,X,X]` is loaded
by `trclos` from a module body: S8b), (3) on both sides, the position
fields of `id`/`str` lines. A difference prints the first differing entry
with 5 lines of context and the module stack. `differs.tsv` lists the
differences approved by the user (none).

Work directories go to `tests/front/work/` (git-ignored) and are removed on
success.
