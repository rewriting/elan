#include "robot.h"
extern struct term *query;

/* Constantes d'execution */
unsigned long rewrite_step=0;
unsigned long tab_rewrite_step[2][382];
int global_indentlevel=0;

/* Table des symboles */
int fsymtabSize = 383;
fsym fsymtab[383];
/* Declaration des pattern_list */

/* Constantes */
struct term *con_303;
struct term *con_302;
struct term *con_301;
struct term *con_300;
struct term *con_381;
struct term *con_380;
struct term *con_1;
struct term *con_0;
struct term *con_293;
struct term *con_292;
struct term *con_290;
struct term *con_379;
struct term *con_378;
struct term *con_377;
struct term *con_376;
struct term *con_375;
struct term *con_374;
struct term *con_372;
struct term *con_371;
struct term *con_370;
struct term *con_287;
struct term *con_285;
struct term *con_284;
struct term *con_283;
struct term *con_282;
struct term *con_369;
struct term *con_368;
struct term *con_357;
struct term *con_356;
struct term *con_352;
struct term *con_351;
struct term *con_265;
struct term *con_349;
struct term *con_348;
struct term *con_347;
struct term *con_346;
struct term *con_344;
struct term *con_343;
struct term *con_342;
struct term *con_341;
struct term *con_240;
struct term *con_239;
struct term *con_237;
struct term *con_236;
struct term *con_235;
struct term *con_234;
struct term *con_310;

/* Redirection de built-ins */
struct term *fun_226(struct term *v0, struct term *v1, struct term *v2)
 { return fun_184(226, v0, v1, v2); }

struct term *fun_225()
 { return fun_189(225); }

struct term *fun_224(struct term *v0, struct term *v1)
 { return fun_185(224, v0, v1); }

struct term *fun_223()
 { return fun_183(223); }

struct term *fun_222(struct term *v0)
 { return fun_190(222, v0); }

struct term *fun_221(struct term *v0)
 { return fun_181(221, v0); }

struct term *fun_220(struct term *v0)
 { return fun_182(220, v0); }

struct term *fun_218(struct term *v0, struct term *v1)
 { return fun_180(218, v0, v1); }

struct term *fun_286(struct term *v0, struct term *v1)
 { return fun_188(286, v0, v1); }

struct term *fun_279(struct term *v0, struct term *v1, struct term *v2)
 { return fun_187(279, v0, v1, v2); }

struct term *fun_278()
 { return fun_186(278); }

struct term *fun_274(struct term *v0, struct term *v1, struct term *v2)
 { return fun_184(274, v0, v1, v2); }

struct term *fun_273()
 { return fun_189(273); }

struct term *fun_272(struct term *v0, struct term *v1)
 { return fun_185(272, v0, v1); }

struct term *fun_271()
 { return fun_183(271); }

struct term *fun_270(struct term *v0)
 { return fun_190(270, v0); }

struct term *fun_269(struct term *v0)
 { return fun_181(269, v0); }

struct term *fun_268(struct term *v0)
 { return fun_182(268, v0); }

struct term *fun_266(struct term *v0, struct term *v1)
 { return fun_180(266, v0, v1); }

struct term *fun_345(struct term *v0, struct term *v1)
 { return fun_188(345, v0, v1); }

struct term *fun_338(struct term *v0, struct term *v1, struct term *v2)
 { return fun_187(338, v0, v1, v2); }

struct term *fun_337()
 { return fun_186(337); }

struct term *fun_333(struct term *v0, struct term *v1, struct term *v2)
 { return fun_184(333, v0, v1, v2); }

struct term *fun_332()
 { return fun_189(332); }

struct term *fun_331(struct term *v0, struct term *v1)
 { return fun_185(331, v0, v1); }

struct term *fun_330()
 { return fun_183(330); }

struct term *fun_329(struct term *v0)
 { return fun_190(329, v0); }

struct term *fun_328(struct term *v0)
 { return fun_181(328, v0); }

struct term *fun_327(struct term *v0)
 { return fun_182(327, v0); }

struct term *fun_325(struct term *v0, struct term *v1)
 { return fun_180(325, v0, v1); }

struct term *fun_321(struct term *v0, struct term *v1, struct term *v2, struct term *v3)
 { return fun_128(321, v0, v1, v2, v3); }

struct term *fun_238(struct term *v0, struct term *v1)
 { return fun_188(238, v0, v1); }

struct term *fun_231(struct term *v0, struct term *v1, struct term *v2)
 { return fun_187(231, v0, v1, v2); }

struct term *fun_230()
 { return fun_186(230); }

struct term *fun_316(struct term *v0, struct term *v1, struct term *v2)
 { return fun_130(316, v0, v1, v2); }

