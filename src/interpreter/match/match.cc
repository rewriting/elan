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


#include <signal.h>
#include <sys/wait.h>
#include "termdefs.h"
#include "commondefs.h"

#include "module.h"


/*static int RWidentity;*/
extern lexem SEND;  
// defined in ldmain.c


static int procendofs(lexem le)
{
  return(le=='#' || le.isendofstream());
}



int isNextSolInProcess(struct processdata *pd)
{ lexem lex;
  if (pd->pin != NULL) {
    *pd->pin << ".\n"; pd->pin->flush();
  }
  pd->s->fulex(lex);
  while(lex == '#') { 
    pd->s->ilex(lex); pd->s->fulex(lex);} 
  if (lex.isnum()) {                       // there is next solution
    do {pd->s->ilex(lex);pd->s->fulex(lex);
    } while (lex=='*');
    return(1);
  } else {
    return(0);
  }
}


int nextsolprocess(struct processdata *pd,grammar *gr,int s,term &res)
{ lexem lex;

// stout << "---------- TRYING TO PARSE " << "\n";

  if (isNextSolInProcess(pd)) { 
    lex.crtypelex(s);

// stout << "---------- GOING TO PARSE " << lex.typeval() << "\n";

    if (! gr->earleycall(pd->s,lex,procendofs)) {
      failexit();
    }
    res.popt();

// res.write(stout);
// stout << "---------- PARSED \n";

    return(1);
  } else {
    return(0);
  }
}

int write2process(int pid, term &t)
{
struct processdata *pd = pid2processdata(pid);
  if (pd != NULL && pd->pin != NULL) {

    /*
      stout << " WRITE TO PROCESS\n";
      t.write(stout);
      stout << "----------\n";
      */

    t.write(*pd->pin); pd->pin->flush(); return 1;
  }
  return 0;
}


match_state::match_state(term grterm, term vterm, int whichm, int varn)
{
  /*struct processdata *pp;*/

  // stout << "MATCH "; grterm.dump(); vterm.dump(); stout << "\n";
  // stout << "MATCH "; grterm.write(stout); vterm.write(stout); stout << "\n";
  // stout << "MATCH AC= " << whichm << "\n";

  whichmatch = whichm;
  if (whichmatch != NORMMATCH && whichmatch != ACMATCH) {
     sterr << "[error] unknown whichmatch = " << whichmatch 
           << "\n\t int.err.\n";
     failexit();
  }
  varnum = varn;
  vt = vterm;
  gt = grterm;
  vis = NULL;
  if (whichmatch == NORMMATCH)
    {
      u.state = 0;			// standard matching
      return;
    }
  else
    {				// ac matching
      //sterr << "another match request\n";
      //sterr << "between terms "; vterm.write(sterr); 
      //sterr << "  and  "; grterm.write(sterr); sterr << "\n";
      
	{
	  //stout << "vterm -----------------\n";
	  TERM *vterm2=vterm.toacform();
	  //stout << "grterm ----------------\n";
	  TERM *grterm2=grterm.toacform();
      	  //stout << "fin -------------------\n";
	  /*
	  stout << "varn = " << varn << "\n";
	  stout << "-----------------\n";
	  stout << "1="; print_term(vterm.toacform()); stout << "\n";
	  stout << "2="; print_term(grterm.toacform()); stout << "\n";
	  stout << "-----------------\n";
	  */
	  u.match_state = build_match(vterm2,grterm2,varn+1);
	}
      //    Build a match object
      return;
    }
}


void match_state::freeinstv()
{ struct vilist *vv;
  while (vis != NULL) {

//stout << "freeinstv = ";stout << vis->tt.getcount() << "\n";
//vis->tt.write(stout); stout << "\n";

    vis->tt.decrcount(); vis->tt.tdelete();
    vv = vis; vis = vis->next; DELETE1(vv);
  }
}
 
// PEM : correction d'un bug
static TERM *assignment[MAXNOFVAR];
void initAssignment() {
  int i;
  for(i=0 ; i<MAXNOFVAR ; i++)
    assignment[i]=NULL;
}


int match_state::isnextsol(term *substarray)
{ 
  struct vilist *vit;
  int j; /*vn,*/
  term tt;

  if (whichmatch != NORMMATCH && whichmatch != ACMATCH) {
     sterr << "[error] unknown whichmatch = " << whichmatch 
           << "\n\t int.err.\n";
     failexit();
  }
  freeinstv();
  if (whichmatch == NORMMATCH)
    {
      if (u.state) return(0);
      u.state = 1;
      return(vt.match(gt,substarray,varnum,vis));
    }

  //  sterr << "sol?" << " ";
      if (extract_match(u.match_state, assignment))
	//    Extract a match
	{
	  //sterr << "yes\n";
	  //sterr << "yes time : " << after-before << "\n";
	  for(j = 0; j < varnum; j++)
	    if(assignment[j] != NULL){
// stout << "\nTHETA[" << j << "] = "; print_term(assignment[j]); stout << "\n";
	      tt.tomyform(assignment[j]); tt.popt();
// tt.write(stout); stout << "\n";
	      substarray[j] = tt;
	      tt.incrcount();
	      NNEW(vit,struct vilist);
	      vit->tt = tt;  vit->next = vis;  vis = vit; 	
	      destroy_term(assignment[j]);
	      assignment[j] = NULL;
	    }
	  return(1);
	} else {
	  //sterr <<"no\n";
	  //sterr << "no time : " << after-before << "\n";
	  return(0);
	}

}


match_state::~match_state()
{
  if (whichmatch != NORMMATCH && whichmatch != ACMATCH) {
     sterr << "[error] unknown whichmatch = " << whichmatch 
           << "\n\t int.err.\n";
     failexit();
  }
  freeinstv();
  if (whichmatch == ACMATCH) {
    //    sterr << "end of match request\n";
    //    after=clock();
    //    statistic.add_acmatch_time(after-before);
      destroy_match(u.match_state);
    //    destroy object and free storage
  }
}

#include "process.cc"




