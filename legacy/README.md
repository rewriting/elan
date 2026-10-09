# ELAN — legacy sources (archive)

This directory is a **verbatim archive** of the historical ELAN sources
(LORIA, Nancy, 1994–2004), kept for the record. Nothing here is modified:
files, CVS metadata, generated files, old object files and dangling symbolic
links are exactly as found in the original `ELAN.tgz` backup.

Do not edit these files. Work happens in the modern project next to this
directory; the reference build (`../reference/`) applies its patches to a
*copy* of these sources.

## Contents

| Path | What it is |
|------|------------|
| `elan3/src/` | ELAN 3 system (CVS checkout, 2003–2004): interpreter `elan-interpreter` (3.6g, C++), compiler `elan-compiler` (REM 4.3, Java + C runtime), choice-point library `cpl` (0.6), standard library `elan-library`, `gc6.2` |
| `elan3/applications/`, `elan3/contributions/` | ELAN applications and their historical test bench (`tst_all`, `SAMPLES/*.inp`, `SAMPLES/*.out`) |
| `elan3/doc/`, `elan3/tools/` | documentation and tools |
| `elan/src/aterm-1.6.5/` | ATerm library 1.6.5 (CWI), required by cpl and the compiler |
| `elan/src/elan-library/` | an older (2001–2002) ELAN standard library; most reference outputs of the test bench were produced with libraries of this period |
| `elan/sources/Scripts/` | the original test drivers `itest`, `aitest`, `jtest`, ... |
| `elan/sources/Compiler.{2.1,3.0,3.2,4.0}/` | compiler test suites (and the sources of those compiler generations) |

## Notes

* Some files differ only by the case of their name (`strategy/any.eln` and
  `strategy/Any.eln`, `Robot.lgi` and `robot.lgi`, ...). They are distinct
  files: this repository must be checked out on a **case-sensitive** file
  system (see the top-level README).
* Not archived from the original backup: unrelated projects (Tom, aircube,
  CWI tools, ...), prebuilt binaries for long-gone platforms, and
  administrative files containing account data.
