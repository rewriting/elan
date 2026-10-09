#include "bool3.h"
extern struct term *query;

/* Constantes d'execution */
unsigned long rewrite_step=0;
int global_indentlevel=0;

/* Table des symboles */
int fsymtabSize = 217;
fsym fsymtab[217];
/* Declaration des pattern_list */
void init_pattern_list_211();
void delete_pattern_list_211();
void init_pattern_list_212();
void delete_pattern_list_212();

/* Constantes */
struct term *con_210;
struct term *con_209;
struct term *con_208;
struct term *con_207;
struct term *con_206;
struct term *con_205;
struct term *con_204;
struct term *con_203;
struct term *con_201;
struct term *con_200;
struct term *con_1;
struct term *con_0;

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
NULL, NULL, NULL, NULL, (funTabType) &fun_202, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, (funTabType) &fun_211, 
(funTabType) &fun_212, (funTabType) &fun_213, (funTabType) &fun_214, 
(funTabType) &fun_215, (funTabType) &fun_216, NULL};

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
    sv[0] = fun_216(  );
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
  fsym_init(code_216,0,"start",0,0, NULL);
  fsym_init(code_215,1,"not()",0,0, NULL);
  fsym_init(code_214,2,"or(,)",0,0, NULL);
  fsym_init(code_13,2,"greatereq_bool(,)",13,0, NULL);
  fsym_init(code_213,2,"and(,)",0,0, NULL);
  fsym_init(code_12,2,"greater_bool(,)",12,0, NULL);
  fsym_init(code_212,-1,"m(,)",0,0, NULL);
  fsym_init(code_11,2,"lesseq_bool(,)",11,0, NULL);
  fsym_init(code_211,-1,"p(,)",0,0, NULL);
  fsym_init(code_10,2,"less_bool(,)",10,0, NULL);
  fsym_init(code_210,0,"a8",0,0, NULL);
  fsym_init(code_33,2,"greatereq_Bool3(,)",33,0, NULL);
  fsym_init(code_32,2,"greater_Bool3(,)",32,0, NULL);
  fsym_init(code_31,2,"lesseq_Bool3(,)",31,0, NULL);
  fsym_init(code_30,2,"less_Bool3(,)",30,0, NULL);
  fsym_init(code_209,0,"a7",0,0, NULL);
  fsym_init(code_208,0,"a6",0,0, NULL);
  fsym_init(code_207,0,"a5",0,0, NULL);
  fsym_init(code_206,0,"a4",0,0, NULL);
  fsym_init(code_205,0,"a3",0,0, NULL);
  fsym_init(code_204,0,"a2",0,0, NULL);
  fsym_init(code_203,0,"a1",0,0, NULL);
  fsym_init(code_202,0,"b2",0,0, NULL);
  fsym_init(code_201,0,"b1",0,0, NULL);
  fsym_init(code_200,0,"b0",0,0, NULL);
  fsym_init(code_24,1,"(not())",24,0, NULL);
  fsym_init(code_22,2,"(or)",22,0, NULL);
  fsym_init(code_21,2,"(and)",21,0, NULL);
  fsym_init(code_9,2,"neq_bool(,)",9,0, NULL);
  fsym_init(code_8,2,"eq_bool(,)",8,0, NULL);
  fsym_init(code_1,0,"true",0,0, NULL);
  fsym_init(code_0,0,"false",0,0, NULL);
  fsym_init(code_19,2,"neq_Bool3(,)",19,0, NULL);
  fsym_init(code_18,2,"eq_Bool3(,)",18,0, NULL);
  
/* Initialisation des constantes */
  TERM_CONST_ALLOC(con_210, code_210);
  TERM_CONST_ALLOC(con_209, code_209);
  TERM_CONST_ALLOC(con_208, code_208);
  TERM_CONST_ALLOC(con_207, code_207);
  TERM_CONST_ALLOC(con_206, code_206);
  TERM_CONST_ALLOC(con_205, code_205);
  TERM_CONST_ALLOC(con_204, code_204);
  TERM_CONST_ALLOC(con_203, code_203);
  TERM_CONST_ALLOC(con_201, code_201);
  TERM_CONST_ALLOC(con_200, code_200);
  TERM_CONST_ALLOC(con_1, code_1);
  TERM_CONST_ALLOC(con_0, code_0);
  /* Initialisation des pattern_list */
init_pattern_list_211();
init_pattern_list_212();

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
    fail();
  }
end:
destruction:
  FREE(fsymtab[code_216].name);
  FREE(fsymtab[code_215].name);
  FREE(fsymtab[code_214].name);
  FREE(fsymtab[code_13].name);
  FREE(fsymtab[code_213].name);
  FREE(fsymtab[code_12].name);
  FREE(fsymtab[code_212].name);
  FREE(fsymtab[code_11].name);
  FREE(fsymtab[code_211].name);
  FREE(fsymtab[code_10].name);
  FREE(fsymtab[code_210].name);
  FREE(fsymtab[code_33].name);
  FREE(fsymtab[code_32].name);
  FREE(fsymtab[code_31].name);
  FREE(fsymtab[code_30].name);
  FREE(fsymtab[code_209].name);
  FREE(fsymtab[code_208].name);
  FREE(fsymtab[code_207].name);
  FREE(fsymtab[code_206].name);
  FREE(fsymtab[code_205].name);
  FREE(fsymtab[code_204].name);
  FREE(fsymtab[code_203].name);
  FREE(fsymtab[code_202].name);
  FREE(fsymtab[code_201].name);
  FREE(fsymtab[code_200].name);
  FREE(fsymtab[code_24].name);
  FREE(fsymtab[code_22].name);
  FREE(fsymtab[code_21].name);
  FREE(fsymtab[code_9].name);
  FREE(fsymtab[code_8].name);
  FREE(fsymtab[code_1].name);
  FREE(fsymtab[code_0].name);
  FREE(fsymtab[code_19].name);
  FREE(fsymtab[code_18].name);
  TERM_FREE(con_210);
  TERM_FREE(con_209);
  TERM_FREE(con_208);
  TERM_FREE(con_207);
  TERM_FREE(con_206);
  TERM_FREE(con_205);
  TERM_FREE(con_204);
  TERM_FREE(con_203);
  TERM_FREE(con_201);
  TERM_FREE(con_200);
  TERM_FREE(con_1);
  TERM_FREE(con_0);
  /* Destruction des pattern_list */
delete_pattern_list_211();
delete_pattern_list_212();
#ifdef DEBUG
  print_space_usage();
  backStatistics();
#endif
  printf("\nrewrite_step = %u\n",rewrite_step);
}
#include "ac_tools.c"
