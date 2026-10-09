#include "termCommon.h"


static void print_f_prefix(FILE *fich,Gterm *t);
/*
 * output.c
 */
Gterm *term_unflatten(Gterm *t);


void internal_term_print(FILE *fich,Gterm *t, int mode) {
  Verif_void(t,"internal_term_print(fich,t)");
  if(mode==ELAN_IO) {
      //term_println(fich,t);
      //printf("\nin internal_term_println arity de t = %d\n",term_arity(t));
    
      //printf("------------------------------------------------------------\n");
      //printf("  t = ");  term_println(stdout,t);
      //printf("u t = ");  term_println(stdout,term_unflatten(t));
      //printf("------------------------------------------------------------\n");
    
    termOut(fich,term_unflatten(t));
  } else {
    term_print(fich,t);
  }
}

void internal_term_println(FILE *fich,Gterm *t, int mode) {
  internal_term_print(fich,t,mode);
  fprintf(fich,"\n");
}


void term_print(FILE *fich,Gterm *t)
{
  Verif_void(t,"term_print(fich,t)");
  print_f_prefix(fich,t);
}

/*
 * [hassen: Feb 26 01]
 * a voir ici
 */
static void print_f_prefix(FILE *fich,Gterm *t) {
  if(GisIntegerTagged(t)) {
      // fprintf(fich,"integer\n");
    fprintf(fich,"%d",GgetInt(t));
    return;
  } else if(GisIdentifierTagged(t)) {
      //fprintf(fich,"identifier\n");
    fprintf(fich,"ident(%d)",GgetIdentifier(t));
      //fprintf(fich,"%d",GgetIdentifier(t));
    return;
  } else if(GisStringTagged(t)) {
      //fprintf(fich,"\"%s\"",term_getString(t));
    fprintf(fich,"%s",term_getString(t));
    return;
  } else if(GisArrayTagged(t)) {
    printf("print_f_prefix Array not yet implemented\n");
    exit(1);
  }

    // fprintf(fich,"symbol : %d\t%d\n",t,GgetSymb(t));

    //printf("\nGgetSymb(%x) = %d\n",0,GgetSymb(t));
    //printf("\nterm_name(%d) = %d\n",t,term_name(t));
  fprintf(fich,"%s",term_name(t));

  if(term_isAC(t)) {
    struct termac *tac=(struct termac*)t;
    fprintf(fich,"*");
    if(getArity(tac) > 0 ) {
      int i;
        //if(isReduced(t)) fprintf(fich,"[r]");
      fprintf(fich, "(");

      for(i=0 ; i<getArity(tac) ; i++) {
           // print with multiplicity
	if(getMult(tac,i)>1) {
	  int j;
	  fprintf(fich,"[[");
	  for(j=0 ; j<getMult(tac,i) ; j++) {

              //printf("\ngetSubterm(%d,%d) = %d\n",tac,i,getSubterm(tac,i));

            term_print(fich,getSubterm(tac,i));
	    if(j!=getMult(tac,i)-1) {
	      fprintf(fich,",");
	    }
	  }
	  fprintf(fich,"]]");
	} else {
	  term_print(fich,(Gterm *) getSubterm(tac,i));
	}
#ifdef COLOR
//        fprintf(fich,"{%d}",getColor(tac,i));
#endif
	if(i<getArity(tac)-1) {
	  fprintf(fich,",");
	}
      }

      fprintf(fich,")");
    }
  } else {
    int i;
    int arity=term_arity(t);
    if(arity>0) {
      fprintf(fich, "(");
      for(i=0 ; i<arity ; i++) {
        term_print(fich,(Gterm *) GgetArgument(t,i));
        if(i!=arity-1)
          fprintf(fich,",");
      }
      fprintf(fich,")");
    }
  }
}

/*
 * output.c
 */
void term_printREFln(FILE *fich,Gterm *t)
{
  Verif_void(t,"term_printlnREF(fich,t)");
  term_printREF(fich,t);
  fprintf(fich,"\n");
}

void term_printREF(FILE *fich,Gterm *t)
{
  int i;
  Verif_void(t,"term_printREF(fich,t)");
  if(GisIntegerTagged(t)) {
    fprintf(fich,"INT(%d)",GgetInt(t));
    return;
  } else if(GisIdentifierTagged(t)) {
    fprintf(fich,"IDENT(%d)",GgetIdentifier(t));
    return;
  }
    // PROBLEME SUR SUN
  else if(GisStringTagged(t)) {
      // A MODIFIER !!!
    fprintf(fich,"STRING(%s)",GgetString(t));
    return;
  }

  if(term_isAC(t)) {
    struct termac *tac=(struct termac*)t;
    if(getArity(tac)>0) {
      int i,j;
      for(i=0 ; i<getArity(tac) ; i++) {
	for(j=0 ; j<getMult(tac,i) ; j++) {
          if( !((i==getArity(tac)-1) && (j==getMult(tac,i)-1))) {
            fprintf(fich,"FSYM(");
          }

          term_printREF(fich,(Gterm *) getSubterm(tac,i));
          if( !((i==getArity(tac)-1) && (j==getMult(tac,i)-1))) {
            fprintf(fich,".");
          }
        }
      }
      for(i=0 ; i<getArity(tac) ; i++) {
	for(j=0 ; j<getMult(tac,i) ; j++) {
          if( !((i==getArity(tac)-1) && (j==getMult(tac,i)-1))) {
            fprintf(fich,".nil,%d)",GgetSymb((Gterm *)tac));
          }
        }
      }
    } else {
      fprintf(stderr,"term_printREF: error in AC term\n");
      exit(1);
    }
  } else {
    int i;
    int arity=term_arity(t);
    fprintf(fich,"FSYM(");
    if(arity>0) {
      for(i=0 ; i<arity ; i++) {

	term_printREF(fich,(Gterm *) GgetArgument(t,i));
	fprintf(fich,".");
      }
    }
    fprintf(fich,"nil,");
    fprintf(fich,"%d)",GgetSymb(t));
  }

}

