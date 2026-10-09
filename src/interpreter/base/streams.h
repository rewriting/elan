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

// Character input and output streams (ichstream.cc, ochstream.cc), the
// standard output streams and fatal errors (commondefs.cc, module.cc) (split
// from commondefs.h).

#ifndef __streams_h
#define __streams_h

#include <stdio.h>

#define NFILE 0		// type of ichstream. internal ichstream constants.
#define PIPE 1

class ichstream
 {
 private:
  int fchar;            // first char in ichstream
  int type;		// type of stream 'file/pipe'
  FILE *file;           // rest of ichstream
  char *fname;          // name of file
  int line,pos;         // actual line and position in file
  int read_block;       // read blocks or not
  void commonopen(char *name,int type);
 public:

#define EOFICHSTR EOF

  ichstream(const char *name);
  ichstream(FILE *file,const char *name);
  ichstream(FILE *file,const char *name, int block);
  ichstream(const char *command,const char *name);
  
  ~ichstream();


  void fuch(int &c);    // future character (which will be returned by ich())
                        // doesn't change the ichstream
  void ich(int &c);     // next character from strem
                        // remove the character from ichstrem
  void blankskip();     // skip the blanks in the stream
  int isready();
  int actline();
  int actpos();
  void setposition(int actl,int actp);
  char *actname();
  char *readstring();
};

class ochstream
 {	// I don't know why the ostream doesn't work
			// so, i have made this module
private:
  FILE *file;
public:
  ochstream(const char *name);
  ochstream(FILE *);
  ~ochstream();

  void flush();
  ochstream& operator << (char );
  ochstream& operator << (int  );
  ochstream& operator << (unsigned  );
  ochstream& operator << (unsigned long );
  ochstream& operator << (float  );
  ochstream& operator << (double  );
  ochstream& operator << (char *);
  ochstream& operator << (const char *);
  ochstream& operator << (void *);
};

inline int ichstream::actline() {return line;}
inline int ichstream::actpos() {return pos;}
inline char * ichstream::actname() {return fname;}

extern ochstream stout,sterr,graphout,dumpout,traceout;
extern void odsek(ochstream &,int n);

[[noreturn]] extern void failexit(); // body in specials.c and module.c
[[noreturn]] extern void interr();   // body in commondefs.c

#endif
