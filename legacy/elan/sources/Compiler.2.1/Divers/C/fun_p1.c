#include "fun_p1.h"
#include "match_state.h"
#include "term.h"
#include "dart.h"


static **pattern_list_p1;
#define max_nb_pattern_under_p1 1
static int no_pattern_p1_niv_0;
static int no_pattern_p1_niv_1;

void init_pattern_list_p1()
{
  int pattern_tab[max_nb_pattern_under_p1]; // nombre max de pattern sous AC
  
  /*
   * construction de pattern_list
   */
  int nb_pattern_p1_niv_0=1; // 1 regles
  no_pattern_p1_niv_0=0;
  no_pattern_p1_niv_1=0;
  pattern_list_p1=MS_pattern_list_create(nb_pattern_p1_niv_0);
  // p1(empty,x) => x
  pattern_tab[0]=0; // empty
  no_pattern_p1_niv_1++;
  MS_pattern_list_init(pattern_list_p1,no_pattern_p1_niv_0++,1,pattern_tab);
}

void delete_pattern_list_p1()
{
  MS_pattern_list_free(pattern_list_p1,no_pattern_p1_niv_0);
}

/* ------------------------------------------------------------ */
/* fun_p1                                                       */
/* ------------------------------------------------------------ */

struct term *fun_p1(struct term *t)
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
      printf("fun_p1( "); term_print(stdout,t); printf(" ) \n");
    }
  /*
   * initialisation du match_state
   *
   * 1 regles : 
   *   0 : A(empty,x)
   *
   * 1 pattern_in_cbg :
   *   0 : empty
   */
  necessary_link=0; // 1 niveau
  MS_init(ms, match_subterm_p1, no_pattern_p1_niv_0,
	  pattern_list_p1, no_pattern_p1_niv_1, t,
	  necessary_link, max_nb_pattern_under_p1);

  /*
   * Extraction des solutions
   */
  // MODE a definir en fonctions des regles
  mode=POSSIBLE_REST;

  indice=MS_solve(ms,mode);
  if(indice>=0)
    {
      //  ms->indice est le no du pattern qui filtre
#ifdef AFFICHAGE
      indent(); printf("Application de la regle %d\n",ms->no_rule);
#endif
      if(ms->no_rule==0)
	{
	  int i;
	  int nb_variable=1; // 1 var AC
	  struct term *substitution[1];
	  int nb_variable_ac=1;

	  substitution_build(t,ms,
			     nb_variable,substitution,
			     nb_variable_ac,
			     p1_variable_extract
			     );
#ifdef AFFICHAGE	  
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
	    res=fun_p1(substitution[0]);

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
      // p1(x,x) => x
      int *E,*sol;
      int nb_arg_subject;
      int no_arg_subject;
      int indice=0;

      struct term *tmp;
      struct cell_term *cell;
      struct cell_term *copy_cell;
      struct term *list_x;
      struct term *list_y;

#ifdef AFFICHAGE
      if(trace)
	{
	  indent();
	  printf("application de la regle p1(x,x) => x\n");
	}
#endif
      for(cell=term_first(t), nb_arg_subject=0 ;
	  cell != NULL ; cell=cell_next(cell), nb_arg_subject++)
	/* calcul de la taille du sujet */;

      // Construction du probleme 
      E=(int*)MALLOC(nb_arg_subject*sizeof(int));
      sol=(int*)MALLOC(nb_arg_subject*sizeof(int));
      // E is an array of multiplicities

#ifndef CMPOBJ
      if(0)
	{
	  for(cell=term_first(t), no_arg_subject=0 ;
	      cell != NULL ; cell=cell_next(cell), no_arg_subject++)
	    {
	      E[no_arg_subject]=cell_mult(cell);
	      sol[no_arg_subject]=0;
	    }
	  // Extraction d'une solution
	  indice=next_pe_extract(nb_arg_subject-1,E,sol,2);
	}
      else
	{
	  for(cell=term_first(t), no_arg_subject=0 ;
	      cell != NULL ; cell=cell_next(cell), no_arg_subject++)
	    {
	      E[no_arg_subject]=cell_mult(cell);
	      sol[no_arg_subject]=0;
	    }
	  // Extraction d'une solution
	  indice=next_pe_extract2(indice,nb_arg_subject-1,E,sol,2);
	}

      // Traitement d'une solution
      if(indice > 0 ) //indice>=0) pour le 1er cas
#else
	  for(cell=term_first(t), no_arg_subject=0 ;
	      cell != NULL ; cell=cell_next(cell), no_arg_subject++)
	    {
	      E[no_arg_subject]=cell_mult(cell);
	      sol[no_arg_subject]=0;
	    }
	  // Extraction d'une solution
	  indice=next_pe_extract(nb_arg_subject-1,E,sol,2);
	  if(indice >= 0 )
#endif
	{
	  int i;
#ifdef AFFICHAGE
	  indent(); printf("list_x = ");
	  for( i=0 ; i<nb_arg_subject ;  i++)
	    printf("%d ",sol[i]);
	  printf("\n");
	  // list_y == context
	  indent(); printf("context = list_y = ");
	  for( i=0 ; i<nb_arg_subject ;  i++)
	    printf("%d ",E[i]-sol[i]);
	  printf("\n");
#endif
	  extract_xy_from_pe(t,E,sol,2,&list_x,&list_y);
	  FREE(E);
	  FREE(sol);
	  //freeterm(t);

#ifdef AFFICHAGE
	  indent(); printf("list_x = ");
	  term_printnl(stdout,list_x);
	  indent(); printf("list_y = ");
	  term_printnl(stdout,list_y);
#endif
	}
      else
	{
#ifdef AFFICHAGE
	  indent(); printf("il faut retourner le terme de depart\n");
#endif
	  res=t;
	  FREE(E);
	  FREE(sol);
	  goto end_no_rewrite;
	}



#ifdef AFFICHAGE
      indent(); printf("Construction de la partie droite\n");
#endif
      // recuperation du contexte
      // Construction de la partie droite
      tmp=term_removeTopSymbol(list_x);
      //if(tmp!=NULL)
      //	list_x=tmp;
      if(tmp==NULL)
	tmp=fun_p1(list_x);
      list_x=tmp;

      if(term_first(list_y) != NULL)
	{
#ifdef AFFICHAGE
	  indent(); printf("list_x = "); term_printnl(stdout,list_x);
	  indent(); printf("list_y = "); term_printnl(stdout,list_y);
#endif
	  if(onfterm)
	    {
	      term_add_onf_term(list_y,list_x);
	    }
	  else
	    {
	      term_add_last(list_y,list_x);
	      term_flatten(list_y);
	      term_onf(list_y);
	    }

#ifdef AFFICHAGE
	   indent();
	   printf("On relance fun_p1 sur : "); term_printnl(stdout,list_y);
#endif
	   res=fun_p1(list_y);
	   goto end;
	}
      else
	{
	  if(list_y->counter == 0)
	    freeterm(list_y);
#ifdef DEBUG
	  else
	    {
	      indent(indentlevel);
	      printf("Probleme de destruction\n");
	      exit(0);
	    }
#endif
	  res=list_x;
	  goto end;
	}
    }
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
 * empty   : bit 0
 */

void p1_variable_extract(struct term *t,
			 int id_pattern,
			 struct term *extract_substitution[],
			 int *indice,
			 // Pour le 2eme niveau :
			 struct match_state *ms,
			 int no_arg_subject,
			 int no_pattern
			 )
{
  switch(id_pattern)
    {
    case 0: 
      break;
    default:
      fprintf(stderr,"p1_variable_extract : bad pattern number\n");
      exit(0);
    }
}


int match_subterm_p1(struct term *t,int no_arg_subject, bitSet *mask, BG *cbg)
{
  int nb_bit=0;
  bitSet_init_clear(mask);
  switch(t->symb)
    {
      // empty
    case code_empty:
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
