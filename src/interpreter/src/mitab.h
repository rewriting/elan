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


#ifndef __mitab_h
#define __mitab_h

/*
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
*/

#define NOINT -1


class mitab
  {
 private:
  int tsize,nin;          // size  of table, actual number of numbers in it
  int forpos;		   // actual position for for-cycle
  struct recstr{
    int n,nofoc;           // number and number of occurences
  } *id;               // hashed table of pairs

  void init();
  void stinit(int);
 public:

  int posid;                // position of last founded member 
                            // (sets as side effect of addstr and member)

  mitab(int size);
  mitab(int size, int ...);
  ~mitab();

//  char *ide(int n);                   // give string of number n
  void dump(char *); //dump of tab with name
//  void write(char *,char *); //write table values
  int addn(int n);   // add n into the table and return 
                                    // his number
  void removen(int n);
  void removenonp(int ind);   // remove string of index n from table !!
                                      // remove must be done in reversed 
                                      // order than add !!!!!!!!!!!!!!!!!!!!!!
  int member(int n);   // is n a member of the table ?
                                  // if yes the posid is its index

		// following functions for list the strings in for-cycle
  void forinit();
  int forcond();
  void fornext();
  int foractval();
};

extern void failexit();          // body in specials.c and module.c
//inline char *mitab::ide(int i) {return (id[i]);}


#endif
