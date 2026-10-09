/*  [Huy: Sep 20 00]  */
#include "tools.h"
#include "trace.h"
#include "term.h"
#include "builtin.h"

#define MAX_FUN_ARITY 16
#define THETA_FUNCTION "(THETA_"
#define THETA_TERM "(THETA"
#define MAX_NUM_LAZY 100
#define MAX_LENGTH_SYMBOL 10

struct lazy_annotation{
  unsigned int symbol;  /* code of function symbol */
  unsigned int arg;     
}lazy_annotation[MAX_NUM_LAZY];
int index_lazy=-1;

extern int coqMode;
extern int fsymtabSize;
extern fsym fsymtab[]; /* declaration in elan/module.h */

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
struct term * unthunk(struct term * t){
  int code;
  char sym[MAX_LENGTH_SYMBOL];
  struct term * tmp;

  memset(sym,'\0',sizeof(sym));
  code = getSymb(t);

  if (thunk_check(code)==1){ /* check again for case of a constant*/
    strncpy(sym,&fsymtab[code].name[strlen(THETA_FUNCTION)],
            strlen(fsymtab[code].name)-strlen(THETA_FUNCTION)-1);
    setSymb(t,getCodeByName(sym));
    return t;
  }
  if (thunk_check(code)==2){ /* check again for case of a constant*/
    TERM_ARITY_ALLOC(tmp,1,getCodeByName("inst"));
    tmp->sub[0] = t;
    return tmp;
  }  

  return t;
}

struct term * phi_star(struct term * t){
  int code, arity, i;

  if(isIntegerTagged(t) || isIdentifierTagged(t) || isStringTagged(t)) {
    return t;
  }

  t = unthunk(t);
  arity = term_arity(t);
  code = getSymb(t);

  for (i=0; i<arity ; i++){
      /* unthunk also the eager subterm */
    if (!lazy_check(code,i)) t->sub[i]=phi_star(t->sub[i]);    
  }
  return t;  
}

/* thunk a lazy  subterm f(t1,..,tn) -> THETA_F(f(t1,..,tn)) */
struct term * thunk(struct term * t){
  int code, arity, i;
  char sym[MAX_LENGTH_SYMBOL+sizeof(THETA_FUNCTION)], tmp[MAX_LENGTH_SYMBOL];
  
  if(isIntegerTagged(t) || isIdentifierTagged(t) || isStringTagged(t)) {
    return t;
  }
  arity = term_arity(t);
  if (arity == 0) return t;

  code = getSymb(t);
  memset(sym,'\0',sizeof(sym));
  memset(tmp,'\0',sizeof(tmp));
  strncpy(tmp, &fsymtab[code].name[1], strlen(fsymtab[code].name)-2);
  sprintf(sym,"%s%s", &THETA_FUNCTION[1],tmp);
  setSymb(t,getCodeByName(sym));

  for (i=0; i<arity; i++){
    t->sub[i] = thunk(t->sub[i]);
  }
  return t;
}

/* thunk an input term */
struct term * thunk_term(struct term * t){
  int code, arity, i;

  if(isIntegerTagged(t) || isIdentifierTagged(t) || isStringTagged(t)) {
    return t;
  }
  
  arity = term_arity(t);
  code = getSymb(t);
  
  for (i=0; i<arity; i++){
    if (lazy_check(code,i)) t->sub[i] = thunk(t->sub[i]);
    else
      t->sub[i] = thunk_term(t->sub[i]);
  }
  return t;
}

struct term * lazy_subterm_normalise(struct term * t){
  int code, arity, i, pos1;
  
  if(isIntegerTagged(t) || isIdentifierTagged(t) || isStringTagged(t)) {
    return t;
  }
  arity = term_arity(t);

  code = getSymb(t);

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
    t->sub[i] = lazy_subterm_normalise(t->sub[i]);
    if (coqMode){
      MAXPOS = pos1;
    }
  }
  return t;
}

/* normalise a term based on lazy evaluation */
struct term * norm_lazy(struct term * t){
  int code, arity, i;

  if(isIntegerTagged(t) || isIdentifierTagged(t) || isStringTagged(t)) {
    return t;
  }
  arity = term_arity(t);
  code = getSymb(t);
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
