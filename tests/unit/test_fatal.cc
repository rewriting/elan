// Unit tests of fatal errors (base/fatal.h) and of terms built on a loaded
// symbol table.
//
// Until S3b, failexit() called exit(1): code that reports a fatal error
// could not be tested in-process. Inside a FatalCatcher scope it now throws
// ElanFatal; it still exits directly in a forked child, while an exception
// is propagating, and from a destructor (through the terminate handler).
// The exit cases are checked in a forked process.
#include "commondefs.h"
#include "termdefs.h"
#include "module.h"
#include "elan_main.h"
#undef CHECK                              // termdefs.h has its own CHECK (a flag)
#include "check.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

// Runs f in a forked child; returns its exit status (-1 if it did not exit).
static int status_of_child(void (*f)()) {
  fflush(stdout); fflush(stderr);
  pid_t pid = fork();
  if (pid == 0) { f(); _exit(98); }      // 98: f returned
  int st = 0;
  waitpid(pid, &st, 0);
  return WIFEXITED(st) ? WEXITSTATUS(st) : -1;
}

TEST(failexit_throws_elanfatal_inside_a_catcher) {
  FatalCatcher catcher;
  int status = -1;
  try { failexit(); } catch (const ElanFatal &e) { status = e.status; }
  CHECK_EQ(status, 1);
}

// The term module reports misuse with failexit(): testable now.
TEST(term_getstring_of_a_number_is_a_fatal_error) {
  FatalCatcher catcher;
  term t;
  t.crstterm(42, TNUMBER);
  term n;
  n.popt();
  bool caught = false;
  try { n.getstring(); } catch (const ElanFatal &) { caught = true; }
  CHECK(caught);
}

TEST(stringtab_overflow_is_a_fatal_error) {
  FatalCatcher catcher;
  stringtab tab(4);
  bool caught = false;
  try {
    tab.addstr("a"); tab.addstr("b"); tab.addstr("c"); tab.addstr("d");
  } catch (const ElanFatal &) { caught = true; }
  CHECK(caught);
}

TEST(failexit_exits_without_a_catcher) {
  CHECK_EQ(status_of_child([] { failexit(); }), 1);
}

TEST(failexit_exits_directly_in_a_forked_child) {
  CHECK_EQ(status_of_child([] {
    FatalCatcher catcher;
    fatal_in_forked_child();
    try { failexit(); } catch (...) { _exit(99); }   // 99: it was thrown
  }), 1);
}

struct FailsWhenDestroyed { ~FailsWhenDestroyed() { failexit(); } };

TEST(failexit_in_a_destructor_during_unwinding_exits) {
  CHECK_EQ(status_of_child([] {
    FatalCatcher catcher;
    try { FailsWhenDestroyed d; throw 5; } catch (...) { _exit(99); }
  }), 1);
}

TEST(failexit_in_a_destructor_exits_through_the_terminate_handler) {
  CHECK_EQ(status_of_child([] {
    FatalCatcher catcher;
    try { FailsWhenDestroyed d; } catch (...) { _exit(99); }
  }), 1);
}

// ---------------------------------------------------------------------------
// Terms on a loaded symbol table: elan_main loads a small program (the
// library of the source tree), then terms are built with its symbol codes
// and printed.

static std::string source_root() {
  std::string f = __FILE__;               // <root>/tests/unit/test_fatal.cc
  for (int i = 0; i < 3; i++) f = f.substr(0, f.rfind('/'));
  return f;
}

static void write_file(const std::string &name, const char *text) {
  FILE *f = fopen(name.c_str(), "w");
  fputs(text, f);
  fclose(f);
}

// Runs elan_main in dir with stdin and stdout redirected; returns its status.
static int run_elan(const std::string &dir, const char *lgi, const char *input) {
  write_file(dir + "/input", input);
  char old[4096];
  if (!getcwd(old, sizeof old)) return -1;
  if (chdir(dir.c_str()) != 0) return -1;
  fflush(stdout);
  int saved_in = dup(0), saved_out = dup(1);
  int in = open("input", O_RDONLY), out = open("output", O_WRONLY | O_CREAT | O_TRUNC, 0644);
  dup2(in, 0); dup2(out, 1); close(in); close(out);
  char prog[] = "elan", batch[] = "-b", name[256];
  snprintf(name, sizeof name, "%s", lgi);   // elan_main writes into its arguments
  char *argv[] = {prog, batch, name, nullptr};
  int status = elan_main(3, argv);
  fflush(stdout);
  dup2(saved_in, 0); dup2(saved_out, 1); close(saved_in); close(saved_out);
  clearerr(stdin);
  if (chdir(old) != 0) return -1;
  return status;
}

