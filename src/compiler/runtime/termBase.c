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
#include <stdint.h>
#include "termBase.h"
#include "builtin.h"
#include "termIn.h"
#include "termOut.h"

void term_alloc(Gterm **ptr_dest, int size_sname, unsigned int funsym) {
  Gterm *dest;
  *ptr_dest=(Gterm*) MALLOC(size_sname);
    dest=*ptr_dest;
    GsetSymb(dest,funsym);
#ifdef DEBUG
cptTermAlloc++;
  /*      if(symb_isAC(funsym)) {
          printf("warning in term_alloc ac: use termac_alloc\n");
          }
  */
#endif
}



void Gterm_init(int argc,char **argv,long *ptr_bottomOfStack) {
}

void Gfsym_init(int code, int a, char *n, char *s,
                int sem, int dstrat,
                Gterm* (*semaction)(Gterm *)) {
  fsymtab[code].arity=a;
  fsymtab[code].name=n;
  fsymtab[code].sort=s;
  fsymtab[code].semantic=sem;
  fsymtab[code].modulo=0;
  fsymtab[code].defstrat=dstrat;
  fsymtab[code].semact=semaction;
  fsymtab[code].prec=code;  //[QUANG: Sep 19 01] initially precedence = code
}

void intern_Gmake_const(Gterm **ptr_dest,int code) {
  TERM_CONST_ALLOC((*ptr_dest),code);
}

//int *STRANGE_ADDRESS = 0x8c42010;
//int *STRANGE_ADDRESS = 0x0;
//int allocatedBug=0;

int intern_GgetSymb(Gterm *v1) {
  
    //int funsym = getSymb(v1);
    //if(funsym>1000) {
    //printf("GgetSymb strange: (%x) %d\n",v1,funsym);
    //printf("allocated bug\n");
      //allocatedBug=1;
      //STRANGE_ADDRESS = v1;
    //assert(0);
    //}

  return getSymb(v1);
}

void intern_GsetSymb(Gterm *v1,int funsym) {

    //if(funsym> 1000) {
    //printf("GsetSymb strange: (%x) %d\n",v1,funsym);
    // printf("allocated bug\n");
    //assert(0);
    //}
    setSymb(v1,funsym);
}

void intern_GmakeAppl0(Gterm **ptr_dest,int code) {
  TERM_ALLOC(*ptr_dest,term0,code);
}

void intern_GmakeAppl1(Gterm **ptr_dest,int code,Gterm *subterm) {
  TERM_ALLOC((*ptr_dest),term1,code);
  GsetArgument((*ptr_dest),0,subterm);
}

void intern_GmakeAppl2(Gterm **ptr_dest,int code,Gterm *subterm0
                       ,Gterm *subterm1) {
  TERM_ALLOC((*ptr_dest),term2,code);
  GsetArgument(*ptr_dest,0,subterm0);
  GsetArgument(*ptr_dest,1,subterm1);
}

void intern_GmakeAppl3(Gterm **ptr_dest,int code,Gterm *subterm0
                       ,Gterm *subterm1,Gterm *subterm2) {
  TERM_ARITY_ALLOC((*ptr_dest),3,code);
  GsetArgument(*ptr_dest,0,subterm0);
  GsetArgument(*ptr_dest,1,subterm1);
  GsetArgument(*ptr_dest,2,subterm2);
}

void intern_GmakeAppl4(Gterm **ptr_dest,int code,Gterm *subterm0
                       ,Gterm *subterm1,Gterm *subterm2
                       ,Gterm *subterm3) {
  TERM_ARITY_ALLOC((*ptr_dest),4,code);
  GsetArgument(*ptr_dest,0,subterm0);
  GsetArgument(*ptr_dest,1,subterm1);
  GsetArgument(*ptr_dest,2,subterm2);
  GsetArgument(*ptr_dest,3,subterm3);
}

void intern_GmakeAppl5(Gterm **ptr_dest,int code,Gterm *subterm0
                       ,Gterm *subterm1,Gterm *subterm2
                       ,Gterm *subterm3,Gterm *subterm4) {
  TERM_ARITY_ALLOC((*ptr_dest),5,code);
  GsetArgument(*ptr_dest,0,subterm0);
  GsetArgument(*ptr_dest,1,subterm1);
  GsetArgument(*ptr_dest,2,subterm2);
  GsetArgument(*ptr_dest,3,subterm3);
  GsetArgument(*ptr_dest,4,subterm4);
}

