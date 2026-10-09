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

*/
/*
		(c)	INRIA-Lorraine & CRIN
			615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
			email: elan@loria.fr

	$Source: /CVS/aircube/elan3/src/elan-compiler/runtime/acmatchdefs.h,v $
	$Revision: 1.1.1.1 $
	$Date: 2001/11/29 08:28:44 $
	$Author: pem $
*/
#ifndef acmatchdefs_h
#define acmatchdefs_h

#include "sym_types.h"
#include "term_types.h"
#include "eker_tools.h"

extern void *build_match(TERM *p, TERM *s, int n);

extern BOOL extract_match(void *m, TERM *a[]);
extern void destroy_match(void *m);
extern void destroy_term(TERM *t);
extern TERM_LIST *make_term_list(TERM *n, TERM_LIST *rest);
extern TERM *make_term(int id, TERM_LIST *args,SYM_TYPE type);
extern TERM *make_ac_term(int id, AC_LIST *args,SYM_TYPE type);
extern AC_LIST *make_ac_list(TERM *new, int multiplicity, AC_LIST *rest);

extern void eker_print_term(TERM *t);
extern void eker_print_tlist(TERM_LIST *l);
extern void eker_print_aclist(AC_LIST *l);
#endif

