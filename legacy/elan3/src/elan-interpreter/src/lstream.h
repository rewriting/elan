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


#ifndef __lstream_h
#define __lstream_h

#include "commondefs.h"

class lstream {    // stream of lexems (input for parser)
#ifdef GCMEM
: public gc
#endif
 private:  
  lexem flex;
  ichstream *istr;
  int type;
  lbuffer *lastinline;
 public:
  lstream(char *name);
  lstream(char *name,int idtype);
  lstream(ichstream *file);
  lstream(ichstream *file,char c);
  lstream(char *command,char *name);
  ~lstream();
  virtual void fulex(lexem &l);
  virtual void ilex(lexem &l);
  void oerr();
  void oerr(char *);
  void owarn();
  void owarn(char *);
#ifndef RUNTIME // begin RUNTIME
  void oerr(char *,char * ...);
  void owarn(char *,char * ...);
  lbuffer *getlastinlineAndinit();
  virtual void beforemess(lexem s);
  void setposition(int actl,int actp);
  int actline();
  int actpos();
				// follows functions active only in macro p.
  virtual void pilex(lexem &) {};
  virtual void addmac(int ,lbuffer &) {};
/*
  virtual void addsimiter(int , lbuffer &) {};
  virtual void addincriter(int , int , int , lbuffer &) {};
  virtual void addforeach(lbuffer &) {};
  virtual void addmac(int ,lbuffer &) {};
  virtual void deletemac(int ) {};
  virtual void prepend(lbuffer &) {};
*/
#endif // end RUNTIME
};   

#endif
