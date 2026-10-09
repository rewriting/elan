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

//------------------------------------------------NORMALISATION

#include "rtdatas.h"
#include "module.h"
#include "compiledefs.h"
#include "strategy.h"
#include <string.h>
#include "command.h"

#define NORM_RULE  "NORMALISE_RULE"
#define NORM_STRAT "LNORMAL_STRAT"
#define NSTRAT     "GNORMAL_STRAT"



//------------------------------------------------NORMALISATION1
//#define BLABLA

//trrules.getnnrules(s) == NULL && 

#define ISCONSTRUCT(s) (s==DOUBLECONSTRUCT || \
   ( fsymtab[s].get_semantic() == 0 && \
    fsymtab[s].isnotdefinedas()))
/*#define ISCONSTRUCT(s) (s==DOUBLECONSTRUCT || \
  (s>=FSYMCODESBEG && trrules.getnnrules(s) == NULL && fsymtab[s].isnotdefinedas())) */

int type_in_list(int typ, struct strlist *sl, strategy **strat)
{

  for(; sl; sl=sl->next) {
    if (typ == sl->str->typeofstr()) {
      *strat = sl->str;
      return 1; }
  }
  return 0;
}

strategy *gen_normalisation1(int norm_index,int modu, int sort, 
			     struct strlist *normlist)
{
  strategy  *nstrat, *lstrat; /* *substrat, */
 char *nrule, *norm_strat_name, *nstrat_name; 
 int nrulei, norm_strat_defs, norm_strat_refs, nstrat_defs, nstrat_refs;
 struct namelist *nm, *nplist;
 char tmp[STRLEN];
  int j, k, kmax, i, ii, typ;
  struct grammrulelist *gr;
  struct sgrammrule *actr;
  lexem nolexem, typle, whtypelex;
  int closure[NNONTERMINALS], work[NNONTERMINALS], constant[NNONTERMINALS];
  int norm_strat_refss[NNONTERMINALS]; 
  int nstrat_refss[NNONTERMINALS];
  strategy *nstrats[NNONTERMINALS];
  int fsym_map[MAXNFSYM];
  int something_new;
  strategy **whstr, *rep_strat = NULL, *normstrat;
  term xxx, yyy, *rlab1; /*, falseterm;*/
  transrule *rwrule;
  term lhs, rhs, *rlab, whvar;
  int lvars = 0; int ar,arity; /*,kk;*/
  int rvars = 0;
  struct strlist *slist, *sl;

  NNEW(rlab1,term);
  nolexem.crendofstreamlex();
  whtypelex.crendofstreamlex(); // S2: always set before use; silences -Wmaybe-uninitialized

