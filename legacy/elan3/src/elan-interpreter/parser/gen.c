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
    Marian Vittek		e-mail: vittek@guma.ii.fmph.uniba.sk

*/

/*
		(c) 	Marian Vittek, 1991
			Bratislava, Slovakia
*/
#include "globdef.h"

/*		......... file gen.c ........... */




static int *hashtab;     /* hashtab[HASHSIZE];  tab. stavov hasovana podla poloziek*/

static struct {int used,r;} clo[MAXLP];

static int runum,rupri,ruleft,rulen,complstate;
static struct actlist **sssnred;

static int makeclosure(int st)
{ int j,n,sy,*p;
  register int i;
  struct rslist *bodyl;
  n=sttab[st].polozky.number;
  p=sttab[st].polozky.pol;
  for (i=0; i<n; i++) {
    clo[i].used=0;
    clo[i].r= *(p++);
  }
  j=0;
  while (j<i) {
    sy=rbody[clo[j].r];
    if (sy>0) {
      { register int k;
	   for (k=0; k<j; k++) if (sy==rbody[clo[k].r]) goto nextj;
      }
      bodyl=rulenet[sy];
      while (bodyl != NULL) {
	   clo[i].used=0; clo[i++].r=bodyl->rside;
	   bodyl=bodyl->next;
      }
    }
    nextj:
    j++;
  }
  /* i by sa malo kontrolovat priebezne, ale ... */
  if (i>=MAXLP) 
    oferr(NULL,"[makeclosure] closure overflowed over MAXLP");
  return(i);
}

static void sortpolbody(int a,int b)
{ register int j,s,i;
  for (i=a+1; i<b; i++) {
    s=polbody[i];
    j=i-1;
    while (polbody[j]>s && a<=j) { polbody[j+1]=polbody[j]; j--;}
    polbody[j+1]=s;
  }
}

static int addstate(int libo)
{ int i,hash,n1,n2,*p1,*p2;
  struct setpol *ss;
  sortpolbody(libo,ipolbody);
  hash=0;
  for (i=libo;i<ipolbody; i++) hash+=polbody[i];
  hash=hash % HASHSIZE;
  n1=ipolbody-libo; p1= polbody+libo;
  while (hashtab[hash]) {
    ss= &(sttab[hashtab[hash]].polozky);
    n2=ss->number; p2=ss->pol;
    if (n1 == n2) {
      n2++; while (--n2 && ( *(p1++)== *(p2++) ));
      if (!n2) { /* old state  */
        ipolbody=libo;
        return(hashtab[hash]);
      }
      p1= polbody+libo;
    }
    hash=(hash+127)%HASHSIZE;
  }
  /*	new state	*/
  hashtab[hash]=isttab;
  sttab[isttab].polozky.number=n1;
  sttab[isttab].polozky.pol= &polbody[libo];
  sttab[isttab].gotos=NULL;
  sttab[isttab].actions=NULL;
  sttab[isttab].laset=NULL;
  return(isttab++);
}

static void addgoto(int fromst,int ons,int tost)
{ struct gotolist *gg,**ggg;
  struct actlist  *aa,**aaa;
  if (ons>0) {
    gg=ALLOS(struct gotolist);
    gg->symbol=ons;
    gg->tostate=tost;
    ggg= &sttab[fromst].gotos;
    PLACE(ggg,ons);
    gg->next= *ggg;
    *ggg=gg;
  } else {
    aa=ALLOS(struct actlist);
    aa->symbol=ons;
    aa->action=shift;
    aa->act.shiftstate=tost;
    aaa= &sttab[fromst].actions;
    PLACE(aaa,ons);
    aa->next= *aaa;
    *aaa=aa;
  }
}

void stategen()
{ int st,i,j,pp,symb,libody;
  hashtab=ALLOSS(HASHSIZE,int);
  for (i=0; i<HASHSIZE; i++) hashtab[i]=0;
  for (st=0;st<isttab; st++) {
    pp=makeclosure(st);
    for (i=0; i<pp; i++) {
      if (clo[i].used) continue;
      symb=rbody[clo[i].r];
	 if (symb) {
	   libody=ipolbody;
	   if(ipolbody>=POLTL)
		oferr(NULL,"[stategen] polbody overflowed over POLTL");
	   polbody[ipolbody++]=clo[i].r+1;
	   for (j=i+1; j<pp; j++)
	    if (symb == rbody[clo[j].r]) {
		  clo[j].used=1;
		  if(ipolbody>=POLTL)
		    oferr(NULL,"[stategen] polbody overflowed over POLTL");
		  polbody[ipolbody++]=clo[j].r+1;
		}
	   addgoto(st,symb,addstate(libody));
      }
    }
  }
  CFRE(hashtab);
}

