#!/usr/bin/env python3
"""Unit tests of check_limits.py (stdlib unittest)."""
import shutil
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import check_limits as cl  # noqa: E402

FILES = ["interpreter/base/constants.h", "interpreter/term/codes.h",
         "compiler/runtime/termIn.h", "compiler/runtime/codes.h",
         "compiler/earley/runtimeInit.cc", "compiler/earley/commondefs.h",
         "compiler/earley/codes.h"]


class EvaluateTest(unittest.TestCase):
    def test_expressions_of_defines(self):
        d = cl.defines("#define A 3000  // comment\n#define B (A+4) /* c */\n"
                       "#define C -260\n#define D (C-A-1)\n")
        self.assertEqual(cl.evaluate("B", d), 3004)
        self.assertEqual(cl.evaluate("D", d), -3261)

    def test_stringtab_sizes(self):
        self.assertEqual(cl.stringtab_sizes("//stringtab x(1);\nstringtab typet(500);\n"),
                         {"typet": 500})


class CheckTest(unittest.TestCase):
    def setUp(self):
        self.tmp = Path(tempfile.mkdtemp())
        for f in FILES:
            (self.tmp / f).parent.mkdir(parents=True, exist_ok=True)
            shutil.copy(cl.SRC / f, self.tmp / f)

    def tearDown(self):
        shutil.rmtree(self.tmp)

    def edit(self, f, old, new):
        p = self.tmp / f
        text = p.read_text(errors="replace")
        self.assertIn(old, text)
        p.write_text(text.replace(old, new, 1))

    def test_the_sources_agree(self):
        self.assertEqual(cl.check(), [])

    def test_runtime_identifier_table_size(self):
        self.edit("compiler/runtime/termIn.h", "TABOFIDENT_SIZE 3000", "TABOFIDENT_SIZE 4000")
        self.assertEqual(len(cl.check(self.tmp)), 1)

    def test_earley_sort_table_size(self):
        self.edit("compiler/earley/runtimeInit.cc", "\nstringtab typet(500)", "\nstringtab typet(600)")
        self.assertIn("typet", cl.check(self.tmp)[0])

    def test_earley_copy_of_ntypes(self):
        self.edit("compiler/earley/commondefs.h", "#define NTYPES 500", "#define NTYPES 501")
        self.assertTrue(any("NTYPES" in p for p in cl.check(self.tmp)))

    def test_builtin_code(self):
        self.edit("compiler/runtime/codes.h", "#define PLUS ", "#define PLUS 99 //")
        self.assertTrue(any("PLUS" in p for p in cl.check(self.tmp)))

    def test_use_of_the_stale_fsymcodesbeg(self):
        (self.tmp / "compiler/earley/x.cc").write_text("int f() { return FSYMCODESBEG; }\n")
        self.assertTrue(any("x.cc:1" in p for p in cl.check(self.tmp)))


if __name__ == "__main__":
    unittest.main()
