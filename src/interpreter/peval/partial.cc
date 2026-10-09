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

//#define DEBUG2
//#define BLABLA
//#define DEBUG3

#include "rtdatas.h"
#include "module.h"
#include "compiledefs.h"
#include "strategy.h"
#include <string.h>
#include "command.h"

int rule_was_added = 0;
int phase = 0;
int partcount = 0;
int partial_index = 0;
int extend_index = 0;
#define PARTSTR "PEVAL"
#define PEVALSTRING "__"
#define is_false_term(x) (x.inf() == TNORMFS && x.head() == FALSEVAL)

int nofalse(struct wherelist *wh)
{
  for(;wh; wh=wh->next) {
    if (wh->leftvarn == IFVARN &&
	is_false_term(wh->whereterm)) return 0; }
  return 1;
}

int transrule::no_false()
{
  return nofalse(wheres);
}

extern int is_instance(term *subt, term *t);
extern int peval_loop_test;

int rule_comes_from[MAXNOFTRN];
termset rule_redexes[MAXNOFTRN];

//----------------------------------------------------------------------
void Patterms::init()
{
int i;
  //stout << "patterms_init\n";

  for(i=0; i < MAXNOFPATTERNS; i++) {
    Patterm[i].terms_to_compare.empty();
    Patterm[i].terms_compared.empty();
  }
  Pattermsi = 0;
}

void Patterms::empty()
{
  init(); // ????
}

void Patterms::copy(Patterms *p)
{
int i;
  p->Pattermsi = Pattermsi;
  for(i=0; i < Pattermsi; i++) {
    p->where_to_replace[i] = where_to_replace[i];
    Patterm[i].terms_to_compare.copy(p->Patterm[i].terms_to_compare);
    Patterm[i].terms_compared.copy(p->Patterm[i].terms_compared); }
}

void Patterms::add(int /*redex*/, term *t, /*int varn, */ int typ)
{
  /*int varn = t->varnumbers()+1;*/

  //stout << "patterms_add redex=" << redex << " "; t->write(stout); stout << "::" << typet.ide(typ) << "\n";
  if (Pattermsi >= MAXNOFPATTERNS) {
    sterr << "\n[peval] too many patterns, more than MAXNOFPATTERNS="
          << MAXNOFPATTERNS << "\n\t fatal\n";
    failexit(); }
  Patterm[Pattermsi].terms_to_compare.single(t,/*varn,*/typ);
  Patterm[Pattermsi].terms_compared.empty(); 
  where_to_replace[Pattermsi] = t;
  // stout << "where_to_replace["<<Pattermsi<<"]" << where_to_replace[Pattermsi] << "\n";
  Pattermsi++;
}

void Patterms::dump()
{
int i;
  for(i=0; i < Pattermsi; i++) {
    stout << i << "-th term (" << Pattermsi << ")\n";
    Patterm[i].terms_to_compare.dump(); stout << "::";
    Patterm[i].terms_compared.dump(); 
  }
}

int ith_subtype(int ith, int typ, int /*module*/, int fsym)
{
struct grammrulelist *gr;
struct sgrammrule *actr;
 int k,p; /* ,modu;*/
lexem nolexem;
nolexem.crendofstreamlex();
  // stout << ith << "," << module << ";" << fsym << "\n";
  //stout << ith << "," << import.ide(module) << ";" << fsym << "type=" << typ << " " << typet.ide(typ) << "\n";

  //importglobgr[module]->dump(); failexit();

// !!! a reflechir
// !!!

//  for(import.forinit(); import.forcond(); import.fornext()) {
//   modu = import.forindex();

   //stout << ith << "," << import.ide(modu) << ":::::" << fsym << "\n";
   //if (importglobgr[modu]) importglobgr[modu]->dump();

   //stout << importglobgr[modu] << "\n";
   //stout << importglobgr[modu]->get_rule_list(typ) << "\n";

//   if (importglobgr[modu]) {
//     for(gr = importglobgr[modu]->get_rule_list(typ); gr; gr = gr->next){
  {
     for(gr = globtermgr.get_rule_list(typ); gr; gr = gr->next){

     actr = gr->r;
     if (actr->rulenumber == fsym) {
       //stout << "GRAMRULE \n"; dumpgrrule(actr);
       for(p=0,k=0;actr->rside[k] != nolexem;k++) {
	 if (actr->rside[k].nonterminal()) {
	   if (p == ith) return actr->rside[k].typeval();
	   else p++; }
       } 
     }
//    }
    }
}
  sterr << ith << "-th subsort has not been found\n"; 
  //stout << ith << "," << import.ide(module) << ";" << fsym << "\n";
  globtermgr.dump();
  failexit(); 
  return -1; /* to avoid warning */
}

