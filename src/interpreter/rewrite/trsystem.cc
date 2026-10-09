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


#include "rtdatas.h"
#include "module.h"
#include "strategy.h"
#include <string.h>
#include "command.h"

int nomalisation_index = 0;

void freewherelist(struct wherelist *ll)
{ struct wherelist *lll;
  while (ll != NULL) {
    ll->whereterm.tdelete();
    lll = ll->next; CFRE(ll); ll = lll;
  }
}

void wherelisdump(ochstream &f,struct wherelist *wl,int deep)
{
  if (wl) {
    f << "\n"; wherelidump(f,wl,deep); 
    wherelisdump(f,wl->next,deep);
  }
}

void wherelidump(ochstream &f,struct wherelist *wl,int ods)
{
    odsek(f,ods+4);
    if (wl->leftvarn == IFVARN) f << "if ";
    else if (wl->leftvarn == TRYCHOICEEND) {
	struct WHEREbranches *brlist = wl->wherebranch_list;
	f << "try";
	while (brlist) {
	    f << "\n";
	    odsek(f,ods+8);
	    f << "choice";
	    wherelisdump(f,brlist->wherebranch,ods+8);
	    brlist=brlist->next;
	}
	f << "\n";
	odsek(f,ods+4);
	f << "end";
	return ;
    }
    else if (wl->leftvarn == WHEREPATTERN) {
      f << "where (" << typet.ide(wl->pattype) << ")";
      wl->pattern.write(f);
      f << " := (" << trrules.strategyname_refs(wl->strateg) << ") ";
    }
    else  {
      f << "where VAR(" << wl->leftvarn << ") := ("
          << trrules.strategyname_refs(wl->strateg) << ") ";
    }
    wl->whereterm.write(f);
}

/* ---------------           transrule      ---------------------- */

transrule::transrule(int varn,term left,term right, int module, int info,
		     struct tseq *rhss,
		     int acl,int nameindx,term rlabl,struct wherelist *wh)
{
  leftside = left;
  rightside = right;
  varnum = varn;
  modul = module;
  infos = info;
  rule_counter = 0;
  wheres = wh; // NULL before
  whichmatch = acl;
  rlabel = rlabl;
  rhs = rhss;
/* in Marian's original version the following line was missing BUG-3 */
  nameindex = nameindx;
/* end of BUG-3 */
  breaked = 0;
}

transrule::~transrule()
{
  sterr << "\n[transrule::~transrule]\t\t\t!!!\n";
}

extern int ith_subsort(int symb, int i);
transrule *transrule::crExtRule()
{ transrule *res;
  term leff,rright;
  term rlabel;
  lexem typ;

  typ.crtypelex(ith_subsort(leftside.head(),1)); // should be binary
  leff.pusht(leftside);
  leff.crExtVar(varnum,typ);
  leff.crterm(leftside.head());
  leff.popt();
  rright.pusht(rightside);
  rright.crExtVar(varnum,typ);
  rright.crterm(leftside.head());
  rright.popt();
  NNEW(res,
  transrule(varnum+1,leff,rright,modul,infos,rhs,whichmatch,nameindex,rlabel,NULL));
  res->wheres = wheres;
  return(res);
}

transrule *transrule::crStratAppRule()
{ transrule *res;
 term leff; /*,rright;*/
  term rlabel;
  leff.pusht(leftside);
  leff.crterm(COMPILMAIN);
  leff.popt();
  NNEW(res, transrule(varnum,leff,rightside,modul,infos,rhs,whichmatch,nameindex,rlabel,NULL));
  res->wheres = wheres;
  return(res);
}


void transrule::setvarn(int v)
{
  varnum = v;
}

void transrule::setnameindex(int v)
{
  nameindex = v;
}

void transrule::setwheres(struct wherelist *whs)
{
 wheres = whs;
}

void addtrywheretolist(int rev, struct WHEREbranches *whbrs, struct wherelist **p)
{ struct wherelist *wl;
 wl = new_wherelist();
 wl->leftvarn = TRYCHOICEEND;
  wl->wherebranch_list = whbrs;
  if (rev) {
    while ((*p) != NULL) p=&((*p)->next);
    (*p) = wl; wl->next=NULL; }
  else {
    wl->next = *p;
    *p = wl; }
}

//140698
struct wherelist *copy_wherelist(struct wherelist *wl)
{
struct wherelist *new_wl;
  if (wl) {
    new_wl = new_wherelist();
    *new_wl = *wl;
    if (wl->leftvarterm.isvalidterm()) {
      wl->leftvarterm.copyrec(new_wl->leftvarterm); }
    if (wl->whereterm.isvalidterm()) {
      wl->whereterm.copyrec(new_wl->whereterm); }
    if (wl->leftvarn == WHEREPATTERN && wl->pattern.isvalidterm()) {
      wl->pattern.copyrec(new_wl->pattern); }
    if (wl->wherebranch_list)
      new_wl->wherebranch_list = copy_WHEREbranches(wl->wherebranch_list);
    new_wl->next = copy_wherelist(wl->next);
    return(new_wl);
  }
  else return NULL;
}

//140698
struct WHEREbranches *copy_WHEREbranches(struct WHEREbranches * wb)
{
  struct WHEREbranches *new_wb;
  if (wb) {
    AALLOS(new_wb, struct WHEREbranches);
    *new_wb = *wb;   // to be sure
    new_wb->wherebranch = copy_wherelist(wb->wherebranch);
    new_wb->next = copy_WHEREbranches(wb->next);
    return(new_wb);
  } else return NULL;
}

struct wherelist *new_wherelist()
{
struct wherelist *p;
term noterm; noterm.stinit();
  AALLOS(p, struct wherelist);
  p->pattern = noterm; 
  p->whereterm = noterm;
  p->leftvarterm = noterm; 
  p->wherebranch_list = NULL;
  p->next = NULL;
  return(p);
}

void transrule::addtrywhere(int rev,struct WHEREbranches *whbrs)
{ 
  addtrywheretolist(rev,whbrs,&wheres);
}


void transrule::addpatternwhere(int rev,strategy **strat,term tl,term tr,int whtype)
{
  addpatternwheretolist(rev,strat,tl,tr,whtype,&wheres);
}

void addpatternwheretolist(int rev,strategy **strat,term tl,term tr,int whtype,
		    struct wherelist **p)
{ struct wherelist *wl;
  term noterm; noterm.stinit();
  wl = new_wherelist();
  wl->leftvarn = WHEREPATTERN;
  wl->strateg =  strat;
  wl->pattern = tl;
  wl->pattype = whtype;
  wl->whereterm = tr;
  if (rev) {
    //p = &wheres;
    while ((*p) != NULL) p=&((*p)->next);
    (*p) = wl; wl->next=NULL; }
  else {
    wl->next = *p;
    *p = wl; } 
}

