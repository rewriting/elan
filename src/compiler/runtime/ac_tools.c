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
#include "ac_tools.h"


/* ------------------------------------------------------------ */
/* Outils                                                       */
/* ------------------------------------------------------------ */

/*
 * Ces procedures permettent d'extraire des elements identiques
 * is_maximal : tous les elements sans backtracking possible
 * minimal : un seul element repestant la multiplicite est extrait
 * maximal : tous les elements repestant la multiplicite sont extraits
 */


/*
 * retourne 1 s'il y a une solution
 */
int is_maximal_identical_element(struct termac *t, 
				 multiplicityType multiplicity,
				 Gterm **ptr_list_x) {
  struct termac *list_x;
  int i,res=1;
  
  for(i=0; res && i<getArity(t) ; i++) {
    res = res && (getMult(t,i)%multiplicity==0);
  }

  if(res!=1) {
    return 0;
  }
  TERMAC_ALLOC(list_x,getArity(t),GgetSymb((Gterm *)t));
  termac_copyTopSymbol(list_x,t);
  for(i=0 ; i<getArity(t) ; i++) {
    setMult(list_x,i,getMult(list_x,i)/multiplicity);
  }
  return 1;
}

// TODO: color
void extract_maximal_identical_element(struct termac *t, 
				       multiplicityType multiplicity,
				       struct termac **ptr_list_x,
				       struct termac **ptr_list_y) {
  struct termac *list_x;
  struct termac *list_y;
  int i;
  
  TERMAC_ALLOC((*ptr_list_x),getArity(t),GgetSymb((Gterm *)t));
  TERMAC_ALLOC((*ptr_list_y),getArity(t),GgetSymb((Gterm *)t));
  list_x=(*ptr_list_x);
  list_y=(*ptr_list_y);

  for(i=0 ; i<getArity(t) ; i++) {
    int currentMult = getMult(t,i);
    if(currentMult>=multiplicity) {
	// extract and copy the cell
      termac_add_last(list_x,getSubterm(t,i),currentMult/multiplicity);
	// update the context
      if(currentMult%multiplicity!=0) {
        termac_add_last(list_y,getSubterm(t,i),currentMult%multiplicity);
      }
    } else {
	// copy the context
      termac_add_last(list_y,getSubterm(t,i),currentMult);
    }
  }
    //printf("xy noshared %d\n",xy_noshared++);
}

// TODO: color
void extract_minimal_identical_element(struct termac *t, 
				       multiplicityType multiplicity,
				       struct termac **ptr_list_x,
				       struct termac **ptr_list_y) {
  struct termac *list_x;
  struct termac *list_y;
  int found=0;
  int i;
  
  TERMAC_ALLOC((*ptr_list_x),getArity(t),GgetSymb((Gterm *)t));
  TERMAC_ALLOC((*ptr_list_y),getArity(t),GgetSymb((Gterm *)t));
  list_x=(*ptr_list_x);
  list_y=(*ptr_list_y);

  for(i=0 ; i<getArity(t) ; i++) {
    int currentMult = getMult(t,i);
    if(!found && currentMult>=multiplicity) {
      found=1;
	// extract and copy the cell
      termac_add_last(list_x,getSubterm(t,i),1);
	// update the context
      if(currentMult>multiplicity) {
        termac_add_last(list_y,getSubterm(t,i),currentMult-multiplicity);
      }
    } else {
	// copy the context
      termac_add_last(list_y,getSubterm(t,i),currentMult);
    }
  }
    //printf("xy noshared %d\n",xy_noshared++);
}

static int sub_noshared=0;
static int sub_shared=0;

