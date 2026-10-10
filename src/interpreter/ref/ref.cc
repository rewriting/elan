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


#ifndef __aterm_h
#define __aterm_h

#include "commondefs.h"
#include "termdefs.h"
#include "rtdatas.h"
#include "module.h"
#include "command.h"
#include "strategy.h"
#include <string.h>
extern void addstandards(grammar &gr);
extern int Strategyname_to_ref_index(char *strname, int typ);
extern int cexport;

int adump   = 0;    // export to aterm form
int aimport = 0;    // import from  aterm form
int reduceimport = 0;    // import from  reduce form

//---------------------------
extern int max(int a, int b);
//---------------------------

void Query_dump(ochstream &af)
{
  af << "QUERY(" << sourcetypei << "," << qresulttypei << ",";
  if (mainstrategy != -1)
    af << trrules.strategy_refs_into_defs(
	    trrules.strategyindex_refs(
	     trrules.strategyname_refs(mainstrategy)));
  else
    af << -1;
  af << ",";
  startwith.Awrite(af); af << ",";
    checkwith.Awrite(af); af << ") end \n";
}

int lexem::Adump(ochstream &af)
{
  if (nonterminal())
    af << "Type(" << typeval() << ")";
  else if (ischar())
    af << "Char(" << charval() << ")";
  else if (isnum())
    af << "Num(" << numval() << ")";
  else if (isstring())
    af << "String(" << stringval() << ")";
  else if (isident())
    af << "Ident(" << idval() << ")";
  else if (isblankk())
    af << "Blank";
  //  return; // delete
  else {
    sterr << "\n[fatal] unknown lexem\n";
    dump(); 
    stout << "\n"; }
  af << ".";
  return 0;
}

void grammar::rmark(int set, int flag)
{
struct grammrulelist *gr;
int i;
  for (i=0; i<NNONTERMINALS; i++) {
    for(gr=nontt[i]; gr; gr=gr->next) { 
      if ((gr->infos & RSTATMSK)) {
	if (set) gr->r->topglobgram = flag; 
	else     gr->r->topglobgram |= flag; 
      }
    }
  }
}

void dump_visibilities(ochstream &ff)
{ int ii, jj;
  ff << "visibilities\n";
  for(ii = 0; ii < MAXNOFIMPORTS; ii++)
    if (visibilities[ii]) {
      ff << ii << ":";
      for(jj = 0; jj < MAXNOFIMPORTS; jj++)
	if (visibilities[ii][jj])
	  ff << jj << ".";
      ff << "nil.\n"; 
    }
  ff << "nil end\n\n";
}

#define PRINTABLE 8

void grammar::Adump(ochstream &af,int flag)
{ struct grammrulelist *gr;
 int i, j; /*, k;*/
  lexem bintype;
  // af <<"symbols " << "\n";
  for (i=0; i<NNONTERMINALS; i++) {
    if (nontt[i]!=NULL) {
      bintype.crtypelex(i);
      af <<"GrammarForSort " << i << ":" << // builtintype(i)
	((bintype==booltype)||
	 (bintype==identype)||
	 (bintype==numtype)||
	 (bintype==stringtype))
	 << ":\n";
      for(gr=nontt[i]; gr; gr=gr->next) {
	if ((gr->infos & RSTATMSK ) && (gr->r->topglobgram == flag)) { 
	  af << flag << ":"
	     << gr->r->rulenumber << ":"
	     << ((gr->r->priority) & RPRIORITYMSK) << ":"
	     << ((gr->r == fsymtab[gr->r->rulenumber].textform())? 
		 PRINTABLE : 0) +
	        ((gr->r->priority) / (RPRIORITYMSK+1)) << ":"
	     << gr->r->semantic << ":"
	     << fsymtab[gr->r->rulenumber].infos() << ":"
	     << gr->r->defstrat << ":";
	  // gr->r->leftside.Adump(af);
	  for (j=0; gr->r->rside[j].isnotendofstream();j++)
	    gr->r->rside[j].Adump(af);
	  af << "nil:";  // LOCAL STRATEGIES ARE NOT EXPORTED YET
	  af << "nil.\n"; }
      }
      af << "nil end\n";
    }
  }
  af << "EndDef end\n";
}

void stringtab::Adump(ochstream &gout, const char *heading)
{
  gout << heading << "\n";
  for(forinit(); forcond(); fornext()) {
    gout << forindex() << ":" 
	 << "\"" << ide(forindex())
	 << "\"" << ".\n";
  }
  gout << "nil\n";
  gout << "end\n\n";
}

void term::Awrite(ochstream &gout)
{
  Awriterec(gout);
}

