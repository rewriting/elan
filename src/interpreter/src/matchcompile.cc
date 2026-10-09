/*
  
    ELAN

    Copyright (C) 1994-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
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

    Peter Borovansky		e-mail: borovan@fmph.uniba.sk
    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr
    Marian Vittek		e-mail: vittek@guma.ii.fmph.uniba.sk

*/

#include "termdefs.h"
#include "module.h"
#include "compiledefs.h"

#define MAXOK 0xffffffff
/*
#ifdef PEM
#ifdef __DECCXX 
#define MAXOK 0xffffffffffffffff
#elif defined(__alpha__)
#define MAXOK 0xffffffffffffffff
#endif
#endif
*/
// #include "tabident.h"

static int deep=0;
unsigned long mask;
/*static unsigned long full;*/
static struct matchhash {
  unsigned long okset;
  int label;
  struct rtna **stack;
  int stacki;
} hashtab[MAXNOFRULECOMBINE];

static int matchlabel=0;

//int variableaff=1;

//#define allocrtn(n,a) (struct rtnode *)calloc(n,HRTSIZE+a*SRTSIZE)

/* --------- functions for hash ------------ */

static void inithash()
{ int i;
  for(i=0;i<MAXNOFRULECOMBINE;i++) hashtab[i].okset = 0;
}

static unsigned hashind;

static int ismemberhash(unsigned long ok,struct rtna **st,int sti,int *lab)
{ unsigned long p;
  int i;
  p = hashind = ok%MAXNOFRULECOMBINE;
  while (hashtab[hashind].okset) {
    if (hashtab[hashind].okset == ok && hashtab[hashind].stacki == sti) {
      for(i=0;i<sti;i++) if (st[i]!=hashtab[hashind].stack[i]) break;
      if (i==sti) {
	*lab = hashtab[hashind].label;
	return(1);
      }
    }
    hashind=(hashind+127)%MAXNOFRULECOMBINE;
    if (hashind==p) {
      fprintf(stderr,"[error]matchcompile.c: hash table overflowed over MAXNOFRULECOMBINE == %d\n",MAXNOFRULECOMBINE);
      interr();
    }
  }
  return(0);
}

static void addnotmembertohash(unsigned long ok,struct rtna **st,int sti,int lab)   
// has to be called immediatedly after ismember, because of using hashind
{ int i;
  hashtab[hashind].okset = ok;
  hashtab[hashind].label = lab;
  hashtab[hashind].stacki = sti;
  hashtab[hashind].stack = new struct rtna *[sti];
  for(i=0;i<sti;i++) hashtab[hashind].stack[i] = st[i];
}

static void freehash()
{ int i;
  for(i=0;i<MAXNOFRULECOMBINE;i++) 
    if (hashtab[hashind].okset) delete hashtab[hashind].stack;
}




/* --------- recognition tree --------------- */

static struct rtlistn *founcorr(struct rtlistn * l,int s)
{ struct rtlistn * a;
//  if (s==Var) return(l);
  a=l->tail;
  while ((a!=NULL) && ((a->head->fsym) <s)) { l=a; a=a->tail; };
  if ((a!=NULL) && (a->head->fsym == s)) return(a);
  AALLOS(a,struct rtlistn);
  a->head = emptyrt();
  a->tail = l->tail;  l->tail=a;
  return(a);
}

/* addproto2(prt,t,0)
*/