struct term *fun_315(struct term *v0, struct term *v1)
 { return fun_129(315, v0, v1); }


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
NULL, NULL, NULL, (funTabType) &fun_201, (funTabType) &fun_202, 
(funTabType) &fun_203, (funTabType) &fun_204, (funTabType) &fun_205, 
(funTabType) &fun_206, (funTabType) &fun_207, (funTabType) &fun_208, 
(funTabType) &fun_209, (funTabType) &fun_210, (funTabType) &fun_211, 
(funTabType) &fun_212, (funTabType) &fun_213, (funTabType) &fun_214, 
(funTabType) &fun_215, (funTabType) &fun_216, (funTabType) &fun_217, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &fun_295, NULL, (funTabType) &fun_297, 
(funTabType) &fun_298, (funTabType) &fun_299, NULL, NULL, NULL, NULL, 
NULL, (funTabType) &fun_305, NULL, (funTabType) &fun_307, 
(funTabType) &fun_308, (funTabType) &fun_309, NULL, 
(funTabType) &fun_311, (funTabType) &fun_312, (funTabType) &fun_313, 
(funTabType) &fun_314, NULL, NULL, (funTabType) &fun_317, 
(funTabType) &fun_318, (funTabType) &fun_319, (funTabType) &fun_320, 
NULL, (funTabType) &fun_322, (funTabType) &fun_323, 
(funTabType) &fun_324, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &fun_353, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, (funTabType) &fun_362, (funTabType) &fun_363, 
(funTabType) &fun_364, (funTabType) &fun_365, (funTabType) &fun_366, 
(funTabType) &fun_367, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

/* tableau de pointeurs de fonctions qui retournent */
/* un pointeur sur un term */
funTabType strTab[] = {
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, (funTabType) &str_91, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, (funTabType) &str_99, NULL, NULL, NULL, 
(funTabType) &str_103, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &str_111, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &str_163, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &str_171, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, (funTabType) &str_182, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, (funTabType) &str_194, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, (funTabType) &str_254, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, (funTabType) &str_361, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &str_373, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &str_433, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, (funTabType) &str_465, NULL, NULL, NULL, NULL, 
(funTabType) &str_470, (funTabType) &str_471, (funTabType) &str_472, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, (funTabType) &str_498, NULL};

/* Query */
static struct term *main_query() {
  struct term *res;
  /* TERME DE DEPART */
  {
    /* [](allsearch,.(.(state(,)([](0),[](0)),nil),nil)) */
    struct term *tmp, *sv[11];
    TERM_ALLOC(sv[5],term1,code_200);
    sv[5]->sub[0] = (setIntegerTag(0));
    TERM_ALLOC(sv[3],term1,code_200);
    sv[3]->sub[0] = (setIntegerTag(0));
    TERM_ALLOC(sv[6],term2,code_288);
    sv[6]->sub[0] = sv[5];
    sv[6]->sub[1] = sv[3];
    TERM_ALLOC(sv[7],term2,code_294);
    sv[7]->sub[0] = sv[6];
    sv[7]->sub[1] = con_293;
    TERM_ALLOC(sv[8],term2,code_304);
    sv[8]->sub[0] = sv[7];
    sv[8]->sub[1] = con_303;
    sv[10] = fun_325( con_380,sv[8] );
    res=str_373(sv[10]);
  }
  return res;
}