void intern_GmakeAppl6(Gterm **ptr_dest,int code,Gterm *subterm0
                       ,Gterm *subterm1,Gterm *subterm2
                       ,Gterm *subterm3,Gterm *subterm4
                       ,Gterm *subterm5) {
  TERM_ARITY_ALLOC((*ptr_dest),6,code);
  GsetArgument(*ptr_dest,0,subterm0);
  GsetArgument(*ptr_dest,1,subterm1);
  GsetArgument(*ptr_dest,2,subterm2);
  GsetArgument(*ptr_dest,3,subterm3);
  GsetArgument(*ptr_dest,4,subterm4);
  GsetArgument(*ptr_dest,5,subterm5);
}

void GmakeAppl(Gterm **ptr_dest,int code,int arity,...) {
  int i;
  Gterm *ArrayArgs[256];
  va_list args;
  va_start(args, arity);
  for(i=0; i<arity; i++) {
    ArrayArgs[i] = va_arg(args, Gterm *);
  }
  va_end(args);
  GmakeApplArray(*ptr_dest,code,ArrayArgs);
}

void intern_GmakeAppl_Array(Gterm **ptr_dest,int code,Gterm *ArrayArgs[]){
  int i, arity;
    //printf("\nin intern_GmakeAppl_Array t = %d",t);
  arity=code_arity(code);
  GmakeApplArity(*ptr_dest,arity,code);
  for(i=0 ; i<arity ; i++) {
    GsetArgument(*ptr_dest,i,ArrayArgs[i]);
  }
}

Gterm* intern_GgetArgument(Gterm* t,int pos) {
  return getFreeSubterm(t,pos);
}

void intern_GmakeAppl_Arity(Gterm **ptr_dest,int arity,int code) {
  TERM_ARITY_ALLOC((*ptr_dest),arity,code);
}

int GlistIsEmpty(GtermList *L) {
  return(!(L));
}

Gterm *GlistGetHead(GtermList *list) {
  return list->term;
}

void intern_GsetArgument(Gterm **ptr_dest,int pos, Gterm *t) {
  setFreeSubterm(*ptr_dest,pos,t);
}

GtermList *GlistGetTail(GtermList *list) {
  return list->next;
}


long term_notDestructEqual(register Gterm *t1,register Gterm *t2) {
  register int arity;

  if(t1==t2) return(1);

    //printf("t1 = "); term_printnl(stdout,t1);
    //printf("t2 = "); term_printnl(stdout,t2);

  if(GisTagged(t1) || GisTagged(t2)) {
      // [pem: Jun 23 99] : ne sert a rien car teste precedemment
    if(GisIntegerTagged(t1) || GisIntegerTagged(t2) ||
       GisIdentifierTagged(t1) || GisIdentifierTagged(t2) ) {
      return t1==t2;
    } else if(GisStringTagged(t1) || GisStringTagged(t2)) {
      if(GisStringTagged(t1) && GisStringTagged(t2)) {
        return strcmp((char*)GgetString(t1),(char*)GgetString(t2));
      } else {
        printf("error in term_notDestructEqual\n");
        exit(1);
      }
    }
  } else if(GgetSymb(t1) != GgetSymb(t2)) {
    return (0);
  }


  if(hashTerm(t1) != hashTerm(t2)) {
      //printf("equal: hcode %d != %d\n",hashTerm(t1),hashTerm(t2));
      //printf("\tt1 = "); term_println(stdout,t1);
      //printf("\tt2 = "); term_println(stdout,t2);
    return (0);
  }

  if((arity=term_arity(t1))==0) {
    return(1);
  }

  if(!term_isAC(t1)) {
    int i;
    for( i=0 ; i<arity ; i++) {
      if(!term_notDestructEqual(GgetArgument(t1,i), GgetArgument(t2,i))) {
        return(0);
      }
    }
    return (1);
  } else {
    register int p1, p2;
    struct termac *tac1=(struct termac*)t1;
    struct termac *tac2=(struct termac*)t2;
    for(p1=0, p2=0; ; p1++, p2++) {
      if(p1==getArity(tac1)) return(p2==getArity(tac2)?1:0);
      if(p2==getArity(tac2)) return(0);
      if(!term_notDestructEqual(getSubterm(tac1,p1),getSubterm(tac2,p2) ))
        return (0);
      if(getMult(tac1,p1) != getMult(tac2,p2)) return (0);
    }
    return (1);
  }
}


