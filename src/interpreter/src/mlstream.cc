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
#include "termdefs.h"
#include "module.h"

#define ADDNUMMAC(name,val) {lbuffer tmp; lexem lex; \
	lex.crnumlex(val); tmp.put(lex);addmac(name,tmp); }

extern int ppexsyntan(lstream &,int,void (*ltol)(lexem,lexem&));
			 //body in ppexparser.c gener. from ppexgram.t

static lexem RWFOR,RWEACH,RWSUCH,RWTHAT,RWAND,RWANDIF,RWTHEN,RWIN,RWIF;

static lexem vartypetab[MAXNOFVAR];
static lexem varnames[MAXNOFVAR];
static struct sgrammrule *vartab[MAXNOFVAR];
extern char *attach_type(char *s,int t);

//static struct sgrammrule *dollar_vartab[MAXNOFVAR];// copie de vartab pour les
//                                     //regles de variable avec un dollar
static char wasaffected[MAXNOFVAR];
static int vartabi=0;
static int varnamesi=0;
static int cexpresult;

mlstream::mlstream(char *name)
         :lstream(name,NONUNDERID)
{ lexem l;
  NNEW(mactab ,mitab(MAXNOFMAC));
  macactstacki =0;
  RWFOR.cridlex(tabofident.addstr("FOR"));
  RWEACH.cridlex(tabofident.addstr("EACH"));
  RWSUCH.cridlex(tabofident.addstr("SUCH"));
  RWTHAT.cridlex(tabofident.addstr("THAT"));
  RWAND.cridlex(tabofident.addstr("AND"));
  RWANDIF.cridlex(tabofident.addstr("ANDIF"));
  RWIF.cridlex(tabofident.addstr("IF"));
  ilex(l);
}

mlstream::~mlstream()
{
  int i;
  DELETE1(mactab);
  for(i=0; i<MAXNOFMAC; i++) macb[i].clear();
  for(i=0; i<macactstacki; i++) {
    macactstack[i].currentb.clear();
    macactstack[i].actionbuf.clear();
  }
}

static void cexlextolex(lexem l,lexem &ll)
{ 
  if (l.isnum()) ll.crnumlex();
  else ll=l;
}

void mlstream::fulex(lexem &l)
{
  l = mflex;
}

int mlstream::getmflex(lexem &l)
{ int r;
 l = mflex;					// get new first lexem
  if (macactstacki == 0) {
     do {lstream::ilex(mflex);} while (mflex.isblankk());
     return(1);
  }
  do {r = buffilex(mflex);} while (mflex.isblankk());
  return(r);
}

int mlstream::macrotest()
{
  /* lexem ll;*/
  lbuffer tmpb;
				// and test mflex for being a macro
    if (mflex.isident() && mactab->member(mflex.idval())) { // it is a macro
       if (macb[mactab->posid].isempty()) {
	  oerr("[preprocessor] non defined macro '",mflex.alfsy(),
               "' used",NULL);
	  beforemess(mflex);
          failexit();
       }
       mflex.crblanklex();
       addsimplecopy(macb[mactab->posid]);
       return(1);
    } else return(0);
}

void mlstream::pilex(lexem &l)
{
  if (getmflex(l))
    macrotest();
}

void mlstream::ilex(lexem &l)
{
  pilex(l);
  if (macroexp()) {
    collident();
    while (macroexp())
      collident();
  }
  else
    collident();
}


void mlstream::collpilex(lexem &l)
{
  pilex(l);
  collident();
}