void substitution_build(struct termac *t, match_state *ms,
			int nb_variable, Gterm *substitution[],
			int nb_variable_ac,
			void (*variable_extract)(),
			int base_id_pattern
			) {
  int no_pattern;
  struct termac *list_x;
  int *pattern_list;
  int indice;
  int i;
  

  //printf("begin substitution_build\n");
    //printf("sub_shared   %d\n",sub_shared++);
  TERMAC_ALLOC(list_x,getArity(t),GgetSymb((Gterm *)t));

  for(i=0 ; i<getArity(t) ; i++) {
      /*
       * S'il y a une variable AC (un contexte) 
       * les elements non completement capture sont copie dans le contexte
       */
    if(nb_variable_ac >= 1 && ms->bg_multiplicity[i] != 0) {
      termac_add_lastColor(list_x,getSubterm(t,i),ms->bg_multiplicity[i],getColor(t,i));
    }
  }
   
    //printf("fin de construction de list_x\n");

  indice=0;
    // sauvegarde du contexte dans substitution[0..nb_variable_ac-1]
  for(i=0 ; i<nb_variable_ac ; i++) {
    substitution[i] = (Gterm*)list_x;
    indice++;
  }

    /*
  for(i=0 ; i<ms->nb_rule ; i++) {
    printf("ms->no_rule=%d\t",i);
    pattern_list=MS_get_pattern_list(ms,i);
    printf("id_pattern = %d\n",pattern_list[0]);
  }
  */

  //if(substitution[0]!=NULL)
  //printf("substitution[0] = "); term_printnl(stdout,substitution[0]);

  //printf("ms->no_rule=%d\n",ms->no_rule);
  pattern_list=MS_get_pattern_list(ms,ms->no_rule);
    //printf("ac_tools: pattern_list=%d\n",pattern_list);

  for(no_pattern=0 ; no_pattern < BG_size(ms->bg) ; no_pattern++) {
    int no_arg_subject;
    int id_pattern;
      
    no_arg_subject=ms->bg_solution[no_pattern];
    id_pattern=pattern_list[no_pattern];

      //printf("no_arg_subject = %d\n",no_arg_subject);
      //printf("start variable_extract on: "); term_printnl(stdout,subject[no_arg_subject]);
    variable_extract(getSubterm(t,no_arg_subject),
		     id_pattern,
		     substitution,&indice,
                       // Pour le 2eme niveau :
		     ms,
		     no_arg_subject,
		     no_pattern,
		     base_id_pattern
		     );
      //printf("end variable_extract\n");
  }
    /*
     * s'il n'y a pas de variable AC : nb_variable_ac==0
     * il ne doit pas y avoir de contexte : 
     */
  if(nb_variable_ac==0) {
    for(i=0 ; i<getArity(t) ; i++) {
      if(ms->bg_multiplicity[i] != 0) {
          //printf("not an empty context !!!\n");
	fail();
      }
    }
  }

}

/*
 * Cette variante ne recupere pas l'instance des variables sous AC
 */
void substitution_build_without_context(struct termac *t,
                                        match_state *ms,
					int nb_variable,
                                        Gterm *substitution[],
					int nb_variable_ac,
					void (*variable_extract)(),
					int base_id_pattern
					) {
  int i,no_pattern;
  int *pattern_list;
  int indice;

  indice=nb_variable_ac;
    /*
  for(i=0 ; i<ms->nb_rule ; i++) {
    printf("ms->no_rule=%d\t",i);
    pattern_list=MS_get_pattern_list(ms,i);
    printf("id_pattern = %d\n",pattern_list[0]);
  }
  */

  //printf("ms->no_rule=%d\n",ms->no_rule);
  pattern_list=MS_get_pattern_list(ms,ms->no_rule);

  for(no_pattern=0 ; no_pattern < BG_size(ms->bg) ; no_pattern++) {
    int no_arg_subject;
    int id_pattern;
      
    no_arg_subject=ms->bg_solution[no_pattern];
    id_pattern=pattern_list[no_pattern];

      //printf("id_pattern = %d\n",id_pattern);

      //printf("i=%d\tno_arg_subject=%d\n",i,no_arg_subject);
      //    printf("term = "); term_printnl(stdout,getSubterm(t,i));

      //printf("start variable_extract\n");
    variable_extract(getSubterm(t,no_arg_subject),
		     id_pattern,
		     substitution,&indice,
                       // Pour le 2eme niveau :
		     ms,
		     no_arg_subject,
		     no_pattern,
		     base_id_pattern
		     );
      //printf("end variable_extract\n");
  }
    /*
     * s'il n'y a pas de variable AC : nb_variable_ac==0
     * il ne doit pas y avoir de contexte : 
     */
  if(nb_variable_ac==0) {
    for(i=0 ; i<getArity(t) ; i++) {
      if(ms->bg_multiplicity[i] != 0) {
          /*
           * [pem: Oct 17 00]
           * Il y a surement un probleme ici lorsque cette fonction est
           * appelee par une regle deterministe sans choicePoint
           */
          //printf("not an empty context !!!\n");
	fail();
      }
    }
  }
}