void addwheretolist(int rev,int varn,strategy **strat,term t,lexem whtype, 
		    struct wherelist **p)
{ struct wherelist *wl;
  term noterm; noterm.stinit();
  wl = new_wherelist();
  if (varn != IFVARN) {
//    wl->leftvarterm.stinit();
    wl->leftvarterm.crvar(varn,whtype);
    wl->leftvarterm.popt();
  }
  wl->leftvarn = varn;
  wl->strateg =  strat;
  wl->whereterm = t;
  if (rev) {
    //p = &wheres;
    while ((*p) != NULL) p=&((*p)->next);
    (*p) = wl; wl->next=NULL; }
  else {
    wl->next = *p;
    *p = wl; }
}

void transrule::addwhere(int rev,int varn,strategy **strat,term t,lexem whtype)
{ 
  addwheretolist(rev,varn,strat,t,whtype,&wheres);
}

void transrule::getr(int&n,term&l,term&r,int &nami,struct wherelist *&wh,
		     struct tseq *&rhss,
		     int &whm,term &rlabl)
{
  n = varnum;
  l = leftside;
  r = rightside;
  wh = wheres;
  rhss = rhs;
  nami = nameindex;
  whm = whichmatch;
  rlabl = rlabel;
}

void transrule::getname()
{
  trrules.rulename(nameindex);
}

term *transrule::getleft()
{
  return (&leftside);
}

int transrule::getvarn()
{
  return(varnum);
}

int transrule::isOnBuiltins()
{ return(leftside.isofbuiltintype());
}


void transrule::consistency()
{ /*struct wherelist *wl;*/
  leftside.consistency();
  rightside.consistency();
}

/*
lexem transrule::ruletype()
{
  return leftside.termtype();
}
*/

int transrule::ruletypeval() {
  return leftside.termtype().typeval();
}

void transrule::dump(int ods)
{ struct wherelist *wl;
  //odsek(dumpout,ods);
 // dumpout // <<"\tnameindex = "<<nameindex
	  //   <<"module = " << import.ide(getmodule())
	  //   <<"\ttype = " <<typet.ide(this->ruletype().typeval()) 
	  //   <<"\tfsym = " <<  (this->getleft())->head()
	  //   <<"\tvars = "<<varnum;

//  if (nameindex != -1)
//    dumpout << "[" << trrules.rulename(nameindex) << "]\n";

//  if (whichmatch == ACMATCH) dumpout << "\tmatch = "<<whichmatch;  dumpout <<"\n";
  if (rlabel.isvalidterm()) {
     dumpout << "[";
     rlabel.write(dumpout); 
     dumpout << "]\t";
   }
  odsek(dumpout,ods);
 leftside.write(dumpout);   // leftside.dump();
  dumpout << "\t => ";
  if (rhs) {
    dump_tseq(dumpout,/*ods*/ 10,rhs); dumpout << "\n"; }
  else {
    rightside.write(dumpout);  //rightside.dump();
    dumpout << "\n";
    wl = wheres;
    wherelisdump(dumpout,wl,ods);
  }
  dumpout.flush();
}

/* ---------------           strategy         -------------------  */

int compatible(int typ1, int typ2)
{ int typ = 0; /* initialised to avoid warning */
//  stout << "[[" << typ1 << "." << typ2 << "]] = ";
  if (typ1 == -1) typ = typ2;
  else if (typ2 == -1) typ = typ1;
  else if (typ1 == typ2) typ = typ1;
  else {  
    sterr << "\n[compatible] strategy subtypes are not compatible " 
	  // << typ1 << "#" << typ2 
	  << "\n";
    failexit(); }
  return typ;
}

strategy::strategy()
{
  typeofstrategy = -1; // unknown
  next = NULL;
  u.cr.nm = NULL;
  breaked = 0;
}

void strategy::appendrname(int nam)
{ struct namelist **p,*np;
  p = &(u.cr.nm);
  while (*p != NULL) p = &((*p)->next);
  NNEW(np ,struct namelist);
  (*p) = np;
  np -> next = NULL;
  np -> strname = nam;
}

void strategy::setnext(strategy *n)
{ 
  next = n;
  if (n != NULL)
    typeofstrategy = 
      compatible(typeofstrategy,n->typeofstrategy); // ..typing
}

void strategy::setsubst(strategy *s)
{ 
  u.substrategy = s;
  typeofstrategy = compatible(typeofstrategy,s->typeofstrategy); // ..typing
}

void strategy::setstl(struct strlist *s)
{struct strlist *ss;
  setstl_no_test(s);
  for(ss = s; ss; ss=ss->next) 
    typeofstrategy = compatible(typeofstrategy,ss->str->typeofstrategy);
}

void strategy::setstl_no_test(struct strlist *s)
{/*struct strlist *ss;*/
  u.stl = s;
}

void strategy::setnamelist(struct namelist *nm)
{
  u.cr.nm = nm;
}

int  strategy::typeofstr()
{
  return typeofstrategy;
}

void  strategy::settypeof(int typ)
{
  typeofstrategy = typ;
}

void strategy::setname(int n, int modu)
{ strname = n; strmod = modu;
}

void strategy::setprocname(const char *n)
{ u.procc.pname = n;
}

void strategy::setprocmaxn(int n)
{ u.procc.maxn= n;
}

void strategy::setprocgr(grammar *g)
{ u.procc.locgr = g;
}

void strategy::setproctype(int n)
{ 
  typeofstrategy = n; // ..typing
  u.procc.restype = n;
}

void strategy::setinlinedbuffer(lbuffer *ll)
{ u.inlinedbuffer = ll;
}

lbuffer * strategy::getinlinedbuffer()
{ return(u.inlinedbuffer);
}


//-------------------------------------------------
// C'EST QUOI CE BORDEL ???????????????????? PEM
void DeletE(struct strlist *sl)
{ strlist *p;
  if (sl) {
      sl->str->Delete();
      p = sl; sl=sl->next;
      CFRE(p);
  } else
    return ;
}

