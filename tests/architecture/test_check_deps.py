#!/usr/bin/env python3
"""Unit tests of check_deps.py (stdlib unittest)."""
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import check_deps as cd  # noqa: E402

RULES = {"lex": ["base"], "base": [], "term": ["lex", "base"]}


def tree(files):
    root = Path(tempfile.mkdtemp())
    for rel, text in files.items():
        p = root / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text)
    return root


class CheckDepsTest(unittest.TestCase):
    def test_allowed_edge_passes(self):
        root = tree({"base/a.h": "", "lex/l.cc": '#include "a.h"\n'})
        self.assertEqual(cd.violations(root, RULES, set()), [])

    def test_forbidden_edge_is_reported(self):
        root = tree({"term/t.h": "", "lex/l.cc": '#include "t.h"\n'})
        self.assertEqual(cd.violations(root, RULES, set()), [("lex/l.cc", "term/t.h")])

    def test_same_module_is_allowed(self):
        root = tree({"lex/x.h": "", "lex/l.cc": '#include "x.h"\n'})
        self.assertEqual(cd.violations(root, RULES, set()), [])

    def test_relative_path_include_is_attributed_to_its_real_module(self):
        root = tree({"term/t.h": "", "lex/l.cc": '#include "../term/t.h"\n'})
        self.assertEqual(cd.violations(root, RULES, set()), [("lex/l.cc", "term/t.h")])

    def test_angle_bracket_include_of_a_module_header_is_checked(self):
        root = tree({"term/t.h": "", "lex/l.cc": '#include <t.h>\n'})
        self.assertEqual(cd.violations(root, RULES, set()), [("lex/l.cc", "term/t.h")])

    def test_headers_in_module_subdirectories_are_not_on_the_include_path(self):
        root = tree({"term/sub/t.h": "", "lex/l.cc": '#include "t.h"\n'})
        self.assertEqual(cd.violations(root, RULES, set()), [])

    def test_umbrella_header_is_forbidden_below_its_level(self):
        root = tree({"base/commondefs.h": "", "base/b.cc": '#include "commondefs.h"\n',
                     "term/t.cc": '#include "commondefs.h"\n'})
        self.assertEqual(cd.violations(root, RULES, set(), umbrella={"base/commondefs.h": {"base", "lex"}}),
                         [("base/b.cc", "base/commondefs.h")])

    def test_listed_exception_passes(self):
        root = tree({"term/t.h": "", "lex/l.cc": '#include "t.h"\n'})
        self.assertEqual(cd.violations(root, RULES, {("lex/l.cc", "term/t.h")}), [])

    def test_system_and_unknown_headers_are_ignored(self):
        root = tree({"lex/l.cc": '#include <stdio.h>\n#include "nowhere.h"\n'})
        self.assertEqual(cd.violations(root, RULES, set()), [])

    def test_commented_include_is_ignored(self):
        root = tree({"term/t.h": "", "lex/l.cc": '// #include "t.h"\n/* #include "t.h" */\n'})
        self.assertEqual(cd.violations(root, RULES, set()), [])

    def test_exceptions_file_parsing(self):
        root = tree({"exc.txt": "# comment\nlex/l.cc -> term/t.h  # printing needs it\n\n"})
        self.assertEqual(cd.load_exceptions(root / "exc.txt"), {("lex/l.cc", "term/t.h")})

    def test_stale_exception_is_reported(self):
        root = tree({"lex/l.cc": ""})
        self.assertEqual(cd.stale_exceptions(root, RULES, {("lex/l.cc", "term/t.h")}),
                         [("lex/l.cc", "term/t.h")])


if __name__ == "__main__":
    unittest.main()
