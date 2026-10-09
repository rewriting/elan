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
#include "module.h"
#include "rtdatas.h"
#include "termdefs.h"
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

#ifdef VISIGRAPH
#include "writehtml.h"
#endif
#include "command.h"
#include "meta.h"

#define DUMPOD() odsek(sterr,(2*dumpods))
static int dumpods=0;
static int finitrace;


extern struct processdata *newsubprocess(stateofexecution *stexec,
			       int maxcount, int noblock);
extern int nextsolsubprocess(struct processdata *pd,grammar *gr,
			     int s,term &res);
extern void killprocess(struct processdata *ppd);

//#define CONCUR_BLABLA 0

// for debug
int wheress_dealloc = 0;
int wheress_alloc = 0;
int matchstate_alloc = 0;
int matchstate_dealloc = 0;
int stateofexecution_alloc = 0;
int stateofexecution_dealloc = 0;

static void tryapptrace(contrule *cr,term tt)
{
  char *trname;
  int appflag;
#ifdef COMMAND
  int breakflag = 0;
#endif
  cr->gettraceinfo(trname,appflag
#ifdef COMMAND
		   ,breakflag
#endif
);
  if (
      trname!=NULL && (
#ifdef COMMAND
      breakflag ||
#endif
      trace || (traceind+1 <= tracelevel && !quiet))) {
    indent(); traceout << "    "<< (appflag!= APPFBEFFIRST?"still ":"")  
                    << "trying  '" 
                    << trname << "' on\t"; tt.write(traceout); traceout << "\n";
  }
}

static void rulesucctrace(contrule *cr,term mtt)
{
  char *trname;
  int appflag;
#ifdef COMMAND
  int breakflag = 0;
#endif
  cr->gettraceinfo(trname,appflag
#ifdef COMMAND
		   ,breakflag
#endif
      );
   if (
       trname !=NULL && (
#ifdef COMMAND
       breakflag ||
#endif
       trace || (traceind <= tracelevel && ! quiet))) {
      indent(); traceout << "    '" << trname << "' :\t";
      if (
#ifdef COMMAND
	  breakflag ||
#endif
	  trace) { mtt.write(traceout);traceout<<"\n"; }
   }
   
}

static void ruleredtrace(contrule *cr,term res)
{
#ifdef COMMAND
  char *trname;
  int appflag;
  int breakflag = 0;
  if (commands)
    cr->gettraceinfo(trname,appflag,breakflag);
#endif
   if (
#ifdef COMMAND
       breakflag ||
#endif
       trace || (traceind <= tracelevel && ! quiet)) {
      if (
#ifdef COMMAND
	  breakflag ||
#endif
	  trace) {traceout << "\n"; indent();  traceout << "    [red] :\t";}
      res.write(traceout);traceout<<"\n";
   }
   
}

static void rulefinishedtrace(contrule *cr)
{
  char *trname;
  int appflag;
#ifdef COMMAND
  int breakflag;
#endif
  cr->gettraceinfo(trname,appflag
#ifdef COMMAND
		   ,breakflag
#endif
);
    if (
	trname !=NULL && (
#ifdef COMMAND
	breakflag ||
#endif
	trace || (traceind+1 <= tracelevel && ! quiet))) {
      indent(); traceout << "    fail of '" << trname << "' \n";
    }
}

static void wheresettrace(int vn, term t,strategy **st)
{
  if (
#ifdef COMMAND
     (commands && st && *st && (*st)->breaked)||// HACK-I DO NOT UNDERSTAND 
#endif
      trace || (traceind-1 <= tracelevel && !quiet)) {
    indent(); traceout << "setting VAR("<<vn<<") on ";
                 t.write(traceout);traceout<<"\n";
  }
}

static void wherepatternsettrace(term pat, term t,strategy **st)
{
  if (
#ifdef COMMAND
     (commands && st && *st && (*st)->breaked)||// HACK-I DO NOT UNDERSTAND 
#endif
     trace || (traceind-1 <= tracelevel && !quiet)) {
    indent(); traceout << "setting "; pat.write(traceout);
	      traceout << " on "; t.write(traceout);traceout<<"\n";
  }
}

static void wherepatternfailtrace(term pat, strategy **st)
{
  if (
#ifdef COMMAND
      (commands && st && *st && (*st)->breaked)||// HACK-I DO NOT UNDERSTAND
#endif
      trace || (traceind-1 <= tracelevel && !quiet)) {
    indent(); traceout << "affectation of "; pat.write(traceout);
    traceout <<" failed\n";
  }
}

static void wherefailtrace(int vn,strategy **st)
{
  if (
#ifdef COMMAND
      (commands && st && *st && (*st)->breaked)||// HACK-I DO NOT UNDERSTAND
#endif
      trace || (traceind-1 <= tracelevel && !quiet)) {
    indent(); traceout << "affectation of VAR("<<vn<<") failed\n";
  }
}

static void whereiftrace(term wt)
{
  if (trace || (traceind-1 <= tracelevel && !quiet)) {
    indent(); traceout << "condition is "; wt.write(traceout); traceout << "\n";
  }
}

static void wherestarttrace(term t,strategy **st)
{
  if (
#ifdef COMMAND
      (commands && st && *st && (*st)->breaked)|| // HACK-I DO NOT UNDERSTAND
#endif
      trace || (traceind <= tracelevel && !quiet)) {
    indent(); traceout << "  applying strategy '" << 
		trrules.strategyname_refs(st) << "' on\t";
    t.write(traceout); traceout << "\n";
  }
}

static void processrestrace(term res,char *pname)
{
  if (trace || (traceind <= tracelevel && !quiet)) {
    indent(); traceout << "    process '" << pname << "' : "; 
    res.write(traceout); traceout << "\n";
  }
}



static void callprtrace(char *p,term t)
{
  if (trace || (traceind+1 <= tracelevel && !quiet)) {
    indent(); traceout << "    calling processus '" << p << "' with argument:\t";
              t.write(traceout);traceout<<"\n";
  }
}


/* -----------------  for stateofexecution  ------------------    */
//----------------------------------------------QQQ
int strstore_empty(struct strlist *strstore)
{
  return (strstore == NULL);
}

void strstore_push(strategy *str, struct strlist **strstore)
{
struct strlist *ss;

  //stout << "PUSH strategie:"; str->dump(); stout << "\n";

  AALLOS(ss, struct strlist);
  ss->str = str;
  ss->next = *strstore;
  *strstore = ss;
}

struct strlist *strstore_copy(struct strlist *strstore)
{
struct strlist *strst;
struct strlist **strst_p = & strst;
  while(strstore) {
    AALLOS(*strst_p, struct strlist);
    (*strst_p)->str = strstore->str;
    strst_p = &((*strst_p)->next);
    strstore = strstore->next;
  }
  *strst_p = NULL;
  return strst;
}

strategy *strstore_pop(struct strlist **strstore)
{
strategy *ss;
struct strlist *p;
 // stout << "#" << strstore << ":" << *strstore << "\n";
 if (strstore == NULL) { //??
   sterr << "[fatal] strstore_nil int.err.\n"; failexit(); }
 if (*strstore == NULL) {
   sterr << "[fatal] strstore_pop int.err.\n"; failexit();
   return NULL; }
  else {
    p = *strstore; ss = p->str; (*strstore) = p->next; CFRE(p);
    // stout << "POP strategie:"; ss->dump(); stout << "\n";
    return ss; }
}

void strstore_free(struct strlist *strstore)
{
struct strlist *p;
  while (strstore) {
    p = strstore; strstore = strstore->next;
    p->str = NULL; CFRE(p); }
}