/*
 *	Compare flattened/sorted terms using lexicographic order for
 *	argument list of free function symbols and multiset order for
 *	argument lists of AC function symbols.
 */



int Gterm_cmp(register Gterm *t1, register Gterm *t2) {
  register int r, arity1;

  if(t1==t2) {
    return (0);
  }

  if(GisTagged(t1)) {
    if(GisIntegerTagged(t1)) {
      if(GgetInt(t1) != GgetInt(t2)) {
        return(GgetInt(t1) - GgetInt(t2));
      } else {
        printf("error in term_cmp\n");
        exit(1);
      }
    } else if(GisIdentifierTagged(t1)) {
      return (GgetIdentifier(t1) - GgetIdentifier(t2));
    } else if(GisStringTagged(t1)) {
      return strcmp((char*)GgetString(t1),(char*)GgetString(t2));
    }
  }

    /*
      printf("t1 = "); term_printnl(stdout,t1);
      printf("t2 = "); term_printnl(stdout,t2);
      printf("------------------------------\n");
    */

   //[QUANG: Sep 19 01]  use precedence to compare symbols
  if (fsymtab[GgetSymb(t1)].prec != fsymtab[GgetSymb(t2)].prec) {
      return (fsymtab[GgetSymb(t1)].prec - fsymtab[GgetSymb(t2)].prec);
  }
  // instead of
  //if(GgetSymb(t1) != GgetSymb(t2)) {
  //  return(GgetSymb(t1) - GgetSymb(t2));
  //}
  if((arity1=term_arity(t1))==0) {
    return(0);
  }


  if(!term_isAC(t1)) {
      /* lexicographic ordering on subterms */
    int i;
    for( i=0 ; i<arity1 ; i++) {
      if((r = Gterm_cmp(GgetArgument(t1,i), GgetArgument(t2,i)))) {
        return(r);
      }
    }
     /*
        #ifdef HCODE
        if(getHcode(t1) != getHcode(t2)) {
          //printf("cmp_0: hcode %d != %d\n",getHcode(t1),getHcode(t2));
          return getHcode(t1) - getHcode(t2);
          }
          #endif
      */
    return 0;
      /*
       * on peut creer du partage ici
       */
  } else {
      /* multiset ordering on subterms */
    register int p1, p2;
    struct termac *tac1=(struct termac*)t1;
    struct termac *tac2=(struct termac*)t2;
      /*
        #ifdef HCODE
        if(getHcode(t1) != getHcode(t2)) {
        printf("cmp_AC: hcode %d != %d\n",getHcode(t1),getHcode(t2));
          //printf("t1 = "); term_println(stdout,t1);
            //printf("t2 = "); term_println(stdout,t2);
            return getHcode(t1) - getHcode(t2);
            }
            #endif
      */

    for(p1=0, p2=0; ; p1++, p2++) {
      if(p1==getArity(tac1)) return(p2==getArity(tac2)?0:(-1));
      if(p2==getArity(tac2)) return(1);
      if((r = Gterm_cmp(getSubterm(tac1,p1), getSubterm(tac2,p2)))) {

          /*
            #ifdef HCODE
              // just to verify
              if(r==0 && getHcode(t1) != getHcode(t2)) {
              printf("warning: hcode %d != %d\n",getHcode(t1),getHcode(t2));
              printf("t1 = "); term_println(stdout,t1);
              printf("t2 = "); term_println(stdout,t2);
              exit(1);
              }
              #endif
          */
          /*
        if(r!=0) {
          printf("missed\n");
        }
          */
        return(r);
      }
        /*
         * on peut creer du partage ici
         */
      setSubterm(tac2,p2,(Gterm *) getSubterm(tac1,p1));

      if(getMult(tac1,p1) < getMult(tac2,p2)) {
          //printf("missed\n");
        return(-1);
      }
      if(getMult(tac1,p1) > getMult(tac2,p2)) {
          //printf("missed\n");
        return(1);
      }
    }
  }
    /*
      #ifdef HCODE
        // just to verify
        if(getHcode(t1) != getHcode(t2)) {
        printf("warning: hcode %d != %d\n",getHcode(t1),getHcode(t2));
        printf("t1 = "); term_println(stdout,t1);
        printf("t2 = "); term_println(stdout,t2);
        exit(1);
        }
        #endif
    */
  return(0);
}


