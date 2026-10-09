// Minimal unit-test support for the ELAN interpreter (no external framework).
//
//   #include "check.h"
//   TEST(stringtab_adds_and_finds) { ...; CHECK(cond); CHECK_EQ(a, b); }
//   int main() { return run_tests(); }
//
// Each TEST registers itself; run_tests() runs them all, prints the failures
// and returns the number of failed checks (0 = success) for CTest.
#ifndef ELAN_TESTS_CHECK_H
#define ELAN_TESTS_CHECK_H

#include <cstdio>
#include <cstring>
#include <vector>

namespace elantest {
struct Case { const char *name; void (*fn)(); };
inline std::vector<Case> &cases() { static std::vector<Case> v; return v; }
inline int &failures() { static int n = 0; return n; }
struct Register { Register(const char *n, void (*f)()) { cases().push_back({n, f}); } };
inline void fail(const char *file, int line, const char *what) {
  std::fprintf(stderr, "%s:%d: CHECK failed: %s\n", file, line, what);
  ++failures();
}
}  // namespace elantest

#define TEST(name)                                                        \
  static void name();                                                     \
  static elantest::Register name##_registration(#name, name);            \
  static void name()

#define CHECK(cond) \
  do { if (!(cond)) elantest::fail(__FILE__, __LINE__, #cond); } while (0)

#define CHECK_EQ(a, b) \
  do { if (!((a) == (b))) elantest::fail(__FILE__, __LINE__, #a " == " #b); } while (0)

#define CHECK_STREQ(a, b) \
  do { if (std::strcmp((a), (b)) != 0) elantest::fail(__FILE__, __LINE__, #a " eq " #b); } while (0)

inline int run_tests() {
  for (const auto &c : elantest::cases()) {
    int before = elantest::failures();
    c.fn();
    std::printf("%s %s\n", elantest::failures() == before ? "ok  " : "FAIL", c.name);
  }
  std::printf("%zu tests, %d failed checks\n", elantest::cases().size(), elantest::failures());
  return elantest::failures() == 0 ? 0 : 1;
}

#endif
