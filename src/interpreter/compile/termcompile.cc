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
#include "module.h"
#include "compiledefs.h"

// Base of the sharing score of searchbestshare: the former term depth
// limit MAXTERMDEEP (20000), kept so that the generated code is unchanged.
#define SHAREDEEPBASE 20000

#define ISSHARED(lt,rt) \
  (rt.t->compif.rs.sharetype != NOSHARE && \
   rt.t->compif.rs.sharedterm == lt.t\
  )
//                     consult this also with definition of buil-in functions


static FILE *rsff;


/* -----------------  compilation functions -------------------------*/


/*  firstly some functions to handle variables allocations    */
/* is used also for writting vars declarations (to not double declarations) */

static int varnnumbers[MAXRSGENVAR];

void allocVarInit()
{ int *p;
  for(p=varnnumbers;p<varnnumbers+MAXRSGENVAR;p++) *p=0;
}


int allocVarFirstFree()
{ int *p,rr;
  for(p=varnnumbers+1;*p;p++);
  rr = p-varnnumbers;
  if (rr >= MAXRSGENVAR-1) {
    fprintf(stderr,"[firstfreevar] termcompile.c number of generated variables exceeded MAXRSGENVAR == %d\n",MAXRSGENVAR); interr();
  }
  return(rr);
}

void allocVarSetUsed(int n)
{ varnnumbers[n]=1;
}

void allocVarSetUnused(int n)
{ varnnumbers[n]=0;
}
 
int allocVarIsUnused(int n)
{ return(varnnumbers[n]==0);
}



/* ----------------------------------------------------------------- */

void setrsff(FILE *ff)
{ rsff = ff;
}

void term::setcompilevar(int n)
{
  //stout << "\nSETCOMPILEVAR " << head() << "\n";
  t->compif.ls.varnum = n;
  t->compif.ls.nofshares = 0;
  t->compif.ls.sharedterm = NULL;
}

int term::isconstant()
{ 
  switch (t->infos) {
  case TIDENT: 	  case TNUMBER:	  case TVAR:
    return(1);
  case TSTRING:
    sterr << "isconstant for TSTRING is not implemented yet"; failexit();
  case TNORMFS: 
    return(headarity() == 0);
  default :     
    interr();
  }
  return 0; /* to avoid warning */
}


void term::markvarshares(term r,int intors)
{ int i,a;
  switch (r.t->infos) {
  case TIDENT: 	  case TNUMBER:	  
		break;
  case TSTRING:
    break;
  case TVAR:	if (t->fsymi == r.t->fsymi) {
                  if (intors) {
		    t->compif.ls.nofshares ++;
		    t->compif.ls.sharedterm = r.t;
		  }
		  r.t->compif.rs.sharedterm = t;
		  r.t->compif.rs.sharetype = PERFSHARE;
                }
		break;
  case TNORMFS: 
		a= r.headarity();
		for(i=0;i<a;i++) {
    		  markvarshares(r.t->subt[i],intors);
  		}
		break;
  default :     interr();
  }
}

void term::markpfshares(term r,int intors)
{ int i,a,fl;
  term rt,tt;
  switch (r.t->infos) {
  case TVAR:  break;
  case TSTRING:
    break;
  case TNORMFS: case TIDENT: 	  case TNUMBER:	
		                                  // yet shared ???
		if (r.t->compif.rs.sharetype == PERFSHARE) return;

                if (t->infos == r.t->infos && t->fsymi == r.t->fsymi) {   
		                                             // possible share
		  fl = 1; i=0; a = headarity();
		  while (i<a && fl) {
		    rt = r.t->subt[i]; tt = t->subt[i];
		    fl = fl && ISSHARED(tt,rt);
		    i++;
		  }
	          if (fl) {
		    if (intors) {
		      t->compif.ls.nofshares ++;
		      t->compif.ls.sharedterm = r.t;
		    }
		    r.t->compif.rs.sharedterm = t;
		    r.t->compif.rs.sharetype = PERFSHARE;
		    for (i=0;i<a;i++) {       // decrement the subterms shares
		      if (r.t->subt[i].t->compif.rs.sharetype == PERFSHARE) {
			if (intors) {t->subt[i].t->compif.ls.nofshares -- ;}
			if (t->subt[i].t->compif.ls.nofshares==0) 
			  r.t->subt[i].t->compif.rs.sharetype = NOSHARE;
		      }
		    }
		    break;
		  }
		}
		a= r.headarity();
		for(i=0;i<a;i++) {
    		  markpfshares(r.t->subt[i],intors);
  		}
		break;
  default :     interr();
  }
}

