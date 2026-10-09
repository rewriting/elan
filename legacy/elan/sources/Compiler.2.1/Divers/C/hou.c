#include "hou.h"
extern struct term *query;

/* Constantes d'execution */
unsigned long rewrite_step=0;
unsigned long tab_rewrite_step[2][480];
int trace=0;
int global_indentlevel=0;
int debugMode=0;

/* Table des symboles */
int fsymtabSize = 481;
fsym fsymtab[481];
/* Declaration des pattern_list */
void init_pattern_list_458_439();
void delete_pattern_list_458_439();
void init_pattern_list_463_439();
void delete_pattern_list_463_439();
void init_pattern_list_439();
void delete_pattern_list_439();
void init_pattern_list_467_439();
void delete_pattern_list_467_439();
void init_pattern_list_465_439();
void delete_pattern_list_465_439();
                    void init_pattern_list_252_repeat1_cons1_repeat1_ONE1_one_439();
                    void delete_pattern_list_252_repeat1_cons1_repeat1_ONE1_one_439();
                  void init_pattern_list_252_repeat1_cons1_repeat1_ONE3_cons1_one_439();
                  void delete_pattern_list_252_repeat1_cons1_repeat1_ONE3_cons1_one_439();
                void init_pattern_list_252_repeat1_cons4_repeat1_one_439();
                void delete_pattern_list_252_repeat1_cons4_repeat1_one_439();
          void init_pattern_list_158_repeat1_one_439();
          void delete_pattern_list_158_repeat1_one_439();

/* Constantes */
struct term *con_350;
struct term *con_349;
struct term *con_347;
struct term *con_336;
struct term *con_335;
struct term *con_317;
struct term *con_310;
struct term *con_307;
struct term *con_306;
struct term *con_305;
struct term *con_301;
struct term *con_294;
struct term *con_291;
struct term *con_290;
struct term *con_286;
struct term *con_279;
struct term *con_278;
struct term *con_271;
struct term *con_480;
struct term *con_267;
struct term *con_266;
struct term *con_259;
struct term *con_258;
struct term *con_252;
struct term *con_245;
struct term *con_240;
struct term *con_1;
struct term *con_0;
struct term *con_229;
struct term *con_228;
struct term *con_226;
struct term *con_436;
struct term *con_219;
struct term *con_426;
struct term *con_419;
struct term *con_418;
struct term *con_411;
struct term *con_402;
struct term *con_401;
struct term *con_400;

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
(funTabType) &fun_207, (funTabType) &fun_208, (funTabType) &fun_209, 
(funTabType) &fun_210, (funTabType) &fun_211, (funTabType) &fun_212, 
(funTabType) &fun_213, (funTabType) &fun_214, (funTabType) &fun_215, 
(funTabType) &fun_216, (funTabType) &fun_217, (funTabType) &fun_218, 
NULL, NULL, (funTabType) &fun_221, NULL, (funTabType) &fun_223, 
(funTabType) &fun_224, (funTabType) &fun_225, NULL, 
(funTabType) &fun_227, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &fun_233, (funTabType) &fun_234, (funTabType) &fun_235, 
(funTabType) &fun_236, (funTabType) &fun_237, (funTabType) &fun_238, 
(funTabType) &fun_239, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &fun_247, NULL, (funTabType) &fun_249, 
(funTabType) &fun_250, (funTabType) &fun_251, NULL, NULL, 
(funTabType) &fun_254, (funTabType) &fun_255, (funTabType) &fun_256, 
(funTabType) &fun_257, NULL, NULL, NULL, NULL, (funTabType) &fun_262, 
(funTabType) &fun_263, (funTabType) &fun_264, (funTabType) &fun_265, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, (funTabType) &fun_273, 
NULL, (funTabType) &fun_275, (funTabType) &fun_276, 
(funTabType) &fun_277, NULL, NULL, NULL, (funTabType) &fun_281, NULL, 
(funTabType) &fun_283, (funTabType) &fun_284, (funTabType) &fun_285, 
NULL, (funTabType) &fun_287, NULL, NULL, NULL, NULL, 
(funTabType) &fun_292, (funTabType) &fun_293, NULL, NULL, 
(funTabType) &fun_296, NULL, (funTabType) &fun_298, 
(funTabType) &fun_299, (funTabType) &fun_300, NULL, 
(funTabType) &fun_302, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &fun_308, (funTabType) &fun_309, NULL, NULL, 
(funTabType) &fun_312, NULL, (funTabType) &fun_314, 
(funTabType) &fun_315, (funTabType) &fun_316, NULL, NULL, 
(funTabType) &fun_319, (funTabType) &fun_320, (funTabType) &fun_321, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, (funTabType) &fun_339, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, (funTabType) &fun_355, (funTabType) &fun_356, 
(funTabType) &fun_357, (funTabType) &fun_358, (funTabType) &fun_359, 
(funTabType) &fun_360, (funTabType) &fun_361, (funTabType) &fun_362, 
(funTabType) &fun_363, (funTabType) &fun_364, (funTabType) &fun_365, 
(funTabType) &fun_366, (funTabType) &fun_367, (funTabType) &fun_368, 
(funTabType) &fun_369, (funTabType) &fun_370, (funTabType) &fun_371, 
(funTabType) &fun_372, (funTabType) &fun_373, (funTabType) &fun_374, 
(funTabType) &fun_375, (funTabType) &fun_376, (funTabType) &fun_377, 
(funTabType) &fun_378, (funTabType) &fun_379, NULL, 
(funTabType) &fun_381, (funTabType) &fun_382, (funTabType) &fun_383, 
(funTabType) &fun_384, (funTabType) &fun_385, (funTabType) &fun_386, 
(funTabType) &fun_387, (funTabType) &fun_388, (funTabType) &fun_389, 
(funTabType) &fun_390, (funTabType) &fun_391, (funTabType) &fun_392, 
(funTabType) &fun_393, (funTabType) &fun_394, (funTabType) &fun_395, 
(funTabType) &fun_396, (funTabType) &fun_397, (funTabType) &fun_398, 
(funTabType) &fun_399, NULL, NULL, NULL, (funTabType) &fun_403, 
(funTabType) &fun_404, (funTabType) &fun_405, (funTabType) &fun_406, 
(funTabType) &fun_407, (funTabType) &fun_408, (funTabType) &fun_409, 
(funTabType) &fun_410, NULL, NULL, (funTabType) &fun_413, NULL, 
(funTabType) &fun_415, (funTabType) &fun_416, (funTabType) &fun_417, 
NULL, NULL, NULL, (funTabType) &fun_421, NULL, (funTabType) &fun_423, 
(funTabType) &fun_424, (funTabType) &fun_425, NULL, 
(funTabType) &fun_427, (funTabType) &fun_428, (funTabType) &fun_429, 
NULL, NULL, NULL, NULL, NULL, (funTabType) &fun_435, NULL, NULL, 
NULL, (funTabType) &fun_439, NULL, (funTabType) &fun_441, 
(funTabType) &fun_442, (funTabType) &fun_443, (funTabType) &fun_444, 
(funTabType) &fun_445, (funTabType) &fun_446, (funTabType) &fun_447, 
(funTabType) &fun_448, NULL, NULL, (funTabType) &fun_451, 
(funTabType) &fun_452, (funTabType) &fun_453, (funTabType) &fun_454, 
(funTabType) &fun_455, (funTabType) &fun_456, (funTabType) &fun_457, 
(funTabType) &fun_458, (funTabType) &fun_459, (funTabType) &fun_460, 
(funTabType) &fun_461, (funTabType) &fun_462, (funTabType) &fun_463, 
(funTabType) &fun_464, (funTabType) &fun_465, (funTabType) &fun_466, 
(funTabType) &fun_467, (funTabType) &fun_468, (funTabType) &fun_469, 
(funTabType) &fun_470, (funTabType) &fun_471, (funTabType) &fun_472, 
(funTabType) &fun_473, (funTabType) &fun_474, (funTabType) &fun_475, 
(funTabType) &fun_476, (funTabType) &fun_477, (funTabType) &fun_478, 
(funTabType) &fun_479, NULL, NULL};

/* tableau de pointeurs de fonctions qui retournent */
/* un pointeur sur un term */
funTabType strTab[] = {
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, (funTabType) &str_26, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, (funTabType) &str_89, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, (funTabType) &str_122, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &str_158, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, (funTabType) &str_172, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &str_248, NULL, NULL, NULL, (funTabType) &str_252, NULL, 
NULL, NULL, NULL, NULL, (funTabType) &str_258, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &str_270, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
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
NULL, NULL, (funTabType) &str_390, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, (funTabType) &str_402, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &str_468, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, (funTabType) &str_494, NULL};

