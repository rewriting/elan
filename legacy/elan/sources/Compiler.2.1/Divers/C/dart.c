#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "term.h"
#include "bitset.h"
#include "bgraph.h"
#include "match_state.h"
#include "dart.h"
#include <stdarg.h> 

#include "fun_p1.h"
#include "fun_p2.h"
#include "fun_m2.h"

//#include "back.h"

#ifdef PEANO
struct term *fun_plus();
struct term *fun_mult();
#endif

/*
 * Constantes d'execution
 */
int trace = 0;
int onfterm = 1;
long rewrite_step=0;
int indentlevel=0;

#define addindent() indentlevel++;
#define subindent() indentlevel--;

/*
 * Table des symboles
 */
fsym fsymtab[256];

/*
 * creation des constantes
 */
struct term *const_empty;

#ifdef PEANO
struct term *const_o;
#endif

long *bp_main;
main()
{
  long bp;
  struct term *t[30];
  struct term *tmp;

  BG *bgt[10];
  bitSet *mask;
  int i,j,indice;

  int bg_solution[10];
  int bg_multiplicity[10];

  bp_main=&bp;
  //backTrackInit();
  
  init_alloc();

  fsym_init(code_empty,0,"empty");

  fsym_init(code_set,1,"set");
  fsym_init(code_int,1,"int");

  fsym_init(code_p1,-1,"p1");
  fsym_init(code_p2,-1,"p2");
  fsym_init(code_m2,-1,"m2");

#ifdef PEANO
  fsym_init(code_o,0,"o");
  fsym_init(code_s,1,"s");
  fsym_init(code_plus,2,"plus");
  fsym_init(code_mult,2,"mult");
#endif

  init_pattern_list_p1();
  init_pattern_list_p2();
  init_pattern_list_p2p1();
  init_pattern_list_m2();
  init_pattern_list_m2p1();
  

  /*
   * creation des constantes
   */

  TERM_CONST_ALLOC(const_empty,code_empty);

#ifdef PEANO
  TERM_CONST_ALLOC(const_o,code_o);
#endif
  
  printf("\n\n\n**************************************************\n");
  
  /*
  {
    struct term *p1;
    struct term *set_1, *int_1;
    struct term *set_11, *int_11;
    struct term *set_2, *int_2;
    // p1(set(1),set(1),set(2),empty,empty)

    // set_1
    TERM_ALLOC(int_1,term1,freelist_term1,code_int);
    int_1->sub[0] = (struct term*) 1;
    TERM_ALLOC(set_1,term1,freelist_term1,code_set);
    set_1->sub[0] = int_1; addcounter(int_1);
    // set_11
    TERM_ALLOC(int_11,term1,freelist_term1,code_int);
    int_11->sub[0] = (struct term*) 1;
    TERM_ALLOC(set_11,term1,freelist_term1,code_set);
    set_11->sub[0] = int_11; addcounter(int_11);
    // set_2
    TERM_ALLOC(int_2,term1,freelist_term1,code_int);
    int_2->sub[0] = (struct term*)2;
    TERM_ALLOC(set_2,term1,freelist_term1,code_set);
    set_2->sub[0] = int_2; addcounter(int_2);
    // p1
    TERM_ALLOC(p1,term2,freelist_term2,code_p1); 
    term_add_onf_term(p1,const_empty);
    term_add_onf_term(p1,const_empty);
    term_add_onf_term(p1,set_1);
    term_add_onf_term(p1,set_11);
    term_add_onf_term(p1,set_2);
    term_add_onf_term(p1,set_2);
    term_add_onf_term(p1,set_2);
    term_printnl(stdout,p1);
    tmp=fun_p1(p1);
    //tmp=p1;
    goto end;
  }
  */

  /*    
  {
    struct term *p2;
    struct term *set_1, *int_1;
    struct term *set_2, *int_2;
    // p2(set(1),set(1),set(2),empty)

    // set_1
    TERM_ALLOC(int_1,term1,freelist_term1,code_int);
    int_1->sub[0] = (struct term*)1;
    TERM_ALLOC(set_1,term1,freelist_term1,code_set);
    set_1->sub[0] = int_1; addcounter(int_1);
    // set_2
    TERM_ALLOC(int_2,term1,freelist_term1,code_int);
    int_2->sub[0] = (struct term*)2;
    TERM_ALLOC(set_2,term1,freelist_term1,code_set);
    set_2->sub[0] = int_2; addcounter(int_2);
    // p2
    TERM_ALLOC(p2,term2,freelist_term2,code_p2); 
    term_add_onf_term(p2,const_empty);
    term_add_onf_term(p2,set_1);
    term_add_onf_term(p2,set_1);
    term_add_onf_term(p2,set_2);
    term_printnl(stdout,p2);
    tmp=fun_p2(p2);
  }
  */
  /*
  {
  // p2( p1(set(12),empty), p1(set(13),set(14),empty),  empty)
    struct term *p2;
    struct term *set_1, *int_1;
    struct term *set_2, *int_2;
    struct term *set_3, *int_3;
    struct term *p1_1, *p1_2;

    // p1_1
    TERM_ALLOC(int_1,term1,freelist_term1,code_int);
    int_1->sub[0] = (struct term*)12;
    TERM_ALLOC(set_1,term1,freelist_term1,code_set);
    set_1->sub[0] = int_1;
    addcounter(int_1);
    TERM_ALLOC(p1_1,term2,freelist_term2,code_p1);
    term_add_onf_term(p1_1,const_empty);
    term_add_onf_term(p1_1,set_1);
    p1_1=fun_p1(p1_1);

    // p1_2
    TERM_ALLOC(int_2,term1,freelist_term1,code_int);
    int_2->sub[0] = (struct term*)13;
    TERM_ALLOC(set_2,term1,freelist_term1,code_set);
    set_2->sub[0] = int_2;
    addcounter(int_2);
    TERM_ALLOC(int_3,term1,freelist_term1,code_int);
    int_3->sub[0] = (struct term*)14;
    TERM_ALLOC(set_3,term1,freelist_term1,code_set);
    set_3->sub[0] = int_3;
    addcounter(int_3);
    TERM_ALLOC(p1_2,term2,freelist_term2,code_p1);
    term_add_onf_term(p1_2,const_empty);
    term_add_onf_term(p1_2,set_2);
    term_add_onf_term(p1_2,set_3);
    p1_2=fun_p1(p1_2);

    // p2
    TERM_ALLOC(p2,term2,freelist_term2,code_p2); 
    term_add_onf_term(p2,const_empty);
    term_add_onf_term(p2,p1_1);
    term_add_onf_term(p2,p1_2);
    printf("term[0]="); term_printnl(stdout,p2);
    tmp=fun_p2(p2);
    goto end;
  }
  */  

#ifdef PEANO
#define SUCC(T,N) {\
    struct term *tmp_s1, *tmp_s2;\
    int j;\
    tmp_s1=const_o;\
    for(j=0 ; j<(N) ; j++)\
      {\
	TERM_ALLOC(tmp_s2,term1,freelist_term1,code_s);\
	tmp_s2->sub[0]=tmp_s1; addcounter(tmp_s1);\
	tmp_s1=tmp_s2;\
      }\
      T=tmp_s1;}