/* Procedure principale */
long *bp_main;
int main(int argc,char **argv) {
  long bp;
  struct term *res;
  int i,j,queryMode=0;
  bp_main=&bp;
  backTrackInit();
  init_alloc();
  for(i=0 ; i<fsymtabSize ; i++) {
    fsym_init(i,0,"nullString",0,0,NULL);
  }
  fsym_init(code_229,3,"[=>=>]",0,0, NULL);
  fsym_init(code_228,2,"[=>]",0,0, NULL);
  fsym_init(code_227,2,"(letin)",0,0, NULL);
  fsym_init(code_226,3,"ifthenelsefi",-184,0, NULL);
  fsym_init(code_225,0,"",-189,0, NULL);
  fsym_init(code_224,2,"||",-185,0, NULL);
  fsym_init(code_223,0,"fail",-183,0, NULL);
  fsym_init(code_222,1,"one()",-190,0, NULL);
  fsym_init(code_221,1,"first()",-181,0, NULL);
  fsym_init(code_220,1,"dk()",-182,0, NULL);
  fsym_init(code_309,2,"ccat(,)",0,0, NULL);
  fsym_init(code_308,1,"size_of_list[state]_list()",0,0, NULL);
  fsym_init(code_307,2,"-thelem()",0,0, NULL);
  fsym_init(code_306,1,"elem()",0,0, NULL);
  fsym_init(code_305,2,"@",0,0, NULL);
  fsym_init(code_304,2,".",0,0, NULL);
  fsym_init(code_303,0,"nil",0,0, NULL);
  fsym_init(code_139,1,"whileendwhile",139,0, NULL);
  fsym_init(code_302,0,"extractrule2",0,0, NULL);
  fsym_init(code_138,1,"dk()",138,0, NULL);
  fsym_init(code_301,0,"extractrule1",0,0, NULL);
  fsym_init(code_137,1,"dc()",137,0, NULL);
  fsym_init(code_300,0,"listExtract",0,0, NULL);
  fsym_init(code_136,1,"dk()",136,0, NULL);
  fsym_init(code_135,1,"dc()",135,0, NULL);
  fsym_init(code_134,2,"",134,0, NULL);
  fsym_init(code_133,1,"",133,0, NULL);
  fsym_init(code_132,2,",",132,0, NULL);
  fsym_init(code_131,1,"",131,0, NULL);
  fsym_init(code_130,3,"meta_apply(,,)",130,0, NULL);
  fsym_init(code_219,2,"(())",0,0, NULL);
  fsym_init(code_218,2,"[]",-180,0, NULL);
  fsym_init(code_217,1,"valueOf()",0,0, NULL);
  fsym_init(code_216,1,"itob_int()",0,0, NULL);
  fsym_init(code_215,1,"btoi_int()",0,0, NULL);
  fsym_init(code_214,2,"less_int(,)",0,0, NULL);
  fsym_init(code_213,2,"lesseq_int(,)",0,0, NULL);
  fsym_init(code_212,2,"greatereq_int(,)",0,0, NULL);
  fsym_init(code_211,2,"greater_int(,)",0,0, NULL);
  fsym_init(code_210,2,"neq_int(,)",0,0, NULL);
  fsym_init(code_129,2,"meta_apply(,)",129,0, NULL);
  fsym_init(code_128,4,"meta_apply(,,,)",128,0, NULL);
  fsym_init(code_209,2,"eq_int(,)",0,0, NULL);
  fsym_init(code_208,1,"umin()",0,0, NULL);
  fsym_init(code_207,2,"or(,)",0,0, NULL);
  fsym_init(code_206,2,"div(,)",0,0, NULL);
  fsym_init(code_205,2,"and(,)",0,0, NULL);
  fsym_init(code_204,2,"mod(,)",0,0, NULL);
  fsym_init(code_203,2,"time(,)",0,0, NULL);
  fsym_init(code_202,2,"minus(,)",0,0, NULL);
  fsym_init(code_201,2,"plus(,)",0,0, NULL);
  fsym_init(code_200,1,"[]",0,0, NULL);
  fsym_init(code_81,1,"",0,0, NULL);
  fsym_init(code_79,1,"-",0,0, NULL);
  fsym_init(code_9,2,"neq_builtinInt(,)",9,0, NULL);
  fsym_init(code_8,2,"eq_builtinInt(,)",8,0, NULL);
  fsym_init(code_6,2,"div(,)",6,0, NULL);
  fsym_init(code_5,2,"time(,)",5,0, NULL);
  fsym_init(code_382,3,"move(,,)",0,0, NULL);
  fsym_init(code_4,2,"minus(,)",4,0, NULL);
  fsym_init(code_381,0,"moves",0,0, NULL);
  fsym_init(code_3,2,"plus(,)",3,0, NULL);
  fsym_init(code_380,0,"allsearch",0,0, NULL);
  fsym_init(code_1,0,"true",0,0, NULL);
  fsym_init(code_0,0,"false",0,0, NULL);
  fsym_init(code_67,1,"",0,0, NULL);
  fsym_init(code_299,2,"ccat(,)",0,0, NULL);
  fsym_init(code_298,1,"size_of_state_list()",0,0, NULL);
  fsym_init(code_297,2,"-thelem()",0,0, NULL);
  fsym_init(code_296,1,"elem()",0,0, NULL);
  fsym_init(code_295,2,"@",0,0, NULL);
  fsym_init(code_294,2,".",0,0, NULL);
  fsym_init(code_293,0,"nil",0,0, NULL);
  fsym_init(code_292,0,"moves",0,0, NULL);
  fsym_init(code_291,1,"move()",0,0, NULL);
  fsym_init(code_290,0,"win",0,0, NULL);
  fsym_init(code_379,0,"search",0,0, NULL);
  fsym_init(code_378,0,"exitable",0,0, NULL);
  fsym_init(code_377,0,"right",0,0, NULL);
  fsym_init(code_376,0,"left",0,0, NULL);
  fsym_init(code_375,0,"down",0,0, NULL);
  fsym_init(code_374,0,"up",0,0, NULL);
  fsym_init(code_373,2,"room(,)",0,0, NULL);
  fsym_init(code_372,0,"E",0,0, NULL);
  fsym_init(code_371,0,"R",0,0, NULL);
  fsym_init(code_370,0,"L",0,0, NULL);
  fsym_init(code_289,2,"state(,)",0,0, NULL);
  fsym_init(code_288,2,"state(,)",0,0, NULL);
  fsym_init(code_287,0,"CONCAT",0,0, NULL);
  fsym_init(code_286,2,";",-188,0, NULL);
  fsym_init(code_285,0,"eval",0,0, NULL);
  fsym_init(code_284,0,"eval_DK",0,0, NULL);
  fsym_init(code_283,0,"eval_DC",0,0, NULL);
  fsym_init(code_282,0,"eval_ONE",0,0, NULL);
  fsym_init(code_281,1,"return",0,0, NULL);
  fsym_init(code_280,1,"",0,0, NULL);
  fsym_init(code_369,0,"D",0,0, NULL);
  fsym_init(code_368,0,"U",0,0, NULL);
  fsym_init(code_367,2,"room(,)",0,0, NULL);
  fsym_init(code_366,0,"E",0,0, NULL);
  fsym_init(code_365,0,"R",0,0, NULL);
  fsym_init(code_364,0,"L",0,0, NULL);
  fsym_init(code_363,0,"D",0,0, NULL);
  fsym_init(code_362,0,"U",0,0, NULL);
  fsym_init(code_361,2,"ccat(,)",0,0, NULL);
  fsym_init(code_360,2,"ccat(*)",0,0, NULL);
  fsym_init(code_190,1,"one()",190,0, NULL);
  fsym_init(code_279,3,"ifthenorelsefi",-187,0, NULL);
  fsym_init(code_278,0,"id",-186,0, NULL);
  fsym_init(code_277,3,"[=>=>]",0,0, NULL);
  fsym_init(code_276,2,"[=>]",0,0, NULL);
  fsym_init(code_275,2,"(letin)",0,0, NULL);
  fsym_init(code_274,3,"ifthenelsefi",-184,0, NULL);
  fsym_init(code_273,0,"",-189,0, NULL);
  fsym_init(code_272,2,"||",-185,0, NULL);
  fsym_init(code_271,0,"fail",-183,0, NULL);
  fsym_init(code_270,1,"one()",-190,0, NULL);
  fsym_init(code_359,2,"@",0,0, NULL);
  fsym_init(code_358,2,"append(,)",0,0, NULL);
  fsym_init(code_357,0,"nil",0,0, NULL);
  fsym_init(code_356,0,"nil_list[state]",0,0, NULL);
  fsym_init(code_355,1,"size_of_list[state]_list()",0,0, NULL);
  fsym_init(code_354,1,"size()",0,0, NULL);
  fsym_init(code_353,2,"occursin",0,0, NULL);
  fsym_init(code_189,0,"",189,0, NULL);
  fsym_init(code_352,0,"narrow",0,0, NULL);
  fsym_init(code_188,2,";",188,0, NULL);
  fsym_init(code_351,0,"loop",0,0, NULL);
  fsym_init(code_187,3,"ifthenorelsefi",187,0, NULL);
  fsym_init(code_350,1,"call()",0,0, NULL);
  fsym_init(code_186,0,"id",186,0, NULL);
  fsym_init(code_185,2,"",185,0, NULL);
  fsym_init(code_184,3,"ifthenelsefi",184,0, NULL);
  fsym_init(code_183,0,"fail",183,0, NULL);
  fsym_init(code_182,1,"dk()",182,0, NULL);
  fsym_init(code_181,1,"dc()",181,0, NULL);
  fsym_init(code_180,2,"]",180,0, NULL);
  fsym_init(code_269,1,"first()",-181,0, NULL);
  fsym_init(code_268,1,"dk()",-182,0, NULL);
  fsym_init(code_33,2,"greatereq_list[list[state]](,)",33,0, NULL);
  fsym_init(code_267,2,"(())",0,0, NULL);
  fsym_init(code_32,2,"greater_list[list[state]](,)",32,0, NULL);
  fsym_init(code_266,2,"[]",-180,0, NULL);
  fsym_init(code_31,2,"lesseq_list[list[state]](,)",31,0, NULL);
  fsym_init(code_265,0,"sub1",0,0, NULL);
  fsym_init(code_30,2,"less_list[list[state]](,)",30,0, NULL);
  fsym_init(code_264,1,"umin()",0,0, NULL);
  fsym_init(code_263,2,"or(,)",0,0, NULL);
  fsym_init(code_262,2,"and(,)",0,0, NULL);
  fsym_init(code_261,2,"mod(,)",0,0, NULL);
  fsym_init(code_260,2,"div(,)",0,0, NULL);
  fsym_init(code_349,0,"cut",0,0, NULL);
  fsym_init(code_348,0,"exit",0,0, NULL);
  fsym_init(code_347,0,"next",0,0, NULL);
  fsym_init(code_346,0,"CONCAT",0,0, NULL);
  fsym_init(code_345,2,";",-188,0, NULL);
  fsym_init(code_344,0,"eval",0,0, NULL);
  fsym_init(code_343,0,"eval_DK",0,0, NULL);
  fsym_init(code_342,0,"eval_DC",0,0, NULL);
  fsym_init(code_341,0,"eval_ONE",0,0, NULL);
  fsym_init(code_340,1,"return",0,0, NULL);
  fsym_init(code_29,2,"or(,)",29,0, NULL);
  fsym_init(code_28,2,"and(,)",28,0, NULL);
  fsym_init(code_27,2,"mod(,)",27,0, NULL);
  fsym_init(code_26,1,"itob_builtinInt()",26,0, NULL);
  fsym_init(code_259,2,"time(,)",0,0, NULL);
  fsym_init(code_25,1,"btoi_builtinInt()",25,0, NULL);
  fsym_init(code_258,2,"minus(,)",0,0, NULL);
  fsym_init(code_24,1,"(not())",24,0, NULL);
  fsym_init(code_257,2,"plus(,)",0,0, NULL);
  fsym_init(code_256,1,"(-)",0,0, NULL);
  fsym_init(code_22,2,"(or)",22,0, NULL);
  fsym_init(code_255,2,"(|)",0,0, NULL);
  fsym_init(code_21,2,"(and)",21,0, NULL);
  fsym_init(code_254,2,"(&)",0,0, NULL);
  fsym_init(code_20,1,"umin_builtinInt()",20,0, NULL);
  fsym_init(code_253,2,"(%)",0,0, NULL);
  fsym_init(code_252,2,"(/)",0,0, NULL);
  fsym_init(code_251,2,"(*)",0,0, NULL);
  fsym_init(code_250,2,"(-)",0,0, NULL);
  fsym_init(code_339,1,"",0,0, NULL);
  fsym_init(code_338,3,"ifthenorelsefi",-187,0, NULL);
  fsym_init(code_337,0,"id",-186,0, NULL);
  fsym_init(code_336,3,"[=>=>]",0,0, NULL);
  fsym_init(code_335,2,"[=>]",0,0, NULL);
  fsym_init(code_334,2,"(letin)",0,0, NULL);
  fsym_init(code_333,3,"ifthenelsefi",-184,0, NULL);
  fsym_init(code_332,0,"",-189,0, NULL);
  fsym_init(code_331,2,"||",-185,0, NULL);
  fsym_init(code_330,0,"fail",-183,0, NULL);
  fsym_init(code_19,2,"neq_list[list[state]](,)",19,0, NULL);
  fsym_init(code_18,2,"eq_list[list[state]](,)",18,0, NULL);
  fsym_init(code_249,2,"(+)",0,0, NULL);
  fsym_init(code_248,1,"-",0,0, NULL);
  fsym_init(code_247,2,"|",0,0, NULL);
  fsym_init(code_13,2,"greatereq_builtinInt(,)",13,0, NULL);
  fsym_init(code_246,2,"/",0,0, NULL);
  fsym_init(code_12,2,"greater_builtinInt(,)",12,0, NULL);
  fsym_init(code_245,2,"&",0,0, NULL);
  fsym_init(code_11,2,"lesseq_builtinInt(,)",11,0, NULL);
  fsym_init(code_244,2,"%",0,0, NULL);
  fsym_init(code_10,2,"less_builtinInt(,)",10,0, NULL);
  fsym_init(code_243,2,"*",0,0, NULL);
  fsym_init(code_242,2,"-",0,0, NULL);
  fsym_init(code_241,2,"+",0,0, NULL);
  fsym_init(code_240,0,"loop",0,0, NULL);
  fsym_init(code_329,1,"one()",-190,0, NULL);
  fsym_init(code_328,1,"first()",-181,0, NULL);
  fsym_init(code_327,1,"dk()",-182,0, NULL);
  fsym_init(code_326,2,"(())",0,0, NULL);
  fsym_init(code_325,2,"[]",-180,0, NULL);
  fsym_init(code_324,4,"setof(,,,)",0,0, NULL);
  fsym_init(code_323,3,"setof(,,)",0,0, NULL);
  fsym_init(code_322,2,"setof(,)",0,0, NULL);
  fsym_init(code_158,1,"internstring()",158,0, NULL);
  fsym_init(code_321,4,"meta_apply(,,,)",-128,0, NULL);
  fsym_init(code_157,2,"strcmp(,)",157,0, NULL);
  fsym_init(code_320,4,"meta_apply(,,,)",0,0, NULL);
  fsym_init(code_156,2,"strspn(,)",156,0, NULL);
  fsym_init(code_154,3,"internsubstr(,,)",154,0, NULL);
  fsym_init(code_153,3,"intern[<-]",153,0, NULL);
  fsym_init(code_152,2,"intern[]",152,0, NULL);
  fsym_init(code_151,2,"+",151,0, NULL);
  fsym_init(code_150,1,"strlen()",150,0, NULL);
  fsym_init(code_239,0,"CONCAT",0,0, NULL);
  fsym_init(code_238,2,";",-188,0, NULL);
  fsym_init(code_237,0,"eval",0,0, NULL);
  fsym_init(code_236,0,"eval_DK",0,0, NULL);
  fsym_init(code_235,0,"eval_DC",0,0, NULL);
  fsym_init(code_234,0,"eval_ONE",0,0, NULL);
  fsym_init(code_233,1,"return",0,0, NULL);
  fsym_init(code_232,1,"",0,0, NULL);
  fsym_init(code_231,3,"ifthenorelsefi",-187,0, NULL);
  fsym_init(code_230,0,"id",-186,0, NULL);
  fsym_init(code_319,4,"set_of(,,,)",0,0, NULL);
  fsym_init(code_318,3,"set_of(,,)",0,0, NULL);
  fsym_init(code_317,2,"set_of(,)",0,0, NULL);
  fsym_init(code_316,3,"meta_apply(,,)",-130,0, NULL);
  fsym_init(code_315,2,"meta_apply(,)",-129,0, NULL);
  fsym_init(code_314,1,"string()",0,0, NULL);
  fsym_init(code_313,3,"substr(,,)",0,0, NULL);
  fsym_init(code_312,3,"[<-]",0,0, NULL);
  fsym_init(code_311,2,"[]",0,0, NULL);
  fsym_init(code_147,2,",",147,0, NULL);
  fsym_init(code_310,0,"listExtract",0,0, NULL);
  fsym_init(code_146,1,"",146,0, NULL);
  fsym_init(code_145,1,"if",145,0, NULL);
  fsym_init(code_144,1,"call:state",144,0, NULL);
  fsym_init(code_143,0,"id",143,0, NULL);
  fsym_init(code_142,2,"",142,0, NULL);
  fsym_init(code_141,1,"",141,0, NULL);
  fsym_init(code_140,1,"iterateenditerate",140,0, NULL);
  
/* Initialisation des constantes */
  TERM_CONST_ALLOC(con_303, code_303);
  TERM_CONST_ALLOC(con_302, code_302);
  TERM_CONST_ALLOC(con_301, code_301);
  TERM_CONST_ALLOC(con_300, code_300);
  TERM_CONST_ALLOC(con_381, code_381);
  TERM_CONST_ALLOC(con_380, code_380);
  TERM_CONST_ALLOC(con_1, code_1);
  TERM_CONST_ALLOC(con_0, code_0);
  TERM_CONST_ALLOC(con_293, code_293);
  TERM_CONST_ALLOC(con_292, code_292);
  TERM_CONST_ALLOC(con_290, code_290);
  TERM_CONST_ALLOC(con_379, code_379);
  TERM_CONST_ALLOC(con_378, code_378);
  TERM_CONST_ALLOC(con_377, code_377);
  TERM_CONST_ALLOC(con_376, code_376);
  TERM_CONST_ALLOC(con_375, code_375);
  TERM_CONST_ALLOC(con_374, code_374);
  TERM_CONST_ALLOC(con_372, code_372);
  TERM_CONST_ALLOC(con_371, code_371);
  TERM_CONST_ALLOC(con_370, code_370);
  TERM_CONST_ALLOC(con_287, code_287);
  TERM_CONST_ALLOC(con_285, code_285);
  TERM_CONST_ALLOC(con_284, code_284);
  TERM_CONST_ALLOC(con_283, code_283);
  TERM_CONST_ALLOC(con_282, code_282);
  TERM_CONST_ALLOC(con_369, code_369);
  TERM_CONST_ALLOC(con_368, code_368);
  TERM_CONST_ALLOC(con_357, code_357);
  TERM_CONST_ALLOC(con_356, code_356);
  TERM_CONST_ALLOC(con_352, code_352);
  TERM_CONST_ALLOC(con_351, code_351);
  TERM_CONST_ALLOC(con_265, code_265);
  TERM_CONST_ALLOC(con_349, code_349);
  TERM_CONST_ALLOC(con_348, code_348);
  TERM_CONST_ALLOC(con_347, code_347);
  TERM_CONST_ALLOC(con_346, code_346);
  TERM_CONST_ALLOC(con_344, code_344);
  TERM_CONST_ALLOC(con_343, code_343);
  TERM_CONST_ALLOC(con_342, code_342);
  TERM_CONST_ALLOC(con_341, code_341);
  TERM_CONST_ALLOC(con_240, code_240);
  TERM_CONST_ALLOC(con_239, code_239);
  TERM_CONST_ALLOC(con_237, code_237);
  TERM_CONST_ALLOC(con_236, code_236);
  TERM_CONST_ALLOC(con_235, code_235);
  TERM_CONST_ALLOC(con_234, code_234);
  TERM_CONST_ALLOC(con_310, code_310);
  /* Initialisation des pattern_list */

  for(i=0 ; i<2 ; i++) {
    for(j=0 ; j<382; j++) {
      tab_rewrite_step[i][j] = 0;
    }

  }

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
  FREE(fsymtab[code_229].name);
  FREE(fsymtab[code_228].name);
  FREE(fsymtab[code_227].name);
  FREE(fsymtab[code_226].name);
  FREE(fsymtab[code_225].name);
  FREE(fsymtab[code_224].name);
  FREE(fsymtab[code_223].name);
  FREE(fsymtab[code_222].name);
  FREE(fsymtab[code_221].name);
  FREE(fsymtab[code_220].name);
  FREE(fsymtab[code_309].name);
  FREE(fsymtab[code_308].name);
  FREE(fsymtab[code_307].name);
  FREE(fsymtab[code_306].name);
  FREE(fsymtab[code_305].name);
  FREE(fsymtab[code_304].name);
  FREE(fsymtab[code_303].name);
  FREE(fsymtab[code_139].name);
  FREE(fsymtab[code_302].name);
  FREE(fsymtab[code_138].name);
  FREE(fsymtab[code_301].name);
  FREE(fsymtab[code_137].name);
  FREE(fsymtab[code_300].name);
  FREE(fsymtab[code_136].name);
  FREE(fsymtab[code_135].name);
  FREE(fsymtab[code_134].name);
  FREE(fsymtab[code_133].name);
  FREE(fsymtab[code_132].name);
  FREE(fsymtab[code_131].name);
  FREE(fsymtab[code_130].name);
  FREE(fsymtab[code_219].name);
  FREE(fsymtab[code_218].name);
  FREE(fsymtab[code_217].name);
  FREE(fsymtab[code_216].name);
  FREE(fsymtab[code_215].name);
  FREE(fsymtab[code_214].name);
  FREE(fsymtab[code_213].name);
  FREE(fsymtab[code_212].name);
  FREE(fsymtab[code_211].name);
  FREE(fsymtab[code_210].name);
  FREE(fsymtab[code_129].name);
  FREE(fsymtab[code_128].name);
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
  FREE(fsymtab[code_81].name);
  FREE(fsymtab[code_79].name);
  FREE(fsymtab[code_9].name);
  FREE(fsymtab[code_8].name);
  FREE(fsymtab[code_6].name);
  FREE(fsymtab[code_5].name);
  FREE(fsymtab[code_382].name);
  FREE(fsymtab[code_4].name);
  FREE(fsymtab[code_381].name);
  FREE(fsymtab[code_3].name);
  FREE(fsymtab[code_380].name);
  FREE(fsymtab[code_1].name);
  FREE(fsymtab[code_0].name);
  FREE(fsymtab[code_67].name);
  FREE(fsymtab[code_299].name);
  FREE(fsymtab[code_298].name);
  FREE(fsymtab[code_297].name);
  FREE(fsymtab[code_296].name);
  FREE(fsymtab[code_295].name);
  FREE(fsymtab[code_294].name);
  FREE(fsymtab[code_293].name);
  FREE(fsymtab[code_292].name);
  FREE(fsymtab[code_291].name);
  FREE(fsymtab[code_290].name);
  FREE(fsymtab[code_379].name);
  FREE(fsymtab[code_378].name);
  FREE(fsymtab[code_377].name);
  FREE(fsymtab[code_376].name);
  FREE(fsymtab[code_375].name);
  FREE(fsymtab[code_374].name);
  FREE(fsymtab[code_373].name);
  FREE(fsymtab[code_372].name);
  FREE(fsymtab[code_371].name);
  FREE(fsymtab[code_370].name);
  FREE(fsymtab[code_289].name);
  FREE(fsymtab[code_288].name);
  FREE(fsymtab[code_287].name);
  FREE(fsymtab[code_286].name);
  FREE(fsymtab[code_285].name);
  FREE(fsymtab[code_284].name);
  FREE(fsymtab[code_283].name);
  FREE(fsymtab[code_282].name);
  FREE(fsymtab[code_281].name);
  FREE(fsymtab[code_280].name);
  FREE(fsymtab[code_369].name);
  FREE(fsymtab[code_368].name);
  FREE(fsymtab[code_367].name);
  FREE(fsymtab[code_366].name);
  FREE(fsymtab[code_365].name);
  FREE(fsymtab[code_364].name);
  FREE(fsymtab[code_363].name);
  FREE(fsymtab[code_362].name);
  FREE(fsymtab[code_361].name);
  FREE(fsymtab[code_360].name);
  FREE(fsymtab[code_190].name);
  FREE(fsymtab[code_279].name);
  FREE(fsymtab[code_278].name);
  FREE(fsymtab[code_277].name);
  FREE(fsymtab[code_276].name);
  FREE(fsymtab[code_275].name);
  FREE(fsymtab[code_274].name);
  FREE(fsymtab[code_273].name);
  FREE(fsymtab[code_272].name);
  FREE(fsymtab[code_271].name);
  FREE(fsymtab[code_270].name);
  FREE(fsymtab[code_359].name);
  FREE(fsymtab[code_358].name);
  FREE(fsymtab[code_357].name);
  FREE(fsymtab[code_356].name);
  FREE(fsymtab[code_355].name);
  FREE(fsymtab[code_354].name);
  FREE(fsymtab[code_353].name);
  FREE(fsymtab[code_189].name);
  FREE(fsymtab[code_352].name);
  FREE(fsymtab[code_188].name);
  FREE(fsymtab[code_351].name);
  FREE(fsymtab[code_187].name);
  FREE(fsymtab[code_350].name);
  FREE(fsymtab[code_186].name);
  FREE(fsymtab[code_185].name);
  FREE(fsymtab[code_184].name);
  FREE(fsymtab[code_183].name);
  FREE(fsymtab[code_182].name);
  FREE(fsymtab[code_181].name);
  FREE(fsymtab[code_180].name);
  FREE(fsymtab[code_269].name);
  FREE(fsymtab[code_268].name);
  FREE(fsymtab[code_33].name);
  FREE(fsymtab[code_267].name);
  FREE(fsymtab[code_32].name);
  FREE(fsymtab[code_266].name);
  FREE(fsymtab[code_31].name);
  FREE(fsymtab[code_265].name);
  FREE(fsymtab[code_30].name);
  FREE(fsymtab[code_264].name);
  FREE(fsymtab[code_263].name);
  FREE(fsymtab[code_262].name);
  FREE(fsymtab[code_261].name);
  FREE(fsymtab[code_260].name);
  FREE(fsymtab[code_349].name);
  FREE(fsymtab[code_348].name);
  FREE(fsymtab[code_347].name);
  FREE(fsymtab[code_346].name);
  FREE(fsymtab[code_345].name);
  FREE(fsymtab[code_344].name);
  FREE(fsymtab[code_343].name);
  FREE(fsymtab[code_342].name);
  FREE(fsymtab[code_341].name);
  FREE(fsymtab[code_340].name);
  FREE(fsymtab[code_29].name);
  FREE(fsymtab[code_28].name);
  FREE(fsymtab[code_27].name);
  FREE(fsymtab[code_26].name);
  FREE(fsymtab[code_259].name);
  FREE(fsymtab[code_25].name);
  FREE(fsymtab[code_258].name);
  FREE(fsymtab[code_24].name);
  FREE(fsymtab[code_257].name);
  FREE(fsymtab[code_256].name);
  FREE(fsymtab[code_22].name);
  FREE(fsymtab[code_255].name);
  FREE(fsymtab[code_21].name);
  FREE(fsymtab[code_254].name);
  FREE(fsymtab[code_20].name);
  FREE(fsymtab[code_253].name);
  FREE(fsymtab[code_252].name);
  FREE(fsymtab[code_251].name);
  FREE(fsymtab[code_250].name);
  FREE(fsymtab[code_339].name);
  FREE(fsymtab[code_338].name);
  FREE(fsymtab[code_337].name);
  FREE(fsymtab[code_336].name);
  FREE(fsymtab[code_335].name);
  FREE(fsymtab[code_334].name);
  FREE(fsymtab[code_333].name);
  FREE(fsymtab[code_332].name);
  FREE(fsymtab[code_331].name);
  FREE(fsymtab[code_330].name);
  FREE(fsymtab[code_19].name);
  FREE(fsymtab[code_18].name);
  FREE(fsymtab[code_249].name);
  FREE(fsymtab[code_248].name);
  FREE(fsymtab[code_247].name);
  FREE(fsymtab[code_13].name);
  FREE(fsymtab[code_246].name);
  FREE(fsymtab[code_12].name);
  FREE(fsymtab[code_245].name);
  FREE(fsymtab[code_11].name);
  FREE(fsymtab[code_244].name);
  FREE(fsymtab[code_10].name);
  FREE(fsymtab[code_243].name);
  FREE(fsymtab[code_242].name);
  FREE(fsymtab[code_241].name);
  FREE(fsymtab[code_240].name);
  FREE(fsymtab[code_329].name);
  FREE(fsymtab[code_328].name);
  FREE(fsymtab[code_327].name);
  FREE(fsymtab[code_326].name);
  FREE(fsymtab[code_325].name);
  FREE(fsymtab[code_324].name);
  FREE(fsymtab[code_323].name);
  FREE(fsymtab[code_322].name);
  FREE(fsymtab[code_158].name);
  FREE(fsymtab[code_321].name);
  FREE(fsymtab[code_157].name);
  FREE(fsymtab[code_320].name);
  FREE(fsymtab[code_156].name);
  FREE(fsymtab[code_154].name);
  FREE(fsymtab[code_153].name);
  FREE(fsymtab[code_152].name);
  FREE(fsymtab[code_151].name);
  FREE(fsymtab[code_150].name);
  FREE(fsymtab[code_239].name);
  FREE(fsymtab[code_238].name);
  FREE(fsymtab[code_237].name);
  FREE(fsymtab[code_236].name);
  FREE(fsymtab[code_235].name);
  FREE(fsymtab[code_234].name);
  FREE(fsymtab[code_233].name);
  FREE(fsymtab[code_232].name);
  FREE(fsymtab[code_231].name);
  FREE(fsymtab[code_230].name);
  FREE(fsymtab[code_319].name);
  FREE(fsymtab[code_318].name);
  FREE(fsymtab[code_317].name);
  FREE(fsymtab[code_316].name);
  FREE(fsymtab[code_315].name);
  FREE(fsymtab[code_314].name);
  FREE(fsymtab[code_313].name);
  FREE(fsymtab[code_312].name);
  FREE(fsymtab[code_311].name);
  FREE(fsymtab[code_147].name);
  FREE(fsymtab[code_310].name);
  FREE(fsymtab[code_146].name);
  FREE(fsymtab[code_145].name);
  FREE(fsymtab[code_144].name);
  FREE(fsymtab[code_143].name);
  FREE(fsymtab[code_142].name);
  FREE(fsymtab[code_141].name);
  FREE(fsymtab[code_140].name);
  TERM_FREE(con_303);
  TERM_FREE(con_302);
  TERM_FREE(con_301);
  TERM_FREE(con_300);
  TERM_FREE(con_381);
  TERM_FREE(con_380);
  TERM_FREE(con_1);
  TERM_FREE(con_0);
  TERM_FREE(con_293);
  TERM_FREE(con_292);
  TERM_FREE(con_290);
  TERM_FREE(con_379);
  TERM_FREE(con_378);
  TERM_FREE(con_377);
  TERM_FREE(con_376);
  TERM_FREE(con_375);
  TERM_FREE(con_374);
  TERM_FREE(con_372);
  TERM_FREE(con_371);
  TERM_FREE(con_370);
  TERM_FREE(con_287);
  TERM_FREE(con_285);
  TERM_FREE(con_284);
  TERM_FREE(con_283);
  TERM_FREE(con_282);
  TERM_FREE(con_369);
  TERM_FREE(con_368);
  TERM_FREE(con_357);
  TERM_FREE(con_356);
  TERM_FREE(con_352);
  TERM_FREE(con_351);
  TERM_FREE(con_265);
  TERM_FREE(con_349);
  TERM_FREE(con_348);
  TERM_FREE(con_347);
  TERM_FREE(con_346);
  TERM_FREE(con_344);
  TERM_FREE(con_343);
  TERM_FREE(con_342);
  TERM_FREE(con_341);
  TERM_FREE(con_240);
  TERM_FREE(con_239);
  TERM_FREE(con_237);
  TERM_FREE(con_236);
  TERM_FREE(con_235);
  TERM_FREE(con_234);
  TERM_FREE(con_310);
  /* Destruction des pattern_list */
#ifdef DEBUG
  print_space_usage();
  backStatistics();
#endif
  printf("\nrewrite_step = %u\n",rewrite_step);
  for(j=0 ; j<382; j++) {
    if(tab_rewrite_step[0][j] > 0 || tab_rewrite_step[1][j] > 0)
      printf("tab_rewrite_step[%d] :	%u	%u\n",j,tab_rewrite_step[0][j],tab_rewrite_step[1][j]);
  }
}
#include "ac_tools.c"