int strstore_no_more_strategies(struct strlist *strstore)
{
  while (strstore) {
   if (strstore->str) return false;
   strstore = strstore->next; }
  return true;
}
//--------------------------------------------------
stateofexecution::stateofexecution(term &t,strategy *st)
{
  AALLOS(sl ,struct strstatelist)
  actsl = sl;
  sl->prev = NULL;
  sl->actst = st;
  sl->strstore = NULL;
  init_actsl(t);
}

stateofexecution::~stateofexecution()
{
  statistic.dec_ncp();
}

void stateofexecution::init_actsl(term t)
{
  actsl->u = NULL;
  actsl->next = NULL;
  t.topcopy(actsl->trt);
}

void stateofexecution::free()
{ struct strstatelist *ll,*lll;
  struct statelist *ss,*sss;
  int i;
  if (actsl == NULL) {        // nothing to do, state is over.
     return;
  } else if (sl->actst == NULL) {      // it was state with NULL strategy
    sl->trt.tdelete();
    CFRE(sl);
    strstore_free(sl->strstore); // DDD
    actsl = sl = NULL;
    return;
  }
  ll = sl;
  for(;;) {
    ll->trt.tdelete();
    strstore_free(ll->strstore); // DDD
    if (ll->u != NULL) {
      switch (ll->actst->strnam()) {
      case STRNAMEDONTCARE: case STRNAMEDONTKNOW: case STRNAMEONE: 
        DELETE2(ll->u->choose.cr);
	break;
      case STRNAMEDONTKNOWCON:
        DELETE2(ll->u->choose.cr);
	for(i=0; i < ll->u->processes.nofsubprocesses; i++)
	  if (ll->u->processes.subprocesses && ll->u->processes.subprocesses[i])
	    freeprocess(ll->u->processes.subprocesses[i]);
	break;
      case STRNAMEDONTCARE2: case STRNAMEONE2: case STRNAMEDONTKNOW2: 
      case STRNAMEDONTCARECON2: case STRNAMEDONTKNOWCON2: case STRNAMEONECON2: 
        ll->u->choose2.st->free();
	DELETE1(ll->u->choose2.st);
	if (ll->actst->strnam() == STRNAMEDONTCARECON2 ||
	    ll->actst->strnam() == STRNAMEDONTKNOWCON2 ||
	    ll->actst->strnam() == STRNAMEONECON2) {
	  for(i=0; i < ll->u->processes.nofsubprocesses; i++)
	    if (ll->u->processes.subprocesses && ll->u->processes.subprocesses[i])
	      freeprocess(ll->u->processes.subprocesses[i]);
	}
	break;
      case STRNAMEREPEAT:case STRNAMEITERATE:
        ss = ll->u->rep_iter;
	while (ss!=NULL) {
          ss->s->free(); DELETE1(ss->s);
          sss = ss->next; CFRE(ss); 
          ss=sss;
        }
	break;
      case STRNAMEDCPROCESSCALL: case STRNAMEDKPROCESSCALL:
	freeprocess(ll->u->proc.pd);
	break;
      }
    }
    if (ll == actsl) {CFRE(ll); break;}
    lll = ll; ll = ll->next; CFRE(lll);
  }
//sterr << "[warning] stateofexecution::free end\n";
  actsl = sl = NULL;
}


int stateofexecution::nextsolution(term &res)
{ term rs;
  int det, was_last_strategy, is_null_strategy;
  struct strstatelist *pp;

  if (actsl != NULL && sl->actst == NULL)
    is_null_strategy = strstore_no_more_strategies(actsl->strstore);
  else 
    is_null_strategy = false;

  if (is_null_strategy) {		// NULL strategy
    res = actsl->trt;
    CFRE(sl); 
    strstore_free(sl->strstore); // DDD
    actsl = sl = NULL;
    return(1); }
  while (actsl != NULL) {
    while ((det=incr_actsl(rs))) {
      if (actsl->actst->nex() == NULL)
	was_last_strategy = strstore_no_more_strategies(actsl->strstore);
      else 
	was_last_strategy = false;

      if (was_last_strategy) {  // it was the last strategy
          res= rs;
	                               //	  actsl->statist->fromThisWasSucces();
	  if (det== -1){
              pp=actsl; actsl=actsl->prev;pp->trt.tdelete();
	      CFRE(pp->u);
	      strstore_free(pp->strstore); // DDD
	      CFRE(pp); }
	  return((actsl == NULL) ? -1 : 1);  // are there choice-points
      } else {                           // was not last strategy
	  if (det == -1) {
            CFRE(actsl->u); actsl->trt.tdelete();
	    actsl->actst = actsl->actst->nex();    // skip strategy
	    while (actsl->actst == NULL) {
	      // stout << "%2%\n";
	      actsl->actst = strstore_pop(&(actsl->strstore));
	      actsl->u = NULL; } 
	  } else {
	    AALLOS(actsl->next ,struct strstatelist);  // new choice point
	    actsl->next->prev = actsl;              // back-chain
	    actsl->next->actst = actsl->actst->nex();  // skip strategy
	    actsl->next->strstore =  strstore_copy(actsl->strstore);
                                         //	    actsl->next->statist->forwardFrom(actsl->statist);
	    actsl = actsl->next;         // skip
	    while (actsl->actst == NULL) {
	      // stout << "%3%\n";
	      actsl->actst = strstore_pop(&(actsl->strstore));
	      actsl->u = NULL; } 
	  }
	  init_actsl(rs);
	  rs.tdelete();
       }
    }
                                            //    if (actsl->prev == NULL) topstat->backFrom(actsl->statist);
                                            //    else actsl->prev->statist->backFrom(actsl->statist);
    pp = actsl; actsl = actsl->prev; 
    strstore_free(pp->strstore); // DDD
    CFRE(pp->u); CFRE(pp);
  }
  return(0);
}

void stateofexecution::rep_iter_add_state(term t,int afterdeterm)
{ struct statelist *s;
  if (afterdeterm) {
    s = actsl->u->rep_iter;
    if (s==NULL) {sterr << "[stateofexecution::rep_iter_add_state] int.err.\n";
		  failexit();}
//    s->s->free();                    // state is over =>not neccessary,just trt
    DELETE1(s->s);
  } else {
    NNEW(s ,struct statelist);
    s->next = actsl->u->rep_iter;
    actsl->u->rep_iter = s;
  }
  NNEW(s->s ,stateofexecution (t,actsl->actst->substrateg()));
}

int stateofexecution::repeat_incr(term &res)
{ term tt;
  int nr,det;
  struct statelist *s;
  det =1; nr = 1;
  /* != 0 suggested by warning */
  while ((nr=actsl->u->rep_iter->s->nextsolution(tt)) != 0
//	&& !tt.equal(actsl->u->rep_iter->s->sl->trt) 
        ){
       det= det && (nr == -1);
       res.tdelete();
       res = tt;
       rep_iter_add_state(tt,nr== -1);
  }
  actsl->u->rep_iter->s->free(); DELETE1(actsl->u->rep_iter->s);
  s = actsl->u->rep_iter->next; 
  CFRE(actsl->u->rep_iter);
  actsl->u->rep_iter = s;
  return(det?-1:1);
}

