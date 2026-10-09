#ifndef _eker_tools_h
#define _eker_tools_h

#include <stdio.h>
#include "term.h"
#include "acmatchdefs.h"

extern TERM *toEkerForm(struct term *t);
extern struct term *fromEkerForm(TERM *t);

extern print_term(TERM *t);
extern print_tlist(TERM_LIST *l);
extern print_aclist(AC_LIST *l);

#endif