/* Initialisation des patterns AC */
TERM *EkerTerm[100];
static void EkerTermInit() {
  TERM_LIST *vlist[100];
  AC_LIST *acvlist[100];
  struct term *sv[100];
  /* =((:(var9,|-(var10,var11))),(:((var8),|-(var10,var11)))) */
    /* AC pattern construction phase */
  sv[14] = (struct term*) make_term(0,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[14],(TERM_LIST *) NULL);
  sv[12] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[12],(TERM_LIST *) NULL);
  sv[11] = (struct term*) make_term(2,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[11],vlist[1]);
  sv[13] = (struct term*) make_term(340,vlist[1],FUNC);
  vlist[0] = make_term_list((TERM*) sv[13],vlist[0]);
  sv[15] = (struct term*) make_term(348,vlist[0],FUNC);
  vlist[0] = make_term_list((TERM*) sv[15],(TERM_LIST *) NULL);
  sv[16] = (struct term*) make_term(433,vlist[0],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[16],1,(AC_LIST *) NULL);
  sv[7] = (struct term*) make_term(3,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[7],(TERM_LIST *) NULL);
  sv[8] = (struct term*) make_term(346,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[8],(TERM_LIST *) NULL);
  sv[12] = (struct term*) make_term(1,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[12],(TERM_LIST *) NULL);
  sv[11] = (struct term*) make_term(2,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[11],vlist[2]);
  sv[6] = (struct term*) make_term(340,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[6],vlist[1]);
  sv[9] = (struct term*) make_term(348,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[9],(TERM_LIST *) NULL);
  sv[10] = (struct term*) make_term(433,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[10],1,acvlist[0]);
  sv[17] = (struct term*) make_ac_term(435,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[17]);
  //printf("\n");
  ac_sort((TERM*)sv[17]);
  EkerTerm[6] = (TERM*)sv[17];
  /* =(([,,,,](:((la.)(var9,:(var8,|-(.(var9,var10),var11))),|-(var10,(->)(var9,var11))),:(var16,|>(var12,var10)),nil,var12,(->)(var9,var11))),([,,,,](:((la.)(var9,:(var14,|-(.(var9,var10),var11))),|-(var10,(->)(var9,var11))),:(var17,|>(var12,var10)),nil,var12,(->)(var9,var11)))) */
    /* AC pattern construction phase */
  sv[39] = (struct term*) make_term(0,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[39],(TERM_LIST *) NULL);
  sv[37] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[37],(TERM_LIST *) NULL);
  sv[39] = (struct term*) make_term(0,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[39],(TERM_LIST *) NULL);
  sv[34] = (struct term*) make_term(2,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[34],vlist[2]);
  sv[35] = (struct term*) make_term(260,vlist[2],FUNC);
  vlist[2] = make_term_list((TERM*) sv[35],(TERM_LIST *) NULL);
  sv[33] = (struct term*) make_term(3,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[33],vlist[2]);
  sv[36] = (struct term*) make_term(340,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[36],vlist[1]);
  sv[38] = (struct term*) make_term(348,vlist[1],FUNC);
  vlist[0] = make_term_list((TERM*) sv[38],vlist[0]);
  sv[40] = (struct term*) make_term(343,vlist[0],FUNC);
  vlist[0] = make_term_list((TERM*) sv[40],(TERM_LIST *) NULL);
  sv[34] = (struct term*) make_term(2,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[34],(TERM_LIST *) NULL);
  sv[39] = (struct term*) make_term(0,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[39],(TERM_LIST *) NULL);
  sv[33] = (struct term*) make_term(3,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[33],vlist[2]);
  sv[31] = (struct term*) make_term(231,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[31],vlist[1]);
  sv[32] = (struct term*) make_term(340,vlist[1],FUNC);
  vlist[0] = make_term_list((TERM*) sv[32],vlist[0]);
  sv[41] = (struct term*) make_term(348,vlist[0],FUNC);
  vlist[0] = make_term_list((TERM*) sv[41],(TERM_LIST *) NULL);
  sv[29] = (struct term*) make_term(4,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[29],(TERM_LIST *) NULL);
  sv[27] = (struct term*) make_term(5,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[27],(TERM_LIST *) NULL);
  sv[34] = (struct term*) make_term(2,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[34],vlist[2]);
  sv[28] = (struct term*) make_term(341,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[28],vlist[1]);
  sv[30] = (struct term*) make_term(354,vlist[1],FUNC);
  vlist[0] = make_term_list((TERM*) sv[30],vlist[0]);
  sv[26] = (struct term*) make_term(310,NULL,CONSTAN);
  vlist[0] = make_term_list((TERM*) sv[26],vlist[0]);
  sv[27] = (struct term*) make_term(5,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[27],vlist[0]);
  sv[39] = (struct term*) make_term(0,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[39],(TERM_LIST *) NULL);
  sv[33] = (struct term*) make_term(3,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[33],vlist[1]);
  sv[25] = (struct term*) make_term(231,vlist[1],FUNC);
  vlist[0] = make_term_list((TERM*) sv[25],vlist[0]);
  sv[42] = (struct term*) make_term(380,vlist[0],FUNC);
  vlist[0] = make_term_list((TERM*) sv[42],(TERM_LIST *) NULL);
  sv[43] = (struct term*) make_term(434,vlist[0],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[43],1,(AC_LIST *) NULL);
  sv[39] = (struct term*) make_term(0,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[39],(TERM_LIST *) NULL);
  sv[19] = (struct term*) make_term(6,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[19],(TERM_LIST *) NULL);
  sv[39] = (struct term*) make_term(0,NULL,VAR);
  vlist[3] = make_term_list((TERM*) sv[39],(TERM_LIST *) NULL);
  sv[34] = (struct term*) make_term(2,NULL,VAR);
  vlist[3] = make_term_list((TERM*) sv[34],vlist[3]);
  sv[17] = (struct term*) make_term(260,vlist[3],FUNC);
  vlist[3] = make_term_list((TERM*) sv[17],(TERM_LIST *) NULL);
  sv[33] = (struct term*) make_term(3,NULL,VAR);
  vlist[3] = make_term_list((TERM*) sv[33],vlist[3]);
  sv[18] = (struct term*) make_term(340,vlist[3],FUNC);
  vlist[2] = make_term_list((TERM*) sv[18],vlist[2]);
  sv[20] = (struct term*) make_term(348,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[20],vlist[1]);
  sv[21] = (struct term*) make_term(343,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[21],(TERM_LIST *) NULL);
  sv[34] = (struct term*) make_term(2,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[34],(TERM_LIST *) NULL);
  sv[39] = (struct term*) make_term(0,NULL,VAR);
  vlist[3] = make_term_list((TERM*) sv[39],(TERM_LIST *) NULL);
  sv[33] = (struct term*) make_term(3,NULL,VAR);
  vlist[3] = make_term_list((TERM*) sv[33],vlist[3]);
  sv[15] = (struct term*) make_term(231,vlist[3],FUNC);
  vlist[2] = make_term_list((TERM*) sv[15],vlist[2]);
  sv[16] = (struct term*) make_term(340,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[16],vlist[1]);
  sv[22] = (struct term*) make_term(348,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[22],(TERM_LIST *) NULL);
  sv[13] = (struct term*) make_term(7,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[13],(TERM_LIST *) NULL);
  sv[27] = (struct term*) make_term(5,NULL,VAR);
  vlist[3] = make_term_list((TERM*) sv[27],(TERM_LIST *) NULL);
  sv[34] = (struct term*) make_term(2,NULL,VAR);
  vlist[3] = make_term_list((TERM*) sv[34],vlist[3]);
  sv[12] = (struct term*) make_term(341,vlist[3],FUNC);
  vlist[2] = make_term_list((TERM*) sv[12],vlist[2]);
  sv[14] = (struct term*) make_term(354,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[14],vlist[1]);
  sv[11] = (struct term*) make_term(310,NULL,CONSTAN);
  vlist[1] = make_term_list((TERM*) sv[11],vlist[1]);
  sv[27] = (struct term*) make_term(5,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[27],vlist[1]);
  sv[39] = (struct term*) make_term(0,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[39],(TERM_LIST *) NULL);
  sv[33] = (struct term*) make_term(3,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[33],vlist[2]);
  sv[10] = (struct term*) make_term(231,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[10],vlist[1]);
  sv[23] = (struct term*) make_term(380,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[23],(TERM_LIST *) NULL);
  sv[24] = (struct term*) make_term(434,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[24],1,acvlist[0]);
  sv[44] = (struct term*) make_ac_term(435,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[44]);
  //printf("\n");
  ac_sort((TERM*)sv[44]);
  EkerTerm[0] = (TERM*)sv[44];
  /* =(([,,,,](:(var9,|-(var10,var11)),:(var12,|>(var13,var10)),var14,var15,var16)),([,,,,](:((var8),|-(var15,var16)),:(id,|>(var15,var15)),nil,var15,var16))) */
    /* AC pattern construction phase */
  sv[31] = (struct term*) make_term(0,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[31],(TERM_LIST *) NULL);
  sv[29] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[29],(TERM_LIST *) NULL);
  sv[28] = (struct term*) make_term(2,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[28],vlist[1]);
  sv[30] = (struct term*) make_term(340,vlist[1],FUNC);
  vlist[0] = make_term_list((TERM*) sv[30],vlist[0]);
  sv[32] = (struct term*) make_term(348,vlist[0],FUNC);
  vlist[0] = make_term_list((TERM*) sv[32],(TERM_LIST *) NULL);
  sv[26] = (struct term*) make_term(3,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[26],(TERM_LIST *) NULL);
  sv[24] = (struct term*) make_term(4,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[24],(TERM_LIST *) NULL);
  sv[29] = (struct term*) make_term(1,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[29],vlist[2]);
  sv[25] = (struct term*) make_term(341,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[25],vlist[1]);
  sv[27] = (struct term*) make_term(354,vlist[1],FUNC);
  vlist[0] = make_term_list((TERM*) sv[27],vlist[0]);
  sv[23] = (struct term*) make_term(5,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[23],vlist[0]);
  sv[22] = (struct term*) make_term(6,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[22],vlist[0]);
  sv[21] = (struct term*) make_term(7,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[21],vlist[0]);
  sv[33] = (struct term*) make_term(380,vlist[0],FUNC);
  vlist[0] = make_term_list((TERM*) sv[33],(TERM_LIST *) NULL);
  sv[34] = (struct term*) make_term(434,vlist[0],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[34],1,(AC_LIST *) NULL);
  sv[16] = (struct term*) make_term(8,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[16],(TERM_LIST *) NULL);
  sv[17] = (struct term*) make_term(346,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[17],(TERM_LIST *) NULL);
  sv[22] = (struct term*) make_term(6,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[22],(TERM_LIST *) NULL);
  sv[21] = (struct term*) make_term(7,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[21],vlist[2]);
  sv[15] = (struct term*) make_term(340,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[15],vlist[1]);
  sv[18] = (struct term*) make_term(348,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[18],(TERM_LIST *) NULL);
  sv[13] = (struct term*) make_term(349,NULL,CONSTAN);
  vlist[2] = make_term_list((TERM*) sv[13],(TERM_LIST *) NULL);
  sv[22] = (struct term*) make_term(6,NULL,VAR);
  vlist[3] = make_term_list((TERM*) sv[22],(TERM_LIST *) NULL);
  sv[22] = (struct term*) make_term(6,NULL,VAR);
  vlist[3] = make_term_list((TERM*) sv[22],vlist[3]);
  sv[12] = (struct term*) make_term(341,vlist[3],FUNC);
  vlist[2] = make_term_list((TERM*) sv[12],vlist[2]);
  sv[14] = (struct term*) make_term(354,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[14],vlist[1]);
  sv[11] = (struct term*) make_term(310,NULL,CONSTAN);
  vlist[1] = make_term_list((TERM*) sv[11],vlist[1]);
  sv[22] = (struct term*) make_term(6,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[22],vlist[1]);
  sv[21] = (struct term*) make_term(7,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[21],vlist[1]);
  sv[19] = (struct term*) make_term(380,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[19],(TERM_LIST *) NULL);
  sv[20] = (struct term*) make_term(434,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[20],1,acvlist[0]);
  sv[35] = (struct term*) make_ac_term(435,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[35]);
  //printf("\n");
  ac_sort((TERM*)sv[35]);
  EkerTerm[4] = (TERM*)sv[35];
  /* =(([,,,,](:(#(var8),|-(var9,var10)),:(id,|>(var9,var9)),var11,var12,var13)),([,,,,](:(#(var14),|-(var15,var16)),:(id,|>(var15,var15)),var17,var12,var13))) */
    /* AC pattern construction phase */
  sv[33] = (struct term*) make_term(0,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[33],(TERM_LIST *) NULL);
  sv[34] = (struct term*) make_term(345,vlist[0],FUNC);
  vlist[0] = make_term_list((TERM*) sv[34],(TERM_LIST *) NULL);
  sv[27] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[27],(TERM_LIST *) NULL);
  sv[31] = (struct term*) make_term(2,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[31],vlist[1]);
  sv[32] = (struct term*) make_term(340,vlist[1],FUNC);
  vlist[0] = make_term_list((TERM*) sv[32],vlist[0]);
  sv[35] = (struct term*) make_term(348,vlist[0],FUNC);
  vlist[0] = make_term_list((TERM*) sv[35],(TERM_LIST *) NULL);
  sv[29] = (struct term*) make_term(349,NULL,CONSTAN);
  vlist[1] = make_term_list((TERM*) sv[29],(TERM_LIST *) NULL);
  sv[27] = (struct term*) make_term(1,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[27],(TERM_LIST *) NULL);
  sv[27] = (struct term*) make_term(1,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[27],vlist[2]);
  sv[28] = (struct term*) make_term(341,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[28],vlist[1]);
  sv[30] = (struct term*) make_term(354,vlist[1],FUNC);
  vlist[0] = make_term_list((TERM*) sv[30],vlist[0]);
  sv[26] = (struct term*) make_term(3,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[26],vlist[0]);
  sv[13] = (struct term*) make_term(4,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[13],vlist[0]);
  sv[12] = (struct term*) make_term(5,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[12],vlist[0]);
  sv[36] = (struct term*) make_term(380,vlist[0],FUNC);
  vlist[0] = make_term_list((TERM*) sv[36],(TERM_LIST *) NULL);
  sv[37] = (struct term*) make_term(434,vlist[0],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[37],1,(AC_LIST *) NULL);
  sv[21] = (struct term*) make_term(6,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[21],(TERM_LIST *) NULL);
  sv[22] = (struct term*) make_term(345,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[22],(TERM_LIST *) NULL);
  sv[15] = (struct term*) make_term(7,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[15],(TERM_LIST *) NULL);
  sv[19] = (struct term*) make_term(8,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[19],vlist[2]);
  sv[20] = (struct term*) make_term(340,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[20],vlist[1]);
  sv[23] = (struct term*) make_term(348,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[23],(TERM_LIST *) NULL);
  sv[17] = (struct term*) make_term(349,NULL,CONSTAN);
  vlist[2] = make_term_list((TERM*) sv[17],(TERM_LIST *) NULL);
  sv[15] = (struct term*) make_term(7,NULL,VAR);
  vlist[3] = make_term_list((TERM*) sv[15],(TERM_LIST *) NULL);
  sv[15] = (struct term*) make_term(7,NULL,VAR);
  vlist[3] = make_term_list((TERM*) sv[15],vlist[3]);
  sv[16] = (struct term*) make_term(341,vlist[3],FUNC);
  vlist[2] = make_term_list((TERM*) sv[16],vlist[2]);
  sv[18] = (struct term*) make_term(354,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[18],vlist[1]);
  sv[14] = (struct term*) make_term(9,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[14],vlist[1]);
  sv[13] = (struct term*) make_term(4,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[13],vlist[1]);
  sv[12] = (struct term*) make_term(5,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[12],vlist[1]);
  sv[24] = (struct term*) make_term(380,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[24],(TERM_LIST *) NULL);
  sv[25] = (struct term*) make_term(434,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[25],1,acvlist[0]);
  sv[38] = (struct term*) make_ac_term(435,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[38]);
  //printf("\n");
  ac_sort((TERM*)sv[38]);
  EkerTerm[2] = (TERM*)sv[38];
  /* =(([,,,,](:(#(var9),|-(var10,var11)),:(id,|>(var10,var10)),var12,var13,var14)),([,,,,](:(#(var9),|-(var10,var11)),:(id,|>(var10,var10)),var15,var13,var14))) */
    /* AC pattern construction phase */
  sv[17] = (struct term*) make_term(0,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[17],(TERM_LIST *) NULL);
  sv[28] = (struct term*) make_term(345,vlist[0],FUNC);
  vlist[0] = make_term_list((TERM*) sv[28],(TERM_LIST *) NULL);
  sv[23] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[23],(TERM_LIST *) NULL);
  sv[15] = (struct term*) make_term(2,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[15],vlist[1]);
  sv[27] = (struct term*) make_term(340,vlist[1],FUNC);
  vlist[0] = make_term_list((TERM*) sv[27],vlist[0]);
  sv[29] = (struct term*) make_term(348,vlist[0],FUNC);
  vlist[0] = make_term_list((TERM*) sv[29],(TERM_LIST *) NULL);
  sv[25] = (struct term*) make_term(349,NULL,CONSTAN);
  vlist[1] = make_term_list((TERM*) sv[25],(TERM_LIST *) NULL);
  sv[23] = (struct term*) make_term(1,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[23],(TERM_LIST *) NULL);
  sv[23] = (struct term*) make_term(1,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[23],vlist[2]);
  sv[24] = (struct term*) make_term(341,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[24],vlist[1]);
  sv[26] = (struct term*) make_term(354,vlist[1],FUNC);
  vlist[0] = make_term_list((TERM*) sv[26],vlist[0]);
  sv[22] = (struct term*) make_term(3,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[22],vlist[0]);
  sv[10] = (struct term*) make_term(4,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[10],vlist[0]);
  sv[9] = (struct term*) make_term(5,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[9],vlist[0]);
  sv[30] = (struct term*) make_term(380,vlist[0],FUNC);
  vlist[0] = make_term_list((TERM*) sv[30],(TERM_LIST *) NULL);
  sv[31] = (struct term*) make_term(434,vlist[0],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[31],1,(AC_LIST *) NULL);
  sv[17] = (struct term*) make_term(0,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[17],(TERM_LIST *) NULL);
  sv[18] = (struct term*) make_term(345,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[18],(TERM_LIST *) NULL);
  sv[23] = (struct term*) make_term(1,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[23],(TERM_LIST *) NULL);
  sv[15] = (struct term*) make_term(2,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[15],vlist[2]);
  sv[16] = (struct term*) make_term(340,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[16],vlist[1]);
  sv[19] = (struct term*) make_term(348,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[19],(TERM_LIST *) NULL);
  sv[13] = (struct term*) make_term(349,NULL,CONSTAN);
  vlist[2] = make_term_list((TERM*) sv[13],(TERM_LIST *) NULL);
  sv[23] = (struct term*) make_term(1,NULL,VAR);
  vlist[3] = make_term_list((TERM*) sv[23],(TERM_LIST *) NULL);
  sv[23] = (struct term*) make_term(1,NULL,VAR);
  vlist[3] = make_term_list((TERM*) sv[23],vlist[3]);
  sv[12] = (struct term*) make_term(341,vlist[3],FUNC);
  vlist[2] = make_term_list((TERM*) sv[12],vlist[2]);
  sv[14] = (struct term*) make_term(354,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[14],vlist[1]);
  sv[11] = (struct term*) make_term(6,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[11],vlist[1]);
  sv[10] = (struct term*) make_term(4,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[10],vlist[1]);
  sv[9] = (struct term*) make_term(5,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[9],vlist[1]);
  sv[20] = (struct term*) make_term(380,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[20],(TERM_LIST *) NULL);
  sv[21] = (struct term*) make_term(434,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[21],1,acvlist[0]);
  sv[32] = (struct term*) make_ac_term(435,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[32]);
  //printf("\n");
  ac_sort((TERM*)sv[32]);
  EkerTerm[1] = (TERM*)sv[32];
  /* =((:(var9,|-(.(var8,var10),var11))),(:(var12,|-(.(var8,var10),var11)))) */
    /* AC pattern construction phase */
  sv[17] = (struct term*) make_term(0,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[17],(TERM_LIST *) NULL);
  sv[14] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[14],(TERM_LIST *) NULL);
  sv[13] = (struct term*) make_term(2,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[13],vlist[1]);
  sv[15] = (struct term*) make_term(260,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[15],(TERM_LIST *) NULL);
  sv[12] = (struct term*) make_term(3,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[12],vlist[1]);
  sv[16] = (struct term*) make_term(340,vlist[1],FUNC);
  vlist[0] = make_term_list((TERM*) sv[16],vlist[0]);
  sv[18] = (struct term*) make_term(348,vlist[0],FUNC);
  vlist[0] = make_term_list((TERM*) sv[18],(TERM_LIST *) NULL);
  sv[19] = (struct term*) make_term(433,vlist[0],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[19],1,(AC_LIST *) NULL);
  sv[9] = (struct term*) make_term(4,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[9],(TERM_LIST *) NULL);
  sv[14] = (struct term*) make_term(1,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[14],(TERM_LIST *) NULL);
  sv[13] = (struct term*) make_term(2,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[13],vlist[2]);
  sv[7] = (struct term*) make_term(260,vlist[2],FUNC);
  vlist[2] = make_term_list((TERM*) sv[7],(TERM_LIST *) NULL);
  sv[12] = (struct term*) make_term(3,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[12],vlist[2]);
  sv[8] = (struct term*) make_term(340,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[8],vlist[1]);
  sv[10] = (struct term*) make_term(348,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[10],(TERM_LIST *) NULL);
  sv[11] = (struct term*) make_term(433,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[11],1,acvlist[0]);
  sv[20] = (struct term*) make_ac_term(435,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[20]);
  //printf("\n");
  ac_sort((TERM*)sv[20]);
  EkerTerm[7] = (TERM*)sv[20];
  /* =(([,,,,](:(#(var14),|-(var15,var16)),:(id,|>(var15,var15)),var17,var12,var13)),([,,,,](:((var8),|-(var9,var10)),var11,nil,var12,var13))) */
    /* AC pattern construction phase */
  sv[31] = (struct term*) make_term(0,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[31],(TERM_LIST *) NULL);
  sv[32] = (struct term*) make_term(345,vlist[0],FUNC);
  vlist[0] = make_term_list((TERM*) sv[32],(TERM_LIST *) NULL);
  sv[29] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[29],(TERM_LIST *) NULL);
  sv[28] = (struct term*) make_term(2,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[28],vlist[1]);
  sv[30] = (struct term*) make_term(340,vlist[1],FUNC);
  vlist[0] = make_term_list((TERM*) sv[30],vlist[0]);
  sv[33] = (struct term*) make_term(348,vlist[0],FUNC);
  vlist[0] = make_term_list((TERM*) sv[33],(TERM_LIST *) NULL);
  sv[26] = (struct term*) make_term(349,NULL,CONSTAN);
  vlist[1] = make_term_list((TERM*) sv[26],(TERM_LIST *) NULL);
  sv[29] = (struct term*) make_term(1,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[29],(TERM_LIST *) NULL);
  sv[29] = (struct term*) make_term(1,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[29],vlist[2]);
  sv[25] = (struct term*) make_term(341,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[25],vlist[1]);
  sv[27] = (struct term*) make_term(354,vlist[1],FUNC);
  vlist[0] = make_term_list((TERM*) sv[27],vlist[0]);
  sv[24] = (struct term*) make_term(3,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[24],vlist[0]);
  sv[23] = (struct term*) make_term(4,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[23],vlist[0]);
  sv[22] = (struct term*) make_term(5,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[22],vlist[0]);
  sv[34] = (struct term*) make_term(380,vlist[0],FUNC);
  vlist[0] = make_term_list((TERM*) sv[34],(TERM_LIST *) NULL);
  sv[35] = (struct term*) make_term(434,vlist[0],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[35],1,(AC_LIST *) NULL);
  sv[17] = (struct term*) make_term(6,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[17],(TERM_LIST *) NULL);
  sv[18] = (struct term*) make_term(346,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[18],(TERM_LIST *) NULL);
  sv[15] = (struct term*) make_term(7,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[15],(TERM_LIST *) NULL);
  sv[14] = (struct term*) make_term(8,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[14],vlist[2]);
  sv[16] = (struct term*) make_term(340,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[16],vlist[1]);
  sv[19] = (struct term*) make_term(348,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[19],(TERM_LIST *) NULL);
  sv[13] = (struct term*) make_term(9,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[13],vlist[1]);
  sv[12] = (struct term*) make_term(310,NULL,CONSTAN);
  vlist[1] = make_term_list((TERM*) sv[12],vlist[1]);
  sv[23] = (struct term*) make_term(4,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[23],vlist[1]);
  sv[22] = (struct term*) make_term(5,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[22],vlist[1]);
  sv[20] = (struct term*) make_term(380,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[20],(TERM_LIST *) NULL);
  sv[21] = (struct term*) make_term(434,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[21],1,acvlist[0]);
  sv[36] = (struct term*) make_ac_term(435,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[36]);
  //printf("\n");
  ac_sort((TERM*)sv[36]);
  EkerTerm[5] = (TERM*)sv[36];
  /* =(([,,,,](:(var11,|-(var12,var13)),:(var14,|>(var15,var12)),var16,var9,var10)),([,,,,](:((var8),|-(var9,var10)),:(id,|>(var9,var9)),nil,var9,var10))) */
    /* AC pattern construction phase */
  sv[31] = (struct term*) make_term(0,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[31],(TERM_LIST *) NULL);
  sv[29] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[29],(TERM_LIST *) NULL);
  sv[28] = (struct term*) make_term(2,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[28],vlist[1]);
  sv[30] = (struct term*) make_term(340,vlist[1],FUNC);
  vlist[0] = make_term_list((TERM*) sv[30],vlist[0]);
  sv[32] = (struct term*) make_term(348,vlist[0],FUNC);
  vlist[0] = make_term_list((TERM*) sv[32],(TERM_LIST *) NULL);
  sv[26] = (struct term*) make_term(3,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[26],(TERM_LIST *) NULL);
  sv[24] = (struct term*) make_term(4,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[24],(TERM_LIST *) NULL);
  sv[29] = (struct term*) make_term(1,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[29],vlist[2]);
  sv[25] = (struct term*) make_term(341,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[25],vlist[1]);
  sv[27] = (struct term*) make_term(354,vlist[1],FUNC);
  vlist[0] = make_term_list((TERM*) sv[27],vlist[0]);
  sv[23] = (struct term*) make_term(5,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[23],vlist[0]);
  sv[22] = (struct term*) make_term(6,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[22],vlist[0]);
  sv[21] = (struct term*) make_term(7,NULL,VAR);
  vlist[0] = make_term_list((TERM*) sv[21],vlist[0]);
  sv[33] = (struct term*) make_term(380,vlist[0],FUNC);
  vlist[0] = make_term_list((TERM*) sv[33],(TERM_LIST *) NULL);
  sv[34] = (struct term*) make_term(434,vlist[0],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[34],1,(AC_LIST *) NULL);
  sv[16] = (struct term*) make_term(8,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[16],(TERM_LIST *) NULL);
  sv[17] = (struct term*) make_term(346,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[17],(TERM_LIST *) NULL);
  sv[22] = (struct term*) make_term(6,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[22],(TERM_LIST *) NULL);
  sv[21] = (struct term*) make_term(7,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[21],vlist[2]);
  sv[15] = (struct term*) make_term(340,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[15],vlist[1]);
  sv[18] = (struct term*) make_term(348,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[18],(TERM_LIST *) NULL);
  sv[13] = (struct term*) make_term(349,NULL,CONSTAN);
  vlist[2] = make_term_list((TERM*) sv[13],(TERM_LIST *) NULL);
  sv[22] = (struct term*) make_term(6,NULL,VAR);
  vlist[3] = make_term_list((TERM*) sv[22],(TERM_LIST *) NULL);
  sv[22] = (struct term*) make_term(6,NULL,VAR);
  vlist[3] = make_term_list((TERM*) sv[22],vlist[3]);
  sv[12] = (struct term*) make_term(341,vlist[3],FUNC);
  vlist[2] = make_term_list((TERM*) sv[12],vlist[2]);
  sv[14] = (struct term*) make_term(354,vlist[2],FUNC);
  vlist[1] = make_term_list((TERM*) sv[14],vlist[1]);
  sv[11] = (struct term*) make_term(310,NULL,CONSTAN);
  vlist[1] = make_term_list((TERM*) sv[11],vlist[1]);
  sv[22] = (struct term*) make_term(6,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[22],vlist[1]);
  sv[21] = (struct term*) make_term(7,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[21],vlist[1]);
  sv[19] = (struct term*) make_term(380,vlist[1],FUNC);
  vlist[1] = make_term_list((TERM*) sv[19],(TERM_LIST *) NULL);
  sv[20] = (struct term*) make_term(434,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[20],1,acvlist[0]);
  sv[35] = (struct term*) make_ac_term(435,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[35]);
  //printf("\n");
  ac_sort((TERM*)sv[35]);
  EkerTerm[3] = (TERM*)sv[35];
}