void  mlstream::collident()
{ lbuffer tmpb;
 lexem fl; /*,fl1;*/
  char *colide,*cc;
  int coll;
  if (mflex.isident())  {
    coll=0;
    pilex(fl); tmpb.put(fl);
    cc = fl.alfsy();
    AALLOSS(colide ,strlen(cc)+1,char); strcpy(colide,cc);
    while (mflex == '_') {     // merge the underscore ident
      coll=1;
      cc = addsuffix(colide,"_");
      CFRE(colide); colide = cc;
      pilex(fl);
      if (mflex.isnum() || mflex.isident()) {
        cc = addsuffix(colide,mflex.alfsy());
        CFRE(colide); colide = cc;
        pilex(fl);
      }
      tmpb.get(fl); fl.cridlex(colide); tmpb.put(fl);
    }
    addsimplecopy(tmpb);
    if (coll) macrotest();
  }
}

/*
void mlstream::prepend(lbuffer &buff)
{ lexem ll;
  lbuffer tb;
  actmacro.copy(tb); actmacro.clear();
  buff.copy(actmacro); actmacro.put(mflex); tb.append(actmacro);
  pilex(ll);
}
*/

void mlstream::addmac(int name,lbuffer &body)
{ int i;
  //sterr << " adding macro " << tabofident.ide(name) << "\n";
  if (mactab->member(name)) {
     owarn();
     sterr << "[mlstream] redefinition of macro " << tabofident.ide(name) << "\n";
  }
  i= mactab->addn(name);
  body.copy(macb[i]);
}


void mlstream::deletemac(int name)		// in reversed order
						// than added
{
//sterr << " deleting macro " << tabofident.ide(name) << "\n";
  if (! mactab->member(name)) {
      oerr("try to delete non existing macro, int.err.\n");
      failexit();
  }
  mactab->removenonp(mactab->posid);
  macb[mactab->posid].clear();
}

void mlstream::addactionprelim()
{
  if (macactstacki>=MAXNESTMAC) {
//actionstackdump();
     oerr();
     sterr << "too much nested macro construction, more than MAXNESTMAC="
           << MAXNESTMAC << " sorry, FATAL\n";
     failexit();
  }
}

void mlstream::addsimplecopy(lbuffer &b)
{ lexem ll;
  addactionprelim();
  struct macactelem &acte = macactstack[macactstacki];
  acte.currentb.clear();
  acte.lastlexem = mflex;
  acte.whichaction = SIMPLECOPYACTION;
  acte.actdata.simiter.i = 1;
  b.copy(acte.actionbuf);
  macactstacki++;
  pilex(ll);
}

void mlstream::addsimiter(int n, lbuffer &b)
{ lexem ll;
  addactionprelim();
  struct macactelem &acte = macactstack[macactstacki];
  acte.currentb.clear();
  acte.lastlexem = mflex;
  acte.whichaction = SIMITERACTION;
  acte.actdata.simiter.i = n;
  b.copy(acte.actionbuf);
  macactstacki++;
  pilex(ll);
}

void mlstream::addiditer(int b,int e,int ch,lbuffer &buf)
{ lexem ll;
  addactionprelim();
  struct macactelem &acte = macactstack[macactstacki];
  acte.currentb.clear();
  acte.lastlexem = mflex;
  acte.whichaction = IDITERACTION;
  acte.actdata.iditer.b = b;
  acte.actdata.iditer.e = e;
  acte.actdata.iditer.ch = ch;
  buf.copy(acte.actionbuf);
  macactstacki++;
  pilex(ll);
}

void mlstream::addincriter(int mn, int b, int e, lbuffer &buf)
{ lexem ll;
  lbuffer nullbuff;
  addactionprelim();
  struct macactelem &acte = macactstack[macactstacki];
  acte.currentb.clear();
  acte.lastlexem = mflex;
  acte.whichaction = INCRITERACTION;
  acte.actdata.incriter.b = b;
  acte.actdata.incriter.e = e;
  acte.actdata.incriter.mname = mn;
  buf.copy(acte.actionbuf);
  addmac(mn,nullbuff);
  macactstacki++;
  pilex(ll);
}


