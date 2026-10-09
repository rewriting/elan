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

#ifndef module_h
#define module_h

#include "commondefs.h"
#include "termdefs.h"

extern stringtab processtab;
extern struct processdatalist *processlists[];
#include "rtdatas.h"

#define SIZE_rulecashstrings  1024
extern stringtab rulecashstrings;  
extern struct transrulelist *rulecashtable[];

struct definedaslist {
  lexem fname;
  int code;
  struct definedaslist *next;
};

struct inlineslist {
  lbuffer *text;
  struct inlineslist *next;
};

extern stringtab *stratrules;  /* size MAXNOFTRN+MAXNOFSTRAT */
extern strategy  *all_strateg[];
extern int all_strategi;

extern stringtab import;	// name of modules, which were imported
extern grammar *importglobgr[MAXNOFIMPORTS];
extern char *visibilities[MAXNOFIMPORTS];
// grammaires exportees pour chaque module 
// les indices sont les memes que dans le tableau import
extern stringtab typet;
extern stringtab builtinmodules;
extern fsym fsymtab[FSYMTABSIZE];
extern int fsymtabi;
extern lexem booltype,identype,numtype,internIdentType,internIntType;
extern lexem stringtype,internStringType;
//extern lexem strIdentType;
extern int strIdentVal;
extern lexem strIdentLex;
extern lexem internStratIntType;

extern grammar globtermgr;
extern grammar *actgram();
extern grammar *topgrammar;

extern int interruptnow;

extern trsystem trrules;

extern struct definedaslist *definedasl;
extern struct inlineslist *inlinesl;

extern term trueterm;
extern term falseterm;
extern void settrueterm();
extern void modinit();
extern int all_modules_loaded;
extern char *specsource,*specname,*modname;

// body in msemact.cc (moved from commondefs.h)
extern int semact(int, lexem ,lstream *);  //semaction for compiling module

#endif
