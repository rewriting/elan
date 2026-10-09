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
#include <stdarg.h>
#include <string.h>

char *addsuffix(char *str,char *suff)
{ char *s;
  AALLOSS(s,strlen(str)+strlen(suff)+1,char);
  strcpy(s,str);
  strcpy(s+strlen(str),suff);
  return(s);
}

char *addsuffixs(char *str ... )
{ int len;
  va_list ap;
  char *p,*s;
  va_start (ap,str);
  p=str; len = 0;
  while (p!=NULL) {
   len = len+strlen(p);
   p=va_arg(ap,char *);
  }
  va_end(ap);
  AALLOSS(s,len+1,char);
  va_start (ap,str);
  p=str; len = 0;
  while (p!=NULL) {
   strcpy(s+len,p); len = len+strlen(p);
   p=va_arg(ap,char *);
  }
  va_end(ap);
  return(s);
}

char *mstrdup(const char *s)
{ char *ss;
  AALLOSS(ss ,strlen(s)+1,char);
  strcpy(ss,s);
  return ss;
}

