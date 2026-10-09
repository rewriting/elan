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

#ifdef VISIGRAPH
#include "visigraph.h"
//#include "writehtml.h"
#endif
#include "module.h"
#include <signal.h>
#include <setjmp.h>
//#include <unistd.h>
//#include <syscall.h>
 
#include "commondefs.h"
#include "compiledefs.h"
#include "stdio.h"
#include "command.h"
#include "strategy.h"
#include "rtdatas.h"
#include "strategy.h"
int lis_norm = 0;
int alg_normalisation = 1;
#ifdef STORM
extern void storm_connect(trsystem trrules);
#include "storm_proto.h"
#endif

extern void conform_strategies(int warn);
extern stringtab ldrwt;
extern int ldsyntan(lstream &,int,void (*ltol)(lexem l1,lexem &l2));
					// text in ldparser.c
void Query_dump(ochstream &af);
extern int atermsyntan(lstream &,int,void (*ltol)(lexem l1,lexem &l2));
					// text in atermparser.c
extern int reducesyntan(lstream &,int,void (*ltol)(lexem l1,lexem &l2));
extern void lextomodlex(lexem l,lexem &ml); // text in semact.cc
extern void dump_visibilities(ochstream &ff);
extern void Aread_visibilities(void *af);
extern void lextoamodlex(lexem l,lexem &ml); // text in semact.cc
extern void mkmodname1(struct chlist *&,lexem); // text in semact.cc
extern void mkmodname2(struct chlist *&); // text in semact.cc
extern void mkmodname3(struct chlist *&); // text in semact.cc
extern void mkmodname4(struct chlist *&); // text in semact.cc
extern void mkmodname5(struct chlist *&); // text in semact.cc
extern void addstandards(grammar &gr);          // text in semact.cc
extern void crStandModules();                   // text in semact.cc
extern char *linkdate;                          // text in linkdate.c
#include "banner.cc" 
static char * sortname,*axiomname,*oldaxiomname;
       char *specsource,*specname,*modname;
static char *outputName=0; // nom du a.out
static lstream *specstream;
lexem RWmtfin,qresulttype,sourcetype,querytype;
int qresulttypei,sourcetypei;
int mainstrategy = -1;
int wascheckwith;
term startwith,checkwith,maint;
lstream *mainstream;
struct sgrammrule *axadded = NULL;  // query added rule
struct sgrammrule *ax1added = NULL;  // result added rule

static struct chlist *actmn;
static int trac,qui,bat;
static int lgimoduli = -1;
int optimize = 0;
int alpha_syntax = 0;
int compile = 0;
int cexport = 0;
int Bins = 0;
int expmatch = 0;
int statis = 0;
int big   = 0;
int determLink=0;
int earley_analyser=0;
int lgi_imports = 0;
int reverse_wheres=1;
int generate_code=0; // pour ne pas effacer les *.[cho]
#ifdef STORM
int acmatch_with_storm=0;
#endif
// -- for partial evaluation
int peval_switch = 0;
int extend_strategies = 0;
int peval_loop_test = 0;
int peval_compression = 0;

int no_more_switch = 0;
lexem Sif,Swhere,Send,Sstart,Squery,Sresult,
      Sspecification,Spart,Sspart,SBrac,SEND
     ,Scase,Sotherwise,Sthen,Sswitch, Schoose, Stry
     ;
char *atermfile;
extern term redTerm;
extern int  redStrategyIndex;
extern int  redStrategyIndex_refs;

//////////////////////////////////////////////////////////////////
// Some functions passed to earley parser and testing if a lexem 
// can be considered as the end of term
// arguments: le -- the tested lexem
///////////////////////////////////////////////////////////////////

static int mtendofin(lexem le)
{
  return(le==RWmtfin || le.isendofstream());
}

#ifdef HISTORY
static int endofin(lexem le)
{
  return(le.isendofstream());
}
#endif

static int axchendofin(lexem le)
{
  return(le==Send || le==Spart || le.isendofstream());
}

static int qchendofin(lexem le)
{
  return(le==Sstart || le.isendofstream());
}

int qendofin(lexem le)
{
  return(le==Send || le.isendofstream());
}

static int sendofin(lexem le)
{
  return(le==Send || le == SBrac || le.isendofstream());
}



/////////////////////////////genaxiommodule/////////////////////////
// load the automatically generated module after having read a part of 
// the specification file
// The genrated module contains only one symbol (for ex Vars) and one 
// rewriting rule (for ex. Vars => x y z ;)
// arguments: axt    -- the parsed body of the specification part
//            axname -- the name of the specification part
//            sort   -- the type of the specification part
//
void genaxiommodule(term &axt,char * axname, char *sort)
{ int pi; lexem an,sn; term tt;
  term rlabel;
  struct sgrammrule *gr;
  pi = import.addstr(axname);        // add axname module as imported
  init_visi(pi);
  NNEW(importglobgr[pi] ,grammar);   // crate new fresh grammar in importglobgr[pi]
  an.cridlex(axname); sn.crtypelex(typet.addstr(sort));  // set usefull lexems an and sn
//importglobgr[pi]->addrw(an);
  importglobgr[pi]->addsymbol(an);                       // push axname into rside of the grammar rule
                                                         // create new functional symbol and rely it 
                                                         // with the new created grammar rule
//                                   4000
  gr = importglobgr[pi]->addrule(sn,0,RGLOP,fsymtabi);
  fsym s(0,gr,FSNOINFO);
  if (adump) {
    s.set_semantic(0);
    s.set_locstrat(0,NULL);
    gr -> semantic = 0;
    gr->fsymcode = fsymtabi; 
  }
  fsymtab[fsymtabi++] = s;                               // insert the symbol into symbol table
  tt.stinit(); tt.crterm(fsymtabi-1); tt.popt();         // create the constant "axname"
    trrules.addrule(NULL,0,tt,axt,
		  pi,RGLOP,  // unnamed rule
		  NULL,
		  NORMMATCH,rlabel,NULL);       // add rewriting rule "axname" => axt
   // Marian does not store Vars symbol in the globtermgr
   if (adump) {
     globtermgr.addgrammar(* importglobgr[pi],RGLOP,RGLOP); 
   }
}


