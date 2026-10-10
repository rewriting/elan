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

#include <string.h>
#include "command.h"
#include "codes.h"

#include "module.h"
#include "compiledefs.h"

#define FREERULE(rule) { CFRE(rule->rside);  CFRE(rule);}
static unsigned usedtype[NTYPES/NBITS+1];

grammar::grammar()
{ int i;
  for (i=0; i<NNONTERMINALS; i++) {
    nontt[i]=NULL;
    alastnontt[i] = &(nontt[i]);
  }
  buffi=ntbuffi=0; 
  buff[buffi].crendofstreamlex();
}

grammar::~grammar()
{ //if (warnings) {
//    sterr << "[warning] ~grammar() sorry not yet implemented, but don't panic\n";
  //}
}

struct sgrammrule *grammar::addrule(lexem &leftside, int priority, int infos, int num)
{ 
  return addrule_(buff,&buffi,leftside,priority,infos,num);
}

struct sgrammrule *grammar::addrule_(lexem *ebuff,int *ebuffi,lexem &leftside, int priority, int infos, int num)
{ int i;
  lexem *rs;  
  if (num<0) {
    sterr <<"[grammar.c] adding variable by addrule";
    interr();
  }
  AALLOSS(rs ,(*ebuffi)+1,lexem);
  for (i=0; i<=*ebuffi; i++) rs[i]=ebuff[i];
  *ebuffi=ntbuffi=0;
  return(addrul(leftside,rs,priority,infos,num));
}

void grammar::addsymbol(lexem &l) { addsymbol_(buff,&buffi,l); }
void grammar::addsymbol_(lexem *ebuff,int *ebuffi,lexem &l)
 {
  if ((*ebuffi)+2 >= MLENGRRULE) {
     sterr << "length of rule is overflow over MLENGRRULE="
          << MLENGRRULE << "\n\t FATAL !!!\n";
     failexit();
  }
  ebuff[(*ebuffi)++]=l; ebuff[(*ebuffi)].crendofstreamlex();
}

struct sgrammrule * grammar::addrul(lexem &leftside,lexem *body,int priority, int infos ,int num)
{
  struct sgrammrule *gr;
  struct grammrulelist *gl; /*,**ggl;*/
  AALLOS(gr,struct sgrammrule);
  gr->leftside= leftside;
  gr->priority=priority;
  gr->rulenumber=num;
  gr->rside = body;
  gr->semantic=0; //31
  gr->defstrat=0;
  gr->fsymcode = -1;
  AALLOS(gl ,struct grammrulelist);
  gl->r = gr;
  gl->infos=infos;
/* 
  gl->next = nontt[leftside.typeval()];
  nontt[leftside.typeval()] = gl;
*/

  gl->next= NULL;
  *(alastnontt[leftside.typeval()]) = gl;
  alastnontt[leftside.typeval()] = &(gl->next);

/*
  gl->next= NULL;
  ggl =  & (nontt[leftside.typeval()]);
  while (*ggl != NULL) ggl = &((*ggl)->next);
  *ggl = gl;
 */
  return(gr);
}


void grammar::addnont(lexem &l) { addnont_(buff,&buffi,l); }
void grammar::addnont_(lexem *ebuff,int *ebuffi,lexem &l)
{
  int ntbuffi = 0;
  while (ntbuffi<(*ebuffi) && ebuff[ntbuffi].isnotendofstream()) ntbuffi++;
  if (ebuff[ntbuffi].isnotendofstream()) 
     sterr<<"[grammar::addnont] something is wrong, int.err.\n";
  ebuff[ntbuffi++]=l;
  if (ntbuffi>*ebuffi) { *ebuffi=ntbuffi; ebuff[*ebuffi].crendofstreamlex(); }
}


// Pour ELAN seul
#include "strategy.h"
void grammar::combine()
{
  int i,j,k;
  struct grammrulelist *gr1, *gr2;
  struct sgrammrule *gr;
  for(k = 0; k < inlinecodesi; k++) {
    i = inlinecodes[k].from; j = inlinecodes[k].to;
    if (nontt[i] && nontt[j])
      for(gr1 = nontt[i]; gr1; gr1 = gr1->next) 
	for(gr2 = nontt[j]; gr2; gr2 = gr2->next) {

//	  if (gr1->infos != gr2->infos) stout << "infos" 
//        << gr1->infos << "." << gr2->infos << "\n";
//          if (gr1->r->rulenumber == gr2->r->rulenumber) stout << 
//                               "aliased" << gr1->r->rulenumber << "+" 
//                               << gr2->r->rulenumber << "\n";
	  if ((gr1->r->rulenumber == gr2->r->rulenumber || // aliased
               gr1->infos == gr2->infos)  && gr_compatible(gr1->r,gr2->r) 
	      && (!fsymrule_exists(gr1->r,gr2->r))) {
	    fsymrule_add(gr1->r,gr2->r);
            gr = add_fsymrule(gr1->r,gr2->r,gr1->infos);
	    add_frule(gr,gr1->r,gr2->r);
	  }
	}
     }
}