  // compute transitive closure
  for(k=0; k< NNONTERMINALS; k++) { closure[k]=0; constant[k]=-1; }
  closure[sort] = 1; something_new = 1;
  // iterate
  while (something_new) {
    something_new = 0;
    for (j=0; j<NNONTERMINALS; j++) {
      for(gr = importglobgr[modu]->get_rule_list(j); gr!=NULL; gr = gr->next){
	actr = gr->r;
	if (actr->rulenumber >= FSYMCODESBEG && // QQQ HACK
	    ISCONSTRUCT(actr->rulenumber) &&
	    actr->leftside.nonterminal() && 
	    closure[actr->leftside.typeval()]) {
	  for(k=0;actr->rside[k] != nolexem;k++) {
	    if (actr->rside[k].nonterminal() && 
		closure[actr->rside[k].typeval()] == 0) {
	      closure[actr->rside[k].typeval()] = 1; something_new = 1; 
	    }
	  }
	}
      }
    }
  }
 // compute constant types
  for(i=0; i < NNONTERMINALS; i++) {
    if (closure[i]) {
      for(k=0; k< NNONTERMINALS; k++) work[k]=0;
      work[i] = 1; something_new = 1;
      // iterate
      while (something_new) {
	something_new = 0;
	for (j=0; j<NNONTERMINALS; j++) {
	  for(gr = importglobgr[modu]->get_rule_list(j); gr!=NULL; gr = gr->next){
	    actr = gr->r;
	    if (actr->rulenumber >= FSYMCODESBEG && // QQQ HACK
		ISCONSTRUCT(actr->rulenumber) &&
		actr->leftside.nonterminal() && 
		work[actr->leftside.typeval()]) {
	      for(k=0;actr->rside[k] != nolexem;k++) {
		if (actr->rside[k].nonterminal() && 
		    work[actr->rside[k].typeval()] == 0) {
		  work[actr->rside[k].typeval()] = 1; something_new = 1; 
		}
	      }
	    }
	  }
	}
      }
    constant[i] = 1;
    for(ii=0; ii < NNONTERMINALS; ii++)  
      if (type_in_list(ii, normlist, &normstrat)) {
	constant[i] &= (work[ii] == 0); }
    }
  }
//----------------------------------------------------------
  for(typ=0; typ< NNONTERMINALS; typ++) {
    if (closure[typ] && (!(constant[typ]))) {
     // construction of substrategy dc(NORM_RULE)

     slist = NULL;

     NNEW(nstrat, strategy); 
     //nstrat->setname(STRNAMEDONTCARE2,modu);
     nstrat->setname(STRNAMEONE2,modu);
     nstrat->settypeof(typ);
     snprintf(tmp,sizeof(tmp),"%s%d",NSTRAT,norm_index); /*,typ);*/
     nstrat_name = attach_type_mod(tmp,typ,modu);
     nstrat_defs = trrules.strategyindex_defs(nstrat_name,RGLOP);
     trrules.settypeofstrategy_defs(nstrat_defs,typ);
     if (trrules.setstrategy_defs(nstrat_defs,nstrat)) {
       if (!batch) sterr << "[warning] double definition of strategy " << nstrat_name << "\n"; } 
     nstrat_refs = trrules.strategyindex_refs(nstrat_name);
     trrules.assign_one_ref(1,nstrat_refs);
     nstrat_refss[typ] = nstrat_refs;
     nstrats[typ] = nstrat;

     if (typ == sort) rep_strat = nstrat;


     if (type_in_list(typ, normlist, &normstrat)) {
       snprintf(tmp,sizeof(tmp),"%s%d",NORM_STRAT,norm_index);
       norm_strat_name = attach_type_mod(tmp,typ,modu);
       norm_strat_defs = trrules.strategyindex_defs(norm_strat_name,RGLOP);
       trrules.settypeofstrategy_defs(norm_strat_defs,typ);
       if (trrules.setstrategy_defs(norm_strat_defs,normstrat)) {
	 if (!batch) sterr << "[warning] double definition of strategy " << norm_strat_name << "\n"; } 
       norm_strat_refs = trrules.strategyindex_refs(norm_strat_name);
       trrules.assign_one_ref(1,norm_strat_refs);
       norm_strat_refss[typ] = norm_strat_refs;
       //stout << typ << ":" << typet.ide(typ) << ":" << norm_strat_refss[typ] << "\n";
     }
    }
  }

