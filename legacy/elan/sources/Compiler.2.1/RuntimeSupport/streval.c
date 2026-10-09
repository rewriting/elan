#include "../../elan/codes.h"
#include "term.h"
#include "back.h"
#include "streval.h"

TERMSTR(term0,1);
TERMSTR(term3,3);

//#define DBG
 
#define BODY_FUN0(N) struct term *N(int code) \
                     { \
                       struct term *sv;\
                       TERM_ALLOC(sv,term0,code); \
                       setSymb(sv,code);\
		       return sv; \
		     }

#define BODY_FUN1(N) struct term *N(int code, struct term *s) \
                     { \
                       struct term *sv;\
                       TERM_ALLOC(sv,term1,code); \
                       setSymb(sv,code); sv->sub[0] = s; \
		       return sv; \
		     }

#define BODY_FUN2(N) struct term *N(int code, struct term *s, struct term *t) \
                     { \
                       struct term *sv;\
                       TERM_ALLOC(sv,term2,code); \
                       setSymb(sv,code); sv->sub[0] = s; sv->sub[1] = t; \
		       return sv; \
		     }

#define BODY_FUN3(N) struct term *N(int code, struct term *b, struct term *s, struct term *t) \
                     { \
                       struct term *sv;\
                       TERM_ALLOC(sv,term3,code); \
                       setSymb(sv,code); sv->sub[0] = b; sv->sub[1] = s; sv->sub[2] = t; \
		       return sv; \
		     }


// [s]t
  BODY_FUN2(fun_180);

// dc(@)
  BODY_FUN1(fun_181);

// dk(@)
  BODY_FUN1(fun_182);

// fail
  BODY_FUN0(fun_183);

// id
  BODY_FUN0(fun_186);

// if @ then @ orelse @ fi
  BODY_FUN3(fun_184);

// @ @	
  BODY_FUN2(fun_185);

// if @ then @ orelse @ fi
  BODY_FUN3(fun_187);

//  @ ';' @
  BODY_FUN2(fun_188);

// Epsilon 
  BODY_FUN0(fun_189);

// one(@)
  BODY_FUN1(fun_190);


struct term *str_eval(struct term *T)
{
struct term *s;
struct term *t;
 struct term* (*semact)(struct term *);
int ts;
  if (-term_semantic(T) == DS_APPLY) {
    s = T->sub[0];
    t = T->sub[1];
    ts = term_defstrat(s);
    if (IS_LAB_FLAG(ts)) {
      int apply_code = LAB_F(ts);   
      int dstr_code = LAB_LAB(ts);
#ifdef DBG
      printf("apply_symbol=%d, rule=%d\n", apply_code, dstr_code);
      term_printnl(stdout, T);
#endif
      semact = term_semact(s);
      return (*semact)(T);} 
    else if (IS_DSTR_FLAG(ts)) {
      int apply_code = DSTR_F(ts);
      int dstr_code = DSTR_LAB(ts);
#ifdef DBG
      printf("apply_symbol=%d, rule=%d\n", apply_code, dstr_code);
      term_printnl(stdout, T);
#endif
      semact = term_semact(s);
      return (*semact)(T); }
    else
      return str_eval2(s,t);
  } else {
    fprintf(stderr,"strategy term does not contain applictation symbol\n"); 
    exit(1); }
}

struct term *str_eval2(struct term *s, struct term *t)
{
int defstrat = term_defstrat(s);
int is_one = 0;
#ifdef DBG
printf("str_eval2 entry, fymb = %d, sem=%d, defstr=%d,(%d,%d,%d)\n",
                    getSymb(s),
                    -term_semantic(s),defstrat,
                    IS_FSYM_FLAG(defstrat),
                    IS_LAB_FLAG(defstrat),
                    IS_DSTR_FLAG(defstrat));
		   
//if (IS_FSYM_FLAG(defstrat) + IS_LAB_FLAG(defstrat) + IS_DSTR_FLAG(defstrat) == 0) 
#endif
{
#ifdef DBG
printf("strategy =");
  term_printnl(stdout,s);
printf("term =");
  term_printnl(stdout,t);
#endif
}

