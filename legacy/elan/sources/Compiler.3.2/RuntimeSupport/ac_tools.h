#ifndef _ac_tools_h
#define _ac_tools_h

#include "term.h"
#include "match_state.h"
#include "bgraph.h"

extern int is_maximal_identical_element(struct termac *t, 
                                        multiplicityType multiplicity,
                                        struct term **ptr_list_x);

extern void extract_maximal_identical_element(struct termac *t, 
                                              multiplicityType multiplicity,
                                              struct termac **ptr_list_x,
                                              struct termac **ptr_list_y);

extern void extract_minimal_identical_element(struct termac *t, 
                                              multiplicityType multiplicity,
                                              struct termac **ptr_list_x,
                                              struct termac **ptr_list_y);

extern void substitution_build(struct termac *t, match_state *ms,
                               int nb_variable, struct term *substitution[],
                               int nb_variable_ac,
                               void (*variable_extract)(),
                               int base_id_pattern);

extern void substitution_build_without_context(struct termac *t,
                                               match_state *ms,
                                               int nb_variable,
                                               struct term *substitution[],
                                               int nb_variable_ac,
                                               void (*variable_extract)(),
                                               int base_id_pattern);

extern struct termac *rest_extract(struct termac *t, match_state *ms);

extern void extract_xy_from_pe(struct termac *t, multiplicityType E[], multiplicityType sol[],
                               multiplicityType multiplicity,
                               struct termac **ptr_list_x,
                               struct termac **ptr_list_y);

extern int minimal_extract_fail(int taille, multiplicityType E[], multiplicityType sol[], multiplicityType multiplicity, int det);

extern int minimal_extract(int taille, multiplicityType E[], multiplicityType sol[], multiplicityType multiplicity, int det);

extern int next_minimal_extract(int total, multiplicityType E[], multiplicityType sol[], multiplicityType multiplicity);
extern int next_maximal_extract(int total, multiplicityType E[], multiplicityType sol[], multiplicityType multiplicity);


extern int maximal_extract_fail(int taille, multiplicityType E[], multiplicityType sol[], multiplicityType multiplicity, int det);

extern int next_pe_extract(int pos, multiplicityType E[], multiplicityType sol[], multiplicityType multiplicity);

extern int next_pe_extract_fail(int pos, multiplicityType E[], multiplicityType sol[], multiplicityType multiplicity);

extern int next_pe_extract2(int total, int pos, multiplicityType E[], multiplicityType sol[], multiplicityType multiplicity);

extern int next_pe_extract2_fail(int total,int pos, multiplicityType E[], multiplicityType sol[], multiplicityType multiplicity);

extern int match_subterm_AC(int base_id_pattern,
                            int no_arg_subject,
                            int *mask,
                            BG *cbg,
                            int (*my_match_subterm)(),
                            int nb_rule,
                            int **pattern_list,
                            int nb_pattern_in_cbg,
                            struct term *t,
                            int max_nb_pattern_under_AC);

#endif