int strbuiltins(term &res)
{
  /*int pid,fstat;*/
  /*lexem le;*/
  term trm; /*, *tt; *pidt, */
  /*char *fname,*fkind;*/
  int hd; /*,det;*/
  /*struct processdata *pd;*/

      hd = res.semantic();
      switch(hd) {
        case LGISPC_META_REDUCE:
	  trm.stinit();
          if (meta_reduce(res.subterm(0)->getstring(),
			  res.subterm(1)->getstring(),
331,
			  res.subterm(2)->getstring(),
			  res.subterm(3)->getstring(),
			  res.subterm(4)->head(),// solution
			  &trm)) {
	    res = trm;
	    return -1; }
	  else
	    return(0);
        case REF_META_REDUCE:
	  trm.stinit();
          if (meta_reduce(res.subterm(0)->getstring(),
			  res.subterm(1)->getstring(),
331,
			  res.subterm(2)->getstring(),
			  NULL,   // spc == NULL
			  res.subterm(3)->head(),// solution
			  &trm)) {
	    res = trm;
	    return -1; }
	  else
	    return(0);
/*
        case META_REDUCE:
          if (meta_reduce(res.subterm(0)->getstring(),
			  res.subterm(1)->getstring(),
331,
			  res.subterm(2)->getstring(), NULL,
			  res.subterm(3)->head(),// solution
			  &res)) {
	    return -1; }
	  else
	    return(0);
      case META_BAGOFREDUCE: {
  	  int cons_fsym = res.subterm(1)->head();
	  int nil_fsym = res.subterm(1)->subterm(1)->head();
	  meta_reduce_bagof(cons_fsym, nil_fsym,
		    res.subterm(0)->getstring(),
		    res.subterm(1)->subterm(0)->getstring(),
331,
		    res.subterm(2)->getstring(), NULL,
		    res.subterm(3)->head(),// from
		    res.subterm(4)->head(),// numb of solution
		    &res);
	  return -1; }
*/
      case LGISPC_META_BAGOFREDUCE: {
  	  int cons_fsym = res.subterm(1)->head();
	  int nil_fsym = res.subterm(1)->subterm(1)->head();
	  meta_reduce_bagof(cons_fsym, nil_fsym,
		    res.subterm(0)->getstring(),
		    res.subterm(1)->subterm(0)->getstring(),
331,
		    res.subterm(2)->getstring(),
		    res.subterm(3)->getstring(),
		    res.subterm(4)->head(),// from
		    res.subterm(5)->head(),// numb of solution
		    &res);
	  return -1; }
      case REF_META_BAGOFREDUCE: {
  	  int cons_fsym = res.subterm(1)->head();
	  int nil_fsym = res.subterm(1)->subterm(1)->head();
	  meta_reduce_bagof(cons_fsym, nil_fsym,
		    res.subterm(0)->getstring(),
		    res.subterm(1)->subterm(0)->getstring(),
331,
		    res.subterm(2)->getstring(),
			    NULL,  // spc == NULL
		    res.subterm(3)->head(),// from
		    res.subterm(4)->head(),// numb of solution
		    &res);
	  return -1; }
      case NEW_META_APPLY: {
	  int cons_fsym = res.subterm(1)->head();
	  int nil_fsym = res.subterm(1)->subterm(1)->head();
	  
	  // stout << " CONS = " << cons_fsym << " NIL = " << nil_fsym;
	  new_meta_apply( 
// type problem with intergers
	     ((fsymtab[res.subterm(1)->subterm(0)->head()].
	       textform())->leftside).typeval(), /* typ */
	     cons_fsym, nil_fsym,
             res.subterm(0), // strategy
	     res.subterm(1)->subterm(0), // term
	     res.subterm(2)->head(), // begin from solution
	     res.subterm(3)->head(), // number of sols
	     &res);
	  return -1; // det result
	  break; }
        case META_APPLY:
	  // res.write(stout);
          if (meta_apply( 
	     ((fsymtab[res.head()].textform())->leftside).typeval(), /* typ */
            res.subterm(0),res.subterm(1),res.subterm(2)->head(),&res)) {
            return -1; }
	  else
	    return(0);
        default:	
	  sterr << "[META-built] META strategy over wrong term ::";
          res.write(sterr); sterr << "\n\n"; sterr.flush();
	  interr();
      }
  return 0;
}

