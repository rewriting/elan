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


/*
 !!!!!!!!!!!!! in the comments, ---  "CP" stands for "Currently Parsed"
                                ---  "RW" stands for "ReWritting"
*/

#include "commondefs.h"
#include "termdefs.h"
#include "module.h"
#include "strategy.h"
#include "codes.h"
 
extern lexem Sif,Swhere,Send,Sstart;

extern lexem Sif,Swhere,Send,Sstart;            // some usefull reserved words

LoaderState ld;                          // the state of the module parser (strategy.h)
 std::deque<ModuleFrame> modframes(1);   // stack of the CP modules being read (strategy.h)
 int stacki = 0;                              // index to modframes
 int impmoduli = -1;                      // index of currently imported module (rtdatas.h)
int withrhs = 0;                          // set by parse/esemact.cc (termdefs.h)
char *visibilities[MAXNOFIMPORTS];      // matrix of module-to-module visibilities

void grow_modframes(int level)
{
  while ((int)modframes.size() <= level) modframes.emplace_back();
}

void grow_strstack(int n)
{
  if ((int)ld.strstack.size() < n) ld.strstack.resize(n, NULL);
}

void grow_strlstack(int n)
{
  if ((int)ld.strlstack.size() < n) ld.strlstack.resize(n, NULL);
}

extern lexem strIdentLex;
extern int handlewherepattern(lstream *f, int pattyp, struct wherelist *wl);

extern int pretydumpsitset(lstream *f,stringtab *types);
extern int acsymbolinleftside;		             // body in esemact.c   | is there an AC symbol in the left hand side of CP rule?
extern void esemactinit();		             // body in esemact.c   | init term construction semantic actions
extern int pretydumpsitset(lstream *f,stringtab *types); // pretty dump of earley's situations table
extern int msyntan(lstream &,int,void (*ltol)(lexem l1,lexem &l2));    // parser for an elan (.eln) module
extern struct WHEREbranches *parse_try(lstream *f);


int stratmoduli(int x, int y)
{
char modnam[STRLEN];
  if (x==y)
    snprintf(modnam,sizeof(modnam),"%s[%s]",STRAT_MODNAME1,typet.ide(x));
  else
    snprintf(modnam,sizeof(modnam),"%s[%s,%s]",STRAT_MODNAME2,typet.ide(x),typet.ide(y));
  if (!import.member(modnam)) { 
    sterr << "\nmissing file " << modnam << "\n";
    sterr << "\n[fatal] internal error\n"; failexit(); }
  return import.posid;
}

int evalmoduli(int x, int y) 
{
char modnam[STRLEN];
  if (x == y) 
    snprintf(modnam,sizeof(modnam),"%s[%s]",STRAT_MODNAME1,typet.ide(x)); 
  else {
    snprintf(modnam,sizeof(modnam),"%s[%s,%s]",STRAT_MODNAME2,typet.ide(x),typet.ide(y)); }

  if (!import.member(modnam)) { 
    if (!batch) {
    sterr << "[fatal] declare module " << modnam << " before using its features\n";
    sterr << "\ninternal error\n"; }
    failexit(); }
  return import.posid;
}

char *attach_type(const char *s,int t)
{
char full_name[STRLEN];
  if (s == NULL || strlen(s) == 0) return NULL; /////// exception for nonamed rules
  snprintf(full_name,sizeof(full_name),"%s%s%s",s,TYPE_SEPARATOR,typet.ide(t));
  return strdup(full_name);
}

char *attach_mod(const char *s,int modu)
{
char full_name[STRLEN];
  if (s == NULL || strlen(s) == 0) return NULL; /////// exception for nonamed rules
  snprintf(full_name,sizeof(full_name),"%s%s%s",s,MODULE_SEPARATOR,import.ide(modu));
  return strdup(full_name);
}

static char *attach_type_modu(const char *s,int t,char *modu)
{
char full_name[STRLEN];
  if (s == NULL || strlen(s) == 0) return NULL; /////// exception for nonamed rules
  // S2: sprintf could overflow full_name with long names
  if (snprintf(full_name,sizeof(full_name),"%s%s%s%s%s",s,TYPE_SEPARATOR,typet.ide(t), MODULE_SEPARATOR, modu)
      >= (int) sizeof(full_name)) {
    sterr << "\n[semact] name too long\n"; failexit(); }
  return strdup(full_name);
}

char *attach_type_mod(const char *s,int t,int modul)
{
  return attach_type_modu(s,t,import.ide(modul));
}


static char *attach_modu_loc(const char *s, char *modu, const char *loc)
{
char full_name[STRLEN];
  if (s == NULL || strlen(s) == 0) return NULL; /////// exception for nonamed rules
  snprintf(full_name,sizeof(full_name),"%s%s%s%s%s",s,
	  MODULE_SEPARATOR, modu,
	  LOC_SEPARATOR,loc);
  return strdup(full_name);
}

char *attach_mod_loc(const char *s, int modul, int infos)
{
  return attach_modu_loc(s,import.ide(modul), ((infos & RGLOP)?"GL":"LO"));
}

int detach_s1_s2(const char *separator, const char *s,char **name, char **type)
{
char full_name[STRLEN];
char *p;
  strcpy(full_name,s);
  p = strstr(full_name,separator);
  if (p) {
    *p = 0;
    *name = strdup(full_name);
    p += strlen(separator);
    *type = strdup(p);
    return 1; }
  else
    return 0;
}

int detach_name_type(const char *s,char **name, char **type)
{
  return(detach_s1_s2(TYPE_SEPARATOR,s,name,type));
}
int detach_name_module(const char *s,char **name, char **type)
{
  return(detach_s1_s2(MODULE_SEPARATOR,s,name,type));
}
int detach_name_type_module(const char *s, char **name, char **type, char **modu)
{
char *ww;
  return (detach_s1_s2(TYPE_SEPARATOR,s,name,&ww) &&
	  detach_s1_s2(MODULE_SEPARATOR,ww,type,modu) );
}

int is_prefix_of_name(const char *s1, const char *s2)
{
char sss[STRLEN];
const char *s;
  snprintf(sss,sizeof(sss),"%s%s",s1,TYPE_SEPARATOR);
  s = strstr(s2,sss);
  if (s == s2) return 0; else return -1;
}

int endofin(lexem le)
{
  return(le==Sif || le==Swhere || le==Schoose || le==Stry || 
	 le==Send || le.isendofstream());
}

int endofinco(lexem le)
{
  return(le==Sif || le==Swhere || le==Send || le==Scase || le==Sotherwise ||
	 le==Schoose || le==Stry || 
	 le==Sthen || 
	 // *** NO_MORE_SWITCH le == Sswitch || 
	 le.isendofstream());
}

void lextomodlex(lexem l,lexem &ml)
{
  if (l.isident()) {
    if (mrwt.member(l.idval())) ml = l;
    else ml.cridlex();
  } else if (l.isnum()) {ml.crnumlex(); }
    else if (l.isstring()) { ml.crstringlex(); }
    else ml = l;
}

void lextoamodlex(lexem l,lexem &ml)
{
  if (l.isident()) {
    // stout << "#" << l.alfsy() << " ";
    if (amrwt.member(l.idval())) { // stout << "member \n"; 
    ml = l; }
    else { // stout << "nonmember \n"; 
    ml.cridlex(); }
  } else if (l.isnum()) {ml.crnumlex(); }
    else if (l.isstring()) { ml.crstringlex(); }
    else ml = l;
}

 void typecheck(lstream *f,char *s)
{     
    if (! (typet.member(s)) ){
        f->owarn("\tunknown type ",s," used\n\n",NULL);
     }     
}

void create_nested()
{ struct nvlist *ll;
  NNEW(ll ,struct nvlist);
  ll->i = ld.vartabi;
  ll-> ruletype = ld.actruletype;
  ll->next = ld.nestedvartabi; ld.nestedvartabi = ll;
}

void add_to_fsymtab(int actarityy,struct sgrammrule *gr, int defstrat)
{
  fsym s(actarityy,gr,ld.finfos);
  if (ld.actcode>=MAXCODE) return;             // strategies and statements
  if (ld.actcode>=MAXNFSYM) {
    sterr << "There is more functional symbols then MAXNFSYM="
          << MAXNFSYM << "\n\tsorry, FATAL !!!\n";
    failexit();
  }
  s.set_semantic(ld.actsemantic); 
  s.set_locstrat(ld.locstratlen,ld.locstrattable); ld.locstratlen = 0;ld.locstrattable = NULL;
  gr->defstrat = defstrat;
  gr->semantic =ld.actsemantic;
  ld.actsemantic = 0;
  fsymtab[ld.actcode]=s;
  gr->fsymcode = ld.actcode;
  if (ld.actcode == fsymtabi) fsymtabi++;
  ld.actcode = fsymtabi;
}

void mkmodname1(struct chlist *&actimp,lexem l)
{ struct chlist *chpp;
        AALLOS(chpp ,struct chlist);
	AALLOSS(chpp->s ,strlen(l.alfsy())+1,char);
	strcpy(chpp->s,l.alfsy());
        chpp->next = actimp; actimp=chpp;
}

void mkmodname2(struct chlist *&actimp)
{ struct chlist *chpp;
   char *s;
          chpp = actimp->next;

	  if (chpp == NULL) {
	     sterr << "[semact.mkmodname2] int.err\n"; failexit();
          }
	  s= chpp->s;
	  chpp->s = addsuffixs(s,"[",actimp->s,"]",NULL);
	  CFRE(s); CFRE(actimp);
          actimp = chpp;
}

void mkmodname3(struct chlist *&actimp)
{ struct chlist *chpp;
  char *s;
          chpp = actimp->next;
	  if (chpp == NULL) {
	     sterr << "[semact.mkmodname3] int.err\n"; failexit();
          }
	  s= chpp->s;
	  chpp->s = addsuffixs(s,",",actimp->s,NULL);
	  CFRE(s); CFRE(actimp);
          actimp = chpp;
}

void mkmodname4(struct chlist *&actimp) // <X>
{ struct chlist *chpp;
  char *s;
          chpp = actimp;
	  if (chpp == NULL) {
	     sterr << "[semact.mkmodname4] int.err\n"; failexit();
          }
	  s= chpp->s;
	  chpp->s = NULL;
	  chpp->s = addsuffixs("<",s,"->",s,">",NULL);
	  CFRE(s); 
          actimp = chpp;
}

void mkmodname5(struct chlist *&actimp) // <X->Y>
{ struct chlist *chpp;
  char *s;
          chpp = actimp->next;
	  if (chpp == NULL) {
	     sterr << "[semact.mkmodname5] int.err\n"; failexit();
          }
	  s= chpp->s;
	  chpp->s = NULL;
	  chpp->s = addsuffixs("<",s,"->",actimp->s,">",NULL);
	  CFRE(s); CFRE(actimp);
          actimp = chpp;
}

grammar *actgram()
{ return(& modframes[stacki].gr);
}

void setmodname(const char *s, char *ss)
{
  modframes[stacki].modname=s; modframes[stacki].filemodname = ss;
}

void addstandards(grammar &gr)
{ lexem le;
      le.cridlex(); gr.addsymbol(le); 
      gr.addrule(internIdentType,STANDPRI,RSTANDOP,IDENTRULE);
      le.crnumlex(); gr.addsymbol(le);
      gr.addrule(internIntType,STANDPRI,RSTANDOP,NUMRULE);
      le.crstringlex(); gr.addsymbol(le); 
      gr.addrule(internStringType,STANDPRI,RSTANDOP,STRINGRULE);
}