void strategy::Delete()
{ 
  switch (strname) {
    case STRMETA:
    case STRIDENTITY:
    case STRFAIL:
  case STRNAMEDONTCARE: case STRNAMEDONTKNOW: case STRNAMEONE:  case STRNORM_IN: case STRNORM_OUT:
    case STRCALL:
      if (next)
	next->Delete();
      break;
    case STRNAMEREPEAT: case STRNAMEITERATE :
     u.substrategy->Delete();
     break;
  case STRNAMEDONTCARE2: case STRNAMEDONTKNOW2: case STRNAMEONE2: 
  case STRNAMEDONTCARECON2: case STRNAMEDONTKNOWCON2: case STRNAMEONECON2:
      DeletE(this->u.stl);
      break;
    default : dumpout << "\n[copy] unknown strategy in strdelete internal error\n";
  }
  DELETE1(this); // allocated with new (term2strategy, meta.cc)
}

struct strlist *copies(struct strlist *sl)
{ strlist *p;
  if (sl) {
    AALLOS(p, struct strlist);
    p->str = sl->str->copy();
    p->next = copies(sl->next);   // fucking recursion, but who cares ...
    return p;
  }
  else
    return NULL;
}

strategy *strategy::copy()
{ 
  strategy *p = NULL; /* initialised to avoid warning */
  NNEW(p,strategy);

  switch (strname) {
    case STRMETA:
    case STRIDENTITY:
    case STRFAIL:
  case STRNAMEDONTCARE: case STRNAMEDONTKNOW: case STRNAMEONE: case STRNORM_IN: case STRNORM_OUT:
    case STRCALL:
      *p = *this; 
      if (next)
	p->next = next->copy();   // NEW !!!
      return p;
    case STRNAMEREPEAT: case STRNAMEITERATE :
      *p = *this;
      p->u.substrategy = this->u.substrategy->copy();
      return p;
  case STRNAMEDONTCARE2: case STRNAMEDONTKNOW2: case STRNAMEONE2: 
  case STRNAMEDONTCARECON2: case STRNAMEDONTKNOWCON2: case STRNAMEONECON2:
      *p = *this;
      p->u.stl = copies(this->u.stl);
      return p;
    default : dumpout << "\n[copy] unknown strategy in strcopy internal error\n";
  }
  return p; /* to avoid warning */
}
int strategy::conform(int warn)
{ int sort = -1; // unknown
  int res, rest;	
  // stout << "\n-------------------\n";
  // dump();
  // stout << "\n...................\n";
  res = conform2(0,warn,typeofstr());
  if (res) {
    rest = infertypes(warn,sort);
    /*
    if (sort != typeofstr()) {
      sterr << "\ndefined and infered types of a strategy are not equal\n";
      sterr << sort << " != " << typeofstr() << "\n";
      dump();
    }
    */
    //stout << "\n^^^^^^^^^^^^^^^^^^^\n";
    if (!rest) {
      sterr << "\nSORT of strategy is unknown\n"; 
      this->dump(); } }
  return res;
}

// n = -1 dc, +1 dk, 0 unknown
int strategy::conform2(int n,int warn, int typ)
{ strategy *st;
  int r = 1;
  st = this;
  while (st !=NULL) {
    r &= st->simpleconform(n,warn, typ); 
    st = st->next; }
  return r;
}

int strategy::is_call()
{
  if (strname == STRCALL && next == NULL) 
    return u.procc.maxn;
  else
    return 0;
}

int match(int a, int b, int &r)
{
  // stout << "match " << a << "," << b << ";" << r << "\n";
  if (a == -1 && b == -1) { r = -1; return 1; }
  if (a == -1) { r = b; return 1; }
  if (b == -1) { r = a; return 1; }
  if (a == b) { r = a; return 1; }
  if (!batch) {sterr << "\n[warning] uncompatible sorts in a strategy\n";}
  return 0;
}

int strategy::infertypes(int warn,int &sort)
{strategy *p = this;
 int r = 1;
 for(p=this; p; p=p->next) 
   r &= p->infertype(warn,sort);
 return r;
}

int strategy::infertype(int warn,int &sort)
{ struct namelist *np; /*, **npp;*/
 struct strlist *sl,  *aux_sl; /* *sl1,*/
 /*  char   *ss;*/
  struct transrulelist *trlist;
  /*transrule *rule;*/
  /* r initialised to avoid warning */
  int    r =0, aa,  typ, refs; /* res, */
  aa = sort; // S2: match() leaves aa unset on incompatible sorts; sort is then
             // kept, as in the later iterations of the rule loop below
   // stout << "INFER >> " << sort << "\n";
   //this->dump();
   //stout << "-----------------------\n";
  switch (strname) {
      case STRIDENTITY: case STRFAIL:
        r= 1; goto ret;
      case STRNAMEREPEAT: case STRNAMEITERATE :
        r=u.substrategy->infertypes(warn,sort); goto ret;
      case STRNAMETALL: // [pem: Oct 26 00]
      case STRNAMETONE:
      case STRNAMETSOME:
      case STRNAMEREWRITE:
        r=u.substrategy->infertypes(warn,sort); goto ret;
      case STRNAMENORMALISE:
        r = u.substrategy->infertypes(warn,sort);
        if (r != -1) {
          if (alg_normalisation == 1) {
            setname(STRNAMEREPEAT,getmodule());
            AALLOS(aux_sl,struct strlist); aux_sl->next = NULL;
            aux_sl->str = u.substrategy;
            aux_sl->str->settypeof(sort);
            setsubst(gen_normalisation1(
              nomalisation_index++,getmodule(),sort/*!typeofstr()!*/,aux_sl));}
          else {
            setname(STRNAMEDONTCARE,getmodule());
	  setnamelist(gen_normalisation2(nomalisation_index++,sort,u.substrategy));}
	//dump(); 
      }
      goto ret;
   case STRNAMENORMALISE2:
     r = 1;    // not finished
     for(sl = u.stl; sl!=NULL; sl=sl->next) {
       int snsort = -1;
       r &= sl->str->infertypes(warn,snsort);
       // stout << "SNSORTS " << r << "=" <<
	//  snsort << "," << sl->str->typeofstr() << "\n";
     }
     if (r != -1) {
	if (alg_normalisation == 1) {
	  setname(STRNAMEREPEAT,getmodule());
	  setsubst(gen_normalisation1(nomalisation_index++,getmodule(),typeofstr(),
				      u.stl));}
	else {
	  sterr << "NORMALISE not implemented\n"; failexit(); }
     }
     goto ret;
  case STRNAMEDONTCARE2: case STRNAMEDONTKNOW2: case STRNAMEONE2: 
  case STRNAMEDONTCARECON2: case STRNAMEDONTKNOWCON2: case STRNAMEONECON2:
     
      r = 1;
      for(sl = u.stl; sl!=NULL; sl=sl->next ) 
	r &= sl->str->infertypes(warn,sort);
      goto ret;
  case STRNAMEDONTCARE: case STRNAMEDONTKNOW: case STRNAMEONE: case STRNORM_IN: case STRNORM_OUT: // [Huy: May  4 00] 
   
      r = 1;
      for(np = u.cr.nm; np!=NULL; np=np->next) {
	for(trlist = trrules.getrules(np->strname,getmodule());
	    trlist; trlist=trlist->next) {
	  if (trlist && trlist->rule) 
	    typ = trlist->rule->ruletypeval();
	  else
	    typ = -1;
	  r &= match(sort,typ,aa);
	  sort = aa;
	}
      }
      goto ret;
    case STRCALL:
      refs = trrules.strategy_refs_into_defs(u.procc.maxn);
      if (refs >= 0) 
	typ = trrules.typeofstrategy_defs(refs);
      else
	typ = -1;
      //stout << typ << "\n";
      r = match(sort,typ,aa);
      sort = aa;
      goto ret;
    case STRMETA: 
    case STRNAMEDCPROCESSCALL:
    case STRNAMEDKPROCESSCALL:
      r=1; goto ret;  // rien a faire ?
    default : dumpout << "\n[infer-type]unknown strategy, internal error\n";
  }
ret:
  // stout << "INFER << " << sort << " r = " << r <<"\n";
  // dump();
  // stout << "-------\n";
  return r;
}

