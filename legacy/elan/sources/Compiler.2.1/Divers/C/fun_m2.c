#include "fun_m2.h"
#include "fun_p1.h"
#include "match_state.h"
#include "term.h"
#include "dart.h"

static **pattern_list_m2;
#define max_nb_pattern_under_m2 2
static int no_pattern_m2_niv_0;
static int no_pattern_m2_niv_1;

void init_pattern_list_m2()
{
  int pattern_tab[max_nb_pattern_under_m2]; // nombre max de pattern sous AC
  /*
   * construction de pattern_list
   */
  int nb_pattern_m2_niv_0=3; // 3 regles
  no_pattern_m2_niv_0=0;
  no_pattern_m2_niv_1=0;
  pattern_list_m2=MS_pattern_list_create(nb_pattern_m2_niv_0);
  // m2(empty,x) => ...
  pattern_tab[0]=0; // empty
  no_pattern_m2_niv_1++;
  MS_pattern_list_init(pattern_list_m2,no_pattern_m2_niv_0++,1,pattern_tab);
  // m2(set(x),set(y)) => ...
  pattern_tab[0]=1; // set(x)
  pattern_tab[1]=1; // set(y)
  no_pattern_m2_niv_1++;
  MS_pattern_list_init(pattern_list_m2,no_pattern_m2_niv_0++,2,pattern_tab);
  // m2( p1(set(x),y) , z ) => ...
  pattern_tab[0]=2; // p1(set(x),y)
  no_pattern_m2_niv_1++;
  MS_pattern_list_init(pattern_list_m2,no_pattern_m2_niv_0++,1,pattern_tab);

}

static **pattern_list_m2p1;
#define max_nb_pattern_under_m2p1 1
static int no_pattern_m2p1_niv_0;
static int no_pattern_m2p1_niv_1;

void init_pattern_list_m2p1()
{
  int pattern_tab[max_nb_pattern_under_m2p1]; // nombre max de pattern sous AC
  /*
   * construction de pattern_list
   */
  int nb_pattern_m2p1_niv_0=1; // 1 regles
  no_pattern_m2p1_niv_0=0;
  no_pattern_m2p1_niv_1=0;
  pattern_list_m2p1=MS_pattern_list_create(nb_pattern_m2p1_niv_0);
  // p1(set(x),y)) => ...
  pattern_tab[0]=0; // set(x)
  no_pattern_m2p1_niv_1++;
  MS_pattern_list_init(pattern_list_m2p1,no_pattern_m2p1_niv_0++,1,pattern_tab);
}

void delete_pattern_list_m2()
{
  MS_pattern_list_free(pattern_list_m2,no_pattern_m2_niv_0);
}

void delete_pattern_list_m2p1()
{
  MS_pattern_list_free(pattern_list_m2p1,no_pattern_m2p1_niv_0);
}



/* ------------------------------------------------------------ */
/* fun_m2                                                       */
/* ------------------------------------------------------------ */

struct term *fun_m2(struct term *t)
{
  match_state *ms;
  int indice;
  int i,j;
  int mode;
  struct term *res;
  int necessary_link;

  addindent();
  ms=MS_create();
  if(trace)
    {
      indent(indentlevel);
      printf("fun_m2( "); term_print(stdout,t); printf(" ) \n");
    }

  /*
   * initialisation du match_state
   *
   * 3 regles : 
   *   0 : m2(empty,x)
   *   1 : m2(set(x),set(y))
   *   2 : m2(p1(set(x),y) , z)
   *
   * 3 pattern_in_cbg :
   *   0 : empty
   *   1 : set(x)
   *   2 : p1(set(x),y)
   */
  necessary_link=1; // 2 niveaux
  MS_init(ms, match_subterm_m2, no_pattern_m2_niv_0,
	  pattern_list_m2, no_pattern_m2_niv_1, t,
	  necessary_link, max_nb_pattern_under_m2 );
#ifdef M2
  printf("Fin du filtrage\n");
#endif
  mode=POSSIBLE_REST;

  indice=MS_extended_solve(ms,mode);
      
