/*
  
    REM - Reduce ELAN Machine

    Copyright (C) 2000-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
			     Nancy, France.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr

*/
#include "termCommon.h"
#include "tools.h"
#include "trace.h"
#include "termIn.h"
#include "builtin.h"
#include "termOut.h"

/*
#ifdef __cplusplus
typedef Gterm* (*funTabType)(...);
#else
typedef Gterm* (*funTabType)();
#endif
extern funTabType strTab[];
*/


extern int earleyQueryStrategy;
extern int rewrite_real_step;
extern Gterm * T;


char trace_file[MAX_FILENAME_LEN];
char query_file[MAX_FILENAME_LEN];
char query_sort_file[MAX_FILENAME_LEN];

POS position;     // suite de position
int MAXPOS;       // lg de la suite de position
struct NODE *racine;      // racine de l'arbre de position
struct TR_COQ *head_tr;    // debut de la liste da trace
struct TR_COQ *tail_tr;    // fin de la liste de trace
RNAME rname;
unsigned int nb_node;
Gterm * T;
Gterm *globalT;
Gterm **T_addr=&globalT;
extern int coqMode;
int strCall;
int first_step=1;  //[NGUYEN: May 31 01] 
extern FILE *file_query;
/******* input from text files ************************/
extern int earleyPos ;

extern Gterm *computeTerm(int *ppos);
extern int fsymtabSize;
extern Gfsym fsymtab[]; /* declaration in termBase.h */

Gterm *GcoqEarleyParser(char *querySortName) {
    int e ;
    Gterm *query;
    FILE *fp_query = NULL;  
  
    earleyInit() ;
    if (*querySortName != 0) {
	if ((earleyQuerySort = findTab(tabSort,TABOFSORT_SIZE,querySortName))==-1) return NULL;
    }

    if ((fp_query = fopen(query_file,"r"))!= NULL){
	e = earleyCall(fp_query,earleyQuerySort) ;
	if (e) {
	    query = computeTerm(&earleyPos) ;
	    printf("\n") ;
	    fflush(stdout) ;
	    fclose(fp_query);
	} else {     
	    fclose(fp_query);
	    return NULL;
	}
    }
    // [Huy: Apr 18 00] 
    else{
	e = earleyCall(NULL,earleyQuerySort) ;
	if (e) {
	    query = computeTerm(&earleyPos) ;
	    printf("\n") ;
	    fflush(stdout) ;
	} else{
	    return NULL;
	}
    }
    globalT=NULL;// [NGUYEN: Apr  4 01] Initialise T
    return(query) ;
}

/*
 * parser for the following grammar (3 cases)
 * ()
 * ( id )
 * ( id : id )
 */
/*  [Huy: May 14 00]  changement pour pouvoir prendre input d'un fichier texte*/

int GcoqprefixParser(char *s1, char *s2) {
    char c;
    FILE *fp_query_sort ;  //ce qui peut remplacer le stdin

    if (!coqMode) {
      fprintf(stdout,"Enter a query of the form: ([[<strat>:]<sort>]) <term> end\n");
      fflush(stdout);
    }

    s1[0]='\0';
    s2[0]='\0';
    if ((fp_query_sort=fopen(query_sort_file,"r")) == NULL){
	fp_query_sort = stdin;
    }
    if(fscanf(fp_query_sort," ( %c",&c) != 1) {
	printf("'(' expected\n");
	return 1; 
    }
    if(c==')') {
	/* () */;
    } else {
	ungetc(c,fp_query_sort); //stdin
	if(fscanf(fp_query_sort," %[^ :)] %c",s1,&c) != 2) {
	    printf("identifier expected\n");
	    return 1; 
	}
    
	if(c==')') {
	    /* ( id ) */;
	} else {
	    if(c!=':') {
		printf("':' expected\n");
		return 1; 
	    }
	    if(fscanf(fp_query_sort," %[^ )] %c",s2,&c) != 2) {
		printf("identifier expected\n");
	    }
	    /* ( id : id ) */
	}
    }
    if (fp_query_sort != stdin) fclose(fp_query_sort);
    return 0;

}

