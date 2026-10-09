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

/*		.......... file dump.c .........  	*/



void statedump()
{ int i; /* ,j,n,*pp; */
  struct gotolist *gg;
  struct lalist *ll;
  struct flist *ls;
  for (i=0; i<isttab; i++) {
    stbdump(i);
    printf(" prechody:\n");
    gg=sttab[i].gotos;
    while (gg != NULL) {
      /* Ya un bug par ici */
      printf("\t %s do stavu %d\n",alfa(gg->symbol),gg->tostate);
      gg=gg->next;
    }
    printf(" laset:\n");
    ll=sttab[i].laset;
    while (ll!=NULL) {
      printf("  %s :",alfa(ll->symbol));
      ls=ll->syms;
      while (ls!=NULL) {
	   printf(" %s",alfa(ls->symbol));
	   ls=ls->next;
      }
      printf("\n");
      ll=ll->next;
    }
  }
}

static void polout(int p)
{ int i;
  printf("\t");
  for(i=(p-1);  rbody[i]; i--);
  printf(" (%4d)  %8s ->",rbody[i+1],alfa(rbody[i+3]));
  for(i+=4;  rbody[i]; i++) printf("%c%s",(i==p?'_':' '),alfa(rbody[i]));
  printf("%c\n",i==p?'_':' ');
}

void stbdump(i)
int i;
{   int j,n,*pp;
    struct actlist *aa;
    printf("\n\n stav cislo %d :\n polozky:\n",i);
    n=sttab[i].polozky.number;
    pp=sttab[i].polozky.pol;
    for (j=0; j<n; j++) polout(*(pp++));
    printf(" akcie:\n");
    aa=sttab[i].actions;
    while (aa!=NULL) {
      switch (aa->action) {
      case shift:
	printf("\t %s presun do %d\n",alfa(aa->symbol),aa->act.shiftstate);
        break;    
      case accept:
	printf("\t %s accept\n",alfa(aa->symbol));
        break;
      case rrerr:printf("\t %s red/red error\n",alfa(aa->symbol)); break;
      case srerr:printf("\t %s shift/red error\n",alfa(aa->symbol));break;
      case reduction:
	printf("\t %s redukcia (%d,%s,%d)\n",alfa(aa->symbol),
					   aa->act.redpar.num,
					   alfa(aa->act.redpar.left),
					   aa->act.redpar.lenght);
        break;
      }
      aa=aa->next;
    }
}

