#!/usr/bin/env python3
"""Unit tests of the tools of tests/apps (spec S9; run: python3 test_tools.py)."""
import contextlib
import io
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
import triage as tr  # noqa: E402


def copy_test(tmp, kind="I", out="q", inp="q"):
    """A test of a living-copy directory App with p.lgi and SAMPLES/q.inp."""
    d = Path(tmp) / "applications" / "App"
    (d / "SAMPLES").mkdir(parents=True, exist_ok=True)
    (d / "p.lgi").write_text("LPL p description end\n")
    (d / "SAMPLES" / f"{inp}.inp").write_text("q end\n")
    return dict(id=f"applications/App::{kind}::p:no:{inp}:{out}", dir=d, kind=kind, lgi="p",
                spc="no", inp=inp, out=out, flags=[], copy=True, long=False)


class PlanTest(unittest.TestCase):
    def test_one_job_per_output_I_for_itest_aitest_J_for_gtest(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            i = copy_test(tmp, "I")
            a = dict(i, kind="A", id="App::A::p:no:q:q")
            g = copy_test(tmp, "G", out="q-c")
            h = dict(i, copy=False, id="App::I::h:no:h:h", out="h")
            jobs = me.plan([i, a, g, h])
            self.assertEqual([(t["kind"], p.name) for t, p in jobs], [("I", "q.out"), ("J", "q-c.out")])
            self.assertEqual(jobs[0][1], i["dir"] / "SAMPLES" / "q.out")

    def test_a_test_whose_out_exists_gets_no_job_to_write_it(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            t = copy_test(tmp)
            (t["dir"] / "SAMPLES" / "q.out").write_text("done\n")
            old = rt.PREFIX
            rt.PREFIX = Path(tmp) / "none"
            try:
                with contextlib.redirect_stdout(io.StringIO()):
                    self.assertEqual(me.write_expected(me.plan([t]), False, 30), (0, 1, 0))
            finally:
                rt.PREFIX = old

    def test_an_output_shared_by_interpreter_and_compiled_lines_is_refused(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            with self.assertRaises(SystemExit):
                me.plan([copy_test(tmp, "I"), copy_test(tmp, "G")])


class WriteExpectedTest(unittest.TestCase):
    def fake_reference(self, tmp):
        prefix = Path(tmp) / "ref"
        (prefix / "bin").mkdir(parents=True)
        elan = prefix / "bin" / "elan"
        elan.write_text('#!/bin/sh\necho "reference $*"; cat\n')
        elan.chmod(0o755)
        return prefix

    def write(self, jobs, force):
        with contextlib.redirect_stdout(io.StringIO()):
            return me.write_expected(jobs, force, 30)

    def test_a_reference_run_with_a_nonzero_rc_is_a_failure_not_an_output(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            t = copy_test(tmp)
            prefix = Path(tmp) / "ref"
            (prefix / "bin").mkdir(parents=True)
            (prefix / "bin" / "elan").write_text("#!/bin/sh\necho partial; exit 4\n")
            (prefix / "bin" / "elan").chmod(0o755)
            old = rt.PREFIX
            rt.PREFIX = prefix
            buf = io.StringIO()
            try:
                with contextlib.redirect_stdout(buf):
                    r = me.write_expected(me.plan([t]), False, 30)
            finally:
                rt.PREFIX = old
            self.assertEqual(r, (0, 0, 1))
            self.assertIn("rc=4", buf.getvalue())
            self.assertFalse((t["dir"] / "SAMPLES" / "q.out").exists())

    def test_writes_then_keeps_unless_forced(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            t = copy_test(tmp)
            old = rt.PREFIX
            rt.PREFIX = self.fake_reference(tmp)
            try:
                jobs = me.plan([t])
                self.assertEqual(self.write(jobs, False), (1, 0, 0))
                target = t["dir"] / "SAMPLES" / "q.out"
                self.assertEqual(target.read_text(), "reference -b p.lgi\nq end\n")
                target.write_text("edited\n")
                self.assertEqual(self.write(jobs, False), (0, 1, 0))
                self.assertEqual(target.read_text(), "edited\n")
                self.assertEqual(self.write(jobs, True), (1, 0, 0))
            finally:
                rt.PREFIX = old

    def test_an_existing_output_is_kept_unless_forced(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            t = copy_test(tmp)
            target = t["dir"] / "SAMPLES" / "q.out"
            target.write_text("existing\n")
            old = rt.PREFIX
            rt.PREFIX = self.fake_reference(tmp)
            try:
                self.assertEqual(self.write(me.plan([t]), False), (0, 1, 0))
                self.assertEqual(target.read_text(), "existing\n")
                self.assertEqual(self.write(me.plan([t]), True), (1, 0, 0))
                self.assertEqual(target.read_text(), "reference -b p.lgi\nq end\n")
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

    def test_programs_are_relative_to_the_repository(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            root = Path(tmp)
            (root / "applications" / "A").mkdir(parents=True)
            (root / "applications" / "A" / "p.lgi").write_text("")
            (root / "contributions" / "C").mkdir(parents=True)
            (root / "contributions" / "C" / "c.lgi").write_text("")
            (root / "other").mkdir()
            (root / "other" / "o.lgi").write_text("")
            self.assertEqual(cc.programs(root), {"applications/A/p.lgi", "contributions/C/c.lgi"})


class TriageTest(unittest.TestCase):
    def test_load_status_with_a_fake_prefix(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            prefix = Path(tmp) / "p"
            (prefix / "bin").mkdir(parents=True)
            (prefix / "bin" / "elan").write_text(
                '#!/bin/sh\ncase "$2" in good.lgi) exit 0;; *) echo "[error] bad module" >&2; exit 1;; esac\n')
            (prefix / "bin" / "elan").chmod(0o755)
            app = Path(tmp) / "applications" / "App"
            app.mkdir(parents=True)
            (app / "good.lgi").write_text("")
            (app / "other.lgi").write_text("")
            self.assertEqual(tr.load_status_in(app, "good", prefix, 30), "ok")
            self.assertEqual(tr.load_status_in(app, "absent", prefix, 30), "NOLGI")
            self.assertEqual(tr.load_status_in(app, "other", prefix, 30), "[error] bad module")

    def fake_prefix(self, tmp, script):
        prefix = Path(tmp) / "p"
        (prefix / "bin").mkdir(parents=True)
        (prefix / "bin" / "elan").write_text("#!/bin/sh\n" + script + "\n")
        (prefix / "bin" / "elan").chmod(0o755)
        app = Path(tmp) / "applications" / "App"
        app.mkdir(parents=True)
        (app / "x.lgi").write_text("")
        return app, prefix

    def test_nonzero_rc_without_an_error_line_is_rc_n(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            app, prefix = self.fake_prefix(tmp, "exit 7")
            self.assertEqual(tr.load_status_in(app, "x", prefix, 30), "rc=7")

    def test_silent_batch_failure_takes_the_reason_of_the_unbatched_run(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            app, prefix = self.fake_prefix(
                tmp, 'if [ "$1" = -b ]; then exit 1; fi; echo "[fatal] some reason" >&2; exit 1')
            self.assertEqual(tr.load_status_in(app, "x", prefix, 30), "[fatal] some reason")

    def test_usage_text_and_silence_are_reported(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            app, prefix = self.fake_prefix(
                tmp, 'if [ "$1" != -b ]; then echo "usage: elan [--elanlib D] f" >&2; fi; exit 1')
            self.assertEqual(tr.load_status_in(app, "x", prefix, 30), "needs-spc: no .spc")

    def test_a_program_needing_a_spc_is_tried_with_the_spc_files(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            app, prefix = self.fake_prefix(
                tmp, 'case "$3" in b.spc) exit 0;; "") echo "usage: elan f" >&2; exit 1;; '
                     '*) echo "[error] bad spc" >&2; exit 1;; esac')
            self.assertEqual(tr.load_status_in(app, "x", prefix, 30), "needs-spc: no .spc")
            (app / "a.spc").write_text("")
            self.assertEqual(tr.load_status_in(app, "x", prefix, 30),
                             "needs-spc: none of 1 loads ([error] bad spc)")
            (app / "b.spc").write_text("")
            self.assertEqual(tr.load_status_in(app, "x", prefix, 30), "ok:b.spc")

    def test_a_location_error_is_joined_with_its_message_on_one_line(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            app, prefix = self.fake_prefix(
                tmp, r"printf '[error] :file=x.lgi\tline=3\tpos=4\n\nsyntax error\n' >&2; exit 1")
            st = tr.load_status_in(app, "x", prefix, 30)
            self.assertEqual(st, "[error] :file=x.lgi line=3 pos=4 syntax error")
            self.assertNotIn("\t", st)

    def test_candidates_match_the_program_name(self):
        inps, outs = tr.candidates("applications/APT/bubleLOG.lgi")
        self.assertIn("bubleLOG.inp", inps)
        self.assertIn("bubleLOG.out", outs)


if __name__ == "__main__":
    unittest.main()
