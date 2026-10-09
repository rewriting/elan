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
#include "match_state.h"
#include "tools.h"
#include "termCommon.h"
#include "link.h"


// A MODIFIER
//#include "dart.h"

int allocatedBug=0;
//int *STRANGE_ADDRESS = (int*)178981320;

match_state *MS_create() {
  match_state *ms;
  ms=(match_state*) IMALLOC(sizeof(match_state));

    /*
  if(ms==(match_state*)STRANGE_ADDRESS) {
    printf("allocatedBug (%d)\n",STRANGE_ADDRESS);
    allocatedBug=1;
  }
    */
  
#ifdef DEBUG
  ms->cbg=0;
  ms->nb_rule=0;
  ms->pattern_list=0;
  ms->bg=0;
  ms->no_rule=0;
  ms->bg_solution=0;
  ms->bg_multiplicity=0;
#endif
  return ms;
}

void MS_delete(match_state *ms) {
  int i;
  Verif_void(ms,"MS_delete(ms)");
  //Verif_void(ms->cbg,"MS_delete(ms->cbg)");
  Verif_void(ms->pattern_list,"MS_delete(ms->pattern_list)");
  Verif_void(ms->bg,"MS_delete(ms->bg)");
  Verif_void(ms->bg_solution,"MS_delete(ms->bg_solution)");
  Verif_void(ms->bg_multiplicity,"MS_delete(ms->bg_multiplicity)");
  if(ms->cbg != NULL)
    BG_delete(ms->cbg);
  // avec BGSHARE il faut peut etre detruire les bitSet du bg

    {
    //MS_pattern_list_free(ms->pattern_list,ms->nb_rule);
    if(ms->bg->link_tab != NULL) {
      for(i=0 ; i<ms->bg->size ; i++) {
	if(ms->bg->link_tab[i]!=NULL)
	  LINK_delete(ms->bg->link_tab[i]);
      }
      IFREE(ms->bg->link_tab);
    }
    IFREE(ms->bg->bs_tab);
    IFREE(ms->bg);
    }
  IFREE(ms->bg_solution);
  IFREE(ms->bg_multiplicity);
  IFREE(ms);
}

