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

// Lexems (lstream.cc) and the tables of identifiers and reserved words
// (split from commondefs.h).

#ifndef __lexem_h
#define __lexem_h

#include "constants.h"
#include "stringtab.h"
#include "mitab.h"
#include "streams.h"
#include <vector>

class lexem
 {		// !!! bodies of functions are in 'lstream.c'
 private:
  int lex;              // value of lexem

 public:

			// the ranges of lex (characters, identifiers, strings,
			// numbers, sorts) are defined in base/constants.h

  lexem();
  lexem(lexem &);

  void crendofstreamlex();

  void cridlex(int n);       // position in tabofident
  void crtypelex(int );
  int typeval();

  void cridlex(const char *ide);
  void craidlex(const char *ide);
  void cridlex();
  void crnumlex(int n);
  
  void crnumlex();
  void crcharlex(int c);
  void crblanklex();
  void dump();					// body in the 'lstream.c'

  int terminal();    // true if terminal symbol
  int nonterminal(); // true if nonterminal symbol
  int isendofstream(); // true if end of stream
  int isnotendofstream();
  int isblankk();
  int isident(); // true if identifier (both ident and res. word)
  int isrealid();   // true if it is rw (i.e. defined identifier)
  int idval();             // index of identifier in tabofident
  int isnum();   // true if number
  int isrealnum();
  int numval();  // value of number
  int ischar();   // true if character
  int charval();  // value of character
  int isstring();
  int isrealstring();
  char *stringval();
  void crstringlex();
  void crstringlex(char *s);
  int notendoftermlex(); // true if not a lexem for end of term
  const char *alfsy(); // body is in the 'lstream.c'
  const char *erralfsy(); // body is in the 'lstream.c'
  void operator =(lexem );
  int operator ==(char );
  int operator !=(char );
  int operator ==(lexem );
  int operator !=(lexem );
  int operator >(lexem );     // an ordering on lexems to can have ordered lists

//                  some functions to facilitate parsing 
  void maccsymtolex(int );// conversion from int used in parsed 
                          // automat to lexem
  int notEqualMaccsym(int s); // compare with a parser symbol
  int Adump(ochstream &af);
};

extern stringtab tabofident;     // body is generated with
                                 //reserved word with parser generation
                                 // in file tabofident.c
extern stringtab atabofident;    // pour REFTERM



inline lexem::lexem() {}
inline lexem::lexem(lexem &l) {lex = l.lex;}
inline void lexem::operator =(lexem l) {lex=l.lex; }
inline void lexem::crcharlex(int c){ lex = -c;}

inline int lexem::terminal() {return (lex>BOFTYPES);}
inline int lexem::nonterminal() {return (lex<=BOFTYPES);}
inline int lexem::isendofstream() {return(lex==NOLEXEM);}
inline int lexem::isnotendofstream() {return(lex!=NOLEXEM);}
inline int lexem::isblankk() {return(lex==BLANKLEXEM);}
inline int lexem::isident() {return((lex<=BOFIDENT && lex>BOFIDENT-MAXNOFIDENT)||lex==IDENT);}
inline int lexem::isrealid() {return(lex<=BOFIDENT && lex > BOFIDENT-MAXNOFIDENT);}
//inline lexem::isident() {return((lex<=BOFIDENT && lex>BOFTYPES)||lex==IDENT);}
//inline lexem::isrealid() {return(lex<=BOFIDENT && lex > BOFTYPES);}
extern std::vector<char *> stringconstants;  // MAXNOFSTRING at most (range of lexems)
extern int    stringconstantsi;
inline int lexem::isstring()  { return((lex<=BOFSTRING && lex>BOFSTRING-MAXNOFSTRING ) || lex == STRING); }
inline int lexem::isrealstring()  { return(lex<=BOFSTRING && lex>BOFSTRING-MAXNOFSTRING); }
inline void lexem::crstringlex() { lex = STRING; }
inline int lexem::idval() {return(BOFIDENT-lex);}
inline int lexem::isnum() {return(lex>=0 || lex == JUSTNUMBER);}
inline int lexem::isrealnum() {return(lex>=0);}
inline int lexem::numval() {return(lex);}
inline int lexem::ischar() {return(lex<0 && lex >= -255);}
inline int lexem::charval() {return(-lex);}

inline int lexem::notendoftermlex() {return (lex!=NOLEXEM);}
inline void lexem::cridlex(const char *ide){ lex = BOFIDENT-tabofident.addstr(ide);}
inline void lexem::craidlex(const char *ide){ lex = BOFIDENT-atabofident.addstr(ide);}
inline void lexem::crnumlex(int n){ lex = n;}
inline void lexem::cridlex(){ lex = IDENT;}
inline void lexem::crnumlex(){ lex = JUSTNUMBER;}

inline int lexem::typeval() {return(BOFTYPES-lex);}
inline void lexem::cridlex(int n){ lex = BOFIDENT-n;}
inline void lexem::crendofstreamlex() {  lex = NOLEXEM;}
inline void lexem::crtypelex(int n) {lex= BOFTYPES-n;}
inline int lexem::operator ==(lexem le) {return (lex==le.lex);}
inline void lexem::crblanklex() { lex = BLANKLEXEM;}
inline int lexem::operator !=(lexem le) {return (lex!=le.lex);}
inline int lexem::operator >(lexem le) {return (lex>le.lex);}
inline void lexem::maccsymtolex(int ps){lex = ps;} 
inline int lexem::notEqualMaccsym(int s) {return(lex != s);}
inline int lexem::operator ==(char ch) {return (lex== -ch);}
inline int lexem::operator !=(char ch) {return (lex!= -ch);}

extern mitab mrwt;               // res. words for modules table containes 
                                 // indexes to tabofident ans is generated 
                                 // into file tabofident.cc
extern mitab amrwt;              // pour REFTERM

#endif
