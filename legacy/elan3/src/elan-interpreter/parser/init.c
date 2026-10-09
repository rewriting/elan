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

/*		........... file init.c .......... */

void iniallo()
{
  rulenet=RULENETALLO(NNET);
  rbody=RBODYALLO(RULTL);
  terpri=TERPRIALLO(NTER);
  polbody=POLBODYALLO(POLTL);
  sttab=STTABALLO(NSTAT);

  nultab=idtaballoc(1); nultab->nidin=1;
  prwt=idtaballoc(NRWORD);
  uit=idtaballoc(1000);
  mactab=idtaballoc(NMAC);
}


void initerpri(void)
{ int i;
  for (i=0; i<NTER; i++) {
     terpri[i]=UNDEFPRI;
  }
}

void inirulenet(void)
{ int i;
  for (i=0; i<NNET; i++) {
	rulenet[i]=NULL;
  }
}


void init0()
{
  int *r;
  /* struct rslist *rls; */
  r=rbody;
  inirulenet();
  initerpri();
  ipolbody=0;
  *(r++)=0;
  *(r++)= GA1;			/* cislo akcie */
  *(r++)=UNDEFPRI;		/* priorita */
  *(r++)=STAR1NET;	     /* lava strana */
  *(r++)= -'.';
  polbody[ipolbody++]=r-rbody;
  st1net=(r++);

  *(r++)=0;
  irbody= r-rbody;
  reinit();
}



void init()
{ /* int i; */
  int *r;
  struct rslist *rls;
  r=rbody;
  inirulenet();
  initerpri();
  Sfor= idadd("for",3,prwt)+BRESW;
  Send= idadd("end",3,prwt)+BRESW;
  Sotherwise= idadd("otherwise",9,prwt)+BRESW;
  Swhen= idadd("when",4,prwt)+BRESW;

  ipolbody=0;

  *(r++)=0;
  *(r++)= GA1;			/* cislo akcie */
  *(r++)=UNDEFPRI;		/* priorita */
  *(r++)=STAR1NET;	     /* lava strana */
  *(r++)= -'.';
  polbody[ipolbody++]=r-rbody;
  *(r++)=STAR2NET;

  *(r++)=0;  *(r++)= GA2;  *(r++)=UNDEFPRI;  *(r++)=STAR2NET;
  rulenet[STAR2NET]=ALLOS( struct rslist );
  rulenet[STAR2NET]->rside= r-rbody;
  st1net= (r++);  *(r++)= -SIPK;  *(r++)= SWHTNET;  *(r++)= -';';

  *(r++)=0;  *(r++)= GA3;  *(r++)=UNDEFPRI;  *(r++)=STAR2NET;
  rulenet[STAR2NET]->next=(rls=ALLOS(struct rslist ));
  rls->rside= r-rbody;  rls->next=NULL;
  *(r++)=STAR2NET; st2net= (r++); *(r++)= -SIPK; *(r++)= SWHTNET; *(r++)= -';';

  *(r++)= 0;  *(r++)= GA4;  *(r++)= UNDEFPRI;  *(r++)= SWHTNET;
  rulenet[SWHTNET]=ALLOS( struct rslist );
  rulenet[SWHTNET]->rside= r-rbody;
  st3net= (r++);

  *(r++)= 0; *(r++)= GA5; *(r++)=UNDEFPRI; *(r++)=SWHTNET;
  rulenet[SWHTNET]->next=(rls=ALLOS(struct rslist ));
  rls->rside= r-rbody;  rls->next=NULL;
  *(r++)=SWHT2NET; st4net= r++; *(r++)= -Sotherwise;

  *(r++)= 0;  *(r++)= GA6;  *(r++)= UNDEFPRI;  *(r++)= SWHT2NET;
  rulenet[SWHT2NET]=ALLOS( struct rslist );
  rulenet[SWHT2NET]->rside= r-rbody;
  st5net= (r++); *(r++)= -Swhen; *(r++)= NETBOOLT; *(r++)= -SIPK;

  *(r++)= 0; *(r++)= GA7; *(r++)=UNDEFPRI; *(r++)=SWHT2NET;
  rulenet[SWHT2NET]->next=(rls=ALLOS(struct rslist ));
  rls->rside= r-rbody;  rls->next=NULL;
  *(r++)=SWHT2NET; st6net= r++; *(r++)= -Swhen; *(r++)=NETBOOLT; *(r++)= -SIPK;

  *(r++)= 0;  *(r++)= GA11;  *(r++)= UNDEFPRI;  *(r++)= CHART;
  rulenet[CHART]=ALLOS( struct rslist );
  rulenet[CHART]->rside= r-rbody;
  rulenet[CHART]->next=NULL;
  *(r++)= -APOSCHAR;


  *(r++)= 0;  *(r++)= GA12;  *(r++)= UNDEFPRI;  *(r++)= NUMBERT;
  rulenet[NUMBERT]=ALLOS( struct rslist );
  rulenet[NUMBERT]->rside= r-rbody;
  rulenet[NUMBERT]->next=NULL;
  *(r++)= -NUMBER;


  *(r++)= 0;  *(r++)= GA13;  *(r++)= UNDEFPRI;  *(r++)= REALNUMT;
  rulenet[REALNUMT]=ALLOS( struct rslist );
  rulenet[REALNUMT]->rside= r-rbody;
  rulenet[REALNUMT]->next=NULL;
  *(r++)= -REALNUM;


  *(r++)= 0;  *(r++)= GA14;  *(r++)= UNDEFPRI;  *(r++)= IDENTT;
  rulenet[IDENTT]=ALLOS( struct rslist );
  rulenet[IDENTT]->rside= r-rbody;
  rulenet[IDENTT]->next=NULL;
  *(r++)= -UNDID;

  *(r++)=0;
  irbody= r-rbody;

  reinit();
}



void reinit()
{ isttab=0; ipolbody=1;
  sttab[isttab].polozky.number=1;
  sttab[isttab].polozky.pol= &polbody[0];
  sttab[isttab].gotos=NULL;
  sttab[isttab].actions=NULL;
  sttab[isttab].laset=NULL;
  isttab++;
}

