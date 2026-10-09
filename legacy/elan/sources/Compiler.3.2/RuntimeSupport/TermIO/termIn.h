#ifndef _termin_h
#define _termin_h
/*
 * Some constants
 */
#define IDENTCODE 0
#define TYPECODE  1
#define CHARCODE  2
#define NUMCODE   3
#define BLANKCODE 4
#define STRINGLENGTH 1000

/* sizes of arrays */

#define TABOFGRAM_SIZE  50000
#define TABOFARITY_SIZE 50000
#define TABOFIDENT_SIZE 3000
#define TABOFSORT_SIZE  500


#define RGLOP              01   /* global rule */
#define RPRIORITYMSK       007777
#define NUMRULE            69
#define IDENTRULE          70
#define DEFAULTRULE        0
#ifdef STRINGS
#define STRINGRULE         82
#endif

char buf[STRINGLENGTH];
int  currentSort ;
int  arity[TABOFARITY_SIZE] ; /* upper bound ??? */
char *tabIdent[TABOFIDENT_SIZE] ;
char *tabSort[TABOFSORT_SIZE] ;


extern struct term *termParser(int queryMode, int evaluationMode);
extern struct term *EarleyParser();
extern void EarleyParserInit();

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
extern int coqMode;
extern int printMode;
extern int strCall;

#endif

