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

#include "termdefs.h"
#include <vector>
#include "module.h"
#include "codes.h"
#include "compiledefs.h"
#include "strategy.h"

#include "command.h"

#include "strategy.h"

/* ------------ building term from R-derivation output -------------------*/

// Stack of the terms being built (grows on demand: terms of any depth;
// MAXTERMDEEP = 20000 until S3b). tstack[0..tstacki-1] are in use.
static std::vector<term> tstack;
static int tstacki = 0; 
int writewasident = 1;
int equal_non_ground = 0;     // local hack


#define TERMWRITEBLANK() (writewasident?" ":"")


int fsyminfo(int fsi)
{
  return(fsymtab[fsi].infos());
}

term::term()
{
  t=NULL;
}

int term::semantic()
{
  int i = head();
  if (0 <= i && i < MAXNFSYM)
    return(fsymtab[i].get_semantic());
  else { sterr << "\n [term::semantic] fatal error\n"; failexit(); }
}

int term::isvalidterm()           // QQQQQQQ
{ return(t!=NULL);
}

void term::stinit()
{ //tstacki=0;
}

struct hterm * term::makterm(int fsi,int inf,int count, term *st)
{
  NNEW(t,struct hterm);
  t->compif.isVarExt=0;
  t->fsymi=fsi;
  t->infos= inf;
  t->counter=count;
  t->subt=st;
  statistic.incNumberOfAllocTerm();
  return(t);
}

void term::addterm(int fsi,int inf,int count, term *st)
{  pushht(makterm(fsi,inf,count,st));
}

void term::pushht(struct hterm *ht)
{
  if (tstacki >= (int)tstack.size()) tstack.resize(tstacki + 1);
  tstack[tstacki++].t= ht;
}

void term::popt()
{
  if (tstacki<=0) {
    sterr<< "[term::pop]tstacki underflow\n";
    interr();
  }
  t=tstack[--tstacki].t;
}

void term::pusht(term t)
{
  pushht(t.t);
}

void term::crstterm(int val,int type)
{
  addterm(val,type,0,NULL);
}

void term::crststring(char *val)
{
  addterm(0,TSTRING,0,(term*)val);
}

char *term::getstring()
{

    //sterr << "this = '" << this << "'\n";
    //sterr << "this->subt = '" << ((char*)(t->subt)) << "'\n";
  
  if (t->infos == TSTRING)
   return((char*)(t->subt));
  else {
    sterr << "\n not a string\n";
    failexit();
  }
}

void term::crvar(int nv)
{
  addterm(nv,TVAR,0,NULL);   // this should not work, when you compile !!!
}

void term::crvar(int nv, lexem sort)
{
  addterm(nv,TVAR,0,NULL);
  tstack[tstacki-1].t->compif.varsort = sort;     // !!!!!!!!!
}

void term::crExtVar(int nv, lexem sort)
{
  addterm(nv,TVAR,0,NULL);
  tstack[tstacki-1].t->compif.varsort = sort;
  tstack[tstacki-1].t->compif.isVarExt = 1;
}

void term::crdoublecon(double vv)
{
  int doublebuf[2];   // S2: was an int* aliasing vv (strict-aliasing UB)
  memcpy(doublebuf,&vv,sizeof(vv));
  crstterm(doublebuf[1],TNUMBER);
  crstterm(doublebuf[0],TNUMBER);
  crterm(DOUBLECONSTRUCT);
}

double term::doubleval()
{ int dv[2];
  if (t->fsymi != DOUBLECONSTRUCT) {
    fprintf(stderr,"[doubleval] term.c double const exp. \n"); interr();
  }
  dv[0] = t->subt[0].head();
  dv[1] = t->subt[1].head();
  double d;           // S2: was *(double*)dv (strict-aliasing UB)
  memcpy(&d,dv,sizeof(d));
  return(d);
}

void term::crdoubleunmin()
{ term tt;
  int dv[2];
  tt = tstack[tstacki-1];
  if (tt.t->fsymi != DOUBLECONSTRUCT) {
    fprintf(stderr,"[doubleval] term.c double const exp. \n"); interr();
  }
  dv[0] = tt.t->subt[0].head();
  dv[1] = tt.t->subt[1].head();
  double d;           // S2: was *(double*)dv (strict-aliasing UB)
  memcpy(&d,dv,sizeof(d));
  d = - d;
  memcpy(dv,&d,sizeof(d));
  tt.t->subt[0].t->fsymi = dv[0];
  tt.t->subt[1].t->fsymi = dv[1];
}

double term::doublepop()
{ double rval;
  term tt;
  tt = tstack[--tstacki];
  rval = tt.t->fsymi;
  tt.tdelete();
  return(rval);
}

void term::crdouble1()
{
  crdoublecon(doublepop());
}

void term::crdouble2()
{ double dval,ddval;
  dval = doublepop();
  ddval = doublepop();
  if (ddval != 0) 
    dval = dval + ((0.0+ddval)/pow(10,1+floor(log10(ddval))));
  crdoublecon(dval);
}


void term::crdouble3a()
{ double dval,ddval;
  dval = doublepop();
  ddval = doublepop();
  if (ddval != 0) 
    dval = dval + ((0.0+ddval)/pow(10,1+floor(log10(ddval))));
  ddval = doublepop();
  dval = dval * pow(10,ddval);
  crdoublecon(dval); 
}

void term::crdouble3b()
{ double dval,ddval;
  dval = doublepop();
  ddval = doublepop();
  if (ddval != 0) 
    dval = dval + ((0.0+ddval)/pow(10,1+floor(log10(ddval))));
  ddval = doublepop();
  dval = dval * pow(10,-ddval);
  crdoublecon(dval); 
}

void term::crdouble4()
{ double dval,ddval;
  dval = 0;
  ddval = doublepop();
  if (ddval != 0) 
    dval = dval + ((0.0+ddval)/pow(10,1+floor(log10(ddval))));
  crdoublecon(dval);
}


void term::crdouble5a()
{ double dval,ddval;
  dval = 0;
  ddval = doublepop();
  if (ddval != 0) 
    dval = dval + ((0.0+ddval)/pow(10,1+floor(log10(ddval))));
  ddval = doublepop();
  dval = dval * pow(10,ddval);
  crdoublecon(dval); 
}

void term::crdouble5b()
{ double dval,ddval;
  dval = 0;
  ddval = doublepop();
  if (ddval != 0) 
    dval = dval + ((0.0+ddval)/pow(10,1+floor(log10(ddval))));
  ddval = doublepop();
  dval = dval * pow(10,-ddval);
  crdoublecon(dval); 
}

void term::crterm(int fsi)
{
  crterm(fsi,fsymtab[fsi].arity());
}

void term::crterm(int fsi,int a)
{ int i;
  term tt,*st;
  st=NULL;
  if (a>0) {

    NNEW(st ,term[a]);
    for (i=0; i<a; i++) {
      if (tstacki==0) {
        sterr << "[term::crterm] tstacki underflow, internal error !!!\n";
        failexit();
      }
      tt=tstack[--tstacki];
      tt.incrcount();
      st[i]=tt;
    }
  }
  addterm(fsi,TNORMFS,0,st);
}

void term::crterm_reverse(int fsi,int a)
{ int i;
  term tt,*st;
  st=NULL;
  if (a>0) {

    NNEW(st ,term[a]);
    for (i=0; i<a; i++) {
      if (tstacki==0) {
        sterr << "[term::crterm] tstacki underflow, internal error !!!\n";
        failexit();
      }
      tt=tstack[--tstacki];
      tt.incrcount();
      st[a-i-1]=tt;
    }
  }
  addterm(fsi,TNORMFS,0,st);
}