  for (typ=0; typ<NNONTERMINALS; typ++) {
     if (closure[typ] && (!(constant[typ]))) {
       slist = NULL;

       if (lis_norm) {       // -------- LEFT_MOST_INNER_MOST
	 if (type_in_list(typ, normlist, &normstrat)) {
	 // M => M rule
	 snprintf(tmp,sizeof(tmp),"%s%d",NORM_RULE,norm_index); typle.crtypelex(typ);
	 nrule= attach_type(tmp,typ); nrulei = trrules.trruleindex(nrule);
	 xxx.stinit(); xxx.crvar(0,typle); xxx.popt();
	 yyy.stinit(); yyy.crvar(1,typle); yyy.popt();
	 rwrule = trrules.addrule(nrule,2,xxx,yyy,modu,RGLOP,
				  NULL,
				  NORMMATCH,*rlab1,NULL);  
	 whstr = trrules.getstrategyadr_refs(norm_strat_refss[typ]);
	 rwrule->addwhere(reverse_wheres,1,whstr,xxx,typle);
	 // default rule
	 NNEW(lstrat, strategy); 
	 // lstrat->setname(STRNAMEDONTCARE,modu);
	 lstrat->setname(STRNAMEONE,modu);
         NNEW(nm, struct namelist); nm->next = NULL; nm->strname = nrulei;
         AALLOS(sl,struct strlist);sl->next = slist;sl->str = lstrat;slist = sl;
         lstrat->setnamelist(nm); lstrat->settypeof(typ);
       }
       }                        // -------- LEFT_MOST_INNER_MOST
       for (ii=0; ii < MAXNFSYM; ii++) fsym_map[ii] = 0;
       for (gr = importglobgr[modu]->get_rule_list(typ);gr!=NULL;gr=gr->next) {
	 actr = gr->r;
	 if (actr->rulenumber >= FSYMCODESBEG && 
	     ISCONSTRUCT(actr->rulenumber) && (!fsym_map[actr->rulenumber]) &&
	     closure[actr->leftside.typeval()] && (!(constant[typ]))) {
	   lvars = 0; rvars = 0;

	   for(arity=0,k=0;actr->rside[k] != nolexem;k++)
	     if (actr->rside[k].nonterminal()) arity++;
	   kmax = k;

	   nplist = NULL;

	   fsym_map[actr->rulenumber] = 1;

	   for(ar=0; ar < arity; ar++) {
	     for(lvars=0,k=kmax-1;k>=0;k--) 
	       if (actr->rside[k].nonterminal()) {
		 if (ar == lvars) whtypelex = actr->rside[k]; 
		 lvars++; }
	     
	     if (constant[whtypelex.typeval()]) continue;
	     
	     lhs.stinit();
	     for(lvars=0,k=kmax-1;k>=0;k--) {
	       if (actr->rside[k].nonterminal()) 
		 lhs.crvar(lvars++,actr->rside[k]); }
	     lhs.crterm(actr->rulenumber);
	     lhs.popt();
	     //lhs.write(stout);  
	    
	     rhs.stinit();
	     for(rvars=0,k=kmax-1;k>=0;k--) {
	       if (actr->rside[k].nonterminal()) {
		 if (ar == rvars) 
		   rhs.crvar(arity,whtypelex); 
		 else
		   rhs.crvar(rvars,actr->rside[k]); 
		 rvars++; }
	     }
	     rhs.crterm(actr->rulenumber);
	     rhs.popt();
	     //rhs.write(stout);  

	     NNEW(rlab,term);
	     snprintf(tmp,sizeof(tmp),"%s%d_%d",NORM_RULE,norm_index,actr->rulenumber);
	     nrule= attach_type(tmp,typ); nrulei = trrules.trruleindex(nrule);
	     rwrule = trrules.addrule(nrule,arity+1,lhs,rhs,modu,RGLOP,
				      NULL,
				      NORMMATCH,*rlab,NULL);  
	     whvar.stinit(); whvar.crvar(ar,whtypelex); whvar.popt();
	     whstr = trrules.getstrategyadr_refs
	       (nstrat_refss[whtypelex.typeval()]);
	     rwrule->addwhere(reverse_wheres,arity,whstr,whvar,whtypelex); 
	     if (!nplist) {
	       NNEW(nm, struct namelist); nm->next = nplist; 
	       nm->strname = nrulei;
	       nplist= nm; }
	   }
	   if (nplist) {
	     NNEW(lstrat, strategy); 
	     // lstrat->setname(STRNAMEDONTCARE,modu);
	     lstrat->setname(STRNAMEONE,modu);
	     AALLOS(sl,struct strlist);sl->next = slist;sl->str = lstrat;
	     slist = sl;
	     lstrat->setnamelist(nplist); lstrat->settypeof(typ); }
	 }
       }
       if (!lis_norm)    {       //---------#ifdef LEFT_MOST_OUTER_MOST
	 if (type_in_list(typ, normlist, &normstrat)) {
	 // M => M rule
	 snprintf(tmp,sizeof(tmp),"%s%d",NORM_RULE,norm_index); typle.crtypelex(typ);
	 nrule= attach_type(tmp,typ); nrulei = trrules.trruleindex(nrule);
	 xxx.stinit(); xxx.crvar(0,typle); xxx.popt();
	 yyy.stinit(); yyy.crvar(1,typle); yyy.popt();
	 rwrule = trrules.addrule(nrule,2,xxx,yyy,modu,RGLOP,
				  NULL,
				  NORMMATCH,*rlab1,NULL);  
	 whstr = trrules.getstrategyadr_refs(norm_strat_refss[typ]);
	 rwrule->addwhere(reverse_wheres,1,whstr,xxx,typle);
	 // default rule
	 NNEW(lstrat, strategy); 
//	 lstrat->setname(STRNAMEDONTCARE,modu);
	 lstrat->setname(STRNAMEONE,modu);
         NNEW(nm, struct namelist); nm->next = NULL; nm->strname = nrulei;
         AALLOS(sl,struct strlist);sl->next = slist;sl->str = lstrat;slist = sl;
         lstrat->setnamelist(nm); lstrat->settypeof(typ);
       }
      }                            //---------#ifdef LEFT_MOST_OUTER_MOST     
       nstrats[typ]->setstl(slist);
     }
  }


  return rep_strat;
}
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================
//==============================================================

