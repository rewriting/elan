/*
  
    REM - Reduce ELAN Machine

    Copyright (C) 2000-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
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

    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr
    Christophe Ringeissen	e-mail: Christophe.Ringeissen@loria.fr
    Marian Vittek		e-mail: vittek@guma.ii.fmph.uniba.sk

*/
/*
		(c)	INRIA-Lorraine & CRIN
			615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
			email: elan@loria.fr

	$Source: /CVS/aircube/elan3/src/elan-compiler/earley/stringtab.h,v $
	$Revision: 1.1.1.1 $
	$Date: 2001/11/29 08:28:44 $
	$Author: pem $
*/

#ifndef __stringtab_h
#define __stringtab_h

#ifdef SUN
#include "stream.h"
#endif

#ifdef HP
#include <iostream.h>
#include <fstream.h>
#endif

#ifdef ALPHA
#include <iostream.h>
#include <fstream.h>
#endif

#ifdef ATERM
//#include "lstream.h"
#endif

#ifdef GCMEM
#include "gc_cpp.h"
#endif

#ifdef EARLEY
class ochstream;
#endif


class stringtab
#ifdef GCMEM
: public gc
#endif
 {
 private:
  int tsize,strin;          // size  of table, actual number of strings in it
  int forpos;		   // actual position for for-cycle
  char **id;               // hashed table of strings

  void stringtab::init();
  void stringtab::stinit(int);
 public:

  int posid;                // position of last founded member 
                            // (sets as side effect of addstr and member)

  stringtab(int size);
  stringtab(int size, char * ...);
  ~stringtab();

  char *ide(int n);                   // give string of number n
  void stringtab::dump(char *name); //dump of tab with name
#ifdef EARLEY
  void stringtab::earleyDump(ochstream&,char *name); //dump of tab with name
#endif
  void stringtab::write(char *,char *); //write table values
  int stringtab::addstr(char *s);   // add string s into the table and return 
                                    // his number
  void stringtab::removestr(int n);   // remove string of #n from table !!!!!!!
                                      // remove must be done in reversed 
                                      // order than add !!!!!!!!!!!!!!!!!!!!!!
  int stringtab::member(char *s);   // is s a member of the table ?
  int stringtab::index(char *s);   // is s a member of the table ?

		// following functions for list the strings in for-cycle
  void forinit();
  int forcond();
  void fornext();
  int  forindex();
  char *foractval();
#ifdef ATERM
  void Adump(ochstream &gout,char *heading);
  void Aread(void *f, int skip); // should be (lstream *f);
#endif
};

extern void failexit();          // body in specials.c and module.c

inline char *stringtab::ide(int i) {return (id[i]);}

#endif





