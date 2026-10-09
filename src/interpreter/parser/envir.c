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


struct nlist {
  struct nlist *next;
  int num;
  };

static TABID *envtypt;
static struct nlist *envnumt[1023];
static char envtyp[1023];
static int envindt[1023];
static int alityp[1023];
static int lowfree;

/* static char tb[200]; */


static void envaddnum(int t,int n)
{ struct nlist *aa;
  struct nlist **p;
  aa=ALLOS(struct nlist);
  aa->num=n;
  aa->next=NULL;
  p= &envnumt[t];
  while (*p != NULL) p= &((*p)->next);
  *p=aa;
}


void envirread()
{ int i,tn;
/* int i,sy,tn,an;*/
/* FILE *f; */
  /* char c; */
  envtypt=idtaballoc(1023);
  for (i=0; i<1023; i++) envnumt[i]=NULL;
  for (i=0; i<1023; i++) envindt[i]= -1;

  tn=idadd("identifier",10,envtypt);
  envtyp[tn]='t';
  envaddnum(tn,1);
  envindt[1]=tn;
  alityp[tn]=1;
  tn=idadd("number",6,envtypt);
  envtyp[tn]='t';
  envaddnum(tn,2);
  envindt[2]=tn;
  alityp[tn]=2;
  tn=idadd("character",9,envtypt);
  envtyp[tn]='t';
  envaddnum(tn,3);
  envindt[3]=tn;
  alityp[tn]=3;
  tn=idadd("realnum",7,envtypt);
  envtyp[tn]='t';
  envaddnum(tn,4);
  envindt[4]=tn;
  alityp[tn]=4;
  tn=idadd("boolean",7,envtypt);
  envtyp[tn]='t';
  envaddnum(tn,5);
  envindt[5]=tn;
  alityp[tn]=5;
  tn=idadd("ident",5,envtypt);
  envtyp[tn]='t';
  envaddnum(tn,6);
  envindt[6]=tn;
  alityp[tn]=6;
  tn=idadd("int",3,envtypt);
  envtyp[tn]='t';
  envaddnum(tn,7);
  envindt[7]=tn;
  alityp[tn]=7;
  tn=idadd("char",4,envtypt);
  envtyp[tn]='t';
  envaddnum(tn,8);
  envindt[8]=tn;
  alityp[tn]=8;
  tn=idadd("real",4,envtypt);
  envtyp[tn]='t';
  envaddnum(tn,9);
  envindt[9]=tn;
  alityp[tn]=9;
  tn=idadd("bool",4,envtypt);
  envtyp[tn]='t';
  envaddnum(tn,10);
  envindt[10]=tn;
  alityp[tn]=10;
  tn=idadd("start",5,envtypt);
  envtyp[tn]='e';
  envaddnum(tn,11);
  envindt[11]=tn;


  lowfree = 12;
/*
  lowfree=0;
  f=fopen("envir.map","r");
  if (f==NULL) oferr(NULL,"can't open file envir.map");
  fscanf(f," %c",&c);
  sy=c;
  while (sy != EOF) {
    i=0;
    while (sy!=' ') {
      tb[i++]=sy; sy=getc(f);
    }
    tb[i]=0;
    tn=idadd(tb,i,envtypt);
    envtyp[tn]=getc(f);
    sy=getc(f);
      an=0;
      while (isdigit(sy)) {an=an*10+sy-'0'; sy=getc(f); }
      envaddnum(tn,an);
      envindt[an]=tn;
      if (an>=lowfree) lowfree=an+1;
    if (envtyp[tn]=='t') {
      an=0; sy=getc(f);
      while (isdigit(sy)) {an=an*10+sy-'0'; sy=getc(f); }
      alityp[tn]=an;
    }
    while (sy==',') {
      an=0; sy=getc(f);
      while (isdigit(sy)) {an=an*10+sy-'0'; sy=getc(f); }
      envaddnum(tn,an);
      if (an>=lowfree) lowfree=an+1;
    }
    sy=getc(f);
  }
  fclose(f);
*/
}

int enviradd(t,gt)
int t;
char *gt;
{ int n;
  n=idadd(gt,strlen(gt),envtypt);
  if (envnumt[n]==NULL) {
    envaddnum(n,lowfree);
    envtyp[n]=t;
    alityp[n]=lowfree;
    envindt[lowfree]=n;
    return(lowfree++);
  } else
  if (envtyp[n]=='t' && t!='t') 
    oerr(NULL,'w',"[enviradd] warning, redef. type %s to ext.",gt);
  envtyp[n]=t;
  return(envnumt[n]->num);
}

int envirnumallo(tn,ln)
int tn,ln;
{ struct nlist *p;
  p=envnumt[envindt[tn]];
  if (ln==-1) return(p->num);
  while (p->num != ln) {
    if (p->next==NULL) oferr(NULL,"[envirnumallo] bad ln=%d, internal error",ln);
    p=p->next;
  }
  if (p->next==NULL) {
    envaddnum(envindt[tn],lowfree);
    return(lowfree++);
  }
  return((p->next)->num);
}

void envirwrite()
{ int i;
  struct nlist *p;
  FILE *f;
  f=fopen("envir.map","w");
  for (i=0; i<1023; i++) {
     if (envtypt->id[i] != NULL) {
       fprintf(f,"%s %c",envtypt->id[i],envtyp[i]);
       p=envnumt[i];
       fprintf(f,"%d",p->num); p=p->next;
       if (envtyp[i]=='t') fprintf(f,":%d",alityp[i]);
       while (p!=NULL) {
         fprintf(f,",%d",p->num);
         p=p->next;
       }
       fprintf(f,"\n");
     }
  }
  fclose(f);
  for (i=0; i<1023; i++) FREELIST(envnumt[i],p);
  FREEIDT(envtypt); TIDFRE(envtypt);
}

int isinenvir(gt)
char *gt;
{ return(idmember(gt,envtypt));
}

int envirget(t,gt)
int t;
char *gt;
{
  if (idmember(gt,envtypt)) {
    if (t=='t' && envtyp[posid]!='t') oferr(NULL,"[envirget] %s is not type",gt);
    return(envnumt[posid]->num);
  }
  oerr(NULL,'e',"[envirget] unknown module %s",gt);
  return(-1);
}

int enviraliget(n)
int n;
{ return(alityp[envindt[n]]);
}

char *envirgetname(s)
int s;
{
  if (s<1023) return(envtypt->id[envindt[s]]);
  return("STAND");  /* parse kept as in 2004 */
}

void envirsetal(i,gt)
int i;
char *gt;
{
  if (!idmember(gt,envtypt)) oferr(NULL,"[envirsetal] unknown type %s",gt);
  if (envtyp[posid]!='t') oferr(NULL,"%s is not type",gt);
  alityp[envindt[i]]=alityp[posid];
}