/*HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH*/
/*HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH*/
 // ehm modification a faire
static struct termac *merge_sorted_term(struct termac *tac1,
                                        struct termac *tac2);
static struct termac *termac_insert_bubble(struct termac *tac, int pos);

static int termac_lookup(Gterm *subterm,
                         struct termac *tac,
                         int low, int high, int *indice);
#ifdef CC_GC
// ehm modification
void finalize(Gterm *obj,int cd) {
  struct termac *tac=(struct termac*)obj;
    /*
      if(obj != (Gterm*)CC_GC_decode((struct termac*)tac->subterm[2*getSize(tac)])) {
      printf("***** WARNING *****\n");
      printf("finalization\n");
      printf("obj [%u] = ",obj); term_println(stdout,obj);
      printf("cd  = %d\n",cd);
      printf("backRef = [%u]\n",CC_GC_decode((struct termac*)tac->subterm[2*getSize(tac)]));
      }
    */
    /* mise a NULL du backRef */
//  tac->subterm[2*getSize(tac)]=NULL;
}
#endif
#define GinitMinimalSize(s) ((1+((s)>>2))<<2)
void termac_alloc(struct termac **ptr_dest, int size, unsigned int funsym) {
  struct termac *dest;
  // ehm temporaire
  unsigned long cptTermacAlloc=0;
  size = GinitMinimalSize(size);
//  if(size<8) size=8;
//  if(size<4) size=4;

  *ptr_dest=(struct termac*) MALLOC(sizeof(struct termac));
  dest=*ptr_dest;
  GsetSymbAC(dest,funsym);
  setSizeArity(dest,size,0);
  GsetAC(dest);
    //
    // si l'allocation est faite au moment de l'insertion du premier element
    // c'est localise dans term.c et ac_tool.c
    //
#ifdef CC_GC
  (dest->subterm)=(Gterm**)MALLOC(((1+size)*sizeof(Gterm*))<<1);
  dest->subterm[2*getSize(dest)]=(Gterm*)CC_GC_encode(dest);
  GC_register_finalizer(dest,finalize,1,0,0);
#else
  (dest->subterm)=(Gterm**) MALLOC(((size*sizeof(Gterm*))<<1));
#endif

#ifdef DEBUG
  cptTermacAlloc++;
#endif
}
  // ehm modification a fiare
#ifdef NOTMACRO

void termac_add_lastColor(struct termac *tac, Gterm *subterm, int mult,int color) {
    /*
      if(getArity(tac) == 0) {
      int size = initMinimalSize(getSize(tac));
      (tac->subterm)=(Gterm**) MALLOC(2*size*sizeof(Gterm*));
        // printf("termac_add_last: init size = %d\n",size);
        } else
    */
  int arity = getArity(tac);
  if(arity == getSize(tac)) {
    //printf("\n I'm in termCommon.c line 251\n");
    termac_resize(tac,2*arity);
  }
  //printf("\n I'm in termCommon.c line 254\n");
  setColorMult(tac,arity,color,mult);
  setSubterm(tac,arity,subterm);
  //printf("\n in termac_add_lastColor tac ="); term_print(stdout,tac);printf("\n");
  setArity(tac,arity+1);

#ifdef UPDATE_HCODE
  //printf("\n I'm in termCommon.c line 260\n");
  setHcode(tac,INTERN_HFUNCTION(tac,subterm));
/*
    // calcul du hcode : a enlever par la suite
  {
    int i;
    setHcode(tac,GgetSymb(tac));
    for(i=0 ; i<getArity(tac) ; i++) {
      setHcode(tac,HFUNCTION(tac,(Gterm *) getSubterm(tac,i)));
    }
      //printf("add_last tac[hcode = %d] = ",getHcode(tac));
      //term_println(stdout,((Gterm*)tac));
  }
*/
#endif

}
#endif

   // ehm modification
void termac_resize(struct termac *tac, int size) {
  if(size > getSize(tac)) {
#ifdef CC_GC
    Gterm **newSubterm = (Gterm**) MALLOC(((1+size)*sizeof(Gterm*))<<1);
#else
    Gterm **newSubterm = (Gterm**) MALLOC((size*sizeof(Gterm*))<<1);
#endif
    int i;
#ifdef AFFICHAGE
    printf("termac_resize from %d to %d (%d)\n",getSize(tac),size,getArity(tac));
#endif

    for(i=0 ; i<2*getArity(tac) ; i++) {
      newSubterm[i]=tac->subterm[i];
    }
    setSize(tac,size);
    tac->subterm = newSubterm;
#ifdef CC_GC
    tac->subterm[2*size]=(Gterm*)CC_GC_encode(tac);
#endif
  }
}

#ifdef NOTMACRO
int term_isAC(Gterm *t) {
  Verif_void(t,"term_isAC(t)");

  //return 0;
    // TO BE IMPLEMENTED
    return isAC(t);
}
#endif

static Gterm *subterm_unflatten(unsigned int funsym,
                                      struct termac *tac,
                                      int i,int multiplicity) {
  Gterm *res;
  Gterm *tmp1, *tmp2;
  if(i==getArity(tac)-1 && multiplicity<=1) {
      /* si c'est le dernier element de la liste */
      //printf("\nin subterm_unflatten arity de t = %d\n",fsymtab[322].arity);
    return term_unflatten(getSubterm(tac,i));
  } else {
      /* TODO : corriger si funsym est AC */
      //printf("\nin subterm_unflatten arity de t = %d\n",fsymtab[322].arity);
    //printf("\nin termCommon.c line 326\n");
      //GmakeApplArity(res,2,funsym);
      //GsetArgument(res,0,term_unflatten(getSubterm(tac,i)));

    tmp1 = term_unflatten(getSubterm(tac,i));
    
    //printf("\nin termCommon.c line 328 res = ");term_print(stdout,res);printf("\n");
    multiplicity--;
    if(multiplicity<=0) {
      i++;
      tmp2 = subterm_unflatten(funsym,tac,i,getMult(tac,i));
        //GsetArgument(res,1,tmp2);
    } else {
      tmp2 = subterm_unflatten(funsym,tac,i,multiplicity);
        //GsetArgument(res,1,tmp2);
    }

    GmakeAppl2(res, funsym, tmp1, tmp2);
    
    return res;
  }
}

