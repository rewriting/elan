#!/bin/sh
# No two paths of the repository may differ only by case (files or
# directories): the tree must check out on case-insensitive file systems
# (macOS, Windows). A file that must keep such a name is stored renamed and
# listed in CASE-RENAMES.tsv; this script also checks that every line there
# names a stored file that exists and an original that does not.
cd "$(dirname "$0")/.." || exit 2
git ls-files | python3 -c '
import sys, collections
files = [l.rstrip("\n") for l in sys.stdin]
names = collections.defaultdict(set)
for f in files:
    parts = f.split("/")
    for i in range(1, len(parts) + 1):
        p = "/".join(parts[:i])
        names[p.lower()].add(p)
bad = sorted(sorted(v) for v in names.values() if len(v) > 1)
for v in bad:
    print("check_case: paths differing only by case:", " ".join(v))
tracked = set(files)
for line in open("CASE-RENAMES.tsv"):
    if line.strip() and not line.startswith("#"):
        stored, orig = line.rstrip("\n").split("\t")
        if stored not in tracked:
            print("check_case: CASE-RENAMES.tsv: stored file not tracked:", stored); bad.append(1)
        if orig in tracked:
            print("check_case: CASE-RENAMES.tsv: original name still tracked:", orig); bad.append(1)
sys.exit(1 if bad else 0)
' && echo "check_case: ok ($(git ls-files | wc -l | tr -d ' ') paths)"
