#!/usr/bin/env python3
"""Make the living copy of the ELAN 3 applications (spec S9 §8), once:
legacy/elan3/applications -> applications/, legacy/elan3/contributions ->
contributions/, without CVS/ and build products; each tst_all (csh)
becomes a TESTS file (format: tests/apps/README.md); the overlay pilot of
tests/legacy-overlay/elan3/ is folded in; the Robot case collisions are
resolved for good (RENAMES). Refuses to overwrite an existing copy.
Usage: import_legacy.py"""
import re
import shutil
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parent.parent
sys.path.insert(0, str(REPO / "tests" / "legacy-bench"))
import run_tests as rt  # noqa: E402

IGNORE = shutil.ignore_patterns("CVS", "*.o", "a.out", ".elan.*", ".elan-go.*",
                                "*.make", "*.ref", "tst_all", "tst_all.overlay",
                                "tst_one", "PORTING.md")
# Robot: Nim.eln/nim.eln, Robot.lgi/robot.lgi... are two variants of a
# program; the capitalised one (stored as x-case-*, CASE-RENAMES.tsv) is
# renamed with a 2 (module, LPL and file names, imports, test lines).
ROBOT = Path("applications/Robot")
RENAMES = {"Nim": "nim2", "Robot": "robot2", "Space": "space2", "State": "state2"}
HEADER = "# tests of {name} (from legacy/elan3 tst_all; format: tests/apps/README.md)\n"


def tests_lines(tst_all, renames):
    """The test lines of a csh tst_all, in the TESTS format; renames apply
    to the LGI, INP and OUT fields."""
    lines = []
    for cmd, a, long_ in rt.parse_tst_all(tst_all):
        names = (0, 2) if cmd == "jtest" else (0, 2, 3)
        a = [renames.get(w, w) if i in names else w for i, w in enumerate(a)]
        lines.append(("long " if long_ else "") + " ".join([cmd, *a]))
    return lines


# The reader of TESTS files is the bench's (run_tests imports nothing of
# this module: no circular import).
parse_tests = rt.parse_tests


def rename_robot(d):
    """In the copy of Robot (d): x-case-X.* -> x2.* (RENAMES), contents
    of the renamed files rewritten (module/LPL names, imports); Quick.eln,
    used only by nim2, imports State: it follows."""
    word = re.compile(r"\b(" + "|".join(RENAMES) + r")\b")
    for p in sorted(d.rglob("x-case-*")):
        stem, _, ext = p.name[len("x-case-"):].partition(".")
        new = p.with_name(f"{RENAMES[stem]}.{ext}")
        p.rename(new)
        if ext in ("eln", "lgi"):
            new.write_text(word.sub(lambda m: RENAMES[m.group(1)], new.read_text()))
    quick = d / "Quick.eln"
    if quick.exists():
        quick.write_text(word.sub(lambda m: RENAMES[m.group(1)], quick.read_text()))


def import_tree(src, dst, overlay):
    if dst.exists():
        raise SystemExit(f"{dst} exists: the copy is made once")
    shutil.copytree(src, dst, symlinks=True, ignore=IGNORE)
    for tst in sorted(src.rglob("tst_all")):
        if tst.parent == src or "CVS" in tst.parts:
            continue
        rel = tst.parent.relative_to(src)
        renames = RENAMES if Path(dst.name) / rel == ROBOT else {}
        lines = tests_lines(tst, renames)
        ov = overlay / rel / "tst_all.overlay" if overlay else None
        if ov is not None and ov.exists():
            lines += tests_lines(ov, renames)
        (dst / rel / "TESTS").write_text(HEADER.format(name=rel.as_posix())
                                         + "".join(f"{l}\n" for l in lines))
    if overlay is not None and overlay.is_dir():
        for f in sorted(overlay.rglob("*")):
            if f.is_file() and f.name not in ("tst_all.overlay", "PORTING.md"):
                target = dst / f.relative_to(overlay)
                target.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(f, target)
    if (dst / "Robot").is_dir():
        rename_robot(dst / "Robot")


def main():
    ov = REPO / "tests" / "legacy-overlay" / "elan3"
    for top in ("applications", "contributions"):
        import_tree(rt.ELAN3 / top, REPO / top, ov / top)
        print(f"import_legacy: {top}/ made")
    return 0


if __name__ == "__main__":
    sys.exit(main())