void Patterms::compare(int module)
{
int i, j, varn, varn1, common_fsym, all_commons, common_typ, orig_typ, 
    fsym_reduced, all_valid, arity_fsym, common_size, typ, typ1, fsymi,
    j_size, all_equal;
char new_fsym[STRLEN];
lexem le;
term *tt, *tt1, *ct;
struct sgrammrule *gr;

 if (!batch) { stout << "patterms_compare " << module << "\n"; dump(); }
//stout << " Pattermsi = " << Pattermsi << "\n";

  orig_typ = -1; fsym_reduced = 0;
  if (Pattermsi <= 1) return; // sterr << "[int.err.] compare -- not enough patterns\n"; return; 
  for(;;) {
    //dump();
    all_valid = 1;
    for(i=0; i<Pattermsi; i++) {
      if (Patterm[i].terms_to_compare.size() > 0) {
	Patterm[i].terms_to_compare.ith_term(0, &tt, &varn,&typ);
        all_valid &= tt->isvalidterm(); }
      else
	all_valid = false;
    }
    // stout << "ALL_VALID = " << all_valid << "\n";

    if (!all_valid) break;

    //  common_fsym, common_typ
    common_fsym = 0; all_commons = 1; common_typ = -1;
    for(i=0; i<Pattermsi; i++) {
      Patterm[i].terms_to_compare.ith_term(0, &tt, &varn, &typ);

      if (common_typ  == -1) common_typ = typ;
      else {
	if (common_typ != typ) all_commons = 0; }
      if (tt->inf() == TNORMFS) {
	if (common_fsym) all_commons &= (common_fsym == tt->head());
	else common_fsym = tt->head(); }
      else all_commons = 0;
    }
 
   // stout << "ALL_COMMONS = " << all_commons << " fsym = " << common_fsym << " typ = " << common_typ << 
   // "ISFUNCONSTRUCTOR1(common_fsym)=" << ISFUNCONSTRUCTOR1(common_fsym)<< trrules.getnnrules(common_fsym) << fsymtab[common_fsym].get_semantic() << "\n";

    if (orig_typ == -1) orig_typ = common_typ;

// 3105    if (all_commons && ISFUNCONSTRUCTOR(common_fsym)) { // decompose to to_be_compared
    if (all_commons && ISFUNCONSTRUCTOR1(common_fsym)) { // decompose to to_be_compared

      arity_fsym = fsymtab[common_fsym].arity(); fsym_reduced++;
      for(i=0; i<Pattermsi; i++) {
	Patterm[i].terms_to_compare.ith_term(0, &tt, &varn, &typ);
	Patterm[i].terms_to_compare.remove_first();
	for(j=0; j < arity_fsym; j++) {
          //stout << " i-th TYP " << tt->subterm(j) << "\n";
	  Patterm[i].terms_to_compare.
	    addterm(false, tt->subterm(j),/*varn,*/
		    ith_subtype(j,typ,module,common_fsym)); 
	} } }
    else { // move to compared
     //........... are they equal with
      j_size = Patterm[0].terms_compared.size();
      for(j=0; j<j_size; j++) {
	for(i=0, all_equal = 1; i < Pattermsi; i++) {
	  Patterm[i].terms_compared.ith_term(j, &tt, &varn, &typ);
	  Patterm[i].terms_to_compare.ith_term(0, &tt1, &varn1, &typ1);
          all_equal &= tt->equal_nground(*tt1);
	}
	if (all_equal) {
	  if (!batch) stout << "ALLEQUAL-ALLEQUAL-ALLEQUAL-ALLEQUAL-ALLEQUAL-ALLEQUAL-ALLEQUAL-ALLEQUAL-ALLEQUAL-ALLEQUAL-ALLEQUAL-ALLEQUAL\n";
	  for(i=0; i<Pattermsi; i++) {
	    Patterm[i].terms_compared.ith_term(j, &tt, &varn, &typ); tt->write(stout); stout << "=?=";
	    Patterm[i].terms_to_compare.ith_term(0, &tt1, &varn1, &typ1); tt1->write(stout); stout << "\n";

	    Patterm[i].terms_to_compare.remove_first(); }
	  goto finito;}
      }
     //...........
     for(i=0; i<Pattermsi; i++) {
       Patterm[i].terms_to_compare.ith_term(0, &tt, &varn, &typ);
       Patterm[i].terms_to_compare.remove_first();
       Patterm[i].terms_compared.addterm(false,tt,/*varn,*/typ); }
    finito:; 
    }
  }  // end of for(;;)

  if (orig_typ != -1) {
    if (!batch){
      stout << "Results to be collected \n";
      for(i=0; i<Pattermsi; i++) {
	Patterm[i].terms_compared.dump(); } }
    common_size = -1;
    for(i=0; i<Pattermsi; i++) {
      j = Patterm[i].terms_compared.size();
      if (common_size == -1) common_size = j;
      else if (j != common_size) {
	sterr << "[int.err.] common size error\n"; failexit(); }
    }
    if (!batch) stout << "common size " << common_size << import.ide(module) << "\n";
    // create new fsymbol
    fsymi = fsymtabi;
    if (fsym_reduced > 1) {
      if (common_size) snprintf(new_fsym,sizeof(new_fsym),"FOO_%d(",fsymi);
      else snprintf(new_fsym,sizeof(new_fsym),"FOO_%d",fsymi);
      le.cridlex(new_fsym); globtermgr.addsymbol(le); //importglobgr[module]->addsymbol(le);
      for(j=0; j < common_size; j++) {
	Patterm[0].terms_compared.ith_term(j,&tt,&varn, &typ);
	le.crtypelex(typ); globtermgr.addsymbol(le); //importglobgr[module]->addsymbol(le); 
      }
      if (common_size) { le.cridlex(")"); globtermgr.addsymbol(le); //importglobgr[module]->addsymbol(le); 
      }
      le.crtypelex(orig_typ);
      gr = globtermgr.addrule(le,RNOPRIOR,RNOINFO|RGLOP,fsymi);//gr = importglobgr[module]->addrule(le,RNOPRIOR,RNOINFO,fsymi);
      if (!batch) dumpgrrule(gr);
      add_to_fsymtab(common_size,gr,0);
    }
    //importglobgr[module]->dump(); 

    if (fsym_reduced > 1) {
      // compressed term construction
      if (!batch) stout << "fsymi " << fsymi << "=" << new_fsym << "\n";
      for(i=0; i < Pattermsi; i++) {
	NNEW(ct, term); ct->stinit();
//	for(j=0; j < common_size; j++) {  should be inversed
	for(j=common_size-1; j >=0; j--) {
	  Patterm[i].terms_compared.ith_term(j,&tt,&varn, &typ);
	  ct->pusht(*tt); }
	ct->crterm(fsymi, common_size);
	ct->popt();
      //stout << "Where_to_replace["<<i<<"]" << where_to_replace[i] << "\n";
	if (!batch) { where_to_replace[i]->write(stout); stout << " is replaced by "; ct->write(stout); stout << "\n"; }
      *(where_to_replace[i]) = *ct; } }
  }
}

