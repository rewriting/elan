// Unit tests of the encodings that appear in .ref files, in REM and in the
// compiled runtime (docs/ref-format.md): the lexem ranges (lex/lexem.h) and
// the 12-bit packing of the defstrat words (term/codes.h). They pin the 2004
// values: changing one of them renumbers the exported programs.
#include "check.h"
#include "commondefs.h"
#include "codes.h"

// ---------------------------------------------------------------- lexem ranges
TEST(lexem_range_constants_are_the_2004_ones) {
  CHECK_EQ(NOLEXEM, -256);
  CHECK_EQ(BLANKLEXEM, -257);
  CHECK_EQ(IDENT, -259);
  CHECK_EQ(BOFIDENT, -260);
  CHECK_EQ(MAXNOFIDENT, 3000);
  CHECK_EQ(STRING, -3260);
  CHECK_EQ(BOFSTRING, -3261);
  CHECK_EQ(MAXNOFSTRING, 10000);
  CHECK_EQ(JUSTNUMBER, -13261);
  CHECK_EQ(BOFTYPES, -13262);
  CHECK_EQ(DEFAULTSYM, -32000);
  CHECK_EQ(NTYPES, 500);
}

TEST(lexem_ranges_do_not_overlap) {
  lexem l;
  l.crcharlex(255);                 // characters: -1 .. -255
  CHECK(l.ischar());
  CHECK(!l.isident());
  l.cridlex(0);                     // identifiers: BOFIDENT .. BOFIDENT-MAXNOFIDENT+1
  CHECK(l.isrealid());
  CHECK(!l.isstring());
  l.cridlex(MAXNOFIDENT - 1);
  CHECK(l.isrealid());
  CHECK_EQ(l.idval(), MAXNOFIDENT - 1);
  CHECK(!l.isstring());
  CHECK(!l.isnum());
  l.crstringlex();                  // the "any string" lexem lies between them
  CHECK(l.isstring());
  CHECK(!l.isrealstring());
  CHECK(!l.isident());
  l.crnumlex();                     // JUSTNUMBER: just below the strings
  CHECK(l.isnum());
  CHECK(!l.isstring());
  CHECK(l.terminal());
}

TEST(lexem_sorts_lie_below_the_terminals_and_above_defaultsym) {
  lexem l;
  l.crtypelex(0);
  CHECK(l.nonterminal());
  CHECK_EQ(l.typeval(), 0);
  l.crtypelex(STRATTYPE);           // the largest sort code (NTYPES+3)
  CHECK(l.nonterminal());
  CHECK_EQ(l.typeval(), STRATTYPE);
  CHECK(BOFTYPES - STRATTYPE > DEFAULTSYM);
}

TEST(lexem_string_constants_are_numbered_from_bofstring) {
  lexem l;
  char s[] = "golden";
  int before = stringconstantsi;
  l.crstringlex(s);
  CHECK(l.isrealstring());
  CHECK_STREQ(l.stringval(), "golden");
  CHECK_EQ(stringconstantsi, before + 1);
}

// ---------------------------------------------------------------- defstrat words
TEST(defstrat_fsym_flag_packs_two_12_bit_codes) {
  int w = FSYM_FLAG(300, 4095);
  CHECK(IS_FSYM_FLAG(w));
  CHECK(!IS_LAB_FLAG(w));
  CHECK(!IS_DSTR_FLAG(w));
  CHECK_EQ(FSYM_F1(w), 300);
  CHECK_EQ(FSYM_F2(w), 4095);
  CHECK_EQ(w, 0x1000000 | (300 << 12) | 4095);
}

TEST(defstrat_lab_flag_packs_symbol_and_label) {
  int w = LAB_FLAG(MAXNFSYM + 4, 1999);       // RULECONSTRULE1: the largest symbol code
  CHECK(IS_LAB_FLAG(w));
  CHECK(!IS_FSYM_FLAG(w));
  CHECK_EQ(LAB_F(w), MAXNFSYM + 4);
  CHECK_EQ(LAB_LAB(w), 1999);
}

TEST(defstrat_dstr_flag_packs_symbol_and_strategy) {
  int w = DSTR_FLAG(2001, 7);
  CHECK(IS_DSTR_FLAG(w));
  CHECK(!IS_APPLY_FLAG(w));
  CHECK_EQ(DSTR_F(w), 2001);
  CHECK_EQ(DSTR_LAB(w), 7);
}

TEST(defstrat_apply_flag) {
  int w = APPLY_FLAG(42);
  CHECK(IS_APPLY_FLAG(w));
  CHECK(!IS_FSYM_FLAG(w));
  CHECK_EQ(w & 0xffffff, 42);
}

TEST(defstrat_minus_one_gives_the_2004_bits) {
  // f2 = -1 (no code found) borrows from the f1 field, as the 2004 two's
  // complement arithmetic did
  CHECK_EQ(FSYM_FLAG(5, -1), 0x1000000 + (5 << 12) - 1);
  CHECK_EQ(FSYM_F1(FSYM_FLAG(5, -1)), 4);
  CHECK_EQ(FSYM_F2(FSYM_FLAG(5, -1)), 0xfff);
}

int main() { return run_tests(); }