void RSambiguity(int warn, char *name, char *type, char *modu)
{
  if (warn) { 
    sterr << "\nthere is an rule/strategy ambiguity because of " << name 
	  << " for " << type << " in module " << modu << "\n";
    failexit(); }
}

void Sundefined(int warn, char *name, char *type, char *modu)
{
  if (warn) { 
    sterr << "\nthere is an unresolved strategy reference of " << name 
	  << " for " << type << " in module " << modu << "\n";
    failexit();
  }
}

void RSundefined(int warn, char *name, char *type, char *modu)
{
  if (warn) {
    sterr << "\nundefined rule or strategy " << name << " for " << type 
	  << " in module " << modu << "\n"; 
    failexit(); }
}

void SSambiguity(int warn, char *name, char *type, char *modu)
{
  if (warn) { 
    sterr << "\nthere is an strategy/strategy ambiguity because of a reference " << name 
	  << " for " << type << " in module " << modu << "\n"; 
    failexit(); } 
}

int strategy::simpleconform(int n, int warn, int typ)
{ struct namelist *np, **npp;
  struct strlist *sl, *sl1;
  char   *ss; /* *name; *///, *type;
  int    sindx,rindx,r;
  struct transrulelist *trlist;
  r = 1;
  // stout << "simpleconform entry " << typ;this->dump();stout << "\n";
    switch (strname) {
        case STRNAMEREPEAT:
        case STRNAMEITERATE :
        case STRNAMENORMALISE:
        case STRNAMETALL: //[pem: Oct 26 00]
        case STRNAMETONE:
        case STRNAMETSOME:
        case STRNAMEREWRITE:
	return u.substrategy->conform2(n,warn,typ);
    case STRNAMEDONTCARE2: case STRNAMEDONTKNOW2: case STRNAMEONE2: case STRNORM_IN: case STRNORM_OUT: // [Huy: May  4 00] 
    case STRNAMEDONTCARECON2: case STRNAMEDONTKNOWCON2: case STRNAMEONECON2:
    case STRNAMENORMALISE2:
        for(sl = u.stl; sl!=NULL; sl=sl->next ) {
	  if (sl->str->strname != STRCALL) goto nothing_to_do;
	  ss = stratrules->ide(sl->str->u.procc.maxn);
	  //sindx = trrules.strategymember_refs(ss);
	  //sindx = trrules.strat_refs_into_defs(attach_mod(ss,getmodule()));
	  sindx = trrules.strat_refs_into_defs(
			   attach_type_mod(ss,typ,getmodule()));
	  if (sindx == -2) { // ambig
	    //detach_name_type(ss,&name,&type); 
	    //SSambiguity(warn,name,type,import.ide(getmodule())); }
	    SSambiguity(warn,ss,typet.ide(typ),import.ide(getmodule())); }
	  rindx = trrules.trrulemember(attach_type(ss,typ));
	  if (rindx !=-1) trlist = trrules.getrules(rindx,getmodule());
	  else trlist = NULL;
	  if (trlist && sindx !=-1) { 
	    //detach_name_type(ss,&name,&type);
	    //RSambiguity(warn,name,type,import.ide(getmodule())); }
	    RSambiguity(warn,ss,typet.ide(typ),import.ide(getmodule())); }
	  else if (trlist == NULL) {
	    // should be a strategy
////////???????	    trrules.strategy_refs_into_defs(sindx); // NEW
	    goto nothing_to_do; }
	}
	// dc/dk over list of rules
	npp = &np;
	for(sl = u.stl; sl!=NULL;  ) {
	  ss = stratrules->ide(sl->str->u.procc.maxn);
	  //stout << "RULE! = " << ss;
	  //rindx = trrules.trruleindex(ss);
	  rindx = trrules.trruleindex(attach_type(ss,typ));
	  NNEW(*npp ,struct namelist);
	  (*npp)->strname = rindx;
	  npp = &((*npp)->next);
	  sl1 = sl;
	  sl=sl->next;
	  CFRE(sl1);
	}
	*npp = NULL;
      	if (strname == STRNAMEDONTCARE2) strname = STRNAMEDONTCARE;
	else if (strname == STRNAMEDONTKNOW2) strname = STRNAMEDONTKNOW;
	else if (strname == STRNAMEDONTCARECON2) strname = STRNAMEDONTCARECON;
	else if (strname == STRNAMEDONTKNOWCON2) strname = STRNAMEDONTKNOWCON;
	else if (strname == STRNAMEONECON2) strname = STRNAMEONECON;
	else if (strname == STRNAMEONE2) strname = STRNAMEONE;
	else if (strname == STRNORM_IN) strname = STRNORM_IN;    // [Huy: May  4 00] 
	else if (strname == STRNORM_OUT) strname = STRNORM_OUT;    // [Huy: May  4 00] 
	else strname = STRNAMEDONTKNOW;
        u.cr.nm = np;
	return 1;
	/////////////////////
	nothing_to_do:
	for(sl = u.stl; sl!=NULL; sl=sl->next ) {
	  r &= sl->str->conform2
	    ((strname == STRNAMEDONTKNOW2)?1:-1,
	     warn,
	     (strname==STRNAMENORMALISE2)?sl->str->typeofstr():typ); }
	return r;
    case STRCALL:
	ss = stratrules->ide(u.procc.maxn);
	//sindx = trrules.strategymember_refs(ss);
	sindx = trrules.strat_refs_into_defs(
	       attach_type_mod(ss,typ,getmodule()));
	if (sindx == -2) { // ambig
	  // detach_name_type(ss,&name,&type); 
	  // RSambiguity(warn,name,type,import.ide(getmodule())); }
	  RSambiguity(warn,ss,typet.ide(typ),import.ide(getmodule())); }
	// rindx = trrules.trrulemember(ss);
	rindx = trrules.trrulemember(attach_type(ss,typ));
	if (rindx !=-1) trlist = trrules.getrules(rindx,getmodule());
	else trlist = NULL;
	if (trlist && sindx != -1) { 
	    //detach_name_type(ss,&name,&type);
	    //RSambiguity(warn,name,type,import.ide(getmodule())); }
	    RSambiguity(warn,ss,typet.ide(typ),import.ide(getmodule())); }
	else if (trlist) {
	  //stout << "RULE = " << ss;
	  //rindx = trrules.trruleindex(ss);
	  rindx = trrules.trruleindex(attach_type(ss,typ));
	  strname = STRNAMEDONTKNOW;

	  NNEW(np ,struct namelist);
	  np->next = NULL; np->strname = rindx;
	  u.cr.nm = np;
	} else if (sindx != -1) {
	  //stout << "STRATEGY = " << ss;
	  sindx = trrules.strategyindex_refs(
			       attach_type_mod(ss,typ,getmodule()));
	  u.procc.maxn = sindx;
	} else {
	  //detach_name_type(ss,&name,&type);
	  //RSundefined(warn,name,type,import.ide(getmodule()));
	  RSundefined(warn,ss,typet.ide(typ),import.ide(getmodule()));
	}
	return 1;
    }
  return 1;
}