/* ------------ building term from L-derivation output -------------------*/
/*

static struct {
   term t;
   int i;
} lstack[MAXTERMDEEP];
static term lresterm;
static int lstacki=0;

void term::lstinit()
{
//  lstacki=0;
  lresterm.t=NULL;
}

void term::laddterm(struct hterm *tt)
{ int *acti;
  term *actt;
  do {
    lstacki--;
    if (lstacki < 0) {
       if (lresterm.t != NULL) sterr << "[term::laddterm] something is wrong\n";
       lresterm.t=tt; 
       break;
    }
    acti =  &(lstack[lstacki].i);
    actt =  &(lstack[lstacki].t);
    actt->t->subt[*acti].t = tt;
    actt->t->subt[*acti].incrcount();
    (*acti)++;
    tt = actt->t;
  } while(*acti == fsymtab[actt->head()].arity());
  lstacki++;
  if (lstacki>=MAXTERMDEEP) {
     sterr << "[term::addterm] lstacki overflow over MAXTERMDEEP\n\t"
	  << "internal error, sorry!!\n";
     failexit();
  }
}

void term::lpopt()
{
  if (lstacki != 0 || lresterm.t == NULL) sterr << "[term::lpopt] lstacki = "
                          << lstacki << ",internal error\n";
  t=lresterm.t;
}

void term::lcrstterm(int val,int type)
{
  laddterm(makterm(val,type,0,NULL));
}

void term::lcrvar(int nv)
{
  laddterm(makterm(nv,TVAR,0,NULL));
}

void term::lcrterm(int fsi)
{ term *st;
  int a;
  st = NULL;
  a = fsymtab[fsi].arity();
  if (a==0) laddterm(makterm(fsi,TNORMFS,0,st));
  else {
    NNEW(st ,term[a]);
    lstack[lstacki].t.t = makterm(fsi,TNORMFS,0,st);
    lstack[lstacki].i = 0;
    lstacki++;
    if (lstacki>=MAXTERMDEEP) {
     sterr << "[term::addterm] lstacki overflow over MAXTERMDEEP\n\t"
	  << "internal error, sorry!!\n";
     failexit();
    }
  }

}

*/
/* ------------ building term from UNIF output ----------------------*/
/*

void term::unistinit()
{
  lstacki=0;
  lresterm.t=NULL;
}

void term::unipopt()
{
  if (lstacki != 0 || lresterm.t == NULL) sterr << "[term::unipopt] lstacki = "
                          << lstacki << ",internal error\n";
  t=lresterm.t;
}

void term::uniaddterm()
{ int *acti;
  term *actt;
  term t,*st;
  lstacki--;
  t =  lstack[lstacki].t;
  if (t.inf() == TNORMFS && fsymtab[t.head()].arity()!=lstack[lstacki].i)
    sterr << "[term::uniaddterm 1] something is wrong\n";
  lstacki--;
  if (lstacki < 0) {
     if (lresterm.t != NULL) sterr << "[term::uniaddterm] something is wrong\n";
     lresterm=t; 
     lstacki=0;
     return;
  }
  acti =  &(lstack[lstacki].i);
  actt =  &(lstack[lstacki].t);
  if (*acti==2 && fsymtab[actt->head()].arity()==2 
               && fsymtab[actt->head()].infos()==FSASSOCCOM) {
    NNEW(st,term[2]);
    st[0] = *actt;
    st[0].incrcount();
    lstack[lstacki].t.t = makterm(actt->head(),TNORMFS,0,st);
    lstack[lstacki].i = 1;
    acti =  &(lstack[lstacki].i);
    actt =  &(lstack[lstacki].t);
  }
  if (*acti == fsymtab[actt->head()].arity()) 
    sterr << "[term::uniaddterm 2] something is wrong\n";
  actt->t->subt[*acti] = t;
  actt->t->subt[*acti].incrcount();
  (*acti)++;
  lstacki++;
  if (lstacki>=MAXTERMDEEP) {
     sterr << "[term::addterm] lstacki overflow over MAXTERMDEEP\n\t"
	  << "internal error, sorry!!\n";
     failexit();
  }

}

void term::unicrstterm(int val,int type)
{
  lstack[lstacki].t.t=makterm(val,type,0,NULL);
  lstack[lstacki++].i=0;
  uniaddterm();
}

void term::unicrvar(int nv)
{
  lstack[lstacki].t.t=makterm(nv,TVAR,0,NULL);
  lstack[lstacki++].i=0;
  uniaddterm();
}

void term::unicrterm(int fsi)
{ term *st;
  int a;
  st = NULL;
  a = fsymtab[fsi].arity();
  if (a==0) {
    lstack[lstacki].t.t=makterm(fsi,TNORMFS,0,st);
    lstack[lstacki++].i=0;
    uniaddterm();
  } else {
    NNEW(st ,term[a]);
    lstack[lstacki].t.t = makterm(fsi,TNORMFS,0,st);
    lstack[lstacki++].i = 0;
  }
}
*/

/* ------------- transforms with ac structure of terms ----------------*/

TERM * term::toacform()
{ TERM_LIST *l1,*l2;
 TERM *ret = NULL; /* initialised to avoid warning */
  int i,a;
  int val,codedValue;
  //stout << ">>>toac "; this->write(stout); stout << "\n";

  if (t==NULL) {
    sterr << "[term.cc] toacform: "; interr();
  }
  switch (t->infos) {
  case TIDENT: 	ret = make_term(MAXNFSYM+t->fsymi,NULL,CONSTAN,0,NULL); break;
  case TNUMBER:
    /*
     * Il y a un bug avec les entiers negatifs
     */
    val = t->fsymi;
    codedValue=(val>=0)?(2*val):-(2*val-1);
    //printf("val = %d  \t",val);
    //printf("codedValue = %d\n",codedValue);
    ret = make_term(MAXNFSYM+MAXNOFIDENT+codedValue,NULL,CONSTAN,0,NULL);
    break;
  case TSTRING: sterr << "\nTSTRING in toacform not implemented yet\n"; interr();
  case TVAR:	ret = make_term(t->fsymi,NULL,VAR,0,NULL); break;
  case TNORMFS:
	  	a = headarity();
          	if (a==0) { ret = make_term(t->fsymi,NULL,CONSTAN,0,NULL); goto rrr; }
		i=0;
		l1=l2=make_term_list(t->subt[i++].toacform(),(TERM_LIST *) NULL);
		while (i<a) {
	          l2 = make_term_list(t->subt[i++].toacform(),l2);
	        }
	        if (fsymtab[t->fsymi].infos() == FSASSOCCOM) 
			ret = make_term(t->fsymi,l2,ACFUNC,a,l1);
		else 	ret = make_term(t->fsymi,l2,FUNC,a,l1);
		break;
  default :     interr();
  }
  rrr:
// stout << "<<<toac ";   print_term(ret); stout << "\n";
  return ret;
}

void term::tomyform(TERM * tt)
{ TERM_LIST *l;
  int a;
  if(tt == NULL){
    fprintf(stderr,"[term.cc] (null ptr)"); interr();
  }
  switch(tt->type){
  case VARIABLE:
		crvar(tt->sym);
		break; 
  case CONSTANT:
		// if (0 < tt->sym  && tt->sym < MAXNFSYM) crterm(tt->sym); // Bug- 27/11/98 Pedros
		if (0 <= tt->sym  && tt->sym < MAXNFSYM) 
		  crterm(tt->sym);
		// else if (0 < tt->sym  && tt->sym < MAXNFSYM+MAXNOFIDENT) // Bug- 27/11/98 Pedros
		else if (0 <= tt->sym  && tt->sym < MAXNFSYM+MAXNOFIDENT) 
		  crstterm(tt->sym-MAXNFSYM,TIDENT);
		else {
		  int codedValue=tt->sym-MAXNFSYM-MAXNOFIDENT;
		  int value=((codedValue%2)==0)?codedValue/2:-(codedValue-1)/2;
		  //printf("codedValue = %d\t",codedValue);
		  //printf("value = %d\n",value);
		  crstterm(value,TNUMBER);
		}
    break;
  case FUNCTION:
	l = tt->rest.f.arg_list; a=0;
  	while(l != NULL){
 	   tomyform(l->arg); a++;
 	   l = l->next_arg;
 	}
	if (a != fsymtab[tt->sym].arity()) {
	  sterr<<"[term.cc] arity problem"; interr();
	}
	crterm(tt->sym);
	break;
  case AC_NORMAL:
	l = tt->rest.f.arg_list;
	tomyform(l->arg); l = l->next_arg;
  	while(l != NULL){
 	   tomyform(l->arg);
	   crterm(tt->sym);
 	   l = l->next_arg;
 	}
	break;
  default: fprintf(stderr,"\n[term.cc] non expected case in print_term"); interr();
  }
}



/* ------------------------------------------------------------------*/


int term::tcmp(term with)	       // terms must be grounds !!!!!!!!!!
{ int i,a,res;
  if (t->infos == TVAR || with.t->infos==TVAR) {
	sterr << "[ error] var in comparing grounds terms\n"; interr();
  }
  switch (t->infos) {
  case TIDENT:   case TNUMBER:
                if (with.t->infos!=t->infos) {
		  fprintf(stderr,"[warning] comparing two terms of different basic types\n");
                  return(t->infos - with.t->infos);
		}
		if (t->fsymi == with.t->fsymi) return(0);
		return((t->fsymi > with.t->fsymi)?1:-1);
  case TSTRING: if (with.t->infos!=t->infos) {
                  fprintf(stderr,"[warning] comparing a string with a term\n"); }
                return(strcmp(getstring(),with.getstring()));
  case TNORMFS: res = (int)t->fsymi - (int)with.t->fsymi;
		if (res != 0) return((res>0)?1:-1);
		if (fsymtab[t->fsymi].infos()==FSASSOCCOM){
		   match_state m(*this,with,ACMATCH,0);
                   fprintf(stderr,"[warning] trying to order terms with AC symbols\n");
		   return(m.isnextsol(NULL));
	        }
		a= headarity();
		i=0;
  		while(i<a && res==0) {
    		  res = t->subt[i].tcmp(with.t->subt[i]);
		  i++;
  		}
		return(res);
		break;
  default :     interr();
  }
  return 0; /* to avoid warning */
}