//////////////////////////setstandards////////////////////////////
// initialisation of some useful constants
//
void static setstandards()    
{
      Sif.cridlex("if");
      Swhere.cridlex("where");
      Send.cridlex("end");
      Sstart.cridlex("start");
      Squery.cridlex("query");
      Scase.cridlex("case");
      Sthen.cridlex("then");
      Sswitch.cridlex("switch");
      Sotherwise.cridlex("otherwise");
      Schoose.cridlex("choose");
      Stry.cridlex("try");
      Sresult.cridlex("result");
      Spart.cridlex("part");
      Sspart.cridlex("spart");
      SEND.cridlex("END");
      SBrac.crcharlex(']');
      Sspecification.cridlex("specification");
      booltype.crtypelex(typet.addstr("bool"));
      identype.crtypelex(typet.addstr("ident"));
      numtype.crtypelex(typet.addstr("builtinInt"));
      internIdentType.crtypelex(typet.addstr("intern ident"));
      stringtype.crtypelex(typet.addstr("builtinString"));
      internStringType.crtypelex(typet.addstr("intern string"));
      internIntType.crtypelex(typet.addstr("intern int"));
}

//////////////////////////handheadofspec////////////////////////
// parse the first line of a specification file
// arguments: specstream -- input file
//            specname -- the name of the file
//
static void handheadofspec(lstream *specstream,char *specname)
{ lexem fl;
  specstream->fulex(fl);                           // get future lexem into fl
  if (fl != Sspecification) {                      // is it "specification"?
    specstream->oerr();                            // no, error
    sterr << "[handheadofspec.paxmmain] bad head of specification\n";
    sterr << "\t'specification' expected\n";
    failexit();
  }
  specstream->ilex(fl); specstream->fulex(fl);     // read a lexem and get the future lexem
  if (! fl.isident()) {                            // is it an identifier?
     specstream->oerr();                           // no, error
     sterr << "[handheadofspec.paxmmain] bad head of specification\n";
     sterr << "\tname of specification expected\n";
  }
  lexem spn; spn.cridlex(specname);                // create a lexem from the name of file
  if(fl != spn) {                                  // does it correspond to the name of LPL?
    specstream->oerr("name of specification doesn't match the name of file\n");  // no, error
  }
  specstream->ilex(fl);                            // read a lexem
}


//////////////////////////////withterm///////////////////////////////////
// parse the "with term" (i.e. term following after "start with" or 
// "check with" key words in the specification)
// arguments: f     -- input file
//            resw  -- the key word "part" or "query", which will be considered
//                     as a variable
//            type  -- the type of of the "with term"
//            ltol  -- the function testing if a lexem can be considered as 
//                     the end of the parsed term
//            t     <- resulting parsed term
//
static int withterm(lstream *f,lexem resw,lexem type,int (*ltol)(lexem),term &t)
{
  lexem ll;
                           // add the key word "part" or "query" as a variable
  axadded = topgrammar->addvarrule(sourcetype,resw,VARSPRI,RVAR,-1);
  if (commands) {
    ax1added = topgrammar->addvarrule(qresulttype,Sresult,VARSPRI,RVAR,-1);
  }
  { int resan;
    resan=topgrammar->earleycall(f,type,ltol); // parse the term
    if (! resan) {                             // if not parsed correctly ...?
	f->fulex(ll);
	while (ll.isnotendofstream()) f->ilex(ll);
	return(HANDERRORIM);
    }
    t.popt();                                  // take the resulting term
  }
  if (!commands)
    {
      // delete the key word "part" or "query"
      topgrammar->deleterule(axadded);
      axadded = NULL;
    }
  return(NORMCONT);                   // return the NORMal CONTinuation
}


term *replace_qs_rs(term *readedt)
{
  int i,topp;
  term *argg;
  term *p;
  
  AALLOSS(p,queries.top()+results.top()+1,term);
  p[0].stinit(); p[0].crvar(0); p[0].popt(); 
  topp = queries.top();
  for (i=0; i < topp; i++) p[topp-i] = *(queries.trm(i));
  topp = results.top();
  for (i=0; i < topp; i++) p[topp-i+queries.top()] = *(results.trm(i));
  NNEW(argg,term); (*readedt).copyinstall(false,*argg,p); 
  DELETE1(p);
  return argg;
}

void replace_start(term &sw,term &maint, term *readedt)
{
  term tt, *arg, *argg, *arggg;
  argg = replace_qs_rs(readedt);
  if (argg->head() == RUN) {
     arg = argg->subterm(0);
     NNEW(arggg,term); arg->copyinstall(false,*arggg,NULL); 
     queries.push(sourcetype,arggg);

     sw.copyinstall(false,tt,arggg);
     maint.stinit(); maint.pusht(tt); maint.crterm(argg->head());maint.popt();
  } else 
      maint = *argg;
}


void replace_check(term &sw,term &checkt, term *readedt)
{
  term *argg;;
  
  argg = replace_qs_rs(readedt);
  if (argg->head() == RUN) {
     sw.copyinstall(false,checkt,argg->subterm(0));
  } else
      checkt = trueterm;
}


////////////////////////////////////////////////////////////////////////
// The following three functions handle the parsing of parts of the 
// specification and of the query term. The parsing is provided in the following
// manner. Firstly the "starttbuildinit" is called to make the first pass
// of earley's algorithm; then the "starttbuildnextparse" is succesively 
// called to give the possible parsings (as the grammar can be ambiguous,
// several parsings have to be considered). When a good parsing is found the 
// function "starttbuildfin" has to be called 
/////////////////////////////////////////////////////////////////////////



/////////////////////////////starttbuildinit//////////////////////////////
// arguments:
//         ls        -- input file
//         finalsym  -- an identifier which can finish the readed term 
//                      (obviously it is the name of the following part of the 
//                       specification or the "end" for the last Part)
//         tables    <- returns the earleys initialised tables
//
static int starttbuildinit(int usage, lstream *ls,char *finalsym,struct earleystables *&tables)
{
  if (commands && usage) {
     RWmtfin.crcharlex(';');		// !! will be not deleted !!

//stout << querytype.typeval() << "querytype\n";

     if (! topgrammar->earley(*ls,querytype,tables,mtendofin))
       return(0);
  } else {
     RWmtfin.cridlex(finalsym);		// !! will be not deleted !!
     if (! topgrammar->earley(*ls,sourcetype,tables,mtendofin))
       return(0);
  }  
  return(1);
}


//////////////////////////starttbuildnextparse//////////////////////////////
// arguments:
//         checkt    <- in this term the "check with" term is copied with
//                      the "part" variable instantiated on the currently 
//                      parsed term
//         readedt   <- the currently parsed term
//         ambigdis  -- the ambiguities are enable/disable ?
//         tables    <> the currents earleys tables
//
static int starttbuildnextparse(int usage, lstream *ls,
             term &checkt,term *readedt,int ambigdis,struct earleystables *&tables)
{
  readedt->stinit();
  if (topgrammar->earleysecondpass(ls,ambigdis,tables)) {
    readedt->popt();
    if (commands && usage) {
	replace_check(checkwith,checkt,readedt);

    } else
	checkwith.copyinstall(false,checkt,readedt);
    return(1);
  }
  return(0);
}