//-------------------------
void strategy::dump()
{
//  dumpout << "[strategy::dump] begin\n";
  dump2(0);
//  dumpout << "[strategy::dump] end\n";
}

void strategy::dump2(int n)
{
  strategy *st;
  st = this;
  while (st !=NULL) {
    dumpout << "\n";
    odsek(dumpout,n);
    st->simpledump(n);
    st = st->next;
    if (st) dumpout << ";"; }
}

void strategy::simpledump(int n)
{ struct namelist *np;
 struct strlist *sl;
 switch (strname) {
     case STRNAMEDONTCARE :
     case STRNAMEDONTKNOW :
     case STRNAMEONE:
     case STRNORM_IN:
     case STRNORM_OUT: 
      if (strname == STRNORM_IN) // [Huy: May  4 00] 
         dumpout << "norm_in(";
       else
       if (strname == STRNORM_OUT) // [Huy: May  4 00] 
         dumpout << "norm_out(";
       else
         if (strname == STRNAMEONE) 
	  dumpout << "dc one(";
        else
	  dumpout << (strname==STRNAMEDONTKNOW ? "dk(" : "dc(");
	np = u.cr.nm;
	while (np!=NULL) {
	  dumpout << trrules.rulename(np->strname) << " ";
	  np = np->next; }
	dumpout << ")";
	break;
    case STRNAMEREPEAT: case STRNAMEITERATE :
	dumpout << (strname==STRNAMEREPEAT ? "repeat* (" : "iterate (");
	u.substrategy->dump2(n+2);
	dumpout << "\n"; odsek(dumpout,n);
	dumpout << ")";
	break;
    case STRNAMEDONTCARE2: case STRNAMEDONTKNOW2: case STRNAMEONE2:
    case STRNAMENORMALISE2: 
    case STRNAMEDONTCARECON2: case STRNAMEDONTKNOWCON2: case STRNAMEONECON2:
        if (strname == STRNAMEONE2)
	  dumpout << "DC ONE(";
        else if (strname == STRNAMENORMALISE2)
	  dumpout << "NORMALISE(";
        else if (strname == STRNAMEDONTKNOWCON2)
	  dumpout << "DKCON(";
        else if (strname == STRNAMEDONTCARECON2)
	  dumpout << "DCCON(";
        else if (strname == STRNAMEONECON2)
	  dumpout << "ONECON(";
        else
	  dumpout << (strname==STRNAMEDONTKNOW2 ? "DK(" : "DC(");
	sl = u.stl;
	while (sl!=NULL) {
	  sl->str->dump2(n+2);
	  if (sl->next!=NULL) {
	    dumpout<< " ||"; }
	  sl=sl->next; }
	odsek(dumpout,n); dumpout << ")";
	break;
    case STRMETA:
        dumpout << "META";	
        break;
    case STRFAIL:
        dumpout << "fail"; break;
    case STRIDENTITY:
        dumpout << "id"; break;
    case STRCALL:
	dumpout << trrules.strategyname_refs(u.procc.maxn);
        break;
    case STRNAMEDCPROCESSCALL:
      dumpout << "dccall(" 
	      << u.procc.pname << "," << u.procc.maxn << ","
	      << typet.ide(u.procc.restype) << ")";
      break;
    case STRNAMEDKPROCESSCALL:
      dumpout << "dkcall(" 
	      << u.procc.pname << "," << u.procc.maxn << ","
	      << typet.ide(u.procc.restype) << ")";
      break;
     case STRNAMETALL: // [pem: Oct 26 00]
       dumpout << "tall(";
       u.substrategy->dump2(n+2);
       dumpout << "\n"; odsek(dumpout,n);
       dumpout << ")";
       break;
     case STRNAMETONE: // [pem: Oct 26 00]
       dumpout << "tone(";
       u.substrategy->dump2(n+2);
       dumpout << "\n"; odsek(dumpout,n);
       dumpout << ")";
       break;
     case STRNAMETSOME: // [pem: Oct 26 00]
       dumpout << "tsome(";
       u.substrategy->dump2(n+2);
       dumpout << "\n"; odsek(dumpout,n);
       dumpout << ")";
       break;
     case STRNAMEREWRITE: // [pem: Apr  8 02]
       dumpout << "rewrite(";
       u.substrategy->dump2(n+2);
       dumpout << "\n"; odsek(dumpout,n);
       dumpout << ")";
       break;

    default : dumpout << "\n[simple-dump] unknown strategy, internal error\n";
    }
}


/* ----------------          trsystem         -------------------- */