/*
void grammar::addrw(lexem l) 
{
  reswtab->addn(l.idval());
}

void grammar::removerw(lexem l) 
{
  reswtab->removen(l.idval());
}
*/
struct sgrammrule * grammar::adddollarvarrule(lexem &leftside,lexem &l,int priority, int infos ,int num)
{ lexem *rs;  
  AALLOSS(rs ,3,lexem);
  rs[0].crcharlex('$'); rs[1] = l; rs[2].crendofstreamlex();
  return(addrul(leftside,rs,priority,infos,num));
}

struct sgrammrule * grammar::addvarrule(lexem &leftside,lexem &l,int priority, int infos ,int num)
{ lexem *rs;  
  AALLOSS(rs ,2,lexem);
  rs[0] = l; rs[1].crendofstreamlex();
  return(addrul(leftside,rs,priority,infos,num));
}


void grammar::deleterule(struct sgrammrule *rule)
{
  struct grammrulelist **gr,*dr;
  int ind;
  ind = rule->leftside.typeval();
  gr = & nontt[ind];
  while (*gr!=NULL && (*gr)->r !=rule)
    gr= & (*gr)->next;
  if (*gr != NULL && (*gr)->r ==rule) {
    dr = *gr;
    *gr = (*gr)->next;
    if (*gr == NULL) alastnontt[ind] = gr;
    FREERULE(dr->r); CFRE(dr);
  } 
  else
    sterr << "[deleterule] deleting of nonexist rule, internal error !!";
}

void grammar::preprocdeleterule(struct sgrammrule *rule)
{
  struct grammrulelist **gr,*dr;
  int ind;

  sterr << "DEBUT grammar::deleterule\n";
  ind = rule->leftside.typeval();
  sterr << "ind="<<ind<<"\n";;
  gr = & nontt[ind];
  sterr << "gr=";writegrrule(sterr,(*gr)->r,&typet);

  sterr << "\tDEBUT boucle\n";
  while (*gr!=NULL && (*gr)->r !=rule)
    {
      sterr << "\t";writegrrule(sterr,(*gr)->r,&typet);
      gr= & (*gr)->next;
    }

  sterr << "\tDEBUT test\n";

  if (*gr != NULL && (*gr)->r ==rule) {
    dr = *gr;
    *gr = (*gr)->next;
    if (*gr == NULL)
      alastnontt[ind] = gr;
    sterr << "\tOn detruit : ";writegrrule(sterr,dr->r,&typet);
    FREERULE(dr->r); CFRE(dr);
  } 
  else
    sterr << "[preprocdeleterule] deleting of nonexist rule, internal error !!";

  sterr << "FIN grammar::deleterule\n";

}

int grammar::rbodyeq(lexem *b)
{
  lexem *bb;
  bb=buff;
  while(*b==*bb && b->isnotendofstream() && bb->isnotendofstream()){b++;bb++;}
  return(b->isendofstream() && bb->isendofstream());
}

int grammar::hardcompatible(lexem *b)
{
  lexem *bb;
  bb=buff;
  while(((b->terminal() && bb->terminal()) || 
	   (b->nonterminal() && bb->nonterminal() && 
	    ((ISBUILTIN((*b)))==(ISBUILTIN((*bb)))))) && 
             b->isnotendofstream() && bb->isnotendofstream()){
    b++;bb++;
    
  }
  return(b->isendofstream() && bb->isendofstream());
}


int grammar::addalias(struct sgrammrule *rule, lexem &leftside)
{ struct grammrulelist *g;
  lexem *lp;
  lp= rule->rside;
  while (lp->isnotendofstream()) {
    while (lp->terminal() && (lp->isnotendofstream())) lp++;
    if (lp->isnotendofstream()) {
      if ((*lp)!=internIntType && (*lp)!=internIdentType) addnont(*lp);
      lp++;
    }
  }
  g= nontt[leftside.typeval()];
  while (g!=NULL && (( ! rbodyeq(g->r->rside)) || g->r==rule)) {
    g=g->next;
  }
  buffi=ntbuffi=0;
  if (g==NULL) return(0);
  rule->rulenumber = g->r->rulenumber; 
  /***/ rule->semantic = g->r->semantic;
  return(1);
}

int grammar::addhardalias(struct sgrammrule *rule, lexem &leftside)
{ struct grammrulelist *g;
/*lexem *lp;*/
  g= nontt[leftside.typeval()];
  if (!hardcompatible(rule->rside)) {
    sterr << "\n[addhardalias] profil of hardalias is not hardcompatible with the symbol";
    return(0);
  }
  while (g!=NULL && (( ! rbodyeq(g->r->rside)) || g->r==rule)) {
    g=g->next;
  }
  buffi=ntbuffi=0;
  if (g==NULL) return(0);
  rule->rulenumber = g->r->rulenumber; 
  return(1);
}