struct termac *rest_extract(struct termac *t, match_state *ms) {
  int i;
  struct termac *list_x;

    /*
     * si le terme est partage
     */
  TERMAC_ALLOC(list_x,getArity(t),GgetSymb((Gterm *)t));

  for(i=0 ; i<getArity(t) ; i++) {
    if(ms->bg_multiplicity[i] != 0) {
      termac_add_lastColor(list_x,getSubterm(t,i),ms->bg_multiplicity[i],getColor(t,i));
    }
  }
  return list_x;
}

static int xy_noshared=0;
static int xy_shared=0;

void extract_xy_from_pe(struct termac *t,
                        multiplicityType E[],
                        multiplicityType sol[],
                        multiplicityType multiplicity,
			struct termac **ptr_list_x,
                        struct termac **ptr_list_y) {
  struct termac *list_x;
  struct termac *list_y;
  int i;

    /*
     * faire attention au choicePoint
     * en restaurant la pile, list_x est tjs a NULL
     */
  
    //printf("t = "); term_printnl(stdout,(Gterm*)t);
  
  if((*ptr_list_x) == NULL) {
      //printf("Alloc list_x [ptr_list_x=%d]\n",ptr_list_x);
    TERMAC_ALLOC((*ptr_list_x),getArity(t),GgetSymb((Gterm *)t));
  } else {
      //printf("size_list_x=%d arity_t=%d\n",getSize(*ptr_list_x),getArity(t));
    if(getArity(t) > getSize(*ptr_list_x)) {
        //printf("Resize list_x\n");
      termac_resize(*ptr_list_x,getArity(t));
    }
    
  }
  if(*ptr_list_y == NULL) {
    TERMAC_ALLOC((*ptr_list_y),getArity(t),GgetSymb((Gterm *)t));
  } else {
      //printf("size_list_y=%d arity_t=%d\n",getSize(*ptr_list_y),getArity(t));
    if(getArity(t) > getSize(*ptr_list_y)) {
        //printf("Resize list_y\n");
      termac_resize(*ptr_list_y,getArity(t));
    }
  }
  
  list_x=(*ptr_list_x);
  list_y=(*ptr_list_y);
  
    //assert(t!=list_x);
    //assert(t!=list_y);
        
  setArity(list_x,0);
  setArity(list_y,0);
  
  for(i=0 ; i<getArity(t) ; i++) {
    if(sol[i] == 0) {
        //termac_add_lastColor(list_y,getSubterm(t,i),E[i],getColor(t,i));
        // [pem: Oct 24 00]
      termac_add_lastColor(list_y,getSubterm(t,i),getMult(t,i),getColor(t,i));
    } else {
	// copier la cellule
      termac_add_lastColor(list_x,getSubterm(t,i),sol[i]/multiplicity,getColor(t,i));
        // copier le reste
        //if(E[i] > sol[i]) {
        //termac_add_lastColor(list_y,getSubterm(t,i),E[i]-sol[i],getColor(t,i));
          // [pem: Oct 24 00]
      if(getMult(t,i) > sol[i]) {
        termac_add_lastColor(list_y,getSubterm(t,i),getMult(t,i)-sol[i],getColor(t,i));
      }
    }
  }
    //printf("xy noshared %d\n",xy_noshared++);
}

