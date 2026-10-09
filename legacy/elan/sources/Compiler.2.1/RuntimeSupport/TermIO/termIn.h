#ifndef _termin_h

extern struct term *termParser(int queryMode, int evaluationMode);
extern struct term *EarleyParser();

extern void esemactinit();
extern void tabofidentInit();
extern void typetInit();
extern void grammarInit();
extern void earleyInit();
extern int earleyCall();
extern void prefixParser(char *s1, char *s2);

extern char *tabIdentStr[];
extern int tabIdentIndex[];
extern int tabIdentSize;

extern char *tabSortStr[];
extern int tabSortIndex[];
extern int tabSortSize;

extern char *tabStrategyStr[];
extern int tabStrategyIndex[];
extern int tabStrategySize;

extern earleyQuerySort ; 
extern earleyQueryStrategy ; 



#endif

