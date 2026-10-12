#!/bin/sh
# Copy the repository's tracked files into DEST with the original names of
# the files listed in CASE-RENAMES.tsv: the layout of the imported archives
# (legacy/) as they were. DEST must be on a case-sensitive file system.
# Usage: tests/restore_case.sh DEST
DEST=${1:?usage: restore_case.sh DEST}
cd "$(dirname "$0")/.." || exit 2
mkdir -p "$DEST" || exit 2
git ls-files -z | tar --null -T - -cf - | tar -xf - -C "$DEST" || exit 1
grep -v '^#' CASE-RENAMES.tsv | while IFS="$(printf '\t')" read -r stored orig; do
  [ -n "$stored" ] || continue
  mv "$DEST/$stored" "$DEST/$orig" || exit 1
done
