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

 int rulen,rulepri;                       // the number and the priority of the currently parsed (CP) rule 
 int rinfos,finfos2;                      // syntactic priority and associativity of CP symbol
 int finfos;                              // is the CP symbol AC ?
 int actarity,actprofis,actcode;          // arity of the CP symbol; CP profil; and current code number
 int actsemantic = 0;                     // arity of the CP symbol; CP profil; and current code number
 lexem actleftside;                       // codomain of the CP profile
 lexem actruletype;                       // type of terms in the CP rul
 lexem sntype;

 lexem actvartype,actvartype1;            // CP variables type
 lexem actwheretype;                      // type of the CP where affectation
 int actwherevar,actwhstrategy,actstratindex;  // CP variable of a where; its strategy; and index of the strategy 
 int wasdefinedas;                        // only for the COMPILER, is the symbol defined as an inlined function?
 struct sgrammrule *actalr;               // CP functional symbol grammar rule
 struct nvlist *nestedvartabi=NULL;                          // a stack of var declarations (for the case of nested declarations using 
						//				nested "rules" construction)
 struct sgrammrule *vartab[MAXNOFVAR];    // grammar rules for variables
 struct sgrammrule *dollar_vartab[MAXNOFVAR];// copie de vartab pour les
                                     //regles de variable avec un dollar
 lexem actvtab[MAXNOFVAR];                // names of variables
 int actvarrename[MAXNOFVAR];             // table used during the renaming of variables inside RW-rules
 int actvarnum;                           // the real number of variables inside an RW-rule
 int maxvarnum = 0;
 int actvtabi,vartabi;                    // numbers of variables (two values differ during parsing variable declarations
						//			 for the same type)
 term lside,rside,condition;              // left hand side, right hand side and the condition of the CP RW rule
 const char *actmodname[MAXINCLDEEP];           // stack of the names of CP modules 
 char *actfilemodname[MAXINCLDEEP];       // stack of the files where the CP modules are placed
// int newmodule[MAXINCLDEEP];
 grammar grstack[MAXINCLDEEP];            // stack of the grammars of CP modules
 int stacki = 0;                              // index to modules stacks
 int importrinfos[MAXINCLDEEP];           // kind (GLOBAL/LOCAL) of the CP import of a module
 strategy *strstack[MAXINCLSTRAT];        // stack used while parsing nested strategies (REPEAT,ITERATE)
 int strstacki=0;                         // index to strstack
 struct strlist *strlstack[MAXINCLSTRAT]; // stack used while parsing nested list of strategies (DONT CARE/KNOW CHOOSE)
 int strlstacki=0;                        // index to strlstack
 term ter1;                               // a temporary term variable
 term ter2;                               // a temporary term variable
 const char *acttrrulename;                     // the name of the CP RW rule
 const char *actstrategyname;                   // the name of MV strategy
 transrule *acttrrule;                    // the CP RW rule
 strategy *actstrategy;                   // the CP strategy
 char *actargmodname;                     // the name of the CP module
 int symbolcode;                          // the value of a character defined by its ASCII code
 int stratindex;                          // type index of Marian's strategy
 int impmoduli = -1;                      // index of currently imported module
 int in_stratmoduli = 0;                  // true if in str* module
 int stratmoduli_fromi = -1;              // X of str* module
 int stratmoduli_toi = -1;                // Y
 term dstr_rs;                            // right-hand side of a dstr rule
 int is_explimpl = 0;            // explode-implode module should be loaded
 int ignore = 0;                 // ignore deeper levels
 int explimpl_index = 0;         //  file counter
 #define MAXANYS 100             // limit (not tested!!!)
 int     anysi = 0;              // pointer to anys
 int     anys[MAXANYS];          // modules for which any[X] has been imported
 int in_strategies = 0;
 int in_stratop = 0;
 int strattype = -1;
 int is_symbappl = 0;
 #define MAXSYMBAPPL             100
 int symbappli = 0;
 struct ilist *symbappl[MAXSYMBAPPL];
 int symbappl_index = 0;         // file counter; 
int withrhs = 0;
struct tseq *act_rhs;
int  pattype = 0;
int locstratlen = 0;
int *locstrattable = NULL;         // table of local strategies
struct ilist *locstrat = NULL;     // local strategy of a symbol
struct ilist **locstratend = &locstrat;     // local strategy end

int RENAME_ALL_VARS = 0;
int RENAME_IDENTITY = 0;

extern lexem strIdentLex;
extern int handlewherepattern(lstream *f, int pattyp, struct wherelist *wl);
 struct chlist *actarglist,*actimp;	// used to pass the arg. also
char *visibilities[MAXNOFIMPORTS];      // matrix of module-to-module visibilities
 
extern int pretydumpsitset(lstream *f,stringtab *types);
extern int acsymbolinleftside;		             // body in esemact.c   | is there an AC symbol in the left hand side of CP rule?
extern void esemactinit();		             // body in esemact.c   | init term construction semantic actions
extern int pretydumpsitset(lstream *f,stringtab *types); // pretty dump of earley's situations table
int calledstr;                                       // strategy in call(..)
int wherecount = 0;
extern int msyntan(lstream &,int,void (*ltol)(lexem l1,lexem &l2));    // parser for an elan (.eln) module
extern struct WHEREbranches *parse_try(lstream *f);


int stratmoduli(int x, int y)
{
char modnam[STRLEN];
  if (x==y)
    sprintf(modnam,"%s[%s]",STRAT_MODNAME1,typet.ide(x));
  else
    sprintf(modnam,"%s[%s,%s]",STRAT_MODNAME2,typet.ide(x),typet.ide(y));
  if (!import.member(modnam)) { 
    sterr << "\nmissing file " << modnam << "\n";
    sterr << "\n[fatal] internal error\n"; failexit(); }
  return import.posid;
}

int evalmoduli(int x, int y) 
{
char modnam[STRLEN];
  if (x == y) 
    sprintf(modnam,"%s[%s]",STRAT_MODNAME1,typet.ide(x)); 
  else {
    sprintf(modnam,"%s[%s,%s]",STRAT_MODNAME2,typet.ide(x),typet.ide(y)); }

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
  sprintf(full_name,"%s%s%s",s,TYPE_SEPARATOR,typet.ide(t));
  return strdup(full_name);
}

