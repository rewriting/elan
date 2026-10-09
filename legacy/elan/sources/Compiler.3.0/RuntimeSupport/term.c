#include "term.h"
#include "tools.h"
#include "builtin.h"
#include "Back.h"

static void print_f_prefix(FILE *fich,struct term *t);
static struct termac *merge_sorted_term(struct termac *tac1,
                                        struct termac *tac2);
static struct termac *termac_insert_bubble(struct termac *tac, int pos);

static int termac_lookup(struct term *subterm,
                         struct termac *tac,
                         int low, int high, int *indice);

void term_alloc(struct term **ptr_dest, int size_sname, unsigned int funsym) {
  struct term *dest;
  *ptr_dest=(struct term*) MALLOC(size_sname);
  dest=*ptr_dest;
  setSymb(dest,funsym);
#ifdef DEBUG
  cptTermAlloc++;
    /*
      if(symb_isAC(funsym)) {
      printf("warning in term_alloc ac: use termac_alloc\n");
      }
    */
#endif
}

struct term *term_build(int nbArg, int code, ...) {
  va_list    argv;
  struct term *res;
  int i;
  va_start(argv, code);
  TERM_ARITY_ALLOC(res,nbArg,code);
  for(i=0 ; i<nbArg ; i++) {
    res->sub[i] = (struct term *) va_arg(argv, struct term *);
  }
  va_end(argv);
  return res;
}

void fsym_init(int code, int a, char *n,
	       int sem, int dstrat,
	       struct term* (*semaction)(struct term *)) {
  fsymtab[code].arity=a;
  fsymtab[code].name=n;
  fsymtab[code].semantic=sem;
  fsymtab[code].modulo=0;
  fsymtab[code].defstrat=dstrat;
  fsymtab[code].semact=semaction;
}

#ifdef CC_GC
void finalize(struct term *obj,int cd) {
  struct termac *tac=(struct termac*)obj;
    /*
      if(obj != (struct term*)CC_GC_decode((struct termac*)tac->subterm[2*getSize(tac)])) {
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

#define initMinimalSize(s) ((1+((s)>>2))<<2)
void termac_alloc(struct termac **ptr_dest, int size, unsigned int funsym) {
  struct termac *dest;

  size = initMinimalSize(size);
//  if(size<8) size=8;
//  if(size<4) size=4;
  
  *ptr_dest=(struct termac*) MALLOC(sizeof(struct termac));
  dest=*ptr_dest;
  setSymb(dest,funsym);
  setSizeArity(dest,size,0);
  setAC(dest);
    /*
     * si l'allocation est faite au moment de l'insertion du premier element 
     * c'est localise dans term.c et ac_tool.c
     */
#ifdef CC_GC
  (dest->subterm)=(struct term**) MALLOC(((1+size)*sizeof(struct term*))<<1);
  dest->subterm[2*getSize(dest)]=(struct term*)CC_GC_encode(dest);
  GC_register_finalizer(dest,finalize,1,0,0);
#else
  (dest->subterm)=(struct term**) MALLOC(((size*sizeof(struct term*))<<1));
#endif
    
#ifdef DEBUG
  cptTermacAlloc++;
#endif
}

#ifdef NOTMACRO
void termac_add_lastColor(struct termac *tac, struct term *subterm, int mult, int color) {
    /*
      if(getArity(tac) == 0) {
      int size = initMinimalSize(getSize(tac));
      (tac->subterm)=(struct term**) MALLOC(2*size*sizeof(struct term*));
        // printf("termac_add_last: init size = %d\n",size);
        } else
    */
  int arity = getArity(tac);
  if(arity == getSize(tac)) {
    termac_resize(tac,2*arity);
  }
  setColorMult(tac,arity,color,mult);
  setSubterm(tac,arity,subterm);
  setArity(tac,arity+1);
}
#endif

void termac_resize(struct termac *tac, int size) {
  if(size > getSize(tac)) {
#ifdef CC_GC
    struct term **newSubterm = (struct term**) MALLOC(((1+size)*sizeof(struct term*))<<1);
#else
    struct term **newSubterm = (struct term**) MALLOC((size*sizeof(struct term*))<<1);
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
    tac->subterm[2*size]=(struct term*)CC_GC_encode(tac);
#endif
  }
}

#ifdef NOTMACRO
int term_isAC(struct term *t) {
  Verif_void(t,"term_isAC(t)");
  return isAC(t);
}
#endif