void term::marknoshares()
{ int i,a;
  switch (t->infos) {
  case TIDENT: 	  case TNUMBER:	  case TVAR:
		break; 
  case TSTRING:
    break;
  case TNORMFS: 
		a= headarity();
		for(i=a-1;i>=0;i--) {
    		  t->subt[i].marknoshares();
  		}
		break;
  default :     interr();
  }
  t->compif.rs.sharetype = NOSHARE;
  t->compif.rs.sharedterm = NULL;
}

void term::raffrsidevars2()
{ 
  int i,a;
  if (t->compif.rs.sharetype != PERFSHARE) {
    switch (t->infos) {
    case TSTRING:
      break;
    case TIDENT: 	  case TNUMBER:	  case TVAR:
      break;
    case TNORMFS: 
      a= headarity();
      for(i=a-1;i>=0;i--) {
	t->subt[i].raffrsidevars2();
      }
      if (t->compif.rs.sharetype == NOSHARE) {
	t->compif.rs.varnnum = allocVarFirstFree();  
	allocVarSetUsed(t->compif.rs.varnnum);
      }
      if (t->compif.rs.sharetype !=  RECURSIONSHARE &&
	  t->compif.rs.sharetype != ITERATIONSHARE) {
	for(i=a-1;i>=0;i--) {
	  if (t->subt[i].t->compif.rs.sharetype != PERFSHARE &&
	      t->subt[i].t->infos == TNORMFS) {
	    allocVarSetUnused(t->subt[i].t->compif.rs.varnnum);
	  }
	}
      }
      break;
    default :     interr();
    }
  }
}

void term::raffrsidevars1()
{
  int i,a;
  if (t->compif.rs.sharetype != PERFSHARE) {
    if (t->compif.rs.sharetype != NOSHARE) {
      t->compif.rs.varnnum = allocVarFirstFree();  
      allocVarSetUsed(t->compif.rs.varnnum);
    }
    switch (t->infos) {
    case TSTRING: break;
    case TIDENT: 	  case TNUMBER:	  case TVAR:
      break;
    case TNORMFS: 
      a= headarity();
      for(i=a-1;i>=0;i--) {
	t->subt[i].raffrsidevars1();
      }
      break;
    default :     interr();
    }
  }
}

void term::affrsidevars()
{ 
  allocVarInit();
  raffrsidevars1();
  raffrsidevars2();
}


void term::searchpfshares(term r,int intorside)
{ int i,a;
  switch (t->infos) {
  case TVAR:	markvarshares(r,intorside);
		break;
  case TSTRING: break;
  case TNORMFS: case TIDENT: 	  case TNUMBER:
                if (head()==TRUEVAL || head()==FALSEVAL) break;
		a= headarity();
		for(i=0;i<a;i++) {
    		  t->subt[i].searchpfshares(r,intorside);
  		}
		markpfshares(r,intorside);
		break;
  default :     interr();
  }
}

static int isbestshare;
static struct hterm *bshl, *bshr;
static int bestsharedeep;


