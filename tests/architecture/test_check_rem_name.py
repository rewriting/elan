#!/usr/bin/env python3
"""Unit tests of check_rem_name.py (stdlib unittest)."""
import shutil
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import check_rem_name as cr  # noqa: E402


class BareCallsTest(unittest.TestCase):
    def test_calls_are_found(self):
        text = ('elanc -b -nosplit -quiet x\n'
                'run(["elanc", *opts])\n'
                'if [ -x "$(PREFIX)/bin/elanc" ]; then\n'
                "HAVE_REM=; command -v elanc >/dev/null\n")
        self.assertEqual([n for n, _ in cr.bare_calls(text)], [1, 2, 3, 4])

    def test_defaults_of_parameters_are_found(self):
        text = ('ELANC=${ELANC:-elanc}\n'
                'E=${ELANC-elanc}; F=${X:=elanc}\n'
                ': "${ELANC:=elanc}"\n'
                'ELANC=${ELANC:-elanc-go}\n')
        self.assertEqual([n for n, _ in cr.bare_calls(text)], [1, 2, 3])

    def test_a_hash_inside_quotes_is_not_a_comment(self):
        text = ('echo "step #1"; elanc -b x\n'
                "echo 'a #b' && elanc x\n"
                'x = "#" ; run(["elanc"])\n'
                'echo "a \\" #" ; elanc x\n'
                'echo "#ok" # elanc in a comment\n')
        self.assertEqual([n for n, _ in cr.bare_calls(text)], [1, 2, 3, 4])

    def test_other_names_comments_and_marker_are_not(self):
        text = ('elanc-rem -b x\n'
                'elanc-go -b x; elanc-rs x\n'
                '$ELANC -v x >$ELANC.log; cat elanc.log; ls .elan.enum\n'
                '# elanc -b x (a comment)\n'
                '   # indented comment: elanc\n'
                'rm -f bin/elanc  # bare-elanc-ok: the install removes it\n'
                'x = "elanc-rem"   # REM, whatever bin/elanc is\n')
        self.assertEqual(cr.bare_calls(text), [])


class CheckTest(unittest.TestCase):
    def setUp(self):
        self.root = Path(tempfile.mkdtemp())

    def tearDown(self):
        shutil.rmtree(self.root)

    def write(self, rel, text):
        p = self.root / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text)

    def test_scripts_are_checked_data_and_work_are_not(self):
        self.write("Makefile", "check:\n\telanc x\n")
        self.write("tests/a/t.sh", "elanc x\n")
        self.write("tests/a/r.py", "x = 'elanc-rem'\n")
        self.write("tests/a/programs/p.sh", "elanc x\n")
        self.write("tests/a/work/w.sh", "elanc x\n")
        self.write("tests/a/README.md", "elanc x\n")
        self.write(".github/workflows/ci.yml", "run: elanc x\n")
        problems = cr.check(self.root)
        self.assertEqual(sorted(p.split(":")[0] for p in problems),
                         [".github/workflows/ci.yml", "Makefile", "tests/a/t.sh"])


if __name__ == "__main__":
    unittest.main()