/*
 * Array
 */

Gterm *term_newArray(int n, Gterm *t) {
  Gterm **array;
  Gterm *res;
  int i;
  array = (Gterm**) MALLOC(n * sizeof(Gterm*));

  GmakeAppl2(res,CODE_ARRAY,((Gterm*)(intptr_t)n), ((Gterm*)array)); 
  for(i=0 ; i<n ; i++) {
    array[i] = t;
  }
  return res;
}

Gterm *term_getArray(Gterm *t, int n) {
  Gterm **array;
  int size;
  if(GgetSymb(t) != CODE_ARRAY) {
    printf("getArray error: symb = %d\n",GgetSymb(t));
    exit(1);
  }
  
  size  = (int)(intptr_t) GgetArgument(t,0);
  array = (Gterm**) GgetArgument(t,1);
  if(n<0 || n>=size) {
    printf("getArray error: size = %d\tn = %d\n",size,n);
    exit(1);
  }
  return array[n];
}

Gterm *term_setArray(Gterm *t, int n, Gterm *subterm) {
  Gterm **array;
  int size;
  if(GgetSymb(t) != CODE_ARRAY) {
    printf("setArray error: symb = %d\n",GgetSymb(t));
    exit(1);
  }

  size  = (int)(intptr_t) GgetArgument(t,0);
  array = (Gterm**) GgetArgument(t,1);
  if(n<0 || n>=size) {
    printf("setArray error: size = %d\tn = %d\n",size,n);
    exit(1);
  }
  array[n] = subterm;
  return t;
}

int term_getLength(Gterm *t) {
  if(GgetSymb(t) != CODE_ARRAY) {
    printf("getLength error: symb = %d\n",GgetSymb(t));
    exit(1);
  }
  return (int)(intptr_t) GgetArgument(t,0);
}

/*
 * String
 */
Gterm *term_newString(char *string) {
  Gterm *res;
  int n;
  n = strlen(string);
  TERM_ALLOC(res,term2,CODE_STRING);
  setFreeSubterm(res,0,(struct term*)(intptr_t) n);
  setFreeSubterm(res,1,(struct term*) string);
  return res;
}

char *term_getString(Gterm *t) {
  char *string;
  int size;
  size = (int)(intptr_t) getFreeSubterm(t,0);
  string = (char *) MALLOC((size+1)*sizeof(char));
  strcpy(string,(char *)getFreeSubterm(t,1));
    //printf("%s",string);
  return string;
}





#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

int hashTerm(Gterm *t) {
  int hash;

  if(GisTagged(t)) {
    return (int)(intptr_t)t & HASHMASK;
  }

  hash = getHcode(t);
    //printf("hashTerm: t = "); term_println(stdout,t);
  if(hash != HASHMASK) {
      //printf("t = "); term_print(stdout,t); printf(" previous hcode = %d\n",hash);
    return hash;
  } else {
    hash = doobs_hfunction(t) & HASHMASK;
    setHcode(t,hash);
      //printf("t = "); term_print(stdout,t); printf(" computed hcode = %d\n",hash);
      //printf("getHcode(%x) = %d\n",t,getHcode(t));
    return hash;
  }
} 

