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

/*...........................................................................
*/

static char ide[20];

static struct {
  char *bm,*bmi;
  } inmac[MAXINMAC];
int inmaci=0;

static struct {
  char *amcb;
  int amcbi,srg;
  } amast[MAXINMAC];
int amasti=0;

/* static int amac; */


static struct sbmac {
   int  arity;
   char *body;
   } bmac[NMAC];

/* static int arg[MAXARG]; */
/* static int argi; */

static char *aarg[MAXARG];
static int sarg;        /* flag pre semaction, ci kopirujem aktualny arg.*/

/*...........................................................................
*/



int opeinfile(s,ff)
char *s;
INFILE *ff;
{ char c,*ss;
  ff->f=fopen(s,"r");
  if (ff->f==NULL) {
    ss=ALLOSS(strlen(s)+strlen(LIBDIR)+1,char);
    strcpy(ss,LIBDIR); strcpy(ss+strlen(LIBDIR),s);
    ff->f=fopen(ss,"r");
    if (ff->f==NULL) { CFRE(ss); goto opefail;}
    else ff->name=ss;
  } else {
opefail:
    ff->name=ALLOSS(strlen(s)+1,char);
    strcpy(ff->name,s);
  }
  ff->line=1; ff->pos=0;
  ff->instr=0;
  if (ff->f == NULL) return(0);
/*  fscanf(ff->f," %c",&c); */ c=getc(ff->f);
  ff->fch=c;
  if (c=='\n') (ff->line)++;
  return(1);
}

void cloinfile(ff)
INFILE *ff;
{ CFRE(ff->name);
  if (ff->f!=NULL) fclose(ff->f);
}

#define fuch (aff->fch)

int ich()
{ register int c;
  c=aff->fch;
  while (inmaci && ! *inmac[inmaci-1].bmi) CFRE(inmac[--inmaci].bm);
  if (inmaci) aff->fch= *(inmac[inmaci-1].bmi++);
  else if (c!=EOF) { if ((aff->fch=getc(aff->f)) == '\n') {
                       (aff->line)++; aff->pos = 0;
                     }
                     else (aff->pos)++;
                   }
  return(c);
}

static void blankskip(void)
{ while (aff->fch==' '||aff->fch=='\n'||aff->fch=='\t') ich();
}
/*..........................................................................*/

static void stoamacb(void)
{ int i;
  char *p;
  p=ALLOSS(amacbi,char);
  amast[amasti].amcb=p;
  amast[amasti].srg=sarg;
  amast[amasti++].amcbi=amacbi;
  for(i=0; i<amacbi; i++) *(p++)= amacb[i];
}

static void resamacb(void)
{  int i;
  char *p,*q;
  amasti--;
  q=p=amast[amasti].amcb;
  amacbi=amast[amasti].amcbi;
  sarg=amast[amasti].srg;
  for(i=0; i<amacbi; i++) amacb[i]= *(p++);
  CFRE(q);
}

/*static char *back(p)
char *p;
{ register int n;
  p--;
  if (*p == ']') {
    n=1; p--;
    while (n) {
      if (*p==']') n++; else if (*p=='[') n--;
      p--;
    }
  }
  while (isalpha(*p) || isdigit(*p)) p--;
  return(p);
  }*/

int sast(rw,ui,f)
TABID *rw,*ui;
INFILE *f;
{ int sy;
  char *p;
  sarg=2; amacbi=0; sy=lexan(rw,ui,f);
  if (sy!=UNDID) oferr(f,"UNDID was expected as type");
  p=ui->id[uival];  while (*p) amacb[amacbi++]= *(p++);
  amacb[amacbi++]=0;
  return(lexan(rw,ui,f));
}

static int handldef(INFILE *f,int sy)
{ /* int i; */
  inruleb=0; amacbi=0;
  oferr(f,"\tmacro nie je implementovane");
  if (!sy) exit(EXIT_FAILURE);
  return(sy);
}