void grammar::markusedtype(int winfo)
{ int i,j;
  struct grammrulelist *gr;
  lexem ll;
  for (i=0; i<=NTYPES/NBITS; i++) usedtype[i]=0;
  for (i=0; i<NNONTERMINALS; i++) 
    if (nontt[i]!=NULL) {
       gr=nontt[i];
       while (gr!=NULL) { 
         if ((gr->infos & RSTATMSK) == winfo) {
           j=0;
           while (gr->r->rside[j].isnotendofstream()) {
           ll=gr->r->rside[j];
           if (ll.nonterminal()) {
             SETBIT(usedtype,(ll.typeval()));
           }
           j++;
         }
         SETBIT(usedtype,(gr->r->leftside.typeval()));
         }
         gr=gr->next;
       }
    }
}


void grammar::genglobgr(ochstream &gout,char *mn,stringtab *typ)
{ int i,j;
  markusedtype(RGLOP);
  gout << "module " << mn;
  j=0;
  for(i=0; i<NTYPES; i++)
    if (ISSETBIT(usedtype,i)) {
      if (!(j%5)) gout << "\n";
      if (j==0) gout << "sort "; else gout << " ";
      gout << typ->ide(i);
      j++;
    } 
  if (j) gout << ";";
  write(gout,RGLOP,typ,"\noperators\nglobal\n","end");
  gout << "\nend\n";
}

void grammar::freetopl()      // free top list of grammar
{ int i; 
  struct grammrulelist *gr,*gr2;
  for (i=0; i<NNONTERMINALS; i++) {
    gr = nontt[i];
    while (gr!=NULL) {
      gr2 = gr; gr= gr->next; CFRE(gr2);
    }
    nontt[i]=NULL;
    alastnontt[i] = &(nontt[i]);
  }
}
 
// kind = 0    ... f(int,int)
// kind = 1    ... declaration
// kind = 2    ... f(v1,..vn)
// kind = 3    ... (t1)v1. ... (tn).vn.nil
// kind = 4    ... v1, ... ,vn
// kind = 5    ... Symbol[t1, ..., tn, t]
void out_symbol(ochstream &anymod, struct sgrammrule *actr, int kind)
{
int k;
int vn = 0;
char interp = ' ';
lexem nolexem;
nolexem.crendofstreamlex();

  if (kind == 5) anymod << "Symbol[";
  for(k=0;actr->rside[k] != nolexem;k++) {
    if (actr->rside[k].nonterminal()) {
      vn++;
      switch (kind) {
        case 0 : anymod 
		   << " " << typet.ide(actr->rside[k].typeval()) << " ";
	         break;
        case 1 : anymod << " var" << vn << ":" << 
		   typet.ide(actr->rside[k].typeval()) << "; ";
	         break;
        case 2 : anymod << " var" << vn << " ";
	         break;
        case 3 : anymod << "(" << typet.ide(actr->rside[k].typeval()) << ")" <<
		   "var" << vn << ".";
	         break;
        case 4 : anymod << interp << "var" << vn; 
                 interp = ',';
	         break;
        case 5 : anymod << interp << typet.ide(actr->rside[k].typeval());
                 interp = ',';
	         break;
      }
    }
    else {
      if (kind == 0 || kind == 2) anymod << actr->rside[k].alfsy(); }
  }
  if (kind == 1) { 
    if (!vn) anymod << "hovno : Any; "; }
  else if (kind == 3) anymod << "nil ";
  else if (kind == 5) anymod << interp << typet.ide(actr->leftside.typeval()) << "]" ;
}


int is_any_type(int t)
{
int i;
  for(i=0; i < ld.anysi; i++) if (t == ld.anys[i]) return 1;
  return 0;
}

int is_any_symbol(struct sgrammrule *actr)
{
int k;
lexem nolexem; nolexem.crendofstreamlex();

  if (!is_any_type(actr->leftside.typeval())) return 0; 
  for(k=0;actr->rside[k] != nolexem;k++) {
    if (actr->rside[k].nonterminal()) {
      if (!is_any_type(actr->rside[k].typeval())) return 0; }
  }
  /* print-out
  stout << "\n................ (";
  for(k=0;actr->rside[k] != nolexem;k++) {
    if (actr->rside[k].nonterminal()) {
      stout << " " << typet.ide(actr->rside[k].typeval()); } 
    else
      stout << " " << actr->rside[k].alfsy(); }
  if (actr->leftside.nonterminal())
    stout << ")" << typet.ide(actr->leftside.typeval()) << "\n";
  else
    stout << ")" << actr->leftside.alfsy() << "\n";
  */
  return 1;
}

