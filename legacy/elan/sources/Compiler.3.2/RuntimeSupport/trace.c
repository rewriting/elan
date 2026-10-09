#include "term.h"
#include "tools.h"
#include "trace.h"
#include "termIn.h"
#include "builtin.h"

#ifdef __cplusplus
typedef struct term* (*funTabType)(...);
#else
typedef struct term* (*funTabType)();
#endif
extern funTabType strTab[];
extern int earleyQueryStrategy;
extern int rewrite_real_step;
extern struct term * T;
extern int coqMode;


POS position;     // suite de position
int MAXPOS;       // lg de la suite de position
struct NODE *racine;      // racine de l'arbre de position
struct TR_COQ *head_tr;    // debut de la liste da trace
struct TR_COQ *tail_tr;    // fin de la liste de trace
RNAME rname;
unsigned int nb_node;
struct term * T;
int strCall;


/******* input from text files ************************/
extern int earleyPos ;

extern struct term *computeTerm(int *ppos);

struct term *coqEarleyParser(char *querySortName) {
    int e ;
    struct term *query;
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
    return(query) ;
}

/*
 * parser for the following grammar (3 cases)
 * ()
 * ( id )
 * ( id : id )
 */
/*  [Huy: May 14 00]  changement pour pouvoir prendre input d'un fichier texte*/