    switch (-term_semantic(s)) {
      case DS_ID:
	return t;
	break;
      case DS_FAIL:
	fail();
      case DS_CONC:
	{
	  struct term *s1, *s2;
	  struct term *res;
	  s1 = s->sub[0];
	  s2 = s->sub[1];
	  res = str_eval2(s1,t);
	  return str_eval2(s2,res);
	 break; }
      case DS_DK:
	{
	  struct term *strlist, *res;
	  int is_last = 0;

	  for(strlist = s->sub[0]; 
	      -term_semantic(strlist) != DS_EPSILON;
	      strlist = strlist->sub[1]) {
	    if (-term_semantic(strlist) != DS_COMMA &&
	        -term_semantic(strlist) != DS_COMMA_CONCUR) {
	      fprintf(stderr,"DS_COMMA, DS_COMMA_CONCUR symbol expected in DC/DK strategy list\n");
	      exit(1); }
	    is_last = (-term_semantic(strlist->sub[1]) == DS_EPSILON);
	    if (!is_last) {
	      if(!setChoicePoint()) {
		res = str_eval2(strlist->sub[0],t);
		goto lab_dk; }
	    } else { // 0206
	      res = str_eval2(strlist->sub[0],t); }
	  }
	lab_dk:
	  return res;
	  break; }
      case DS_ONE:
        is_one = 1;  // THIS SHOULD FOLLOW 
      case DS_DC:
	{
	  struct term *strlist, *res;
	  int *wasr = (int*)(allocStable(sizeof(int)));
	  int is_last = 0;
	  *wasr = 0;

	  for(strlist = s->sub[0]; 
	      -term_semantic(strlist) != DS_EPSILON;
	      strlist = strlist->sub[1]) {
	    if (-term_semantic(strlist) != DS_COMMA &&
	        -term_semantic(strlist) != DS_COMMA_CONCUR) {
	      fprintf(stderr,"DS_COMMA, DS_COMMA_CONCUR symbol expected in DC/DK strategy list\n");
	      exit(1); }
	    is_last = (-term_semantic(strlist->sub[1]) == DS_EPSILON);
	    if (!is_last) {
	      if(!setChoicePoint()) {
		if (is_one) { CUTOPEN(); }
		res = str_eval2(strlist->sub[0],t);
		if (is_one) { CUTCLOSE(); }
		if (*wasr == 0) *wasr = 1;
		goto lab_dc;
	      }
	      if (*wasr != 0) fail();
	    } else // is_last
	      res = str_eval2(strlist->sub[0],t);
	  }
	lab_dc:
	  return res;
	  break; }
      case DS_IFTE:
	{
      	  struct term *cond, *s1, *s2;
	  cond = s->sub[0]; s1 = s->sub[1]; s2 = s->sub[2];
	  if (cond->symb) { // > 0 is true
	    return str_eval2(s1,t); }
	  else {
	    return str_eval2(s2,t); }
	}
      case DS_IFTOE:
	{
	  struct term *res, *cond, *s1, *s2;
	  int *wasr = (int*)(allocStable(sizeof(int)));
	  *wasr = 0;
	  cond = s->sub[0]; s1 = s->sub[1]; s2 = s->sub[2];
	  if(!setChoicePoint()) {
	    res = str_eval2(cond,t);
	    if (*wasr == 0) *wasr = 1;
	    return str_eval2(s1,res);
	  }
	  if (*wasr != 0) fail();
	  return str_eval2(s2,t);
	}
      default:
	if (IS_LAB_FLAG(defstrat)) {
	  int apply_code = LAB_F(defstrat);
	  int dstr_code = LAB_LAB(defstrat);
	  struct term *sv;
	  struct term* (*semact)(struct term *);
 	  TERM_ALLOC(sv,term2,apply_code);
	  sv->sub[0] = s;
	  sv->sub[1] = t;
#ifdef DBG
	  printf("apply_symbol=%d, rule=%d\n", apply_code, dstr_code);
	  term_printnl(stdout, sv);
#endif
	  semact = term_semact(s);
	  return (*semact)(sv);} 
	else if (IS_FSYM_FLAG(defstrat)) {
	  int arity = term_arity(s);
	  int f1 = FSYM_F1(defstrat);
	  int f2 = FSYM_F2(defstrat);
	  struct term *res;
#ifdef DBG
	  printf("DEFSTRAT %d = %d,%d\n", defstrat, FSYM_F1(defstrat), FSYM_F2(defstrat));
#endif
	  if (getSymb(t) == f1) {
	    int i;
	    if (arity == 0 && f1 == f2) return t;
	    else {
	      TERM_ARITY_ALLOC(res,arity,f2);
	      for(i=0; i<arity; i++) 
		res->sub[i] = str_eval2(s->sub[i],t->sub[i]);
	      return res;
	    }
	  }
	  else
	    fail(); }
	else if (IS_DSTR_FLAG(defstrat)) {
	  int apply_code = DSTR_F(defstrat);  
	  int dstr_code = DSTR_LAB(defstrat);
	  struct term *sv;
	  struct term* (*semact)(struct term *);
	  TERM_ALLOC(sv,term2,apply_code);
	  sv->sub[0] = s;
	  sv->sub[1] = t;
#ifdef DBG
	  printf("apply_symbol=%d, rule=%d\n", apply_code, dstr_code);
	  term_printnl(stdout, sv);
#endif
	  semact = term_semact(s);
	  return (*semact)(sv); 
	} else {
	  fprintf(stderr,"unknown strategy constructor %d in str_eval\n",-term_semantic(s)); 
	  exit(1); }
    }
}