/**** Structure of trace ******/
// [Huy: Apr 18 00]
extern int coqMode;
int Ginitialise_trace()
{
    struct NODE *node;

    head_tr = NULL;
    tail_tr = NULL;
    if((node=(struct NODE*)MALLOC(sizeof(struct NODE))) != NULL) {
	node->father = NULL;
	node->next =  NULL;
	node->son = NULL;
	node->ps = 2;
	racine = node;
    }
    else
	return 1;

    return 0;
}

void GsetTermNoReduced(Gterm *t){
    int arity, i;
    if(GisIntegerTagged(t) || GisIdentifierTagged(t) || GisStringTagged(t)) {
	return;
    }


  arity = term_arity(t);
  for(i=0 ; i<arity ; i++) {
    GsetTermNoReduced((Gterm*)GgetArgument(t,i));
  }
  GclearReduced(t);
}

int Gtrace_pretty_print(TR_COQ * head,int deep, FILE * fp_trace)
{
    TR_COQ *tr;
    NODE *node;
    char tmp[5];
    char output[1000],output1[1000];
    int i,j;

    if (coqMode == 0) return 2;
    i = 0;
    tr = head;

 
    while (tr != NULL)
	{ 
	    memset(output,'\0',sizeof(output));
	    node = tr->rnode;
	    while (node != racine)
		{
		    i = i+1;
		    if (node->father == racine)
			{ 
			    snprintf(tmp,sizeof(tmp),"%d",node->ps);
			}
		    else
			{
			    snprintf(tmp,sizeof(tmp),";%d",node->ps);
			}
		    if (snprintf(output1,sizeof(output1),"%s%s",tmp,output) >= (int)sizeof(output1)) {
			/* too long a position: truncated (it overflowed output1) */
		    }
		    strcpy(output,output1);
		    node = node->father;
		}
	    for (j=1; j<=deep; j++) fprintf(fp_trace," ");
	    fprintf(fp_trace, "%s @ [%s]\n",tr->rname,output);
	    if (tr->cond_node->leftside != NULL)
		Gtrace_pretty_print(tr->cond_node->leftside,deep+2,fp_trace);
	    if (tr->cond_node->rightside != NULL) 
		Gtrace_pretty_print(tr->cond_node->rightside,deep+2,fp_trace);
	    tr = tr->next;
	}
    //write(nsock,"END",3);
    //printf("\n# position:%d\n",i); 

    return 0;
}