// [pem: Oct 20 00]
//static int match_mask[MAX_CBG_SIZE];
int MS_init(match_state **ptr_ms,
	     int (*match_subterm)(Gterm *v0, int no_arg_subject, int *mask, BG *cbg),
	     int nb_rule,
	     int **pattern_list,
	     int nb_pattern_in_cbg,
	     struct termac *subject,
	     int necessary_link,
	     int max_nb_pattern_under_AC)
{
  match_state *ms=*ptr_ms=NULL;
  BG *cbg=NULL;
  multiplicityType *bg_multiplicity;

  int nb_subterm_subject=0;
  int no_pattern,no_arg_subject;
  bitSet *mask;
  int no_bit,nb_bit; // pour optimiser l'initialisation de CBG
  
  /*
  // Ancienne version
  bitSet32 *match_mask;
  bitSet32_create(match_mask,nb_pattern_in_cbg);  
  */
  int match_mask[MAX_CBG_SIZE];

  //bitSet_stack_create(match_mask,MAX_CBG_SIZE);
  
  //Verif_void(ms,"MS_init(ms)");
  
  //bitSet_init_size(match_mask,nb_pattern_in_cbg); 

  /*
   * Calcul du nb de sous-termes du sujet
   */
  nb_subterm_subject=getArity(subject);
  /* Creation de bg_multiplicity */
  bg_multiplicity=(multiplicityType*)IMALLOC(nb_subterm_subject*sizeof(multiplicityType));

  /*
   * creation du cbg vide (pas encore de lien)
   */ 
  if(necessary_link) {
    *ptr_ms=ms=MS_create();
    ms->nb_rule=nb_rule;
    ms->pattern_list=pattern_list;
    ms->no_rule=0;

    ms->cbg=cbg=BG_create(nb_pattern_in_cbg,necessary_link); // avec LINK ?
    ms->bg_multiplicity=bg_multiplicity;
    for(no_pattern=0 ; no_pattern < nb_pattern_in_cbg ; no_pattern++) {
      bitSet_create(mask,nb_subterm_subject);
      bitSet_init_clear(mask);
      BG_set(ms->cbg,no_pattern,mask);
    }
  } else {
    ms=NULL;
  }


#ifdef DEBUG
  if(nb_pattern_in_cbg >= (MAX_CBG_SIZE*NBITS)) {
    fprintf(stderr,"Too many pattern in CBG. Increase MAX_CBG_SIZE\n");
    exit(1);
  }
#endif

  //bitSet_init_size(match_mask,nb_pattern_in_cbg);

  for(no_arg_subject=0 ; no_arg_subject<getArity(subject); no_arg_subject++) {
      /*
     * match_mask contient les patterns sous le symbole AC qui filtrent
     * vers le sous-terme v
     */

    nb_bit=match_subterm(getSubterm(subject,no_arg_subject),no_arg_subject,match_mask,cbg);

    /*
      printf("nb_bit = %d\n",nb_bit);
      printf("no_arg_subject=%d\n",no_arg_subject);
      */
    
    /*
     * on construit le bipartie graph en representation compacte (CBG)
     * on associe a chaque pattern les sous-termes v qu'il filtre.
     * match_mask est un tableau d'entiers. Chaque entier est le numero 
     * des patterns du CBG qui filtrent le sujet no_arg_subject
     */

    if(nb_bit != 0) {
      if(ms==NULL) {
	int no_pattern;

          //printf("coucou\n");
                
	*ptr_ms=ms=MS_create();
	ms->nb_rule=nb_rule;
	ms->pattern_list=pattern_list;
	ms->no_rule=0;

          //printf("nb_rule=%d\n",nb_rule);
          //printf("MS_pattern_list_size=%d\n",MS_pattern_list_size(ms,ms->no_rule));
        
	ms->cbg=cbg=BG_create(nb_pattern_in_cbg,necessary_link);
	ms->bg_multiplicity=bg_multiplicity;
	for(no_pattern=0 ; no_pattern < nb_pattern_in_cbg ; no_pattern++) {
	  bitSet_create(mask,nb_subterm_subject);
	  bitSet_init_clear(mask);
	  BG_set(ms->cbg,no_pattern,mask);
	}
      }
      for(no_bit=0 ; no_bit<nb_bit ; no_bit++) {
	bitSet_set(BG_get(ms->cbg,match_mask[no_bit]),no_arg_subject);
      }
    }
    /* initialisation de bg_multiplicity */
//    [pem: Apr 23 99]
//    bg_multiplicity[no_arg_subject]=getMult(cell);
    
  }
  // Ancienne version
  //bitSet32_delete(match_mask);

  /*
   * 1er critere de detection d'echec
   */
  if(ms==NULL) {
      //printf("no solution !!!\n");
    /*
     * Destruction du debut de MS
     */
    IFREE(bg_multiplicity);
    //IFREE(ms);
    return -1;
  }


 /*
  * size(pattern_list[no_rule]) devrait suffire mais cela complique
  * la gestion de la memoire
  */
  //  creation du bg
  ms->bg=BG_create(max_nb_pattern_under_AC,0); // sans LINK
  ms->bg_solution=(int*)IMALLOC(max_nb_pattern_under_AC*sizeof(int));
/* initialisation de bg_solution */
// [pem: Apr 23 99]
//  for(i=0 ; i<BG_size(ms->bg) ; i++) {
//    ms->bg_solution[i]=-1;
//  }
  /* initialisation du bg */
    // [pem: Apr 23 99]
//  BG_set_size(ms->bg,MS_pattern_list_size(ms,ms->no_rule));
//  BG_cbg2bg(MS_get_pattern_list(ms,ms->no_rule),ms->cbg,ms->bg);
  return 1;
}