void addproto2(struct rtnode * *prt,term *t,int fathervar)
{ int i,a,f,myvar;
  term *s;
  struct rtnode * l;
  /*struct rtlistn *rl;*/
  a= t->headarity(); 
  if (*prt==NULL) {
    AALLOS(*prt, struct rtnode);
    l= *prt;
    l->fsym = t->head(); 
    l->infos = t->inf();
    l->OKset=mask;
    AALLOSS(l->subrt,a,struct rtna);
    for (i=0;i<a;i++) {
      l->subrt[i].affvar = myvar = allocVarFirstFree();
      allocVarSetUsed(myvar);
      l->subrt[i].father = fathervar;
      l->subrt[i].fatheri = i;
      l->subrt[i].ELIMset = 0;
      l->subrt[i].nofSubTrees = 0;             // will be computed in makert
      l->subrt[i].setrt.head = emptyrt();
      l->subrt[i].setrt.tail = NULL;
      s = t->subterm(i);
      if (s->isofbuiltintype()) {
	if (s->inf() == TNORMFS && s->head()!=TRUEVAL && s->head()!=FALSEVAL) {
   	  if ((!Bins)) { // only if '-B' is not switched on
	    if (!batch) {				   
	      sterr << "[warning] \ta functional symbol"
		    << " of built-in type in the left hand side of a rule\n";
	      sterr << "[matchcompile]: \tthe rule is not compiled correctly\n";
	      actcruleerror();
	      sterr << "\n"; 
	    }
	    //	    exit(EXIT_FAILURE);
	  } else l->subrt[i].isbuiltins = 1;
	} else l->subrt[i].isbuiltins = 1;
      } else l->subrt[i].isbuiltins = 0;
    }
  } else {
    l= *prt; l->OKset=l->OKset | mask;
  };
  for (i=0; i<a; i++) {
    myvar = l->subrt[i].affvar;
    s = t->subterm(i);
    s->setcompilevar(myvar);
    f = s->head();
    if (s->inf()==TVAR)
      addproto2(&(l->subrt[i].setrt.head),s,myvar);
    else
      addproto2(&(founcorr(&(l->subrt[i].setrt),f)->head),s,myvar);
  };
}

static void addproto1(struct rtnode *prt,term *t)
{ int i,a,f,myvar;
  term *s;
  struct rtlistn * l;
  struct rtna *p;
  if (prt==NULL)
    return;
  f=prt->fsym; 
  if (prt->infos != TNORMFS)
    a=0;
  else
    a=fsymtab[f].arity();
  for (i=0; i<a; i++) {
    myvar = prt->subrt[i].affvar;
    allocVarSetUsed(myvar); 
    if (t->inf()!=TVAR)
      s = t->subterm(i);
    else
      s=t;
    f = s->head();
    l=&((p=(&(prt->subrt[i])))->setrt);
    if (s->inf()!=TVAR) {
      while ((l=l->tail)!=NULL && l->head->fsym!=f);
      if (l!=NULL)
	addproto1(l->head,s);
    } else {
      while ((l=l->tail)!=NULL)
	addproto1(l->head,s);
    }
  }
}

static void addproto0(struct rtnode *prt)
{ int i,a,f,myvar;
/*term *s;*/
  struct rtlistn * l;
  struct rtna *p;
  if (prt==NULL)
    return;
  f=prt->fsym; 
  if (prt->infos != TNORMFS)
    a=0;
  else
    a=fsymtab[f].arity();
  for (i=0; i<a; i++) {
    myvar = prt->subrt[i].affvar;
    allocVarSetUsed(myvar); 
    l=&((p=(&(prt->subrt[i])))->setrt);
    while ((l=l->tail)!=NULL)
      addproto0(l->head);
  }
}

void addproto(struct rtnode * *prt,term *t)
{
// first look for allocated variables
   allocVarInit();
   addproto0(*prt);          // !!!!!!!!!!!!!!! to remake seriously !!!!!!!!!!!!!!!!!
// secondly add term and allocate variables
   addproto2(prt,t,0);
}

/* call makert(rrt,0,0)
*/

void makert(struct rtnode *rrt,unsigned long oth)
{
  int i,a;
  unsigned long u,elim;
  struct rtlistn * l;
  struct rtna *p;
  if(rrt->infos != TNORMFS) {
    a=0;
  } else {
    a = fsymtab[rrt->fsym].arity(); 
  }
  elim=MAXOK;
  for (i=0;i<a;i++) {
    l=&((p=(&(rrt->subrt[i])))->setrt);
    p->nofSubTrees = 0;
    if (l->head==NULL) {
      AALLOS(l->head, struct rtnode);
      l->head->subrt = NULL;
      l->head->OKset=0;
    }
    l->head->fsym=Other;
    l->head->OKset= u = (l->head->OKset | oth);
    while ((l=l->tail) != NULL) {
      p->nofSubTrees ++;
      l->head->OKset = l->head->OKset | u;
      makert(l->head,u);
    }
    p->ELIMset= rrt->OKset & ~u;
    if (p->ELIMset & elim) {
      p->ELIMset=MAXOK;
    }
    elim=elim & (u?u:MAXOK);/* ale treba osetrit actunif=0 */
  }
}


/* call with writert(rrt,0) */