int term::equal(term with)	       // terms must be grounds !!!!!!!!!!
{ int i,a,res;

  //stout << "*this = "; this->write(stout); stout << "\n";
  //stout << " with = "; with.write(stout); stout << "\n";
 
  if (t->infos == TVAR || with.t->infos==TVAR) {
    if (equal_non_ground) {
      return (t->infos == with.t->infos); }
    else {
      sterr << "[error ] var in comparing grounds terms\n"; interr(); }
  }
  switch (t->infos) {
  case TIDENT:   case TNUMBER:
    return(with.t->infos==t->infos && with.t->fsymi==t->fsymi);
    break;
  case TSTRING:
    return(with.t->infos==t->infos && (!strcmp(with.getstring(),getstring())));
    break;
  case TNORMFS:
    res = (t->fsymi == with.t->fsymi);
    if(res==0) {
      return(0);
    }
    if(fsymtab[t->fsymi].infos()==FSASSOCCOM) {
      match_state m(*this,with,ACMATCH,0);
      res=m.isnextsol(NULL);
      //stout << "res = " << res << "\n\n";
      return(res);
    }
    a= headarity();
    i=0;
    while(i<a && res) {
      res = res && t->subt[i].equal(with.t->subt[i]);
      i++;
    }
    return(res);
    break;
  default :     interr();
  }
  return 0;
}

int term::equal_nground(term with)	       // terms must be grounds !!!!!!!!!!
{ int i,a,res; 
  if (t->infos == TVAR || with.t->infos==TVAR) {
      return (t->infos == with.t->infos && with.t->fsymi == t->fsymi); }
  switch (t->infos) {
  case TIDENT:   case TNUMBER:
		return(with.t->infos==t->infos && with.t->fsymi == t->fsymi);
  case TSTRING: return(with.t->infos==t->infos && (!strcmp(with.getstring(),getstring())));
  case TNORMFS: res = t->fsymi == with.t->fsymi;
		if (! res) return(0);
		if (fsymtab[t->fsymi].infos()==FSASSOCCOM){
		   match_state m(*this,with,ACMATCH,0);
		   return(m.isnextsol(NULL));
	        }
		a= headarity();
		i=0;
  		while(i<a && res) {
    		  res = res && t->subt[i].equal_nground(with.t->subt[i]);
		  i++;
  		}
		return(res);
		break;
  default :     interr();
  }
  return 0;
}

term term::acextend()
{ term res;
  res.pushht(t);
  res.crvar(0);
  res.crterm(head());
  res.popt();
  return(res);
}

void term::toptdelete()
{ term *st;
  int i,a;
  if (t->counter == 0) {
    a= headarity();
    if (a>0) {
      st = t->subt;
      for (i=0; i<a; i++) st[i].decrcount();
      delete [] st;
    }
    DELETE1(t);
  }
}



int term::occursin(int validepos, term tt)		// only for ground terms
{ int i,a;
  term et,sext;
  switch (tt.t->infos) {
  case TVAR : interr();
  case TIDENT:   case TNUMBER:
		return(tt.t->infos==t->infos && tt.t->fsymi == t->fsymi);
  case TSTRING: return(0);
  case TNORMFS: if (validepos && equal(tt)) return(1);
		if (validepos && t->fsymi==tt.t->fsymi 
                    && fsymtab[t->fsymi].infos()==FSASSOCCOM){
		   et = acextend();
		   match_state m(tt,et,ACMATCH,1);
		   if (m.isnextsol(&sext)) {et.toptdelete(); return(1);}
		   et.toptdelete();
		   return(occursin(0,tt));
	        } else {
		  a= tt.headarity();
		  for (i=0; i<a; i++) {
		    et = *tt.subterm(i);
		    if (occursin(validepos || et.t->fsymi!=t->fsymi, et)) return(1);
		  }
		  return(0);
		}
		break;
  default :     interr();
  }
  return 0;
}


void term::rewrite(term t2)
{ int i,a1,a2;
  term tt,*st;
  st = t2.t->subt;
//  st = NULL;  // ???????
  a1= headarity();
  a2= t2.headarity();
  if (a2>0) {
    NNEW(st,term[a2]);
    for(i=0;i<a2;i++) {
      tt= t2.t->subt[i];
      tt.incrcount();
      st[i]=tt;
    }
  }
  if (a1>0) {
    for(i=0;i<a1;i++) {
      t->subt[i].decrcount(); t->subt[i].tdelete();
    }
    delete [] t->subt;
  }
  t->fsymi = t2.head();
  t->infos = t2.inf();
  t->subt = st;
}

int term::stermreplace(int vp,term wt, term byt, term &intothis)
{ term tt,*st,sti,et,sext,ssext;
  int i,j,a,res,rres;
  if (vp && this->equal(wt)) {
     intothis = byt;
     return(1);
  }
  if (vp && t->fsymi==wt.t->fsymi && fsymtab[t->fsymi].infos()==FSASSOCCOM){
	et = wt.acextend();
	match_state m(*this,et,ACMATCH,1);
	if (m.isnextsol(&sext)) {
	  et.toptdelete();
	  if (! sext.stermreplace(1,wt,byt,ssext)) ssext = sext;
	  intothis.pusht(ssext);
	  intothis.pusht(byt);
	  intothis.crterm(head());
	  return(1);
	}
	et.toptdelete();
	vp = 0;
  }
  a = headarity();
  st = NULL; res=0;
  for(i=0; i<a; i++) {
      sti = t->subt[i];
      rres = sti.stermreplace((vp || sti.t->fsymi!=wt.t->fsymi),wt,byt,tt);
      switch ((res<<1) | rres) {
      case 0 : break;
      case 1 : NNEW(st ,term[a]);
               for (j=0; j<i; j++) {
			st[j]= t->subt[j];
			st[j].incrcount();
		}
						// case will continue to case 3
	       [[fallthrough]];
      case 3 : st[i] = tt;
	       break;
      case 2 : st[i] = t->subt[i];
		break;
      default : sterr << "[stermreplace:term.cc] "; interr();
      }
      res = res || rres;
      if (res) st[i].incrcount();
  }
  if (res) {
    NNEW(intothis.t ,struct hterm);
    statistic.incNumberOfAllocTerm();

    intothis.t->compif.isVarExt=0;
    intothis.t->fsymi = t->fsymi;
    intothis.t->infos = t->infos;
    intothis.t->counter =0;
    intothis.t->subt = st;
  }
  return(res);
}

void term::copyinstallrec(int check,term &intothis,term *substarray)
{ term tt,*st, into;
  int i,a;
//          st = NULL;
  if (t->infos == TVAR) {
      into = substarray[t->fsymi];
      if (into.t == NULL) {
	if (check) {
	  intothis = *this; }
	else {
//tt = *st;
          sterr << "term::copyinstall internal error installing variable # "
		<< t->fsymi << "\n"; 


          failexit(); }
      } else 
	{
	  intothis = into; }
//     intothis.incrcount();
     return;
  }
  a = headarity();
  st = t->subt;
//  st = NULL;
  if (a>0) {
    NNEW(st ,term[a]);
    for(i=0; i<a; i++) {
      t->subt[i].copyinstallrec(check,tt,substarray);
      tt.incrcount();
      st[i]=tt;
    }
  }
  NNEW(intothis.t ,struct hterm);
  statistic.incNumberOfAllocTerm();

  intothis.t->compif.isVarExt=0;
  intothis.t->fsymi = t->fsymi;
  intothis.t->infos = t->infos;
  intothis.t->counter =0;
  intothis.t->subt = st;
}

void term::copyinstall(int check,term &intothis,term *substarray)
{

//stout << "COPYINSTALL \n" << this << "\n"; this->write(stout);stout << "\n\n";

  if (t->infos == TVAR) {
    if ((substarray[t->fsymi].t == NULL) && check) {
      intothis = *this; }
    else {
//      substarray[t->fsymi].write(stout); stout << "\n";
        substarray[t->fsymi].topcopy(intothis); }
  }
  else copyinstallrec(check,intothis,substarray);
}

