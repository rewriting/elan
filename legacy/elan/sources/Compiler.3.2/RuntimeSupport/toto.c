//#include "builtin.h"

#define funTabCall0
#define funTabCall1 arg[0]
#define funTabCall2 funTabCall1,arg[1]
#define funTabCall3 funTabCall2,arg[2]
#define funTabCall4 funTabCall3,arg[3]
#define funTabCall5 funTabCall4,arg[4]

main() {

  funTabCall5;

  /*
  struct term* t1;
  struct term* t2;
  struct term* t3;
  
  t1=setIntegerTag(777);
  printf("isTag : %d\n",isIntegerTagged(t1));
  printf("isTag : %d\n",isIdentifierTagged(t1));
  printf("isTag : %d\n",isStringTagged(t1));
  printf("val = %d\n",getInt(t1));

  t2=setIdentifierTag(777);
  printf("isTag : %d\n",isIntegerTagged(t2));
  printf("isTag : %d\n",isIdentifierTagged(t2));
  printf("isTag : %d\n",isStringTagged(t2));
  printf("val = %d\n",getIdentifier(t2));

  t3=setStringTag("toto");
  printf("isTag : %d\n",isIntegerTagged(t3));
  printf("isTag : %d\n",isIdentifierTagged(t3));
  printf("isTag : %d\n",isStringTagged(t3));
  printf("val = %s\n",getString(t3));
  */
}