int stateofexecution::incr_actsl(term &res)
{ struct statelist *s;
  int r,nr = 0;

  struct strlist *strlst;
  /*pid_t       pid;*/
  stateofexecution *loc_state;

again:
  /*
stout << " ==== incr_actsl ====  " << this << "\n";
stout <<"term = "; actsl->trt.write(stout); stout << "\n";
stout << "strategy = "; actsl->actst->dump(); stout << "\n";
//dump(0);

*/

  res = actsl->trt;			// only because of pretty tracing
  if (actsl->actst->strnam() == STRCALL) goto strcall;
  if (actsl->u == NULL) {			// just initialize
    NNEW(actsl->u,union simple_str_state);
    switch (actsl->actst->strnam()) {
    case STRNAMEDONTCARE: case STRNAMEDONTKNOW: case STRNAMEONE: case STRNAMEDONTKNOWCON:
		actsl->u->choose.r = NULL;
		actsl->u->choose.ns = actsl->actst->rulenamelist();
		goto incrnextrule;

    case STRNAMEDONTCARECON2: case STRNAMEDONTKNOWCON2: case STRNAMEONECON2:
      // signal(SIGUSR1,zooo);

	for (strlst = actsl->actst->substrlist(); strlst; strlst=strlst->next) nr++;
	//stout << "THERE ARE " << nr << " SUBSTRATEGIES \n";

	actsl->u->processes.nofsubprocesses = nr;
	AALLOSS(actsl->u->processes.subprocesses,nr , struct processdata *);

	actsl->u->processes.gr = topgrammar;                    // temporaire
	actsl->u->processes.stsym =  actsl->actst->typeofstr(); // sort

	for (nr=0,strlst = actsl->actst->substrlist(); strlst; strlst=strlst->next,nr++) {
	  NNEW(loc_state, stateofexecution(actsl->trt,strlst->str));
	  //stout << "Sub-process " << nr << " is running\n";
	   //***strlst->str->dump();actsl->trt.write(stout);

	  actsl->u->processes.subprocesses[nr] = 
	    newsubprocess(loc_state,9999, 1);
	    }	
	goto incsubprocesses;
    case STRNAMEDONTCARE2: case STRNAMEDONTKNOW2: case STRNAMEONE2:
		actsl->u->choose2.s = actsl->actst->substrlist();
		goto incrnextstr;
    case STRNAMEDCPROCESSCALL: case STRNAMEDKPROCESSCALL:
      
/*
stout << "PROCESS WILL BE CREATED\n";
stout << actsl->actst->subprocname() << "\n";
stout << actsl->actst->subprocmaxn() << "\n";
stout << actsl->actst->subprocgr() << "\n";
//actsl->actst->subprocgr()->dump();

stout << "-------- \n";
*/

		actsl->u->proc.pd = newprocess(actsl->actst->subprocname(),
					       NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
					       actsl->actst->subprocmaxn(),0);
		actsl->u->proc.gr = actsl->actst->subprocgr();
		actsl->u->proc.stsym = actsl->actst->subproctype();
		fsymtab_remakealias(actsl->actst->subprocgr());
		callprtrace(actsl->actst->subprocname(),actsl->trt);
	        actsl->trt.write(*actsl->u->proc.pd->pin);
		fsymtab_remakealias(topgrammar);	// because of tracing
		*actsl->u->proc.pd->pin << "\n";
                actsl->u->proc.pd->pin->flush();
// the following lines were added to hack the communication with UNIF
//		if (actsl->u->proc.pd->counter == 0 && 
//		    actsl->actst->strnam() == STRNAMEDCPROCESSCALL) {
//		  DELETE2(actsl->u->proc.pd->pin);
//		  actsl->u->proc.pd->pin = NULL;
//		}
		goto incsubprocc;
    case STRNAMEREPEAT:
		actsl->u->rep_iter = NULL;
		actsl->trt.topcopy(res);
		rep_iter_add_state(res,0);
		nr = repeat_incr(res);
		return(nr);
    case STRNAMEITERATE:
		actsl->u->rep_iter = NULL;
		actsl->trt.topcopy(res);
		rep_iter_add_state(res,0);
		return(1);
    case STRINLINE:
		sterr <<"[error] strategy \"inline\" can't be interpreted\n";
		sterr <<"\tyou can't use it in the interpreted parts of code\n";
		interr();
    case STRMETA:
      if (res.semantic() == META_APPL) {
        strategy *s;
        /*int n;*/
        term2strategy(((fsymtab[res.head()].textform())->leftside).typeval(), /* typ */
		      res.subterm(0),&s);
        // s->dump();
        actsl->actst = s;
        actsl->trt = *(res.subterm(1));
        actsl->u = NULL  ;//REDOOOOOOOOOOOOOOO
        goto again; }
      else
        goto strbuilt;
    case STRIDENTITY:
                goto stridentity;
    case STRFAIL:
                goto strfail;
    case STRCALL:
                goto strcall;
    case STRNORM_IN:                         // [Huy: May  4 00] 
    case STRNORM_OUT:                         // [Huy: May  4 00] 
                goto strnorm;
    case STRNAMETALL:                         // [pem: Oct 26 00]
    case STRNAMETONE: 
    case STRNAMETSOME: 
      goto strtall;
    case STRNAMEREWRITE: 
      goto strrewrite;
    default:
		sterr << "[error] don't know interpret this strategy\n"; 
		interr();
   }
  }
						// increment old state
  switch (actsl->actst->strnam()) {
  case STRNAMEONE:
     sterr<<"[stateofexecution.c] next sol. of dont care, int.warn.\n";
     return(0);
  case STRNAMEDONTCARE:
  case STRNAMEDONTKNOW:
  case STRNAMEDONTKNOWCON:
	for(;;) {
	  if (finitrace || trace) {
	    tryapptrace(actsl->u->choose.cr,actsl->trt);
	  }
	  if (actsl->u->choose.cr->nextapp(res,!quiet)) {
		rulesucctrace(actsl->u->choose.cr,res);
                reduce(res,trace);
		ruleredtrace(actsl->u->choose.cr,res);
		if (actsl->actst->strnam() == STRNAMEONE) {
                  // actsl->trt.tdelete();
	          DELETE2(actsl->u->choose.cr);
                  return(-1);
		}
		else if (actsl->actst->strnam() == STRNAMEDONTCARE) {
                  // actsl->trt.tdelete();
	          actsl->u->choose.ns = NULL;
	          actsl->u->choose.r = NULL;
                  return(1);
                } else return(1);
	  }
	  if (actsl->u->choose.r == NULL || trace) {
             rulefinishedtrace(actsl->u->choose.cr);
	  }
	  DELETE2(actsl->u->choose.cr);
incrnextrule:
	  finitrace = (actsl->u->choose.r == NULL);
	  while (actsl->u->choose.r == NULL) {
	    if (actsl->u->choose.ns == NULL) {
/**/		actsl->trt.tdelete();
		return(0);
	    }
	    actsl->u->choose.r =trrules.getrules(actsl->u->choose.ns->strname,
				((actsl->actst)?actsl->actst->getmodule():-1)); // ????
	    if (actsl->u->choose.r == NULL) {
	      sterr << "\n[warning] non existing rule '" <<
		trrules.rulename(actsl->u->choose.ns->strname) <<
		"' used\n\n";
	    }
	    actsl->u->choose.ns = actsl->u->choose.ns->next;
	  }
	 NNEW(actsl->u->choose.cr,contrule(actsl->u->choose.r->rule,actsl->trt));
	 statistic.inc_manyp();
	 actsl->u->choose.r = actsl->u->choose.r->next;
	}
  case STRNAMEDONTCARECON2:
  case STRNAMEDONTKNOWCON2:
  case STRNAMEONECON2:
    {
      int pool_stat[20];
      int nr_min, pool_min;

      if (actsl->u->processes.nofsubprocesses >= 20) {
	sterr << "\n[fatal] -- too many subprocesses in dkcon\n";
	failexit(); }

      // AALLOSS(pool_stat,actsl->u->processes.nofsubprocesses, int);
      for (r=0; r<actsl->u->processes.nofsubprocesses; r++) pool_stat[r]=0;

	while(1) { 
	incsubprocesses:
        // find non-finished processus
	pool_min = 99999; nr_min = -1;
	for (nr=0; nr<actsl->u->processes.nofsubprocesses; nr++)
	  if (actsl->u->processes.subprocesses[nr])  // not finished
            if (pool_stat[nr] < pool_min) {
              pool_min = pool_stat[nr]; nr_min = nr; }
	if (nr_min == -1) return(0); // unfound... all are finished
	nr = nr_min;
	#ifdef CONCUR_BLABLA
	stout << "READING from " << nr << " PROCES of type " << actsl->u->processes.stsym  << "\n";	
	#endif
	r=nextsolsubprocess(actsl->u->processes.subprocesses[nr],
			   actsl->u->processes.gr,
			   actsl->u->processes.stsym,
			   res);

	if (r == 1) {
          #ifdef CONCUR_BLABLA
	  stout << "RESULT from " << nr << " PROCES IS "; res.write(stout); stout << "\n";
	  #endif
	  if (actsl->actst->strnam() == STRNAMEONECON2) {

    for(r=0; r<actsl->u->processes.nofsubprocesses; r++) 
	      if (actsl->u->processes.subprocesses[r]) {
	      actsl->u->processes.subprocesses[r]->counter = -1;
	      freeprocess(actsl->u->processes.subprocesses[r]);
	      actsl->u->processes.subprocesses[r] = NULL; }
	    return -1;
	  }
	  else if (actsl->actst->strnam() == STRNAMEDONTCARECON2) {
	    for(r=0; r<actsl->u->processes.nofsubprocesses; r++) 
	      if (r != nr && actsl->u->processes.subprocesses[r]) {
		actsl->u->processes.subprocesses[r]->counter = -1;
		freeprocess(actsl->u->processes.subprocesses[r]);
		actsl->u->processes.subprocesses[r] = NULL; }
	    return 1;
	  } else
	    return(1); }
	else if (r == 0) {
	  pool_stat[nr]++;
	  sleep(1); 
	}
	else { // -1
	  #ifdef CONCUR_BLABLA
	  stout << "NO_MORE SOLUTION from " << nr << " PROCES\n";
	  #endif
	actsl->u->processes.subprocesses[nr]->counter = -1;
	freeprocess(actsl->u->processes.subprocesses[nr]);
	actsl->u->processes.subprocesses[nr] = NULL; }
	}

      return(0);
    }
  case STRNAMEDONTKNOW2:  case STRNAMEDONTCARE2: case STRNAMEONE2:
	for(;;) { int rs;
	/* != 0 suggested by warning */
	  if ((rs = actsl->u->choose2.st->nextsolution(res)) != 0) {
	    if (actsl->actst->strnam() == STRNAMEDONTCARE2) {
	      actsl->u->choose2.s = NULL; return(rs); }
	    if (actsl->actst->strnam() == STRNAMEONE2) {
	      actsl->u->choose2.s = NULL;
	      actsl->u->choose2.st -> free(); DELETE1(actsl->u->choose2.st);
	      return -1; }
	    return(1);
	  }
	  actsl->u->choose2.st -> free(); DELETE1(actsl->u->choose2.st);
  incrnextstr:
	  if (actsl->u->choose2.s == NULL) {
/**/		actsl->trt.tdelete();
		return(0);
	  }
	  NNEW(actsl->u->choose2.st ,
                 stateofexecution(actsl->trt,actsl->u->choose2.s->str));
	  statistic.inc_ncp(); 
	  actsl->u->choose2.s = actsl->u->choose2.s->next;
	}
  case STRNAMEDCPROCESSCALL: case STRNAMEDKPROCESSCALL:
  incsubprocc:
	while (nextsolprocess(actsl->u->proc.pd, actsl->u->proc.gr,
               actsl->u->proc.stsym,res)) {
	  processrestrace(res,actsl->actst->subprocname());
	  if (actsl->actst->strnam() == STRNAMEDCPROCESSCALL) {
	     freeprocess(actsl->u->proc.pd);
             return(-1);
          }
	  return(1);
	}
	freeprocess(actsl->u->proc.pd);
/**/	actsl->trt.tdelete();
	return(0);
  case STRMETA:
       strbuilt:
       return strbuiltins(res);
  case STRFAIL:
      strfail:
      //sterr << "fail is not implemented yet\n"; failexit();
      return 0;
  case STRNORM_IN:                               // [Huy: May  4 00] 
  case STRNORM_OUT:
       strnorm:
         sterr << "Normalise for labelled rules is not intepreted\n"; 
         return 0;
  case STRNAMETALL:
       strtall:
         sterr << "Traversal is not intepreted\n"; 
         return 0;
  case STRNAMEREWRITE:
       strrewrite:
         sterr << "Rewrite step is not intepreted\n"; 
         return 0;
  case STRIDENTITY:
       stridentity:
       res.incrcount();
       return -1;
  case STRCALL:
       strcall:
       {
       int cstri;
       strategy *cstr;
       cstri = actsl->actst->subprocmaxn();

       //stout << trrules.strategyname_refs(cstri);

       //stout << "stri = " << cstri << "\n";
       cstr = trrules.getstrategy_refs(cstri);

       //stout << "cstr = " << cstr << "\n";

       if (cstr == NULL) {
         sterr << "[stateofexecution] unknown strategy in call used\n";
         failexit(); }
       // stout << "str = "; cstr->dump(); stout << "\n";
       strstore_push(actsl->actst->nex(),&(actsl->strstore)); // push the rest
       actsl->actst = cstr;
       goto again;
       }
  case STRNAMEREPEAT: case STRNAMEITERATE:
	while (actsl->u->rep_iter != NULL) {
          /* != 0 suggested by warning */
	  while ((nr=actsl->u->rep_iter->s->nextsolution(res))!=0) {
//	    if (!res.equal(actsl->u->rep_iter->s->sl->trt) ){
	      rep_iter_add_state(res, nr== -1);
	      if (actsl->actst->strnam() == STRNAMEREPEAT) repeat_incr(res);
	      return(1);
//	    }
	  }
	  actsl->u->rep_iter->s->free(); DELETE1(actsl->u->rep_iter->s);
	  s = actsl->u->rep_iter->next; 
	  CFRE(actsl->u->rep_iter);
	  actsl->u->rep_iter = s;
	}
/**/        actsl->trt.tdelete();
	return(0);
  default: sterr << "[stateofexecution] unknown strategy " 
	         << actsl->actst->strnam() << " used, int.err.\n";
	failexit();
  }  
  return 0; /* to avoid warning */
}