void term::Awriterec(ochstream &gout)
{ int i;
/* lexem *p;*/
  const char *ch;
  switch (t->infos) {
  case TVAR:
    if(cexport && t->compif.isVarExt)
      gout << " EVAR(";
    else
      gout << " VAR(";
    gout << t->fsymi << ","
	 << t->compif.varsort.typeval() << ")";
    break;
  case TNUMBER:
    gout << "INT(" <<  (int) t->fsymi << ")";
    break;
  case TSTRING: 
    // gout << "STRING(\"" << (char*)(t->subt) << "\")";
    ch = (char*)(t->subt);
    gout << "STRING(";
    for(i=0; ch[i]; i++)
      gout << (int)(ch[i]) << ".";
    gout << "nil)";
    break;
  case TIDENT: 
    gout << "IDENT(" << t->fsymi << ")";// tabofident.ide(t->fsymi) << ")";
    break;
  case TNORMFS: 
    gout << "FSYM(" ; 
    if (cexport &&   // only if it is ref form for the compiler
	(head() == Call)) {
	char sname[STRLEN];
	int tindex, sindex;
	char *tname;
	term2string(this,sname);
          //stout << "convert strategy name to index \n";
          //stout << "sname = '" << sname << "'\n";
        
	tname = strstr(sname,":");

        if(tname) {
            /*char foo[STRLEN];*/
	  char *quote1, *quote2;
          
          tname++;
            /*
              stout << "tname = '" << tname << "'\n";
              stout << "tinfo = '" << t->infos << "'\n";
              term2string(subterm(0),foo);
              stout << "foo = '" << foo << "'\n";
            */
          quote1 = strstr(sname, "\"");
          quote1++;
          quote2 = strstr(quote1, "\"");
	    /*
	      stout << "quote1 = '" << quote1 << "'\n";
	      stout << "quote2 = '" << quote2 << "'\n";
	      stout << "len = '" << quote2-quote1 << "'\n";
	    */
          *quote2 = '\0';
	  
            //stout << "quote1 = '" << quote1 << "'\n";

          tindex = typet.index(tname);
          if (1 || tindex>0) {
              //sindex = Strategyname_to_ref_index(subterm(0)->getstring(),tindex);
            sindex = Strategyname_to_ref_index(quote1,tindex);
            
              //stout << sindex << "\n";
            gout << "/**/INT(" << sindex << ").nil," << head() << ")";
          } else {
            sterr << "\n[fatal] Call strategy type has not been declared\n";
            failexit();
          }
	} else {
          sterr << "\n[fatal] Call type name not found\n";
          failexit();
        }
    } else {
      for(ch=" ",i=0; i < headarity(); i++) {
        gout << ch; ch = "."; t->subt[i].Awriterec(gout); }
      gout << ch << "nil, " << t->fsymi 
           << ")";
    }
    break;
      default :     interr();
  }
}

void Awherelisdump(ochstream &f,struct wherelist *wl)
{
  if (wl) {
    Awherelidump(f,wl); f << ".\n"; Awherelisdump(f,wl->next); }
  else
    f << "nil";
}

void Awherelidump(ochstream &f,struct wherelist *wl)
{
  if (wl->leftvarn == TRYCHOICEEND) {
     f  << "TRY(\n"; 
     struct WHEREbranches *brlist = wl->wherebranch_list;
     while (brlist) {
       Awherelisdump(f,brlist->wherebranch);
       f  << "."; 
       brlist=brlist->next;
     }
     f  << "nil)\n"; 
  } else { 
   if (wl->leftvarn == IFVARN) f << "IFF(";
    else if (wl->leftvarn == WHEREPATTERN) {
      f << "PWHERE(";
      wl->pattern.Awrite(f);
      f  << "," << wl->pattype << ",";
      if (wl->strateg) 
	f << trrules.strategy_refs_into_defs(
	     trrules.strategyindex_refs(
	     trrules.strategyname_refs(wl->strateg))) << ","; 
      else
	f << "-1,";
    }
    else  {
      f << "WHERE(";
      // 310598 wl->leftvarterm.Awrite(f);
      f << "VAR(" << wl->leftvarn << "," <<  wl->leftvarterm.termtype().typeval() << ")";
      if (wl->strateg) 
	f << "," << trrules.strategy_refs_into_defs(
	            trrules.strategyindex_refs(
	            trrules.strategyname_refs(wl->strateg))) << ","; 
      else 
	f << ",-1,";
    }
    wl->whereterm.Awrite(f); f << ")";
  }
}

void Aswitchdump(ochstream &f, struct tseq *tseq)
{
  if (tseq->is_case == 0) {
    f << "NOSWITCH(";
    Awherelisdump(f,tseq->seq);
    f << ",";
    tseq->u.one_branch.result->Awrite(f);
    f << ")";
  } else {
    f << "SWITCH(";
    Awherelisdump(f,tseq->seq);
    f << ",";
    Abranchlistdump(f,tseq->u.more_branches.brlist);
    f << ")";
  }
}

void Abranchlistdump(ochstream &f, struct branch *brlist)
{
  while (brlist) {
    brlist->test.Awrite(f);
    f << ",";
    Aswitchdump(f,brlist->tseq);
    f << ".";
    brlist = brlist->next;
  }
  f << "nil";
}

void transrule::Adump(ochstream &af,int /*ods*/)
{ struct wherelist *wl;
  if (rhs) 
    af << "\nSWRULE(\n";
  else
    af << "\nRULE(\n";
  if (nameindex != -1) 
    af << nameindex << ",";
  else 
    af << "-1,\n"; 
  af << ruletypeval() << ","
     << getmodule() << ","
     << getinfos() << ","
     << whichmatch << ","
     << getvarn() << ",";
  leftside.Awrite(af); af << ",\n";
  if (rhs) {
    Aswitchdump(af,rhs);
  } else {
    rightside.Awrite(af);
    af << ",\n";
    wl = wheres; Awherelisdump(af,wl);
  }
  af << ")\n\n";
}

void trsystem::Adump_tabs(ochstream &af)
{
  rulenames->Adump(af,"RuleNames");
  strategynames_defs->Adump(af,"StrategyNames");
//  strategynames_refs->Adump(af,"strategynames");
}

void trsystem::Aread_tabs(lstream *f)
{
  rulenames->Aread(f,0);
  strategynames_defs->Aread(f,0);
//  strategynames_refs->Aread(f,0);
}

void trsystem::joinrdefs()
{
  int ii;
  for(ii=0; ii < MAXNOFSTRAT; ii++) {
    strategies_cross[ii] = ii;
    strategies_refs[ii] = strategies_defs[ii]; }
}

