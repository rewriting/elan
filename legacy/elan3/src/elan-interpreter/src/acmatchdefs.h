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

#ifndef acmatchdefs_h
#define acmatchdefs_h

#include "sym_types.h"
#include "eker_term.h"


extern "C" void *build_match(TERM *p, TERM *s, int n);

extern "C" BOOL extract_match(void *m, TERM *a[]);
extern "C" void destroy_match(void *m);
extern "C" void destroy_term(TERM *t);
extern "C" TERM_LIST *make_term_list(TERM *n, TERM_LIST *rest);
extern "C" TERM *make_term(int id, TERM_LIST *args,SYM_TYPE type,int len, TERM_LIST *p);
/*
extern "C" void *build_match();

extern "C" BOOL extract_match();
extern "C" void destroy_match();
extern "C" void destroy_term();
extern "C" TERM_LIST *make_term_list();
extern "C" TERM *make_term();
*/

//void print_tlist(TERM_LIST *);
//void print_term(TERM *t);



#endif