int add_trace(TR_COQ *head_cond_left_tr,
	      TR_COQ *head_cond_right_tr)
{
    TR_COQ *t;
    NODE *node,*rnode, *fnode;
    COND_NODE *cond;
    int i,dk_flag;

    if ((t=(TR_COQ *)MALLOC(sizeof(struct TR_COQ)))!= NULL)
	{
	    strcpy(t->rname,rname);
	    if ((cond = (COND_NODE *)MALLOC(sizeof(struct COND_NODE)))!= NULL){
		cond->leftside = head_cond_left_tr;
		cond->rightside = head_cond_right_tr;
	    }
	    t->cond_node = cond;


	    fnode = racine;
	    for (i=0; i<MAXPOS ; i++)
		{
		    node = fnode->son;
		    if (node == NULL)  
			{
			    if ((rnode = (NODE *)MALLOC(sizeof(struct NODE))) != NULL) // liste de fils vide
				{
				    rnode->father = fnode;
				    rnode->next = NULL;
				    rnode->son = NULL;
				    rnode->ps = position[i];
				    fnode->son = rnode;
				    fnode = rnode;
				}
			}
		    else   // liste de fils non-vide
			{
			    if (node->ps == position[i]) 
				{
				    fnode = node;
				}
			    else if (node->ps > position[i]) //liste a un noeud plus grand que pos
				{
				    if ((rnode = (NODE *)MALLOC(sizeof(struct NODE))) != NULL) 
					{
					    rnode->father = fnode;
					    rnode->next = node;
					    rnode->son = NULL;
					    rnode->ps = position[i];
					    fnode->son = rnode;
					    fnode = rnode;
					}
				}else{
				    dk_flag = 0;
				    while (node->next != NULL && dk_flag ==0 )  // chercher et inserer en ordre de pos
					{
					    if (node->next->ps >= position[i]) dk_flag = 1;
					    else
						node = node->next;
					}
				    if (dk_flag == 1){
					if (node->next->ps == position[i]) // la position est deja dans la liste
					    {
						fnode = node->next;
					    }
					else
					    {
						if ((rnode = (NODE *)MALLOC(sizeof(struct NODE))) != NULL) 
						    {   // inserer au milieu de la liste
							rnode->father = fnode;
							rnode->next = node->next;
							rnode->son = NULL;
							rnode->ps = position[i];
							node->next = rnode;
							fnode = rnode;
						    } 
					    }
				    }
				    else // dk_flag==0 inserer a la fin de la liste
					{
					    if ((rnode = (NODE *)MALLOC(sizeof(struct NODE))) != NULL) 
						{
						    rnode->father = fnode;
						    rnode->next = NULL;
						    rnode->son = NULL;
						    rnode->ps = position[i];
						    node->next = rnode;
						    fnode = rnode;
						}
					}
     
				}
			}
		}

     
	    t->rnode = fnode;
	    t->next = NULL;
	    if (head_tr == NULL)
		{
		    head_tr = t;
		    tail_tr = t;
		}
	    else
		{
		    tail_tr->next = t;
		    tail_tr= t;
		}
	}
    else
	return 1;

    return 0;
}
/* inutil grace au gabarge collector */
int trace_free()
{
    TR_COQ *tr,*tr1;

    tr = head_tr;
    nb_node = 0;
    while(tr != NULL)   // liberer la liste de trace
	{
	    tr1 = tr->next ;
	    IFREE(tr);   
	    tr = tr1;
	}

    tree_free(racine);
    printf("# node:%d\n",nb_node);
    return 0;
}

int tree_free(NODE *node) // fonction recursive pour liberer la memoire
{
    NODE *snode,*snode1;

    if (node == NULL) return 0;
    snode = node->son;
    if (snode == NULL) { IFREE(node); nb_node= nb_node+1;}
    else
	{
	    while(snode != NULL)
		{
		    snode1 = snode->next;
		    tree_free(snode);
		    snode = snode1;
		}
	    IFREE(node); nb_node = nb_node +1;
	}
    return 0;
}



/* fin d'ajout pour la trace */

/*******
  HUY 16/02/00: parcourir le terme (leftmost-innermost) pour appliquer la strategie Norm 
  normalise pour les regles nommees
  ********/
extern int strTabSize;
extern int strMode;      //par default: leftmost-innermost

 // [Huy: May  2 00] 
/* Normalisation in set of unlaballed rules 
used reduced flag */
Gterm * norm_0(Gterm *t) {
    int arity, i;

    if (t == NULL) return NULL;
    if(GisIntegerTagged(t) || GisIdentifierTagged(t) || GisStringTagged(t)) {
	return t;
    }

    if (GisReduced(t)) return t;
    arity = term_arity(t);
    for(i=0 ; i<arity ; i++) {
    	GsetArgument(t,i,norm_0((Gterm*)GgetArgument(t,i)));
    }
    
    t = (Gterm*)GspecialApply(t);
    return t;
}

/****************
      leftmost-innermost
      **********************/