//////////////////////////starttbuildfin///////////////////////////////
// arguments:
//         maint     <- in this term the "start with" term with instatiated
//                      "query" variable is copied
//         readedt   -- the final parsedterm
//
static void  starttbuildfin(int usage,term &maint,term *readedt)
{
    if (commands && usage) 
	replace_start(startwith,maint,readedt);
     else 
	startwith.copyinstall(false,maint,readedt);
  readedt->tdelete();
}

///////////////////////////selectstartwithterm///////////////////////////////
// parse a part of the specification file (or a query), select a parsing
// satisfying the "check with" function and returns the correct term it 
// in its first arguments
// arguments:
//      maint    <- resulting term
//      ls       -- input file
//      finalsym -- an identifier which can finish the readed term 
//                  (obviously it is the name of the following part of the 
//                  specification or the "end" for the last Part)
//
static int selectstartwithterm(int usage, term &maint,lstream *ls,char *finalsym)
{ term *readedt;
  struct earleystables *etables;
  if (! starttbuildinit(usage,ls,finalsym,etables)) {  // the term is synt. correct?
     topgrammar->earleyfree(etables);return(0);  // no, abort
  }
  NNEW(readedt ,term);
  while(starttbuildnextparse(usage,ls,maint,readedt,!(wascheckwith),etables)){
    trace = trac; quiet = qui; batch = bat;   // a possible parsing
    if ( ! istrueterm(maint)) {               // is the "check with" 
                                              // the true term
      if (trace) {
	 fsymtab_remakealias(topgrammar);
         sterr << "[check] checking term :";
         maint.write(sterr);
         sterr << "\n";
      }
      readedt->incrcount();
      reduce(maint,trace);                     // no reduce the "check with" 
                                               // term
      readedt->decrcount();
    }
//    trac = trace; qui = quiet; bat= batch;
    trace = 0; quiet =1;
    if (istrueterm(maint)) {                   // is the result the "true"
      starttbuildfin(usage,maint,readedt);           // yes, choose this parsing
      topgrammar->earleyfree(etables);         //  free the tables and return 1
      DELETE1( readedt );
      return(1);
    }
  }                                            // no, try another parsing 
  topgrammar->earleyfree(etables);             // no more parsing possible
  DELETE1( readedt );                          // free the tables and return 0
  return(0);
}


/////////////////////////////////handleaxiom////////////////////////////////
// Handle one part of the specification file, read its name, its body and
// creates the automatic module containg the rewriting rule: Name => body.
// No arguments, the needed values are passed from semantic actions in static
// variables.
//
static void handleaxiom()            // arguments passed by static vars !!!!
{ lexem fl;
 term axterm; /* ,axconst;*/
  if (import.member(oldaxiomname)) {        // is there a module of this name
    specstream->oerr();                     // yes, write an error
    sterr << "\t name of axiom '" << oldaxiomname 
          << "' clash with a name of module\n";
    failexit();
  }
  specstream->ilex(fl);                     // no, continue, read the name 
  lexem axn; axn.cridlex(oldaxiomname);     // from the specification
  if (fl == axn) {                          // the names corresponds?
					    // yes, parse the body
    if (! selectstartwithterm(0,axterm,specstream,axiomname)) {
      specstream->oerr("[pqmain] no parsing of specification part '",
		oldaxiomname,
		"' was accepted,\n\tsemantic error in query\n",NULL);
      failexit();
    }
    genaxiommodule(axterm,oldaxiomname,sortname);    // generates the automatic
                                                     // module
  } else {                                   // no, write an error
    specstream->oerr("bad name of specification part\n"); 
    sterr << "\t '" << oldaxiomname << "' expected\n";
    failexit();
  }
}

int loadquery(lstream *f, lexem typ)
{
   if (commands) load_query_mod(f);
   if (withterm(f,Squery,typ,qendofin,startwith) == HANDERRORIM) return(HANDERRORIM);
   return 0;
}