// ?????????????????
void term::copy(term &intothis)
{
  copyrec(intothis);
}

void term::copyrec(term &intothis)
{ term tt,*st;
  int i,a;
    a = headarity(); 
    st = t->subt;
//    st = NULL;  // ???????
      if (a>0) {
	NNEW(st ,term[a]);
	for(i=0; i<a; i++) {
	  t->subt[i].copyrec(tt);
	  tt.incrcount();
	  st[i]=tt; } }
  NNEW(intothis.t ,struct hterm);
  statistic.incNumberOfAllocTerm();

  intothis.t->compif.isVarExt=0;
  intothis.t->fsymi = t->fsymi;
  intothis.t->infos = t->infos;
  intothis.t->counter =0;
  intothis.t->subt = st;
  intothis.t->compif.varsort = t->compif.varsort;  // bug found thanks to PEM's aterm
}
// ???????????????????????????///

void term::topcopy(term &intothis)
{ term *st;
  int i,a;
  a = headarity();
  st = t->subt;
//  st = NULL;  // ?????????
  if (a>0) {
    NNEW(st ,term[a]);
    for(i=0; i<a; i++) {
      st[i]= t->subt[i];
      st[i].incrcount();
    }
  }
  NNEW(intothis.t ,struct hterm);
  statistic.incNumberOfAllocTerm();
  
  intothis.t->compif.isVarExt=0;
  intothis.t->fsymi = t->fsymi;
  intothis.t->infos = t->infos;
  intothis.t->counter =0;
  intothis.t->subt = st;
}

int term::getcount()
{
  return (t->counter);
}

void term::incrcount()
{
  (t->counter)++;
}

void term::decrcount()
{
  (t->counter)--;
}

void term::tdelete()
{ term *st;
  int i,a;

//MEMDEBUG

//      if (t->infos == TNORMFS) {
//	  write(stout); stout << "/" << t->counter << " "; }

  if (t->counter == 0) {
    a= headarity();
    if (a>0) {
      st = t->subt;
      for (i=0; i<a; i++) { st[i].decrcount(); st[i].tdelete();}
      delete [] st;
    }
    DELETE1(t);
  }
}

int term::head()
{
  return(t->fsymi);
}

int term::headarity()
{
  if (t->infos == TVAR || t->infos == TIDENT || t->infos==TNUMBER) return(0);
  if (t->infos == TSTRING) return(0);
  else return(fsymtab[t->fsymi].arity());
}

int term::inf()
{
  return(t->infos);
}

term *term::subterm(int i)
{
  return(&(t->subt[i]));
}

void term::operator =(term tt)
{
  t = tt.t;
}

int max(int a, int b)
{
  if (a < b) return b; else return a;
}

int term::varnumbers()
{
  int i,m; /* ,a */
  switch (t->infos) {
  case TIDENT: 	case TNUMBER: return -1;
  case TVAR:	return(t->fsymi);
  case TSTRING: return(-1);
  case TNORMFS: m = -1; 
		for(i=0; i<headarity(); i++) 
    		  m = max(m,t->subt[i].varnumbers());
		return m;
  default :     interr();
  }
}

int term::cont_var(unsigned varnum)
{
 int i,a;

  switch (t->infos) {
  case TIDENT: 	case TNUMBER: return(0);
  case TVAR:	return(varnum == t->fsymi);
  case TSTRING: return(0);
  case TNORMFS: a= headarity();
		for(i=0; i<a; i++) 
    		  if (t->subt[i].cont_var(varnum)) return(1);
		return(0);
  default :     interr();
  }
}

int  term::ren_var(unsigned old, int nova)
{
 int i,a;
  switch (t->infos) {
  case TIDENT: 	case TNUMBER: 
		break;
  case TSTRING: break;
  case TVAR:	if (old == t->fsymi)
                  t->fsymi = nova;
		break;
  case TNORMFS:
		a= headarity();
		for(i=0; i<a; i++) 
    		  t->subt[i].ren_var(old,nova);
		break;
  default :     interr();
  }
  return(0);
}

void term::remove_inlines()
{ int i,j,a;
  switch (t->infos) {
  case TIDENT: 	case TNUMBER: case TVAR:
		break;
  case TSTRING: break;
  case TNORMFS:
    a= headarity();
    for(i=0; i<a; i++) t->subt[i].remove_inlines();
    if ((j=is_inlin(t->fsymi)) != -1) 
      add_inline_symbol(FALSE,j,t->subt[0],t->subt[1],t->subt[1]); 
    else if ((j=is_inlineplus(t->fsymi)) != -1) 
   // 3005      add_inline_symbol(TRUE,j,t->subt[0],t->subt[1],t->subt[2]); 
      add_inline_symbol(TRUE,j,t->subt[0],t->subt[2],t->subt[1]); 
    else if ((j=is_let(t->fsymi)) != -1)
      add_let_symbol(j,t->subt[0],t->subt[1]);
    break;
  default :     interr();
  }
}

/*
if @        : (bool) Strany                 code 145;
@           : (Strany) Stranies             code 146;
@ , @       : (Strany Stranies) Stranies    code 147;
*/
int is_aff_let(int varn, term *p)
{
  if (p->head()/*semantic()*/ != STRANYIF) 
    if (p->subterm(0)->inf() == TVAR && p->subterm(0)->head() == varn)
      return 1;
  return 0;
}

int is_ref_let(int varn, term *p)
{
  return (p->subterm(1))->cont_var(varn);
}

int is_aff(int varn, term *p)
{
  if (p->head()/*semantic()*/ != STRANYIF) 
    if (p->subterm(1)->inf() == TVAR && p->subterm(1)->head() == varn)
      return 1;
  return 0;
}

int is_ref(int varn, term *p)
{
  return (p->subterm(0))->cont_var(varn);
}

int is_affect(int varn, term anies, int (*fn)(int, term*) )
{
term *any = &anies;
int cnt = 0;
  for(any = &anies,cnt=0; any->head()/*semantic()*/ == STRANYCONS; any = any->subterm(0))
    cnt += (*fn)(varn,any->subterm(1));
  if (any->head()/*semantic()*/ != STRANYNIL) {
    sterr << "is_affected int.err.\n"; failexit(); }
  cnt += (*fn)(varn,any->subterm(0));
  return cnt;
}

//*strany
int is_affected(int varn, term anies) { return is_affect(varn,anies,is_aff);}
int is_referenced(int varn, term anies) { return is_affect(varn,anies,is_ref);}
//*let in
int is_affected_let(int varn, term anies) { return is_affect(varn,anies,is_aff_let);}
int is_referenced_let(int varn, term anies) { return is_affect(varn,anies,is_ref_let);}

int  affected[MAXNOFVAR];

void inlinewhere(term *t) // for lets
{
term *var, *strat;
int varn;
char *strnam;
int strx, stry;
//char strnam[STRLEN];
  var = t->subterm(0); strat = t->subterm(1);
  if (var->inf() != TVAR) {
    sterr << "[fatal] a variable should be assigned\n"; failexit(); }
  varn = var->head();

  inverse_apply_code(*strat,&strx,&stry);
  strnam = attach_type_mod("eval",ld.vartab[varn]->leftside.typeval(),
			   evalmoduli(strx,stry));
// EEEEE	   evalmoduli(vartab[varn]->leftside.typeval()));
  ld.actwhstrategy = trrules.strategyindex_refs(strnam); 
  ld.acttrrule->addwhere(0,varn,
		      (ld.actwhstrategy==-1)?
                      ((strategy**)NULL):
                      trrules.getstrategyadr_refs(ld.actwhstrategy),
                      *strat,ld.vartab[varn]->leftside);
}