Gterm * norm_1(Gterm *t) {
    int arity, i;
   Gterm *t_tmp;
   int pos1;

    if(GisIntegerTagged(t) || GisIdentifierTagged(t) || GisStringTagged(t)) {
	return t;
    }
      //if(GisReduced(t)) {
      //return t; // [Huy: Apr 30 00]
      //}
    arity = term_arity(t);
  
    if(!term_isAC(t)) {
	for(i=0 ; i<arity ; i++) {
	    if(coqMode) {
		pos1 = MAXPOS; 
		position[MAXPOS]=i;
		MAXPOS = MAXPOS +1;
	    }
	    GsetArgument(t,i,norm_1((Gterm*)GgetArgument(t,i)));
	    if(coqMode) {
		MAXPOS = pos1;
	    }
	}
    } else {
      // *** TODO

	struct termac *tac=(struct termac*)t;

	//fprintf(stderr,"(");
	for(i=0 ; i<getArity(tac) ; i++) {
	    if(coqMode) {
		pos1 = MAXPOS; 
		position[MAXPOS]=i;
		MAXPOS = MAXPOS +1;
	    } 
	    setSubterm(tac,i,norm_1(getSubterm(tac,i)));
	    if (coqMode) {
		MAXPOS = pos1;
	    }
	}
	//fprintf(stderr,")");
	
	// il faut trier les sous-termes de t
	if(1) { //computeONF) {
	    struct termac *newtac = NULL;
	    int i,j;
	    // n'est plus utile
	    // TERMAC_ALLOC(newtac,getArity(tac),getSymb(tac));
	    for(i=0 ; i<getArity(tac) ; i++) {
		for(j=0 ; j<getMult(tac,i) ; j++) {
		    newtac = term_add_onf_term_color(newtac,GgetSymb(tac),getSubterm(tac,i),getColor(tac,i));
		}
	    }
	    t_tmp=(Gterm*) newtac;
	    t=t_tmp;
	}
        
    }
    t = norm_0(t);
    //fprintf(stderr,"%d %d\n",strTabSize,strCall);
    t = ((strTabFunType)strTab[(strTabSize-1)/2+strCall])(t); // str_xxx 
      //GsetReduced(t); // [Huy: Apr 30 00]

    return t;
}

/************** rightmost-innermost *****************/
Gterm * norm_3(Gterm *t) {
    int arity, i;
    int pos1;
    if(GisIntegerTagged(t) || GisIdentifierTagged(t) || GisStringTagged(t)) {
	return t;
    }
  
 
    arity = term_arity(t);
  
    if(!term_isAC(t)) {
	for(i=arity-1 ; i>=0 ; i--) {
	    pos1 = MAXPOS; 
	    position[MAXPOS]=i;
	    MAXPOS = MAXPOS +1;
	    GsetArgument(t,i,norm_3((Gterm*)GgetArgument(t,i)));
	    MAXPOS = pos1;
	}
    } else {
	// *** TODO
 // [NGUYEN: Feb 21 01] 
	struct termac *tac=(struct termac*)t;
	Gterm *nt;
	
	for(i=0 ; i<getArity(tac) ; i++) {
	    if(coqMode) {
		pos1 = MAXPOS; 
		position[MAXPOS]=i;
		MAXPOS = MAXPOS +1;
	    }
	    nt = norm_3(getSubterm(tac,i));
	    if(coqMode) {
		MAXPOS = pos1;
	    }
	    //computeONF |= (nt!=getSubterm(tac,i));
	    setSubterm(tac,i,nt);
	}
	
	// il faut trier les sous-termes de t
	if(1) { //computeONF) {
	    struct termac *newtac = NULL;
	    int i,j;
	    // n'est plus utile
	    // TERMAC_ALLOC(newtac,getArity(tac),getSymb(tac));
	    for(i=0 ; i<getArity(tac) ; i++) {
		for(j=0 ; j<getMult(tac,i) ; j++) {
		    newtac = term_add_onf_term_color(newtac,GgetSymb(tac),getSubterm(tac,i),getColor(tac,i));
		}
	    }
	    t=(Gterm*) newtac;
	}
      
    }
    t = norm_0(t);
    t = ((strTabFunType)strTab[(strTabSize-1)/2+strCall])(t); // str_xxx 
    GsetReduced(t); // [Huy: Apr 30 00] 
    return t;
}