int MS_reinit(match_state *ms,struct termac *subject, int no_rule) {
  int i;
  int no_arg_subject;

  if(ms==NULL) {
      //printf("ms==NULL\tsubject = "); term_printnl(stdout,subject);
    return -1;
  }
  
  for(no_arg_subject=0 ; no_arg_subject<getArity(subject) ; no_arg_subject++) {
    /* initialisation de bg_multiplicity */
    ms->bg_multiplicity[no_arg_subject]=getMult(subject,no_arg_subject);
  }
  /* initialisation du bg */
  ms->no_rule=no_rule;
  BG_set_size(ms->bg,MS_pattern_list_size(ms,ms->no_rule));

    //printf("no_rule=%d\n",no_rule);
    //printf("MS_pattern_list_size=%d\n",MS_pattern_list_size(ms,ms->no_rule));
    
  if(!BG_cbg2bg(MS_get_pattern_list(ms,ms->no_rule),ms->cbg,ms->bg)) {
      /* there is no solution */
      //printf("there is no solution\n");
    return -1;
  }
  
  /* initialisation de bg_solution */
  for(i=0 ; i<MS_pattern_list_size(ms,ms->no_rule) ; i++) {
    ms->bg_solution[i]=-1;
  }
  return 1;
}




int MS_solve(match_state *ms, int mode) {
  (void)mode;
  int indice_solution=-1;
  int i;
  Verif_void(ms,"MS_solve(ms)");

  // comparaison du nb de patterns et du nb de sous-termes du sujet
  /*
  for(i=0 ; i<bitSet_size(BG_get(ms->bg,0)) ; i++)
    total_nb_subterm_subject+=ms->bg_multiplicity[i];
  if( (BG_size(ms->bg) > total_nb_subterm_subject) || 
      (mode==NO_REST   && BG_size(ms->bg) != total_nb_subterm_subject) ||
      (mode==WITH_REST && BG_size(ms->bg) == total_nb_subterm_subject) 
      )
    {
      printf("Pas de solution : taille du sujet\n");
      return -1;
    }
    */

  begin_MS_solve:

  //MS_print(ms);
  if(ms->bg_solution[0] == -1) {
    indice_solution=BG_solve_one(ms->bg,ms->bg_solution,ms->bg_multiplicity,
				 bitSet_size(BG_get(ms->bg,0)),0);
  } else {
    /* BG_size(ms->bg)-1 est la valeur a donner pour continuer */
    indice_solution=BG_solve_one(ms->bg,ms->bg_solution,ms->bg_multiplicity,
				 bitSet_size(BG_get(ms->bg,0)),
				 BG_size(ms->bg)-1);
  }
  
  if(indice_solution >= 0) {
  } else {
    next_bg:
      // On passe au prochain BG non greedy
    ms->no_rule++;
    if(ms->no_rule < ms->nb_rule) {
      BG_set_size(ms->bg,MS_pattern_list_size(ms,ms->no_rule));
      if(!BG_cbg2bg(MS_get_pattern_list(ms,ms->no_rule),ms->cbg,ms->bg)) {
          /* there is no solution */
        goto next_bg;
      }
        /* initialisation de bg_solution */
      for(i=0 ; i<BG_size(ms->bg) ; i++) {
        ms->bg_solution[i]=-1;
      }
        /* on re-cherche une solution */
      goto begin_MS_solve;
    } else {
        //indice_solution=-1;
    }
  }
  return indice_solution;
}