void grammar::anycode_rules(ochstream &anymod)
{
  int j;
  struct grammrulelist *gr;
  struct sgrammrule *actr;

  for (j=0; j<NNONTERMINALS; j++) {
    for (gr = nontt[j]; gr!=NULL; gr = gr->next) {
     actr = gr->r;
     if ( (gr->infos & (RIMPORTBIT|RGLOP)) == RGLOP &&
	  is_any_symbol(actr)) {
      // explode ----------------------------------
      anymod << "\nrules  for list[Any]\n";
      out_symbol(anymod,actr,1); // declarations
      anymod << "\nglobal\n";
      anymod <<"  [] explode( (" << typet.ide(actr->leftside.typeval()) << ")";
      out_symbol(anymod,actr,2);  // f(v1,...,vn)
      anymod << " ) => ";
      out_symbol(anymod,actr,3);  // v1....vn.nil
      anymod << " end\n";
      anymod << "end\n";
      // implode -----------------------------------
      anymod << "\nrules  for Any\n";
      out_symbol(anymod,actr,1); // declarations
      anymod << "\nglobal\n";
      anymod << "  [] implode( ";
      out_symbol(anymod,actr,0);  // f(@,@)
      anymod << ",";
      out_symbol(anymod,actr,3);  // v1....vn.nil
      anymod << " ) => (" << typet.ide(actr->leftside.typeval()) << ")";
      out_symbol(anymod,actr,2);  // f(v1,...,vn)
      anymod << " end\n";
      anymod << "end\n";
      // Functor ------------------------------------
      anymod << "\nrules for Functor\n";
      out_symbol(anymod,actr,1); // declarations
      anymod << "\nglobal\n";
      anymod <<"  [] functor( (" << typet.ide(actr->leftside.typeval()) << ")";
      out_symbol(anymod,actr,2);  // f(v1,...,vn)
      anymod << " ) => ";
      out_symbol(anymod,actr,0);  // f(@,@)
      anymod << " end\n";
      anymod << "end\n";
     }
    }
  }
}

void grammar::anycode_ops(ochstream &anymod)
{
  int j;
  struct grammrulelist *gr;
  struct sgrammrule *actr;
  for (j=0; j<NNONTERMINALS; j++) {
    for (gr = nontt[j]; gr!=NULL; gr = gr->next) {
      actr = gr->r;
      if ( (gr->infos & (RIMPORTBIT|RGLOP)) == RGLOP &&
	   is_any_symbol(actr)) {
	anymod << "\t";
	out_symbol(anymod,actr,0);  // f('@','@')
	anymod << " : Functor;\n"; }
    }
  }
}

int grammar::anysymbol_exists()
{
  int j;
  struct grammrulelist *gr;
  for (j=0; j<NNONTERMINALS; j++) {
    for (gr = nontt[j]; gr!=NULL; gr = gr->next) {
      // stout << " infos = " << gr->infos << "\n"; 
      if ( (gr->infos & (RIMPORTBIT|RGLOP)) == RGLOP &&
	  is_any_symbol(gr->r)) return 1;
    }
  }
  return 0;
}

void grammar::any_code(char *anymodstr, char *anymodfname, const char *name)
{int i;
  ochstream anymod(anymodfname);

  anymod << "module " << anymodstr << "\n";
  anymod << "\nimport local Any list[Any] " << name << " ";
  for(i=0; i < ld.anysi; i++) anymod << " " << ANY_MOD << "[" << 
    typet.ide(ld.anys[i]) << "] ";
  anymod << "; \nend";
  anymod << "\noperators global\n";
  anycode_ops(anymod);
  anymod << "\nend\n";
  anycode_rules(anymod);
  anymod << "\nend\n";
}

int symbappl_equal(struct ilist *lst, struct sgrammrule *actr)
{
  lexem nolexem;
  int k;
  nolexem.crendofstreamlex();
  for(k=0;actr->rside[k] != nolexem;k++) {
    if (actr->rside[k].nonterminal()) {
      if (lst == NULL || actr->rside[k].typeval() != lst->i) 
	return 0; 
      else
	lst=lst->next; }
  }
  if (lst == NULL) return 0;
  if (lst->next != NULL) return 0;
  if (!(actr->leftside.nonterminal())) return 0;
  if (actr->leftside.typeval() != lst->i) 
    return 0; 
  return 1;
}

int is_symbappl_symbol(struct sgrammrule *actr)
{
  int i;
  for (i=0; i < ld.symbappli; i++) {
    if (symbappl_equal(ld.symbappl[i],actr))
      return 1;
  }
  return 0;
}

void grammar::symbapplcode_ops(ochstream &symbapplmod)
{
  int j;
  struct grammrulelist *gr;
  struct sgrammrule *actr;
  for (j=0; j<NNONTERMINALS; j++) {
    for (gr = nontt[j]; gr!=NULL; gr = gr->next) {
      actr = gr->r;
      if ( (gr->infos & (RIMPORTBIT|RGLOP)) == RGLOP &&
	   is_symbappl_symbol(actr)) {
	symbapplmod << "\t";
	out_symbol(symbapplmod,actr,0);  // f('@','@')
	symbapplmod << " :"; 
	out_symbol(symbapplmod,actr,5);  // Symbol[t1, ,tn,t]
	symbapplmod << " ;\n"; }
    }
  }
}

