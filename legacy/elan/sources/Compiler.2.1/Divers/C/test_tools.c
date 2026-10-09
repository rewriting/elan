#include "tools.h"

//#define MALLOC(n) malloc(n)
//#define FREE(p) free(p)

//#define MALLOC(n) intern_alloc(n)
//#/define FREE(p) intern_free((long*)p)

//#define MALLOC(n) GC_malloc_atomic(n)
//#define FREE(p) GC_free(p)

main() {
  int i;
  char *adr;

  init_alloc();

  for(i=0 ; i<10000000 ; i++) {
    adr = (char*) MALLOC(i%100);

    //printf("%d\n",adr);

    FREE(adr);
  }
}


/*
GC_malloc               : 7.799u 0.116s
GNU malloc              : 5.215u 0.023s
malloc                  : 4.932u 0.021s
GC_malloc_uncollectable : 4.649u 0.018s
GC_malloc_atomic        : 3.479u 0.021s
intern_alloc            : 1.936u 0.008s 

 */