  if(indice>=0) 
    { 
#ifdef M2
      indent();
      printf("Application de la regle %d\n",ms->no_rule);
#endif
      /*
       * recuperation de la substitution
       */
      
      if(ms->no_rule == 0)
	{
	  int i;
	  int nb_variable=1; // 1 var AC
	  struct term *substitution[1];
	  int nb_variable_ac=1;

	  substitution_build(t,ms,
			     nb_variable,substitution,
			     nb_variable_ac,
			     m2_variable_extract
			     );
	  
#ifdef M2
	  for(i=0 ; i <nb_variable ; i++)
	    {
	      indent();
	      printf("\tsubstitution[%d] = ",i);
	      term_printnl(stdout,substitution[i]);
	    }
#endif 
	  // Construction de la partie droite
	  res=term_removeTopSymbol(substitution[0]);
	  if(res==NULL)
	    res=fun_m2(substitution[0]);

	  addcounter(res);
	  for(i=0 ; i <nb_variable ; i++)
	    {
	      subcounter(substitution[i]);
	      
	      /*
	      printf("substitution[%d]->counter = %d\n",
		     i,substitution[i]->counter);
	      printf("substitution[%d] = ",i);
	      term_printnl(stdout,substitution[i]);
	      */

	      if(substitution[i]->counter == 0)
		freeterm(substitution[i]);
	    }
	  subcounter(res);
	  goto end;
	}
      
      if(ms->no_rule == 1)
	{
	  int i;
	  int nb_variable=3; // 2 var + 1 context AC
	  struct term *substitution[3];
	  int nb_variable_ac=1;
	  struct term *tmp_set;
	  struct term *tmp_int;
	  
	  substitution_build(t,ms,
			     nb_variable,substitution,
			     nb_variable_ac,
			     m2_variable_extract
			     );
	      
#ifdef M2
	  for(i=0 ; i <nb_variable ; i++)
	    {
	      indent();
	      printf("\tsubstitution[%d] = ",i);
	      term_printnl(stdout,substitution[i]);
	    }
#endif 
	  // Construction de la partie droite
	  TERM_ALLOC(tmp_set,term1,NULL,code_set);
#ifdef PEANO
	  tmp_int = fun_mult(substitution[1], substitution[2]);
				   
#else
	  TERM_ALLOC(tmp_int,term1,NULL,code_int);
	  tmp_int->sub[0] = (struct term*)
	    (((long)(substitution[1]->sub[0])) *
	     ((long)(substitution[2])->sub[0]));
#endif	  
	  tmp_set->sub[0]=tmp_int; addcounter(tmp_int);
	  term_add_onf_term(substitution[0] , tmp_set);
	  res=term_removeTopSymbol(substitution[0]);
	  if(res==NULL)
	    res=fun_m2(substitution[0]);

	  addcounter(res);
	  for(i=0 ; i <nb_variable ; i++)
	    {
	      subcounter(substitution[i]);
	      if(substitution[i]->counter == 0)
		freeterm(substitution[i]);
	    }
	  subcounter(res);
	  goto end;
	}
      

      if(ms->no_rule == 2)
	{
	  int i;
	  int nb_variable=3; // 2 var + 1 var AC
	  struct term *substitution[3];
	  int nb_variable_ac=1;

	  struct term* p1;
	  struct term* m2_1;
	  struct term* m2_2;
	  struct term* set;
	  struct term* tmp;
	  struct term *s1, *s2;

	  substitution_build(t,ms,
			     nb_variable,substitution,
			     nb_variable_ac,
			     m2_variable_extract
			     );
	  
#ifdef M2
	  for(i=0 ; i <nb_variable ; i++)
	    {
	      indent();
	      printf("\tsubstitution[%d] = ",i);
	      term_printnl(stdout,substitution[i]);
	    }
#endif 
	  // Construction de la partie droite
	  tmp=term_removeTopSymbol(substitution[0]);
#ifndef CMPOBJ
	  if(tmp==NULL)
	    s2=fun_m2(substitution[0]);
	  else
	    s2=tmp;
#else
	  // TEST 200.000
	  if(tmp==NULL)
	    s2=substitution[0];
	  else
	    s2=tmp;
#endif
	  
	  tmp=term_removeTopSymbol(substitution[1]);
	  if(tmp!=NULL)
	    s1=tmp;
	  else
	    s1=substitution[1];
	  
	  //printf("s2 = "); term_printnl(stdout,s2);
	  //printf("s1 = "); term_printnl(stdout,s1);

	  addcounter(s2);
	  TERM_ALLOC(m2_1,term2,NULL,code_m2);
	  TERM_ALLOC(set,term1,NULL,code_set);
	  set->sub[0]=substitution[2]; addcounter(substitution[2]);
	  term_add_onf_term(m2_1,set);
	  term_add_onf_term(m2_1,s2);
	  m2_1=fun_m2(m2_1);
	  subcounter(s2);
	  
	  addcounter(m2_1);
	  TERM_ALLOC(m2_2,term2,NULL,code_m2);
	  term_add_onf_term(m2_2,s1);
	  term_add_onf_term(m2_2,s2);
	  m2_2=fun_m2(m2_2);
	  subcounter(m2_1);
	  
	  TERM_ALLOC(p1,term2,NULL,code_p1);
	  term_add_onf_term(p1,m2_1);
	  term_add_onf_term(p1,m2_2);
	  p1=fun_p1(p1);
	  
	  res=p1;

	  addcounter(res);
	  for(i=0 ; i <nb_variable ; i++)
	    {
	      subcounter(substitution[i]);
	      if(substitution[i]->counter == 0)
		freeterm(substitution[i]);
	    }
	  subcounter(res);
	  goto end;
	}
    }
  else
    {
      indent(indentlevel);
      printf("il faut retourner le terme de depart\n");
      res=t;
      goto end_no_rewrite;
    }