void grammar::symbapplcode_rules(ochstream &symbapplmod)
{
  int j;
  struct grammrulelist *gr;
  struct sgrammrule *actr;

  for (j=0; j<NNONTERMINALS; j++) {
    for (gr = nontt[j]; gr!=NULL; gr = gr->next) {
      actr = gr->r;
      if ( (gr->infos & (RIMPORTBIT|RGLOP)) == RGLOP &&
	  is_symbappl_symbol(actr)) {
      symbapplmod << "\nrules for " <<
	typet.ide(actr->leftside.typeval()) << "\n";
      out_symbol(symbapplmod,actr,1); // declarations
      symbapplmod << "\nglobal\n";
      symbapplmod <<"  [] ";
      out_symbol(symbapplmod,actr,0);  
      symbapplmod << " ( ";
      out_symbol(symbapplmod,actr,4);
      symbapplmod << " ) => ";
      out_symbol(symbapplmod,actr,2);  
      symbapplmod << " end\n ";
      symbapplmod << "end\n";
     }
    }
  }
}

int grammar::symbappl_exists()
{
  int j;
  struct grammrulelist *gr;
  for (j=0; j<NNONTERMINALS; j++) {
    for (gr = nontt[j]; gr!=NULL; gr = gr->next) {
      if ( (gr->infos & (RIMPORTBIT|RGLOP)) == RGLOP &&
	  is_symbappl_symbol(gr->r)) return 1;
    }
  }
  return 0;
}

void grammar::symbappl_code(char *symbapplmodstr, char *symbapplmodfname, const char *name)
{int i,j;
 struct ilist *lst;
  ochstream symbapplmod(symbapplmodfname);

  symbapplmod << "module " << symbapplmodstr << "\n";
  symbapplmod << "\nimport local " << name;
  for(i=0; i < ld.symbappli; i++) {
    for(j=-1,lst=ld.symbappl[i]; lst; lst=lst->next) j++;
    symbapplmod << " " << SYMBOL_MODNAME << "[" << j;
    for(lst=ld.symbappl[i]; lst; lst=lst->next)
      symbapplmod << "," << typet.ide(lst->i);
    symbapplmod << "] ";
  }
  symbapplmod << "; \nend\n";
  symbapplmod << "\noperators global\n";
  symbapplcode_ops(symbapplmod);
  symbapplmod << "\nend\n";
  symbapplcode_rules(symbapplmod);
  symbapplmod << "\nend\n";
}

void grammar::add_apply_code(int x, int y)
{
lexem le;
 struct sgrammrule *gr;
 le.crcharlex('['); addsymbol(le);
 le.crtypelex(add_strat_nont(x,y)); addsymbol(le);
 le.crcharlex(']'); addsymbol(le);
 le.crtypelex(x); addsymbol(le);
 le.crtypelex(y);
 gr=addrule(le,RNOPRIOR,RNOINFO,ld.actcode);
 dumpgrrule(gr);
 add_to_fsymtab(2,gr,APPLY_FLAG(0));
 if (inlinecodesi >= (int)inlinecodes.size()) inlinecodes.resize(inlinecodesi+1);
 inlinecodes[inlinecodesi].from = x;
 inlinecodes[inlinecodesi].to = y;
// inlinecodes[inlinecodesi].stratsort = add_strat_nont(x,y);
 inlinecodes[inlinecodesi].applycode = gr->rulenumber; 
 gr->defstrat = APPLY_FLAG(0);
 inlinecodes[inlinecodesi].inlin = -1;
 inlinecodes[inlinecodesi].inlineplus = -1;
 inlinecodes[inlinecodesi].let = -1;
 inlinecodesi++;
}