void mlstream::addforeach(lbuffer &buf,struct wherelist *wh,int bvarnamesi)
{ lexem ll;
  lbuffer nullbuff;
  int i;

//stout << "addforeach\n";
//wherelisdump(stout,wh,0);

  addactionprelim();
  struct macactelem &acte = macactstack[macactstacki];
  acte.currentb.clear();
  acte.lastlexem = mflex;
  acte.whichaction = NEXTMATCHACTION;
  acte.actdata.nextmatch.lastws = NULL;
  acte.actdata.nextmatch.whl = wh;
  AALLOSS(acte.actdata.nextmatch.substarray ,vartabi-bvarnamesi, term);
  AALLOSS(acte.actdata.nextmatch.varnames ,vartabi-bvarnamesi, int);
  acte.actdata.nextmatch.varnum = vartabi-bvarnamesi;
  for (i=0; i<vartabi-bvarnamesi; i++) {
     acte.actdata.nextmatch.varnames[i] = varnames[i+bvarnamesi].idval();
     addmac(acte.actdata.nextmatch.varnames[i],nullbuff);
  }
  buf.copy(acte.actionbuf);
  macactstacki++;
  pilex(ll);
}

void mlstream::deleteforeachmacs(int *names, int n)
{ int i;
  for(i=n-1; i>=0; i--) {
    deletemac(names[i]);
  }
}

void mlstream::addforeachmacs(int *names, int n,term *varmatchinst)
{ int i;
  lbuffer macbuf;
  for(i=0; i<n; i++) {
    macbuf.clear(); 
    varmatchinst[i].writetobuf(macbuf);
    addmac(names[i],macbuf);
  }
}



int mlstream::buffilex(lexem &l)
{
  struct macactelem &acte = macactstack[macactstacki-1];
  while (acte.currentb.isempty()) {
     switch (acte.whichaction) {
	case SIMITERACTION: case SIMPLECOPYACTION:
		if (acte.actdata.simiter.i <= 0) {
		  l = acte.lastlexem;
                  macactstacki--;
		  if (l.isblankk()) pilex(l);
		  return(0);
                }
		acte.actdata.simiter.i --;
		acte.actionbuf.copy(acte.currentb);
		break;
	case IDITERACTION:
		if (acte.actdata.iditer.b > acte.actdata.iditer.e) {
		  l = acte.lastlexem;
                  macactstacki--;
		  if (l.isblankk()) pilex(l);
		  return(0);
                }
		acte.actionbuf.copy(acte.currentb);
		acte.currentb.put('_'); 
                { lexem le;
		  le.crnumlex(acte.actdata.iditer.b);
		  acte.currentb.put(le);
		  if (acte.actdata.iditer.b != acte.actdata.iditer.e){
		    le.crcharlex(acte.actdata.iditer.ch);
		    acte.currentb.put(le);
		  }
		}
		acte.actdata.iditer.b ++;
		break;
	case INCRITERACTION:
		if (acte.actdata.incriter.b > acte.actdata.incriter.e){
		  deletemac(acte.actdata.incriter.mname);
		  l = acte.lastlexem;
                  macactstacki--;
		  if (l.isblankk()) pilex(l);
		  return(0);
                }
		acte.actionbuf.copy(acte.currentb);
		deletemac(acte.actdata.incriter.mname);
		ADDNUMMAC(acte.actdata.incriter.mname,acte.actdata.incriter.b);
		acte.actdata.incriter.b ++;
		break;
	case NEXTMATCHACTION:

//stout << "WHERELIST \n";
//wherelisdump(stout,acte.actdata.nextmatch.whl,0);

		if (!isWhereBacktrackNextSol(acte.actdata.nextmatch.whl,
                     acte.actdata.nextmatch.substarray,
                     acte.actdata.nextmatch.lastws,0,
                     acte.actdata.nextmatch.varnum)) {
		  deleteforeachmacs(acte.actdata.nextmatch.varnames,
		                    acte.actdata.nextmatch.varnum);
		  freeWhereBacktrack(acte.actdata.nextmatch.varnum,
				     acte.actdata.nextmatch.lastws,
                                     acte.actdata.nextmatch.substarray);
		  CFRE(acte.actdata.nextmatch.substarray);
		  CFRE(acte.actdata.nextmatch.varnames);
		  freewherelist(acte.actdata.nextmatch.whl);
		  l = acte.lastlexem;
                  macactstacki--;
		  if (l.isblankk()) pilex(l);
		  return(0);
		}
		deleteforeachmacs(acte.actdata.nextmatch.varnames,
		                  acte.actdata.nextmatch.varnum);
		addforeachmacs(acte.actdata.nextmatch.varnames,
		               acte.actdata.nextmatch.varnum,
                               acte.actdata.nextmatch.substarray);
		acte.actionbuf.copy(acte.currentb);
		break;
	default: sterr << "[mlstream::buffilex] unknown macro action #"
                       <<acte.whichaction<<" internal error !!!\n";
     }
  }
  acte.currentb.get(l);
  return(acte.whichaction != SIMPLECOPYACTION);
}

