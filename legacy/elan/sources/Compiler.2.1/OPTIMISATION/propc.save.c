#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "term.h"
#include "bitset.h"
#include "bgraph.h"
#include "match_state.h"
#include "propc.h"
#include "back.h"
#include "builtin.h"
#include "streval.h"


/* Constantes d'execution */
//int trace = 0; /* 1 : result, 2 : start with */
#define trace 0

long rewrite_step=0;
int indentlevel=0;

#ifdef GREEDY
static int isGreedyRule_202[4] = {1,1,0,1};
static int isGreedyRule_203[2] = {1,0};
static int isGreedyRule_204[1] = {0};
static int isGreedyRule_205[1] = {0};
static int **greedy_rule_tab_202;
static int **greedy_rule_tab_203;
#endif

/* Table des symboles */
fsym fsymtab[329];
/* Declaration des pattern_list */
void init_pattern_list_203();
void delete_pattern_list_203();
void init_pattern_list_202();
void delete_pattern_list_202();

/* Constantes */
struct term *con_219;
struct term *con_218;
struct term *con_217;
struct term *con_216;
struct term *con_215;
struct term *con_214;
struct term *con_213;
struct term *con_212;
struct term *con_211;
struct term *con_210;
struct term *con_1;
struct term *con_0;
struct term *con_209;
struct term *con_208;
struct term *con_301;
struct term *con_300;
struct term *con_225;
struct term *con_224;
struct term *con_223;
struct term *con_222;
struct term *con_221;
struct term *con_220;

/* Redirection de built-ins */

#ifdef BORO

/* substrategies for streval */

/* defined strategies */

#endif

