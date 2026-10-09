# S4 — Library, examples and manual: Implementation Plan

> Inline execution (superpowers:executing-plans); the examples task may be delegated and verified.

**Spec:** `docs/superpowers/specs/2026-10-10-s4-library-examples-docs-design.md`

## Global Constraints
- Branch `s4-library-examples-docs`; every commit keeps `make check` (Clang) 0/0; before merge also GCC check and `make check-sanitize`.
- `legacy/`, `reference/` untouched; snapshots/baseline untouched.
- Commit messages: behaviour change line + `Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>`.

## Review Focus
1. Exact-case lookup must not break lookups that worked (relative paths, `../`, absolute paths, the current directory) → unit tests + bench.
2. Moving `Any.eln` must not change which file any program loads on a case-sensitive FS → bench 0/0.
3. Example expected outputs must come from the reference interpreter, and modern == reference.
4. The manual build must be reproducible from a clean checkout (`make manual` in a fresh clone).

### Task 1: Manual (D1)
- [ ] Copy the needed sources from `legacy/elan3/doc` into `docs/manual/` (all .tex, .sty, .bib/.bbl/.ind/.idx, figures, ElanExamples/, elanlibTex/, examplesTex/, Makefile-free); add `fancyhead.sty`, `lgrind.sty` shims; patch `macros.tex` (`\insertCode` → `\VerbatimInput`) and `manual.tex` (`fancyvrb`, fixed date); `docs/manual/README.md` documents the adaptations.
- [ ] Top-level `make manual`: `latex -interaction=nonstopmode` ×3 and `dvipdf` in a build dir (`build-manual/`), copy `manual.pdf` to `docs/manual/`; fails if latex reports an error (`! ` lines).
- [ ] Verify page count (~99) and commit sources + PDF.

### Task 2: Exact-case module lookup (D2)
- [ ] Unit test (RED): in a temporary directory on the default macOS volume (case-insensitive), create `Foo.eln`; opening `foo.eln` through the library lookup function must fail, opening `Foo.eln` must succeed (skip the case-insensitive part when the temp dir is case-sensitive).
- [ ] Implement in the library search (`base/ichstream.cc`): after a successful `fopen` of a library candidate, check the directory entry has exactly the requested basename (`opendir/readdir` of the parent, byte comparison); otherwise close and continue the search.
- [ ] `git mv src/lib/elanlib/strategy/Any.eln src/lib/elanlib/common/Any.eln`; `make check` 0/0.
- [ ] Commit.

### Task 3: No case collisions + install guard (D3)
- [ ] Check (RED first with a fake collision) in tests/architecture: no two paths under `src/lib/elanlib`, `examples` differing only by case; run by `make check-arch`.
- [ ] Remove the case-insensitive install guard of `make install`; README updated (install on any file system; the repository itself still needs a case-sensitive checkout because of `legacy/`).
- [ ] Commit.

### Task 4: Examples (D4)
- [ ] For each `.lgi` of `legacy/elan3/doc/ElanExamples`: `examples/<name>/` with its files, `input.inp` (queries from the manual text, `docs/manual/*.tex`), `expected.out` from `reference/install/bin/elan -b <name>.lgi < input.inp`; skip (and list in examples/README.md) examples that need the compiler or are not runnable with the interpreter.
- [ ] Runner: discover `examples/<name>/` (logic `<name>.lgi`, `input.inp`, `expected.out`) like regression cases; `make check` runs them; modern output identical to expected.
- [ ] `examples/README.md`: index with one line per example and the manual section.
- [ ] Commit.

### Task 5: Docs
- [ ] README: "Getting started" (build, run an example, read the manual), link to `docs/manual/manual.pdf`, layout table updated. Commit; review; merge.
