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
#ifndef _ac_tools_h
#define _ac_tools_h

#include "termCommon.h"
#include "match_state.h"
#include "bgraph.h"
#include <stdarg.h> 
//#include "Back.h"
#ifdef CSETCHP
#include "choice.h"
#endif
extern int is_maximal_identical_element(struct termac *t, 
                                        multiplicityType multiplicity,
                                        Gterm **ptr_list_x);

extern void extract_maximal_identical_element(struct termac *t, 
                                              multiplicityType multiplicity,
                                              struct termac **ptr_list_x,
                                              struct termac **ptr_list_y);

extern void extract_minimal_identical_element(struct termac *t, 
                                              multiplicityType multiplicity,
                                              struct termac **ptr_list_x,
                                              struct termac **ptr_list_y);

extern void substitution_build(struct termac *t, match_state *ms,
                               int nb_variable, Gterm *substitution[],
                               int nb_variable_ac,
                               void (*variable_extract)(Gterm *v0, int id_pattern, Gterm *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern, int base_id_pattern),
                               int base_id_pattern);

extern void substitution_build_without_context(struct termac *t,
                                               match_state *ms,
                                               int nb_variable,
                                               Gterm *substitution[],
                                               int nb_variable_ac,
                                               void (*variable_extract)(Gterm *v0, int id_pattern, Gterm *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern, int base_id_pattern),
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
                            int (*my_match_subterm)(Gterm *v0, int no_arg_subject, int *mask, BG *cbg),
                            int nb_rule,
                            int **pattern_list,
                            int nb_pattern_in_cbg,
                            Gterm *t,
                            int max_nb_pattern_under_AC);

#endif
