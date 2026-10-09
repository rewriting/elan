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

void failexit()
{
  stout.flush(); sterr.flush();
  kill_all_processus();
  exit(EXIT_FAILURE);
}