void mlstream::insteaderr()
{ sterr << " instead of " << mflex.alfsy();
}


void mlstream::expect(char  l)
{ if (mflex != l) {
     lstream::oerr();
     sterr << "[mlstream] '" << l <<"' expected "; insteaderr();
     beforemess(mflex);
     failexit();
  }
}


void mlstream::expectident()
{ if (! mflex.isident()) { 
     oerr("[mlstream] identifier expected"); insteaderr();
     beforemess(mflex);
     failexit(); 
  }
}


void mlstream::addvarid(lexem l)
{
       if (varnamesi >= MAXNOFVAR) {
          sterr << "\n too much variables in 'FOR EACH'\n";
          sterr << "more then MAXNOFVAR=" << MAXNOFVAR << "\n";
          failexit();
       }
       wasaffected[varnamesi]='n';
       varnames[varnamesi++] = l;
}


void mlstream::addvars(char* type)
{ lexem actvartype;
   if (! (typet.member(type))) {
     oerr("[preprocessor] unknown type ",type," used",NULL);
     beforemess(mflex);
     failexit();
  }
  actvartype.crtypelex(typet.posid);
  for(; vartabi<varnamesi; vartabi++) vartypetab[vartabi] = actvartype;
}


int mlstream::constexpexp()
{ /*lexem ll;*/
  int res;
  if (mflex.isnum()) {
    return(mflex.numval());
  } 
  if (mflex == '(') {
    if (! ppexsyntan(*this,0,cexlextolex)) failexit();
    res = cexpresult;
    expect(')');
    return(res);
  }
  oerr();
  sterr << "[preprocessor] constant or '(' expected";
  beforemess(mflex);
  failexit();
  return 0; /* to avoid warning */
}

static int pppendofin(lexem le)
{
  return(le.isendofstream()|| le==':' || le==RWAND || le==RWANDIF || le==RWIF);
}

void mlstream::fillmacbuf(lbuffer &tmpb)
{ lexem fl;
  int i;
  i=0; getmflex(fl);
  while (i!=0 || mflex != '}') {
    getmflex(fl); 
    if (mflex.isendofstream()) {
        oerr("macro construction buffer throw end of file\n");
        sterr << "\t did you forget a '}' somewhere ?\n";
	failexit();
    }
    tmpb.put(fl);
    if (fl == '{') i++;
    else if (fl == '}') i--;
  }
}

void mlstream::parsetype(char *&tn)
{ char *n,*nn;
  lexem fl;
  expectident();
  collpilex(fl);
  n = fl.alfsy();
  if (mflex == '[') {
    collpilex(fl); parsetype(nn);
    n = addsuffixs(n,"[",nn,NULL);
    while (mflex == ',') {
      collpilex(fl); parsetype(nn);
      n = addsuffixs(n,",",nn,NULL);
    }
    expect(']');
    collpilex(fl);
    n = addsuffix(n,"]");
  }
  tn = n;
}