void term::searchbestshare(term r)
{ int i,a,b,fl;
  term rt,tt;
  switch (r.t->infos) {
  case TSTRING:
    sterr << "searchbestshare for TSTRING is not implemented yet"; failexit();
  case TIDENT: 	  case TNUMBER:	case TVAR:  
		break;
  case TNORMFS: fl = 0;  a = headarity(); b=r.headarity();
		                                   // if it is perf share
		if (r.t->compif.rs.sharetype == PERFSHARE) return; 
                if (b && (a >= b) && ISCONSTRUCTOR(r.t->fsymi) &&
		    r.t->compif.rs.sharetype==NOSHARE) { // possible share
		  fl = SHAREDEEPBASE - bestsharedeep;
		  if (t->fsymi == r.t->fsymi) fl++;
		  for(i=0;i<b;i++) {
		    rt = r.t->subt[i]; tt = t->subt[i];
		    fl = fl + ISSHARED(tt,rt);
		  }
	          if (fl > isbestshare) {
		    bshl=t;  bshr=r.t; isbestshare = fl;
		  }
		}
		for(i=0;i<b;i++) {
    		  searchbestshare(r.t->subt[i]);
  		}
		break;
  default :     interr();
  }
}


void term::findbestshare(term r,int dep)
{ int i,a;
  switch (t->infos) {
  case TSTRING:
    sterr << "findbestshare for TSTRING is not implemented yet"; failexit();
  case TIDENT: 	  case TNUMBER:	  case TVAR:
		break;
  case TNORMFS: if (head()==TRUEVAL || head()==FALSEVAL) break;
		a= headarity();
		for(i=0;i<a;i++) {
    		  t->subt[i].findbestshare(r,dep+1);
  		}
		bestsharedeep = dep;
		if (a && t->compif.ls.nofshares==0) 
		  searchbestshare(r);
		break;
  default :     interr();
  }
}



void term::searchshares(term r,int sharetype)
{ int i,a;
  a = headarity();
  do {
    isbestshare = 0;
    for(i=0;i<a;i++) t->subt[i].findbestshare(r,0);
    if (isbestshare) {
      bshl->compif.ls.nofshares ++;
      bshl->compif.ls.sharedterm = bshr;
      bshr->compif.rs.sharedterm = bshl;
      bshr->compif.rs.sharetype = sharetype;
    }
  } while (isbestshare);
}

static int lastrecursionfounded=0;
static struct hterm *lastrecleftmainterm;

void term::rlastrecursion(unsigned fsym)
{ int i;
  switch (t->infos) {
  case TSTRING:
    sterr << "rlastrecursion for TSTRING is not implemented yet"; failexit();
  case TIDENT: 	  case TNUMBER:	  case TVAR:
		break;
  case TNORMFS: 
		if (t->fsymi == fsym) {
		  lastrecursionfounded =1;
		  t->compif.rs.sharetype = RECURSIONSHARE;
		  t->compif.rs.sharedterm = lastrecleftmainterm;
		  return;
		}
		if (ISCONSTRUCTOR(t->fsymi)) {
		  i = headarity()-1;
		  while((i>=0) && lastrecursionfounded==0) {
    		    t->subt[i].rlastrecursion(fsym);
		    i--;
  		  }
	        }
		break;
  default :     interr();
  }
}

int term::lastrecursion(term r)
{
  lastrecursionfounded=0; lastrecleftmainterm = t;
  r.rlastrecursion(head());
  if (r.t->infos == t->infos && r.t->fsymi == t->fsymi) {// iteration
    r.t->compif.rs.sharetype = ITERATIONSHARE;
    return(0);
  } else return(lastrecursionfounded);
}

/*
intend(FILE *ff,int i)
{ for(;i>0;i--) fprintf(ff,"  ");
}
*/

/* gen share of share affectation (if any) for sharing my 'shofshi' son */

void term::genShOfShAffectation(FILE *ff,int deep,int myindex)
{ term sht;
  int myvar;
  if (isItShareofshare(myindex,myvar,sht)) {
    if (t->compif.ls.sharedterm->compif.rs.sharetype != OTHERSTRSHARE ) {
      intend(ff,deep); 
      fprintf(ff,"sv%d->sub[%d]= ",myvar,myindex);
      sht.gencorrvalue();
      fprintf(ff,";\n");
    }
  }
}


