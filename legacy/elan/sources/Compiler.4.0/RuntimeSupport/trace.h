#ifndef _trace_h
#define _trace_h

/* ajout de nouvelle structure de trace */
typedef int POS[500];
typedef char RNAME[64];

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

/* temporary files for communicating with Coq */
char trace_file[24];
char query_file[24];
char query_sort_file[24];

extern POS position;     // suite de position
extern int MAXPOS;       // lg de la suite de position
extern struct NODE *racine;      // racine de l'arbre de position
extern struct TR_COQ *head_tr;    // debut de la liste da trace
extern struct TR_COQ *tail_tr;    // fin de la liste de trace
extern RNAME rname;
extern unsigned int nb_node;
extern Gterm * T;
extern int strCall;

extern int initialise_trace();
extern int trace_pretty_print(TR_COQ * head,int deep, FILE *fp_trace);
extern void setTermNoReduced(Gterm *t);
extern int add_trace(TR_COQ *head_cond_left_tr,
	      TR_COQ *head_cond_right_tr);

extern int trace_display();
extern int trace_free();
extern Gterm* norm_in(Gterm *t);
extern Gterm* norm_out(Gterm *t);
extern Gterm* norm_lazy(Gterm *t);
extern Gterm *coqEarleyParser(char *querySortName);
extern int coqprefixParser(char *s1, char *s2);
extern Gterm * subterm_remove(Gterm * t, Gterm * s);
extern void termOutTrace(FILE *fich,Gterm *t,Gterm *pi);
extern int getCodeByNamePrefix(char * sym_name);
extern void trace_backup(Gterm *res,int *pt_backup, int *MAXPOS_tmp, POS position_tmp);
extern void trace_recover(int pt_backup, int MAXPOS_tmp,POS position_tmp);
extern void rhoproofterm_print(Gterm *pi);
#endif
