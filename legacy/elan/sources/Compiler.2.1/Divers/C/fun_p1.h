#ifndef _fun_p1_h
#define _fun_p1_h

#include "term.h"
#include "bitset.h"
#include "bgraph.h"

struct term *fun_p1();

extern void init_pattern_list_p1();
extern void delete_pattern_list_p1();

int match_subterm_p1(struct term *t,int no_arg_subject,
		      bitSet *mask, BG *cbg);
void p1_variable_extract(struct term *t,
			 int id_pattern,
			 struct term *extract_substitution[],
			 int *indice,
			 // Pour le 2eme niveau :
			 struct match_state *ms,
			 int no_arg_subject,
			 int no_pattern
			 );


#endif