//----------------------------------------------------------------------

void peval_init()
{
int i;
  for(i=0; i < MAXNOFTRN; i++)
    rule_redexes[i].empty();
}

termset::termset()
{
  tlist = NULL;
}

void termset::empty()
{
  tlist = NULL;
}

void termset::remove_first()
{
  if (tlist) 
    tlist = tlist->next;
  else {
    stout << "[int.err.] remove_first on empty list\n"; failexit(); }
}

//termset::~termset()
//{
//}

void termset::freeset() //~termset()
{
  /* */
  struct termlist *p,*tl = tlist;
  while(tl) {
    //stout << "TDELETE " << tl->t << "\n";
    tl->t->tdelete();
    p=tl;
    tl=tl->next;
    DELETE1(p); }
  /* */
}

void termset::unione(termset ts)
{
int i, varn, typ;
term *t;
  for(i=0; i < ts.size(); i++) {
    ts.ith_term(i,&t,&varn,&typ);
    addterm(true,t,/*varn,*/typ); }
}


void termset::addterm(int subsume, term *t, /*int varn,*/ int typp)
{
  struct termlist *tl;
  term *TT;
  if (subsume) {
  for(tl = tlist; tl; tl=tl->next) {
    if (is_instance(t,tl->t)) return;
    if (is_instance(tl->t,t)) {
      tl->t = t; return; } } }
//  stout << "ADD-TERM " << t << "\n";
  AALLOS(tl, struct termlist);
  tl->next = tlist;
  NNEW(TT, term); t->copy(*TT);  tl->t = TT;
  tl->typ = typp;
//  tl->t = t; 
/*  tl->varn = varn; */   tl->varn = TT->varnumbers()+1;
  tlist = tl;
}

void termset::ith_term(int i, term **t, int *varn, int *typ)
{
  struct termlist *tl = tlist;
  for(; tl && i > 0 ; tl=tl->next) i--;
  if (tl) {
    // stout << "I-TH " << i << " = " << tl->t << "\n";
    *t = tl->t; *varn = tl->varn; *typ = tl->typ; }
  else {
    *t = NULL; *varn = 0; *typ = -1; }
}

int termset::size()
{
  int i = 0;
  struct termlist *tl = tlist;
  for(; tl; tl=tl->next) i++;
  return i;
}

void termset::dump()
{
  struct termlist *tl = tlist;
  stout << "{ ";
  for(; tl; tl=tl->next) {
    tl->t->write(stout);
    stout << ":" << tl->varn;
    if (tl->typ > 0) stout << "::" << typet.ide(tl->typ);
    stout << ", ";
  }
  stout << " }\n";
}

void termset::single(term *t, /* int varn, */ int typ)
{
  termset(); addterm(false,t, /* varn, */ typ);
}

int no_loop(int rindex, term redex)
{
int j, k, varn, tmp, res, typ;
term *tt;
  //stout << "NO_LOOP TEST "; redex.write(stout); stout << "\n";
  //for (j = rindex; j; j = rule_comes_from[j] ) {
  //  stout << trrules.rulename(j); stout << "/" << j << ", "; }
  //stout << "\n";
  for (j = rindex; j; j = rule_comes_from[j] ) {
    for(k=0; k < rule_redexes[j].size(); k++) {
      rule_redexes[j].ith_term(k,&tt,&varn,&typ);
      //stout << "COMPARE "; redex.write(stout); stout << "/" << rindex; 
      //stout << " with "; tt->write(stout); stout << "/" << j << "\n";
      tmp = equal_non_ground;
      equal_non_ground = 1;
      res = redex.equal(*tt);
      equal_non_ground = tmp;
      // stout << "COMPARE RESULT " << res <<"\n";
      if (res) {
	//stout << "@@@@@@@@@@@@@@@@ NO_LOOP FALSE.\n";	
	return false;}
    }
  }
  // stout << "-----------NO_LOOP \n";
  return true;
}