trsystem::trsystem()
{ int i;
  NNEW(rulenames ,stringtab(MAXNOFTRN));
  NNEW(strategynames_defs ,stringtab(MAXNOFSTRAT));
  NNEW(strategynames_refs ,stringtab(MAXNOFSTRAT));
  for(i=0; i<MAXNOFTRN; i++) { rules[i]=NULL; lastrule[i] = &(rules[i]);}
  for(i=0; i<MAXNFSYM; i++) { nnrules[i]=NULL; lastnnrule[i] = &(nnrules[i]);}
  for(i=0; i<MAXNOFSTRAT; i++) { 
    strategies_defs[i]=NULL; 
    strategies_refs[i]=NULL;
    strategies_cross[i]=-1;
    strategyinfos_defs[i]=0; }
}


transrule *trsystem::addrule(char *name, int lefthead, transrule *rr)
{ int i;
  struct transrulelist *tl;

  if (name == NULL && ((rr->getinfos()) & RLOCOOP)) {
    if (alpha_syntax) {
      if (!batch && !quiet) {
	sterr << "\n[warning] local unnamed rule in module " ;
	sterr << import.ide(rr->getmodule()) << "\n";
	rr->dump(0); sterr << "\n"; } }
    else {
      if (!batch) {
      sterr << "\n[fatal] local unnamed rule in module ";
      sterr << import.ide(rr->getmodule()) << "\n";
      rr->dump(0); sterr << "\n"; }
      failexit(); }
  }

  NNEW(tl ,struct transrulelist);
  tl->rule = rr;
  tl->next = NULL;
  if (name!=NULL) {
     i = rulenames->addstr(name);
     rr->setnameindex(i);

     /*
stout << "\n ADD RULE index = " 
      << i
      << " name = "
      << name
      << "\n";
     */

     *(lastrule[i]) = tl;
     lastrule[i] = &(tl->next);
  } else {
     rr->setnameindex(-1);
     *(lastnnrule[lefthead]) = tl;
     lastnnrule[lefthead] = &(tl->next);
  }
  return(rr);
}


transrule *trsystem::addrule(char *name,int varn,term left,term right,
			     int modul, int infos,
			     struct tseq *rhs,
			     int acl,term rlabel,struct wherelist *wh)
{ transrule *rr;
  int i;
  /*
  if (name != NULL && strlen(name) > 0)
    stout << "ADDING rule " << name << " from module " 
	  << import.ide(modul) << " with flags " << infos << "\n";
	  */
  NNEW(rr,transrule(varn,left,right,modul, infos,rhs,acl,0,rlabel,wh));
  if (name!=NULL) i = rulenames->addstr(name); else i = -1;
  rr->setnameindex(i);
  addrule(name,left.head(),rr);
  return(rr);
}

/*
 * Ajoute une regle avec variable d'extension dans le cas AC
 */
void trsystem::addExtRule(transrule *rrule)
{ int n,nameind;
  term l,r;
  term rlabel;
  struct wherelist *wh;
  struct tseq *rhs;
  int whichmatch;
  rrule->getr(n,l,r,nameind,wh,
	      rhs,
	      whichmatch,rlabel);
  if (nameind==-1 && fsymtab[l.head()].infos()==FSASSOCCOM) 
	addrule(NULL,l.head(),rrule->crExtRule());
}

int trsystem::trruleindex(char *name)
{
  return(rulenames->addstr(name));
}

int trsystem::trrulemember(char *name)
{
  return(rulenames->index(name));
}

void trsystem::assign_one_ref(int warn, int ref)
{
char *ss, *nname, *type, *modu;
int rrr;
    rrr = strategy_refs_into_defs(ref);
    if (rrr == -2) {
      ss = strategynames_refs->ide(ref);
      detach_name_type_module(ss,&nname,&type,&modu); 
      RSambiguity(warn,nname,type,modu); } 
    else if (rrr == -1) {
      ss = strategynames_refs->ide(ref);
      detach_name_type_module(ss,&nname,&type,&modu); 
      Sundefined(warn,nname,type,modu);
    }
}

void trsystem::assign_all_refs(int warn)
{
  /*char *ss, *nname, *type, *modu;*/
  /*int rrr;*/
  for(strategynames_refs->forinit(); strategynames_refs->forcond(); 
      strategynames_refs->fornext())
    assign_one_ref(warn,strategynames_refs->forindex());
}

strategy **trsystem::getstrategyadr_refs(int stratindex)
{ 
  return(&(strategies_refs[stratindex])); 
}

strategy *trsystem::getstrategy_refs(int stratindex)
{
  return(*getstrategyadr_refs(stratindex));
}

strategy **trsystem::getstrategyadr_defs(int stratindex)
{ 
  return(&(strategies_defs[stratindex])); 
}

strategy *trsystem::getstrategy_defs(int stratindex)
{
  return(*getstrategyadr_defs(stratindex));
}

int trsystem::typeofstrategy_defs(int stratindex)
{
  return(typeofstrategies_defs[stratindex]);    // indexed by _defs
}

void trsystem::settypeofstrategy_defs(int stratindex, int typ)
{
  typeofstrategies_defs[stratindex] = typ;
}
//---
int trsystem::strategymember_defs(char *name)
{ return(strategynames_defs->index(name)); }

int trsystem::strategyindex_defs(char *name, int infos)
{
int r = strategynames_defs->addstr(name); 
  strategyinfos_defs[r] = infos;
//  if(!batch)stout << "Definition " << name << " = " << r << ((infos == RGLOP)?" GLOBAL":" LOCAL") << "\n";
  return r;
}

const char * trsystem::strategyname_defs(int n)
{ return(strategynames_defs->ide(n)); }

const char * trsystem::strategyname_defs(strategy **s)
{ int n;
  if (s==NULL) return("");
  n = s-strategies_defs;
  if (strategynames_defs->ide(n) == NULL) return("NULL");
  return(strategynames_defs->ide(n));
}
//---

int trsystem::strat_refs_into_defs(char *name)
{
char *p, ch, ch2, *ss, *modul1, *modul2, *p2;
int m1, m2, count = 0;
int defs = -1;
//  if(!batch) stout << "REFS_INTO_DEFS :" << name << ":\n";
  p = strstr(name,MODULE_SEPARATOR);
  if (!p) { sterr << "\n[refs->defs] internal error\n"; sterr << name << "\n"; failexit(); }
  else {
    ch = *p; *p = 0; modul1 = p+strlen(MODULE_SEPARATOR);
    for(strategynames_defs->forinit(); strategynames_defs->forcond() && defs >= -1; 
	strategynames_defs->fornext()) {
      ss = strategynames_defs->foractval();
      p2 = strstr(ss,MODULE_SEPARATOR);
      if (!p2) { sterr << "\n[refs-> defs] internal error\n"; sterr << ss << "\n"; failexit(); }
      ch2 = *p2; *p2 = 0; modul2 = p2+strlen(MODULE_SEPARATOR);
      if (!strcmp(name,ss)) {
	m1 = import.addstr(modul1); m2 = import.addstr(modul2);
	if (visible(m1,m2,strategyinfos_defs[strategynames_defs->forindex()])) {
	  if (count > 0) defs = -2;
	  else { count ++; defs = strategynames_defs->forindex(); }}}
      *p2 = ch2; }
    *p = ch;
//    if (!batch) stout << "REF_INTO_DEF " << name << " = " << defs << "\n";
    return defs;
  }
  return 0; /* to avoid warning */
}