void writert(struct rtnode * rrt)
{ int a,i,f;
  struct rtlistn * l;
  struct rtna *p;
  f=rrt->fsym; 
  if (rrt->infos != TNORMFS) a=0;
  else a=fsymtab[f].arity();
  fprintf(stderr,"%d:%lo",f,rrt->OKset);
  if (a!=0) {
	fprintf(stderr,"(");
	for (i=0; i<a; i++) {
		l=&((p=(&(rrt->subrt[i])))->setrt);
		fprintf(stderr,"%o:(b?%d){",p->ELIMset,p->isbuiltins);
		writert(l->head);
		while ((l=l->tail)!=NULL) {
			fprintf(stderr,","); writert(l->head);
		};
		fprintf(stderr,"}");
	};
	fprintf(stderr,")");
  };
};


static char *genmatchvarprefix;
//static char *prefix;

void genmatchvarrec(FILE *ff,struct rtnode * rrt)
{ int a,i,f;
  struct rtlistn * l;
  struct rtna *p;

  f=rrt->fsym; 
  if (rrt->infos != TNORMFS) a=0;
  else a=fsymtab[f].arity();
  for (i=0; i<a; i++) {
    l=&((p=(&(rrt->subrt[i])))->setrt);
    if (allocVarIsUnused(p->affvar)) {
      fprintf(ff,"%s*v%d",genmatchvarprefix,p->affvar);
      allocVarSetUsed(p->affvar);
      genmatchvarprefix = ",";     
    }
    genmatchvarrec(ff,l->head);
    while ((l=l->tail)!=NULL) {
      genmatchvarrec(ff,l->head);
    };
  };
};

void genmatchvars(FILE *ff,struct rtnode * rrt)
{ int a,i,f;
  struct rtlistn * l;
  struct rtna *p;
  allocVarInit();
  f=rrt->fsym; 
  if (rrt->infos != TNORMFS) a=0;
  else a=fsymtab[f].arity();
  for (i=0; i<a; i++) {
    l=&((p=(&(rrt->subrt[i])))->setrt);
    genmatchvarrec(ff,l->head);
    while ((l=l->tail)!=NULL) {
      genmatchvarrec(ff,l->head);
    };
  };
};



void intend(FILE *ff,int i)
{ for(;i>0;i--) fprintf(ff,"  ");
}

int actnoruleapp;

static int actfunction,actisbuiltin,actisrecursion,actisfun;
static int actlab,actdontcare;
static struct rtnode *actrrt;
static int stacki=0;                        // can put into genexpmatch

// !!!!!!!!!!!!!! the same macro is in termcompile !!!!!!!!!!!!!!!
#define LEAVETRACEGEN(RESVAL) {\
  if (trace) {\
    intend(ff,deep); \
    fprintf(ff,"fprintf(stderr,\"[dump]!!! evalnorulequit } :: \");termwrite(");\
    RESVAL;fprintf(ff,",0);fprintf(stderr,\"\\176\\n\");\n");\
  }\
}


static void genconstructnoruleapp(FILE *ff,int deep)
{  int a,i;
   a = fsymtab[actfunction].arity();
   if (a==0) {
     LEAVETRACEGEN(fprintf(ff,"con%d",actfunction));
     fprintf(ff,"return(con%d);\n",actfunction);
   } else {
     intend(ff,deep);fprintf(ff,"{ struct term *norv;\n");
     intend(ff,deep);
     fprintf(ff,"ALLOC(norv,term%d,f%dlist,%d);\n",a,a,actfunction);
     intend(ff,deep);fprintf(ff,"norv->fs = %d;\n",actfunction);
     for(i=0; i<a; i++) {
       intend(ff,deep);fprintf(ff,"norv->sub[%d] = v%d;\n",i,actrrt->subrt[i].affvar);
     }
     if (actisrecursion) {
       intend(ff,deep);
       fprintf(ff,"*ares = norv; ");
       LEAVETRACEGEN(fprintf(ff,"res"));
       fprintf(ff,"return(res); }\n");
     } else {
       LEAVETRACEGEN(fprintf(ff,"norv"));
       intend(ff,deep);
       fprintf(ff,"return(norv); }\n");
     }
   }
}

/* returns 0 if head pattern can not be removed by farther matching */
/* returns 1 and set j to selected subpattern if it can             */


