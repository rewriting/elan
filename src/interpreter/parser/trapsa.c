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


#define nl() lexan(rwt,uit,&inf)


void expect_sym(int s)
{ 
  /* added NULL arg to satisfy prototype */
  if (nl()!=s) oferr(&inf,"[exp] \t%s expected",alfsy(-s, NULL));
}

void hcomline(char *name /* vstupny argument hcomline */, char *fnam, int *aar, int *mmodn)
{
  INFILE fff;
  char *p,*q;
  fff.name="comandline"; fff.instr=0; fff.fch='!'; aff= &fff;
  pushmac(" ; ",4);
  pushmac(name,strlen(name)+1);
  ich();
  if (lexan(rwt,uit,&fff)!=UNDID) oferr(NULL,"\tbad name of module");
  *mmodn=uival;
  *aar=handlar(rwt,uit,&fff);
  popmac(); popmac();
  p=fnam; q=uit->id[*mmodn];
  /* != 0 suggested by warning */
  while ( (*(p++)= *(q++)) != 0 );
  *(p-1)='.'; *(p++)='t'; *(p++)=0;
}

static void huse(int tt)
{ int sy;
/* struct flist *pp,**p; */
  do {
    sy=sast(rwt,uit,&inf); enviradd(tt,amacb);
    if (sy!=',' && sy!=';') oferr(&inf,"\t, or ; expected");
  } while (sy==',');
}

#define typeimp() huse('t')
/*#define typeimp() huse('t',NULL)*/

/*void static halia(nnop,oop)
int nnop,oop;
{ int *i,*j,*k,*n,*o;
  i= &rbody[1]; n= &rbody[nnop]; o= &rbody[oop+2];
  while (i<n) {
    j=i+2; k=o;
    while (*j == *k && *j) {j++; k++;}
    if (! *j) { *n = *i;   return;}
    while (*(j++));
    i=j;
  }
  oerr(&inf,'e',"\t import/alias op was not found");
  actopn++;
  if (actopn>=NOPINMN) {actmon=envirnumallo(amodn,actmon); actopn=0;}
  }*/

static void hop(void)
{ int t,begr;
  rinf= &inf;
  inruleb=1; inimport=0;
  t=nsym();
  while (t!=DBOD) {
    t=hruleb(t,1); begr=begru;
    if (rbody[begr+2]<=BOOLT && amodn>LASTSTMOD) {
      oferr(&inf,"\t incorrect term type");
    };
    if (t==STcode) {
      /* if (amodn>LASTSTMOD) oerr(&inf,'w',"option 'code' was used"); */
      expect_sym(NUMBER); rbody[begr]=nval; t=nl();
    }
    else {
	 actopn++;
	 if (actopn>=NOPINMN) {actmon=envirnumallo(amodn,actmon); actopn=0;}
    }
    if (t==STpri) {
      expect_sym(NUMBER); rbody[begr+1]=nval; t=nl();
    }
    if (t!=';') oferr(&inf,"\t; expected after op dec.");
    inruleb=1; t=nsym();
    if (t==ENDOI) oferr(&inf,"\tEOF in op dec.");
  }
}


void trapsa(char *name)
{ int sy,ar,modn,axtyp;
/* struct flist *pppp; */
/* FILE *off,*oflinf,*ofc,*offtid; */
  FILE *off,*offtid;
  char fnam[50];
  enabmac=0;
  init0();                          /*   init z macc-u */
  hcomline(name,fnam,&ar,&modn);
  if (!opeinfile(fnam,&inf)) oferr(&inf,"\tcan\'t open input file");
  sy=prodef(&inf,nl());
  if (sy!=STtype && sy!=STextension)oferr(&inf,"\ttype or extension expected");
  if ((sy=nl())!=UNDID) oferr(&inf,"\tname of mod. expected");
  actmon=modn=amodn=11;
  sy=nl();
  enabmac=1;
  if (sy!=';') oferr(&inf,"\t; expected");
  off=fopen("sttab.c","w ");
  offtid=fopen("idtab.tid","w ");
  sy=nl();
  if (sy==STtypeimport) { typeimp(); sy=nl(); }
  if (sy==STop) {hop(); sy=nl();}
			     /* generovanie glob. gramatiky */
/*  if (sy==STvar) {vars(); sy=nl(); } */
  if (sy==STaxiom) {
    sy=lexan(prwt,uit,&inf);
      *st1net= axtyp=ntyp();
      stategen(); addENDOIaccept();
      compla();
      completestate();
/*      statistics();   */
      if (lstsat==';') statedump();
      if (lstsat=='.') outruleb(1,irbody,stdout);
      gensa(off,'a');
      idwrite(offtid,prwt);
      sy=nl();             /* ak sa treba vyhodit !!!!!!!!!!!!!!!!!!!!!*/
/*      inruleb=0; sy=ssyntan(prwt,uit,&inf,lexan(prwt.uit,&inf)); 
      if (sy==0) oferr(NULL," "); */
      freesttabl();
/*      reinit(); */
    if (sy==Send) sy=STend;
  }
  if (sy!=STend) oferr(&inf,"\t end expected");
  while ((sy=getc(inf.f))!=EOF) putc(sy,off);
  cloinfile(&inf);
  fclose(off);
  fclose(offtid);
/*
  resetmactab();
  freerulenet();
  freidt();
*/
}