#endif

  {
  // single = p1( set(1), ..., set(25) )
    struct term *singles;
    struct term *doubles;
    struct term *triples;
    struct term *all;
    struct term *finish;
    struct term *tmp_int;
    struct term *tmp_set;
    struct term *tmp_p1;
    long i;

#ifdef PEANO
#define ADDSET(T,N) {\
    struct term *tmp_s1, *tmp_s2;\
    int j;\
    tmp_s1=const_o;\
    for(j=0 ; j<(N) ; j++)\
      {\
	TERM_ALLOC(tmp_s2,term1,freelist_term1,code_s);\
	tmp_s2->sub[0]=tmp_s1; addcounter(tmp_s1);\
	tmp_s1=tmp_s2;\
      }\
    TERM_ALLOC(tmp_set,term1,freelist_term1,code_set);\
    tmp_set->sub[0] = tmp_s1; addcounter(tmp_s1);\
    term_add_onf_term((T),tmp_set); }
#else
#define ADDSET(T,N) {\
    TERM_ALLOC(tmp_int,term1,freelist_term1,code_int);\
    tmp_int->sub[0] = (struct term*) (N);\
    TERM_ALLOC(tmp_set,term1,freelist_term1,code_set);\
    tmp_set->sub[0] = tmp_int; addcounter(tmp_int);\
    term_add_onf_term((T),tmp_set); }
