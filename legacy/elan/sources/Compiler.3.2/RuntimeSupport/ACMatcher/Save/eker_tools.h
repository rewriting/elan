#ifndef _eker_tools_h
#define _eker_tools_h

#include <stdio.h>
#include "term.h"
#include "acmatchdefs.h"

extern TERM *toEkerForm(struct term *t);
extern struct term *fromEkerForm(TERM *t);


#endif