Gterm *term_unflatten(Gterm *t) {
    //printf("term_unflatten: "); term_println(stdout,t);
  // ehm a modifier
  Gterm* TabArgument[100],*t1,*ArrayArgs[256];
  //printf("\n in termCommon.c line 342 \n");
  if(GisTagged(t)) {
    return t;
  }

  if(term_isAC(t)) {
    struct termac *tac=(struct termac*)t;
    //printf("\n***************** in termAC\n");
    // printf("\n in termCommon.c line 355 return =\n");term_print(stdout,subterm_unflatten(GgetSymb(tac),tac,0,getMult(tac,0)));printf("\n");
    return subterm_unflatten(GgetSymbAC(tac),tac,0,getMult(tac,0));
  } else {
    Gterm *res = t;
    int i, arity;
    //printf("\n in termCommon.c line 355 \n");
    arity=term_arity(t);
    //printf("\n in termCommon.c line 357 arity = %d\n",arity);
    // printf("\nterm_unflatten: arity de t = [%d,%d]\n",arity, fsymtab[322].arity);
    //printf("\nt = %d\n",GgetSymb(t));
    
    if(arity>0) {
      /*  //printf("\nterm_unflatten:arity = %d\n",arity);
      //printf("\n in termCommon.c line 363 \n");  	
      for(i=0 ; i<arity ; i++) {
          //printf("i = %d ",i);
        ArrayArgs[i]=(Gterm *) GgetArgument(t,i) ;
      }
      GmakeApplArray(res,GgetSymb(t),ArrayArgs);*/

	//printf("\n***************** in arity>0\n");
      if(arity==1) {
        //ATprintf("\n***************** in arity=1\n");
        GmakeAppl1(res,GgetSymb(t),con_0);
        //ATprintf("\n***************** out arity=1 res = %t\n",res);
      } if(arity==2) {
        //printf("\n***************** in arity=2\n");
        GmakeAppl2(res,GgetSymb(t),con_0,con_0);
      } else {
        GmakeApplArity(res,arity,GgetSymb(t));
      }
      for(i=0 ; i<arity ; i++) {
        GsetArgument(res,i,term_unflatten(GgetArgument(t,i)));
      }
      //printf("\n in termCommon.c line 390 return =\n");term_print(stdout,res);printf("\n");
      return res;
    } else {
	//printf("\n in termCommon.c line 393 return =\n");term_print(stdout,t);printf("\n");
      return t;
    }
  }
}

/*
 ************************************************************
 *
 * flatten
 *
 ************************************************************
 */

/*
 * t and subterm are two onf terms
 */

//#define AFFICHAGE

#ifdef COLOR
struct termac *intern_term_add_onf_term(int isAC,
                                        struct termac *tac,
                                        unsigned int fsym,
                                        Gterm *subterm,
                                        int color)
#else
struct termac *intern_term_add_onf_term(int isAC,
                                        struct termac *tac,
                                        unsigned int fsym,
                                        Gterm *subterm)
