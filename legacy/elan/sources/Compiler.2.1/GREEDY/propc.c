#include "propc.h"
extern struct term *query;

/* Constantes d'execution */
unsigned long rewrite_step=0;
int global_indentlevel=0;

/* Table des symboles */
int fsymtabSize = 229;
fsym fsymtab[229];
/* Declaration des pattern_list */
void init_pattern_list_203();
void delete_pattern_list_203();
void init_pattern_list_202();
void delete_pattern_list_202();

/* Constantes */
struct term *con_218;
struct term *con_217;
struct term *con_216;
struct term *con_215;
struct term *con_214;
struct term *con_213;
struct term *con_212;
struct term *con_211;
struct term *con_210;
struct term *con_209;
struct term *con_208;
struct term *con_201;
struct term *con_200;
struct term *con_225;
struct term *con_224;
struct term *con_223;
struct term *con_222;
struct term *con_221;
struct term *con_220;
struct term *con_1;
struct term *con_0;
struct term *con_219;

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
NULL, NULL, NULL, NULL, (funTabType) &fun_202, (funTabType) &fun_203, 
(funTabType) &fun_204, (funTabType) &fun_205, (funTabType) &fun_206, 
(funTabType) &fun_207, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &fun_226, (funTabType) &fun_227, (funTabType) &fun_228, NULL};

/* tableau de pointeurs de fonctions qui retournent */
/* un pointeur sur un term */
funTabType strTab[] = {
NULL, NULL};

/* Query */
static struct term *main_query() {
  struct term *res;
  /* TERME DE DEPART */
  {
    /* q1 */
    struct term *tmp, *sv[1];
        sv[0] = fun_226(  );
	//sv[0] = fun_227(  );
    res=sv[0];
  }
  return res;
}

int step[229];
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
    step[i]=0;
  }
  fsym_init(code_218,0,"a11",0,0, NULL);
  fsym_init(code_217,0,"a10",0,0, NULL);
  fsym_init(code_216,0,"a9",0,0, NULL);
  fsym_init(code_215,0,"a8",0,0, NULL);
  fsym_init(code_13,2,"greatereq_bool(,)",13,0, NULL);
  fsym_init(code_214,0,"a7",0,0, NULL);
  fsym_init(code_12,2,"greater_bool(,)",12,0, NULL);
  fsym_init(code_213,0,"a6",0,0, NULL);
  fsym_init(code_11,2,"lesseq_bool(,)",11,0, NULL);
  fsym_init(code_212,0,"a5",0,0, NULL);
  fsym_init(code_10,2,"less_bool(,)",10,0, NULL);
  fsym_init(code_211,0,"a4",0,0, NULL);
  fsym_init(code_210,0,"a3",0,0, NULL);
  fsym_init(code_209,0,"a2",0,0, NULL);
  fsym_init(code_208,0,"a1",0,0, NULL);
  fsym_init(code_207,2,"implies(,)",0,0, NULL);
  fsym_init(code_206,1,"not()",0,0, NULL);
  fsym_init(code_205,2,"iff(,)",0,0, NULL);
  fsym_init(code_204,2,"or(,)",0,0, NULL);
  fsym_init(code_203,-1,"xor(,)",0,0, NULL);
  fsym_init(code_202,-1,"and(,)",0,0, NULL);
  fsym_init(code_201,0,"f",0,0, NULL);
  fsym_init(code_228,0,"q3",0,0, NULL);
  fsym_init(code_200,0,"t",0,0, NULL);
  fsym_init(code_227,0,"q2",0,0, NULL);
  fsym_init(code_226,0,"q1",0,0, NULL);
  fsym_init(code_24,1,"(not())",24,0, NULL);
  fsym_init(code_225,0,"a18",0,0, NULL);
  fsym_init(code_224,0,"a17",0,0, NULL);
  fsym_init(code_22,2,"(or)",22,0, NULL);
  fsym_init(code_223,0,"a16",0,0, NULL);
  fsym_init(code_21,2,"(and)",21,0, NULL);
  fsym_init(code_222,0,"a15",0,0, NULL);
  fsym_init(code_221,0,"a14",0,0, NULL);
  fsym_init(code_220,0,"a13",0,0, NULL);
  fsym_init(code_9,2,"neq_bool(,)",9,0, NULL);
  fsym_init(code_8,2,"eq_bool(,)",8,0, NULL);
  fsym_init(code_1,0,"true",0,0, NULL);
  fsym_init(code_0,0,"false",0,0, NULL);
  fsym_init(code_219,0,"a12",0,0, NULL);
  
