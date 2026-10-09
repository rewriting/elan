#ifndef _trace_h
#define _trace_h

/* fichiers temporaires */
char trace_file[20];
char query_file[20];
char query_sort_file[20];

/* ajout de nouvelle structure de trace */
typedef int POS[500];
typedef char RNAME[15];

typedef struct COND_NODE{ // [Huy: May  7 00] 
 struct TR_COQ *leftside;
 struct TR_COQ *rightside;
} COND_NODE;

typedef struct NODE{
 struct NODE *father;
 struct NODE *next;
 struct NODE *son;
 int ps;
} NODE;

typedef struct TR_COQ{
 RNAME rname;
 struct NODE *rnode;
 struct TR_COQ *next;
 struct COND_NODE *cond_node; //pointeur vers la trace d'une condition
} TR_COQ;

extern POS position;     // suite de position
extern int MAXPOS;       // lg de la suite de position
extern struct NODE *racine;      // racine de l'arbre de position
extern struct TR_COQ *head_tr;    // debut de la liste da trace
extern struct TR_COQ *tail_tr;    // fin de la liste de trace
extern RNAME rname;
extern unsigned int nb_node;
extern struct term * T;
extern int strCall;

extern int initialise_trace();
extern int trace_pretty_print(TR_COQ * head,int deep);
extern void setTermNoReduced(struct term *t);
extern int add_trace(TR_COQ *head_cond_left_tr,
	      TR_COQ *head_cond_right_tr);

extern int trace_display();
extern int trace_free();
extern struct term* norm_in(struct term *t);
extern struct term* norm_out(struct term *t);
extern struct term* norm_lazy(struct term *t);
extern struct term *coqEarleyParser(char *querySortName);
extern int coqprefixParser(char *s1, char *s2);

#endif
