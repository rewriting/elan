# S5a — Compiler foundations: Implementation Plan

> Inline execution; the import/build task may be delegated to a subagent and verified by the controller.

**Spec:** `docs/superpowers/specs/2026-10-10-s5a-compiler-foundations-design.md`

## Global Constraints
- Branch `s5a-compiler`; `make check` (Clang) 0/0 at every commit; before merge GCC check and check-sanitize too. macOS only.
- `legacy/`, `reference/` untouched (the reference keeps its own aterm build).
- Commit messages: behaviour change line + `Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>`.

## Review Focus
1. The modern compiled programs must behave exactly like the reference ones (J/JO snapshots), including the GC/stack-copying fix of patch 05.
2. Removing the aterm variant must not remove code the base variant uses (shared runtime files compiled with -DBASETERM).
3. elanc in sh must reproduce the tcsh option handling (option with arguments, .lgi/.spc positional rules, -quiet).
4. Installed layout: REM finds headers/libs through `-DPREFIX` → must point to the modern install prefix.

### Task 1: Import (D1)
- [ ] Copy into `src/compiler/`: `rem/` (legacy elan-compiler/src/rem + patches 03, 04, 06 applied), `runtime/` (legacy runtime + patch 02, base variant files only — keep files used by `-DBASETERM` builds), `earley/`, `cpl/` (choice.c/choice.h + patch 05), `scripts/elanc.src`. Remove generated/binary files (y.tab.*, lex.yy.c are generated from ref.yacc/ref.lex: regenerate in the build; .class files; .o).
- [ ] Verify with diff against `reference/build/elan3/src/...` (same content for imported files).
- [ ] Commit.

### Task 2: Build (D2)
- [ ] CMake `src/compiler/CMakeLists.txt`: Java (find_package(Java) + add_custom_command javac → `classes/`), runtime static lib `runtime-base` (the source list and -D flags of the reference's base variant, from runtime/Makefile.am), `earley` (C++), `choice` (cpl); bison/flex for ref.yacc/ref.lex; install to `$PREFIX/{lib,include/elan-compiler,classes,bin}` like the reference install layout.
- [ ] `elanc` generated from `elanc.src` (`__PREFIX__` → install prefix).
- [ ] Smoke: `elanc enum` + `make -f enum.make` + `./a.out -noInput` on the enum example with the modern install → same output as the reference.
- [ ] Commit.

### Task 3: elanc in sh (D3)
- [ ] Rewrite `scripts/elanc.src` in POSIX sh; compare generated `.ref`/`.make`/C files and outputs with the tcsh version on several bench programs (different options: -nosplit, -optimiseChoicePoint, spc argument).
- [ ] `-aterm`/`-atermns`: error message, exit 1.
- [ ] Commit.

### Task 4: Tests (D4)
- [ ] `make check-compiler`: `run_tests.py --prefix $(PREFIX) --kinds J,JO`; add to `make check`.
- [ ] 0 regressions / 0 snapshot differences; commit; README (compiler section, getting started with `elanc`).