void grammar::lookinlinecode(int x, int y)
{ int i;
  struct grammrulelist *gr;
  struct sgrammrule *actr;
  lexem le0,le1,le2,le3,le4,le5,nolexem,le;
  lexem lle0,lle1,lle2,lle3,lle;
  lexem mle0, mle1, mle2;
                                          ///// THIS SHOULD BE OPTIMISED
  lle0.crcharlex('[');
  lle1.crtypelex(add_strat_nont(x,y));
  lle2.crcharlex(']'); 
  lle3.crtypelex(x); 
  lle.crtypelex(y);

  le0.crcharlex('['); le1.crtypelex(x); 
  le2.crcharlex('='); le3.crcharlex('>'); 
  le4.crtypelex(y); le5.crcharlex(']'); 
  le.crtypelex(add_strat_nont(x,y));

  mle0.cridlex("let"); mle1.cridlex("in");
  mle2.crtypelex(add_strat_nont(x,y));

  nolexem.crendofstreamlex();

//  inlinecodes[inlinecodesi].stratsort = add_strat_nont(x,y);
  inlinecodes[inlinecodesi].applycode = -1; 
  inlinecodes[inlinecodesi].inlin = -1;
  inlinecodes[inlinecodesi].let = -1;
  inlinecodes[inlinecodesi].inlineplus = -1;

  for (i=0; i<NNONTERMINALS; i++) {
    gr = nontt[i];
    while (gr!=NULL) {
      actr = gr->r;
      // []
      if (actr->leftside == lle &&
	  (actr->rside[0] != nolexem && actr->rside[0] == lle0) &&
	  (actr->rside[1] != nolexem && actr->rside[1] == lle1) &&
	  (actr->rside[2] != nolexem && actr->rside[2] == lle2) &&
	  (actr->rside[3] != nolexem && actr->rside[3] == lle3)) {
	// if (!batch) sterr << "ADD APPLY_CODE " << typet.ide(x) << "," << typet.ide(y) << " = " << actr->rulenumber << "\n";
	inlinecodes[inlinecodesi].applycode = actr->rulenumber; 
	gr->r->defstrat = APPLY_FLAG(0);
      }
      // [ x => y] : <x->y>
      if (actr->leftside == le &&
	  (actr->rside[0] != nolexem && actr->rside[0] == le0) &&
	  (actr->rside[1] != nolexem && actr->rside[1] == le1) &&
	  (actr->rside[2] != nolexem && actr->rside[2] == le2) &&
	  (actr->rside[3] != nolexem && actr->rside[3] == le3) &&
	  (actr->rside[4] != nolexem && actr->rside[4] == le4) &&
	  (actr->rside[5] != nolexem && actr->rside[5] == le5)) {
	inlinecodes[inlinecodesi].inlin = actr->rulenumber; }
      // [ x => vi:si => y] : <x->y>
      if (actr->leftside == le &&
	  /******************************************* modified for HK
	  (actr->rside[0] != nolexem && actr->rside[0] == le0) && // [
	  (actr->rside[1] != nolexem && actr->rside[1] == le1) && // x
	  (actr->rside[2] != nolexem && actr->rside[2] == le2) && // =
	  (actr->rside[3] != nolexem && actr->rside[3] == le3) && // >
	  //.....
	  (actr->rside[5] != nolexem && actr->rside[5] == le2) && // =
	  (actr->rside[6] != nolexem && actr->rside[6] == le3) && // >
	  (actr->rside[7] != nolexem && actr->rside[7] == le4) && // y
	  (actr->rside[8] != nolexem && actr->rside[8] == le5)) { // ]
	  ******************************************/
	  (actr->rside[0] != nolexem && actr->rside[0] == le0) && // [
	  (actr->rside[1] != nolexem && actr->rside[1] == le1) && // x
	  (actr->rside[2] != nolexem && actr->rside[2] == le2) && // =
	  (actr->rside[3] != nolexem && actr->rside[3] == le3) && // >
	  (actr->rside[4] != nolexem && actr->rside[4] == le4) && // y
	  //.....
	  (actr->rside[6] != nolexem && actr->rside[6] == le5)) { // ]
	inlinecodes[inlinecodesi].inlineplus = actr->rulenumber; }
      // let ... in <x->y>
      if (actr->leftside == le &&
	  (actr->rside[0] != nolexem && actr->rside[0] == mle0) &&
	  // .. .. . . ..
	  (actr->rside[2] != nolexem && actr->rside[2] == mle1) &&
	  (actr->rside[3] != nolexem && actr->rside[3] == mle2)) {
	inlinecodes[inlinecodesi].let = actr->rulenumber; }
      gr = gr->next; }
  }
  if (inlinecodes[inlinecodesi].applycode == -1 || 
      inlinecodes[inlinecodesi].inlin == -1 || 
      inlinecodes[inlinecodesi].let == -1 ||
      inlinecodes[inlinecodesi].inlineplus == -1) {
    sterr << "[strat] module " << 
      ((x==y)?STRAT_MODNAME1:STRAT_MODNAME2)<<"["<< typet.ide(x)
	<<","<< typet.ide(y) << "] was not imported\n";
    failexit(); }
}

int grammar::lookbuiltincode(int x, int y, lexem le, int *infos)
{ int i;
  struct grammrulelist *gr;
  struct sgrammrule *actr;
  lexem lle, nolexem;

  lle.crtypelex(add_strat_nont(x,y));
  nolexem.crendofstreamlex();
  for (i=0; i<NNONTERMINALS; i++) {
    for(gr = nontt[i]; gr!=NULL;  gr = gr->next) {
      *infos = gr->infos;
      actr = gr->r; // dumpgrrule(actr);
      if ((actr->leftside == lle &&
	  (actr->rside[0] != nolexem && actr->rside[0] == le) &&
	   (actr->rside[1] == nolexem))) {
	// stout << "##" << (actr->priority & RBINSTR);
	if (actr->priority & RBINSTR) {
	// dumpgrrule(actr);
	return actr->rulenumber; } } }
  }
  return -1;
}

void grammar::addgrammar(grammar &g,int which, int as_which)
{ int i;
  struct grammrulelist *gr,*gr2;
  struct sgrammrule *actr;
  /*lexem *l,nolexem;*/
  for (i=0; i<NNONTERMINALS; i++) {
    gr = g.nontt[i];
    while (gr!=NULL) {
      if ((gr->infos & RSTATMSK )== which)   {
//               it is rule for adding, test if it is yet in the grammar
        actr = gr->r;
        gr2 = nontt[i];  
        while (gr2!=NULL && gr2->r!=actr) gr2=gr2->next;
        if (gr2 == NULL) {                  //        it is not here, add it
          AALLOS(gr2,struct grammrulelist);
          gr2->r = gr->r;
          gr2->infos = as_which;
	  gr2->next = NULL;
	  *(alastnontt[i]) = gr2;
	  alastnontt[i] = &(gr2->next);
/*
          gr2->next = nontt[i];
          nontt[i] = gr2;
*/
	}
      }
      gr = gr->next;
    }
  }
}

