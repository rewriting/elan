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
/*  [Huy: Sep 20 00]  */
#include "tools.h"
#include "termCommon.h"
#include "trace.h"
#include "builtin.h"

#define MAX_FUN_ARITY 16
#define THETA_FUNCTION "(THETA_"
#define THETA_TERM "(THETA"
#define INST_SYM "(inst)"
#define MAX_NUM_LAZY 100
#define MAX_LENGTH_SYMBOL 10

struct lazy_annotation{
  unsigned int symbol;  /* code of function symbol */
  unsigned int arg;     
}lazy_annotation[MAX_NUM_LAZY];
int index_lazy=-1;

extern int coqMode;
extern int fsymtabSize;

extern Gfsym fsymtab[]; /* declaration in elan/module.h */

int getCodeByName(char * sym_name){
  int i;
  char sym_tmp[MAX_LENGTH_SYMBOL+sizeof(THETA_FUNCTION)];
    
  sprintf(sym_tmp,"(%s)",sym_name);
  for (i = 0; i< fsymtabSize; i++)
    if (strncmp(fsymtab[i].name,sym_tmp,strlen(sym_tmp))==0)
      return i;
  
  return -1;
}

/* return 1: arg is a lazy argument of code
   return 0: arg is an eager argument of code */  
int lazy_check(int code, int arg)
{
  int i;
   
  for (i=0; i<=index_lazy; i++){
    if (lazy_annotation[i].symbol == code && lazy_annotation[i].arg == arg+1)
    {
      return 1;
    }
  }
  return 0;
}

/* return 1 if code is a THUNK of a function
   return 2 if code is a THUNK of a term */
int thunk_check(int code)
{
    if (strncmp(fsymtab[code].name,THETA_FUNCTION, strlen(THETA_FUNCTION)) == 0)
	return 1; 
    
    if (strncmp(fsymtab[code].name,THETA_TERM, strlen(THETA_TERM))==0)
	return 2;
    
    return 0;
}

/* unthunk the root of t*/
Gterm * unthunk(Gterm * t){
  int code;
  char sym[MAX_LENGTH_SYMBOL];
  Gterm * tmp;

  memset(sym,'\0',sizeof(sym));
  code = GgetSymb(t);

  if (thunk_check(code)==1){ /* check again for case of a constant*/
    strncpy(sym,&fsymtab[code].name[strlen(THETA_FUNCTION)],
            strlen(fsymtab[code].name)-strlen(THETA_FUNCTION)-1);
    GsetSymb(t,getCodeByName(sym));
    return t;
  }
  if (thunk_check(code)==2){ /* THEA_TERM is unthunked to inst(THETA_TERM...) */
    GmakeApplArity(tmp,1,getCodeByName("inst"));
    GsetArgument(tmp,0,t);
    return tmp;
  }  

  return t;
}

Gterm * phi_star(Gterm * t){
  int code, arity, i;
  Gterm * t1;

  if(GisIntegerTagged(t) || GisIdentifierTagged(t) || GisStringTagged(t)) {
    return t;
  }

  t1 =  unthunk(t);
  if (strncmp(fsymtab[GgetSymb(t1)].name,INST_SYM,strlen(INST_SYM))==0){
      /* THEA_TERM was unthunked to inst(THETA_TERM...) */
      t = GgetArgument(t1,0);//t1->sub[0];
  } else {
      t = t1;
  }
  //t = unthunk(t);
  arity = term_arity(t);
  code = GgetSymb(t);

  for (i=0; i<arity ; i++){
      /* unthunk also the eager subterm */
    if (!lazy_check(code,i)) {
      GsetArgument(t,i,phi_star(GgetArgument(t,i)));
    }
  }
  return t1;  
}