/*
 * output.c
 */
void internal_term_println(FILE *fich,struct term *t, int mode) {
  Verif_void(t,"internal_term_printnl(fich,t)");
  if(mode==ELAN_IO) {
      //term_println(fich,t);
    termOut(fich,term_unflatten(t));
  } else {
    term_print(fich,t);
  }
  fprintf(fich,"\n");
}

void term_print(FILE *fich,struct term *t)
{
  Verif_void(t,"term_print(fich,t)");
  print_f_prefix(fich,t);
}


static void print_f_prefix(FILE *fich,struct term *t) {
  if(isIntegerTagged(t)) {
      // fprintf(fich,"integer\n");
    fprintf(fich,"%d",getInt(t));
    return;
  } else if(isIdentifierTagged(t)) {
      //fprintf(fich,"identifier\n");
    fprintf(fich,"ident(%d)",getIdentifier(t));
      //fprintf(fich,"%d",getIdentifier(t));
    return; 
  } else if(isStringTagged(t)) {
      // PROBLEME SUR SUN
    fprintf(fich,"string '%s' ",getString(t));
      //fprintf(fich,"%s",getString(t));
    return;
  }
  
    // fprintf(fich,"symbol : %d\t%d\n",t,getSymb(t));
  fprintf(fich,"%s",term_name(t));
  
  if(term_isAC(t)) {
    struct termac *tac=(struct termac*)t;
    fprintf(fich,"*");
    if(getArity(tac) > 0 ) {
      int i;
        //if(isReduced(t)) fprintf(fich,"[r]");
      fprintf(fich, "(");
      for(i=0 ; i<getArity(tac) ; i++) {
          /*
           * print with multiplicity
           */
	if(getMult(tac,i)>1) {
	  int j;
	  fprintf(fich,"[[");
	  for(j=0 ; j<getMult(tac,i) ; j++) {
	    term_print(fich, getSubterm(tac,i));
	    if(j!=getMult(tac,i)-1) {
	      fprintf(fich,",");
	    }
	  }
	  fprintf(fich,"]]");
	} else {
	  term_print(fich, getSubterm(tac,i));
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
        term_print(fich, t->sub[i]);
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
void term_printREFln(FILE *fich,struct term *t)
{
  Verif_void(t,"term_printlnREF(fich,t)");
  term_printREF(fich,t);
  fprintf(fich,"\n");
}

void term_printREF(FILE *fich,struct term *t)
{
  int i;
  Verif_void(t,"term_printREF(fich,t)");
  if(isIntegerTagged(t)) {
    fprintf(fich,"INT(%d)",getInt(t));
    return;
  } else if(isIdentifierTagged(t)) {
    fprintf(fich,"IDENT(%d)",getIdentifier(t));
    return; 
  }
    // PROBLEME SUR SUN
  else if(isStringTagged(t)) {
      // A MODIFIER !!!
    fprintf(fich,"STRING(%s)",getString(t));
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
          term_printREF(fich, getSubterm(tac,i));
          if( !((i==getArity(tac)-1) && (j==getMult(tac,i)-1))) {
            fprintf(fich,".");
          }
        }
      }
      for(i=0 ; i<getArity(tac) ; i++) {
	for(j=0 ; j<getMult(tac,i) ; j++) {
          if( !((i==getArity(tac)-1) && (j==getMult(tac,i)-1))) {
            fprintf(fich,".nil,%d)",getSymb(tac));
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
	term_printREF(fich, t->sub[i]);
	fprintf(fich,".");
      }
    }
    fprintf(fich,"nil,");
    fprintf(fich,"%d)",getSymb(t));
  }
  
}

struct term *term_unflatten(struct term *t);
static struct term *subterm_unflatten(unsigned int funsym,
                                      struct termac *tac,
                                      int i,int multiplicity) {
  struct term *res;
  if(i==getArity(tac)-1 && multiplicity<=1) {
      /* si c'est le dernier element de la liste */
    return term_unflatten(getSubterm(tac,i));
  } else {
      /* TODO : corriger si funsym est AC */
    TERM_ALLOC(res,term2,funsym);
    res->sub[0] = term_unflatten(getSubterm(tac,i));
    multiplicity--;
    if(multiplicity<=0) {
      i++;
      res->sub[1] = subterm_unflatten(funsym,tac,i,getMult(tac,i));
    } else {
      res->sub[1] = subterm_unflatten(funsym,tac,i,multiplicity);
    }
    return res;
  }
}

struct term *term_unflatten(struct term *t) {
    //printf("term_unflatten: "); term_println(stdout,t);

  if(isTagged(t)) {
    return t;
  }

  if(term_isAC(t)) {
    struct termac *tac=(struct termac*)t;
    return subterm_unflatten(getSymb(tac),tac,0,getMult(tac,0));
  } else {
    struct term *res = t;
    int i, arity=term_arity(t);
    if(arity>0) {
      if(arity==1) {
        TERM_ALLOC(res,term1,getSymb(t));
      } if(arity==2) {
        TERM_ALLOC(res,term2,getSymb(t));
      } else {
        TERM_ARITY_ALLOC(res,arity,getSymb(t));
      }
      for(i=0 ; i<arity ; i++) {
        res->sub[i] = term_unflatten(t->sub[i]);
      }
      return res;
    } else {
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
                                        struct term *subterm,
                                        int color)
#else
struct termac *intern_term_add_onf_term(int isAC,
                                        struct termac *tac,
                                        unsigned int fsym,
                                        struct term *subterm)
#endif
{
  struct termac *res;
  int i;
  int found;
  int isNullTac = (tac==NULL);
  struct termac *subtermac=(struct termac*)subterm;
#ifdef AFFICHAGE
  printf("term_add_onf_term(");
  if(tac!=NULL) term_print(stdout,(struct term*)tac);
  printf(",");
  term_print(stdout,subterm);
  printf(")\n");
#endif

    // Ajout d un sous-terme vide 
  if(term_isAC(subterm) && getArity(((struct termac *)subterm))==0) {
    if(isNullTac) {
        /*
         * ne rien faire et retourner NULL
         */
        //TERMAC_ALLOC(tac,2,fsym);
    }
    res = tac;
    goto fin;
  } 

//  if(isNullTac || getSymb(tac) == getSymb(subterm)) {
  if(fsym == getSymb(subterm)) {
      // il faut applatir et faire un merge list
    if(isNullTac) {
      TERMAC_ALLOC(tac,getArity(subtermac),fsym);
        //printf("termac_alloc(%d)\n",getArity(subtermac));
    }
    
    if(getArity(tac)==0) {
        // cas d un terme vide
      termac_copyTopSymbol(tac,subtermac);
        /*
          {
          int arity = getArity(subtermac);
          int size = initMinimalSize(arity);
          
          struct term **newSubterm = (struct term**) MALLOC(2*size*sizeof(struct term*));
          
          setSize(tac,size);
          setArity(tac,arity);
          for(i=0 ; i<2*arity ; i++) {
          newSubterm[i]=subtermac->subterm[i];
          }
          tac->subterm = newSubterm;
          }
        */
      res=tac;
//      res=subtermac;
      goto fin;
    }

    if(isAC) {
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
#ifdef CC_GC
      tac->subterm[2*getSize(tac)]=(struct term*)CC_GC_encode(tac);
#endif
      res=tac;
    } else {
      printf("List-matching case not implemented\n");
      exit(1);
    }
    goto fin;
  }

    // Il n'y a pas d'applatissement
  if(isAC) {
    if(isNullTac) {
      TERMAC_ALLOC(tac,2,fsym);
    }
    if(getArity(tac)==0) {
        /* initialisation de tac->subterm faite dans termac_add_last */
      termac_add_last(tac,subterm,1);
        // il faut colorier le nouveau sous-terme
      setColor(tac,getArity(tac)-1,color);
      res=tac;
      goto fin;
    }
  } else {
    notYetImplemented("add_onf: list case");
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
      res=tac;
      goto fin;
    }
  }
  fin:
  
#ifdef AFFICHAGE
  printf("result add_onf_term = ");
  term_printnl(stdout,(struct term*)res);
  printf("\n");
#endif
  assert(res!=subtermac);
  return res;
}

  /*
   * recherche de subterm dans tac
   */
static int termac_lookup(struct term *subterm,
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
  comp=term_cmp(subterm, getSubterm(tac,mid));
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
    struct term** p1 = &(tac->subterm[2*getArity(tac)]);
    struct term** p2 = p1+1;
/*
  printf("memmove %d terms from %u to %u (%d)\n",
  getArity(tac)-pos,
  &(tac->subterm[2*pos]),
  &(tac->subterm[2*pos+2]),
  2*(getArity(tac)-pos)*sizeof(struct term*));
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
  2*(getArity(tac)-pos)*sizeof(struct term*));
  for(i=getArity(tac)-1 ; i>=pos ; i--) {
  setSubterm(tac,i+1,getSubterm(tac,i));
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
  term_print(stdout,(struct term*)tac1);
  printf(",");
  term_print(stdout,(struct term*)tac2);
  printf(")\n");
#endif

  
  TERMAC_ALLOC(res,getArity(tac1)+getArity(tac2),getSymb(tac1));
//  (res->subterm)=(struct term**) MALLOC(2*getSize(res)*sizeof(struct term*));
  
  for(i1=0, i2=0, indice=0 ; i1<getArity(tac1) && i2<getArity(tac2) ; ) {
    if(comp=term_cmp(getSubterm(tac1,i1), getSubterm(tac2,i2))) {
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
  term_printnl(stdout,(struct term*)res);
  printf("\n");
#endif
  return res;
}

/*
 *	Compare flattened/sorted terms using lexicographic order for
 *	argument list of free function symbols and multiset order for
 *	argument lists of AC function symbols.
 */

int term_cmp(register struct term *t1, register struct term *t2) {
  register int r, arity1;

  start:
  if(t1==t2) {
    return (0);
  }

  if(isTagged(t1)) {
    if(isIntegerTagged(t1)) {
      if(getInt(t1) != getInt(t2)) {
        return(getInt(t1) - getInt(t2));
      } else {
        printf("error in term_cmp\n");
        exit(1);
      }
    } else if(isIdentifierTagged(t1)) {
      return (getIdentifier(t1) - getIdentifier(t2));
    } else if(isStringTagged(t1)) {
      return strcmp(getString(t1),getString(t2));
    }
  }

    /*
      printf("t1 = "); term_printnl(stdout,t1);
      printf("t2 = "); term_printnl(stdout,t2);
      printf("------------------------------\n");
    */

  if(getSymb(t1) != getSymb(t2)) {
    return(getSymb(t1) - getSymb(t2));
  }
  if((arity1=term_arity(t1))==0) {
    return(0);
  }

  if(!term_isAC(t1)) {
      /* lexicographic ordering on subterms */
    register struct term **tt1=&(t1->sub[0]);
    register struct term **tt2=&(t2->sub[0]);

    for( arity1-- ; arity1 ; arity1--, tt1++, tt2++) {
      if((r = term_cmp(*tt1,*tt2))) {
        return(r);
      }
        /*
         * on peut creer du partage ici
         */
    }
      /* last rec. opt. */
    t1=*tt1;
    t2=*tt2;
    goto start;
  } else {
      /* multiset ordering on subterms */
    register int p1, p2;
    struct termac *tac1=(struct termac*)t1;
    struct termac *tac2=(struct termac*)t2;
    for(p1=0, p2=0; ; p1++, p2++) {
      if(p1==getArity(tac1)) return(p2==getArity(tac2)?0:(-1));
      if(p2==getArity(tac2)) return(1);
      if((r = term_cmp(getSubterm(tac1,p1), getSubterm(tac2,p2))))
        return(r);
        /*
         * on peut creer du partage ici
         */
      setSubterm(tac2,p2,getSubterm(tac1,p1));

      if(getMult(tac1,p1) < getMult(tac2,p2)) return(-1);
      if(getMult(tac1,p1) > getMult(tac2,p2)) return(1);
    }
  }
  return(0);
}

  /*
   * Version optimisee
   */
/*
#define STRONGEQUAL 0 
#define SOFTEQUAL   -1000
#define GREATER     1
#define LOWER       -1
#define INITVALUE   666
#define fastTermCmp(t1,t2) \
  if(t1==t2) {\
    r=0;\
  } else if(isTagged(t1)) {\
    if(isIntegerTagged(t1)) {\
      if(getInt(t1)!=getInt(t2)) {\
        r=(getInt(t1)-getInt(t2));\
      } else {\
          printf("error in term_cmp\n"); exit(1);\
      }\
    } else if(isIdentifierTagged(t1)) {\
      if(getIdentifier(t1)!=getIdentifier(t2)) {\
        r=(getIdentifier(t1)-getIdentifier(t2));\
      } else {\
        printf("error in term_cmp\n"); exit(1);\
      }\
    } else if(isStringTagged(t1)) {\
      r=strcmp(getString(t1),getString(t2));\
    }\
  } else if(getSymb(t1)!=getSymb(t2)) {\
    r=(getSymb(t1)-getSymb(t2));\
  } else if((arity1=term_arity(t1))==0) {\
    r=(0);\
  } else {\
    r=INITVALUE;\
  }


int intern_term_cmp(register struct term *t1, register struct term *t2) {
  register int r, arity1;

  goto next;
  start:
  fastTermCmp(t1,t2);
  if(r!=INITVALUE) {
    goto end;
  } else {
      //printf("continue\n");
    ;
  }
  next:
  arity1=term_arity(t1);
  
  if(!term_isAC(t1)) {
      // lexicographic ordering on subterms
    register struct term **tt1=&(t1->sub[0]);
    register struct term **tt2=&(t2->sub[0]);

    for( arity1-- ; arity1 ; arity1--, tt1++, tt2++) {
        
      fastTermCmp(*tt1,*tt2);
      if(r!=INITVALUE) {
        if(r!=0) {
          goto end;
        }
      } else if((r=intern_term_cmp(*tt1,*tt2))) {
        goto end;
      }
        // on peut creer du partage ici
        //*tt1=*tt2;
    }
      // last rec. opt.
    t1=*tt1;
    t2=*tt2;
    goto start;
  } else {
      // multiset ordering on subterms
    register int p1, p2;
    struct termac *tac1=(struct termac*)t1;
    struct termac *tac2=(struct termac*)t2;
    for(p1=0, p2=0; ; p1++, p2++) {
      if(p1==getArity(tac1)) {
        r=(p2==getArity(tac2)?0:(-1));
        goto end;
      }
      if(p2==getArity(tac2)) {
        r=(1);
        goto end;
      }

      fastTermCmp(getSubterm(tac1,p1),getSubterm(tac2,p2));
      if(r!=INITVALUE) {
        if(r!=0) {
          goto end;
        }
      } else if((r=intern_term_cmp(getSubterm(tac1,p1),getSubterm(tac2,p2)))) {
        goto end;
      }
        // on peut creer du partage ici
      setSubterm(tac2,p2,getSubterm(tac1,p1));

      if(r = getMult(tac1,p1)-getMult(tac2,p2)) {
        goto end;
      }
    }
  }
  r=(0);
  end:
  return r;
}

int term_cmp(struct term *t1, struct term *t2) {
  int r, arity1;

#ifdef DEBUG
  cptCmpTotal++;
#endif

  fastTermCmp(t1,t2);
  if(r==INITVALUE) {
    r=intern_term_cmp(t1,t2);
  } else {
      // printf("fast OK !\n");
    ;
  }
  end:
#ifdef DEBUG
  if(r==0)
    cptCmpEqual++;
#endif
  return r;
}
*/


long term_notDestructEqual(struct term *t1,struct term *t2) {
  int arity;
  
  if(t1==t2) return(1);

    //printf("t1 = "); term_printnl(stdout,t1);
    //printf("t2 = "); term_printnl(stdout,t2);

  if(isTagged(t1) || isTagged(t2)) {
      /*
       * [pem: Jun 23 99] : ne sert a rien car teste precedemment
       */
    if(isIntegerTagged(t1) || isIntegerTagged(t2) ||
       isIdentifierTagged(t1) || isIdentifierTagged(t2) ) {
      return t1==t2;
    } else
      
      if(isStringTagged(t1) || isStringTagged(t2)) {
        if(isStringTagged(t1) && isStringTagged(t2)) {
          return strcmp(getString(t1),getString(t2));
        } else {
          printf("error in term_notDestructEqual\n");
          exit(1);
        }
      } 
  } else if(getSymb(t1) != getSymb(t2)) {
    return (0);
  }

  if ((arity=term_arity(t1))==0) return(1);

  if(!term_isAC(t1)) {
    register int i;
    for(i=0 ; i<arity ; i++) {
      if(!term_notDestructEqual(t1->sub[i], t2->sub[i]))
	return (0);
    }
    return (1);
  } else {
    register int p1, p2;
    struct termac *tac1=(struct termac*)t1;
    struct termac *tac2=(struct termac*)t2;
    for(p1=0, p2=0; ; p1++, p2++) {
      if(p1==getArity(tac1)) return(p2==getArity(tac2)?1:0);
      if(p2==getArity(tac2)) return(0);
      if(!term_notDestructEqual(getSubterm(tac1,p1), getSubterm(tac2,p2)))
        return (0);
      if(getMult(tac1,p1) != getMult(tac2,p2)) return (0);
    }  
    return (1);
  }
}


/*
 * Replace t2 by t3 in t1
 */

#ifdef __cplusplus
typedef struct term* (*funTabType)(...);
#else
typedef struct term* (*funTabType)();
#endif
extern funTabType funTab[];
extern funTabType strTab[];

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

struct term* specialApply(struct term *res) {
  int fsym = getSymb(res);
  struct term **arg=res->sub;
  if(funTab[fsym]==NULL) return res;

    //printf("specialApply : arity = %d\n",term_arity(res));
    //printf("t = "); term_println(stdout,res);
    
  switch(term_arity(res)) {
      // pour le cas AC
      case -1: return funTab[fsym](res);
      case 0:  return funTab[fsym](funTabCall0);
      case 1:  return funTab[fsym](funTabCall1);
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

struct term* normalise(struct term *t) {
  int code, arity, i;
  
  if(isIntegerTagged(t) || isIdentifierTagged(t) || isStringTagged(t)) {
    return t;
  }

  code  = getSymb(t);
  arity = term_arity(t);
    //printf("Normalise\tcode=%d\tarity=%d\n",code,arity);
  if(!term_isAC(t)) {
    for(i=0 ; i<arity ; i++) {
      t->sub[i]=normalise(t->sub[i]);
    }
  } else {
    struct termac *tac=(struct termac*)t;
    struct term *nt;
    int computeONF=0;
    
    for(i=0 ; i<getArity(tac) ; i++) {
      nt = normalise(getSubterm(tac,i));
        //computeONF |= (nt!=getSubterm(tac,i));
      setSubterm(tac,i,nt);
    }
      // [pem: May 31 00]
      // il faut trier les sous-termes de t
    if(1) { //computeONF) {
      struct termac *newtac = NULL;
      int i,j;
        // n'est plus utile
        // TERMAC_ALLOC(newtac,getArity(tac),getSymb(tac));
      for(i=0 ; i<getArity(tac) ; i++) {
        for(j=0 ; j<getMult(tac,i) ; j++) {
          newtac = term_add_onf_term_color(newtac,getSymb(tac),getSubterm(tac,i),getColor(tac,i));
        }
      }
      t=(struct term*) newtac;
    }
  }
    //printf("normalize: "); term_println(stdout,t);
  t=specialApply(t);
    //printf("result:    "); term_println(stdout,t);
    //printf("end normalise\n");
  return t;
}


/*
 * Replace t2 by t3 in t1
 */
static struct term *term_rec_replace(struct term *t1,
				     struct term *t2,
				     struct term *t3)
{
  int i,j,arity;
  struct term *res;
  int renormalise=0;

  if(term_notDestructEqual(t1,t2)) {
    return t3;
  }
    //if(isTagged(t1)) return t1;
  if(isIntegerTagged(t1) || isIdentifierTagged(t1) || isStringTagged(t1)) {
    return t1;
  }

  arity=term_arity(t1);
  if(arity==0) return t1;
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
    struct term *tmp;
      /*
       * Il y a un probleme dans le marquage du partage qui n'est pas recursif
       * Faut-il supprimer cette optimisation ou faire un marquage recursif ?
       * Pour le moment on supprime l'optimisation
       */
      // Il faut dupliquer le symbole de tete
    res=(struct term*)MALLOC(sizeof(struct term)+((arity-2)*sizeof(struct term*)));
    setSymb(res,getSymb(t1));
    for(i=0 ; i<arity ; i++) {
      tmp = term_rec_replace(t1->sub[i],t2,t3);
        //      if(tmp != res->sub[i]) {
      res->sub[i] = tmp;
      if(tmp != t1->sub[i]) {
          //printf("syntactic renormalise\n");
	renormalise=1;
      }
    }
  } else {
    struct term *tmp;
    struct termac *resac;
    struct termac *tac1=(struct termac*) t1;
    
        
      //printf("not yet implemented\n");
      //exit(0);
    TERMAC_ALLOC(resac,getArity(tac1),getSymb(tac1));
    for(i=0 ; i<getArity(tac1) ; i++) {
      tmp = term_rec_replace(getSubterm(tac1,i),t2,t3);
        // Attention a la multiplicite et a la couleur
      for(j=0 ; j<getMult(tac1,i) ; j++)
	resac = term_add_onf_term(resac,getSymb(tac1),tmp);
      if(tmp != getSubterm(tac1,i)) {
          //printf("AC renormalise\n");
	renormalise=1;
      }
    }
    res=(struct term*)resac;
  }
  
    /*
     * re-normalisation
     */
  if(renormalise) {
      /*
        printf("start renormalise\n");
        printf("arity = %d\tfsym = %d\n",arity,getSymb(res));
        printf("term = "); term_printnl(stdout,res);
      */
    res=specialApply(res);
      /*
        printf("term = "); term_printnl(stdout,res);;
        printf("end renormalise\n");
      */
  }

    //printf("res = "); term_printnl(stdout,res);
  return res;
}

struct term *term_replace(struct term *t1,struct term *t2,struct term *t3)
{
    /*
     * Replace t2 by t3 in t1
     */
  if(term_occur(t1,t2)) {
    return term_rec_replace(t1,t2,t3);
  } else {
    return t1;
  }
}


/*
 * t2 occurs in t1
 */
int term_occur(struct term *t1,struct term *t2)
{
  int i,arity;

    //term_print(stdout,t2); printf(" occurs in "); term_printnl(stdout,t1);
    //printf("isAC: %d\tarity = %d\n",term_isAC(t1),arity);

  if(term_notDestructEqual(t1,t2)) return 1;
    //if(isTagged(t1)) return 0;
  if(isIntegerTagged(t1) || isIdentifierTagged(t1) || isStringTagged(t1)) {
    return 0;
  }

  arity=term_arity(t1);
  if (arity==0) return 0;

  if(!term_isAC(t1)) {
    for(i=0 ; i<arity ; i++) {
      if(term_occur(t1->sub[i],t2)) return 1;
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
  for(i=0 ; i<getArity(subterm) ; i++) {
    setSubterm(tac,i,getSubterm(subterm,i));
    setColorMult(tac,i,getColor(subterm,i),getMult(subterm,i));
  }
}

/*
 * transform F(t) into t
 * returns NULL if the term is ok
 */
struct term *term_removeTopSymbol(struct term *t) {
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
      return (struct term*) NULL;
    }
  }
}

struct term *term_metaApply(struct term *t) {
  struct term *strategy;
  struct term *list;
  struct term *mainTerm;
  struct term *nil;
  struct term *strategyNumber;
  int index;
  int start, end, nbSol;
  int all=0;
  int *counter=(int*) allocStable(sizeof(int));

  struct term *applyRes, *newRes;
  struct term ***ptr_oldRes=(struct term***)allocStable(sizeof(struct term **));
  struct term **listRes=(struct term**)allocStable(sizeof(struct term *));
  *counter=0;

    //printf("------------------------------------------------------------\n");
    //printf("symb = %d\n",getSymb(t));
  switch(getSymb(t)) {
      case 128:
          /*
            printf("t   : "); term_printnl(stdout,t);
          */
        strategy=t->sub[0];
        list=t->sub[1];
        start = getInt(t->sub[2]);
        nbSol = getInt(t->sub[3]);

        strategyNumber=strategy->sub[0]->sub[0];
        mainTerm=list->sub[0];
        nil=list->sub[1];
          /* 
             printf("list     : "); term_printnl(stdout,list);
             printf("nil      : "); term_printnl(stdout,nil);
             printf("mainTerm : "); term_printnl(stdout,mainTerm);
             printf("number   : "); term_printnl(stdout,strategyNumber);
          */
        if(isIntegerTagged(strategyNumber)) {
          index=getInt(strategyNumber);
        } else {
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

              TERM_ALLOC(newRes,term2,getSymb(list));
              newRes->sub[0]=applyRes;
              (**ptr_oldRes)=newRes;
              *ptr_oldRes=&(newRes->sub[1]);

                //printf("\nres=%d\tnewRes=%d\n",*listRes,newRes);

            } else {
                // C'est fini
                //printf("C'est fini\n");
                //(**ptr_oldRes)=nil;
              CUTCLOSE();
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
  
    //printf("RESULT = "); term_printnl(stdout,*listRes);
    //*listRes=t;
  return *listRes;
}


listTerm *listTermCreate(struct term *term) {
  listTerm *res;
  res=(listTerm*) MALLOC(sizeof(listTerm));
  res->term=term;
  res->next=NULL;
  res->last=res;
  return res;
}

listTerm *addTermListTerm(listTerm *list , struct term *term) {
    /* insertion en queue */
  listTerm *res;
  res=listTermCreate(term);
  if(list==NULL)
    return res;
  list->last->next=res;
  list->last=res;
  return list;
}

/*
static struct term *internNull=NULL;
struct term *asfNull() {
  if(internNull!=NULL)
    return internNull;
  TERM_ALLOC(internNull,term2,193);
  return internNull;
}

struct term *asfCons(struct term *t1,struct term *t2) {
  struct term *res=NULL;

  printf("t1 = "); term_printnl(stdout,t1);
  printf("t2 = "); term_printnl(stdout,t2);

  if(term_isAC(t1)) {
    printf("t1 is AC\n");
    res = term_add_list_term(t1,t2);

  } else {
    printf("t1 is not AC\n");

    TERM_ALLOC(res,term2,193);
    res = term_add_list_term(res,t1);

    printf("res = "); term_printnl(stdout,res);

    res = term_add_list_term(res,t2);
  }
  return res;
}

struct term *asfHead(struct term *t) {
  if(term_first(t)==NULL) {
    printf("asfHead: empty list\n");
    exit(1);
  }
  return cell_t(term_first(t));
 }

struct term *asfTail(struct term *t) {
  struct term *res;
  if(term_first(t)==NULL) {
    printf("asfTail: empty list\n");
    exit(1);
  } else if(term_first(t)==term_last(t)) {
    //printf("asfTail: single element\n");
    //term_printnl(stdout,t);
    return asfNull();
  }
  TERM_ALLOC(res,term2,getSymb(t));
  term_first(res)=cell_next(term_first(t));
  term_last(res)=term_last(t);
  return res;
}

struct term *asfPrefix(struct term *t) {
  struct term *res;
  struct cell_term *cell;
  struct cell_term *copy_cell;

  if(term_first(t)==NULL || term_first(t)==term_last(t)) {
    printf("asfPrefix: empty list or single element\n");
    term_printnl(stdout,t);
    return asfNull();
  }

  TERM_ALLOC(res,term2,getSymb(t));
  for(cell=term_first(t) ; cell!=term_last(t) ; cell=cell_next(cell))
    {
      CELL_ALLOC(copy_cell);
      CELL_INIT(copy_cell);
      cell_t(copy_cell)=cell_t(cell);
      setMult(copy_cell, getMult(cell));
      cell_add_last(cell,res);
    }
  return res;
}

struct term *asfLast(struct term *t) {
  struct term *res=NULL;
  if(term_first(t)==NULL || term_last(t)==NULL) {
    printf("asfLast: empty list\n");
    exit(1);
  }
  res = cell_t(term_last(res));
  return res;
}

struct term *asfNotEmptyList(struct term *t) {
  if(term_first(t)==NULL && term_last(t)==NULL) {
    return bool2term(0);
  } else {
    return bool2term(1);
  }
}

struct term *asfIsSingleElement(struct term *t) {
  if(term_first(t)!=NULL && term_first(t)==term_last(t)) {
    return bool2term(1);
  } else {
    return bool2term(0);
  }
}
*/

#ifdef COLOR
int isMonoColor(struct term *t) {
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

void setMonoColor(struct term *t) {
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

struct termac *CC_GC_encode(struct termac *tac) {
    //return (struct termac*) ~((unsigned long)tac);
  return tac;
}

struct termac *CC_GC_decode(struct termac *tac) {
    //return (struct termac*) ~((unsigned long)tac);
  return tac;
}

/*
 * Array
 */
struct term *term_newArray(int n, struct term *t) {
  struct term **res;
  int i;
  res = (struct term**) MALLOC((n+1) * sizeof(struct term*));
  res[0] = (struct term*)n;
  for(i=0 ; i<n ; i++) {
    res[1+i] = t;
  }
  return (struct term*) res;
}

struct term *term_getArray(struct term *array, int n) {
  int size = (int) ((struct term**)array)[0];
  if(n<0 || n>size-1) {
    printf("getArray error: size = %d\tn = %d\n",size,n);
    exit(1);
  }
  return ((struct term**)array)[n+1];
}

struct term *term_setArray(struct term *array, int n, struct term *t) {
  int size = (int) ((struct term**)array)[0];
  if(n<0 || n>size-1) {
    printf("setArray error: size = %d\tn = %d\n",size,n);
    exit(1);
  }
  ((struct term**)array)[n+1] = t;
  return array;
}