/*
 * E[] contient les multiplicites des cellules
 * sol[] est initialise a 0
 * taille est initialise avec la taille de E[]-1
 * la solution sol[]=0 n'est pas retournee
 * det : pour savoir s'il faut poser un point de choix
 */

int next_minimal_extract(int total,
                         multiplicityType E[],
                         multiplicityType sol[],
                         multiplicityType multiplicity) {
  int pos = total-1;
    /*
  int i;
  printf("E=(%d)\tsol=(%d)\n",E,sol);
  printf("E   = "); for(i=0 ; i<total ; i++) printf("%d ",E[i]);
  printf("\nsol = "); for(i=0 ; i<total ; i++) printf("%d ",sol[i]);
  printf("\n");
    */
  while(sol[pos]==E[pos] && pos>=0) {
    sol[pos]=0;
    pos--;
  }
  if(pos<0) {
    return 0;
  }
  sol[pos]+=multiplicity;
  return 1;
}

int next_maximal_extract(int total,
                         multiplicityType E[],
                         multiplicityType sol[],
                         multiplicityType multiplicity) {
  int pos = total-1;
  for(pos = total-1 ; pos>=0 && sol[pos]==0 ; pos--) {
    sol[total] += (sol[pos]=E[pos]);
  }

  if(pos>=0) {
    sol[pos]   -= multiplicity;
    sol[total] -= multiplicity;
  }

  if(sol[total]>0) {
    return 1;
  } else {
    return 0;
  }
}