#endif

    TERM_ALLOC(singles,term2,freelist_term2,code_p1);
    for(i=1 ; i<=10 ; i++)
      ADDSET(singles,i);
    addcounter(singles);

    TERM_ALLOC(doubles,term2,freelist_term2,code_m2);
    term_add_onf_term(doubles, singles);
    ADDSET(doubles, 2);
    doubles=fun_m2(doubles);
    addcounter(doubles);

    //tmp=doubles;
    //goto end;

    TERM_ALLOC(triples,term2,freelist_term2,code_m2);
    term_add_onf_term(triples, singles);
    ADDSET(triples, 3);
    triples=fun_m2(triples);
    addcounter(triples);

    //tmp=triples;
    //goto end;


    TERM_ALLOC(all,term2,freelist_term2,code_p1);
    term_add_onf_term(all, singles);
    term_add_onf_term(all, doubles);
    term_add_onf_term(all, triples);
    ADDSET(all, 0);
    ADDSET(all, 25);
    ADDSET(all, 50);
    all=fun_p1(all);
    addcounter(all);

    //tmp=all;
    //goto end;

    TERM_ALLOC(finish,term2,freelist_term2,code_p2);
    TERM_ALLOC(tmp_p1,term2,freelist_term2,code_p1);
    term_add_onf_term(tmp_p1, doubles);
    ADDSET(tmp_p1, 50);
    tmp_p1=fun_p1(tmp_p1);

    term_add_onf_term(finish, tmp_p1);
    term_add_onf_term(finish, all);
    term_add_onf_term(finish, all);
    //term_add_onf_term(finish, all);
    //term_add_onf_term(finish, all);
    //term_add_onf_term(finish, all);
    finish=fun_p2(finish);
    addcounter(finish);

    tmp=finish;
    
    subcounter(singles); 
    if(singles->counter == 0) freeterm(singles); 

    subcounter(doubles); 
    if(doubles->counter == 0) freeterm(doubles);

    subcounter(triples);
    if(triples->counter == 0) freeterm(triples);

    subcounter(all);
    if(all->counter == 0) freeterm(all);

    subcounter(finish);

  }

end:

  printf("result[%d] = ",rewrite_step);
  term_printnl(stdout,tmp);
destruction:
  freeterm(tmp);

  FREE(fsymtab[code_empty].name);
  FREE(fsymtab[code_set].name);
  FREE(fsymtab[code_int].name);
  FREE(fsymtab[code_p1].name);
  FREE(fsymtab[code_p2].name);
  FREE(fsymtab[code_m2].name);
  TERM_FREE(const_empty);

#ifdef PEANO
  FREE(fsymtab[code_o].name);
  FREE(fsymtab[code_s].name);
  FREE(fsymtab[code_plus].name);
  FREE(fsymtab[code_mult].name);
  TERM_FREE(const_o);
#endif

  delete_pattern_list_p1();
  delete_pattern_list_p2();
  delete_pattern_list_p2p1();
  delete_pattern_list_m2();
  delete_pattern_list_m2p1();

#ifdef DEBUG
  print_space_usage();
#endif 

  printf("rewrite_step = %d\n",rewrite_step);
  //exit(0);
}

#ifdef PEANO
/* ------------------------------------------------------------ */
/* fun_plus et fun_mult                                         */
/* ------------------------------------------------------------ */
struct term *fun_plus(struct term *t1, struct term *t2)
{
  struct term *v1, *v2, *v3, *res;
  unsigned long ok;

  addindent();

#ifdef DEBUG
  if(trace)
    {
      indent(indentlevel);
      printf("fun_plus( ");
      term_print(stdout,t1);
      printf(" , ");
      term_print(stdout,t2);
      printf(" )\n");
    }
#endif

