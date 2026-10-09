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

    def test_ignores_directories_without_prog_lgi(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as d:
            (Path(d) / "notes").mkdir()
            self.assertEqual(rt.discover_regression(Path(d)), [])

    def test_missing_root_gives_no_case(self):
        self.assertEqual(rt.discover_regression(rt.HERE / "does-not-exist"), [])


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


class CaseSensitivityTest(unittest.TestCase):
    def test_repository_volume_is_case_sensitive(self):
        self.assertTrue(rt.case_sensitive(rt.HERE / "work"))

    @unittest.skipUnless(sys.platform == "darwin", "macOS default volume only")
    def test_macos_temp_dir_is_not(self):
        self.assertFalse(rt.case_sensitive(Path(tempfile.gettempdir()) / "elan-case-probe"))


if __name__ == "__main__":
    unittest.main()
