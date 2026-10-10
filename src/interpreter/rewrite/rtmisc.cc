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

#include <string.h>
#include <errno.h>
#include <vector>
#include "commondefs.h"
#include "termdefs.h"
#include "module.h"
#include "rtdatas.h"
#include "meta.h"
#include "command.h"
extern int writewasident;
extern int no_more_switch;

struct rstackel {
  term t;
  int n;
};

// The stack of reduce() grows on demand (terms of any depth; MAXTERMDEEP =
// 20000 until S3b): rstack[0..rstacki] are in use, rstacki is an index.
#define INIT() {rstacki = -1;}
#define PUSH(tt,nn) {++rstacki; \
    if (rstacki >= (int)rstack.size()) rstack.resize(2 * rstack.size() + 1); \
    rstack[rstacki].t=(tt); rstack[rstacki].n=(nn);}
#define POP() {rstacki--;}
#define TOP() (rstack[rstacki])
#define TOP1() (rstack[rstacki-1])
#define EMPTY() (rstacki<0)
#define EMPTY1() (rstacki<=0)
#define INCRTOPI() {(rstack[rstacki].n)++;}

/*
static void stackdump()
{ int i;
  sterr << "[rtmisc.stackdump] start\n";
  for (i=0; i<=rstacki; i++) {
    sterr << i << ": [" << rstack[i].n << ",";
    rstack[i].t.write(sterr);
    sterr << "]\n";
  }
  sterr << "[rtmisc.stackdump] stop\n";
}
*/

locstatistics::locstatistics()
{ succNodes = totalNodes = wasSucces = tmpSucc = tmpTotal = 0;
  nonCountednodes = 1;
}

void locstatistics::fromThisWasSucces()
{
  wasSucces = 1; 
  totalNodes++;  succNodes++;                     // !!!!!!!!!!!!!!!!!!!
}

void locstatistics::backFrom(locstatistics *st)
{ 
  totalNodes += st->totalNodes;
  succNodes += st->succNodes;
  totalNodes += st->nonCountednodes;
  if (st->wasSucces) succNodes += st->nonCountednodes;
  wasSucces |= st->wasSucces;
}

/*
void locstatistics::topBackFrom(locstatistics *st)
{
  tmpSucc += st->succNodes; tmpTotal += st->totalNodes;
}
*/

void locstatistics::forwardFrom(locstatistics * /*st*/)
{
  nonCountednodes = 1;
}

void locstatistics::write()
{
  stout << "\n[] local statistics \n\tsuccesfull nodes: " << succNodes
        << "\n\ttotal nodes: "<<  totalNodes << "\n";
}

void locstatistics::dump()
{ sterr << "[locstatdump]  == ["<<succNodes<<","<<totalNodes<<","<<
           nonCountednodes<<","<<wasSucces<<","<<tmpSucc<<","<<tmpTotal<<"]\n";
}


void statistics::init()
{int i;
 transrules = transrtried = 0;
 total_num_of_cpoints = num_of_cpoints = 0; max_of_cpoints = 0; 
 total_num_of_mpoints = num_of_mpoints = 0; max_of_mpoints = 0;
 for(i=0; i<MAXNOFTRN; i++) rulesapp[i] = rulestried[i] = 0;
 for(i=0; i<FSYMTABSIZE; i++) { fsymtab[i].apply = 0;  fsymtab[i].ttry = 0; }
 numberOfAllocTerm=0;
 timestart();
}

void statistics::incNumberOfAllocTerm()
{
  numberOfAllocTerm++;
}

void statistics::dec_ncp()
{
  num_of_cpoints--;
}

void statistics::inc_ncp()
{
  total_num_of_cpoints++;
  num_of_cpoints++;
  if (num_of_cpoints > max_of_cpoints) max_of_cpoints = num_of_cpoints;
}

void statistics::dec_manyp()
{
 num_of_mpoints--;
}

void statistics::inc_manyp()
{
  total_num_of_mpoints++;
  num_of_mpoints++;
  if (num_of_mpoints > max_of_mpoints) max_of_mpoints = num_of_mpoints;
}

void statistics::timestart()
{
  getrusage(RUSAGE_SELF, &before_self);
  getrusage(RUSAGE_CHILDREN, &before_children);
}

void statistics::timestop()
{
  getrusage(RUSAGE_SELF, &after_self);
  getrusage(RUSAGE_CHILDREN, &after_children);
}


void statistics::transruleapp(int ri, term l)
{
  transrules ++;
  rulesapp[ri+1] ++;
  if (ri == -1) // no-named
    fsymtab[l.head()].apply++;
}

void statistics::transruletry(int ri, term l)
{
  transrtried ++;
  rulestried[ri+1] ++;
  if (ri == -1) // no-named
    fsymtab[l.head()].ttry++;
}

extern unsigned long cpt;

