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
#include "rtglobde.h"


static char *nett[MAXARI];
static int   netti;
static int canimport;


static void asocpri(unsigned a)
{ int t,s;
  inruleb=1;
  t=nsym();
  while (t!=DBOD) {
    s=t; inruleb=0;
    if ((t=nsym()) != NUMBER)
       oerr(rinf,'e',"[asocpri] error in symbol priority of %s",
             alfsy(-s,prwt));
    else {
      if (terpri[s]!=UNDEFPRI && terpri[s]!=(a|(nval&PRIMSK)))
         oerr(rinf,'e',"\tconflict priority redefinition of %s",alfsy(-s,prwt));
      else terpri[s]=a | (nval & PRIMSK);
      inruleb=1;
      t=nsym();
    };
  };
}

void static rules(void)
{ int t;
  inruleb=1; inimport=1;
  t=nsym();
  while (t!=DBOD) {
    t=hruleb(t,1);
    if (t==NUMBER) {
      if (nval==0) {
	   oerr(rinf,'e',"[rules] action can't be zero");
	   nval= -1;
      }
	 rbody[begru]=nval;
      if ((t=nsym()) == NUMBER) { rbody[begru+1]=nval; t=nsym();};
    };
    if (t!=';') oerr(rinf,'e',"[rules] error in grammar; ; expected");
    inruleb=1;
    t=nsym();
    if (t==ENDOI)
      oferr(rinf,"[rules] EOF in rules");
  };
  inimport=0;
}

void readgram(n,t)
char *n;
int t;
{ /* int tm; */
  INFILE rff;
  if (! opeinfile(n,&rff))
     oferr(&rff,"\t can't open grammar file of mod. %s ",envirgetname(t));
  rinf= &rff;
  inruleb=0;
  asocpri(LEFTASOC);
  asocpri(RIGHTASOC);
  asocpri(NOASOC);
  rules();
  cloinfile(&rff);
}

void readdebgram(char *n,int t)
{ /* int tm; */
  INFILE rff;
  if (! opeinfile(n,&rff))
     oferr(&rff,"\t can't open grammar file of mod. %s ",envirgetname(t));
  rinf= &rff;
  inruleb=0;
  asocpri(LEFTASOC);
  asocpri(RIGHTASOC);
  asocpri(NOASOC);
  rules();
  inruleb=0;
  asocpri(LEFTASOC);
  asocpri(RIGHTASOC);
  asocpri(NOASOC);
  rules();
  cloinfile(&rff);
}


int nsym()
{ int t;
  if ((t=lexan(nultab,prwt,rinf)) == UNDID) return(uival+BRESW);
  if (t==ENDOI)
    oferr(rinf,"[nsym] EOF in grammar rule body");
  /* mal by to byt znak */
  return(t);
}

int ntyp()
{ int n,n2;
/* struct flist *p; */
  lstsat=sast(rwt,uit,rinf);  n=enviraliget(n2=envirget('t',amacb));

  return(n);
}

static int houtsg(int t,FILE *f)
{
  switch (-t) {
  case NUMBER :t=NUMBERT;break;
  case APOSCHAR :t=CHART; break;
  case UNDID    :t=IDENTT; break;
  case REALNUM  :t=REALNUMT; break;
  }
  if (t>0)         {nett[netti++]=envirgetname(t);  return(1); }
  if (t== -SIPK) { fprintf(f,"-> "); return(0); }
  else fprintf(f,"%s ",alfsy(t,prwt));
  return(0);
}

static void oasocpri(int a,FILE *f)
{ int i;
  for (i=0; i<NTER; i++) {
    if (terpri[i]!=UNDEFPRI && (terpri[i] & ASOCMSK)==a) {
      houtsg(-i,f); fprintf(f,"%d  ",terpri[i]&PRIMSK);
    }
  }
  fprintf(f,"\n::\n");
}


