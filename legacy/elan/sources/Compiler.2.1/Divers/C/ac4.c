#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "term.h"
#include "bitset.h"
#include "bgraph.h"
#include "match_state.h"
#include "ac4.h"
#include "back.h"
#include "builtin.h"

/* Constantes d'execution */
int trace = 1;
long rewrite_step=0;
int indentlevel=0;

/* Table des symboles */
fsym fsymtab[256];
/* Declaration des pattern_list */
void init_pattern_list_71_dk_40();
void delete_pattern_list_71_dk_40();
void init_pattern_list_70_dk_40();
void delete_pattern_list_70_dk_40();

/* Constantes */
struct term *con_18;
struct term *con_17;
struct term *con_16;
struct term *con_15;
struct term *con_14;
struct term *con_13;
struct term *con_12;
struct term *con_11;
struct term *con_69;
struct term *con_68;
struct term *con_67;
struct term *con_66;
struct term *con_65;
struct term *con_64;
struct term *con_2;
struct term *con_63;
struct term *con_1;
struct term *con_62;
struct term *con_61;
struct term *con_60;
struct term *con_75;
struct term *con_74;
struct term *con_73;
struct term *con_72;
struct term *con_71;
struct term *con_70;

/* Procedure principale */
long *bp_main;
main() {
  long bp;
  struct term *res;
  bp_main=&bp;
  backTrackInit();
  init_alloc();
  fsym_init(code_44,1,"not");
  fsym_init(code_43,2,"or");
  fsym_init(code_42,2,"and");
  fsym_init(code_41,-1,"p");
  fsym_init(code_40,-1,"m");
  fsym_init(code_18,0,"a8");
  fsym_init(code_17,0,"a7");
  fsym_init(code_16,0,"a6");
  fsym_init(code_15,0,"a5");
  fsym_init(code_14,0,"a4");
  fsym_init(code_13,0,"a3");
  fsym_init(code_12,0,"a2");
  fsym_init(code_11,0,"a1");
  fsym_init(code_69,0,"r9");
  fsym_init(code_68,0,"r8");
  fsym_init(code_67,0,"r7");
  fsym_init(code_66,0,"r6");
  fsym_init(code_65,0,"r5");
  fsym_init(code_3,2,"eq");
  fsym_init(code_64,0,"r4");
  fsym_init(code_2,0,"false");
  fsym_init(code_63,0,"r3");
  fsym_init(code_1,0,"true");
  fsym_init(code_62,0,"r2");
  fsym_init(code_61,0,"r1");
  fsym_init(code_60,0,"r0");
  fsym_init(code_75,0,"s5");
  fsym_init(code_74,0,"s4");
  fsym_init(code_73,0,"s3");
  fsym_init(code_72,0,"s2");
  fsym_init(code_71,0,"s1");
  fsym_init(code_70,0,"s0");
  fsym_init(code_45,1,"f");
  
/* Initialidsation des constantes */
  TERM_CONST_ALLOC(con_18, code_18);
  TERM_CONST_ALLOC(con_17, code_17);
  TERM_CONST_ALLOC(con_16, code_16);
  TERM_CONST_ALLOC(con_15, code_15);
  TERM_CONST_ALLOC(con_14, code_14);
  TERM_CONST_ALLOC(con_13, code_13);
  TERM_CONST_ALLOC(con_12, code_12);
  TERM_CONST_ALLOC(con_11, code_11);
  TERM_CONST_ALLOC(con_69, code_69);
  TERM_CONST_ALLOC(con_68, code_68);
  TERM_CONST_ALLOC(con_67, code_67);
  TERM_CONST_ALLOC(con_66, code_66);
  TERM_CONST_ALLOC(con_65, code_65);
  TERM_CONST_ALLOC(con_64, code_64);
  TERM_CONST_ALLOC(con_2, code_2);
  TERM_CONST_ALLOC(con_63, code_63);
  TERM_CONST_ALLOC(con_1, code_1);
  TERM_CONST_ALLOC(con_62, code_62);
  TERM_CONST_ALLOC(con_61, code_61);
  TERM_CONST_ALLOC(con_60, code_60);
  TERM_CONST_ALLOC(con_75, code_75);
  TERM_CONST_ALLOC(con_74, code_74);
  TERM_CONST_ALLOC(con_73, code_73);
  TERM_CONST_ALLOC(con_72, code_72);
  TERM_CONST_ALLOC(con_71, code_71);
  TERM_CONST_ALLOC(con_70, code_70);
  /* Initialisation des pattern_list */
  init_pattern_list_71_dk_40();
  init_pattern_list_70_dk_40();

  if (!setChoicePoint()) {
    /* TERME DE DEPART */
    {
      /* f[var=5, subst=-1, NOSHARE](f[var=4, subst=-1, NOSHARE](f[var=3, subst=-1, NOSHARE](f[var=2, subst=-1, NOSHARE](f[var=1, subst=-1, NOSHARE](a1[var=0, subst=-1, NOSHARE]))))) */
      struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5;
      TERM_ALLOC(sv1,term1,NULL,code_45);
      sv1->sub[0] = con_11;
      TERM_ALLOC(sv2,term1,NULL,code_45);
      sv2->sub[0] = sv1;
      TERM_ALLOC(sv3,term1,NULL,code_45);
      sv3->sub[0] = sv2;
      TERM_ALLOC(sv4,term1,NULL,code_45);
      sv4->sub[0] = sv3;
      TERM_ALLOC(sv5,term1,NULL,code_45);
      sv5->sub[0] = sv4;
      res=str_74(sv5);
    }
    printf("\nresult[%d] = ",rewrite_step);
    term_printnl(stdout,res);
    fail();
  }
end:
destruction:
  FREE(fsymtab[code_44].name);
  FREE(fsymtab[code_43].name);
  FREE(fsymtab[code_42].name);
  FREE(fsymtab[code_41].name);
  FREE(fsymtab[code_40].name);
  FREE(fsymtab[code_18].name);
  FREE(fsymtab[code_17].name);
  FREE(fsymtab[code_16].name);
  FREE(fsymtab[code_15].name);
  FREE(fsymtab[code_14].name);
  FREE(fsymtab[code_13].name);
  FREE(fsymtab[code_12].name);
  FREE(fsymtab[code_11].name);
  FREE(fsymtab[code_69].name);
  FREE(fsymtab[code_68].name);
  FREE(fsymtab[code_67].name);
  FREE(fsymtab[code_66].name);
  FREE(fsymtab[code_65].name);
  FREE(fsymtab[code_3].name);
  FREE(fsymtab[code_64].name);
  FREE(fsymtab[code_2].name);
  FREE(fsymtab[code_63].name);
  FREE(fsymtab[code_1].name);
  FREE(fsymtab[code_62].name);
  FREE(fsymtab[code_61].name);
  FREE(fsymtab[code_60].name);
  FREE(fsymtab[code_75].name);
  FREE(fsymtab[code_74].name);
  FREE(fsymtab[code_73].name);
  FREE(fsymtab[code_72].name);
  FREE(fsymtab[code_71].name);
  FREE(fsymtab[code_70].name);
  FREE(fsymtab[code_45].name);
  TERM_FREE(con_18);
  TERM_FREE(con_17);
  TERM_FREE(con_16);
  TERM_FREE(con_15);
  TERM_FREE(con_14);
  TERM_FREE(con_13);
  TERM_FREE(con_12);
  TERM_FREE(con_11);
  TERM_FREE(con_69);
  TERM_FREE(con_68);
  TERM_FREE(con_67);
  TERM_FREE(con_66);
  TERM_FREE(con_65);
  TERM_FREE(con_64);
  TERM_FREE(con_2);
  TERM_FREE(con_63);
  TERM_FREE(con_1);
  TERM_FREE(con_62);
  TERM_FREE(con_61);
  TERM_FREE(con_60);
  TERM_FREE(con_75);
  TERM_FREE(con_74);
  TERM_FREE(con_73);
  TERM_FREE(con_72);
  TERM_FREE(con_71);
  TERM_FREE(con_70);
  /* Destruction des pattern_list */
  delete_pattern_list_71_dk_40();
  delete_pattern_list_70_dk_40();
#ifdef DEBUG
  print_space_usage();
#endif
  printf("\nrewrite_step = %d\n",rewrite_step);
}