struct namelist *gen_normalisation2(int norm_index,int sort, strategy *normstr)
{int modu = normstr->getmodule();
 strategy  *nstrat; /* *substrat, */
 char *nrule, *norm_strat_name, *nstrat_name; 
  int nrulei, norm_strat_defs, norm_strat_refs, nstrat_defs, nstrat_refs;
 struct namelist *nm;
 char tmp[STRLEN];
 int j, k,  i, typ; /* exist, */
  struct grammrulelist *gr;
  struct sgrammrule *actr;
  lexem nolexem, typle;
  int closure[NNONTERMINALS], work[NNONTERMINALS], constant[NNONTERMINALS];
  int norm_strat_refss[NNONTERMINALS]; int nstrat_refss[NNONTERMINALS];
  int something_new;
  strategy **whstr;
  term xxx, yyy, *rlab1;
  transrule *rwrule;

  NNEW(rlab1,term);
  nolexem.crendofstreamlex();

  // compute transitive closure
  for(k=0; k< NNONTERMINALS; k++) { closure[k]=0; constant[k]=-1; }
  closure[sort] = 1; something_new = 1;
  // iterate
  while (something_new) {
    something_new = 0;
    for (j=0; j<NNONTERMINALS; j++) {
      for(gr = importglobgr[modu]->get_rule_list(j); gr!=NULL; gr = gr->next){
	actr = gr->r;
	if (actr->rulenumber >= FSYMCODESBEG && // QQQ HACK
	    actr->leftside.nonterminal() && 
	    closure[actr->leftside.typeval()]) {
	  for(k=0;actr->rside[k] != nolexem;k++) {
	    if (actr->rside[k].nonterminal() && 
		closure[actr->rside[k].typeval()] == 0) {
	      closure[actr->rside[k].typeval()] = 1; something_new = 1; 
	    }
	  }
	}
      }
    }
  }
 // compute constant types
  for(i=0; i < NNONTERMINALS; i++) {
    if (closure[i]) {
      for(k=0; k< NNONTERMINALS; k++) work[k]=0;
      work[i] = 1; something_new = 1;
      // iterate
      while (something_new) {
	something_new = 0;
	for (j=0; j<NNONTERMINALS; j++) {
	  for(gr = importglobgr[modu]->get_rule_list(j); gr!=NULL; gr = gr->next){
	    actr = gr->r;
	    if (actr->rulenumber >= FSYMCODESBEG && // QQQ HACK
		actr->leftside.nonterminal() && 
		work[actr->leftside.typeval()]) {
	      for(k=0;actr->rside[k] != nolexem;k++) {
		if (actr->rside[k].nonterminal() && 
		    work[actr->rside[k].typeval()] == 0) {
		  work[actr->rside[k].typeval()] = 1; something_new = 1; 
		}
	      }
	    }
	  }
	}
      }
    constant[i] = (work[sort] == 0);
    }
  }
//----------------------------------------------------------
  for(typ=0; typ< NNONTERMINALS; typ++) {
    if (closure[typ]) {
     // name of norm rule
     snprintf(tmp,sizeof(tmp),"%s%d",NORM_RULE,norm_index);
     nrule= attach_type(tmp,typ); nrulei = trrules.trruleindex(nrule);

     // construction of substrategy dc(NORM_RULE)
     NNEW(nstrat, strategy); nstrat->setname(STRNAMEDONTCARE,modu);
     NNEW(nm, struct namelist); nm->next = NULL; nm->strname = nrulei;
     nstrat->setnamelist(nm); nstrat->settypeof(typ);
     snprintf(tmp,sizeof(tmp),"%s%d",NSTRAT,norm_index);
     nstrat_name = attach_type_mod(tmp,typ,modu);
     nstrat_defs = trrules.strategyindex_defs(nstrat_name,RGLOP);
     trrules.settypeofstrategy_defs(nstrat_defs,typ);
     if (trrules.setstrategy_defs(nstrat_defs,nstrat)) {
       if (!batch) sterr << "[warning] double definition of strategy\n"; } 
     nstrat_refs = trrules.strategyindex_refs(nstrat_name);
     trrules.assign_one_ref(1,nstrat_refs);
     nstrat_refss[typ] = nstrat_refs;
     snprintf(tmp,sizeof(tmp),"%s%d",NORM_STRAT,norm_index);
     norm_strat_name = attach_type_mod(tmp,typ,modu);
     norm_strat_defs = trrules.strategyindex_defs(norm_strat_name,RGLOP);
     trrules.settypeofstrategy_defs(norm_strat_defs,typ);
     if (trrules.setstrategy_defs(norm_strat_defs,normstr)) {
       if (!batch) sterr << "[warning] double definition of strategy\n"; } 
     norm_strat_refs = trrules.strategyindex_refs(norm_strat_name);
     trrules.assign_one_ref(1,norm_strat_refs);
     norm_strat_refss[typ] = norm_strat_refs;
    }
  }
  for (j=0; j<NNONTERMINALS; j++) 
    if (closure[j]) {
      for (gr = importglobgr[modu]->get_rule_list(j);gr!=NULL;gr=gr->next) {
	actr = gr->r;
	if (actr->rulenumber >= FSYMCODESBEG && 
	    closure[actr->leftside.typeval()]) {
	  term lhs, rhs, resvar, cond, resvar1, *rlab;
	  int lvars = 0; int resvari;
	  int rvars = 0;
	  int var_assignments[MAXNOFVAR], var_types[MAXNOFVAR];
	  int var_assignmentsi = 0;
	  lhs.stinit();
          for(k=0,k=0;actr->rside[k] != nolexem;k++) {  // reverse 
	    if (actr->rside[k].nonterminal()) 
	      lhs.crvar(lvars++,actr->rside[k]); }
	  lhs.crterm(actr->rulenumber);
	  lhs.popt();
	  //lhs.write(stout);

	  rhs.stinit();
	  for(k=0;actr->rside[k] != nolexem;k++) {  //reverse !
	    if (actr->rside[k].nonterminal()) {
	      if (closure[actr->rside[k].typeval()]) {
		var_assignments[rvars] = lvars+var_assignmentsi; 
		var_types[rvars] = actr->rside[k].typeval();
		var_assignmentsi++; }
	      else 
		var_assignments[rvars] = rvars;
	      rhs.crvar(var_assignments[rvars++],actr->rside[k]); }
	  }
	  rhs.crterm(actr->rulenumber);
	  rhs.popt();
	  //rhs.write(stout);
	
	  resvari = lvars+var_assignmentsi;
	  resvar.stinit();
	  resvar.crvar(resvari,actr->leftside);
	  resvar.popt();

	  resvar1.stinit();
	  resvar1.crvar(resvari+1,actr->leftside);
	  resvar1.popt();

	  //-- addrule
          NNEW(rlab,term);

	  snprintf(tmp,sizeof(tmp),"%s%d",NORM_RULE,norm_index);
	  nrule= attach_type(tmp,actr->leftside.typeval()); 
	  rwrule = trrules.addrule(nrule,resvari+2,lhs,resvar1,modu,RGLOP,
				   NULL,
				   NORMMATCH,*rlab,NULL);  
	  whstr = trrules.getstrategyadr_refs
	    (nstrat_refss[actr->leftside.typeval()]);
	  rwrule->addwhere(0,resvari+1,whstr,resvar,actr->leftside);
	  cond.stinit();
	  cond.crvar(resvari,actr->leftside);
	  cond.pusht(lhs);
	  cond.crterm(NEQUALL);
	  cond.popt();
	  rwrule->addwhere(0,IFVARN,NULL,cond,booltype);

	  if (constant[actr->leftside.typeval()] == 0)
	    whstr = trrules.getstrategyadr_refs
	      (norm_strat_refss[actr->leftside.typeval()]);
	  else
	    whstr = (strategy**)NULL;
	  rwrule->addwhere(0,resvari,whstr,rhs,actr->leftside);
	  for(k=0; k<rvars; k++)
	    if (var_assignments[k] != k) {
	      term *vterm;
	      NNEW(vterm,term);
	      vterm->stinit();
	      typle.crtypelex(var_types[k]); vterm->crvar(k,typle);
	      vterm->popt();
	      if (0==constant[var_types[k]])
		whstr = trrules.getstrategyadr_refs
		  (nstrat_refss[var_types[k]]);
	      else
		whstr = (strategy**)NULL;
	      rwrule->addwhere(0,var_assignments[k],whstr,*vterm,typle);
	    }
	  rwrule->dump(0);
	}
      }
      if (!(constant[j])) {
	// M => M rule
        snprintf(tmp,sizeof(tmp),"%s%d",NORM_RULE,norm_index); typle.crtypelex(j);
	nrule= attach_type(tmp,j); 
	xxx.stinit(); xxx.crvar(0,typle); xxx.popt();
	yyy.stinit(); yyy.crvar(0,typle); yyy.popt();
	rwrule = trrules.addrule(nrule,1,xxx,yyy,modu,RGLOP,
				 NULL,
				 NORMMATCH,*rlab1,NULL);  
	rwrule->dump(0); }
    }
  
   // name of norm rule
   NNEW(nm, struct namelist); 
   snprintf(tmp,sizeof(tmp),"%s%d",NORM_RULE,norm_index);
   nm->next = NULL; nm->strname = trrules.trruleindex(attach_type(tmp,sort));
   return nm;
}