void outruleb(b,e,f)
int b,e;
FILE *f;
{ int i,begr,j;
  oasocpri(LEFTASOC,f);
  oasocpri(RIGHTASOC,f);
  oasocpri(NOASOC,f);
  i=b;
  while (i<e) {
    begr=i; netti=0;
    i+=3;
    while (rbody[i]) {
      if (houtsg(rbody[i],f)) fprintf(f,"__ ");
      i++;
    }
    i++;
    fprintf(f,"\t :: %c",netti==0 ? ' ' : '(');
    j=0;
    while (j<netti) {j++; fprintf(f,"%s%c",nett[j-1],j<netti?',':')');}
    houtsg(rbody[begr+2],f); fprintf(f,"%s ",nett[netti-1]);
    fprintf(f,"\t %u",rbody[begr]);
    if (rbody[begr+1]!=UNDEFPRI) fprintf(f," %d",rbody[begr+1]&PRIMSK);
    fprintf(f,";\n");
  }
  fprintf(f,"::\n");
}

int terminalv(int typ)
{ int t = 0; /* initialised to avoid warning */
  switch (typ) {
  case NUMBERT  :t= - NUMBER;break;
  case CHART    :t= - APOSCHAR; break;
  case IDENTT    :t= - UNDID; break;
  case REALNUMT  :t= - REALNUM; break;
  }
  return(t);
}



int hruleb(t,reallyimp)
int t,reallyimp;
{ int etypl,i,j,head;
  struct rslist *bodyl;
  inruleb=1; canimport=reallyimp;
    begru=irbody; etypl=begru+2;
    rbody[irbody++]= (actmon<<6)  + actopn;
    rbody[irbody++]=UNDEFPRI;
    irbody++;
    while (t!=DBOD) {
      if (t==ARGUM) {
	   rbody[etypl]=irbody; etypl=irbody;
      } else {
	   rbody[irbody]= -t;
/*         if (terpri[t]!=UNDEFPRI) rbody[begru+1]=terpri[t] & PRIMSK | NOASOC;
*/
      };
      if (irbody>=RULTL)
	   oferr(rinf,"[rules] rbody overflowed over RULTL");
      irbody++;
      t=nsym();
    };
    rbody[irbody++]=0;
    inruleb=0;
    i=begru+2; j=rbody[i];
    if (i!=etypl && nsym()!='(')
	   oerr(rinf,'e',"[rules] error in grammar; ( expected");
    hruleari=0; hrulemsk=0;
    while (i!=etypl) {
      i=j; j=rbody[i];
	 rbody[i]=ntyp();
         if (rbody[i]<BOOLT) rbody[i]= terminalv(rbody[i]);
	 if (rbody[i]>BOOLT) hrulemsk |= 1<<hruleari;
	 if (++hruleari > MAXARI) oerr(rinf,'e',"\t too much subterms");
      if (lstsat!=((i!=etypl)?',':')')) oerr(rinf,'e',"\t, or ) expected");
    };
    head=ntyp();
    rbody[begru+2]=head;
    if (canimport) {
      bodyl=ALLOS(struct rslist);
      bodyl->rside=begru+3;
      bodyl->next=rulenet[head];
      rulenet[head]=bodyl;
    } else {
      if (canimport!=reallyimp)
	oadderr(" I don\'t import op");
      irbody=begru;
    }
  return(lstsat);
}

void vars(void)
{ int t,tt,begl,actv,bb;
  struct rslist *bodyl;
  inruleb=1;  inimport=0;
  t=nsym(); actv=VARIABEG;
  while (t!=DBOD) {
    begl=0;
    do {
      if (t<BRESW) oferr(rinf,"\tvar ident. expected");
	 rbody[irbody++]=(actv++);
	 if (actv>=VARIAEND) oferr(rinf,"\ttoo much vars");
      rbody[irbody++]=UNDEFPRI;
      rbody[irbody]=begl; begl=irbody++;
      rbody[irbody++]= -t; rbody[irbody++]=0;
      tt=nsym();
      if (tt==',') t=nsym();
    } while (tt==',');
    if (tt!=DBOD) oferr(rinf,"\terror in vars; :: expected");
    t=ntyp();
    if (lstsat!=';') oferr(rinf,"\t; expected");
    while (begl!=0) {
      bb=begl; begl=rbody[begl]; rbody[bb]=t;
      vartyp[rbody[bb-2]-VARIABEG]=t;
      bodyl=ALLOS(struct rslist);
      bodyl->rside=bb+1;
      bodyl->next=rulenet[t];
      rulenet[t]=bodyl;
    }
    t=nsym();
    if (t==ENDOI) oferr(rinf,"\t[vars] EOF in vars");
  }
  numvars= actv-VARIABEG;
}