///////////////////////////ldsemact//////////////////////////////////////
// semantic actions called while parsing a Logic Description fil (.lgi)
// the actions are called according the grammar from ldmodgram.t file.
// argumnts:
//    n  -- the action number
//    l  -- last lexem readed by the parser
//    f  -- input file
//
int ldsemact(int n,lexem l,lstream *f)
{ int ind;
/*int res;*/
  lexem strlex, le;
  struct sgrammrule *rightsrule;
  int resan,whstrategy;
  term strateg,lt,rt,rlabel,ter;
  transrule *tr;
  /*char *strnam;*/
  strategy *mstrat;
  char *mrule;
  int mrulei;
  struct namelist *nm;
  int mainstrategy_defs;

  // char strnam[STRLEN];
  /*char strmod[STRLEN];*/

  switch (n) {
  case 0:                                    // initial action
        oldaxiomname=NULL;
        actmn = NULL;
	break;
  case 1:                                    // the name of a part 
	axiomname = l.alfsy();               // save the name
        if (oldaxiomname != NULL) {          // is this the first part?
          handleaxiom();                     // no, read the part of the 
	  topgrammar->freetopl();            //   specification corresponding
	                                     //    to the previous part
	  addstandards(*topgrammar);         // re-init the topgrammar
        }
	break;
  case 2:                                    // name of the LPL
	if (strcmp(modname,l.alfsy())) {
	   f->oerr(" name of logic doesn't correspond to name of file\n");
	   failexit();
	}
	break;
  case 3:                                 // end of specification
	break;

  case 4:
#ifdef VISIGRAPH
       strcpy(calledmod,actmn->s);
       strcpy(importeur,principal);
       add_dependance();
#endif        
                                 // a module to import

	//if (! import.member(actmn->s)) readmodules(f,actmn->s); // if ! imported, read it
        ///????????????????										    
	importmod_inf(actmn->s,f,lgimoduli,RGLOP); // if ! imported, read it

        if (! import.member(actmn->s)) {
	  sterr << "[ldsemact:ldmain.c] int.err.\n"; failexit(); }
                                      // add the global grammar from the
				      // imported module to topgrammar
        topgrammar->addgrammar(* importglobgr[import.posid],RGLOP,RGLOP);
        oldaxiomname = axiomname;     // save the part name(?? not very clever)
        CFRE( actmn ); actmn=NULL;
	break;
  case 5:                             // the last specification description part
        axiomname = "end";
        handleaxiom();
	break;
  case 6:                              // query without check with
	importmod_inf("bool",f,lgimoduli,RGLOP);
        trueterm.topcopy(checkwith);   // set checkwith on 'true'
        wascheckwith = 0;              // note no check with
        break;
  case 7:                              // query part with check with
        break;                         // Hmmmm.
  case 9:                              // sort of "result of" part for the query
        qresulttype.crtypelex(qresulttypei=typet.addstr(actmn->s));
	//////////// QQQQQQQQQ TEMPOR.HACK -- ADD TO .LGI
        printtype = qresulttype;
	printtypei = qresulttypei;
	printwith.stinit(); printwith.crvar(0); printwith.popt(); /// query
/**/        CFRE(actmn->s); CFRE( actmn ); 
        actmn = NULL;
        break;
  case 10:                                  // "start with" for the query
					    // parse the with term
        all_modules_loaded = 1;								      
//	trrules.assign_all_refs(1);
        if (mainstrategy != -1)
	  trrules.assign_one_ref(1,mainstrategy);
        if (loadquery(f,qresulttype)) return(HANDERRORIM);
        break;
  case 11:                                  // "check with" for the query
                                            //  parse the with term
        if (withterm(f,Squery,booltype,qchendofin,checkwith)== HANDERRORIM) 
	  return(HANDERRORIM);
        wascheckwith = 1;
        break;
  case 12:                                // strategy name
        mainstrategy = trrules.strategyindex_refs(
	    attach_type_mod(l.alfsy(),qresulttypei,lgimoduli));
        break;
  case 13:                                // sort of "result of" part for a spec. part
        sortname = actmn->s;
        sourcetype.crtypelex(sourcetypei=typet.addstr(sortname));
        CFRE( actmn ); actmn = NULL;
        break;
  case 14:                                // high level query with strategy
        all_modules_loaded = 1;								      
        load_strat_mod(f,sourcetypei,qresulttypei);
        strlex.crtypelex(add_strat_nont(sourcetypei,qresulttypei));

        topgrammar->addsymbol(strlex);
    ///    topgrammar->addnont(strlex);
        le.crtypelex(RIGHTSTYPE);
        rightsrule=topgrammar->addrule(le,RNOPRIOR,RNOINFO,RIGHTSRULE);
        esemactinit();
	resan = topgrammar->earleycall(f,le,sendofin);
	if (! resan) {
	    sterr << "[semact] error in start with strategy\n"; failexit();
	}
        strateg.popt();
        //stout << "MAIN STRATEGY ";strateg.write(stout);stout<<"\n";stout.flush();
        f->ilex(le); //skip ']'

	// ancien straux
	mrule= attach_type(START_RULE,qresulttypei);
	mrulei = trrules.trruleindex(mrule);
	mainstrategy_defs = trrules.strategyindex_defs(
           attach_type_mod(START_STRATEGY,qresulttypei,lgimoduli),RGLOP);
        // dk(START_RULE)
	NNEW(mstrat, strategy); NNEW(nm, struct namelist);
	mstrat->setname(STRNAMEDONTKNOW,lgimoduli);
	nm->next = NULL; nm->strname = mrulei;
	mstrat->setnamelist(nm);
        mstrat->settypeof(qresulttypei);
	//mstrat->dump();
        //---
	trrules.settypeofstrategy_defs(mainstrategy_defs,qresulttypei);
	trrules.setstrategy_defs(mainstrategy_defs,mstrat);
	  
			   //... ADDING rule x:X => y:Y where y:=[MAIN]x
        lt.stinit(); lt.crvar(0,sourcetype);  lt.popt();
        rt.stinit(); rt.crvar(1,qresulttype); rt.popt();
        tr = trrules.addrule(mrule,2,lt,rt,lgimoduli,RGLOP,
			     NULL,
			     NORMMATCH,rlabel,NULL);
        mainstrategy = trrules.strategyindex_refs(
           attach_type_mod(START_STRATEGY,qresulttypei,lgimoduli)); 
	whstrategy = trrules.strategyindex_refs(
	  attach_type_mod("eval",qresulttypei,
			  evalmoduli(sourcetypei,qresulttypei))); //EEEE
	// [strateg]x
	ter.stinit(); 
        ter.crvar(0,sourcetype); 
        ter.pusht(strateg);
        ter.crterm(apply_code(sourcetypei,qresulttypei),2);
        ter.popt();
        tr->addwhere(reverse_wheres,1,(whstrategy==-1)?((strategy**)NULL):
			      trrules.getstrategyadr_refs(whstrategy),
			      ter, qresulttype);   //strlex);
	//tr->dump(5);
	// this is not finished, should be more complicated
//WWWW        trrules.assign_all_refs(1);
        if (mainstrategy != -1) trrules.assign_one_ref(1,mainstrategy);
        if (whstrategy != -1) trrules.assign_one_ref(1,whstrategy);
        if (loadquery(f,sourcetype)) return(HANDERRORIM);
      break;
//-------------------
  case 15:                                // init of "specification description" part
                                          // set specsource to the name of the spec. file
	if (specname == NULL) usage();
        ind = strlen(specname);
	ind = ind>=4?ind-4:0;             // calcul la position du `.'
        if (specname[ind]!='.') 
           specsource = addsuffix(specname,".spc");
        else {specsource = strdup(specname); specname[ind]=0; }
	// elimination de A dans : A/toto.spc
	{
	  int i;
	  for(i=ind;i>=0 && specname[i]!='/';i--);
	  if(specname[i]=='/')
	    {
	      ind=i+1;
	      for(i=0;specname[ind+i]!=0;i++)
		specname[i]=specname[ind+i];
		specname[i]=0;
	    }
	}


	NNEW(specstream ,lstream(specsource));        // open input file

	handheadofspec(specstream,specname);          // parse the head of the specification
        startwith.stinit(); startwith.crvar(0); startwith.popt();  // init "startwith" to VAR(0)
	break;
  case 16:                        // parsing the name of a module :    ident       -> name
	mkmodname1(actmn,l);
	break;
  case 17:                        // parsing the name of a module :     name[nlist] -> name
	mkmodname2(actmn);
	break;
  case 18:                        // parsing the name of a module :     name,name   -> nlist
	mkmodname3(actmn);
	break;
  case 23:                        // parsing the name of a module :     name,name   -> nlist
	mkmodname4(actmn);
	break;
  case 24:                        // parsing the name of a module :     name,name   -> nlist
	mkmodname5(actmn);
	break;
  case 19:                                  // "check with" for aspecification part
                                            //  parse the with term
        if (withterm(f,Sspart,booltype,axchendofin,checkwith)== HANDERRORIM) 
	  return(HANDERRORIM);
        wascheckwith = 1;
        break;
  case 20:                             // specification part without check with
        trueterm.topcopy(checkwith);   // set checkwith on 'true'
        wascheckwith = 0;              // note no check with
        break;
  case 21: lgi_imports = RGLOP; break;
  case 22: lgi_imports = RLOCOOP; break;
  }
  return(NORMCONT);
}