void init_visi(int modul)
{
char *visi; int i;
  AALLOSS(visi,MAXNOFIMPORTS,char);
  for(i=0; i < MAXNOFIMPORTS; i++) visi[i]=0;
  visi[modul] = 1;
  visibilities[modul] = visi;
}

void crStandModules()
{ grammar *ggr;
  struct sgrammrule *rule;
  int ind;
  lexem le;
  ind = import.addstr("anyInteger");
  init_visi(ind);
  NNEW(importglobgr[ind] ,grammar);
  ggr = importglobgr[ind];
  addstandards(*ggr);
  ggr->addsymbol(internIntType);
  rule = ggr->addrule(numtype,STANDPRI,RGLOP,NUMTOTERM);      
  { fsym s(0,rule,FSNOINFO);
    fsymtab[NUMTOTERM]=s;
  }
  le.crcharlex('-'); ggr->addsymbol(le);
  ggr->addsymbol(internIntType);
  rule = ggr->addrule(numtype,STANDPRI,RGLOP,INTCONSTUMIN);      
  { fsym s(0,rule,FSNOINFO);
    fsymtab[INTCONSTUMIN]=s;
  }

  ind = import.addstr("anyIdentifier");
  init_visi(ind);
  NNEW(importglobgr[ind] ,grammar);
  ggr = importglobgr[ind];
  addstandards(*ggr);
  ggr->addsymbol(internIdentType);
  rule = ggr->addrule(identype,STANDPRI,RGLOP,IDENTTOTERM);      
  { fsym s(0,rule,FSNOINFO);
    fsymtab[IDENTTOTERM]=s;
  }
  ind = import.addstr("anyString");
  init_visi(ind);
  NNEW(importglobgr[ind] ,grammar);
  ggr = importglobgr[ind];
  addstandards(*ggr);
  ggr->addsymbol(internStringType);
  rule = ggr->addrule(stringtype,STANDPRI,RGLOP,STRINGTOTERM);      
  { fsym s(0,rule,FSNOINFO);
    fsymtab[STRINGTOTERM]=s;
  }
  ind = import.addstr("anyQuotedIdentifier");
  init_visi(ind);
  NNEW(importglobgr[ind] ,grammar);
  ggr = importglobgr[ind];
  addstandards(*ggr);
  le.crcharlex('"');
  ggr->addsymbol(le);
  ggr->addsymbol(internIdentType);
  le.crcharlex('"');
  ggr->addsymbol(le);
  rule = ggr->addrule(identype,STANDPRI,RGLOP,IDENTTOTERM);      
  { fsym s(0,rule,FSNOINFO);
    fsymtab[IDENTTOTERM]=s;
    fsymtab[IDENTTOTERM].set_aliased();
  }     

}

 void addstratprocgr()
{ grammar *g;
  NNEW(g ,grammar);
  addstandards(*g);
  g->addgrammar(modframes[stacki].gr,RLOCOOP,RGLOP);
  g->addgrammar(modframes[stacki].gr,RLOCOOP|RIMPORTBIT,RGLOP);
  ld.actstrategy->setprocgr(g);
}

int sem_action_167(lstream *f, int actsindex, int styp)
{
  trrules.settypeofstrategy_defs(actsindex,styp);
  ld.strstack[0]->settypeof(styp);

  all_strateg[all_strategi++] = ld.strstack[0];    // sauvgarde chaque strategie definie

  if (trrules.setstrategy_defs(actsindex,ld.strstack[0])) {
    if (!batch) f->owarn("[warning] double definition of strategy\n",
			 trrules.strategyname_refs(actsindex),NULL); 
    return(NORMCONT);
  } 
  return(NORMCONT);
}

int sem_action_137(lstream *f,lexem l)
{
  ld.actwherevar=ld.vartabi-1;
  while (ld.actwherevar>=0 && ld.actvtab[ld.actwherevar]!=l)ld.actwherevar--; 
  if (ld.actwherevar<0) {
    f->owarn("\n[semact] variable name expected after where ",NULL);
    return(ERRORIM);
  }
  ld.actwheretype=ld.vartab[ld.actwherevar]->leftside; 
  ld.stratindex = ld.actwheretype.typeval();
  return NORMCONT;
}

int sem_action_138(lstream *f)
{ int resan;
  resan = modframes[stacki].gr.earleycall(f,ld.actwheretype,endofin);
  if (! resan) return(HANDERRORIM);
  ld.ter1.popt();
  if (ld.acttrrule) { // elan rule
	   ld.acttrrule->addwhere(reverse_wheres,ld.actwherevar,
		  (ld.actwhstrategy==-1)?((strategy**)NULL):
                  trrules.getstrategyadr_refs(ld.actwhstrategy),ld.ter1,ld.actwheretype); }
  if (ld.acttrrulelab) { // strategy LAB_ rule
	 ld.ter1.copyrec(ld.ter2);
	 ld.acttrrulelab->addwhere(reverse_wheres,ld.actwherevar,
		 (ld.actwhstrategy==-1)?((strategy**)NULL):
				trrules.getstrategyadr_refs(ld.actwhstrategy),ld.ter2,ld.actwheretype); }
  ld.actwhstrategy = -1;
  return NORMCONT;
}

int sem_action_139(lstream *f)
{ int resan;
  term cond;
  term cond1;
  resan = modframes[stacki].gr.earleycall(f,booltype,endofin);
  if (! resan) return(HANDERRORIM);
  cond.popt();
  if (ld.acttrrule) {
    ld.acttrrule->addwhere(reverse_wheres,IFVARN,NULL,cond,booltype);
  }
  if (ld.acttrrulelab) {
    cond.copyrec(cond1);
    ld.acttrrulelab->addwhere(reverse_wheres,IFVARN,NULL,cond1,booltype); }
  return NORMCONT;
}

#define ILEX(x) { f->ilex(x); f->fulex(x); }

//---------------------------------
// if !pattyp, read normal where
int get_pattern_where(lstream *f,  int pattyp, int wtype, int (*fin)(lexem) )
{ lexem le;
  struct sgrammrule *mainrule1, *mainrule2, *mainrule3;
  int resan;

  strIdentVal = 0; strategywasapplied = 0;
  //--- where (type)pattern:=()term
  if (pattyp) { le.crtypelex(pattyp); modframes[stacki].gr.addsymbol(le);}
  le.crcharlex(':'); modframes[stacki].gr.addsymbol(le);
  le.crcharlex('='); modframes[stacki].gr.addsymbol(le);
  le.crcharlex('('); modframes[stacki].gr.addsymbol(le);  // pour l'instant
  le.crcharlex(')'); modframes[stacki].gr.addsymbol(le);
  le.crtypelex(wtype); modframes[stacki].gr.addsymbol(le);
  le.crtypelex(STARTTYPE); 
  mainrule1=modframes[stacki].gr.addrule(le,RNOPRIOR,RNOINFO,RIGHTSRULE);

  //--- where (type)pattern:=(sss)term
  if (pattyp) { le.crtypelex(pattyp); modframes[stacki].gr.addsymbol(le);}
  le.crcharlex(':'); modframes[stacki].gr.addsymbol(le);
  le.crcharlex('='); modframes[stacki].gr.addsymbol(le);
  le.crcharlex('('); modframes[stacki].gr.addsymbol(le);  
                     modframes[stacki].gr.addsymbol(internIdentType);  
  le.crcharlex(')'); modframes[stacki].gr.addsymbol(le);
  le.crtypelex(wtype); modframes[stacki].gr.addsymbol(le);
  le.crtypelex(STARTTYPE); 
  mainrule2=modframes[stacki].gr.addrule(le,RNOPRIOR,RNOINFO,RIGHTSRULE);

  //--- where (type)pattern:=[SSS]term
  if (pattyp) { le.crtypelex(pattyp); modframes[stacki].gr.addsymbol(le);}
  le.crcharlex(':'); modframes[stacki].gr.addsymbol(le);
  le.crcharlex('='); modframes[stacki].gr.addsymbol(le);
  le.crcharlex('['); modframes[stacki].gr.addsymbol(le);  
  le.crtypelex(wtype); modframes[stacki].gr.addsymbol(le);
  le.crtypelex(STARTTYPE); 
  mainrule3=modframes[stacki].gr.addrule(le,RNOPRIOR,RNOINFO,STRATCONSTRULE);

  le.crtypelex(STARTTYPE); 
// CSBug  esemactinit();
  resan = modframes[stacki].gr.earleycall(f,le,fin);
  if (! resan) return(resan);

  modframes[stacki].gr.deleterule(mainrule1);
  modframes[stacki].gr.deleterule(mainrule2);
  modframes[stacki].gr.deleterule(mainrule3);

  return resan;
}

struct wherelist *parse_seq(lstream *f)
{
lexem clex, le;
int   resan;
 term  rside2; /* lside2,*/
struct wherelist **ppp,*p,*wlist = NULL;
term noterm; noterm.stinit();
   f->fulex(le);
   while (le == Swhere || le == Sif || le == Schoose) {
     p = new_wherelist();
     if (reverse_wheres) {
       ppp = &wlist;
       while ((*ppp) != NULL) ppp=&((*ppp)->next);
       (*ppp) = p; p->next=NULL; }
     else {
       p->next = wlist; wlist = p; }
     if (le == Sif) {
       f->ilex(le);
//CSBug       esemactinit(); 
       resan = modframes[stacki].gr.earleycall(f,booltype,endofinco);
       if (! resan) { f->owarn("[fatal] error in parse_seq \n",NULL); 
                      failexit(); }
       p->leftvarn = IFVARN; p->strateg = NULL;
       p->whereterm.popt(); }
     else if (le == Schoose) {
       p->leftvarn =TRYCHOICEEND;
       p->wherebranch_list = parse_try(f); }
     else if (le == Swhere) {
       ILEX(le);
       clex.crcharlex('(');
       if (le == clex) {                // where (....
	 char typenam[STRLEN];
	 typenam[0] = 0;
	 // hack ako svina
	 clex.crcharlex(')');
	 while (1) {
	   ILEX(le);
	   if (le == clex) break;
	   strcat(typenam, le.alfsy());
	 }
	 p->pattype = typet.addstr(typenam);
	 //stout << "hack ako svina " << typenam << p->pattype << "\n";
	 ILEX(le);
	 if (!handlewherepattern(f,p->pattype,p)) { 
	   f->owarn(" [fatal] error in parse_seq \n",NULL); failexit(); }
	 f->fulex(le);
	 continue;
       }
       // no where pattern
       ld.pattype = 0;
       if (NORMCONT != sem_action_137(f,le)) { 
         f->owarn(" [fatal] error in parse_seq \n",NULL); failexit(); }
       ILEX(le);
       resan = get_pattern_where(f,0,ld.actwheretype.typeval(),endofinco);
//-------------
       if (! resan) { f->owarn(" [fatal] error in parse_seq \n",NULL); failexit(); }
       rside2.popt(); // rside2.write(stout); stout << "\n";
       if (strIdentVal) {
         ld.actwhstrategy = trrules.strategyindex_refs(
           attach_type_mod(strIdentLex.alfsy(),ld.actwheretype.typeval(),impmoduli)); }
       else if (strategywasapplied) {
         int strx, stry;
         // S2: strx/stry were used uninitialised when rside2 is not an applied code
         if (!inverse_apply_code(rside2,&strx,&stry)) interr();
         ld.actwhstrategy = trrules.strategyindex_refs(
           attach_type_mod(EVALSTR,ld.actwheretype.typeval(),
              evalmoduli(strx,stry))); 
       }
       else
         ld.actwhstrategy = -1 ; 
//---------------
       p->leftvarn = ld.actwherevar; 
       p->leftvarterm.crvar(ld.actwherevar,ld.actwheretype);
       p->leftvarterm.popt();
       p->strateg = (ld.actwhstrategy==-1)?((strategy**)NULL):
                     trrules.getstrategyadr_refs(ld.actwhstrategy);
       ld.actwhstrategy = -1 ;  //3005
       p->whereterm = rside2;
     } else {
       f->owarn(" [fatal] if or where expected\n",NULL); failexit(); }
     f->fulex(le);
   } // while where,if
   return wlist;
}