/********* // [Huy: May  1 00] 
    leftmost-outermost
    **************************/
Gterm * norm_4(Gterm *t) {
    int arity,i;
    unsigned long r1,r2;
    int pos1;
    Gterm* t1;
    if(GisIntegerTagged(t) || GisIdentifierTagged(t) || GisStringTagged(t)) {
	return t;
    }
    if (t==T){
	while (1){ //eager au top
	    t = (Gterm*)normalise(t);                  /*normalise by unlabelled rules*/
	    r1 = rewrite_real_step;
	    t = ((strTabFunType)strTab[(strTabSize-1)/2+strCall])(t);//str_xxx
	    r2 = rewrite_real_step;
	    if (r1 == r2) break;                /*no more rule can be applied*/
	}
	T = t; // save the current top     
    } else {
	r1 = rewrite_real_step;
	t = ((strTabFunType)strTab[(strTabSize-1)/2+strCall])(t);/*one step on subterm*/
	r2 = rewrite_real_step;
	if (r1 != r2){
	    GsetReduced(t);   /*normalise par rapport aux regles non-nommes*/
	    return t;
	}
    }

    if(!term_isAC(t)) {// [NGUYEN: Feb 26 01] 
    arity = term_arity(t);
    if (arity == 0) return t;
    for(i=0 ; i<arity ; i++) {
	if (coqMode) {
	    pos1 = MAXPOS; 
	    position[MAXPOS]=i;
	    MAXPOS = MAXPOS +1;
	}
	r1 = rewrite_real_step;
	t1 = norm_4((Gterm*)GgetArgument(t,i)); //[Huy: Jun  5 02] 
	r2 = rewrite_real_step;
	if (coqMode) {
	    MAXPOS = pos1;
	}
        //printf("t1 is no initialized!\n");
        //assert(0);


	if (t1 == NULL) {
	    if (t == T) {
		return norm_4(T);
	    } else {
		return NULL;
	    }
	}
	if (r1 != r2) {
	    GsetArgument(t,i,t1);
	    if (coqMode) {
		MAXPOS = 0;
	    }
	    return NULL;           /*goto top of term*/
	}
    }
    }
    /* 2026: no subterm was rewritten (or t is AC): t is in normal form; the
       2004 code fell off the end and returned an undefined value */
    return t;
}

Gterm * norm_in(Gterm *t) {
    Gterm *res=t;
     if (globalT==NULL){
	    globalT = t;// [NGUYEN: Mar 28 01] contexte
    }
    return norm_1(res);
}

Gterm * norm_out(Gterm *t) {
    Gterm *res=t;
    Gterm * tmp;

    tmp = T;
    T = res;
    res = NULL;
    
    while (res==NULL) {
	res=norm_4(T);
    }
    
    res =(Gterm*) normalise(T);
    T = tmp;
    return res;
}



/* remove subterm s from term t which is headed by a symbol AC  */
Gterm * subterm_remove(Gterm *t, Gterm *s)
{
    int i,j,removed=0;
    Gterm *newt=NULL;

    if (!isAC(t)){
	if (s==NULL){
	    int arity = term_arity(t);
	    GmakeApplArity(newt,arity,GgetSymb(t));
	    for(i=0;i<arity;i++){
		GsetArgument(newt,i,GgetArgument(t,i));
	    }
	}
    }
    else{
	struct termac *tac=(struct termac*)t;	
	if (s==NULL || !isAC(s)){
	    for(i=0;i<getArity(tac);i++){
		for(j=0;j<getMult(tac,i);j++){
		    if (s==NULL || Gterm_cmp(getSubterm(tac,i),s)!=0 || removed){
			newt=(Gterm*)term_add_onf_term(newt,GgetSymb(tac),getSubterm(tac,i));
		    } else {
			removed=1;
		    }
		}
	    }
	} else {
	    struct termac *sac=(struct termac*)s;
	    if (GgetSymb(tac)!=GgetSymb(sac)|| (getArity(sac)==0 && getMult(sac,0)==0)){
		
		for(i=0;i<getArity(tac);i++){
		    for(j=0;j<getMult(tac,i);j++){
			if (Gterm_cmp(getSubterm(tac,i),s)!=0 || removed){
			    newt=(Gterm*)term_add_onf_term(newt,GgetSymb(tac),getSubterm(tac,i));
			} else {
			    removed=1;
			}	    
		    }
		}
	    } else { //s has the same sym with t
		for(i=0;i<getArity(sac);i++){
		    for(j=0;j<getMult(sac,i);j++){
			t=subterm_remove(t,getSubterm(sac,i));
		    }
		}
		newt=t;
	    }
	}
    }
    return newt;
}

 //[QUANG: Oct 12 01] : mise a jour 2 following functions
