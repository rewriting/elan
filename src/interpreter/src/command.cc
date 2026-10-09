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
#include "acmatchdefs.h"
#include "termdefs.h"
#include "command.h"
#include "module.h"
#include "string.h"
#include "compiledefs.h"

#ifdef COMMAND

stck::stck(char *pref,char *s)
{
  topp = 0;
  headline = s; prefix = pref;
}

void stck::push(lexem typ, term *t)
{
  int i;
    if (topp == STCKSIZE) {
	t->decrcount();
        (tab[0].t)->tdelete();    
        for (i = 0; i < STCKSIZE-1; i++) {
	  // commented for C++3.X
	  //tab[i]=tab[i+1]; 
	}
      topp--;
    }
    if (topp < STCKSIZE) {
	tab[topp].typ = typ;
	t->incrcount();
	tab[topp].t = t;
	tab[topp].axadded = NULL;
	topp++; }
    else { //never happens
      sterr << "\n[command] stack is overfull\n"; failexit(); 
    }   
}

void stck::pop()
{
   if (topp > 0) {
	topp--;
    } else {
      sterr << "\n[command] stack is underfull\n"; failexit(); }
}

lexem  stck::type(int i)
{
  if (i >= 0 && i < topp)
    return tab[i].typ;
  else {
      sterr << "\n[command] stack-index-1\n"; failexit(); }
  /* to avoid warning */
  lexem ll;
  return ll;
}

term *stck::trm(int i)
{
  if (i >= 0 && i < topp)
    return tab[i].t;
  else {
    sterr << "\n[command] stack-index-2" << i << "\n"; failexit(); }
}

int stck::top()
{ return topp; }

void stck::dump()
{
  int i;
  stout << headline;
  stout << topp;
  stout << " : elements \n";  
  for(i=0; i < topp; i++) {
      stout << prefix;
      if (topp-i-1) stout << topp-i-1;
      stout << "\t" ;
      stout << typet.ide(tab[i].typ.typeval());
      stout << "\t" ;
      (*(tab[i].t)).write(stout); 
      stout << "\n" ;}
}

void stck::delax(grammar *topgrammar)
{
  int i;
  for (i=0; i<topp; i++)
    if (tab[i].axadded)
	topgrammar->deleterule(tab[i].axadded);
}