void statistics::write(int /*level*/,int big)
{
  int i; /*,j;*/
  int diff_time_self,diff_time_children;
  stout << "\n Statistics:\n";
//  fprintf(stdout," total %d rules applied, %d tried;",transrules,transrtried);
  diff_time_self = ((after_self.ru_utime.tv_sec*1000)
		    + (after_self.ru_utime.tv_usec/1000)) -
                   ((before_self.ru_utime.tv_sec*1000)
		    + (before_self.ru_utime.tv_usec/1000));
  diff_time_children = ((after_children.ru_utime.tv_sec*1000)
			+ (after_children.ru_utime.tv_usec/1000)) -
                       ((before_children.ru_utime.tv_sec*1000)
			+ (before_children.ru_utime.tv_usec/1000));

  fprintf(stdout," total time\t(%.3f+%.3f)=%.3f sec\t(main+subprocesses)\n",
	  ((double)(diff_time_self))/1000.0,
	  ((double)(diff_time_children))/1000.0,
	  ((double)(diff_time_self+diff_time_children))/1000.0 );

  if (diff_time_self > 0) {
    fprintf(stdout,"\n average speed = %ld inf/sec\n",(long)
	    ((1000.0*(double)transrules)/(double)diff_time_self)); }

  fprintf(stdout,"%12s%5d nonamed rules applied, %5d tried\n","",rulesapp[0],rulestried[0]);
  fprintf(stdout,"%12s%5d   named rules applied, %5d tried\n\n","",transrules-rulesapp[0],transrtried-rulestried[0]);

/*  
  for(j=1;j<=2;j++) 
      fprintf(stdout,"%21s%8s%8s%c","rule:","applied","tried",(!(j%2))?'\n':' ');
  for(i=1,j=1; i<MAXNOFTRN; i++) {
    if (trrules.rulename(i-1)!=NULL) {
      fprintf(stdout,"%21s%8d%8d%c",
	      trrules.rulename(i-1),rulesapp[i],rulestried[i],(!(j%2))?'\n':' ');
      j++;
    }
  }
  fprintf(stdout,"\n");
*/

  fprintf(stdout," named rules");
  fprintf(stdout,"\n%10s%8s%8s    %s\n","","applied","tried","rule for symbol");
  for(i=1; i<MAXNOFTRN; i++) {
    if (trrules.rulename(i-1)!=NULL && rulesapp[i]+rulestried[i] > 0) {
      fprintf(stdout,"%10s%8d%8d    %s\n","",
	      rulesapp[i],rulestried[i],trrules.rulename(i-1)); }
    }
  fprintf(stdout,"\n");

  if (big) {
    fprintf(stdout," nonamed rules");
    fprintf(stdout,"\n%10s%8s%8s    %s\n","","applied","tried","rule for symbol");
    for(i=0; i < FSYMTABSIZE; i++) 
      if (fsymtab[i].apply + fsymtab[i].ttry) {
	fprintf(stdout,"%10s%8d%8d    ","",fsymtab[i].apply,fsymtab[i].ttry); 
	out_symbol(stout,fsymtab[i].textform(),0);
	fprintf(stdout,"\n"); 
      }
    fprintf(stdout,"\n");
  }
  stout << "\n\tnumberOfAllocTerm = " << numberOfAllocTerm << "\n";
//  stout << "\n\tcpt = " << ((double)cpt)/(1024*1024) << "\n";
  fprintf(stdout,"\nend of statistics\n");

}

int integer_or_bool_constant(term *t)
{
  return(t->inf() == TNUMBER || 
	 (t->inf() == TNORMFS && 
	  (t->head() == TRUEVAL || t->head() == FALSEVAL)));
}

#define one_int_arg(X) \
       if (integer_or_bool_constant(t.subterm(0))) X; \
       else return 0; 
#define two_int_args(X) \
       if (integer_or_bool_constant(t.subterm(0)) && \
	   integer_or_bool_constant(t.subterm(1))) X; \
       else return 0; 


#include "files.cc"

void result(int constructor, int val, term &res) {
  res.stinit();
  res.crstterm(val,TNUMBER);
  res.crterm(constructor);
}

//void PID_result(int pid, term &res) { result(BUILTPIDCONSTR,pid,res); }
//void ERROR_result(int errn, term &res) { result(BUILTERRORPIPCONSTR ,errn,res); }
//void ERRORX_result(int errn, term &res) { result(BUILTERRORXCONSTR,errn,res); }

#define PID_result(pid,res) INT_result(pid,res)
#define ERROR_result(pid,res) INT_result(pid,res)
#define ERRORX_result(pid,res) INT_result(pid,res)

void INT_result(int num, term &res) { res.stinit(); res.crstterm(num,TNUMBER); }

int GET_LSTREAM(lstream *XXX, int /*pid*/, lexem rtype, term *res) 
{
  /*lexem le;*/
  if (topgrammar->earleycall(XXX,rtype,qendofin)){
    res->popt(); return 1; }
  else return 0; 
}

// Integer builtins on C int, with the results the interpreter always gave
// on two's complement machines (arm64), but defined: + - * and unary minus
// wrap modulo 2^32 (signed overflow was undefined behaviour), x/0 = 0,
// x%0 = x, INT_MIN/-1 = INT_MIN, INT_MIN%-1 = 0 (x/0 trapped on x86).
// tests/regression/int_arith_wraps.
// strcmp: -1, 0 or 1 (C only specifies the sign; tests/regression/strcmp_sign)
static int int_sign(int c) { return (c > 0) - (c < 0); }
static int int_add(int a, int b) { return (int)((unsigned)a + (unsigned)b); }
static int int_sub(int a, int b) { return (int)((unsigned)a - (unsigned)b); }
static int int_mul(int a, int b) { return (int)((unsigned)a * (unsigned)b); }
static int int_neg(int a)        { return (int)(0u - (unsigned)a); }
static int int_div(int a, int b)
{
  if (b == 0) return 0;
  if (b == -1) return int_neg(a);
  return a / b;
}
static int int_mod(int a, int b)
{
  if (b == 0) return a;
  if (b == -1) return 0;
  return a % b;
}