/* Query */
static struct term *main_query() {
  struct term *res;
  /* TERME DE DEPART */
  {
    /* hou()(=,((la.)(x,(la.)(y,(la.)(z,()(()((F),(z)),(y))))),(la.)(x,(la.)(y,(la.)(z,()((z),()(()((G),(y)),(x)))))),slt)) */
    struct term *tmp, *sv[8];
    TERM_ALLOC(sv[4],term1,code_324);
    sv[4]->sub[0] = con_290;
    TERM_ALLOC(sv[5],term1,code_325);
    sv[5]->sub[0] = con_307;
    TERM_ALLOC(sv[3],term2,code_322);
    sv[3]->sub[0] = sv[4];
    sv[3]->sub[1] = sv[5];
    TERM_ALLOC(sv[5],term1,code_325);
    sv[5]->sub[0] = con_306;
    TERM_ALLOC(sv[4],term2,code_322);
    sv[4]->sub[0] = sv[3];
    sv[4]->sub[1] = sv[5];
    TERM_ALLOC(sv[3],term2,code_323);
    sv[3]->sub[0] = con_307;
    sv[3]->sub[1] = sv[4];
    TERM_ALLOC(sv[2],term2,code_323);
    sv[2]->sub[0] = con_306;
    sv[2]->sub[1] = sv[3];
    TERM_ALLOC(sv[1],term2,code_323);
    sv[1]->sub[0] = con_305;
    sv[1]->sub[1] = sv[2];
    TERM_ALLOC(sv[5],term1,code_325);
    sv[5]->sub[0] = con_307;
    TERM_ALLOC(sv[6],term1,code_324);
    sv[6]->sub[0] = con_291;
    TERM_ALLOC(sv[7],term1,code_325);
    sv[7]->sub[0] = con_306;
    TERM_ALLOC(sv[4],term2,code_322);
    sv[4]->sub[0] = sv[6];
    sv[4]->sub[1] = sv[7];
    TERM_ALLOC(sv[7],term1,code_325);
    sv[7]->sub[0] = con_305;
    TERM_ALLOC(sv[6],term2,code_322);
    sv[6]->sub[0] = sv[4];
    sv[6]->sub[1] = sv[7];
    TERM_ALLOC(sv[4],term2,code_322);
    sv[4]->sub[0] = sv[5];
    sv[4]->sub[1] = sv[6];
    TERM_ALLOC(sv[5],term2,code_323);
    sv[5]->sub[0] = con_307;
    sv[5]->sub[1] = sv[4];
    TERM_ALLOC(sv[3],term2,code_323);
    sv[3]->sub[0] = con_306;
    sv[3]->sub[1] = sv[5];
    TERM_ALLOC(sv[2],term2,code_323);
    sv[2]->sub[0] = con_305;
    sv[2]->sub[1] = sv[3];
    sv[0] = fun_475(  );
    TERM_ALLOC(sv[3],term3,code_449);
    sv[3]->sub[0] = sv[1];
    sv[3]->sub[1] = sv[2];
    sv[3]->sub[2] = sv[0];
    sv[0] = fun_451( sv[3] );
    res=sv[0];
  }
  return res;
}