  fprintf(stderr,"On ne peut pas venir ici\n");
  exit(1);

end:  
  rewrite_step++;
end_no_rewrite:
  if(trace)
    {
      indent(indentlevel);
      printf("rewrite[%d] ",rewrite_step);
      term_printnl(stdout,res);
    }
  MS_delete(ms);
  subindent();
  return res;

}

/*
 * Codage :
 * empty        : bit 0
 * set(x)       : bit 1
 * p1(set(x),y) : bit 2
 */

void m2_variable_extract(struct term *t,
			 int id_pattern,
			 struct term *extract_substitution[],
			 int *indice,
			 // Pour le 2eme niveau :
			 struct match_state *ms,
			 int no_arg_subject,
			 int no_pattern
			 )
{
  /*  
  printf("m2_variable_extract\n");
  printf("\tt="); term_printnl(stdout,t);
  printf("\tid_pattern=%d\n",id_pattern);
  printf("\t*indice=%d\n",*indice);
  */

  switch(id_pattern)
    {
    case 0: 
      break;
    case 1:
      extract_substitution[*indice]=t->sub[0];
      addcounter(extract_substitution[*indice]);
      (*indice)++;
      break;
    case 2:
      {
	//printf("m2_variable_extract, pattern 2 : not yet implemented\n");
	int nb_variable=2; // 1 var + 1 var AC
	struct term *substitution[2];
	int nb_variable_ac=1;
	LINK *link;
	match_state *msbg;
	int i;
	// il faut retrouver le msbg du link
#ifdef M2
	printf("id_pattern=%d\n",id_pattern);
#endif
	link=BG_link_get(ms->cbg,id_pattern);
#ifdef DEBUG
	if(link == NULL)
	  {
	    fprintf(stderr,"m2_variable_extract : null link error\n");
	    exit(1);
	  }
#endif
	msbg=LINK_get(link,no_arg_subject);
#ifdef DEBUG
	if(msbg == NULL)
	  {
	    fprintf(stderr,"m2_variable_extract : null msbg error\n");
	    exit(1);
	  }
#endif
#ifdef M2
	printf("\n2eme niveau...\n\n");
#endif
	substitution_build(t,msbg,
			   nb_variable,substitution,
			   nb_variable_ac,
			   m2p1_variable_extract
			   );
	
	/*
	for(i=0 ; i <nb_variable_ac ; i++)
	  {
	    extract_substitution[*indice]=variable_ac[i];
	    (*indice)++;

	    //indent();
	    //printf("\t\t*** variable_ac[%d] = ",i);
	    //term_printnl(stdout,variable_ac[i]);
	  }
	  */
	for(i=0 ; i <nb_variable ; i++)
	  {
	    extract_substitution[*indice]=substitution[i];
	    (*indice)++;

	    //indent();
	    //printf("\t\t*** extract_substitution[%d] = ",i);
	    //term_printnl(stdout,extract_substitution[i]);
	  }


      }
      break;
    default:
      fprintf(stderr,"m2_variable_extract : bad pattern number\n");
      exit(0);
    }
}

