/*
		(c)	INRIA-Lorraine & CRIN
			615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
			email: elan@loria.fr

	$Source: /users/protheo/pot/ELAN/CVSELAN/elan/sources/Compiler/RuntimeSupport/ACMatcher/acmatchdefs.h,v $
	$Revision: 1.1 $
	$Date: 1998/09/04 14:32:00 $
	$Author: borovan $
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
extern TERM *make_term(int id, TERM_LIST *args,SYM_TYPE type,int len, TERM_LIST *p);

#endif

