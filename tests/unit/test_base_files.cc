// Unit tests of base/ file lookup: module files are matched with their exact
// name, also on case-insensitive file systems (default macOS volume).
#include "check.h"
#include "commondefs.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unistd.h>

static std::string make_dir_with(const char *file) {
  char tmpl[] = "/tmp/elan-case-XXXXXX";       // /tmp: the default (case-insensitive) volume on macOS
  std::string d = mkdtemp(tmpl);
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

int main() { return run_tests(); }