/* Initialisation des constantes */
  TERM_CONST_ALLOC(con_218, code_218);
  TERM_CONST_ALLOC(con_217, code_217);
  TERM_CONST_ALLOC(con_216, code_216);
  TERM_CONST_ALLOC(con_215, code_215);
  TERM_CONST_ALLOC(con_214, code_214);
  TERM_CONST_ALLOC(con_213, code_213);
  TERM_CONST_ALLOC(con_212, code_212);
  TERM_CONST_ALLOC(con_211, code_211);
  TERM_CONST_ALLOC(con_210, code_210);
  TERM_CONST_ALLOC(con_209, code_209);
  TERM_CONST_ALLOC(con_208, code_208);
  TERM_CONST_ALLOC(con_201, code_201);
  TERM_CONST_ALLOC(con_200, code_200);
  TERM_CONST_ALLOC(con_225, code_225);
  TERM_CONST_ALLOC(con_224, code_224);
  TERM_CONST_ALLOC(con_223, code_223);
  TERM_CONST_ALLOC(con_222, code_222);
  TERM_CONST_ALLOC(con_221, code_221);
  TERM_CONST_ALLOC(con_220, code_220);
  TERM_CONST_ALLOC(con_1, code_1);
  TERM_CONST_ALLOC(con_0, code_0);
  TERM_CONST_ALLOC(con_219, code_219);
  /* Initialisation des pattern_list */
init_pattern_list_203();
init_pattern_list_202();

  for(i=1 ; i<argc && *argv[i]=='-' ; i++) {
    if(!strcmp(argv[i],"-query")) {
      queryMode=1;
    }
  }
  if (!setChoicePoint()) {
      res=main_query();
    printf("\nresult = ");
    if((long)res==0 || (long)res==1) {
      printf("%d\n",res);
    } else {
      term_printnl(stdout,res);
    }
    fail();
  }

  for(i=0 ; i<fsymtabSize ; i++) {
    if(step[i]!=0) {
      printf("step[%d] = %d\n",i,step[i]);
    }
  }

end:
destruction:
  FREE(fsymtab[code_218].name);
  FREE(fsymtab[code_217].name);
  FREE(fsymtab[code_216].name);
  FREE(fsymtab[code_215].name);
  FREE(fsymtab[code_13].name);
  FREE(fsymtab[code_214].name);
  FREE(fsymtab[code_12].name);
  FREE(fsymtab[code_213].name);
  FREE(fsymtab[code_11].name);
  FREE(fsymtab[code_212].name);
  FREE(fsymtab[code_10].name);
  FREE(fsymtab[code_211].name);
  FREE(fsymtab[code_210].name);
  FREE(fsymtab[code_209].name);
  FREE(fsymtab[code_208].name);
  FREE(fsymtab[code_207].name);
  FREE(fsymtab[code_206].name);
  FREE(fsymtab[code_205].name);
  FREE(fsymtab[code_204].name);
  FREE(fsymtab[code_203].name);
  FREE(fsymtab[code_202].name);
  FREE(fsymtab[code_201].name);
  FREE(fsymtab[code_228].name);
  FREE(fsymtab[code_200].name);
  FREE(fsymtab[code_227].name);
  FREE(fsymtab[code_226].name);
  FREE(fsymtab[code_24].name);
  FREE(fsymtab[code_225].name);
  FREE(fsymtab[code_224].name);
  FREE(fsymtab[code_22].name);
  FREE(fsymtab[code_223].name);
  FREE(fsymtab[code_21].name);
  FREE(fsymtab[code_222].name);
  FREE(fsymtab[code_221].name);
  FREE(fsymtab[code_220].name);
  FREE(fsymtab[code_9].name);
  FREE(fsymtab[code_8].name);
  FREE(fsymtab[code_1].name);
  FREE(fsymtab[code_0].name);
  FREE(fsymtab[code_219].name);
  TERM_FREE(con_218);
  TERM_FREE(con_217);
  TERM_FREE(con_216);
  TERM_FREE(con_215);
  TERM_FREE(con_214);
  TERM_FREE(con_213);
  TERM_FREE(con_212);
  TERM_FREE(con_211);
  TERM_FREE(con_210);
  TERM_FREE(con_209);
  TERM_FREE(con_208);
  TERM_FREE(con_201);
  TERM_FREE(con_200);
  TERM_FREE(con_225);
  TERM_FREE(con_224);
  TERM_FREE(con_223);
  TERM_FREE(con_222);
  TERM_FREE(con_221);
  TERM_FREE(con_220);
  TERM_FREE(con_1);
  TERM_FREE(con_0);
  TERM_FREE(con_219);
  /* Destruction des pattern_list */
delete_pattern_list_203();
delete_pattern_list_202();
#ifdef DEBUG
  print_space_usage();
  backStatistics();
#endif
  printf("\nrewrite_step = %u\n",rewrite_step);
}
#include "ac_tools.c"