int match_subterm_m2(struct term *t,int no_arg_subject, bitSet *mask, BG *cbg)
{
  int nb_bit=0;
  bitSet_init_clear(mask);
  switch(t->symb)
    {
    case code_empty:
      bitSet_set(mask,0);
      nb_bit++;
      break;
    case code_set:
      bitSet_set(mask,1);
      nb_bit++;
      break;
    case code_p1:
      {
	/*
	 * ici on construit le CBG principal
	 */
	int base_id_pattern=2;
	int id_pattern,no_rule;
	int pattern_tab[max_nb_pattern_under_m2p1];
	match_state *ms;
	match_state *msbg;
	LINK *link;
	int necessary_link;

	ms=MS_create();
	/*
	 * initialisation du match_state
	 * 1 regle : p1(set(x),y))
	 * 1 pattern_in_cbg : set(x)
	 */
	necessary_link=0; // 1 niveau
	MS_init(ms, match_subterm_m2p1, no_pattern_m2p1_niv_0,
		pattern_list_m2p1, no_pattern_m2p1_niv_1, t,
		necessary_link, max_nb_pattern_under_m2p1);

	/*
	 * Pour chaque BG du match_state qui a au moins une solution
	 * on le memorise (sous la forme d'un match_state) dans le link
	 * et on met a jour le mask
	 */
#ifdef M2
	printf("  Creation du link\n");
#endif
	//	MS_print(ms); 
	//	printf("\n");

	for(no_rule=0 ; no_rule < ms->nb_rule ; no_rule++)
	  {
	    int nb_pattern;
	    int nb_subterm_subject;
	    int indice,i;

	    int no_element;
	    int *ms_pattern_tab;
	    int **pattern_list2;
#ifdef M2
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
	    for(no_element=0 ;
		no_element < MS_pattern_list_size(ms,no_rule) ;
		no_element++)
	      {
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
	    msbg->bg_multiplicity=(int*)MALLOC(nb_subterm_subject*sizeof(int));
	    /* Creation de bg_solution */
	    msbg->bg_solution=(int*)MALLOC(BG_size(msbg->bg)*sizeof(int));

	    BG_cbg2bg(MS_get_pattern_list(ms,no_rule),ms->cbg,msbg->bg);

	    /* initialisation de bg_solution */
	    for(i=0 ; i<BG_size(msbg->bg) ; i++)
	      msbg->bg_solution[i]=-1;
	    /* initialisation de bg_multiplicity */
	    for(i=0 ; i<nb_subterm_subject ; i++)
	      msbg->bg_multiplicity[i]=ms->bg_multiplicity[i];

	    /*
	      printf("BG=");
	      BG_print(msbg->bg);
	      printf("\n");
	    */
	    
	    /* y-a-t-il une solution ? */
	    indice=BG_solve_one(msbg->bg,msbg->bg_solution,msbg->bg_multiplicity,0);  
#ifdef M2
	    printf("y-a-t-il une solution ?, indice=%d\n",indice);
#endif

	    if(indice>=0)
	      {
		//printf("Re-initialisation du msbg\n");
		//for(i=0 ; i<BG_size(msbg->bg) ; i++)
		//  msbg->bg_solution[i]=-1;

		// !!!!!!!!!!!!!!!!!!!!!!!!!!!
		// 2 : no du subpattern commencant par p1
		/*
		 * il faut etablir un correspondance entre les numeros
		 * des subpatterns de m2p1 et les subpattern de m2
		 * ici, set(x) : bit 0, de m2p1, correspond
		 * au subpattern p1(set(x),y) : bit 2, de m2
		 */
		/*
		 * les patterns commencant par p1(...) sont numerotes:
		 * - base_id_pattern+0
		 * - base_id_pattern+1
		 * - base_id_pattern+2
		 * - ...
		 */
		id_pattern=base_id_pattern+no_rule;
#ifdef M2
		printf("id_pattern=%d\n",id_pattern);
#endif
		// !!!!!!!!!!!!!!!!!!!!!!!!!!!
		bitSet_set(mask,id_pattern);
		nb_bit++;
		/*
		 * si le link existe deja on le complete sinon on le cree
		 */
		link=BG_link_get(cbg,id_pattern);
		if(link==NULL)
		  {
		    link=LINK_create(  bitSet_size(BG_get(cbg,id_pattern)) );
		    BG_link_set(cbg,id_pattern,link);
		  }
		LINK_set(link,no_arg_subject,msbg);
	      }
	    else
	      {
		//printf("Pas de solution : il faut detruire le msbg\n"); 
		/* il faut detruire le msbg */
		MS_pattern_list_free(msbg->pattern_list,msbg->nb_rule);
		MS_delete(msbg);
	      }
	  }
	// IL FAUT DETRUIRE : ms
	MS_delete(ms);
      }
      break;
      default:
      goto match_fail;
    }
  return nb_bit;
match_fail:
  bitSet_init_clear(mask);
  return nb_bit;
}

/*
 * Codage :
 * set(x)   : bit 0
 */

void m2p1_variable_extract(struct term *t,int id_pattern,
			   struct term *extract_substitution[],
			   int *indice,
			   // Pour le 2eme niveau :
			   struct match_state *ms,
			   int no_arg_subject,
			   int no_pattern
			   )
{
  /*
  printf("m2p1_variable_extract\n");
  printf("\tt="); term_printnl(stdout,t);
  printf("\tid_pattern=%d\n",id_pattern);
  printf("\t*indice(%d)=%d\n",indice,*indice);
  */

  switch(id_pattern)
    {
    case 0:
      extract_substitution[*indice]=t->sub[0];
      addcounter(extract_substitution[*indice]);
      (*indice)++;
      break;
    default:
      fprintf(stderr,"m2p1_variable_extract : bad pattern number\n");
      exit(0);
    }
}

int match_subterm_m2p1(struct term *t,int no_arg_subject, bitSet *mask, BG *cbg)
{
  int nb_bit=0;
  bitSet_init_clear(mask);
  switch(t->symb)
    {
      // set(x)
    case code_set:
      bitSet_set(mask,0);
      nb_bit++;
      break;
    default:
      goto match_fail;
    }
  return nb_bit;
match_fail:
  bitSet_init_clear(mask);
  return nb_bit;
}