jmp_buf ebuf;

/////////////////////////////////interrupt////////////////////////////////////
// handles the interruption with ^C of the execution of programs 
////////////////////////////////////////////////////////////////////////////// 
void interrupt_s(int big)
{
  if (commands) {
      stout << "Input type : " << typet.ide(sourcetype.typeval()) << "\n";
      stout << "Output type: " << typet.ide(qresulttype.typeval()) << "\n";
      stout << "Print type : " << typet.ide(printtype.typeval()) << "\n";
      stout << "Strategy   : " << trrules.strategyname_refs(mainstrategy) << "\n";
      stout << "Start_with : " ; startwith.write(stout); stout << "\n";
      stout << "Check_with : " ; checkwith.write(stout); stout << "\n";
      stout << "Print_with : " ; printwith.write(stout); stout << "\n";
      stout << "------------\n";
    }
  statistic.timestop();
  statistic.write(statis,big);
}

void sort_dump()
{
int i;
 dumpout << "\nSorts";
 i = 0;
 for(typet.forinit(); typet.forcond(); typet.fornext()) {
   if (i%5 == 0) dumpout << "\n";
   dumpout << typet.ide(typet.forindex()) << ", "; i++; }
 dumpout << "\n\n";
}

void interrupt_d()
{
  trrules.dump();                            //   rewriting rules
  sort_dump();
  globtermgr.write(dumpout,RGLOP,&typet,     //   grammar
               "Function symbols\n","\n");
}

void interrupt(int sig)
{
  int c;

  while(1) {
#ifdef VISIGRAPH
  stout <<
    "\n\n[ executionAbort Continue Dump Exit Graphmodule Statistics\n" <<
    "  changeTrace changeQuiet (ACDEGSTQ|acdegstq) ] ? ";
#else
  stout << "\n\n[ executionAbort Continue Dump Exit Statistics\n" 
	<< "  changeTrace changeQuiet "
#ifdef TO_BE_DISTRIBUTED
	<< "(ACDESTQ|acdestq)] ?";
#else
  << "Input Output stRategy (ACDESTQIOR|acdestqior)] ?";
#endif
#endif

  stout.flush();
  switch (c=getchar()) {
  case 'A': case 'a':                          // abort this execution
    longjmp(ebuf,1);                           // jump into main procedure
  case 'D': case 'd':                          // dump :
    interrupt_d();
    break;
  case 'R': case'r':                            // get in the name of strategy
    {
      char strname[IDLEN];
      stout << "enter strategy name\n";
      scanf("%s",strname);
      interrupt_r(strname);
    }
  break;
  case 'I': case 'i':                           // get in the input type
    {
      char isortname[IDLEN];
      stout << "enter input sort name\n";
      scanf("%s",isortname);
      interrupt_i(isortname);
    }
  break;
  case 'O': case 'o':                            // get in the output type
    {
      char osortname[IDLEN];
      stout << "enter output sort name\n";
      scanf("%s",osortname);
      interrupt_i(osortname);
    }
  break;
  case 'Q': case 'q':                           // switch quiet/non-quiet
    quiet = qui = ! qui;
    stout << "\n "; if (! quiet) stout << "no "; stout << "quiet";
    trace= 0; trac=0;
    break;
  case 'S': case's':                            // write statistics
    interrupt_s(c=='S');
    break;
  case 'T': case 't':                           // new trace level
    quiet=0; qui=0;
    trac =0; trace =0;
    while (getchar()!='\n');
    sterr << "\nenter new level: ";stout.flush();
    if (isdigit(c=getchar())) {
      ungetc(c,stdin); fscanf(stdin,"%d",&tracelevel);
    } else {trac=1; trace=1;}
    break;
    
#ifdef VISIGRAPH    
  case 'G': case 'g':  graphmod();  //body in visigraph.c
    break;
#endif
  case 'E': case 'e':
    exit(0);                   // exit
    
    /*
     * On ne sait pas implanter le retour au milieu d'un calcul
     */
  case 'C': case 'c' :               // Continue the execution
    while (c!='\n' && c!=EOF) c=getchar();       // flush the current input 
    sterr << "\n[] execution continue:\n";
    signal(SIGINT,interrupt);    
    return;                                      // and return from interrut
    //default: 
  }
  while (c!='\n' && c!=EOF) c=getchar();       // flush the current input 
  signal(SIGINT,interrupt);    
  } // while
}


void conform_strategies(int warn)
{
  int i,r;
  for(i=0; i < all_strategi; i++) {
    // stout << "BEFORE\n"; all_strateg[i]->dump();
    if (all_strateg[i]) {
      r = all_strateg[i]->conform(warn);
     // stout << "AFTER\n"; all_strateg[i]->dump();
      if (r) all_strateg[i]=NULL;
    }
  }
} 


///////////////////////////////main////////////////////////////////
//