void stateofexecution::dump(int n)
{ struct strstatelist *s;
  struct statelist *st;
  struct transrulelist *rr;
  strlist *stl;
  odsek(sterr,n); sterr << "\n[stateofexecution::dump] begin  "
                  << "actsl = " << actsl << "\n";
  s=sl;
  if (actsl == NULL) {
    odsek(sterr,n); sterr << "this state is over.\n";
  } else {
  for(;;) {
    odsek(sterr,n); sterr << "this pack = " << s << " prev:" << s->prev << "trt :"; 
	s->trt.write(sterr);
        sterr << "\tactst :\n";
    if (s->actst == NULL) {odsek(sterr,n); sterr << "no strategy\n"; }
    else s->actst->simpledump(n);
    if (s->u == NULL) {
       odsek(sterr,n); sterr << "just initialized\n";
    } else {
      switch (s->actst->strnam()) {
      case STRNAMEREPEAT: case STRNAMEITERATE:
	odsek(sterr,n); sterr << "repeat/iterate substrategies dump\n";
	st = s->u->rep_iter;
	while (st!=NULL) {
	  st->s->dump(n+2);
	  st=st->next;
	}
	odsek(sterr,n); sterr << "repeat/iterate substrategies end\n";
	break;
      case STRNAMEDONTCARE: case STRNAMEDONTKNOW: case STRNAMEONE: case STRNAMEDONTKNOWCON: 
	odsek(sterr,n); sterr << "the rest of rules in dont care/know choose\n";
	rr = s->u->choose.r;
	while (rr!=NULL) {
	  rr->rule->dump(n+2);
	  rr = rr->next;
	}
	odsek(sterr,n); sterr << "and others ...\n";
	break;
      case STRNAMEDCPROCESSCALL: case STRNAMEDKPROCESSCALL:
        odsek(sterr,n); sterr << "calling of subprocess strategy\n";
	break;
      case STRNAMEDONTCARE2: case STRNAMEDONTKNOW2: case STRNAMEONE2: 
      case STRNAMEDONTCARECON2: case STRNAMEDONTKNOWCON2: case STRNAMEONECON2:
	odsek(sterr,n); sterr << "the rest of strategies in CHOOSE2\n";
	stl = s->u->choose2.s;
	while (stl!=NULL) {
	  stl->str->dump2(n+2);
	  stl=stl->next;
	}
	odsek(sterr,n); sterr << "end of strategy list\n";
      }
    }
    if (s==actsl) break;
    s=s->next;
  } 
  }
  odsek(sterr,n); sterr << "[stateofexecution::dump] end\n\n";
}

/* -----------------  for contrule  ------------------    */

void contrule::dump()
{ struct wheress *w;
  DUMPOD();sterr << "[contrule]dump"
                << " appflag = " << appflag << " rule:" << trule << " ";
  trule->dump(2*dumpods);
  sterr << "\n"; DUMPOD(); sterr << "where exec dump:\n";
  w=lastws;
  while (w!=NULL) {
    wherelidump(sterr,w->actwh,2*dumpods);
    w=w->prev;
  }
}


contrule::contrule(transrule* rule,term mt)
{
//sterr << "contrule() this == " << this << "\n";
  term l,r;
  term rlabel;
  int trname,i;
  int varn,whichmatch;
  struct wherelist *wheres;
  term nullterm;
  struct tseq *rhs;
  nbacktr = 0;
  appflag = APPFBEFFIRST;
  lastws = NULL;
  trule = rule;

  trule->getr(varn,l,r,trname,wheres,
	      rhs,
	      whichmatch,rlabel);
  // stout << "RULENAME " << "\n"; trule->dump(5);
  matchstate_alloc++;
  NNEW(match ,match_state(mt,l,whichmatch,varn));
  AALLOSS(substarray ,varn,term);
  for(i=0; i< varn; i++) substarray[i] = nullterm;

  statistic.transruletry(trname,l);
}


contrule::~contrule()
{
//sterr << "~contrule() this == " << this << "\n";
  if (match != NULL) {
      matchstate_dealloc++;
      DELETE2(match); }
  match = NULL;
  freeWhereBacktrack(trule->getvarnum(),lastws,substarray); 
//if (substarray != NULL && substarray[0].isvalidterm()) {
//stout << "DELETE SUBSTARRAY " << substarray[0].getcount();
//if (substarray[0].getcount()) {
//    substarray[0].write(stout);
//}
//stout << " \n";
//}
  if (substarray != NULL) CFRE(substarray);
  statistic.dec_manyp();
}

/*
void contrule::deletematch()
{
  if (match != NULL) match->freee();
  match = NULL;
  freeWhereBacktrack(trule->getvarnum(),lastws,substarray);
}
*/

