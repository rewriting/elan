#!/usr/bin/env python3
"""Every program (.lgi) of legacy/elan3/applications and contributions is
tested (a tst_all or tst_all.overlay line names it), or listed in
unloadable.tsv (path<TAB>reason), or still to do (todo.tsv) — spec S9 §3.1.
Fails on a program in none of these states or in two, and on a listed path
that names no program. Paths are relative to legacy/elan3, with original
names (CASE-RENAMES.tsv).
Usage: check_coverage.py [--write-todo]
       --write-todo: list in todo.tsv every program neither tested nor
       unloadable (used once, when S9 starts; afterwards the list shrinks)."""
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "legacy-bench"))
import run_tests as rt  # noqa: E402

TREES = ("applications", "contributions")
UNLOADABLE = HERE / "unloadable.tsv"
TODO = HERE / "todo.tsv"


def programs(root):
    orig = {s: o for s, o in rt.CASE_RENAMES}
    found = set()
    for top in TREES:
        for p in (root / top).rglob("*.lgi"):
            if "CVS" in p.parts:
                continue
            found.add(orig.get(p, p).relative_to(root).as_posix())
    return found


def tested(tests, root):
    return {(t["dir"] / f"{t['lgi']}.lgi").relative_to(root).as_posix()
            for t in tests if root in t["dir"].parents}


def load_list(path):
    entries = {}
    if path.exists():
        for line in path.read_text().splitlines():
            if line.strip() and not line.startswith("#"):
                first, _, rest = line.partition("\t")
                entries[first.strip()] = rest.strip()
    return entries


def check(progs, tested, unloadable, todo):
    problems = []
    for p in sorted(progs):
        states = [n for n, s in (("tested", tested), ("unloadable.tsv", unloadable),
                                 ("todo.tsv", todo)) if p in s]
        if not states:
            problems.append(f"{p}: not tested, not in unloadable.tsv nor todo.tsv")
        elif len(states) > 1:
            problems.append(f"{p}: in {len(states)} states ({', '.join(states)})")
    for p in sorted((set(unloadable) | set(todo)) - progs):
        problems.append(f"{p}: listed but no such program")
    return problems


def main(argv):
    progs = programs(rt.ELAN3)
    done = tested(rt.discover(), rt.ELAN3)
    unloadable, todo = load_list(UNLOADABLE), load_list(TODO)
    if argv == ["--write-todo"]:
        rest = sorted(progs - done - set(unloadable))
        TODO.write_text("# programs of S9 still to test (spec S9 §5); check_coverage.py\n"
                        + "".join(f"{p}\n" for p in rest))
        print(f"check_coverage: {len(rest)} programs written to {TODO.name}")
        return 0
    problems = check(progs, done & progs, unloadable, todo)
    for p in problems:
        print(f"check_coverage: {p}")
    print(f"check_coverage: {len(progs)} programs: {len(done & progs)} tested, "
          f"{len(unloadable)} unloadable, {len(todo)} to do"
          + ("" if not problems else f", {len(problems)} problems"))
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
