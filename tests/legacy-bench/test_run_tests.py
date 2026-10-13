#!/usr/bin/env python3
"""Unit tests of run_tests.py (stdlib unittest; run: python3 test_run_tests.py)."""
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import run_tests as rt  # noqa: E402


class SamplesTest(unittest.TestCase):
    def test_uses_samples_directory_when_present(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as d:
            (Path(d) / "SAMPLES").mkdir()
            self.assertEqual(rt.samples(Path(d)), Path(d) / "SAMPLES")

    def test_falls_back_to_directory_itself(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as d:
            self.assertEqual(rt.samples(Path(d)), Path(d))


class RegressionDiscoveryTest(unittest.TestCase):
    def test_finds_cases_with_prog_lgi(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as d:
            case = Path(d) / "dot-printing"
            case.mkdir()
            (case / "prog.lgi").write_text("LPL prog description end\n")
            tests = rt.discover_regression(Path(d))
            self.assertEqual(len(tests), 1)
            t = tests[0]
            self.assertEqual(t["id"], "regression/dot-printing::I::prog:no:input:expected")
            self.assertEqual((t["kind"], t["lgi"], t["spc"], t["inp"], t["out"]),
                             ("I", "prog", "no", "input", "expected"))
            self.assertEqual(t["dir"], case)

    def test_a_case_fixing_a_2004_bug_is_marked(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as d:
            for name, marked in (("fixed", True), ("plain", False)):
                case = Path(d) / name
                case.mkdir()
                (case / "prog.lgi").write_text("LPL prog description end\n")
                if marked:
                    (case / "fixes-2004").write_text("why\n")
            tests = {t["dir"].name: t for t in rt.discover_regression(Path(d))}
            self.assertTrue(tests["fixed"]["fixes_2004"])
            self.assertFalse(tests["plain"]["fixes_2004"])

    def test_cases_fixing_2004_bugs_are_not_run_on_the_reference(self):
        t = {"fixes_2004": True}
        self.assertTrue(rt.modern_only(t, rt.REPO / "reference" / "install"))
        self.assertFalse(rt.modern_only(t, rt.REPO / "build" / "install"))
        self.assertFalse(rt.modern_only({}, rt.REPO / "reference" / "install"))

    def test_bench_cases_listed_as_fixes_are_not_run_on_the_reference(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as d:
            f = Path(d) / "fixes-2004.tsv"
            f.write_text("# comment\nx/Robot::I::robot:non:robot:robot\twhy\n")
            ids = rt.load_fixes_2004(f)
        self.assertEqual(ids, {"x/Robot::I::robot:non:robot:robot"})
        t = {"id": "x/Robot::I::robot:non:robot:robot"}
        self.assertTrue(rt.modern_only(t, rt.REPO / "reference" / "install", ids))
        self.assertFalse(rt.modern_only(t, rt.REPO / "build" / "install", ids))

    def test_modern_only_is_neither_a_regression_nor_an_improvement(self):
        regress, improve = rt.judge({"a::I::p": "MODERN-ONLY"}, {"a::I::p": "PASS"}, {})
        self.assertEqual((regress, improve), ([], []))

    def test_ignores_directories_without_prog_lgi(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as d:
            (Path(d) / "notes").mkdir()
            self.assertEqual(rt.discover_regression(Path(d)), [])

    def test_missing_root_gives_no_case(self):
        self.assertEqual(rt.discover_regression(rt.HERE / "does-not-exist"), [])



class RustCompilerKindTest(unittest.TestCase):
    """Kind R (elanc-rs, spec S6) is compared with the J snapshot of the
    same case, without its rewrite_step line, and builds in a target dir of
    the bench's work directory (not the user's cache)."""

    def test_r_case_uses_the_j_snapshot(self):
        t = {"id": "x/Test::R::p:no:-:p", "kind": "R"}
        self.assertEqual(rt.snapshot_id(t), "x/Test::J::p:no:-:p")
        t = {"id": "x/Test::JO::p:no:-:p", "kind": "JO"}
        self.assertEqual(rt.snapshot_id(t), "x/Test::JO::p:no:-:p")

    def test_r_snapshot_text_drops_rewrite_step(self):
        snap = "\nresult = s(o)\n\nrewrite_step = 12\n"
        self.assertEqual(rt.snapshot_text({"kind": "R"}, snap), "\nresult = s(o)\n\n")
        self.assertEqual(rt.snapshot_text({"kind": "J"}, snap), snap)

    def test_r_snapshot_compared_only_when_the_program_ran(self):
        for st in ("PASS", "PASS~", "PASS≈", "FAIL"):
            self.assertTrue(rt.compares_snapshot({"kind": "R"}, st))
        for st in ("UNSUPPORTED", "ERROR", "TIMEOUT", "NOLGI"):
            self.assertFalse(rt.compares_snapshot({"kind": "R"}, st))
        self.assertTrue(rt.compares_snapshot({"kind": "J"}, "ERROR"))
        self.assertFalse(rt.compares_snapshot({"kind": "I"}, "MODERN-ONLY"))

    def test_g_case_uses_the_j_snapshot_like_r(self):
        t = {"id": "x/Test::G::p:no:-:p", "kind": "G"}
        self.assertEqual(rt.snapshot_id(t), "x/Test::J::p:no:-:p")
        snap = "\nresult = s(o)\n\nrewrite_step = 12\n"
        self.assertEqual(rt.snapshot_text(t, snap), "\nresult = s(o)\n\n")
        self.assertTrue(rt.compares_snapshot(t, "PASS"))
        self.assertFalse(rt.compares_snapshot(t, "UNSUPPORTED"))
        self.assertEqual(rt.COMPILER_KINDS["G"], "elanc-go")
        self.assertEqual(rt.COMPILER_KINDS["R"], "elanc-rs")

    def test_elanc_go_cache_is_in_the_work_directory(self):
        self.assertEqual(Path(rt.env()["ELANC_GO_CACHE"]), rt.WORK / "elanc-go-cache")

    def test_elanc_rs_target_dir_is_in_the_work_directory(self):
        e = rt.env()
        self.assertEqual(Path(e["ELANC_RS_TARGET_DIR"]), rt.WORK / "elanc-rs-target")


class ResultsPruningTest(unittest.TestCase):
    def test_keeps_only_the_most_recent_result_directories(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as d:
            root = Path(d)
            names = [f"20261010-1200{i:02d}" for i in range(25)]
            for n in names:
                (root / n).mkdir()
                (root / n / "x.txt").write_text("x")
            rt.prune_results(root, keep=20)
            self.assertEqual(sorted(p.name for p in root.iterdir()), names[5:])

class ExamplesDiscoveryTest(unittest.TestCase):
    def test_finds_examples_named_after_their_directory(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as d:
            ex = Path(d) / "queens"
            ex.mkdir()
            for f in ("queens.lgi", "input.inp", "expected.out"):
                (ex / f).write_text("")
            (Path(d) / "README.md").write_text("")
            tests = rt.discover_examples(Path(d))
            self.assertEqual(len(tests), 1)
            t = tests[0]
            self.assertEqual(t["id"], "examples/queens::I::queens:no:input:expected")
            self.assertEqual((t["kind"], t["lgi"], t["inp"], t["out"], t["dir"]),
                             ("I", "queens", "input", "expected", ex))

    def test_passes_the_specification_file_if_there_is_one(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as d:
            ex = Path(d) / "poly2"
            ex.mkdir()
            for f in ("poly2.lgi", "someVariables.spc", "input.inp", "expected.out"):
                (ex / f).write_text("")
            t, = rt.discover_examples(Path(d))
            self.assertEqual(t["spc"], "someVariables")
            self.assertEqual(t["id"], "examples/poly2::I::poly2:someVariables:input:expected")

    def test_ignores_directories_without_their_lgi(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as d:
            (Path(d) / "notes").mkdir()
            self.assertEqual(rt.discover_examples(Path(d)), [])


class ExceptionsTest(unittest.TestCase):
    def test_filters_by_platform(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as d:
            f = Path(d) / "exc.tsv"
            f.write_text("# id\tplatform\treason\n"
                         "a::I::x\tlinux\tprintf rounding\n"
                         "b::I::y\tdarwin\tsomething\n"
                         "c::I::z\tall\tundefined behaviour, see S2\n")
            self.assertEqual(rt.load_exceptions(f, "linux"),
                             {"a::I::x": "printf rounding",
                              "c::I::z": "undefined behaviour, see S2"})

    def test_missing_file_means_no_exception(self):
        self.assertEqual(rt.load_exceptions(rt.HERE / "nope.tsv", "linux"), {})


class JudgeTest(unittest.TestCase):
    def test_worse_status_than_baseline_is_a_regression(self):
        regress, improve = rt.judge({"t::I::x": "FAIL"}, {"t::I::x": "PASS"}, {})
        self.assertEqual(regress, [("t::I::x", "PASS", "FAIL")])
        self.assertEqual(improve, [])

    def test_better_status_than_baseline_is_an_improvement(self):
        regress, improve = rt.judge({"t::I::x": "PASS"}, {"t::I::x": "FAIL"}, {})
        self.assertEqual((regress, improve), ([], [("t::I::x", "FAIL", "PASS")]))

    def test_failing_regression_case_fails_even_without_baseline(self):
        rid = "regression/dot::I::prog:no:input:expected"
        regress, _ = rt.judge({rid: "FAIL"}, {}, {})
        self.assertEqual(regress, [(rid, "PASS", "FAIL")])

    def test_regression_case_must_pass_exactly_even_if_baseline_says_fail(self):
        rid = "regression/dot::I::prog:no:input:expected"
        regress, _ = rt.judge({rid: "PASS≈"}, {rid: "FAIL"}, {})
        self.assertEqual(regress, [(rid, "PASS", "PASS≈")])

    def test_platform_exception_exempts_status_regression(self):
        regress, _ = rt.judge({"t::I::x": "FAIL"}, {"t::I::x": "PASS"},
                              {"t::I::x": "platform: printf of doubles"})
        self.assertEqual(regress, [])


class SanitizerTest(unittest.TestCase):
    def test_detects_address_sanitizer_report(self):
        err = "==123==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x60\n#0 ..."
        self.assertIn("heap-buffer-overflow", rt.sanitizer_report(err))

    def test_detects_undefined_behaviour_report(self):
        err = "term.cc:12:5: runtime error: signed integer overflow: 2147483647 + 1\n"
        self.assertIn("signed integer overflow", rt.sanitizer_report(err))

    def test_ordinary_stderr_is_not_a_report(self):
        self.assertIsNone(rt.sanitizer_report("[error] can't open input file 'eq.eln'\n"))

    def test_sanitizer_status_always_regresses(self):
        regress, _ = rt.judge({"t::I::x": "SANITIZER"}, {"t::I::x": "FAIL"}, {})
        self.assertEqual(regress, [("t::I::x", "FAIL", "SANITIZER")])


class RunTest(unittest.TestCase):
    def test_unrunnable_program_is_reported_not_raised(self):
        # e.g. elanc is a #!/bin/tcsh script and tcsh is not installed
        rc, out, err = rt.run(["elan-no-such-program"], rt.HERE)
        self.assertEqual(rc, 127)
        self.assertIn("elan-no-such-program", err)


class CaseRenamesTest(unittest.TestCase):
    ROBOT = rt.ELAN3 / "applications" / "Robot"

    def test_manifest_maps_robot_variant(self):
        self.assertEqual(rt.src_path(self.ROBOT / "Nim.lgi"), self.ROBOT / "x-case-Nim.lgi")
        self.assertEqual(rt.src_path(self.ROBOT / "nim.lgi"), self.ROBOT / "nim.lgi")
        self.assertTrue(rt.src_path(self.ROBOT / "SAMPLES" / "Robot.out").exists())

    def copy(self, tmp):
        wd = Path(tmp) / "Robot"
        rt.shutil.copytree(self.ROBOT, wd, ignore=rt.shutil.ignore_patterns("CVS"))
        return wd

    def test_restore_for_an_upper_case_test(self):
        with tempfile.TemporaryDirectory() as tmp:
            wd = self.copy(tmp)
            rt.restore_case(self.ROBOT, wd, "Nim")
            names = {p.name for p in wd.iterdir()}
            self.assertIn("Nim.lgi", names)
            self.assertIn("State.eln", names)
            self.assertFalse(any(n.startswith("x-case-") for n in names))
            self.assertTrue((wd / "Nim.eln").read_text(encoding="latin-1").startswith("module Nim"))
            self.assertTrue((wd / "SAMPLES" / "Nim.inp").exists())

    def test_no_restore_for_a_lower_case_test(self):
        with tempfile.TemporaryDirectory() as tmp:
            wd = self.copy(tmp)
            rt.restore_case(self.ROBOT, wd, "nim")
            self.assertTrue((wd / "x-case-Nim.lgi").exists())
            self.assertTrue((wd / "nim.eln").read_text(encoding="latin-1").startswith("module nim"))


class OverlayDiscoveryTest(unittest.TestCase):
    def make(self, tmp):
        root = Path(tmp) / "legacy"
        app = root / "elan3" / "applications" / "App"
        (app / "SAMPLES").mkdir(parents=True)
        (app / "tst_all").write_text("itest p no p p\n")
        bare = root / "elan3" / "applications" / "Bare"   # no tst_all of its own
        bare.mkdir(parents=True)
        ov = Path(tmp) / "overlay"
        (ov / "elan3" / "applications" / "App").mkdir(parents=True)
        (ov / "elan3" / "applications" / "App" / "tst_all.overlay").write_text(
            "itest q no q q\naitest q no q q\ngtest q no q q-c\n")
        (ov / "elan3" / "applications" / "Bare").mkdir(parents=True)
        (ov / "elan3" / "applications" / "Bare" / "tst_all.overlay").write_text("itest b no b b\n")
        return root, ov

    def test_overlay_lines_are_tests_with_the_usual_ids(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as tmp:
            root, ov = self.make(tmp)
            tests = rt.discover(suites=[root / "elan3" / "applications"], overlay=ov, root=root)
            ids = {t["id"]: t for t in tests}
            self.assertIn("elan3/applications/App::I::p:no:p:p", ids)
            self.assertFalse(ids["elan3/applications/App::I::p:no:p:p"]["overlay"])
            q = ids["elan3/applications/App::I::q:no:q:q"]
            self.assertTrue(q["overlay"])
            self.assertEqual(q["dir"], root / "elan3" / "applications" / "App")
            self.assertEqual(q["ovdir"], ov / "elan3" / "applications" / "App")
            self.assertIn("elan3/applications/App::A::q:no:q:q", ids)
            self.assertIn("elan3/applications/Bare::I::b:no:b:b", ids)

    def test_gtest_is_kind_G_with_an_input(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as tmp:
            root, ov = self.make(tmp)
            tests = rt.discover(suites=[root / "elan3" / "applications"], overlay=ov, root=root)
            g = [t for t in tests if t["id"] == "elan3/applications/App::G::q:no:q:q-c"]
            self.assertEqual(len(g), 1)
            self.assertEqual((g[0]["kind"], g[0]["inp"], g[0]["out"]), ("G", "q", "q-c"))

    def test_overlay_of_a_missing_directory_is_an_error(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as tmp:
            root, ov = self.make(tmp)
            (ov / "elan3" / "applications" / "Nowhere").mkdir()
            (ov / "elan3" / "applications" / "Nowhere" / "tst_all.overlay").write_text("itest x no x x\n")
            with self.assertRaises(SystemExit):
                rt.discover(suites=[root / "elan3" / "applications"], overlay=ov, root=root)

    def test_real_tree_ids_are_unchanged(self):
        # the default arguments give the historical tests with overlay False
        ids = [t["id"] for t in rt.discover() if not t["overlay"]]
        self.assertIn("elan3/applications/Robot::I::Nim:non:Nim:Nim", ids)


class OverlayCopyTest(unittest.TestCase):
    def make(self, tmp):
        d = Path(tmp) / "legacy" / "App"
        (d / "SAMPLES").mkdir(parents=True)
        (d / "p.lgi").write_text("legacy lgi\n")
        (d / "m.eln").write_text("legacy module\n")
        (d / "SAMPLES" / "old.inp").write_text("old input\n")
        ov = Path(tmp) / "overlay" / "App"
        (ov / "SAMPLES").mkdir(parents=True)
        (ov / "m.eln").write_text("ported module\n")
        (ov / "SAMPLES" / "new.inp").write_text("new input\n")
        (ov / "tst_all.overlay").write_text("itest p no new new\n")
        (ov / "PORTING.md").write_text("notes\n")
        return dict(dir=d, ovdir=ov, lgi="p", spc="no", inp="new", out="new",
                    kind="I", flags=[], overlay=True, id="App::I::p:no:new:new")

    def test_locate_prefers_the_overlay_then_legacy(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as tmp:
            t = self.make(tmp)
            f, rel = rt.locate(t, "new.inp")
            self.assertEqual((f, rel), (t["ovdir"] / "SAMPLES" / "new.inp", Path("SAMPLES/new.inp")))
            f, rel = rt.locate(t, "old.inp")
            self.assertEqual((f, rel), (t["dir"] / "SAMPLES" / "old.inp", Path("SAMPLES/old.inp")))
            self.assertEqual(rt.locate(t, "none.out"), (None, None))

    def test_private_copy_puts_the_overlay_over_legacy(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as tmp:
            t = self.make(tmp)
            wd = Path(tmp) / "wd"
            rt.private_copy(t, wd)
            self.assertEqual((wd / "m.eln").read_text(), "ported module\n")
            self.assertEqual((wd / "p.lgi").read_text(), "legacy lgi\n")
            self.assertTrue((wd / "SAMPLES" / "new.inp").exists())
            self.assertTrue((wd / "SAMPLES" / "old.inp").exists())
            self.assertFalse((wd / "tst_all.overlay").exists())
            self.assertFalse((wd / "PORTING.md").exists())

    def test_produce_runs_with_the_overlay_input(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as tmp:
            t = self.make(tmp)
            prefix = Path(tmp) / "prefix"
            (prefix / "bin").mkdir(parents=True)
            fake = prefix / "bin" / "elan"
            fake.write_text('#!/bin/sh\necho "elan $*"; cat\n')
            fake.chmod(0o755)
            old = rt.PREFIX
            rt.PREFIX = prefix
            try:
                out, err, rc = rt.produce(t, 30)
            finally:
                rt.PREFIX = old
            self.assertEqual(out, "elan -b p.lgi\nnew input\n")
            self.assertEqual(rc, 0)

    def test_a_failing_run_with_empty_stderr_reports_its_rc(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as tmp:
            t = self.make(tmp)
            (t["ovdir"] / "SAMPLES" / "new.out").write_text("expected\n")
            prefix = Path(tmp) / "prefix"
            (prefix / "bin").mkdir(parents=True)
            fake = prefix / "bin" / "elan"
            fake.write_text('#!/bin/sh\necho got; exit 3\n')
            fake.chmod(0o755)
            old = rt.PREFIX
            rt.PREFIX = prefix
            try:
                self.assertEqual(rt.produce(t, 30)[2], 3)
                status, detail, _ = rt.run_test(t, 30)
            finally:
                rt.PREFIX = old
            self.assertEqual(status, "FAIL")
            self.assertIn("rc=3", detail)

    def test_produce_reports_a_missing_input(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as tmp:
            t = dict(self.make(tmp), inp="absent")
            with self.assertRaises(rt.Unrunnable) as e:
                rt.produce(t, 30)
            self.assertEqual(e.exception.status, "NOINP")


class OverlayJudgeTest(unittest.TestCase):
    def test_overlay_tests_must_pass_unless_in_the_baseline(self):
        st = {"A::I::p:no:q:q": "PASS~", "A::G::p:no:q:q-c": "UNSUPPORTED",
              "A::I::r:no:r:r": "FAIL", "A::I::h:no:h:h": "FAIL"}
        regress, _ = rt.judge(st, {"A::I::r:no:r:r": "FAIL"}, set(),
                              overlay_ids=set(st))
        self.assertEqual(sorted(r[0] for r in regress), ["A::I::h:no:h:h", "A::I::p:no:q:q"])

    def test_historical_tests_keep_the_old_rule(self):
        regress, _ = rt.judge({"A::I::p:no:p:p": "FAIL"}, {}, set())
        self.assertEqual(regress, [])


if __name__ == "__main__":
    unittest.main()