#endif
{
  struct termac *res;
  int i;
  int found;
  int isNullTac =(tac==NULL);
  struct termac *subtermac=(struct termac*)subterm; 
#ifdef AFFICHAGE
  printf("term_add_onf_term(");
  if(tac!=NULL) term_print(stdout,(Gterm*)tac);
  printf(",");
  term_print(stdout,subterm);
  printf(")\n");
#endif

  // Ajout d un sous-terme vide
  if(term_isAC(subterm) && getArity(((struct termac *)subterm))==0) {
    //printf("\n in intern_term_add_onf_term I'm line 426 \n");
    if(isNullTac) {
         //printf("\nin intern_term_add_onf_term I'm line 428 \n");
        /*
         * ne rien faire et retourner NULL
         */
        //TERMAC_ALLOC(tac,2,fsym);
    }
    res = tac;
    goto fin;
  }
  if(fsym == GgetSymb(subterm)) {
    //printf("\nin intern_term_add_onf_term I'm line 438 \n");
      // il faut applatir et faire un merge list
    if(isNullTac) {
    //printf("\nin intern_term_add_onf_term I'm line 441 \n");
      TERMAC_ALLOC(tac,getArity(subtermac),fsym);
        //printf("termac_alloc(%d)\n",getArity(subtermac));
    }

    if(getArity(tac)==0) {
    //printf("\nin intern_term_add_onf_term I'm line 447 \n");
        // cas d un terme vide
      termac_copyTopSymbol(tac,subtermac);
      res=tac;
      goto fin;
    }

    if(isAC) {
    //printf("\nin intern_term_add_onf_term I'm line 455 \n");
#ifdef COLOR
        // il faut colorier les sous-termes
      for(i=0 ; i<getArity(subtermac) ; i++) {
        setColor(subtermac,i,color);
      }
#endif
      res=merge_sorted_term(tac,subtermac);
        // pour ne pas changer le pointer retourne
      tac->subterm=res->subterm;
      tac->sizeinfo = res->sizeinfo;
        // setArity(tac,getArity(res));
        // setSize(tac,getSize(res));
#ifdef UPDATE_HCODE
        // il faut calculer le hcode
      setHcode(tac,GgetSymb(tac));
      for(i=0 ; i<getArity(tac) ; i++) {
        setHcode(tac,INTERN_HFUNCTION(tac,getSubterm(tac,i)));
      }
#endif

#ifdef CC_GC
      tac->subterm[2*getSize(tac)]=(Gterm*)CC_GC_encode(tac);
#endif
      res=tac;
    } else {
      //printf("\nin intern_term_add_onf_term I'm line 481 \n");
      printf("List-matching case not implemented\n");
      exit(1);
    }
    goto fin;
  }

    // Il n'y a pas d'applatissement
  if(isAC) {
    //printf("\nin intern_term_add_onf_term I'm line 490 \n");
    if(isNullTac) {
      //printf("\nin intern_term_add_onf_term I'm line 492 \n");
      TERMAC_ALLOC(tac,2,fsym);
    }
    if(getArity(tac)==0) {
       //printf("\nin intern_term_add_onf_term I'm line 496 \n");
    //  printf("\n in termCommon line 491 getArity(");term_print(stdout,tac);printf(")= %d\n",getArity(tac));
        /* initialisation de tac->subterm faite dans termac_add_last */
      termac_add_last(tac,subterm,1);
        // il faut colorier le nouveau sous-terme
      setColor(tac,getArity(tac)-1,color);
      res=tac;
      goto fin;
    }
  } else {
    //printf("\nin intern_term_add_onf_term I'm line 506 \n");
    GnotYetImplemented("add_onf: list case");
  }


    /*
     * insertion triee de subterm dans tac
     */
  {
    int comp;
    comp = termac_lookup(subterm,tac,0,getArity(tac)-1,&i);
      //printf("1) res = %d\tindice = %d\n",comp,i);

    if(comp) {
      int m = getMult(tac,i)+1;
      if(symb_modulo(fsym)>0 && m>=symb_modulo(fsym)) {
          //printf("add_onf mult %d --> %d\n",m,1+((m-1)%(symb_modulo(fsym)-1)));
        setColorMult(tac,i,bicolor,
                     1+((m-1)%(symb_modulo(fsym)-1)));
      } else {
        setColorMult(tac,i,bicolor,m);
      }
      res=tac;
      goto fin;
    } else {
      termac_insert_bubble(tac,i);
      setSubterm(tac,i,subterm);
      setColorMult(tac,i,color,1);

#ifdef UPDATE_HCODE
      setHcode(tac,INTERN_HFUNCTION(tac,subterm));
#endif //UPDATE_HCODE

      res=tac;
      goto fin;
    }
  }
  fin:

    /*
#ifdef HCODE
    // calcul du hcode : a enlever par la suite
  if(res!=NULL && isAC(res)) {
    setHcode(res,GgetSymb(res));
    for(i=0 ; i<getArity(res) ; i++) {
      setHcode(res,HFUNCTION(res,(Gterm *) (Gterm *) GgetArgument(res,i)));
    }
      //printf("add_onf res[hcode = %d] = ",getHcode(res));
      //term_println(stdout,(Gterm*)res);
  }
#endif
    */
#ifdef AFFICHAGE
  printf("result add_onf_term = ");
  term_printnl(stdout,(Gterm*)res);
  printf("\n");
#endif
  assert(res!=subtermac);
  //printf("\n res = ");term_print(stdout,res);
  return res;
}

  /*
   * recherche de subterm dans tac
   */
static int termac_lookup(Gterm *subterm,
                         struct termac *tac,
                         int low,
                         int high,
                         int *indice) {
  int mid,comp;
  if(low>high) {
    *indice=low;
    return 0;
  }

  mid = (low+high)/2;

  comp=Gterm_cmp(subterm,(Gterm *) getSubterm((Gterm*)tac,mid));
  if(comp<0)
    return termac_lookup(subterm,tac,low,mid-1,indice);
  else if(comp>0)
    return termac_lookup(subterm,tac,mid+1,high,indice);
  else {
    *indice=mid;
    return 1;
  }
}

/*
 * insert un "trou" dans un terme AC a une position
 * t[0..pos-1] inchange
 * t[pos..n]   decale d'un cran a droite
 * t[pos]      <- subterm
 */
static struct termac *termac_insert_bubble(struct termac *tac, int pos) {
  int i;
  if(getArity(tac) == getSize(tac)) {
    termac_resize(tac,2*getSize(tac));
  }
    // Optimisation : simple deplacement en cas de resize

  if(getArity(tac) > pos) {
    Gterm** p1 = &(tac->subterm[2*getArity(tac)]);
    Gterm** p2 = p1+1;
/*
  printf("memmove %d terms from %u to %u (%d)\n",
  getArity(tac)-pos,
  &(tac->subterm[2*pos]),
  &(tac->subterm[2*pos+2]),
  2*(getArity(tac)-pos)*sizeof(Gterm*));
*/

    while(p1!=&(tac->subterm[2*pos])) {
      *p2 = *(p2-2);
      *p1 = *(p1-2);
//      printf("move from (%u,%u) to (%u,%u)\n",p1-2,p2-2,p1,p2);
      p2-=2;
      p1-=2;
    }
/*
  memmove(&(tac->subterm[2*pos+2]), &(tac->subterm[2*pos]),
  2*(getArity(tac)-pos)*sizeof(Gterm*));
  for(i=getArity(tac)-1 ; i>=pos ; i--) {
  setSubterm(tac,i+1,(Gterm *) GgetArgument(tac,i));
  setMult(tac,i+1,getMult(tac,i));
  setColor(tac,i+1,getColor(tac,i));
  }
*/
  }
  setArity(tac,getArity(tac)+1);
}

/*
 *	merge two sorted lists in descending order
 *      fait la fusion des termes identiques
 */