static unsigned long headbit(unsigned long a)
{ unsigned long m;
  if (!a) return(0);
  m=1;
  while (!(a&m)) m = m << 1;
  return(m);
}


static int selectSubtreeFromStack
(struct rtna **stack,int stacki,unsigned long ok,int /*dontcare*/,int *j,unsigned long *okk)
{ /*unsigned long condpref,elimm;*/
  int mini,minval,i;
  ok = headbit(ok);
  *okk = ok;
  if (ok==0) {
    fprintf(stderr,"[selectSubtreeFromStack] matchcompile.c "); interr();
  }
  mini = -1; minval = MAXINT;
  for (i=0;i<stacki;i++)
    if ((stack[i]->ELIMset & ok) &&
	stack[i]->setrt.head->OKset==0 && stack[i]->nofSubTrees <= minval) {
	mini = i; minval = stack[i]->nofSubTrees;
      }
  if (mini== -1) {
    for (i=0;i<stacki;i++)
      if ((stack[i]->ELIMset & ok) && stack[i]->nofSubTrees <= minval) {
	mini = i; minval = stack[i]->nofSubTrees;
      }
  }
  *j = mini;
  return(mini != -1);
}


#define GENVARAFFECTATIONS() {\
  if (!(vargenerated)) {\
    vargenerated =1;\
    for(iii=a; iii>0; iii--) {\
      ppp = stack[stacki-iii];\
      if (ppp->father != 0) {\
        intend(ff,deep);fprintf(ff,"v%d = v%d->sub[%d];\n",\
	              ppp->affvar,ppp->father,ppp->fatheri);\
      }\
    }\
  }\
}

static void gen_dump_noruleapp(FILE *ff,int deep)
{  int a,i;
if (!batch) {
   fprintf(ff,"fprintf(%s,\"gen_dump_noruleapp: \\n\");\n",OUTPUTS);
   a = fsymtab[actfunction].arity();
   if (a==0)
     {
       if(earley_analyser)
	 fprintf(ff,"earleyTermWrite(%s,",OUTPUTS);
       else
	 fprintf(ff,"termwrite(");
       fprintf(ff,"con%d",actfunction);
       fprintf(ff,",0);\n");
       intend(ff,deep);
       fprintf(ff,"exitnorule(%d);\n",actfunction);
     }
   else
     {
       intend(ff,deep);fprintf(ff,"{ struct term *norv;\n");
       intend(ff,deep);
       fprintf(ff,"ALLOC(norv,term%d,f%dlist,%d);\n",a,a,actfunction);
       intend(ff,deep);fprintf(ff,"norv->fs = %d;\n",actfunction);
       for(i=0; i<a; i++) {
	 intend(ff,deep);
	 fprintf(ff,"norv->sub[%d] = v%d;\n",i,actrrt->subrt[i].affvar);
       }
       if (actisrecursion)
	 {
	   intend(ff,deep);
	   fprintf(ff,"*ares = norv; ");
	   if(earley_analyser)
	     fprintf(ff,"earleyTermWrite(%s,",OUTPUTS);
	   else
	     fprintf(ff,"termwrite(");
	   fprintf(ff,"res");
	   fprintf(ff,",0);\n");
	 }
       else
	 {
	   if(earley_analyser)
	     fprintf(ff,"earleyTermWrite(%s,",OUTPUTS);
	   else
	     fprintf(ff,"termwrite(");
	   fprintf(ff,"norv");
	   fprintf(ff,",0);\n");
	 }
       intend(ff,deep);
       fprintf(ff,"exitnorule(%d);}\n",actfunction);
     }
}
}

#define GENNORULEAPP() {\
    if (!(actisfun)) {\
      if (actdontcare && (!actisbuiltin))\
        fprintf(ff,"freeterm(v1);; ");\
      if (trace && !batch) { \
        fprintf(ff,"fprintf(%s,\"[trace] no-rule-fail ::\\n\");",OUTPUTS); \
	    fprintf(ff,"fprintf(%s,\"\176\");\n",OUTPUTS);\
	    fprintf(ff,"fflush(%s);\n",OUTPUTS); } \
      genFail(ff,0);\
    }\
    else if (actisbuiltin)  {\
         gen_dump_noruleapp(ff,deep);\
      }\
    else {\
      fprintf(ff,"goto norulelab;\n");\
      actnoruleapp =1;\
    }\
}

