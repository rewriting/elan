#!/usr/bin/env python3
"""Unit tests of the overlay scripts (spec S9; run: python3 test_overlay.py)."""
import sys
import tempfile
import unittest
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(HERE.parent / "legacy-bench"))
import check_coverage as cc  # noqa: E402
import make_expected as me  # noqa: E402
import run_tests as rt  # noqa: E402


def overlay_test(tmp, kind="I", out="q", inp="q"):
    d = Path(tmp) / "legacy" / "App"
    d.mkdir(parents=True, exist_ok=True)
    (d / "p.lgi").write_text("lgi\n")
    ov = Path(tmp) / "overlay" / "App"
    (ov / "SAMPLES").mkdir(parents=True, exist_ok=True)
    (ov / "SAMPLES" / f"{inp}.inp").write_text("the query\n")
    return dict(id=f"App::{kind}::p:no:{inp}:{out}", dir=d, ovdir=ov, kind=kind, lgi="p",
                spc="no", inp=inp, out=out, flags=[], overlay=True, long=False)


class PlanTest(unittest.TestCase):
    def test_one_job_per_output_I_for_itest_aitest_J_for_gtest(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            i = overlay_test(tmp, "I")
            a = dict(i, kind="A", id="App::A::p:no:q:q")
            g = overlay_test(tmp, "G", out="q-c")
            h = dict(i, overlay=False, id="App::I::h:no:h:h", out="h")
            jobs = me.plan([i, a, g, h])
            self.assertEqual([(t["kind"], p.name) for t, p in jobs], [("I", "q.out"), ("J", "q-c.out")])
            self.assertEqual(jobs[0][1], i["ovdir"] / "SAMPLES" / "q.out")

    def test_an_output_shared_by_interpreter_and_compiled_lines_is_refused(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            with self.assertRaises(SystemExit):
                me.plan([overlay_test(tmp, "I"), overlay_test(tmp, "G")])


class WriteExpectedTest(unittest.TestCase):
    def fake_reference(self, tmp):
        prefix = Path(tmp) / "ref"
        (prefix / "bin").mkdir(parents=True)
        elan = prefix / "bin" / "elan"
        elan.write_text('#!/bin/sh\necho "reference $*"; cat\n')
        elan.chmod(0o755)
        return prefix

    def test_writes_then_keeps_unless_forced(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            t = overlay_test(tmp)
            old = rt.PREFIX
            rt.PREFIX = self.fake_reference(tmp)
            try:
                jobs = me.plan([t])
                self.assertEqual(me.write_expected(jobs, False, 30), (1, 0, 0))
                target = t["ovdir"] / "SAMPLES" / "q.out"
                self.assertEqual(target.read_text(), "reference -b p.lgi\nthe query\n")
                target.write_text("edited\n")
                self.assertEqual(me.write_expected(jobs, False, 30), (0, 1, 0))
                self.assertEqual(target.read_text(), "edited\n")
                self.assertEqual(me.write_expected(jobs, True, 30), (1, 0, 0))
            finally:
                rt.PREFIX = old

    def test_a_historical_output_in_legacy_is_kept(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            t = overlay_test(tmp)
            (t["dir"] / "q.out").write_text("historical\n")   # samples(dir) = dir
            old = rt.PREFIX
            rt.PREFIX = self.fake_reference(tmp)
            try:
                self.assertEqual(me.write_expected(me.plan([t]), True, 30), (0, 1, 0))
            finally:
                rt.PREFIX = old


class CoverageTest(unittest.TestCase):
    def test_every_program_in_exactly_one_state(self):
        progs = {"applications/A/p.lgi", "applications/A/q.lgi", "applications/B/r.lgi",
                 "applications/B/s.lgi"}
        problems = cc.check(progs, tested={"applications/A/p.lgi", "applications/B/s.lgi"},
                            unloadable={"applications/A/q.lgi": "ELAN 2 syntax"},
                            todo={"applications/B/s.lgi": "", "applications/C/x.lgi": ""})
        self.assertEqual(sorted(problems), [
            "applications/B/r.lgi: not tested, not in unloadable.tsv nor todo.tsv",
            "applications/B/s.lgi: in 2 states (tested, todo.tsv)",
            "applications/C/x.lgi: listed but no such program",
        ])

    def test_programs_uses_original_names_and_skips_cvs(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            root = Path(tmp)
            (root / "applications" / "A" / "CVS").mkdir(parents=True)
            (root / "applications" / "A" / "p.lgi").write_text("")
            (root / "applications" / "A" / "CVS" / "old.lgi").write_text("")
            (root / "contributions" / "C").mkdir(parents=True)
            (root / "contributions" / "C" / "c.lgi").write_text("")
            self.assertEqual(cc.programs(root), {"applications/A/p.lgi", "contributions/C/c.lgi"})

    def test_robot_upper_case_variant_has_its_original_name(self):
        progs = cc.programs(rt.ELAN3)
        self.assertIn("applications/Robot/Nim.lgi", progs)
        self.assertIn("applications/Robot/nim.lgi", progs)
        self.assertNotIn("applications/Robot/x-case-Nim.lgi", progs)


if __name__ == "__main__":
    unittest.main()