int doobs_hfunction(Gterm *t) {

    
  unsigned long int initval; /* the previous hash value */
  register unsigned long int a,b,c;
  unsigned long int len, arity;
  int k=0;

  if(!term_isAC(t)) {
      /* Set up the internal state */
    initval = GgetSymb(t);
    len = arity = term_arity(t);
    a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
    c = initval;         /* the previous hash value */
    
      /*---------------------------------------- handle most of the key */
    while(len >= 12) {
      a +=
        (hashTerm(GgetArgument(t,k+0))) +
        ((hashTerm(GgetArgument(t,k+1)))<<8) +
        ((hashTerm(GgetArgument(t,k+2)))<<16) +
        ((hashTerm(GgetArgument(t,k+3)))<<24);
      b += (hashTerm(GgetArgument(t,k+4))) +
        (hashTerm(GgetArgument(t,k+5))<<8) +
        (hashTerm(GgetArgument(t,k+6))<<16) +
        (hashTerm(GgetArgument(t,k+7))<<24);
      c += (hashTerm(GgetArgument(t,k+8))) +
        (hashTerm(GgetArgument(t,k+9))<<8) +
        (hashTerm(GgetArgument(t,k+10))<<16) +
        (hashTerm(GgetArgument(t,k+11))<<24);
      mix(a,b,c);
      k += 12;
      len -= 12;
    }
    
      /*------------------------------------- handle the last 11 bytes */
    c += arity;
    switch(len)              /* all the case statements fall through */
    {
        case 11: c+=(hashTerm(GgetArgument(k+t,10)))<<24;
        case 10: c+=(hashTerm(GgetArgument(k+t,9)))<<16;
        case 9 : c+=(hashTerm(GgetArgument(k+t,8)))<<8;
            // the first byte of c is reserved for the length 
        case 8 : b+=(hashTerm(GgetArgument(k+t,7)))<<24;
        case 7 : b+=(hashTerm(GgetArgument(k+t,6)))<<16;
        case 6 : b+=(hashTerm(GgetArgument(k+t,5)))<<8;
        case 5 : b+=hashTerm(GgetArgument(k+t,4));
        case 4 : a+=(hashTerm(GgetArgument(k+t,3)))<<24;
        case 3 : a+=(hashTerm(GgetArgument(k+t,2)))<<16;
        case 2 : a+=(hashTerm(GgetArgument(k+t,1)))<<8;
        case 1 : a+=hashTerm(GgetArgument(k+t,0));
            // case 0: nothing left to add
    }
  } else {
    struct termac *tac=(struct termac*)t;
      /* Set up the internal state */
    initval = GgetSymb(t);
    len = arity = getArity(tac);
    a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
    c = initval;         /* the previous hash value */
    
      /*---------------------------------------- handle most of the key */
    while(len >= 12) {
      a +=
        (hashTerm(getSubterm(tac,k+0))) +
        ((hashTerm(getSubterm(tac,k+1)))<<8) +
        ((hashTerm(getSubterm(tac,k+2)))<<16) +
        ((hashTerm(getSubterm(tac,k+3)))<<24);
      b += (hashTerm(getSubterm(tac,k+4))) +
        (hashTerm(getSubterm(tac,k+5))<<8) +
        (hashTerm(getSubterm(tac,k+6))<<16) +
        (hashTerm(getSubterm(tac,k+7))<<24);
      c += (hashTerm(getSubterm(tac,k+8))) +
        (hashTerm(getSubterm(tac,k+9))<<8) +
        (hashTerm(getSubterm(tac,k+10))<<16) +
        (hashTerm(getSubterm(tac,k+11))<<24);
      mix(a,b,c);
      k += 12;
      len -= 12;
    }
    
      /*------------------------------------- handle the last 11 bytes */
    c += arity;
    switch(len)              /* all the case statements fall through */
    {
        case 11: c+=(hashTerm(getSubterm(tac,k+10)))<<24;
        case 10: c+=(hashTerm(getSubterm(tac,k+9)))<<16;
        case 9 : c+=(hashTerm(getSubterm(tac,k+8)))<<8;
            /* the first byte of c is reserved for the length */
        case 8 : b+=(hashTerm(getSubterm(tac,k+7)))<<24;
        case 7 : b+=(hashTerm(getSubterm(tac,k+6)))<<16;
        case 6 : b+=(hashTerm(getSubterm(tac,k+5)))<<8;
        case 5 : b+=hashTerm(getSubterm(tac,k+4));
        case 4 : a+=(hashTerm(getSubterm(tac,k+3)))<<24;
        case 3 : a+=(hashTerm(getSubterm(tac,k+2)))<<16;
        case 2 : a+=(hashTerm(getSubterm(tac,k+1)))<<8;
        case 1 : a+=hashTerm(getSubterm(tac,k+0));
            /* case 0: nothing left to add */
    }
  }
      //printf("a = %d\tb = %d\tc = %d\n",a,b,c);
    
    mix(a,b,c);
      /*-------------------------------------------- report the result */
      //printf("initval = %d\tlen = %d\n",initval,len);
      //printf("doobs_hashFuntion: t = "); term_print(stdout,t); printf(" ---> %d\n",(c&0x0000FFFF));  
    
   return c;
}