static void addaccept(int st,int sy)
{ struct actlist *aa,**aaa;
  aa=ALLOS(struct actlist);
  aa->symbol=sy;
  aa->action=accept;
  aaa= &sttab[st].actions;
  PLACE(aaa,sy);
  aa->next= *aaa;
  *aaa=aa;
}

void addendforaccept(void)
{
  addaccept(1,-Sfor);
  addaccept(1,-Send);
}

void addENDOIaccept()
{
  addaccept(1,-ENDOI);
}

void addeofaccept(void)
{
  addaccept(1,-ENDOI);
}


void compla()                         /* vypocitaj LA(1) mnoziny symbolov */
{ int st,sy,i,*j,lef,sr,sp,pp,flag;
  struct lalist **ns,*f;
  struct flist **ts,*ff,*ft;
  struct gotolist *g;
  struct actlist *p;
  struct lalist *q;
  struct laaddl *comlas,*cl;
  comlas=NULL;
  for(st=0;st<isttab;st++) {
    pp=makeclosure(st);
    for(i=sttab[st].polozky.number;i<pp;i++) if (!(rbody[clo[i].r-4])) {
      g=sttab[st].gotos; lef= *(j= &rbody[clo[i].r-1]);
      FOUND(g,lef);
      sp=g->tostate;
      sr=st;
      while (*(++j)) {
	   if (*j<0) {
		p=sttab[sr].actions;
		FOUND(p,*j);
		sr=p->act.shiftstate;
	   } else {
		g=sttab[sr].gotos;
		FOUND(g,*j);
		sr=g->tostate;
	   }
      }
      ns= &sttab[sr].laset;
      PLACE(ns,lef);
      if ((*ns==NULL) || ((*ns)->symbol != lef)) {
	   f=ALLOS(struct lalist);
	   f->symbol=lef; f->syms=NULL;
	   f->next= *ns;
	   *ns=f;
      }
      ts= &(*ns)->syms;
      if ((q=sttab[sp].laset)!=NULL) {
        cl=ALLOS(struct laaddl);
        cl->tolas= ts;  cl->froml=q;
        cl->next=comlas; comlas=cl;
      }
      p=sttab[sp].actions;
      while (p!=NULL) {
	   sy=p->symbol;
	   PLACE(ts,sy);
	   if ((*ts==NULL) || ((*ts)->symbol != sy)) {
		ff=ALLOS(struct flist);
		ff->symbol=sy;
		ff->next= *ts;
		*ts=ff;
	   }
	   p=p->next;
      }
    }
  }

  do {
    flag=0; cl=comlas;
    while (cl!=NULL) {
      f=cl->froml;
      while (f!=NULL) {
        ts=cl->tolas; ff=f->syms;
        while (ff!=NULL) {
          sy=ff->symbol;
          PLACE(ts,sy);
          if ((*ts==NULL) || ((*ts)->symbol != sy)) {
            ft=ALLOS(struct flist);
            ft->symbol=sy; ft->next= *ts; *ts=ft;
            flag=1;
          }
          ff=ff->next;
        }
        f=f->next;
      }
      cl=cl->next;
    }
  } while (flag);
  FREELIST(comlas,cl);
}