static struct termac *merge_sorted_term(struct termac *tac1,
                                        struct termac *tac2) {
  struct termac *res;
  int i1,i2;
  int indice;
  int comp;

#ifdef AFFICHAGE
  printf("merge_sorted_term(");
  term_print(stdout,(Gterm*)tac1);
  printf(",");
  term_print(stdout,(Gterm*)tac2);
  printf(")\n");
#endif

  
  TERMAC_ALLOC(res,getArity(tac1)+getArity(tac2),GgetSymbAC(tac1));
//  (res->subterm)=(Gterm**) MALLOC(2*getSize(res)*sizeof(Gterm*));
  
  for(i1=0, i2=0, indice=0 ; i1<getArity(tac1) && i2<getArity(tac2) ; ) {
    if(comp=Gterm_cmp(getSubterm(tac1,i1), getSubterm(tac2,i2))) {
      if(comp<0) {
          // on copie t1
        setSubterm(res,indice,getSubterm(tac1,i1));
        setColorMult(res,indice,getColor(tac1,i1),getMult(tac1,i1));
        indice++;
        i1++;
      } else {
          // on copie t2
        setSubterm(res,indice,getSubterm(tac2,i2));
        setColorMult(res,indice,getColor(tac2,i2),getMult(tac2,i2));
        indice++;
        i2++;
      }
    } else {
      int m = getMult(tac1,i1)+getMult(tac2,i2);
      
        // on fusionne les 2 cellules t1 et t2
        // Optimisation: Creation du partage
      setSubterm(res,indice,getSubterm(tac2,i2));

      if(term_modulo(tac1)>0 && m>=term_modulo(tac1)) {
          //printf("merge  mult %d --> %d\n",m,1+((m-1)%(term_modulo(tac1)-1)));
        setColorMult(res,indice,bicolor,
                     1+((m-1)%(term_modulo(tac1)-1)));
      } else {
        setColorMult(res,indice,bicolor,m);
      }
      
      i1++;
      i2++;
      indice++;
    }
  }
  for( ; i1<getArity(tac1) ; i1++, indice++) {
    setSubterm(res,indice,getSubterm(tac1,i1));
    setColorMult(res,indice,getColor(tac1,i1),getMult(tac1,i1));
  }
  for( ; i2<getArity(tac2) ; i2++, indice++) {
    setSubterm(res,indice,getSubterm(tac2,i2));
    setColorMult(res,indice,getColor(tac2,i2),getMult(tac2,i2));
  }
  setArity(res,indice);
#ifdef AFFICHAGE
  printf("result merge_sorted_term = ");
  term_printnl(stdout,(Gterm*)res);
  printf("\n");
#endif
  return res;
}



#define funTabCall0
#define funTabCall1 arg[0]
#define funTabCall2  funTabCall1,arg[1]
#define funTabCall3  funTabCall2,arg[2]
#define funTabCall4  funTabCall3,arg[3]
#define funTabCall5  funTabCall4,arg[4]
#define funTabCall6  funTabCall5,arg[5]
#define funTabCall7  funTabCall6,arg[6]
#define funTabCall8  funTabCall7,arg[7]
#define funTabCall9  funTabCall8,arg[8]
#define funTabCall10 funTabCall9,arg[9]
#define funTabCall11 funTabCall10,arg[10]
#define funTabCall12 funTabCall11,arg[11]
#define funTabCall13 funTabCall12,arg[12]
#define funTabCall14 funTabCall13,arg[13]
#define funTabCall15 funTabCall14,arg[14]
#define funTabCall16 funTabCall15,arg[15]

Gterm* specialApply(Gterm *res) {
  int fsym = GgetSymb(res),i;
  Gterm **arg;//[16]; //=res->sub; [256]
  GgetArguments_tab(arg,res);
  //ATprintf("\nin GspecialApply L 937 res = %t",res);
  if(funTab[fsym]==NULL){ return res;}

  switch(term_arity(res)) {
      // pour le cas AC
      case -1: return funTab[fsym](res);
      case 0:  return funTab[fsym](funTabCall0);
      case 1:  /*printf("\nin GspecialApply case1");*/return funTab[fsym](funTabCall1);
      case 2:  return funTab[fsym](funTabCall2);
      case 3:  return funTab[fsym](funTabCall3);
      case 4:  return funTab[fsym](funTabCall4);
      case 5:  return funTab[fsym](funTabCall5);
      case 6:  return funTab[fsym](funTabCall6);
      case 7:  return funTab[fsym](funTabCall7);
      case 8:  return funTab[fsym](funTabCall8);
      case 9:  return funTab[fsym](funTabCall9);
      case 10: return funTab[fsym](funTabCall10);
      case 11: return funTab[fsym](funTabCall11);
      case 12: return funTab[fsym](funTabCall12);
      case 13: return funTab[fsym](funTabCall13);
      case 14: return funTab[fsym](funTabCall14);
      case 15: return funTab[fsym](funTabCall15);
      case 16: return funTab[fsym](funTabCall16);
      default:
        printf("increase maxArity=16 in term.c::specialApply\n");
        exit(1);
  }
}