struct tseq *parse_tseq(lstream *f)
{
  /*term resterm, cond;*/
  lexem  le; /* clex,*/
int resan;   
struct tseq *ts;
 /* struct branch  *brlist, **brl; *br,*/
   f->fulex(le);
   /* brlist = NULL; brl = &brlist; */
   AALLOS(ts, struct tseq);
     {
     ts->is_case = 0;
     NNEW(ts->u.one_branch.result, term);
//CSBug     esemactinit();
     resan=modframes[stacki].gr.earleycall(f,ld.actruletype,endofinco);
     if (! resan) { 
       f->owarn(" [fatal] error in parse_tseq\n",NULL); failexit(); }
     ts->u.one_branch.result->popt();
     ts->seq = parse_seq(f); }
   return ts;
}






struct WHEREbranches *parse_try(lstream *f)
{
  /*term resterm, cond;*/
  lexem  le; /*clex,*/
  /*int resan;   */
struct WHEREbranches *br, *brlist, **brl;
   f->fulex(le);
   brlist = NULL; brl = &brlist;
   if (le == Schoose) { 
      ILEX(le); }
   while (le == Stry) {
       while (*brl) brl = &((*brl)->next);  
       AALLOS(*brl, struct WHEREbranches); br = *brl; br->next = NULL;
       ILEX(le);
       br->wherebranch = parse_seq(f);
       f->fulex(le);
   } 
   if (le == Sotherwise) {
       while (*brl) brl = &((*brl)->next);  
       AALLOS(*brl, struct WHEREbranches); br = *brl; br->next = NULL;
       ILEX(le);
       br->wherebranch = parse_seq(f);
       f->fulex(le);
   }
     if (le != Send) { 
       f->owarn(" [fatal] end expected\n",NULL); failexit(); }
     ILEX(le);
   return brlist;
}



// ca sert a quoi ??? 
void addpatternwhere(struct wherelist *wl, strategy **strat, 
				term tl, term tr, int whtype)
{
  wl->leftvarn = WHEREPATTERN;
  wl->strateg =  strat;
  wl->pattern = tl;
  wl->pattype = whtype;
  wl->whereterm = tr;
}

//
//  parsing of t:pattype:=()tt:pattype
//  parsing of t:pattype:=(ss)tt:pattype
//  parsing of t:pattype:=[ss]tt:pattype
//
int handlewherepattern(lstream *f, int pattyp, struct wherelist *wl)
{ 
  term lside1, rside1;
  term lside2, rside2;
  int resan;

  if (!pattyp) return sem_action_138(f);  // clasic where

  resan = get_pattern_where(f, pattyp,pattyp,(wl)?endofinco :endofin);

  if (! resan) failexit();

  lside2.popt(); // lside2.write(stout); stout << "\n";
  rside2.popt(); // rside2.write(stout); stout << "\n";

  if (! resan) { 
      f->owarn(" [fatal] error in handewherepattern\n",NULL); failexit(); }
  if (strIdentVal) 
    ld.actwhstrategy = trrules.strategyindex_refs(
	attach_type_mod(strIdentLex.alfsy(),pattyp,impmoduli)); 
  else if (strategywasapplied) {
    int strx, stry;
    // S2: strx/stry were used uninitialised when rside2 is not an applied code
    if (!inverse_apply_code(rside2,&strx,&stry)) interr();
   ld.actwhstrategy = trrules.strategyindex_refs(
	attach_type_mod(EVALSTR,pattyp,
			evalmoduli(strx,stry)));
  }
  else
    ld.actwhstrategy = -1 ; 
  
  if (wl) { // pattern where
	//	 stout << " ???? ???? \n";
    addpatternwhere(wl,(ld.actwhstrategy==-1)?((strategy**)NULL):
        trrules.getstrategyadr_refs(ld.actwhstrategy),lside2,rside2,pattyp); }
  else if (ld.acttrrule) { // elan rule
     ld.acttrrule->addpatternwhere(reverse_wheres,
     (ld.actwhstrategy==-1)?((strategy**)NULL):
        trrules.getstrategyadr_refs(ld.actwhstrategy),lside2,rside2,pattyp); }
  if (ld.acttrrulelab) { // strategy LAB_ rule
     lside2.copyrec(lside1);
     rside2.copyrec(rside1);
     ld.acttrrulelab->addpatternwhere(reverse_wheres,
        (ld.actwhstrategy==-1)?((strategy**)NULL):
	trrules.getstrategyadr_refs(ld.actwhstrategy),lside1,rside1,pattyp); }
  ld.actwhstrategy = -1 ; // 3005
  return(resan);
}

int handlerulebody(lstream *f)
{ lexem le;
  struct sgrammrule *mainrule,*mainrule1,*rightsrule;
  int resan;
// RIGHTSTYPE ---------
  modframes[stacki].gr.addsymbol(ld.actruletype);
  le.crtypelex(RIGHTSTYPE);
  rightsrule=modframes[stacki].gr.addrule(le,RNOPRIOR,RNOINFO,RIGHTSRULE);
// RULECONSTRULE I -------
  modframes[stacki].gr.addsymbol(ld.actruletype_l);
  le.crcharlex('='); modframes[stacki].gr.addsymbol(le);
  le.crcharlex('>'); modframes[stacki].gr.addsymbol(le);
  le.crtypelex(RIGHTSTYPE); modframes[stacki].gr.addnont(le);
  le.crtypelex(STARTTYPE); 
  mainrule=modframes[stacki].gr.addrule(le,RNOPRIOR,RNOINFO,RULECONSTRULE);
// RULECONSTRULE II -------
  modframes[stacki].gr.addsymbol(ld.actruletype_l);
  le.crcharlex('='); modframes[stacki].gr.addsymbol(le);
  le.crcharlex('>'); modframes[stacki].gr.addsymbol(le);
  le.crtypelex(STARTTYPE); 
  mainrule1=modframes[stacki].gr.addrule(le,RNOPRIOR,RNOINFO,RULECONSTRULE1);
// PARSE --------
  le.crtypelex(STARTTYPE); 
  esemactinit();
  resan = modframes[stacki].gr.earleycall(f,le,
				     endofinco
				     );
  if (! resan) return(resan);
  ld.lside.popt(); 
  if (withrhs) { ld.act_rhs = NULL; ld.rside.popt(); }
  else { ld.act_rhs = parse_tseq(f); 
    f->fulex(le);
    if (le != Send) { 
      f->owarn(" [fatal] end expected after switch\n",NULL); failexit(); }
   }
//--------------------------
  modframes[stacki].gr.deleterule(mainrule1);
  modframes[stacki].gr.deleterule(mainrule);
  modframes[stacki].gr.deleterule(rightsrule);
  return(resan);
}

void appactstrat()
{ 
  if (ld.strstack[ld.strstacki] == NULL) 
	ld.strstack[ld.strstacki-1] = ld.actstrategy;
  else  ld.strstack[ld.strstacki]->setnext(ld.actstrategy);
  ld.strstack[ld.strstacki] = ld.actstrategy;
  NNEW(ld.actstrategy ,strategy);
}

void var_renameinit()
{ int p;
  ld.actvarnum = 0; ld.maxvarnum = 0;
  for (p=0; p<MAXNOFVAR; p++) ld.actvarrename[p] = NORENAME;
}

int var_rename(int v)
{
  if (ld.actvarrename[v] == NORENAME) ld.actvarrename[v]= ld.actvarnum++;
  if (ld.RENAME_IDENTITY) {
      if (v > ld.maxvarnum) ld.maxvarnum = v;
      return v;
  }
  else {
      if (ld.actvarrename[v] > ld.maxvarnum) ld.maxvarnum = ld.actvarrename[v];
      return(ld.actvarrename[v]); }
}

int var_was_renamed(int v)
{
  return(ld.actvarrename[v] != NORENAME);
}

int int_arg(struct chlist *actarglst)
{
 char *p;
 if (!actarglst) return 0;
 p = actarglst->s;
 while (*p)
   if (!((*p >= '0') && (*p <= '9'))) return 0;
   else p++;
 return 1;
}

void printargs(struct chlist *args)
{
  stout << " ARGS: ";
  while(args) {
    stout << args->s << ",";
    args=args->next;
  }
}

 void divideonargs(const char *cname,char *&modn, struct chlist **args)
{ int c;
  char *p, *name;
  AALLOSS(p ,strlen(cname)+1,char);
  strcpy(p,cname);
  name = p;
  c=0; p=name; modn=name; *args = NULL;
  while (*p) {
    if (*p == '[')      c++;
    else if (*p == ']') c--;
    if ((*p == ',' && c==1) || 
        (*p == '[' && c==1)) {
       *p = 0;
       NNEW(*args ,struct chlist); 
       (*args)->s = p+1;
       // stout << "argument = " << (*args)->s << "\n";
       args = &((*args)->next);
    }
    p++;
  }
  if (*(p-1) ==']')  *(p-1) = 0;
  *args = NULL;
}

 void divideonname(char *name,char *&modn)
{ /*int c;*/
  char *p;
  AALLOSS(p ,strlen(name)+1,char);
  strcpy(p,name);
  modn=p; 
  while (*p) if (*p == '[')  *p = 0; else p++;
}


void readmodules(lstream *f, const char *name)
{
int addit = 0;
int impmod, in_stratmod;

   char *modsou;
   if ( import.member(name) ) {
     f->owarn("[readmodules] int.err.\n",NULL); failexit();
   }
   //stout << "Name = " << name << "\n";
   impmod = impmoduli; in_stratmod = ld.in_stratmoduli;
   impmoduli = import.addstr(name);
   importglobgr[import.posid] = NULL;
   divideonargs(name,ld.actargmodname,&ld.actarglist);
   // printargs(actarglist);
   ld.in_stratmoduli = ISSTRATSIG(ld.actargmodname); //(ISSTRAT1MOD(actargmodname) || ISSTRAT2MOD(actargmodname));
   if ISSYMBOLMOD(ld.actargmodname) {
     struct ilist **rr;
     struct chlist *arglist = ld.actarglist;
     if (arglist) arglist = arglist->next; // skip the arity
     if (ld.symbappli >= MAXSYMBAPPL) {
       sterr << "\n[readmodules] too many imports of Symbol modules, more than MAXSYMBAPPL="
             << MAXSYMBAPPL << "\n\t fatal\n";
       failexit(); }
     rr = &(ld.symbappl[ld.symbappli]);
     while (arglist) {
       NNEW(*rr, struct ilist);
       (*rr)->i = typet.addstr(arglist->s);
       rr = &((*rr)->next);
       arglist = arglist->next; }
     *rr = NULL; ld.symbappli++;
   }
   if ISANYMOD(ld.actargmodname) {
     if (ld.anysi >= MAXANYS) {
       sterr << "\n[readmodules] too many imports of any modules, more than MAXANYS="
             << MAXANYS << "\n\t fatal\n";
       failexit(); }
     ld.anys[ld.anysi++] = typet.addstr(ld.actarglist->s); }
   if (ISSTRAT1MOD(ld.actargmodname) || ISSTRAT2MOD(ld.actargmodname)) { 
     struct chlist *p;
     addit = 1;
     ld.stratmoduli_fromi = ld.stratmoduli_toi = typet.addstr(ld.actarglist->s); p = ld.actarglist;
     if ISSTRAT2MOD(ld.actargmodname) { p = p->next; ld.stratmoduli_toi = typet.addstr(p->s); }
   }
   modsou = addsuffix(ld.actargmodname,".eln");
   mlstream ff(modsou); ///mlstream/lstream <=> with/without preprocessor !!!!!
   setmodname(name,ld.actargmodname);
   if (!msyntan(ff,0,lextomodlex)) failexit();
   CFRE(modsou);

   if  (addit) {
     trclos(f); // transitive closure
   } 

   //stout << "NAME = " << name << "\n";
   import.member(name);
   impmoduli = impmod; ld.in_stratmoduli = in_stratmod;
   NNEW(importglobgr[import.posid] ,grammar);

   importglobgr[import.posid]->addgrammar(modframes[stacki].gr,RGLOP|RIMPORTBIT,RGLOP);
   importglobgr[import.posid]->addgrammar(modframes[stacki].gr,RGLOP,RGLOP);

   modframes[stacki].gr.freetopl();

   conform_strategies(1); 
   trrules.assign_all_refs(1); // WWW = 0
}

