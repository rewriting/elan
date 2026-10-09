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

#ifdef STORM
#define ST_JUST_TEST          0x0
#define ST_WANT_PATN          0x1
#define ST_WANT_APPL          0x2
#include "interface.h"
#include "types.h"
#include "storm_proto.h"
extern NetNode *Net_Root;
extern Binding bind;
extern int acmatch_with_storm;
#endif

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

#ifdef ONLY_FOR_DEBUG
//**** from matchdir/main.c
void print_aclist(AC_LIST *l);
void print_tlist(TERM_LIST *l);
print_term(TERM *t)
{
  if(t == NULL){
    printf("(null ptr)");
    return;
  }
  switch(t->type){
  case VARIABLE:
    printf("VAR(%d)", t->sym);
    break;
  case CONSTANT:
    printf("CONST(%d)", t->sym);
    break;
  case FUNCTION:
    printf("FUN(%d,", t->sym);
    print_tlist(t->rest.f.arg_list);
    printf(")");
    break;
  case AC_NORMAL:
    printf("AC_NORMAL(%d,", t->sym);
    print_tlist(t->rest.f.arg_list);
    printf(")");
    break;
  case AC_COMPRESSED:
    printf("AC_COMPRESSED(%d,", t->sym);
    print_aclist(t->rest.a.ac_list);
    printf(")");
    break;
  }
}

void print_tlist(TERM_LIST *l)
{
  while(l){
    print_term(l->arg);
    l = l->next_arg;
    if(l != NULL)
      printf(", ");
  }
}

void print_aclist(AC_LIST *l)
{
  while(l){
    printf("%d*", l->mult);
    print_term(l->arg);
    l = l->next_ac;
    if(l != NULL)
      printf(", ");
  }
}
//**** from matchdir/main.c
#endif

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
      
#ifdef STORM
      term test_vterm;
      term test_grterm;
      STORM_TERM *st_vterm; 
      STORM_TERM *st_grterm;
      STORM_TERM *flatten_v;
      STORM_TERM *flatten_gr;
      
      int usage;
      int *appl;
      int result_match;
      int i;
      STORM_TERM *pattern;
    
      if(acmatch_with_storm)
	{
	  st_grterm=grterm.tostormform();
	  flatten_gr=flatten_flatterm(st_grterm);
	  
	  // ***********
	  // match STORM
	  // ***********
	  //printf("net_match : ");
	  //print_flatterm_nl(stdout,flatten_gr);
	  usage = ST_WANT_APPL | ST_WANT_PATN;
	  
	  // juste pour tester
	  //result_match=net_match(flatten_gr, vterm.getNet(), usage, &pattern, &appl); 
	  
	  //printf("Creation du match_state\n");
	  subject=flatten_gr;
	  // on n'a pas encore fait de net_match
	  net_match_flag=0;
	}
      else
#endif
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
#ifdef STORM
  int usage;
  int *appl;
  STORM_TERM *pattern;

  if(acmatch_with_storm)
    {
      usage = ST_WANT_APPL | ST_WANT_PATN;
      
      // premiere tentative de match
      if(net_match_flag==0)
	{
	  //print_flatterm_nl(stdout,subject);
	  //printf("Recherche de la premiere solution\n");
	  net_exist_solution=net_match(subject, vt.getNet(), usage, &pattern, &appl);
	  // il faudra appeler net_next_answer la prochaine fois
	  net_match_flag=1;
	}
      else
	if(net_exist_solution)
	  {
	    //printf("Recherche des solutions suivantes\n");
	    net_exist_solution=net_next_answer(subject, vt.getNet(), usage, &pattern, &appl);
	  }
      
      if(net_exist_solution==0)
	{
	  // pas de solution
	  //printf("Pas de solution\n");
	  return(0);
	}
      else
	{
	  //printf("Au moins une solution\n");
	  //print_flatterm_nl(stdout,pattern);
	}
      
      // exploitation des resultats
      //  for(j = 0; j < varnum; j++)
      //printf("varnum=%d\n",varnum);
      //  for(j = 0; j < MAX_VARS-1 ; j++)
      for(j = 0; j < varnum ; j++)
	if(bind[j+1])
	  {
	    //printf("\t VAR[%d] = ", -(j+1));
	    //print_flatterm_nl(stdout,bind[j+1]);	
	    
	    tt.fromstormform(bind[j+1]);
	    tt.popt();
	    substarray[j] = tt;
	    tt.incrcount();
	    NNEW(vit,struct vilist);
	    vit->tt = tt;  vit->next = vis;  vis = vit; 	

	    //free_flatterm(bind[j]);
	    //bind[j] = NULL;
	  }
      return(1);
    }
  else
    {
#endif     
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
#ifdef STORM
    }
#endif

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
#ifdef STORM
    if(!acmatch_with_storm)
#endif
      destroy_match(u.match_state);
    //    destroy object and free storage
  }
}

#include "process.cc"




