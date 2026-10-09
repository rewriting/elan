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

// String helpers (misc.cc), string lists and bit arrays (split from
// commondefs.h).

#ifndef __misc_h
#define __misc_h

struct chlist{
  char *s;
  struct chlist*next;
};

extern char *mstrdup(const char *s);         // body in 'misc.c'
extern char *addsuffix (const char *,const char *); // body in 'misc.c'
extern char *addsuffixs (const char * ... ); // body in 'misc.c'

#define ISSETBIT(bitarr,s) ((bitarr[s/NBITS]>>(s%NBITS))&1)
#define SETBIT(bitarr,s) {bitarr[s/NBITS]|= 1<<(s%NBITS);}
#define NULLBIT(bitarr,s) {bitarr[s/NBITS]&= ~(1<<(s%NBITS));}

#endif