#define GENCASE(fsym) \
	intend(ff,deep);\
	if (fsymtab[fsym].textform())\
	  if(fsymtab[fsym].textform()->rside[0].nonterminal())\
	    fprintf(ff,"case %d: /* '%s' */ \n",fsym,\
		    typet.ide(fsymtab[fsym].textform()->rside[0].typeval()));\
	    else\
	    fprintf(ff,"case %d: /* '%s' */ \n",fsym,\
		    (fsymtab[fsym].textform()->rside[0]).alfsy());\
	else\
	  fprintf(ff,"case %d:\n",fsym);

/* call genexpmatch(...,rrt,0xffffffff,0) */

static void genexpmatch
(FILE *ff,struct rtnode *rrt,unsigned long ok)
{ int a,i,iii,f,j,tl;
  struct rtlistn * l;
  struct rtna *p,*ppp;
  struct rtna **savestack;
  int savestacki,vargenerated;
  unsigned long hok;
static struct rtna *stack[MAXRTSIZE];      // stack & stacki are static !!!!!!!!!!!!!!!
static int deep=0;

  intend(ff,deep);fprintf(ff,"/*genexpmatch*/\n");

  deep++;
  f = rrt->fsym; 
  if (rrt->infos != TNORMFS) a=0;
  else a=fsymtab[f].arity();
  ok &= rrt->OKset;
  if (stacki+a >= MAXRTSIZE) {
    fprintf(stderr,"[matchcompile.c] stacki overflowed over MAXRTSIZE == %d",
	    MAXRTSIZE); interr();
  }
//  if (a) {intend(ff,deep); fprintf(ff,"{\n");}
  for(i=a-1; i>=0; i--) stack[stacki++] = &(rrt->subrt[i]);

  if (ismemberhash(ok,stack,stacki,&tl)) {
    fprintf(ff,"goto m%o;\n",tl); goto finish;
  }
  addnotmembertohash(ok,stack,stacki,matchlabel);
  fprintf(ff,"m%o:;\n",matchlabel++);
  vargenerated = 0;
nextpattern:
  if (ok==0) {
    intend(ff,deep); 
    GENNORULEAPP();
    stacki = 0;
  } else {

//    if (ismemberhash(ok,stack,stacki,&tl)) {
//      fprintf(ff,"goto m%o;\n",tl); goto finish;
//    }
//    addnotmembertohash(ok,stack,stacki,matchlabel);
//    fprintf(ff,"m%o:;\n",matchlabel++);

    if (selectSubtreeFromStack(stack,stacki,ok,actdontcare,&j,&hok)) {
      GENVARAFFECTATIONS();
      p = stack[j];
      for (i=j+1;i<stacki;i++) stack[i-1]=stack[i];
      stacki--;
      intend(ff,deep); 
      if (p->isbuiltins) { //??????????????????????????????????????????
        if (Bins) {
          fprintf(ff,"switch(((int) v%d)/2) {\n",p->affvar); /*,p->father,p->fatheri);*/
	} else {
          //fprintf(ff,"switch((int) v%d){\n",p->affvar,p->father,p->fatheri);
	  fprintf(ff,"switch(getInt(v%d)){\n",p->affvar); /*,p->father,p->fatheri);*/
	}
      } else {
	fprintf(ff,"switch(v%d->fs){\n",p->affvar); /*,p->father,p->fatheri);*/
      }
      AALLOSS(savestack, stacki,struct rtna *);
      for (i=0;i<stacki;i++) savestack[i] = stack[i];
      savestacki=stacki;
      l = p->setrt.tail;
      while (l!=NULL) {
	GENCASE(l->head->fsym);
	genexpmatch(ff,l->head,ok);
	stacki = savestacki;
	for(i=0;i<stacki;i++) stack[i] = savestack[i];
	l=l->tail;
      }
      CFRE(savestack);
      intend(ff,deep); fprintf(ff,"default:\n");
      genexpmatch(ff,p->setrt.head,ok);
      intend(ff,deep);fprintf(ff,"}\n");
    } else {
      if (actisfun && !(isconditioned(hok))) {
	if (ismemberhash(hok,stack,0,&tl)) {
	  if (tl != matchlabel-1) {
	    fprintf(ff,"goto m%o;\n",tl); 
	    goto finish;
	  }
	} else {
	  addnotmembertohash(hok,stack,0,matchlabel);
	  fprintf(ff,"m%o:;\n",matchlabel++);
	}
	GENVARAFFECTATIONS();
	ok = hok; stacki=0;
      } else {
	GENVARAFFECTATIONS();
      }
      //fprintf(ff,"generating body of rule %o\n",hok);
      if (actisfun) 
	trrules.genrwrulennappcode(ff,deep,actfunction,hok,(ok&~hok)==0);
      else 
	genstratrulennappcode(ff,deep,hok,actlab,actdontcare,(ok&~hok)==0);
      // last functions use static variable actrrules !!!
      ok &= ~hok;
      if (ok) goto nextpattern;
    }
  }
 finish:
//  if (a) {intend(ff,deep); fprintf(ff,"}\n");} 
  deep--;

  intend(ff,deep);fprintf(ff,"/*end-genexpmatch*/\n");

}