/* is my i-th son share of my share ?                       */
/* if yes return my variable number, if not return 0        */

int term::isItShareofshare(int i,int &myvar, term &sharedson) 
{ struct hterm *rightside,*lsti,*rsti;
  term rst;
  int sharetyp,subsharetyp,a,ra;
  if (t->compif.ls.nofshares) {
    a=headarity();  
    rightside = t->compif.ls.sharedterm;
    rst.t= rightside; ra = rst.headarity();
    if (a>=i || ra>=i) return(0);
    myvar = rightside->compif.rs.varnnum;
    sharetyp = rightside->compif.rs.sharetype;
    if (sharetyp != FIRSTSTRSHARE && sharetyp != OTHERSTRSHARE ) return(0);
    lsti = t->subt[i].t;  sharedson = rightside->subt[i]; rsti = sharedson.t;
    subsharetyp = rsti->compif.rs.sharetype;
    if (subsharetyp == PERFSHARE) return(myvar);
    if (subsharetyp == FIRSTSTRSHARE || subsharetyp == OTHERSTRSHARE) {
      if (rsti->compif.rs.sharedterm == lsti) return(myvar);         // comparing pointers
      else return(0);
    }
    return(0);
  }
  return(0);
}




/*
  genfreeundershare(... meanings of args as in genfreeleft1 ... )
 
  - generate freeing of left hand side of rule under some constructor marked for share

*/

void term::genfreeundershare(FILE *ff,int deep,term father, int myindex)
{ int i,a,sha;
  term ttt;
  int sharetyp,vnum,shn,shvnum;
  struct hterm *rightside; /*,*lst,*rst;*/
  vnum = t->compif.ls.varnum;
  shn = t->compif.ls.nofshares;
  switch (t->infos) {
  case TVAR:
      if (shn && ! isofbuiltintype()) {
	  intend(ff,deep); fprintf(ff,"v%d->counter += %d;\n",vnum,shn);
      }
      father.genShOfShAffectation(ff,deep,myindex);
      break;
  case TSTRING:
    sterr << "genfreeundershare for TSTRING is not implemented yet"; failexit();
  case TNORMFS:   case TIDENT: 	  case TNUMBER:	  
    a= headarity();
    if (shn) {
      rightside = t->compif.ls.sharedterm;
      sharetyp = rightside->compif.rs.sharetype;
      if (a) {
	if (sharetyp == PERFSHARE) {
	  if (! isofbuiltintype()) {
	    intend(ff,deep); fprintf(ff,"v%d->counter += %d;\n",vnum,shn);
	  }
	} else if (sharetyp == FIRSTSTRSHARE) {
          if (shn != 1) {fprintf(stderr,"[termcompile.c]genfreeundershare"); interr();}
	  ttt.t=rightside; sha = ttt.headarity(); 
	  shvnum = rightside->compif.rs.varnnum;
	  intend(ff,deep);
	  fprintf(ff,"ALLOC(sv%d,term%d,f%dlist,%d);\n",shvnum,sha,sha,rightside->fsymi);
	  if (t->fsymi == rightside->fsymi) {       // I wanted to share funsym
	    intend(ff,deep);fprintf(ff,"sv%d->fs = %d;\n",shvnum,t->fsymi);
	  }
	} else if (sharetyp == OTHERSTRSHARE) {
	  /*
	   * Il y a un BUG ICI
	   * la regle [] f(f(x))=>f(x) fait planter le systeme
	   * on a sharetyp==ITERATIONSHARE
	   * OTHERSTRSHARE n'est jamais affecte
	   */
	  intend(ff,deep); fprintf(ff,"sv%d = NULL;\n",rightside->compif.rs.varnnum);
	} else {
	  sterr << "[genfreeundershare]termcompile.c unknown share type "
	        << sharetyp; 
dump();
ttt.t=rightside;ttt.dump();
	  interr();
	}
      }
      father.genShOfShAffectation(ff,deep,myindex);
    }
    for(i=0;i<a;i++) {
      t->subt[i].genfreeundershare(ff,deep,*this,i);
    }
    break;
  default :     interr();
  }
}


