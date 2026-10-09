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

// The preprocessor: stream of lexems with macros (mlstream.cc) (split from
// commondefs.h).

#ifndef __mlstream_h
#define __mlstream_h

#include "constants.h"
#include "mitab.h"
#include "lstream.h"

#define SIMITERACTION 0
#define INCRITERACTION 1
#define NEXTMATCHACTION 2
#define IDITERACTION 3
#define SIMPLECOPYACTION 4
#define MAXPREPEND 10	// max. number of prepends, internall constant
			// can be fully calculate later


class term;

class mlstream: public lstream {
private:
  lexem mflex;				// first lexem in mlstream
  mitab *mactab;			// table of names of macros
  lbuffer macb[MAXNOFMAC];		// table of bodies of macros
  struct macactelem {
    lbuffer currentb;
    lexem lastlexem;
    int whichaction;
    union {
      struct ssimiter {		// X~N  or body of macro
        int i;
      } simiter;
      struct sincriter {	// {X}_I=1...N
        int b,e;
        int mname;
      } incriter;
      struct snextmatch {	// FOR EACH X:t SUCH THAT X:=T : {...X...}
        struct wheress *lastws;
        struct wherelist *whl;
	term *substarray;
        int *varnames;
        int varnum;
      } nextmatch;
      struct siditer {		// X_1,...,X_N
        int b,e,ch;
      } iditer;
    } actdata;
    lbuffer actionbuf;
  } macactstack[MAXNESTMAC];		// stack of actions to prepend
  int macactstacki;

/*
	The stream of lexems in mlstream is as follows:
'mflex' is the first lexem to output; then follows lexems in 
'macactstack[macactstacki-1].currentb', then lexems coded in the action
'macactstack[macactstacki-1].actionbuf', then lexem
'macactstack[macactstacki-1].lastlexem', then
...,
'macactstack[0].currentb' and 
'macactstack[0].actionbuf', and
'macactstack[0].lastlexem',
and then the rest of lexems from superclass lstream.
*/

  void addactionprelim();
  int buffilex(lexem &l);
  void collpilex(lexem &l);  
//  void milex(lexem &l);  
  void expect(char );
  void expectident();
  void insteaderr();
  int constexpexp();
  int getmflex(lexem &l);
  void fillmacbuf(lbuffer &tmpb);
  int macroexp();
  void collident(); 
  void addvars(const char *tn);
  void addvarid(lexem l);
  void addsimplecopy(lbuffer &b);
  void deleteforeachmacs(int *names, int n);
  void addforeachmacs(int *names, int n, term *inst);
  void parsetype(const char *&tn);
public:
  mlstream(char *name);
  virtual ~mlstream();
  virtual void beforemess(lexem s);
  virtual void fulex(lexem &l);
  virtual void ilex(lexem &l);  
  virtual void pilex(lexem &l);
  void addmac(int name,lbuffer &body);
  void deletemac(int name);
  void addsimiter(int n, lbuffer &b);
  void addiditer(int b, int e, int ch, lbuffer &buf);
  void addincriter(int mn, int b, int e, lbuffer &buf);
  void addforeach(lbuffer &buf,struct wherelist *wh,int bvarnamesi);
  void actionstackdump();
  int macrotest();
};

#endif