Gterm* normalise(Gterm *t) {
  int code, arity, i;
  //printf("\n******************* in normalise ************\n");
  if(GisTagged(t)) {
    return t;
  }
  code  = GgetSymb(t);
  arity = term_arity(t);
  //printf("\n code = %d , arity = %d\n",code,arity);
  //printf("\nNormalise\tcode=%d\tarity=%d\n",code,arity);
  if(!term_isAC(t)) {
    for(i=0 ; i<arity ; i++) {
      //printf("before: getSymb = %d\n",GgetSymb());
      GsetArgument(t,i,normalise((Gterm *) GgetArgument(t,i)));
      //printf("after:  getSymb = %d\n",GgetSymb(t));
        //printf("t=(%x)\n",t);
    //ATprintf("\nin normalise L981 t= %t",t);
    }
  } else {
    struct termac *tac=(struct termac*)t;
    Gterm *nt;
    int computeONF=0;

    for(i=0 ; i<getArity(tac) ; i++) {

      //nt = normalise(getSubterm(tac,i));
        //computeONF |= (nt!=getSubterm(tac,i));
      setSubterm(tac,i,normalise(getSubterm(tac,i)));
    }

#ifdef UPDATE_HCODE
    setHcode(tac,GgetSymb(tac));
    for(i=0 ; i<getArity(tac) ; i++) {
      setHcode(tac,INTERN_HFUNCTION(tac,getSubterm(tac,i)));
    }
#endif // UPDATE_HCODE

      // [pem: May 31 00]
      // il faut trier les sous-termes de t
    if(1) { //computeONF) {
      struct termac *newtac = NULL;
      int i,j;
        // n'est plus utile
        // TERMAC_ALLOC(newtac,getArity(tac),GgetSymb(tac));
      for(i=0 ; i<getArity(tac) ; i++) {
        for(j=0 ; j<getMult(tac,i) ; j++) {

          newtac = term_add_onf_term_color(newtac,GgetSymb((Gterm*)tac),getSubterm(tac,i),getColor(tac,i));
        }
      }
      t=(Gterm*) newtac;
    }
  }
  //ATprintf("\nin normalise L1019 t= %t",t);
    //printf("normalize: "); internal_term_println(stdout,t,ELAN_IO);
    //ATprintf("************ t = %t",t);
    //term_print(stdout,t);printf("\n");
  t=(Gterm *)GspecialApply(t);
  //ATprintf("\nin normalise L1024 t= %t",t);
    //printf("result:    "); internal_term_println(stdout,t,ELAN_IO);
    //printf("end normalise\n");
  return t;
}


/*
 * Replace t2 by t3 in t1
 */
Gterm *term_rec_replace(Gterm *t1,
                        Gterm *t2,
                        Gterm *t3) {
  register int arity;
  Gterm *res=NULL;
  int renormalise=0;
  //ehm modification a faire
  Gterm *TabArgument[100];

  if(term_notDestructEqual(t1,t2)) {
    return t3;
  }
    //if(GisTagged(t1)) return t1;
  if(GisIntegerTagged(t1) || GisIdentifierTagged(t1) || GisStringTagged(t1)) {
    return t1;
  }

  arity=term_arity(t1);
  if((arity=term_arity(t1)) == 0) return t1;
    /*
    printf("replace( ");
    term_print(stdout,t2);
    printf(" by ");
    term_print(stdout,t3);
    printf(" in ");
    term_print(stdout,t1);
    printf(" )\n");
    */

  if(!term_isAC(t1)) {
    Gterm *tmp;
    int i;
      /*
       * Il y a un probleme dans le marquage du partage qui n'est pas recursif
       * Faut-il supprimer cette optimisation ou faire un marquage recursif ?
       * Pour le moment on supprime l'optimisation
       */
    for(i=0 ; i<arity ; i++) {
      tmp = term_rec_replace(GgetArgument(t1,i),t2,t3);
      if(res != NULL) {
        GsetArgument(res,i,tmp);
      } else if(tmp != GgetArgument(t1,i)) {
        int j;
          // il y a eu un remplacement : il faut dupliquer le symbole de tete
        renormalise=1;
          //printf("copie du symbole de tete\n");
        GmakeApplArity(res,arity,GgetSymb(t1));
        for(j=0 ; j<i ; j++) {
          GsetArgument(res,j,(Gterm *) GgetArgument(t1,j));
        }
        GsetArgument(res,i,tmp);
      }
    }

    if(res==NULL) {
      res = t1;
    }

  } else {
    Gterm *tmp;
    struct termac *resac;
    struct termac *tac1=(struct termac*) t1;
    int i,j;
      //printf("not yet implemented\n");
      //exit(0);
    TERMAC_ALLOC(resac,getArity(tac1),GgetSymbAC(tac1));
    for(i=0 ; i<getArity(tac1) ; i++) {

      tmp = term_rec_replace(getSubterm(tac1,i),t2,t3);
        // Attention a la multiplicite et a la couleur
      for(j=0 ; j<getMult(tac1,i) ; j++)
	resac = term_add_onf_term(resac,GgetSymbAC(tac1),tmp);

      if(tmp != getSubterm(tac1,i)) {
          //printf("AC renormalise\n");
	renormalise=1;
      }
    }
    res=(Gterm*)resac;
  }

    /*
     * re-normalisation
     */
  if(renormalise) {
      /*
        printf("start renormalise\n");
        printf("arity = %d\tfsym = %d\n",arity,GgetSymb(res));
        printf("term = "); term_printnl(stdout,res);
      */
    res=(Gterm*)GspecialApply(res);
      /*
        printf("term = "); term_printnl(stdout,res);;
        printf("end renormalise\n");
      */
  }

    //printf("res = "); term_printnl(stdout,res);
  return res;
}

/*
 * Replace t2 by t3 in t1
 */

Gterm *term_replace(Gterm *t1,Gterm *t2,Gterm *t3) {
    /*
     * Replace t2 by t3 in t1
     */
  if(term_occur(t1,t2)) {
      /*
    printf("replace "); internal_term_println(stdout,t2,ELAN_IO);
    printf("by "); internal_term_println(stdout,t3,ELAN_IO);
    printf("in "); internal_term_println(stdout,t1,ELAN_IO);
      */
    return term_rec_replace(t1,t2,t3);
  } else {
    return t1;
  }
}


/*
 * t2 occurs in t1
 */
