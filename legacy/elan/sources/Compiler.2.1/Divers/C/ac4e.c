#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "term.h"
#include "bitset.h"
#include "bgraph.h"
#include "match_state.h"
#include "ac4e.h"
#include "back.h"
#include "builtin.h"

/* Constantes d'execution */
int trace = 0; /* 1 : result, 2 : start with */
long rewrite_step=0;
int global_indentlevel=0;

/* Table des symboles */
fsym fsymtab[212];
/* Declaration des pattern_list */

/* Constantes */
struct term *con_211;
struct term *con_210;
struct term *con_1;
struct term *con_0;
struct term *con_209;
struct term *con_208;
struct term *con_207;
struct term *con_206;
struct term *con_205;
struct term *con_204;

/* Redirection de built-ins */

/* Procedure principale */
long *bp_main;
void main() {
  long bp;
  struct term *res;
  bp_main=&bp;
  backTrackInit();
  init_alloc();
  fsym_init(code_211,0,"s1",0,0, NULL);
  fsym_init(code_210,0,"s0",0,0, NULL);
  fsym_init(code_13,2,"greatereq_bool(,)",13,0, NULL);
  fsym_init(code_12,2,"greater_bool(,)",12,0, NULL);
  fsym_init(code_11,2,"lesseq_bool(,)",11,0, NULL);
  fsym_init(code_10,2,"less_bool(,)",10,0, NULL);
  fsym_init(code_9,2,"neq_bool(,)",9,0, NULL);
  fsym_init(code_8,2,"eq_bool(,)",8,0, NULL);
  fsym_init(code_1,0,"true",0,0, NULL);
  fsym_init(code_0,0,"false",0,0, NULL);
  fsym_init(code_209,0,"f",0,0, NULL);
  fsym_init(code_208,0,"e",0,0, NULL);
  fsym_init(code_207,0,"d",0,0, NULL);
  fsym_init(code_206,0,"c",0,0, NULL);
  fsym_init(code_205,0,"b",0,0, NULL);
  fsym_init(code_204,0,"a",0,0, NULL);
  fsym_init(code_203,2,"g(,)",0,0, NULL);
  fsym_init(code_202,-1,"V(,)",0,0, NULL);
  fsym_init(code_201,-1,"U(,)",0,0, NULL);
  fsym_init(code_200,1,"f()",0,0, NULL);
  fsym_init(code_24,1,"(not())",24,0, NULL);
  fsym_init(code_22,2,"(or)",22,0, NULL);
  fsym_init(code_21,2,"(and)",21,0, NULL);
  
/* Initialidsation des constantes */
  TERM_CONST_ALLOC(con_211, code_211);
  TERM_CONST_ALLOC(con_210, code_210);
  TERM_CONST_ALLOC(con_1, code_1);
  TERM_CONST_ALLOC(con_0, code_0);
  TERM_CONST_ALLOC(con_209, code_209);
  TERM_CONST_ALLOC(con_208, code_208);
  TERM_CONST_ALLOC(con_207, code_207);
  TERM_CONST_ALLOC(con_206, code_206);
  TERM_CONST_ALLOC(con_205, code_205);
  TERM_CONST_ALLOC(con_204, code_204);
  /* Initialisation des pattern_list */

  if (!setChoicePoint()) {
    /* TERME DE DEPART */
    {
      /* U(,)(f()(a),f()(b),f()(c),f()(d),f()(e)) */
      struct term *tmp, *sv[11];
      TERM_ALLOC(sv[1],term1,code_200);
      sv[1]->sub[0] = con_208;
      TERM_ALLOC(sv[3],term1,code_200);
      sv[3]->sub[0] = con_207;
      TERM_ALLOC(sv[5],term1,code_200);
      sv[5]->sub[0] = con_206;
      TERM_ALLOC(sv[7],term1,code_200);
      sv[7]->sub[0] = con_205;
      TERM_ALLOC(sv[9],term1,code_200);
      sv[9]->sub[0] = con_204;
      TERM_ALLOC(sv[10],term2,code_201);
      sv[10]->sub[0] = sv[9];
      sv[10]->sub[1] = sv[7];
      sv[10]->sub[2] = sv[5];
      sv[10]->sub[3] = sv[3];
      sv[10]->sub[4] = sv[1];
      res=str_57(sv[10]);
    }
    printf("\nresult[%d] = ",rewrite_step);
    if((long)res==0 || (long)res==1) {
      printf("%d\n",res);
    } else {
      term_printnl(stdout,res);
    }
    fail();
  }
end:
destruction:
  FREE(fsymtab[code_211].name);
  FREE(fsymtab[code_210].name);
  FREE(fsymtab[code_13].name);
  FREE(fsymtab[code_12].name);
  FREE(fsymtab[code_11].name);
  FREE(fsymtab[code_10].name);
  FREE(fsymtab[code_9].name);
  FREE(fsymtab[code_8].name);
  FREE(fsymtab[code_1].name);
  FREE(fsymtab[code_0].name);
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
  TERM_FREE(con_211);
  TERM_FREE(con_210);
  TERM_FREE(con_1);
  TERM_FREE(con_0);
  TERM_FREE(con_209);
  TERM_FREE(con_208);
  TERM_FREE(con_207);
  TERM_FREE(con_206);
  TERM_FREE(con_205);
  TERM_FREE(con_204);
  /* Destruction des pattern_list */
#ifdef DEBUG
  print_space_usage();
  backStatistics();
#endif
  printf("\nrewrite_step = %d\n",rewrite_step);
}
#include "ac4e.core.c"
