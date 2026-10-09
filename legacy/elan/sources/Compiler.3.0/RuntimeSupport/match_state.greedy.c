#include "match_state.h"
#include "tools.h"
#include "term.h"
#include "link.h"
// A MODIFIER
#include "dart.h"

match_state *MS_create()
{
  match_state *ms;
  ms=(match_state*) IMALLOC(sizeof(match_state));
#ifdef DEBUG
  ms->cbg=0;
  ms->nb_rule=0;
  ms->pattern_list=0;
  ms->bg=0;
  ms->no_rule=0;
  ms->bg_solution=0;
#endif
  return ms;
}

void MS_delete(match_state *ms)
{
  int i;
  Verif_void(ms,"MS_delete(ms)");
  //Verif_void(ms->cbg,"MS_delete(ms->cbg)");
  Verif_void(ms->pattern_list,"MS_delete(ms->pattern_list)");
  Verif_void(ms->bg,"MS_delete(ms->bg)");
  Verif_void(ms->bg_solution,"MS_delete(ms->bg_solution)");
  Verif_void(ms->bg_multiplicity,"MS_delete(ms->bg_multiplicity)");
  if(ms->cbg != NULL)
    BG_delete(ms->cbg);
  // avec TEST il faut peut etre detruire les bitSet du bg


  //MS_pattern_list_free(ms->pattern_list,ms->nb_rule);
#ifdef TEST
  if(ms->bg->link_tab != NULL)
    {
      for(i=0 ; i<ms->bg->size ; i++)
	{
	  if(ms->bg->link_tab[i]!=NULL)
	    LINK_delete(ms->bg->link_tab[i]);
	}
      IFREE(ms->bg->link_tab);
    }
  IFREE(ms->bg->bs_tab);
  IFREE(ms->bg);
#else
  BG_delete(ms->bg);
#endif
  IFREE(ms->bg_solution);
  IFREE(ms->bg_multiplicity);
  IFREE(ms);
}

void MS_init(match_state *ms,
	     int (*match_subterm)(),
	     int nb_rule,
	     int **pattern_list,
	     int nb_pattern_in_cbg,
	     struct term *subject,
	     int necessary_link,
	     int max_nb_pattern_under_AC)
{
  int nb_subterm_subject=0;

  int i,no_pattern,no_arg_subject;
  int no_rule;
  bitSet *mask;
  struct cell_term *cell;
  //bitSet_type match_mask[1+(MAX_CBG_SIZE/NBITS)];
  bitSet *match_mask;
  int no_bit,nb_bit; // pour optimiser l'initialisation de CBG


  Verif_void(ms,"MS_init(ms)");
  
  //bitSet_init_size(match_mask,nb_pattern_in_cbg); 

  ms->nb_rule=nb_rule;
  ms->pattern_list=pattern_list;
  ms->no_rule=0;

  /*
   * Calcul du nb de sous-termes du sujet
   */
  for(cell=term_first(subject) ; cell!=NULL ; cell=cell_next(cell))
    nb_subterm_subject++;

  /*
   * creation du cbg vide (pas encore de lien)
   */ 
  ms->cbg=BG_create(nb_pattern_in_cbg,necessary_link); // avec LINK ?
  for(no_pattern=0 ; no_pattern < nb_pattern_in_cbg ; no_pattern++)
    {
      bitSet_create(mask,nb_subterm_subject);
      bitSet_init_clear(mask);
      BG_set(ms->cbg,no_pattern,mask);
    }
  
  /* Creation de bg_multiplicity */
  ms->bg_multiplicity=(int*)IMALLOC(nb_subterm_subject*sizeof(int));

#ifdef DEBUG
  if(nb_pattern_in_cbg >= (MAX_CBG_SIZE*NBITS))
    {
      fprintf(stderr,"Too many pattern in CBG. Increase MAX_CBG_SIZE\n");
      exit(1);
    }
#endif
  bitSet_create(match_mask,nb_pattern_in_cbg);
  bitSet_init_size(match_mask,nb_pattern_in_cbg);
  /*
   * IL FAUDRAIT ESSAYER DE CONSTRUIRE UNE SOLUTION POUR FAIRE
   * DU GREEDY MATCHING
   */
  for(cell=term_first(subject),no_arg_subject=0 ;
      cell!=NULL ;
      cell=cell_next(cell),no_arg_subject++)
    {
      /*
       * match_mask contient les patterns sous le symbole AC qui filtrent
       * vers le sous-terme v
       */
      nb_bit=match_subterm(cell_t(cell),no_arg_subject,match_mask,ms->cbg);

      /*
      printf("no_arg_subject=%d\n",no_arg_subject);
      */

      /*
       * on construit le bipartie graph en representation compacte (CBG)
       * on associe a chaque pattern les sous-termes v qui qu'il filtre
       */
      //if(nb_bit != 0) // juste pour accelere un peu
	{
	  for(no_pattern=0, no_bit=0 ;
	      //      (no_bit<nb_bit) && // juste pour accelere un peu
	      (no_pattern<nb_pattern_in_cbg) ;
	      no_pattern++)
	    {
	      if( bitSet_get(match_mask,no_pattern) )
		{
		  bitSet_set(BG_get(ms->cbg,no_pattern),no_arg_subject);
		  no_bit++;
		}
	    }
	}



      /* initialisation de bg_multiplicity */
      ms->bg_multiplicity[no_arg_subject]=cell_mult(cell);
    }
  bitSet_delete(match_mask);

 /*
  * size(pattern_list[no_rule]) devrait suffire mais cela complique
  * la gestion de la memoire
  */
  //  creation du bg
  ms->bg=BG_create(max_nb_pattern_under_AC,0); // sans LINK
  ms->bg_solution=(int*)IMALLOC(max_nb_pattern_under_AC*sizeof(int));
  /* initialisation de bg_solution */
  for(i=0 ; i<BG_size(ms->bg) ; i++) {
    ms->bg_solution[i]=-1;
  }
  /* initialisation du bg */
  BG_set_size(ms->bg,MS_pattern_list_size(ms,ms->no_rule));
  BG_cbg2bg(MS_get_pattern_list(ms,ms->no_rule),ms->cbg,ms->bg);
}

