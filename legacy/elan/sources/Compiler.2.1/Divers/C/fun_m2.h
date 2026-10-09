#ifndef _fun_m2_h
#define _fun_m2_h

#include "term.h"
#include "bitset.h"
#include "bgraph.h"
#include "match_state.h"

struct term *fun_m2();
extern void init_pattern_list_m2();
extern void init_pattern_list_m2p1();
extern void delete_pattern_list_m2();
extern void delete_pattern_list_m2p1();

int match_subterm_m2(struct term *t,int no_arg_subject,
		      bitSet *mask, BG *cbg);
int match_subterm_m2p1(struct term *t,int no_arg_subject,
			bitSet *mask, BG *cbg);
void m2_variable_extract(struct term *t,
			 int id_pattern,
			 struct term *extract_substitution[],
			 int *indice,
			 // Pour le 2eme niveau :
			 struct match_state *ms,
			 int no_arg_subject,
			 int no_pattern
			 );

void m2p1_variable_extract(struct term *t,
			 int id_pattern,
			 struct term *extract_substitution[],
			 int *indice,
			 // Pour le 2eme niveau :
			 struct match_state *ms,
			 int no_arg_subject,
			 int no_pattern
			 );


#endif