void trsystem::set_strategies_cross(int strindex_refs, int strindex,strategy *str)
{
  strategies_cross[strindex_refs] = strindex;
  strategies_refs[strindex_refs] = str;
}

int trsystem::strategy_refs_into_defs(int iname)
{
char *name;
int defs = -1;
  if (strategies_cross[iname] != -1) // already assigned
    return strategies_cross[iname];
  name = strategynames_refs->ide(iname);
  defs = strat_refs_into_defs(name);
//  if (!batch) stout << "REFS_INTO_DEFS " << name << " = " << defs << "\n";
  if (defs >= 0) {
    strategies_cross[iname] = defs;
    strategies_refs[iname] = strategies_defs[defs]; }
  return defs;
}

int trsystem::strategymember_refs(char *name)
{ return(strategynames_refs->index(name)); }

int trsystem::strategyindex_refs(const char *name)
{ 
int r = strategynames_refs->addstr(name); 
  //stout << "Reference  " << name << " = " << r << "\n";
  return r;
}

const char * trsystem::strategyname_refs(int n)
{ return(strategynames_refs->ide(n)); }

int trsystem::strategyremove_refs(int n)
{ 
  //stout << "Remove  " << strategynames_refs->ide(n) << "\n";
  strategynames_refs->removestr(n);
  return 0;
}

const char * trsystem::strategyname_refs(strategy **s)
{ int n;
  if (s==NULL) return("");
  n = s-strategies_refs;
  if (strategynames_refs->ide(n) == NULL) return("NULL");
  return(strategynames_refs->ide(n));
}
//---
char * trsystem::rulename(int n)
{ return(rulenames->ide(n));
}

void trsystem::remove_rulename(int n)
{
  rulenames->removestr(n);
}

int trsystem::setstrategy_defs(int n,strategy *st)
{
  if (strategies_defs[n] != NULL) return(1);   // TO DO ST WITH _refs
  strategies_defs[n] = st;
  return(0);
}

struct transrulelist *trcash(int ruleindex, int modul) 
{
char foo[STRLEN];
int i;
if (all_modules_loaded) {
  snprintf(foo,sizeof(foo),"%d,%d",ruleindex,modul);
  i = rulecashstrings.member(foo);
  //stout << "GETCASH rules from module " << ruleindex << "." <<import.ide(modul) << i << "\n";
  if (i>=0) {
    i = rulecashstrings.addstr(foo);
    return rulecashtable[i]; }
  else
    return NULL; }
else
  return NULL;
}

void add_to_cash(int ruleindex,int modul,struct transrulelist *ntr)
{
char foo[STRLEN];
int i;
if (all_modules_loaded) {
  snprintf(foo,sizeof(foo),"%d,%d",ruleindex,modul);
  i = rulecashstrings.addstr(foo);
  //stout << "ADDCASH rules from module " << ruleindex << "." <<import.ide(modul) << i << "\n";
  if (i>=0) {
    rulecashtable[i] = ntr; } }
}

int visible(int i, int j, int infos)
{/*int r;*/
  // stout << "visible " << import.ide(i) << " -> " << import.ide(j) << " " << infos << "\n";
  if (aimport || reduceimport)  
        // visibility table is not exported, thus not imported also
        //.. for understanding, see localize1,2
    return 1;
  else {
    if (i == j) return 1;
    if (infos == RLOCOOP)  return 0;
    return (visibilities[i][j] != 0); }
}

struct transrulelist *trsystem::getrules(int ruleindex) 
{
  return(rules[ruleindex]); 
}

struct transrulelist *trsystem::getrules(int ruleindex, int modul) 
{
struct transrulelist *tr, *ntr, **pntr;
  if (modul == -1)
    return(rules[ruleindex]);  // pour l'instante
  else {
    if ((tr = trcash(ruleindex,modul)))
      return tr;
    else {
      /*
	stout << "GET rules " 
	<< ruleindex 
	<< " from module " 
	<< import.ide(modul) 
	<< " list "
	<< rules[ruleindex]
	<< "\n";
      */
      ntr = NULL; pntr = &ntr;
      for(tr = rules[ruleindex]; tr; tr=tr->next) {
        if (visible(modul, tr->rule->getmodule(), tr->rule->getinfos() )) {
          NNEW(*pntr,struct transrulelist);
          (*pntr)->rule = tr->rule; pntr = &((*pntr)->next); }
      (*pntr) = NULL; }

      /*
	stout << ntr << "\n";
      */

      if (ntr) 
        add_to_cash(ruleindex,modul,ntr);
      return ntr;
    }
  }
}



/* --------------  interpreter functions  --------------------- */

int trsystem::nnrewrited(term &t)
{ struct transrulelist *actr;
  contrule *cr;
  term tt;
  tt = t;				// only because of pretty tracing
  actr = nnrules[t.head()];
  while (actr!=NULL) {
    NNEW(cr, contrule(actr->rule,t));
    statistic.inc_manyp();
    if (cr->nextapp(tt,trace)) {
      t.rewrite(tt);
      tt.tdelete();
      DELETE2(cr);
      return(1); }
    DELETE2(cr);
    actr = actr->next;
  }
  return(0);
}

