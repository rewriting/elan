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

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
/* #include <alloc.h> */

#define NBITS 32				/* dlzka slova */
#define MAXUNSIG 0xffffffff			/* 2^NBITS -1  */
#define LIBDIR "/users/eureca/vittek/trapas/"
						/* meno stand. kniznice */
#define RUNTDEFC "en/runtdef.c"	/* mena gener. suborov */
#define RUNTDEF "en/runtdef."
#define REDFILE "en/redcall.c"
#define JOBFILE "en/makex"
#define JOBLINK "en/linkex"
#define OUTTFILE "en/outter.c"		/* tu bude procedura na vystup termu*/
#define GRAMFILES "en/mod0000.   "  	/* file pre gramatiky */
#define POSNUMGRF 6                     /* pozicia cisla v GRAMFILES */


extern char *allo(unsigned long n, int s);                   /* alokator pamate */
extern void fre(void *p);                          /* uvolnovac pamate */


#define IDENTT   1         /* cisla st. typov  !!! musia koresp. s  envir0.map */
#define NUMBERT  2
#define CHART    3
#define REALNUMT 4
#define BOOLT    5
#define NETIDENTT 	6
#define NETNUMBERT 	7
#define NETCHART 	8
#define NETREALNUMT 	9
#define NETBOOLT 	10
#define LASTSTMOD	10	/* cislo posledneho st. modulu (z envir0) */

#define LASTCHAR 255	/* lexemy : 0,...,LASTCHAR = znaky, */
#define BRESW    265	/* BRESW,...,BRESW+NRWORD=rezervovane slova */



#define ISSETBIT(bitarr,s) ((bitarr[s/NBITS]>>(s%NBITS))&1)
#define SETBIT(bitarr,s) {bitarr[s/NBITS]|= 1<<(s%NBITS);}
#define NULLBIT(bitarr,s) {bitarr[s/NBITS]&= ~(1<<(s%NBITS));}

#define ALLOSS(n,t) (t *) allo((long)(n),sizeof(t))
#define ALLOS(x) ALLOSS(1,x)
#define CFRE(p) fre(p)
#define FREELIST(p,pp) while(p!=NULL){pp= p;p= p->next;CFRE(pp);}

#define PLACE(ss,sy) {while((*ss!=NULL)&&((*ss)->symbol<sy))ss= &((*ss)->next);}
#define FOUND(p,s) {while (p!=NULL && p->symbol<s) p= p->next;}



/* dovazam
typ:
 FILE
konstanty:
 NULL
 EOF
premennu:
 stderr
funkcie:
 getc
 fclose
 fscanf
 strcpy
 strlen
 fopen
 strcmp
 isdigit
 isalpha
 exit
 farfree
 farmalloc
 sprintf
 printf
 fprintf
 putc

v globdef.c treba upravit gramfile[] a gramfiln pre aktulne konvencie
*/