void trsystem::Aread(lstream * /*f*/)
{
}

void trsystem::Adump(ochstream &af)
{ struct transrulelist *tl;
  strategy *sl;
  struct tseq *rhs;
  int i;
  int n,nameind;
  term l,r;
  struct wherelist *wh;
  int whichmatch;
    term rlabel;

  for (i=0; i<MAXNOFTRN; i++) {
    tl= rules[i];
    while (tl!=NULL) {

    tl->rule->getr(n,l,r,nameind,wh,
		   rhs,
		   whichmatch,rlabel);
      af << "/*" << i << " " << rulenames->ide(i) << "*/";
      tl->rule->Adump(af,0);
      af << " end\n"; // af << ".\n";
      tl=tl->next;
    }
  }
  // af << "nil,\n";
  for (i=0; i<MAXNFSYM; i++) {
    tl= nnrules[i]; 
    while (tl!=NULL) {
    tl->rule->getr(n,l,r,nameind,wh,
		   rhs,
		   whichmatch,rlabel);
      tl->rule->Adump(af,0);
      af << " end\n"; // af << ".\n";
      tl=tl->next;
    }
  } 
  af << "EndDef end\n";
  for (i=0; i<MAXNOFSTRAT; i++) {
    sl= strategies_defs[i];
    if (sl!=NULL) {
      af << "STRATEGY(" << i << "," << sl->typeofstr()
	 << "," << sl-> getmodule() << ",";
      sl->Adump(af);
      af << ") \nend\n"; } 
  }
  af << "EndDef end\n";
}


void strategy::Adump(ochstream &af)
{
  strategy *st;
  for (st=this; st; st = st->next) {
    st->Asimpledump(af);
    if (st->next) af << " ; "; }
}

void strategy::Asimpledump(ochstream &af)
{ struct namelist *np;
   struct strlist *sl;
    switch (strname) {
    case STRNAMEDCPROCESSCALL:
    case STRNAMEDKPROCESSCALL:
      unsigned i;
      af << (strname == STRNAMEDCPROCESSCALL ? "dccall" : "dkcall") << "(";
      for(i=0; i < strlen(u.procc.pname); i++)
	af << (int)(u.procc.pname[i]) << ".";      // list of chars
      af << "nil,";
      af << u.procc.maxn << "," << u.procc.restype << ")";
      break;
    case STRNAMEDONTCARE : case STRNAMEDONTKNOW : case STRNAMEONE:  case STRNORM_IN: case STRNORM_OUT: // [Huy: May  4 00] 
      if (strname == STRNORM_IN) 
	af << "normin("; 
      else
      if (strname == STRNORM_OUT) 
	af << "normout("; 
      else
       if (strname == STRNAMEONE) 
	af << "one(";
      else
	af << (strname==STRNAMEDONTKNOW ? "dk" : "dc") << "(";
      for(np = u.cr.nm; np; np = np->next)
	trrules.rulelistdump(af,np->strname,getmodule());
      af << "nil)";
      break;
    case STRNAMEREPEAT: case STRNAMEITERATE :
      af << (strname==STRNAMEREPEAT ? "repeat*" : "iterate*")
	 << "(";
      u.substrategy->Adump(af);
      af << ")";
      break;
    case STRNAMETALL: // [pem: Oct 26 00]
      af << "tall(";
      u.substrategy->Adump(af);
      af << ")";
      break;
    case STRNAMETONE: // [pem: Oct 26 00]
      af << "tone(";
      u.substrategy->Adump(af);
      af << ")";
      break;
    case STRNAMETSOME: // [pem: Oct 26 00]
      af << "tsome(";
      u.substrategy->Adump(af);
      af << ")";
      break;
    case STRNAMEREWRITE: // [pem: Apr  8 02]
      af << "rewrite(";
      u.substrategy->Adump(af);
      af << ")";
      break;
    case STRNAMEDONTCARE2: case STRNAMEDONTKNOW2: case STRNAMEONE2:
      if (strname == STRNAMEONE2)
	af << "ONE(";
      else
	af << (strname==STRNAMEDONTKNOW2 ? "DK" : "DC") << "(";
      for(sl = u.stl; sl; sl=sl->next) {
  	sl->str->Adump(af);
  	if (sl->next) af<< " , "; }
      af << ")";
      break;
    case STRMETA:
      af << "META";	
      break;
    case STRFAIL:
      af << " fail "; break;
    case STRIDENTITY:
        af << " id "; break;
    case STRCALL:
	af << "call(" 
	   << trrules.strategy_refs_into_defs(
		trrules.strategyindex_refs(
		  trrules.strategyname_refs(u.procc.maxn)))
	   << ")";
        break;
    default : af << "\n[Asimple-dump] unknown strategy, internal error\n";
    }
}

//------------------- this part has been made for simplify the life of PEM
int old_rule_indexes[MAXNOFTRN];

void trsystem::rulelistdump(ochstream &af, int rindex, int module)
{
int new_index, i;
struct transrulelist *tr;
int module_map[MAXNOFIMPORTS];
  for(i=0; i < MAXNOFIMPORTS; i++) module_map[i] = 0;
  // stout << rulenames->ide(rindex) << " in module " 
  //	   << import.ide(module) << " is developed to ";
  for(tr = rules[rindex]; tr; tr=tr->next) 
    if (visible(module, tr->rule->getmodule(), tr->rule->getinfos()) &&
	((module_map[tr->rule->getmodule()] & (tr->rule->getinfos())) == 0)) {
      module_map[tr->rule->getmodule()] |= tr->rule->getinfos();
      new_index = rulenames->addstr(attach_mod_loc(
			rulenames->ide(rindex),
			tr->rule->getmodule(),
			tr->rule->getinfos()));
      af << new_index << ".";
  //    stout << rulenames->ide(new_index) << ".";
    }
  // stout << "\n";
}

