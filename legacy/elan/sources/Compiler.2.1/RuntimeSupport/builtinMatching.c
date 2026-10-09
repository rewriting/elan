#include "builtinMatching.h"
#include "term.h"
#include "builtin.h"

/*
 * builtin de filtrage syntaxique
 * retourne le temre `fail' s'il n'y a pas de match
 * retourne le terme `nil' sil les deux termes sont identiques
 * retourne un terme (liste de termes) : listes d'instances s'il y a une solution
 */

#define MAX_SYNTACTICMATCHING_VAR 100
static struct term *term_buildResult(int n, struct term **tabRes,
				     struct term *listVar);
static int term_storeVariable(struct term *t, int n, struct term **tabVar,
			      struct term **ptr_nil);
static int term_indexVariable(struct term *t, int nbVar, struct term **tabVar);
static long term_equalMatch(struct term *t1, struct term *t2,
			    int nbVar, struct term **tabVar,
			    struct term **tabRes);



/*
 * stocke les variables dans tabRes
 * recupere la constante nil
 * retourne le nombre de variables
 */
static int term_storeVariable(struct term *listVar,
			      int n,
			      struct term **tabVar,
			      struct term **ptr_nil) {
  int a=term_arity(listVar);
  if(a==0) {
    *ptr_nil=listVar;
    return n;
  } else if(a==2) {
    if(n>MAX_SYNTACTICMATCHING_VAR-1) {
      fprintf(stderr,"term_storeVariable: increase MAX_SYNTACTICMATCHING_VAR=%d\n",MAX_SYNTACTICMATCHING_VAR); 
      exit(1); 
    }
    tabVar[n]=listVar->sub[0];
    return term_storeVariable(listVar->sub[1],n+1,tabVar,ptr_nil);
  } else {
    fprintf(stderr,"term_storeVariable: listVar term is not correct\n");
    term_printnl(stderr,listVar);
    exit(1);
  }
}

static struct term *term_buildResult(int n, struct term **tabRes,
				     struct term *listVar) {
  
  if(term_arity(listVar)==0) {
    setShared(listVar);
    return listVar;
  } else {
    struct term *res;
    TERM_ALLOC(res,term2,getSymb(listVar));
    res->sub[0]=tabRes[n];
    setShared(res->sub[0]);
    res->sub[1]=term_buildResult(n+1,tabRes,listVar->sub[1]);
    return res;
  }
}


struct term *term_syntacticMatching(struct term *pattern,
				    struct term *subject,
				    struct term *listVar,
				    struct term *fail) {
  struct term *tabVar[MAX_SYNTACTICMATCHING_VAR];
  struct term *tabRes[MAX_SYNTACTICMATCHING_VAR];

  struct term *nil=NULL;
  struct term *res;
  int i,nbVar=0;

  Verif_void(pattern,"term_term_match(pattern)");
  Verif_void(subject,"term_term_match(subject)");
  Verif_void(listVar,"term_term_match(listVar)");
  Verif_void(fail,"term_term_match(fail)");

  /*  
  printf("term_syntacticMatching\n");
  printf("pattern = "); term_printnl(stdout,pattern);
  printf("subject = "); term_printnl(stdout,subject);
  printf("listVar = "); term_printnl(stdout,listVar);
  */

  nbVar=term_storeVariable(listVar,0,tabVar,&nil);

  /*
  printf("nbVar=%d\n",nbVar);
  printf("nil="); term_printnl(stdout,nil);
  for(i=0; i<nbVar ; i++) {
    printf("tabVar[%d] = ",i); term_printnl(stdout,tabVar[i]);
  }
  */

  for(i=0; i<nbVar ; i++) {
    tabRes[i]=NULL;
  }
  
  if(term_equalMatch(pattern,subject,nbVar,tabVar,tabRes)) {
    /* construction du terme resultat */
    /*
    printf("One solution found\n");
    for(i=0; i<nbVar ; i++) {
	 printf("tabRes[%d] = ",i); term_printnl(stdout,tabRes[i]);
    }
    */
    res = term_buildResult(0,tabRes,listVar);
  } else {
    /* no solution */
    /*
    printf("No solution\n");
    */
    res = fail;
  }
  /*
  printf("result = "); term_printnl(stdout,res);
  */
  return res;
}

/*
 * Recherche de l'indice d'une variable dans le tableau tabVar
 */
static int term_indexVariable(struct term *t, int nbVar, struct term **tabVar) {
  int i;
  for(i=0 ; i<nbVar ; i++) {
    if(term_notDestructEqual(t,tabVar[i])) {
      return i;
    }
  }
  return -1;
}

static long term_equalMatch(struct term *t1, struct term *t2,
			    int nbVar,
			    struct term **tabVar,
			    struct term **tabRes) {
  int i,arity;
  int indexVar;
  /*
  printf("t1 = "); term_printnl(stdout,t1);
  printf("t2 = "); term_printnl(stdout,t2);
  */
  if(t1==t2) return(1);
  //if(isTagged(t1) || isTagged(t2)) return t1==t2;
  if(isIntegerTagged(t1) || isIntegerTagged(t2) ||
     isIdentifierTagged(t1) || isIdentifierTagged(t2) ) {
    return t1==t2;
  } else if(isStringTagged(t1) || isStringTagged(t2)) {
    if(isStringTagged(t1) && isStringTagged(t2)) {
      return strcmp(getString(t1),getString(t2));
    } else {
      printf("error in term_equalMatch\n");
      exit(1);
    }
  } 

  if(term_isAC(t1) || term_isAC(t2)) return(0);

  indexVar=term_indexVariable(t1,nbVar,tabVar);
  /*
  printf("indexVar=%d\n",indexVar);
  */
  if(indexVar>=0) {
    if(tabRes[indexVar]==NULL) {
      tabRes[indexVar]=t2;
      return(1);
    } else {
      return term_notDestructEqual(tabRes[indexVar],t2);
    }
  }
   
  if(getSymb(t1) != getSymb(t2)) return (0);
  arity=term_arity(t1);
  if (arity==0) return(1);
  for(i=0 ; i<arity ; i++) {
    if(!term_equalMatch(t1->sub[i], t2->sub[i],nbVar,tabVar,tabRes)) {
      return (0);
    }
  }
  return (1);
}

