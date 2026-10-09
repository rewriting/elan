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

	$Source: /CVS/aircube/elan3/src/elan-compiler/earley/mitab.h,v $
	$Revision: 1.1.1.1 $
	$Date: 2001/11/29 08:28:44 $
	$Author: pem $
*/

#ifndef __mitab_h
#define __mitab_h




#define NOINT -1


class mitab
  {
 private:
  int tsize,nin;          // size  of table, actual number of numbers in it
  int forpos;		   // actual position for for-cycle
  struct recstr{
    int n,nofoc;           // number and number of occurences
  } *id;               // hashed table of pairs

  void mitab::init();
  void mitab::stinit(int);
 public:

  int posid;                // position of last founded member 
                            // (sets as side effect of addstr and member)

  mitab(int size);
  mitab(int size, int ...);
  ~mitab();

//  char *ide(int n);                   // give string of number n
  void mitab::dump(char *); //dump of tab with name
//  void mitab::write(char *,char *); //write table values
  int mitab::addn(int n);   // add n into the table and return 
                                    // his number
  void mitab::removen(int n);
  void mitab::removenonp(int ind);   // remove string of index n from table !!
                                      // remove must be done in reversed 
                                      // order than add !!!!!!!!!!!!!!!!!!!!!!
  int mitab::member(int n);   // is n a member of the table ?
                                  // if yes the posid is its index

		// following functions for list the strings in for-cycle
  void forinit();
  forcond();
  void fornext();
  int foractval();
};

extern void failexit();          // body in specials.c and module.c
//inline char *mitab::ide(int i) {return (id[i]);}


#endif