struct term* str_74( struct term *v0 ) {
  bitSet *mask;
  struct term *res=v0;
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    term_printnl(stdout,v0);
  }
  {
    /* repeat(dc(r8)) */
    struct term* *wasr=(struct term**) allocStable(sizeof(struct term*));
    *wasr=v0;
    if(setChoicePoint()) {
      res=*wasr;
    } else {
      while(1) {
        {
          /* dc(r8) */
          struct term *v1,*v2;
          bitSet_create(mask,1);
          bitSet_init_clear(mask);
          addindent();
          switch(v0->symb) {
            case code_45:
              v1=v0->sub[0];
              switch(v1->symb) {
                default:
                  bitSet_set(mask,0);
              }
            break;
          }
          if(bitSet_get(mask,0)) {
            /* lhs: f[var=0, subst=-1, NOSHARE](x[mv][var=1, subst=-1, NOSHARE, 1]) */
            struct term *tmp, *sv0, *sv1;
            /* rhs: x[var=-1, subst=-1, PERFECTSHARE] */
            res = v1 ;
            goto stratLab0;
            myend1:
          }
        }
        fail();
        stratLab0:
        v0=res;
        *wasr=res;
      }
    }
  }
end:
  rewrite_step++;
end_no_rewrite:
  if(trace>=1) {
    indent(indentlevel);
    printf("rewrite[%d] ",rewrite_step);
    term_printnl(stdout,res);
  }
  subindent();
  return res;
}