int getCodeByNamePrefix(char * sym_name){
  int i;


  for (i = 0; i< fsymtabSize; i++)
      if (strncmp(fsymtab[i].name,sym_name,strlen(sym_name))==0){
	  if (strlen(fsymtab[i].name)==strlen(sym_name)||
	      fsymtab[i].name[strlen(sym_name)]=='('){
	      return i;
	  }
      }
  return -1;
}
int getArityByNamePrefix(char * sym_name){
  int i;
    
  for (i = 0; i< fsymtabSize; i++)
      if (strncmp(fsymtab[i].name,sym_name,strlen(sym_name))==0){
	  if (strlen(fsymtab[i].name)==strlen(sym_name)||
	      fsymtab[i].name[strlen(sym_name)]=='('){
	      return fsymtab[i].arity;
	  }
      }
  return -1;
}

/* 2026: the generated code keeps the backup of *T_addr in an int
   (RewriteRule.java), too small for a pointer: the pointer is saved in
   T_backups (scanned by the GC) and the int is its index. Backups and
   recoveries are nested; a recovery discards the backups made after it. */
static Gterm **T_backups = NULL;
static int T_backups_size = 0;
static int T_backups_top = 0;

void trace_backup(Gterm *res,int *pt_backup, int *MAXPOS_tmp, POS position_tmp){	
    int i;

    if (T_backups_top == T_backups_size) {
	T_backups_size = T_backups_size ? 2*T_backups_size : 16;
	T_backups = (Gterm **) GC_realloc(T_backups, T_backups_size*sizeof(Gterm *));
    }
    T_backups[T_backups_top] = *T_addr;
    *pt_backup=T_backups_top++;
    //fprintf(stderr,"Backup tmp:%d T: %d\n",*T_addr,T);
    *MAXPOS_tmp=MAXPOS;
    for(i=0;i<MAXPOS;i++){
	position_tmp[i]=position[i];
    }
    if (file_query !=NULL){
	fprintf(file_query,"[");
    }
    fprintf(stderr,"[");
    first_step = 1;
    MAXPOS=0;
    globalT = res;
    return;
}

void trace_recover(int pt_backup, int MAXPOS_tmp,POS position_tmp){
    int i;

    *T_addr=T_backups[pt_backup];
    T_backups_top=pt_backup;
    //fprintf(stderr,"Recover tmp:%d T:%d\n",*T_addr,T);
    MAXPOS=MAXPOS_tmp;
    for(i=0;i<MAXPOS;i++){
	position[i]=position_tmp[i];
    }
    if (file_query !=NULL){
	fprintf(file_query,"]");
    }
    fprintf(stderr,"]");   
    first_step = 0;
    return;
}

