#include "main.h"
#include "aterm1.h"
#include "aterm2.h"

#define HASH_SIZE 100000
#define SPACE_SIZE HASH_SIZE*20

struct term* termArray[SPACE_SIZE];

struct cell {
  struct term *term;
  struct cell *next;
} cell;

struct cell *hashArray[HASH_SIZE]; 

int          current_space_index; // act position in field
int          space_index; // elements in field
int          myeqcount; // number of comparisions made
int          mytranscount; // number of transitions

void init() {
  int i;
  for(i=0 ; i<HASH_SIZE ; i++) {
    hashArray[i] = NULL;
  }
  current_space_index = 0;
  space_index = 0;
}

struct term *ATerm2term(ATerm t) {
  AFun afun;
  int code;
  int arity;
  struct term *res;
  int i;
  
  afun = ATgetAFun(t);
  code = atoi(ATgetName(afun));
  arity = ATgetArity(afun);

  TERM_ARITY_ALLOC(res,arity,code);
  for(i=0 ; i<arity ; i++) {
    setFreeSubterm(res,i,ATerm2term(ATgetArgument(t,i)));
  }
  return res;
}

ATerm term2ATerm(struct term *t) {
  int arity;
  ATerm args[32];
  AFun afun;
  char name[32];
  int i;
  
  if(isTagged(t)) {
    printf("isTagged not yet implemented\n");
    exit(1);
  }

  arity=term_arity(t);
  sprintf(name,"%d",getSymb(t));
  afun = ATmakeAFun(name,arity,ATfalse);
  
  for(i=0 ; i<arity ; i++) {
    args[i] = term2ATerm(getFreeSubterm(t,i));
  }
  args[i] = NULL;
  
  return (ATerm)ATmakeApplArray(afun,args);
}

void printTerm(char* s, struct term* t) {
  printf("%s ",s);
  termOut(stdout,t);
  printf("\n");
}

// compute a hash-value of a term
#define compute_hash_val(t) hashTerm(t)

// compares two terms
#define term_eq(t1,t2) term_notDestructEqual(t1,t2)

// inserts term into field
// acts like a set, not like a list
// uses hash-values if possible

void insert(struct term* t) {
  int i;
  int hc;
  int size;
  struct cell *c;
  if(space_index == SPACE_SIZE) {
    printf("termArray[%d] is full\n",SPACE_SIZE );
    exit(1);
  }
  hc = compute_hash_val(t) % HASH_SIZE;

  for(c=hashArray[hc] ; c!=NULL ; c=c->next) {
    if(term_eq(c->term,t)) {
      break;
    }
  }
  if(c==NULL) {
    termArray[space_index++] = t;
    c = (struct cell*) malloc(sizeof(cell));
    c->next = hashArray[hc];
    c->term = t;
    hashArray[hc] = c;

      //ATprintf("t = %t\n", term2ATerm(t));
      //term_println(stdout, ATerm2term(term2ATerm(t)));
    
    if(!term_eq(t, ATerm2term(term2ATerm(t)))) {
      printf("strange\n");
        //ATprintf("t = %t\n", term2ATerm(t));
      printf("t1 = "); term_println(stdout,t);
      printf("t2 = "); term_println(stdout, ATerm2term(term2ATerm(t)));
      exit(1);
    }
    
  }
}

int isFinish() {
  return current_space_index == space_index;
}

struct term* getnext() {
  struct term* t;
  t = termArray[current_space_index];
  current_space_index++;
    //printf("."); fflush(stdout);
  if(current_space_index%10000 == 0) {
    printf("current_space_index = %d\t",current_space_index);
    printf("space_index = %d\n",space_index);
  }
  
  return t;
}


void build_whole_system(struct term* term) {
  int i;
  struct term* ht;
  struct term* res;

  int getRccstIndex     = getSymbolIndex("getRccst");
  int getCcstIndex      = getSymbolIndex("getCcst");
  int getLabelCcstIndex = getSymbolIndex("getLabelCcst");
  int getHashTableIndex = getSymbolIndex("getHashTable");
  int buildRccstIndex   = getSymbolIndex("buildRccst");
  int calc_strIndex     = getStrategyIndex("calc_str");
  int cleanAllIndex     = getSymbolIndex(".cleanAll");
  
  res = funTab[getRccstIndex](term);
  res = funTab[getCcstIndex](res);
  ht  = funTab[getHashTableIndex](term);
    //printf("ht= "); term_println(stdout,ht);
  
    //printf("Start-Term:\n");
    //termOut(stdout,res);
    //printf("\n\n");
  insert(res);
  while (!isFinish()) {
    if(current_space_index%30000 == 0) {
      printf("clean\n");
      ht = funTab[cleanAllIndex](ht);
    }
    
    if(!setChoicePoint()) {
      res = getnext();  
      res = funTab[buildRccstIndex](res,ht);
      res = strTab[calc_strIndex](res); //calc succs
        //printf("-");fflush(stdout);
      mytranscount++;
      res = funTab[getRccstIndex](res);
      res = funTab[getLabelCcstIndex](res);
      insert(res);
      fail();
    }
  }
  printf("\n\nSize: %d\n",space_index);
  printf("Transitions: %d\n",mytranscount);
  printf("Compare-Count: %d \n", myeqcount);
  printf("Hashvalues were used\n");
  return;
}

int main(int argc,char **argv) {
  long bp;
  struct term *res;
  struct term *tmp1, *tmp2;

    //traceLevel = 2;
  initElanLib(&bp);
  ATinit(argc, argv,(ATerm*) &bp);
  
  init();
  
  if(!setChoicePoint()) {
    /* Input */
    res=normalise(EarleyParser("rccst"));
      //printf("\nquery = "); termOut(stdout,term_unflatten(res)); printf("\n");
    res = strTab[getStrategyIndex("start_str")](res);
      //printf("\nresult = "); termOut(stdout,term_unflatten(res)); printf("\n");
    build_whole_system(res);
    
    // ------------------------------------------------------------
    fail();
  }

  exit(0);
}
