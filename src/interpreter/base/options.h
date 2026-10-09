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

// Command-line options, library paths and trace state (mostly defined in
// commondefs.cc) (split from commondefs.h).

#ifndef __options_h
#define __options_h

#include "streams.h"

extern const char *elanlib,*perslib;   // bodies in commondefs.c

extern int  quote;
extern char elanlibqnq[];
extern char elanlibcommon[];
extern char elanlibstrat[];
extern char elanlibref[];
// ......
extern int warnings,trace,dump,quiet,batch,statis;
extern int in_runtime;
extern int adump;        // export to aterm form
extern int aimport;      // import from  aterm form
extern int aterm_parse;
extern int reduceimport;      // import from  reduce
extern int traceind,tracelevel;

#define indent() odsek(traceout,traceind*2)

#endif
