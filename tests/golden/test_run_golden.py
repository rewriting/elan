#!/usr/bin/env python3
"""Unit tests of run_golden.py (stdlib unittest; run: python3 test_run_golden.py)."""
import os
import stat
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import run_golden as rg  # noqa: E402


class ManifestTest(unittest.TestCase):
    def test_ref_and_run_cases(self):
        cases = rg.parse_manifest(
            "# comment\n\n"
            "ref\tfib\tlegacy/x\tfib\t-\n"
            "ref\tq\tlegacy/y\tprolog\tqueens\n"
            "run\tdump\tlegacy/x\tSAMPLES/a.inp\t-b -d fib.lgi\n"
            "run\terr\tprog\t-\tprog.lgi\n")
        self.assertEqual(cases[0], dict(kind="ref", id="fib", dir="legacy/x", lgi="fib", spc=None))
        self.assertEqual(cases[1]["spc"], "queens")
        self.assertEqual(cases[2], dict(kind="run", id="dump", dir="legacy/x",
                                        stdin="SAMPLES/a.inp", args=["-b", "-d", "fib.lgi"]))
        self.assertIsNone(cases[3]["stdin"])

    def test_malformed_line_is_an_error(self):
        with self.assertRaises(rg.CaseError):
            rg.parse_manifest("ref fib legacy/x fib -\n")      # spaces, not tabs

    def test_duplicate_id_is_an_error(self):
        with self.assertRaises(rg.CaseError):
            rg.parse_manifest("ref\ta\td\tl\t-\nrun\ta\td\t-\tx.lgi\n")

    def test_id_must_be_a_file_name(self):
        with self.assertRaises(rg.CaseError):
            rg.parse_manifest("ref\ta/b\td\tl\t-\n")

    def test_the_real_manifest_parses(self):
        self.assertGreater(len(rg.parse_manifest(rg.CASES.read_text())), 0)


class NormaliseTest(unittest.TestCase):
    def test_paths_become_placeholders(self):
        s = rg.normalise("/w/x/m.eln /p/share/elanlib " + str(rg.REPO) + "/a",
                         Path("/w/x"), Path("/p"))
        self.assertEqual(s, "<WORK>/m.eln <PREFIX>/share/elanlib <REPO>/a")

    def test_times_and_speeds_are_hidden(self):
        fast = " total time\t(0.000+0.000)=0.000 sec\t(main+subprocesses)\n   15 nonamed\n"
        slow = (" total time\t(0.711+0.000)=0.711 sec\t(main+subprocesses)\n"
                "\n average speed = 92364 inf/sec\n   15 nonamed\n")
        want = " total time\t(<T>+<T>)=<T> sec\t(main+subprocesses)\n   15 nonamed\n"
        self.assertEqual(rg.normalise(fast, Path("/w"), Path("/p")), want)
        self.assertEqual(rg.normalise(slow, Path("/w"), Path("/p")), want)


class RenderTest(unittest.TestCase):
    def test_format(self):
        self.assertEqual(rg.render_run(["-b", "x.lgi"], "out\n", "", 1),
                         "$ elan -b x.lgi\n--- stdout\nout\n--- stderr\n--- exit status 1\n")

    def test_missing_final_newline_is_added(self):
        self.assertIn("--- stdout\nout\n--- stderr\nerr\n--- exit",
                      rg.render_run([], "out", "err", 0))


class CompareTest(unittest.TestCase):
    def test_missing_equal_and_different(self):
        with tempfile.TemporaryDirectory() as d:
            (Path(d) / "a.txt").write_bytes(b"same\n")
            (Path(d) / "b.txt").write_bytes(b"old\n")
            problems = dict(rg.compare({"a.txt": b"same\n", "b.txt": b"new\n",
                                        "c.txt": b""}, Path(d)))
            self.assertNotIn("a.txt", problems)
            self.assertIn("-old", problems["b.txt"])
            self.assertIn("+new", problems["b.txt"])
            self.assertIn("no golden", problems["c.txt"])


FAKE_ELAN = r"""#!/bin/sh
# fake elan: writes the .ref named after --export/--cexport, echoes the rest
while [ $# -gt 0 ]; do
  case "$1" in
    --export|--cexport) echo "$1 from $(cat m.eln)" > "$2"; shift;;
    *) echo "arg $1";;
  esac
  shift
done
cat
echo "in $PWD" >&2
exit 3
"""


class ProduceTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        t = Path(self.tmp.name)
        self.prefix = t / "prefix"
        (self.prefix / "bin").mkdir(parents=True)
        elan = self.prefix / "bin" / "elan"
        elan.write_text(FAKE_ELAN)
        elan.chmod(elan.stat().st_mode | stat.S_IXUSR)
        self.prog = t / "prog"
        self.prog.mkdir()
        (self.prog / "m.eln").write_text("module m")
        (self.prog / "input").write_text("query\n")
        self.work = t / "work"

    def tearDown(self):
        self.tmp.cleanup()

    def test_ref_case_gives_both_exports(self):
        # the fake elan exits with 3: an export must succeed
        case = dict(kind="ref", id="p", dir=str(self.prog), lgi="prog", spc=None)
        with self.assertRaises(rg.CaseError):
            rg.produce(case, self.prefix, self.work)

    def test_run_case_captures_normalised_outputs(self):
        case = dict(kind="run", id="p", dir=str(self.prog), stdin="input", args=["-b", "prog.lgi"])
        out = rg.produce(case, self.prefix, self.work)["p.txt"].decode()
        self.assertEqual(out, "$ elan -b prog.lgi\n--- stdout\narg -b\narg prog.lgi\nquery\n"
                              "--- stderr\nin <WORK>\n--- exit status 3\n")
        self.assertEqual(list(self.work.iterdir()), [])          # the copy is removed

    def test_gen_py_runs_in_the_copy(self):
        (self.prog / "gen.py").write_text("open('m.eln', 'w').write('generated')\n")
        fake = self.prefix / "bin" / "elan"
        fake.write_text(FAKE_ELAN.replace("exit 3", "exit 0"))
        case = dict(kind="ref", id="p", dir=str(self.prog), lgi="prog", spc=None)
        res = rg.produce(case, self.prefix, self.work)
        self.assertEqual(res["p.ref"], b"--export from generated\n")
        self.assertEqual(res["p.cref"], b"--cexport from generated\n")
        self.assertEqual((self.prog / "m.eln").read_text(), "module m")   # source untouched


if __name__ == "__main__":
    os.chdir(Path(__file__).resolve().parent)
    unittest.main()