int MS_solve_rule(match_state *ms) {
  Verif_void(ms,"MS_solve_rule(ms)");

  if(ms->bg_solution[0] == -1) {
    return  BG_solve_one(ms->bg,ms->bg_solution,ms->bg_multiplicity,
			 bitSet_size(BG_get(ms->bg,0)),0);
  } else {
    /* BG_size(ms->bg)-1 est la valeur a donner pour continuer */
    return BG_solve_one(ms->bg,ms->bg_solution,ms->bg_multiplicity,
			bitSet_size(BG_get(ms->bg,0)), BG_size(ms->bg)-1);
  }
}


/*
bitSet *MS_get_possible_rule(match_state *ms)
{
  for(no_rule=ms->no_rule ; no_rule < ms->nb_rule ; no_rule++)
    {
      pattern_list_size=MS_pattern_list_size(match_state *ms, int no_rule);
      pattern_list=MS_get_pattern_list(ms,ms->no_rule);

      for(i=0 ; i < pattern_list_size ; i++)
	{
	}
	}
}
*/


void MS_print(match_state *ms)
{
  int i,j;
  Verif_void(ms,"MS_print(ms)");
  printf("------------------------------------------------------------\n");
  printf("nb_rule=%d\n",ms->nb_rule);
  if(ms->cbg)
    {
      printf("CBG = ");
      BG_print(ms->cbg); 
      printf("\n");
    }

  printf("pattern_list = ");
  for(i=0 ; i<ms->nb_rule ; i++)
    {
      printf("[");
      for(j=0 ; j<MS_pattern_list_size(ms,i) ; j++)
	printf(" %d .",MS_get_pattern_list(ms,i)[j]);
      printf("] . ");
    }
  printf("\n");

  printf("BG = ");
  BG_print(ms->bg);
  printf("\n");

  printf("no_rule=%d\n",ms->no_rule);

  printf("bg_solution = [");
  for(i=0 ; i<BG_size(ms->bg) ; i++)
    printf("%d ",ms->bg_solution[i]);
  printf("]\n");

  printf("bg_multiplicity = [");
  for(i=0 ; i<bitSet_size(BG_get(ms->cbg,0)) ; i++)
    printf("%d  ",ms->bg_multiplicity[i]);
  printf("]\n");

  printf("------------------------------------------------------------\n");
}

int MS_pattern_list_size(match_state *ms, int no_rule)
{
  Verif_void(ms,"MS_pattern_list_size(ms)");
  Verif_void(ms->pattern_list,"MS_get_pattern_list_size(ms->pattern_list)");
  return (ms->pattern_list[no_rule])[0];
}

int *MS_get_pattern_list(match_state *ms, int no_rule)
{
  Verif_void(ms,"MS_get_pattern_list(ms)");
  Verif_void(ms->pattern_list,"MS_get_pattern_list(ms->pattern_list)");
  /* +1 pour sauter la taille */
  return (ms->pattern_list[no_rule])+1;
}

int **MS_pattern_list_create(int nb_pattern)
{
  int **res;
  int i;
  res=(int**)IMALLOC(nb_pattern*sizeof(int*));
  for(i=0 ; i<nb_pattern ; i++)
    res[i]=(int*)0;
  return res;
}

void MS_pattern_list_free(int **pattern_list,int nb_pattern)
{
  int i;
  Verif_void(pattern_list,"MS_pattern_list_free(pattern_list)");
  for(i=0 ; i<nb_pattern ; i++)
    {
      if(pattern_list[i]) {
	IFREE(pattern_list[i]);
      }
    }
  IFREE(pattern_list);
}

void MS_pattern_list_init(int **pattern_list,
			  int number,
			  int nb_element,
			  int *pattern_tab)
{
  int i;
  Verif_void(pattern_list,"MS_pattern_list_init(pattern_list)");
  Verif_void(pattern_tab,"MS_pattern_list_init(pattern_tab)");
  pattern_list[number]=(int*)IMALLOC((1+nb_element)*sizeof(int));
  (pattern_list[number])[0]=nb_element;
  for(i=0 ; i<nb_element ; i++)
    (pattern_list[number])[i+1]=pattern_tab[i];
}





