/*
		(c)	INRIA-Lorraine & CRIN
			615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
			email: elan@loria.fr

	$Source: /users/protheo/pot/ELAN/CVSELAN/elan/sources/Compiler/RuntimeSupport/ACMatcher/acmatchdefs.h,v $
	$Revision: 1.2 $
	$Date: 1998/11/25 08:28:56 $
	$Author: moreau $
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