/* call genreclinmatch(...,rrt,0xffffffff,0) */

static int genreclinmatch
(FILE *ff,struct rtnode *rrt,unsigned long ok)
{ int a,i,f, isany; /*,iii, j,tl, */
  struct rtlistn * l;
  struct rtna *p; /*,*ppp;*/
  /* unsigned long hok;*/

  intend(ff,deep);fprintf(ff,"/*genreclinmatch*/\n");

  deep++;
  f = rrt->fsym; 
  if (rrt->infos != TNORMFS) a=0;
  else a=fsymtab[f].arity();
  ok = rrt->OKset;
  for(i=0; i<a; i++) {
    p = &(rrt->subrt[i]);
    if (p->father != 0) {
     intend(ff,deep);fprintf(ff,"v%d=v%d->sub[%d];\n",p->affvar,p->father,p->fatheri);}}
  if (ok==0) {
    intend(ff,deep); GENNORULEAPP();
  } else {
    intend(ff,deep); 
    fprintf(ff,"ok &= 0%lo;\n",ok);
    for(i=0; i<a; i++) {
      p = &(rrt->subrt[i]);
      if (p->ELIMset != 0 ) {
	if (p->ELIMset != MAXOK) {
	  intend(ff,deep); fprintf(ff,"if (ok& 0%o) \n",p->ELIMset); }

        if (Bins && p->isbuiltins) { //--- BORO
          intend(ff,deep);fprintf(ff,"if(isTagged(v%d)){ /*-----------*/ \n",p->affvar);
	  intend(ff,deep); 
	  // looking for rules
	  isany = (p->setrt.head->infos != TNORMFS); // ??????
	  for (l = p->setrt.tail; l!=NULL; l=l->tail) {
	    fprintf(ff,"/* infos1=%d */\n",l->head->infos); 
	    if (l->head->infos != TNORMFS) isany = 1; }
	  if (!isany) {
	    GENNORULEAPP(); }
	  else {
	    fprintf(ff,"switch(((int) v%d)/2){\n",p->affvar); 
	    // .... 5,6, cases
	    //     ---- BEGIN BLOCK     
	    intend(ff,deep);fprintf(ff,"/*BL1*/\n");
	    l = p->setrt.tail;
	    while (l!=NULL) {
	      if (l->head->infos != TNORMFS) {
		GENCASE(l->head->fsym);
		genreclinmatch(ff,l->head,ok);
		intend(ff,deep); fprintf(ff,"  break;\n"); }
	      l=l->tail;
	    }
	    intend(ff,deep); fprintf(ff,"default:\n");
	    genreclinmatch(ff,p->setrt.head,ok);
	    //    ---- END BLOCK
	    intend(ff,deep);fprintf(ff,"}\n");      
	  }
          intend(ff,deep);fprintf(ff,"} else{ /*-----------*/\n");
	  intend(ff,deep); 
	  // looking for rules
	  isany = (p->setrt.head->infos == TNORMFS); // ??????
	  for (l = p->setrt.tail; l!=NULL; l=l->tail) {
	    fprintf(ff,"/* infos2=%d */\n",l->head->infos); 
	    if (l->head->infos == TNORMFS) isany = 1; }
	  if (!isany) {
	    GENNORULEAPP(); }
	  else {
	    fprintf(ff,"switch(v%d->fs){\n",p->affvar); /*,p->father,p->fatheri);*/
	    // .... f,g cases
	    //     ---- BEGIN BLOCK     
	    intend(ff,deep);fprintf(ff,"/*BL2*/\n");
	    l = p->setrt.tail;
	    while (l!=NULL) {
	      if (l->head->infos == TNORMFS) {
		GENCASE(l->head->fsym);
		genreclinmatch(ff,l->head,ok);
		intend(ff,deep); fprintf(ff,"  break;\n"); }
	      l=l->tail;
	    }
	    intend(ff,deep); fprintf(ff,"default:\n");
	    genreclinmatch(ff,p->setrt.head,ok);
	    //    ---- END BLOCK
	    intend(ff,deep);fprintf(ff,"}\n");      
	  }
          intend(ff,deep);fprintf(ff,"} /*-----------*/ \n");
	} else { //--- MARIAN
  	  intend(ff,deep); 
          if (p->isbuiltins) 
    fprintf(ff,"switch(getInt(v%d)){\n",p->affvar);/*,p->father,p->fatheri);*/
	  else
    fprintf(ff,"switch(v%d->fs){\n",p->affvar);/*,p->father,p->fatheri); */
          //... all cases
//     ---- BEGIN BLOCK     
	   intend(ff,deep);fprintf(ff,"/*BL3*/\n");
	   l = p->setrt.tail;
	   while (l!=NULL) {
	     GENCASE(l->head->fsym);
	     genreclinmatch(ff,l->head,ok);
	     intend(ff,deep); fprintf(ff,"  break;\n");
	     l=l->tail;
	   }
	   intend(ff,deep); fprintf(ff,"default:\n");
	   genreclinmatch(ff,p->setrt.head,ok);
//    ---- END BLOCK
	  intend(ff,deep);fprintf(ff,"}\n");      
        }
      }
    }
  }
  deep--;
  intend(ff,deep);fprintf(ff,"/*end-genreclinmatch*/\n");
  return 0;
}