int MS_solve(match_state *ms, int mode)
{
  int indice_solution=-1;
  int i;
  int total_nb_subterm_subject=0;
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
  if(ms->bg_solution[0] == -1)
    indice_solution=BG_solve_one(ms->bg,ms->bg_solution,ms->bg_multiplicity,
				 bitSet_size(BG_get(ms->bg,0)),0);
  else
    /* BG_size(ms->bg)-1 est la valeur a donner pour continuer */
    indice_solution=BG_solve_one(ms->bg,ms->bg_solution,ms->bg_multiplicity,
				 bitSet_size(BG_get(ms->bg,0)),
				 BG_size(ms->bg)-1);
  
#ifdef AFFICHAGE    
  //printf("BG_solve_one=%d\n",indice_solution);
#endif
  if(indice_solution >= 0)
    {
#ifdef AFFICHAGE
      printf("\t*** Solution : ");
      for(i=0 ; i<BG_size(ms->bg) ; i++)
      printf("%d ",ms->bg_solution[i]);
      printf("\n");
#endif
    }
  else
    {
#ifdef AFFICHAGE
      //printf("\ton passe au pattern suivant\n");
#endif
      // On passe au prochain BG non greedy
      ms->no_rule++;
      if(ms->no_rule < ms->nb_rule)
	{
#ifndef TEST
	  /* destruction de l'ancien bg */
	  for(i=0 ; i<BG_size(ms->bg) ; i++)
	    bitSet_delete( BG_get(ms->bg,i) );
#endif
	  BG_set_size(ms->bg,MS_pattern_list_size(ms,ms->no_rule));
	  BG_cbg2bg(MS_get_pattern_list(ms,ms->no_rule),ms->cbg,ms->bg);
	  /* initialisation de bg_solution */
	  for(i=0 ; i<BG_size(ms->bg) ; i++)
	    ms->bg_solution[i]=-1;	  
	  /* on re-cherche une solution */
	  goto begin_MS_solve;
	}
      else
	{
	  //indice_solution=-1;
#ifdef AFFICHAGE
	  //printf("\tPlus de solution\n");
#endif
	}
    }
#ifdef AFFICHAGE
  //printf("fin du solve : indice_solution=%d\n",indice_solution);
#endif
  return indice_solution;
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
      if(pattern_list[i])
	IFREE(pattern_list[i]);
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

/*
 * Utilise le sujet pour construire le reste
 */
struct term *MS_give_rest(match_state *ms, struct term *t)
{
  bitSet *used;
  struct cell_term *cell, *tmp;
  int i;

  bitSet_create(used,bitSet_size(BG_get(ms->bg,0)));
  bitSet_init_clear(used);
  for(i=0 ; i<BG_size(ms->bg) ; i++)
    bitSet_set(used,ms->bg_solution[i]);
 
  for(cell=term_first(t), i=0 ; cell != NULL ; tmp=cell, cell=cell_next(cell),i++)
    {
      if(bitSet_get(used,i))
	{
	  cell_next(tmp)=cell_next(cell);
	  cell_next(cell)=NULL;
	  //cell_term_free(cell);
	}
    }
}


int MS_extended_solve(match_state *ms, int mode)
{
  int indice_solution;
  int i;
  int total_nb_subterm_subject=0;
  int *pattern_list;

  Verif_void(ms,"MS_extended_solve(ms)");

  // comparaison du nb de patterns et du nb de sous-termes du sujet
  /*
  for(i=0 ; i<bitSet_size(BG_get(ms->bg,0)) ; i++)
    total_nb_subterm_subject+=ms->bg_multiplicity[i];
#ifdef AFFICHAGE
  printf("nb pattern = %d\n",BG_size(ms->bg));
  printf("nb subterm = %d\n",total_nb_subterm_subject);
#endif
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


  /*
   * solution suivante des sous-problemes
   */

  if(ms->bg_solution[0] != -1) // il y a une solution en cours
    {
      int i;
      // on cherche dans le cbg, le premier msbg 
#ifdef AFFICHAGE
      printf("  Recherche de premier msbg\n");
#endif

      pattern_list=MS_get_pattern_list(ms,ms->no_rule);
      for( i=0 ; i < BG_size(ms->bg) ; i++)
	{
	  LINK *link;
	  match_state *msbg;
	  int indice;
	  int id_pattern, no_arg_subject;
#ifdef AFFICHAGE
	  printf("  i=%d\n",i);
#endif
	  id_pattern=pattern_list[i];
	  no_arg_subject=ms->bg_solution[i];
	  if( (link=BG_link_get(ms->cbg,id_pattern)) != NULL )
	    if( (msbg=LINK_get(link,no_arg_subject)) != NULL )
	      {
#ifdef AFFICHAGE
		printf("  on en a un ! reste-t-il une solution ?\n");
#endif
		indice=MS_solve(msbg,POSSIBLE_REST);
		if(indice>=0)
		  {
#ifdef AFFICHAGE
		    printf("OK on a une solution\n");
#endif
		    return indice;
		  }
		else
		  {
		    /*
		     * il faut re-initialiser le msbg
		     * et cherche un cran plus loin
		     */
#ifdef AFFICHAGE
		    printf("Re-initialisation du msbg\n");
#endif
		    for(i=0 ; i<BG_size(msbg->bg) ; i++)
		      msbg->bg_solution[i]=-1;
		    BG_solve_one(msbg->bg,msbg->bg_solution,
				 msbg->bg_multiplicity,
				 bitSet_size(BG_get(msbg->bg,0)),0);   
		  }
	      }
	}
      /*
       * if( i >= BG_size(ms->bg) )
       * il faut la solution suivante du BG
       */
    }
  



  if(ms->bg_solution[0] == -1)
    indice_solution=BG_solve_one(ms->bg,ms->bg_solution,ms->bg_multiplicity,
				 bitSet_size(BG_get(ms->bg,0)),0);
  else
    /* BG_size(ms->bg)-1 est la valeur a donner pour continuer */
    indice_solution=BG_solve_one(ms->bg,ms->bg_solution,ms->bg_multiplicity,
				 bitSet_size(BG_get(ms->bg,0)),
				 BG_size(ms->bg)-1);
#ifdef AFFICHAGE    
  printf("BG_solve_one=%d\n",indice_solution);
#endif
  if(indice_solution >= 0)
    {
#ifdef AFFICHAGE
      printf("\t*** Solution : ");
      for(i=0 ; i<BG_size(ms->bg) ; i++)
	printf("%d ",ms->bg_solution[i]);
      printf("\n");
#endif
    }
  else
    {
#ifdef AFFICHAGE
      printf("\ton passe au pattern suivant\n");
#endif
      ms->no_rule++;
      if(ms->no_rule < ms->nb_rule)
	{
#ifndef TEST
	  /* destruction de l'ancien bg */
	  for(i=0 ; i<BG_size(ms->bg) ; i++)
	    bitSet_delete( BG_get(ms->bg,i) );
#endif
	  BG_set_size(ms->bg,MS_pattern_list_size(ms,ms->no_rule));
	  BG_cbg2bg(MS_get_pattern_list(ms,ms->no_rule),ms->cbg,ms->bg);
	  /* initialisation de bg_solution */
	  for(i=0 ; i<BG_size(ms->bg) ; i++)
	    ms->bg_solution[i]=-1;	  
	  /* on re-cherche une solution */
	  goto begin_MS_solve;
	}
      else
	{
#ifdef AFFICHAGE
	  printf("\tPlus de solution\n");
#endif
	}
    }
#ifdef AFFICHAGE
  printf("fin du solve : indice_solution=%d\n",indice_solution);
#endif
  return indice_solution;
}



/*
 * Greedy matching
 */

int MS_greedy_init(match_state *ms,
		   int (*match_subterm)(),
		   int nb_rule,
		   int **pattern_list,
		   int nb_pattern_in_cbg,
		   struct term *subject,
		   int necessary_link,
		   int max_nb_pattern_under_AC,
		   int **greedy_rule_tab,
		   int *isGreedyRule)
{
  int nb_subterm_subject=0;

  int i,no_pattern,no_arg_subject;
  int no_rule;
  bitSet *mask;
  struct cell_term *cell;
  bitSet *match_mask;

  BG** greedy_bg;
  int* greedy_solution;
  int greedy_multiplicity[MAX_TERM_SIZE];

  Verif_void(ms,"MS_init(ms)");
  
  ms->nb_rule=nb_rule;
  ms->pattern_list=pattern_list;
  ms->no_rule=0;

  /*
   * Calcul du nb de sous-termes du sujet
   */
  no_arg_subject=0;
  for(cell=term_first(subject) ; cell!=NULL ; cell=cell_next(cell)) {
    nb_subterm_subject++;
    greedy_multiplicity[no_arg_subject++]=cell_mult(cell);
  }

  /* Creation de bg_multiplicity */
  ms->bg_multiplicity=(int*)IMALLOC(nb_subterm_subject*sizeof(int));

  /*
   * creation du cbg vide (pas encore de lien)
   */ 
  ms->cbg=BG_create(nb_pattern_in_cbg,necessary_link); // avec LINK ?
  for(no_pattern=0 ; no_pattern < nb_pattern_in_cbg ; no_pattern++)
    {
      bitSet_create(mask,nb_subterm_subject);
      bitSet_init_clear(mask);
      BG_set(ms->cbg,no_pattern,mask);
    }
  
#ifdef DEBUG
  if(nb_pattern_in_cbg >= (MAX_CBG_SIZE*NBITS))
    {
      fprintf(stderr,"Too many pattern in CBG. Increase MAX_CBG_SIZE\n");
      exit(1);
    }
#endif

  greedy_bg = (BG**) IMALLOC(ms->nb_rule * sizeof(BG*));

  ms->bg_solution=(int*)IMALLOC(max_nb_pattern_under_AC * sizeof(int));
  greedy_solution=ms->bg_solution;

  /*
   * Initialisation de greedy_bg
   */
  for(i=0 ; i<ms->nb_rule ; i++) {
    if(isGreedyRule[i]) {
      greedy_bg[i] = BG_create(MS_pattern_list_size(ms,i),0);
      BG_cbg2bg(MS_get_pattern_list(ms,i),ms->cbg,greedy_bg[i]);
    } else {
      greedy_bg[i] = NULL;
    }
  }

  bitSet_create(match_mask,nb_pattern_in_cbg);
  bitSet_init_size(match_mask,nb_pattern_in_cbg);

  for(cell=term_first(subject),no_arg_subject=0 ;
      cell!=NULL ;
      cell=cell_next(cell),no_arg_subject++) {
    int indice_solution=-1;
    /*
     * match_mask contient les patterns sous le symbole AC qui filtrent
     * vers le sous-terme v
     */
    match_subterm(cell_t(cell),no_arg_subject,match_mask,ms->cbg);

    /* initialisation de bg_multiplicity */
    ms->bg_multiplicity[no_arg_subject]=cell_mult(cell);

    //printf("no_arg_subject=%d\n",no_arg_subject);

    /*
     * on construit le bipartie graph en representation compacte (CBG)
     * on associe a chaque pattern les sous-termes v qui qu'il filtre
     */
    for(no_pattern=0 ; no_pattern<nb_pattern_in_cbg ; no_pattern++) {

      if( bitSet_get(match_mask,no_pattern) ) {
	bitSet_set(BG_get(ms->cbg,no_pattern),no_arg_subject);


	// TESTER SI LE BG EST SOLVABLE

	//printf("start greedy:\n");

	/*
	 * Il faut tester les BG associes a no_pattern
	 */
	if(greedy_rule_tab[no_pattern] != NULL) {
	  int *greedy_rules = greedy_rule_tab[no_pattern];
	  int i,j;

	  //printf("\tno_pattern = %d\n",no_pattern);
	  
	  for(i=0 ; i<nb_pattern_in_cbg  && greedy_rules[i]!=-1; i++) {
	    int no_rule = greedy_rules[i];

	    //printf("\tno_rule = %d\n",no_rule);

	    for(j=0 ; j<BG_size(greedy_bg[no_rule]) ; j++) {
	      greedy_solution[j]=-1;
	    }
	    for(j=0 ; j<no_arg_subject+1 ; j++) {
	      greedy_multiplicity[j]=ms->bg_multiplicity[j];
	    }

	    //BG_print(greedy_bg[no_rule]);

	    indice_solution=BG_solve_one(greedy_bg[no_rule],greedy_solution,
					 greedy_multiplicity,
					 no_arg_subject+1,
					 0);
	    if(indice_solution>=0) {
	      //printf("\tno_arg_subject = %d\tindice_solution = %d\n",no_arg_subject,indice_solution);

	      
	      // On prepare la sortie
	      ms->no_rule=no_rule;
	      ms->bg=greedy_bg[no_rule];

	      
	      for(i=0 ; i<nb_subterm_subject ; i++) {
		ms->bg_multiplicity[i]=greedy_multiplicity[i];
	      }
	      
	      /*
	      for(i=0 ; i<no_arg_subject+1 ; i++) {
		ms->bg_multiplicity[i]=greedy_multiplicity[i];
	      }
	      cell=cell_next(cell);
	      no_arg_subject++;
	      for( ; cell!=NULL ; cell=cell_next(cell),no_arg_subject++) {
		ms->bg_multiplicity[no_arg_subject]=cell_mult(cell);
	      }
	      */
	      
	      return indice_solution;
	    }
	  }
	}

      }
    }
  }
  //printf("*** no_arg_subject = %d\n", no_arg_subject);

  bitSet_delete(match_mask);

 /*
  * size(pattern_list[no_rule]) devrait suffire mais cela complique
  * la gestion de la memoire
  */
  //  creation du bg
  ms->bg=BG_create(max_nb_pattern_under_AC,0); // sans LINK

  /* initialisation de bg_solution */
  for(i=0 ; i<BG_size(ms->bg) ; i++) {
    ms->bg_solution[i]=-1;
  }
  /* initialisation du bg */
  // FAIRE ms->no_rule++ TANT QUE BG IS GREEDY

  //while(1+ms->no_rule < ms->nb_rule && isGreedyRule[ms->no_rule]) {
  //ms->no_rule++;
  //}

  BG_set_size(ms->bg,MS_pattern_list_size(ms,ms->no_rule));
  BG_cbg2bg(MS_get_pattern_list(ms,ms->no_rule),ms->cbg,ms->bg);

  return -1;
}

int MS_greedy_solve(match_state *ms, int mode, int *isGreedyRule)
{
  int indice_solution=-1;
  int i;
  int total_nb_subterm_subject=0;
  Verif_void(ms,"MS_solve(ms)");

  begin_MS_solve:

  //MS_print(ms);
  if(!isGreedyRule[ms->no_rule]) {
    if(ms->bg_solution[0] == -1)
      indice_solution=BG_solve_one(ms->bg,ms->bg_solution,ms->bg_multiplicity,
				   bitSet_size(BG_get(ms->bg,0)),0);
    else
      /* BG_size(ms->bg)-1 est la valeur a donner pour continuer */
      indice_solution=BG_solve_one(ms->bg,ms->bg_solution,ms->bg_multiplicity,
				   bitSet_size(BG_get(ms->bg,0)),
				   BG_size(ms->bg)-1);
  }

  if(indice_solution >= 0)
    {
    }
  else
    {
      // On passe au prochain BG non greedy
      ms->no_rule++;
      while(ms->no_rule<ms->nb_rule && isGreedyRule[ms->no_rule]) {
	ms->no_rule++;
      }
      if(ms->no_rule < ms->nb_rule)
	{
#ifndef TEST
	  /* destruction de l'ancien bg */
	  for(i=0 ; i<BG_size(ms->bg) ; i++)
	    bitSet_delete( BG_get(ms->bg,i) );
#endif
	  BG_set_size(ms->bg,MS_pattern_list_size(ms,ms->no_rule));
	  BG_cbg2bg(MS_get_pattern_list(ms,ms->no_rule),ms->cbg,ms->bg);
	  /* initialisation de bg_solution */
	  for(i=0 ; i<BG_size(ms->bg) ; i++)
	    ms->bg_solution[i]=-1;	  
	  /* on re-cherche une solution */
	  goto begin_MS_solve;
	}
      else
	{
	  //indice_solution=-1;
	}
    }
  return indice_solution;
}