static int standardreduction(term &t)
{
  int pid,fstat,hd,nn; /* det, */
  lexem le;
  term tt,*rt; /* *pidt,trm, *tt1,*tt2,*/
  char *fname,*fkind, *termstring, *resstring, *typestring;
  struct processdata *pd;

// stout << "standard = ";  stout << t.semantic() << "\n"; t.write(stout); stout << t.semantic() << "\n";

  tt.stinit();
  switch ((nn=t.semantic())) {
      case PLUS :			//	PLUS
        two_int_args(
          tt.crstterm(int_add(t.subterm(0)->head(),t.subterm(1)->head()),TNUMBER))
          break;
      case MINUS :			//	MINUS
        two_int_args(
          tt.crstterm(int_sub(t.subterm(0)->head(),t.subterm(1)->head()),TNUMBER))
          break;
      case TIMES :			//	TIMES
        two_int_args(
          tt.crstterm(int_mul(t.subterm(0)->head(),t.subterm(1)->head()),TNUMBER))
          break;
      case DIV :			//	DIV
        two_int_args(
          tt.crstterm(int_div(t.subterm(0)->head(),t.subterm(1)->head()),TNUMBER))
          break;
      case MOD :
        two_int_args(
          tt.crstterm(int_mod(t.subterm(0)->head(),t.subterm(1)->head()),TNUMBER))
          break;
      case INTAND :
        two_int_args(
          tt.crstterm(t.subterm(0)->head() & t.subterm(1)->head(),TNUMBER))
          break;
      case INTOR :
        two_int_args(
          tt.crstterm(t.subterm(0)->head() | t.subterm(1)->head(),TNUMBER))
          break;
      case  INTEQUAL:
        two_int_args(
          tt.crterm((t.subterm(0)->head()==t.subterm(1)->head())?TRUEVAL:FALSEVAL))
          break;
      case  INTNONEQUAL:
        two_int_args(
          tt.crterm((t.subterm(0)->head()!=t.subterm(1)->head())?TRUEVAL:FALSEVAL))
          break;
      case  INTLESS:
        two_int_args(
          tt.crterm((t.subterm(0)->head()<t.subterm(1)->head())?TRUEVAL:FALSEVAL))
          break;
      case  INTLESSOREQ:
        two_int_args(
          tt.crterm((t.subterm(0)->head()<=t.subterm(1)->head())?TRUEVAL:FALSEVAL))
          break;
      case  INTGREATER:
        two_int_args(
          tt.crterm((t.subterm(0)->head()>t.subterm(1)->head())?TRUEVAL:FALSEVAL))
          break;
      case  INTGREATEROREQ:
        two_int_args(
          tt.crterm((t.subterm(0)->head()>=t.subterm(1)->head())?TRUEVAL:FALSEVAL))
          break;
      case UNMIN :
        one_int_arg(
          tt.crstterm(int_neg(t.subterm(0)->head()),TNUMBER))
          break;
      case BOOLAND :
	tt.crterm((istrueterm(*(t.subterm(0))) && istrueterm(*(t.subterm(1))))?TRUEVAL:FALSEVAL);
	break;
      case BOOLOR  :
	tt.crterm((istrueterm(*(t.subterm(0))) || istrueterm(*(t.subterm(1))))?TRUEVAL:FALSEVAL);
	break;
      case BOOLNOT :
	tt.crterm(istrueterm(*(t.subterm(0)))?FALSEVAL:TRUEVAL);
	break;
      case BOOLTOINT :
        fprintf(stderr,"[error] ( rtmisc.c) , BOOLTOINT\n");
        fprintf(stderr,"sorry not yet implemented in the interpreted version\n");
        interr();
        break;
      case INTTOBOOL :
        fprintf(stderr,"[error] ( rtmisc.c) , INTTOBOOL\n");
        fprintf(stderr,"sorry not yet implemented in the interpreted version\n");
        interr();
        break;
      case LESS :
	tt.crterm((t.subterm(0)->tcmp(* t.subterm(1))<0)?TRUEVAL:FALSEVAL);
	break;
      case LESSEQ :
	tt.crterm((t.subterm(0)->tcmp(* t.subterm(1))<=0)?TRUEVAL:FALSEVAL);
	break;
      case GREATER :
	tt.crterm((t.subterm(0)->tcmp(* t.subterm(1))>0)?TRUEVAL:FALSEVAL);
	break;
      case GREATEREQ :
	tt.crterm((t.subterm(0)->tcmp(* t.subterm(1))>=0)?TRUEVAL:FALSEVAL);
	break;
      case IDENTEQUAL:
	tt.crterm((t.subterm(0)->head()==t.subterm(1)->head())?TRUEVAL:FALSEVAL);
	break;
      case IDENTNONEQUAL:
	tt.crterm((t.subterm(0)->head()!=t.subterm(1)->head())?TRUEVAL:FALSEVAL);
	break;

      case NUMTODOUBLE1 :
      case NUMTODOUBLE2 :
      case NUMTODOUBLE3a :
      case NUMTODOUBLE3b :
      case NUMTODOUBLE4 :
      case NUMTODOUBLE5a :
      case NUMTODOUBLE5b :
        fprintf(stderr,"[rtmisc.c] double construction\n"); interr();
        break;
      case DOUBLEPLUS :
        tt.crdoublecon(t.subterm(0)->doubleval() + t.subterm(1)->doubleval());
        break;
      case DOUBLEMINUS :
        tt.crdoublecon(t.subterm(0)->doubleval() - t.subterm(1)->doubleval());
        break;
      case DOUBLEMULT :
        tt.crdoublecon(t.subterm(0)->doubleval() * t.subterm(1)->doubleval());
        break;
      case DOUBLEDIV :
        tt.crdoublecon(t.subterm(0)->doubleval() / t.subterm(1)->doubleval());
        break;
      case DOUBLEUNMIN :
        tt.crdoublecon(- t.subterm(0)->doubleval());
        break;
      case DOUBLEEQ :
        tt.crterm((t.subterm(0)->doubleval()==t.subterm(1)->doubleval())?TRUEVAL:FALSEVAL);
        break;
      case DOUBLENEQ :
        tt.crterm((t.subterm(0)->doubleval()!=t.subterm(1)->doubleval())?TRUEVAL:FALSEVAL);
        break;
      case DOUBLELESS :
        tt.crterm((t.subterm(0)->doubleval() <t.subterm(1)->doubleval())?TRUEVAL:FALSEVAL);
        break;
      case DOUBLELESSEQ :
        tt.crterm((t.subterm(0)->doubleval()<=t.subterm(1)->doubleval())?TRUEVAL:FALSEVAL);
        break;
      case DOUBLEGREATER :
        tt.crterm((t.subterm(0)->doubleval()>t.subterm(1)->doubleval())?TRUEVAL:FALSEVAL);
        break;
      case DOUBLEGREATEREQ :
        tt.crterm((t.subterm(0)->doubleval()>=t.subterm(1)->doubleval())?TRUEVAL:FALSEVAL);
        break;
      case DOUBLEEXP :
        tt.crdoublecon(exp(t.subterm(0)->doubleval()));
        break;
      case DOUBLELOG :
        tt.crdoublecon(log(t.subterm(0)->doubleval()));
        break;
      case DOUBLELOG10 :
        tt.crdoublecon(log10(t.subterm(0)->doubleval()));
        break;
      case DOUBLEPOW :
        tt.crdoublecon(pow(t.subterm(0)->doubleval(),t.subterm(1)->doubleval()));
        break;
      case DOUBLESQRT :
        tt.crdoublecon(sqrt(t.subterm(0)->doubleval()));
        break;
      case DOUBLESIN :
        tt.crdoublecon(sin(t.subterm(0)->doubleval()));
        break;
      case DOUBLECOS :
        tt.crdoublecon(cos(t.subterm(0)->doubleval()));
        break;
      case DOUBLETAN :
        tt.crdoublecon(tan(t.subterm(0)->doubleval()));
        break;
      case DOUBLEASIN :
        tt.crdoublecon(asin(t.subterm(0)->doubleval()));
        break;
      case DOUBLEACOS :
        tt.crdoublecon(acos(t.subterm(0)->doubleval()));
        break;
      case DOUBLEATAN :
        tt.crdoublecon(atan(t.subterm(0)->doubleval()));
        break;
      case DOUBLEEXP2 :
      case DOUBLEEXP10 :
      case DOUBLELOG2 :
      case INTTODOUBLE :
        fprintf(stderr,"[rtmisc.c] double arithmetics, sorry not yet impemented\n");
        fprintf(stderr,"\t in interpreted version\n");    interr();
        break;
/*
  case MAKEIDENT :			//	MAKEIDENT
  {
  lexem fl,fl1;
  char *colide,*cc;
  lbuffer buf;
  t.subterm(0)->writetobuf(buf);
  buf.get(fl); 
  cc = fl.alfsy();
  AALLOSS(colide ,strlen(cc)+1,char); strcpy(colide,cc);
  while (! buf.isempty()) {
  buf.get(fl);
  cc = addsuffix(colide,fl.alfsy());
  CFRE(colide); colide = cc;
  }
  fl.cridlex(colide);
  tt.stinit();
  tt.crstterm(fl.idval(),TIDENT);
  }
  break;
*/
      case REPLACE:                           // mapping of a subterm to another term
// Marian's bug -- it seems to me that stermreplace is not well designed
        if (t.subterm(2)->equal(*t.subterm(0))) {
          tt = *t.subterm(1);
          tt.incrcount();
        } else 
// Marian's bug -- up to herey
          if (! t.subterm(2)->stermreplace(1,* t.subterm(0),* t.subterm(1),tt)) {
            t.subterm(2)->topcopy(tt);
          }
	t.rewrite(tt);
	tt.tdelete();
	return(1);
      case OCCUR:				 // test of occuring of term in another
	tt.crterm(t.subterm(0)->occursin(1,*t.subterm(1))?TRUEVAL:FALSEVAL);
	break;
      case  EQUALL:
	tt.crterm(t.subterm(0)->equal(* t.subterm(1))?TRUEVAL:FALSEVAL);
	break;
      case  NEQUALL:
	tt.crterm(t.subterm(0)->equal(* t.subterm(1))?FALSEVAL:TRUEVAL);
	break;
      case  STRLENGTH:
        if (t.subterm(0)->inf() == TSTRING) {
          tt.crstterm(strlen(t.subterm(0)->getstring()),TNUMBER);
          break; }
        else return(0);
      case  STRINDEX:
        if (t.subterm(0)->inf() == TSTRING && t.subterm(1)->inf() == TNUMBER) {
          char *ss;
          int l1,l2;
          ss = t.subterm(0)->getstring();
          l1 = strlen(ss);
          l2 = t.subterm(1)->head();
          if (0 <= l2 && l2 < l1) {
            tt.crstterm(ss[l2],TNUMBER); break; }
          else return 0;}
        else return(0);
      case STRAPPEND:
        if  (t.subterm(0)->inf() == TSTRING &&  t.subterm(1)->inf() == TSTRING) {
          int l1, l2;
          char *ss;
          l1 = strlen(t.subterm(0)->getstring());
          l2 = strlen(t.subterm(1)->getstring());
          AALLOSS(ss ,l1+l2+1,char);
          ss[0] = 0;
          strcat(ss,t.subterm(0)->getstring());
          strcat(ss,t.subterm(1)->getstring());
          tt.crststring(ss); 
          break; }
        else return 0;
      case STRMODIF:               // STRMODIF       153
        if (t.subterm(0)->inf() == TSTRING && t.subterm(1)->inf() == TNUMBER &&
            t.subterm(2)->inf() == TNUMBER) {
          char *ss;
          int l1, l2, l3;
          ss = t.subterm(0)->getstring();
          l1 = strlen(ss);
          l2 = t.subterm(1)->head();
          l3 = t.subterm(2)->head();
          if (0 <= l2 && l2 < l1) {
            ss[l2]=l3;  ///////////////////////// ATTENTION - IT IS IN-PLACE
            tt.crststring(ss); break; }
          else return 0;}
        else return(0);
      case STRSUBSTR:
        if (t.subterm(0)->inf() == TSTRING && t.subterm(1)->inf() == TNUMBER &&
            t.subterm(2)->inf() == TNUMBER) {
          char *ss;
          int l1 = strlen(t.subterm(0)->getstring());
          int l2 = t.subterm(1)->head();
          int l3 = t.subterm(2)->head();
          if (!(0 <= l2 && l2 < l1 && 0 <= l3 && l2+l3 <= l1)) return 0;
          AALLOSS(ss ,l3+1,char);
          memcpy(ss,(char*)(t.subterm(0)->getstring())+l2,l3);
          ss[l3] = 0;
          tt.crststring(ss); 
          break; }
        else return 0;
      case STRSPN:        // #define STRSPN         156
        if (t.subterm(0)->inf() == TSTRING &&  t.subterm(1)->inf() == TSTRING) {
          tt.crstterm(strspn(t.subterm(0)->getstring(),t.subterm(1)->getstring() ),TNUMBER);
          break; }
        else return 0;
      case STRCMP:        // #define STRCMP         157
        if (t.subterm(0)->inf() == TSTRING &&  t.subterm(1)->inf() == TSTRING) {
          tt.crstterm(int_sign(strcmp(t.subterm(0)->getstring(),t.subterm(1)->getstring())),TNUMBER);
          break; }
        else return 0;
      case STRNG:          // 158
        if (t.subterm(0)->inf() == TNUMBER) {
          char *ss;
          AALLOSS(ss ,1+1,char);
          ss[0] = t.subterm(0)->head(); ss[1] = 0;
          tt.crststring(ss); break; }
        else return 0;
//----------------------------------------------------------
      case BUILTGETC:
        pid = (t.subterm(0))->head();
        if (pid < FOPEN_MAX) { 
          if (pid == STDOUT_FILENO || pid == STDERR_FILENO) {
            if (!batch) { sterr << "\n[IO-built] getc - cannot read from output \n"; sterr.flush(); }
            INT_result(9999,tt); break; }
          else if (pid == STDIN_FILENO || FILES[pid].status == STATROPEN) { 
            if (pid==STDIN_FILENO)
              INT_result(getc(stdin),tt);
            else
              INT_result(getc(FILES[pid].f),tt);
            break; }
          else {
            if (!batch) { sterr << "\n[IO-built] getc - file is not opened (for read)\n"; sterr.flush(); }
            INT_result(9999,tt); break; } }
        else { 
          if (!batch) { sterr << "\n[IO-built] getc - cannot read a char from a pipe\n"; sterr.flush(); }
          INT_result(9999,tt); break; } 
      case BUILTPUTC:
        pid = (t.subterm(0))->head();
        if (pid < FOPEN_MAX) { // should be a file
          if (pid == STDIN_FILENO) {
            if (!batch) { sterr << "\n[IO-built] putc - cannot write to input file\n"; sterr.flush(); }
            INT_result(9999,tt); break; }
          else if (pid == STDOUT_FILENO || pid == STDERR_FILENO) {
            hd = t.subterm(1)->head(); 
            fputc(hd,((pid==STDOUT_FILENO)?stdout:stderr));
            INT_result(hd,tt); break; }
          else if (FILES[pid].status == STATWOPEN || FILES[pid].status == STATAOPEN) {
            hd = t.subterm(1)->head();
            fputc(hd,FILES[pid].f);
            INT_result(hd,tt); break; }
          else {
            if (!batch) { sterr << "\n[IO-built] putc - this file is not opened (for write)\n"; sterr.flush(); }
            INT_result(9999,tt); break; } }
        else { 
          if (!batch) { sterr << "\n[IO-built] putc - internal error\n"; sterr.flush(); }
          INT_result(9999,tt); break; }
      case BUILTCREATE:   //stout << "\ncreating a process \n";
      case BUILTCREATENOBLOCK:   //stout << "\ncreating a process \n";
        fname = t.subterm(0)->getstring();
        pd = newprocess(fname,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,1,nn==BUILTCREATENOBLOCK);
        pd->counter = 0;
        if (pd != NULL) 
          PID_result(pd->pid,tt); 
        else {
          if (!batch) { sterr << "\n[IO-built] create - cannot create the process "<<fname<<"\n"; sterr.flush(); }
          ERROR_result(errno,tt);  }
        break;
      case BUILTOPEN: 
        pid = find_fid(); 
        if (!pid) {
          if (!batch) {
            sterr << "\n[strbuilt] open - too many opened files";
            sterr.flush();
          }
          ERROR_result(9999,tt); break;
        }
        fname = t.subterm(0)->getstring();
        fkind = t.subterm(1)->getstring();
        if (strstr(fkind,"r")) fstat = STATROPEN;
        else if (strstr(fkind,"w")) fstat = STATWOPEN;
        else if (strstr(fkind,"a")) fstat = STATAOPEN;
        else { 
          if (!batch) {
            sterr << "\n[IO-built] open - unknown file access during open\n";
            sterr.flush();
          }
          ERROR_result(9999,tt);
          break;
        }
        FILES[pid].nofreads = 0;
        if ((FILES[pid].f = fopen(fname,fkind)) != NULL) {
          if (fstat == STATROPEN) {
            ichstream *ich;
            NNEW(ich,ichstream(FILES[pid].f,fname));
            NNEW(FILES[pid].ls,lstream(ich));
          } else {
            NNEW(FILES[pid].och,ochstream(FILES[pid].f));
          }
          FILES[pid].status = fstat;
          PID_result(pid,tt);
            //stout << "fname = " << fname << " fkind = " << fkind << " pid = "  << pid << "\n";	
          break;
        } else {
          if (!batch) {
            sterr << "\n[IO-built] open - cannot open a file\n";
            sterr.flush();
          }
          ERROR_result(errno,tt);
          break;
        }
      case BUILTCLOSE:
        pid = (t.subterm(0))->head();
        if (pid < FOPEN_MAX) { // should be a file
          close_pid(pid);
          PID_result(pid,tt); 
          break; }
        else { // process
          pd = pid2processdata(pid);
          if (pd != NULL) {
              //???freeprocess(pd);
            kilproc(pd);
            PID_result(pid,tt); break; }
          else {
            if (!batch) { sterr << "\n[IO-built] kill process - internal error\n"; sterr.flush(); }
            ERROR_result(9999,tt); break; } }
      case BUILTWRITE:
        pid = (t.subterm(0))->head(); // 1x because of no @-coerc
        if (pid < FOPEN_MAX) { // should be a file
          if (pid == STDIN_FILENO) {
            if (!batch) { sterr << "\n[IO-built] write - cannot write to input file\n"; sterr.flush(); }
            ERRORX_result(9999,tt); break; }
          else if (pid == STDOUT_FILENO || pid == STDERR_FILENO) {
            rt = t.subterm(1); 
            rt->write(((pid==STDOUT_FILENO)?stout:sterr)); 
//      rt->dump();
            ((pid==STDOUT_FILENO)?stout:sterr).flush(); 
              //*** ???
            rt->incrcount();
            t.rewrite(*rt);
            rt->tdelete();
              //*** ???
              //.pc... t = *rt;
            return(1); }
          else if (FILES[pid].status == STATWOPEN || FILES[pid].status == STATAOPEN) {
            rt = t.subterm(1);
            rt->write(*(FILES[pid].och));
            (FILES[pid].och)->flush(); 
              //*** ???
            rt->incrcount();
            t.rewrite(*rt);
            rt->tdelete();
              //*** .pc.... t = *rt;
            return(1); }
          else {
            if (!batch) { sterr << "\n[IO-built] write - this file is not opened (for write)\n"; sterr.flush(); }
            ERRORX_result(9999,tt); break; } }
        else { // a process
          rt = t.subterm(1);
          if (write2process(pid,*rt)){ 
            rt->incrcount();
            t.rewrite(*rt);
            rt->tdelete();
            return(1); }
          else {
            if (!batch) { sterr << "\n[IO-built] write - internal error\n"; sterr.flush(); }
            ERRORX_result(9999,tt); break; } }
      case BUILTREAD:
        pid = (t.subterm(0))->head();
        NNEW(rt,term);

          //stout << "reading from a file " << pid << " a term of a type " 
          //    << t.head() << "\n";
    
        if (pid < FOPEN_MAX) { // should be a file
          if (pid == STDOUT_FILENO || pid == STDERR_FILENO) {
            if (!batch) {
              sterr << "\n[IO-built] read - cannot read from output \n"; sterr.flush();
            }
            ERRORX_result(9999,tt);
            break;
          } else if (pid == STDIN_FILENO || FILES[pid].status == STATROPEN) { 
            if (FILES[pid].nofreads) { // read 'end'
              ((pid==STDIN_FILENO)?mainstream:FILES[pid].ls)->ilex(le);
            }
            if (GET_LSTREAM(((pid==STDIN_FILENO)?mainstream:FILES[pid].ls),pid,
                            (fsymtab[t.head()].textform())->leftside,rt)) {
              FILES[pid].nofreads++;
              rt->incrcount();
              t.rewrite(*rt);
              rt->tdelete(); //t.incrcount();
              return 1;
            } else {
              if (!batch) { sterr << "\n[IO-built] read - cannot read a term \n"; sterr.flush(); }
              ERRORX_result(9999,tt); break;
            }
          } else {
            if (!batch) {
              sterr << "\n[IO-built] read - file is not opened (for read)\n"; sterr.flush();
            }
            ERRORX_result(9999,tt);
            break;
          }
        } else { // a process   
          pd = pid2processdata(pid);
          if (pd->noblocking && !pd->s->isready()) {
            if (!batch) { 
              sterr << "\n[IO-built] delayed read from a pipe\n";
              sterr.flush(); }
            ERRORX_result(9998,tt); break; }
          else {
            if (!(pd->counter)) {
              pd->counter = 1;
              pd->s->ilex(le);   //# This is an anachronism from M.V. code
              pd->s->ilex(le); } //#
            // S2: was FILES[pid].nofreads, out of bounds (pid is a process
            // id, >= FOPEN_MAX here); the count is now kept per process.
            if (pd->nofreads) pd->s->ilex(le);  // read 'end'
            if (GET_LSTREAM(pd->s,pid,(fsymtab[t.head()].textform())->leftside,rt)) {
              pd->nofreads++;
              rt->incrcount();
              t.rewrite(*rt);
              rt->tdelete(); //t.incrcount();
              return 1;
            } else {
              if (!batch) {
                sterr << "\n[IO-built] read - cannot read a term from a pipe\n"; sterr.flush();
              }
              ERRORX_result(9999,tt);
              break;
            }
	  }
        }
      case SYNTACTICMATCHING:
      case SYNTACTICUNIFICATION:
        t.subterm(0)->
          syntacticMatching((nn == SYNTACTICUNIFICATION),
                            tt,*t.subterm(1),*t.subterm(2),*t.subterm(3));
          /*
            stout << "t  = "; t.write(stout); stout << "\n";
            stout << "tt = "; tt.write(stout); stout << "\n";
            stout << "c'est fini !!!\n";
          */
        t.rewrite(tt);
        tt.tdelete();
          //    stout << "t  = "; t.write(stout); stout << "\n";
        return(1);
//------------------
      case REF2STRING:
          // refstr filename
          // refstring2string(@,@)	: (string string) string  	    code 160;	// ChR's prettyprint box

        termstring = t.subterm(0)->getstring();
        fname = t.subterm(1)->getstring();

          // shit - hopely temporary - because of ChR's shitty parser
        { char commandline[STRLEN];
          //   sprintf(commandline,"jolifier.sh %s",fname);
        snprintf(commandline,sizeof(commandline),"jolifieur.sh %s",fname);
        (void)!system(commandline);
        }

        pd = newprocess(REF2STRINGBOXNAME,fname,NULL,NULL,NULL,NULL,NULL,NULL,NULL,1,0);
        if (pd != NULL) 
          PID_result(pd->pid,tt); 
        else {
          if (!batch) { sterr << "\n[REF2STRING] create - cannot create the process "<< REF2STRINGBOXNAME <<"\n"; sterr.flush(); }
          ERROR_result(errno,tt);  }
        pd->counter = 0;
        *pd->pin << termstring << "\n"; 
        pd->pin->flush();

        DELETE1(pd->pin);

          //stout << "REF2STRING <<" << termstring << "\n";
        resstring = (pd->is)->readstring();
          //stout << "REF2STRING >> " << resstring << "\n";
        tt.crststring(resstring);
        t.rewrite(tt);
        tt.tdelete();
          //    stout << "t  = "; t.write(stout); stout << "\n";
        return(1);
      case STRING2REF: 
          // str    sort   filename
          // string2refstring(@':'@,@) : (string string string) string code 161;	// ChR's parser box

          //*** stout << " STRING2REFSTRING <<\n"; t.write(stout); stout << "\n";

        termstring = t.subterm(0)->getstring();
        typestring =  t.subterm(1)->getstring();
        fname = t.subterm(2)->getstring();

          // shit - hopely temporary - because of ChR's shitty parser
        { char commandline[STRLEN];
          //   sprintf(commandline,"jolifier.sh %s",fname);
        snprintf(commandline,sizeof(commandline),"jolifieur.sh %s",fname);
        (void)!system(commandline);
        }

        pd = newprocess(STRING2REFBOXNAME,fname,"-b",NULL,NULL,NULL,NULL,NULL,NULL,1,0);
        if (pd != NULL) 
          PID_result(pd->pid,tt); 
        else {
          if (!batch) { sterr << "\n[STRING2REF] create - cannot create the process "<< REF2STRINGBOXNAME <<"\n"; sterr.flush(); }
          ERROR_result(errno,tt);  }
        pd->counter = 0;
          //stout << " STRING2REF" << termstring << ":" << typestring << "\n";
        *pd->pin << typestring << "\n"; 
        *pd->pin << termstring << " end\n"; 
        pd->pin->flush();
        resstring = (pd->is)->readstring();

          //*** stout << "STRING2REFSTRING >> \n" << resstring << "\n";

        tt.crststring(resstring);
        t.rewrite(tt);
        tt.tdelete();
          //    stout << "t  = "; t.write(stout); stout << "\n";
        return(1);
//--------------
      case TERM2REFSTRING:
        tt.crststring(t.subterm(0)->term2refstring());
        t.rewrite(tt);
        tt.tdelete();
        return(1);
      case REFSTRING2TERM:
          //*** stout << "REFSTRING2TERM <<"; t.write(stout); stout << "\n";
        rt = refstring2term(t.subterm(0)->getstring());
        tt = *rt; 
        t.rewrite(tt); 
          //*** stout << "REFSTRING2TERM  >>"; t.write(stout); stout << "\n";
        tt.tdelete();
        return(1);
/*
  case TYPE2STRING:
  tt.crststring(typet.ide(((fsymtab[t.head()].textform())->
  rside[2]).typeval()));
  t.rewrite(tt); 
  tt.tdelete();
  return(1);
*/
      case IDENT2STRING: 
      {
	char BUFFER[STRLEN]; BUFFER[0] = 0;writewasident = 0;
	t.subterm(0)->writerectostring(BUFFER);
	tt.crststring(strdup(BUFFER));
	t.rewrite(tt); 
	tt.tdelete();
	return(1); }
      case SPEC2REF:
      { char commandline[STRLEN];

        //*** stout << "SPEC2REF <<"; t.write(stout); stout << "\n";

      snprintf(commandline,sizeof(commandline),"elan -b --export TMP.ref %s %s",
              t.subterm(0)->getstring(),
              t.subterm(1)->getstring());
      (void)!system(commandline);
      tt.crststring((char*)"TMP.ref"); tt.incrcount(); // S2: stored without copy, as in 2004
      t.rewrite(tt);
      tt.tdelete();

        //*** stout << "SPEC2REF  >>"; t.write(stout); stout << "\n";

      return(1); }
      case THISPROGRAM:
        tt.stinit(); 
        if (specname)
          tt.crststring(strdup(specname));
        else
          tt.crststring(strdup(""));
        if (modname)
          tt.crststring(strdup(modname));
        else
          tt.crststring(strdup(""));
        tt.crterm(SPECIFICATION,2);
        tt.popt();
          //*** stout << "THISPROGRAM  = "; tt.write(stout); stout << "\n";
        t.rewrite(tt); t.incrcount();
          //*** stout << "THISPROGRAM  = "; t.write(stout); stout << "\n";
        tt.tdelete();
        return(1);
      case TERM2STRING: {
        char BUFFER[50000]; BUFFER[0] = 0;
        t.subterm(0)->writetostring(BUFFER);
        tt.crststring(strdup(BUFFER));
        t.rewrite(tt); t.incrcount();
        tt.tdelete();
        return(1); }
      default: return(0);
  }
  tt.popt();
  t.rewrite(tt);
  tt.tdelete();
  return(1);
}


