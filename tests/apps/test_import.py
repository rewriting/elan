#!/usr/bin/env python3
"""Unit tests of the importer of the living copy (spec S9 §8; run: python3 test_import.py)."""
import tempfile
import unittest
from pathlib import Path

import import_legacy as il


class TestsFileTest(unittest.TestCase):
    def test_tst_all_lines_become_tests_lines(self):
        with tempfile.TemporaryDirectory() as tmp:
            t = Path(tmp) / "tst_all"
            t.write_text("#! /bin/csh -f\nif ($1 =~ *I*) then\necho \"Interpreter\"\n"
                         "itest p no q r\nif ($1 =~ *L*) then\nitest p s q2 r2\nendif\nendif\n"
                         "if ($1 =~ *J*) then\njtest p no jr\nendif\n")
            self.assertEqual(il.tests_lines(t, {}),
                             ["itest p no q r", "long itest p s q2 r2", "jtest p no jr"])

    def test_renames_apply_to_lgi_inp_out_not_spc(self):
        with tempfile.TemporaryDirectory() as tmp:
            t = Path(tmp) / "tst_all"
            t.write_text("itest Nim non Nim Nim\naitest nim non nim nim\n")
            self.assertEqual(il.tests_lines(t, {"Nim": "nim2"}),
                             ["itest nim2 non nim2 nim2", "aitest nim non nim nim"])

    def test_parse_tests(self):
        with tempfile.TemporaryDirectory() as tmp:
            t = Path(tmp) / "TESTS"
            t.write_text("# comment\nitest p no q r\n\nlong aitest p s q r -flag  # why\n")
            self.assertEqual(list(il.parse_tests(t)),
                             [("itest", ["p", "no", "q", "r"], False),
                              ("aitest", ["p", "s", "q", "r", "-flag"], True)])


    def test_parse_tests_refuses_an_unknown_command(self):
        with tempfile.TemporaryDirectory() as tmp:
            t = Path(tmp) / "TESTS"
            t.write_text("itest p no q r\nitset p no q r\n")
            with self.assertRaises(SystemExit) as e:
                list(il.parse_tests(t))
            self.assertIn(f"{t}:2", str(e.exception))
            self.assertIn("itset", str(e.exception))

class ImportTreeTest(unittest.TestCase):
    def test_copy_cleans_converts_and_folds_the_overlay(self):
        with tempfile.TemporaryDirectory() as tmp:
            src, dst, ov = Path(tmp) / "src", Path(tmp) / "dst", Path(tmp) / "ov"
            app = src / "applications" / "App"
            (app / "CVS").mkdir(parents=True)
            (app / "SAMPLES").mkdir()
            (app / "p.lgi").write_text("LPL p description end\n")
            (app / "p.o").write_text("x")
            (app / "p.ref").write_text("x")
            (app / "SAMPLES" / "q.inp").write_text("q end\n")
            (app / "tst_all").write_text("itest p no q r\n")
            (ov / "applications" / "App" / "SAMPLES").mkdir(parents=True)
            (ov / "applications" / "App" / "tst_all.overlay").write_text("gtest p no q r-c\n")
            (ov / "applications" / "App" / "SAMPLES" / "r-c.out").write_text("out\n")
            il.import_tree(src / "applications", dst / "applications", ov / "applications")
            out = dst / "applications" / "App"
            self.assertTrue((out / "p.lgi").exists())
            self.assertFalse((out / "CVS").exists() or (out / "p.o").exists()
                             or (out / "p.ref").exists() or (out / "tst_all").exists())
            self.assertEqual((out / "TESTS").read_text(),
                             "# tests of App (from legacy/elan3 tst_all; format: tests/apps/README.md)\n"
                             "itest p no q r\ngtest p no q r-c\n")
            self.assertEqual((out / "SAMPLES" / "r-c.out").read_text(), "out\n")

    def test_robot_rename_follows_into_quick(self):
        with tempfile.TemporaryDirectory() as tmp:
            d = Path(tmp) / "Robot"
            d.mkdir()
            (d / "x-case-State.eln").write_text("module State\nend\n")
            (d / "Quick.eln").write_text("module Quick\nimport global State list[state];\n")
            il.rename_robot(d)
            self.assertEqual((d / "state2.eln").read_text(), "module state2\nend\n")
            self.assertIn("import global state2 ", (d / "Quick.eln").read_text())


if __name__ == "__main__":
    unittest.main()