/* thunk a lazy  subterm f(t1,..,tn) -> THETA_F(f(t1,..,tn)) */
Gterm * thunk(Gterm * t){
  int code, arity, i;
  char sym[MAX_LENGTH_SYMBOL+sizeof(THETA_FUNCTION)], tmp[MAX_LENGTH_SYMBOL];
  
  if(GisIntegerTagged(t) || GisIdentifierTagged(t) || GisStringTagged(t)) {
    return t;
  }
  arity = term_arity(t);
  if (arity == 0) return t;

  code = GgetSymb(t);
  memset(sym,'\0',sizeof(sym));
  memset(tmp,'\0',sizeof(tmp));
  strncpy(tmp, &fsymtab[code].name[1], strlen(fsymtab[code].name)-2);
  sprintf(sym,"%s%s", &THETA_FUNCTION[1],tmp);
  GsetSymb(t,getCodeByName(sym));

  for (i=0; i<arity; i++){
    GsetArgument(t,i,thunk(GgetArgument(t,i)));
  }
  return t;
}

/* thunk an input term */
Gterm * thunk_term(Gterm * t){
  int code, arity, i;

  if(GisIntegerTagged(t) || GisIdentifierTagged(t) || GisStringTagged(t)) {
    return t;
  }
  
  arity = term_arity(t);
  code = GgetSymb(t);
  
  for (i=0; i<arity; i++){
    if (lazy_check(code,i)) {
      GsetArgument(t,i,thunk(GgetArgument(t,i)));
    } else {
      GsetArgument(t,i,thunk_term(GgetArgument(t,i)));
    }
  }
  return t;
}

Gterm * lazy_subterm_normalise(Gterm * t){
  int code, arity, i, pos1;

  if(GisIntegerTagged(t) || GisIdentifierTagged(t) || GisStringTagged(t)) {
    return t;
  }
  arity = term_arity(t);

  code = GgetSymb(t);

  if (thunk_check(code)){
      t = lazy_subterm_normalise(norm_in(phi_star(t)));
      return t;
  }      
  
  for (i=0; i<arity; i++){
      /* shift for trace */
    if (coqMode){
	pos1 = MAXPOS; 
	position[MAXPOS]=i;
	MAXPOS = MAXPOS +1;
	}
    GsetArgument(t,i,lazy_subterm_normalise(GgetArgument(t,i)));
    if (coqMode){
	MAXPOS = pos1;
    }
  }
  return t;
}

/* normalise a term based on lazy evaluation */
Gterm *norm_lazy(Gterm * t){
  int code, arity, i;

  if(GisIntegerTagged(t) || GisIdentifierTagged(t) || GisStringTagged(t)) {
    return t;
  }
  arity = term_arity(t);
  code = GgetSymb(t);
  if (index_lazy == -1) {/* normal leftmost-innermost , no lazy */
      t = norm_in (t);
  }else
  { /* lazy annotation exists => lazy normalisation procedure */
      t  = lazy_subterm_normalise(norm_in(thunk_term(t)));
  }

  return t;
}

int lazy_annotation_read(char * file_lazy)
{
  FILE *fp_lazy;
  char buff[255],lazy_arg_list[MAX_FUN_ARITY*2];
  int lazy_arg;
  char ch,sym[MAX_LENGTH_SYMBOL];
  int i =0,j;


  if ((fp_lazy = fopen(file_lazy,"r")) == NULL) return 0;
  memset(buff,'\0',sizeof(buff));	
  while ((ch=getc(fp_lazy))!=EOF){
    if (ch != '\n') {
      buff[i]= ch;
      i = i+1;
    }
    else
    {
      j=0;
      while(buff[j] != '@') {
        sym[j]=buff[j]; 
        j= j+1;
      }
      sym[j]='\0';
      strcpy(lazy_arg_list,&buff[j+1]);
      while (strchr(lazy_arg_list,';')!= NULL){
        sscanf(lazy_arg_list,"%d;%s",&lazy_arg,lazy_arg_list);
        index_lazy = index_lazy + 1;
        lazy_annotation[index_lazy].symbol = getCodeByName(sym);
        lazy_annotation[index_lazy].arg = lazy_arg;
      }
      index_lazy = index_lazy + 1;
      lazy_annotation[index_lazy].symbol = getCodeByName(sym);
      lazy_annotation[index_lazy].arg = atoi(lazy_arg_list);	
      i = 0;
      memset(buff,'\0',sizeof(buff));	
    }
  }
  fclose(fp_lazy);
  return 0;
}