int term_occur(Gterm *t1,Gterm *t2)
{
  int i,arity;

    //term_print(stdout,t2); printf(" occurs in "); term_printnl(stdout,t1);
    //printf("isAC: %d\tarity = %d\n",term_isAC(t1),term_arity(t1));

    //  printf("t1 = "); term_printnl(stdout,t1);
    //printf("t2 = "); term_printnl(stdout,t2);
  
  if(term_notDestructEqual(t1,t2)) {
      //printf("TRUE\n");
    return 1;
  }
    //printf("FALSE\n");
  
    //if(GisTagged(t1)) return 0;
  if(GisIntegerTagged(t1) || GisIdentifierTagged(t1) || GisStringTagged(t1)) {
    return 0;
  }

  arity=term_arity(t1);
  if(arity==0) return 0;

  if(!term_isAC(t1)) {
    for(i=0 ; i<arity ; i++) {

      if(term_occur((Gterm *) GgetArgument(t1,i),t2)) return 1;
    }
  } else {
    struct termac *tac1=(struct termac*)t1;
    for(i=0 ; i<getArity(tac1) ; i++) {

      if(term_occur(getSubterm(tac1,i),t2)) return 1;
    }
  }
  return 0;
}
void termac_copyTopSymbol(struct termac *tac, struct termac *subterm) {
  int i;
//  printf("termac_copyTopSymbol\n");
//  printf("termac_copyTopSymbol: should not be used\n");
//  exit(1);
  termac_resize(tac,getArity(subterm));
  setArity(tac,getArity(subterm));
#ifdef UPDATE_HCODE
  setHcode(tac,getHcode(subterm));
#endif
  for(i=0 ; i<getArity(subterm) ; i++) {

    setSubterm(tac,i,(Gterm *) getSubterm(subterm,i));
    setColorMult(tac,i,getColor(subterm,i),getMult(subterm,i));
  }
}


void termac_copyTopSymbolExcept(struct termac *dest,
                                int pos,
                                struct termac *source) {
  int idest,isource;
  termac_resize(dest,getArity(source));
  setArity(dest,getArity(source));
#ifdef UPDATE_HCODE
  setHcode(dest,getHcode(source));
#endif
  for(idest=0,isource=0 ; isource<getArity(source) ; isource++) {
    if(isource==pos) {
      if(getMult(source,isource) > 1) {

        setSubterm(dest,idest,getSubterm(source,isource));
        setColorMult(dest,idest,getColor(source,isource),getMult(source,isource)-1);
        idest++;
      } else {
          // do not copy the element
          // decrement the arity
        setArity(dest,getArity(source)-1);
      }
    } else {

      setSubterm(dest,idest,getSubterm(source,isource));
      setColorMult(dest,idest,getColor(source,isource),getMult(source,isource));
      idest++;
    }
  }
}

void term_copyTopSymbol(Gterm *t, Gterm *source) {
  int i;
  for(i=0 ; i<term_arity(t) ; i++) {

    GsetArgument(t,i,(Gterm *) GgetArgument(source,i));
  }
}


/*
 * transform F(t) into t
 * returns NULL if the term is ok
 */
Gterm *term_removeTopSymbol(Gterm *t) {
  Verif_void(t,"term_removeTopSymbol(t)");
  if(!isAC(t)) {
      /*
       * pour fonctionner avec l'ACMatcher qui peut retourner
       * des termes syntaxiques
       */
    return t;
  } else {
    struct termac *tac=(struct termac*)t;
    if(getArity(tac)==0) {
	// le terme vide F() n'est pas modifie
      return t;
    } else if(getArity(tac)==1 && getMult(tac,0)==1) {
      return getSubterm(tac,0);
    } else {
      return (Gterm*) NULL;
    }
  }
}
#ifdef COLOR
int isMonoColor(Gterm *t) {
  int i,res;
  int color;
  struct termac *tac=(struct termac*)t;

  if(!isAC(t)) {
    fprintf(stderr,"isMonoColor: not an AC term\n");
    exit(1);
  }

  if(getArity(tac)==0) {
    fprintf(stderr,"isMonoColor: no subterm\n");
    exit(1);
  }

    //printf("isMono: "); term_printnl(stdout,t);

  color=getColor(tac,0);
  res = (color!=bicolor);
  for(i=0 ; res && i<getArity(tac) ; i++) {
    res = res && (color == getColor(tac,i));
  }
  return res;
}

static int freeColor=0;

void setMonoColor(Gterm *t) {
  int i;
  int color=bicolor;
  struct termac *tac=(struct termac*) t;

    //freeColor = (freeColor+1)%4096;
    //color=freeColor;

  for(i=0 ; color==bicolor && i<getArity(tac) ; i++) {
    color=getColor(tac,i);
  }

    //printf("setMonoColor = %d\n",color);

  if(color==bicolor) color=1;
  for(i=0 ; i<getArity(tac) ; i++) {
    setColor(tac,i,color);
  }
}
#endif


extern char *tabStrategyStr[];
extern int tabStrategyIndex[];
extern int tabStrategySize;