int restrict_strategy(int orig_rule, lstream *f,
		      strategy *s_in, termset &pat_in, 
		      strategy **s_out, termset &pat_out, int module,
		      Patterms *Pate_in, Patterms *Pate_out
		      )
{
strategy *s, *s_help, *ss, **p_ss;
int is_restricted, s_changed = 0;

// case STRNAMEDONTCARE2: case STRNAMEDONTKNOW2: STRNAMEONE2:
struct strlist *slist, *ssl, **p_ssl;

// case STRNAMEDONTCARE: case STRNAMEDONTKNOW: STRNAMEONE: 
 int modu, p, pattrn_varn, j, ok1, specialised, rindex, typ; /* , size */
struct namelist *nlist, *new_nlist, **p_new_nlist;
struct transrulelist *tr;
term *pattrn, *cpattrn, *substarray, *new_rhs, *new_rhs1, nullterm;
struct vilist *vis;
//--
int varn, name, whichmatch, varj, indx;
term l, r, rlabel, *tvalue;
struct wherelist *wh;
transrule *newtrrule;
struct tseq *rhs;
termset pat1, pat2, tset_help;
Patterms *pate1, *pate2;
  ss = NULL; p_ss = &ss;
  pat_in.copy(pat1);    //    pat1.setlst(pat_in.getlst());
  //stout << "PAT1 = "; pat1.dump();
  pate1 = Pate_in;  
  for(indx=0,s = s_in; s != NULL; s=s->nex(),indx++) {
    if (s->nex()) {
      NNEW(pate2, Patterms); pate2->init(); }
    else { pate2 = Pate_out; }
    switch (s->strnam()) {
      case STRNAMEDONTCARE2: case STRNAMEDONTKNOW2: case STRNAMEONE2: { // over strategies
	ssl = NULL; p_ssl = &ssl;
        for (slist = s->substrlist(); slist; slist=slist->next) {
	  is_restricted = restrict_strategy(orig_rule,f,
				slist->str, pat1, &s_help, tset_help,module
				, pate1, pate2
					    );
	  s_changed |= is_restricted;
	  if (s_help) {
	    AALLOS(*p_ssl,struct strlist);
	    if (is_restricted) {
              (*p_ssl)->str = s_help; 
	      pat2.unione(tset_help); }  // ------ c'est pas bien
	    else {
	      (*p_ssl)->str = slist->str; }
	    p_ssl = &((*p_ssl)->next);
          }
	}
	*p_ssl = NULL;
        // ***

// stout << ":::\n ";
//        if ( (s_changed && ssl) || ((!s_changed) && s->substrlist()) ) {
// stout << ">>>\n ";


        if (ssl) {
          //{ struct strlist *xx; xx = ssl; stout << " SSL \n"; while (xx) { xx->str->dump(); xx = xx->next; stout << "\n------------\n"; } stout << "====================\n"; }
          AALLOS(*p_ss, strategy);
	  (*p_ss)->setname(s->strnam(),s->getmodule());
	  (*p_ss)->settypeof(s->typeofstr());
          (*p_ss)->setstl(ssl);
	  p_ss = (*p_ss)->nex_addr();
        }

/*
        if ( (s_changed && ssl) || ((!s_changed) && s->substrlist()) ) {
          AALLOS(*p_ss, strategy);
	  (*p_ss)->setname(s->strnam(),s->getmodule());
	  (*p_ss)->settypeof(s->typeofstr());
	  if (s_changed) {
	    (*p_ss)->setstl(ssl); }
	  else {
	    (*p_ss)->setstl(s->substrlist()); }
	  p_ss = (*p_ss)->nex_addr();
        }
*/
	tset_help.freeset();  // ?????????
      }
    break;
  case STRNAMEDONTCARE: case STRNAMEDONTKNOW: case STRNAMEONE: { // over rules
      modu = s->getmodule();
      new_nlist = NULL; p_new_nlist = &new_nlist;
      for(p = 0; p < pat1.size(); p++) {
	pat1.ith_term(p, &pattrn, &pattrn_varn, &typ);
	for (nlist = s->rulenamelist(); nlist; nlist = nlist->next) {
	  //stout << trrules.rulename(nlist->strname) << "\n"; 
	  for(tr=trrules.getrules(nlist->strname,modu); tr; 
                                    tr=tr->next) {

	    tr->rule->getr(varn, l, r, name, wh,
			   rhs,
			   whichmatch, rlabel);
	    //tr->rule->dump(5);
	    NNEW(cpattrn,term);
	    pattrn->copy(*cpattrn);
//            #ifdef DEBUG3
//            stout << "PATTERN + CPATTRN "; pattrn->write(stout); cpattrn->write(stout); stout << "\n"; pattrn->dump(); cpattrn->dump(); stout << "\n";
//            #endif


	    cpattrn->shift_vars(varn);




	    AALLOSS(substarray, varn+pattrn_varn, term); vis = NULL;
	    for(j=0; j < varn+pattrn_varn; j++) substarray[j] = nullterm;


	    if (
		(ok1 = l.unify(*cpattrn,substarray,varn+pattrn_varn,vis)) 
//  -- omitted,
//		&& nofalse(wh)
//
		) {
	      // rhs
	      NNEW(new_rhs,term); NNEW(new_rhs1,term);
                          //toto------------->              
              r.copy(*new_rhs1);
	      new_rhs1->copyinstall(true,*new_rhs,substarray);

              new_rhs->leave_constructors(new_rhs->varnumbers()+1);

	      //stout << "before add: "; pat2.dump();
              //new_rhs->write(stout); stout << ".." << varn+pattrn_varn << "\n";
	      pat2.addterm(true,new_rhs,/*varn+pattrn_varn,*/-1);

	      //stout << "after add: "; pat2.dump(); stout << "\n";

	      for(j=0, specialised = 0; j<varn; j++) {
		if (substarray[j].isvalidterm()) {
                  // stout << "TESTTERM j = " << j << " "; substarray[j].write(stout); stout << "\n";
		  // tvalue = deref(&(substarray[j]),substarray,varj);
		  tvalue = substarray[j].deref(substarray,varj);
		  if (tvalue->inf() != TVAR) {
		    specialised++;
                    // stout << "VAR(" << j << ") was instanciated\n"; 
              } } }
	      if (specialised > 0) {
		s_changed = 1;
		//stout << "S_CHANGED = " << s_changed << "\n";
		// stout << "\nREDEX[" << phase << "] "; cpattrn->write(stout);  stout << "\n";
		AALLOS(newtrrule,transrule);
		tr->rule->copyinstall(newtrrule,substarray,varn+pattrn_varn
                , pate1
                );

                if (peval_compression) {
                  pate2->add(false, new_rhs,
                                   /*varn+pattrn_varn,*/
                                   new_rhs->termtype().typeval()); }

		rindex = newtrrule->addpartrule(f,partial_index);
		if (!flowcheckrule(f,newtrrule)) {
		  f->owarn("\nint.error in search_matchable_rule\n",NULL);
	 	  failexit(); }
		// stout << "[*" << trrules.rulename(nlist->strname) << " specified*] ";


                if (peval_compression) {
                  pate1->add(false,newtrrule->getleft(),
                             /*newtrrule->getvarn(),*/
                             newtrrule->getleft()->termtype().typeval()); }

		if (!batch) {
                  stout << "\nNEW RULE: " << trrules.rulename(rindex)<< "/" << rindex
                        << " generated from " 
                        << trrules.rulename(orig_rule) << "/" << orig_rule << "\n"; 
                        newtrrule->dump(5); stout << "\n"; }

                rule_comes_from[rindex] = orig_rule;

//{int jj; for (jj = 0; jj < varn+pattrn_varn; jj++) {
//    stout << "sarray " << &(substarray[jj]) << " "; substarray[jj].write(stout); 
//    stout << "\n"; }
//}
		// ... create rule list
		AALLOS(*p_new_nlist,struct namelist);
		//if (new_nlist) stout << "New_list is non-empty\n";
		(*p_new_nlist)->strname = rindex;
		p_new_nlist = &((*p_new_nlist)->next); }
	      else {
                pate1->empty();
             //***s_changed = 0;
		//stout << "S_CHANGED = " << s_changed << "\n";
		// stout << "[*" << trrules.rulename(nlist->strname) << " not changed*] "; 
		AALLOS(*p_new_nlist,struct namelist);
		(*p_new_nlist)->strname = nlist->strname;
		p_new_nlist = &((*p_new_nlist)->next); } }
//		break; } }
	    else {
	      s_changed = 1;
	      // stout << "S_CHANGED = " << s_changed << "\n";
	      // stout << "[*" << trrules.rulename(nlist->strname) << " ommited*] "; 
	    }
	  }
	}
	*p_new_nlist = NULL;
	// ... create new strategy

        if ( (s_changed && new_nlist) || ((!s_changed) && s->rulenamelist()) ) {
	  AALLOS(*p_ss, strategy);
	  (*p_ss)->setname(s->strnam(),modu);
	  (*p_ss)->settypeof(s->typeofstr());
	  if (s_changed) {
	    (*p_ss)->setnamelist(new_nlist); }
	  else {
	     (*p_ss)->setnamelist(s->rulenamelist()); }
	  p_ss = (*p_ss)->nex_addr();
        }
      }
  }
  break;
  case STRFAIL:
    AALLOS(*p_ss, strategy);
    (*p_ss)->setname(s->strnam(),s->getmodule());
    (*p_ss)->settypeof(s->typeofstr());
    p_ss = (*p_ss)->nex_addr();
    pat2.empty();
    pate1->empty();
    break;
  case STRIDENTITY:
    AALLOS(*p_ss, strategy);
    (*p_ss)->setname(s->strnam(),s->getmodule());
    (*p_ss)->settypeof(s->typeofstr());
    p_ss = (*p_ss)->nex_addr();
    // pat2. ????
    pate1->empty();
    break;
   default:
      return 0;   // nothing changed
      if (!batch) { stout << "RESTRICT_STRATEGY\n"; s_in->dump(); stout << "\n"; }
      sterr << "\n[int.err.] restrict_strategy error - undefined strategy \n";
    }
    pat2.copy(pat1);           //  pat1.setlst(pat2.getlst());
    pat2.empty();
      if (peval_compression) {
        if (indx && pate1->Pattermsi) { 
          //stout << "$$$ 1 $$$\n";
          pate1->compare(module); 
          pate1->empty(); /* deallocate pate1 */ }
        if (s->nex()) pate1 = pate2; // pate2->copy(pate1); 
      }
  }
  *p_ss = NULL;

//stout << "FIN OF RESTRICT_STRATEGY\n";

  *s_out = ss; 
  if (s_changed) {
     pat1.copy(pat_out);   // pat_out.setlst(pat1.getlst()); 

/*
    if (ss == NULL) {
      stout << "IN  STRATEGY "; stout << "\n";s_in->dump(); stout << "\n-----\n";
      stout << "OUT STRATEGY "; stout << "\n";ss->dump(); stout << "\n-----\n";
      stout << "CHANGED && NULL\n"; }
*/
    pat2.freeset(); 
  }
  else { 
    // pat1.freeset(); 
    pat2.freeset(); 
  }
  return s_changed;
}

