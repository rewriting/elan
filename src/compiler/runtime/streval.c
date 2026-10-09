/*
  
    REM - Reduce ELAN Machine

    Copyright (C) 2000-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
			     Nancy, France.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr

*/
#include "codes.h"   // ../../../../elan/
#include "termCommon.h"
#include "tools.h"
#include "streval.h"
//#include "Back.h"
#include "choice.h"

//TERMSTR(term0,1);
//TERMSTR(term3,3);


//#define DBG
 
#define BODY_FUN0(N) Gterm *N(int code) \
                     { \
                       Gterm *sv;\
                       GmakeAppl0(sv,code); \
		       return sv; \
		     }

#define BODY_FUN1(N) Gterm *N(int code, Gterm *s) \
                     { \
                       Gterm *sv;\
                       GmakeAppl1(sv,code,s); \
                       return sv; \
		     }

#define BODY_FUN2(N) Gterm *N(int code, Gterm *s, Gterm *t) \
                     { \
                       Gterm *sv;\
                       GmakeAppl2(sv,code,s,t); \
                       return sv; \
		     }

#define BODY_FUN3(N) Gterm *N(int code, Gterm *b, Gterm *s, Gterm *t) \
                     { \
                       Gterm *sv;\
                       GmakeAppl3(sv,code,b,s,t); \
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

// tone(@)
  BODY_FUN1(fun_205);
// tall(@)
  BODY_FUN1(fun_206);
// tsome(@)
  BODY_FUN1(fun_207);

Gterm *str_eval(Gterm *T)
{
Gterm *s;
Gterm *t;
 Gterm* (*semact)(Gterm *);
int ts;
  if (-term_semantic(T) == DS_APPLY) {
    s = GgetArgument(T,0);
    t = GgetArgument(T,1);
    ts = term_defstrat(s);
    if (IS_LAB_FLAG(ts)) {
      int apply_code = LAB_F(ts);   
      int dstr_code = LAB_LAB(ts);
      semact = term_semact(s);
      return (*semact)(T);} 
    else if (IS_DSTR_FLAG(ts)) {
      int apply_code = DSTR_F(ts);
      int dstr_code = DSTR_LAB(ts);
      semact = term_semact(s);
      return (*semact)(T); }
    else
      return str_eval2(s,t);
  } else {
    fprintf(stderr,"strategy term does not contain applictation symbol\n"); 
    exit(1); }
}

Gterm *str_eval2(Gterm *s, Gterm *t)
{
int defstrat = term_defstrat(s);
int is_one = 0;
{
}

    switch (-term_semantic(s)) {
      case DS_ID:
	return t;
	break;
      case DS_FAIL:
	fail();
      case DS_CONC:
	{
	  Gterm *s1, *s2;
	  Gterm *res;
	  s1 = GgetArgument(s,0);
	  s2 = GgetArgument(s,1);
	  res = str_eval2(s1,t);
	  return str_eval2(s2,res);
	 break; }
      case DS_DK:
	{
	  Gterm *strlist, *res;
	  int is_last = 0;

	  for(strlist = GgetArgument(s,0);
	      -term_semantic(strlist) != DS_EPSILON;
	      strlist = GgetArgument(strlist,1)) {
	    if (-term_semantic(strlist) != DS_COMMA &&
	        -term_semantic(strlist) != DS_COMMA_CONCUR) {
	      fprintf(stderr,"DS_COMMA, DS_COMMA_CONCUR symbol expected in DC/DK strategy list\n");
	      exit(1); }
	    is_last = (-term_semantic(GgetArgument(strlist,1)) == DS_EPSILON);
	    if (!is_last) {
	      if(!setChoicePoint()) {
		res = str_eval2(GgetArgument(strlist,0),t);
		goto lab_dk; }
	    } else { // 0206
	      res = str_eval2(GgetArgument(strlist,0),t);
            }
	  }
	lab_dk:
	  return res;
	  break; }
      case DS_ONE:
        is_one = 1;  // THIS SHOULD FOLLOW 
      case DS_DC:
	{
	  Gterm *strlist, *res;
	  int *wasr = (int*)(allocStable(sizeof(int)));
	  int is_last = 0;
	  *wasr = 0;

	  for(strlist = GgetArgument(s,0);
	      -term_semantic(strlist) != DS_EPSILON;
	      strlist = GgetArgument(strlist,1)) {
	    if (-term_semantic(strlist) != DS_COMMA &&
	        -term_semantic(strlist) != DS_COMMA_CONCUR) {
	      fprintf(stderr,"DS_COMMA, DS_COMMA_CONCUR symbol expected in DC/DK strategy list\n");
	      exit(1); }
	    is_last = (-term_semantic(GgetArgument(strlist,1)) == DS_EPSILON);
	    if (!is_last) {
	      if(!setChoicePoint()) {
		if (is_one) { CUTOPEN(); }
		res = str_eval2(GgetArgument(strlist,0),t);
		if (is_one) { CUTCLOSE(); }
		if (*wasr == 0) *wasr = 1;
		goto lab_dc;
	      }
	      if (*wasr != 0) fail();
	    } else // is_last
	      res = str_eval2(GgetArgument(strlist,0),t);
	  }
	lab_dc:
	  return res;
	  break; }
      case DS_IFTE:
	{
      	  Gterm *cond, *s1, *s2;
	  cond = GgetArgument(s,0);
          s1 = GgetArgument(s,1);
          s2 = GgetArgument(s,2);
	  if (GgetSymb(cond)) { // > 0 is true  //ehm//cond->symb
	    return str_eval2(s1,t); }
	  else {
	    return str_eval2(s2,t); }
	}
      case DS_IFTOE:
	{
	  Gterm *res, *cond, *s1, *s2;
	  int *wasr = (int*)(allocStable(sizeof(int)));
	  *wasr = 0;
	  cond = GgetArgument(s,0);
          s1 = GgetArgument(s,1);
          s2 = GgetArgument(s,2);
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
	  Gterm *sv;
	  Gterm* (*semact)(Gterm *);
 	  GmakeAppl2(sv,apply_code,s,t);
	  semact = term_semact(s);
	  return (*semact)(sv);} 
	else if (IS_FSYM_FLAG(defstrat)) {
	  int arity = term_arity(s);
	  int f1 = FSYM_F1(defstrat);
	  int f2 = FSYM_F2(defstrat);
	  Gterm *res;
	  if (GgetSymb(t) == f1) {
	    int i;
	    if (arity == 0 && f1 == f2) return t;
	    else {
	      GmakeApplArity(res,arity,f2);
	      for(i=0; i<arity; i++) 
		GsetArgument(res,i,str_eval2(GgetArgument(s,i),GgetArgument(t,i)));
	      return res;
	    }
	  }
	  else
	    fail(); }
	else if (IS_DSTR_FLAG(defstrat)) {
	  int apply_code = DSTR_F(defstrat);  
	  int dstr_code = DSTR_LAB(defstrat);
	  Gterm *sv;
	  Gterm* (*semact)(Gterm *);
	  GmakeAppl2(sv,apply_code,s,t);
	  semact = term_semact(s);
	  return (*semact)(sv); 
	} else {
	  fprintf(stderr,"unknown strategy constructor %d in str_eval\n",-term_semantic(s)); 
	  exit(1); }
    }
}