/* ------------------- output funct. follows ----------------------- */

void dumpgrrule(struct sgrammrule *gr)
{ int j;
  stout << "[" << gr->rulenumber << "] ";
  stout << "pri(" << gr ->priority << ") ";

  if (gr->leftside.nonterminal())
    stout << typet.ide(gr->leftside.typeval());
  else
    stout << gr->leftside.alfsy();
  // Marian ... gr->leftside.dump();
  stout << " --> ";
  j=0;
  while (gr->rside[j].isnotendofstream()) {
    if (gr->rside[j].nonterminal()) 
      stout << " " << typet.ide(gr->rside[j].typeval()); 
    else
      stout << " " << gr->rside[j].alfsy(); 
    // Marian ... gr->rside[j].dump();
    j++;
  }
  stout << "\n";
}


void grammar::dump()
{ struct grammrulelist *gr;
  int i;
  /*
  stout << "\n[buff dump]\n buffi = " << 
    buffi << "ntbuffi = " << ntbuffi << "\n buff = ";
  for (i=0; i<buffi; i++) buff[i].dump();
  stout << "[end of buff dump]\n";
  */
//  stout << "\n[grammar dump] startsym = "; startsym.dump();
  stout << "\nrules :\n";
  for (i=0; i<NNONTERMINALS; i++) 
    if (nontt[i]!=NULL) {
       stout <<" for nonterminal " << i << ": \n";
       gr=nontt[i];
       while (gr!=NULL) { 
	 stout << " info = " << gr->infos << "\n";
         dumpgrrule(gr->r);
         gr=gr->next;
       }
    }
  stout << "[end of grammar dump]\n";
}


void writegrrule(ochstream &cou,struct sgrammrule *gr,stringtab *typet)
{ int j,wast;
  lexem ll;
  j=0;
  if (//commands && 
      (SPEC_N == 0 || (SPEC_N > 0 && SPEC_N == gr->rulenumber)) &&
      (SPEC_I == NULL || (SPEC_I != NULL && 0==strcmp(SPEC_I,gr->rside[0].alfsy())))) {
  while (gr->rside[j].isnotendofstream()) {
    ll=gr->rside[j];
    if (ll.nonterminal()) cou << " @ ";
    else cou << "'"<< ll.alfsy() << "' ";
    j++;
  }
  cou << "\t : ";
  j=0; wast=0;
  while (gr->rside[j].isnotendofstream()) {
    ll=gr->rside[j];
    if (ll.nonterminal()) {
      cou << (wast?" ":"(") << typet->ide(ll.typeval());
      wast=1;
    }
    j++;
  }
  if (wast) cou << ")";
  cou << typet->ide(gr->leftside.typeval());
  if(gr->rulenumber >= 0 )
    {
      if ((gr->priority & RASSOCMSK) == RLEFTASSOC)
	cou << "\t assocLeft ";
      if ((gr->priority & RASSOCMSK) == RRIGHTASSOC)
	cou << "\t assocRight ";
      cou << "\t pri " << (gr ->priority & RPRIORITYMSK);
      cou << " code " << gr->rulenumber;
      if ( gr->semantic) { cou << " sem " << gr->semantic; }
      if (fsymtab[gr->rulenumber].get_locstratlen()) {
	  int ii;
	  cou << " strategy ";
	  for (ii=0; ii < fsymtab[gr->rulenumber].get_locstratlen(); ii++)
	      cou << fsymtab[gr->rulenumber].get_locstrat(ii) << " "; }
    }
  else
    {
      cou << "\t VAR(" << -gr->rulenumber << ")";
      cou << "\t (pri " << (gr ->priority & RPRIORITYMSK)<<")";
    }
    cou << ";\n";
  }
}

void earleyPrettyDumpGrammarRule(ochstream &stout,struct sgrammrule *gr)
{
  int j;
  stout << "priority ";
  stout << gr ->priority << " "; 
  stout << "rulenumber ";
  stout << gr->rulenumber << " ";
  stout << ", arity=" <<  fsymtab[gr->rulenumber].arity()
	<< ", isconstructor=" <<  ISCONSTRUCTOR(gr->rulenumber)
	<< "\n";

  if (gr->leftside.nonterminal())
    {
      // un type : term, bool ...
      stout << typet.ide(gr->leftside.typeval());
      //stout << "[" << gr->leftside.typeval() <<"]";
      stout << "[" << gr->leftside.numval() <<"]";
    }
  else
    {
      // un symbole : f,g,a,( ...
      stout << gr->leftside.alfsy();
      stout << "cela ne devrait pas arriver\n";
      failexit();
    }
  stout << " ";
  stout << " -->  ";
  j=0;
  while (gr->rside[j].isnotendofstream())
    {
      if (gr->rside[j].nonterminal()) 
	{
	  // un type : term, bool ...
	  stout << typet.ide(gr->rside[j].typeval()); 
	  //stout << "[" << gr->rside[j].typeval() <<"]";
	  stout << "[" << gr->rside[j].numval() << "]";
	}
      else
	{
	  // un symbole : f,g,a,( ...
	  stout << gr->rside[j].alfsy(); 
	  stout << "[" << gr->rside[j].idval()
		<< "(" << gr->rside[j].numval() << ")" << "]";


	  //stout << "[" << gr->rside[j].idval() <<"]";


	}
      j++;
    }
  stout << "\n";
}