void mod_strategy(strategy *str, strategy ***p_whstrateg)
{
char sss[STRLEN], *strname;
int strindex_defs, strindex_refs, type, modu;
  type = str->typeofstr(); modu = str->getmodule();
//  sprintf(sss,"%s%d",PARTSTR,partcount++);
  snprintf(sss,sizeof(sss),"%s#%d",PARTSTR,partcount++);
  strname = attach_type_mod(sss,type,modu);

  //stout << strname << " = "; str->dump(); stout << "\n"; 
  strindex_defs = trrules.strategyindex_defs(strname,RLOCOOP);
  strindex_refs = trrules.strategyindex_refs(strname);
  trrules.settypeofstrategy_defs(strindex_defs,type);
  trrules.setstrategy_defs(strindex_defs,str);
  *p_whstrateg = trrules.getstrategyadr_refs(strindex_refs);
  trrules.assign_one_ref(1,strindex_refs);
  partial_index++; 
}

void trsystem::peval(lstream *f)
{
  /*int i; */ // this loop should be very expensive !!!!!!!!
//  for(import.forinit(); import.forcond(); import.fornext()) peval(f,import.forindex());

  peval(f,-1);
}

void trsystem::peval(lstream *f,int module)
{ 
  int i; /*, strindex_defs, strindex_refs;*/
struct transrulelist *tr;
strategy *fail_strategy;
/*strategy *str;*/
/*char sss[STRLEN], *strname;*/
 phase = 0;
 rule_was_added = 1;
 while(rule_was_added) {
  rule_was_added = 0;
  phase++;
  for(i=0; i<MAXNOFTRN; i++) {
    for(tr=trrules.getrules(i,module); tr; tr=tr->next) {
      int varn, name, whichmatch;
      term l,r,rlabel;
      struct wherelist *wh;
      /*char *sname,*ss,*modd, *type;*/
      struct tseq *rhs;
      /* term strat;*/
      tr->rule->getr(varn, l, r, name, wh,
	       rhs,
	       whichmatch, rlabel);
      if (tr->rule->no_false()) {
	for(; wh; wh=wh->next) {
	  if (wh->leftvarn != IFVARN && wh->strateg != NULL &&
	      (
	       (peval_loop_test && NULL == strstr(trrules.strategyname_refs(wh->strateg),PARTSTR)) ||
	       (!peval_loop_test && no_loop(i,wh->whereterm)
		))) { 
	    termset t_in;
	    termset t_out;
	    strategy *s_out;
	    int is_reduced;
	    term *cwhterm, *r_redex;
	    Patterms Pate_in, Pate_out;
	    NNEW(cwhterm, term);
	    wh->whereterm.copy(*cwhterm);

//	    if (!batch) { cwhterm->write(stout); stout << "\t-->\t";}


//!!!!!!!?????	    cwhterm->leave_constructors(cwhterm->varnumbers()+1);


//	    if (!batch) { cwhterm->write(stout); stout << "\n"; }

	    t_in.single(cwhterm,/*varn,*/-1);  // deleted by ~tset()


	    if (peval_compression) {
	      Pate_in.init(); Pate_out.init();
//	      Pate_in.add(true,cwhterm,/*varn,*/ cwhterm->termtype().typeval()); }
	      Pate_in.add(true,&(wh->whereterm),
			  /*varn,*/wh->whereterm.termtype().typeval()); }

            is_reduced = restrict_strategy(i,f,
				*(wh->strateg),t_in,&s_out,t_out,
					   tr->rule->getmodule()
				, &Pate_in, &Pate_out
					   );
	    if (is_reduced) {
	      
	      NNEW(r_redex, term); wh->whereterm.copy(*r_redex);
	      //stout << "===ADD-REDEX "; r_redex->write(stout); stout << "\n";

	      rule_redexes[i].addterm(true,r_redex,
				      /*r_redex->varnumbers(),*/-1);

	      if (peval_compression) {
		if (Pate_in.Pattermsi) {
		  Pate_in.compare(tr->rule->getmodule());
		  Pate_in.init(); Pate_out.init(); } 
	      }

	      if (!batch) {
               stout << "REDEX in the rule\n"; tr->rule->dump(0); stout <<"\n";
//	       stout << "STRAT_IN:"; (*(wh->strateg))->dump();  stout << "\n";
               stout << "TSET_IN : "; t_in.dump(); stout << "\n"; }
	       if (s_out == NULL) {
		 // old version :: tr->rule->addwhere(0,IFVARN,NULL,falseterm,booltype); 
		 NNEW(fail_strategy, strategy);
		 fail_strategy->setnext(NULL);
		 fail_strategy->setname(STRFAIL,(*(wh->strateg))->getmodule());
		 fail_strategy->settypeof((*(wh->strateg))->typeofstr());
		 mod_strategy(fail_strategy, &(wh->strateg)); }
	       else {
		 if (!batch) {
		   stout << "STRAT_OUT:"; s_out->dump(); stout << "\n";
		   //stout << "TSET_OUT :"; t_out.dump(); stout << "\n"; 
		 }
		 mod_strategy(s_out, &(wh->strateg));  
		 if (!batch) stout << "\n"; 
	       }
	       if (!batch) {
		 stout << "\nMODIFIED RULE:\n"; 
		 tr->rule->dump(0); stout << "\n";
	         stout << "---------------------------------------------------\n"; 
	       }
	    }
	    t_in.freeset();
	    t_out.freeset();
	  }
	}
      }
    }
  }
 }
}

