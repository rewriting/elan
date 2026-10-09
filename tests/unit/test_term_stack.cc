// Unit tests of the term construction stack (term/term.cc): terms are built
// bottom-up on a stack (crstterm/crterm push, popt pops). Until S3b the stack
// had MAXTERMDEEP = 20000 entries and a deeper construction stopped elan;
// it now grows on demand.
#include "commondefs.h"
#include "termdefs.h"
#undef CHECK                              // termdefs.h has its own CHECK (a flag)
#include "check.h"

TEST(term_stack_holds_more_than_the_old_maxtermdeep) {
  const int n = 50000;                    // 2.5 times the old limit
  term t;
  for (int i = 0; i < n; i++) t.crstterm(i, TNUMBER);
  bool ok = true;
  for (int i = n - 1; i >= 0; i--) {
    term u;
    u.popt();
    if (u.inf() != TNUMBER || u.head() != i) ok = false;
  }
  CHECK(ok);
}

TEST(term_stack_is_reusable_after_growth) {
  term t;
  t.crstterm(7, TNUMBER);
  t.crstterm(8, TNUMBER);
  term a, b;
  a.popt();
  b.popt();
  CHECK_EQ(a.head(), 8);
  CHECK_EQ(b.head(), 7);
}

int main() { return run_tests(); }