/*
   genfreeleft1(of,odeep,   
     - generate freeing of left hand side of rule
*/
void term::genfreeleft1(FILE *ff,int deep,term father, int myindex)
{ int i,a,shn,shtype;
  struct hterm *rightside;
  shn =  t->compif.ls.nofshares;
  rightside = t->compif.ls.sharedterm;
  switch (t->infos) {
  case TSTRING: break;
  case TIDENT: 	  case TNUMBER:
		break;
  case TVAR:
      if (shn) {
	if (rightside->compif.rs.sharetype != PERFSHARE) {
	  sterr<<"[genfreeleft1] termcompile ";
	  interr();
	}
	i = t->compif.ls.nofshares-1;
	if (i && (! isofbuiltintype())) {
          intend(ff,deep); fprintf(ff,"v%d->counter += %d;\n",t->compif.ls.varnum,i);
	}
      } else if (! isofbuiltintype()) {
        fprintf(ff,"/*%%type = %d*/\n",t->compif.varsort.typeval());
	intend(ff,deep); fprintf(ff,"freeterm( v%d);\n",t->compif.ls.varnum);
      }
      break;
  case TNORMFS: 
    a= headarity();
    if (a) {
      if (shn && rightside->compif.rs.sharetype == PERFSHARE) {
	i = t->compif.ls.nofshares-1;
	if (i && (! isofbuiltintype())) {
	  intend(ff,deep); 
	  fprintf(ff,"v%d->counter += %d;\n",t->compif.ls.varnum,i);
	}
        for(i=0;i<a;i++) {
	  t->subt[i].genfreeundershare(ff,deep+1,*this,i);
	}
      } else {
        intend(ff,deep); 
	                               // gen: can share structure ?
	fprintf(ff,"if (v%d->counter) {\n",t->compif.ls.varnum);
	                               // structure will not be shared
	intend(ff,deep+1);
        fprintf(ff,"v%d->counter --;\n",t->compif.ls.varnum);
	  genfreeundershare(ff,deep+1,father,myindex);
	intend(ff,deep); fprintf(ff,"} else {\n");
                                       // structure would be shared
	  shtype = shn?(rightside->compif.rs.sharetype):NOSHARE;
	  intend(ff,deep+1);
          if (shn && (shtype == FIRSTSTRSHARE || shtype == OTHERSTRSHARE)) {
	    fprintf(ff,"sv%d = v%d;\n",rightside->compif.rs.varnnum,t->compif.ls.varnum);
	  } else {
	    fprintf(ff,"FREE(v%d);\n",t->compif.ls.varnum);
	  }
          for(i=0;i<a;i++) {
            t->subt[i].genfreeleft1(ff,deep+1,*this,i);
          }
        intend(ff,deep); fprintf(ff,"}\n");
      }
    }
    break;
  default :     interr();
  }
}

void term::genfreeleft(FILE *ff,int deep)
{ int i,a;
  term ttt;
//fprintf(stderr,"[!!!!!!!-------GENFREELEFT---------!!!!!!!]\n");
//dump();
  if (t->infos == TVAR) {       // occurs when freeing where variables
    genfreeleft1(ff,deep,*this,-1);
    return;
  } else if (t->infos != TNORMFS) {
    fprintf(stderr,"[termcompile]genfreeleft "); interr();
  } 
  if (t->compif.ls.nofshares != 0) {
    fprintf(stderr,"[termcompile]genfreeleft something is wrong ");
//
//sterr << t->compif.ls.nofshares;
//
    fprintf(stderr,"(possible infinite loop ?) in :\n");
    write(sterr); sterr << " => "; 
    ttt.t = t->compif.ls.sharedterm; ttt.write(sterr);
    sterr << "\n\n";
    failexit();
  }
  a = headarity();
  for(i=0;i<a;i++) t->subt[i].genfreeleft1(ff,deep,*this,i);
}

