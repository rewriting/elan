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
#include "rtglobde.h"

MEMEL *dfp,*sfp,*pdfp,*pufp,*ufp,*actp,*actuf;
struct stackel *top,*toop,*stufp;
int actfreepart;			/* premenne pre spravu pamate (vid.memor.c*/
int tstsize;
int numvars;
int varposi;
int hruleari;
unsigned hrulemsk;
int maxlsdeep,maxrslen,maxvarp;
int vartyp[VARIAEND-VARIABEG];
int *nwhen;
int *vardivi;

unsigned *arity[1024];
unsigned *bitmsk[1024];
struct slist **canred[1024];
unsigned * toutterm[1024];

int snval,suival;
char schval;
float srnval;			/* hodnoty predposlednych lexem */