int mlstream::macroexp()
{ lbuffer tmpb,andbuf;
 lexem fl; /*,fl1;*/
  struct wherelist *ssnm;
  char *tn;
  int iffirst = false;
  int tmpi;

  if (mflex == '{') {
     fillmacbuf(tmpb);
     pilex(fl);
     if (mflex == '~') {				// {XXX}~N
       pilex(fl);
       tmpi = constexpexp();
      mflex.crblanklex();
       addsimiter(tmpi,tmpb);
       return(1);      
     } else if (mflex == '_') {			//{XXX}_I=N1...N2
       int name;
       int b,e;
       pilex(fl);
       expectident(); name = mflex.idval();
       pilex(fl);
       expect('='); pilex(fl);
       b = constexpexp(); pilex(fl);
       expect('.'); pilex(fl);
       expect('.'); pilex(fl);
       expect('.'); pilex(fl);
       e = constexpexp();
       mflex.crblanklex();
       addincriter(name,b,e,tmpb);
       return(1);
     } else {
	oerr("[mlstream::ilex] syntax error in macro construction");
	beforemess(mflex);
        failexit();
     }
  }
  pilex(fl); tmpb.put(fl);
  if (mflex == '~') {				// X~N
     collpilex(fl); 
     tmpi = constexpexp();
     mflex.crblanklex();
     addsimiter(tmpi,tmpb);
     return(1);
  }
  if ((fl == RWFOR && mflex == RWEACH) || fl == RWIF) { 	// FOR EACH
	int bvartabi,bvarnamesi;
	bvarnamesi = varnamesi;            // for the case of nested FOR EACH
        bvartabi = varnamesi = vartabi;
        tmpb.clear();

//stout << "FOR EACH\n";

       if (fl != RWIF) {
	do {
	  ilex(fl);
	  if (mflex != ':') {
	    expectident(); ilex(fl);
            addvarid(fl);
	    while (mflex==',') {
	      ilex(fl); expectident(); ilex(fl);
              addvarid(fl);
	    }
	  }
          expect(':'); collpilex(fl);
          parsetype(tn);
          addvars(tn);
        } while (mflex== ';');
        for (int i=bvarnamesi; i<vartabi; i++) {
//	  actgram()->addrw(varnames[i]);
	  vartab[i]= actgram()->addvarrule(
		     vartypetab[i],varnames[i],VARSPRI,RVAR,-i-1+bvarnamesi);
	  //	  dollar_vartab[vartabi] =  actgram()->adddollarvarrule(
	  //   vartypetab[i],varnames[i],VARSPRI,RVAR,-i-1+bvarnamesi);
	  //	  writegrrule(sterr,vartab[vartabi],&typet);
	  //	  cout<<"actvartype="<< typet.ide(vartypetab[i].typeval())<<"\n";
 
        }
        if ( mflex != RWSUCH ) {
                oerr("\tSUCH expected "); insteaderr();
		beforemess(mflex);
		failexit();
        }
        collpilex(fl);
        if (mflex != RWTHAT) {
                oerr("\tTHAT expected "); insteaderr();
		beforemess(mflex);
		failexit();
        }
       } // if (fl != RWIF) 
       else iffirst = true;
        { int i;
          struct wherelist **lastm,*snm;
          lexem startsym;
          lastm = &ssnm;
          do {
	    AALLOS(snm ,struct wherelist);
            (*lastm) = snm;
            snm->strateg = NULL;
            if (!iffirst) {
                iffirst = false;
		collpilex(fl);
	    }
	    if (fl == RWAND && mflex ==RWIF) 
		collpilex(fl);
	    if (fl == RWANDIF || fl == RWIF) {
	      snm->leftvarn = IFVARN;
	      startsym=booltype;
	    } else {
              expectident();
              collpilex(fl);
              for (i=bvarnamesi; i <vartabi && varnames[i]!=fl; i++);
              if (i>=vartabi) {
                 oerr("[preprocesor] variable name expected after SUCH THAT or AND instead of ",fl.alfsy(),NULL);
		 beforemess(mflex);
	         failexit();
              }
	      wasaffected[i]='y';
              startsym = vartab[i]->leftside;
	      snm->leftvarn = i-bvarnamesi;
	      expect(':'); collpilex(fl);
	      expect('='); collpilex(fl);
	      expect('('); collpilex(fl);
	      if (mflex.isident()) {
		int reff;
                 collpilex(fl); 

//stout << "STRNAME " << fl.alfsy() << "\n";
		 reff = trrules.strategyindex_refs(
		   attach_type_mod(fl.alfsy(),startsym.typeval(),impmoduli));
                 snm->strateg = trrules.getstrategyadr_refs(reff);
		 conform_strategies(1);
		 trrules.assign_one_ref(1,reff);

              }
	      expect(')'); collpilex(fl);
            }
            if (! actgram()->earleycall(this,startsym,pppendofin)) failexit();
            snm->whereterm.popt();

//stout << "WHERETERM " << snm->whereterm << "\n";
//snm->whereterm.write(stout);stout << "\n";

  	    lastm = & (snm->next);
          } while (mflex==RWAND || mflex==RWANDIF || mflex == RWIF);
          (*lastm) = NULL;
  	  for( i=vartabi-1; i>=bvarnamesi; i--) {
	    if (wasaffected[i]=='n') {
		oerr("[preprocessor] non affected variable ",
		     varnames[i].alfsy()," in FOR EACH",NULL);
		beforemess(mflex);
		failexit();
	    }
	    //	    writegrrule(stout,vartab[i],&typet);
	    actgram()->deleterule(vartab[i]);
	    //	    sterr << "PREPROC DOLLAR delete\n";
	    //writegrrule(stout,dollar_vartab[i],&typet);
	    //	    actgram()->preprocdeleterule(dollar_vartab[i]);
//	    actgram()->removerw(varnames[i]);
	  }
          expect(':'); pilex(fl);
        }
        expect('{');
	fillmacbuf(tmpb);
	mflex.crblanklex();
	addforeach(tmpb,ssnm,bvarnamesi);
        vartabi = bvartabi;
	varnamesi = bvarnamesi;
	return(1);
  }
  if (fl.isident() &&  mflex == '_') {			// maybe X_1,...,X_Nste
      int bn,en;
      char ch;
      lexem ide;
      lbuffer idebuf;
      ide = fl; idebuf.put(ide);
      pilex(fl); tmpb.put(fl);
      if (mflex.isnum() || mflex=='(') {
       bn=constexpexp();
        pilex(fl); tmpb.put(fl);
        if (mflex.ischar()) {
          ch = mflex.charval();
          pilex(fl); tmpb.put(fl);
          pilex(fl); tmpb.put(fl);
          if (fl == '.' && mflex == '.') {
            pilex(fl); tmpb.put(fl);
            pilex(fl); tmpb.put(fl);
            if (fl == '.' && mflex == ch) {
              pilex(fl); tmpb.put(fl);
              pilex(fl); tmpb.put(fl);
              if (fl == ide && mflex == '_') {
                pilex(fl); tmpb.put(fl);
                if (mflex.isnum() || mflex=='(') {   // it is X_1,...,X_N
                  en=constexpexp();
		  mflex.crblanklex();
                  addiditer(bn,en,ch,idebuf);
	          return(1);
  } } } } } } }
  addsimplecopy(tmpb);
  return(0);
}

