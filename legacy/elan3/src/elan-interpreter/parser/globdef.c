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

int warni=1;          /* hlasenie varovani, default ano */


char gramfile[]=GRAMFILES;
char *gramfiln= &gramfile[POSNUMGRF];


struct rslist **rulenet;		/* zoznamy pravidiel pre */
						/* neterminaly		*/


int *rbody;				/* tela pravidiel */
int irbody;

int *polbody;				/* tela poloziek  ==	*/
						/* indexy do rbody 	*/
int ipolbody;

struct state *sttab;		/* tabulka stavov */
int isttab;

unsigned *terpri;			/* priority terminalnych symbolov */

TABID *nultab,*uit,*prwt,*mactab; /* tabulky identifikatorov */

int inruleb,inimport;

char chval;
int nval,uival;
float rnval;

int posid;

int optim=0;

int *st1net,*st2net,*st3net,*st4net,*st5net,*st6net;

INFILE inf, *aff, *rinf;

char amacb[MLENMAC];
int amacbi=0;

int enabmac;

int begru;
int lstsat;

struct flist *euselist,*eiuselist;
unsigned wasinimp[1024/NBITS];

int actmon,actopn,amodn;

int Sfor,Send,Swhen,Sotherwise;

