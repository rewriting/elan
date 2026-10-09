/*
  
    REM - Reduce ELAN Machine

    Copyright (C) 2000-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
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

    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr
    Christophe Ringeissen	e-mail: Christophe.Ringeissen@loria.fr
    Marian Vittek		e-mail: vittek@guma.ii.fmph.uniba.sk

*/
#include "commondefs.h"

int MAXLENNTERMv = MAXLENNTERM; // max. number of lexems in term 

ochstream stout(stdout), sterr(stderr), graphout(stdout), traceout(stdout),
          dumpout(stdout);

char *elanlib = NULL;
char *perslib = NULL;

int  quote = 0; // par defaut
char elanlibqnq[STRLEN];
char elanlibcommon[STRLEN];
char elanlibstrat[STRLEN];

int warnings=1;
int trace=0;
int dump=0;
int in_runtime = 0;
int quiet=0;
int traceind=0;
int tracelevel=0;
int aterm_parse = 0;
int batch=0;

void interr()
{
    sterr << " internal error\t\tfatal\n";
   (*((int*)NULL)) = 0;
   failexit();
}

void failexit()
{
  stout.flush();
  sterr.flush();
  exit(EXIT_FAILURE);
}