struct term* str_73( struct term *v0 ) {
  bitSet *mask;
  struct term *res=v0;
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    term_printnl(stdout,v0);
  }
  {
    /* dk(r7) */
    struct term *v1;
    bitSet_create(mask,4);
    bitSet_init_clear(mask);
    addindent();
    switch(v0->symb) {
      default:
        bitSet_set(mask,0);
        bitSet_set(mask,1);
        bitSet_set(mask,2);
        bitSet_set(mask,3);
    }
    if(bitSet_get(mask,0)) {
      /* lhs: x[var=0, subst=-1, NOSHARE] */
      struct term *tmp, *sv0;
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend1;
      }
      /* rhs: a1[var=0, subst=-1, NOSHARE] */
      res = con_11 ;
      goto stratLab0;
      myend1:
    }
    if(bitSet_get(mask,1)) {
      /* lhs: x[var=0, subst=-1, NOSHARE] */
      struct term *tmp, *sv0;
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend2;
      }
      /* rhs: a2[var=0, subst=-1, NOSHARE] */
      res = con_12 ;
      goto stratLab0;
      myend2:
    }
    if(bitSet_get(mask,2)) {
      /* lhs: x[var=0, subst=-1, NOSHARE] */
      struct term *tmp, *sv0;
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend3;
      }
      /* rhs: a3[var=0, subst=-1, NOSHARE] */
      res = con_13 ;
      goto stratLab0;
      myend3:
    }
    if(bitSet_get(mask,3)) {
      /* lhs: x[var=0, subst=-1, NOSHARE] */
      struct term *tmp, *sv0;
      /* rhs: a4[var=0, subst=-1, NOSHARE] */
      res = con_14 ;
      goto stratLab0;
      myend4:
    }
  }
  fail();
  stratLab0:
  v0=res;
end:
  rewrite_step++;
end_no_rewrite:
  if(trace>=1) {
    indent(indentlevel);
    printf("rewrite[%d] ",rewrite_step);
    term_printnl(stdout,res);
  }
  subindent();
  return res;
}

