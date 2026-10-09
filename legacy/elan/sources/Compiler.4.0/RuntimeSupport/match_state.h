#ifndef _match_state_h
#define _match_state_h
#include "bgraph.h"
#include "termCommon.h"

  /*
   * cbg          : Compact Bipartite Graph
   * nb_rule      : nombre de regles de la classe C1 necessitant l'appel
   *                a ce filtrage AC
   * pattern_list : tableau de tableau d'entiers. 
   *                1 tableau par pattern, il contient la liste de
   *                patterns sous AC impliques
   *                la 1ere case de chaque tableau stock le nombre 
   *                d'elements du sous-tableau (de la liste)
   * bg           : current Bipartie Graph
   * no_rule      : numero du pattern correspondant au bg courant
   * bg_solution  : derniere solution du bg courant
   * bg_multiplicity : multiplicite des elements du bg courant
   *                   en fonction du bg_solution
   */

typedef struct match_state {
  BG *cbg;
  int nb_rule;
  int **pattern_list;
  BG *bg;
  int no_rule;
  int *bg_solution;
  multiplicityType *bg_multiplicity;
} match_state;

#define NO_REST 1
#define WITH_REST 2
#define POSSIBLE_REST 3

extern match_state *MS_create();
extern void MS_delete(match_state *ms);
extern int MS_reinit(match_state *ms,struct termac *subject, int no_rule);
extern int MS_init(match_state **ms,int (*match_subterm)(),
		    int nb_pattern, int **pattern_list,
		    int nb_pattern_in_cbg,struct termac *subject,
		    int necessary_link, int max_nb_pattern_under_AC);
extern int  MS_solve(match_state *ms, int mode);
extern int  MS_solve_rule(match_state *ms);
extern void MS_print(match_state *ms);


extern int  MS_pattern_list_size(match_state *ms, int no_rule);
extern int *MS_get_pattern_list(match_state *ms, int no_rule);
int **MS_pattern_list_create(int size);
void MS_pattern_list_free(int **pattern_list,int nb_pattern);
void MS_pattern_list_init(int **pattern_list,int number,
			  int nb_element,int *pattern_tab);


#endif