int transrule::addpartrule(lstream * /*f*/,int partindex)
{
char newname[STRLEN];
/*strategy *str;*/
/*struct namelist *nm;*/
int rindex;
  if (nameindex < 0) { sterr << "addpartrule intern.error"; failexit(); }
//  sprintf(newname,"%d%s%s",partindex,PEVALSTRING,trrules.rulename(nameindex));
  snprintf(newname,sizeof(newname),"%s%s%d",trrules.rulename(nameindex),PEVALSTRING,partindex);
  rindex = trrules.trruleindex(newname);
  trrules.addrule(newname,leftside.head(),this);
  rule_was_added = 1;
  return rindex;
}

void trsystem::expand_strategies()
{
int i;
  for(i=0; i < MAXNOFSTRAT; i++)
    if (strategies_defs[i]) {
      strategies_defs[i]->count_rules(); }
  for(i=0; i < MAXNOFSTRAT; i++)
    if (strategies_defs[i]) {
      strategies_defs[i]->expand_strategy(); }
}

//-------------------------------------------------------------------

void strategy::count_rules()
{strategy *p = this;
 for(p=this; p; p=p->next) p->count_rul();
}

void strategy::expand_strategy()
{strategy *p = this;
 for(p=this; p; p=p->next) p->expand_str();
}