int hasocpri(f,sy)
INFILE *f;
int sy;
{
  // initialised to NOASOC to avoid uninitialisation warning
  unsigned as=NOASOC;
  rinf=f; inruleb=0;
  while (sy==STleftasoc || sy==STrightasoc || sy==STnoasoc) {
    switch (sy) {
    case STleftasoc :as=LEFTASOC; break;
    case STrightasoc:as=RIGHTASOC;break;
    case STnoasoc   :as=NOASOC;   break;
    };
    while ((sy=lexan(rwt,uit,f))==NUMBER) {
      sy=nsym();
      if(terpri[sy]!=UNDEFPRI && terpri[sy]!=(as|(nval&PRIMSK)))
         oerr(f,'e',"\tconflict priority redefinition of %s",alfsy(-sy,prwt));
      terpri[sy]=as | (nval & PRIMSK);
    }
    if (sy != ';') oferr(&inf,"\t; expected");
    sy=lexan(rwt,uit,f);
  }
  return(sy);
}


int prodef(f,sy)
int sy;
INFILE *f;
{ INFILE f2;
  while (sy==STinclude) {
    blankskip();
    if (fuch!='\"') oferr(f,"\t \'\"\' expected");
    ich();
    amacbi=0; while(fuch!='\"') amacb[amacbi++]=ich();
    amacb[amacbi++]=0;
    ich(); blankskip();
    if (ich()!=';') oferr(f,"\t \';\' expected");
    if (!opeinfile(amacb,&f2)) oferr(NULL,"\tcan't open file %s",amacb);
    if (prodef(&f2,lexan(nultab,uit,&f2))!=ENDOI)
         oferr(&f2,"\t illegal text in included file");
    cloinfile(&f2);
    sy=lexan(nultab,uit,f);
  }
  if (sy==STdefine) sy=handldef(f,sy);
  sy=hasocpri(f,sy);
  return(sy);
}

void pushmac(t,i)
char *t;
int i;
{ char *p;
  if (inmaci>=MAXINMAC) oferr(NULL,"\t too many nested macros \n");
  p=ALLOSS(i,char);
  inmac[inmaci].bm=p;
  inmac[inmaci].bmi=p;
  strcpy(p,t);
  inmaci++;
}

void popmac()
{ 
  if (inmaci)  CFRE(inmac[--inmaci].bm);
}

static void handlarg(int ar,TABID *rw,TABID *ui,INFILE *f)
{ 
  if (ar>0) 
    if (ar!=handlar(rw,ui,f)) oferr(f,"bad number of argument");
}

int handlar(rw,ui,f)
TABID *rw,*ui;
INFILE *f;
{
  int i,ls,ena;
  char *p;
  ena=enabmac;
  if(lexan(rw,ui,f)!='[') return(0);
  stoamacb(); enabmac=0;
  i=0;
  {
      sarg=1; amacbi=0; ls=lexan(rw,ui,f);
      if (ls!=UNDID) oferr(f,"UNDID as name of file expected");
      p=ALLOSS(strlen(ui->id[uival])+1,char); strcpy(p,ui->id[uival]);
      aarg[i++]=p;
      if (i>=MAXARG) oferr(f,"\ttoo many arguments of module");
  };
  resamacb(); enabmac=ena;
  return(i);
}

void handlfarg(ar)
int ar;
{ int i,am;
 if (ar) {
  exp('['); i=0;
  do {
    exp(UNDID);
    am=idadd(uit->id[uival],strlen(uit->id[uival]),mactab);
    bmac[am].arity=0;
    bmac[am].body=ALLOSS(strlen(aarg[i])+1,char);
    strcpy(bmac[am].body,aarg[i]);
    CFRE(aarg[i]);
    i++;
    exp((i==ar)?']':',');
  } while (i<ar);
 }
}

void resetmactab()
{ int i;
  for (i=0; i<NMAC; i++)
    if (mactab->id[i]!=NULL) {
      CFRE(bmac[i].body);
      CFRE(mactab->id[i]);
      mactab->id[i]=NULL;
    }
}