void term::add_let_symbol(int j, term asses, term exp)
{
char sss[STRLEN];
lexem resle, selfle, le,inle;
struct sgrammrule *gr;
int cde, i, uu, arty;
term ls, rs, *rlab, labterm, *any, resvar ;
char *strnam;
//char strnam[STRLEN];

  this->stinit(); 
  snprintf(sss,sizeof(sss),"INLINE%d",++ninlines);
  inle.cridlex(sss); modframes[stacki].gr.addsymbol(inle);
  le.crcharlex('('); modframes[stacki].gr.addsymbol(le);
  for(i=0, arty=0; i<ld.actvtabi; i++) {
    uu = is_affected_let(i,asses);
    if (uu > 1) {
      sterr<<"[error] variable Var(" <<i<< ") is several times assigned\n"; 
      failexit(); }
    if ((exp.cont_var(i) || is_referenced_let(i,asses)) && uu == 0) {
      modframes[stacki].gr.addnont(ld.vartab[i]->leftside);
      arty++; this->crvar(i,ld.vartab[i]->leftside); 
    }
  }
  le.crcharlex(')'); modframes[stacki].gr.addsymbol(le);
  le.crtypelex(add_strat_nont(inlinecodes[j].from,inlinecodes[j].to));
  gr = modframes[stacki].gr.addrule(le,RNOPRIOR,RGLOP,ld.actcode);
  cde = ld.actcode;
  add_to_fsymtab(arty,gr,0 ); // later DS_LET
  this->crterm(cde,arty);
  this->popt();
  this->copy(labterm);
  // ------------------------------------------
  selfle.crtypelex(inlinecodes[j].from); 
  resle.crtypelex(inlinecodes[j].to); 
  ls.stinit();
  ls.crvar(ld.actvtabi,selfle);
  ls.pusht(labterm);
  ls.crterm(apply_code(inlinecodes[j].from,inlinecodes[j].to),2);
  ls.popt();
  //  ls.write(stout); stout.flush();
  //--------------
  resvar.stinit();
  resvar.crvar(ld.actvtabi+1,resle);
  resvar.popt();
  //---------------
  rs.stinit();
  rs.crvar(ld.actvtabi,selfle);
  rs.pusht(exp);
  rs.crterm(apply_code(inlinecodes[j].from,inlinecodes[j].to),2);
  rs.popt();
  //  rs.write(stout); stout.flush();
  NNEW(rlab, term); 

  strnam = attach_type("DSTR",inlinecodes[j].to);
  ld.acttrrule = trrules.addrule(strnam,ld.actvtabi+2,ls,resvar, // rs,
			      stratmoduli(inlinecodes[j].from,inlinecodes[j].to),RGLOP,
			      NULL,
			      (acsymbolinleftside?ACMATCH:NORMMATCH),*rlab,NULL);

  for(any = &asses; any->head()/*semantic()*/ == STRANYCONS; any = any->subterm(0))
    inlinewhere(any->subterm(1));
  if (any->head()/*semantic()*/ != STRANYNIL) {
    sterr << "add_inline_code int.err.\n"; failexit(); }
  inlinewhere(any->subterm(0));

//--- add resvar

  ld.actwhstrategy = trrules.strategyindex_refs(attach_type_mod(EVALSTR,
			inlinecodes[j].to, evalmoduli(inlinecodes[j].from, inlinecodes[j].to)));
  ld.acttrrule->addwhere(reverse_wheres,ld.actvtabi+1,
		      (ld.actwhstrategy==-1)?((strategy**)NULL):
		      trrules.getstrategyadr_refs(ld.actwhstrategy),
		      rs,resle);
}

void inlinewhereif(term *t)  // for [=> => ]
{
term *var, *strat, wt;
int varn, wtype;
char *strnam;
//char strnam[STRLEN];
/// RRRRR int strx, stry;
  if (t->head()/*semantic()*/ == STRANYIF) {
    ld.acttrrule->addwhere(0,IFVARN,NULL,*(t->subterm(0)),booltype); }
  else {
    // 3005 var = t->subterm(0); strat = t->subterm(1);
    var = t->subterm(1); strat = t->subterm(0);
    if (var->inf() != TVAR) {
      sterr << "[fatal] a variable should be affected\n"; failexit(); }
    varn = var->head();
    wtype = ld.vartab[varn]->leftside.typeval();
    ///// RRRRR inverse_apply_code(*strat,&strx,&stry);
    strnam = attach_type_mod("eval",wtype,evalmoduli(wtype,wtype));
	// RRRR evalmoduli(strx,stry)); 
        // RRR in inline should be a strategy that does not change the sort
    ld.actwhstrategy = trrules.strategyindex_refs(strnam); 
    wt.stinit(); 
    wt.pusht(*var); 
    wt.pusht(*strat);
    wt.crterm(apply_code(wtype,wtype),2);
    wt.popt();
    ld.acttrrule->addwhere(0,affected[varn],
			(ld.actwhstrategy==-1)?
			((strategy**)NULL):
			trrules.getstrategyadr_refs(ld.actwhstrategy),
			wt,ld.vartab[varn]->leftside);
    ld.actwhstrategy = -1; // 3005
  }
}

void term::add_inline_symbol(int extended, int j, term t1, term anies, term t2)
{
char sss[STRLEN];
lexem le,inle;
struct sgrammrule *gr;
int cde, i, uu, arty, naffected;
term ls, rs, *rlab, labterm, *any;
//char strnam[STRLEN];
char *strnam;

  naffected = 0;
  this->stinit(); 
  snprintf(sss,sizeof(sss),"INLINE%d",++ninlines);
  inle.cridlex(sss); modframes[stacki].gr.addsymbol(inle);
  le.crcharlex('('); modframes[stacki].gr.addsymbol(le);
  for(i=0, arty=0; i<ld.actvtabi; i++) {
    affected[i] = 0;
    if (extended && (uu = is_affected(i,anies))) {
      if (uu > 1) {
	sterr<<"[error] variable Var(" <<i<< ") is several times affected\n"; 
	failexit(); }
      affected[i] = ld.actvtabi+naffected; naffected++;
      t2.ren_var(i,affected[i]);
    }
    if ((!(t1.cont_var(i))) && 
	(t2.cont_var(i) 
	|| (extended && anies.cont_var(i))
	 )) { // not exact !!!
      modframes[stacki].gr.addnont(ld.vartab[i]->leftside);
      arty++; this->crvar(i,ld.vartab[i]->leftside); 
    }
  }
  le.crcharlex(')'); modframes[stacki].gr.addsymbol(le);
  le.crtypelex(add_strat_nont(inlinecodes[j].from,inlinecodes[j].to));
  gr = modframes[stacki].gr.addrule(le,RNOPRIOR,RGLOP,ld.actcode);
  cde = ld.actcode;
  add_to_fsymtab(arty,gr,0);  // later DS_INLINE
  this->crterm(cde,arty);
  this->popt();
  this->copy(labterm);
  // ------------------------------------------
  ls.stinit();
  ls.pusht(t1);
  ls.pusht(labterm);
  ls.crterm(apply_code(inlinecodes[j].from,inlinecodes[j].to),2);
  ls.popt();
  //  ls.write(stout); stout.flush();
  //--------------
  rs = t2;
  //  rs.write(stout); stout.flush();
  NNEW(rlab, term); 

  strnam = attach_type("DSTR",inlinecodes[j].to);
  ld.acttrrule = trrules.addrule(strnam,ld.actvtabi+naffected,ls,rs,
			      stratmoduli(inlinecodes[j].from,inlinecodes[j].to),RGLOP,
			      NULL,
			      (acsymbolinleftside?ACMATCH:NORMMATCH),*rlab,NULL);

  if (extended) {
    for(any = &anies; any->head()/*semantic()*/ == STRANYCONS; 
       any = any->subterm(0)) {
      for (i=0; i < ld.actvtabi; i++) {
	if (is_affected(i,*(any->subterm(0))))
         any->subterm(1)->ren_var(i,affected[i]); }
        inlinewhereif(any->subterm(1)); }
    if (any->head()/*semantic()*/ != STRANYNIL) {
      sterr << "add_inline_code int.err.\n"; failexit(); }
    inlinewhereif(any->subterm(0));  }

  // ------------------------------------------
  // stout << sss << "into grstack([" << stacki << "])\n"; 
  // modframes[stacki].gr.dump();
}

int term::ren_vars_lin(int check, term &r, struct wherelist **whs)
{ int i,a,rr,ch,vn,actrr;
struct wherelist *whss, *wl;
term *cond;
  switch (t->infos) {
  case TIDENT:  case TNUMBER:break;
  case TSTRING:break;
  case TVAR:      vn = t->fsymi;
                rr = var_was_renamed(vn);
                if ((check == CHECK && ! rr) || 
                    (check == ADDRENAME && rr && 
		     compile)) {
		     //		     (compile || adump) )) {
		  actrr = var_rename(ld.actvtabi); //...
		  t->fsymi = actrr;  // in lhs
                  r.ren_var(vn,ld.actvtabi); // in rhs
                  for(whss=*whs; whss; whss=whss->next) {
		    whss->whereterm.ren_var(vn,ld.actvtabi);
		  }
		  NNEW(cond,term);
		  cond->stinit();
		  cond->crvar(vn,ld.vartab[vn]->leftside);
		  cond->crvar(ld.actvtabi,ld.vartab[vn]->leftside);
		  cond->crterm(EQUALL);
		  cond->popt();
		  AALLOS(wl ,struct wherelist);
		  wl->leftvarn = IFVARN;
		  wl->strateg =  NULL;
		  wl->whereterm = *cond;
		  wl->next = *whs;
		  *whs = wl;
	          ld.actvtabi++; }
		else 
		  t->fsymi = var_rename(vn);
                break;
  case TNORMFS:
                a= headarity();
                for(i=0; i<a; i++) {
                  if ((ch=t->subt[i].ren_vars_lin(check,r,whs))) 
		    return(ch); }
                break;
  default :     interr();
  }
  return(0);
}

