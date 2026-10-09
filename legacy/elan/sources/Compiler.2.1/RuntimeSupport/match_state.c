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
  // avec BGSHARE il faut peut etre detruire les bitSet du bg

#ifdef GREEDY
  if(ms->cbg != ms->bg)
#endif
    {
    //MS_pattern_list_free(ms->pattern_list,ms->nb_rule);
#ifdef BGSHARE
    if(ms->bg->link_tab != NULL) {
      for(i=0 ; i<ms->bg->size ; i++) {
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
    }
  IFREE(ms->bg_solution);
  IFREE(ms->bg_multiplicity);
  IFREE(ms);
}


static int nb_subterm(struct term *subject) {
  struct cell_term *cell;
  int nb_subterm_subject=0;
  for(cell=term_first(subject) ; cell!=NULL ; cell=cell_next(cell)) {
    nb_subterm_subject++;
  }
  return nb_subterm_subject;
}


int MS_init(match_state **ptr_ms,
	     int (*match_subterm)(),
	     int nb_rule,
	     int **pattern_list,
	     int nb_pattern_in_cbg,
	     struct term *subject,
	     int necessary_link,
	     int max_nb_pattern_under_AC)
{
  match_state *ms=*ptr_ms=NULL;
  BG *cbg=NULL;
  int *bg_multiplicity;

  int nb_subterm_subject=0;
  int i,no_pattern,no_arg_subject;
  bitSet *mask;
  struct cell_term *cell;
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
#ifdef NOTMACRO
  nb_subterm_subject=nb_subterm(subject);
#else
  for(cell=term_first(subject) ; cell!=NULL ; cell=cell_next(cell))
    nb_subterm_subject++;
#endif
  
  /* Creation de bg_multiplicity */
  bg_multiplicity=(int*)IMALLOC(nb_subterm_subject*sizeof(int));

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

  for(cell=term_first(subject),no_arg_subject=0 ;
      cell!=NULL ;
      cell=cell_next(cell),no_arg_subject++) {
    /*
     * match_mask contient les patterns sous le symbole AC qui filtrent
     * vers le sous-terme v
     */
    nb_bit=match_subterm(cell_t(cell),no_arg_subject,match_mask,cbg);
    //printf("nb_bit = %d\n",nb_bit);

    /*
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
	*ptr_ms=ms=MS_create();
	ms->nb_rule=nb_rule;
	ms->pattern_list=pattern_list;
	ms->no_rule=0;
	
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

    /*
    *** Ancienne version
    if(nb_bit != 0) // juste pour accelerer un peu
      {
	for(no_pattern=0, no_bit=0 ; // (no_bit<nb_bit) &&
	    // juste pour accelerer un peu
	    (no_pattern<nb_pattern_in_cbg) ; no_pattern++) {
	  if( bitSet32_get(match_mask,no_pattern) ) {
	    
	    if(ms==NULL) {
	      int no_pattern;

	      *ptr_ms=ms=MS_create();
	      ms->nb_rule=nb_rule;
	      ms->pattern_list=pattern_list;
	      ms->no_rule=0;

	      ms->cbg=cbg=BG_create(nb_pattern_in_cbg,necessary_link);
	      ms->bg_multiplicity=bg_multiplicity;
	      for(no_pattern=0 ; no_pattern < nb_pattern_in_cbg ; no_pattern++) {
		bitSet_create(mask,nb_subterm_subject);
		bitSet_init_clear(mask);
		BG_set(ms->cbg,no_pattern,mask);
	      }
	    }

	    bitSet_set(BG_get(ms->cbg,no_pattern),no_arg_subject);
	    no_bit++;
	  }
	}
      }
      */
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

int MS_reinit(match_state *ms,struct term *subject, int no_rule) {
  int i;
  struct cell_term *cell;
  int no_arg_subject;

  if(ms==NULL) {
      //printf("ms==NULL\tsubject = "); term_printnl(stdout,subject);
    return -1;
  }

  for(cell=term_first(subject),no_arg_subject=0 ; cell!=NULL ;
      cell=cell_next(cell),no_arg_subject++) {
    /* initialisation de bg_multiplicity */
    ms->bg_multiplicity[no_arg_subject]=getMult(cell);
  }
  /* initialisation du bg */
  ms->no_rule=no_rule;
  BG_set_size(ms->bg,MS_pattern_list_size(ms,ms->no_rule));
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
  
#ifdef AFFICHAGE    
  //printf("BG_solve_one=%d\n",indice_solution);
#endif
  if(indice_solution >= 0) {
#ifdef AFFICHAGE
      printf("\t*** Solution : ");
      for(i=0 ; i<BG_size(ms->bg) ; i++)
      printf("%d ",ms->bg_solution[i]);
      printf("\n");
#endif
  } else {
#ifdef AFFICHAGE
      //printf("\ton passe au pattern suivant\n");
#endif
    next_bg:
      // On passe au prochain BG non greedy
    ms->no_rule++;
    if(ms->no_rule < ms->nb_rule) {
#ifndef BGSHARE
        /* destruction de l'ancien bg */
      for(i=0 ; i<BG_size(ms->bg) ; i++) {
        bitSet_delete( BG_get(ms->bg,i) );
      }
#endif
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

int MS_solve_rule(match_state *ms)
{
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
void MS_give_rest(match_state *ms, struct term *t) {
  bitSet *used;
  struct cell_term *cell;
  struct cell_term *tmp=NULL;
  int i;

  bitSet_create(used,bitSet_size(BG_get(ms->bg,0)));
  bitSet_init_clear(used);
  for(i=0 ; i<BG_size(ms->bg) ; i++) {
    bitSet_set(used,ms->bg_solution[i]);
  }
  
  for(cell=term_first(t), i=0 ; cell != NULL ; tmp=cell, cell=cell_next(cell),i++) {
    if(bitSet_get(used,i)) {
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
  



  if(ms->bg_solution[0] == -1) {
    indice_solution=BG_solve_one(ms->bg,ms->bg_solution,ms->bg_multiplicity,
				 bitSet_size(BG_get(ms->bg,0)),0);
  } else {
    /* BG_size(ms->bg)-1 est la valeur a donner pour continuer */
    indice_solution=BG_solve_one(ms->bg,ms->bg_solution,ms->bg_multiplicity,
				 bitSet_size(BG_get(ms->bg,0)),
				 BG_size(ms->bg)-1);
  }
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
      next_bg:
#ifdef AFFICHAGE
      printf("\ton passe au pattern suivant\n");
#endif
      ms->no_rule++;
      if(ms->no_rule < ms->nb_rule)
	{
#ifndef BGSHARE
	  /* destruction de l'ancien bg */
	  for(i=0 ; i<BG_size(ms->bg) ; i++) {
	    bitSet_delete( BG_get(ms->bg,i) );
	  }
#endif
	  BG_set_size(ms->bg,MS_pattern_list_size(ms,ms->no_rule));
	  if(!BG_cbg2bg(MS_get_pattern_list(ms,ms->no_rule),ms->cbg,ms->bg)) {
              /* there is no solution */
            goto next_bg;
          }
          
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

#define AFFICHAGE
int MS_extended_solve_rule(match_state *ms)
{
  int indice_solution;
  int *pattern_list;

  Verif_void(ms,"MS_extended_solve(ms)");

  printf("MS_extended_solve\n");

  //MS_print(ms);

  /*
   * Resolution du 1er niveau
   */
  if(ms->bg_solution[0] == -1) {
    indice_solution = BG_solve_one(ms->bg,ms->bg_solution,ms->bg_multiplicity,
				   bitSet_size(BG_get(ms->bg,0)),0);
  } else {
    /* BG_size(ms->bg)-1 est la valeur a donner pour continuer */
    indice_solution = BG_solve_one(ms->bg,ms->bg_solution,ms->bg_multiplicity,
				   bitSet_size(BG_get(ms->bg,0)),
				   BG_size(ms->bg)-1);
  }

  if(indice_solution<0) {
    return indice_solution;
  }

  /*
   * solution suivante des sous-problemes
   */

  if(ms->bg_solution[0] != -1) { // il y a une solution en cours
    int i;
    // on cherche dans le cbg, le premier msbg 
#ifdef AFFICHAGE
    printf("  Recherche du premier msbg\n");
#endif

    pattern_list=MS_get_pattern_list(ms,ms->no_rule);
    for( i=0 ; i < BG_size(ms->bg) ; i++) {
      LINK *link;
      match_state *msbg;
      int indice;
      int id_pattern, no_arg_subject;
#ifdef AFFICHAGE
      printf("  i=%d\n",i);
#endif
      id_pattern=pattern_list[i];
      no_arg_subject=ms->bg_solution[i];
      if( (link=BG_link_get(ms->cbg,id_pattern)) != NULL ) {
	if( (msbg=LINK_get(link,no_arg_subject)) != NULL ) {
#ifdef AFFICHAGE
	  printf("  on en a un ! reste-t-il une solution ?\n");
#endif
	  indice=MS_solve(msbg,POSSIBLE_REST);
	  if(indice>=0) {
#ifdef AFFICHAGE
	    printf("OK on a une solution\n");
#endif
	    return indice;
	  } else {
	    /*
	     * il faut re-initialiser le msbg
	     * et cherche un cran plus loin
	     */
#ifdef AFFICHAGE
	    printf("Re-initialisation du msbg\n");
#endif
	    for(i=0 ; i<BG_size(msbg->bg) ; i++) {
	      msbg->bg_solution[i]=-1;
	    }
	    BG_solve_one(msbg->bg,msbg->bg_solution,
			 msbg->bg_multiplicity,
			 bitSet_size(BG_get(msbg->bg,0)),0);   
	  }
	}
      }
    }
    /*
     * if( i >= BG_size(ms->bg) )
     * il faut la solution suivante du BG
     */
  }
  printf("pas de sol !!!\n");
  return -1;
}


/*
 * Greedy matching
 */
#ifdef RTA
static int diff=0;
static int total=0;
static int total_match=0;
static int total_fail=0;
static int total_call=0;
#endif

int MS_greedy_init(match_state **ptr_ms,
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
  match_state *ms=*ptr_ms=NULL;
  BG *cbg=NULL;
  int *bg_multiplicity;

  int nb_subterm_subject=0;
  int i,no_pattern,no_arg_subject;
  bitSet *mask;
  struct cell_term *cell;
  bitSet32 *match_mask;
  bitSet *greedy_bitset;

  bitSet32_create(match_mask,nb_pattern_in_cbg); 

  //Verif_void(ms,"MS_greedy_init(ms)");
#ifdef RTA
  total_call++;
  printf("total_call=%d\n",total_call);
#endif

  /*
   * Calcul du nb de sous-termes du sujet
   */
#ifdef NOTMACRO
  nb_subterm_subject=nb_subterm(subject);
#else
  nb_subterm_subject=0;
  for(cell=term_first(subject) ; cell!=NULL ; cell=cell_next(cell))
    nb_subterm_subject++;
#endif

  bitSet_create(greedy_bitset,nb_subterm_subject);

  /* Creation de bg_multiplicity */
  bg_multiplicity=(int*)IMALLOC(nb_subterm_subject*sizeof(int));

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
  if(nb_pattern_in_cbg >= (MAX_CBG_SIZE*NBITS))
    {
      fprintf(stderr,"Too many pattern in CBG. Increase MAX_CBG_SIZE\n");
      exit(1);
    }
#endif

  for(cell=term_first(subject),no_arg_subject=0 ;
      cell!=NULL ;
      cell=cell_next(cell),no_arg_subject++) {
    int indice_solution=-1;
    /*
     * match_mask contient les patterns sous le symbole AC qui filtrent
     * vers le sous-terme v
     */
    int nb_bit=match_subterm(cell_t(cell),no_arg_subject,match_mask,cbg);

    //printf("no_arg_subject=%d\n",no_arg_subject);

    /*
     * on construit le bipartie graph en representation compacte (CBG)
     * on associe a chaque pattern les sous-termes v qui qu'il filtre
     */
    if(nb_bit != 0) // juste pour accelerer un peu
      {
	for(no_pattern=0 ; no_pattern<nb_pattern_in_cbg ; no_pattern++) {
	  if( bitSet32_get(match_mask,no_pattern) ) {
	    if(ms==NULL) {
	      int no_pattern;

	      *ptr_ms=ms=MS_create();
	      ms->nb_rule=nb_rule;
	      ms->pattern_list=pattern_list;
	      ms->no_rule=0;

	      ms->cbg=cbg=BG_create(nb_pattern_in_cbg,necessary_link);
	      ms->bg_multiplicity=bg_multiplicity;
	      for(no_pattern=0 ; no_pattern < nb_pattern_in_cbg ; no_pattern++) {
		bitSet_create(mask,nb_subterm_subject);
		bitSet_init_clear(mask);
		BG_set(ms->cbg,no_pattern,mask);
	      }
	    }
	    bitSet_set(BG_get(ms->cbg,no_pattern),no_arg_subject);
	  }
	}
      }
    /* initialisation de bg_multiplicity */
    bg_multiplicity[no_arg_subject]=getMult(cell);
    
    if(ms!=NULL) {
    for(no_pattern=0 ; no_pattern<nb_pattern_in_cbg ; no_pattern++) {
      if( bitSet32_get(match_mask,no_pattern) ) {
	if(greedy_rule_tab[no_pattern] != NULL) {
	  
	  int *greedy_rules = greedy_rule_tab[no_pattern];
	  for(i=0 ; i<nb_pattern_in_cbg  && greedy_rules[i]!=-1; i++) {
	    int no_rule = greedy_rules[i];
	    // NE PAS FAIRE 2 FOIS LA MEME REGLE
	    
	    // Calcul de P1 v ... v Pn des pattern impliques
	    int *liste_pattern = MS_get_pattern_list(ms,no_rule);
	    int nb_pattern;
	    int j;
	    
	    nb_pattern = MS_pattern_list_size(ms,no_rule);

	    //printf("start greedy: rule %d\n",i);
	    bitSet_init_clear(greedy_bitset);
	    for(j=0 ; j<nb_pattern ; j++) {
	      bitSet *b;
	      b = BG_get(ms->cbg,liste_pattern[j]);
	      //printf("no_pattern = %d\tb  = ",liste_pattern[j]);
	      //bitSet_print(b); 
	      //printf("\tgb = "); bitSet_print(greedy_bitset); 
	      //printf("\n");
	      bitSet_or(greedy_bitset,b);
	    }
	    //printf("new gb = "); bitSet_print(greedy_bitset); 
	    //printf("\tnb_bit = %d\n",bitSet_nb_bit(greedy_bitset));
	    
	    if(bitSet_nb_bit(greedy_bitset) >= nb_pattern) {
	      //printf("\tno_arg_subject = %d\t no_rule = %d\n", no_arg_subject,no_rule);
	      ms->no_rule=no_rule;
	      //  creation du bg
	      ms->bg=BG_create(max_nb_pattern_under_AC,0); // sans LINK
	      BG_set_size(ms->bg,nb_pattern);
	      BG_cbg2bg(liste_pattern,ms->cbg,ms->bg);
	      
	      /*
	      for(j=0 ; j<nb_pattern ; j++) {
		BG_set(ms->cbg,j,BG_get(ms->cbg,liste_pattern[j]));
	      }
	      ms->bg=ms->cbg;
	      BG_set_size(ms->bg,nb_pattern);
	      */

	      /* initialisation de bg_solution */
	      ms->bg_solution=(int*)IMALLOC(max_nb_pattern_under_AC*sizeof(int));
	      for(j=0 ; j<nb_pattern ; j++) {
		ms->bg_solution[j]=-1;
	      }
	      indice_solution=BG_solve_one(ms->bg,ms->bg_solution,
					   ms->bg_multiplicity,
					   no_arg_subject+1,
					   0);
	      if(indice_solution<0) {
		printf("probleme !!!\n");
	      }
#ifdef RTA    
	      //printf("no_arg_subject = %d\t nb_total = %d\n", no_arg_subject,nb_subterm_subject);

	      total_match+=no_arg_subject+1;
	      total+=nb_subterm_subject;
	      diff=total-total_match;
	      printf("total_match=%d\t total=%d\t diff=%d\n",total_match,total,diff);
#endif

	      //if(no_arg_subject > 10)
	      //term_printnl(stdout, subject);


	      // On prepare la sortie
	      cell=cell_next(cell);
	      no_arg_subject++;
	      for( ; cell!=NULL ; cell=cell_next(cell),no_arg_subject++) {
		ms->bg_multiplicity[no_arg_subject]=getMult(cell);
	      }

	      bitSet32_delete(match_mask);
	      bitSet_delete(greedy_bitset);
		
	      return indice_solution;
	    } 
	  }
	}
      }
    }
    } // if(ms!=NULL)
  }
  bitSet_delete(greedy_bitset);
  bitSet32_delete(match_mask);

  /*
   * 1er critere de detection d'echec
   */
  if(ms==NULL) {
#ifdef RTA
    total_fail++;
    printf("total_fail=%d\n",total_fail);
#endif


    //printf("no solution !!!\n");
    /*
     * Destruction du debut de MS
     */
    
    IFREE(bg_multiplicity);
    return -2;

  }

#ifdef RTA
  //printf("*** no_arg_subject = %d\n", no_arg_subject);
#endif


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
  // FAIRE ms->no_rule++ TANT QUE BG IS GREEDY

  while(1+ms->no_rule < ms->nb_rule && isGreedyRule[ms->no_rule]) {
    ms->no_rule++;
  }

  BG_set_size(ms->bg,MS_pattern_list_size(ms,ms->no_rule));
  BG_cbg2bg(MS_get_pattern_list(ms,ms->no_rule),ms->cbg,ms->bg);

#ifdef RTA
  total_match+=nb_subterm_subject;
  total+=nb_subterm_subject;
  diff=total-total_match;
  printf("total_match=%d\t total=%d\t diff=%d\n",total_match,total,diff);
#endif

  //if(nb_subterm_subject ==256 )
  //term_printnl(stdout, subject);

  return -1;
}

int MS_greedy_solve(match_state *ms, int mode, int *isGreedyRule)
{
  int indice_solution=-1;
  int i;
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
#ifndef BGSHARE
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



int MS_init_test(match_state **ptr_ms,
		 int nb_subterm_subject,
		 int *bg_multiplicity,
	     int (*match_subterm)(),
	     int nb_rule,
	     int **pattern_list,
	     int nb_pattern_in_cbg,
	     struct term *subject,
	     int necessary_link,
	     int max_nb_pattern_under_AC)
{
  match_state *ms=*ptr_ms=NULL;
  BG *cbg=NULL;
  int i,no_pattern,no_arg_subject;
  bitSet *mask;
  struct cell_term *cell;
  int no_bit,nb_bit; // pour optimiser l'initialisation de CBG

  bitSet32 *match_mask;
  bitSet32_create(match_mask,nb_pattern_in_cbg);

  //bitSet_stack_create(match_mask,MAX_CBG_SIZE);

  //Verif_void(ms,"MS_init(ms)");

  //bitSet_init_size(match_mask,nb_pattern_in_cbg); 

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

  for(cell=term_first(subject),no_arg_subject=0 ;
      cell!=NULL ;
      cell=cell_next(cell),no_arg_subject++) {
    /*
     * match_mask contient les patterns sous le symbole AC qui filtrent
     * vers le sous-terme v
     */
    nb_bit=match_subterm(cell_t(cell),no_arg_subject,match_mask,cbg);
    
    /*
      printf("no_arg_subject=%d\n",no_arg_subject);
      */
    
    /*
     * on construit le bipartie graph en representation compacte (CBG)
     * on associe a chaque pattern les sous-termes v qui qu'il filtre
     */
    if(nb_bit != 0) // juste pour accelerer un peu
      {
	for(no_pattern=0, no_bit=0 ; // (no_bit<nb_bit) &&
	    // juste pour accelerer un peu
	    (no_pattern<nb_pattern_in_cbg) ; no_pattern++) {
	  if( bitSet32_get(match_mask,no_pattern) ) {
	    
	    if(ms==NULL) {
	      int no_pattern;

	      *ptr_ms=ms=MS_create();
	      ms->nb_rule=nb_rule;
	      ms->pattern_list=pattern_list;
	      ms->no_rule=0;

	      ms->cbg=cbg=BG_create(nb_pattern_in_cbg,necessary_link);
	      ms->bg_multiplicity=bg_multiplicity;
	      for(no_pattern=0 ; no_pattern < nb_pattern_in_cbg ; no_pattern++) {
		bitSet_create(mask,nb_subterm_subject);
		bitSet_init_clear(mask);
		BG_set(ms->cbg,no_pattern,mask);
	      }
	    }

	    bitSet_set(BG_get(ms->cbg,no_pattern),no_arg_subject);
	    no_bit++;
	  }
	}
      }

  }
  bitSet32_delete(match_mask);

  /*
   * 1er critere de detection d'echec
   */
  if(ms==NULL) {
    //printf("no solution !!!\n");
    /*
     * Destruction du debut de MS
     */
    //IFREE(bg_multiplicity);

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
  for(i=0 ; i<BG_size(ms->bg) ; i++) {
    ms->bg_solution[i]=-1;
  }
  /* initialisation du bg */
  BG_set_size(ms->bg,MS_pattern_list_size(ms,ms->no_rule));
  BG_cbg2bg(MS_get_pattern_list(ms,ms->no_rule),ms->cbg,ms->bg);
  return 1;
}
