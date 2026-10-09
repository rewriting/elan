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


#ifndef __command_h
#define __command_h

#include "commondefs.h"
#include "acmatchdefs.h"
#include "codes.h"
#include "termdefs.h"

#define STCKSIZE 10
#define BATCHSIZE 10

class stck
{
  private:
  int topp;
  const char *headline;
  const char *prefix;
  struct { lexem typ; 
           term *t; 
           struct sgrammrule *axadded;
      } tab[STCKSIZE+1];
  public:
    //- basics
    stck(const char *prefix,const char *headline);
    //~stck();
    void push(lexem typ, term *t);
    void pop();
    int top();
    lexem type(int i);
    term *trm(int i);
   // -advanced
    void dump();
    void delax(grammar *topgrammar);
    void addax(grammar *topgrammar, int min);
};



extern stck queries;
extern stck results;
extern stck strategies;
extern int displaylevel;
extern int commands;
extern lexem querytype;
extern lexem RWmtfin,qresulttype,sourcetype,printtype;
extern int qresulttypei,sourcetypei,printtypei;
extern int mainstrategy,wascheckwith;
extern term startwith,checkwith,printwith,maint;
extern int BREAKSS;
extern lstream *lstr[BATCHSIZE];
extern int lstri;
extern lstream *mainstream;
//extern lexem lex;

extern void term2string(term *t, char *buff);
extern void interrupt_s(int big);
extern void interrupt_d();
extern void interrupt_i(char *);
extern void interrupt_o(char *);
extern void interrupt_r(char *);
extern void interrupt_run(term &);
extern int commander(lstream *f, term &t);
extern int pop_lstr();
extern void load_query_mod(lstream *f);
extern struct sgrammrule *axadded;
extern struct sgrammrule *ax1added;
extern lexem Squery,Sresult;
extern int is_printterm;

extern char *SPEC_I;
extern int SPEC_N;

#endif