  ok=0xffffffff;
  ok &= 03;
  switch(t2->symb)
    {
    case code_o:
      ok &= 02;
      break;
    case code_s:
      v3=t2->sub[0];
      ok &= 01;
      break;
    default:
      goto match_fail;
    }
  // plus(x,s(y)) => s(plus(x,y))
  if (ok & 01) 
    {
      TERM_ALLOC(res,term1,freelist_term1,code_s);
      v2 =  fun_plus(t1,v3);
      res->sub[0] = v2;
      addcounter(v2);

      if(t1->counter == 0)
	{
	  freeterm(t1);
	}
      if(t2->counter == 0)
	{
	  freeterm(t2);
	}
      goto end;
    }
  // plus(x,o) => x
  if (ok & 02)
    {
      res=t1;
      if(t2->counter == 0)
	{
	  freeterm(t2);
	}
      goto end;
    }
  
match_fail:
  TERM_ALLOC(res,term2,freelist_term2,code_plus);
  res->sub[0] = t1;
  res->sub[1] = t2;
  goto end_no_rewrite;

end:  
  rewrite_step++;
end_no_rewrite:
  if(trace)
    {
      indent(indentlevel);
      printf("rewrite[%d] ",rewrite_step);
      term_printnl(stdout,res);
    }
  subindent();
  return res;
}


struct term *fun_mult(struct term *t1, struct term *t2)
{
  struct term *v1, *v2, *v3, *res;
  unsigned long ok;

  addindent();

#ifdef DEBUG
  if(trace)
    {
      indent(indentlevel);
      printf("fun_mult( ");
      term_print(stdout,t1);
      printf(" , ");
      term_print(stdout,t2);
      printf(" )\n");
    }
#endif

  ok=0xffffffff;
  ok &= 03;
  switch(t2->symb)
    {
    case code_o:
      ok &= 02;
      break;
    case code_s:
      v3=t2->sub[0];
      ok &= 01;
      break;
    default:
      goto match_fail;
    }
  // mult(x,s(y)) => plus(mult(x,y),x)
  if (ok & 01) 
    {
      addcounter(t1);
      v1  = fun_mult(t1,v3);
      res = fun_plus(v1,t1);
      subcounter(t1);

      if(t1->counter == 0)
	{
	  freeterm(t1);
	}
      if(t2->counter == 0)
	{
	  freeterm(t2);
	}
      goto end;
    }
  // mult(x,o)  => o 
  if (ok & 02)
    {
      res=t2;

      if(t1->counter == 0)
	{
	  freeterm(t1);
	}
      goto end;
    }
  
match_fail:
  TERM_ALLOC(res,term2,freelist_term2,code_mult);
  res->sub[0] = t1;
  res->sub[1] = t2;
  goto end_no_rewrite;

end:  
  rewrite_step++;
end_no_rewrite:
  if(trace)
    {
      indent(indentlevel);
      printf("rewrite[%d] ",rewrite_step);
      term_printnl(stdout,res);
    }
  subindent();
  return res;
}

#endif


/* ------------------------------------------------------------ */
/* Outils                                                       */
/* ------------------------------------------------------------ */