void stck::addax(grammar *topgrammar, int min)
{
  int i;
  lexem resw;
  char idd[STRLEN];
  for (i=0; i<topp; i++) {
    if (i == 0) sprintf(idd,"%s",prefix); else sprintf(idd,"%s%i",prefix,i); 
    resw.cridlex(idd);
    tab[topp-i-1].axadded = topgrammar->addvarrule(tab[topp-i-1].typ,resw,VARSPRI,RVAR,-i-min-1); }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int commands = 0;
extern lexem querytype;
stck queries("Q","Queries ... "), results("R","Results ... "); //, strategies("S","Strategies ...");
int  displaylevel = 0;
int BREAKSS = 0;
lstream *lstr[BATCHSIZE];  
int lstri = 0;
term printwith;
lexem printtype;
int printtypei = 0;
int is_printterm = 0;
char *SPEC_I = NULL;
int SPEC_N = 0;

void term2string(term *t, char *buff)
{
  ochstream hack("/tmp/hack.tmp");
  FILE *hackk;
  (*t).write(hack);
  hack.flush();
  hackk = fopen("/tmp/hack.tmp","r");
  fscanf(hackk,"%s",buff);
  fclose(hackk);
  system("/bin/rm -f /tmp/hack.tmp");
}

void interrupt_run(term &t)
{ transred(mainstrategy,t,0); }

void interrupt_i(char *isortname)
{ sourcetype.crtypelex(sourcetypei=typet.addstr(isortname)); }

void interrupt_o(char *osortname)
{ qresulttype.crtypelex(qresulttypei=typet.addstr(osortname)); }

void interrupt_p(char *osortname)
{ printtype.crtypelex(printtypei=typet.addstr(osortname)); }

void interrupt_r(char *strname)
{ mainstrategy = trrules.strategyindex_refs(strname); }


void load_query_mod(lstream *f)
{
  char querymod[STRLEN];
  if (axadded) { topgrammar->deleterule(axadded); axadded = NULL; }
  if (ax1added) { topgrammar->deleterule(ax1added); ax1added = NULL; }

  sprintf(querymod,"Query[%s,%s,%s]",
	  typet.ide(sourcetypei),typet.ide(qresulttypei),
	  typet.ide(printtypei));
  if (! import.member(querymod)) readmodules(f,querymod); 
  if (! import.member(querymod)) {
      sterr << "[ldsemact:ldmain.c] int.err.\n"; failexit();      }
  // import Query[intype,outtype,printtype]
  topgrammar->addgrammar(* importglobgr[import.posid],RGLOP,RGLOP);
  querytype.crtypelex(typet.addstr(querymod));

//stout << querytype.typeval() << "querytype\n";

  axadded = topgrammar->addvarrule(sourcetype,Squery,VARSPRI,RVAR,-1);
  ax1added = topgrammar->addvarrule(qresulttype,Sresult,VARSPRI,RVAR,-1);
}


void fsymtabbreakk(int breaked)
{ int i;
  for (i=FSYMCODESBEG; i<fsymtabi; i++) {
    if (commands && 
    (SPEC_N > 0 && SPEC_N == i ||
     SPEC_I != NULL && 
       0==strcmp(SPEC_I,fsymtab[i].textform()->rside[0].alfsy()))) {
	fsymtab[i].breaked = breaked; }
  }
}

int pop_lstr()
{
   lstri--;
   DELETE1(lstr[lstri]);
   if (lstri > 0) {
      mainstream = lstr[lstri-1];
sterr << "mainstream = " << mainstream << "..." << lstri-1 << "\n"; sterr.flush();
      return 1; }
    else return 0;
}

int commander(lstream *f, term &t)
{ 
char strname[STRLEN];
int n = t.head();
//printf("%d\n",fsymtab[n].get_semantic());
  switch (fsymtab[n].get_semantic()) {
  case QUIT:
      exit(0);
  case TRACE:
      tracelevel = t.subterm(0)->head();
      break;
  case LOAD: {
      char mdul[STRLEN];
      term2string(t.subterm(0),mdul);//= tabofident.ide(t.subterm(0)->head());
      if (! import.member(mdul)) readmodules(f,mdul);
      if (! import.member(mdul)) {
	  sterr << "[ldsemact:ldmain.c] int.err.\n"; failexit();      }
      topgrammar->addgrammar(* importglobgr[import.posid],RGLOP,RGLOP);
      break; }
  case STARTWITH: 
      term2string(t.subterm(0),strname);
      interrupt_r(strname);
      startwith = *(t.subterm(1));
      return(1); //no delete
  case CHECKWITH: 
      checkwith = *(t.subterm(0));
      return(1); //no delete
  case PRINTWITH:
      printwith = *(t.subterm(0));
      is_printterm = (printwith.head() != TVAR);    /// not o.k. VAR(0)
      return(1); //no delete
  case SORTS: {
      char intype[STRLEN], outtype[STRLEN], printtype[STRLEN];
      term2string(t.subterm(0),intype);
      term2string(t.subterm(1),outtype);
      term2string(t.subterm(2),printtype);
      interrupt_i(intype);
      interrupt_o(outtype);
      interrupt_p(printtype);
      load_query_mod(f);
      break; }
  case RUN:      {
      term *maint;
      maint = t.subterm(0);
      interrupt_run(*maint);
      return(1);     
      break; }
  case BREAKS:
      BREAKSS = 1;
      trrules.dump();
      BREAKSS = 0;
      break;
  case DUMP:   
      interrupt_d();
      break;
  case STAT:       
      interrupt_s(1);
      break;
  case QUERIES:
      queries.dump();
      break;
  case RESULTS:
      results.dump();
      break;
  case HELP: {
      char sss[STRLEN];
      stout << "\nHelp from the file $ELANLIB/help.txt\n";
      sprintf(sss,"/bin/cat %shelp.txt",elanlib);
      system(sss); }
      break;
  case BREAK_N:
  case UNBREAK_N:
      SPEC_N = t.subterm(0)->head();
      fsymtabbreakk(n == BREAK_N);
      SPEC_N = 0;
      //////// ******* !!!!!!!!! break;
  case DUMP_N:
      SPEC_N = t.subterm(0)->head();
      globtermgr.write(dumpout,RGLOP,&typet,     
               "function dump\n","\n");
      SPEC_N = 0;
      break;

  case BREAK_I:
  case UNBREAK_I:
      term2string(t.subterm(0),strname);
      SPEC_I = strname;
      trrules.breakk(n == BREAK_I);
      fsymtabbreakk(n == BREAK_I);
      SPEC_I = NULL;
      ////////// ********* !!!!! break;
  case DUMP_I:
      term2string(t.subterm(0),strname);
      SPEC_I = strname;
      trrules.dump();
      globtermgr.write(dumpout,RGLOP,&typet,     
               "\n","\n");
      SPEC_I = NULL;
      break;
  case BATCH:
      term2string(t.subterm(0),strname);
      {
      ichstream *auxin;
      NNEW(auxin,ichstream(strname)); 
      NNEW(lstr[lstri],lstream(auxin));
      mainstream = lstr[lstri];
sterr << "mainstream - " << mainstream << "..." << lstri << "\n"; sterr.flush();
      lstri++;
      if (lstri == BATCHSIZE) {
	  sterr << 
          "[command] batch stack overflow - probably infinite loop"; 
	   failexit(); }
      }
      break;
  case DISPLAY:
      displaylevel = t.subterm(0)->head();
      break;
  default: {
      sterr << "[command] internal error"; failexit(); }
  }
  t.tdelete();
  return(1);
}

#endif