void mlstream::beforemess(lexem s)
{ int n;
  sterr << "\n\t before  ";
  for (n=0; n<7 && s.isnotendofstream(); n++) { 
    ilex(s);
    sterr << s.alfsy() << " ";
  }
  sterr << "\n";
}



void mlstream::actionstackdump()
{ int i;
  sterr << "\t\t\t\t[actionstackdump] start\n";
  for(i=0; i<macactstacki; i++) {
    sterr << "\t\t[actionstackdump] dumping macactstack["<<i<<"]\n";
    sterr << "\t\t\tactionbuf :\n"; macactstack[i].actionbuf.dump();
    sterr << "\t\t\tcurrentb:\n"; macactstack[i].currentb.dump();
    sterr << "\t\t\tlastlexem = ";macactstack[i].lastlexem.dump();sterr<<"\n";
    sterr << "\t\t\twhichaction = " << macactstack[i].whichaction << "\n";
  }
  sterr << "\t\t\t\t[actionstackdump] stop\n\n";
}

static void deeperr(lstream *f)
{
  f->oerr();
  sterr << "[mlstream.deeperr] evalstacki overflowed over "
        << "MAXDEEPCONSTEXP = " << MAXDEEPCONSTEXP << ", fatal\n";
  failexit();
}

#define DEEPTEST() {if (evalstacki>=MAXDEEPCONSTEXP) deeperr(f);}

