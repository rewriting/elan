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
#include "builtinMatching.h"

/*
 * builtin de filtrage syntaxique
 * retourne le temre `fail' s'il n'y a pas de match
 * retourne le terme `nil' sil les deux termes sont identiques
 * retourne un terme (liste de termes) : listes d'instances s'il y a une solution
 */

#define MAX_SYNTACTICMATCHING_VAR 100
static Gterm *term_buildResult(int n, Gterm **tabRes,
				     Gterm *listVar);
static int term_storeVariable(Gterm *t, int n, Gterm **tabVar,
			      Gterm **ptr_nil);
static int term_indexVariable(Gterm *t, int nbVar, Gterm **tabVar);
static long term_equalMatch(Gterm *t1, Gterm *t2,
			    int nbVar, Gterm **tabVar,
			    Gterm **tabRes);



/*
 * stocke les variables dans tabRes
 * recupere la constante nil
 * retourne le nombre de variables
 */
static int term_storeVariable(Gterm *listVar,
			      int n,
			      Gterm **tabVar,
			      Gterm **ptr_nil) {
  int a=term_arity(listVar);
  if(a==0) {
    *ptr_nil=listVar;
    return n;
  } else if(a==2) {
    if(n>MAX_SYNTACTICMATCHING_VAR-1) {
      fprintf(stderr,"term_storeVariable: increase MAX_SYNTACTICMATCHING_VAR=%d\n",MAX_SYNTACTICMATCHING_VAR); 
      exit(1); 
    }
    tabVar[n]=(Gterm*)GgetArgument(listVar,0);
    return term_storeVariable((Gterm*)GgetArgument(listVar,1),n+1,tabVar,ptr_nil);
  } else {
    fprintf(stderr,"term_storeVariable: listVar term is not correct\n");
    term_printnl(stderr,listVar);
    exit(1);
  }
}

static Gterm *term_buildResult(int n, Gterm **tabRes,
				     Gterm *listVar) {
  
  if(term_arity(listVar)==0) {
    return listVar;
  } else {
    Gterm *res;
    GmakeAppl2(res,GgetSymb(listVar),tabRes[n],term_buildResult(n+1,tabRes,(Gterm*)GgetArgument(listVar,1)));
    /*TERM_ALLOC(res,term2,getSymb(listVar));
    GsetArgument(res,0,tabRes[n]);
    GsetArgument(res,1,term_buildResult(n+1,tabRes,GgetArgument(listVar,1)));*/
    return res;
  }
}


Gterm *term_syntacticMatching(Gterm *pattern,
				    Gterm *subject,
				    Gterm *listVar,
				    Gterm *fail) {
  Gterm *tabVar[MAX_SYNTACTICMATCHING_VAR];
  Gterm *tabRes[MAX_SYNTACTICMATCHING_VAR];

  Gterm *nil=NULL;
  Gterm *res;
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
static int term_indexVariable(Gterm *t, int nbVar, Gterm **tabVar) {
  int i;
  for(i=0 ; i<nbVar ; i++) {
    if(term_notDestructEqual(t,tabVar[i])) {
      return i;
    }
  }
  return -1;
}

static long term_equalMatch(Gterm *t1, Gterm *t2,
			    int nbVar,
			    Gterm **tabVar,
			    Gterm **tabRes) {
  int i,arity;
  int indexVar;
  /*
  printf("t1 = "); term_printnl(stdout,t1);
  printf("t2 = "); term_printnl(stdout,t2);
  */
  if(t1==t2) return(1);
  //if(isTagged(t1) || isTagged(t2)) return t1==t2;
  if(GisIntegerTagged(t1) || GisIntegerTagged(t2) ||
     GisIdentifierTagged(t1) || GisIdentifierTagged(t2) ) {
    return t1==t2;
  } else if(GisStringTagged(t1) || GisStringTagged(t2)) {
    if(GisStringTagged(t1) && GisStringTagged(t2)) {
      return strcmp(GgetString(t1),GgetString(t2));
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
   
  if(GgetSymb(t1) != GgetSymb(t2)) return (0);
  arity=term_arity(t1);
  if (arity==0) return(1);
  for(i=0 ; i<arity ; i++) {

    if(!term_equalMatch((Gterm*)GgetArgument(t1,i),(Gterm*) GgetArgument(t2,i),nbVar,tabVar,tabRes)) {
      return (0);
    }
  }
  return (1);
}