void term::shift_vars(int delta)
{ int i;
  switch (t->infos) {
case TIDENT:  case TNUMBER: break;
case TSTRING:   break;
case TVAR:      
                t->fsymi=t->fsymi+delta;
                break;
case TNORMFS:   
                for(i=0; i<headarity(); i++) t->subt[i].shift_vars(delta);
                break;
default :     interr();
  }
}

int term::ren_vars(int check)
{ int i,a,rr,ch,vn;
switch (t->infos) {
case TIDENT:  case TNUMBER: break;
case TSTRING:   break;
case TVAR:      vn = t->fsymi;
                rr = var_was_renamed(vn);
                if ((check == CHECK && ! rr) || 
                    (check == ADDRENAME && rr && compile)) {
		  return 1; }
		// stout << "[" << t->fsymi << "->";
                t->fsymi = var_rename(vn);
		// stout << t->fsymi << "]";
                break;
case TNORMFS:
                a= headarity();
                for(i=0; i<a; i++) {
                  if ((ch=t->subt[i].ren_vars(check))) return(ch);
                }
                break;
default :     interr();
}
  return(0);
}

void term::write(ochstream &gout)
{ writewasident = 1;
  if (t->infos == TSTRING) gout << (char*)(t->subt); else  
  writerec(gout);
}

void term::writerec(ochstream &gout)
{ int i;
  lexem *p;
  switch (t->infos) {
  case TVAR:	gout << " VAR(" << t->fsymi 
		     // << "," << t->compif.varsort.typeval()
		     << ")";
		writewasident = 0;
		break;
  case TNUMBER:
    p = (fsymtab[NUMTOTERM].textform())->rside;
    goto write;
  case TSTRING:
	  gout << "\"" << (char*)(t->subt) << "\"";
	  break;
  case TIDENT: 
    p = (fsymtab[IDENTTOTERM].textform())->rside;
    goto write;
  case TNORMFS: 
      if (t->fsymi == DOUBLECONSTRUCT) {
	  gout << doubleval() ;
	  writewasident = 0;
	  break;
      }
      p = (fsymtab[t->fsymi].textform())->rside;
  write:
	  i=0;
	  while ( p->isnotendofstream()) {
	    if (p->nonterminal()) {
	      if ((*p)==internIdentType) {
		if (t->infos == TIDENT) {
		  gout << TERMWRITEBLANK() << tabofident.ide(t->fsymi);
		} else gout << "IDENT";
		writewasident = 1;
	      } else if ((*p)==internIntType) {
		if (t->infos == TNUMBER) 
		  gout << TERMWRITEBLANK() << (int) t->fsymi;
		else gout << "0000";
		writewasident = 0;
	      } else 
		t->subt[i++].writerec(gout);
	    } else  {
	      if (p->isident()) {
		gout << TERMWRITEBLANK() << p->alfsy();
		if (commands && displaylevel) {
		    gout << "_" << (int)(t->fsymi); }
		writewasident = 1;
	      } else if (!p->isblankk()) {
		writewasident = 0;
		gout << p->alfsy();
	      }
	    }
	    p++;
	  }
		break;
  default :     interr();

  }
}

int term::matchrec(term with,term *substarray)
{ int i,a,res;
  switch (t->infos) {
  case TIDENT:   case TNUMBER:	
		return(with.t->infos==t->infos && with.t->fsymi == t->fsymi);
  case TSTRING: return(with.t->infos==t->infos && (!strcmp(with.getstring(),getstring())));
  case TVAR:	if (substarray[t->fsymi].t!=NULL) {
		    return(substarray[t->fsymi].equal(with));
		}
		substarray[t->fsymi] = with; 
		return(1);
  case TNORMFS:
		a= headarity();
		res = (with.t->infos == TNORMFS && t->fsymi == with.t->fsymi);
		i=0;
  		while(i<a && res) {
    		  res = res && t->subt[i].matchrec(with.t->subt[i],substarray);
		  i++;
  		}
		return(res);
		break;
  default :     interr();
  }
  return 0;
}

int term::match(term with,term *substarray,int varnum,struct vilist *&iv)
{ term nullterm;
  struct vilist *vit;
  for(int i=0; i<varnum; i++) substarray[i]= nullterm;
  if (t->infos == TVAR) {
     with.topcopy(substarray[t->fsymi]);
     substarray[t->fsymi].incrcount();
     NNEW(vit,struct vilist);
     vit->tt = substarray[t->fsymi];  vit->next = iv;  iv = vit;
      return(1);
    } else {
      return(matchrec(with,substarray));
  }
}



void term::writetobuf(lbuffer &outb)
{ int i;
  lexem *p;
  lexem ll;
  if (t==NULL) return;
  switch (t->infos) {
  case TVAR:	sterr << "[term::writetobuf] internall error\n";
		failexit();
		break;
  case TNUMBER:
    p = (fsymtab[NUMTOTERM].textform())->rside;
    goto write;
  case TIDENT: 
    p = (fsymtab[IDENTTOTERM].textform())->rside;
    goto write;
  case TSTRING:    p = (fsymtab[STRINGTOTERM].textform())->rside;
    goto write;
  case TNORMFS:
	  p = (fsymtab[t->fsymi].textform())->rside;
  write:
	  i=0;
	  while ( p->isnotendofstream()) {
	    if (p->nonterminal()) {
	      if ((*p)==internIdentType) {
		if (t->infos == TIDENT) {
		  ll.cridlex(t->fsymi); outb.put(ll);
		} else outb.put(*p);
	      } else if ((*p)==internIntType) {
		if (t->infos == TNUMBER) {
		  ll.crnumlex(t->fsymi); outb.put(ll);
		} else outb.put(*p);
	      } else 
		t->subt[i++].writetobuf(outb);
	    } else  
	      outb.put(*p);
	    p++;
	  }
		break;
  default :     interr(); 
  }
}

void term::topdump()
{
  sterr << "at " << t << "   " 
    << "t[" << t->fsymi << "," << t->infos << "," << t->counter << "](...) ";
}

void term::consistency()
{ int i,a;
  a = headarity();
  for(i=0; i<a; i++) 
    t->subt[i].consistency();
}

void term::dumprec()
{ int i,a;
  stout << "[" 
        << "(" << this << "):"
        << "(" << this->t << ")="
	<< t->fsymi << "," << t->infos << "," 
        << t->counter 
	<< "[" << t->compif.ls.varnum << ","
	<< t->compif.ls.nofshares << ","
	<< t->compif.rs.sharetype << ","
	<< t->compif.rs.varnnum << ","
//	<< "<" << t->compif.rs.sharedterm << ","<< t->compif.varsort.typeval() <<   ">" 
	<< "]" 
//	<< "<" << this <<">]"
	<< "(\n";
  a = headarity();
  for(i=0; i<a; i++) {
    t->subt[i].dumprec();
    if (i+1<a) stout << ","; 
  }
  stout << ")";
}

int term::is_variable(int *index)
{
  if (t->infos == TVAR) { *index = t->fsymi; return 1; }
  else  return 0;
}

void term::dump()
{
  stout << "[term::dump]begin\n";
  dumprec();
  stout << "\n[term::dump]end\n";
}

int term::contains_AC()
{ int i,a; /* ,n; */
  switch (t->infos) {
  case TNORMFS:
    if (fsymtab[t->fsymi].infos() == FSASSOCCOM) return 1;
    a = headarity();
    for (i=0; i<a; i++) if (t->subt[i].contains_AC()) return 1;
    return 0;
  default :     return 0;
  }
}

/* ------------ functions for UNIF-interface (brrr) ------------- */