void trsystem::dump()
{ struct transrulelist *tl;
  strategy *sl;
  struct tseq *rhs;
  int i,j,k;
  k=0;
  int n,nameind;
  term l,r;
  struct wherelist *wh;
  int whichmatch;
    term rlabel;
  for (i=0; i<MAXNOFTRN; i++) {
    tl= rules[i]; j=0;
    while (tl!=NULL) {

    tl->rule->getr(n,l,r,nameind,wh,
		   rhs,
		   whichmatch,rlabel);
  if (//commands && 
     ((BREAKSS && tl->rule->breaked) ||
      (!BREAKSS && 
        (SPEC_I == NULL || 
         (SPEC_I != NULL && 
          (0==is_prefix_of_name(SPEC_I,rulenames->ide(i)) ||
           0==strcmp(SPEC_I,fsymtab[l.head()].textform()->rside[0].alfsy()))))))){
      j++; k++;
      //dumpout << "\n[" << k << "]\n";
      dumpout << "\n" 
	      << ((tl->rule->getinfos() == RGLOP)?"global ":"local ")
	      << "rule "<<rulenames->ide(i)
	      << "/" << import.ide(tl->rule->getmodule())
	      <<" ["<<j<<"/"<<k<< "/"
	      <<"fsym=" <<  (tl->rule->getleft())->head() << "/"
	      <<"vars="<<tl->rule->getvarnum()
              <<"]\n";
      tl->rule->dump(0);
//      dumpout << "\nend of rule";
      dumpout << "\n";
  }
      tl=tl->next;
    }
  }
  for (i=0; i<MAXNFSYM; i++) {
    tl= nnrules[i]; j=0;
    while (tl!=NULL) {
    tl->rule->getr(n,l,r,nameind,wh,
		   rhs,
		   whichmatch,rlabel);
  if (//commands && 
     ((BREAKSS && tl->rule->breaked) ||
      (!BREAKSS && (SPEC_I == NULL ||
                   (SPEC_I != NULL && 
           0==strcmp(SPEC_I,fsymtab[l.head()].textform()->rside[0].alfsy())))))) {
      j++; k++;
//      dumpout << "\n[" << k << "]\n";
      if (!batch) {
      dumpout << "\n"
	      << ((tl->rule->getinfos() == RGLOP)?"global ":"local ")
	      << "unnamed rule "
	      <<" ["<<j<<"/"<<k<< "/"
	      <<"fsym=" <<  (tl->rule->getleft())->head() << "/"
	      <<"vars="<<tl->rule->getvarnum()
              <<"]\n";
      tl->rule->dump(0);
//      dumpout << "\nend of rule";
      dumpout << "\n"; }
  }
      tl=tl->next;
    }
  }

  for (i=0; i<MAXNOFSTRAT; i++) {
    sl= strategies_defs[i];
    if (sl!=NULL) {
  if (//commands && 
      ((BREAKSS && sl->breaked) ||
       (!BREAKSS && (
           SPEC_I == NULL ||
	   (SPEC_I != NULL && 0==is_prefix_of_name(SPEC_I,strategyname_defs(i))))))) {
    dumpout << "\nstrategy " 
	    << ((strategyinfos_defs[i] == RGLOP)?"global ":"local ")
	    << strategyname_defs(i) ;
//      if (sl->typeofstr() != -1) dumpout << " for " << typet.ide(sl->typeofstr()) ;
      sl->dump();
//      dumpout<< "\nend of strategy\n";
      dumpout<< "\n";
  }
    }
  }
}




void trsystem::consistency()
{ struct transrulelist *tl;
  /*strategy *sl;*/
  struct tseq *rhs;
  int i; /*,j,k;*/
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
  if (//commands && 
     ((BREAKSS && tl->rule->breaked) ||
      (!BREAKSS && 
        (SPEC_I == NULL || 
         (SPEC_I != NULL && 
          (0==is_prefix_of_name(SPEC_I,rulenames->ide(i)) ||
           0==strcmp(SPEC_I,fsymtab[l.head()].textform()->rside[0].alfsy()))))))){
      tl->rule->consistency();
  }
      tl=tl->next;
    }
  }
  for (i=0; i<MAXNFSYM; i++) {
    tl= nnrules[i];
    while (tl!=NULL) {
    tl->rule->getr(n,l,r,nameind,wh,
		   rhs,
		   whichmatch,rlabel);
  if (//commands && 
     ((BREAKSS && tl->rule->breaked) ||
      (!BREAKSS && (SPEC_I == NULL ||
                   (SPEC_I != NULL && 
           0==strcmp(SPEC_I,fsymtab[l.head()].textform()->rside[0].alfsy())))))) {
      tl->rule->consistency();
  }
      tl=tl->next;
    }
  }
}

void trsystem::breakk(int breaked)
{ struct transrulelist *tl;
  strategy *sl;
  int i; /*,j,k;*/
  int n,nameind;
  term l,r;
  struct wherelist *wh;
  struct tseq *rhs;
  int whichmatch;
    term rlabel;
  for (i=0; i<MAXNOFTRN; i++) {
    tl= rules[i];
    while (tl!=NULL) {

    tl->rule->getr(n,l,r,nameind,wh,
		   rhs,
		   whichmatch,rlabel);
  if (commands && 
     (  
        SPEC_I == NULL ||
        (SPEC_I != NULL && 
          (0==is_prefix_of_name(SPEC_I,rulenames->ide(i))||
           0==strcmp(SPEC_I,fsymtab[l.head()].textform()->rside[0].alfsy()))))) {
        tl->rule->breaked = breaked;
  }
      tl=tl->next;
    }
  }
  for (i=0; i<MAXNFSYM; i++) {
    tl= nnrules[i];
    while (tl!=NULL) {
    tl->rule->getr(n,l,r,nameind,wh,
		   rhs,
		   whichmatch,rlabel);
  if (commands && 
     (
      SPEC_I == NULL ||
      (SPEC_I != NULL && 
      0==strcmp(SPEC_I,fsymtab[l.head()].textform()->rside[0].alfsy())))) {
      tl->rule->breaked = breaked;
  }
      tl=tl->next;
    }
  }

  for (i=0; i<MAXNOFSTRAT; i++) {
    sl= strategies_defs[i];
    if (sl!=NULL) {
  if (commands && 
      (
       SPEC_I == NULL ||(SPEC_I != NULL && 
       0==is_prefix_of_name(SPEC_I,strategyname_defs(i)))))
      sl->breaked = breaked;
    }
  }
}

void dump_seq(ochstream &f, int deep, struct wherelist *seq)
{
  wherelisdump(f,seq,deep);
}
#define INDENTATION 4


void dump_tseq(ochstream &f, int deep, struct tseq *tseq)
{
term *tt;
  if (tseq->is_case) {
    f << "\n";
    odsek(f,deep); f << "switch";
    dump_brlist(f,deep+INDENTATION,tseq->u.more_branches.brlist); 
    f << "\n"; odsek(f,deep); f << "end";
    dump_seq(f,deep,tseq->seq); }
  else {
    tt = tseq->u.one_branch.result;
    f << "\n"; odsek(f,deep); tt->write(f);
    dump_seq(f,deep,tseq->seq); 
  }
}

void dump_brlist(ochstream &f, int deep, struct branch *brlist)
{
  while (brlist) {
    f << "\n";
    odsek(f,deep); f << "case ";
    brlist->test.write(f);
    f << " then ";
    dump_tseq(f,deep+INDENTATION, brlist->tseq);
    brlist = brlist->next;
  }
}