Gterm *term_metaApply(Gterm *t) {
  Gterm *strategy;
  Gterm *list;
  Gterm *mainTerm;
  Gterm *nil;
  Gterm *strategyNumber;
  Gterm *dest,*temp;
  int index;
  int start, end, nbSol;
  int all=0;
  int *counter=(int*) allocStable(sizeof(int));
  Gterm *applyRes, *newRes;
  Gterm ***ptr_oldRes=(Gterm***)allocStable(sizeof(Gterm **));
  Gterm **listRes=(Gterm**)allocStable(sizeof(Gterm *));
  *counter=0;
  //printf("------------------------------------------------------------\n");
  //printf("symb = %d\n",GgetSymb(t));
  switch(GgetSymb(t)) {
      case 128:
          //printf("t   : ");
        term_printnl(stdout,t);
        strategy=GgetArgument(t,0);
        list=GgetArgument(t,1);
        start = GgetInt(GgetArgument(t,2));
        nbSol = GgetInt(GgetArgument(t,3));
        strategyNumber=GgetArgument(GgetArgument(strategy,0),0);
        mainTerm=GgetArgument(list,0);
        nil=GgetArgument(list,1);
          /*             printf("list     : ");
                         term_printnl(stdout,list);
                         printf("nil      : ");
                         term_printnl(stdout,nil);
                         printf("mainTerm : ");
                         term_printnl(stdout,mainTerm);
                         printf("number   : ");
                         term_printnl(stdout,strategyNumber);          */
        if(GisIntegerTagged(strategyNumber)) {
          index=GgetInt(strategyNumber);
        } else if(GisStringTagged(strategyNumber)) {
          int i;
          int strIndex;
          char *strategyName;
            //fprintf(stderr,"string = %s\n",getString(strategyNumber));
          strategyName = GgetString(strategyNumber);
          for(i=0, strIndex=0 ; strIndex==0 && i<tabStrategySize ; i++) {
            if(!strncmp(strategyName,tabStrategyStr[i],strlen(strategyName))) {
              strIndex = tabStrategyIndex[i];
            }
          }
          if(strIndex==0) {
            printf("Strategy '%s' not found\n",strategyName);
            exit(1);
          }
          index = strIndex;
            //printf("strIndex = %d\n",strIndex);
        } else {
          printf("strategyNumber = ");
          term_printnl(stdout,strategyNumber);
          fprintf(stderr,"term_metaApply: internal error\n");
          exit(1);
        }
          //printf("index = %d\n",index);
          //printf("start = %d\tnbSol = %d\n",start,nbSol);
        if(nbSol==0) {
          all=1;
        }
        end = start+nbSol;
          //*ptr_oldRes=&oldRes;
        *listRes=NULL;
        *ptr_oldRes=&(*listRes);
        *counter=0;
        if(setChoicePoint()==0){
          CUTOPEN();
            //printf("**********\n");
          applyRes = strTab[index](mainTerm);
          (*counter)++;
                 //printf("build=%d\ti=%d\tend=%d\n",*counter>start,*counter,end);
          if(*counter>start) {
            if(*counter<=end || all) {
                // Construire la solution
                //term_print(stdout,applyRes);
              GmakeApplArity(newRes,2,GgetSymb(list));
              GsetArgument(newRes,0,applyRes);
              (**ptr_oldRes)=newRes;
              dest=GgetArgument(newRes,1);
              *ptr_oldRes=&(dest);
                //printf("\nres=%d\tnewRes=%d\n",*listRes,newRes);
            } else {
                // C'est fini
                //printf("C'est fini\n");
                //(**ptr_oldRes)=nil;              CUTCLOSE();
            }
          }
          fail();
        } else {
          (**ptr_oldRes)=nil;
            //printf("\n");
            //printf("**********\n");
        }
        break;
      case 129:
        break;
      case 130:
        break;
  }
    // printf("RESULT = ");
  term_printnl(stdout,*listRes);
    //*listRes=t;
  return *listRes;
}

struct termac *CC_GC_encode(struct termac *tac) {
    //return (struct termac*) ~((unsigned long)tac);
  return tac;
}

struct termac *CC_GC_decode(struct termac *tac) {
    //return (struct termac*) ~((unsigned long)tac);
  return tac;
}
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH

Gterm *term_build(int nbArg, int code, ...) {
  va_list    argv;
  Gterm *res;
  int i;
  va_start(argv, code);
  GmakeApplArity(res,nbArg,code);
  for(i=0 ; i<nbArg ; i++) {
    GsetArgument(res,i,((Gterm *) va_arg(argv, Gterm *)));
  }
  va_end(argv);
  return res;
}

/*
 * String
 */

char *build_string(int n) {
  char *res;
  res = (char*)MALLOC(2*sizeof(char)) ;
  res[0] = (char)(n) ;
  res[1] = '\0' ;
  return res ;
}

char *ccat(char *s1, char *s2) {
 char *res ;
 int n1 = strlen(s1);
 int n2 = strlen(s2);

 res =(char *)MALLOC((n1+n2+1)*sizeof(char)) ;
 strcpy(res,s1) ;
 strcat(res,s2) ;
 return res ;
}

char *findIdent(unsigned long n) {
  char *res ;
  int size ;
  size = strlen(tabIdent[n]) ;
  res = (char *)MALLOC((size+1)*sizeof(char)) ;
  strcpy(res,tabIdent[n]) ;
  return res ;
}

int selectChar(char *string,int n) {
  return (int) string[n] ;
}

char *substitute(char *string,int n1, int n2) {
  char * res ;
  int size ;
  size = strlen(string) ;
  res = (char *)MALLOC((size+1)*sizeof(char)) ;
  strcpy(res,string) ;
  res[n1] = (char)(n2) ;
  return res ;
}

char *subString(char *string,int i,int l) {
  char *res ;
  int j ;
  res = (char *)MALLOC((l+1)*sizeof(char)) ;
  for(j=0 ; j<l ; j++) {
    res[j] = string[i+j] ;
  }
  res[l] = '\0' ;
  return res ;
}

/*
 * I/O
 */

int open_file(char *file, char *mode) {
  FILE *f ;
  int n = tabfile->size ;
    //printf("open_file(%s,%s)\n",file,mode);
  assert(n<MAXFILE);
  f = fopen(file,mode);
  if(n<MAXFILE && f!=0){
    tabfile->files[n] = f ;
    (tabfile->size)++ ;
  } else {
    fprintf(stderr,"Error in open_file(%s,%s)\n",file,mode);
    exit(1);
  }
  return n;
}

int close_file(int pid) {
  fclose(tabfile->files[pid]);
  return pid;
}

int Getc(int pid){
  char c;
  FILE *fp;
  fp = tabfile->files[pid];
  c = getc(fp);
  return (int)(c);
}

int Putc(int pid, int c){
  FILE *fp;
  fp = tabfile->files[pid];
  putc((char)c,fp);
  return c;
}



























