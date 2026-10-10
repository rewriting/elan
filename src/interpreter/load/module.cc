/*
  
    ELAN

    Copyright (C) 1994-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
			     Nancy, France.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    Peter Borovansky		e-mail: borovan@fmph.uniba.sk
    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr
    Marian Vittek		e-mail: vittek@guma.ii.fmph.uniba.sk

*/

#include <cstdlib>
#include <exception>
#include "module.h"
#include "strategy.h"

stringtab import(MAXNOFIMPORTS);
grammar *importglobgr[MAXNOFIMPORTS];

stringtab typet(NTYPES,"bool","ident","builtinInt",
		"builtinString","intern string",
		"intern ident","intern int",NULL);

stringtab builtinmodules(64,"bool","builtinInt","ident","double",
		"doubleConstants","common",
		"cmp","replace","occur","builtinSyntacticMatching",
		"builtinIO","builtinStdio", // ELAN's ios
                "Query",          // description of command language
                "builtinString",
                 "test",// Modules ajoutes pour faire des tests
                 "pem1",// Modules ajoutes pour faire des tests
                 "pem2",// Modules ajoutes pour faire des tests
                 "fac",// Modules ajoutes pour faire des tests
                 "Meta_apply",    // meta_apply and set_of for interepreter 
                 "Meta_capply",   // meta_apply and set_of for compiler
                 "Meta_strat",    // description of Marian's strategies in ELN 
		 "strass",        // codes for lets and inline rules
                 "REF",
                 "refstring2string",
//               "ref2term",
                 "ref2string",
                 "Meta_Apply",
                 "divers2string",
                 "strsig",
                 "strat",
                 "strconc",
                 "builtinArray",
                 "builtinHashTerm",
                 "builtinEqMolecule",
		 NULL);	// modules authorised to use code
lexem booltype,identype,numtype,internIdentType,internIntType;
int all_modules_loaded = 0;
lexem stringtype,internStringType;
int strIdentVal = 0;
lexem strIdentLex;
lexem internStratIntType;

stringtab *stratrules; /* MAXNOFTRN+MAXNOFSTRAT */
strategy  *all_strateg[MAXNOFSTRAT];
int       all_strategi = 0;

statistics statistic;

grammar globtermgr;
grammar *topgrammar;

term trueterm, falseterm;

fsym fsymtab[FSYMTABSIZE];

int fsymtabi=FSYMCODESBEG;

stringtab processtab(MAXNOFSUBPROCESS);
struct processdatalist *processlists[MAXNOFSUBPROCESS];
struct definedaslist *definedasl=NULL;
struct inlineslist *inlinesl=NULL;

stringtab rulecashstrings(SIZE_rulecashstrings);  
struct transrulelist *rulecashtable[SIZE_rulecashstrings];

trsystem trrules;

int interruptnow=0;

void modinit()
{ 
  struct sgrammrule *gr;
  globtermgr.addsymbol(booltype);
  gr = globtermgr.addrule(booltype,STANDPRI,RSTANDOP,RIGHTSRULE);
  fsym s(1,gr,0);
  fsymtab[COMPILMAIN]=s;
}

void settrueterm()
{
  trueterm.stinit();  trueterm.crterm(TRUEVAL);   trueterm.popt();
  falseterm.stinit(); falseterm.crterm(FALSEVAL); falseterm.popt();
}

// Fatal errors: see base/fatal.h.
namespace {
int fatal_catchers = 0;           // FatalCatcher scopes alive
bool fatal_forked_child = false;  // in a forked child (match/process.cc)
bool fatal_direct = false;        // in the ^C handler
bool fatal_exiting = false;       // exit() has started (static destruction)
std::terminate_handler fatal_previous_terminate = nullptr;

// A failexit() in a noexcept function (a destructor) ends here: exit as
// failexit() did before S3b.
[[noreturn]] void fatal_terminate()
{
  if (std::exception_ptr e = std::current_exception()) {
    try { std::rethrow_exception(e); }
    catch (const ElanFatal &) { fatal_exit_now(); }
    catch (...) {}
  }
  if (fatal_previous_terminate) fatal_previous_terminate();
  std::abort();
}

bool fatal_can_throw()
{
  return fatal_catchers > 0 && !fatal_forked_child && !fatal_direct
      && !fatal_exiting && std::uncaught_exceptions() == 0;
}
}  // namespace

const char *ElanFatal::what() const noexcept { return "elan: fatal error"; }

FatalCatcher::FatalCatcher()
{
  static bool installed = false;
  if (!installed) {
    installed = true;
    fatal_previous_terminate = std::set_terminate(fatal_terminate);
    std::atexit([] { fatal_exiting = true; });
  }
  fatal_catchers++;
}

FatalCatcher::~FatalCatcher() { fatal_catchers--; }

void fatal_in_forked_child() { fatal_forked_child = true; }

void fatal_direct_exit(bool on) { fatal_direct = on; }

void fatal_cleanup()
{
  stout.flush(); sterr.flush();
  kill_all_processus();
}

void fatal_exit_now()
{
  fatal_cleanup();
  exit(EXIT_FAILURE);
}

void failexit()
{
  if (fatal_can_throw()) throw ElanFatal();
  fatal_exit_now();
}



