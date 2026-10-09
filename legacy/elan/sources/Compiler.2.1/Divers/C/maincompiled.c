#include"RTCommons.h"

extern  struct term *f1list;
TERMSTR(term1,1);
extern  struct term *f2list;
TERMSTR(term2,2);
extern  struct term *f3list;
TERMSTR(term3,3);
extern  struct term ccon200;
extern  struct term *con200;
extern  struct term *fun200();
extern  struct term ccon201;
extern  struct term *con201;
extern  struct term ccon202;
extern  struct term *con202;
extern  struct term ccon203;
extern  struct term *con203;
extern  struct term ccon204;
extern  struct term *con204;
extern  struct term ccon205;
extern  struct term *con205;
extern  struct term ccon206;
extern  struct term *con206;
extern  struct term ccon210;
extern  struct term *con210;

extern struct term *str_s0_Foo_bug(
#ifdef __cplusplus
struct term*
#endif
);

extern int nofreductions,nofsreductions,nonamed_tried,nofr2,named_tried,number_of_fail;
struct term *mainterm()
{ 
return(fun200());}



int Bins = 0;


long *bp_main;

main()
{
  long bp;
  struct term *pp;
  bp_main=&bp;
  backTrackInit();
  timestart();
  fprintf(stderr,"\176 [main] start:\176");
  fflush(stderr);
  if (! setChoicePoint()) {
  pp=mainterm();
  pp=str_s0_Foo_bug(pp);
  fprintf(stderr,"\n[] result term: \n    ");
  fflush(stderr);
  termwrite(pp,0);
  fprintf(stdout,"\176");
  fflush(stdout);
  freeterm(pp);
  number_of_fail++;fail();return(NULL);
  } else {
  }
}