int handlecondition(term c, term *substarray)
{ term tt;
  int res;
  res = 1;
  if (! istrueterm(c)) {
//traceout << "[handlecondition] cond :";
//c.write(traceout);traceout<<"\n";
    c.copyinstall(false,tt,substarray);
    if (trace) {
       traceind+=6;
       indent(); traceout<<"condition: \n";
//       tt.write(traceout);traceout<<"\n";
    }
    reduce(tt,trace);
    if (trace) {
//      indent(); traceout<<"\t\trewrited to: ";
//      tt.write(traceout);traceout<<"\n";
      traceind-=6;
    }
    res = istrueterm(tt);
    tt.tdelete();
  }
  return(res);
}

void reduce(term &mt,int trace)		// main reduce loop; 
{
  std::vector<struct rstackel> rstack(1024);
  int rstacki;	// rstack[rstacki] == top
  term actt;
  int acti;
  int estrat_len = 0;
  int ith_strat = 0;
  int nred;
  int breaked = 0;

  if (trace) {
   traceind+=3;
   indent();traceout<<"[reduce] start:\n";
   indent();traceout<<"[0] ";mt.write(traceout);traceout<<"\n";
  }
  nred=0;
  INIT();
  PUSH(mt,0);
  while (! EMPTY()) {
    actt = TOP().t;
    acti = TOP().n;

   if (actt.inf() == TIDENT || actt.inf() == TNUMBER) {
       POP();
       continue; }

    if ((estrat_len = fsymtab[actt.head()].get_locstratlen())) {  

	// stout << "*** MYreduce " << fsymtab[actt.head()].get_locstratlen() << "\n";
	// stout << "*** reduce " << actt.head();
	// actt.write(stout); stout << "/" << acti << "\n";

	// there is an eval strategy
      if (acti >= estrat_len) { POP(); }
      else if (actt.inf() == TIDENT || actt.inf() == TNUMBER) {POP();
      } else if (
		 ((ith_strat=fsymtab[actt.head()].get_locstrat(acti)) > 0) &&
		          // estrat[acti] > 0 &&
		          // estrat[acti]-1 < fsymtab[actt.head()].arity()
		 ith_strat-1 < fsymtab[actt.head()].arity()
	  ) {
	  INCRTOPI();
	  PUSH((*actt.subterm( ith_strat-1)),0);
      } else if (fsymtab[actt.head()].infos()==FSASSOCCOM && (!EMPTY1()) 
		 && actt.head()==TOP1().t.head()){
	  POP();
      } else if (
		 fsymtab[actt.head()].get_locstrat(acti)== 0) {
	if (commands && fsymtab[actt.head()].breaked) { 
          breaked = 1;
          indent(); traceout << "[" << nred << "] ";
          actt.write(traceout); traceout << "\n";	}
	else breaked = 0;
	if (trrules.nnrewrited(actt) || standardreduction(actt)) {
	    nred++;
	if (commands && breaked) { 
	  breaked = 0;
          indent(); traceout << "[" << nred << "] ";
          actt.write(traceout); traceout << "\n";	}
	if (trace) {
	    indent(); traceout << "[" << nred << "] ";
	    rstack[0].t.write(traceout); traceout << "\n"; 
        }
	POP(); PUSH(actt,0);
	}  INCRTOPI();
      }
    } else { // no eval strategy
    if (actt.inf() == TIDENT || actt.inf() == TNUMBER) {POP();
    } else if (acti < fsymtab[actt.head()].arity()) {
      INCRTOPI();
      PUSH((*actt.subterm(acti)),0);
    } else if (fsymtab[actt.head()].infos()==FSASSOCCOM && (!EMPTY1()) 
               && actt.head()==TOP1().t.head()){
      POP();
    } else {
	if (commands && fsymtab[actt.head()].breaked) { 
          breaked = 1;
          indent(); traceout << "[" << nred << "] ";
          actt.write(traceout); traceout << "\n";	}
	else breaked = 0;
      if (trrules.nnrewrited(actt) || standardreduction(actt)) {
        nred++;
	if (commands && breaked) { 
	  breaked = 0;
          indent(); traceout << "[" << nred << "] ";
          actt.write(traceout); traceout << "\n";	}
	if (trace) {
           indent(); traceout << "[" << nred << "] ";
           rstack[0].t.write(traceout); traceout << "\n"; 
        }
        POP(); PUSH(actt,0);
      } else POP();
    }

   }
  }
  if (trace) {
    indent(); traceout << "[reduce] stop :\n"; 
    traceind-=3;
  }
}