int lexan(rw,ui,ff)
TABID *rw,*ui;
INFILE *ff;
{ int i,c;
  snval=nval; suival=uival; schval=chval; srnval=rnval;
  aff=ff;
/*  if (aff->instr) return(handlestring());	*//* citaj v stringu */
lexa1:
  blankskip();
  c=ich();
  while (c=='{' && fuch=='{') {
    while (fuch!='}')
     while (ich()!='}')
      if (fuch==EOF) {oerr(aff,'e',"[lexan] comment throught EOF");return(ENDOI);}
    ich(); blankskip(); c=ich();
  }

  if (isalpha(c)) {
    i=1; ide[0]=c;
    while (isalpha(fuch) || isdigit(fuch))ide[i>=19?19:i++]=ich();
    ide[i]=0;
    if (enabmac && idmember(ide,mactab)) {
      char mcb[MLENMAC]; int mcbi;
      char *ab,*bo;
      int aarit;
      aarit=bmac[posid].arity;
      bo=bmac[posid].body;
      handlarg(aarit,rw,ui,ff);
      mcbi=0;
      while (*bo) {
        if (*bo>=MAXARG) mcb[mcbi++]= *bo;
        else {
          for(ab=aarg[*bo-1]; *ab; mcb[mcbi++]= *(ab++));
        }
        if (mcbi>=MLENMAC) oferr(ff,"\tmacro body is too long");
        bo++;
      }
      mcb[mcbi++]=ff->fch; mcb[mcbi++]=0;
      for (i=0; i<aarit; i++) CFRE(aarg[i]);
      pushmac(mcb,mcbi);
      ich(); 
      goto lexa1;
    }
    if (idmember(ide,rw)) return(BRESW+posid);
    uival=idadd(ide,i,ui);
    return(UNDID);
  }
  if (isdigit(c)) {
    if (inruleb) oerr(aff,'e',"[lexan] number in rbody");
    nval=c-'0';
    while (isdigit(fuch)) nval=nval*10+ich()-'0';
    return(NUMBER);
  }
/*
  if (c=='"') {
    if (inruleb) oerr(aff,'e',"[lexan] string in rbody");
    if (fuch=='"') oferr(aff,"[lexan] empty string in\"\"");
    aff->instr=1;
    return(handlestring());
  }
*/
/*
  if (c=='\'') {
    c=ich();
    if (c=='\\' ) 
      if (isdigit(fuch)) {
        chval=0;
        while (isdigit(fuch)) chval=chval*10+ich()-'0';
      } else {
      switch (c=ich()){
      case 't' :chval='\t';break;
      case 'b' :chval='\b';break;
      case 'n' :chval='\n';break;
      case '\\' :chval='\\';break;
      case '\"' :chval='\"';break;
      case '\'' :chval='\'';break;
      default   :chval=c;
      }
    } else chval=c;
    if (ich()!='\'') oerr(aff,'e',"[lexan] error in char constant");
    if (inruleb) return(chval); 
    return(APOSCHAR);
  }
*/
  if (inruleb && c==':' && fuch==':') {
    ich(); return(DBOD);
  }
  if (inruleb && c=='_' && fuch=='_') {
    ich(); return(ARGUM);
  }
/*
  if (c=='=' && fuch=='>') {
    ich(); return(SIPK);
  }
*/
  if (c==EOF) return(ENDOI);

  return(c);
}


/*
static handlestring()
{ 
  if (aff->instr==1) {
    aff->instr=2;
    chval=ich();
    if (chval=='"') {aff->instr=0; return(ZEROCHAR);}
    return(APOSCHAR);
  } else {
    aff->instr=1;
    return('.');
  }
}
*/

/*...........................................................................*/

static char alfsych[]=" ";
static char alfsyci[]="\'\\ \'";

char *alfsy(s,t)
int s;
TABID *t;
{ 
  s= -s;
  if (s<LASTCHAR) { 
    switch (s){
    case '\t' :alfsyci[2]='t';return(alfsyci);
    case '\b' :alfsyci[2]='b';return(alfsyci);
    case '\n' :alfsyci[2]='n';return(alfsyci);
    case '\\' :alfsyci[2]='\\';return(alfsyci);
    case '\"' :alfsyci[2]='\"';return(alfsyci);
    case '\'' :alfsyci[2]='\'';return(alfsyci);
    default   :alfsych[0]=(char)s; return(alfsych); 
    }
  }
  if (s>=BRESW) return(t->id[s-BRESW]);
  return(alfstter(s));
}

char *alfstter(s)
int s;
{
  switch(s) {
  case APOSCHAR :return("CHAR");
  case NUMBER   :return("NUMBER");
  case REALNUM  :return("REALNUM");
  case UNDID    :return("IDENT");
  case UNDID_   :return("IDENT_");
  case ENDOI    :return("EOF");
  case SIPK     :return("FLASH");
  }
  return("ERROR.VAL");
}