/* Procedure principale */
long *bp_main;
main() {
  long bp;
  struct term *res;
  bp_main=&bp;
  backTrackInit();
  init_alloc();
  fsym_init(code_219,0,"a12",0,0, NULL);
  fsym_init(code_218,0,"a11",0,0, NULL);
  fsym_init(code_217,0,"a10",0,0, NULL);
  fsym_init(code_216,0,"a9",0,0, NULL);
  fsym_init(code_19,2,"neq_Prop(,)",19,0, NULL);
  fsym_init(code_215,0,"a8",0,0, NULL);
  fsym_init(code_18,2,"eq_Prop(,)",18,0, NULL);
  fsym_init(code_214,0,"a7",0,0, NULL);
  fsym_init(code_213,0,"a6",0,0, NULL);
  fsym_init(code_212,0,"a5",0,0, NULL);
  fsym_init(code_211,0,"a4",0,0, NULL);
  fsym_init(code_210,0,"a3",0,0, NULL);
  fsym_init(code_13,2,"greatereq_int(,)",13,0, NULL);
  fsym_init(code_12,2,"greater_int(,)",12,0, NULL);
  fsym_init(code_11,2,"lesseq_int(,)",11,0, NULL);
  fsym_init(code_10,2,"less_int(,)",10,0, NULL);
  fsym_init(code_9,2,"neq_int(,)",9,0, NULL);
  fsym_init(code_8,2,"eq_int(,)",8,0, NULL);
  fsym_init(code_6,2,"div(,)",6,0, NULL);
  fsym_init(code_5,2,"time(,)",5,0, NULL);
  fsym_init(code_4,2,"minus(,)",4,0, NULL);
  fsym_init(code_3,2,"plus(,)",3,0, NULL);
  fsym_init(code_1,0,"true",0,0, NULL);
  fsym_init(code_0,0,"false",0,0, NULL);
  fsym_init(code_33,2,"greatereq_Prop(,)",33,0, NULL);
  fsym_init(code_209,0,"a2",0,0, NULL);
  fsym_init(code_32,2,"greater_Prop(,)",32,0, NULL);
  fsym_init(code_208,0,"a1",0,0, NULL);
  fsym_init(code_31,2,"lesseq_Prop(,)",31,0, NULL);
  fsym_init(code_207,2,"implies(,)",0,0, NULL);
  fsym_init(code_30,2,"less_Prop(,)",30,0, NULL);
  fsym_init(code_206,1,"not",0,0, NULL);
  fsym_init(code_205,-1,"iff",0,0, NULL);
  fsym_init(code_204,-1,"or",0,0, NULL);
  fsym_init(code_203,-1,"xor",0,0, NULL);
  fsym_init(code_202,-1,"and",0,0, NULL);
  fsym_init(code_301,0,"f",0,0, NULL);
  fsym_init(code_300,0,"t",0,0, NULL);
  fsym_init(code_228,0,"q3",0,0, NULL);
  fsym_init(code_227,0,"q2",0,0, NULL);
  fsym_init(code_226,0,"q1",0,0, NULL);
  fsym_init(code_29,2,"or(,)",29,0, NULL);
  fsym_init(code_225,0,"a18",0,0, NULL);
  fsym_init(code_28,2,"and(,)",28,0, NULL);
  fsym_init(code_224,0,"a17",0,0, NULL);
  fsym_init(code_27,2,"mod(,)",27,0, NULL);
  fsym_init(code_223,0,"a16",0,0, NULL);
  fsym_init(code_26,1,"itob()",26,0, NULL);
  fsym_init(code_222,0,"a15",0,0, NULL);
  fsym_init(code_25,1,"btoi()",25,0, NULL);
  fsym_init(code_221,0,"a14",0,0, NULL);
  fsym_init(code_24,1,"(not())",24,0, NULL);
  fsym_init(code_220,0,"a13",0,0, NULL);
  fsym_init(code_22,2,"(or)",22,0, NULL);
  fsym_init(code_21,2,"(and)",21,0, NULL);
  fsym_init(code_20,1,"umin()",20,0, NULL);
  
/* Initialidsation des constantes */
  TERM_CONST_ALLOC(con_219, code_219);
  TERM_CONST_ALLOC(con_218, code_218);
  TERM_CONST_ALLOC(con_217, code_217);
  TERM_CONST_ALLOC(con_216, code_216);
  TERM_CONST_ALLOC(con_215, code_215);
  TERM_CONST_ALLOC(con_214, code_214);
  TERM_CONST_ALLOC(con_213, code_213);
  TERM_CONST_ALLOC(con_212, code_212);
  TERM_CONST_ALLOC(con_211, code_211);
  TERM_CONST_ALLOC(con_210, code_210);
  TERM_CONST_ALLOC(con_1, code_1);
  TERM_CONST_ALLOC(con_0, code_0);
  TERM_CONST_ALLOC(con_209, code_209);
  TERM_CONST_ALLOC(con_208, code_208);
  TERM_CONST_ALLOC(con_301, code_301);
  TERM_CONST_ALLOC(con_300, code_300);
  TERM_CONST_ALLOC(con_225, code_225);
  TERM_CONST_ALLOC(con_224, code_224);
  TERM_CONST_ALLOC(con_223, code_223);
  TERM_CONST_ALLOC(con_222, code_222);
  TERM_CONST_ALLOC(con_221, code_221);
  TERM_CONST_ALLOC(con_220, code_220);
  /* Initialisation des pattern_list */
  init_pattern_list_203();
  init_pattern_list_202();

  if (!setChoicePoint()) {
    /* TERME DE DEPART */
    {
      /* q1[var=0, subst=-1, NOSHARE] */
      struct term *tmp, *sv0, *sv1, *sv2;
      
      // implies(and(a1,implies(a1,a2)),a2)
      /*
      sv0 = fun_207( con_208, con_209 );
      TERM_ALLOC(sv1,term2,code_202);
      term_add_onf_term(sv1,con_208);
      term_add_onf_term(sv1,sv0);
      sv1 = fun_202( sv1 );
      res = fun_207( sv1, con_209 );
      */

      // implies(and(a1,implies(a1,a2),implies(a2,a3)),a3)
      /*
      sv0 = fun_207( con_208, con_209 );
      sv1 = fun_207( con_209, con_210 );
      TERM_ALLOC(sv2,term2,code_202);
      term_add_onf_term(sv2,con_208);
      term_add_onf_term(sv2,sv0);
      term_add_onf_term(sv2,sv1);
      sv2 = fun_202( sv2 );
      res = fun_207( sv2, con_210 );
      */
      
      //sv0 = fun_226(  );
      sv0 = fun_227(  );
      //sv0 = fun_228(  );
      res=sv0;
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
  FREE(fsymtab[code_219].name);
  FREE(fsymtab[code_218].name);
  FREE(fsymtab[code_217].name);
  FREE(fsymtab[code_216].name);
  FREE(fsymtab[code_19].name);
  FREE(fsymtab[code_215].name);
  FREE(fsymtab[code_18].name);
  FREE(fsymtab[code_214].name);
  FREE(fsymtab[code_213].name);
  FREE(fsymtab[code_212].name);
  FREE(fsymtab[code_211].name);
  FREE(fsymtab[code_210].name);
  FREE(fsymtab[code_13].name);
  FREE(fsymtab[code_12].name);
  FREE(fsymtab[code_11].name);
  FREE(fsymtab[code_10].name);
  FREE(fsymtab[code_9].name);
  FREE(fsymtab[code_8].name);
  FREE(fsymtab[code_6].name);
  FREE(fsymtab[code_5].name);
  FREE(fsymtab[code_4].name);
  FREE(fsymtab[code_3].name);
  FREE(fsymtab[code_1].name);
  FREE(fsymtab[code_0].name);
  FREE(fsymtab[code_33].name);
  FREE(fsymtab[code_209].name);
  FREE(fsymtab[code_32].name);
  FREE(fsymtab[code_208].name);
  FREE(fsymtab[code_31].name);
  FREE(fsymtab[code_207].name);
  FREE(fsymtab[code_30].name);
  FREE(fsymtab[code_206].name);
  FREE(fsymtab[code_205].name);
  FREE(fsymtab[code_204].name);
  FREE(fsymtab[code_203].name);
  FREE(fsymtab[code_202].name);
  FREE(fsymtab[code_301].name);
  FREE(fsymtab[code_300].name);
  FREE(fsymtab[code_228].name);
  FREE(fsymtab[code_227].name);
  FREE(fsymtab[code_226].name);
  FREE(fsymtab[code_29].name);
  FREE(fsymtab[code_225].name);
  FREE(fsymtab[code_28].name);
  FREE(fsymtab[code_224].name);
  FREE(fsymtab[code_27].name);
  FREE(fsymtab[code_223].name);
  FREE(fsymtab[code_26].name);
  FREE(fsymtab[code_222].name);
  FREE(fsymtab[code_25].name);
  FREE(fsymtab[code_221].name);
  FREE(fsymtab[code_24].name);
  FREE(fsymtab[code_220].name);
  FREE(fsymtab[code_22].name);
  FREE(fsymtab[code_21].name);
  FREE(fsymtab[code_20].name);
  TERM_FREE(con_219);
  TERM_FREE(con_218);
  TERM_FREE(con_217);
  TERM_FREE(con_216);
  TERM_FREE(con_215);
  TERM_FREE(con_214);
  TERM_FREE(con_213);
  TERM_FREE(con_212);
  TERM_FREE(con_211);
  TERM_FREE(con_210);
  TERM_FREE(con_1);
  TERM_FREE(con_0);
  TERM_FREE(con_209);
  TERM_FREE(con_208);
  TERM_FREE(con_301);
  TERM_FREE(con_300);
  TERM_FREE(con_225);
  TERM_FREE(con_224);
  TERM_FREE(con_223);
  TERM_FREE(con_222);
  TERM_FREE(con_221);
  TERM_FREE(con_220);
  /* Destruction des pattern_list */
  delete_pattern_list_203();
  delete_pattern_list_202();
#ifdef DEBUG
  print_space_usage();
#endif
  printf("\nrewrite_step = %d\n",rewrite_step);
}

struct term* fun_204(struct term *v0 ) {
  struct term *v1,*v2;
  bitSet32 *mask;
  struct term *res;
  match_state *ms=NULL;
  int mode;
  int necessary_link;
  int indice=-1;
  int ACPattern=0;
  bitSet32_create(mask,1);
  bitSet32_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_204(");
    term_print(stdout,v0);
    printf(")\n");
  }
  if(term_first(v0)==term_last(v0) && cell_mult(term_first(v0))==1) {
    res=cell_t(term_first(v0));
    goto end_no_rewrite;
  }
  /* Begin syntactical matching */
  bitSet32_set(mask,0);
  /* End syntactical matching */
  if( bitSet32_get(mask,0) ) {
    ACPattern=1;
    /* End AC matching */
  }
  if(ACPattern && ms!=NULL) {
    indice = MS_solve(ms,mode);
  } else {
    indice = 0;
  }
  if(bitSet32_get(mask,0)) {
    {
      int nb_variable=2;
      int nb_variable_ac=1;
      struct term *substitution[2];
      int i;
      /* [] or(,)[var0[var=1, subst=0, NOSHARE, 2].var1[var=0, subst=1, NOSHARE, 2]][mv][var=0, subst=-1, NOSHARE] => xor(,)[var0[var=-1, subst=-1, PERFECTSHARE].var1[var=-1, subst=-1, PERFECTSHARE].and(,)[var0[var=-1, subst=-1, PERFECTSHARE].var1[var=-1, subst=-1, PERFECTSHARE]][var=2, subst=-1, NOSHARE]][var=3, subst=-1, NOSHARE] */
      struct term *tmp, *sv0, *sv1, *sv2, *sv3;
      for(i=0 ; i<nb_variable ; i++) {
        substitution[i]=v0;
      }
      sv1=substitution[0];
      sv0=substitution[1];
      {
        int *E,*sol;
        int nb_arg_subject;
        int no_arg_subject;
        int indice=0;
        struct cell_term *cell;
        struct cell_term *copy_cell;
        for(cell=term_first(sv1), nb_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), nb_arg_subject++);
        E=(int*)IMALLOC(nb_arg_subject*sizeof(int));
        sol=(int*)IMALLOC(nb_arg_subject*sizeof(int));
        for(cell=term_first(sv1), no_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), no_arg_subject++) {
          E[no_arg_subject]=cell_mult(cell);
          sol[no_arg_subject]=0;
        }
        indice=next_pe_extract(nb_arg_subject-1,E,sol,1);
        if(indice >= 0 ) {
          int i;
          extract_xy_from_pe(sv1,E,sol,1,&sv1,&sv0);
          IFREE(E);
          IFREE(sol);
        } else {
          IFREE(E);
          IFREE(sol);
          goto myend0;
        }
      }
      tmp = term_removeTopSymbol(sv0);
      if(tmp == NULL)
        sv0=fun_204(sv0);
      else
        sv0=tmp;
      
      setShared(sv0);

      tmp = term_removeTopSymbol(sv1);
      if(tmp == NULL)
        sv1=fun_204(sv1);
      else
        sv1=tmp;

      setShared(sv1);

      TERM_ALLOC(sv2,term2,code_202);
      term_add_onf_term(sv2,sv1);
      term_add_onf_term(sv2,sv0);
      sv2 = fun_202( sv2 );
      TERM_ALLOC(sv3,term2,code_203);
      term_add_onf_term(sv3,sv1);
      term_add_onf_term(sv3,sv0);
      term_add_onf_term(sv3,sv2);
      sv3 = fun_203( sv3 );
      res = sv3 ;
      goto end;
    }
    myend0:;
  }
match_fail:
  res=v0;
  if(trace>=1) {
    indent(indentlevel); printf("*** match fail 204\n");
  }
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(trace>=1) {
    indent(indentlevel);
    printf("rewrite[%d] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_delete(mask);
  if(ACPattern && ms!=NULL)
    MS_delete(ms);
  subindent();
  return res;
}

struct term* fun_206(struct term *v1 ) {
  struct term *v2,*v3,*v4;
  bitSet32 *mask;
  struct term *res;
  bitSet32_create(mask,1);
  bitSet32_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_206(");
    term_print(stdout,v1);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
    bitSet32_set(mask,0);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask,0)) {
    /* lhs: not(var0[mv][var=1, subst=-1, NOSHARE, 1])[mv][var=0, subst=-1, NOSHARE] */
    struct term *tmp, *sv0, *sv1;
    /* rhs: xor(,)[var0[var=-1, subst=-1, PERFECTSHARE].t[var=0, subst=-1, NOSHARE]][var=1, subst=-1, NOSHARE] */
    TERM_ALLOC(sv1,term2,code_203);
    term_add_onf_term(sv1,v1);
    term_add_onf_term(sv1,con_300);
    sv1 = fun_203( sv1 );
    res = sv1 ;
    goto end;
  myend1:;
  }
match_fail:
  TERM_ALLOC(res,term1, 206);
  res->sub[0] = v1;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(trace>=1) {
    indent(indentlevel);
    printf("rewrite[%d] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_delete(mask);
  subindent();
  return res;
}

struct term* fun_226( ) {
  struct term *v1,*v2;
  bitSet32 *mask;
  struct term *res;
  bitSet32_create(mask,1);
  bitSet32_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_226(");
    printf(")\n");
  }
  /* Begin syntactical matching */
  bitSet32_set(mask,0);
  /* End syntactical matching */
  if(bitSet32_get(mask,0)) {
    /* lhs: q1[mv][var=0, subst=-1, NOSHARE] */
    struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5, *sv6, *sv7, *sv8, *sv9, *sv10, *sv11, *sv12, *sv13, *sv14, *sv15, *sv16, *sv17, *sv18, *sv19, *sv20, *sv21, *sv22, *sv23, *sv24, *sv25, *sv26, *sv27, *sv28, *sv29, *sv30, *sv31, *sv32, *sv33, *sv34, *sv35, *sv36, *sv37, *sv38, *sv39, *sv40, *sv41, *sv42, *sv43, *sv44, *sv45, *sv46, *sv47, *sv48, *sv49, *sv50, *sv51, *sv52, *sv53, *sv54, *sv55, *sv56, *sv57, *sv58, *sv59, *sv60, *sv61, *sv62, *sv63, *sv64, *sv65, *sv66, *sv67, *sv68, *sv69, *sv70, *sv71, *sv72, *sv73, *sv74, *sv75, *sv76, *sv77, *sv78, *sv79, *sv80, *sv81, *sv82, *sv83, *sv84, *sv85, *sv86, *sv87, *sv88, *sv89, *sv90, *sv91, *sv92, *sv93, *sv94, *sv95, *sv96, *sv97, *sv98, *sv99, *sv100, *sv101, *sv102, *sv103, *sv104, *sv105, *sv106, *sv107, *sv108, *sv109, *sv110, *sv111, *sv112, *sv113, *sv114, *sv115, *sv116, *sv117, *sv118, *sv119, *sv120, *sv121, *sv122, *sv123, *sv124, *sv125, *sv126, *sv127, *sv128, *sv129, *sv130, *sv131, *sv132, *sv133, *sv134, *sv135, *sv136, *sv137, *sv138, *sv139, *sv140, *sv141, *sv142, *sv143, *sv144, *sv145, *sv146, *sv147, *sv148, *sv149, *sv150, *sv151, *sv152, *sv153, *sv154, *sv155, *sv156, *sv157, *sv158, *sv159, *sv160, *sv161, *sv162;
    /* rhs: implies(and(,)[iff(,)[or(,)[iff(,)[xor(,)[a4[var=156, subst=-1, NOSHARE].a5[var=155, subst=-1, NOSHARE]][var=157, subst=-1, NOSHARE].not(not(not(a6[var=151, subst=-1, NOSHARE])[var=152, subst=-1, NOSHARE])[var=153, subst=-1, NOSHARE])[var=154, subst=-1, NOSHARE]][var=158, subst=-1, NOSHARE].not(a3[var=149, subst=-1, NOSHARE])[var=150, subst=-1, NOSHARE]][var=159, subst=-1, NOSHARE].or(,)[a1[var=147, subst=-1, NOSHARE].a2[var=146, subst=-1, NOSHARE]][var=148, subst=-1, NOSHARE].not(and(,)[not(xor(,)[and(,)[xor(,)[iff(,)[a4[var=138, subst=-1, NOSHARE].a9[var=137, subst=-1, NOSHARE]][var=139, subst=-1, NOSHARE].a7[2][var=136, subst=-1, NOSHARE]][var=140, subst=-1, NOSHARE].xor(,)[iff(,)[a5[2][var=133, subst=-1, NOSHARE]][var=134, subst=-1, NOSHARE].a2[var=132, subst=-1, NOSHARE]][var=135, subst=-1, NOSHARE].a11[var=131, subst=-1, NOSHARE]][var=141, subst=-1, NOSHARE].or(,)[and(,)[a10[var=128, subst=-1, NOSHARE].a11[var=127, subst=-1, NOSHARE]][var=129, subst=-1, NOSHARE].a9[var=126, subst=-1, NOSHARE]][var=130, subst=-1, NOSHARE].a2[var=125, subst=-1, NOSHARE]][var=142, subst=-1, NOSHARE])[var=143, subst=-1, NOSHARE].a7[var=124, subst=-1, NOSHARE].a8[var=123, subst=-1, NOSHARE]][var=144, subst=-1, NOSHARE])[var=145, subst=-1, NOSHARE]][var=160, subst=-1, NOSHARE].implies(iff(,)[or(,)[iff(,)[xor(,)[a4[var=117, subst=-1, NOSHARE].a5[var=116, subst=-1, NOSHARE]][var=118, subst=-1, NOSHARE].not(not(not(a6[var=112, subst=-1, NOSHARE])[var=113, subst=-1, NOSHARE])[var=114, subst=-1, NOSHARE])[var=115, subst=-1, NOSHARE]][var=119, subst=-1, NOSHARE].not(a3[var=110, subst=-1, NOSHARE])[var=111, subst=-1, NOSHARE]][var=120, subst=-1, NOSHARE].or(,)[a1[var=108, subst=-1, NOSHARE].a2[var=107, subst=-1, NOSHARE]][var=109, subst=-1, NOSHARE].not(and(,)[not(xor(,)[and(,)[xor(,)[iff(,)[a4[var=99, subst=-1, NOSHARE].a9[var=98, subst=-1, NOSHARE]][var=100, subst=-1, NOSHARE].a7[2][var=97, subst=-1, NOSHARE]][var=101, subst=-1, NOSHARE].xor(,)[iff(,)[a5[2][var=94, subst=-1, NOSHARE]][var=95, subst=-1, NOSHARE].a2[var=93, subst=-1, NOSHARE]][var=96, subst=-1, NOSHARE].a11[var=92, subst=-1, NOSHARE]][var=102, subst=-1, NOSHARE].or(,)[and(,)[a10[var=89, subst=-1, NOSHARE].a11[var=88, subst=-1, NOSHARE]][var=90, subst=-1, NOSHARE].a9[var=87, subst=-1, NOSHARE]][var=91, subst=-1, NOSHARE].a2[var=86, subst=-1, NOSHARE]][var=103, subst=-1, NOSHARE])[var=104, subst=-1, NOSHARE].a7[var=85, subst=-1, NOSHARE].a8[var=84, subst=-1, NOSHARE]][var=105, subst=-1, NOSHARE])[var=106, subst=-1, NOSHARE]][var=121, subst=-1, NOSHARE],not(and(,)[not(iff(,)[not(a9[var=78, subst=-1, NOSHARE])[var=79, subst=-1, NOSHARE].not(a11[var=76, subst=-1, NOSHARE])[var=77, subst=-1, NOSHARE]][var=80, subst=-1, NOSHARE])[var=81, subst=-1, NOSHARE].implies(and(,)[a1[var=73, subst=-1, NOSHARE].a2[var=72, subst=-1, NOSHARE]][var=74, subst=-1, NOSHARE],not(xor(,)[or(,)[xor(,)[or(,)[a7[var=66, subst=-1, NOSHARE].a8[var=65, subst=-1, NOSHARE]][var=67, subst=-1, NOSHARE].implies(and(,)[a3[var=62, subst=-1, NOSHARE].a4[var=61, subst=-1, NOSHARE]][var=63, subst=-1, NOSHARE],implies(a5[var=59, subst=-1, NOSHARE],a6[var=58, subst=-1, NOSHARE])[var=60, subst=-1, NOSHARE])[var=64, subst=-1, NOSHARE]][var=68, subst=-1, NOSHARE].xor(,)[iff(,)[a9[var=55, subst=-1, NOSHARE].a10[var=54, subst=-1, NOSHARE]][var=56, subst=-1, NOSHARE].a11[var=53, subst=-1, NOSHARE]][var=57, subst=-1, NOSHARE].xor(,)[a2[2][var=51, subst=-1, NOSHARE].a7[var=50, subst=-1, NOSHARE]][var=52, subst=-1, NOSHARE]][var=69, subst=-1, NOSHARE].iff(,)[xor(,)[not(a6[var=46, subst=-1, NOSHARE])[var=47, subst=-1, NOSHARE].a6[var=45, subst=-1, NOSHARE]][var=48, subst=-1, NOSHARE].or(,)[a4[var=43, subst=-1, NOSHARE].a9[var=42, subst=-1, NOSHARE]][var=44, subst=-1, NOSHARE]][var=49, subst=-1, NOSHARE]][var=70, subst=-1, NOSHARE])[var=71, subst=-1, NOSHARE])[var=75, subst=-1, NOSHARE]][var=82, subst=-1, NOSHARE])[var=83, subst=-1, NOSHARE])[var=122, subst=-1, NOSHARE]][var=161, subst=-1, NOSHARE],not(and(,)[not(iff(,)[not(a9[var=36, subst=-1, NOSHARE])[var=37, subst=-1, NOSHARE].not(a11[var=34, subst=-1, NOSHARE])[var=35, subst=-1, NOSHARE]][var=38, subst=-1, NOSHARE])[var=39, subst=-1, NOSHARE].implies(and(,)[a1[var=31, subst=-1, NOSHARE].a2[var=30, subst=-1, NOSHARE]][var=32, subst=-1, NOSHARE],not(xor(,)[or(,)[xor(,)[or(,)[a7[var=24, subst=-1, NOSHARE].a8[var=23, subst=-1, NOSHARE]][var=25, subst=-1, NOSHARE].implies(and(,)[a3[var=20, subst=-1, NOSHARE].a4[var=19, subst=-1, NOSHARE]][var=21, subst=-1, NOSHARE],implies(a5[var=17, subst=-1, NOSHARE],a6[var=16, subst=-1, NOSHARE])[var=18, subst=-1, NOSHARE])[var=22, subst=-1, NOSHARE]][var=26, subst=-1, NOSHARE].xor(,)[iff(,)[a9[var=13, subst=-1, NOSHARE].a10[var=12, subst=-1, NOSHARE]][var=14, subst=-1, NOSHARE].a11[var=11, subst=-1, NOSHARE]][var=15, subst=-1, NOSHARE].xor(,)[a2[2][var=9, subst=-1, NOSHARE].a7[var=8, subst=-1, NOSHARE]][var=10, subst=-1, NOSHARE]][var=27, subst=-1, NOSHARE].iff(,)[xor(,)[not(a6[var=4, subst=-1, NOSHARE])[var=5, subst=-1, NOSHARE].a6[var=3, subst=-1, NOSHARE]][var=6, subst=-1, NOSHARE].or(,)[a4[var=1, subst=-1, NOSHARE].a9[var=0, subst=-1, NOSHARE]][var=2, subst=-1, NOSHARE]][var=7, subst=-1, NOSHARE]][var=28, subst=-1, NOSHARE])[var=29, subst=-1, NOSHARE])[var=33, subst=-1, NOSHARE]][var=40, subst=-1, NOSHARE])[var=41, subst=-1, NOSHARE])[var=162, subst=-1, NOSHARE] */
    TERM_ALLOC(sv2,term2,code_204);
    term_add_onf_term(sv2,con_211);
    term_add_onf_term(sv2,con_216);
    sv2 = fun_204( sv2 );
    sv5 = fun_206( con_213 );
    TERM_ALLOC(sv6,term2,code_203);
    term_add_onf_term(sv6,sv5);
    term_add_onf_term(sv6,con_213);
    sv6 = fun_203( sv6 );
    TERM_ALLOC(sv7,term2,code_205);
    term_add_onf_term(sv7,sv6);
    term_add_onf_term(sv7,sv2);
    sv7 = fun_205( sv7 );
    TERM_ALLOC(sv10,term2,code_203);
    term_add_onf_term(sv10,con_209);
    term_add_onf_term(sv10,con_209);
    term_add_onf_term(sv10,con_214);
    sv10 = fun_203( sv10 );
    TERM_ALLOC(sv14,term2,code_205);
    term_add_onf_term(sv14,con_216);
    term_add_onf_term(sv14,con_217);
    sv14 = fun_205( sv14 );
    TERM_ALLOC(sv15,term2,code_203);
    term_add_onf_term(sv15,sv14);
    term_add_onf_term(sv15,con_218);
    sv15 = fun_203( sv15 );
    sv18 = fun_207( con_212,con_213 );
    TERM_ALLOC(sv21,term2,code_202);
    term_add_onf_term(sv21,con_210);
    term_add_onf_term(sv21,con_211);
    sv21 = fun_202( sv21 );
    sv22 = fun_207( sv21,sv18 );
    TERM_ALLOC(sv25,term2,code_204);
    term_add_onf_term(sv25,con_214);
    term_add_onf_term(sv25,con_215);
    sv25 = fun_204( sv25 );
    TERM_ALLOC(sv26,term2,code_203);
    term_add_onf_term(sv26,sv25);
    term_add_onf_term(sv26,sv22);
    sv26 = fun_203( sv26 );
    TERM_ALLOC(sv27,term2,code_204);
    term_add_onf_term(sv27,sv26);
    term_add_onf_term(sv27,sv15);
    term_add_onf_term(sv27,sv10);
    sv27 = fun_204( sv27 );
    TERM_ALLOC(sv28,term2,code_203);
    term_add_onf_term(sv28,sv27);
    term_add_onf_term(sv28,sv7);
    sv28 = fun_203( sv28 );
    sv29 = fun_206( sv28 );
    TERM_ALLOC(sv32,term2,code_202);
    term_add_onf_term(sv32,con_208);
    term_add_onf_term(sv32,con_209);
    sv32 = fun_202( sv32 );
    sv33 = fun_207( sv32,sv29 );
    sv35 = fun_206( con_218 );
    sv37 = fun_206( con_216 );
    TERM_ALLOC(sv38,term2,code_205);
    term_add_onf_term(sv38,sv37);
    term_add_onf_term(sv38,sv35);
    sv38 = fun_205( sv38 );
    sv39 = fun_206( sv38 );
    TERM_ALLOC(sv40,term2,code_202);
    term_add_onf_term(sv40,sv39);
    term_add_onf_term(sv40,sv33);
    sv40 = fun_202( sv40 );
    sv41 = fun_206( sv40 );
    TERM_ALLOC(sv44,term2,code_204);
    term_add_onf_term(sv44,con_211);
    term_add_onf_term(sv44,con_216);
    sv44 = fun_204( sv44 );
    sv47 = fun_206( con_213 );
    TERM_ALLOC(sv48,term2,code_203);
    term_add_onf_term(sv48,sv47);
    term_add_onf_term(sv48,con_213);
    sv48 = fun_203( sv48 );
    TERM_ALLOC(sv49,term2,code_205);
    term_add_onf_term(sv49,sv48);
    term_add_onf_term(sv49,sv44);
    sv49 = fun_205( sv49 );
    TERM_ALLOC(sv52,term2,code_203);
    term_add_onf_term(sv52,con_209);
    term_add_onf_term(sv52,con_209);
    term_add_onf_term(sv52,con_214);
    sv52 = fun_203( sv52 );
    TERM_ALLOC(sv56,term2,code_205);
    term_add_onf_term(sv56,con_216);
    term_add_onf_term(sv56,con_217);
    sv56 = fun_205( sv56 );
    TERM_ALLOC(sv57,term2,code_203);
    term_add_onf_term(sv57,sv56);
    term_add_onf_term(sv57,con_218);
    sv57 = fun_203( sv57 );
    sv60 = fun_207( con_212,con_213 );
    TERM_ALLOC(sv63,term2,code_202);
    term_add_onf_term(sv63,con_210);
    term_add_onf_term(sv63,con_211);
    sv63 = fun_202( sv63 );
    sv64 = fun_207( sv63,sv60 );
    TERM_ALLOC(sv67,term2,code_204);
    term_add_onf_term(sv67,con_214);
    term_add_onf_term(sv67,con_215);
    sv67 = fun_204( sv67 );
    TERM_ALLOC(sv68,term2,code_203);
    term_add_onf_term(sv68,sv67);
    term_add_onf_term(sv68,sv64);
    sv68 = fun_203( sv68 );
    TERM_ALLOC(sv69,term2,code_204);
    term_add_onf_term(sv69,sv68);
    term_add_onf_term(sv69,sv57);
    term_add_onf_term(sv69,sv52);
    sv69 = fun_204( sv69 );
    TERM_ALLOC(sv70,term2,code_203);
    term_add_onf_term(sv70,sv69);
    term_add_onf_term(sv70,sv49);
    sv70 = fun_203( sv70 );
    sv71 = fun_206( sv70 );
    TERM_ALLOC(sv74,term2,code_202);
    term_add_onf_term(sv74,con_208);
    term_add_onf_term(sv74,con_209);
    sv74 = fun_202( sv74 );
    sv75 = fun_207( sv74,sv71 );
    sv77 = fun_206( con_218 );
    sv79 = fun_206( con_216 );
    TERM_ALLOC(sv80,term2,code_205);
    term_add_onf_term(sv80,sv79);
    term_add_onf_term(sv80,sv77);
    sv80 = fun_205( sv80 );
    sv81 = fun_206( sv80 );
    TERM_ALLOC(sv82,term2,code_202);
    term_add_onf_term(sv82,sv81);
    term_add_onf_term(sv82,sv75);
    sv82 = fun_202( sv82 );
    sv83 = fun_206( sv82 );
    TERM_ALLOC(sv90,term2,code_202);
    term_add_onf_term(sv90,con_217);
    term_add_onf_term(sv90,con_218);
    sv90 = fun_202( sv90 );
    TERM_ALLOC(sv91,term2,code_204);
    term_add_onf_term(sv91,sv90);
    term_add_onf_term(sv91,con_216);
    sv91 = fun_204( sv91 );
    TERM_ALLOC(sv95,term2,code_205);
    term_add_onf_term(sv95,con_212);
    term_add_onf_term(sv95,con_212);
    sv95 = fun_205( sv95 );
    TERM_ALLOC(sv96,term2,code_203);
    term_add_onf_term(sv96,sv95);
    term_add_onf_term(sv96,con_209);
    sv96 = fun_203( sv96 );
    TERM_ALLOC(sv100,term2,code_205);
    term_add_onf_term(sv100,con_211);
    term_add_onf_term(sv100,con_216);
    sv100 = fun_205( sv100 );
    TERM_ALLOC(sv101,term2,code_203);
    term_add_onf_term(sv101,sv100);
    term_add_onf_term(sv101,con_214);
    term_add_onf_term(sv101,con_214);
    sv101 = fun_203( sv101 );
    TERM_ALLOC(sv102,term2,code_202);
    term_add_onf_term(sv102,sv101);
    term_add_onf_term(sv102,sv96);
    term_add_onf_term(sv102,con_218);
    sv102 = fun_202( sv102 );
    TERM_ALLOC(sv103,term2,code_203);
    term_add_onf_term(sv103,sv102);
    term_add_onf_term(sv103,sv91);
    term_add_onf_term(sv103,con_209);
    sv103 = fun_203( sv103 );
    sv104 = fun_206( sv103 );
    TERM_ALLOC(sv105,term2,code_202);
    term_add_onf_term(sv105,sv104);
    term_add_onf_term(sv105,con_214);
    term_add_onf_term(sv105,con_215);
    sv105 = fun_202( sv105 );
    sv106 = fun_206( sv105 );
    TERM_ALLOC(sv109,term2,code_204);
    term_add_onf_term(sv109,con_208);
    term_add_onf_term(sv109,con_209);
    sv109 = fun_204( sv109 );
    sv111 = fun_206( con_210 );
    sv113 = fun_206( con_213 );
    sv114 = fun_206( sv113 );
    sv115 = fun_206( sv114 );
    TERM_ALLOC(sv118,term2,code_203);
    term_add_onf_term(sv118,con_211);
    term_add_onf_term(sv118,con_212);
    sv118 = fun_203( sv118 );
    TERM_ALLOC(sv119,term2,code_205);
    term_add_onf_term(sv119,sv118);
    term_add_onf_term(sv119,sv115);
    sv119 = fun_205( sv119 );
    TERM_ALLOC(sv120,term2,code_204);
    term_add_onf_term(sv120,sv119);
    term_add_onf_term(sv120,sv111);
    sv120 = fun_204( sv120 );
    TERM_ALLOC(sv121,term2,code_205);
    term_add_onf_term(sv121,sv120);
    term_add_onf_term(sv121,sv109);
    term_add_onf_term(sv121,sv106);
    sv121 = fun_205( sv121 );
    sv122 = fun_207( sv121,sv83 );
    TERM_ALLOC(sv129,term2,code_202);
    term_add_onf_term(sv129,con_217);
    term_add_onf_term(sv129,con_218);
    sv129 = fun_202( sv129 );
    TERM_ALLOC(sv130,term2,code_204);
    term_add_onf_term(sv130,sv129);
    term_add_onf_term(sv130,con_216);
    sv130 = fun_204( sv130 );
    TERM_ALLOC(sv134,term2,code_205);
    term_add_onf_term(sv134,con_212);
    term_add_onf_term(sv134,con_212);
    sv134 = fun_205( sv134 );
    TERM_ALLOC(sv135,term2,code_203);
    term_add_onf_term(sv135,sv134);
    term_add_onf_term(sv135,con_209);
    sv135 = fun_203( sv135 );
    TERM_ALLOC(sv139,term2,code_205);
    term_add_onf_term(sv139,con_211);
    term_add_onf_term(sv139,con_216);
    sv139 = fun_205( sv139 );
    TERM_ALLOC(sv140,term2,code_203);
    term_add_onf_term(sv140,sv139);
    term_add_onf_term(sv140,con_214);
    term_add_onf_term(sv140,con_214);
    sv140 = fun_203( sv140 );
    TERM_ALLOC(sv141,term2,code_202);
    term_add_onf_term(sv141,sv140);
    term_add_onf_term(sv141,sv135);
    term_add_onf_term(sv141,con_218);
    sv141 = fun_202( sv141 );
    TERM_ALLOC(sv142,term2,code_203);
    term_add_onf_term(sv142,sv141);
    term_add_onf_term(sv142,sv130);
    term_add_onf_term(sv142,con_209);
    sv142 = fun_203( sv142 );
    sv143 = fun_206( sv142 );
    TERM_ALLOC(sv144,term2,code_202);
    term_add_onf_term(sv144,sv143);
    term_add_onf_term(sv144,con_214);
    term_add_onf_term(sv144,con_215);
    sv144 = fun_202( sv144 );
    sv145 = fun_206( sv144 );
    TERM_ALLOC(sv148,term2,code_204);
    term_add_onf_term(sv148,con_208);
    term_add_onf_term(sv148,con_209);
    sv148 = fun_204( sv148 );
    sv150 = fun_206( con_210 );
    sv152 = fun_206( con_213 );
    sv153 = fun_206( sv152 );
    sv154 = fun_206( sv153 );
    TERM_ALLOC(sv157,term2,code_203);
    term_add_onf_term(sv157,con_211);
    term_add_onf_term(sv157,con_212);
    sv157 = fun_203( sv157 );
    TERM_ALLOC(sv158,term2,code_205);
    term_add_onf_term(sv158,sv157);
    term_add_onf_term(sv158,sv154);
    sv158 = fun_205( sv158 );
    TERM_ALLOC(sv159,term2,code_204);
    term_add_onf_term(sv159,sv158);
    term_add_onf_term(sv159,sv150);
    sv159 = fun_204( sv159 );
    TERM_ALLOC(sv160,term2,code_205);
    term_add_onf_term(sv160,sv159);
    term_add_onf_term(sv160,sv148);
    term_add_onf_term(sv160,sv145);
    sv160 = fun_205( sv160 );
    TERM_ALLOC(sv161,term2,code_202);
    term_add_onf_term(sv161,sv160);
    term_add_onf_term(sv161,sv122);
    sv161 = fun_202( sv161 );
    sv162 = fun_207( sv161,sv41 );
    res = sv162 ;
    goto end;
    myend2:;
  }
match_fail:
  fprintf(stderr,"Match Fail error in fun_226\n");
  exit(0);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(trace>=1) {
    indent(indentlevel);
    printf("rewrite[%d] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_delete(mask);
  subindent();
  return res;
}

struct term* fun_227( ) {
  struct term *v1,*v2;
  bitSet32 *mask;
  struct term *res;
  bitSet32_create(mask,1);
  bitSet32_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_227(");
    printf(")\n");
  }
  /* Begin syntactical matching */
  bitSet32_set(mask,0);
  /* End syntactical matching */
  if(bitSet32_get(mask,0)) {
    /* lhs: q2[mv][var=0, subst=-1, NOSHARE] */
    struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5, *sv6, *sv7, *sv8, *sv9, *sv10, *sv11, *sv12, *sv13, *sv14, *sv15, *sv16, *sv17, *sv18, *sv19, *sv20, *sv21, *sv22, *sv23, *sv24, *sv25, *sv26, *sv27, *sv28, *sv29, *sv30, *sv31, *sv32, *sv33, *sv34, *sv35, *sv36, *sv37, *sv38, *sv39, *sv40, *sv41, *sv42, *sv43, *sv44, *sv45, *sv46, *sv47, *sv48, *sv49, *sv50, *sv51, *sv52, *sv53, *sv54, *sv55, *sv56, *sv57, *sv58, *sv59, *sv60, *sv61, *sv62, *sv63, *sv64, *sv65, *sv66, *sv67, *sv68, *sv69, *sv70, *sv71, *sv72, *sv73, *sv74, *sv75, *sv76, *sv77, *sv78, *sv79, *sv80, *sv81, *sv82, *sv83, *sv84, *sv85, *sv86, *sv87, *sv88, *sv89, *sv90, *sv91, *sv92, *sv93, *sv94, *sv95, *sv96, *sv97, *sv98, *sv99, *sv100, *sv101, *sv102, *sv103, *sv104, *sv105, *sv106, *sv107, *sv108, *sv109, *sv110, *sv111, *sv112, *sv113, *sv114, *sv115, *sv116, *sv117, *sv118, *sv119, *sv120, *sv121, *sv122, *sv123, *sv124, *sv125, *sv126, *sv127, *sv128, *sv129, *sv130, *sv131, *sv132, *sv133, *sv134, *sv135, *sv136, *sv137, *sv138, *sv139, *sv140, *sv141, *sv142, *sv143, *sv144, *sv145, *sv146, *sv147, *sv148, *sv149, *sv150, *sv151, *sv152, *sv153, *sv154, *sv155, *sv156, *sv157, *sv158, *sv159, *sv160, *sv161, *sv162, *sv163, *sv164, *sv165, *sv166, *sv167, *sv168, *sv169, *sv170, *sv171, *sv172, *sv173, *sv174;
    /* rhs: implies(and(,)[not(and(,)[xor(,)[or(,)[a2[var=168, subst=-1, NOSHARE].a3[var=167, subst=-1, NOSHARE]][var=169, subst=-1, NOSHARE].a1[var=166, subst=-1, NOSHARE].a4[var=165, subst=-1, NOSHARE]][var=170, subst=-1, NOSHARE].xor(,)[iff(,)[xor(,)[or(,)[and(,)[a9[var=159, subst=-1, NOSHARE].a10[var=158, subst=-1, NOSHARE]][var=160, subst=-1, NOSHARE].xor(,)[iff(,)[a6[var=155, subst=-1, NOSHARE].a7[var=154, subst=-1, NOSHARE]][var=156, subst=-1, NOSHARE].iff(,)[a8[var=152, subst=-1, NOSHARE].a9[var=151, subst=-1, NOSHARE]][var=153, subst=-1, NOSHARE]][var=157, subst=-1, NOSHARE]][var=161, subst=-1, NOSHARE].not(a5[var=149, subst=-1, NOSHARE])[var=150, subst=-1, NOSHARE]][var=162, subst=-1, NOSHARE].not(not(a2[var=146, subst=-1, NOSHARE])[var=147, subst=-1, NOSHARE])[var=148, subst=-1, NOSHARE].implies(or(,)[a6[var=143, subst=-1, NOSHARE].a9[var=142, subst=-1, NOSHARE]][var=144, subst=-1, NOSHARE],or(,)[a5[var=140, subst=-1, NOSHARE].a10[var=139, subst=-1, NOSHARE]][var=141, subst=-1, NOSHARE])[var=145, subst=-1, NOSHARE]][var=163, subst=-1, NOSHARE].not(or(,)[implies(not(a8[var=134, subst=-1, NOSHARE])[var=135, subst=-1, NOSHARE],or(,)[a4[var=132, subst=-1, NOSHARE].a9[var=131, subst=-1, NOSHARE]][var=133, subst=-1, NOSHARE])[var=136, subst=-1, NOSHARE].a9[var=130, subst=-1, NOSHARE]][var=137, subst=-1, NOSHARE])[var=138, subst=-1, NOSHARE]][var=164, subst=-1, NOSHARE]][var=171, subst=-1, NOSHARE])[var=172, subst=-1, NOSHARE].implies(not(and(,)[xor(,)[or(,)[a2[var=124, subst=-1, NOSHARE].a3[var=123, subst=-1, NOSHARE]][var=125, subst=-1, NOSHARE].a1[var=122, subst=-1, NOSHARE].a4[var=121, subst=-1, NOSHARE]][var=126, subst=-1, NOSHARE].xor(,)[iff(,)[xor(,)[or(,)[and(,)[a9[var=115, subst=-1, NOSHARE].a10[var=114, subst=-1, NOSHARE]][var=116, subst=-1, NOSHARE].xor(,)[iff(,)[a6[var=111, subst=-1, NOSHARE].a7[var=110, subst=-1, NOSHARE]][var=112, subst=-1, NOSHARE].iff(,)[a8[var=108, subst=-1, NOSHARE].a9[var=107, subst=-1, NOSHARE]][var=109, subst=-1, NOSHARE]][var=113, subst=-1, NOSHARE]][var=117, subst=-1, NOSHARE].not(a5[var=105, subst=-1, NOSHARE])[var=106, subst=-1, NOSHARE]][var=118, subst=-1, NOSHARE].not(not(a2[var=102, subst=-1, NOSHARE])[var=103, subst=-1, NOSHARE])[var=104, subst=-1, NOSHARE].implies(or(,)[a6[var=99, subst=-1, NOSHARE].a9[var=98, subst=-1, NOSHARE]][var=100, subst=-1, NOSHARE],or(,)[a5[var=96, subst=-1, NOSHARE].a10[var=95, subst=-1, NOSHARE]][var=97, subst=-1, NOSHARE])[var=101, subst=-1, NOSHARE]][var=119, subst=-1, NOSHARE].not(or(,)[implies(not(a8[var=90, subst=-1, NOSHARE])[var=91, subst=-1, NOSHARE],or(,)[a4[var=88, subst=-1, NOSHARE].a9[var=87, subst=-1, NOSHARE]][var=89, subst=-1, NOSHARE])[var=92, subst=-1, NOSHARE].a9[var=86, subst=-1, NOSHARE]][var=93, subst=-1, NOSHARE])[var=94, subst=-1, NOSHARE]][var=120, subst=-1, NOSHARE]][var=127, subst=-1, NOSHARE])[var=128, subst=-1, NOSHARE],not(implies(implies(and(,)[or(,)[xor(,)[not(a4[var=78, subst=-1, NOSHARE])[var=79, subst=-1, NOSHARE].a2[var=77, subst=-1, NOSHARE].a3[var=76, subst=-1, NOSHARE]][var=80, subst=-1, NOSHARE].a1[var=75, subst=-1, NOSHARE]][var=81, subst=-1, NOSHARE].not(xor(,)[and(,)[a6[var=71, subst=-1, NOSHARE].a7[var=70, subst=-1, NOSHARE]][var=72, subst=-1, NOSHARE].a5[var=69, subst=-1, NOSHARE]][var=73, subst=-1, NOSHARE])[var=74, subst=-1, NOSHARE]][var=82, subst=-1, NOSHARE],implies(xor(,)[implies(a8[var=65, subst=-1, NOSHARE],a9[var=64, subst=-1, NOSHARE])[var=66, subst=-1, NOSHARE].a10[var=63, subst=-1, NOSHARE]][var=67, subst=-1, NOSHARE],xor(,)[and(,)[or(,)[a1[var=59, subst=-1, NOSHARE].a4[var=58, subst=-1, NOSHARE]][var=60, subst=-1, NOSHARE].a4[var=57, subst=-1, NOSHARE]][var=61, subst=-1, NOSHARE].a2[var=56, subst=-1, NOSHARE]][var=62, subst=-1, NOSHARE])[var=68, subst=-1, NOSHARE])[var=83, subst=-1, NOSHARE],or(,)[and(,)[a1[var=53, subst=-1, NOSHARE].a8[var=52, subst=-1, NOSHARE]][var=54, subst=-1, NOSHARE].xor(,)[or(,)[a4[var=49, subst=-1, NOSHARE].a7[var=48, subst=-1, NOSHARE]][var=50, subst=-1, NOSHARE].a2[var=47, subst=-1, NOSHARE]][var=51, subst=-1, NOSHARE].not(not(not(a6[var=43, subst=-1, NOSHARE])[var=44, subst=-1, NOSHARE])[var=45, subst=-1, NOSHARE])[var=46, subst=-1, NOSHARE]][var=55, subst=-1, NOSHARE])[var=84, subst=-1, NOSHARE])[var=85, subst=-1, NOSHARE])[var=129, subst=-1, NOSHARE]][var=173, subst=-1, NOSHARE],not(implies(implies(and(,)[or(,)[xor(,)[not(a4[var=35, subst=-1, NOSHARE])[var=36, subst=-1, NOSHARE].a2[var=34, subst=-1, NOSHARE].a3[var=33, subst=-1, NOSHARE]][var=37, subst=-1, NOSHARE].a1[var=32, subst=-1, NOSHARE]][var=38, subst=-1, NOSHARE].not(xor(,)[and(,)[a6[var=28, subst=-1, NOSHARE].a7[var=27, subst=-1, NOSHARE]][var=29, subst=-1, NOSHARE].a5[var=26, subst=-1, NOSHARE]][var=30, subst=-1, NOSHARE])[var=31, subst=-1, NOSHARE]][var=39, subst=-1, NOSHARE],implies(xor(,)[implies(a8[var=22, subst=-1, NOSHARE],a9[var=21, subst=-1, NOSHARE])[var=23, subst=-1, NOSHARE].a10[var=20, subst=-1, NOSHARE]][var=24, subst=-1, NOSHARE],xor(,)[and(,)[or(,)[a1[var=16, subst=-1, NOSHARE].a4[var=15, subst=-1, NOSHARE]][var=17, subst=-1, NOSHARE].a4[var=14, subst=-1, NOSHARE]][var=18, subst=-1, NOSHARE].a2[var=13, subst=-1, NOSHARE]][var=19, subst=-1, NOSHARE])[var=25, subst=-1, NOSHARE])[var=40, subst=-1, NOSHARE],or(,)[and(,)[a1[var=10, subst=-1, NOSHARE].a8[var=9, subst=-1, NOSHARE]][var=11, subst=-1, NOSHARE].xor(,)[or(,)[a4[var=6, subst=-1, NOSHARE].a7[var=5, subst=-1, NOSHARE]][var=7, subst=-1, NOSHARE].a2[var=4, subst=-1, NOSHARE]][var=8, subst=-1, NOSHARE].not(not(not(a6[var=0, subst=-1, NOSHARE])[var=1, subst=-1, NOSHARE])[var=2, subst=-1, NOSHARE])[var=3, subst=-1, NOSHARE]][var=12, subst=-1, NOSHARE])[var=41, subst=-1, NOSHARE])[var=42, subst=-1, NOSHARE])[var=174, subst=-1, NOSHARE] */
    sv1 = fun_206( con_213 );
    sv2 = fun_206( sv1 );
    sv3 = fun_206( sv2 );
    TERM_ALLOC(sv7,term2,code_204);
    term_add_onf_term(sv7,con_211);
    term_add_onf_term(sv7,con_214);
    sv7 = fun_204( sv7 );
    TERM_ALLOC(sv8,term2,code_203);
    term_add_onf_term(sv8,sv7);
    term_add_onf_term(sv8,con_209);
    sv8 = fun_203( sv8 );
    TERM_ALLOC(sv11,term2,code_202);
    term_add_onf_term(sv11,con_208);
    term_add_onf_term(sv11,con_215);
    sv11 = fun_202( sv11 );
    TERM_ALLOC(sv12,term2,code_204);
    term_add_onf_term(sv12,sv11);
    term_add_onf_term(sv12,sv8);
    term_add_onf_term(sv12,sv3);
    sv12 = fun_204( sv12 );
    TERM_ALLOC(sv17,term2,code_204);
    term_add_onf_term(sv17,con_208);
    term_add_onf_term(sv17,con_211);
    sv17 = fun_204( sv17 );
    TERM_ALLOC(sv18,term2,code_202);
    term_add_onf_term(sv18,sv17);
    term_add_onf_term(sv18,con_211);
    sv18 = fun_202( sv18 );
    TERM_ALLOC(sv19,term2,code_203);
    term_add_onf_term(sv19,sv18);
    term_add_onf_term(sv19,con_209);
    sv19 = fun_203( sv19 );
    sv23 = fun_207( con_215,con_216 );
    TERM_ALLOC(sv24,term2,code_203);
    term_add_onf_term(sv24,sv23);
    term_add_onf_term(sv24,con_217);
    sv24 = fun_203( sv24 );
    sv25 = fun_207( sv24,sv19 );
    TERM_ALLOC(sv29,term2,code_202);
    term_add_onf_term(sv29,con_213);
    term_add_onf_term(sv29,con_214);
    sv29 = fun_202( sv29 );
    TERM_ALLOC(sv30,term2,code_203);
    term_add_onf_term(sv30,sv29);
    term_add_onf_term(sv30,con_212);
    sv30 = fun_203( sv30 );
    sv31 = fun_206( sv30 );
    sv36 = fun_206( con_211 );
    TERM_ALLOC(sv37,term2,code_203);
    term_add_onf_term(sv37,sv36);
    term_add_onf_term(sv37,con_209);
    term_add_onf_term(sv37,con_210);
    sv37 = fun_203( sv37 );
    TERM_ALLOC(sv38,term2,code_204);
    term_add_onf_term(sv38,sv37);
    term_add_onf_term(sv38,con_208);
    sv38 = fun_204( sv38 );
    TERM_ALLOC(sv39,term2,code_202);
    term_add_onf_term(sv39,sv38);
    term_add_onf_term(sv39,sv31);
    sv39 = fun_202( sv39 );
    sv40 = fun_207( sv39,sv25 );
    sv41 = fun_207( sv40,sv12 );
    sv42 = fun_206( sv41 );
    sv44 = fun_206( con_213 );
    sv45 = fun_206( sv44 );
    sv46 = fun_206( sv45 );
    TERM_ALLOC(sv50,term2,code_204);
    term_add_onf_term(sv50,con_211);
    term_add_onf_term(sv50,con_214);
    sv50 = fun_204( sv50 );
    TERM_ALLOC(sv51,term2,code_203);
    term_add_onf_term(sv51,sv50);
    term_add_onf_term(sv51,con_209);
    sv51 = fun_203( sv51 );
    TERM_ALLOC(sv54,term2,code_202);
    term_add_onf_term(sv54,con_208);
    term_add_onf_term(sv54,con_215);
    sv54 = fun_202( sv54 );
    TERM_ALLOC(sv55,term2,code_204);
    term_add_onf_term(sv55,sv54);
    term_add_onf_term(sv55,sv51);
    term_add_onf_term(sv55,sv46);
    sv55 = fun_204( sv55 );
    TERM_ALLOC(sv60,term2,code_204);
    term_add_onf_term(sv60,con_208);
    term_add_onf_term(sv60,con_211);
    sv60 = fun_204( sv60 );
    TERM_ALLOC(sv61,term2,code_202);
    term_add_onf_term(sv61,sv60);
    term_add_onf_term(sv61,con_211);
    sv61 = fun_202( sv61 );
    TERM_ALLOC(sv62,term2,code_203);
    term_add_onf_term(sv62,sv61);
    term_add_onf_term(sv62,con_209);
    sv62 = fun_203( sv62 );
    sv66 = fun_207( con_215,con_216 );
    TERM_ALLOC(sv67,term2,code_203);
    term_add_onf_term(sv67,sv66);
    term_add_onf_term(sv67,con_217);
    sv67 = fun_203( sv67 );
    sv68 = fun_207( sv67,sv62 );
    TERM_ALLOC(sv72,term2,code_202);
    term_add_onf_term(sv72,con_213);
    term_add_onf_term(sv72,con_214);
    sv72 = fun_202( sv72 );
    TERM_ALLOC(sv73,term2,code_203);
    term_add_onf_term(sv73,sv72);
    term_add_onf_term(sv73,con_212);
    sv73 = fun_203( sv73 );
    sv74 = fun_206( sv73 );
    sv79 = fun_206( con_211 );
    TERM_ALLOC(sv80,term2,code_203);
    term_add_onf_term(sv80,sv79);
    term_add_onf_term(sv80,con_209);
    term_add_onf_term(sv80,con_210);
    sv80 = fun_203( sv80 );
    TERM_ALLOC(sv81,term2,code_204);
    term_add_onf_term(sv81,sv80);
    term_add_onf_term(sv81,con_208);
    sv81 = fun_204( sv81 );
    TERM_ALLOC(sv82,term2,code_202);
    term_add_onf_term(sv82,sv81);
    term_add_onf_term(sv82,sv74);
    sv82 = fun_202( sv82 );
    sv83 = fun_207( sv82,sv68 );
    sv84 = fun_207( sv83,sv55 );
    sv85 = fun_206( sv84 );
    TERM_ALLOC(sv89,term2,code_204);
    term_add_onf_term(sv89,con_211);
    term_add_onf_term(sv89,con_216);
    sv89 = fun_204( sv89 );
    sv91 = fun_206( con_215 );
    sv92 = fun_207( sv91,sv89 );
    TERM_ALLOC(sv93,term2,code_204);
    term_add_onf_term(sv93,sv92);
    term_add_onf_term(sv93,con_216);
    sv93 = fun_204( sv93 );
    sv94 = fun_206( sv93 );
    TERM_ALLOC(sv97,term2,code_204);
    term_add_onf_term(sv97,con_212);
    term_add_onf_term(sv97,con_217);
    sv97 = fun_204( sv97 );
    TERM_ALLOC(sv100,term2,code_204);
    term_add_onf_term(sv100,con_213);
    term_add_onf_term(sv100,con_216);
    sv100 = fun_204( sv100 );
    sv101 = fun_207( sv100,sv97 );
    sv103 = fun_206( con_209 );
    sv104 = fun_206( sv103 );
    sv106 = fun_206( con_212 );
    TERM_ALLOC(sv109,term2,code_205);
    term_add_onf_term(sv109,con_215);
    term_add_onf_term(sv109,con_216);
    sv109 = fun_205( sv109 );
    TERM_ALLOC(sv112,term2,code_205);
    term_add_onf_term(sv112,con_213);
    term_add_onf_term(sv112,con_214);
    sv112 = fun_205( sv112 );
    TERM_ALLOC(sv113,term2,code_203);
    term_add_onf_term(sv113,sv112);
    term_add_onf_term(sv113,sv109);
    sv113 = fun_203( sv113 );
    TERM_ALLOC(sv116,term2,code_202);
    term_add_onf_term(sv116,con_216);
    term_add_onf_term(sv116,con_217);
    sv116 = fun_202( sv116 );
    TERM_ALLOC(sv117,term2,code_204);
    term_add_onf_term(sv117,sv116);
    term_add_onf_term(sv117,sv113);
    sv117 = fun_204( sv117 );
    TERM_ALLOC(sv118,term2,code_203);
    term_add_onf_term(sv118,sv117);
    term_add_onf_term(sv118,sv106);
    sv118 = fun_203( sv118 );
    TERM_ALLOC(sv119,term2,code_205);
    term_add_onf_term(sv119,sv118);
    term_add_onf_term(sv119,sv104);
    term_add_onf_term(sv119,sv101);
    sv119 = fun_205( sv119 );
    TERM_ALLOC(sv120,term2,code_203);
    term_add_onf_term(sv120,sv119);
    term_add_onf_term(sv120,sv94);
    sv120 = fun_203( sv120 );
    TERM_ALLOC(sv125,term2,code_204);
    term_add_onf_term(sv125,con_209);
    term_add_onf_term(sv125,con_210);
    sv125 = fun_204( sv125 );
    TERM_ALLOC(sv126,term2,code_203);
    term_add_onf_term(sv126,sv125);
    term_add_onf_term(sv126,con_208);
    term_add_onf_term(sv126,con_211);
    sv126 = fun_203( sv126 );
    TERM_ALLOC(sv127,term2,code_202);
    term_add_onf_term(sv127,sv126);
    term_add_onf_term(sv127,sv120);
    sv127 = fun_202( sv127 );
    sv128 = fun_206( sv127 );
    sv129 = fun_207( sv128,sv85 );
    TERM_ALLOC(sv133,term2,code_204);
    term_add_onf_term(sv133,con_211);
    term_add_onf_term(sv133,con_216);
    sv133 = fun_204( sv133 );
    sv135 = fun_206( con_215 );
    sv136 = fun_207( sv135,sv133 );
    TERM_ALLOC(sv137,term2,code_204);
    term_add_onf_term(sv137,sv136);
    term_add_onf_term(sv137,con_216);
    sv137 = fun_204( sv137 );
    sv138 = fun_206( sv137 );
    TERM_ALLOC(sv141,term2,code_204);
    term_add_onf_term(sv141,con_212);
    term_add_onf_term(sv141,con_217);
    sv141 = fun_204( sv141 );
    TERM_ALLOC(sv144,term2,code_204);
    term_add_onf_term(sv144,con_213);
    term_add_onf_term(sv144,con_216);
    sv144 = fun_204( sv144 );
    sv145 = fun_207( sv144,sv141 );
    sv147 = fun_206( con_209 );
    sv148 = fun_206( sv147 );
    sv150 = fun_206( con_212 );
    TERM_ALLOC(sv153,term2,code_205);
    term_add_onf_term(sv153,con_215);
    term_add_onf_term(sv153,con_216);
    sv153 = fun_205( sv153 );
    TERM_ALLOC(sv156,term2,code_205);
    term_add_onf_term(sv156,con_213);
    term_add_onf_term(sv156,con_214);
    sv156 = fun_205( sv156 );
    TERM_ALLOC(sv157,term2,code_203);
    term_add_onf_term(sv157,sv156);
    term_add_onf_term(sv157,sv153);
    sv157 = fun_203( sv157 );
    TERM_ALLOC(sv160,term2,code_202);
    term_add_onf_term(sv160,con_216);
    term_add_onf_term(sv160,con_217);
    sv160 = fun_202( sv160 );
    TERM_ALLOC(sv161,term2,code_204);
    term_add_onf_term(sv161,sv160);
    term_add_onf_term(sv161,sv157);
    sv161 = fun_204( sv161 );
    TERM_ALLOC(sv162,term2,code_203);
    term_add_onf_term(sv162,sv161);
    term_add_onf_term(sv162,sv150);
    sv162 = fun_203( sv162 );
    TERM_ALLOC(sv163,term2,code_205);
    term_add_onf_term(sv163,sv162);
    term_add_onf_term(sv163,sv148);
    term_add_onf_term(sv163,sv145);
    sv163 = fun_205( sv163 );
    TERM_ALLOC(sv164,term2,code_203);
    term_add_onf_term(sv164,sv163);
    term_add_onf_term(sv164,sv138);
    sv164 = fun_203( sv164 );
    TERM_ALLOC(sv169,term2,code_204);
    term_add_onf_term(sv169,con_209);
    term_add_onf_term(sv169,con_210);
    sv169 = fun_204( sv169 );
    TERM_ALLOC(sv170,term2,code_203);
    term_add_onf_term(sv170,sv169);
    term_add_onf_term(sv170,con_208);
    term_add_onf_term(sv170,con_211);
    sv170 = fun_203( sv170 );
    TERM_ALLOC(sv171,term2,code_202);
    term_add_onf_term(sv171,sv170);
    term_add_onf_term(sv171,sv164);
    sv171 = fun_202( sv171 );
    sv172 = fun_206( sv171 );
    TERM_ALLOC(sv173,term2,code_202);
    term_add_onf_term(sv173,sv172);
    term_add_onf_term(sv173,sv129);
    sv173 = fun_202( sv173 );
    sv174 = fun_207( sv173,sv42 );
    res = sv174 ;
    goto end;
    myend3:;
  }
match_fail:
  fprintf(stderr,"Match Fail error in fun_227\n");
  exit(0);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(trace>=1) {
    indent(indentlevel);
    printf("rewrite[%d] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_delete(mask);
  subindent();
  return res;
}

struct term* fun_228( ) {
  struct term *v1,*v2;
  bitSet32 *mask;
  struct term *res;
  bitSet32_create(mask,1);
  bitSet32_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_228(");
    printf(")\n");
  }
  /* Begin syntactical matching */
  bitSet32_set(mask,0);
  /* End syntactical matching */
  if(bitSet32_get(mask,0)) {
    /* lhs: q3[mv][var=0, subst=-1, NOSHARE] */
    struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5, *sv6, *sv7, *sv8, *sv9, *sv10, *sv11, *sv12, *sv13, *sv14, *sv15, *sv16, *sv17, *sv18, *sv19, *sv20, *sv21, *sv22, *sv23, *sv24, *sv25, *sv26, *sv27, *sv28, *sv29, *sv30, *sv31, *sv32, *sv33, *sv34, *sv35, *sv36, *sv37, *sv38, *sv39, *sv40, *sv41, *sv42, *sv43, *sv44, *sv45, *sv46, *sv47, *sv48, *sv49, *sv50, *sv51, *sv52, *sv53, *sv54, *sv55, *sv56, *sv57, *sv58, *sv59, *sv60, *sv61, *sv62, *sv63, *sv64, *sv65, *sv66, *sv67, *sv68, *sv69, *sv70, *sv71, *sv72, *sv73, *sv74, *sv75, *sv76, *sv77, *sv78, *sv79, *sv80, *sv81, *sv82, *sv83, *sv84, *sv85, *sv86, *sv87, *sv88, *sv89, *sv90, *sv91, *sv92, *sv93, *sv94, *sv95, *sv96, *sv97, *sv98, *sv99, *sv100, *sv101, *sv102, *sv103, *sv104, *sv105, *sv106, *sv107, *sv108, *sv109, *sv110, *sv111, *sv112, *sv113, *sv114, *sv115, *sv116, *sv117, *sv118, *sv119, *sv120, *sv121, *sv122, *sv123, *sv124, *sv125, *sv126, *sv127, *sv128, *sv129, *sv130, *sv131, *sv132, *sv133, *sv134, *sv135, *sv136, *sv137, *sv138, *sv139, *sv140, *sv141, *sv142, *sv143, *sv144, *sv145, *sv146, *sv147, *sv148, *sv149, *sv150, *sv151, *sv152, *sv153, *sv154, *sv155, *sv156, *sv157, *sv158, *sv159, *sv160, *sv161, *sv162, *sv163, *sv164, *sv165, *sv166, *sv167, *sv168, *sv169, *sv170, *sv171, *sv172, *sv173, *sv174;
    /* rhs: implies(and(,)[not(and(,)[xor(,)[or(,)[a2[var=168, subst=-1, NOSHARE].a3[var=167, subst=-1, NOSHARE]][var=169, subst=-1, NOSHARE].a1[var=166, subst=-1, NOSHARE].a4[var=165, subst=-1, NOSHARE]][var=170, subst=-1, NOSHARE].xor(,)[iff(,)[xor(,)[or(,)[and(,)[a10[var=159, subst=-1, NOSHARE].a11[var=158, subst=-1, NOSHARE]][var=160, subst=-1, NOSHARE].xor(,)[iff(,)[a6[var=155, subst=-1, NOSHARE].a7[var=154, subst=-1, NOSHARE]][var=156, subst=-1, NOSHARE].iff(,)[a8[var=152, subst=-1, NOSHARE].a9[var=151, subst=-1, NOSHARE]][var=153, subst=-1, NOSHARE]][var=157, subst=-1, NOSHARE]][var=161, subst=-1, NOSHARE].not(a5[var=149, subst=-1, NOSHARE])[var=150, subst=-1, NOSHARE]][var=162, subst=-1, NOSHARE].implies(or(,)[and(,)[iff(,)[a1[var=144, subst=-1, NOSHARE].a2[var=143, subst=-1, NOSHARE]][var=145, subst=-1, NOSHARE].a3[var=142, subst=-1, NOSHARE]][var=146, subst=-1, NOSHARE].a4[var=141, subst=-1, NOSHARE]][var=147, subst=-1, NOSHARE],not(not(a4[var=138, subst=-1, NOSHARE])[var=139, subst=-1, NOSHARE])[var=140, subst=-1, NOSHARE])[var=148, subst=-1, NOSHARE]][var=163, subst=-1, NOSHARE].not(a9[var=136, subst=-1, NOSHARE])[var=137, subst=-1, NOSHARE].implies(implies(a6[var=133, subst=-1, NOSHARE],a1[var=132, subst=-1, NOSHARE])[var=134, subst=-1, NOSHARE],not(a1[var=130, subst=-1, NOSHARE])[var=131, subst=-1, NOSHARE])[var=135, subst=-1, NOSHARE]][var=164, subst=-1, NOSHARE]][var=171, subst=-1, NOSHARE])[var=172, subst=-1, NOSHARE].implies(not(and(,)[xor(,)[or(,)[a2[var=124, subst=-1, NOSHARE].a3[var=123, subst=-1, NOSHARE]][var=125, subst=-1, NOSHARE].a1[var=122, subst=-1, NOSHARE].a4[var=121, subst=-1, NOSHARE]][var=126, subst=-1, NOSHARE].xor(,)[iff(,)[xor(,)[or(,)[and(,)[a10[var=115, subst=-1, NOSHARE].a11[var=114, subst=-1, NOSHARE]][var=116, subst=-1, NOSHARE].xor(,)[iff(,)[a6[var=111, subst=-1, NOSHARE].a7[var=110, subst=-1, NOSHARE]][var=112, subst=-1, NOSHARE].iff(,)[a8[var=108, subst=-1, NOSHARE].a9[var=107, subst=-1, NOSHARE]][var=109, subst=-1, NOSHARE]][var=113, subst=-1, NOSHARE]][var=117, subst=-1, NOSHARE].not(a5[var=105, subst=-1, NOSHARE])[var=106, subst=-1, NOSHARE]][var=118, subst=-1, NOSHARE].implies(or(,)[and(,)[iff(,)[a1[var=100, subst=-1, NOSHARE].a2[var=99, subst=-1, NOSHARE]][var=101, subst=-1, NOSHARE].a3[var=98, subst=-1, NOSHARE]][var=102, subst=-1, NOSHARE].a4[var=97, subst=-1, NOSHARE]][var=103, subst=-1, NOSHARE],not(not(a4[var=94, subst=-1, NOSHARE])[var=95, subst=-1, NOSHARE])[var=96, subst=-1, NOSHARE])[var=104, subst=-1, NOSHARE]][var=119, subst=-1, NOSHARE].not(a9[var=92, subst=-1, NOSHARE])[var=93, subst=-1, NOSHARE].implies(implies(a6[var=89, subst=-1, NOSHARE],a1[var=88, subst=-1, NOSHARE])[var=90, subst=-1, NOSHARE],not(a1[var=86, subst=-1, NOSHARE])[var=87, subst=-1, NOSHARE])[var=91, subst=-1, NOSHARE]][var=120, subst=-1, NOSHARE]][var=127, subst=-1, NOSHARE])[var=128, subst=-1, NOSHARE],not(implies(implies(and(,)[or(,)[xor(,)[not(a4[var=78, subst=-1, NOSHARE])[var=79, subst=-1, NOSHARE].a2[var=77, subst=-1, NOSHARE].a3[var=76, subst=-1, NOSHARE]][var=80, subst=-1, NOSHARE].a1[var=75, subst=-1, NOSHARE]][var=81, subst=-1, NOSHARE].not(xor(,)[and(,)[a6[var=71, subst=-1, NOSHARE].a7[var=70, subst=-1, NOSHARE]][var=72, subst=-1, NOSHARE].a5[var=69, subst=-1, NOSHARE]][var=73, subst=-1, NOSHARE])[var=74, subst=-1, NOSHARE]][var=82, subst=-1, NOSHARE],implies(xor(,)[implies(a8[var=65, subst=-1, NOSHARE],a9[var=64, subst=-1, NOSHARE])[var=66, subst=-1, NOSHARE].a10[var=63, subst=-1, NOSHARE]][var=67, subst=-1, NOSHARE],xor(,)[and(,)[implies(a2[var=59, subst=-1, NOSHARE],a8[var=58, subst=-1, NOSHARE])[var=60, subst=-1, NOSHARE].a11[var=57, subst=-1, NOSHARE]][var=61, subst=-1, NOSHARE].a8[var=56, subst=-1, NOSHARE]][var=62, subst=-1, NOSHARE])[var=68, subst=-1, NOSHARE])[var=83, subst=-1, NOSHARE],not(or(,)[not(a7[var=52, subst=-1, NOSHARE])[var=53, subst=-1, NOSHARE].implies(or(,)[and(,)[a8[var=48, subst=-1, NOSHARE].a9[var=47, subst=-1, NOSHARE]][var=49, subst=-1, NOSHARE].a5[var=46, subst=-1, NOSHARE].a8[var=45, subst=-1, NOSHARE]][var=50, subst=-1, NOSHARE],not(a2[var=43, subst=-1, NOSHARE])[var=44, subst=-1, NOSHARE])[var=51, subst=-1, NOSHARE]][var=54, subst=-1, NOSHARE])[var=55, subst=-1, NOSHARE])[var=84, subst=-1, NOSHARE])[var=85, subst=-1, NOSHARE])[var=129, subst=-1, NOSHARE]][var=173, subst=-1, NOSHARE],not(implies(implies(and(,)[or(,)[xor(,)[not(a4[var=35, subst=-1, NOSHARE])[var=36, subst=-1, NOSHARE].a2[var=34, subst=-1, NOSHARE].a3[var=33, subst=-1, NOSHARE]][var=37, subst=-1, NOSHARE].a1[var=32, subst=-1, NOSHARE]][var=38, subst=-1, NOSHARE].not(xor(,)[and(,)[a6[var=28, subst=-1, NOSHARE].a7[var=27, subst=-1, NOSHARE]][var=29, subst=-1, NOSHARE].a5[var=26, subst=-1, NOSHARE]][var=30, subst=-1, NOSHARE])[var=31, subst=-1, NOSHARE]][var=39, subst=-1, NOSHARE],implies(xor(,)[implies(a8[var=22, subst=-1, NOSHARE],a9[var=21, subst=-1, NOSHARE])[var=23, subst=-1, NOSHARE].a10[var=20, subst=-1, NOSHARE]][var=24, subst=-1, NOSHARE],xor(,)[and(,)[implies(a2[var=16, subst=-1, NOSHARE],a8[var=15, subst=-1, NOSHARE])[var=17, subst=-1, NOSHARE].a11[var=14, subst=-1, NOSHARE]][var=18, subst=-1, NOSHARE].a8[var=13, subst=-1, NOSHARE]][var=19, subst=-1, NOSHARE])[var=25, subst=-1, NOSHARE])[var=40, subst=-1, NOSHARE],not(or(,)[not(a7[var=9, subst=-1, NOSHARE])[var=10, subst=-1, NOSHARE].implies(or(,)[and(,)[a8[var=5, subst=-1, NOSHARE].a9[var=4, subst=-1, NOSHARE]][var=6, subst=-1, NOSHARE].a5[var=3, subst=-1, NOSHARE].a8[var=2, subst=-1, NOSHARE]][var=7, subst=-1, NOSHARE],not(a2[var=0, subst=-1, NOSHARE])[var=1, subst=-1, NOSHARE])[var=8, subst=-1, NOSHARE]][var=11, subst=-1, NOSHARE])[var=12, subst=-1, NOSHARE])[var=41, subst=-1, NOSHARE])[var=42, subst=-1, NOSHARE])[var=174, subst=-1, NOSHARE] */
    sv1 = fun_206( con_209 );
    TERM_ALLOC(sv6,term2,code_202);
    term_add_onf_term(sv6,con_215);
    term_add_onf_term(sv6,con_216);
    sv6 = fun_202( sv6 );
    TERM_ALLOC(sv7,term2,code_204);
    term_add_onf_term(sv7,sv6);
    term_add_onf_term(sv7,con_212);
    term_add_onf_term(sv7,con_215);
    sv7 = fun_204( sv7 );
    sv8 = fun_207( sv7,sv1 );
    sv10 = fun_206( con_214 );
    TERM_ALLOC(sv11,term2,code_204);
    term_add_onf_term(sv11,sv10);
    term_add_onf_term(sv11,sv8);
    sv11 = fun_204( sv11 );
    sv12 = fun_206( sv11 );
    sv17 = fun_207( con_209,con_215 );
    TERM_ALLOC(sv18,term2,code_202);
    term_add_onf_term(sv18,sv17);
    term_add_onf_term(sv18,con_218);
    sv18 = fun_202( sv18 );
    TERM_ALLOC(sv19,term2,code_203);
    term_add_onf_term(sv19,sv18);
    term_add_onf_term(sv19,con_215);
    sv19 = fun_203( sv19 );
    sv23 = fun_207( con_215,con_216 );
    TERM_ALLOC(sv24,term2,code_203);
    term_add_onf_term(sv24,sv23);
    term_add_onf_term(sv24,con_217);
    sv24 = fun_203( sv24 );
    sv25 = fun_207( sv24,sv19 );
    TERM_ALLOC(sv29,term2,code_202);
    term_add_onf_term(sv29,con_213);
    term_add_onf_term(sv29,con_214);
    sv29 = fun_202( sv29 );
    TERM_ALLOC(sv30,term2,code_203);
    term_add_onf_term(sv30,sv29);
    term_add_onf_term(sv30,con_212);
    sv30 = fun_203( sv30 );
    sv31 = fun_206( sv30 );
    sv36 = fun_206( con_211 );
    TERM_ALLOC(sv37,term2,code_203);
    term_add_onf_term(sv37,sv36);
    term_add_onf_term(sv37,con_209);
    term_add_onf_term(sv37,con_210);
    sv37 = fun_203( sv37 );
    TERM_ALLOC(sv38,term2,code_204);
    term_add_onf_term(sv38,sv37);
    term_add_onf_term(sv38,con_208);
    sv38 = fun_204( sv38 );
    TERM_ALLOC(sv39,term2,code_202);
    term_add_onf_term(sv39,sv38);
    term_add_onf_term(sv39,sv31);
    sv39 = fun_202( sv39 );
    sv40 = fun_207( sv39,sv25 );
    sv41 = fun_207( sv40,sv12 );
    sv42 = fun_206( sv41 );
    sv44 = fun_206( con_209 );
    TERM_ALLOC(sv49,term2,code_202);
    term_add_onf_term(sv49,con_215);
    term_add_onf_term(sv49,con_216);
    sv49 = fun_202( sv49 );
    TERM_ALLOC(sv50,term2,code_204);
    term_add_onf_term(sv50,sv49);
    term_add_onf_term(sv50,con_212);
    term_add_onf_term(sv50,con_215);
    sv50 = fun_204( sv50 );
    sv51 = fun_207( sv50,sv44 );
    sv53 = fun_206( con_214 );
    TERM_ALLOC(sv54,term2,code_204);
    term_add_onf_term(sv54,sv53);
    term_add_onf_term(sv54,sv51);
    sv54 = fun_204( sv54 );
    sv55 = fun_206( sv54 );
    sv60 = fun_207( con_209,con_215 );
    TERM_ALLOC(sv61,term2,code_202);
    term_add_onf_term(sv61,sv60);
    term_add_onf_term(sv61,con_218);
    sv61 = fun_202( sv61 );
    TERM_ALLOC(sv62,term2,code_203);
    term_add_onf_term(sv62,sv61);
    term_add_onf_term(sv62,con_215);
    sv62 = fun_203( sv62 );
    sv66 = fun_207( con_215,con_216 );
    TERM_ALLOC(sv67,term2,code_203);
    term_add_onf_term(sv67,sv66);
    term_add_onf_term(sv67,con_217);
    sv67 = fun_203( sv67 );
    sv68 = fun_207( sv67,sv62 );
    TERM_ALLOC(sv72,term2,code_202);
    term_add_onf_term(sv72,con_213);
    term_add_onf_term(sv72,con_214);
    sv72 = fun_202( sv72 );
    TERM_ALLOC(sv73,term2,code_203);
    term_add_onf_term(sv73,sv72);
    term_add_onf_term(sv73,con_212);
    sv73 = fun_203( sv73 );
    sv74 = fun_206( sv73 );
    sv79 = fun_206( con_211 );
    TERM_ALLOC(sv80,term2,code_203);
    term_add_onf_term(sv80,sv79);
    term_add_onf_term(sv80,con_209);
    term_add_onf_term(sv80,con_210);
    sv80 = fun_203( sv80 );
    TERM_ALLOC(sv81,term2,code_204);
    term_add_onf_term(sv81,sv80);
    term_add_onf_term(sv81,con_208);
    sv81 = fun_204( sv81 );
    TERM_ALLOC(sv82,term2,code_202);
    term_add_onf_term(sv82,sv81);
    term_add_onf_term(sv82,sv74);
    sv82 = fun_202( sv82 );
    sv83 = fun_207( sv82,sv68 );
    sv84 = fun_207( sv83,sv55 );
    sv85 = fun_206( sv84 );
    sv87 = fun_206( con_208 );
    sv90 = fun_207( con_213,con_208 );
    sv91 = fun_207( sv90,sv87 );
    sv93 = fun_206( con_216 );
    sv95 = fun_206( con_211 );
    sv96 = fun_206( sv95 );
    TERM_ALLOC(sv101,term2,code_205);
    term_add_onf_term(sv101,con_208);
    term_add_onf_term(sv101,con_209);
    sv101 = fun_205( sv101 );
    TERM_ALLOC(sv102,term2,code_202);
    term_add_onf_term(sv102,sv101);
    term_add_onf_term(sv102,con_210);
    sv102 = fun_202( sv102 );
    TERM_ALLOC(sv103,term2,code_204);
    term_add_onf_term(sv103,sv102);
    term_add_onf_term(sv103,con_211);
    sv103 = fun_204( sv103 );
    sv104 = fun_207( sv103,sv96 );
    sv106 = fun_206( con_212 );
    TERM_ALLOC(sv109,term2,code_205);
    term_add_onf_term(sv109,con_215);
    term_add_onf_term(sv109,con_216);
    sv109 = fun_205( sv109 );
    TERM_ALLOC(sv112,term2,code_205);
    term_add_onf_term(sv112,con_213);
    term_add_onf_term(sv112,con_214);
    sv112 = fun_205( sv112 );
    TERM_ALLOC(sv113,term2,code_203);
    term_add_onf_term(sv113,sv112);
    term_add_onf_term(sv113,sv109);
    sv113 = fun_203( sv113 );
    TERM_ALLOC(sv116,term2,code_202);
    term_add_onf_term(sv116,con_217);
    term_add_onf_term(sv116,con_218);
    sv116 = fun_202( sv116 );
    TERM_ALLOC(sv117,term2,code_204);
    term_add_onf_term(sv117,sv116);
    term_add_onf_term(sv117,sv113);
    sv117 = fun_204( sv117 );
    TERM_ALLOC(sv118,term2,code_203);
    term_add_onf_term(sv118,sv117);
    term_add_onf_term(sv118,sv106);
    sv118 = fun_203( sv118 );
    TERM_ALLOC(sv119,term2,code_205);
    term_add_onf_term(sv119,sv118);
    term_add_onf_term(sv119,sv104);
    sv119 = fun_205( sv119 );
    TERM_ALLOC(sv120,term2,code_203);
    term_add_onf_term(sv120,sv119);
    term_add_onf_term(sv120,sv93);
    term_add_onf_term(sv120,sv91);
    sv120 = fun_203( sv120 );
    TERM_ALLOC(sv125,term2,code_204);
    term_add_onf_term(sv125,con_209);
    term_add_onf_term(sv125,con_210);
    sv125 = fun_204( sv125 );
    TERM_ALLOC(sv126,term2,code_203);
    term_add_onf_term(sv126,sv125);
    term_add_onf_term(sv126,con_208);
    term_add_onf_term(sv126,con_211);
    sv126 = fun_203( sv126 );
    TERM_ALLOC(sv127,term2,code_202);
    term_add_onf_term(sv127,sv126);
    term_add_onf_term(sv127,sv120);
    sv127 = fun_202( sv127 );
    sv128 = fun_206( sv127 );
    sv129 = fun_207( sv128,sv85 );
    sv131 = fun_206( con_208 );
    sv134 = fun_207( con_213,con_208 );
    sv135 = fun_207( sv134,sv131 );
    sv137 = fun_206( con_216 );
    sv139 = fun_206( con_211 );
    sv140 = fun_206( sv139 );
    TERM_ALLOC(sv145,term2,code_205);
    term_add_onf_term(sv145,con_208);
    term_add_onf_term(sv145,con_209);
    sv145 = fun_205( sv145 );
    TERM_ALLOC(sv146,term2,code_202);
    term_add_onf_term(sv146,sv145);
    term_add_onf_term(sv146,con_210);
    sv146 = fun_202( sv146 );
    TERM_ALLOC(sv147,term2,code_204);
    term_add_onf_term(sv147,sv146);
    term_add_onf_term(sv147,con_211);
    sv147 = fun_204( sv147 );
    sv148 = fun_207( sv147,sv140 );
    sv150 = fun_206( con_212 );
    TERM_ALLOC(sv153,term2,code_205);
    term_add_onf_term(sv153,con_215);
    term_add_onf_term(sv153,con_216);
    sv153 = fun_205( sv153 );
    TERM_ALLOC(sv156,term2,code_205);
    term_add_onf_term(sv156,con_213);
    term_add_onf_term(sv156,con_214);
    sv156 = fun_205( sv156 );
    TERM_ALLOC(sv157,term2,code_203);
    term_add_onf_term(sv157,sv156);
    term_add_onf_term(sv157,sv153);
    sv157 = fun_203( sv157 );
    TERM_ALLOC(sv160,term2,code_202);
    term_add_onf_term(sv160,con_217);
    term_add_onf_term(sv160,con_218);
    sv160 = fun_202( sv160 );
    TERM_ALLOC(sv161,term2,code_204);
    term_add_onf_term(sv161,sv160);
    term_add_onf_term(sv161,sv157);
    sv161 = fun_204( sv161 );
    TERM_ALLOC(sv162,term2,code_203);
    term_add_onf_term(sv162,sv161);
    term_add_onf_term(sv162,sv150);
    sv162 = fun_203( sv162 );
    TERM_ALLOC(sv163,term2,code_205);
    term_add_onf_term(sv163,sv162);
    term_add_onf_term(sv163,sv148);
    sv163 = fun_205( sv163 );
    TERM_ALLOC(sv164,term2,code_203);
    term_add_onf_term(sv164,sv163);
    term_add_onf_term(sv164,sv137);
    term_add_onf_term(sv164,sv135);
    sv164 = fun_203( sv164 );
    TERM_ALLOC(sv169,term2,code_204);
    term_add_onf_term(sv169,con_209);
    term_add_onf_term(sv169,con_210);
    sv169 = fun_204( sv169 );
    TERM_ALLOC(sv170,term2,code_203);
    term_add_onf_term(sv170,sv169);
    term_add_onf_term(sv170,con_208);
    term_add_onf_term(sv170,con_211);
    sv170 = fun_203( sv170 );
    TERM_ALLOC(sv171,term2,code_202);
    term_add_onf_term(sv171,sv170);
    term_add_onf_term(sv171,sv164);
    sv171 = fun_202( sv171 );
    sv172 = fun_206( sv171 );
    TERM_ALLOC(sv173,term2,code_202);
    term_add_onf_term(sv173,sv172);
    term_add_onf_term(sv173,sv129);
    sv173 = fun_202( sv173 );
    sv174 = fun_207( sv173,sv42 );
    res = sv174 ;
    goto end;
    myend4:;
  }
match_fail:
  fprintf(stderr,"Match Fail error in fun_228\n");
  exit(0);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(trace>=1) {
    indent(indentlevel);
    printf("rewrite[%d] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_delete(mask);
  subindent();
  return res;
}
int match_subterm_203(struct term *v0,int no_arg_subject, bitSet32 *mask, BG *cbg);
void variable_extract_203(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern);
static int **pattern_list_203;
static int no_pattern_203_niv_0;
static int nb_pattern_203_niv_0 = 2;
static int nb_pattern_203_niv_1 = 1;
#define max_nb_pattern_under_203 1

struct term* fun_203(struct term *v0 ) {
  struct term *v1,*v2;
  bitSet32 *mask;
  struct term *res;
  match_state *ms=NULL;
  int mode;
  int necessary_link;
  int indice=-1;
#ifdef GREEDY
  int greedy_indice=-1;
#endif
  int ACPattern=0;
  bitSet32_create(mask,2);
  bitSet32_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_203(");
    term_print(stdout,v0);
    printf(")\n");
  }
  if(term_first(v0)==term_last(v0) && cell_mult(term_first(v0))==1) {
    res=cell_t(term_first(v0));
    goto end_no_rewrite;
  }
  /* Begin syntactical matching */
  bitSet32_set(mask,0);
  bitSet32_set(mask,1);
  /* End syntactical matching */
  if( bitSet32_get(mask,0) || bitSet32_get(mask,1) ) {
    ACPattern=1;
    /* Begin AC matching */
    necessary_link=0;
    mode=POSSIBLE_REST;
    //ms=MS_create();
#ifdef GREEDY
    greedy_indice = MS_greedy_init(ms, match_subterm_203, no_pattern_203_niv_0, pattern_list_203, nb_pattern_203_niv_1, v0, necessary_link, max_nb_pattern_under_203,greedy_rule_tab_203,isGreedyRule_203);
    /* End AC matching */
  }
  if(greedy_indice != -1) { 
    indice=greedy_indice;
  } else {
    if(ACPattern && ms!=NULL) {
      indice = MS_greedy_solve(ms,mode,isGreedyRule_203);
      //indice = MS_solve(ms,mode);
    } else {
      indice = 0;
    }
  }
#else
  indice = MS_init(&ms, match_subterm_203, no_pattern_203_niv_0, pattern_list_203, nb_pattern_203_niv_1, v0, necessary_link, max_nb_pattern_under_203);
  /* End AC matching */
  }
  if(indice>=0) {
    if(ACPattern && ms!=NULL) {
      indice = MS_solve(ms,mode);
    } else {
      indice = 0;
    } 
  } else {
    ms=NULL;
  }
#endif
  if(bitSet32_get(mask,0)) {
    if(indice>=0 && ms->no_rule==0) {
      int nb_variable=1;
      int nb_variable_ac=1;
      struct term *substitution[1];
      int i;
      /* [] xor(,)[var0[var=0, subst=0, NOSHARE, 1].f[var=0, subst=-1, NOSHARE]][mv][var=0, subst=-1, NOSHARE] => var0[var=-1, subst=-1, PERFECTSHARE] */
      struct term *tmp, *sv0;
      substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_203);
      sv0=substitution[0];
      tmp = term_removeTopSymbol(sv0);
      if(tmp == NULL)
        sv0=fun_203(sv0);
      else
        sv0=tmp;
      res = sv0 ;
      goto end;
    }
    myend5:;
  }
  if(bitSet32_get(mask,1)) {
    {
      int nb_variable=2;
      int nb_variable_ac=1;
      struct term *substitution[2];
      int i;
      /* [] xor(,)[var0[2][var=0, subst=0, NOSHARE].var1[var=2, subst=1, NOSHARE, 1]][mv][var=0, subst=-1, NOSHARE] => xor(,)[var1[var=-1, subst=-1, PERFECTSHARE].f[var=1, subst=-1, NOSHARE]][var=3, subst=-1, NOSHARE] */
      struct term *tmp, *sv0, *sv1, *sv2, *sv3;
      for(i=0 ; i<nb_variable ; i++) {
        substitution[i]=v0;
      }
      sv0=substitution[0];
      sv2=substitution[1];
      {
        int *E,*sol;
        int nb_arg_subject;
        int no_arg_subject;
        int indice=0;
        struct cell_term *cell;
        struct cell_term *copy_cell;
        for(cell=term_first(sv0), nb_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), nb_arg_subject++);
        E=(int*)IMALLOC(nb_arg_subject*sizeof(int));
        sol=(int*)IMALLOC(nb_arg_subject*sizeof(int));
        for(cell=term_first(sv0), no_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), no_arg_subject++) {
          E[no_arg_subject]=cell_mult(cell);
          sol[no_arg_subject]=0;
        }
        //indice=next_pe_extract(nb_arg_subject-1,E,sol,2);
        //if(indice >= 0 ) {
        indice=next_pe_extract2(0,nb_arg_subject-1,E,sol,2);
        if(indice > 0 ) {
          int i;
          extract_xy_from_pe(sv0,E,sol,2,&sv0,&sv2);
          IFREE(E);
          IFREE(sol);
        } else {
          IFREE(E);
          IFREE(sol);
          goto myend6;
        }
      }
      tmp = term_removeTopSymbol(sv2);
      if(tmp == NULL)
        sv2=fun_203(sv2);
      else
        sv2=tmp;
      TERM_ALLOC(sv3,term2,code_203);
      term_add_onf_term(sv3,sv2);
      term_add_onf_term(sv3,con_301);
      sv3 = fun_203( sv3 );
      res = sv3 ;
      goto end;
    }
    myend6:;
  }
match_fail:
  if(trace>=1) {
    indent(indentlevel); printf("*** match fail 203\n");
  }
  res=v0;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(trace>=1) {
    indent(indentlevel);
    printf("rewrite[%d] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_delete(mask);
  if(ACPattern && ms!=NULL)
    MS_delete(ms);
  subindent();
  return res;
}

int match_subterm_203(struct term *v0,int no_arg_subject,bitSet32 *mask,BG *cbg) {
  struct term *v1,*v2;
  int nb_bit=0;
  bitSet32_init_clear(mask);
  switch(getSymb(v0)) {
    case code_301:
      bitSet32_set(mask,0);
      // TODO
      nb_bit++;
    break;
  }
  return nb_bit;
}

void variable_extract_203(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern) {
  switch(id_pattern) {
    /* f[var=0, subst=-1, NOSHARE] */
  case 0:
    break;
  default:
    fprintf(stderr,"variable_extract_203: bad pattern number\n");
    exit(0);
  }
}

void init_pattern_list_203() {
  int pattern_tab[max_nb_pattern_under_203];
#ifdef GREEDY
  int *greedy_rules;
#endif
  no_pattern_203_niv_0=0;
  pattern_list_203=MS_pattern_list_create(nb_pattern_203_niv_0);
  /* xor(,)[var0[var=0, subst=0, NOSHARE, 1].f[var=0, subst=-1, NOSHARE]][mv][var=0, subst=-1, NOSHARE] */
  pattern_tab[0]=0;
  MS_pattern_list_init(pattern_list_203,no_pattern_203_niv_0++,1,pattern_tab);
  /* xor(,)[var0[2][var=0, subst=0, NOSHARE].var1[var=2, subst=1, NOSHARE, 1]][mv][var=0, subst=-1, NOSHARE] */
#ifdef GREEDY
  greedy_rule_tab_203=(int**)MALLOC(nb_pattern_203_niv_1 * sizeof(int*));

  greedy_rules=(int*)MALLOC(nb_pattern_203_niv_0 * sizeof(int));
  greedy_rules[0] = 0;
  greedy_rules[1] = -1;
  greedy_rule_tab_203[0] = greedy_rules;
#endif
}

void delete_pattern_list_203() {
  MS_pattern_list_free(pattern_list_203,no_pattern_203_niv_0);
}

struct term* fun_205(struct term *v0 ) {
  struct term *v1,*v2;
  bitSet32 *mask;
  struct term *res;
  match_state *ms=NULL;
  int mode;
  int necessary_link;
  int indice=-1;
  int ACPattern=0;
  bitSet32_create(mask,1);
  bitSet32_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_205(");
    term_print(stdout,v0);
    printf(")\n");
  }
  if(term_first(v0)==term_last(v0) && cell_mult(term_first(v0))==1) {
    res=cell_t(term_first(v0));
    goto end_no_rewrite;
  }
  /* Begin syntactical matching */
  bitSet32_set(mask,0);
  /* End syntactical matching */
  if( bitSet32_get(mask,0) ) {
    ACPattern=1;
    /* End AC matching */
  }
  if(ACPattern && ms!=NULL) {
    indice = MS_solve(ms,mode);
  } else {
    indice = 0;
  }
  if(bitSet32_get(mask,0)) {
    {
      int nb_variable=2;
int nb_variable_ac=1;
      struct term *substitution[2];
      int i;
      /* [] iff(,)[var0[var=1, subst=0, NOSHARE, 1].var1[var=0, subst=1, NOSHARE, 1]][mv][var=0, subst=-1, NOSHARE] => not(xor(,)[var0[var=-1, subst=-1, PERFECTSHARE].var1[var=-1, subst=-1, PERFECTSHARE]][var=2, subst=-1, NOSHARE])[var=3, subst=-1, NOSHARE] */
      struct term *tmp, *sv0, *sv1, *sv2, *sv3;
      for(i=0 ; i<nb_variable ; i++) {
        substitution[i]=v0;
      }
      sv1=substitution[0];
      sv0=substitution[1];
      {
        int *E,*sol;
        int nb_arg_subject;
        int no_arg_subject;
        int indice=0;
        struct cell_term *cell;
        struct cell_term *copy_cell;
        for(cell=term_first(sv1), nb_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), nb_arg_subject++);
        E=(int*)IMALLOC(nb_arg_subject*sizeof(int));
        sol=(int*)IMALLOC(nb_arg_subject*sizeof(int));
        for(cell=term_first(sv1), no_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), no_arg_subject++) {
          E[no_arg_subject]=cell_mult(cell);
          sol[no_arg_subject]=0;
        }
        indice=next_pe_extract(nb_arg_subject-1,E,sol,1);
        if(indice >= 0 ) {
          int i;
          extract_xy_from_pe(sv1,E,sol,1,&sv1,&sv0);
          IFREE(E);
          IFREE(sol);
        } else {
          IFREE(E);
          IFREE(sol);
          goto myend7;
        }
      }
      tmp = term_removeTopSymbol(sv0);
      if(tmp == NULL)
        sv0=fun_205(sv0);
      else
        sv0=tmp;
      tmp = term_removeTopSymbol(sv1);
      if(tmp == NULL)
        sv1=fun_205(sv1);
      else
        sv1=tmp;
      TERM_ALLOC(sv2,term2,code_203);
      term_add_onf_term(sv2,sv1);
      term_add_onf_term(sv2,sv0);
      sv2 = fun_203( sv2 );
      sv3 = fun_206( sv2 );
      res = sv3 ;
      goto end;
    }
    myend7:;
  }
match_fail:
  res=v0;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(trace>=1) {
    indent(indentlevel);
    printf("rewrite[%d] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_delete(mask);
  if(ACPattern && ms!=NULL)
    MS_delete(ms);
  subindent();
  return res;
}

struct term* fun_207(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6;
  bitSet32 *mask;
  struct term *res;
  bitSet32_create(mask,1);
  bitSet32_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_207(");
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
    default:
      switch(getSymb(v2)) {
        default:
          bitSet32_set(mask,0);
      }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask,0)) {
    /* lhs: implies(var0[mv][var=1, subst=-1, NOSHARE, 2],var1[mv][var=2, subst=-1, NOSHARE, 1])[mv][var=0, subst=-1, NOSHARE] */
    struct term *tmp, *sv0, *sv1, *sv2;
    /* rhs: not(xor(,)[var0[var=-1, subst=-1, PERFECTSHARE].and(,)[var0[var=-1, subst=-1, PERFECTSHARE].var1[var=-1, subst=-1, PERFECTSHARE]][var=0, subst=-1, NOSHARE]][var=1, subst=-1, NOSHARE])[var=2, subst=-1, NOSHARE] */

    setShared(v1);

    TERM_ALLOC(sv0,term2,code_202);
    term_add_onf_term(sv0,v1);
    term_add_onf_term(sv0,v2);
    sv0 = fun_202( sv0 );
    TERM_ALLOC(sv1,term2,code_203);
    term_add_onf_term(sv1,v1);
    term_add_onf_term(sv1,sv0);
    sv1 = fun_203( sv1 );
    sv2 = fun_206( sv1 );
    res = sv2 ;
    goto end;
    myend8:;
  }
match_fail:
  TERM_ALLOC(res,term2, 207);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(trace>=1) {
    indent(indentlevel);
    printf("rewrite[%d] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_delete(mask);
  subindent();
  return res;
}
int match_subterm_202(struct term *v0,int no_arg_subject, bitSet32 *mask, BG *cbg);
void variable_extract_202(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern);
static int **pattern_list_202;
static int no_pattern_202_niv_0;
static int nb_pattern_202_niv_0 = 4;
static int nb_pattern_202_niv_1 = 3;
#define max_nb_pattern_under_202 1

struct term* fun_202(struct term *v0 ) {
  struct term *v1,*v2;
  bitSet32 *mask;
  struct term *res;
  match_state *ms=NULL;
  int mode;
  int necessary_link;
  int indice=-1;
#ifdef GREEDY
  int greedy_indice=-1;
#endif
  int ACPattern=0;
  bitSet32_create(mask,4);
  bitSet32_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_202(");
    term_print(stdout,v0);
    printf(")\n");
  }
  if(term_first(v0)==term_last(v0) && cell_mult(term_first(v0))==1) {
    res=cell_t(term_first(v0));
    goto end_no_rewrite;
  }
  /* Begin syntactical matching */
  bitSet32_set(mask,0);
  bitSet32_set(mask,1);
  bitSet32_set(mask,2);
  bitSet32_set(mask,3);
  /* End syntactical matching */
  if( bitSet32_get(mask,0) || bitSet32_get(mask,1) || bitSet32_get(mask,2) || bitSet32_get(mask,3) ) {
    ACPattern=1;
    /* Begin AC matching */
    necessary_link=0;
    mode=POSSIBLE_REST;
//ms=MS_create();
#ifdef GREEDY
    greedy_indice = MS_greedy_init(ms, match_subterm_202, no_pattern_202_niv_0, pattern_list_202, nb_pattern_202_niv_1, v0, necessary_link, max_nb_pattern_under_202,greedy_rule_tab_202,isGreedyRule_202);
    /* End AC matching */
  }
  if(greedy_indice != -1) { 
    indice=greedy_indice;
  } else {
    if(ACPattern && ms!=NULL) {
      indice = MS_greedy_solve(ms,mode,isGreedyRule_202);
      //indice = MS_solve(ms,mode);
    } else {
      indice = 0;
    }
  }
#else
    indice = MS_init(&ms, match_subterm_202, no_pattern_202_niv_0, pattern_list_202, nb_pattern_202_niv_1, v0, necessary_link, max_nb_pattern_under_202);
    /* End AC matching */
  }
  if(indice>=0) {
    if(ACPattern && ms!=NULL) {
      indice = MS_solve(ms,mode);
    } else {
      indice = 0;
    } 
  } else {
    ms=NULL;
  }
#endif
  if(bitSet32_get(mask,0)) {
    if(indice>=0 && ms->no_rule==0) {
      int nb_variable=1;
      int nb_variable_ac=1;
      struct term *substitution[1];
      int i;
      /* [] and(,)[var0[var=0, subst=0, NOSHARE, 1].t[var=0, subst=-1, NOSHARE]][mv][var=0, subst=-1, NOSHARE] => var0[var=-1, subst=-1, PERFECTSHARE] */
      struct term *tmp, *sv0;
      substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_202);
      sv0=substitution[0];
      tmp = term_removeTopSymbol(sv0);
      if(tmp == NULL)
        sv0=fun_202(sv0);
      else
        sv0=tmp;
      res = sv0 ;
      goto end;
    }
    myend9:;
  }
  if(bitSet32_get(mask,1)) {
    if(indice>=0 && ms->no_rule==1) {
      int nb_variable=1;
      int nb_variable_ac=1;
      struct term *substitution[1];
      int i;
      /* [] and(,)[var0[var=0, subst=0, NOSHARE].f[var=0, subst=-1, NOSHARE]][mv][var=0, subst=-1, NOSHARE] => f[var=1, subst=-1, NOSHARE] */
      struct term *tmp, *sv0, *sv1;
      substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_202);
      res = con_301 ;
      goto end;
    }
    myend10:;
  }
  if(bitSet32_get(mask,2)) {
    {
      int nb_variable=2;
      int nb_variable_ac=1;
      struct term *substitution[2];
      int i;
      /* [] and(,)[var0[2][var=1, subst=0, NOSHARE, 1].var1[var=0, subst=1, NOSHARE, 1]][mv][var=0, subst=-1, NOSHARE] => and(,)[var0[var=-1, subst=-1, PERFECTSHARE].var1[var=-1, subst=-1, PERFECTSHARE]][var=2, subst=-1, NOSHARE] */
      struct term *tmp, *sv0, *sv1, *sv2;
      for(i=0 ; i<nb_variable ; i++) {
        substitution[i]=v0;
      }
      sv1=substitution[0];
      sv0=substitution[1];
      {
        int *E,*sol;
        int nb_arg_subject;
        int no_arg_subject;
        int indice=0;
        struct cell_term *cell;
        struct cell_term *copy_cell;
        for(cell=term_first(sv1), nb_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), nb_arg_subject++);
        E=(int*)IMALLOC(nb_arg_subject*sizeof(int));
        sol=(int*)IMALLOC(nb_arg_subject*sizeof(int));
        for(cell=term_first(sv1), no_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), no_arg_subject++) {
          E[no_arg_subject]=cell_mult(cell);
          sol[no_arg_subject]=0;
        }
        //indice=next_pe_extract(nb_arg_subject-1,E,sol,2);
        //if(indice >= 0 ) {
        indice=next_pe_extract2(0,nb_arg_subject-1,E,sol,2);
        if(indice > 0 ) {
          int i;
          extract_xy_from_pe(sv1,E,sol,2,&sv1,&sv0);
          IFREE(E);
          IFREE(sol);
        } else {
          IFREE(E);
          IFREE(sol);
          goto myend11;
        }
      }
      tmp = term_removeTopSymbol(sv0);
      if(tmp == NULL)
        sv0=fun_202(sv0);
      else
        sv0=tmp;
      tmp = term_removeTopSymbol(sv1);
      if(tmp == NULL)
        sv1=fun_202(sv1);
      else
        sv1=tmp;
      TERM_ALLOC(sv2,term2,code_202);
      term_add_onf_term(sv2,sv1);
      term_add_onf_term(sv2,sv0);
      sv2 = fun_202( sv2 );
      res = sv2 ;
      goto end;
    }
    myend11:;
  }
  if(bitSet32_get(mask,3)) {
    if(indice>=0 && ms->no_rule==2) {
      int nb_variable=3;
      int nb_variable_ac=1;
      struct term *substitution[3];
      int i;
      /* [] and(,)[var0[var=1, subst=0, NOSHARE, 2].xor(,)[var1[var=3, subst=1, NOSHARE, 1].var2[var=0, subst=2, NOSHARE, 1]][var=0, subst=-1, NOSHARE]][mv][var=0, subst=-1, NOSHARE] => xor(,)[and(,)[var0[var=-1, subst=-1, PERFECTSHARE].var1[var=-1, subst=-1, PERFECTSHARE]][var=4, subst=-1, NOSHARE].and(,)[var0[var=-1, subst=-1, PERFECTSHARE].var2[var=-1, subst=-1, PERFECTSHARE]][var=2, subst=-1, NOSHARE]][var=5, subst=-1, NOSHARE] */
      struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5;
      substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_202);
      sv3=substitution[1];
      sv0=substitution[2];
      {
        int *E,*sol;
        int nb_arg_subject;
        int no_arg_subject;
        int indice=0;
        struct cell_term *cell;
        struct cell_term *copy_cell;
        for(cell=term_first(sv3), nb_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), nb_arg_subject++);
        E=(int*)IMALLOC(nb_arg_subject*sizeof(int));
        sol=(int*)IMALLOC(nb_arg_subject*sizeof(int));
        for(cell=term_first(sv3), no_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), no_arg_subject++) {
          E[no_arg_subject]=cell_mult(cell);
          sol[no_arg_subject]=0;
        }
        indice=next_pe_extract(nb_arg_subject-1,E,sol,1);
        if(indice >= 0 ) {
          int i;
          extract_xy_from_pe(sv3,E,sol,1,&sv3,&sv0);
          IFREE(E);
          IFREE(sol);
        } else {
          IFREE(E);
          IFREE(sol);
          goto myend12;
        }
      }
      tmp = term_removeTopSymbol(sv0);
      if(tmp != NULL)
        sv0=tmp;
      sv1=substitution[0];
      tmp = term_removeTopSymbol(sv1);
      if(tmp == NULL)
        sv1=fun_202(sv1);
      else
        sv1=tmp;

      setShared(sv1);

      TERM_ALLOC(sv2,term2,code_202);
      term_add_onf_term(sv2,sv1);
      term_add_onf_term(sv2,sv0);
      sv2 = fun_202( sv2 );
      tmp = term_removeTopSymbol(sv3);
      if(tmp != NULL)
        sv3=tmp;
      TERM_ALLOC(sv4,term2,code_202);
      term_add_onf_term(sv4,sv1);
      term_add_onf_term(sv4,sv3);
      sv4 = fun_202( sv4 );
      TERM_ALLOC(sv5,term2,code_203);
      term_add_onf_term(sv5,sv4);
      term_add_onf_term(sv5,sv2);
      sv5 = fun_203( sv5 );
      res = sv5 ;
      goto end;
    }
    myend12:;
  }
match_fail:
  if(trace>=1) {
    indent(indentlevel); printf("*** match fail 202\n");
  }
  res=v0;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(trace>=1) {
    indent(indentlevel);
    printf("rewrite[%d] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_delete(mask);
  if(ACPattern && ms!=NULL)
    MS_delete(ms);
  subindent();
  return res;
}

int match_subterm_202(struct term *v0,int no_arg_subject,bitSet32 *mask,BG *cbg) {
  struct term *v1,*v2;
  int nb_bit=0;
  bitSet32_init_clear(mask);
  switch(getSymb(v0)) {
    case code_203:
      /* AC case: Not tested */
      bitSet32_set(mask,2);
      // TODO
      nb_bit++;
    break;
    case code_301:
      bitSet32_set(mask,1);
      // TODO
      nb_bit++;
    break;
    case code_300:
      bitSet32_set(mask,0);
      // TODO
      nb_bit++;
    break;
  }
  return nb_bit;
}

void variable_extract_202(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern) {
  switch(id_pattern) {
    /* t[var=0, subst=-1, NOSHARE] */
  case 0:
    break;
    /* f[var=0, subst=-1, NOSHARE] */
  case 1:
    break;
    /* xor(,)[var1[var=3, subst=1, NOSHARE, 1].var2[var=0, subst=2, NOSHARE, 1]][var=0, subst=-1, NOSHARE] */
  case 2:
    {
      /* Not tested */
      int nb_variable=2;
      int nb_variable_ac=1;
      struct term *substitution[2];
      LINK *link;
      match_state *msbg;
      int i;
      link=BG_link_get(ms->cbg,id_pattern);
      if(link==NULL) {
        for(i=0 ; i<nb_variable ; i++) {
          extract_substitution[*indice]=v0;
          (*indice)++;
        }
      } else {
        msbg=LINK_get(link,no_arg_subject);
        substitution_build(v0,msbg,nb_variable,substitution,nb_variable_ac,variable_extract_202);
        for(i=0 ; i <nb_variable ; i++) {
          extract_substitution[*indice]=substitution[i];
          (*indice)++;
        }
      }
    }
    break;
  default:
    fprintf(stderr,"variable_extract_202: bad pattern number\n");
    exit(0);
  }
}

void init_pattern_list_202() {
  int pattern_tab[max_nb_pattern_under_202];
#ifdef GREEDY
  int *greedy_rules;
#endif
  no_pattern_202_niv_0=0;
  pattern_list_202=MS_pattern_list_create(nb_pattern_202_niv_0);
  /* and(,)[var0[var=0, subst=0, NOSHARE, 1].t[var=0, subst=-1, NOSHARE]][mv][var=0, subst=-1, NOSHARE] */
  pattern_tab[0]=0;
  MS_pattern_list_init(pattern_list_202,no_pattern_202_niv_0++,1,pattern_tab);
  /* and(,)[var0[var=0, subst=0, NOSHARE].f[var=0, subst=-1, NOSHARE]][mv][var=0, subst=-1, NOSHARE] */
  pattern_tab[0]=1;
  MS_pattern_list_init(pattern_list_202,no_pattern_202_niv_0++,1,pattern_tab);
  /* and(,)[var0[2][var=1, subst=0, NOSHARE, 1].var1[var=0, subst=1, NOSHARE, 1]][mv][var=0, subst=-1, NOSHARE] */
  /* and(,)[var0[var=1, subst=0, NOSHARE, 2].xor(,)[var1[var=3, subst=1, NOSHARE, 1].var2[var=0, subst=2, NOSHARE, 1]][var=0, subst=-1, NOSHARE]][mv][var=0, subst=-1, NOSHARE] */
  pattern_tab[0]=2;
  MS_pattern_list_init(pattern_list_202,no_pattern_202_niv_0++,1,pattern_tab);

#ifdef GREEDY
  greedy_rule_tab_202=(int**)MALLOC(nb_pattern_202_niv_1 * sizeof(int*));

  greedy_rules=(int*)MALLOC(nb_pattern_202_niv_0 * sizeof(int));
  greedy_rules[0] = 0;
  greedy_rules[1] = -1;
  greedy_rule_tab_202[0] = greedy_rules;

  greedy_rules=(int*)MALLOC(nb_pattern_202_niv_0 * sizeof(int));
  greedy_rules[0] = 1;
  greedy_rules[1] = -1;
  greedy_rule_tab_202[1] = greedy_rules;

  greedy_rules=(int*)MALLOC(nb_pattern_202_niv_0 * sizeof(int));
greedy_rules[0] = 2; // no de la regle dans le cbg
  greedy_rules[1] = -1;
  greedy_rule_tab_202[2] = greedy_rules;

#endif

}

void delete_pattern_list_202() {
  MS_pattern_list_free(pattern_list_202,no_pattern_202_niv_0);
}
