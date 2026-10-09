# S4 — Library, examples and manual

Date: 2026-10-10 · Status: decided autonomously (user: keep developing; macOS
verification only). S3b (interpreter internals) is postponed: it is deep,
risky work with little benefit for the "executable heritage" goal right now,
whereas S4 makes the project usable by someone who discovers it.

## 1. Goal and success criteria

1. **Manual**: the ELAN 3.6 user manual (LaTeX, 2003) is rebuilt from source
   (`make manual` → `docs/manual/manual.pdf`, ~99 pages); the PDF is committed.
2. **Library**: `src/lib/elanlib` has no file names differing only by case, so
   ELAN installs and runs on a default (case-insensitive) macOS file system;
   module lookup by the interpreter is exact-case.
3. **Examples**: `examples/` holds the examples of the manual
   (`legacy/elan3/doc/ElanExamples`), each runnable with documented queries and
   expected outputs; `make check` runs them.
4. `make check`, GCC check and `make check-sanitize` stay 0/0 (snapshots
   unchanged).

## 2. Decisions

| # | Decision | Reason |
|---|----------|--------|
| D1 | `docs/manual/` is a copy of the LaTeX sources of `legacy/elan3/doc` with three documented adaptations: `fancyhead.sty` → shim loading `fancyhdr`; `lgrind.sty` (removed from TeX Live) → shim loading the `lgrind-ck.sty` shipped with the 2003 doc; listings (formerly `ElanTeX/*.tex` produced by the `lgrind` program, which no longer exists) included verbatim with `fancyvrb`. Built with `latex` ×3 + `dvipdf` (the figures are MetaPost/EPS, not accepted by pdflatex). The title page date is fixed ("V3.6, rebuilt 2026") instead of `\today`, for reproducible output. | Faithful content; only the tooling that vanished is replaced. |
| D2 | Library collision `strategy/any.eln` (module `any[X]`) vs `strategy/Any.eln` (module `Any`): `Any.eln` moves to `common/`. The loader (`ichstream`, library search) accepts a file only if its name matches the requested name **exactly** (byte comparison of the directory entry), so on a case-insensitive file system `any.eln` no longer opens `common/Any.eln`. | Both modules keep their names; behaviour is unchanged on case-sensitive systems (the bench proves it) and becomes correct on case-insensitive ones. |
| D3 | The install guard against case-insensitive prefixes (S1) is removed once the library has no collision; a check (`tests/architecture`) fails if a case collision is reintroduced in `src/lib/elanlib` or `examples/`. | Installation on a normal Mac. |
| D4 | `examples/<name>/` = the example's `.lgi`/`.eln` files, `README.md` (what it shows, how to run it, manual section), `input.inp` (queries taken from the manual) and `expected.out` produced by the **reference** interpreter (2004 build) and checked identical with the modern one. The bench runner discovers them (one `I` test per example, logic `<name>.lgi`). | Examples are documentation that cannot rot. |

## 3. Out of scope

Rewriting the manual in another format; the compiler chapter's examples (S5);
interpreter internals (S3b).