void strategy::count_rul()
{ struct namelist *np; /*, **npp;*/
 struct strlist *sl; /*, *sl1;*/
 /* char   *ss;*/
  struct transrulelist *trlist;
  /*  transrule *rule;*/
  /*int    r, aa, res, typ, refs;*/
  /*transrule *new_trrule;*/

  switch (strname) {
    case STRIDENTITY: case STRFAIL:
      break;
    case STRNAMEREPEAT: case STRNAMEITERATE:
      u.substrategy->count_rules();
      break;
    case STRNAMENORMALISE:
      u.substrategy->count_rules();
      break;
    case STRNAMEDONTCARE2: case STRNAMEDONTKNOW2: case STRNAMEONE2:
      for(sl = u.stl; sl!=NULL; sl=sl->next)
	sl->str->count_rules();
      break;
    case STRNAMEDONTCARE: case STRNAMEDONTKNOW: case STRNAMEONE:
      for(np = u.cr.nm; np!=NULL; np=np->next) {
	struct transrulelist *trlst;
	/*struct namelist *np_f, *np_h;*/
	/* int  cnt, rcnt; i, */
	trlist = trrules.getrules(np->strname,getmodule());
	//stout << "Try to Unfold " << trrules.rulename(np->strname) << " in " << import.ide(getmodule()) << "\n";
	for(trlst = trlist; trlst; trlst=trlst->next) {
	  trlst->rule->add_rule_counter();
	}
      }
      break;
    case STRCALL:
    case STRMETA: 
    case STRNAMEDCPROCESSCALL:
    case STRNAMEDKPROCESSCALL:
      break;
    default : dumpout << "\n[expand_strategy]unknown strategy, internal error\n";
  }
}

void strategy::expand_str()
{ struct namelist *np, **npp;
 struct strlist *sl, **slp; /* *sl1, **aux;*/
 /*char   *ss;*/
  struct transrulelist *trlist;
  /*transrule *rule;*/
  /* int    runame; r, aa, res, typ, refs, */
  transrule *new_trrule;
  int  cnt, rcnt; /* i */
  switch (strname) {
    case STRIDENTITY: case STRFAIL:
      break;
    case STRNAMEREPEAT: case STRNAMEITERATE:
      u.substrategy->expand_strategy();
      break;
    case STRNAMENORMALISE:
      u.substrategy->expand_strategy();
      break;
    case STRNAMEDONTCARE2: case STRNAMEDONTKNOW2: case STRNAMEONE2:
      for(rcnt = 0,sl = u.stl; sl!=NULL; sl=sl->next) {
	if (sl->str->strnam() != STRFAIL) rcnt++; }
      if (rcnt == 0) { // 0- delete it
	  setname(STRFAIL,getmodule());
  	  return;
      }
      for(slp = &(u.stl); (*slp)!=NULL; ) {
	(*slp)->str->expand_strategy(); 
	if ((*slp)->str->strnam() != STRFAIL) {
	  slp = &((*slp)->next); }
	else {
	  (*slp) = (*slp)->next; }
      }
      break;
    case STRNAMEDONTCARE: case STRNAMEDONTKNOW: case STRNAMEONE:
      for(np = u.cr.nm, npp = &(u.cr.nm); np!=NULL; np=np->next) {
	struct transrulelist *trlst;
	struct namelist *np_f; /*, *np_h;*/
	trlist = trrules.getrules(np->strname,getmodule());
	for(cnt=0, rcnt=0, trlst = trlist; trlst; trlst=trlst->next) {
	  if (trlst->rule->no_false()) rcnt++;
	  cnt++;
	}

       //stout << "Try to Unfold " << trrules.rulename(np->strname) << " in " << import.ide(getmodule()) << "\n";
       //stout << "rcnt = " << rcnt << " cnt=" << cnt << "\n";
        
        if (rcnt == 0) { // 0- delete it
	  setname(STRFAIL,getmodule());
  	  return;
	}

	if (rcnt != cnt || // there is a false rule
	    rcnt > 1) {    //  +1- unfold it

	  np_f = np->next;  // delete a vagon
	  if (rcnt >= 1) {
	    if (!batch) { stout << "Unfold rule " << trrules.rulename(np->strname) << " into " << cnt << " rules " << "\n"; }
	    for(trlst = trlist; trlst; trlst=trlst->next) {
	      if (trlst->rule->no_false()) {
		AALLOS(*npp, struct namelist);
		if (trlst->rule->get_rule_counter() > 1) {
		  if (!batch) stout << ".";
		  AALLOS(new_trrule, transrule);
		  trlst->rule->copy(new_trrule);
		  (*npp)->strname = new_trrule->
		    indexrule(extend_index++);
		} else {
		  // runame = trlst->rule->get_nameindex();
		 // stout << "DELETE RULE = " << runame << "\n";
		  (*npp)->strname = trrules.renamerule(
				   trlst->rule->get_nameindex(),
				   extend_index++,trlst->rule);
//		  trrules.remove_rulename(runame); //0106
		}
		npp = &((*npp)->next); }
	    }
	  }
	    (*npp) = np_f;
	} else {
	  npp = &((*npp)->next);
	}
      }
      break;
    case STRCALL:
    case STRMETA: 
    case STRNAMEDCPROCESSCALL:
    case STRNAMEDKPROCESSCALL:
      break;
    default : dumpout << "\n[expand_strategy]unknown strategy, internal error\n";
  }
}