void substitution_build(struct term *t, match_state *ms,
			int nb_variable, struct term *substitution[],
			int nb_variable_ac,
			void (*variable_extract)()
			)
{
  va_list argv;
  int i,j,no_pattern;

  struct term *subject[MAX_TERM_SIZE];
  struct term *list_x;
  struct cell_term *cell;
  struct cell_term *copy_cell;
  int *pattern_list;
  int indice;

  /*
   * si le terme est partage
   */
  if(1 || t->counter > 0)
    {
      TERM_ALLOC(list_x,term2,freelist_term2,t->symb);
      for(cell=term_first(t),i=0 ; cell != NULL ; cell=cell_next(cell),i++)
	{
	  // mise du sujet dans un tableau
#ifdef DEBUG
	  if(i >= MAX_TERM_SIZE)
	    {
	      term_printnl(stdout,t);
	      fprintf(stderr,"subject too big\n");
	      fprintf(stderr,"substitution_build : i >= MAX_TERM_SIZE\n");
	      exit(1);
	    }
#endif
	  subject[i]=cell_t(cell);

	  if(nb_variable_ac == 1 && ms->bg_multiplicity[i] != 0)
	    {
#ifdef AFFICHAGE
	      indent(indentlevel);
	      term_print(stdout,cell_t(cell));
	      printf("\t--> copier dans list_x\n");
#endif
	      copy_cell=cell_create();
	      cell_t(copy_cell)=cell_t(cell);
	      cell_mult(copy_cell)=ms->bg_multiplicity[i];
	      cell_t(cell)->counter++;
	      /* ajout dans list_x */
	      cell_add_last(copy_cell,list_x);
	    }
	}

      indice=0;
      // sauvegarde du reste
      if(nb_variable_ac == 1)
	{
	  substitution[0] = list_x;
	  addcounter(substitution[0]);
	  indice++;
	}
      
      //printf("ms->no_rule=%d\n",ms->no_rule);
      pattern_list=MS_get_pattern_list(ms,ms->no_rule);
      for(no_pattern=0 ; no_pattern < BG_size(ms->bg) ; no_pattern++)
	 {
	   int no_arg_subject;
	   int id_pattern;

	   no_arg_subject=ms->bg_solution[no_pattern];
	   id_pattern=pattern_list[no_pattern];
	   variable_extract(subject[no_arg_subject],
			    id_pattern,
			    substitution,&indice,
			    // Pour le 2eme niveau :
			    ms,
			    no_arg_subject,
			    no_pattern
			    );
			       
	   
   
	 }

    }

  //printf("t->counter = %d\n",t->counter);
  if(t->counter == 0)
    freeterm(t);

}


struct term *rest_extract(struct term *t, match_state *ms)
{
  int i;
  struct term *list_x;
  struct cell_term *cell;
  struct cell_term *copy_cell;

  /*
   * si le terme est partage
   */
  if(t->counter > 0)
    {
      TERM_ALLOC(list_x,term2,freelist_term2,t->symb);
      for(cell=term_first(t),i=0 ; cell != NULL ; cell=cell_next(cell),i++)
	{
	  if(ms->bg_multiplicity[i] != 0)
	    {
#ifdef AFFICHAGE
	      indent(indentlevel);
	      term_print(stdout,cell_t(cell));
	      printf("\t--> copier dans list_x\n");
#endif
	      copy_cell=cell_create();
	      cell_t(copy_cell)=cell_t(cell);
	      cell_mult(copy_cell)=ms->bg_multiplicity[i];
	      cell_t(cell)->counter++;
	      /* ajout dans list_x */
	      cell_add_last(copy_cell,list_x);
	    }
	}
      // ici : t->counter > 0
      //freeterm(t);
      return list_x;
    }
  else
    {
      struct cell_term *last_cell;
      struct cell_term *next_cell;

#ifdef AFFICHAGE
      indent(indentlevel); printf("pas de partage\n");
      indent(indentlevel); term_printnl(stdout,t);
#endif

      for(cell=term_first(t), last_cell=term_first(t), i=0 ;
	  cell != NULL ; cell=next_cell, i++)
	{
	  next_cell=cell_next(cell);

	  //printf("last_cell=%d\n",last_cell);
	  //printf("cell     =%d\n",cell);
	  //printf("next_cell=%d\n",next_cell);
	  //term_print(stdout,cell_t(cell));

	  if(ms->bg_multiplicity[i] != 0)
	    {
	      //printf("\t modif de la multiplicite\n");
	      cell_mult(cell)=ms->bg_multiplicity[i];
	      last_cell=cell;
	    }
	  else
	    {
	      //printf("\t destruction\n");
	      // destruction de la cellule
	      cell_delete(last_cell,cell,t);
	      subcounter(cell_t(cell));
	      if(cell_t(cell)->counter == 0)
		freeterm(cell_t(cell));
	      cell_free(cell);
	    }
	}
      return t;
    }
}

void extract_xy_from_pe(struct term *t, int *E, int *sol, int multiplicity,
			struct term **ptr_list_x,struct term **ptr_list_y)
{
  int i;
  struct cell_term *cell;
  struct term *list_x;
  struct term *list_y;

