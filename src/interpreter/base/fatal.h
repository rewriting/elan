// Fatal errors of the interpreter (S3b).
//
// A fatal error prints its message (on sterr or stderr) and calls
// elan_main is not re-entrant after an ElanFatal: the loader state (stacki,
// stack indices) is left as it was when the error occurred.
// failexit(). Inside a FatalCatcher scope -- elan_main, a unit test --
// failexit() throws ElanFatal; elan_main catches it at top level, calls
// fatal_cleanup() (flush stout and sterr, then kill the subprocesses) and
// returns 1. The output and the exit status are those of the former
// failexit(), which did the same and called exit(1); the difference is that
// the stack is unwound (destructors of locals run: input files are closed,
// output files such as the .ref file are flushed and closed, as exit() did).
//
// failexit() still exits directly (fatal_exit_now(): fatal_cleanup(), then
// exit(1)) where unwinding is not possible or not wanted:
//   - outside any FatalCatcher: static initialisation, tools;
//   - after exit() has started (static destruction);
//   - in a forked child (match/process.cc: fatal_in_forked_child() right
//     after fork()): unwinding would run the parent's frames in the child;
//   - in the ^C handler (driver/ldmain.cc: FatalDirectExit), a signal
//     handler, left by longjmp;
//   - while an exception is already propagating (a destructor run by the
//     unwinding).
// A failexit() in a destructor outside unwinding throws out of a noexcept
// function: the terminate handler installed by FatalCatcher recognises
// ElanFatal and calls fatal_exit_now(), as before. The same handler covers
// an ElanFatal for which the C++ runtime finds no handler and calls
// std::terminate without unwinding (GCC drops a catch it proves unreachable,
// e.g. behind a cleanup that never returns).
//
// No exception crosses C frames: the C AC matcher (acmatcher/, libmatch)
// calls back no C++ code, and the generated parsers are C++.
#ifndef __fatal_h
#define __fatal_h

#include <exception>

class ElanFatal : public std::exception {
 public:
  int status = 1;                       // exit status of elan
  const char *what() const noexcept override;
};

[[noreturn]] extern void failexit();    // body in load/module.cc (tools: parse/tools/specials.cc)
[[noreturn]] extern void interr();      // body in base/commondefs.cc
extern void fatal_cleanup();            // flush stout, sterr; kill the subprocesses
[[noreturn]] extern void fatal_exit_now(); // fatal_cleanup(), then exit(1)
extern void fatal_in_forked_child();    // from now on, failexit() exits directly

// While a FatalCatcher exists, failexit() throws ElanFatal (unless one of
// the cases above applies). Scopes nest.
class FatalCatcher {
 public:
  FatalCatcher();
  ~FatalCatcher();
  FatalCatcher(const FatalCatcher &) = delete;
  FatalCatcher &operator=(const FatalCatcher &) = delete;
};

// failexit() exits directly while this flag is set (the ^C handler).
extern void fatal_direct_exit(bool on);

#endif