int main(int argc, char **argv)
{
  char *modsource,*moddest_c,*callcompilstr,*calllinkstr=NULL,*nsoptstr; /* ,*includef*/
  char *moddest_h=NULL,*moddest;

  FILE *genof_c,*genmof;
  FILE *genof_h;
  lexem lex;
  int i,ind,len;
  nsoptstr="";
  optimize=0;

#ifdef PEM
    //init_alloc();
#endif
  initAssignment();  

  elanlib = getenv("ELANLIB");                      // get environemt variable ELANLIB
  if (elanlib==NULL) {                              // is it defined?, if not set it on default .
      // sterr << "[warning] environment variable ELANLIB is not defined\n"
      //	   << "\tdefault is " << PREFIX 
      //           << "\n";
     elanlib = PREFIX;
  }
  elanlib = addsuffix(elanlib,"/");                 // add there the "/"
  perslib=getenv("SECONDELANLIB");                  // get environemt variable SECONDELANLIB
  i=1;

  NNEW(stratrules ,stringtab(MAXNOFTRN+MAXNOFSTRAT)); // table of all rule & strat names
  for(all_strategi=0; all_strategi < MAXNOFSTRAT; all_strategi++) 
    all_strateg[all_strategi]=NULL;
  all_strategi = 0;                                 // table of all defined strategies

  while (i<argc && *argv[i]=='-')
    {                 // examine the options given at the command line

      // -t trace level
      if(!strcmp(argv[i],"-t") || !strcmp(argv[i],"--trace"))
	{
	  if (isdigit(*argv[i+1]))
	    {
	      i++; 
	      if (! sscanf(argv[i],"%d",&tracelevel))
		interr();
	    }
	  else trace =1;
	}
      // dump
      else if(!strcmp(argv[i],"-d") || !strcmp(argv[i],"--dump"))
	dump = 1;     
      else if(!strcmp(argv[i],"--export")) {
	adump = 1; atermfile = argv[i+1]; i++; }
      else if(!strcmp(argv[i],"--cexport")) {
	adump = 1; cexport = 1; atermfile = argv[i+1]; i++; }
      else if(!strcmp(argv[i],"--import")) {
	aimport = 1; }
      else if(!strcmp(argv[i],"--reduce")) {
	//batch = 1; quiet = 1; warnings= 0; dump= 0; trace= 1; 
	reduceimport = 1; }
      //  warnings disable
      else if(!strcmp(argv[i],"-w") ||!strcmp(argv[i],"--warningsoff") )
	warnings = 0;
      // quiet regime
      else if(!strcmp(argv[i],"-q") || !strcmp(argv[i],"--quiet"))
 	{ quiet = 1; warnings= 0; dump= 0; trace= 0; }
      else if(!strcmp(argv[i],"--quote"))
	quote = 1;
      else if(!strcmp(argv[i],"--noquote"))
	quote = 0;
      else if(!strcmp(argv[i],"--extend")) {
	extend_strategies = 1; }
      else if(!strcmp(argv[i],"--no_more")) {
	no_more_switch = 1; }
      else if(!strcmp(argv[i],"--peval")) {
	peval_switch = 1; extend_strategies = 1; }
      else if(!strcmp(argv[i],"--compr")) {
	peval_switch = 1; extend_strategies = 1; peval_compression = 1; }
      else if(!strcmp(argv[i],"--ptest")) {
	peval_switch = 1; extend_strategies = 1; peval_loop_test = 1; }
      else if(!strcmp(argv[i],"--lis"))
	lis_norm = 1;
      else if(!strcmp(argv[i],"--los"))
	lis_norm = 0;
      else if(!strcmp(argv[i],"--norm1"))
	alg_normalisation = 1;
      else if(!strcmp(argv[i],"--norm2"))
	alg_normalisation = 2;
      else if(!strcmp(argv[i],"--MAXLENNTERM")) {
	if (! sscanf(argv[++i],"%d",&MAXLENNTERMv)) interr(); }
      // batch regime
      else if(!strcmp(argv[i],"-b") || !strcmp(argv[i],"--batch"))
	{ batch = 1; quiet = 1; warnings= 0; dump= 0; trace= 0; }
      // switch B means compile built-ins rules
      else if(!strcmp(argv[i],"-B") || !strcmp(argv[i],"--builtin"))
 	Bins = 1; 
      //
      else if(!strcmp(argv[i],"-s") || !strcmp(argv[i],"--statistic")) 
	{ statis = 1; big = 0; }
      // 
      else if(!strcmp(argv[i],"-S"))
	{ statis = 1; big = 1; }
      // 
      //else if(!strcmp(argv[i],"-S"))
      //statis = 2;
      // elan library
      else if(!strcmp(argv[i],"--elanlib"))
	{ elanlib = argv[i+1];
	elanlib = addsuffix(elanlib,"/");
	i++; }
      // personal library
      else if(!strcmp(argv[i],"-l") || !strcmp(argv[i],"--secondlib"))
	{ perslib = argv[i+1]; i++; }
      // for compiler: optimize
      else if(!strcmp(argv[i],"-O") || !strcmp(argv[i],"--optimize"))
	{ optimize = 1; nsoptstr=addsuffix(nsoptstr," -O"); }
      // compiler <-> interpreter
      else if(!strcmp(argv[i],"-c") || !strcmp(argv[i],"--compiler"))
	compile = 1;  
      // for compiler: link the resulting code with a deterministic
      // run-time library (usefull for the architectures where the
      // back-tracking library is not yet implemented)
      else if(!strcmp(argv[i],"-n") || !strcmp(argv[i],"--deterministic"))
	{ determLink = 1; nsoptstr=addsuffix(nsoptstr," -n"); }
      // for compiler: generate faster many-to-one matching alg.
      else if(!strcmp(argv[i],"-e") || !strcmp(argv[i],"--expmatch"))
	expmatch = 1;
      // command language
      else if(!strcmp(argv[i],"-C") || !strcmp(argv[i],"--command"))
	commands = 1;
#ifdef STORM
      // AC-matching with Storm
      else if(!strcmp(argv[i],"--storm"))
	acmatch_with_storm=1;
#endif
      else if(!strcmp(argv[i],"--earley") || !strcmp(argv[i],"--exe"))
	{
#ifndef EARLEY
	  printf("recompile ELAN with EARLEY flag\n");
	  failexit();
#endif
	  earley_analyser=1;
	  compile = 1;  
	  nsoptstr=addsuffix(nsoptstr," -EARLEY");
	}
      else if(!strcmp(argv[i],"--code"))
	generate_code=1;
      else if(!strcmp(argv[i],"-o") || !strcmp(argv[i],"--output"))
	{ outputName = argv[i+1]; i++; }


      //
      // default
      //
      else
	{
	  sterr << "[warning] unknown option '" << argv[i] << "'";
	  if (compile) {
	    sterr << " passed to cElanScript\n\n";
	  } else {
	    usage(); 
	  }
	  nsoptstr=addsuffixs(nsoptstr," ",argv[i],NULL);
	}
      // pass to the next option
      i++;                                            
    }
  if (!batch)
    banner();
  if (argc != i+2 && argc != i+1) {
    usage();
  }
  strcpy(elanlibqnq,elanlib); strcat(elanlibqnq,(!quote)?"share/elanlib/noquote/":"share/elanlib/quote/");
  strcpy(elanlibcommon,elanlib); strcat(elanlibcommon,"share/elanlib/common/");
  strcpy(elanlibstrat,elanlib); strcat(elanlibstrat,"share/elanlib/strategy/");
  strcpy(elanlibref,elanlib); strcat(elanlibref,"share/elanlib/ref/");
  init_files();

  // if the personal library was not affected by the user
  if (perslib == NULL) perslib="..";                
  // set it on ..
  perslib = addsuffix(perslib,"/");
  // get the .lgi filename
  // on cherche dans argv[i] le premiere '/' en partant de la fin

  {
    //modname=argv[i];                                      
    char *pos;
    for(pos=argv[i]+strlen(argv[i]) ; pos!=argv[i] && *pos!='/' ; pos-- );
    if(*pos=='/') pos++;
    modname=pos;
    ind = strlen(modname); ind = ind>=4?ind-4:0;
  }
  // if it has no extension put there .lgi
  if (modname[ind]!='.')
    if(aimport || reduceimport) {
      modsource = addsuffix(modname,".ref");
    } else {
      modsource = addsuffix(modname,".lgi");
    }
  else {
    modsource = strdup(modname);
    modname[ind]=0;
  }
  
  lgimoduli = impmoduli = import.addstr(modsource);
  init_visi(impmoduli);


#ifdef VISIGRAPH
  strcpy(principal,modsource);
  inittabmodule();
#endif       
 
#ifdef STORM
  if(acmatch_with_storm)
     {
       sig_init();
       STORM_init(); 
     }
#endif
  // get the .spc filename
  specname=argv[i+1];
  // init the value of trueterm on true
  settrueterm();     
  // create an empty grammar in topgrammar
  NNEW(topgrammar ,grammar);
  // set values of usefull symbols
  setstandards();                 
  // add default rules into topgrammar        
  addstandards(*topgrammar);      
  // create built-in modules
  crStandModules();                   
  // memorize user-defined options
  trac = trace; qui = quiet; bat = batch; 
  // set quiet execution for the evaluations made by
  trace = 0; quiet =1;            
  // the pre-processor
  // ................................................
  if (aimport) {
    if (!batch) {
      stout << "\nImport form file " << modname << "\n\n"; }
    lstream ff(modsource);

    // tabofident.dump("tabofident");
    tabofident.Aread(&ff,0);
    //tabofident.dump("tabofident");
    typet.Aread(&ff,0);         // typet.dump("sorts ");
    import.Aread(&ff,0);
    // ...builtinmodules.Aread(&ff);
    // REPLACED BY LOCALISATION .... Aread_visibilities(&ff);
    // tabofident.Adump(stout, "idents ");
    trrules.Aread_tabs(&ff);
    // parse the rest
    aterm_parse = 1;
    if (! atermsyntan(ff,0,lextoamodlex))
      failexit(); 

    // assigning refs table to the defs table
    trrules.joinrdefs();

    aterm_parse = 0; }
  else 
  if (reduceimport) {
    if (!batch) {
      stout << "\nReduce form file " << modname << "\n\n"; }

    // ichstream mainin(stdin,"Reduce form file"); lstream reducestream(&mainin);  
    lstream reducestream(modsource);
    tabofident.Aread(&reducestream,0);
    typet.Aread(&reducestream,0); 
    import.Aread(&reducestream,0);
    trrules.Aread_tabs(&reducestream);
    aterm_parse = 1;
    if (! reducesyntan(reducestream,0,lextoamodlex))
      failexit(); 
    // assigning refs table to the defs table
    trrules.joinrdefs();
    aterm_parse = 0; 
    trace = trac; quiet = qui;
    batch = bat; interruptnow=0; traceind=0;
    transred(redStrategyIndex,redTerm,big);  
    exit(0);
    }
  else
    {
      /*lexem fl;*/
    // open the inut .lgi file
    lstream ff(modsource);                         
    // parse it   
    if (! ldsyntan(ff,0,lextomodlex))
      failexit(); 

    conform_strategies(1);
    if (extend_strategies)
      trrules.expand_strategies();
    if (peval_switch) { peval_init(); trrules.peval(&ff); }
    }
  // reset the printed form of aliases
  fsymtab_remakealias(topgrammar);
  //topgrammar->write(dumpout,RGLOP,&typet,
//	       "all functions dump\n","end of functions dump\n");
//               "Function symbols\n","\n");
  // .....................................
  // if dump selected
  if (dump) {      
    //   dump the rules                                
    trrules.dump();
    //   dump the signature
    sort_dump();
    globtermgr.write(dumpout,RGLOP,&typet, "Function symbols\n","\n");
  }
  if (adump) {

    trrules.localize1();

    ochstream af(atermfile);
    tabofident.Adump(af,"Identifiers");
    typet.Adump(af,"Sorts");
    import.Adump(af,"Modules");
    //..... REPLACED by LOCALIASATION dump_visibilities(af);
    trrules.Adump_tabs(af);

    globtermgr.rmark(1,0); // set 0
    topgrammar->rmark(1,0); // set 0

    globtermgr.rmark(0,INGLOBGRAM); // or bit
    topgrammar->rmark(0,INTOPGRAM); // or bit

    globtermgr.Adump(af,INGLOBGRAM|INTOPGRAM);
    globtermgr.Adump(af,INGLOBGRAM);
    topgrammar->Adump(af,INTOPGRAM);

    trrules.Adump(af);
    //sort_dump();
    //globtermgr.write(dumpout,RGLOP,&typet,"Function symbols\n","\n");
    Query_dump(af);
    trrules.localize2();   // ca serve a rien, mal fait
    if (!batch) {
      stout << "\nExport file " << atermfile << " has been created\n\n"; }
    return 0;
  }
  // init some values to compile strategies
  modinit(); 

#ifdef EARLEY

  //globtermgr.earleyPrettyDump(stout);
  //topgrammar->earleyPrettyDump(stout);
#endif
  // recover the user defined execution switches
  trace = trac;
  quiet = qui;
  batch = bat;
  interruptnow=0;
  traceind=0;
  // IF COMPILER
  if (compile)
    {
      //   set moddest onto the name of output file
      moddest_c = strdup(modsource);
      len=strlen(moddest_c);
      moddest_c[len-3]='c';
      moddest_c[len-2]=0; 
      genof_c = fopen(moddest_c,"w");
      moddest=strdup(modsource);
      moddest[len-4]=0;
	//      moddest_c[len-4]=0;

	if (!batch) { fprintf(stderr,"[.eln->.c]:\n"); }
      //   set rsff static variable for the code generator
      setrsff(genof_c); 

#ifdef EARLEY
      if(earley_analyser)
	{
	  moddest_h = strdup(modsource);
	  len=strlen(moddest_h);
	  moddest_h[len-3]='h';
	  moddest_h[len-2]=0; 
	  genof_h = fopen(moddest_h,"w");

	  // generation de la grammaire et des tables de types et d'identificateurs
	  ochstream earleyOut(genof_h);

          // globtermgr.earleyDump(earleyOut);

	  /* BUG ??? */ topgrammar->earleyDump(earleyOut);

	  //earleyOut << "#define SOURCETYPE " << sourcetype.numval() << "\n";
// BIZZARDDDE	  earleyOut << "#define SOURCETYPE " << qresulttype.numval() << "\n";
	  earleyOut << "#define SOURCETYPE " << qresulttype.typeval() << "\n";
	  earleyOut.~ochstream();

	  // Generation de maincompiled.c dans moddest_c
	  fprintf(genof_c,"#include \"%s\"\n\n",moddest_h);
	  // generate the construction of the given term
	  genmaintfile(genof_c,maint,mainstrategy); 
	}
#endif
      //   generate the preambule of the code
      genpreambule(genof_c);
      //   compile the rewriting rules
      trrules.compile(genof_c);
      //   generate the epilog
      genepilog(genof_c);     
      //   close output file        
      fclose(genof_c);        

#ifdef EARLEY
      if(earley_analyser)
	{
	  if(outputName==0) outputName = addsuffixs("a.out",NULL);
	  callcompilstr = addsuffixs(
#if defined(i386) || defined(__i386)
        "gcc -DNONUNDERSCORED -w -I",
#else
        "gcc -O2 -DNONUNDERSCORED -w -I",
#endif
          elanlib,
          "Compiled -L",elanlib,"Compiled/`uname -m` -DEARLEY ",
           moddest_c," -o ",outputName," -lelan -learley -lm",NULL);
          if (!batch) 
	    fprintf(stderr,"[.c->%s]: %s\n",outputName,callcompilstr);
	  system(callcompilstr);
	  if(!generate_code)
	    {
	      // on efface moddest_c et moddest_h
	      char * callremovefile;
              callremovefile= addsuffixs("/bin/rm -f ",moddest_c," ",moddest_h,NULL);
	      if (!batch) { fprintf(stderr,"%s\n",callremovefile);}
              system(callremovefile);
	      CFRE(callremovefile);
	    }
	}
      else
	{
	  //   set arguments strings to call the cElanScript
	  callcompilstr = addsuffixs("$ELANLIB/Compiled/`uname -m`/cElanScript -c ",nsoptstr," ",moddest,NULL); 
	  calllinkstr = addsuffixs("$ELANLIB/Compiled/`uname -m`/cElanScript -l ",nsoptstr," ",moddest," -N ",NULL);
// nonearley.a containt fictive IOS for -c
//	  calllinkstr = addsuffixs("$ELANLIB/Compiled/`uname -m`/cElanScript -l ",nsoptstr," ",moddest,NULL);
	  if (!batch) {fprintf(stderr,"[.c->.o]: %s\n",callcompilstr);}
	  //  call cElanScript to call C-compiler
	  system(callcompilstr);  
	}
#else
      //   set arguments strings to call the cElanScript
      callcompilstr = addsuffixs("$ELANLIB/Compiled/`uname -m`/cElanScript -c ",nsoptstr," ",moddest,NULL); 
      calllinkstr = addsuffixs("$ELANLIB/Compiled/`uname -m`/cElanScript -l ",nsoptstr," ",moddest,NULL);
      if (!batch) {fprintf(stderr,"[.c->.o]: %s\n",callcompilstr);}
      //  call cElanScript to call C-compiler
      system(callcompilstr);  
#endif
    }

  {
    if(!earley_analyser) {
    lstream *pip;
    /* term resterm;*/
    // set the ebuf for the case of longjmp interrup
    setjmp(ebuf);                                   
    // enable ^C - interrupt    
    signal(SIGINT,interrupt);                       

    if (!batch) 
	if (commands) {
	    stout << "\nenter command finished by ';':\n"; }
	else {
	    stout << "\nenter query term finished by the key word 'end':\n"; }
    stout.flush();
    // init input char stream from standard input
    ichstream mainin(stdin,"query source");
    // init input lexem stream from standard input   
    lstream mainstreamo(&mainin);  
    mainstream = &mainstreamo;
   if (commands)
    lstr[lstri++] = mainstream;
    do {
      // get future lexem
      mainstream->fulex(lex);                        
      if (lex.isendofstream()) {
      if (commands)
	  if (pop_lstr()) continue; else break; else
	    // is it end of stream?
	     break; 
	}
      if (commands) {
	queries.addax(topgrammar,1);
	results.addax(topgrammar,queries.top()+1); }
      // get the term entered by the user
      if (! selectstartwithterm(1,maint,mainstream,"end")) { 
        mainstream->oerr("[pqmain] no parsing of query was accepted, semantic error in query\n");
      } else {
	// IF COMPILER
	if (compile) {  
	  //    init compilations fields in the term
  	  maint.marknoshares();  
	  maint.affrsidevars();
	  //    open maincompiled.c
          genmof=fopen("maincompiled.c","w");
	  //     generate the construction of the given term
	  genmaintfile(genmof,maint,mainstrategy); 
	  fclose(genmof);
	  if (!batch) {fprintf(stderr,"[.o->a.out]: %s\n",calllinkstr);}
	  //     call cElanScript link the executable
	  system(calllinkstr);                              
        }  
	// recover the user defined execution switches
        trace = trac;
	quiet = qui;
	batch = bat; 
	interruptnow=0;
	traceind=0;
	// set the ebuf for the case of longjmp interrup
	if (setjmp(ebuf) == 0)  { 
	  // enable ^C - interrupt
	  signal(SIGINT,interrupt);
	  // IF COMPILER
          if (compile) {          
	    fprintf(OUTS,"[execute]:\n  ");
	    // execute "time a.out" 
	    // and catch the result into the pipe
	    pip = new lstream("2>&1 time a.out","pipe to executed job");
	    // flush some dummy chars # from the pipe
	    do {pip->ilex(lex);pip->fulex(lex);
	    } while (lex == '#');
	    //    write results terms while any
	    while (writereturnedterm(pip,stout))
	      fflush(OUTS);
	    fprintf(OUTS,"\n[kill]:\n");
	    //       delete the pipe (and kill the subprocess)
	    delete pip;                                      
#ifndef PEM
	    //       remove the core (one never knows)
	    system("/bin/rm -f core ");                     
#endif
          } else {
            in_runtime++;
	    // IF INTERPRETER
	    if (commands) {
              commander(mainstream,maint); }
            else {
	      //     rewrite the input term according the mainstrategy
	        transred(mainstrategy,maint,big);  }
            in_runtime--;
          }
	}
	// ????
        trace = 0; quiet =1;   
       }
      // look at the future lexem
      mainstream->fulex(lex);                       
      if ((!batch) && lex.isnotendofstream()) {
    if (commands) {
      stout << "\nenter command finished by ';':\n"; }
    else {
      stout << "\nenter query term finished by the key word 'end':\n"; }
         stout.flush();      
       }
  if (commands) {
    queries.delax(topgrammar);
    results.delax(topgrammar); }
  // get the lexem
  mainstream->ilex(lex);     
  
    if (lex.isendofstream()) {
      if (commands)
        if (pop_lstr()) continue; else break; else
	  // is it end of stream?
	  break;  
	}

    } while (1) ; //while (lex.isnotendofstream());
    }
  }
  DELETE1( topgrammar );                      
  // kill all processus stored in tables
  kill_all_processus();                                     
  return(0);
}

