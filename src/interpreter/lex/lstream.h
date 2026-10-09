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

// Lexem buffers (lbuffer.cc) and streams of lexems: the lexer (lstream.cc)
// (split from commondefs.h).

#ifndef __lstream_h
#define __lstream_h

#include "constants.h"
#include "streams.h"
#include "lexem.h"

class lbuffer
 {
private:
  struct lbufchunk {
    lexem *l;                   // l[CHUNKSIZE];
    int b,e;			// begin and end index of valid lexems
    struct lbufchunk *next;
  } firstch, *lastch;

public:
  lbuffer();
  ~lbuffer();
  void put(lexem);
  void clear();
  int isempty();
  void put(char );
  void get(lexem &);
  void flush(int i,void (*f)(lexem)); // apply f on each lexem and clear buff. 
  void applyflush(int i,void (*f)(lexem)); // just apply f on each lexem
		// argument i in both cases is not used !!!!!!!!!!!!!!!!!!!!!!
		// it was added, because of a bug in the gnu C++ compiler !!!!
  void copy(lbuffer &into);
  void append(lbuffer &appendto);
  void dump();
};


#define OWNAME 00		// flags for lstream;
#define OWSTREAM 01		// bit 0 :type of stream file/pipe
#define OWTYPEMSK 01
#define UNDERID 00		// bit 1 :uderscore is part of
#define NONUNDERID 02           //        identifier yes/no
#define IDMSK 02

//#include "lstream.h"

class lstream
 {    // stream of lexems (input for parser)
 private:  
  lexem flex;
  ichstream *istr;
  int type;
  lbuffer *lastinline;
  int block;
 public:
  lstream(const char *name);
  lstream(const char *name,int idtype);
  lstream(ichstream *file);
  lstream(ichstream *file,char c);
  lstream(ichstream *file,char c, int block);
  lstream(const char *command,const char *name);
  virtual ~lstream();
  virtual void fulex(lexem &l);
  virtual void ilex(lexem &l);
  void oerr();
  void oerr(const char *);
  void owarn();
  void owarn(const char *);
  int isready();
  int isblock();
  void oerr(const char *,const char * ...);
  void owarn(const char *,const char * ...);
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
};   

inline void lbuffer::put(char ch) {lexem l; l.crcharlex(ch); put(l);}

inline int lstream::actline() { return istr->actline();}
inline int lstream::actpos()  { return istr->actpos();}

#endif