void freeWhereBacktrack(int varn, struct wheress * &lastws,term *substarray)
{ struct wheress *ws;
  int j;
  term nullterm;
  while (lastws!=NULL) {
    if (lastws->TRYCHOICE) {
//    if (lastws->actwh->leftvarn == TRYCHOICEEND) {
	//stout << " free of TRYCHOICEEND\n";
    } else {
	lastws->actexstate->free(); 

//MEMDDEBUG    stout << "d"; 
	stateofexecution_dealloc++;
	DELETE1(lastws->actexstate);
	if (lastws->actwh->leftvarn == WHEREPATTERN) {
	    ws = lastws;
	    for(j = 0; j < varn; j++)
		if (ws->trail[j]) { 
		ws->trail[j] = 0;
		substarray[j].decrcount(); 
                substarray[j].tdelete(); substarray[j] = nullterm; }
	    CFRE(ws->trail); }
	else  if (lastws->actwh->leftvarn != IFVARN) { // ???
		substarray[lastws->actwh->leftvarn].decrcount();
		  //      if (substarray[lastws->actwh->leftvarn].getcount()) {
//	  substarray[lastws->actwh->leftvarn].write(stout); 
//	  stout << "\n" << substarray[lastws->actwh->leftvarn].getcount() 
//		<< "\n\n";
//     }
	    substarray[lastws->actwh->leftvarn].tdelete();
	    }
    }
    ws = lastws; lastws = lastws->prev;
    wheress_dealloc++;
    CFRE(ws);
  }
}

void contrule::gettraceinfo(char *&name,int &appfl
#ifdef COMMAND
			    , int &breakflag
#endif
)
{ term l,r;
  term rlabel;
  struct tseq *rhs;
  int varn,whichmatch,nameind;
  struct wherelist *wheres;
  trule->getr(varn,l,r,nameind,wheres,
	      rhs,
	      whichmatch,rlabel);
  name = trrules.rulename(nameind);
  appfl = appflag;
#ifdef COMMAND
  breakflag = trule->breaked;
#endif
}

  /**********
  struct tseq {
  int is_case;
  struct wherelist *seq;
    union {
      struct sone_branch {  // is_case = 0
			    term *result; } one_branch;
      struct smore_branches {  // is_case = 1
			       struct branch *brlist; } more_branches;
    };
  };
  *****/
//---------------------------------------------------
void freeBacktrack(struct wheress * &lastws)
{
  struct wheress *ws;
  while (lastws) {
    lastws->actexstate->free(); 
    stateofexecution_dealloc++;
    DELETE1(lastws->actexstate);
    matchstate_dealloc++;
    DELETE1(lastws->mstate);

    ws = lastws; lastws = lastws->prev;
    CFRE(ws);
  }
}


void freeWhereBacktrack1(int varn, struct wheress * &lastws,term *substarray)
{ struct wheress *ws;
  int j;
  term nullterm;
  while (lastws!=NULL) {
    lastws->actexstate->free(); DELETE1(lastws->actexstate);
    if (lastws->actwh->leftvarn == WHEREPATTERN) {
      ws = lastws;
      for(j = 0; j < varn; j++)
	if (ws->trail[j]) { ws->trail[j] = 0;
	  substarray[j].decrcount(); substarray[j].tdelete(); 
	  substarray[j] = nullterm; }
        CFRE(ws->trail);
    }
    else 
    if (lastws->actwh->leftvarn != IFVARN) { // ???

stout << "DELETE VAR " << lastws->actwh->leftvarn << "\n";
substarray[lastws->actwh->leftvarn].write(stout);

       substarray[lastws->actwh->leftvarn].decrcount();
       substarray[lastws->actwh->leftvarn].tdelete(); }

    ws = lastws; lastws = lastws->prev;
    CFRE(ws);
  }
}


void printvars(char *s,int varn, term *substarray)
{
//#ifdef GGG
 int j;      
 stout << s << "\n";
 for(j = 0; j < varn; j++) {
   stout << "Var("<<j<<")="; 
   if (substarray[j].isvalidterm()) {
       stout << "\n\n" << substarray[j].getcount();
       substarray[j].write(stout); }
   else stout << "NULL"; 
   stout << ", "; }
   stout << "\n"; 
//#endif
}

int isTseqBacktrackNextSol(term &res, struct tseq *rhs, term *substarray,
                   struct wheress * &lastws, int notbatch, int nback,
	           struct wherelist *&lastwheres, struct term *&lastresult,
	           int varn)
{
struct branch *br = NULL;
int    cres = 0;
term   tst;
   // *** stout << "isTseqBacktrackNextSol " << nback << "\n";
  if (nback) { // redo
    // *** stout << "redo ... \n";
    // *** wherelisdump(stout,lastwheres,5);
    if (isWhereBacktrackNextSol(lastwheres,substarray,lastws,notbatch,varn)) {
      lastresult->copyinstall(false,res,substarray);

res.incrcount(); // ???

      return 1; }
    else
      return 0;
  }
  if (rhs->is_case) {
    // *** stout << "more branches ... \n";
    // *** wherelisdump(stout,rhs->seq,5);
    if (rhs->seq == NULL ||
	isWhereBacktrackNextSol(rhs->seq,substarray,lastws,notbatch,varn)) {
      // *** stout << "search true case ... \n";
      for(cres = 0,br = rhs->u.more_branches.brlist; br; br=br->next) {
	br->test.copyinstall(false,tst,substarray);
        reduce(tst,trace);
	cres = istrueterm(tst);
	if (cres) break;
      }      
      if (cres) { // found
        // *** stout << "found ...\n";
#ifdef GCMEM
	freeBacktrack(lastws); 
	lastws = NULL;
#else
        freeBacktrack(lastws);       //???? HACK ... lastws = NULL;
#endif
        // *** stout << "install ...\n";
	return 
          isTseqBacktrackNextSol(res,br->tseq,substarray,lastws,
				 notbatch,nback,lastwheres,lastresult,varn); }
      else
	return 0; }
    else
      return 0; } 
  else {
    // *** stout << "one branch ... \n";
    lastwheres = rhs->seq;
    lastresult = rhs->u.one_branch.result;
    // *** wherelisdump(stout,lastwheres,5);
    if (rhs->seq == NULL ||
	isWhereBacktrackNextSol(rhs->seq,substarray,lastws,notbatch,varn)) {
      rhs->u.one_branch.result->copyinstall(false,res,substarray);

res.incrcount(); // ???

      return 1; }
    else
      return 0;
  }
}

#ifdef VRSION1901
int isTseqBacktrackNextSol(term &res, struct tseq *rhs, term *substarray,
                   struct wheress * &lastws, int notbatch, int nback,
	           struct wherelist *&lastwheres, struct term *&lastresult,
	           int varn)
{
struct branch *br = NULL;
int    cres = 0;
term   tst;
   // *** stout << "isTseqBacktrackNextSol " << nback << "\n";
  if (nback) { // redo
    // *** stout << "redo ... \n";
    // *** wherelisdump(stout,lastwheres,5);
    if (isWhereBacktrackNextSol(lastwheres,substarray,lastws,notbatch,varn)) {
      lastresult->copyinstall(false,res,substarray);

//1901
 res.incrcount(); // ???

      return 1; }
    else
      return 0;
  }
  if (rhs->is_case) {
    // *** stout << "more branches ... \n";
    // *** wherelisdump(stout,rhs->seq,5);
    if (rhs->seq == NULL ||
	isWhereBacktrackNextSol(rhs->seq,substarray,lastws,notbatch,varn)) {
      // *** stout << "search true case ... \n";
      for(cres = 0,br = rhs->more_branches.brlist; br; br=br->next) {
	br->test.copyinstall(false,tst,substarray);
        reduce(tst,trace);
	cres = istrueterm(tst);
//1901	tst.tdelete(); //1901
	if (cres) break;
      }      
      if (cres) { // found
        // *** stout << "found ...\n";


//1901        
      freeBacktrack(lastws);       //???? HACK ... lastws = NULL;
//stout << "FREE IN SWITCH \n";
//freeWhereBacktrack1(varn,lastws,substarray);
//stout << "DONE \n";

        // *** stout << "install ...\n";
	{
	    //struct wheress *loc_lastws = NULL;
        cres =  isTseqBacktrackNextSol(res,br->tseq,substarray,
	    //			       loc_lastws,
	    			       lastws,
				 notbatch,nback,lastwheres,lastresult,varn); 
//stout << "FREE IN SWITCH \n";
//        freeWhereBacktrack1(varn,lastws,substarray);
//	lastws = loc_lastws;
//	freeWhereBacktrack1(varn,loc_lastws,substarray);
//stout << "DONE \n";
 
        return cres;
	}
      }
      else
	return 0; }
    else
      return 0; } 
  else {
    // *** stout << "one branch ... \n";
    lastwheres = rhs->seq;
    lastresult = rhs->one_branch.result;
    // *** wherelisdump(stout,lastwheres,5);
    if (rhs->seq == NULL ||
	isWhereBacktrackNextSol(rhs->seq,substarray,lastws,notbatch,varn)) {
      rhs->one_branch.result->copyinstall(false,res,substarray);

//1901 
res.incrcount(); // 

      return 1; }
    else
      return 0;
  }
}
#endif