/* print context trace for AC normalisation */
void rhoproofterm_print_AC(Gterm *pi){
    int i;	
    Gterm *t=globalT;
    Gterm * tmp_subterm;
    Gterm * tmpt;
    struct termac * tmptac;

    //    fprintf(stderr,"Term:");
    //termOut(stderr,term_unflatten(t));
    //fprintf(stderr,"\n");
    //termOut(stderr,term_unflatten(pi));
    //fprintf(stderr,"\n");

    tmpt = t;
    if (!first_step){  //[NGUYEN: May 31 01] if not first_step then do not print ;
	if (file_query !=NULL){
	    fprintf(file_query,";");
	}
	fprintf(stderr,";");
    }

    for (i=0;i<MAXPOS-1;i++){
	if (isAC(tmpt)){
	    tmptac=(struct termac *)tmpt;
	    tmpt=getSubterm(tmptac,position[i]);
	} else {
	    tmpt=GgetArgument(tmpt,position[i]);
	}
    }

    if (isAC(tmpt)){
	tmptac=(struct termac *)tmpt;
	if (MAXPOS>=1){
	    tmp_subterm = getSubterm(tmptac,position[MAXPOS-1]);
	    setSubterm(tmptac,position[MAXPOS-1],pi);
	    if (file_query !=NULL){
		termOut(file_query,term_unflatten(t)); /* print term */
	    }
	    termOut(stderr,term_unflatten(t)); /* print term */
	    //fprintf(stderr,"\n");
	    setSubterm(tmptac,position[MAXPOS-1],tmp_subterm);
	} else {
	    if (file_query !=NULL){
		termOut(file_query,term_unflatten(pi)); /* print term */
	    }
	    termOut(stderr,term_unflatten(pi)); /* print term */
	    fprintf(stderr,"\n");
	}
    } else {
	if (MAXPOS>=1){
	    tmp_subterm = GgetArgument(tmpt,position[MAXPOS-1]);
	    GsetArgument(tmpt,position[MAXPOS-1],pi);
	    if (file_query !=NULL){
		termOut(file_query,term_unflatten(t)); /* print term */
	    }
	    termOut(stderr,term_unflatten(t)); /* print term */
	    //fprintf(stderr,"\n");
	    GsetArgument(tmpt,position[MAXPOS-1],tmp_subterm);
	} else {
	    if (file_query !=NULL){
		termOut(file_query,term_unflatten(pi)); /* print term */
	    }
	    termOut(stderr,term_unflatten(pi)); /* print term */
	    //fprintf(stderr,"\n");
	}
    }
    first_step=0;
    return;
}


/* print context trace for syntactic normalisation */
void rhoproofterm_print(Gterm *pi){
    int i;	
    Gterm *t=globalT;
    Gterm * tmp_subterm;
    Gterm * tmpt;

    //fprintf(stderr,"\nTerm:");
    //termOut(stderr,term_unflatten(t));
    //fprintf(stderr,"\n");
    //term_println(stderr,t);
    //term_println(stderr,pi);
    //termOut(stderr,term_unflatten(pi));
    //fprintf(stderr,"\n");

    tmpt = t;
    if (!first_step){
	if (file_query !=NULL){
	    fprintf(file_query,";");
	}
	fprintf(stderr,";");
    }
    for (i=0;i<MAXPOS-1;i++){
	tmpt=GgetArgument(tmpt,position[i]);
    }

    if (MAXPOS>=1){
	tmp_subterm = GgetArgument(tmpt,position[MAXPOS-1]);
	GsetArgument(tmpt,position[MAXPOS-1],pi);
	if (file_query !=NULL){
          termOut(file_query,term_unflatten(t)); /* print term */
	}
	termOut(stderr,term_unflatten(t)); /* print term */
	//fprintf(stderr,"\n");
	GsetArgument(tmpt,position[MAXPOS-1],tmp_subterm);
	
    } else {
	if (file_query !=NULL){
          termOut(file_query,term_unflatten(pi)); /* print term */
	}
	termOut(stderr,term_unflatten(pi)); /* print term */
	//fprintf(stderr,"\n");
    }
    first_step = 0;
    return;
}

/*  QUANG: Sep 19 01] for extension rule #rule is always printed before any other symbols */
void extension_rule_prec(char * sym_name)
{
 int i;
    
  for (i = 0; i< fsymtabSize; i++)
      if (strncmp(fsymtab[i].name,sym_name,strlen(sym_name))==0) {
	  fsymtab[i].prec = 50;
      }
  
  return;
}