int ppexsemact(int n,lexem l,lstream* f)
{
  static int evalstack[MAXDEEPCONSTEXP];
  static int evalstacki=0;
//sterr << "[mlstream.ppexsemact] action # " << n << "\n";
  switch (n) {
  case 0 :
//	evalstacki=0;
	break;
  case 1 :
	cexpresult = evalstack[--evalstacki];
	return(ACCEPTIM);
  case 2 :	
	DEEPTEST();
	evalstack[evalstacki++] = l.numval();
	break;
  case 3 :
	evalstacki--;
	evalstack[evalstacki-1]=evalstack[evalstacki-1]+evalstack[evalstacki];
	break;
  case 4 :
	evalstacki--;
	evalstack[evalstacki-1]=evalstack[evalstacki-1]-evalstack[evalstacki];
	break;
  case 5 :
	evalstacki--;
	evalstack[evalstacki-1]=evalstack[evalstacki-1]*evalstack[evalstacki];
	break;
  case 6 :
	evalstacki--;
	evalstack[evalstacki-1]=evalstack[evalstacki-1]/evalstack[evalstacki];
	break;
  case 7 :
	evalstacki--;
	evalstack[evalstacki-1]=evalstack[evalstacki-1]%evalstack[evalstacki];
	break;
  case 8 :
	evalstack[evalstacki-1]= -evalstack[evalstacki-1];
	break;
  case 9 :
	evalstacki--;
	evalstack[evalstacki-1]=evalstack[evalstacki-1]&&evalstack[evalstacki];
	break;
  case 10 :
	evalstacki--;
	evalstack[evalstacki-1]=evalstack[evalstacki-1]==evalstack[evalstacki];
	break;
  case 11 :
	evalstacki--;
	evalstack[evalstacki-1]=evalstack[evalstacki-1]!=evalstack[evalstacki];
	break;
  case 12 :
	evalstacki--;
	evalstack[evalstacki-1]=evalstack[evalstacki-1]<=evalstack[evalstacki];
	break;
  case 13 :
	evalstacki--;
	evalstack[evalstacki-1]=evalstack[evalstacki-1]>=evalstack[evalstacki];
	break;
  case 14 :
	evalstacki--;
	evalstack[evalstacki-1]=evalstack[evalstacki-1]>evalstack[evalstacki];
	break;
  case 15 :
	evalstacki--;
	evalstack[evalstacki-1]=evalstack[evalstacki-1]<evalstack[evalstacki];
	break;
  case 16:
	DEEPTEST();
	evalstack[evalstacki++] = l.idval();
	break;
  }
  return(NORMCONT);
}