#ifdef RSWITCH
int isNewTseqBacktrackNextSol(term &res, struct tseq *rhs, term *substarray,
             struct wheress * &lastws, int notbatch, int varn)
{
  if (rhs->is_case) {
    if (rhs->seq == NULL ||
	isWhereBacktrackNextSol(rhs->seq,substarray,lastws,notbatch,varn)) {
	struct wherelist *p;
	NNEW(p, struct wherelist);
	p->next = NULL; p->leftvarn = SWITCHCASEEND;
	p->branch_list = rhs->more_branches.brlist;
	return isWhereBacktrackNextSol(p,substarray,lastws,notbatch,varn);
    } else
	return 0;
  }
  else {
      if (rhs->seq == NULL ||
	  isWhereBacktrackNextSol(rhs->seq,substarray,lastws,notbatch,varn)) {
	  rhs->one_branch.result->copyinstall(false,res,substarray);
      reduce(res,trace);
      return 1;
      }
      else
	  return 0;
  }
}

int isTseqBacktrackNextSol(term &res, struct tseq *rhs, term *substarray,
                   struct wheress * &lastws, int notbatch, int nback,
	           struct wherelist *&lastwheres, struct term *&lastresult,
	           int varn)
{
struct branch *br = NULL;
int    cres = 0;
term   tst;
int RET;

// printvars("Tseq entry",varn,substarray);


   // *** stout << "isTseqBacktrackNextSol " << nback << "\n";
  if (nback) { // redo
    // *** stout << "redo ... \n";
    // *** wherelisdump(stout,lastwheres,5);
    if (isWhereBacktrackNextSol(lastwheres,substarray,lastws,notbatch,varn)) {
      lastresult->copyinstall(false,res,substarray);

      reduce(res,trace);

//res.incrcount(); // ???

//stout << "##################\n";
//res.write(stout); stout << "\n";
//stout << res.getcount() << "\n\n";

      RET=1; goto ret; }
    else {
	RET=0; goto ret; }
  }
  if (rhs->is_case) {

    // *** stout << "more branches ... \n";
    // *** wherelisdump(stout,rhs->seq,5);
    if (rhs->seq == NULL ||
	isWhereBacktrackNextSol(rhs->seq,substarray,lastws,notbatch,varn)) {
      // *** stout << "search true case ... \n";

// printvars("before switching",varn,substarray);

      for(cres = 0,br = rhs->more_branches.brlist; br; br=br->next) {
	br->test.copyinstall(false,tst,substarray);

// printvars("before cond",varn,substarray);

        reduce(tst,trace);
	cres = istrueterm(tst);
	tst.tdelete();

// printvars("after cond",varn,substarray);

	if (cres) break;
      }      
      if (cres) { // found

struct wheress *loc_lastws = lastws; lastws = NULL; //2301

      cres =  isTseqBacktrackNextSol(res,br->tseq,substarray,lastws,
				 notbatch,nback,lastwheres,lastresult,varn); 

      freeWhereBacktrack(varn,loc_lastws,substarray); //2301
      freeWhereBacktrack(varn,lastws,substarray); //2301

//      printvars("before RETurning",varn,substarray);

      RET = cres; goto ret;
      }
      else {
	  RET = 0; goto ret; } }
    else {
     RET = 0; goto ret;} }
  else {
    // *** stout << "one branch ... \n";
    lastwheres = rhs->seq;
    lastresult = rhs->one_branch.result;
    // *** wherelisdump(stout,lastwheres,5);
    if (rhs->seq == NULL ||
	isWhereBacktrackNextSol(rhs->seq,substarray,lastws,notbatch,varn)) {
      rhs->one_branch.result->copyinstall(false,res,substarray);
      
      reduce(res,trace);

//res.incrcount(); // ???
//res.write(stout); stout << "\n";
//stout << res.getcount() << "\n\n";

      freeWhereBacktrack(varn,lastws,substarray); //2301

//      printvars("before returning",varn,substarray);


      RET = 1; goto ret; }
    else {
	RET = 0; goto ret; }
  }

ret:
freeWhereBacktrack(varn,lastws,substarray); //2301
  return RET;
}
#endif


int contrule::nextapp(term &mtt,int notbatch)
{ 
  term l,r;
  term rlabel;
  int varn,whichmatch; /*,j;*/
  struct tseq *rhs;
  /* term ttt,wt,nullterm;*/
  int trnameind;
  struct wherelist *wheres;
  trule->getr(varn,l,r,trnameind,wheres,
	      rhs,
	      whichmatch,rlabel);
  traceind+=3;
  if (appflag == APPFBEFFIRST) {
     appflag = APPFINMATCH;
  }  else {
     goto nextwhereass;
  }

//nextmatch:
     while (match->isnextsol(substarray)) {
     whflag = 1;
nextwhereass:
       while (( (wheres == NULL && rhs == NULL) && whflag)
       || (!rhs && isWhereBacktrackNextSol(wheres,substarray,lastws,notbatch,varn))
       || (rhs && 
	   isTseqBacktrackNextSol(mtt,rhs,substarray,lastws,notbatch,
				  nbacktr,lastwheres,lastresult,varn))){
         whflag=0;
	 nbacktr++;
	 if (!rhs) {
	     r.copyinstall(false,mtt,substarray); }
	 // else mtt comes from isTseqBacktrackNextSol
	 statistic.transruleapp(trnameind,l);
	 traceind-=3;
	 return(1);
       }
     }
    traceind-=3;
    return(0);
}


struct wherelist *appendwherelists(struct wherelist *a,
				   struct wherelist *b, int *len)
{
  if (a) {
      struct wherelist *p;
      NNEW(p, struct wherelist);
      *p = *a;
      p->next = appendwherelists(a->next,b,len);
      (*len) += 1;
      return p; }
  else {
      *len = 0; return b; }  
}

int LEN(struct wherelist *a)
    {
	int n = 0;
	while (a) { a=a->next; n++; }
	return n;
    }

struct wherelist *delete_n_wheres(int len, struct wherelist *a)
{ struct wherelist *p;
    while (len) {
	len --;
	p = a;
	if (a == NULL) sterr << "LIST NOT LONG ENOUGH !!!\n";
	a = a->next;
	DELETE1(p); }
  return a;
}

int isWhereBacktrackNextSol(struct wherelist *wheres,term *substarray,
                   struct wheress * &lastws, int notbatch, int varn)
{  struct wheress *ws;
   term nullterm,wt, *sarray;  
   term wp, res;
   int j;
   struct wherelist *actwh;
   int cres;
  // printvars("isWhereBacktrackNextSol",varn,substarray);
   if (wheres == NULL) return(0);
   if (lastws  != NULL) goto backward0;    // next solution required
   actwh = wheres;
   goto forward0;                         // initialization & first solution
forward:
  if (actwh->next == NULL) return(1);
  actwh=actwh->next;
forward0:

  wheress_alloc++;

//  if (wheress_alloc %100 == 0) 
//      stout << "\n" 
//	    << wheress_alloc << ":" << wheress_dealloc << "\n"
//	    << matchstate_alloc << ":" << matchstate_dealloc << "\n"
//	    << stateofexecution_alloc << ":" << stateofexecution_dealloc << "\n"
//	  << " = " << 
//	  (wheress_alloc-wheress_dealloc)*sizeof(struct wheress)
//	  << "\n";


  NNEW(ws ,struct wheress);
  ws->actwh = actwh; ws->prev = lastws; lastws = ws;
  ws->TRYCHOICE = 0;
  if (actwh->leftvarn == TRYCHOICEEND) {
    int len;
    /*struct wheress *p;*/
    //stout << "CREATE TRYCHOICEEND\n";
    if (actwh->wherebranch_list == NULL) goto forward0;

    ws->TRYCHOICE = 1;
    ws->wherebranches = actwh->wherebranch_list;

    //stout << "ACTWHREST LEN=" << LEN(actwh->next) << "\n";

    actwh = appendwherelists(actwh->wherebranch_list->wherebranch,
			     actwh->next,&len);

    //stout << "ACTWH TOTAL LEN=" << LEN(actwh) << "\n";

    ws->actwh = actwh;
    ws->len = len;

    //stout << "BRANCH LEN=" << len << "\n";
    // stout << "VARN = " << lastws->actwh->leftvarn << "\n";
    goto forward0;
  }

#ifdef RSWITCH
  if (actwh->leftvarn == SWITCHCASEEND) {
      struct wheress *p;
      NNEW(p, struct wheress);
      p->prev = lastws; p->branch_list = actwh->branch_list;
      lastws = p;
      goto backward;
  }
#endif
  ws->mstate=NULL;
  // stout << "##9\n"; //HORA
  // actwh->whereterm.write(stout); stout << "\n"; // HORA

  actwh->whereterm.copyinstall(false,wt,substarray);
  reduce(wt,trace);
  if (actwh->strateg==NULL) {		// standard strat
     stateofexecution_alloc++;
    NNEW(ws->actexstate ,stateofexecution(wt,NULL));
    // statistic.inc_ncp();
  } else {
    if (*(actwh->strateg)==NULL) {
       sterr << "\n[error] strategy '"<<trrules.strategyname_refs(actwh->strateg)
	     << "' was not defined\n\n";
       failexit();
    }
    if (notbatch) wherestarttrace(wt,actwh->strateg);
    stateofexecution_alloc++;
    NNEW(ws->actexstate ,stateofexecution(wt,*(actwh->strateg)));
    statistic.inc_ncp(); 
  }
  switch (actwh->leftvarn) {
    case  IFVARN:
      if (notbatch) whereiftrace(wt);
      cres = istrueterm(wt); 
      wt.tdelete();
      if (cres) goto forward; 
      else goto backward;
    case WHEREPATTERN:
      // same as for where
      wt.tdelete();
  backwardpat:
      if (!(ws->actexstate->nextsolution(res))) goto backward;
      res.incrcount(); // ???
      actwh->pattern.copyinstall(true,wp,substarray);
      matchstate_alloc++;
      // wp.write(stout); stout << "CONTAINS AC " << wp.contains_AC() << "\n";
      NNEW(ws->mstate,
	   match_state(res,wp,(
//			       fsyminfo(wp.head()) == FSASSOCCOM
			       wp.contains_AC()
			       )?ACMATCH:NORMMATCH,varn));
      res.tdelete();
  testpat:
      AALLOSS(ws->trail ,varn, char); AALLOSS(sarray ,varn, term); 
      for(j = 0; j < varn; j++) { sarray[j] = nullterm; ws->trail[j]=0; }
      if (ws->mstate && ws->mstate->isnextsol(sarray)) {
	for(j = 0; j < varn; j++)
	  if (sarray[j].isvalidterm()) { 
	    substarray[j] = sarray[j]; 
            substarray[j].incrcount(); 
	    ws->trail[j] = 1; }
	CFRE(sarray);
	if (notbatch)
	  wherepatternsettrace(ws->mstate->get_lt(),ws->mstate->get_rt(), ws->actwh->strateg);
	goto forward; }
      else  {
	CFRE(sarray);
	if (notbatch) wherepatternfailtrace(ws->actwh->pattern,ws->actwh->strateg);
	goto backwardpat; }
   default:
    wt.tdelete();
  test:
    if (lastws->actexstate->nextsolution(substarray[lastws->actwh->leftvarn])) {
      substarray[lastws->actwh->leftvarn].incrcount();
      if (notbatch)
	wheresettrace(lastws->actwh->leftvarn,substarray[lastws->actwh->leftvarn],
		      lastws->actwh->strateg);
      goto forward; }
    if (notbatch) wherefailtrace(lastws->actwh->leftvarn,lastws->actwh->strateg);
    substarray[lastws->actwh->leftvarn]=nullterm;
    goto backward;
  }
backward:
#ifdef RSWITCH
  if(lastws->actwh->leftvarn == SWITCHCASEEND) {
      struct branch *brlist;
      if (lastws->branch_list != NULL) {
	  struct branch *br = lastws->branch_list;
	  lastws->branch_list = lastws->branch_list->next;
	  // test condition
        if (1) {
          if (isNewTseqBacktrackNextSol(
	    res, br->tseq, substarray, lastws,notbatch, varn))
	      return 1;
	  else
	      goto backward;
	} else goto backward;  
      } else {
	  struct wheress *pp = lastws;
	  DELETE1(lastws);
	  lastws = pp;
	  if (lastws) 
	      goto backward;
	  else
	      return 0;
      }
  }
#endif
  if (lastws->TRYCHOICE) {
    int len;

    //stout << "BACKWARD BACKTRACK TRYCHOICEEND\n";
    //stout << "BRANCH LEN " << lastws->len;
    //stout << "DELETE LEN=" << lastws->len << "\n";

    //stout << "ACTWH BEFDEL LEN=" << LEN(actwh) << "\n";

    actwh = delete_n_wheres(lastws->len,actwh);
    //stout << "ACTWH LEN=" << LEN(actwh) << "\n";
    
    lastws->wherebranches = lastws->wherebranches->next; // next branch
    if (lastws->wherebranches == NULL) {
	//stout << "SWITCH FAILS\n";
	ws = lastws; lastws = lastws->prev; 
	CFRE(ws);
	if (lastws == NULL) return(0); 
	else goto backward0;
    }
    actwh = appendwherelists(lastws->wherebranches->wherebranch,
			     actwh,&len);
    //stout << "BRANCH LEN=" << len << "\n";
    lastws->len = len;
    lastws->actwh = actwh;
    goto forward0;
  }
  if (lastws->actwh->leftvarn == WHEREPATTERN) {
      matchstate_dealloc++;
      DELETE1(lastws->mstate); }
  lastws->actexstate->free(); 
  stateofexecution_dealloc++;
  DELETE1(lastws->actexstate);
  ws = lastws; lastws = lastws->prev; 
  CFRE(ws);
  if (lastws == NULL) return(0); 
backward0: 
  actwh = lastws-> actwh; 
  if (lastws->TRYCHOICE) goto backward;
  switch (actwh->leftvarn) {
    case IFVARN:
      goto backward;  
//    case TRYCHOICEEND:
//      goto backward;
    case WHEREPATTERN:
      ws = lastws;
      for(j = 0; j < varn; j++)
	if (ws->trail[j]) { ws->trail[j] = 0;
	  substarray[j].decrcount(); substarray[j].tdelete(); substarray[j] = nullterm; }
      // *** actwh->pattern.decrcount(substarray);  
      goto testpat; 
    default:
      substarray[lastws->actwh->leftvarn].decrcount(); 
      substarray[actwh->leftvarn].tdelete(); 
      goto test;
  } 
} 

//-----------------------------------------------------



