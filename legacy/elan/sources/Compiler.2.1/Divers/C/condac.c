#include "condac.h"
extern struct term *query;

/* Constantes d'execution */
unsigned long rewrite_step=0;
int global_indentlevel=0;

/* Table des symboles */
int fsymtabSize = 206;
fsym fsymtab[206];
/* Declaration des pattern_list */
void init_pattern_list_200();
void delete_pattern_list_200();

/* Constantes */
struct term *con_1;
struct term *con_0;
struct term *con_205;
struct term *con_204;
struct term *con_203;

/* Redirection de built-ins */

/* tableau de pointeurs de fonctions qui retournent */
/* un pointeur sur un term */
#ifdef __cplusplus
typedef struct term* (*funTabType)(...);
#else
typedef struct term* (*funTabType)();
#endif

funTabType funTab[] = {
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, (funTabType) &fun_200, NULL, (funTabType) &fun_202, NULL, 
NULL, NULL, NULL};

/* tableau de pointeurs de fonctions qui retournent */
/* un pointeur sur un term */
funTabType strTab[] = {
NULL, NULL};

/* Query */
static struct term *main_query() {
  struct term *res;
  /* TERME DE DEPART */
  {
    /* start */
    struct term *tmp, *sv[1];
    sv[0] = fun_202(  );
    res=sv[0];
  }
  return res;
}

/* Procedure principale */
long *bp_main;
int main(int argc,char **argv) {
  long bp;
  struct term *res;
  int i,queryMode=0;
  bp_main=&bp;
  backTrackInit();
  init_alloc();
  for(i=0 ; i<fsymtabSize ; i++) {
    fsym_init(i,0,"nullString",0,0,NULL);
  }
  fsym_init(code_9,2,"neq_bool(,)",9,0, NULL);
  fsym_init(code_8,2,"eq_bool(,)",8,0, NULL);
  fsym_init(code_1,0,"true",0,0, NULL);
  fsym_init(code_0,0,"false",0,0, NULL);
  fsym_init(code_19,2,"neq_term(,)",19,0, NULL);
  fsym_init(code_18,2,"eq_term(,)",18,0, NULL);
  fsym_init(code_13,2,"greatereq_bool(,)",13,0, NULL);
  fsym_init(code_12,2,"greater_bool(,)",12,0, NULL);
  fsym_init(code_11,2,"lesseq_bool(,)",11,0, NULL);
  fsym_init(code_24,1,"(not())",24,0, NULL);
  fsym_init(code_10,2,"less_bool(,)",10,0, NULL);
  fsym_init(code_22,2,"(or)",22,0, NULL);
  fsym_init(code_21,2,"(and)",21,0, NULL);
  fsym_init(code_205,0,"c",0,0, NULL);
  fsym_init(code_33,2,"greatereq_term(,)",33,0, NULL);
  fsym_init(code_204,0,"b",0,0, NULL);
  fsym_init(code_32,2,"greater_term(,)",32,0, NULL);
  fsym_init(code_203,0,"a",0,0, NULL);
  fsym_init(code_31,2,"lesseq_term(,)",31,0, NULL);
  fsym_init(code_202,0,"start",0,0, NULL);
  fsym_init(code_30,2,"less_term(,)",30,0, NULL);
  fsym_init(code_201,2,"g(,)",0,0, NULL);
  fsym_init(code_200,-1,"f(,)",0,0, NULL);
  
/* Initialisation des constantes */
  TERM_CONST_ALLOC(con_1, code_1);
  TERM_CONST_ALLOC(con_0, code_0);
  TERM_CONST_ALLOC(con_205, code_205);
  TERM_CONST_ALLOC(con_204, code_204);
  TERM_CONST_ALLOC(con_203, code_203);
  /* Initialisation des pattern_list */
init_pattern_list_200();

  for(i=1 ; i<argc && *argv[i]=='-' ; i++) {
    if(!strcmp(argv[i],"-query")) {
      queryMode=1;
    }
  }
  if (!setChoicePoint()) {
    if(queryMode) {
      yyparse();
      res=query;
    } else {
      res=main_query();
    }
    printf("\nresult = ");
    if((long)res==0 || (long)res==1) {
      printf("%d\n",res);
    } else {
      term_printnl(stdout,res);
    }
    backStatistics();
    fail();
  }
end:
destruction:
  FREE(fsymtab[code_9].name);
  FREE(fsymtab[code_8].name);
  FREE(fsymtab[code_1].name);
  FREE(fsymtab[code_0].name);
  FREE(fsymtab[code_19].name);
  FREE(fsymtab[code_18].name);
  FREE(fsymtab[code_13].name);
  FREE(fsymtab[code_12].name);
  FREE(fsymtab[code_11].name);
  FREE(fsymtab[code_24].name);
  FREE(fsymtab[code_10].name);
  FREE(fsymtab[code_22].name);
  FREE(fsymtab[code_21].name);
  FREE(fsymtab[code_205].name);
  FREE(fsymtab[code_33].name);
  FREE(fsymtab[code_204].name);
  FREE(fsymtab[code_32].name);
  FREE(fsymtab[code_203].name);
  FREE(fsymtab[code_31].name);
  FREE(fsymtab[code_202].name);
  FREE(fsymtab[code_30].name);
  FREE(fsymtab[code_201].name);
  FREE(fsymtab[code_200].name);
  TERM_FREE(con_1);
  TERM_FREE(con_0);
  TERM_FREE(con_205);
  TERM_FREE(con_204);
  TERM_FREE(con_203);
  /* Destruction des pattern_list */
delete_pattern_list_200();
#ifdef DEBUG
  print_space_usage();
  backStatistics();
#endif
  printf("\nrewrite_step = %u\n",rewrite_step);
}
#include "ac_tools.c"