static void conerr(enum actenum a,int sy)
{ /* int i,j,*pi;
     char c; */
  if (a == shift) {
    /*
    oerr(NULL,'w',"\tshift/reduce in state %d on symbol %s",
               complstate,alfa(sy));
	       */
    oerr(NULL,'w',"\tshift/reduce in state %d on symbol %c",
               complstate,alfa(sy));
/*  stbdump(complstate); printf("\n\n");
    oadderr("\t  shift in ");  c='(';
    pi=sttab[complstate].polozky.pol;
    for (i=sttab[complstate].polozky.number; i>0; i--) {
      if (rbody[*pi] == sy) {
	 for (j= *pi-1; rbody[j]; j--);
	 oadderr("%c%d",c,rbody[j+1]);
	 c=',';
      }
      pi++;
    }
    oadderr(") <-> reduce with (%d)\n",runum);
*/
  } else {
    /*   oerr(NULL,'w',"\treduce/reduce in state %d on symbol %s",*/
   oerr(NULL,'w',"\treduce/reduce in state %d on symbol %c",
              complstate,alfa(sy));
/*   stbdump(complstate); printf("\n\n");
   oadderr("\t reduce with (%d) <-> reduce with (%d)",
	   (*sssnred)->act.redpar.num,runum);
*/
  }
}

static enum actenum mkconfl(unsigned prip,int sym)	/* riesi shift/reduce konflikt */
{ unsigned pris;
  pris=terpri[-sym];
  if (prip==UNDEFPRI || pris == UNDEFPRI) {
	conerr(shift,sym);
	return(srerr);
  }
  pris=pris & PRIMSK;
  prip=prip & PRIMSK;
  if (prip == pris ) {
	switch (terpri[-sym] & ASOCMSK) {
	case NOASOC :conerr(shift,sym);
			   return(srerr);
	case LEFTASOC: return(reduction);
	case RIGHTASOC:return(shift);
	}
  }
  if (pris < prip) return(reduction);
  else return(shift);
}



static void addred(int sy,int defval)
{ struct actlist *aa;
  enum actenum mkcr;
  PLACE(sssnred,sy);
  if ((*sssnred != NULL) && (*sssnred)->symbol == sy) {
					/* konflikt  !!!!!!!!!!!! */
    aa= *sssnred;
    if (aa->action == reduction) {          	/* redukcia/redukcia */
      if (aa->act.redpar.num==runum && aa->act.redpar.left==ruleft && 
          aa->act.redpar.lenght==rulen) return; /* importovana op  */
      conerr(reduction,sy);                     /* red/red neriesi */
      aa->action=rrerr;
      return;
    } else {                                    /* presun/redukcia */
      if ((mkcr=mkconfl(rupri,sy)) == shift) return;
      if (mkcr == srerr) { aa->action =srerr; return; }
    }
    if (defval) {		/* redukcia na default po konflikte */
      *sssnred=aa->next;
      CFRE(aa);
      return;
    }

  } else {				/* nie je to konflikt */
    if (defval) return;
    aa=ALLOS(struct actlist);
    aa->next= *sssnred;
    *sssnred=aa;
  }
					/* zapis novu redukciu	*/
  aa->symbol=sy;
  aa->action=reduction;
  aa->act.redpar.num=runum;
  aa->act.redpar.left=ruleft;
  aa->act.redpar.lenght=rulen;
}

void completestate()
{ int *pp,pn,i,j,nred;
 int nrl=0; /* initialised to avoid warning */
  struct flist *ff;
  struct lalist *f;
  for(complstate=0;complstate<isttab;complstate++) {
    pp=sttab[complstate].polozky.pol;
    pn=sttab[complstate].polozky.number;
    nred=0;
    for (i=0; i<pn; i++) {
	 if (! rbody[*pp]) {
	   rulen= -3;
	   for (j= *pp-1; rbody[j]; j--) rulen++;
	   runum=rbody[++j]; rupri=rbody[++j]; ruleft=rbody[++j];
	   sssnred= &sttab[complstate].actions;
           f=sttab[complstate].laset;
	   FOUND(f,ruleft);
	   if (f!=NULL) {
             ff=f->syms;
	     while (ff != NULL) {
               // && and || where ambiguous there
	       addred(ff->symbol,((optim && ! nred) || pn==1));
	       ff=ff->next;
	     }
	     if (!nred) {nred=j; nrl=rulen;}
           }
	 }
	 pp++;
    }
    if (optim || pn==1) 
      if (nred) { /* this if prevents bad use of nrl */
	 sssnred= &sttab[complstate].actions;
	 ruleft=rbody[nred--];rupri=rbody[nred--];runum=rbody[nred];
	 rulen=nrl;
	 addred(DEFAULTSYM,0);
    }
  }
}


