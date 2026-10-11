#!/usr/bin/env python3
"""Unit tests of tracecmp.py (plan S8a, Review Focus 5): the comparison of
load mode removes only its three exclusions; any other difference fails."""
import io
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import tracecmp  # noqa: E402

BASE = """@begin m.eln
id 646 module
id 109 m
str 0 abc
num 3
char 59
eos
@end
""".splitlines()


class TraceCmpTest(unittest.TestCase):
    def setUp(self):
        self.dir = tempfile.TemporaryDirectory()

    def tearDown(self):
        self.dir.cleanup()

    def write(self, name, lines):
        p = os.path.join(self.dir.name, name)
        with open(p, "w") as f:
            f.write("".join(l + "\n" for l in lines))
        return p

    def run_cmp(self, mode, c, g, extra=()):
        out = io.StringIO()
        st = tracecmp.main(["--mode", mode, self.write("c", c), self.write("g", g), *extra], out)
        return st, out.getvalue()

    def both(self, c, g):
        return self.run_cmp("lex", c, g)[0], self.run_cmp("load", c, g)[0]

    def test_identical(self):
        self.assertEqual(self.both(BASE, BASE), (0, 0))

    def test_lexem_differs(self):
        g = BASE[:]; g[4] = "num 4"
        self.assertEqual(self.both(BASE, g), (1, 1))

    def test_lexem_missing(self):
        self.assertEqual(self.both(BASE, BASE[:4] + BASE[5:]), (1, 1))

    def test_lexem_extra(self):
        self.assertEqual(self.both(BASE, BASE[:4] + ["char 1"] + BASE[4:]), (1, 1))

    def test_marker_missing(self):
        c = BASE[:3] + ["@foreach-begin", "@foreach-end"] + BASE[3:]
        g = BASE[:3] + ["@foreach-begin"] + BASE[3:]
        self.assertEqual(self.both(c, g), (1, 1))

    def test_foreach_bodies(self):
        c = BASE[:3] + ["@foreach-begin", "id 97 a", "@foreach-begin", "id 98 b",
                        "@foreach-end", "@foreach-end"] + BASE[3:]
        g = BASE[:3] + ["@foreach-begin", "@foreach-end"] + BASE[3:]
        st, out = self.run_cmp("load", c, g)
        self.assertEqual(st, 0, out)
        self.assertIn("foreach=1", out)
        self.assertEqual(self.run_cmp("lex", c, g)[0], 1)

    def test_foreach_body_on_go_side_fails(self):
        c = BASE[:3] + ["@foreach-begin", "@foreach-end"] + BASE[3:]
        g = BASE[:3] + ["@foreach-begin", "id 97 a", "@foreach-end"] + BASE[3:]
        self.assertEqual(self.run_cmp("load", c, g)[0], 1)

    def test_strconc_cxx_side_only(self):
        sub = ["@begin strconc.eln", "id 1 module", "@begin x.eln", "@end", "eos", "@end"]
        c = BASE[:6] + sub + BASE[6:]
        st, out = self.run_cmp("load", c, BASE)
        self.assertEqual(st, 0, out)
        self.assertIn("strconc=1", out)
        self.assertEqual(self.run_cmp("load", BASE, c)[0], 1)   # on the Go side
        self.assertEqual(self.run_cmp("lex", c, BASE)[0], 1)

    def test_positions(self):
        g = [l.replace("id 646", "id 7").replace("str 0", "str 12") for l in BASE]
        self.assertEqual(self.both(BASE, g), (1, 0))

    def test_text_after_position_still_compared(self):
        g = BASE[:]; g[2] = "id 109 n"
        self.assertEqual(self.both(BASE, g), (1, 1))

    def test_extra_begin_on_go_side(self):
        g = BASE[:6] + ["@begin y.eln", "eos", "@end"] + BASE[6:]
        self.assertEqual(self.both(BASE, g), (1, 1))

    def test_stderr_and_status(self):
        e1, e2 = self.write("e1", ["err"]), self.write("e2", ["other"])
        self.assertEqual(self.run_cmp("lex", BASE, BASE, ["--stderr", e1, e2])[0], 1)
        self.assertEqual(self.run_cmp("load", BASE, BASE, ["--stderr", e1, e1])[0], 0)
        self.assertEqual(self.run_cmp("lex", BASE, BASE, ["--status", "0", "1"])[0], 1)
        self.assertEqual(self.run_cmp("load", BASE, BASE, ["--status", "1", "1"])[0], 0)

    def test_report_names_module_stack(self):
        c = ["@begin a.eln", "id 1 x", "@begin b.eln", "id 2 y", "@end", "@end"]
        g = ["@begin a.eln", "id 1 x", "@begin b.eln", "id 2 z", "@end", "@end"]
        st, out = self.run_cmp("lex", c, g)
        self.assertEqual(st, 1)
        self.assertIn("a.eln > b.eln", out)


if __name__ == "__main__":
    unittest.main()