char *attach_mod(const char *s,int modu)
{
char full_name[STRLEN];
  if (s == NULL || strlen(s) == 0) return NULL; /////// exception for nonamed rules
  sprintf(full_name,"%s%s%s",s,MODULE_SEPARATOR,import.ide(modu));
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
  sprintf(full_name,"%s%s%s%s%s",s,
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
  sprintf(sss,"%s%s",s1,TYPE_SEPARATOR);
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
  ll->i = vartabi;
  ll-> ruletype = actruletype;
  ll->next = nestedvartabi; nestedvartabi = ll;
}

void add_to_fsymtab(int actarityy,struct sgrammrule *gr, int defstrat)
{
  fsym s(actarityy,gr,finfos);
  if (actcode>=MAXCODE) return;             // strategies and statements
  if (actcode>=MAXNFSYM) {
    sterr << "There is more functional symbols then MAXNFSYM="
          << MAXNFSYM << "\n\tsorry, FATAL !!!\n";
    failexit();
  }
  s.set_semantic(actsemantic); 
  s.set_locstrat(locstratlen,locstrattable); locstratlen = 0;locstrattable = NULL;
  gr->defstrat = defstrat;
  gr->semantic =actsemantic;
  actsemantic = 0;
  fsymtab[actcode]=s;
  gr->fsymcode = actcode;
  if (actcode == fsymtabi) fsymtabi++;
  actcode = fsymtabi;
}

void mkmodname1(struct chlist *&actimp,lexem l)
{ struct chlist *chpp;
        NNEW(chpp ,struct chlist);
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
{ return(& grstack[stacki]);
}

void setmodname(const char *s, char *ss)
{
  actmodname[stacki]=s; actfilemodname[stacki] = ss;
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
  g->addgrammar(grstack[stacki],RLOCOOP,RGLOP);
  g->addgrammar(grstack[stacki],RLOCOOP|RIMPORTBIT,RGLOP);
  actstrategy->setprocgr(g);
}

int sem_action_167(lstream *f, int actsindex, int styp)
{
  trrules.settypeofstrategy_defs(actsindex,styp);
  strstack[0]->settypeof(styp);

  all_strateg[all_strategi++] = strstack[0];    // sauvgarde chaque strategie definie

  if (trrules.setstrategy_defs(actsindex,strstack[0])) {
    if (!batch) f->owarn("[warning] double definition of strategy\n",
			 trrules.strategyname_refs(actsindex),NULL); 
    return(NORMCONT);
  } 
  return(NORMCONT);
}

int sem_action_137(lstream *f,lexem l)
{
  actwherevar=vartabi-1;
  while (actwherevar>=0 && actvtab[actwherevar]!=l)actwherevar--; 
  if (actwherevar<0) {
    f->owarn("\n[semact] variable name expected after where ",NULL);
    return(ERRORIM);
  }
  actwheretype=vartab[actwherevar]->leftside; 
  stratindex = actwheretype.typeval();
  return NORMCONT;
}

int sem_action_138(lstream *f)
{ int resan;
  resan = grstack[stacki].earleycall(f,actwheretype,endofin);
  if (! resan) return(HANDERRORIM);
  ter1.popt();
  if (acttrrule) { // elan rule
	   acttrrule->addwhere(reverse_wheres,actwherevar,
		  (actwhstrategy==-1)?((strategy**)NULL):
                  trrules.getstrategyadr_refs(actwhstrategy),ter1,actwheretype); }
  if (acttrrulelab) { // strategy LAB_ rule
	 ter1.copyrec(ter2);
	 acttrrulelab->addwhere(reverse_wheres,actwherevar,
		 (actwhstrategy==-1)?((strategy**)NULL):
				trrules.getstrategyadr_refs(actwhstrategy),ter2,actwheretype); }
  actwhstrategy = -1;
  return NORMCONT;
}

int sem_action_139(lstream *f)
{ int resan;
  term cond;
  term cond1;
  resan = grstack[stacki].earleycall(f,booltype,endofin);
  if (! resan) return(HANDERRORIM);
  cond.popt();
  if (acttrrule) {
    acttrrule->addwhere(reverse_wheres,IFVARN,NULL,cond,booltype);
  }
  if (acttrrulelab) {
    cond.copyrec(cond1);
    acttrrulelab->addwhere(reverse_wheres,IFVARN,NULL,cond1,booltype); }
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
  if (pattyp) { le.crtypelex(pattyp); grstack[stacki].addsymbol(le);}
  le.crcharlex(':'); grstack[stacki].addsymbol(le);
  le.crcharlex('='); grstack[stacki].addsymbol(le);
  le.crcharlex('('); grstack[stacki].addsymbol(le);  // pour l'instant
  le.crcharlex(')'); grstack[stacki].addsymbol(le);
  le.crtypelex(wtype); grstack[stacki].addsymbol(le);
  le.crtypelex(STARTTYPE); 
  mainrule1=grstack[stacki].addrule(le,RNOPRIOR,RNOINFO,RIGHTSRULE);

  //--- where (type)pattern:=(sss)term
  if (pattyp) { le.crtypelex(pattyp); grstack[stacki].addsymbol(le);}
  le.crcharlex(':'); grstack[stacki].addsymbol(le);
  le.crcharlex('='); grstack[stacki].addsymbol(le);
  le.crcharlex('('); grstack[stacki].addsymbol(le);  
                     grstack[stacki].addsymbol(internIdentType);  
  le.crcharlex(')'); grstack[stacki].addsymbol(le);
  le.crtypelex(wtype); grstack[stacki].addsymbol(le);
  le.crtypelex(STARTTYPE); 
  mainrule2=grstack[stacki].addrule(le,RNOPRIOR,RNOINFO,RIGHTSRULE);

  //--- where (type)pattern:=[SSS]term
  if (pattyp) { le.crtypelex(pattyp); grstack[stacki].addsymbol(le);}
  le.crcharlex(':'); grstack[stacki].addsymbol(le);
  le.crcharlex('='); grstack[stacki].addsymbol(le);
  le.crcharlex('['); grstack[stacki].addsymbol(le);  
  le.crtypelex(wtype); grstack[stacki].addsymbol(le);
  le.crtypelex(STARTTYPE); 
  mainrule3=grstack[stacki].addrule(le,RNOPRIOR,RNOINFO,STRATCONSTRULE);

  le.crtypelex(STARTTYPE); 
// CSBug  esemactinit();
  resan = grstack[stacki].earleycall(f,le,fin);
  if (! resan) return(resan);

  grstack[stacki].deleterule(mainrule1);
  grstack[stacki].deleterule(mainrule2);
  grstack[stacki].deleterule(mainrule3);

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
       resan = grstack[stacki].earleycall(f,booltype,endofinco);
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
       pattype = 0;
       if (NORMCONT != sem_action_137(f,le)) { 
         f->owarn(" [fatal] error in parse_seq \n",NULL); failexit(); }
       ILEX(le);
       resan = get_pattern_where(f,0,actwheretype.typeval(),endofinco);
//-------------
       if (! resan) { f->owarn(" [fatal] error in parse_seq \n",NULL); failexit(); }
       rside2.popt(); // rside2.write(stout); stout << "\n";
       if (strIdentVal) {
         actwhstrategy = trrules.strategyindex_refs(
           attach_type_mod(strIdentLex.alfsy(),actwheretype.typeval(),impmoduli)); }
       else if (strategywasapplied) {
         int strx, stry;
         // S2: strx/stry were used uninitialised when rside2 is not an applied code
         if (!inverse_apply_code(rside2,&strx,&stry)) interr();
         actwhstrategy = trrules.strategyindex_refs(
           attach_type_mod(EVALSTR,actwheretype.typeval(),
              evalmoduli(strx,stry))); 
       }
       else
         actwhstrategy = -1 ; 
//---------------
       p->leftvarn = actwherevar; 
       p->leftvarterm.crvar(actwherevar,actwheretype);
       p->leftvarterm.popt();
       p->strateg = (actwhstrategy==-1)?((strategy**)NULL):
                     trrules.getstrategyadr_refs(actwhstrategy);
       actwhstrategy = -1 ;  //3005
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
     resan=grstack[stacki].earleycall(f,actruletype,endofinco);
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
    actwhstrategy = trrules.strategyindex_refs(
	attach_type_mod(strIdentLex.alfsy(),pattyp,impmoduli)); 
  else if (strategywasapplied) {
    int strx, stry;
    // S2: strx/stry were used uninitialised when rside2 is not an applied code
    if (!inverse_apply_code(rside2,&strx,&stry)) interr();
   actwhstrategy = trrules.strategyindex_refs(
	attach_type_mod(EVALSTR,pattyp,
			evalmoduli(strx,stry)));
  }
  else
    actwhstrategy = -1 ; 
  
  if (wl) { // pattern where
	//	 stout << " ???? ???? \n";
    addpatternwhere(wl,(actwhstrategy==-1)?((strategy**)NULL):
        trrules.getstrategyadr_refs(actwhstrategy),lside2,rside2,pattyp); }
  else if (acttrrule) { // elan rule
     acttrrule->addpatternwhere(reverse_wheres,
     (actwhstrategy==-1)?((strategy**)NULL):
        trrules.getstrategyadr_refs(actwhstrategy),lside2,rside2,pattyp); }
  if (acttrrulelab) { // strategy LAB_ rule
     lside2.copyrec(lside1);
     rside2.copyrec(rside1);
     acttrrulelab->addpatternwhere(reverse_wheres,
        (actwhstrategy==-1)?((strategy**)NULL):
	trrules.getstrategyadr_refs(actwhstrategy),lside1,rside1,pattyp); }
  actwhstrategy = -1 ; // 3005
  return(resan);
}

int handlerulebody(lstream *f)
{ lexem le;
  struct sgrammrule *mainrule,*mainrule1,*rightsrule;
  int resan;
// RIGHTSTYPE ---------
  grstack[stacki].addsymbol(actruletype);
  le.crtypelex(RIGHTSTYPE);
  rightsrule=grstack[stacki].addrule(le,RNOPRIOR,RNOINFO,RIGHTSRULE);
// RULECONSTRULE I -------
  grstack[stacki].addsymbol(actruletype_l);
  le.crcharlex('='); grstack[stacki].addsymbol(le);
  le.crcharlex('>'); grstack[stacki].addsymbol(le);
  le.crtypelex(RIGHTSTYPE); grstack[stacki].addnont(le);
  le.crtypelex(STARTTYPE); 
  mainrule=grstack[stacki].addrule(le,RNOPRIOR,RNOINFO,RULECONSTRULE);
// RULECONSTRULE II -------
  grstack[stacki].addsymbol(actruletype_l);
  le.crcharlex('='); grstack[stacki].addsymbol(le);
  le.crcharlex('>'); grstack[stacki].addsymbol(le);
  le.crtypelex(STARTTYPE); 
  mainrule1=grstack[stacki].addrule(le,RNOPRIOR,RNOINFO,RULECONSTRULE1);
// PARSE --------
  le.crtypelex(STARTTYPE); 
  esemactinit();
  resan = grstack[stacki].earleycall(f,le,
				     endofinco
				     );
  if (! resan) return(resan);
  lside.popt(); 
  if (withrhs) { act_rhs = NULL; rside.popt(); }
  else { act_rhs = parse_tseq(f); 
    f->fulex(le);
    if (le != Send) { 
      f->owarn(" [fatal] end expected after switch\n",NULL); failexit(); }
   }
//--------------------------
  grstack[stacki].deleterule(mainrule1);
  grstack[stacki].deleterule(mainrule);
  grstack[stacki].deleterule(rightsrule);
  return(resan);
}

void appactstrat()
{ 
  if (strstack[strstacki] == NULL) 
	strstack[strstacki-1] = actstrategy;
  else  strstack[strstacki]->setnext(actstrategy);
  strstack[strstacki] = actstrategy;
  NNEW(actstrategy ,strategy);
}

void var_renameinit()
{ int p;
  actvarnum = 0; maxvarnum = 0;
  for (p=0; p<MAXNOFVAR; p++) actvarrename[p] = NORENAME;
}

int var_rename(int v)
{
  if (actvarrename[v] == NORENAME) actvarrename[v]= actvarnum++;
  if (RENAME_IDENTITY) {
      if (v > maxvarnum) maxvarnum = v;
      return v;
  }
  else {
      if (actvarrename[v] > maxvarnum) maxvarnum = actvarrename[v];
      return(actvarrename[v]); }
}

int var_was_renamed(int v)
{
  return(actvarrename[v] != NORENAME);
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
   impmod = impmoduli; in_stratmod = in_stratmoduli;
   impmoduli = import.addstr(name);
   importglobgr[import.posid] = NULL;
   divideonargs(name,actargmodname,&actarglist);
   // printargs(actarglist);
   in_stratmoduli = ISSTRATSIG(actargmodname); //(ISSTRAT1MOD(actargmodname) || ISSTRAT2MOD(actargmodname));
   if ISSYMBOLMOD(actargmodname) {
     struct ilist **rr;
     struct chlist *arglist = actarglist;
     if (arglist) arglist = arglist->next; // skip the arity
     rr = &(symbappl[symbappli]);
     while (arglist) {
       NNEW(*rr, struct ilist);
       (*rr)->i = typet.addstr(arglist->s);
       rr = &((*rr)->next);
       arglist = arglist->next; }
     *rr = NULL; symbappli++;
   }
   if ISANYMOD(actargmodname) {
     anys[anysi++] = typet.addstr(actarglist->s); }
   if (ISSTRAT1MOD(actargmodname) || ISSTRAT2MOD(actargmodname)) { 
     struct chlist *p;
     addit = 1;
     stratmoduli_fromi = stratmoduli_toi = typet.addstr(actarglist->s); p = actarglist;
     if ISSTRAT2MOD(actargmodname) { p = p->next; stratmoduli_toi = typet.addstr(p->s); }
   }
   modsou = addsuffix(actargmodname,".eln");
   mlstream ff(modsou); ///mlstream/lstream <=> with/without preprocessor !!!!!
   if (stacki >= MAXINCLDEEP) {
     f->owarn("includes are deepest then MAXINCLDEEP =",NULL);
     failexit(); }
   setmodname(name,actargmodname);
   if (!msyntan(ff,0,lextomodlex)) failexit();
   CFRE(modsou);

   if  (addit) {
     trclos(f); // transitive closure
   } 

   //stout << "NAME = " << name << "\n";
   import.member(name);
   impmoduli = impmod; in_stratmoduli = in_stratmod;
   NNEW(importglobgr[import.posid] ,grammar);

   importglobgr[import.posid]->addgrammar(grstack[stacki],RGLOP|RIMPORTBIT,RGLOP);
   importglobgr[import.posid]->addgrammar(grstack[stacki],RGLOP,RGLOP);

   grstack[stacki].freetopl();

   conform_strategies(1); 
   trrules.assign_all_refs(1); // WWW = 0
}

void importmod(char *impmodule,lstream *f, int supermodule)
{
  importmod_inf( impmodule, f, supermodule,  importrinfos[stacki]);
}

void importmod_inf(const char *impmodule,lstream *f, int supermodule, int rinf)
{
  int xx=0; /* initialised to avoid warning */
  if (supermodule != -1) { xx = impmoduli; impmoduli= supermodule; }
	if (! import.member(impmodule)) {
            stacki++;
            readmodules(f,impmodule);
            grstack[stacki].freetopl();
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
            grstack[stacki].addgrammar(
		  *importglobgr[import.posid],RGLOP,
		  rinf|RIMPORTBIT); 
	  // grstack[stacki].dump();
	    if (is_explimpl) {
	      char mname[STRLEN];
	      is_explimpl = 0;
	      sprintf(mname,"explimpl%d",explimpl_index++);
	      importmod(mname,f,-1);
	      system("/bin/rm -f explimpl*.eln");
	    }
	    if (is_symbappl) {
	      char mname[STRLEN];
	      is_symbappl = 0;
	      sprintf(mname,"symbappl%d",symbappl_index++);
	      importmod(mname,f,-1);
	      system("/bin/rm -f symbappl*.eln");
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
       save_RENAME_ALL_VARS_MODE = RENAME_ALL_VARS; // global programming :-)))
       RENAME_ALL_VARS = 1;
         l.ren_vars((ADDRENAME));
         if (tseq == NULL) {
           r.ren_vars((ADDRENAME));
           fcheck_rename_wlist(f,trrule,whs);
         } else {
           fcheck_rename_tseq(f,trrule,tseq);
         }
       RENAME_ALL_VARS = save_RENAME_ALL_VARS_MODE;
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

      for(i=0; i<MAXNOFVAR; i++)  { locvarrename[i] = actvarrename[i]; }
      for(brlist = whs->wherebranch_list; brlist; brlist=brlist->next) {

      int length = 0;
      struct wherelist *long_branch =
              appendwherelists(brlist->wherebranch,whs->next,&length);
      if (fcheck_wlist(f,trrule,long_branch,r) != NORMCONT) return(ERRORIM);
      for(i=0; i<MAXNOFVAR; i++) actvarrename[i] = locvarrename[i];      
      delete_n_wheres(length,long_branch);  
       }
       return NORMCONT;
      }
    else if(whs->leftvarn==WHEREPATTERN) {
	// in this case do not test already instantiated variables
      whs->pattern.ren_vars((ADDRENAME)); // all variables will be instant.
    } else
    if (whs->leftvarn!=IFVARN) {
      if (var_was_renamed(whs->leftvarn) && (RENAME_ALL_VARS == 0)) {
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
    for(i=0; i<MAXNOFVAR; i++) locvarrename[i] = actvarrename[i];
    for(br = tseq->u.more_branches.brlist; br; br=br->next) {
      if (br->test.ren_vars((CHECK))) {
	br->test.write(stout); stout.flush();
	f->owarn("\n[semact] non instantiated variable in a case branch\n");
	trrule->dump(0);
	return(ERRORIM); } 
      if (fcheck_tseq(f,trrule,br->tseq) != NORMCONT) return(ERRORIM);
      for(i=0; i<MAXNOFVAR; i++) actvarrename[i] = locvarrename[i];
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
  aaa = actvtabi;

// stout << "\nRULE BEFORE 1st PHASE\n";trrule->dump(0);

  if (fcheck_rename_rule(f,trrule,l,r,whs,rhs)!= NORMCONT) return (ERRORIM);

// stout << "\nRULE AFTER 1st PHASE\n";trrule->dump(0);

  save_RENAME_IDENTITY_MODE = RENAME_IDENTITY;
  RENAME_IDENTITY = 1;

  if (l.ren_vars_lin((ADDRENAME),r,&whs))
//  if (l.ren_vars((ADDRENAME))) 
    {
    f->owarn("\n[semact] do not know to compile non linear rules\n\tplease transform it into a conditional one",NULL);
    return(ERRORIM);
    }
  actvtabi=aaa; // trick for preserving actvtabi
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

  RENAME_IDENTITY = save_RENAME_IDENTITY_MODE;
  trrule->setvarn(maxvarnum+1);
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
	import.member(actmodname[stacki]);
        init_visi(import.posid);
  	if (! batch) {
	  //---------
	  odsek(graphout,stacki<<1);
	  graphout << "handling module "<< actmodname[stacki];
	  graphout << "\n"; 
	  graphout.flush();
	}
      addstandards(grstack[stacki]);
      rulepri=0; finfos=FSNOINFO; finfos2 = RNOPRIOR; actarity=0; actprofis=0;
      actsemantic=0;
      actcode=fsymtabi; actvtabi=vartabi=0; nestedvartabi = NULL;
      condition = trueterm; actwhstrategy = -1; actimp= NULL; 
      wasdefinedas=0;
      break;
  case 32:  case 33:  case 34:  case 35:  case 36:
  case 37:  case 38:  case 39:  case 40:  case 41:
  case 42:  case 43:  case 44:  case 45:  case 46:
  case 47:  case 58:  case 59:  case 60:  case 61:
  case 62:  case 63:  case 64:  case 91:  case 92:
 case 93:  case 94:  case 95:  case 96:  case 123:
  case 124:  case 125:  case 126:  case 127: // characters in 'op' 
     le.crcharlex(n);
     grstack[stacki].addsymbol(le);
     break;

/* from now, the actions are ordered    */


  case 8:				// module in import
	importmod(actimp->s,f,-1);
        CFRE(actimp); 
        actimp = NULL;        
	break;
  case 18: case 19:                          // end of profil 
     if (actarity!=actprofis) {
        f->owarn("\n[semact] arity of term is not compatible with the profil",NULL);
        return(ERRORIM);
     }
     if ((actimp->s)[0] != '<') // hack
       typecheck(f,actimp->s);
     actleftside.crtypelex(typet.addstr(actimp->s));
     CFRE(actimp->s); CFRE(actimp); 
     actimp=NULL;
     break;
  case 20:                                   // atrib= [AC]
     if (actarity!=2) {
       f->owarn("\n[semact] only binary operator can be AC\n",NULL);
       return(ERRORIM);
     }
     finfos=FSASSOCCOM;
     finfos2 = RRIGHTASSOC;
     break;
  case 21:                                   // atrib= [C]
     finfos=FSCOMM;
     break;
  case 22:                                   // end of one 'op' or 'str'
    { int j,k, oldfsymtabi;
      term ls, rs, rlabel;
      lexem  xi; 
      /*char *sss;*/
      int lhs_type = actleftside.typeval();
      int ii = stratsort2index(lhs_type);

      gr=grstack[stacki].addrule(actleftside,rulepri | finfos2,rinfos,actcode);

      //dumpgrrule(gr);

      if ((!in_stratop) ||  (finfos2 & RBINSTR) || actsemantic != 0 ||
	  ii == -1)
	add_to_fsymtab(actarity,gr,0);
      else {
	//stout << "symbol " << actcode << "lhstype " << lhs_type << "," 
	//      << ii << "," << spair[ii].from << "," << spair[ii].to << "\n";
	int app_code = apply_code(spair[ii].from,spair[ii].to);

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

	  add_to_fsymtab(actarity,gr, DSTR_FLAG(app_code, actcode)); 
      } 
      rulepri=0;finfos=FSNOINFO; 
      actprofis=0; profistcki = -1;
      actcode=fsymtabi; finfos2 = RNOPRIOR;
      oldfsymtabi = fsymtabi-1; //hack

      for(j=0;j<numb_selectors;j++) {
        lexem ktype;
         // @.name 
         grstack[stacki].addsymbol(actleftside);
         le.crcharlex('.'); grstack[stacki].addsymbol(le);
         le.cridlex(typet.ide(selectors[j].name)); 
           grstack[stacki].addsymbol(le);
//       xi.crtypelex(selectors[j].type);

         xi.crtypelex(ith_subsort(oldfsymtabi,
                                  selectors[j].possition+1));

         gr=grstack[stacki].addrule(xi,rulepri | finfos2,rinfos,actcode);

         add_to_fsymtab(1,gr,0);
         // r.rule f(x1,...xn).pos = pos
         ls.stinit();
         for (k=0;k<actarity;k++) {
           ktype.crtypelex(ith_subsort(oldfsymtabi,actarity-k));
           ls.crvar(k,ktype); }
         ls.crterm(oldfsymtabi,actarity); 
         ls.crterm(actcode-1,1);             // hack
         rs.stinit();
         rs.crvar(actarity-selectors[j].possition-1,xi);
         trrules.addrule(NULL,actarity,ls,rs,
                         impmoduli,RGLOP,  // unnamed rule
                         NULL,
                         (acsymbolinleftside?ACMATCH:NORMMATCH),rlabel,NULL);  
         //----------------------
         // @[.name<-@]
         grstack[stacki].addsymbol(actleftside);
         le.crcharlex('['); grstack[stacki].addsymbol(le);
         le.crcharlex('.'); grstack[stacki].addsymbol(le);
         le.cridlex(typet.ide(selectors[j].name)); 
           grstack[stacki].addsymbol(le);
         le.crcharlex('<'); grstack[stacki].addsymbol(le);
         le.crcharlex('-'); grstack[stacki].addsymbol(le);
         grstack[stacki].addsymbol(xi);
         le.crcharlex(']'); grstack[stacki].addsymbol(le);
         gr=grstack[stacki].
           addrule(actleftside,rulepri|finfos2,RGLOP,actcode);
         add_to_fsymtab(2,gr,0);
         // r.rule f(x1,...xn)[.pos<-val] => f...
         ls.stinit();
         ls.crvar(actarity,xi);
         for (k=0;k<actarity;k++) {
           ktype.crtypelex(ith_subsort(oldfsymtabi,actarity-k));
           ls.crvar(k,ktype); }
         ls.crterm(oldfsymtabi,actarity);  
         ls.crterm(actcode-1,2);             // hack
         rs.stinit();
         for (k=0;k<actarity;k++) {
           ktype.crtypelex(ith_subsort(oldfsymtabi,actarity-k));
           if (k+selectors[j].possition+1 == actarity) {
             rs.crvar(actarity,xi); }
           else {
             rs.crvar(k,ktype); }
         }
         rs.crterm(oldfsymtabi,actarity);  
         trrules.addrule(NULL,actarity+1,ls,rs,
                         impmoduli,RGLOP, // unnamed
                         NULL,
                         (acsymbolinleftside?ACMATCH:NORMMATCH),rlabel,NULL);
       }
     actarity=0;
     break;
    }
  case 23:                                   // priority 
     rulepri= l.numval();
     break;
  case 25:					// alias
     if (actarity!=actprofis) {
        f->owarn("\n[semact] arity of alias is not compatible with the profil\n",NULL);
        return(ERRORIM);
     }
     if ( ! grstack[stacki].addalias(actalr,actleftside)) {
        f->owarn("\n[semact] alias not found, non existant symbol:\n",NULL);
        writegrrule(sterr,actalr,&typet);
        return(ERRORIM);
     }
     fsymtab[actalr->rulenumber].set_aliased();
     if (wasdefinedas) {
       fsymtab[actalr->rulenumber].set_definedas();
       definedasl->code = actalr->rulenumber;
     }
     rulepri=0; finfos=FSNOINFO; actarity=0; actprofis=0;
     finfos2 = RNOPRIOR; wasdefinedas = 0;
     break;
  case 26:                                    // identifier in 'op'
     grstack[stacki].addsymbol(l);
//     grstack[stacki].addrw(l);
     break;
  case 27:                                   // number in 'op'
     grstack[stacki].addsymbol(l);
     break;
  case 28:case 29:                           // identifier in sort declaration
      typet.addstr(actimp->s);
      CFRE(actimp->s); CFRE(actimp); 
      actimp=NULL;
      break;
  case 30:case 31:                           // identifier in profil
     typecheck(f,actimp->s);
     le.crtypelex(typet.addstr(actimp->s));
     grstack[stacki].addnont(le);
     actprofis++;
     CFRE(actimp->s); CFRE(actimp); 
     actimp=NULL;
     sel_poss++;
     break;
  case 130:                                  // add one 'op' for alias
     actalr=grstack[stacki].addrule(actleftside,rulepri|finfos2,rinfos,0);  
     actarity=0; profistcki = -1;
     break;
  case 133:                                 // add one local variable
       if (actvtabi >= MAXNOFVAR) {
          f->owarn("\n[semact] too much variables in rule, more then MAXNOFVAR\n",NULL);
          return(ERRORIM);
       }
       actvtab[actvtabi++]=l;
       break;
#ifdef Gtyp
  case 129:
  case 132:                                 // variables type
    { int pos;
       ACTIMP2POS;
      // syntactic convention removed ..........
      if (n == 129) pos_l = pos;  // if <X> ....
      actvartype.crtypelex(add_strat_nont(pos_l,pos)); // code of the sprofil
      break;
      }
  case 134:
    {int pos;
      ACTIMP2POS;
      actvartype.crtypelex(pos);  
      break;
    }
#endif
  case 135:                                 // add variables from one declare
       for(; vartabi<actvtabi; vartabi++) {
//          grstack[stacki].addrw(actvtab[vartabi]);
          vartab[vartabi]= grstack[stacki].addvarrule(actvartype,
                                 actvtab[vartabi],VARSPRI,RVAR,-vartabi-1);
	  dollar_vartab[vartabi] = grstack[stacki].adddollarvarrule(actvartype,
                                 actvtab[vartabi],VARSPRI,RVAR,-vartabi-1);
       }
       break;
  case 151:          // end of strategy rule
    if (!strategywasapplied) 
    { lexem reslex;
      reslex.crtypelex(actruletypeindex);
      actwhstrategy = trrules.strategyindex_refs(attach_type_mod(EVALSTR,
			actruletypeindex,
			evalmoduli(actruletypeindex_l,actruletypeindex)));
      acttrrule->addwhere(reverse_wheres,actvtabi,
			  (actwhstrategy==-1)?((strategy**)NULL):
			  trrules.getstrategyadr_refs(actwhstrategy),
			  dstr_rs,reslex);
      actwhstrategy = -1; // ??? 2705
    }
  // SHOULD CONTINUE WITH case 136
  case 136:                                // end of rule
       // remove all variable rules
       while( actvtabi > nestedvartabi->i) {
          actvtabi--;
          grstack[stacki].deleterule(vartab[actvtabi]);
          grstack[stacki].deleterule(dollar_vartab[actvtabi]);
         }
       actvtabi = vartabi = nestedvartabi->i;
       actruletype = nestedvartabi->ruletype;
       { struct nvlist *ll; 
         ll= nestedvartabi ->next; CFRE(nestedvartabi); 
         nestedvartabi = ll;
       }
       break;
  case 137:                               // where variable 
      strstacki=1;
      strstack[0]= strstack[1]= NULL;
      NNEW(actstrategy ,strategy);      
//      appactstrat();
      //actwhstrategy = actstratindex;
       pattype = 0;
      rrr = sem_action_137(f,l);
      sprintf(sss,"WHERE%d",wherecount);
      actstratindex = trrules.strategyindex_refs(             // like in case 158
		attach_type_mod(sss,actwheretype.typeval(),impmoduli));  
      return rrr;
  case 138:
    trrules.strategyremove_refs(actstratindex);					       
    return sem_action_138(f);
  case 142:
    { strategy *str;
      int ss;
    actwhstrategy = actstratindex;
    str = strstack[0];
    if ( (ss = str->is_call())) {
      // stout << str->is_call() << stratrules->ide(ss) << "\n";

      trrules.strategyremove_refs(actstratindex);
      //actwhstrategy = trrules.strategyindex_refs(attach_mod(stratrules->ide(ss),impmoduli));
      actwhstrategy = trrules.strategyindex_refs(
		 attach_type_mod(stratrules->ide(ss),actwheretype.typeval(),impmoduli));
      //   DELETE1(str); 
      return sem_action_138(f);
      }
    else {
      sprintf(sss,"WHERE%d",wherecount);
      sem_action_167(f,trrules.strategyindex_defs(
		attach_type_mod(sss,actwheretype.typeval(),impmoduli),RLOCOOP),
		     actwheretype.typeval());
      wherecount++; }
    }
    if (!handlewherepattern(f,pattype,NULL)) return(HANDERRORIM);
    else return(NORMCONT); 
  case 161:	// choose
    { struct WHEREbranches *whbrs;
     // stout << " CHOOSE - \n";
     whbrs = parse_try(f);
     if (acttrrule) { // elan rule
	acttrrule->addtrywhere(reverse_wheres,whbrs); }
     if (acttrrulelab) { // strategy LAB_ rule
//140898	acttrrulelab->addtrywhere(reverse_wheres,whbrs); 
	acttrrulelab->addtrywhere(reverse_wheres,copy_WHEREbranches(whbrs)); 
     }
     // stout << " - END\n";
    }
     break;
  case 139:                               // if term in trrule
         return sem_action_139(f);					      
  case 140:				  // global operation def.
        rinfos=RGLOP; importrinfos[stacki] = RGLOP;
	break;
  case 141:				  // local operation def.
        rinfos=RLOCOOP;importrinfos[stacki] = RLOCOOP;
	break;
  case 150:  // RWrules
        rinfos=RLOCOOP;importrinfos[stacki] = RLOCOOP;
	in_strategies = 0;
        //rinfos=RGLOP; importrinfos[stacki] = RGLOP;  // rules are global by default
	goto l_143;
  case 143:				  // initial declare in rbody
	l_143:					       
	create_nested();                  // or even if no declares
        acttrrulename=NULL;
	break;
  case 144:				  // module was parsed,
					  // make glob grammar
        globtermgr.addgrammar(grstack[stacki],RGLOP,RGLOP);
        globtermgr.addgrammar(grstack[stacki],RLOCOOP,RGLOP);
	{
	  /* char *visi; */
          /* int i;*/
	  import.member(actmodname[stacki]);
	  /* visi = visibilities[import.posid]; */
	  if (! batch) {
	    odsek(graphout,stacki<<1);
	    graphout << "end of " << actmodname[stacki];
//	    for(i=0; i < MAXNOFIMPORTS; i++) 
//	      if (visi[i] > 0) graphout << " G" << i;
//	      else if (visi[i] < 0) graphout << " L" << i; 
	    graphout << "\n";
	  graphout.flush(); }
	}
	if (ignore == 0 && grstack[stacki].anysymbol_exists()) {
	  char mname[STRLEN], fname[STRLEN];
	  is_explimpl = 1;
	  sprintf(mname,"explimpl%d",explimpl_index);
	  sprintf(fname,"explimpl%d.eln",explimpl_index);
	  ignore++;
	  grstack[stacki].any_code(mname,fname, actmodname[stacki]); 
	  ignore--;
	}
	if (grstack[stacki].symbappl_exists()) {
	  char mname[STRLEN], fname[STRLEN];
	  is_symbappl = 1;
	  sprintf(mname,"symbappl%d",symbappl_index);
	  sprintf(fname,"symbappl%d.eln",symbappl_index);
	  grstack[stacki].symbappl_code(mname,fname, actmodname[stacki]); 
	}
	break;
  case 145:				// code handl
        actcode=l.numval();
	  if (! builtinmodules.member(actfilemodname[stacki]))
	    f->owarn("[warning] using the 'code' option in user's module\n",NULL);
        break;
  case 146:                               // pattern
      pattype = typet.addstr(actimp->s);
      CFRE(actimp->s); CFRE(actimp); 
      actimp=NULL;
      break;
  case 147:
 //   if (actstrategy) DELETE1(actstrategy);
    if (!handlewherepattern(f,pattype,NULL)) return(HANDERRORIM);
    else return(NORMCONT); 
  case 148:      // code nnn
  case 149:      // code -nnn
        actsemantic=l.numval();
	if (! builtinmodules.member(actfilemodname[stacki]))
	  f->owarn("[warning] using the 'semantic' option in user's module\n",NULL);
	if (n == 148) actcode = actsemantic;                       // like plus
	else actsemantic=-actsemantic;						
        break;
  case 152:
     finfos2 = RBINSTR;
     break;
  case 153:                                   // place for argument in 'op' 
     le.crendofstreamlex();
     grstack[stacki].addsymbol(le);
     actarity++;
     break;
  case 154:                                  // name of transision rule
     acttrrulename = l.alfsy();
     break; 
  case 155:                                  // body of the transition rule
     {int i,j,pom, app_code; /* rindex, rindex_orig, */
     char *rname;
     struct sgrammrule *gr;
     lexem strlex,le,lle;
     term ter;
     term rlabel;                           // term corresponding to a label

     if (! handlerulebody(f)) return(HANDERRORIM);

     acttrrulelab = NULL; acttrrule = NULL;
     if (acttrrulename) { 
      term lside1, rside1;   // named rule
         int strx, stry;
     if (act_rhs) goto fin1;
      if (in_strategies) { 
	stratindex = strattype; 
	goto fin1; }
      if (is_def_str(actruletypeindex_l,actruletypeindex) == -1) goto fin1;
       le.cridlex(acttrrulename); grstack[stacki].addsymbol(le);
       lside.copy(lside1); rside.copy(rside1);
       if (actlvtabi > 0) {
	 le.crcharlex('('); grstack[stacki].addsymbol(le);
	 for (i=0; i<actlvtabi; i++) {
           for(j = 0; j<actvtabi; j++)
	     if (actvtab[j] == actlvtab[i]) break;
	   if (j >= actvtabi) {
	       f->owarn("\n[semact] variable not declared\n",NULL);
	       return(ERRORIM); }
	   pom = (vartab[j]->leftside).typeval();
	   if (lside1.cont_var(j)&&rside1.cont_var(j))  // occurs on both side 
             le.crtypelex(add_strat_nont(pom,pom));     // strategy argument
	   else 
	     le.crtypelex(pom);                         // else ordinal
	   grstack[stacki].addsymbol(le);
	   if (i+1 < actlvtabi) {
	     le.crcharlex(','); grstack[stacki].addsymbol(le); }
	 }
	 le.crcharlex(')'); grstack[stacki].addsymbol(le);
       }
     strlex.crtypelex(add_strat_nont(actruletypeindex_l,actruletypeindex));
     gr = grstack[stacki].addrule(strlex,RNOPRIOR,rinfos,fsymtabi);

     rname = attach_type("LAB",actruletypeindex);
     trrules.trruleindex(rname);
     trrules.trruleindex(
		attach_mod_loc(
		  attach_type(acttrrulename,actruletypeindex_l),
		  impmoduli,
		  rinfos));

//0106     add_to_fsymtab(actlvtabi,gr,LAB_FLAG(rindex_orig)); 

     app_code = apply_code(actruletypeindex_l,actruletypeindex);

     //stout << "LAB_FLAG(" << app_code << "," << actcode << ")\n";

     add_to_fsymtab(actlvtabi,gr,LAB_FLAG(app_code,actcode)); 

     //-------- create term for the label
     rlabel.stinit();
     for (i=actlvtabi; i>0; ) { i--;
       for(j = 0; j<actvtabi; j++) if (actvtab[j] == actlvtab[i]) break; 
       lle.crtypelex((vartab[j]->leftside).typeval());
       rlabel.crvar(-(vartab[j]->rulenumber)-1,lle); }
     rlabel.crterm(fsymtabi-1);  // should be actcode
     rlabel.popt();
     {    // complex_label
       term ls, *rlab;         //..........ADDING
       int wherei;
       for (i=actlvtabi; i>0; ) { // rename strategy variables
	i--;
        for(j = 0; j<actvtabi; j++) if (actvtab[j] == actlvtab[i]) break; 
        if (lside1.cont_var(j) && rside1.cont_var(j))  {
	  lside1.ren_var(j,vartabi+2*i); rside1.ren_var(j,vartabi+2*i+1); } }
     ls.stinit(); 

     ls.pusht(lside1); 
     ls.pusht(rlabel);
     ls.crterm(app_code,2);
     ls.popt();
     //ls.write(stout); stout << "\n"; stout.flush();
     //ls.write(stout); stout << "===>"; rside1.write(stout); stout << "\n";
     NNEW(rlab,term);
     acttrrulelab = 
        trrules.addrule(rname,
			vartabi,ls,rside1,
			stratmoduli(actruletypeindex_l,actruletypeindex),
			RGLOP,       // AAA
				    NULL,
		          (acsymbolinleftside?ACMATCH:NORMMATCH),*rlab,NULL);
     //stout << "##1 ##"; acttrrulelab->dump(0); stout << "\n";
     for (i=actlvtabi; i>0; ) { // rename strategy variables
       i--;
       for(j = 0; j<actvtabi; j++) if (actvtab[j] == actlvtab[i]) break; 
       if (lside1.cont_var(vartabi+2*i) && rside1.cont_var(vartabi+2*i+1))  {
	  wherei = (vartab[j]->leftside).typeval();
          actwheretype.crtypelex(wherei);
	  ter.stinit(); 
          ter.crvar(vartabi+2*i,actwheretype); 
          ter.crvar(j,strlex);  // i
          ter.crterm(apply_code(wherei,wherei),2);
          ter.popt();
       	  inverse_apply_code(ter,&strx,&stry);
	  actwhstrategy = trrules.strategyindex_refs(
		attach_type_mod(EVALSTR,wherei,evalmoduli(strx,stry)));
				// EEEE evalmoduli(wherei)));
	  acttrrulelab->addwhere(reverse_wheres,vartabi+2*i+1,
			      (actwhstrategy==-1)?((strategy**)NULL):
			      trrules.getstrategyadr_refs(actwhstrategy),
			      ter,actwheretype); 
	  actwhstrategy = -1; // 3005
	}
     } //for (i=actlvtabi; i>0; )
    }
     if (actlvtabi) { actlvtabi = 0; break; /*goto fin2;*/ }
     else goto fin1;  // fujjjjj
  }  // if (acttrulename)
  fin1:
  actlvtabi = 0;
  if (actruletypeindex_l != actruletypeindex) {
    f->owarn("\nthis case is not implemented\n",NULL); failexit(); }
  acttrrule = 
    trrules.addrule(attach_type(acttrrulename,actruletypeindex_l),
		    vartabi,lside,rside,
//*********
// here, there is a proble, see tcstrat.eln (rules vs strategies
		    ((in_strategies)? 
		     stratmoduli(actruletypeindex_l,actruletypeindex):
		     impmoduli), 
//********

		    ((in_strategies)?RGLOP:rinfos),  // HACK a reflechir
			      act_rhs,
			(acsymbolinleftside?ACMATCH:NORMMATCH),rlabel,NULL);
	//stout << "##2 ##"; acttrrule->dump(0); stout << "\n";
     }
     /* fin2: */ break; 
    case 156: // dotname [.]
      acttrrulename = "DSTR";
      break;
  //------------ Marian's strategies
  case 158:				// name of strategy / begin of str.
        actstrategyname = l.alfsy();
	strstacki=1;
	strstack[0]= strstack[1]= NULL;
	NNEW(actstrategy ,strategy);
	break; 
  case 159: // NEW
	actstrategy->setprocmaxn(calledstr);
        actstrategy->setname(STRCALL,impmoduli);
        /* ---  !!! a continue !!! --- */
        [[fallthrough]];
  case 160:				// another element. strategy
	appactstrat();
	break;
  case 163:				// repeat / iterate (begin)
	if (strstacki+2 >= MAXINCLSTRAT) {
	   f->owarn("\n[semact] strstacki overflowed over MAXINCLSTRAT\n",NULL);
	   return(ERRORIM);
	}
	strstack[strstacki+1]=strstack[strstacki+2]=NULL;
	strstacki+=2;
	break;
  case 164:				// endrepeat
	actstrategy->setname(STRNAMEREPEAT,impmoduli); 
	actstrategy->setsubst(strstack[strstacki-1]);
	strstacki-=2;
	break;
  case 165:				// enditerate
	actstrategy->setname(STRNAMEITERATE,impmoduli);
	actstrategy->setsubst(strstack[strstacki-1]);
	strstacki-=2;
	break;
  case 166:                             // enditerateplus
        {
         // iterate+ A = A;iterate A  // la meme connerie pour repart+
         strategy *ss,*ww;
         ww = ss = strstack[strstacki-1];
         strstacki-=2;
         while(ss) {
            *actstrategy = *(ss->copy());   // copy
            appactstrat();
            ss = ss->nex();
         }
	actstrategy->setname(STRNAMEITERATE,impmoduli); 
	actstrategy->setsubst(ww);
        }
        break;
  case 167:                             // typed strategy
	actstratindex = trrules.strategyindex_defs(
          attach_type_mod(actstrategyname,stratindex,impmoduli),rinfos);
        return sem_action_167(f,actstratindex,stratindex);
/*
  case 168:  // type of strategy
        typecheck(f,actimp->s);
        stratindex = typet.addstr(actimp->s);
        CFRE(actimp->s); CFRE(actimp); 
        actimp=NULL;
        break;
*/
  case 169:				// module name
	if (strcmp(actargmodname,l.alfsy())) {
	  f->owarn("\n[semact] name of module doesn't correspond with the name of file\n",NULL);
	  return(ERRORIM);
	}
	break;
  case 170:                         // normalise
	actstrategy->setname(STRNAMENORMALISE,impmoduli); 
	actstrategy->setsubst(strstack[strstacki-1]);
	strstacki-=2;
	break;
  case 171:                         // repeat+
        {
         // tranformation: repeat+ A = A;repaet A
         strategy *ss,*ww;
         ww = ss = strstack[strstacki-1];
         strstacki-=2;
         while(ss) {
            *actstrategy = *(ss->copy());   // copy
            appactstrat();
            ss = ss->nex();
         }
	actstrategy->setname(STRNAMEREPEAT,impmoduli); 
	actstrategy->setsubst(ww);
        }
        break;
  case 174: // pour l'instant = dont care // first choose finish
  case 175:				  // dont care2 finish
       	actstrategy->setname(STRNAMEDONTCARE2,impmoduli);	
	actstrategy->setstl(strlstack[strlstacki-2]);
	strstacki-=2;
	strlstacki-=2;
	break;

 case 250:		      // [Huy: May  4 00] 	 
	actstrategy->setname(STRNORM_IN,impmoduli);
	actstrategy->setstl(strlstack[strlstacki-2]);
	strstacki-=2;
	strlstacki-=2;
	break;

 case 251:		      // [Huy: May  4 00] 	 
	actstrategy->setname(STRNORM_OUT,impmoduli);
	actstrategy->setstl(strlstack[strlstacki-2]);
	strstacki-=2;
	strlstacki-=2;
	break;
 case 252:		// [pem: Oct 26 00] endtall
	actstrategy->setname(STRNAMETALL,impmoduli); 
	actstrategy->setsubst(strstack[strstacki-1]);
	strstacki-=2;
	break;
 case 253:		// [pem: Oct 26 00] endtall
	actstrategy->setname(STRNAMETONE,impmoduli); 
	actstrategy->setsubst(strstack[strstacki-1]);
	strstacki-=2;
	break;
 case 254:		// [pem: Oct 26 00] endtall
	actstrategy->setname(STRNAMETSOME,impmoduli); 
	actstrategy->setsubst(strstack[strstacki-1]);
	strstacki-=2;
	break;

 case 255:		// [pem: Apr  8 02] endrewrite
	actstrategy->setname(STRNAMEREWRITE,impmoduli); 
	actstrategy->setsubst(strstack[strstacki-1]);
	strstacki-=2;
	break;
  case 176:				// dont care2 finish
	actstrategy->setname(STRNAMEDONTKNOW2,impmoduli);
	actstrategy->setstl(strlstack[strlstacki-2]);
	strstacki-=2;
	strlstacki-=2;
	break;
  case 162:				// dont care2 finish
	actstrategy->setname(STRNAMEDONTCARECON2,impmoduli);
	actstrategy->setstl(strlstack[strlstacki-2]);
	strstacki-=2;
	strlstacki-=2;
	break;
  case 168:				// dont care2 finish
	actstrategy->setname(STRNAMEONECON2,impmoduli);
	actstrategy->setstl(strlstack[strlstacki-2]);
	strstacki-=2;
	strlstacki-=2;
	break;
  case 181:				// dont care2 finish
	actstrategy->setname(STRNAMEDONTKNOWCON2,impmoduli);
	actstrategy->setstl(strlstack[strlstacki-2]);
	strstacki-=2;
	strlstacki-=2;
	break;
  case 179:				// one2 finish
	actstrategy->setname(STRNAMEONE2,impmoduli);
	actstrategy->setstl(strlstack[strlstacki-2]);
	strstacki-=2;
	strlstacki-=2;
	break;
  case 214:				// typed normalise finish
                                        // normalise(s1:S1, ..., sn:Sn)
	actstrategy->setname(STRNAMENORMALISE2,impmoduli);
	actstrategy->setstl_no_test(strlstack[strlstacki-2]);
	strstacki-=2;
	strlstacki-=2;
	break;
  case 177:				// first elem in strlist
  case 211:                             //               strtypedlist
	{ struct strlist *ss;
	  NNEW(ss , struct strlist);
	  ss->next = NULL;
	  ss->str = strstack[strstacki-1];
          if (n == 211)
            ss->str->settypeof(sntype.typeval());
	  strstack[strstacki-1]=strstack[strstacki]=NULL;
//	  NNEW(actstrategy ,strategy);
	  strlstack[strlstacki++] = ss;
	  strlstack[strlstacki++] = ss;
	}
	break;
  case 178:				// another elem in strlist
  case 212:                             //                 strtypedlist
	{ struct strlist *ss;
	  NNEW(ss , struct strlist);
	  ss->next = NULL;
	  ss->str = strstack[strstacki-1];
          if (n == 212)
            ss->str->settypeof(sntype.typeval());
	  strstack[strstacki-1]=strstack[strstacki]=NULL;
//	  NNEW(actstrategy ,strategy);
	  strlstack[strlstacki-1]->next = ss;
	  strlstack[strlstacki-1] = ss;
	}
	break;
  //------------ Marian's strategies
  case 180:  // stratcall
//        calledstr = stratrules->addstr(attach_type(l.alfsy(),stratindex));
        calledstr = stratrules->addstr(l.alfsy());
        break; 
  case 182:
        le.crcharlex(symbolcode);
        grstack[stacki].addsymbol(le);
        break;
  case 183:
        symbolcode = l.numval();
        break;
  case 184: { int err;                  // check variables in tr.rule 
					// && generate extensional rule
      // stout << "\nBEFORE FLOW #1 acttrrule\n"; acttrrule->dump(0);
      // if (acttrrulelab) { stout << "\nBEFORE FLOW #2 acttrrulelab\n"; acttrrulelab->dump(0); }

        if ((err = flowcheckrule(f,acttrrule)) != NORMCONT) return err;
        return(flowcheckrule(f,acttrrulelab));
       }
       break;
  case 185:				// identifier in import
        mkmodname1(actimp,l);
	break;
  case 186:
        mkmodname2(actimp);
	break;
  case 187:
        mkmodname3(actimp);
        break;
  case 172: // <X>
        mkmodname4(actimp);
        break;
  case 173: // <X->Y>
        mkmodname5(actimp);
        break;
  case 188:
	{ lbuffer bf;
	  lexem le;
	  if (actarglist==NULL) {
	     f->oerr("number of formal and actual parameters for module ",
	     actmodname[stacki]," doesn't correspond\n",NULL);
	     return(HANDERRORIM);
          }
          if (int_arg(actarglist)) {
            // stout << "passujem " << atoi(actarglist->s) << " ako int argument \n";
	    le.crnumlex( atoi(actarglist->s) );
	    bf.put(le);
          } else 
           {
          char ss[STRLEN];
            int  ii, jj;
            int chr;
            ii = 0; jj = 0;
            // stout << "passujem " << actarglist->s << " ako string argument \n";
            while((chr=actarglist->s[ii])) {
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
          actarglist=actarglist->next; 
	}
	break;
  case 189:
	if (actarglist!=NULL) {
	     f->oerr("number of actual and formal parameters for module ",NULL);
	     f->oerr(actmodname[stacki]," doesn't correspond\n",NULL);
	     return(HANDERRORIM);
	}
	break;
  case 190:  // strategy fail
        actstrategy->setname(STRFAIL,impmoduli);
        break;
  case 191:
	actstrategy->setname(STRNAMEDCPROCESSCALL,impmoduli);
	addstratprocgr();
	break;
  case 192:
	actstrategy->setname(STRNAMEDKPROCESSCALL,impmoduli);
	addstratprocgr();
	break;
  case 193:
	actstrategy->setprocname(l.alfsy());
	break;
  case 194:
	actstrategy->setprocmaxn(l.numval());
	break;
  case 195:
        actstrategy->setname(STRMETA,impmoduli);
        break;
  case 196:
        actstrategy->setname(STRIDENTITY,impmoduli);
        break;
  case 197:
        actstrategy->setproctype(typet.addstr(actimp->s));
        CFRE(actimp); 
        actimp = NULL;
	break;
  case 198:
	finfos2 = RLEFTASSOC;
	break;
  case 199:
	finfos2 = RRIGHTASSOC;
	break;
  case 201:                                 // end of imports
	fsymtab_remakealias(&(grstack[stacki]));
        break;
  case 202:                                 // default local import
        rinfos=RLOCOOP; importrinfos[stacki] = RLOCOOP;
	break;
  case 203:                                 // inline in strategy
	actstrategy->setname(STRINLINE,impmoduli);
        lbuf = f->getlastinlineAndinit();
        if (lbuf == NULL) {
	 f->oerr("\tno previous inline (i.e. /# ... #/) construction defined\n");
	  return(HANDERRORIM);
	}
        actstrategy->setinlinedbuffer(lbuf);
        break;
  case 204:                                  // definedas in op definition
        AALLOS(ddt,struct definedaslist);
	ddt->fname = l;
	ddt->next = definedasl;             
        definedasl = ddt;
	wasdefinedas=1;
        break;
  case 205:                                   // inline at the end of module
        AALLOS(inl,struct inlineslist);
        inl->text = f->getlastinlineAndinit();
        inl->next = inlinesl;
        inlinesl = inl;
        break;
  case 206:                                   // hard Alias
    if (actarity!=actprofis) {
      f->owarn("\n[semact] arity of hardAlias definition is not compatible with the profil\n",NULL);
      return(ERRORIM);
    }
    if ( ! (grstack[stacki].addhardalias(actalr,actleftside))) {
      f->owarn("\n[semact] hardAlias was not found\n",NULL);
      return(ERRORIM);
    }
    fsymtab[actalr->rulenumber].set_aliased();
    if (wasdefinedas) {
      fsymtab[actalr->rulenumber].set_definedas();
      definedasl->code = actalr->rulenumber;
    }
    rulepri=0; finfos=FSNOINFO; actarity=0; actprofis=0;
    finfos2 = RNOPRIOR; wasdefinedas = 0;
    break;
  case 207:                                    // hard alias beggining
    if (actarity!=actprofis) {
      f->owarn("\n[semact] arity of hardAlias is not compatible with the profil\n",NULL);
      return(ERRORIM);
    }
    actprofis=0;
    break;
  case 208:
    grstack[stacki].combine(); // (NEW)
    if (in_stratmoduli) {
      /*int i,j;*/
     if (inlinecodesi+1 >= MAXSPAIR) {
       f->owarn("\n[semact] SPAIR overflow\n",NULL); exit(ERRORIM); }
     inlinecodes[inlinecodesi].from = stratmoduli_fromi; 
     inlinecodes[inlinecodesi].to   = stratmoduli_toi;
//     inlinecodes[inlinecodesi].stratsort = add_strat_nont(stratmoduli_fromi,stratmoduli_toi);
     grstack[stacki].lookinlinecode(stratmoduli_fromi,stratmoduli_toi);
     inlinecodesi++; 
    }
    break;
  case 210:                                    // epsilon lexem
     le.crblanklex();
     grstack[stacki].addsymbol(le);
    break;
  case 213:
    typecheck(f,actimp->s);
    sntype.crtypelex(typet.addstr(actimp->s));
    CFRE(actimp->s); CFRE(actimp); 
    actimp=NULL;
    break;
  // local strategies

  case 240:
     {
     struct ilist *p;
     int ind; 
     *locstratend = NULL;

     for (locstratlen = 0,p = locstrat; p; p=p->next) 
       locstratlen++;

     NNEW(locstrattable, int[locstratlen]);
     //AALLOSS(locstrattable, locstratlen ,int);

     //stout << "local strategy\n";
     for (ind = 0; locstrat; locstrat=locstrat->next) {
     //  stout << locstrat->i << ":"; 
       locstrattable[ind++] = locstrat->i; }
     //stout << "\n";

     locstrat = NULL; locstratend = &locstrat; 
    break; }
  case 241:
  case 242:
    {
    NNEW(*locstratend ,struct ilist);
    (*locstratend)->i =  l.numval(); (*locstratend)->next = locstrat;
    locstratend = &((*locstratend)->next);
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