/*
static int unifncount(int n)
{ int i;
  i=1; n= n / 10;
  while (n>0) {n=n/10; i++;}
  return(i);
}

int term::unifscount()
{ int i,a,n;
  switch (t->infos) {
  case TIDENT:   case TNUMBER:	  case TVAR:	
		n = 1+unifncount(t->fsymi);
		break;
  case TSTRING: sterr << "\nTSTRING in unifcount not implemented yet\n"; interr();
  case TNORMFS:
	  a = headarity();
          if (a==0) n = 1+unifncount(t->fsymi);
          else {
            n=1+unifncount(t->fsymi)+2+a+1;
            if (fsymtab[t->fsymi].infos() == FSASSOCCOM) n+=2;
	    for (i=0; i<a; i++) n += t->subt[i].unifscount();
          }
		break;
  default :     interr();
  }
  return(n);
}

int term::uniftout(char *buff)
{ int i,a,n;
  switch (t->infos) {
  case TIDENT:  
                sprintf(buff,"b%d",t->fsymi);
		n = 1+unifncount(t->fsymi);
		break;
  case TSTRING: sterr << "\nTSTRING in uniftcount not implemented yet\n"; interr();
  case TNUMBER:
                sprintf(buff,"a%d",t->fsymi);
		n = 1+unifncount(t->fsymi);
		break;
  case TVAR:
                sprintf(buff,"V%d",t->fsymi);
		n = 1+unifncount(t->fsymi);
		break;
  case TNORMFS:
	  a = headarity();
          if (a==0) {
	    sprintf(buff,"c%d",t->fsymi);
	    n = 1+unifncount(t->fsymi);
          } else {
            n=3+unifncount(t->fsymi);       
            if (fsymtab[t->fsymi].infos() == FSASSOCCOM) {
              sprintf(buff,"F%d  \\(",t->fsymi); n+=2;
            } else sprintf(buff,"f%d\\(",t->fsymi);
	    for (i=0; i<a; i++) {
              n += t->subt[i].uniftout(buff+n);
              buff[n++] = ',';
            }
            buff[n-1]='\\'; buff[n++]=')';
          }
		break;
  default :     interr();
  }
  return(n);
}

*/

/*

void term::ekeruniftout(ochstream &f)
{ int i,a;
  switch (t->infos) {
  case TIDENT:  
                f << "b" << t->fsymi;
		break;
  case TSTRING: sterr << "\nTSTRING in ekeruniftout not implemented yet\n"; interr();
  case TNUMBER:
                f << "a" << t->fsymi;
 		break;
  case TVAR:
                f << "V" << t->fsymi;
		break;
  case TNORMFS:
	  a = headarity();
          if (a==0) {
            f << "c" << t->fsymi;
          } else {
            if (fsymtab[t->fsymi].infos() == FSASSOCCOM) {
                f << "F" << t->fsymi << "(";
//              fprintf(f,"F%d(",t->fsymi);
            } else {
                f << "f" << t->fsymi << "(";
//		fprintf(f,"f%d(",t->fsymi);
	    }
	    for (i=0; i<a; i++) {
              t->subt[i].ekeruniftout(f);
		f << ((i+1==a)?')':',');
//              fprintf(f,"%c",(i+1==a)?')':',');
            }
          }
		break;
  default :     interr();
  }
}

*/


/*
 * builtin de filtrage syntaxique
 * retourne le terme `fail' s'il n'y a pas de match
 * retourne le terme `nil' sil les deux termes sont identiques
 * retourne un terme (liste de termes) : listes d'instances
 * s'il y a une solution
 */

/*
 * stocke les variables dans tabRes
 * recupere la constante nil
 * retourne le nombre de variables
 */
#define MAX_SYNTACTICMATCHING_VAR 100

int term::storeVariable(int n, term *tabVar) {
  int a=headarity();
  if(a==0) {
    return n;
  } else if(a==2) {
    if(n>MAX_SYNTACTICMATCHING_VAR-1) {
	 fprintf(stderr,"term_storeVariable: increase MAX_SYNTACTICMATCHING_VAR=%d\n",MAX_SYNTACTICMATCHING_VAR); 
	 failexit(); 
    }
    tabVar[n]=t->subt[0];
    return t->subt[1].storeVariable(n+1,tabVar);
  } else {
    fprintf(stderr,"term_storeVariable: current term is not correct\n");  
    failexit();
  }
  return n;
}

void term::buildResult(term &dest,int n, term *tabRes) {
  term *st=NULL;
  if(headarity()==2) {
    NNEW(st ,term[2]);
    st[0]=tabRes[n];
    st[0].incrcount();
    t->subt[1].buildResult(st[1],n+1,tabRes);
    st[1].incrcount();
  }
  NNEW(dest.t ,struct hterm);
  statistic.incNumberOfAllocTerm();
    
  dest.t->compif.isVarExt=0;
  dest.t->fsymi = t->fsymi;
  dest.t->infos = t->infos;
  dest.t->counter = 0;
  dest.t->subt = st;
}

void term::buildMGUResult(term &dest,int n, 
			  int nbVar, term *tabVar, term *tabRes) {
term *st=NULL;
int  varnx;
term *deref_term;
 
  if(headarity()==2) {
    NNEW(st ,term[2]);
    if (tabRes[n].isvalidterm()) {
      deref_term = tabRes[n].derefVariable(nbVar,tabVar,tabRes,varnx);
      st[0]=*deref_term; }
    else
      st[0]=tabVar[n];
    st[0].incrcount();
    t->subt[1].buildMGUResult(st[1],n+1,nbVar,tabVar,tabRes);
    st[1].incrcount();
  }
  NNEW(dest.t ,struct hterm);
  statistic.incNumberOfAllocTerm();
  
  dest.t->compif.isVarExt=0;
  dest.t->fsymi = t->fsymi;
  dest.t->infos = t->infos;
  dest.t->counter = 0;
  dest.t->subt = st;
}


void term::syntacticMatching(int unify,term &dest,term subject, 
			     term listVar, term fail)
{
  class term tabVar[MAX_SYNTACTICMATCHING_VAR];
  class term tabRes[MAX_SYNTACTICMATCHING_VAR];

  /*struct term cons=listVar;*/
  int i,nbVar=0;

  /*
  stout << "syntacticMatching\n";
  stout << "pattern = "; this.write(stout); stout << "\n";
  stout << "subject = "; subject.write(stout); stout << "\n";
  stout << "listVar = "; listVar.write(stout); stout << "\n";
   */
  nbVar=listVar.storeVariable(0,tabVar);
  /*
  stout << "nbVar=" << nbVar << "\n";
  for(i=0; i<nbVar ; i++) {
    stout << "tabVar[" << i << "] = "; tabVar[i].write(stout); stout << "\n";
  } 
  */
  for(i=0; i<nbVar ; i++) {
    tabRes[i].t=NULL;
  }
  
  //  if(this.equalMatch(subject,nbVar,tabVar,tabRes)) {
  if ( (unify)?equalUnify(subject,nbVar,tabVar,tabRes):
               equalMatch(subject,nbVar,tabVar,tabRes)) {
    /* construction du terme resultat */
    /*
    stout << "One solution found\n";
    for(i=0; i<nbVar ; i++) { 
	 stout << "tabRes[" << i << "] = "; tabRes[i].write(stout); stout << "\n";
    }
    */
    if (unify)
      listVar.buildMGUResult(dest,0,nbVar,tabVar,tabRes);
    else
      listVar.buildResult(dest,0,tabRes);
  } else {
    /* no solution */
    //stout << "No solution\n";
    dest = fail;
    dest.incrcount();
  }
  //stout << "result = "; dest.write(stout); stout << "\n";
}

int term::indexVariable(int nbVar, term *tabVar) {
  for(int i=0 ; i<nbVar ; i++) {
    //    if(this.equal(tabVar[i]))
    if(equal(tabVar[i]))
	 return i;
  }
  return -1;
}

//------------------------------- UNIFY for builtinUnify

term *term::derefVariable(int nbVar, 
			  term *tabVar, term *tabRes,
			  int &varn)
{
  varn = indexVariable(nbVar,tabVar);
  if (varn != -1) {
    // write(stout); stout << "-DEREF VAR " << varn << "\n";

    if (tabRes[varn].isvalidterm()) {
      //stout << "^";
      return 
	tabRes[varn].derefVariable(nbVar,tabVar,tabRes,varn); }
    else
      return this;
  } else 
    return this;
}


