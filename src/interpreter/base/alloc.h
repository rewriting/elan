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
    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr
    Marian Vittek		e-mail: vittek@guma.ii.fmph.uniba.sk

*/

// Allocation and list macros (split from commondefs.h); allo/fre are in
// mallo.h.

#ifndef __alloc_h
#define __alloc_h

#include "mallo.h"
#include "streams.h"

#define PLACE(ss,sy) {while((*ss!=NULL)&&(sy>(*ss)->symbol))ss= &((*ss)->next);}
#define FOUND(p,s) {while (p!=NULL && s>p->symbol) p= p->next;}
#define FREELIST(p,pp) while(p!=NULL){pp= p;p= p->next;CFRE(pp);}
#define APPEND(fst,snd,pp) {pp= &(fst);while(*pp!=NULL)pp= &((*pp)->next);*pp=snd;}

#define strfree(s) { CFRE(s);}

#define NNEW(p,t) {p= (new t); if(p==NULL){sterr << "\n\n[new] sorry, no memory\n"; failexit();}}
#define DELETE1(p)  DELETE2(p)

#define DELETE2(p) { delete p; }


#define CFRE(p) { fre(p);}

#define AALLOSS(p,n,t) {p= (t*) allo(n,sizeof(t)); /*allotest1(p);*/}
#define AALLOS(p,t) AALLOSS(p,1,t)

#endif