void grammar::earleyPrettyDump(ochstream &stout)
{ struct grammrulelist *gr;
  int i;
  //  stout << "\nrules :\n";
  for (i=0; i<NNONTERMINALS; i++) 
    if (nontt[i]!=NULL) {
      stout <<" for nonterminal ";
      stout << i << "\n";
       gr=nontt[i];
       while (gr!=NULL) { 
	 stout << " info ";
	 stout << gr->infos << "\n";
         earleyPrettyDumpGrammarRule(stout,gr->r);
         gr=gr->next;
       }
    }
  stout << "[end of grammar dump]\n";
}

int earleyDumpGrammarRule(ochstream &stout,struct sgrammrule *gr)
{
  int j;
  int compteur;
  stout << gr ->priority << " , "; 
  stout << gr->rulenumber << " , ";

  if (gr->leftside.nonterminal())
    {
      // un type : term, bool ...
      stout << gr->leftside.numval() << " , ";
    }
  else
    {
      // un symbole : f,g,a,( ...
      stout << gr->leftside.alfsy();
      stout << "cela ne devrait pas arriver\n";
      failexit();
    }
  // on compte le nombre de membres droits
  compteur=0;
  while (gr->rside[compteur].isnotendofstream())
    compteur++;
  stout << compteur << " , ";

  j=0;
  while (gr->rside[j].isnotendofstream())
    {
      if (gr->rside[j].nonterminal()) 
	{
	  // un type : term, bool ...
	  stout << gr->rside[j].numval() <<" , ";
	}
      else
	{
	  // un symbole : f,g,a,( ...
	  stout << gr->rside[j].numval() <<" , ";
	}
      j++;
    }
  return 4+1+compteur;
}

void grammar::earleyDump(ochstream &stout)
{ struct grammrulelist *gr;
  int i;
  int size=0;
  
  // dump de la  table de symboles
  tabofident.earleyDump(stout,"char_tabofident");

  // dump de la  table de types
  typet.earleyDump(stout,"char_typet");

  // dump des arites
  // dump des symboles constructeurs
  // c'est fait dans compilemisc.c : genfsymtab(FILE *ff)

  // dump de la grammaire
  stout << "int grammar[] = {\n";
  for (i=0; i<NNONTERMINALS; i++) 
    if (nontt[i]!=NULL) {
       gr=nontt[i];
       while (gr!=NULL) { 
	 stout << gr->infos << " , ";
         size+=earleyDumpGrammarRule(stout,gr->r);
	 stout << "\n";
         gr=gr->next;
       }
    }
  // 0 pour finir
  stout << "0};\n";
  stout << "#define GRAMMAR_SIZE " << size << "\n";
}


void grammar::gr_rule_mapp(int /*nothing*/, void (*fun)(struct sgrammrule *))
{ struct grammrulelist *gr;
  int i;
  for (i=0; i<NNONTERMINALS; i++) 
    if (nontt[i]!=NULL) {
       gr=nontt[i];
       while (gr!=NULL) { 
         (*fun)(gr->r);
         gr=gr->next;
       }
    }
}

void grammar::pretydump(stringtab *tyt)
{ int i;
  struct grammrulelist *gr;
//  stout << "\n[grammar dump] startsym = " << startsym.alfsy();
  stout << "\nrules :\n";
  for (i=0; i<NNONTERMINALS; i++) 
    if (nontt[i]!=NULL) {
       stout <<" for nonterminal " << tyt->ide(i) << ": \n";
       gr=nontt[i];
       while (gr!=NULL) { 
         writegrrule(stout,gr->r,tyt);
         gr=gr->next;
       }
    }
  stout << "[end of grammar dump]\n";
}

void grammar::write(ochstream &sout, int winfo,stringtab *tyt,const char *bef, const char *aft)
{ int i,j;
  struct grammrulelist *gr;
  j=0;
  for (i=0; i<NNONTERMINALS; i++) 
    if (nontt[i]!=NULL) {
       gr=nontt[i];
       while (gr!=NULL) { 
         if ((gr->infos & RSTATMSK )== winfo) {
            if (!j) {sout << bef; j=1;}
            writegrrule(sout,gr->r,tyt);
         }
         gr=gr->next;
       }
    }
  if (j) sout << aft;
}