/* Procedure principale */
long *bp_main;
int main(int argc,char **argv) {
  long bp;
  struct term *res;
  int i,j;
  int queryMode=0;
  int REFMode=0;
  bp_main=&bp;
  GC_free_space_divisor=2;
  for(i=1 ; i<argc && *argv[i]=='-' ; i++) {
    if(!strcmp(argv[i],"-query")) {
      queryMode=1;
    }
    if(!strcmp(argv[i],"-REF")) {
      REFMode=1;
    }
    if(!strcmp(argv[i],"-debug")) {
      debugMode=1;
    }
    if(!strcmp(argv[i],"-trace")) {
      trace=2;
    }
  }
  if(queryMode) {
    yyparse();
  }
  init_alloc();
  for(i=0 ; i<fsymtabSize ; i++) {
    fsym_init(i,0,"nullString",0,0,NULL);
  }
  fsym_init(code_391,1,"scnd()",0,0, NULL);
  fsym_init(code_390,1,"size()",0,0, NULL);
  fsym_init(code_389,1,"exp()",0,0, NULL);
  fsym_init(code_388,1,"exp()",0,0, NULL);
  fsym_init(code_387,1,"red",0,0, NULL);
  fsym_init(code_386,1,"lnf",0,0, NULL);
  fsym_init(code_385,1,"fromstaten()",0,0, NULL);
  fsym_init(code_384,1,"fromstate()",0,0, NULL);
  fsym_init(code_383,1,"tostate()",0,0, NULL);
  fsym_init(code_382,3,"shift(,,)",0,0, NULL);
  fsym_init(code_381,2,"shift(,)",0,0, NULL);
  fsym_init(code_380,5,"[,,,,]",0,0, NULL);
  fsym_init(code_379,1,"context2()",0,0, NULL);
  fsym_init(code_378,1,"context()",0,0, NULL);
  fsym_init(code_377,1,"type()",0,0, NULL);
  fsym_init(code_376,2,"replace_c(,)",0,0, NULL);
  fsym_init(code_375,2,"replace_c(,)",0,0, NULL);
  fsym_init(code_374,3,"replace(,,)",0,0, NULL);
  fsym_init(code_373,1,"make_context()",0,0, NULL);
  fsym_init(code_372,2,"collect_fbvars(,)",0,0, NULL);
  fsym_init(code_371,2,"to(,)",0,0, NULL);
  fsym_init(code_370,1,"removect()",0,0, NULL);
  fsym_init(code_79,1,"-",0,0, NULL);
  fsym_init(code_369,1,"removect()",0,0, NULL);
  fsym_init(code_368,1,"remove()",0,0, NULL);
  fsym_init(code_367,1,"remove()",0,0, NULL);
  fsym_init(code_366,2,"fill(,)",0,0, NULL);
  fsym_init(code_365,2,"fill(,)",0,0, NULL);
  fsym_init(code_364,1,"fill()",0,0, NULL);
  fsym_init(code_363,1,"fill()",0,0, NULL);
  fsym_init(code_362,2,"QQ(,)",0,0, NULL);
  fsym_init(code_361,1,"Q()",0,0, NULL);
  fsym_init(code_360,2,"FF(,)",0,0, NULL);
  fsym_init(code_68,1,"",0,0, NULL);
  fsym_init(code_67,1,"",0,0, NULL);
  fsym_init(code_359,1,"F()",0,0, NULL);
  fsym_init(code_358,3,"rt(,,)",0,0, NULL);
  fsym_init(code_357,1,"dBtol()",0,0, NULL);
  fsym_init(code_356,2,"tr(,)",0,0, NULL);
  fsym_init(code_355,1,"ltodB()",0,0, NULL);
  fsym_init(code_354,2,":",0,0, NULL);
  fsym_init(code_353,1,"()",0,0, NULL);
  fsym_init(code_352,2,".",0,0, NULL);
  fsym_init(code_351,2,"o",0,0, NULL);
  fsym_init(code_350,0,"^",0,0, NULL);
  fsym_init(code_349,0,"id",0,0, NULL);
  fsym_init(code_348,2,":",0,0, NULL);
  fsym_init(code_347,0,"dummy",0,0, NULL);
  fsym_init(code_346,1,"",0,0, NULL);
  fsym_init(code_345,1,"#",0,0, NULL);
  fsym_init(code_344,2,"[]",0,0, NULL);
  fsym_init(code_343,2,"(la.)",0,0, NULL);
  fsym_init(code_342,2,"()",0,0, NULL);
  fsym_init(code_341,2,"|>",0,0, NULL);
  fsym_init(code_340,2,"|-",0,0, NULL);
  fsym_init(code_339,1,"()",0,0, NULL);
  fsym_init(code_338,2,".",0,0, NULL);
  fsym_init(code_337,2,"o",0,0, NULL);
  fsym_init(code_336,0,"^",0,0, NULL);
  fsym_init(code_335,0,"id",0,0, NULL);
  fsym_init(code_334,1,"",0,0, NULL);
  fsym_init(code_333,1,"#",0,0, NULL);
  fsym_init(code_332,2,"[]",0,0, NULL);
  fsym_init(code_331,2,"(la.)",0,0, NULL);
  fsym_init(code_330,2,"()",0,0, NULL);
  fsym_init(code_33,2,"greatereq_list[wtterm](,)",33,0, NULL);
  fsym_init(code_32,2,"greater_list[wtterm](,)",32,0, NULL);
  fsym_init(code_31,2,"lesseq_list[wtterm](,)",31,0, NULL);
  fsym_init(code_30,2,"less_list[wtterm](,)",30,0, NULL);
  fsym_init(code_329,1,"",0,0, NULL);
  fsym_init(code_328,1,"",0,0, NULL);
  fsym_init(code_327,2,"(la.)",0,0, NULL);
  fsym_init(code_326,2,"()",0,0, NULL);
  fsym_init(code_325,1,"",0,0, NULL);
  fsym_init(code_324,1,"",0,0, NULL);
  fsym_init(code_323,2,"(la.)",0,0, NULL);
  fsym_init(code_322,2,"()",0,0, NULL);
  fsym_init(code_321,1,"ispair()",0,0, NULL);
  fsym_init(code_320,1,"2-th()",0,0, NULL);
  fsym_init(code_29,2,"or(,)",29,0, NULL);
  fsym_init(code_28,2,"and(,)",28,0, NULL);
  fsym_init(code_27,2,"mod(,)",27,0, NULL);
  fsym_init(code_26,1,"itob_builtinInt()",26,0, NULL);
  fsym_init(code_25,1,"btoi_builtinInt()",25,0, NULL);
  fsym_init(code_24,1,"(not())",24,0, NULL);
  fsym_init(code_22,2,"(or)",22,0, NULL);
  fsym_init(code_21,2,"(and)",21,0, NULL);
  fsym_init(code_20,1,"umin_builtinInt()",20,0, NULL);
  fsym_init(code_319,1,"1-th()",0,0, NULL);
  fsym_init(code_318,2,"[,]",0,0, NULL);
  fsym_init(code_317,0,"listExtract",0,0, NULL);
  fsym_init(code_316,2,"ccat(,)",0,0, NULL);
  fsym_init(code_315,1,"size_of_wtterm_list()",0,0, NULL);
  fsym_init(code_314,2,"-thelem()",0,0, NULL);
  fsym_init(code_313,1,"elem()",0,0, NULL);
  fsym_init(code_312,2,"@",0,0, NULL);
  fsym_init(code_311,2,".",0,0, NULL);
  fsym_init(code_310,0,"nil",0,0, NULL);
  fsym_init(code_19,2,"neq_list[wtterm](,)",19,0, NULL);
  fsym_init(code_18,2,"eq_list[wtterm](,)",18,0, NULL);
  fsym_init(code_17,2,"occurs(,)",17,0, NULL);
  fsym_init(code_16,3,"replace(,,)",16,0, NULL);
  fsym_init(code_15,2,"neq_ident(,)",15,0, NULL);
  fsym_init(code_14,2,"eq_ident(,)",14,0, NULL);
  fsym_init(code_13,2,"greatereq_builtinInt(,)",13,0, NULL);
  fsym_init(code_12,2,"greater_builtinInt(,)",12,0, NULL);
  fsym_init(code_11,2,"lesseq_builtinInt(,)",11,0, NULL);
  fsym_init(code_10,2,"less_builtinInt(,)",10,0, NULL);
  fsym_init(code_309,0,"listdefs_Bvars",0,0, NULL);
  fsym_init(code_308,0,"list_Bvars",0,0, NULL);
  fsym_init(code_307,0,"z",0,0, NULL);
  fsym_init(code_306,0,"y",0,0, NULL);
  fsym_init(code_305,0,"x",0,0, NULL);
  fsym_init(code_304,3,"|-:",0,0, NULL);
  fsym_init(code_303,1,"V_Bvars()",0,0, NULL);
  fsym_init(code_302,1,"typeof()",0,0, NULL);
  fsym_init(code_301,0,"listExtract",0,0, NULL);
  fsym_init(code_300,2,"ccat(,)",0,0, NULL);
  fsym_init(code_299,1,"size_of_obj_Bvars_list()",0,0, NULL);
  fsym_init(code_298,2,"-thelem()",0,0, NULL);
  fsym_init(code_297,1,"elem()",0,0, NULL);
  fsym_init(code_296,2,"@",0,0, NULL);
  fsym_init(code_295,2,".",0,0, NULL);
  fsym_init(code_294,0,"nil",0,0, NULL);
  fsym_init(code_293,0,"listdefs_Mvars",0,0, NULL);
  fsym_init(code_292,0,"list_Mvars",0,0, NULL);
  fsym_init(code_291,0,"G",0,0, NULL);
  fsym_init(code_290,0,"F",0,0, NULL);
  fsym_init(code_289,3,"|-:",0,0, NULL);
  fsym_init(code_288,1,"V_Mvars()",0,0, NULL);
  fsym_init(code_287,1,"typeof()",0,0, NULL);
  fsym_init(code_286,0,"listExtract",0,0, NULL);
  fsym_init(code_285,2,"ccat(,)",0,0, NULL);
  fsym_init(code_284,1,"size_of_obj_Mvars_list()",0,0, NULL);
  fsym_init(code_283,2,"-thelem()",0,0, NULL);
  fsym_init(code_282,1,"elem()",0,0, NULL);
  fsym_init(code_281,2,"@",0,0, NULL);
  fsym_init(code_280,2,".",0,0, NULL);
  fsym_init(code_279,0,"nil",0,0, NULL);
  fsym_init(code_278,0,"listExtract",0,0, NULL);
  fsym_init(code_277,2,"ccat(,)",0,0, NULL);
  fsym_init(code_276,1,"size_of_vardefs_list()",0,0, NULL);
  fsym_init(code_275,2,"-thelem()",0,0, NULL);
  fsym_init(code_274,1,"elem()",0,0, NULL);
  fsym_init(code_273,2,"@",0,0, NULL);
  fsym_init(code_272,2,".",0,0, NULL);
  fsym_init(code_271,0,"nil",0,0, NULL);
  fsym_init(code_270,1,"",0,0, NULL);
  fsym_init(code_480,0,"hounify",0,0, NULL);
  fsym_init(code_269,1,"\\cvar<>",0,0, NULL);
  fsym_init(code_268,2,"\\concc<><>",0,0, NULL);
  fsym_init(code_267,0,"
ilc",0,0, NULL);
  fsym_init(code_266,0,"
ocontext",0,0, NULL);
  fsym_init(code_265,1,"tex()",0,0, NULL);
  fsym_init(code_264,1,"size()",0,0, NULL);
  fsym_init(code_263,1,"known()",0,0, NULL);
  fsym_init(code_262,2,"-thelem()",0,0, NULL);
  fsym_init(code_261,1,"cvar()",0,0, NULL);
  fsym_init(code_260,2,".",0,0, NULL);
  fsym_init(code_479,0,"noback",0,0, NULL);
  fsym_init(code_478,0,"bfs",0,0, NULL);
  fsym_init(code_477,0,"lt",0,0, NULL);
  fsym_init(code_476,0,"dbt",0,0, NULL);
  fsym_init(code_475,0,"slt",0,0, NULL);
  fsym_init(code_474,0,"wtt",0,0, NULL);
  fsym_init(code_473,2,"modd(,)",0,0, NULL);
  fsym_init(code_472,1,"odd()",0,0, NULL);
  fsym_init(code_471,1,"limit()",0,0, NULL);
  fsym_init(code_470,2,"bitin",0,0, NULL);
  fsym_init(code_259,0,"nil",0,0, NULL);
  fsym_init(code_258,0,"nocontext",0,0, NULL);
  fsym_init(code_257,0,"Bvars",0,0, NULL);
  fsym_init(code_256,0,"Mvars",0,0, NULL);
  fsym_init(code_255,1,"type()",0,0, NULL);
  fsym_init(code_254,1,"variabla()",0,0, NULL);
  fsym_init(code_253,2,":",0,0, NULL);
  fsym_init(code_252,0,"listExtract",0,0, NULL);
  fsym_init(code_251,2,"ccat(,)",0,0, NULL);
  fsym_init(code_250,1,"size_of_vardef_list()",0,0, NULL);
  fsym_init(code_469,3,"replace_norm(,,)",0,0, NULL);
  fsym_init(code_468,3,"replace123(,,)",0,0, NULL);
  fsym_init(code_467,3,"replace_all(,,)",0,0, NULL);
  fsym_init(code_466,3,"replace_norm(,,)",0,0, NULL);
  fsym_init(code_465,3,"replace_norm(,,)",0,0, NULL);
  fsym_init(code_464,3,"back(,,)",0,0, NULL);
  fsym_init(code_463,3,"back(,,)",0,0, NULL);
  fsym_init(code_462,3,"back(,,)",0,0, NULL);
  fsym_init(code_461,2,"sufix(,)",0,0, NULL);
  fsym_init(code_460,2,"distrib(,)",0,0, NULL);
  fsym_init(code_249,2,"-thelem()",0,0, NULL);
  fsym_init(code_248,1,"elem()",0,0, NULL);
  fsym_init(code_247,2,"@",0,0, NULL);
  fsym_init(code_246,2,"",0,0, NULL);
  fsym_init(code_245,0,";",0,0, NULL);
  fsym_init(code_244,1,"	var<>",0,0, NULL);
  fsym_init(code_243,1,"()",0,0, NULL);
  fsym_init(code_242,2,"\\fromto<><>",0,0, NULL);
  fsym_init(code_241,1,"\\basictype<>",0,0, NULL);
  fsym_init(code_240,0,"
otype",0,0, NULL);
  fsym_init(code_459,1,"is_var()",0,0, NULL);
  fsym_init(code_458,2,"solved_var(,)",0,0, NULL);
  fsym_init(code_457,3,"collect_mvrs(,,)",0,0, NULL);
  fsym_init(code_456,3,"add_args(,,)",0,0, NULL);
  fsym_init(code_455,3,"exp_vars(,,)",0,0, NULL);
  fsym_init(code_454,3,"add_unsolved",0,0, NULL);
  fsym_init(code_453,3,"complete(,,)",0,0, NULL);
  fsym_init(code_452,4,"add_disj(,,,)",0,0, NULL);
  fsym_init(code_451,1,"hou()",0,0, NULL);
  fsym_init(code_450,2,"=",0,0, NULL);
  fsym_init(code_239,1,"tex()",0,0, NULL);
  fsym_init(code_238,1,"rem_par()",0,0, NULL);
  fsym_init(code_237,1,"known()",0,0, NULL);
  fsym_init(code_236,1,"codomain_type()",0,0, NULL);
  fsym_init(code_9,2,"neq_builtinInt(,)",9,0, NULL);
  fsym_init(code_235,1,"domain_type()",0,0, NULL);
  fsym_init(code_8,2,"eq_builtinInt(,)",8,0, NULL);
  fsym_init(code_234,1,"atomic_type()",0,0, NULL);
  fsym_init(code_233,1,"functional_type()",0,0, NULL);
  fsym_init(code_6,2,"div(,)",6,0, NULL);
  fsym_init(code_232,1,"tvar()",0,0, NULL);
  fsym_init(code_5,2,"time(,)",5,0, NULL);
  fsym_init(code_231,2,"(->)",0,0, NULL);
  fsym_init(code_4,2,"minus(,)",4,0, NULL);
  fsym_init(code_230,1,"",0,0, NULL);
  fsym_init(code_3,2,"plus(,)",3,0, NULL);
  fsym_init(code_1,0,"true",0,0, NULL);
  fsym_init(code_0,0,"false",0,0, NULL);
  fsym_init(code_449,3,"=,",0,0, NULL);
  fsym_init(code_448,2,"[.fvars<-]",0,0, NULL);
  fsym_init(code_447,1,".fvars",0,0, NULL);
  fsym_init(code_446,2,"[.mvrs<-]",0,0, NULL);
  fsym_init(code_445,1,".mvrs",0,0, NULL);
  fsym_init(code_444,2,"[.solved<-]",0,0, NULL);
  fsym_init(code_443,1,".solved",0,0, NULL);
  fsym_init(code_442,2,"[.disj<-]",0,0, NULL);
  fsym_init(code_441,1,".disj",0,0, NULL);
  fsym_init(code_440,6,"(,,,,,)",0,0, NULL);
  fsym_init(code_229,0,"notype",0,0, NULL);
  fsym_init(code_228,0,"A",0,0, NULL);
  fsym_init(code_227,0,"Btypes",0,0, NULL);
  fsym_init(code_226,0,"listExtract",0,0, NULL);
  fsym_init(code_225,2,"ccat(,)",0,0, NULL);
  fsym_init(code_224,1,"size_of_identifier_list()",0,0, NULL);
  fsym_init(code_223,2,"-thelem()",0,0, NULL);
  fsym_init(code_222,1,"elem()",0,0, NULL);
  fsym_init(code_221,2,"@",0,0, NULL);
  fsym_init(code_220,2,".",0,0, NULL);
  fsym_init(code_439,-1,"-AND-",0,0, NULL);
  fsym_init(code_438,1,"",0,0, NULL);
  fsym_init(code_437,1,"",0,0, NULL);
  fsym_init(code_436,0,"ueq_true",0,0, NULL);
  fsym_init(code_435,-1,"=",0,0, NULL);
  fsym_init(code_434,1,"",0,0, NULL);
  fsym_init(code_433,1,"",0,0, NULL);
  fsym_init(code_432,1,"",0,0, NULL);
  fsym_init(code_431,1,"",0,0, NULL);
  fsym_init(code_430,1,"",0,0, NULL);
  fsym_init(code_219,0,"nil",0,0, NULL);
  fsym_init(code_218,1,"valueOf()",0,0, NULL);
  fsym_init(code_217,1,"itob_int()",0,0, NULL);
  fsym_init(code_216,1,"btoi_int()",0,0, NULL);
  fsym_init(code_215,2,"less_int(,)",0,0, NULL);
  fsym_init(code_214,2,"lesseq_int(,)",0,0, NULL);
  fsym_init(code_213,2,"greatereq_int(,)",0,0, NULL);
  fsym_init(code_212,2,"greater_int(,)",0,0, NULL);
  fsym_init(code_211,2,"neq_int(,)",0,0, NULL);
  fsym_init(code_210,2,"eq_int(,)",0,0, NULL);
  fsym_init(code_429,3,"ifte(,,)",0,0, NULL);
  fsym_init(code_428,3,"ifte(,,)",0,0, NULL);
  fsym_init(code_427,3,"ifte(,,)",0,0, NULL);
  fsym_init(code_426,0,"listExtract",0,0, NULL);
  fsym_init(code_425,2,"ccat(,)",0,0, NULL);
  fsym_init(code_424,1,"size_of_uconj_list()",0,0, NULL);
  fsym_init(code_423,2,"-thelem()",0,0, NULL);
  fsym_init(code_422,1,"elem()",0,0, NULL);
  fsym_init(code_421,2,"@",0,0, NULL);
  fsym_init(code_420,2,".",0,0, NULL);
  fsym_init(code_209,1,"umin()",0,0, NULL);
  fsym_init(code_208,2,"or(,)",0,0, NULL);
  fsym_init(code_207,2,"div(,)",0,0, NULL);
  fsym_init(code_206,2,"and(,)",0,0, NULL);
  fsym_init(code_205,2,"mod(,)",0,0, NULL);
  fsym_init(code_204,2,"time(,)",0,0, NULL);
  fsym_init(code_203,2,"minus(,)",0,0, NULL);
  fsym_init(code_202,2,"plus(,)",0,0, NULL);
  fsym_init(code_201,1,"[]",0,0, NULL);
  fsym_init(code_200,1,"",0,0, NULL);
  fsym_init(code_419,0,"nil",0,0, NULL);
  fsym_init(code_418,0,"listExtract",0,0, NULL);
  fsym_init(code_417,2,"ccat(,)",0,0, NULL);
  fsym_init(code_416,1,"size_of_objbvars_list()",0,0, NULL);
  fsym_init(code_415,2,"-thelem()",0,0, NULL);
  fsym_init(code_414,1,"elem()",0,0, NULL);
  fsym_init(code_413,2,"@",0,0, NULL);
  fsym_init(code_412,2,".",0,0, NULL);
  fsym_init(code_411,0,"nil",0,0, NULL);
  fsym_init(code_410,1,"listvardefstolistgoal()",0,0, NULL);
  fsym_init(code_409,2,"typeof(,)",0,0, NULL);
  fsym_init(code_408,2,"typech(,)",0,0, NULL);
  fsym_init(code_407,2,"typech(,)",0,0, NULL);
  fsym_init(code_406,1,"typeof()",0,0, NULL);
  fsym_init(code_405,2,"typech(,)",0,0, NULL);
  fsym_init(code_404,2,"typech(,)",0,0, NULL);
  fsym_init(code_403,1,"typech()",0,0, NULL);
  fsym_init(code_402,0,"normal2",0,0, NULL);
  fsym_init(code_401,0,"normal2",0,0, NULL);
  fsym_init(code_400,0,"redform",0,0, NULL);
  fsym_init(code_399,3,"poss(,,)",0,0, NULL);
  fsym_init(code_398,2,"pos(,)",0,0, NULL);
  fsym_init(code_397,1,"meta()",0,0, NULL);
  fsym_init(code_396,1,"normalize()",0,0, NULL);
  fsym_init(code_395,1,"normalize()",0,0, NULL);
  fsym_init(code_394,1,"norm()",0,0, NULL);
  fsym_init(code_393,1,"norm()",0,0, NULL);
  fsym_init(code_392,1,"if_comp()",0,0, NULL);
  
/* Initialisation des constantes */
  TERM_CONST_ALLOC(con_350, code_350);
  TERM_CONST_ALLOC(con_349, code_349);
  TERM_CONST_ALLOC(con_347, code_347);
  TERM_CONST_ALLOC(con_336, code_336);
  TERM_CONST_ALLOC(con_335, code_335);
  TERM_CONST_ALLOC(con_317, code_317);
  TERM_CONST_ALLOC(con_310, code_310);
  TERM_CONST_ALLOC(con_307, code_307);
  TERM_CONST_ALLOC(con_306, code_306);
  TERM_CONST_ALLOC(con_305, code_305);
  TERM_CONST_ALLOC(con_301, code_301);
  TERM_CONST_ALLOC(con_294, code_294);
  TERM_CONST_ALLOC(con_291, code_291);
  TERM_CONST_ALLOC(con_290, code_290);
  TERM_CONST_ALLOC(con_286, code_286);
  TERM_CONST_ALLOC(con_279, code_279);
  TERM_CONST_ALLOC(con_278, code_278);
  TERM_CONST_ALLOC(con_271, code_271);
  TERM_CONST_ALLOC(con_480, code_480);
  TERM_CONST_ALLOC(con_267, code_267);
  TERM_CONST_ALLOC(con_266, code_266);
  TERM_CONST_ALLOC(con_259, code_259);
  TERM_CONST_ALLOC(con_258, code_258);
  TERM_CONST_ALLOC(con_252, code_252);
  TERM_CONST_ALLOC(con_245, code_245);
  TERM_CONST_ALLOC(con_240, code_240);
  TERM_CONST_ALLOC(con_1, code_1);
  TERM_CONST_ALLOC(con_0, code_0);
  TERM_CONST_ALLOC(con_229, code_229);
  TERM_CONST_ALLOC(con_228, code_228);
  TERM_CONST_ALLOC(con_226, code_226);
  TERM_CONST_ALLOC(con_436, code_436);
  TERM_CONST_ALLOC(con_219, code_219);
  TERM_CONST_ALLOC(con_426, code_426);
  TERM_CONST_ALLOC(con_419, code_419);
  TERM_CONST_ALLOC(con_418, code_418);
  TERM_CONST_ALLOC(con_411, code_411);
  TERM_CONST_ALLOC(con_402, code_402);
  TERM_CONST_ALLOC(con_401, code_401);
  TERM_CONST_ALLOC(con_400, code_400);
  EkerTermInit();

  backTrackInit();
  /* Initialisation des pattern_list */
init_pattern_list_458_439();
init_pattern_list_463_439();
init_pattern_list_439();
init_pattern_list_467_439();
init_pattern_list_465_439();
                    init_pattern_list_252_repeat1_cons1_repeat1_ONE1_one_439();
                  init_pattern_list_252_repeat1_cons1_repeat1_ONE3_cons1_one_439();
                init_pattern_list_252_repeat1_cons4_repeat1_one_439();
          init_pattern_list_158_repeat1_one_439();
  for(i=0 ; i<2 ; i++) {
    for(j=0 ; j<480; j++) {
      tab_rewrite_step[i][j] = 0;
    }
  }

  if (!setChoicePoint()) {
    if(queryMode) {
      res = normalise(query);
    } else {
      res=main_query();
    }
    if(!REFMode) {
      printf("\nresult = ");
    }
    if((long)res==0 || (long)res==1) {
      printf("%d\n",res);
    } else {
      if(REFMode) {
        term_printREFln(stdout,res);
      } else {
        term_printnl(stdout,res);
      }
    }
    if(!REFMode) {
      backStatistics();
    }
    fail();
  }
end:
destruction:
  FREE(fsymtab[code_391].name);
  FREE(fsymtab[code_390].name);
  FREE(fsymtab[code_389].name);
  FREE(fsymtab[code_388].name);
  FREE(fsymtab[code_387].name);
  FREE(fsymtab[code_386].name);
  FREE(fsymtab[code_385].name);
  FREE(fsymtab[code_384].name);
  FREE(fsymtab[code_383].name);
  FREE(fsymtab[code_382].name);
  FREE(fsymtab[code_381].name);
  FREE(fsymtab[code_380].name);
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
  FREE(fsymtab[code_79].name);
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
  FREE(fsymtab[code_68].name);
  FREE(fsymtab[code_67].name);
  FREE(fsymtab[code_359].name);
  FREE(fsymtab[code_358].name);
  FREE(fsymtab[code_357].name);
  FREE(fsymtab[code_356].name);
  FREE(fsymtab[code_355].name);
  FREE(fsymtab[code_354].name);
  FREE(fsymtab[code_353].name);
  FREE(fsymtab[code_352].name);
  FREE(fsymtab[code_351].name);
  FREE(fsymtab[code_350].name);
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
  FREE(fsymtab[code_33].name);
  FREE(fsymtab[code_32].name);
  FREE(fsymtab[code_31].name);
  FREE(fsymtab[code_30].name);
  FREE(fsymtab[code_329].name);
  FREE(fsymtab[code_328].name);
  FREE(fsymtab[code_327].name);
  FREE(fsymtab[code_326].name);
  FREE(fsymtab[code_325].name);
  FREE(fsymtab[code_324].name);
  FREE(fsymtab[code_323].name);
  FREE(fsymtab[code_322].name);
  FREE(fsymtab[code_321].name);
  FREE(fsymtab[code_320].name);
  FREE(fsymtab[code_29].name);
  FREE(fsymtab[code_28].name);
  FREE(fsymtab[code_27].name);
  FREE(fsymtab[code_26].name);
  FREE(fsymtab[code_25].name);
  FREE(fsymtab[code_24].name);
  FREE(fsymtab[code_22].name);
  FREE(fsymtab[code_21].name);
  FREE(fsymtab[code_20].name);
  FREE(fsymtab[code_319].name);
  FREE(fsymtab[code_318].name);
  FREE(fsymtab[code_317].name);
  FREE(fsymtab[code_316].name);
  FREE(fsymtab[code_315].name);
  FREE(fsymtab[code_314].name);
  FREE(fsymtab[code_313].name);
  FREE(fsymtab[code_312].name);
  FREE(fsymtab[code_311].name);
  FREE(fsymtab[code_310].name);
  FREE(fsymtab[code_19].name);
  FREE(fsymtab[code_18].name);
  FREE(fsymtab[code_17].name);
  FREE(fsymtab[code_16].name);
  FREE(fsymtab[code_15].name);
  FREE(fsymtab[code_14].name);
  FREE(fsymtab[code_13].name);
  FREE(fsymtab[code_12].name);
  FREE(fsymtab[code_11].name);
  FREE(fsymtab[code_10].name);
  FREE(fsymtab[code_309].name);
  FREE(fsymtab[code_308].name);
  FREE(fsymtab[code_307].name);
  FREE(fsymtab[code_306].name);
  FREE(fsymtab[code_305].name);
  FREE(fsymtab[code_304].name);
  FREE(fsymtab[code_303].name);
  FREE(fsymtab[code_302].name);
  FREE(fsymtab[code_301].name);
  FREE(fsymtab[code_300].name);
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
  FREE(fsymtab[code_480].name);
  FREE(fsymtab[code_269].name);
  FREE(fsymtab[code_268].name);
  FREE(fsymtab[code_267].name);
  FREE(fsymtab[code_266].name);
  FREE(fsymtab[code_265].name);
  FREE(fsymtab[code_264].name);
  FREE(fsymtab[code_263].name);
  FREE(fsymtab[code_262].name);
  FREE(fsymtab[code_261].name);
  FREE(fsymtab[code_260].name);
  FREE(fsymtab[code_479].name);
  FREE(fsymtab[code_478].name);
  FREE(fsymtab[code_477].name);
  FREE(fsymtab[code_476].name);
  FREE(fsymtab[code_475].name);
  FREE(fsymtab[code_474].name);
  FREE(fsymtab[code_473].name);
  FREE(fsymtab[code_472].name);
  FREE(fsymtab[code_471].name);
  FREE(fsymtab[code_470].name);
  FREE(fsymtab[code_259].name);
  FREE(fsymtab[code_258].name);
  FREE(fsymtab[code_257].name);
  FREE(fsymtab[code_256].name);
  FREE(fsymtab[code_255].name);
  FREE(fsymtab[code_254].name);
  FREE(fsymtab[code_253].name);
  FREE(fsymtab[code_252].name);
  FREE(fsymtab[code_251].name);
  FREE(fsymtab[code_250].name);
  FREE(fsymtab[code_469].name);
  FREE(fsymtab[code_468].name);
  FREE(fsymtab[code_467].name);
  FREE(fsymtab[code_466].name);
  FREE(fsymtab[code_465].name);
  FREE(fsymtab[code_464].name);
  FREE(fsymtab[code_463].name);
  FREE(fsymtab[code_462].name);
  FREE(fsymtab[code_461].name);
  FREE(fsymtab[code_460].name);
  FREE(fsymtab[code_249].name);
  FREE(fsymtab[code_248].name);
  FREE(fsymtab[code_247].name);
  FREE(fsymtab[code_246].name);
  FREE(fsymtab[code_245].name);
  FREE(fsymtab[code_244].name);
  FREE(fsymtab[code_243].name);
  FREE(fsymtab[code_242].name);
  FREE(fsymtab[code_241].name);
  FREE(fsymtab[code_240].name);
  FREE(fsymtab[code_459].name);
  FREE(fsymtab[code_458].name);
  FREE(fsymtab[code_457].name);
  FREE(fsymtab[code_456].name);
  FREE(fsymtab[code_455].name);
  FREE(fsymtab[code_454].name);
  FREE(fsymtab[code_453].name);
  FREE(fsymtab[code_452].name);
  FREE(fsymtab[code_451].name);
  FREE(fsymtab[code_450].name);
  FREE(fsymtab[code_239].name);
  FREE(fsymtab[code_238].name);
  FREE(fsymtab[code_237].name);
  FREE(fsymtab[code_236].name);
  FREE(fsymtab[code_9].name);
  FREE(fsymtab[code_235].name);
  FREE(fsymtab[code_8].name);
  FREE(fsymtab[code_234].name);
  FREE(fsymtab[code_233].name);
  FREE(fsymtab[code_6].name);
  FREE(fsymtab[code_232].name);
  FREE(fsymtab[code_5].name);
  FREE(fsymtab[code_231].name);
  FREE(fsymtab[code_4].name);
  FREE(fsymtab[code_230].name);
  FREE(fsymtab[code_3].name);
  FREE(fsymtab[code_1].name);
  FREE(fsymtab[code_0].name);
  FREE(fsymtab[code_449].name);
  FREE(fsymtab[code_448].name);
  FREE(fsymtab[code_447].name);
  FREE(fsymtab[code_446].name);
  FREE(fsymtab[code_445].name);
  FREE(fsymtab[code_444].name);
  FREE(fsymtab[code_443].name);
  FREE(fsymtab[code_442].name);
  FREE(fsymtab[code_441].name);
  FREE(fsymtab[code_440].name);
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
  FREE(fsymtab[code_439].name);
  FREE(fsymtab[code_438].name);
  FREE(fsymtab[code_437].name);
  FREE(fsymtab[code_436].name);
  FREE(fsymtab[code_435].name);
  FREE(fsymtab[code_434].name);
  FREE(fsymtab[code_433].name);
  FREE(fsymtab[code_432].name);
  FREE(fsymtab[code_431].name);
  FREE(fsymtab[code_430].name);
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
  FREE(fsymtab[code_429].name);
  FREE(fsymtab[code_428].name);
  FREE(fsymtab[code_427].name);
  FREE(fsymtab[code_426].name);
  FREE(fsymtab[code_425].name);
  FREE(fsymtab[code_424].name);
  FREE(fsymtab[code_423].name);
  FREE(fsymtab[code_422].name);
  FREE(fsymtab[code_421].name);
  FREE(fsymtab[code_420].name);
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
  FREE(fsymtab[code_419].name);
  FREE(fsymtab[code_418].name);
  FREE(fsymtab[code_417].name);
  FREE(fsymtab[code_416].name);
  FREE(fsymtab[code_415].name);
  FREE(fsymtab[code_414].name);
  FREE(fsymtab[code_413].name);
  FREE(fsymtab[code_412].name);
  FREE(fsymtab[code_411].name);
  FREE(fsymtab[code_410].name);
  FREE(fsymtab[code_409].name);
  FREE(fsymtab[code_408].name);
  FREE(fsymtab[code_407].name);
  FREE(fsymtab[code_406].name);
  FREE(fsymtab[code_405].name);
  FREE(fsymtab[code_404].name);
  FREE(fsymtab[code_403].name);
  FREE(fsymtab[code_402].name);
  FREE(fsymtab[code_401].name);
  FREE(fsymtab[code_400].name);
  FREE(fsymtab[code_399].name);
  FREE(fsymtab[code_398].name);
  FREE(fsymtab[code_397].name);
  FREE(fsymtab[code_396].name);
  FREE(fsymtab[code_395].name);
  FREE(fsymtab[code_394].name);
  FREE(fsymtab[code_393].name);
  FREE(fsymtab[code_392].name);
  TERM_FREE(con_350);
  TERM_FREE(con_349);
  TERM_FREE(con_347);
  TERM_FREE(con_336);
  TERM_FREE(con_335);
  TERM_FREE(con_317);
  TERM_FREE(con_310);
  TERM_FREE(con_307);
  TERM_FREE(con_306);
  TERM_FREE(con_305);
  TERM_FREE(con_301);
  TERM_FREE(con_294);
  TERM_FREE(con_291);
  TERM_FREE(con_290);
  TERM_FREE(con_286);
  TERM_FREE(con_279);
  TERM_FREE(con_278);
  TERM_FREE(con_271);
  TERM_FREE(con_480);
  TERM_FREE(con_267);
  TERM_FREE(con_266);
  TERM_FREE(con_259);
  TERM_FREE(con_258);
  TERM_FREE(con_252);
  TERM_FREE(con_245);
  TERM_FREE(con_240);
  TERM_FREE(con_1);
  TERM_FREE(con_0);
  TERM_FREE(con_229);
  TERM_FREE(con_228);
  TERM_FREE(con_226);
  TERM_FREE(con_436);
  TERM_FREE(con_219);
  TERM_FREE(con_426);
  TERM_FREE(con_419);
  TERM_FREE(con_418);
  TERM_FREE(con_411);
  TERM_FREE(con_402);
  TERM_FREE(con_401);
  TERM_FREE(con_400);
  /* Destruction des pattern_list */
delete_pattern_list_458_439();
delete_pattern_list_463_439();
delete_pattern_list_439();
delete_pattern_list_467_439();
delete_pattern_list_465_439();
                    delete_pattern_list_252_repeat1_cons1_repeat1_ONE1_one_439();
                  delete_pattern_list_252_repeat1_cons1_repeat1_ONE3_cons1_one_439();
                delete_pattern_list_252_repeat1_cons4_repeat1_one_439();
          delete_pattern_list_158_repeat1_one_439();
#ifdef DEBUG
  backStatistics();
#endif
  if(!REFMode) {
    printf("\nrewrite_step = %u\n",rewrite_step);
    printf("                      	fails	success\n");
    for(j=0 ; j<480; j++) {
      if(tab_rewrite_step[0][j] > 0 || tab_rewrite_step[1][j] > 0)
        printf("tab_rewrite_step[%d] :	%u	%u\n",j,tab_rewrite_step[0][j],tab_rewrite_step[1][j]);
    }
  }
  exit(0);
}
#include "ac_tools.c"