struct term* str_72( struct term *v0 ) {
  bitSet *mask;
  struct term *res=v0;
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    term_printnl(stdout,v0);
  }
  {
    /* DC(dk(r1),dk(r2),dk(r3,r4)) */
    int *wasr=(int*) allocStable(sizeof(int));
    *wasr=0;
    if(!setChoicePoint()) {
    /* Si la strategie suivante echoue, on passe a la suivante car *wasr==0 */
      {
        /* dk(r1) */
        struct term *v1;
        bitSet_create(mask,1);
        bitSet_init_clear(mask);
        addindent();
        switch(v0->symb) {
          case code_11:
            bitSet_set(mask,0);
          break;
        }
        if(bitSet_get(mask,0)) {
          /* lhs: a1[var=0, subst=-1, NOSHARE] */
          struct term *tmp, *sv0;
          /* rhs: a3[var=0, subst=-1, NOSHARE] */
          res = con_13 ;
          goto stratLab1;
          myend2:
        }
      }
      fail();
      stratLab1:
      v0=res;
      /* La strategie a donne un resultat */
      if(*wasr==0) {
        *wasr=1;
      }
      goto stratLab0;
    }
    /* On vient d'un fail */
    if(*wasr!=0) {
      /* Si on a un resultat on propage le fail */
      fail();
    }
    /* Sinon on essai la strategie suivante */
    if(!setChoicePoint()) {
    /* Si la strategie suivante echoue, on passe a la suivante car *wasr==0 */
      {
        /* dk(r2) */
        struct term *v1;
        bitSet_create(mask,1);
        bitSet_init_clear(mask);
        addindent();
        switch(v0->symb) {
          case code_11:
            bitSet_set(mask,0);
          break;
        }
        if(bitSet_get(mask,0)) {
          /* lhs: a1[var=0, subst=-1, NOSHARE] */
          struct term *tmp, *sv0;
          /* rhs: a4[var=0, subst=-1, NOSHARE] */
          res = con_14 ;
          goto stratLab3;
          myend4:
        }
      }
      fail();
      stratLab3:
      v0=res;
      /* La strategie a donne un resultat */
      if(*wasr==0) {
        *wasr=1;
      }
      goto stratLab0;
    }
    /* On vient d'un fail */
    if(*wasr!=0) {
      /* Si on a un resultat on propage le fail */
      fail();
    }
    /* Sinon on essai la strategie suivante */
    {
      /* dk(r3,r4) */
      struct term *v1;
      bitSet_create(mask,2);
      bitSet_init_clear(mask);
      addindent();
      switch(v0->symb) {
        case code_12:
          bitSet_set(mask,0);
          bitSet_set(mask,1);
        break;
      }
      if(bitSet_get(mask,0)) {
        /* lhs: a2[var=0, subst=-1, NOSHARE] */
        struct term *tmp, *sv0;
        if(setChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend6;
        }
        /* rhs: a5[var=0, subst=-1, NOSHARE] */
        res = con_15 ;
        goto stratLab5;
        myend6:
      }
      if(bitSet_get(mask,1)) {
        /* lhs: a2[var=0, subst=-1, NOSHARE] */
        struct term *tmp, *sv0;
        /* rhs: a6[var=0, subst=-1, NOSHARE] */
        res = con_16 ;
        goto stratLab5;
        myend7:
      }
    }
    fail();
    stratLab5:
    v0=res;
    goto stratLab0;
  stratLab0:
  }
end:
  rewrite_step++;
end_no_rewrite:
  if(trace>=1) {
    indent(indentlevel);
    printf("rewrite[%d] ",rewrite_step);
    term_printnl(stdout,res);
  }
  subindent();
  return res;
}
int match_subterm_71_dk_40(struct term *v0,int no_arg_subject, bitSet *mask, BG *cbg);
void variable_extract_71_dk_40(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern);
static **pattern_list_71_dk_40;
static int no_pattern_71_dk_40_niv_0;
static int nb_pattern_71_dk_40_niv_0 = 1;
static int nb_pattern_71_dk_40_niv_1 = 1;
#define max_nb_pattern_under_71_dk_40 1

int match_subterm_71_dk_40(struct term *v0,int no_arg_subject,bitSet *mask,BG *cbg) {
      struct term *v1,*v2;
      int nb_bit=0;
      bitSet_init_clear(mask);
      switch(v0->symb) {
        case code_45:
          v1=v0->sub[0];
          switch(v1->symb) {
            default:
              bitSet_set(mask,0);
          }
        break;
      }
      return nb_bit;
}
    
void variable_extract_71_dk_40(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern) {
      switch(id_pattern) {
        /* f[var=0, subst=-1, NOSHARE](y[mv][var=1, subst=-1, NOSHARE]) */
      case 0:
        extract_substitution[*indice]=v0->sub[0];
        (*indice)++;
        break;
      default:
        fprintf(stderr,"variable_extract_71_dk_40: bad pattern number\n");
        exit(0);
      }
    }
    
void init_pattern_list_71_dk_40() {
      int pattern_tab[max_nb_pattern_under_71_dk_40];
      no_pattern_71_dk_40_niv_0=0;
      pattern_list_71_dk_40=MS_pattern_list_create(nb_pattern_71_dk_40_niv_0);
      /* m[var=0, subst=-1, NOSHARE](x[var=0, subst=-1, NOSHARE],f[var=0, subst=-1, NOSHARE](y[mv][var=1, subst=-1, NOSHARE])) */
      pattern_tab[0]=0;
      MS_pattern_list_init(pattern_list_71_dk_40,no_pattern_71_dk_40_niv_0++,1,pattern_tab);
    }
    