void transred(int stratindex, term mt, int big)
{ term rt;
  strategy *p;
  if (stratindex == -1) 
    p = NULL;
  else
    p = trrules.getstrategy_refs(stratindex);
  if (!batch) { 
    traceout << "\n[] start with term :\n   "; 
    mt.write(traceout);
    traceout << "\n\n";
  }
   if (stratindex != -1 && p==NULL && warnings && (!batch)) {
    sterr << "[warning] main strategy was not defined \t!\n";
    sterr << "\t empty strategy used\n\n";
  }
  statistic.init();
  statistic.timestart();
    //statistic.timestop();
  reduce(mt,trace);
  stateofexecution execst(mt,p);
//execst.dump();
  while (execst.nextsolution(rt)) {
    statistic.timestop();
   if (commands) {
      term *axterm;
      NNEW(axterm ,term);
      *axterm = rt; 
      results.push(qresulttype,axterm); // do not delete rt, byt axterm
						  }
   if (commands && is_printterm) {
      term printt;
    if (!quiet) traceout << "\n\n";
    if (!batch) traceout << "\n[] print term:\n   ";
//execst.dump(10);
    printwith.copyinstall(false,printt,&rt);
    printt.write(traceout);
    printt.tdelete();
    if (!quiet) traceout << "\n\n\n";
    traceout << "\n"; 
    } else {
	if (!quiet) traceout << "\n\n";
	if (!batch) traceout << "\n[] result term:\n   ";
        //execst.dump(10);
	traceout.flush(); rt.write(stout); 
        if (batch) { /*WWW*/ stout << " end\n"; stout.flush(); }
	if (!quiet) traceout << "\n\n\n";
	traceout << "\n";
	if (!commands) rt.tdelete();
    }  ///////////// copy -- see bellow
  }
  if (!batch) traceout << "\n\n[] ";

//--- Marian had "fin" -- changed because of communication  

  if (no_more_switch) {
    if (batch) { /*WWW*/stout << " no_more end\n"; stout.flush(); }
    else { traceout << "end\n\n";   traceout.flush(); }        
  } else {
    if (batch) { /*WWW*/stout << " end end\n"; stout.flush(); }
    { traceout << "end\n\n";   traceout.flush(); } 
  }

//  if (!batch && statis) {
//  [pem: Apr 12 99]
  if(statis) {
    statistic.write(statis,big);
  }
  execst.free();
}

int istrueterm(term t)
{
  return(t.inf()==TNORMFS && t.head()==TRUEVAL);
}