// The code of the user symbol whose syntax starts with the identifier name
// and has the given arity, or -1.
static int symbol_code(const char *name, int arity) {
  for (int i = FSYMCODESBEG; i < fsymtabi; i++) {
    struct sgrammrule *gr = fsymtab[i].textform();
    if (gr && fsymtab[i].arity() == arity && gr->rside[0].isident()
        && strcmp(gr->rside[0].alfsy(), name) == 0)
      return i;
  }
  return -1;
}

static std::string written(term t) {
  FILE *f = tmpfile();
  {
    ochstream os(f);
    t.write(os);
    os.flush();
    rewind(f);
    char buf[256] = {0};
    size_t n = fread(buf, 1, sizeof buf - 1, f);
    return std::string(buf, n);
  }                                       // ~ochstream closes f
}

static bool program_loaded = false;

TEST(elan_main_loads_a_program) {
  char tmpl[] = "/tmp/elan-term-XXXXXX";
  std::string dir = mkdtemp(tmpl);
  // ELANLIB is a prefix: the library is in $ELANLIB/share/elanlib
  CHECK_EQ(mkdir((dir + "/share").c_str(), 0755), 0);
  CHECK_EQ(symlink((source_root() + "/src/lib/elanlib").c_str(), (dir + "/share/elanlib").c_str()), 0);
  setenv("ELANLIB", dir.c_str(), 1);
  write_file(dir + "/nat.eln",
             "module nat\n"
             "sort nat; end\n"
             "operators global\n"
             "  zero : nat;\n"
             "  succ(@) : (nat) nat;\n"
             "  @ + @ : (nat nat) nat;\n"
             "end\n"
             "end\n");
  write_file(dir + "/prog.lgi",
             "LPL prog description\n"
             "  query    of sort nat\n"
             "  result   of sort nat\n"
             "  import   nat\n"
             "  start with () query\n"
             "end\n");
  CHECK_EQ(run_elan(dir, "prog.lgi", "succ(zero) end\n"), 0);
  program_loaded = true;
  FILE *f = fopen((dir + "/output").c_str(), "r");
  char buf[4096] = {0};
  size_t n = f ? fread(buf, 1, sizeof buf - 1, f) : 0;
  if (f) fclose(f);
  CHECK(std::string(buf, n).find("succ(zero)") != std::string::npos);
  std::string cmd = "rm -rf '" + dir + "'";
  CHECK_EQ(system(cmd.c_str()), 0);
}

TEST(term_built_with_loaded_symbols_is_printed_with_their_syntax) {
  if (!program_loaded) { CHECK(program_loaded); return; }
  int zero = symbol_code("zero", 0), succ = symbol_code("succ", 1);
  CHECK(zero >= FSYMCODESBEG);
  CHECK(succ >= FSYMCODESBEG);
  if (zero < 0 || succ < 0) return;
  term t;
  t.stinit();
  t.crterm(zero);
  t.crterm(succ);
  t.crterm(succ);
  t.popt();
  CHECK_EQ(t.headarity(), 1);
  CHECK_EQ(t.head(), succ);
  CHECK_EQ(written(t), std::string(" succ(succ(zero))"));   // write() starts with a blank
}

// Last: a fatal error leaves the tables half-loaded. elan_main returns 1
// (it used to exit) with the message on stderr, and the caller goes on.
TEST(elan_main_returns_1_on_a_fatal_error) {
  char tmpl[] = "/tmp/elan-fatal-XXXXXX";
  std::string dir = mkdtemp(tmpl);
  write_file(dir + "/bad.lgi",
             "LPL bad description\n"
             "  query    of sort nat\n"
             "  result   of sort nat\n"
             "  import   nosuchmodule\n"
             "  start with () query\n"
             "end\n");
  CHECK_EQ(run_elan(dir, "bad.lgi", ""), 1);
  std::string cmd = "rm -rf '" + dir + "'";
  CHECK_EQ(system(cmd.c_str()), 0);
}

int main() { return run_tests(); }