static const char* genrsidedeclFirstString;

void term::rgenrsidedecl(FILE *ff,int deep)
{ int i,a;
  if (t->compif.rs.sharetype != PERFSHARE) {
      switch (t->infos) {
  case TSTRING: break;
      case TIDENT: 	  case TNUMBER:	  case TVAR:
	  break;
      case TNORMFS: 
	  a= headarity();
	  if (a && allocVarIsUnused(t->compif.rs.varnnum)) {
	      fprintf(ff,"%s*sv%d",genrsidedeclFirstString,t->compif.rs.varnnum);
	      if (t->compif.rs.varnnum == 0) {
		  fprintf(stderr,"[gencorrval]termcompile.c "); interr();
	      }
	      allocVarSetUsed(t->compif.rs.varnnum);
	      genrsidedeclFirstString = ",";
	  }
	  for(i=0;i<a;i++) {
	      t->subt[i].rgenrsidedecl(ff,deep);
	  }
	  break;
      default :     interr();
      }
  }
}


void term::genrsidedecl(FILE *ff,int deep)
{ 
  allocVarInit();
  intend(ff,deep); fprintf(ff,"{ ");
  genrsidedeclFirstString = "struct term ";
  rgenrsidedecl(ff,deep); 
  if (*genrsidedeclFirstString != 's') fprintf(ff,";");
  fprintf(ff,"\n");
}

static int wasrrecursion;
static struct hterm *leftwasrecterm;
static struct hterm *rightwasrecterm;
static int vaares,saares;
static int rsdeep;

//int term::isItShareofshare(int i)  // is it share of share at i-th subterm ??


void term::gencorrvalue()
{
  struct hterm *lefts;
  if (t->compif.rs.sharetype == PERFSHARE) {
    lefts = t->compif.rs.sharedterm;
    if (isofbuiltintype()) {
      fprintf(rsff,"v%d",lefts->compif.ls.varnum);
    } else {
      fprintf(rsff,"DD(v%d)",lefts->compif.ls.varnum);
    }
  } else {
    switch (t->infos) {
    case TSTRING:
      fprintf(rsff,"((struct term*)\"%s\")",getstring());
      break;
    case TIDENT: 	  case TNUMBER:
      if (Bins)
	fprintf(rsff,"((struct term*)(%d+%d+1))",t->fsymi,t->fsymi);
      else
	//fprintf(rsff,"((struct term*)%d)",t->fsymi);
	fprintf(rsff,"(setTag(%d))",t->fsymi);
      break;
    case TNORMFS:
      if (headarity()) {
	if (isofbuiltintype()) {
	  fprintf(rsff,"sv%d",t->compif.rs.varnnum);
	} else {
	  fprintf(rsff,"DD(sv%d)",t->compif.rs.varnnum);
	}
      } else if (t->fsymi<FSYMCODESBEG){ // builtins constants == code
	if (Bins) 
	  fprintf(rsff,"((struct term*)%d)",2*t->fsymi+1);
	else
	  fprintf(rsff,"(setTag(%d))",t->fsymi);
      }
      else if (ISCONSTRUCTOR(t->fsymi)) fprintf(rsff,"con%d",t->fsymi);
      else fprintf(rsff,"fun%d()",t->fsymi); //reductible constant
      break;
    case TVAR:default : sterr<<"[termcompile.c]gencorrvalue "; interr();
    }
  }  
}

