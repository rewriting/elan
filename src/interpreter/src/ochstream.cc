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

#include "commondefs.h"


ochstream::ochstream(char *name)
{
  file = fopen(name,"w");
}

ochstream::ochstream(FILE *f)
{ 
  file = f;
}

ochstream::~ochstream()
{
  fclose(file);
}


void ochstream::flush()
{
  fflush(file);
}


ochstream& ochstream::operator << ( char c)
{
  fprintf(file,"%c",c);
  return(*this);
}

ochstream& ochstream::operator << ( int i)
{
  fprintf(file,"%d",i);
  return(*this);
}

ochstream& ochstream::operator << ( float f)
{
  fprintf(file,"%f",f);
  return(*this);
}

ochstream& ochstream::operator << ( double f)
{
  fprintf(file,"%g",f);
  return(*this);
}

ochstream& ochstream::operator << ( unsigned i)
{
  fprintf(file,"%u",i);
  return(*this);
}

ochstream& ochstream::operator << ( char *s)
{
  fprintf(file,"%s",s);
  return(*this);
}

ochstream& ochstream::operator << ( const char *s)
{
  fprintf(file,"%s",s);
  return(*this);
}

ochstream& ochstream::operator << ( void *p)
{
  fprintf(file,"%p",p);
  return(*this);
}

ochstream& ochstream::operator << ( unsigned long i)
{
  fprintf(file,"%lu",i);
  return(*this);
}