int transrule::indexrule(int indx)
{
char newname[STRLEN];
/*strategy *str;*/
/*struct namelist *nm;*/
int rindex;
  if (nameindex < 0) { stout << "indexrule intern.error"; failexit(); }
//  sprintf(newname,"%d%s",indx,trrules.rulename(nameindex));
  snprintf(newname,sizeof(newname),"%s%d",trrules.rulename(nameindex),indx);
  rindex = trrules.trruleindex(newname);
  trrules.addrule(newname,leftside.head(),this);
  return rindex;
}

int trsystem::renamerule(int nameindex,int indx, transrule *rr)
{
char newname[STRLEN];
/*strategy *str;*/
/*struct namelist *nm;*/
int rindex;
struct transrulelist *tl;
struct transrulelist **tlp;


  if (nameindex < 0) { stout << "renamerule intern.error"; failexit(); }
  snprintf(newname,sizeof(newname),"%s%d",trrules.rulename(nameindex),indx);
  rindex = trrules.trruleindex(newname);
  rr->setnameindex(rindex);

  NNEW(tl ,struct transrulelist);
  tl->rule = rr;
  tl->next = NULL;
  *(lastrule[rindex]) = tl;
  lastrule[rindex] = &(tl->next);

  for(tlp=&(rules[nameindex]); 
      *tlp ; 
      tlp=&((*tlp)->next)) {
    //    stout << "+";
    if ((*tlp)->rule == rr) {
      //      stout << ".";
      *tlp = (*tlp)->next; break; }
  }
  return rindex; 
}

void transrule::copy(transrule *intothis)
{
struct wherelist *wh,**pwheres;
/*term left, right;*/
  *intothis = *this;
  this->leftside.copy(intothis->leftside);
  this->rightside.copy(intothis->rightside);
  wh = this->wheres; pwheres = &(intothis->wheres);
  for(; wh; wh=wh->next) {
    AALLOS(*pwheres, struct wherelist);
    *(*pwheres) = *wh; 
    wh->whereterm.copy((*pwheres)->whereterm);
    pwheres = &((*pwheres)->next);
  }
  *pwheres = NULL;
}
//------------------------------------------------------------------

// TEMPORARY VERSION
// subt = sigma(t)
int is_instance(term *subt, term *t)
{
   if (t->isvalidterm() &&
       subt->isvalidterm() &&
       t->inf() == TVAR)
     return 1;
   else
     return 0;
}

int term::leave_constructors(int varn)
{
 int i,a,vn;

  switch (t->infos) {
  case TIDENT: 	case TNUMBER: 
    return varn;
  case TSTRING: 
    return varn;
  case TVAR:	
    return varn;
  case TNORMFS: 
//    if (ISCONSTRUCTOR(t->infos)) {
//stout << this->head() << "is " << ISFUNCONSTRUCTOR(this->head())
//      << "," <<trrules.getnnrules(this->head())
//      << "," <<fsymtab[this->head()].get_semantic()
//      << "\n";
    if (!ISFUNCONSTRUCTOR(this->head())) {
      term *t;
      int head;

      // stout << "LEAVE CONSTRUCTORS :: " << varn << " head= " << head << " "; write(stout); stout << "\n";

      t = this; head = t->head();
      t->tdelete();
      t->stinit();
      t->crvar(varn++, fsymtab[head].textform()->leftside);
      t->popt(); 

      return varn; }
    else {
      a = headarity(); vn = varn;
      for(i=0; i<a; i++)
	vn = t->subt[i].leave_constructors(vn);
      return vn; }
  default :     interr();
  }
  return(0);

}

// make a instacaited copy of a rule
void transrule::copyinstall(transrule *intothis, term *substarray, int varn, Patterms * /*pate1*/)
{
struct wherelist *wh,**pwheres;
 term left, right, whtrm; /*, varnt, varnt1;*/
 /*int varindex;*/

  *intothis = *this;
  intothis->varnum = varn;

  //this->leftside.write(stout); stout << "\n";
  this->leftside.copyinstall(true,left,substarray);
  left.copy(intothis->leftside);  // to de-alias all terms
  //intothis->leftside.write(stout); stout << "\n";

  //this->rightside.write(stout); stout << "\n";
  this->rightside.copyinstall(true,right,substarray);
  right.copy(intothis->rightside); // to de-alias all terms
  //intothis->rightside.write(stout); stout << "\n";

  // wheres
  wh = this->wheres; pwheres = &(intothis->wheres);
  for(; wh; wh=wh->next) {

    /*
    varindex = wh->leftvarn;
    varnt.stinit(); varnt.crvar(varindex,booltype); varnt.popt();
    varnt.copyinstall(true,varnt1,substarray);
    varnt.write(stout); stout << " --->> "; varnt1.write(stout); stout << "\n";
    */

    /*
    varnt = substarray[varindex];
    if (varnt.inf() != TVAR) {
      sterr << "transrule::copyinstall internal error installing variable # "
	    << varindex << "\n"; failexit(); }
	    */
    AALLOS(*pwheres, struct wherelist);
    *(*pwheres) = *wh; 
    //wh->whereterm.write(stout); stout << "\n";
    wh->whereterm.copyinstall(true,whtrm,substarray);
    whtrm.copy((*pwheres)->whereterm); // to de-alias all terms
    //(*pwheres)->whereterm.write(stout); stout << "\n";
    pwheres = &((*pwheres)->next);
  }
  *pwheres = NULL;
}