void delete_pattern_list_71_dk_40() {
      MS_pattern_list_free(pattern_list_71_dk_40,no_pattern_71_dk_40_niv_0);
    }

struct term* str_71( struct term *v0 ) {
  bitSet *mask;
  struct term *res=v0;
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    term_printnl(stdout,v0);
  }
  {
    /* dk(r6) */
    struct term *v1;
    match_state *ms=NULL;
    int mode;
    int necessary_link;
    int indice=-1;
    int ACPattern=0;
    bitSet_create(mask,1);
    bitSet_init_clear(mask);
    addindent();
    switch(v0->symb) {
      case code_40:
        bitSet_set(mask,0);
      break;
    }
    if( bitSet_get(mask,0) ) {
      ACPattern=1;
      /* Begin AC matching */
      necessary_link=0;
      mode=POSSIBLE_REST;
      ms=MS_create();
      MS_init(ms, match_subterm_71_dk_40, no_pattern_71_dk_40_niv_0, pattern_list_71_dk_40, nb_pattern_71_dk_40_niv_1, v0, necessary_link, max_nb_pattern_under_71_dk_40);
      /* End AC matching */
    }
    if(ACPattern && ms!=NULL) {
      start:
      indice = MS_solve(ms,mode);
      printf("\nsetChoicePoint\n");
      /* choicePoint AC matching */
      if(indice>=0 && setChoicePoint()) {
        printf("on revient d'un fail\n");
        goto start;
      }
    } else {
      indice = 0;
    }
    if(bitSet_get(mask,0)) {
      if(indice>=0 && ms->no_rule==0) {
        int nb_variable=2;
        int nb_variable_ac=1;
        struct term *substitution[2];
        int i;
        /* [r6] m[var=0, subst=-1, NOSHARE](x[var=0, subst=0, NOSHARE],f[var=0, subst=-1, NOSHARE](y[var=1, subst=1, NOSHARE, 1])) => f[var=2, subst=-1, NOSHARE](y[var=-1, subst=-1, PERFECTSHARE]) */
        struct term *tmp, *sv0, *sv1, *sv2;
        substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_71_dk_40);
        sv1=substitution[1];
        TERM_ALLOC(sv2,term1,NULL,code_45);
        sv2->sub[0] = sv1;
        res = sv2 ;
        goto stratLab0;
      }
      myend1:
    }
  }
  fail();
  stratLab0:
  v0=res;
end:
  rewrite_step++;
end_no_rewrite:
  if(trace>=1) {
    indent(indentlevel);
    printf("rewrite[%d] ",rewrite_step);
    term_printnl(stdout,res);
  }
  subindent();
  return res;
}
int match_subterm_70_dk_40(struct term *v0,int no_arg_subject, bitSet *mask, BG *cbg);
void variable_extract_70_dk_40(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern);
static **pattern_list_70_dk_40;
static int no_pattern_70_dk_40_niv_0;
static int nb_pattern_70_dk_40_niv_0 = 1;
static int nb_pattern_70_dk_40_niv_1 = 1;
#define max_nb_pattern_under_70_dk_40 1

int match_subterm_70_dk_40(struct term *v0,int no_arg_subject,bitSet *mask,BG *cbg) {
      struct term *v1,*v2;
      int nb_bit=0;
      bitSet_init_clear(mask);
      switch(v0->symb) {
        case code_45:
          v1=v0->sub[0];
          switch(v1->symb) {
            default:
              bitSet_set(mask,0);
          }
        break;
      }
      return nb_bit;
}
    
void variable_extract_70_dk_40(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern) {
      switch(id_pattern) {
        /* f[var=0, subst=-1, NOSHARE](y[mv][var=1, subst=-1, NOSHARE]) */
      case 0:
        extract_substitution[*indice]=v0->sub[0];
        (*indice)++;
        break;
      default:
        fprintf(stderr,"variable_extract_70_dk_40: bad pattern number\n");
        exit(0);
      }
    }
    