void importmod(char *impmodule,lstream *f, int supermodule)
{
  importmod_inf( impmodule, f, supermodule,  modframes[stacki].importrinfos);
}

void importmod_inf(const char *impmodule,lstream *f, int supermodule, int rinf)
{
  int xx=0; /* initialised to avoid warning */
  if (supermodule != -1) { xx = impmoduli; impmoduli= supermodule; }
	if (! import.member(impmodule)) {
            grow_modframes(stacki+1);
            stacki++;
            readmodules(f,impmodule);
            modframes[stacki].gr.freetopl();
            stacki--;
  	  } 
          import.member(impmodule);         // to set import.posid
	 {
	   char *visi; int i;
//	   if (!batch) {
//	     stout << "Module " << import.ide(impmoduli) << impmoduli 
//		   << " imports the module " 
//		   << import.ide(import.posid) << impmoduli << " rinf=" << rinf << "\n"; }
           visi = visibilities[import.posid];
	   for(i=0; i< MAXNOFIMPORTS; i++) {
	     if (rinf == RGLOP && visi[i] > 0)  visibilities[impmoduli][i] = 1;
	     if (rinf == RLOCOOP && visi[i] > 0) 
	       if (visibilities[impmoduli][i] != 1) visibilities[impmoduli][i] = -1;
	   }
	  }

          if (importglobgr[import.posid] == NULL) {
           f->owarn("there is probably a cycle in imports of module ",
		    impmodule,"\n",NULL);
          } else {
            modframes[stacki].gr.addgrammar(
		  *importglobgr[import.posid],RGLOP,
		  rinf|RIMPORTBIT); 
	  // modframes[stacki].gr.dump();
	    if (ld.is_explimpl) {
	      char mname[STRLEN];
	      ld.is_explimpl = 0;
	      snprintf(mname,sizeof(mname),"explimpl%d",ld.explimpl_index++);
	      importmod(mname,f,-1);
	      (void)!system("/bin/rm -f explimpl*.eln");
	    }
	    if (ld.is_symbappl) {
	      char mname[STRLEN];
	      ld.is_symbappl = 0;
	      snprintf(mname,sizeof(mname),"symbappl%d",ld.symbappl_index++);
	      importmod(mname,f,-1);
	      (void)!system("/bin/rm -f symbappl*.eln");
	    }
	  }
  if (supermodule != -1) { impmoduli = xx; }
}

void fcheck_rename_wlist(lstream *f,transrule *trrule, struct wherelist *whs)
{
struct WHEREbranches *brlist;
  while (whs) {
      switch(whs->leftvarn) {
        case IFVARN:
          whs->whereterm.ren_vars((ADDRENAME));
          break;
        case WHEREPATTERN:
          whs->whereterm.ren_vars((ADDRENAME));
          whs->pattern.ren_vars((ADDRENAME));
          break;
        case TRYCHOICEEND:
          for(brlist = whs->wherebranch_list; brlist; brlist=brlist->next) {
          fcheck_rename_wlist(f,trrule,brlist->wherebranch); }
          break;
      default: 
          whs->leftvarn = var_rename(whs->leftvarn);
          whs->whereterm.ren_vars((ADDRENAME));
          whs->leftvarterm.ren_vars((ADDRENAME));
         break;
     }
    whs = whs->next;
  }
}


int fcheck_rename_tseq(lstream *f,transrule *trrule, struct tseq *tseq)
{
  /* int locvarrename[MAXNOFVAR];*/
 struct branch *br;
 /*term noterm;*/
 /* int i;*/
  if (tseq->is_case) {
    fcheck_rename_wlist(f,trrule,tseq->seq);
    for(br = tseq->u.more_branches.brlist; br; br=br->next) {
      br->test.ren_vars(ADDRENAME);
      fcheck_rename_tseq(f,trrule,br->tseq);
    } }
  else {
      fcheck_rename_wlist(f,trrule,tseq->seq);
      (tseq->u.one_branch.result)->ren_vars(ADDRENAME); }
  return NORMCONT; 
}

int fcheck_rename_rule(lstream *f,transrule *trrule, 
                     term l, term r, struct wherelist *whs, struct tseq *tseq)
{
      /*int locvarrename[MAXNOFVAR];*/
      int save_RENAME_ALL_VARS_MODE; /* i, save_RENAME_IDENTITY_MODE;*/
      /*struct WHEREbranches *brlist;*/
      /*term noterm;*/

      var_renameinit();
      //save_actvarnum = actvarnum;
      //for(i=0; i<MAXNOFVAR; i++)  { locvarrename[i] = actvarrename[i]; }
       save_RENAME_ALL_VARS_MODE = ld.RENAME_ALL_VARS; // global programming :-)))
       ld.RENAME_ALL_VARS = 1;
         l.ren_vars((ADDRENAME));
         if (tseq == NULL) {
           r.ren_vars((ADDRENAME));
           fcheck_rename_wlist(f,trrule,whs);
         } else {
           fcheck_rename_tseq(f,trrule,tseq);
         }
       ld.RENAME_ALL_VARS = save_RENAME_ALL_VARS_MODE;
     //stout << "MAX " << maxvarnum << "\n";
     var_renameinit();
  return NORMCONT;
}

int fcheck_wlist(lstream *f,transrule *trrule, struct wherelist *whs,
                 term r)
{
  while (whs != NULL) {
    // stout << "fcheck_wlist  \n "; trrule->dump(0);
    if (whs->leftvarn != TRYCHOICEEND) {
    if (whs->whereterm.ren_vars((CHECK))) {
      switch(whs->leftvarn) {
        case IFVARN:
          f->owarn("\n[semact] non instantiated variable in if: \n",NULL);
          whs->whereterm.write(sterr); stout << "\n";
           trrule->dump(0);
           return(ERRORIM);
        case WHEREPATTERN:
          f->owarn("\n[semact] non instantiated variable in pattern-where: \n",NULL);
          sterr << "WHERE ... :=(...)\n";
          whs->whereterm.write(sterr); stout << "\n";
         trrule->dump(0);
          return(ERRORIM);
        case TRYCHOICEEND:
          stout << "analyse choose-end\n";
          trrule->dump(0);
          return(ERRORIM);
      default: {
	f->owarn("\n[semact] non instantiated variable in where: \n",NULL);
        sterr << "WHERE VAR(" << whs->leftvarn << ") :=(...)";
        whs->whereterm.write(sterr); stout << "\n";
        trrule->dump(0);
        return(ERRORIM); }
      }
      trrule->dump(0);
      return(ERRORIM); } }

    if (whs->leftvarn == TRYCHOICEEND) 
      { 
      int locvarrename[MAXNOFVAR];
      int i; /*  save_RENAME_IDENTITY_MODE;  , save_RENAME_ALL_VARS_MODE*/
      struct WHEREbranches *brlist;
      /* term noterm; */

      for(i=0; i<MAXNOFVAR; i++)  { locvarrename[i] = ld.actvarrename[i]; }
      for(brlist = whs->wherebranch_list; brlist; brlist=brlist->next) {

      int length = 0;
      struct wherelist *long_branch =
              appendwherelists(brlist->wherebranch,whs->next,&length);
      if (fcheck_wlist(f,trrule,long_branch,r) != NORMCONT) return(ERRORIM);
      for(i=0; i<MAXNOFVAR; i++) ld.actvarrename[i] = locvarrename[i];      
      delete_n_wheres(length,long_branch);  
       }
       return NORMCONT;
      }
    else if(whs->leftvarn==WHEREPATTERN) {
	// in this case do not test already instantiated variables
      whs->pattern.ren_vars((ADDRENAME)); // all variables will be instant.
    } else
    if (whs->leftvarn!=IFVARN) {
      if (var_was_renamed(whs->leftvarn) && (ld.RENAME_ALL_VARS == 0)) {
	f->owarn(
	  "\n[semact] affectation of instantiated variable in where: ",NULL);
        stout << "VAR(" << whs->leftvarn << ")\n";
	trrule->dump(0);
	return(ERRORIM); }
      whs->leftvarterm.ren_vars((ADDRENAME));
      whs->leftvarn = var_rename(whs->leftvarn); 
    }
    whs = whs->next; }

    if (r.isvalidterm()) {
     if (r.ren_vars((CHECK))) {
       r.write(stout); stout.flush();
       f->owarn("\n[semact] non instantiated variable in right side of the rule\n",NULL);
       trrule->dump(0);
       return(ERRORIM); }
     }
  return(NORMCONT);
}

int fcheck_r(lstream *f,transrule *trrule, term r)
{
  if (r.ren_vars((CHECK))) {
    r.write(stout); stout.flush();
    f->owarn("\n[semact] non instantiated variable in right side of the rule\n",NULL);
    trrule->dump(0);
    return(ERRORIM); }
  return (NORMCONT);
}

int fcheck_tseq(lstream *f,transrule *trrule, struct tseq *tseq)
{
 int locvarrename[MAXNOFVAR];
 struct branch *br;
term noterm;
 int i;
  // stout << "fcheck_tseq  \n "; trrule->dump(0);
  if (tseq->is_case) {
    // real switch statement
    if (fcheck_wlist(f,trrule,tseq->seq,noterm) != NORMCONT) return(ERRORIM);
     // stout << "po fcheck_wlist  \n "; trrule->dump(0);
    for(i=0; i<MAXNOFVAR; i++) locvarrename[i] = ld.actvarrename[i];
    for(br = tseq->u.more_branches.brlist; br; br=br->next) {
      if (br->test.ren_vars((CHECK))) {
	br->test.write(stout); stout.flush();
	f->owarn("\n[semact] non instantiated variable in a case branch\n");
	trrule->dump(0);
	return(ERRORIM); } 
      if (fcheck_tseq(f,trrule,br->tseq) != NORMCONT) return(ERRORIM);
      for(i=0; i<MAXNOFVAR; i++) ld.actvarrename[i] = locvarrename[i];
    }
    return NORMCONT; }
  else {
    if (fcheck_wlist(f,trrule,tseq->seq,noterm) != NORMCONT) return(ERRORIM);
    if (fcheck_r(f,trrule,*(tseq->u.one_branch.result))!=NORMCONT) return(ERRORIM);
    return NORMCONT; }
}