#define P2
int match_subterm_AC(int base_id_pattern,
		     int no_arg_subject,
		     int *mask,
		     BG *cbg,
		     int (*my_match_subterm)(),
		     int nb_rule,
		     int **pattern_list,
		     int nb_pattern_in_cbg,
		     Gterm *t,
		     int max_nb_pattern_under_AC) {
    /*
   * ici on construit le CBG principal
   */
  //int base_id_pattern=2;
  int id_pattern,no_rule;
  int nb_bit=0;
  match_state *ms;
  match_state *msbg;
  LINK *link;
  int necessary_link;
  int indice;

    //int pattern_tab[max_nb_pattern_under_AC];
  int *pattern_tab = (int*) AMALLOC(max_nb_pattern_under_AC*sizeof(int));
  
  
    //  printf("match_subterm_AC:\n\tt = "); term_printnl(stdout,t);

  //ms=MS_create();
  /*
   * initialisation du match_state
   * 1 regle : p1(set(x),y))
   * 1 pattern_in_cbg : set(x)
   */
  necessary_link=0; // 1 niveau
  indice = MS_init(&ms, my_match_subterm, nb_rule, pattern_list,
                   nb_pattern_in_cbg,(struct termac*)t,
		   necessary_link, max_nb_pattern_under_AC);
  
  if(indice==-1) {
      /*
       * C'est que le MS est vide (pas de sous terme)
       */
    id_pattern=base_id_pattern+0;
    mask[nb_bit++]=id_pattern;

    //printf("id_pattern=%d\n",id_pattern);
    //printf("c'est fini\n");
    return nb_bit;
      //printf("internal error in match_subterm_AC\n");
      //exit(1);
  }

    /*
   * Pour chaque BG du match_state qui a au moins une solution
   * on le memorise (sous la forme d'un match_state) dans le link
   * et on met a jour le mask
   */
#ifdef P2
  printf("  Creation du link\n");
#endif
    //	MS_print(ms); 
    //	printf("\n");
  
  for(no_rule=0 ; no_rule < ms->nb_rule ; no_rule++) {
    int nb_pattern;
    int nb_subterm_subject;
    int indice,i;
      
    int no_element;
    int *ms_pattern_tab;
    int **pattern_list2;
#ifdef P2
    printf("  BG no %d\n",no_rule);
#endif
    nb_pattern=MS_pattern_list_size(ms,no_rule);
    nb_subterm_subject=bitSet_size(BG_get(ms->cbg,0));
      
    msbg=MS_create();
      
      // --------------------------------------------------
      /* Creation d'une pattern list pour LA regle */
    pattern_list2=MS_pattern_list_create(1);
      // Copie de pattern_tab
    ms_pattern_tab=MS_get_pattern_list(ms,no_rule);
    for(no_element=0 ; no_element < MS_pattern_list_size(ms,no_rule) ; no_element++) {
      pattern_tab[no_element] = ms_pattern_tab[no_element];
    }
    MS_pattern_list_init(pattern_list2,0,no_element,pattern_tab);
    msbg->pattern_list=pattern_list2;
    msbg->nb_rule=1;
    msbg->no_rule=0;
    msbg->cbg=NULL;
      // --------------------------------------------------
    
      /* Creation de bg */
    msbg->bg=BG_create(nb_pattern,0); // sans LINK
      /* Creation de bg_multiplicity */
    msbg->bg_multiplicity=(multiplicityType*)AMALLOC(nb_subterm_subject*sizeof(multiplicityType));
      /* Creation de bg_solution */
    msbg->bg_solution=(int*)AMALLOC(BG_size(msbg->bg)*sizeof(int));
    
    BG_cbg2bg(MS_get_pattern_list(ms,no_rule),ms->cbg,msbg->bg);
    
      /* initialisation de bg_solution */
    for(i=0 ; i<BG_size(msbg->bg) ; i++) {
      msbg->bg_solution[i]=-1;
    }
      /* initialisation de bg_multiplicity */
    for(i=0 ; i<nb_subterm_subject ; i++) {
      msbg->bg_multiplicity[i]=ms->bg_multiplicity[i];
    }
      
      /*
        printf("BG=");
        BG_print(msbg->bg);
        printf("\n");
      */
    
      /* y-a-t-il une solution ? */
    indice=BG_solve_one(msbg->bg,msbg->bg_solution,msbg->bg_multiplicity,
			bitSet_size(BG_get(msbg->bg,0)),0);  
#ifdef P2
    printf("y-a-t-il une solution ?, indice=%d\n",indice);
#endif
      
    if(indice>=0) {
        //printf("Re-initialisation du msbg\n");
        //for(i=0 ; i<BG_size(msbg->bg) ; i++)
        //  msbg->bg_solution[i]=-1;
      
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!
        // 2 : no du subpattern commencant par p1
        /*
         * il faut etablir un correspondance entre les numeros
         * des subpatterns de p2p1 et les subpattern de p2
         * ici, set(x) : bit 0, de p2p1, correspond
         * au subpattern p1(set(x),y) : bit 2, de p2
         */
        /*
         * les patterns commencant par p1(...) sont numerotes:
         * - base_id_pattern+0
         * - base_id_pattern+1
         * - base_id_pattern+2
         * - ...
         */
      id_pattern=base_id_pattern+no_rule;
#ifdef P2
      printf("base_id_pattern=%d\tno_rule=%d\n",base_id_pattern,no_rule);
      printf("id_pattern=%d\n",id_pattern);
#endif
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!
      mask[nb_bit++]=id_pattern;
        // Ancienne version
        //bitSet32_set(mask,id_pattern);
        //nb_bit++;
        /*
         * si le link existe deja on le complete sinon on le cree
         */
      link=BG_link_get(cbg,id_pattern);
      if(link==NULL) {
	printf("le link n'existe pas encore\n");
	link=LINK_create(  bitSet_size(BG_get(cbg,id_pattern)) );
	BG_link_set(cbg,id_pattern,link);
      }
      LINK_set(link,no_arg_subject,msbg);
    } else {
        //printf("Pas de solution : il faut detruire le msbg\n"); 
        /* il faut detruire le msbg */
      MS_pattern_list_free(msbg->pattern_list,msbg->nb_rule);
      MS_delete(msbg);
    }
  }
    // IL FAUT DETRUIRE : ms
  MS_delete(ms);
  return nb_bit;
}

