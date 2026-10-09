# ELAN

ELAN is a rewriting-logic language and environment developed at LORIA (Nancy)
from 1994 to 2004: computations are rewrite rules, and *strategies* control how
and where they are applied, including non-deterministic search with
backtracking. This repository preserves the original system and revives it.

## Layout

| Path | Purpose |
|------|---------|
| `legacy/` | verbatim archive of the historical sources (never edited) |
| `reference/` | rebuild of ELAN 3 (2004) from `legacy/` with minimal portability patches; the behavioural reference |
| `tests/legacy-bench/` | the historical test bench (778 tests), its baseline and reference snapshots |
| `src/interpreter/`, `src/lib/elanlib/` | the modern, maintained interpreter and its standard library (S1: sources still identical to the reference) |
| `tests/regression/` | targeted regression tests (one directory per case) |
| `ci/`, `.github/workflows/` | Linux image for local checks, CI on Ubuntu and macOS |

## Requirements

macOS on Apple Silicon (other Unix systems should only need path changes):

```sh
brew install gcc openjdk bdw-gc bison flex libtool cmake
```

### Case-sensitive file system

Some legacy files differ only by the case of their name
(`strategy/any.eln` / `strategy/Any.eln`, `Robot.lgi` / `robot.lgi`, ...), and
the default macOS file system is case-insensitive. Work in a case-sensitive
APFS volume:

```sh
hdiutil create -size 20g -type SPARSE -fs "Case-sensitive APFS" -volname elan ~/github/elan.sparseimage
mkdir -p ~/github/elan
hdiutil attach ~/github/elan.sparseimage -mountpoint ~/github/elan -nobrowse
cd ~/github/elan && git clone https://github.com/rewriting/elan .
```

After a reboot, only the `hdiutil attach` line is needed.

## Quick start

```sh
make                 # build the interpreter (CMake, into build/)
make check           # runner unit tests + 368 interpreter tests of the bench
make install PREFIX=/Volumes/elan-tools   # PREFIX must be case-sensitive (see above)
make reference       # build the 2004 reference system (interpreter + compiler)
make check-reference # full bench (778 tests) against the reference
```

On macOS the default compilers are Homebrew's `gcc-16`/`g++-16`; override
with `make ELAN_CC=gcc-17 ELAN_CXX=g++-17`. Linux needs
`build-essential cmake bison flex libfl-dev default-jdk-headless libgc-dev automake tcsh python3`;
`ci/Dockerfile.linux` reproduces the Linux CI job locally. See
`CONTRIBUTING.md` for the rules every change follows.

## License

ELAN is distributed under the GNU General Public License (see the headers of
the legacy sources).