int flowcheckrule(lstream *f,transrule *trrule)
{ int n,wm,aaa; /* ,vn */
 term l,r; /* orig_left, */
 term rlabl; /* , cond;*/
  int nm;
  int save_RENAME_IDENTITY_MODE;
  struct wherelist *whs; /*, *whss;*/

  struct tseq *rhs;
  if (trrule == NULL) return NORMCONT;
  trrule->getr(n,l,r,nm,whs,rhs,wm,rlabl);
  var_renameinit();
  aaa = ld.actvtabi;

// stout << "\nRULE BEFORE 1st PHASE\n";trrule->dump(0);

  if (fcheck_rename_rule(f,trrule,l,r,whs,rhs)!= NORMCONT) return (ERRORIM);

// stout << "\nRULE AFTER 1st PHASE\n";trrule->dump(0);

  save_RENAME_IDENTITY_MODE = ld.RENAME_IDENTITY;
  ld.RENAME_IDENTITY = 1;

  if (l.ren_vars_lin((ADDRENAME),r,&whs))
//  if (l.ren_vars((ADDRENAME))) 
    {
    f->owarn("\n[semact] do not know to compile non linear rules\n\tplease transform it into a conditional one",NULL);
    return(ERRORIM);
    }
  ld.actvtabi=aaa; // trick for preserving actvtabi
  if (rlabl.isvalidterm()) rlabl.ren_vars((ADDRENAME));
  if (!rhs)
  {
    if (fcheck_wlist(f,trrule, whs,r) != NORMCONT) return (ERRORIM);
  // c'est teste dans wlist
  //  if (fcheck_r(f,trrule,r) != NORMCONT) return (ERRORIM);
    trrule->setwheres(whs);
    //l.write(stout); stout << "\n";
    //r.write(stout); stout << "\n";
    //wherelisdump(stout,whs,5);stout << "\n";
  }
  else { // if (!rhs)
    if (fcheck_tseq(f,trrule,rhs)  != NORMCONT) return (ERRORIM);
    goto fin;
  }
  trrules.addExtRule(trrule);
  fin:

  ld.RENAME_IDENTITY = save_RENAME_IDENTITY_MODE;
  trrule->setvarn(ld.maxvarnum+1);
  return(NORMCONT);
}

int ith_subsort(int symb, int i)
{
  int k;
  struct sgrammrule *sgr = fsymtab[symb].textform();
  lexem nolexem; nolexem.crendofstreamlex();
  for(k=0; sgr->rside[k] != nolexem; k++) {
    if (sgr->rside[k].nonterminal()) i--;
    if (i==0) break;
  }
  if (sgr->rside[k] == nolexem) {
    sterr << "\n[ith_subsort] fatal - no such subterm\n";
    failexit(); 
    return 0; /* to avoid warning */
  }
  else
    return(sgr->rside[k].typeval()); 
}