GtermList *GlistTermCreate(Gterm *term) {
	GtermList *res;
	res=(GtermList*) MALLOC(sizeof(GtermList));
        res->term=term;
        res->next=NULL;
        res->last=(GtermList*)res;
        return res;
}

GtermList *GaddTermListTerm(GtermList *list , Gterm *term) {
    /* insertion en queue */
  GtermList *res;
  res=GlistTermCreate(term);
  if(list==NULL)
    return res;
  list->last->next=res;
  list->last=res;
  return list;
}






/*
 * Modifier a partir d'ici
 */
/*
extern int fsymtabSize;

int getSymbolIndex(char *symbolName) {
  int i,symbolIndex;

  for(i=0, symbolIndex=0 ; symbolIndex==0 && i<fsymtabSize ; i++) {
    if((strlen(fsymtab[i].name)==strlen(symbolName)) &&
       !strcmp(symbolName,fsymtab[i].name)) {
      symbolIndex = i;
    }
  }
  if(symbolIndex==0) {
    printf("Symbol '%s' not found\n",symbolName); 
    exit(1);
  }

  return symbolIndex;
}
*/






#include "SmilesGasEl/datatypes/datatypes.c"
#include "SmilesGasEl/datatypes/molecule.c"
#include "SmilesGasEl/parser/parser_common.c"
#include "SmilesGasEl/parser/parser_mol.c"
#include "SmilesGasEl/parser/parser_str.c"
#include "SmilesGasEl/algo/huxu/huxu.c"
#include "SmilesGasEl/algo/usmiles/usmiles.c"





/*
 * earleyRes, earleyKind and earleyPos are initialized in runtimeInit.cc
 *
extern int earleyRes[];
extern int earleyKind[];
extern int earleyPos;
extern char *earleyString[] ;
*/


int isEqMolecule(Gterm *m1, Gterm *m2) {
  int result;
  /*
   * mettre le code ici
   */

  /*
  int e;
  */

  char str_m1[256];
  char str_m2[256];

  /*  Gterm *m1_bis;
  Gterm *m2_bis;
  */

  FILE *tmp_file;

  /* char *sort = NULL;
  int earleySort;
  */


  /*
   * Recuperer une chaine a partir d'un terme
   */

  tmp_file = tmpfile();
  termOut(tmp_file, term_unflatten (m1));
  rewind (tmp_file);
  fgets(str_m1,255,tmp_file); 
  fclose (tmp_file);

  tmp_file = tmpfile();
  termOut(tmp_file, term_unflatten (m2));
  rewind (tmp_file);
  fgets(str_m2,255,tmp_file); 
  fclose (tmp_file);

  /*
   * Recuperer un terme a partir d'une chaine
   *
  sort = fsymtab[GgetSymb(m1)].sort ;
  earleyInit() ;

  tmp_file = fopen("tmp_smile_file","w");
  termOut(tmp_file,m1);
  fclose(tmp_file);

  if(sort == NULL) {
    printf("term_read error\n");
    exit(1);
  } else {
    earleySort = findTab(tabSort,TABOFSORT_SIZE,sort);
    if(earleySort == -1) {
      printf("term_read error: sort not found\n");
      exit(1);
    }
  }

  tmp_file = fopen("tmp_smile_file","r");
  e = earleyCall(tmp_file,earleySort);
  fclose(tmp_file);

  if(e) {
    m1_bis = computeTerm(&earleyPos);
    termOut(stdout,m1_bis);
  } else {     
    printf("term_read error: earleyCall failed\n");
    exit(1);
  }

  return term_notDestructEqual(m1,m1_bis);
  */







  //result = HUandXUCompare (str_m1, str_m2);

  result = USmilesGasElCompare (str_m1, str_m2);
  //fprintf(stderr,"compare(%s,%s) => %d\n",str_m1,str_m2, result);

  return result;
}
