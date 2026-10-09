# S3b — Interpreter internals

Date: 2026-10-10 · Status: decided autonomously (user: keep developing;
macOS verification only). Based on a read-only study of limits, the REF
contract, parser state, errors and term ownership.

## 1. Key fact

`stringtab` positions are hash positions and *are* the codes of identifiers,
sorts, modules, rule and strategy names; these codes, the symbol codes
(user symbols from `FSYMCODESBEG`=300, builtins below) and the 12-bit
`defstrat` packing (`FSYM_FLAG`/`LAB_FLAG`/`DSTR_FLAG`) appear in `.ref` files,
in REM and in the compiled runtime (which keeps copies of `MAXNOFIDENT`,
`NTYPES`). The bench compares only stdout: it would not notice a renumbering.

## 2. Goal and success criteria

1. **Golden tests** that pin what the bench does not see: the exported `.ref`
   files (byte-for-byte) for a set of bench programs, the `-d` dump, the
   statistics output, and stderr + exit status of deliberate errors; unit
   tests of the lexem ranges and of the defstrat packing.
2. **Contract guards**: limits in one header with `static_assert`s (symbol
   codes < 4096, sort range above `DEFAULTSYM`, `NNONTERMINALS`), a check that
   the compiler's copies agree with the interpreter, the REF contract
   documented.
3. **No silent overflow**: every fixed array that could overflow silently
   (`anys`, `symbappl`, `Gtypestack`, the `.ref` parser stacks, `arities`)
   fails with a message.
4. **Dynamic internal stacks** (not encodings): term construction stack
   (`MAXTERMDEEP`), `MAXINCLSTRAT`, `MAXPROFISTCK`, `MAXSPAIR`, string
   constants storage, `MAXNESTMAC`, include depth (`MAXINCLDEEP`).
5. **Parser state grouped**: the loader's scalars in one global struct (same
   sharing semantics); the four per-level arrays in a vector of module frames.
6. **Fatal errors as an exception** caught in `elan_main` (same messages,
   same exit status 1, subprocesses killed); forked children keep exiting
   directly; ^C (longjmp) untouched.
7. `make check`, GCC check, `make check-sanitize`, and the new golden tests
   stay green; no snapshot changes.

## 3. Not changed (contract)

Hashed table sizes (`MAXNOFIDENT`, `NTYPES`, `MAXNOFIMPORTS`, `MAXNOFTRN`,
`MAXNOFSTRAT`, `MRWTSIZE`, `MAXNOFMAC`), `FSYMCODESBEG`, builtin codes, the
`defstrat` packing, `IDLEN` truncation (49 characters), the `MAXLENNTERM`
default (printed by `--help`). Term ownership stays manual (`term` is a
non-owning handle stored in malloc'd structs): documented, no RAII.

## 4. Also

Fix the double `fclose` of `earleyOut` in the driver (`-c` path).
