#include "server.h"

/*
 * getSymbolIndex(<symbol name without '@'>): returns the internal index corresponding to this symbol
 *  - sybol is either a constructor either a defined symbol
 *
 * getStrategyIndex(<strategy name>): returns the internal index corresponding to the strategy
 *
 * funTab: array of functions corresponding to defined symbols
 *  (i.e symbols that root an unlabeled rewrite system)
 * strTab: array of strategies
 *
 * GmakeAppl1(dest,symbol_code,arg1):
 *  - allocate a term rooted by symbol_code
 *  - set arg1 as the (first) subterm,
 *  - store the result in dest
 *  NOTE : dest and arg1 must be different!
 *
 * GmakeAppl2(dest,symbol_code,arg1,arg2)
 * ...
 * GmakeAppl(Gterm **ptr_dest,int code,int arity,...);
 *  - see $prefix/include/elan-compiler/termBase.h 
 *
 * GgetArgument(term,pos)
 * GgetSymb(t)    
 *  - see $prefix/include/elan-compiler/termBase.h 
*/


void printTerm(char* s, Gterm* t) {
  printf("%s ",s);
  termOut(stdout,t);
  printf("\n");
}

Gterm *makePeano(int n) {
  int zero_index;
  int suc_index;
  int i;
  Gterm *res, *tmp;
  zero_index = getSymbolIndex("o");
  suc_index  = getSymbolIndex("s()");

  printf("zero_index = %d\n",zero_index);
  printf("suc_index = %d\n",suc_index);

  GmakeAppl0(tmp,zero_index);
  
  for(i=0 ; i<n ;i++) {
    GmakeAppl1(res,suc_index,tmp);
    tmp = res;
  }
  return res;
}

Gterm *makeTest(Gterm *t) {
  int zero_index;
  int test_index;
  int i;
  Gterm *res, *tmp;
  zero_index = getSymbolIndex("o");
  test_index = getSymbolIndex("test(,)");
  GmakeAppl0(tmp,zero_index);
  GmakeAppl2(res,test_index,t,tmp);
  return res;
}

Gterm *makeFib(Gterm *t) {
  int fib_index;
  Gterm *res;
  fib_index = getSymbolIndex("fib()");
  GmakeAppl1(res,fib_index,t);
  return res;
}

int main(int argc,char **argv) {
  long bp;
  Gterm *query;
  Gterm *res;

  initElanLib(&bp);
    
  if(!setChoicePoint()) {

    query = makePeano(5);
    
    printf("\nquery = "); termOut(stdout,term_unflatten(query)); printf("\n");
    res = funTab[getSymbolIndex("fib()")](query);
    printf("\nresult fib = "); termOut(stdout,term_unflatten(res)); printf("\n");

    query = makeFib(res);
    printf("\nquery = "); termOut(stdout,term_unflatten(query)); printf("\n");
    res = normalise(query);
    printf("\nresult normalise= "); termOut(stdout,term_unflatten(res)); printf("\n");

    query = makeTest(res);
    res = strTab[getPrefixStrategyIndex("s0")](query);
    printf("\nresult s0  = "); termOut(stdout,term_unflatten(res)); printf("\n");

      //res=normalise(EarleyParser("term"));
      //printf("\nresult earley = "); termOut(stdout,term_unflatten(res)); printf("\n");

    fail();
  }

  exit(0);
}

#define elan_get_term(t) funTab[getSymbolIndex("get_term()")](t)

Gterm *fun_220(Gterm *t) {
  Gterm *res;
  Gterm *subterm;
  Gterm *tmp;
  int any_index = GgetSymb(t); // or getSymbolIndex("[]")
  
    //subterm = GgetArgument(t,0);
  subterm = elan_get_term(t);
    // dispatching on GgetSymb(subterm)
  GmakeAppl1(tmp,getSymbolIndex("s()"),subterm);
  GmakeAppl1(res,any_index,tmp);

  return res;
}
