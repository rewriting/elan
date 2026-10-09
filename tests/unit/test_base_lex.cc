// Unit tests of base/ (stringtab) and lex/ (lexem), linked with elan_core.
// They pin down the 2004 behaviour: identifier codes are hash positions.
#include "check.h"
#include "commondefs.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

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

TEST(mitab_add_counts_occurrences_and_finds) {
  mitab t(100);
  int i = t.addn(7);
  CHECK(t.member(7));
  CHECK_EQ(t.posid, i);
  CHECK_EQ(i, 7);                                // position is n modulo size
  CHECK_EQ(t.addn(107), (7 + 211) % 100);        // collision: probe by 211
  CHECK(!t.member(8));
}

TEST(lexem_identifier_is_coded_by_its_tabofident_position) {
  lexem l;
  l.cridlex("someident");
  CHECK(l.isident());
  CHECK_EQ(l.idval(), tabofident.index("someident"));
  CHECK_STREQ(l.alfsy(), "someident");
}

// Lex a string through the real lexer (lstream over an in-memory FILE*).
static std::vector<std::string> lex_all(const char *text) {
  FILE *f = fmemopen((void *)text, strlen(text), "r");
  ichstream in(f, "test");
  lstream ls(&in);
  std::vector<std::string> out;
  lexem l;
  for (int n = 0; n < 50; n++) {
    ls.ilex(l);
    if (l.isendofstream()) break;
    if (l.isblankk()) continue;
    out.push_back(l.isnum() ? "#" + std::to_string(l.numval()) : std::string(l.alfsy()));
  }
  return out;
}

TEST(lstream_lexes_identifiers_numbers_and_characters) {
  std::vector<std::string> expected = {"f", "(", "x", ",", "#42", ")"};
  CHECK(lex_all("f(x, 42)") == expected);
}

int main() { return run_tests(); }
