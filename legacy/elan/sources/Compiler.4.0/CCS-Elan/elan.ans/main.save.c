#include "main.h"



#define MYFSIZE 25000
#define MYHASH

struct term* myprog; // for the fix program term
  // store hash_vals in table
struct termfield {
  struct term* term;
  int hash_val;
} myfield[MYFSIZE];

int          mycount; // act position in field
int          myfsize; // elements in field
int          myeqcount; // number of comparisions made
int          mytranscount; // number of transitions

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
  if (myfsize == MYFSIZE) {// table is full?
    printf("table is full\n");
    return;
  }
  hc = compute_hash_val(t);
  for (i=0; i<myfsize; i++) {
    if (hc == myfield[i].hash_val) {
      myeqcount++;
      if (term_eq(t,myfield[i].term))
	break;
    }
  }
  if (i==myfsize) {
    myfield[myfsize].term = t;
    myfield[myfsize].hash_val = hc;
    myfsize++;
  }
}

// 
int fertig() {
  return (myfsize == mycount);
}

struct term* getnext() {
  struct term* t;
  t = myfield[mycount].term;
    //printf("."); fflush(stdout);
  mycount++;

  if(mycount%1000 == 0) {
    printf("mycount = %d\n",mycount);
  }
  
  return t;
}


void build_whole_system(struct term* term) {
  int i;
  struct term* ht;
  struct term* res;

  int getRccstIndex     = getSymbolIndex("getRccst");
  int getCcstIndex      = getSymbolIndex("getCcst");
  int getLabelCcstIndex      = getSymbolIndex("getLabelCcst");
  int getHashTableIndex = getSymbolIndex("getHashTable");
  int buildRccstIndex   = getSymbolIndex("buildRccst");
  int calc_strIndex     = getStrategyIndex("calc_str");
  mycount = 0;

  res = funTab[getRccstIndex](term);
  res = funTab[getCcstIndex](res);
  ht  = funTab[getHashTableIndex](term);
    //printf("ht= "); term_println(stdout,ht);
  
    //printf("Start-Term:\n");
    //termOut(stdout,res);
    //printf("\n\n");
  insert(res);
  while (!fertig()) {
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
  printf("\n\nSize: %d\n",myfsize);
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