void trsystem::localize1()  // rename old indexes to new ones
{
int new_index, i;
struct transrulelist *tr;
  for(i=0; i< MAXNOFTRN; i++) old_rule_indexes[i] = 0;
  for(rulenames->forinit(); rulenames->forcond(); rulenames->fornext()) {
    old_rule_indexes[rulenames->forindex()] = 1;
    for(tr = rules[rulenames->forindex()]; tr; tr=tr->next) {
      new_index = rulenames->addstr(attach_mod_loc(
			rulenames->foractval(),
			tr->rule->getmodule(), 
			tr->rule->getinfos()));
      //stout 
      // << rulenames->foractval()
      // << " -> " 
      // << rulenames->ide(new_index) << "\n";
      tr->rule->setnameindex(new_index);
    }
  }
}

void trsystem::localize2()  // remove old indexes
{
  int i; /* new_index, */
  for(i=0; i< MAXNOFTRN; i++) 
    if (old_rule_indexes[i]) {
      rulenames->removestr(i); rules[i] = NULL; }
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

void stringtab::Aread(void *voidf, int skip)
{
lexem le, indlex, strlex;
lstream *f = (lstream *)voidf; // shitty
  f->ilex(le);
  if (!batch) {
    stout << "Importing " << le.alfsy() << "\n"; }
  for(;;) {
    f->ilex(indlex); 
    if (!strcmp(indlex.alfsy(),"nil")) break;
    f->ilex(le); f->ilex(strlex); f->ilex(le);
    if (!skip)
      id[indlex.numval()] = strlex.stringval();
    else 
      stout << "[" << indlex.numval() << "]" << " == " 
	     << strlex.stringval() << "\n";
    strin++; }
  f->ilex(le);
}

void Aread_visibilities(void *voidf)
{
  lexem le, indlex,  numlex; /* strlex, */
  int i, j;
  char *visi;
  lstream *f = (lstream *)voidf; // shitty
  for(i = 0; i < MAXNOFIMPORTS; i++) visibilities[i] = NULL;
  f->ilex(le);
  if (!batch) {
    stout << "Importing " << le.alfsy() << "\n"; }
  for(;;) {
    f->ilex(indlex); // index
    if (!strcmp(indlex.alfsy(),"nil")) break;
    f->ilex(le);    // :
    i = indlex.numval();
    // stout << i << "\n";
    AALLOSS(visi,MAXNOFIMPORTS,char);
    for(j=0; j < MAXNOFIMPORTS; j++) visi[j] = 0;
    for(;;) {
      f->ilex(numlex);   // index
      if (!strcmp(numlex.alfsy(),"nil")) break;
      f->ilex(le);       // .
	// stout << numlex.numval() << "\n";
      visi[numlex.numval()] = 1; }
    visibilities[i] = visi;
    f->ilex(le); }
  f->ilex(le);
}

//---------------------------------------------------- PARSER
//---------------------------------------------------- PARSER
//---------------------------------------------------- PARSER
//---------------------------------------------------- PARSER

// The state of the .ref parser (atermsemact, reducesemact: the semantic
// actions of the parsers generated from aterm.ref and aterm.reduce), until
// S3b separate globals. One instance, rp, used only in this file. The
// strategies being read are built on the loader's stacks (ld.strstack,
// ld.strlstack, ld.actstrategy: load/strategy.h); the result of a reduce
// file (redTerm, redStrategyIndex...) stays global, the driver reads it.
struct RefParserState {
  int strindex_refs = 0;
  int strindex = 0;
  int strindex1 = 0;
  int nval = 0;                       // the last number read
  int ar = 0;
  int rlab = 0;
  int amodule = 0;
  lexem lex, lefside, varsortlex;
  int spriority = 0, srulenumber = 0, ssemantic = 0, Ffsymi = 0, Finfos = 0, Farity = 0,
      topglopflag = 0, varindex = 0, varsort = 0, Wherevar = 0, Wherestrat = 0,
      querysorti = 0, resultsorti = 0, mainstrati = 0;
  term cterm, Lhs, Rhs, bcond, ifterm, startwtt, checkwtt, whereterm;
  transrule *rrule = nullptr;
  struct namelist *rlst = nullptr;
  int strconstr = 0;
  struct strlist *strliststack[64] = {};
  int strliststacktop = 0;
  int rstack[64] = {};
  int rstacktop = 0;
  int strategytype = 0;
  char stringconst[STRLEN] = {};
  int stringconsti = 0;
  int achead = 0;
  int ainfos = 0;
  int acsymbol = 0;
  int ssyntactic = 0;
  int varn = 0;
  // the fixed-size stacks below are pushed with REFPUSH (checked)
  struct wherelist *wlist = nullptr;
  struct wherelist *wliststack[50] = {};  int wliststacki = 0;
  term bcondstack[50];  int bcondstacki = 0;
  struct branch *listbranchstack[50] = {}; int listbranchstacki = 0;
  struct tseq *switchstack[50] = {}; int switchstacki = 0;
  struct branch *branchstack[50] = {}; int branchstacki = 0;
  struct WHEREbranches *WHEREbranchesstack[50] = {}; int WHEREbranchesstacki = 0;
  lexem whtype;
};
static RefParserState rp;

// The stacks of the .ref parser have fixed sizes: a .ref file nested more
// deeply fails with a message instead of overflowing them.
[[noreturn]] static void ref_stack_overflow(const char *stack)
{
  sterr << "\n[import] " << stack << " overflowed: the .ref file is nested too deeply"
        << "\n\t fatal\n";
  failexit();
}
// REFPUSH(s, t, v) pushes v on the stack rp.s of top rp.t
#define REFPUSH(stack, top, value) do { \
    if (rp.top >= (int)(sizeof(rp.stack) / sizeof(rp.stack[0]))) ref_stack_overflow(#stack); \
    rp.stack[rp.top++] = (value); } while (0)


int atermsemact(int n,lexem l,lstream * /*f*/)
{
  struct namelist *nl;
  struct sgrammrule *rule = NULL; /* initialised to avoid warning */
  // stout << "@" << n << "@\n";
  if (n >= 0 && n < 300) {
   switch (n) {
    case 0: 
      rp.cterm.stinit(); rp.Lhs.stinit(); rp.Rhs.stinit(); rp.bcond.stinit();
      rp.strliststacktop = 0; rp.strliststack[rp.strliststacktop] = NULL;
      break;
    case 1:  rp.lex.crtypelex(rp.nval); rp.ar++;
      //sterr << "type(" << nval << ")";
      break;
    case 2:  rp.lex.crcharlex(rp.nval); 
      //sterr << "char(" << nval << ")";
      break;
    case 3:  rp.lex.crnumlex(rp.nval); 
      //sterr << "num(" << nval << ")";
      break;
    case 4:  
      //sterr << "#" << n << "#"; //lex.crstringlex(nval); 
      //lex.dump(); sterr << ".";
      break;
    case 5:  rp.lex.cridlex(rp.nval); 
      //sterr << "id(" << nval << ")";
      break;
    case 6: rp.lex.crblanklex();
      break;
    case 7:  rp.nval = l.numval();
      break;
    case 8:   rp.nval =  -l.numval();
      break;
    case 9:
      rp.ssyntactic = rp.nval;
      break;
    case 10:
      rp.acsymbol = rp.nval; rp.ar =0;
      break;
    case 11:  
	if (rp.topglopflag & INTOPGRAM) {
	  rule = topgrammar->
	         addrule(rp.lefside,rp.spriority+(rp.ssyntactic*(RPRIORITYMSK+1)),
			 RGLOP /*???*/ , rp.srulenumber);
	  // stout << "#1"; dumpgrrule(rule);
	}
	if (rp.topglopflag & INGLOBGRAM) {
	  rule = globtermgr.
	    addrule(rp.lefside,rp.spriority+(rp.ssyntactic*(RPRIORITYMSK+1)),
		    RGLOP /*???*/ , rp.srulenumber); }
	rule->semantic =rp.ssemantic;
	//stout << "#2"; dumpgrrule(rule);



//	if ((topglopflag & INTOPGRAM) || (fsymtab[srulenumber].textform() == NULL))
        if ( rp.ssyntactic & PRINTABLE)
	{
	  fsym s(rp.ar,rule,rp.acsymbol);
	  //s.dump();
	  //stout << "setsemantic " << ssemantic << "\n";
	  s.set_semantic(rp.ssemantic); 
	  fsymtab[rp.srulenumber] = s;
	  if (rp.srulenumber >= fsymtabi) fsymtabi = rp.srulenumber+1;
	} 
      break;
    case 13:  rp.spriority = rp.nval;
      break;
    case 14:  rp.srulenumber = rp.nval;
      break;
    case 15:  rp.ssemantic = rp.nval;
      break;
    case 16: rp.lefside.crtypelex(rp.nval);
      break;
    case 17: rp.topglopflag = rp.nval;
      break;
    case 18:
      rp.varindex = rp.nval;
      break;
    case 19:
      rp.varsort = rp.nval; rp.varsortlex.crtypelex(rp.varsort);
      break;
    case 20:  rp.cterm.crvar(rp.varindex,rp.varsortlex);
      //cterm.write(stout); stout << "\n";
      //stout << "var " << nval << "\n";
      break;
    case 21:  rp.cterm.crstterm(rp.nval,TNUMBER);
      //cterm.write(stout); stout << "\n";
      //stout << "int " << nval << "\n";
      break;
    case 22:  rp.cterm.crstterm(rp.nval,TIDENT);
      //cterm.write(stout); stout << "\n";
      break;
    case 23:
      rp.stringconst[rp.stringconsti++] = 0;
      rp.cterm.crststring(strdup(rp.stringconst));
      rp.stringconsti = 0;
      break;
    case 24:  
      // cterm.crterm_reverse(Ffsymi,Farity);
      rp.cterm.crterm_reverse(rp.Ffsymi,fsymtab[rp.Ffsymi].arity());

      //cterm.write(stout); stout << "\n";
      //stout << "@" << Ffsymi << "," << Farity << "\n";
      break;
    case 25: rp.Ffsymi = rp.nval;
      break;
    case 26: rp.Finfos = rp.nval;
      break;
    case 27: rp.Farity = rp.nval;
      break;
    case 28: 
      rp.whereterm.popt();
      break;
    case 29: rp.Wherestrat = rp.nval;
      break;
    case 30:  
      rp.ifterm.popt();
      // rrule->addwhere(1,IFVARN,NULL,ifterm,booltype);
      addwheretolist(1,IFVARN,NULL,rp.ifterm,booltype,&rp.wlist);
      break;
    case 31:  
      {
	rp.ifterm.popt();
	if (!rp.whereterm.is_variable(&rp.Wherevar)) {
	  sterr << "[fatal] in where parsing \n"; failexit(); }
	addwheretolist(1,rp.Wherevar,
		       (rp.Wherestrat==-1)?((strategy**)NULL):
		       trrules.getstrategyadr_refs(rp.Wherestrat),rp.ifterm,
		       rp.whtype,&rp.wlist);
      }
      break;
    case 45: // PWHERE
      {
	rp.ifterm.popt();
	addpatternwheretolist(1,(rp.Wherestrat==-1)?((strategy**)NULL):
			      trrules.getstrategyadr_refs(rp.Wherestrat),
			      rp.whereterm, rp.ifterm,rp.whtype.typeval(),
			      &rp.wlist);
      }	  
      break;	 
    case 46:
      rp.whtype.crtypelex(rp.nval);
      break;
    case 32:
      rp.stringconst[rp.stringconsti++] = rp.nval;
      if (rp.stringconsti >= STRLEN) {
	sterr << "\n[IMPORT] string constant too long\n"; failexit(); }
      break;
    case 33:
      rp.varn = rp.nval;
      break;
    case 34:  
	rp.Lhs.popt();
	// stout << "\n<LHS=";       // Lhs.dump();      Lhs.write(stout); stout << ">"; 
      break;
    case 35: 
      {
	/*term rlabel;*/
        rp.Rhs.popt();
	// stout << "<RHS=";       Rhs.dump(); // Rhs.write(stout);     stout << ">"; 
	/*
	if (!batch) {
	  if (rlab != -1 && amodule != -1) {
	    stout << "Import rule " << trrules.rulename(rlab)
	          << " form module " << import.ide(amodule)
		  << "\n"; }
	}
	rrule = trrules.addrule(((rlab == -1)?(char*)NULL:trrules.rulename(rlab)),
			varn, Lhs,Rhs,
			amodule, ainfos,
			      NULL,
			achead,rlabel,NULL);
    rlab = -1; amodule = -1;
			*/
   }
      break;
    case 41: {
	term rlabel;
        rp.Rhs.popt();
	// stout << "<RHS=";       Rhs.dump(); // Rhs.write(stout);     stout << ">"; 
	if (!batch) {
	  if (rp.rlab != -1 && rp.amodule != -1) {
	    stout << "Import rule " << trrules.rulename(rp.rlab)
	          << " form module " << import.ide(rp.amodule)
		  << "\n"; }
	}
	rp.rrule = trrules.addrule(((rp.rlab == -1)?(char*)NULL:trrules.rulename(rp.rlab)),
			rp.varn, rp.Lhs,rp.Rhs,rp.amodule, rp.ainfos, NULL,
			rp.achead,rlabel,NULL);
	rp.rlab = -1; rp.amodule = -1;
	break;
    }
    case 36:
      rp.achead = rp.nval;
      break;
    case 37:
      rp.rlab = rp.nval;
      break;
    case 38:
      rp.amodule = rp.nval;
      break;
    case 39:
      rp.ainfos = rp.nval;
      break;
    case 40:
      if (rp.topglopflag & INTOPGRAM)
	topgrammar->addsymbol(rp.lex);
      if (rp.topglopflag & INGLOBGRAM)
	globtermgr.addsymbol(rp.lex);
      break;
    case 126:
      REFPUSH(wliststack, wliststacki, rp.wlist); rp.wlist = NULL;
      break;
    case 127:
      REFPUSH(WHEREbranchesstack, WHEREbranchesstacki, NULL);
      break;
    case 128:
        { struct WHEREbranches *brlist;
	NNEW(brlist, struct WHEREbranches);
	brlist->wherebranch = rp.wliststack[--rp.wliststacki];
	brlist->next = rp.WHEREbranchesstack[--rp.WHEREbranchesstacki];
	REFPUSH(WHEREbranchesstack, WHEREbranchesstacki, brlist);
	}
      break;
    case 129:  // TRY
       { struct WHEREbranches *brlist;
        rp.wlist = rp.wliststack[--rp.wliststacki];
	brlist = rp.WHEREbranchesstack[--rp.WHEREbranchesstacki];
            addtrywheretolist(1,brlist,&rp.wlist);
	}	
      break;
    case 130:
    case 131: {
      struct tseq *tseq = NULL;
      AALLOS(tseq, struct tseq);
      tseq->is_case = (n == 131);
      tseq->seq = rp.wliststack[--rp.wliststacki];
      if (tseq->is_case == 0) {
	NNEW(tseq->u.one_branch.result,term);
	*(tseq->u.one_branch.result) = rp.Rhs;
      } else {
	tseq->u.more_branches.brlist = rp.listbranchstack[--rp.listbranchstacki];
      }
      REFPUSH(switchstack, switchstacki, tseq); 
     }
      break;
    case 134:
      rp.bcond.popt();
      REFPUSH(bcondstack, bcondstacki, rp.bcond); rp.bcond.stinit();
      break;
    case 135:
      { struct branch *sbranch; /*,**p;*/
        AALLOS(sbranch, struct branch);
	sbranch->tseq = rp.switchstack[--rp.switchstacki];
	sbranch->test = rp.bcondstack[--rp.bcondstacki];
	REFPUSH(branchstack, branchstacki, sbranch);
      }
     break;
    case 133:
	//wherelisdump(stout,wlist,0);
	rp.rrule->setwheres(rp.wlist);
	rp.wlist = NULL;
	break;
    case 136:
	{ term rlabel;
	  struct tseq *tseq;
	// stout << " Right hand side \n"; dump_tseq(stout, 10, tseq);
	tseq = rp.switchstack[--rp.switchstacki];
       	if (tseq->is_case == 0) {
  	  rp.rrule = trrules.addrule(((rp.rlab == -1)?(char*)NULL:trrules.rulename(rp.rlab)),
		  	  rp.varn, rp.Lhs,*(tseq->u.one_branch.result),
			  rp.amodule, rp.ainfos,
			      NULL,
			rp.achead,rlabel,tseq->seq);
	} else {
	  term noterm;
	  noterm.stinit();
  	  rp.rrule = trrules.addrule(((rp.rlab == -1)?(char*)NULL:trrules.rulename(rp.rlab)),
		  	  rp.varn, rp.Lhs,noterm,
			  rp.amodule, rp.ainfos,
			      tseq,
			rp.achead,rlabel,NULL);
	}
	rp.rlab = -1; rp.amodule = -1;
	}
      break;
    case 137 :
      REFPUSH(wliststack, wliststacki, rp.wlist); rp.wlist = NULL;
      break;
    case 138:
      REFPUSH(listbranchstack, listbranchstacki, NULL);
      break;
   case 139: 
        { struct branch *branchlist = NULL;
	struct branch  *br; /* **p*/
	branchlist = rp.listbranchstack[--rp.listbranchstacki];
	
	br = rp.branchstack[--rp.branchstacki];
	br->next = branchlist;
	branchlist = br;

	REFPUSH(listbranchstack, listbranchstacki, branchlist);

	/***
	p = &branchlist;
	while (*p) {
	  p = &((*p)->next);
	}
	*p = branchstack[--branchstacki];
	(*p)->next = NULL;
	REFPUSH(listbranchstack, listbranchstacki, branchlist); 
	***/
	}
      break;
    case 140:
      sourcetypei = rp.nval; sourcetype.crtypelex(sourcetypei);
      break;
    case 141: qresulttypei = rp.nval; qresulttype.crtypelex(qresulttypei);
      break;
    case 142: mainstrategy = rp.nval; 
      break;
    case 143: startwith.popt();
      // stout << "startwith = "; startwith.write(stout); stout << "\n";
      break;
    case 144:  checkwith.popt();
      // stout << "checkwith = "; checkwith.write(stout); stout << "\n";
      break;
    case 159:
      ld.actstrategy->setprocmaxn(ld.calledstr);
      ld.actstrategy->settypeof(rp.strategytype);
      ld.actstrategy->setname(STRCALL,impmoduli);
      break;                            // UUU differece between semact.c
    case 160:				// another element. strategy
      appactstrat();
      break;
    case 163:
      grow_strstack(ld.strstacki+3);
      ld.strstack[ld.strstacki+1]=ld.strstack[ld.strstacki+2]=NULL;
      ld.strstacki+=2;
      break;
    case 164:
      ld.actstrategy->setname(STRNAMEREPEAT,impmoduli); 
      ld.actstrategy->settypeof(rp.strategytype);
      ld.actstrategy->setsubst(ld.strstack[ld.strstacki-1]);
      ld.strstacki-=2;
      break;
    case 165:
      ld.actstrategy->setname(STRNAMEITERATE,impmoduli);
      ld.actstrategy->settypeof(rp.strategytype);
      ld.actstrategy->setsubst(ld.strstack[ld.strstacki-1]);
      ld.strstacki-=2;
      break;
    case 172:
      ld.actstrategy->setname(STRNAMEONE,impmoduli);
      ld.actstrategy->settypeof(rp.strategytype);
      ld.actstrategy->setnamelist(rp.rlst);
      // UUU see 160:      appactstrat();
      break;
    case 173:
      ld.actstrategy->setname(STRNAMEDONTCARE,impmoduli);
      ld.actstrategy->settypeof(rp.strategytype);
      ld.actstrategy->setnamelist(rp.rlst);
      // UUU see 160:      appactstrat();
      break;
    case 174:
      ld.actstrategy->setname(STRNAMEDONTKNOW,impmoduli);
      ld.actstrategy->settypeof(rp.strategytype);
      ld.actstrategy->setnamelist(rp.rlst);
      // UUU see 160:  appactstrat();
      break;
    case 171:
      ld.actstrategy->setname(STRNAMEONE2,impmoduli);
      ld.actstrategy->settypeof(rp.strategytype);
      ld.actstrategy->setstl(ld.strlstack[ld.strlstacki-2]);
      ld.strstacki-=2;
      ld.strlstacki-=2;
      break;
    case 175:
      ld.actstrategy->setname(STRNAMEDONTCARE2,impmoduli);
      ld.actstrategy->settypeof(rp.strategytype);
      ld.actstrategy->setstl(ld.strlstack[ld.strlstacki-2]);
      ld.strstacki-=2;
      ld.strlstacki-=2;
      break;
    case 176:
      ld.actstrategy->setname(STRNAMEDONTKNOW2,impmoduli);
      ld.actstrategy->settypeof(rp.strategytype);
      ld.actstrategy->setstl(ld.strlstack[ld.strlstacki-2]);
      ld.strstacki-=2;
      ld.strlstacki-=2;
      break;
    case 177:
      { struct strlist *ss;
      AALLOS(ss , struct strlist);
      ss->next = NULL;
      ss->str = ld.strstack[ld.strstacki-1];
      ld.strstack[ld.strstacki-1]=ld.strstack[ld.strstacki]=NULL;
      //	  NNEW(actstrategy ,strategy);
      grow_strlstack(ld.strlstacki+2);
      ld.strlstack[ld.strlstacki++] = ss;
      ld.strlstack[ld.strlstacki++] = ss;
      }
      break;
    case 178:
      { struct strlist *ss;
      AALLOS(ss , struct strlist);
      ss->next = NULL;
      ss->str = ld.strstack[ld.strstacki-1];
      ld.strstack[ld.strstacki-1]=ld.strstack[ld.strstacki]=NULL;
      //	  NNEW(actstrategy ,strategy);
      ld.strlstack[ld.strlstacki-1]->next = ss;
      ld.strlstack[ld.strlstacki-1] = ss;
      }
      break;
    case 179:
      rp.strindex = rp.nval;
      ld.strstacki=1;
      ld.strstack[0]= ld.strstack[1]= NULL;
      NNEW(ld.actstrategy ,strategy);
      break;
    case 180:  
      rp.strategytype = rp.nval;
      break;
    case 181:
      trrules.settypeofstrategy_defs(rp.strindex,rp.strategytype);
      // strstack[strstacki] -> dump();
      ld.strstack[0]->settypeof(rp.strategytype);
      trrules.setstrategy_defs(rp.strindex,ld.strstack[0]); 
      if (!batch) 
	stout << "Import strategy " << trrules.strategyname_defs(rp.strindex)
	      << "\n";
      rp.strindex_refs = trrules.strategyindex_refs(trrules.
			strategyname_defs(rp.strindex));
      trrules.set_strategies_cross(rp.strindex_refs,rp.strindex,ld.strstack[0]);
      break;
   case 182:
     ld.calledstr = rp.nval;
     break;
   case 183:
   case 184:
     rp.stringconst[rp.stringconsti] = 0;
     //stout << stringconst  << "\n";
     //stout << ainfos << "\n";
     //stout << typet.ide(strategytype) << "\n";
     rp.stringconsti = 0;

     ld.actstrategy->setname((n == 183) ? 
			  STRNAMEDCPROCESSCALL:STRNAMEDKPROCESSCALL,
			  impmoduli);
     ld.actstrategy->settypeof(rp.strategytype);
     ld.actstrategy->setproctype(rp.strategytype);
     ld.actstrategy->setprocname(rp.stringconst);
     ld.actstrategy->setprocmaxn(rp.ainfos);

      ld.actstrategy->setprocgr(&globtermgr);
//      addstandards(globtermgr);
//      actstrategy->setprocgr(topgrammar);

     break;
   case 190:
     rp.rlst = NULL;
     break;
   case 191:
     AALLOS(nl, struct namelist);
     nl->strname = rp.rstack[rp.rstacktop--];
     nl->next = rp.rlst;
     rp.rlst = nl;
     break;
   case 192:
     if (rp.rstacktop+1 >= 64) ref_stack_overflow("rstack");
     rp.rstack[++rp.rstacktop] = rp.rlab;
     break;
   case 195:  // strategy fail
     ld.actstrategy->setname(STRFAIL,impmoduli);
     ld.actstrategy->settypeof(rp.strategytype);
     break;
   case 196:
     ld.actstrategy->setname(STRIDENTITY,impmoduli);
     ld.actstrategy->settypeof(rp.strategytype);
     break;
   case 197:
     ld.actstrategy->setname(STRMETA,impmoduli);
     ld.actstrategy->settypeof(rp.strategytype);
     break;
   default:
      sterr << "[atermsemact] undefined semantic action " << n 
	    << "\n"; failexit(); 
   }
  }
  return(NORMCONT);
}

term redTerm;
int  redStrategyIndex = -1;
int  redStrategyIndex_refs = -1;

int reducesemact(int n,lexem l,lstream *f)
{
  switch (n) {
    case 111:   // reduce-term
      redTerm = rp.cterm;
      // stout << "REDUCE TERM = "; redTerm.write(stout); stout << "\n";
      //transred(redStrategyIndex,redTerm,0);  
      break;
    case 112:  // reduce-strategy
      // stout << "REDUCE STRATEGY = "; strstack[0]->dump(); stout << "\n";

if (!batch) { stout << "REDUCE_STRATEGY " << rp.strategytype << ":" << impmoduli << "\n"; }

      redStrategyIndex = trrules.strategyindex_defs(
           attach_type_mod("REDUCE_STRATEGY",rp.strategytype,impmoduli),RGLOP);

if (!batch) { stout << "REDUCE_STRATEGY done\n"; }

      trrules.settypeofstrategy_defs(redStrategyIndex,rp.strategytype);
      // strstack[strstacki] -> dump();
      ld.strstack[0]->settypeof(rp.strategytype);
      trrules.setstrategy_defs(redStrategyIndex,ld.strstack[0]); 
      if (!batch) {
	stout << "Import reduce strategy " << trrules.strategyname_defs(redStrategyIndex)
	      << "\n"; }
      redStrategyIndex_refs = trrules.strategyindex_refs(trrules.
			strategyname_defs(redStrategyIndex));
      trrules.set_strategies_cross(redStrategyIndex_refs,redStrategyIndex,ld.strstack[0]);
      break;
    case 113:  
      impmoduli = rp.amodule;
      rp.strategytype = rp.nval;
      sourcetypei = rp.nval; sourcetype.crtypelex(sourcetypei);
      qresulttypei = rp.nval; qresulttype.crtypelex(qresulttypei);
      // ** init
	      //stout << "MODULE= " << impmoduli << "\n";
      ld.strstacki=1;
      ld.strstack[0]= ld.strstack[1]= NULL;
      NNEW(ld.actstrategy ,strategy);
      break;
    default:
      atermsemact(n,l,f);
      break;
  }
  return(NORMCONT);
}


#endif