int term::equalUnify(term with, int nbVar, term *tabVar, term *tabRes)
{
term *termx, *termy;
int  varnx, varny;
int  i,a,res;

  termx = this->derefVariable(nbVar,tabVar,tabRes,varnx);
  termy = with.derefVariable(nbVar,tabVar,tabRes,varny);
  /*
  stout << "this = "; this->write(stout) ; stout << "..."; 
  termx->write(stout) ; stout << "\n";
  stout << "with = "; with.write(stout) ; stout << "...";
  termy->write(stout) ; stout << "\n";
   */
  if (varnx != -1 && varny != -1) {
    if (varnx < varny) 
      tabRes[varnx]=*termy;
    else if (varny < varnx)
      tabRes[varny]=*termx;
    return(1); }
  else if (varnx != -1 && varny == -1) {
    tabRes[varnx]=*termy;
    return(1); }
  else if (varnx == -1 && varny != -1) {
    tabRes[varny]=*termx;
    return(1); }
  else if (varnx != -1 && varny != -1) {
    tabRes[varny]=*termx;
    return(1); }
  else if (varnx == -1 && varny == -1) {
    switch (termx->t->infos) {
    case TIDENT:   case TNUMBER:	
      return(termx->t->infos == termy->t->infos && 
	     termx->t->fsymi == termy->t->fsymi);
    case TSTRING: return(termx->t->infos == termy->t->infos && 
			 (!strcmp(termx->getstring(),termy->getstring())));
    case TNORMFS:
      res = (termy->t->infos == TNORMFS && 
	     termx->t->fsymi == termy->t->fsymi);
      for(i=0,a=termx->headarity(); i<a && res; i++) 
	res = res && 
	  termx->t->subt[i].equalUnify(termy->t->subt[i],
				       nbVar,tabVar, tabRes);
      return(res);
      break;
    default :     interr();
    }
  }
  return 0; /* to avoid warning */
}

//------------------------------------------------------------

int term::equalMatch(term with, int nbVar, term *tabVar, term *tabRes)
{
  int arity;
  int indexVar;
  int res;
  /*
  stout << "this = "; this.write(stout) ; stout << "\n";
  stout << "with = "; with.write(stout) ; stout << "\n";
  */
  switch (t->infos) {
  case TIDENT:   case TNUMBER:	
    return(with.t->infos==t->infos && with.t->fsymi == t->fsymi);
  case TSTRING:
    return(with.t->infos==t->infos && (!strcmp(with.getstring(),getstring())));
  case TVAR:
    sterr << "term::equalMatch Error\n";
    interr();
    break;
  case TNORMFS:
    indexVar = indexVariable(nbVar,tabVar);
    //stout << "indexVar=" << indexVar << "\n";
    if(indexVar>=0) {
	 if(tabRes[indexVar].t == NULL) {
	   tabRes[indexVar]=with;
	   return(1);
	 } else {
	   return tabRes[indexVar].equal(with);
	 }
    }
    res = (with.t->infos == TNORMFS && t->fsymi == with.t->fsymi);
    if(!res) return 0;
    arity=headarity();
    if(arity==0) return 1;
    for(int i=0 ; i<arity ; i++) {
	 if(! t->subt[i].equalMatch(with.t->subt[i],nbVar,tabVar,tabRes))
	   return (0);
    }
    break;
  default: 
    interr();
  }
  return 0; /* to avoid warning */
}

term *term::deref(term *substarray, int &varn)
{
  varn = -1;
  if (inf() == TVAR) {
    varn = head();
    // stout << "DEREF VAR " << varn << "\n";
    if (substarray[varn].isvalidterm()) {
      //stout << "^";
      return substarray[varn].deref(substarray, varn); }
    else
      return this;
  } else 
    return this;
}

//---------------------- Unification
// !!!!!!! NON AC !!!!!!!

int term::unifyrec(term with,term *substarray)
{ int i,a,res, varnx, varny;
  term *termx, *termy;
  termx = this->deref(substarray, varnx);
  termy = with.deref(substarray, varny);
  if (varnx != -1 && varny != -1) {
    if (varnx < varny) substarray[varnx] = *termy;
    else if (varny < varnx) substarray[varny] = *termx;
    // else, elle sont ==
    return(1); }
  else if (varnx != -1 && varny == -1) {
    substarray[varnx] = *termy; return(1); }
  else if (varnx == -1 && varny != -1) {
    substarray[varny] = *termx; return(1); }
  else if (varnx != -1 && varny != -1) {
    if (varnx < varny) {
      substarray[varnx] = *termy; }
    else {
      substarray[varny] = *termx; }
    return 1; }
  else if (varnx == -1 && varny == -1) {
    switch (termx->t->infos) {
      case TIDENT:   case TNUMBER:	
	return(termx->t->infos == termy->t->infos && 
		       termx->t->fsymi == termy->t->fsymi);
      case TSTRING: return(termx->t->infos == termy->t->infos && 
			   (!strcmp(termx->getstring(),termy->getstring())));
      case TNORMFS:
	res = (termy->t->infos == TNORMFS && 
	       termx->t->fsymi == termy->t->fsymi);
	for(i=0,a=termx->headarity(); i<a && res; i++) 
	  res = res && 
	    termx->t->subt[i].unifyrec(termy->t->subt[i],substarray);
	return(res);
	break;
      default :     interr();
    }
  }
  return 0; /* to avoid warning */
}

int term::unify(term with,term *substarray,int varnum,struct vilist *&iv)
{ term nullterm;
  int uni;
  struct vilist *vit;
  for(int i=0; i<varnum; i++) substarray[i]= nullterm;
  if (t->infos == TVAR) {
     with.topcopy(substarray[t->fsymi]);
     substarray[t->fsymi].incrcount();
     NNEW(vit,struct vilist);
     vit->tt = substarray[t->fsymi];  vit->next = iv;  iv = vit;
      return(1);
    } else {
      uni=unifyrec(with,substarray);
      return uni; }
}

//void term::writetostring(char *&strin)
void term::writetostring(char *strin)
{ writewasident = 1;
  if (t->infos == TSTRING) 
    strcat(strin,(char*)(t->subt)); 
  else  
    writerectostring(strin);
}

//void term::writerectostring(char *&strin)
void term::writerectostring(char *strin)
{ int i;
  lexem *p;
  char tempstr[STRLEN];
  switch (t->infos) {
  case TVAR:	
    snprintf(tempstr,sizeof(tempstr)," VAR(%d)",t->fsymi);
    strcat(strin,tempstr);
    writewasident = 0;
    break;
  case TNUMBER:
    p = (fsymtab[NUMTOTERM].textform())->rside;
    goto write;
  case TSTRING: 
    snprintf(tempstr,sizeof(tempstr),"\"%s\"",(char*)(t->subt));
    strcat(strin,tempstr);
    break;
  case TIDENT: 
    p = (fsymtab[IDENTTOTERM].textform())->rside;
    goto write;
  case TNORMFS: 
      p = (fsymtab[t->fsymi].textform())->rside;
  write:
	  i=0;
	  while ( p->isnotendofstream()) {
	    if (p->nonterminal()) {
	      if ((*p)==internIdentType) {
		if (t->infos == TIDENT) {
		  snprintf(tempstr,sizeof(tempstr),"%s%s",TERMWRITEBLANK(),tabofident.ide(t->fsymi));
		  strcat(strin,tempstr); 
		} else strcat(strin,"IDENT");
		writewasident = 1;
	      } else if ((*p)==internIntType) {
		if (t->infos == TNUMBER) {
		  snprintf(tempstr,sizeof(tempstr),"%s%d",TERMWRITEBLANK(),(int) t->fsymi);
		  strcat(strin,tempstr); }
		else strcat(strin,"0000");
		writewasident = 0;
	      } else 
		t->subt[i++].writerectostring(strin);
	    } else  {
	      if (p->isident()) {
		snprintf(tempstr,sizeof(tempstr),"%s%s",TERMWRITEBLANK(),p->alfsy());
		strcat(strin,tempstr);
		writewasident = 1;
	      } else if (!p->isblankk()) {
		writewasident = 0;
		snprintf(tempstr,sizeof(tempstr),"%s",p->alfsy());
		strcat(strin,tempstr);
	      }
	    }
	    p++;
	  }
	  break;
  default :     interr();

  }
}



// Built-in type and sort of a term (moved from termcompile.cc: used by the
// interpreter, not only by the C code generator).
int term::isofbuiltintype()
{ lexem ll;
  switch (t->infos) {
  case TIDENT:  case TNUMBER:
    return(t->infos);
  case TSTRING:
     return(t->infos);
  case TVAR:
    // stout << "VAR compif.varsor " <<t->compif.varsort.typeval() << "\n";
    return(ISBUILTIN((t->compif.varsort)));
  case TNORMFS:
    ll = fsymtab[t->fsymi].textform()->leftside;
    return(ISBUILTIN(ll));
  default :     
    interr();
  }
  return 0; /* to avoid warning */
}

lexem term::termtype()
{
  switch(t->infos) {
    case TIDENT: 
      sterr << "[termtype] INT.ERR.\n";
      interr();	
    case TNUMBER:
         return numtype;
  case TSTRING:
    sterr << "termtype for TSTRING is not implemented yet"; failexit();
    case TVAR:
         return((t->compif.varsort));
    case TNORMFS:
      return fsymtab[t->fsymi].textform()->leftside; 
    default:
      interr();
  }
  lexem ll;
  return ll; /* NULL; *//* to avoid warning */
}