static void gencopyrsides(FILE *ff,unsigned long numr,unsigned long firrm,unsigned long maxnr)
{ unsigned long i;
  unsigned long am;
  if (numr==0) return;
  am=0;
  for (i=(numr+1)/2; i>0; i--) am = (am<<1)|1;
  for (i=0; i<firrm; i++)  am<<=1;
  if (numr>1) {
    fprintf(ff,"if (ok & 0%lo) {\n",am);
    gencopyrsides(ff,(numr+1)/2,firrm,maxnr);
    fprintf(ff,"}\n");
    am = 0;
    for (i=numr/2; i>0; i--) am = (am<<1)|1;
    for (i=0; i<firrm+(numr+1)/2; i++)  am<<=1;
    fprintf(ff,"if (ok & 0%lo) {\n",am);
    gencopyrsides(ff,numr/2,firrm+(numr+1)/2,maxnr);
    fprintf(ff,"}\n");
  } else if (numr==1) {
//    genwhred(nwhen[firrm],firrm,sym);
//fprintf(ff,"generating am==%o, last == %d\n",am,firrm==maxnr);
      if (actisfun) 
	trrules.genrwrulennappcode(ff,deep,actfunction,am,firrm==maxnr);
      else
 	genstratrulennappcode(ff,deep,am,actlab,actdontcare,firrm==maxnr);
  }
}



/* call genlinmatch(...,rrt,0xffffffff,0) */


static int genlinmatch(FILE *ff,struct rtnode *rrt,unsigned long ok)
{ unsigned long nr;

  intend(ff,deep);fprintf(ff,"/*genlinmatch*/\n");

  genreclinmatch(ff,rrt,ok);
  ok=rrt->OKset;  nr=0;
  while (ok) {
    ok = ((ok&~1)>>1);
    nr++; 
  };
  fprintf(ff," if (ok) {\n");
  gencopyrsides(ff,nr,0,nr-1);
  fprintf(ff," }\n");  
  GENNORULEAPP();

  intend(ff,deep);fprintf(ff,"/*end-genlinmatch*/\n");

  return 0;
}