  if(t->counter > 0)
    {
      TERM_ALLOC(*ptr_list_x,term2,freelist_term2,t->symb);
      TERM_ALLOC(*ptr_list_y,term2,freelist_term2,t->symb);
      list_x=*ptr_list_x;
      list_y=*ptr_list_y;
      for(cell=term_first(t),i=0 ; cell!=NULL ; cell=cell_next(cell),i++)
	{
	  if(sol[i] != 0)
	    {
#ifdef AFFICHAGE
	      indent(indentlevel);
	      term_print(stdout,cell_t(cell));
	      printf("\t--> copier dans list_x\n");
#endif
	      term_add_last(list_x,cell_t(cell));
	      cell_mult(term_last(list_x))=sol[i]/multiplicity;
	    }
	  if(E[i]-sol[i] != 0)
	    {
#ifdef AFFICHAGE
	      indent(indentlevel);
	      term_print(stdout,cell_t(cell));
	      printf("\t--> copier dans list_y\n");
#endif
	      term_add_last(list_y,cell_t(cell));
	      cell_mult(term_last(list_y))=E[i]-sol[i];
	    }
	}
      // ici :t->counter > 0
      //freeterm(t);
    }
  else
    {
      struct cell_term *last_cell;
      struct cell_term *next_cell;
    
      //printf("extract_xy_from_pe : pas de partage\n");
      //term_printnl(stdout,t);

      *ptr_list_x=t;
      TERM_ALLOC(*ptr_list_y,term2,freelist_term2,t->symb);
      list_y=*ptr_list_y;
      for(cell=term_first(t), last_cell=term_first(t), i=0 ;
	  cell != NULL ; cell=next_cell, i++)
	{
	  next_cell=cell_next(cell);
	  
	  //printf("last_cell=%d\n",last_cell);
	  //printf("cell     =%d\n",cell);
	  //printf("next_cell=%d\n",next_cell);
	  //term_print(stdout,cell_t(cell));

	  if((sol[i]!=0) && (E[i]-sol[i]==0))
	    {
	      //printf("\t modifier list_x sans toucher list_y\n");
	      cell_mult(cell)=sol[i]/multiplicity;
	      last_cell=cell;
	    }
	  else if((sol[i]!=0) && (E[i]-sol[i]!=0))
	    {
	      //printf("\t modifier list_x et copier dans list_y\n");
	      cell_mult(cell)=sol[i]/multiplicity;
	      term_add_last(list_y,cell_t(cell));
	      cell_mult(term_last(list_y))=E[i]-sol[i];
	      last_cell=cell;
	    }
	  else if((sol[i]==0) && (E[i]-sol[i]==0))
	    {
	      //printf("\t detruire la cellule\n");
	      cell_delete(last_cell,cell,t);
	      subcounter(cell_t(cell));
	      if(cell_t(cell)->counter == 0)
		freeterm(cell_t(cell));
	      cell_free(cell);
	    }
	  else if((sol[i]==0) && (E[i]-sol[i]!=0))
	    {
	      //printf("\t deplacer dans list_y\n");
	      cell_delete(last_cell,cell,t);
	      cell_next(cell)=NULL;
	      cell_mult(cell)=E[i]-sol[i];
	      cell_add_last(cell,list_y);
	    }
	}
    }
}

int next_pe_extract(int pos,int E[], int sol[], int multiplicity)
{
  if(pos<0)
    {
#ifdef AFFICHAGE
      //      printf("Plus de solution\n");
#endif
      return -1;
    }
  sol[pos]+=multiplicity;
  if(sol[pos]<=E[pos])
      return pos;
  /* il faut propager la retenue */
  sol[pos]=0;
  return next_pe_extract(pos-1,E,sol,multiplicity);
}

int next_pe_extract2(int total, int pos,int E[], int sol[], int multiplicity)
{
  int i;
  if(pos<0)
    {
      return -1;
    }
  if(total == 0)
    {
      for(i=0 ; i<=pos ; i++)
	{
	  sol[i]=E[i] - (E[i]%multiplicity);
	  total+=sol[i];
	}
      return total;
    }
  sol[pos]-=multiplicity;
  total-=multiplicity;
  if(sol[pos] >= 0)
      return total;
  /* il faut propager la retenue */
  total-=sol[pos];
  sol[pos] = E[pos] - (E[pos]%multiplicity);;
  total+=sol[pos];
  return next_pe_extract2(total,pos-1,E,sol,multiplicity);
}