void term::genSaveUnsavePF(FILE *ff,int deep)
{ int sht,i,a;
    rsff = ff;
    switch (t->infos) {
  case TSTRING: break;
    case TIDENT: 	  case TNUMBER:
		break;
    case TNORMFS:
		    sht = t->compif.rs.sharetype;
		    if (sht == PERFSHARE) {
		      if (isofbuiltintype()) {
		        sterr << 
			    "[termcompile.c]genSaveUnsave, share of buitinfs";interr(); }
		      intend(ff,deep); 
		      gencorrvalue();fprintf(ff,"->counter ++;\n");
		    } else {
		      a = headarity();
		      for(i=0;i<a;i++) {
		        t->subt[i].genSaveUnsavePF(ff,deep);
		      }
		    }
		break;
    case TVAR: 
		    sht = t->compif.rs.sharetype;
		    if (sht == PERFSHARE) {

    // stout << "VAR compif.varsor " <<t->compif.varsort.typeval() << "\n";
		    if (isofbuiltintype()) {
		       if (Bins) {
			 intend(ff,deep); 
			 fprintf(ff,"if (!isTagged(");
			 gencorrvalue();
			 fprintf(ff,"))"); 
			 gencorrvalue(); fprintf(ff,"-> counter ++ ;\n");
		       } 
		    } else {
		      intend(ff,deep); gencorrvalue();fprintf(ff,"-> counter ++;\n"); }
		    } else {
		      sterr << "[termcompile.c]genSaveUnsave, non shared var";interr(); }
		    break;
    default : sterr<<"[termcompile.c]genSaveUnsave "; interr();
    }
}



void term::handlesubaff(int share, int i)
{ term lefts,tsub,ttsub;
  struct hterm *sub;
  int vnum,vvnum;
  sub = t->subt[i].t;
  vnum = t->compif.rs.varnnum;
  if (sub->compif.rs.sharetype == RECURSIONSHARE) {
                                            // note future adress for result;
//    intend(rsff,rsdeep);
//    fprintf(rsff,"aares = &(sv%d->sub[%d]);\n",vnum,i);

    wasrrecursion =1;
    vaares = vnum; saares=i;
    leftwasrecterm = sub->compif.rs.sharedterm; 
    rightwasrecterm = sub;
    return;
  } else if (share) {
    lefts.t = t->compif.rs.sharedterm;
    if (lefts.isItShareofshare(i,vvnum,ttsub)) return;
  }
  intend(rsff,rsdeep); fprintf(rsff,"sv%d->sub[%d]=",vnum,i);
  tsub.t =sub; tsub.gencorrvalue(); 
  fprintf(rsff,";\n");
}


#define GENNOSHARED() {\
	  intend(rsff,rsdeep);\
	  fprintf(rsff,"ALLOC(sv%d,term%d,f%dlist,%d);\n",\
		t->compif.rs.varnnum,a,a,t->fsymi);\
	  intend(rsff,rsdeep);\
	  fprintf(rsff,"sv%d->fs = %d;\n",t->compif.rs.varnnum,t->fsymi);\
	  for (i=0; i<a; i++) handlesubaff(0,i);\
}

#define GENSHARED() {\
	  leftside = t->compif.rs.sharedterm;\
	  if (t->infos != leftside->infos || t->fsymi != leftside->fsymi) {\
	    intend(rsff,rsdeep);\
	    fprintf(rsff,"sv%d->fs = %d;\n",t->compif.rs.varnnum,t->fsymi);\
	  }\
	  for (i=0; i<a; i++) handlesubaff(1,i);\
}

void term::rgenrside()
{ 
  int i,a; /*,stv;*/
  struct hterm *leftside;
  if (t->compif.rs.sharetype != PERFSHARE) {
    switch (t->infos) {
    case TSTRING: break;
   case TIDENT:    case TNUMBER:    case TVAR:
     break;
    case TNORMFS:
      a= headarity();
      if (a) {
	for(i=a-1;i>=0;i--) {
	  t->subt[i].rgenrside();
	}
	//stout << t->fsymi << " is constructor " << ISCONSTRUCTOR(t->fsymi) << "\n";
	if (ISCONSTRUCTOR(t->fsymi)) {
	  if (t->compif.rs.sharetype == FIRSTSTRSHARE) {
	    GENSHARED();
	  } else if (t->compif.rs.sharetype == OTHERSTRSHARE) {
	    intend(rsff,rsdeep); fprintf(rsff,"if (sv%d==NULL) {\n",t->compif.rs.varnnum);
	    rsdeep++; GENNOSHARED(); rsdeep--;
	    intend(rsff,rsdeep); fprintf(rsff,"} else {\n");
	    rsdeep++; GENSHARED(); rsdeep--;
	    intend(rsff,rsdeep); fprintf(rsff,"}\n");
	  } else {                          // no share
	    GENNOSHARED();
	  }
	} else if (t->compif.rs.sharetype != RECURSIONSHARE &&
		   t->compif.rs.sharetype != ITERATIONSHARE && 
		   (! isconstant())){
	  intend(rsff,rsdeep);
	  fprintf(rsff,"sv%d = fun%d(",t->compif.rs.varnnum,t->fsymi);
	  for(i=0; i<a; i++) {
	    t->subt[i].gencorrvalue();
	    if (i<a-1) fprintf(rsff,",");
	  }
	  fprintf(rsff,");\n");
	}
      }
      break;
    default :     interr();
    }
  }
}

