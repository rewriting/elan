// elan-lextrace: the lexem-stream harness of the Go front end (S8a, spec S8
// S8-11). It is linked with elan_core and with its own compilation of
// src/interpreter/lex/mlstream.cc under -DELAN_LEXTRACE (the trace hooks);
// elan itself does not change (tests/front/check_lextrace_hooks.sh).
//
//   elan-lextrace lex FILE
//       FILE.eln through mlstream (the preprocessor), any other file through
//       a plain lstream (.lgi, .spc: '_' is part of identifiers); one line
//       per lexem on stdout, `eos` last, "@inline <chars>" after a lexem
//       during whose reading an inline /# ... #/ was read; stderr and exit
//       status those of the C++ (a fatal error exits with status 1); an IF
//       (it needs the loader) is refused: message, status 3.
//   elan-lextrace load TRACE [elan options] prog.lgi [spec]
//       elan_main on the remaining arguments (argv[0] kept), the lexems of
//       every module written to TRACE.
//
// Trace format (tests/front/README.md): `id <pos> <text>`, `num <n>`,
// `char <c>` (decimal), `str <i> <text>`, `eos`, `other <raw>`; texts escaped
// (\\ \n \t \" and bytes < 32 or >= 127 as \ooo); markers `@begin NAME`,
// `@end`, `@foreach-begin`, `@foreach-end` (from the hooks).

#include "commondefs.h"
#include "elan_main.h"
#include "fatal.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

// Defined only in the harness's mlstream.o (-DELAN_LEXTRACE): referencing
// them makes the link fail if the archive's mlstream.o were used instead.
extern FILE *lextrace_out;
void lextrace_lexem(FILE *f, lexem l);
void lextrace_chars(FILE *f, const char *s, size_t n);
extern void (*lextrace_refuse_if)(lstream *f);

static void flush_trace() { if (lextrace_out) fflush(lextrace_out); }

static void write_inline(lstream *f)
{
  lbuffer *b = f->getlastinlineAndinit();
  if (b == NULL) return;
  std::vector<char> chars;
  lexem c;
  while (!b->isempty()) { b->get(c); chars.push_back((char)c.charval()); }
  delete b;
  fputs("@inline ", stdout);
  lextrace_chars(stdout, chars.data(), chars.size());
  fputc('\n', stdout);
}

// The library paths as elan_main sets them (driver/ldmain.cc), without
// options: $ELANLIB or PREFIX, $SECONDELANLIB or "..".
static void init_paths()
{
  elanlib = getenv("ELANLIB");
  if (elanlib == NULL) elanlib = PREFIX;
  elanlib = addsuffix(elanlib, "/");
  strcpy(elanlibqnq, elanlib); strcat(elanlibqnq, "share/elanlib/noquote/");
  strcpy(elanlibcommon, elanlib); strcat(elanlibcommon, "share/elanlib/common/");
  strcpy(elanlibstrat, elanlib); strcat(elanlibstrat, "share/elanlib/strategy/");
  strcpy(elanlibref, elanlib); strcat(elanlibref, "share/elanlib/ref/");
  perslib = getenv("SECONDELANLIB");
  if (perslib == NULL) perslib = "..";
  perslib = addsuffix(perslib, "/");
}

// An IF condition read standalone: booltype and the grammar are the
// loader's (unset, earleycall would read garbage: a bus error was seen), so
// `lex` refuses it with status 3; such files are read in `load` mode.
static void refuse_if(lstream *f)
{
  f->oerr();
  sterr << "[elan-lextrace] IF needs the loader (booltype, the module's grammar): use load mode\n";
  fatal_cleanup();
  exit(3);
}

static int lex_mode(const char *name)
{
  size_t n = strlen(name);
  bool eln = n >= 4 && strcmp(name + n - 4, ".eln") == 0;
  lexem l;
  init_paths();
  lextrace_out = stdout;
  lextrace_refuse_if = refuse_if;
  if (eln) {
    mlstream *f = new mlstream((char *)name);
    do { f->ilex(l); write_inline(f); } while (!l.isendofstream());   // the hook writes l
    delete f;
  } else {
    lstream *f = new lstream(name);
    do { f->ilex(l); lextrace_lexem(stdout, l); write_inline(f); } while (!l.isendofstream());
    delete f;
  }
  fflush(stdout);
  return 0;
}

static int load_mode(int argc, char **argv)
{
  FILE *t = fopen(argv[2], "w");
  if (t == NULL) { perror(argv[2]); return 2; }
  lextrace_out = t;
  std::vector<char *> args;
  args.push_back(argv[0]);
  for (int i = 3; i < argc; i++) args.push_back(argv[i]);
  args.push_back(NULL);
  int status = elan_main((int)args.size() - 1, args.data());
  fclose(t);
  lextrace_out = NULL;
  return status;
}

int main(int argc, char **argv)
{
  atexit(flush_trace);           // failexit() may exit without unwinding
  if (argc == 3 && strcmp(argv[1], "lex") == 0) return lex_mode(argv[2]);
  if (argc >= 4 && strcmp(argv[1], "load") == 0) return load_mode(argc, argv);
  fprintf(stderr, "usage: elan-lextrace lex FILE\n"
                  "       elan-lextrace load TRACE [elan options] prog.lgi [spec]\n");
  return 2;
}
