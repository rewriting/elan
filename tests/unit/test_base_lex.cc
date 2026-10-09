// Unit tests of base/ (stringtab) and lex/ (lexem), linked with elan_core.
// They pin down the 2004 behaviour: identifier codes are hash positions.
#include "check.h"
#include "commondefs.h"

TEST(stringtab_add_then_member_and_index) {
  stringtab t(100);
  int i = t.addstr("foo");
  CHECK(t.member("foo"));
  CHECK_EQ(t.index("foo"), i);
  CHECK_STREQ(t.ide(i), "foo");
  CHECK_EQ(t.addstr("foo"), i);           // adding twice gives the same position
}

TEST(stringtab_position_is_char_sum_modulo_size) {
  stringtab t(100);
  CHECK_EQ(t.addstr("ab"), ('a' + 'b') % 100);   // 95
}

TEST(stringtab_collision_probes_by_211) {
  stringtab t(100);
  t.addstr("ab");                                // 95
  CHECK_EQ(t.addstr("ba"), (95 + 211) % 100);    // same sum: next probe, 6
}

TEST(stringtab_unknown_string) {
  stringtab t(100);
  CHECK(!t.member("nope"));
  CHECK_EQ(t.index("nope"), -1);
}

TEST(stringtab_removestr_forgets) {
  stringtab t(100);
  int i = t.addstr("gone");
  t.removestr(i);
  CHECK(!t.member("gone"));
}

TEST(lexem_number) {
  lexem l;
  l.crnumlex(42);
  CHECK(l.isnum());
  CHECK_EQ(l.numval(), 42);
  CHECK(!l.ischar());
  CHECK(l.isnotendofstream());
}

TEST(lexem_character) {
  lexem l;
  l.crcharlex('(');
  CHECK(l.ischar());
  CHECK_STREQ(l.alfsy(), "(");
}

TEST(lexem_end_of_stream) {
  lexem l;
  l.crendofstreamlex();
  CHECK(l.isendofstream());
  CHECK_STREQ(l.alfsy(), "ENDOFLSTREAM");
}

int main() { return run_tests(); }