void init_pattern_list_70_dk_40() {
      int pattern_tab[max_nb_pattern_under_70_dk_40];
      no_pattern_70_dk_40_niv_0=0;
      pattern_list_70_dk_40=MS_pattern_list_create(nb_pattern_70_dk_40_niv_0);
      /* m[var=0, subst=-1, NOSHARE](x[var=0, subst=-1, NOSHARE],f[var=0, subst=-1, NOSHARE](y[mv][var=1, subst=-1, NOSHARE])) */
      pattern_tab[0]=0;
      MS_pattern_list_init(pattern_list_70_dk_40,no_pattern_70_dk_40_niv_0++,1,pattern_tab);
    }
    
void delete_pattern_list_70_dk_40() {
      MS_pattern_list_free(pattern_list_70_dk_40,no_pattern_70_dk_40_niv_0);
    }

struct term* str_70( struct term *v0 ) {
  bitSet *mask;
  struct term *res=v0;
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    term_printnl(stdout,v0);
  }
  {
    /* dk(r0) */
    struct term *v1;
    match_state *ms=NULL;
    int mode;
    int necessary_link;
    int indice=-1;
    int ACPattern=0;
    bitSet_create(mask,1);
    bitSet_init_clear(mask);
    addindent();
    switch(v0->symb) {
      case code_40:
        bitSet_set(mask,0);
      break;
    }
    if( bitSet_get(mask,0) ) {
      ACPattern=1;
      /* Begin AC matching */
      necessary_link=0;
      mode=POSSIBLE_REST;
      ms=MS_create();
      MS_init(ms, match_subterm_70_dk_40, no_pattern_70_dk_40_niv_0, pattern_list_70_dk_40, nb_pattern_70_dk_40_niv_1, v0, necessary_link, max_nb_pattern_under_70_dk_40);
      /* End AC matching */
    }
    if(ACPattern && ms!=NULL) {
      start:
      indice = MS_solve(ms,mode);
      printf("\nsetChoicePoint\n");
      /* choicePoint AC matching */
      if(indice>=0 && setChoicePoint()) {
        printf("on revient d'un fail\n");
        goto start;
      }
    } else {
      indice = 0;
    }
    if(bitSet_get(mask,0)) {
      if(indice>=0 && ms->no_rule==0) {
        int nb_variable=2;
        int nb_variable_ac=1;
        struct term *substitution[2];
        int i;
        /* [r0] m[var=0, subst=-1, NOSHARE](x[var=3, subst=0, NOSHARE, 1],f[var=0, subst=-1, NOSHARE](y[var=1, subst=1, NOSHARE, 2])) => and[var=6, subst=-1, NOSHARE](z[var=-1, subst=-1, PERFECTSHARE],f[var=5, subst=-1, NOSHARE](y[var=-1, subst=-1, PERFECTSHARE]))
	where z[var=0, subst=-1, NOSHARE, 1] := (s1) m[var=4, subst=-1, NOSHARE](x[var=-1, subst=-1, PERFECTSHARE],f[var=2, subst=-1, NOSHARE](y[var=-1, subst=-1, PERFECTSHARE])) */
        struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5, *sv6;
        substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_70_dk_40);
        /* where z[var=0, subst=-1, NOSHARE, 1] := (s1) m[var=4, subst=-1, NOSHARE](x[var=-1, subst=-1, PERFECTSHARE],f[var=2, subst=-1, NOSHARE](y[var=-1, subst=-1, PERFECTSHARE])) */
        sv1=substitution[1];
        TERM_ALLOC(sv2,term1,NULL,code_45);
        sv2->sub[0] = sv1;
        sv3=substitution[0];
        tmp = term_removeTopSymbol(sv3);
        if(tmp != NULL)
          sv3=tmp;
        TERM_ALLOC(sv4,term2,NULL,code_40);
        term_add_onf_term(sv4,sv3);
        term_add_onf_term(sv4,sv2);
        sv0 = str_71( sv4 );
        TERM_ALLOC(sv5,term1,NULL,code_45);
        sv5->sub[0] = sv1;
        TERM_ALLOC(sv6,term2,NULL,code_42);
        sv6->sub[0] = sv0;
        sv6->sub[1] = sv5;
        res = sv6 ;
        goto stratLab0;
      }
      myend1:
    }
  }
  fail();
  stratLab0:
  v0=res;
end:
  rewrite_step++;
end_no_rewrite:
  if(trace>=1) {
    indent(indentlevel);
    printf("rewrite[%d] ",rewrite_step);
    term_printnl(stdout,res);
  }
  subindent();
  return res;
}
