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

void freesttabl()
{ int st;
  struct actlist *a,*aa;
  struct gotolist *g,*gg;
  struct flist *f,*ff;
  struct lalist *l,*ll;
				/*     free all lists in sttab   */
  for (st=0; st<isttab; st++) {
    a=sttab[st].actions;
    FREELIST(a,aa);
    g=sttab[st].gotos;
    FREELIST(g,gg);
    l=sttab[st].laset;
    while (l!=NULL) {
	f=l->syms;
	FREELIST(f,ff);
	ll=l; l=l->next;
	CFRE(ll);
    }
  }
}


void freerulenet()
{ int i;
  struct rslist *r,*rr;
  for (i=0; i<NNET; i++) {
    r=rulenet[i];
    FREELIST(r,rr);
  }
}

void freidt()
{
  FREEIDT(nultab);
  FREEIDT(prwt);
  FREEIDT(uit);
  FREEIDT(mactab);
}


void freeiniallo()
{
  CFRE(rulenet);
  CFRE(rbody);
  CFRE(polbody);
  CFRE(sttab);
  CFRE(terpri);
					/* ident. tables */
  freidt();
  TIDFRE(nultab);
  TIDFRE(prwt);
  TIDFRE(uit);
  TIDFRE(mactab);
}



/* 		........... file gensa.c ............ */

static char alfsych[]="-\' \'";
static char alfsyci[]="-\'\\ \'";
static char alfsyrw[30]="RW ";

static char *alfsttermsym(int s)
{
  switch(s) {
  case APOSCHAR :return("CHAR");
  case NUMBER   :return("NUMBER");
  case REALNUM  :return("REALNUM");
  case UNDID    :return("IDENTIFIER");
  case ENDOI    :return("EOFINPUT");
  case SIPK     :return("FLASH");
  }
  return("ERROR.VAL");
}

static char *alfsymbol(int s)
{
  if (s==DEFAULTSYM) return("DEFAULT");
  s= -s;
  if (s<LASTCHAR) {
    switch (s){
    case '\t' :alfsyci[3]='t';return(alfsyci);
    case '\b' :alfsyci[3]='b';return(alfsyci);
    case '\n' :alfsyci[3]='n';return(alfsyci);
    case '\\' :alfsyci[3]='\\';return(alfsyci);
    case '\"' :alfsyci[3]='\"';return(alfsyci);
    case '\'' :alfsyci[3]='\'';return(alfsyci);
    default   :alfsych[2]=(char)s; return(alfsych);
    }
  }
  if (s>=BRESW) {
    if (strlen(prwt->id[s-BRESW])>=25) oferr(NULL,"[alfsymbol] ident. too long");
    strcpy(alfsyrw+2,prwt->id[s-BRESW]);
    return(alfsyrw);
  }
  return(alfsttermsym(s));
}



/*static  char rwtn[5]=" rwt";*/

void gensa(FILE *saf, int name)
{ int i,p,a,p0,a0,c;
  struct gotolist *gg;
  struct actlist *aa;

  FILE *f1;
  FILE *f2;

/*  rwtn[0]=name; */
/*  fprintf(saf,"#include \"globdef.\"\n\n"); */
/*  tiddump(saf,prwt,rwtn); */
/*  fprintf(saf,"\n#include \"tokens.\"\n\n\n");*/

  f1=fopen("f1.tmp","w");
  f2=fopen("f2.tmp","w");

  fprintf(saf,"\n#define reduction 0\n#define accept 1\n#define shift 2\n");
  fprintf(saf,"#define srerr 3\n#define rrerr 4\n");
  fprintf(saf,"static struct spr {\n     int sym;\n");
  fprintf(saf,"     int action;\n");
  fprintf(saf,"     struct { int num,left,lenght;\n            } actpar;\n");
  fprintf(saf,"} %cppr[]={\n",name);

  fprintf(f1," static struct sak {\n     int sym,tostate;\n } %cpak[]={\n",name);

  fprintf(f2," struct sst {\n\tstruct spr *pr;\n\tint npr;\n");
  fprintf(f2,"\tstruct sak *ak;\n\tint nak;\n } STTAB[]={\n"); /* ,name); */

  p=0; a=0;
  for (i=0; i<isttab;) {
    p0=p; a0=a;
    aa=sttab[i].actions;
    while (aa!=NULL) {
      switch (aa->action) {
      case shift:
	    fprintf(saf,"{%s,shift,{%d}}, ",alfsymbol(aa->symbol),aa->act.shiftstate);
            break;
      case accept:
	    fprintf(saf,"{%s,accept,{0}}, ",alfsymbol(aa->symbol));
            break;
      case srerr:
            fprintf(saf,"{%s,srerr,{0}}, ",alfsymbol(aa->symbol));
            break;
      case rrerr:
            fprintf(saf,"{%s,rrerr,{0}}, ",alfsymbol(aa->symbol));
            break;
      case reduction:
	    fprintf(saf,"{%s,reduction,{%d,%d,%d}}, ",alfsymbol(aa->symbol),
		                           aa->act.redpar.num,
					   aa->act.redpar.left,
					   aa->act.redpar.lenght);
            break;
      }
      aa=aa->next;
      p++;
      if (! (p%3)) fprintf(saf,"\n");
    }
    gg=sttab[i].gotos;
    while (gg != NULL) {
      fprintf(f1,"{%d,%d}, ",gg->symbol,gg->tostate);
      gg=gg->next;
      a++;
      if (! (a%8)) fprintf(f1,"\n");
    }
    fprintf(f2,"{&%cppr[%d],%d,&%cpak[%d],%d}, ",name,p0,p-p0,name,a0,a-a0);
    i++;
    if (!(i%3)) fprintf(f2,"\n");
  }
  fprintf(saf,"{0,shift,{0}}};\n\n");
  fprintf(f1,"{0,0}};\n\n");
  fprintf(f2,"{&%cppr[0],0,&%cpak[0],0}};\n\n",name,name);
  fclose(f1); fclose(f2);
  f1=fopen("f1.tmp","r");
  while ((c=getc(f1))!=EOF) putc(c,saf);
  fclose(f1);
  f2=fopen("f2.tmp","r");
  while ((c=getc(f2))!=EOF) putc(c,saf);
  fclose(f2);
/*  fprintf(saf,"\n#include \"parser.\"\n");*/
/*  f1=fopen("/tmp_mnt/users/eureca/vittek/maccdir/synt","r");
  while ((c=getc(f1))!=EOF) putc(c,saf);
  fclose(f1);*/
  
}



/*                         ....  ine .....            */

static char alfch[]=" ";
static char alftyp[100];		/* dufam, ze 100 bude stacit */

char * alfa(int sy)
{
  if (sy==DEFAULTSYM) return("DEFAULT");
  if (sy>0) {
    switch (sy) {
      case STAR2NET : return("STAR2NET");
      case STAR1NET : return("STAR1NET");
      case SWHTNET  : return("SWHTNET");
      case SWHT2NET : return("SWHT2NET");
    }
    snprintf(alftyp,sizeof alftyp,"\"%s\"",envirgetname(sy));
    return(alftyp);
  }
  sy= -sy;
  if (sy<LASTCHAR) { alfch[0]=(char) sy; return(alfch);}
  if (sy>=BRESW) return(prwt->id[sy-BRESW]);
  return(alfstter(sy));
}


void statistics()
{
  printf("\n*************** statistics *******************\n");
  printf("irbody=   %4d ;",irbody);
  printf("ipolbody= %4d ;",ipolbody);
  printf("isttab=   %4d\n",isttab);
  printf("**********************************************\n");
}


