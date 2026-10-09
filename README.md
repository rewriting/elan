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
| *(to come)* `src/`, `examples/`, `tests/` | the modern, maintained implementation |

## Requirements

macOS on Apple Silicon (other Unix systems should only need path changes):

```sh
brew install gcc openjdk bdw-gc bison flex libtool
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

## Quick start (reference build)

```sh
reference/build.sh
source reference/env.sh
tests/legacy-bench/run_tests.py          # replays the 778 historical tests
```

## License

ELAN is distributed under the GNU General Public License (see the headers of
the legacy sources).