int semact1(int n,lexem l,lstream *f)
{ lexem  le;
  struct sgrammrule *gr;
  lbuffer*lbuf;
  struct definedaslist *ddt;
  struct inlineslist *inl;
  char sss[STRLEN];
  int rrr;
  switch (n) {
  case 0:                                    // begin of semactions
	import.member(modframes[stacki].modname);
        init_visi(import.posid);
  	if (! batch) {
	  //---------
	  odsek(graphout,stacki<<1);
	  graphout << "handling module "<< modframes[stacki].modname;
	  graphout << "\n"; 
	  graphout.flush();
	}
      addstandards(modframes[stacki].gr);
      ld.rulepri=0; ld.finfos=FSNOINFO; ld.finfos2 = RNOPRIOR; ld.actarity=0; ld.actprofis=0;
      ld.actsemantic=0;
      ld.actcode=fsymtabi; ld.actvtabi=ld.vartabi=0; ld.nestedvartabi = NULL;
      ld.condition = trueterm; ld.actwhstrategy = -1; ld.actimp= NULL; 
      ld.wasdefinedas=0;
      break;
  case 32:  case 33:  case 34:  case 35:  case 36:
  case 37:  case 38:  case 39:  case 40:  case 41:
  case 42:  case 43:  case 44:  case 45:  case 46:
  case 47:  case 58:  case 59:  case 60:  case 61:
  case 62:  case 63:  case 64:  case 91:  case 92:
 case 93:  case 94:  case 95:  case 96:  case 123:
  case 124:  case 125:  case 126:  case 127: // characters in 'op' 
     le.crcharlex(n);
     modframes[stacki].gr.addsymbol(le);
     break;

/* from now, the actions are ordered    */


  case 8:				// module in import
	importmod(ld.actimp->s,f,-1);
        CFRE(ld.actimp); 
        ld.actimp = NULL;        
	break;
  case 18: case 19:                          // end of profil 
     if (ld.actarity!=ld.actprofis) {
        f->owarn("\n[semact] arity of term is not compatible with the profil",NULL);
        return(ERRORIM);
     }
     if ((ld.actimp->s)[0] != '<') // hack
       typecheck(f,ld.actimp->s);
     ld.actleftside.crtypelex(typet.addstr(ld.actimp->s));
     CFRE(ld.actimp->s); CFRE(ld.actimp); 
     ld.actimp=NULL;
     break;
  case 20:                                   // atrib= [AC]
     if (ld.actarity!=2) {
       f->owarn("\n[semact] only binary operator can be AC\n",NULL);
       return(ERRORIM);
     }
     ld.finfos=FSASSOCCOM;
     ld.finfos2 = RRIGHTASSOC;
     break;
  case 21:                                   // atrib= [C]
     ld.finfos=FSCOMM;
     break;
  case 22:                                   // end of one 'op' or 'str'
    { int j,k, oldfsymtabi;
      term ls, rs, rlabel;
      lexem  xi; 
      /*char *sss;*/
      int lhs_type = ld.actleftside.typeval();
      int ii = stratsort2index(lhs_type);

      gr=modframes[stacki].gr.addrule(ld.actleftside,ld.rulepri | ld.finfos2,ld.rinfos,ld.actcode);

      //dumpgrrule(gr);

      if ((!ld.in_stratop) ||  (ld.finfos2 & RBINSTR) || ld.actsemantic != 0 ||
	  ii == -1)
	add_to_fsymtab(ld.actarity,gr,0);
      else {
	//stout << "symbol " << actcode << "lhstype " << lhs_type << "," 
	//      << ii << "," << spair[ii].from << "," << spair[ii].to << "\n";
	int app_code = apply_code(ld.spair[ii].from,ld.spair[ii].to);

	/****
	if (IS_PRIMAL_SYMBOL(actsemantic))
	  add_to_fsymtab(actarity,gr,PRIMAL_FLAG);
	else {
	****/
	/*int rindex = trrules.trruleindex(
		attach_mod_loc(
			attach_type("DSTR",spair[ii].to),
			stratmoduli(spair[ii].from,spair[ii].to),
			RGLOP)
			);*/
	  //stout << "DSTR_FLAG(" << app_code << "," << actcode << ")\n";

	  add_to_fsymtab(ld.actarity,gr, DSTR_FLAG(app_code, ld.actcode)); 
      } 
      ld.rulepri=0;ld.finfos=FSNOINFO; 
      ld.actprofis=0; ld.profistcki = -1;
      ld.actcode=fsymtabi; ld.finfos2 = RNOPRIOR;
      oldfsymtabi = fsymtabi-1; //hack

      for(j=0;j<ld.numb_selectors;j++) {
        lexem ktype;
         // @.name 
         modframes[stacki].gr.addsymbol(ld.actleftside);
         le.crcharlex('.'); modframes[stacki].gr.addsymbol(le);
         le.cridlex(typet.ide(ld.selectors[j].name)); 
           modframes[stacki].gr.addsymbol(le);
//       xi.crtypelex(selectors[j].type);

         xi.crtypelex(ith_subsort(oldfsymtabi,
                                  ld.selectors[j].possition+1));

         gr=modframes[stacki].gr.addrule(xi,ld.rulepri | ld.finfos2,ld.rinfos,ld.actcode);

         add_to_fsymtab(1,gr,0);
         // r.rule f(x1,...xn).pos = pos
         ls.stinit();
         for (k=0;k<ld.actarity;k++) {
           ktype.crtypelex(ith_subsort(oldfsymtabi,ld.actarity-k));
           ls.crvar(k,ktype); }
         ls.crterm(oldfsymtabi,ld.actarity); 
         ls.crterm(ld.actcode-1,1);             // hack
         rs.stinit();
         rs.crvar(ld.actarity-ld.selectors[j].possition-1,xi);
         trrules.addrule(NULL,ld.actarity,ls,rs,
                         impmoduli,RGLOP,  // unnamed rule
                         NULL,
                         (acsymbolinleftside?ACMATCH:NORMMATCH),rlabel,NULL);  
         //----------------------
         // @[.name<-@]
         modframes[stacki].gr.addsymbol(ld.actleftside);
         le.crcharlex('['); modframes[stacki].gr.addsymbol(le);
         le.crcharlex('.'); modframes[stacki].gr.addsymbol(le);
         le.cridlex(typet.ide(ld.selectors[j].name)); 
           modframes[stacki].gr.addsymbol(le);
         le.crcharlex('<'); modframes[stacki].gr.addsymbol(le);
         le.crcharlex('-'); modframes[stacki].gr.addsymbol(le);
         modframes[stacki].gr.addsymbol(xi);
         le.crcharlex(']'); modframes[stacki].gr.addsymbol(le);
         gr=modframes[stacki].gr.
           addrule(ld.actleftside,ld.rulepri|ld.finfos2,RGLOP,ld.actcode);
         add_to_fsymtab(2,gr,0);
         // r.rule f(x1,...xn)[.pos<-val] => f...
         ls.stinit();
         ls.crvar(ld.actarity,xi);
         for (k=0;k<ld.actarity;k++) {
           ktype.crtypelex(ith_subsort(oldfsymtabi,ld.actarity-k));
           ls.crvar(k,ktype); }
         ls.crterm(oldfsymtabi,ld.actarity);  
         ls.crterm(ld.actcode-1,2);             // hack
         rs.stinit();
         for (k=0;k<ld.actarity;k++) {
           ktype.crtypelex(ith_subsort(oldfsymtabi,ld.actarity-k));
           if (k+ld.selectors[j].possition+1 == ld.actarity) {
             rs.crvar(ld.actarity,xi); }
           else {
             rs.crvar(k,ktype); }
         }
         rs.crterm(oldfsymtabi,ld.actarity);  
         trrules.addrule(NULL,ld.actarity+1,ls,rs,
                         impmoduli,RGLOP, // unnamed
                         NULL,
                         (acsymbolinleftside?ACMATCH:NORMMATCH),rlabel,NULL);
       }
     ld.actarity=0;
     break;
    }
  case 23:                                   // priority 
     ld.rulepri= l.numval();
     break;
  case 25:					// alias
     if (ld.actarity!=ld.actprofis) {
        f->owarn("\n[semact] arity of alias is not compatible with the profil\n",NULL);
        return(ERRORIM);
     }
     if ( ! modframes[stacki].gr.addalias(ld.actalr,ld.actleftside)) {
        f->owarn("\n[semact] alias not found, non existant symbol:\n",NULL);
        writegrrule(sterr,ld.actalr,&typet);
        return(ERRORIM);
     }
     fsymtab[ld.actalr->rulenumber].set_aliased();
     if (ld.wasdefinedas) {
       fsymtab[ld.actalr->rulenumber].set_definedas();
       definedasl->code = ld.actalr->rulenumber;
     }
     ld.rulepri=0; ld.finfos=FSNOINFO; ld.actarity=0; ld.actprofis=0;
     ld.finfos2 = RNOPRIOR; ld.wasdefinedas = 0;
     break;
  case 26:                                    // identifier in 'op'
     modframes[stacki].gr.addsymbol(l);
//     modframes[stacki].gr.addrw(l);
     break;
  case 27:                                   // number in 'op'
     modframes[stacki].gr.addsymbol(l);
     break;
  case 28:case 29:                           // identifier in sort declaration
      typet.addstr(ld.actimp->s);
      CFRE(ld.actimp->s); CFRE(ld.actimp); 
      ld.actimp=NULL;
      break;
  case 30:case 31:                           // identifier in profil
     typecheck(f,ld.actimp->s);
     le.crtypelex(typet.addstr(ld.actimp->s));
     modframes[stacki].gr.addnont(le);
     ld.actprofis++;
     CFRE(ld.actimp->s); CFRE(ld.actimp); 
     ld.actimp=NULL;
     ld.sel_poss++;
     break;
  case 130:                                  // add one 'op' for alias
     ld.actalr=modframes[stacki].gr.addrule(ld.actleftside,ld.rulepri|ld.finfos2,ld.rinfos,0);  
     ld.actarity=0; ld.profistcki = -1;
     break;
  case 133:                                 // add one local variable
       if (ld.actvtabi >= MAXNOFVAR) {
          f->owarn("\n[semact] too much variables in rule, more then MAXNOFVAR\n",NULL);
          return(ERRORIM);
       }
       ld.actvtab[ld.actvtabi++]=l;
       break;
#ifdef Gtyp
  case 129:
  case 132:                                 // variables type
    { int pos;
       ACTIMP2POS;
      // syntactic convention removed ..........
      if (n == 129) ld.pos_l = pos;  // if <X> ....
      ld.actvartype.crtypelex(add_strat_nont(ld.pos_l,pos)); // code of the sprofil
      break;
      }
  case 134:
    {int pos;
      ACTIMP2POS;
      ld.actvartype.crtypelex(pos);  
      break;
    }
#endif
  case 135:                                 // add variables from one declare
       for(; ld.vartabi<ld.actvtabi; ld.vartabi++) {
//          modframes[stacki].gr.addrw(actvtab[vartabi]);
          ld.vartab[ld.vartabi]= modframes[stacki].gr.addvarrule(ld.actvartype,
                                 ld.actvtab[ld.vartabi],VARSPRI,RVAR,-ld.vartabi-1);
	  ld.dollar_vartab[ld.vartabi] = modframes[stacki].gr.adddollarvarrule(ld.actvartype,
                                 ld.actvtab[ld.vartabi],VARSPRI,RVAR,-ld.vartabi-1);
       }
       break;
  case 151:          // end of strategy rule
    if (!strategywasapplied) 
    { lexem reslex;
      reslex.crtypelex(ld.actruletypeindex);
      ld.actwhstrategy = trrules.strategyindex_refs(attach_type_mod(EVALSTR,
			ld.actruletypeindex,
			evalmoduli(ld.actruletypeindex_l,ld.actruletypeindex)));
      ld.acttrrule->addwhere(reverse_wheres,ld.actvtabi,
			  (ld.actwhstrategy==-1)?((strategy**)NULL):
			  trrules.getstrategyadr_refs(ld.actwhstrategy),
			  ld.dstr_rs,reslex);
      ld.actwhstrategy = -1; // ??? 2705
    }
  // SHOULD CONTINUE WITH case 136
  case 136:                                // end of rule
       // remove all variable rules
       while( ld.actvtabi > ld.nestedvartabi->i) {
          ld.actvtabi--;
          modframes[stacki].gr.deleterule(ld.vartab[ld.actvtabi]);
          modframes[stacki].gr.deleterule(ld.dollar_vartab[ld.actvtabi]);
         }
       ld.actvtabi = ld.vartabi = ld.nestedvartabi->i;
       ld.actruletype = ld.nestedvartabi->ruletype;
       { struct nvlist *ll; 
         ll= ld.nestedvartabi ->next; DELETE1(ld.nestedvartabi); 
         ld.nestedvartabi = ll;
       }
       break;
  case 137:                               // where variable 
      ld.strstacki=1;
      ld.strstack[0]= ld.strstack[1]= NULL;
      NNEW(ld.actstrategy ,strategy);      
//      appactstrat();
      //actwhstrategy = actstratindex;
       ld.pattype = 0;
      rrr = sem_action_137(f,l);
      snprintf(sss,sizeof(sss),"WHERE%d",ld.wherecount);
      ld.actstratindex = trrules.strategyindex_refs(             // like in case 158
		attach_type_mod(sss,ld.actwheretype.typeval(),impmoduli));  
      return rrr;
  case 138:
    trrules.strategyremove_refs(ld.actstratindex);					       
    return sem_action_138(f);
  case 142:
    { strategy *str;
      int ss;
    ld.actwhstrategy = ld.actstratindex;
    str = ld.strstack[0];
    if ( (ss = str->is_call())) {
      // stout << str->is_call() << stratrules->ide(ss) << "\n";

      trrules.strategyremove_refs(ld.actstratindex);
      //actwhstrategy = trrules.strategyindex_refs(attach_mod(stratrules->ide(ss),impmoduli));
      ld.actwhstrategy = trrules.strategyindex_refs(
		 attach_type_mod(stratrules->ide(ss),ld.actwheretype.typeval(),impmoduli));
      //   DELETE1(str); 
      return sem_action_138(f);
      }
    else {
      snprintf(sss,sizeof(sss),"WHERE%d",ld.wherecount);
      sem_action_167(f,trrules.strategyindex_defs(
		attach_type_mod(sss,ld.actwheretype.typeval(),impmoduli),RLOCOOP),
		     ld.actwheretype.typeval());
      ld.wherecount++; }
    }
    if (!handlewherepattern(f,ld.pattype,NULL)) return(HANDERRORIM);
    else return(NORMCONT); 
  case 161:	// choose
    { struct WHEREbranches *whbrs;
     // stout << " CHOOSE - \n";
     whbrs = parse_try(f);
     if (ld.acttrrule) { // elan rule
	ld.acttrrule->addtrywhere(reverse_wheres,whbrs); }
     if (ld.acttrrulelab) { // strategy LAB_ rule
//140898	acttrrulelab->addtrywhere(reverse_wheres,whbrs); 
	ld.acttrrulelab->addtrywhere(reverse_wheres,copy_WHEREbranches(whbrs)); 
     }
     // stout << " - END\n";
    }
     break;
  case 139:                               // if term in trrule
         return sem_action_139(f);					      
  case 140:				  // global operation def.
        ld.rinfos=RGLOP; modframes[stacki].importrinfos = RGLOP;
	break;
  case 141:				  // local operation def.
        ld.rinfos=RLOCOOP;modframes[stacki].importrinfos = RLOCOOP;
	break;
  case 150:  // RWrules
        ld.rinfos=RLOCOOP;modframes[stacki].importrinfos = RLOCOOP;
	ld.in_strategies = 0;
        //rinfos=RGLOP; modframes[stacki].importrinfos = RGLOP;  // rules are global by default
	goto l_143;
  case 143:				  // initial declare in rbody
	l_143:					       
	create_nested();                  // or even if no declares
        ld.acttrrulename=NULL;
	break;
  case 144:				  // module was parsed,
					  // make glob grammar
        globtermgr.addgrammar(modframes[stacki].gr,RGLOP,RGLOP);
        globtermgr.addgrammar(modframes[stacki].gr,RLOCOOP,RGLOP);
	{
	  /* char *visi; */
          /* int i;*/
	  import.member(modframes[stacki].modname);
	  /* visi = visibilities[import.posid]; */
	  if (! batch) {
	    odsek(graphout,stacki<<1);
	    graphout << "end of " << modframes[stacki].modname;
//	    for(i=0; i < MAXNOFIMPORTS; i++) 
//	      if (visi[i] > 0) graphout << " G" << i;
//	      else if (visi[i] < 0) graphout << " L" << i; 
	    graphout << "\n";
	  graphout.flush(); }
	}
	if (ld.ignore == 0 && modframes[stacki].gr.anysymbol_exists()) {
	  char mname[STRLEN], fname[STRLEN];
	  ld.is_explimpl = 1;
	  snprintf(mname,sizeof(mname),"explimpl%d",ld.explimpl_index);
	  snprintf(fname,sizeof(fname),"explimpl%d.eln",ld.explimpl_index);
	  ld.ignore++;
	  modframes[stacki].gr.any_code(mname,fname, modframes[stacki].modname); 
	  ld.ignore--;
	}
	if (modframes[stacki].gr.symbappl_exists()) {
	  char mname[STRLEN], fname[STRLEN];
	  ld.is_symbappl = 1;
	  snprintf(mname,sizeof(mname),"symbappl%d",ld.symbappl_index);
	  snprintf(fname,sizeof(fname),"symbappl%d.eln",ld.symbappl_index);
	  modframes[stacki].gr.symbappl_code(mname,fname, modframes[stacki].modname); 
	}
	break;
  case 145:				// code handl
        ld.actcode=l.numval();
	  if (! builtinmodules.member(modframes[stacki].filemodname))
	    f->owarn("[warning] using the 'code' option in user's module\n",NULL);
        break;
  case 146:                               // pattern
      ld.pattype = typet.addstr(ld.actimp->s);
      CFRE(ld.actimp->s); CFRE(ld.actimp); 
      ld.actimp=NULL;
      break;
  case 147:
 //   if (actstrategy) DELETE1(actstrategy);
    if (!handlewherepattern(f,ld.pattype,NULL)) return(HANDERRORIM);
    else return(NORMCONT); 
  case 148:      // code nnn
  case 149:      // code -nnn
        ld.actsemantic=l.numval();
	if (! builtinmodules.member(modframes[stacki].filemodname))
	  f->owarn("[warning] using the 'semantic' option in user's module\n",NULL);
	if (n == 148) ld.actcode = ld.actsemantic;                       // like plus
	else ld.actsemantic=-ld.actsemantic;						
        break;
  case 152:
     ld.finfos2 = RBINSTR;
     break;
  case 153:                                   // place for argument in 'op' 
     le.crendofstreamlex();
     modframes[stacki].gr.addsymbol(le);
     ld.actarity++;
     break;
  case 154:                                  // name of transision rule
     ld.acttrrulename = l.alfsy();
     break; 
  case 155:                                  // body of the transition rule
     {int i,j,pom, app_code; /* rindex, rindex_orig, */
     char *rname;
     struct sgrammrule *gr;
     lexem strlex,le,lle;
     term ter;
     term rlabel;                           // term corresponding to a label

     if (! handlerulebody(f)) return(HANDERRORIM);

     ld.acttrrulelab = NULL; ld.acttrrule = NULL;
     if (ld.acttrrulename) { 
      term lside1, rside1;   // named rule
         int strx, stry;
     if (ld.act_rhs) goto fin1;
      if (ld.in_strategies) { 
	ld.stratindex = ld.strattype; 
	goto fin1; }
      if (is_def_str(ld.actruletypeindex_l,ld.actruletypeindex) == -1) goto fin1;
       le.cridlex(ld.acttrrulename); modframes[stacki].gr.addsymbol(le);
       ld.lside.copy(lside1); ld.rside.copy(rside1);
       if (ld.actlvtabi > 0) {
	 le.crcharlex('('); modframes[stacki].gr.addsymbol(le);
	 for (i=0; i<ld.actlvtabi; i++) {
           for(j = 0; j<ld.actvtabi; j++)
	     if (ld.actvtab[j] == ld.actlvtab[i]) break;
	   if (j >= ld.actvtabi) {
	       f->owarn("\n[semact] variable not declared\n",NULL);
	       return(ERRORIM); }
	   pom = (ld.vartab[j]->leftside).typeval();
	   if (lside1.cont_var(j)&&rside1.cont_var(j))  // occurs on both side 
             le.crtypelex(add_strat_nont(pom,pom));     // strategy argument
	   else 
	     le.crtypelex(pom);                         // else ordinal
	   modframes[stacki].gr.addsymbol(le);
	   if (i+1 < ld.actlvtabi) {
	     le.crcharlex(','); modframes[stacki].gr.addsymbol(le); }
	 }
	 le.crcharlex(')'); modframes[stacki].gr.addsymbol(le);
       }
     strlex.crtypelex(add_strat_nont(ld.actruletypeindex_l,ld.actruletypeindex));
     gr = modframes[stacki].gr.addrule(strlex,RNOPRIOR,ld.rinfos,fsymtabi);

     rname = attach_type("LAB",ld.actruletypeindex);
     trrules.trruleindex(rname);
     trrules.trruleindex(
		attach_mod_loc(
		  attach_type(ld.acttrrulename,ld.actruletypeindex_l),
		  impmoduli,
		  ld.rinfos));

//0106     add_to_fsymtab(actlvtabi,gr,LAB_FLAG(rindex_orig)); 

     app_code = apply_code(ld.actruletypeindex_l,ld.actruletypeindex);

     //stout << "LAB_FLAG(" << app_code << "," << actcode << ")\n";

     add_to_fsymtab(ld.actlvtabi,gr,LAB_FLAG(app_code,ld.actcode)); 

     //-------- create term for the label
     rlabel.stinit();
     for (i=ld.actlvtabi; i>0; ) { i--;
       for(j = 0; j<ld.actvtabi; j++) if (ld.actvtab[j] == ld.actlvtab[i]) break; 
       lle.crtypelex((ld.vartab[j]->leftside).typeval());
       rlabel.crvar(-(ld.vartab[j]->rulenumber)-1,lle); }
     rlabel.crterm(fsymtabi-1);  // should be actcode
     rlabel.popt();
     {    // complex_label
       term ls, *rlab;         //..........ADDING
       int wherei;
       for (i=ld.actlvtabi; i>0; ) { // rename strategy variables
	i--;
        for(j = 0; j<ld.actvtabi; j++) if (ld.actvtab[j] == ld.actlvtab[i]) break; 
        if (lside1.cont_var(j) && rside1.cont_var(j))  {
	  lside1.ren_var(j,ld.vartabi+2*i); rside1.ren_var(j,ld.vartabi+2*i+1); } }
     ls.stinit(); 

     ls.pusht(lside1); 
     ls.pusht(rlabel);
     ls.crterm(app_code,2);
     ls.popt();
     //ls.write(stout); stout << "\n"; stout.flush();
     //ls.write(stout); stout << "===>"; rside1.write(stout); stout << "\n";
     NNEW(rlab,term);
     ld.acttrrulelab = 
        trrules.addrule(rname,
			ld.vartabi,ls,rside1,
			stratmoduli(ld.actruletypeindex_l,ld.actruletypeindex),
			RGLOP,       // AAA
				    NULL,
		          (acsymbolinleftside?ACMATCH:NORMMATCH),*rlab,NULL);
     //stout << "##1 ##"; acttrrulelab->dump(0); stout << "\n";
     for (i=ld.actlvtabi; i>0; ) { // rename strategy variables
       i--;
       for(j = 0; j<ld.actvtabi; j++) if (ld.actvtab[j] == ld.actlvtab[i]) break; 
       if (lside1.cont_var(ld.vartabi+2*i) && rside1.cont_var(ld.vartabi+2*i+1))  {
	  wherei = (ld.vartab[j]->leftside).typeval();
          ld.actwheretype.crtypelex(wherei);
	  ter.stinit(); 
          ter.crvar(ld.vartabi+2*i,ld.actwheretype); 
          ter.crvar(j,strlex);  // i
          ter.crterm(apply_code(wherei,wherei),2);
          ter.popt();
       	  inverse_apply_code(ter,&strx,&stry);
	  ld.actwhstrategy = trrules.strategyindex_refs(
		attach_type_mod(EVALSTR,wherei,evalmoduli(strx,stry)));
				// EEEE evalmoduli(wherei)));
	  ld.acttrrulelab->addwhere(reverse_wheres,ld.vartabi+2*i+1,
			      (ld.actwhstrategy==-1)?((strategy**)NULL):
			      trrules.getstrategyadr_refs(ld.actwhstrategy),
			      ter,ld.actwheretype); 
	  ld.actwhstrategy = -1; // 3005
	}
     } //for (i=actlvtabi; i>0; )
    }
     if (ld.actlvtabi) { ld.actlvtabi = 0; break; /*goto fin2;*/ }
     else goto fin1;  // fujjjjj
  }  // if (acttrulename)
  fin1:
  ld.actlvtabi = 0;
  if (ld.actruletypeindex_l != ld.actruletypeindex) {
    f->owarn("\nthis case is not implemented\n",NULL); failexit(); }
  ld.acttrrule = 
    trrules.addrule(attach_type(ld.acttrrulename,ld.actruletypeindex_l),
		    ld.vartabi,ld.lside,ld.rside,
//*********
// here, there is a proble, see tcstrat.eln (rules vs strategies
		    ((ld.in_strategies)? 
		     stratmoduli(ld.actruletypeindex_l,ld.actruletypeindex):
		     impmoduli), 
//********

		    ((ld.in_strategies)?RGLOP:ld.rinfos),  // HACK a reflechir
			      ld.act_rhs,
			(acsymbolinleftside?ACMATCH:NORMMATCH),rlabel,NULL);
	//stout << "##2 ##"; acttrrule->dump(0); stout << "\n";
     }
     /* fin2: */ break; 
    case 156: // dotname [.]
      ld.acttrrulename = "DSTR";
      break;
  //------------ Marian's strategies
  case 158:				// name of strategy / begin of str.
        ld.actstrategyname = l.alfsy();
	ld.strstacki=1;
	ld.strstack[0]= ld.strstack[1]= NULL;
	NNEW(ld.actstrategy ,strategy);
	break; 
  case 159: // NEW
	ld.actstrategy->setprocmaxn(ld.calledstr);
        ld.actstrategy->setname(STRCALL,impmoduli);
        /* ---  !!! a continue !!! --- */
        [[fallthrough]];
  case 160:				// another element. strategy
	appactstrat();
	break;
  case 163:				// repeat / iterate (begin)
	grow_strstack(ld.strstacki+3);
	ld.strstack[ld.strstacki+1]=ld.strstack[ld.strstacki+2]=NULL;
	ld.strstacki+=2;
	break;
  case 164:				// endrepeat
	ld.actstrategy->setname(STRNAMEREPEAT,impmoduli); 
	ld.actstrategy->setsubst(ld.strstack[ld.strstacki-1]);
	ld.strstacki-=2;
	break;
  case 165:				// enditerate
	ld.actstrategy->setname(STRNAMEITERATE,impmoduli);
	ld.actstrategy->setsubst(ld.strstack[ld.strstacki-1]);
	ld.strstacki-=2;
	break;
  case 166:                             // enditerateplus
        {
         // iterate+ A = A;iterate A  // la meme connerie pour repart+
         strategy *ss,*ww;
         ww = ss = ld.strstack[ld.strstacki-1];
         ld.strstacki-=2;
         while(ss) {
            *ld.actstrategy = *(ss->copy());   // copy
            appactstrat();
            ss = ss->nex();
         }
	ld.actstrategy->setname(STRNAMEITERATE,impmoduli); 
	ld.actstrategy->setsubst(ww);
        }
        break;
  case 167:                             // typed strategy
	ld.actstratindex = trrules.strategyindex_defs(
          attach_type_mod(ld.actstrategyname,ld.stratindex,impmoduli),ld.rinfos);
        return sem_action_167(f,ld.actstratindex,ld.stratindex);
/*
  case 168:  // type of strategy
        typecheck(f,actimp->s);
        stratindex = typet.addstr(actimp->s);
        CFRE(actimp->s); CFRE(actimp); 
        actimp=NULL;
        break;
*/
  case 169:				// module name
	if (strcmp(ld.actargmodname,l.alfsy())) {
	  f->owarn("\n[semact] name of module doesn't correspond with the name of file\n",NULL);
	  return(ERRORIM);
	}
	break;
  case 170:                         // normalise
	ld.actstrategy->setname(STRNAMENORMALISE,impmoduli); 
	ld.actstrategy->setsubst(ld.strstack[ld.strstacki-1]);
	ld.strstacki-=2;
	break;
  case 171:                         // repeat+
        {
         // tranformation: repeat+ A = A;repaet A
         strategy *ss,*ww;
         ww = ss = ld.strstack[ld.strstacki-1];
         ld.strstacki-=2;
         while(ss) {
            *ld.actstrategy = *(ss->copy());   // copy
            appactstrat();
            ss = ss->nex();
         }
	ld.actstrategy->setname(STRNAMEREPEAT,impmoduli); 
	ld.actstrategy->setsubst(ww);
        }
        break;
  case 174: // pour l'instant = dont care // first choose finish
  case 175:				  // dont care2 finish
       	ld.actstrategy->setname(STRNAMEDONTCARE2,impmoduli);	
	ld.actstrategy->setstl(ld.strlstack[ld.strlstacki-2]);
	ld.strstacki-=2;
	ld.strlstacki-=2;
	break;

 case 250:		      // [Huy: May  4 00] 	 
	ld.actstrategy->setname(STRNORM_IN,impmoduli);
	ld.actstrategy->setstl(ld.strlstack[ld.strlstacki-2]);
	ld.strstacki-=2;
	ld.strlstacki-=2;
	break;

 case 251:		      // [Huy: May  4 00] 	 
	ld.actstrategy->setname(STRNORM_OUT,impmoduli);
	ld.actstrategy->setstl(ld.strlstack[ld.strlstacki-2]);
	ld.strstacki-=2;
	ld.strlstacki-=2;
	break;
 case 252:		// [pem: Oct 26 00] endtall
	ld.actstrategy->setname(STRNAMETALL,impmoduli); 
	ld.actstrategy->setsubst(ld.strstack[ld.strstacki-1]);
	ld.strstacki-=2;
	break;
 case 253:		// [pem: Oct 26 00] endtall
	ld.actstrategy->setname(STRNAMETONE,impmoduli); 
	ld.actstrategy->setsubst(ld.strstack[ld.strstacki-1]);
	ld.strstacki-=2;
	break;
 case 254:		// [pem: Oct 26 00] endtall
	ld.actstrategy->setname(STRNAMETSOME,impmoduli); 
	ld.actstrategy->setsubst(ld.strstack[ld.strstacki-1]);
	ld.strstacki-=2;
	break;

 case 255:		// [pem: Apr  8 02] endrewrite
	ld.actstrategy->setname(STRNAMEREWRITE,impmoduli); 
	ld.actstrategy->setsubst(ld.strstack[ld.strstacki-1]);
	ld.strstacki-=2;
	break;
  case 176:				// dont care2 finish
	ld.actstrategy->setname(STRNAMEDONTKNOW2,impmoduli);
	ld.actstrategy->setstl(ld.strlstack[ld.strlstacki-2]);
	ld.strstacki-=2;
	ld.strlstacki-=2;
	break;
  case 162:				// dont care2 finish
	ld.actstrategy->setname(STRNAMEDONTCARECON2,impmoduli);
	ld.actstrategy->setstl(ld.strlstack[ld.strlstacki-2]);
	ld.strstacki-=2;
	ld.strlstacki-=2;
	break;
  case 168:				// dont care2 finish
	ld.actstrategy->setname(STRNAMEONECON2,impmoduli);
	ld.actstrategy->setstl(ld.strlstack[ld.strlstacki-2]);
	ld.strstacki-=2;
	ld.strlstacki-=2;
	break;
  case 181:				// dont care2 finish
	ld.actstrategy->setname(STRNAMEDONTKNOWCON2,impmoduli);
	ld.actstrategy->setstl(ld.strlstack[ld.strlstacki-2]);
	ld.strstacki-=2;
	ld.strlstacki-=2;
	break;
  case 179:				// one2 finish
	ld.actstrategy->setname(STRNAMEONE2,impmoduli);
	ld.actstrategy->setstl(ld.strlstack[ld.strlstacki-2]);
	ld.strstacki-=2;
	ld.strlstacki-=2;
	break;
  case 214:				// typed normalise finish
                                        // normalise(s1:S1, ..., sn:Sn)
	ld.actstrategy->setname(STRNAMENORMALISE2,impmoduli);
	ld.actstrategy->setstl_no_test(ld.strlstack[ld.strlstacki-2]);
	ld.strstacki-=2;
	ld.strlstacki-=2;
	break;
  case 177:				// first elem in strlist
  case 211:                             //               strtypedlist
	{ struct strlist *ss;
	  AALLOS(ss , struct strlist);
	  ss->next = NULL;
	  ss->str = ld.strstack[ld.strstacki-1];
          if (n == 211)
            ss->str->settypeof(ld.sntype.typeval());
	  ld.strstack[ld.strstacki-1]=ld.strstack[ld.strstacki]=NULL;
//	  NNEW(actstrategy ,strategy);
	  grow_strlstack(ld.strlstacki+2);
	  ld.strlstack[ld.strlstacki++] = ss;
	  ld.strlstack[ld.strlstacki++] = ss;
	}
	break;
  case 178:				// another elem in strlist
  case 212:                             //                 strtypedlist
	{ struct strlist *ss;
	  AALLOS(ss , struct strlist);
	  ss->next = NULL;
	  ss->str = ld.strstack[ld.strstacki-1];
          if (n == 212)
            ss->str->settypeof(ld.sntype.typeval());
	  ld.strstack[ld.strstacki-1]=ld.strstack[ld.strstacki]=NULL;
//	  NNEW(actstrategy ,strategy);
	  ld.strlstack[ld.strlstacki-1]->next = ss;
	  ld.strlstack[ld.strlstacki-1] = ss;
	}
	break;
  //------------ Marian's strategies
  case 180:  // stratcall
//        calledstr = stratrules->addstr(attach_type(l.alfsy(),stratindex));
        ld.calledstr = stratrules->addstr(l.alfsy());
        break; 
  case 182:
        le.crcharlex(ld.symbolcode);
        modframes[stacki].gr.addsymbol(le);
        break;
  case 183:
        ld.symbolcode = l.numval();
        break;
  case 184: { int err;                  // check variables in tr.rule 
					// && generate extensional rule
      // stout << "\nBEFORE FLOW #1 acttrrule\n"; acttrrule->dump(0);
      // if (acttrrulelab) { stout << "\nBEFORE FLOW #2 acttrrulelab\n"; acttrrulelab->dump(0); }

        if ((err = flowcheckrule(f,ld.acttrrule)) != NORMCONT) return err;
        return(flowcheckrule(f,ld.acttrrulelab));
       }
       break;
  case 185:				// identifier in import
        mkmodname1(ld.actimp,l);
	break;
  case 186:
        mkmodname2(ld.actimp);
	break;
  case 187:
        mkmodname3(ld.actimp);
        break;
  case 172: // <X>
        mkmodname4(ld.actimp);
        break;
  case 173: // <X->Y>
        mkmodname5(ld.actimp);
        break;
  case 188:
	{ lbuffer bf;
	  lexem le;
	  if (ld.actarglist==NULL) {
	     f->oerr("number of formal and actual parameters for module ",
	     modframes[stacki].modname," doesn't correspond\n",NULL);
	     return(HANDERRORIM);
          }
          if (int_arg(ld.actarglist)) {
            // stout << "passujem " << atoi(actarglist->s) << " ako int argument \n";
	    le.crnumlex( atoi(ld.actarglist->s) );
	    bf.put(le);
          } else 
           {
          char ss[STRLEN];
            int  ii, jj;
            int chr;
            ii = 0; jj = 0;
            // stout << "passujem " << actarglist->s << " ako string argument \n";
            while((chr=ld.actarglist->s[ii])) {
             if (chr == '[' || chr == ']' || 
                 chr == ',' || 
                 chr == '<' || 
                 chr == '-' || 
                 chr == '>' || 
                 chr == '|') {
               if (jj) { ss[jj++] = 0; le.cridlex(&(ss[0])); bf.put(le); jj = 0; }
               le.crcharlex(chr);
               bf.put(le); }
             else ss[jj++] = chr;
             ii++;
            }
            if (jj) { ss[jj++] = 0; le.cridlex(&(ss[0])); bf.put(le); jj = 0; }
          }
	  f->addmac(l.idval(),bf);
          ld.actarglist=ld.actarglist->next; 
	}
	break;
  case 189:
	if (ld.actarglist!=NULL) {
	     f->oerr("number of actual and formal parameters for module ",NULL);
	     f->oerr(modframes[stacki].modname," doesn't correspond\n",NULL);
	     return(HANDERRORIM);
	}
	break;
  case 190:  // strategy fail
        ld.actstrategy->setname(STRFAIL,impmoduli);
        break;
  case 191:
	ld.actstrategy->setname(STRNAMEDCPROCESSCALL,impmoduli);
	addstratprocgr();
	break;
  case 192:
	ld.actstrategy->setname(STRNAMEDKPROCESSCALL,impmoduli);
	addstratprocgr();
	break;
  case 193:
	ld.actstrategy->setprocname(l.alfsy());
	break;
  case 194:
	ld.actstrategy->setprocmaxn(l.numval());
	break;
  case 195:
        ld.actstrategy->setname(STRMETA,impmoduli);
        break;
  case 196:
        ld.actstrategy->setname(STRIDENTITY,impmoduli);
        break;
  case 197:
        ld.actstrategy->setproctype(typet.addstr(ld.actimp->s));
        CFRE(ld.actimp); 
        ld.actimp = NULL;
	break;
  case 198:
	ld.finfos2 = RLEFTASSOC;
	break;
  case 199:
	ld.finfos2 = RRIGHTASSOC;
	break;
  case 201:                                 // end of imports
	fsymtab_remakealias(&(modframes[stacki].gr));
        break;
  case 202:                                 // default local import
        ld.rinfos=RLOCOOP; modframes[stacki].importrinfos = RLOCOOP;
	break;
  case 203:                                 // inline in strategy
	ld.actstrategy->setname(STRINLINE,impmoduli);
        lbuf = f->getlastinlineAndinit();
        if (lbuf == NULL) {
	 f->oerr("\tno previous inline (i.e. /# ... #/) construction defined\n");
	  return(HANDERRORIM);
	}
        ld.actstrategy->setinlinedbuffer(lbuf);
        break;
  case 204:                                  // definedas in op definition
        AALLOS(ddt,struct definedaslist);
	ddt->fname = l;
	ddt->next = definedasl;             
        definedasl = ddt;
	ld.wasdefinedas=1;
        break;
  case 205:                                   // inline at the end of module
        AALLOS(inl,struct inlineslist);
        inl->text = f->getlastinlineAndinit();
        inl->next = inlinesl;
        inlinesl = inl;
        break;
  case 206:                                   // hard Alias
    if (ld.actarity!=ld.actprofis) {
      f->owarn("\n[semact] arity of hardAlias definition is not compatible with the profil\n",NULL);
      return(ERRORIM);
    }
    if ( ! (modframes[stacki].gr.addhardalias(ld.actalr,ld.actleftside))) {
      f->owarn("\n[semact] hardAlias was not found\n",NULL);
      return(ERRORIM);
    }
    fsymtab[ld.actalr->rulenumber].set_aliased();
    if (ld.wasdefinedas) {
      fsymtab[ld.actalr->rulenumber].set_definedas();
      definedasl->code = ld.actalr->rulenumber;
    }
    ld.rulepri=0; ld.finfos=FSNOINFO; ld.actarity=0; ld.actprofis=0;
    ld.finfos2 = RNOPRIOR; ld.wasdefinedas = 0;
    break;
  case 207:                                    // hard alias beggining
    if (ld.actarity!=ld.actprofis) {
      f->owarn("\n[semact] arity of hardAlias is not compatible with the profil\n",NULL);
      return(ERRORIM);
    }
    ld.actprofis=0;
    break;
  case 208:
    modframes[stacki].gr.combine(); // (NEW)
    if (ld.in_stratmoduli) {
      /*int i,j;*/
     if (inlinecodesi >= (int)inlinecodes.size()) inlinecodes.resize(inlinecodesi + 1);
     inlinecodes[inlinecodesi].from = ld.stratmoduli_fromi; 
     inlinecodes[inlinecodesi].to   = ld.stratmoduli_toi;
//     inlinecodes[inlinecodesi].stratsort = add_strat_nont(stratmoduli_fromi,stratmoduli_toi);
     modframes[stacki].gr.lookinlinecode(ld.stratmoduli_fromi,ld.stratmoduli_toi);
     inlinecodesi++; 
    }
    break;
  case 210:                                    // epsilon lexem
     le.crblanklex();
     modframes[stacki].gr.addsymbol(le);
    break;
  case 213:
    typecheck(f,ld.actimp->s);
    ld.sntype.crtypelex(typet.addstr(ld.actimp->s));
    CFRE(ld.actimp->s); CFRE(ld.actimp); 
    ld.actimp=NULL;
    break;
  // local strategies

  case 240:
     {
     struct ilist *p;
     int ind; 
     *ld.locstratend = NULL;

     for (ld.locstratlen = 0,p = ld.locstrat; p; p=p->next) 
       ld.locstratlen++;

     NNEW(ld.locstrattable, int[ld.locstratlen]);
     //AALLOSS(locstrattable, locstratlen ,int);

     //stout << "local strategy\n";
     for (ind = 0; ld.locstrat; ld.locstrat=ld.locstrat->next) {
     //  stout << locstrat->i << ":"; 
       ld.locstrattable[ind++] = ld.locstrat->i; }
     //stout << "\n";

     ld.locstrat = NULL; ld.locstratend = &ld.locstrat; 
    break; }
  case 241:
  case 242:
    {
    NNEW(*ld.locstratend ,struct ilist);
    (*ld.locstratend)->i =  l.numval(); (*ld.locstratend)->next = ld.locstrat;
    ld.locstratend = &((*ld.locstratend)->next);
    break; }
  }
  return(NORMCONT);
}
 
int semact(int n,lexem l,lstream *f)
{
int r;
  if (n<300) {
   // stout << "LEX=" << n << "\n";
  r = semact1(n,l,f);
  return r; }
  return(semact3(n,l,f));
  return(NORMCONT);
}