int coqprefixParser(char *s1, char *s2) {
    char c;
    FILE *fp_query_sort ;  //ce qui peut remplacer le stdin

    if (!coqMode)
	fprintf(stderr,"Enter a query of the form: ([[<strat>:]<sort>]) <term> end\n");

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
		printf("':' expected\n",c);
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
int initialise_trace()
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

void setTermNoReduced(struct term *t){
    int code, arity, i;

    if(isIntegerTagged(t) || isIdentifierTagged(t) || isStringTagged(t)) {
	return;
    }


  arity = term_arity(t);
  for(i=0 ; i<arity ; i++) {
    setTermNoReduced(getFreeSubterm(t,i));
  }
  clearReduced(t);
}

int trace_pretty_print(TR_COQ * head,int deep, FILE * fp_trace)
{
    TR_COQ *tr;
    NODE *node;
    char tmp[5],rp[2];
    char output[1000],output1[1000];
    int i,ret,j;

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
			    sprintf(tmp,"%d",node->ps);
			}
		    else
			{
			    sprintf(tmp,";%d",node->ps);
			}
		    sprintf(output1,"%s%s",tmp,output);
		    strcpy(output,output1);
		    node = node->father;
		}
	    for (j=1; j<=deep; j++) fprintf(fp_trace," ");
	    fprintf(fp_trace, "%s @ [%s]\n",tr->rname,output);
	    if (tr->cond_node->leftside != NULL)
		trace_pretty_print(tr->cond_node->leftside,deep+2,fp_trace);
	    if (tr->cond_node->rightside != NULL) 
		trace_pretty_print(tr->cond_node->rightside,deep+2,fp_trace);
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
	    FREE(tr);   
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
    if (snode == NULL) { FREE(node); nb_node= nb_node+1;}
    else
	{
	    while(snode != NULL)
		{
		    snode1 = snode->next;
		    tree_free(snode);
		    snode = snode1;
		}
	    FREE(node); nb_node = nb_node +1;
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
struct term * norm_0(struct term *t) {
    int code, arity, i;
    struct cell_term *p;
    int pos1;

    if (t == NULL) return NULL;
    if(isIntegerTagged(t) || isIdentifierTagged(t) || isStringTagged(t)) {
	return t;
    }

    if (isReduced(t)) return t;
    code  = getSymb(t);
    arity = term_arity(t);
    for(i=0 ; i<arity ; i++) {
	setFreeSubterm(t,i,norm_0(getFreeSubterm(t,i)));
    }
    
    t = specialApply(t);
    return t;
}

/****************
      leftmost-innermost
      **********************/

struct term * norm_1(struct term *t) {
    int code, arity, i;
    struct cell_term *p;
    int pos1;


    if(isIntegerTagged(t) || isIdentifierTagged(t) || isStringTagged(t)) {
	return t;
    }
    if(isReduced(t)) {
	return t; // [Huy: Apr 30 00]
    }
    code  = getSymb(t);
    arity = term_arity(t);
  
    if(!term_isAC(t)) {
	for(i=0 ; i<arity ; i++) {
	    if(coqMode) {
		pos1 = MAXPOS; 
		position[MAXPOS]=i;
		MAXPOS = MAXPOS +1;
	    }
	    setFreeSubterm(t,i,norm_1(getFreeSubterm(t,i)));
	    if(coqMode) {
		MAXPOS = pos1;
	    }
	}
    } else {
      // *** TODO
    }
    t = norm_0(t);
    t = strTab[(strTabSize-1)/2+strCall](t); // str_xxx 
    setReduced(t); // [Huy: Apr 30 00] 

    return t;
}

/************** rightmost-innermost *****************/
struct term * norm_3(struct term *t) {
    int code, arity, i;
    struct cell_term *p;
    int pos1;

    if(isIntegerTagged(t) || isIdentifierTagged(t) || isStringTagged(t)) {
	return t;
    }
  
 
    code  = getSymb(t);
    arity = term_arity(t);
  
    if(!term_isAC(t)) {
	for(i=arity-1 ; i>=0 ; i--) {
	    pos1 = MAXPOS; 
	    position[MAXPOS]=i;
	    MAXPOS = MAXPOS +1;
	    setFreeSubterm(t,i,norm_3(getFreeSubterm(t,i)));
	    MAXPOS = pos1;
	}
    } else {
	// *** TODO
    }

    t = strTab[strTabSize/2+strCall](t);
    return t;
}

/********* // [Huy: May  1 00] 
    leftmost-outermost
    **************************/
struct term * norm_4(struct term *t) {
    int arity,i;
    unsigned long r1,r2;
    struct cell_term *p;
    int pos1;
    struct term * t1;

    if(isIntegerTagged(t) || isIdentifierTagged(t) || isStringTagged(t)) {
	return t;
    }
    if (t==T){
	while (1){ //eager au top
	    t = normalise(t);                  /*normalise by unlabelled rules*/
	    r1 = rewrite_real_step;
	    t = strTab[(strTabSize-1)/2+strCall](t);//str_xxx
	    r2 = rewrite_real_step;
	    printf("");
	    if (r1 == r2) break;                /*no more rule can be applied*/
	}
	T = t; // save the current top     
    } else {
	r1 = rewrite_real_step;
	t = strTab[(strTabSize-1)/2+strCall](t);/*one step on subterm*/
	r2 = rewrite_real_step;
	if (r1 != r2){
	    setReduced(t);   /*normalise par rapport aux regles non-nommes*/
	    return t;
	}
    }

    arity = term_arity(t);
    if (arity == 0) return t;
    
    for(i=0 ; i<arity ; i++) {
	if (coqMode) {
	    pos1 = MAXPOS; 
	    position[MAXPOS]=i;
	    MAXPOS = MAXPOS +1;
	}
	r1 = rewrite_real_step;
	t1 = norm_4(getFreeSubterm(t,i));
	r2 = rewrite_real_step;
	if (coqMode) {
	    MAXPOS = pos1;
	}
	if (t1 == NULL) {
	    if (t == T) {
		return norm_4(T);
	    } else {
		return NULL;
	    }
	}
	if (r1 != r2) {
	    setFreeSubterm(t,i,t1);
	    if (coqMode) {
		MAXPOS = 0;
	    }
	    return NULL;           /*goto top of term*/
	}
    }
}

struct term * norm_in(struct term *t) {
    struct term *res=t;
    return norm_1(res);
}

struct term * norm_out(struct term *t) {
    struct term *res=t;
    struct term * tmp;

    tmp = T;
    T = res;
    res = NULL;
    
    while (res==NULL) {
	res=norm_4(T);
    }
    
    res = normalise(T);
    T = tmp;
    return res;
}
