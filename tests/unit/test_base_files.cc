// Unit tests of base/ file lookup: module files are matched with their exact
// name, also on case-insensitive file systems (default macOS volume).
#include "check.h"
#include "commondefs.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <filesystem>
#include <vector>

// Temporary directories created by the tests, removed at the end of main.
static std::vector<std::string> &tmpdirs() { static std::vector<std::string> v; return v; }

static std::string make_dir_with(const char *file) {
  char tmpl[] = "/tmp/elan-case-XXXXXX";       // /tmp: the default (case-insensitive) volume on macOS
  std::string d = mkdtemp(tmpl);
  tmpdirs().push_back(d);
  FILE *f = fopen((d + "/" + file).c_str(), "w");
  fputs("module Foo end\n", f);
  fclose(f);
  return d;
}

TEST(fopen_exact_opens_a_file_with_its_exact_name) {
  std::string d = make_dir_with("Foo.eln");
  FILE *f = fopen_exact((d + "/Foo.eln").c_str());
  CHECK(f != NULL);
  if (f) fclose(f);
}

TEST(fopen_exact_refuses_a_name_differing_only_by_case) {
  std::string d = make_dir_with("Foo.eln");
  FILE *probe = fopen((d + "/foo.eln").c_str(), "r");
  if (probe) {                                 // case-insensitive volume: plain fopen accepts it
    fclose(probe);
    CHECK(fopen_exact((d + "/foo.eln").c_str()) == NULL);
  }
}

TEST(fopen_exact_missing_file) {
  std::string d = make_dir_with("Foo.eln");
  CHECK(fopen_exact((d + "/Bar.eln").c_str()) == NULL);
}

TEST(fopen_exact_relative_name_in_current_directory) {
  std::string d = make_dir_with("Foo.eln");
  char old[4096];
  CHECK(getcwd(old, sizeof old) != NULL);
  CHECK(chdir(d.c_str()) == 0);
  FILE *f = fopen_exact("Foo.eln");
  CHECK(f != NULL);
  if (f) fclose(f);
  CHECK(fopen_exact("foo.eln") == NULL || access("foo.eln", F_OK) != 0);
  CHECK(chdir(old) == 0);
}

// A file named by the user (current directory, first attempt of the search)
// opens as in 2004, whatever its case: only library lookups are exact-case.
TEST(ichstream_user_named_file_keeps_2004_behaviour) {
  std::string d = make_dir_with("Foo.eln");
  char old[4096];
  CHECK(getcwd(old, sizeof old) != NULL);
  CHECK(chdir(d.c_str()) == 0);
  FILE *probe = fopen("foo.eln", "r");
  if (probe) {                                 // case-insensitive volume
    fclose(probe);
    ichstream in("foo.eln");                   // failexit() (exit) if refused
    int c;
    in.ich(c);
    CHECK_EQ(c, 'm');
  }
  CHECK(chdir(old) == 0);
}

int main() {
  int status = run_tests();
  for (auto &d : tmpdirs()) std::filesystem::remove_all(d);
  return status;
}