void term::genlstreccallvaraff(struct hterm *rightt)
{ int i,a,lvn;
  a = headarity();
  intend(rsff,rsdeep);fprintf(rsff,"{ struct term ");
  for(i=0;i<a;i++) fprintf(rsff,"*rv%d%c",i,(i+1==a)?';':','); 
  fprintf(rsff,"\n");
  intend(rsff,rsdeep);
  for(i=0;i<a;i++) {
    fprintf(rsff,"  rv%d = ",i);
    rightt->subt[i].gencorrvalue();
    fprintf(rsff,"; ");
  }
  fprintf(rsff,"\n");
  intend(rsff,rsdeep);
  for(i=0;i<a;i++) {
    lvn = t->subt[i].t->compif.ls.varnum;
    fprintf(rsff,"  v%d = rv%d; ",lvn,i);
  }
  fprintf(rsff,"\n");
  intend(rsff,rsdeep);fprintf(rsff,"}\n");
}


// !!!!!!!!!!!!!! the same macro is in matchcompile !!!!!!!!!!!!!!!
#define LEAVETRACEGEN(RESVAL) {\
  if (trace) {\
    intend(ff,deep); \
    fprintf(ff,"fprintf(stderr,\"[dump] evalquit } :: \");termwrite(");\
    RESVAL;fprintf(ff,",%d);fprintf(stderr,\"\176\");\n",isofbuiltintype());\
/* fprintf(ff,"termdump(");RESVAL;fprintf(ff,",%d);fprintf(stderr,\"\176\");\n",isofbuiltintype()); */ \
  }\
}


void term::genrside(FILE *ff,int deep,int isrec)
{ term ttt;
/*int i,a,lvn,rvn;*/
//fprintf(stderr,"[genride]\n");
  genbuildterm(ff,deep);
  if (t->compif.rs.sharetype == ITERATIONSHARE) {
    ttt.t = t->compif.rs.sharedterm; 
    ttt.genlstreccallvaraff(t);
    intend(ff,deep); fprintf(ff,"}goto beginlabel;\n");
  } else if (isrec==0) {
    LEAVETRACEGEN(gencorrvalue());
    intend(ff,deep); 
    fprintf(ff,"return("); gencorrvalue(); fprintf(ff,");}\n");
  } else {
    intend(ff,deep); 
    fprintf(ff,"*ares ="); gencorrvalue(); fprintf(ff,";\n");
    if (wasrrecursion) {
      intend(ff,deep); fprintf(ff," ares = &(sv%d->sub[%d]);\n",vaares,saares);
      ttt.t = leftwasrecterm;
      ttt.genlstreccallvaraff(rightwasrecterm);
      intend(ff,deep); fprintf(ff,"}goto beginlabel;\n");
    } else {
      LEAVETRACEGEN(fprintf(ff,"res"));
      intend(ff,deep); fprintf(ff,"}return(res);\n");
    }
  }
}


void term::genbuildterm(FILE *ff,int deep)
{
  wasrrecursion=0; rsff=ff; rsdeep=deep;
  rgenrside();
}