void genfunbody(FILE *ff,struct rtnode *rrt,int recursion)
{ int i,f,a;
  f = rrt->fsym; a=fsymtab[f].arity();
  actfunction = f; actrrt = rrt; actisfun = 1;
  actisbuiltin=ISBUILTIN(fsymtab[f].textform()->leftside);

  intend(ff,deep);fprintf(ff,"/*genfunbody*/\n");
 
  actisrecursion = recursion; actnoruleapp =0;
  actdontcare = 1; stacki=0;
  HEADER;
  fprintf(ff,"{");
  genmatchvarprefix = "struct term "; genmatchvars(ff,rrt);//actisbuiltin); //PASSBINS
  if (*genmatchvarprefix!='s') fprintf(ff,";\n");
  if (!expmatch) {fprintf(ff,"  unsigned long ok;\n");}
  if (recursion) {
    fprintf(ff,"  struct term **ares,*res;\n  int lc;\n  ares = &res;\n");
  }
  if (trace && !batch) {
    fprintf(ff,"fprintf(%s,\"[dump] f%deval { :: \177 %d \");",OUTPUTS,f,f);
    for(i=0; i<a; i++) {
      fprintf(ff,"termwriter(v%d,%d);\n",rrt->subrt[i].affvar,rrt->subrt[i].isbuiltins);
    }
    fprintf(ff,"fprintf(%s,\"\177\176\");\n",OUTPUTS);
    fprintf(ff,"fflush(%s);\n",OUTPUTS);
  }
  fprintf(ff,"beginlabel:\n");
  if (!optimize) {
    fprintf(ff,"  nofreductions++;nonamed_tried++;\n");//nofrt
    if (statis) {
	fprintf(ff,"  anofreduction[%d]++;\n",f); 
	fprintf(ff,"  anofrt[%d]++;\n",f);  } }
  if (trace && recursion && !batch) {
    fprintf(ff,"fprintf(%s,\"[dump] cycle :: \177 %d \");",OUTPUTS,f); /*,f);*/
    for(i=0; i<a; i++) {
      fprintf(ff,"termwriter(v%d,%d);\n",rrt->subrt[i].affvar,rrt->subrt[i].isbuiltins); }
    fprintf(ff,"fprintf(%s,\"\177\176\");\n",OUTPUTS);
    fprintf(ff,"fflush(%s);\n",OUTPUTS);
  }
  if (expmatch) {
    inithash(); genexpmatch(ff,rrt,MAXOK); //actisbuiltin); //PASSBINS
    freehash(); 
  } else {
    //    fprintf(ff,"  ok=0xffffffff;\n");
    if(MAXOK==0xffffffff)
      fprintf(ff,"  ok=0xffffffff;\n");
    else
      fprintf(ff,"  ok=0xffffffffffffffff;\n");
    genlinmatch(ff,rrt,MAXOK); //actisbuiltin);  //PASSBINS
  }
  if (actnoruleapp) {
	fprintf(ff,"norulelab:\n");
	if (!optimize) {
          fprintf(ff,"  nofreductions--;\n");
          if (statis)
            fprintf(ff,"  anofreduction[%d]--;\n",f); }
	genconstructnoruleapp(ff,1);
  }
  fprintf(ff,"}\n");

  intend(ff,deep);fprintf(ff,"/*genfunbody*/\n");

}


void genStratAppBody(FILE *ff,struct rtnode *rrt,int lab,int dontcare, int nofree)
{
  actfunction = COMPILMAIN; actrrt = rrt; actisfun = 0;
  if (Bins)
    actisbuiltin = nofree;
  else
    actisbuiltin = 0; 

  intend(ff,deep);fprintf(ff,"/*genStratAppBody*/\n");

  actisrecursion = 0;
  actnoruleapp =0;
  actlab = lab;
  actdontcare = dontcare;
  stacki=0;
  
  fprintf(ff,"  {");
  genmatchvarprefix = "struct term "; genmatchvars(ff,rrt); //actisbuiltin); // PASSBINS
  if (*genmatchvarprefix!='s') fprintf(ff,";\n");
  if (expmatch) {
    inithash();
    genexpmatch(ff,rrt,MAXOK); //actisbuiltin); // PASSBINS
    freehash(); 
  } else {
    //    fprintf(ff,"  ok=0xffffffff;\n");
    if(MAXOK==0xffffffff)
      fprintf(ff,"  ok=0xffffffff;\n");
    else
      fprintf(ff,"  ok=0xffffffffffffffff;\n");
    genlinmatch(ff,rrt,MAXOK);  //actisbuiltin); // PASSBINS
  }
  fprintf(ff,"  }\n");

  intend(ff,deep);fprintf(ff,"/*end-genStratAppBody*/\n");

}






