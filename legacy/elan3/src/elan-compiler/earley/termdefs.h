/*
  
    REM - Reduce ELAN Machine

    Copyright (C) 2000-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
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

    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr
    Christophe Ringeissen	e-mail: Christophe.Ringeissen@loria.fr
    Marian Vittek		e-mail: vittek@guma.ii.fmph.uniba.sk

*/
/*
		(c)	INRIA-Lorraine & CRIN
			615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
			email: elan@loria.fr

	$Source: /CVS/aircube/elan3/src/elan-compiler/earley/termdefs.h,v $
	$Revision: 1.1.1.1 $
	$Date: 2001/11/29 08:28:44 $
	$Author: pem $
*/

#ifndef termdefs_h
#define termdefs_h
#include "commondefs.h"

#include <unistd.h> // pour pid_t

#ifndef RUNTIME // begin RUNTIME
#include "acmatchdefs.h"
#include "codes.h"
#include <math.h>

#ifdef STORM
#include "storm_term.h"
#include "interface.h"
#include "types.h"
#endif
#endif // end RUNTIME

/* ------------------------------------------------------------- */

		// values of info record in fsymtab
#define FSNOINFO 0       
#define FSCOMM 1          // op is marked as commutative
#define FSASSOCCOM 2      // op is marked as assoc. comm.

		// values of info records in term structure, 
                // !!!!! can not be 0 and has to be smaller than FSYMCODESBEG
#define TVAR 1		// term is a variable
#define TNORMFS 2	// term is normal functional symbol
#define TIDENT 3	// term is of standart type identifier
#define TNUMBER 4	// term is of standart type number
#ifdef STRINGS
#define TSTRING 5
#endif

// #define BINTaxpredicateset "axpredicateset"

	//		matching algorithm used
#define ACMATCH 0
#define NORMMATCH 1

#define NORENAME -1          /* value for no renamed variable in rule */

#define ADDRENAME 0          /* arguments for vars renaming functions */
#define CHECK 1

struct processdata {
  int counter;               // counter of callings
  pid_t pid;		     // pid of proc. 
  ochstream *pin;            // input of pipe
  lstream *s;               // output from pipe
  ichstream *is;            // output from pipe (just to can close it)
  struct processdatalist ** actplist;  // from which list of processus is taken
  int noblocking;              // read is blocking or not
};

struct processdatalist {
  struct processdata *pd;
  struct processdatalist *next;
};

#ifndef RUNTIME // begin RUNTIME

class fsym
#ifdef GCMEM
: public gc
#endif
 {
 private:
  int arit;
  struct sgrammrule *textf;     // if textf == NULL, symbol is not defined
  int inf; // AC ou non
  int aliased;
  int semantic;
  int definedas; // pour le compilateur
  int *locstrattable;
  int locstratlen;     // for local strategies
 public:
  int apply;
  int ttry;
#ifdef COMMAND
  int breaked;
#endif
  fsym();
  fsym(int arity, struct sgrammrule *,int inf);
  void set_aliased();  
  void set_definedas();
  arity();
  isnotdefinedas();
  struct sgrammrule *textform();
  infos();
  void new_textf_for_alias(struct sgrammrule *gr);
  void operator =(fsym &s);
  void dump();
  void set_semantic(int sem);
  int  get_semantic();
  int  add_sort();
  int get_locstratlen();
  int get_locstrat(int i);
  void set_locstrat(int len, int *locstrattab);
};

inline  void  fsym::set_locstrat(int len, int *locstrattab) {
    locstratlen = len; locstrattable = locstrattab; }
inline  int  fsym::get_locstratlen() { return locstratlen; }
inline  int  fsym::get_locstrat(int i) { return locstrattable[i]; }
inline  void fsym::set_semantic(int sem) { semantic = sem; }
inline  int  fsym::get_semantic() { return ((semantic<0)?-semantic:semantic); }
inline  int  fsym::add_sort() { return (semantic<0); }

inline fsym::fsym() {aliased=0; textf=NULL;definedas=0; 
semantic = 0;locstrattable = NULL; locstratlen= 0;
#ifdef COMMAND
breaked=0;
#endif
};
inline fsym::fsym(int a, struct sgrammrule *g,int i) 
                {arit=a;textf=g;inf=i;aliased=0;definedas=0;
semantic = 0; locstrattable = NULL; locstratlen= 0;
#ifdef COMMAND
breaked=0;
#endif
};
inline void fsym::set_aliased() {aliased=1;};
inline void fsym::set_definedas() {definedas=1;};
inline fsym::arity() {return arit;};
inline fsym::isnotdefinedas() {return(definedas==0);};
inline struct sgrammrule *fsym::textform() {return textf;};
inline fsym::infos() {return inf;};
inline void fsym::new_textf_for_alias(struct sgrammrule *gr) {
		    if (aliased) {
		      textf = gr; 
		      semantic = gr->semantic;
		    }
		};
inline void fsym::operator =(fsym &s)
                   {arit=s.arit;textf=s.textf;inf=s.inf; 
		   aliased=s.aliased;definedas=s.definedas;
                   semantic = s.semantic;
		   locstratlen = s.locstratlen;
		   locstrattable = s.locstrattable;
#ifdef COMMAND
	breaked=s.breaked;
#endif
		   };
inline void fsym::dump() {stout << "[fsym:"<<arit<<","<<textf<<","<<inf<<"] ";}

struct termlist;
struct matchlist;

//              notes on subterms during compilation

#define NOSHARE 0
#define PERFSHARE 1          // perfect share (common subterms)
#define FIRSTSTRSHARE 2           // structure share, some structure will be shared
#define OTHERSTRSHARE 3           // structure share, some structure can be shared if conditions in the rule
                                  // are true
#define RECURSIONSHARE 4     // last recursion optimalization
#define ITERATIONSHARE 5     // iteration optimalization

struct compileinf {
  lexem varsort;                  // sort of the variable
  struct leftside {             // leftside/rightside would bo union
    int varnum;
    int nofshares;
    struct hterm *sharedterm;
  } ls;
  struct rightside {
    int sharetype;
    int varnnum;
    struct hterm *sharedterm;
  } rs;
};

struct hterm {
  unsigned fsymi;            // head symbol index
  unsigned infos;            // infos, such as [TNORMFS,TVAR,TIDENT,TNUM]
  unsigned counter;          // counter of references
  struct compileinf compif;
  term *subt;                // pointer to a_r_r_a_y_ of subterms
};

class term
#ifdef GCMEM
: public gc
#endif
 {
 private:
  struct hterm *t;
#ifdef STORM
  NetNode *Net;
#endif

  void dumprec();
  void addterm(int fsi,int inf,int count, term *st);
  struct hterm *makterm(int fsi,int inf,int count, term *st);
  void laddterm(struct hterm *);
  void pushht(struct hterm *);
  void copyinstallrec(int check,term &into, term *substarray);
  void writerec(ochstream &);
  void markvarshares(term r,int noteItIntoLs);
  void markpfshares(term r,int noteItIntoLs);
  void findbestshare(term r,int deep);
  void searchbestshare(term r);
  void rlastrecursion(int fsym);
  void genShOfShAffectation(FILE *ff,int deep,int shofshi);
  int isItShareofshare(int i,int &myvar, term &sharedson);
  void genfreeundershare(FILE *ff,int deep,term father, int shofshi);
  void genfreeleft1(FILE *ff,int deep,term father, int shofshi);
  void raffrsidevars1();
  void raffrsidevars2();
  void rgenrside();
  void rgenrsidedecl(FILE *ff,int deep);
  void handlesubaff(int share,int i);
  void genlstreccallvaraff(struct hterm *rightt);
  double doublepop();
   // builtin syntactic matching
   int storeVariable(int n, term *tabVar);
   int indexVariable(int nbVar, term *tabVar);
   int equalMatch(term with, int nbVar, term *tabVar, term *tabRes);
   void buildResult(term &dest, int n, term *tabRes);

 public:
  term();
#ifdef VISIGRAPH
  void writeinfile();
  void writehtml();
#endif
   // builtin syntactic matching
   void syntacticMatching(term &dest,term subject, term listVar, term fail);
  int  isvalidterm();
  int  varnumbers();
  int  cont_var(int varnum);
  int  ren_var(int old, int nova);
  int  remove_inlines();
  void add_inline_symbol(int extended, int j, term t1, term anies, term t2);
  void add_let_symbol(int j, term asses, term exp);
  void consistency();             // check validity of a term
  void stinit();                  // functions for creating a term
  void crstterm(int val,int type);// from R-derivation produced 
#ifdef STRINGS
  void crststring(char *val);
  char *getstring();
#endif
#ifdef PARTE
  shift_vars(int delta);
  int leave_constructors(int varn);
#endif
  void crvar(int );		  // by parser (i.e. using own stack)
  void crterm(int fsi);		  // for ex. term f(a,X) will be
  void popt();			  // created into t by
  void pusht(term);		// t.stinit(); t.crvar('X'); t.crterm('a');
                        // t.crterm('f'); t.popt();
  void  crterm(int fsi,int arity);
  void  crterm_reverse(int fsi,int arity);         // args are in reversed order

  void crvar(int nv, lexem sort);
  void crdouble1();       // functions to create double values
  void crdouble2();
  void crdouble3a();
  void crdouble3b();
  void crdouble4();
  void crdouble5a();
  void crdouble5b();
  void crdoubleunmin();

  void crdoublecon(double vv);
/*

  void lstinit();                  // functions for creating a term
  void lcrstterm(int val,int type);// from L-derivation produced 
  void lcrvar(int );		  // by parser (i.e. using own stack)
  void lcrterm(int fsi);		  // for ex. term f(a,X) will be
  void lpopt();			  // created into t by:
			// t.lstinit(); t.lcrvar('f'); t.lcrterm('a');
                        // t.lcrterm('X'); t.lpopt();

  void unistinit();
  void unicrstterm(int val,int type);  // and finaly for creating term
  void unicrvar(int );		// from UNIF output,
  void unicrterm(int fsi);	// because of flattened terms I can't
  void uniaddterm();		// use this from L-derivation
  void unipopt();
*/

  head();                       // head symbol
  headarity();                  // arity of head
  inf();                        // info record
  double doubleval();           // get value of a double constant
  term *subterm(int);           // n-th subterm
  void tdelete();               // delete term 
  void toptdelete();            // delete only top of term 
  void rewrite(term t);         // replace term by another

/* some compiler functions */
  void setcompilevar(int n);    // set variable name for code generation
  int isconstant();
  void marknoshares();
  void affrsidevars();
  void searchpfshares(term r,int noteItIntoLs);
  void searchshares(term where,int shtype); // search shares, type == FIRSTSTRSHARE || OTHERSTRSHARE 
  int lastrecursion(term r);
  void genfreeleft(FILE *ff,int deep);
  void genrside(FILE *ff,int deep,int isrecursive);
  void genrsidedecl(FILE *ff,int deep);
  void genSaveUnsavePF(FILE *ff,int deep);
  void genbuildterm(FILE *ff,int deep);
  void gencorrvalue();
  int isofbuiltintype();
  lexem termtype();
  int givlsvarnum() {return t->compif.ls.varnum;};

/* and others */
  void copyinstall(int check, term &into, term *substarray);// copy term and install 
                                                 //variables w.r.t. substarray.
  void topcopy(term &into);     // copy just a top symbol of term
  int stermreplace(int vp,term t, term byt, term &result);
                                // replace all occurences of t by byt
                                // return(0) if no replacing provided
                                // vp says if it is valide position (because 
                               // of extensionality of AC symbols )
  int occursin(int vp,term t);  // test if t occurs in this
                                // only for ground terms, 
                                // vp says if it is valide position (because 
                               // of extensionality of AC symbols 
  void operator = (term);	// only assignement of pointers !!!!!!!!

  int match(term gt,term *substarray,int varn,struct vilist *&iv);  
					  // syntactic matching
  int matchrec(term gt,term *substarray); 
#ifdef PARTE
  int unify(term gt,term *substarray,int varn,struct vilist *&iv);  
					  // syntactic matching
  int unifyrec(term gt,term *substarray); 
#endif
					 // using partially installed vars
  term acextend();                       // a+b  => x+a+b
  int equal_nground(term t);             // equality of non-ground terms
  int equal(term t);                      // equality modulo AC (terms 
                                          //     must be grounds !!!!!!!!!! )
  int tcmp(term t);                      // ordering on terms without AC
                                          //     must be grounds !!!!!!!!!! )
#ifdef LIN
  int ren_vars_lin(int check, term &r, struct wherelist **whs);
#endif
  int ren_vars(int add_renames_or_check); // function for renaming variables
                                          // using ren.functions from semact.c
//  void incrpoint() {incrcount();}
//  void decrindex() {decrcount(); tdelete();}
  void incrcount();
  void decrcount();
  int  getcount();
  int semantic();
  int is_variable(int *index);
void incrcount(term *substarray);
void decrcount(term *substarray);
//????
  void copy(term &intothis);
  void copyrec(term &intothis);
//????
                                // output functions
  void write(ochstream &);
  void writetobuf(lbuffer &outb);
#ifdef ATERM
  void Awrite(ochstream &);
  void Awriterec(ochstream &);
#endif
  void dump();
  void topdump();

                         // functions of input/output to AC-matching program

//  int unifscount();
//  int uniftout(char *);
//  void ekeruniftout(ochstream &);

    TERM * toacform();        // conversion to Eker ac form
    void tomyform(TERM * tt); // inverse conversion, result obtained by popt()
#ifdef STORM
    STORM_TERM * tostormform();        // conversion to Eker ac form
    void fromstormform(STORM_TERM * tt); // inverse conversion, result obtained by popt()
  void netInsert();
  NetNode *getNet();
#endif

};


struct vilist {                  // list of instantiated variables
          term tt;
          struct vilist *next;
};

extern void initAssignment();

class match_state
#ifdef GCMEM
: public gc
#endif
 {          //      bodies in match.c
private:
  int whichmatch,varnum;
  term vt,gt;
  struct vilist *vis;
  union {
    void *match_state;              // data for ac matching
    int state;                      // data for synt. matching
  } u;
#ifdef STORM
  STORM_TERM *subject;
  int net_match_flag;
  int net_exist_solution;
#endif  
  void expect(char c);
  handnum(char *s);
  handvar();
  void handsym();
  void handterm(term &t);
  void freeinstv();
public:
  match_state(term,term,int matchtype,int varn);
  term get_lt();
  term get_rt();
  ~match_state();
//  void freee();                          //  !!!!!!!!!!!! just to debug
  isnextsol(term *);
};
inline term match_state::get_lt() { return vt; }
inline term match_state::get_rt() { return gt; }


//extern  void setactfsymtab(fsym *tab);	// bodies in term.c
//extern  void saveactfsymtab(fsym *&tab);
extern  int fsyminfo(int fsi);		
extern void var_renameinit();
extern int var_rename(int );
extern int var_was_renamed(int );

extern void fsymtabdump();		// body in the tmisc.c

extern int istrueterm(term t);		// is t == true ??, body in rtmisc.c

extern early1call(lstream *f,grammar *gr,
                 int (*end)(lexem));	// body in earley.c
extern earlycall(lstream *f,grammar *gr,
                 int (*end)(lexem));	// body in earley.c
extern void readmodules(lstream *f, char *name);           // body in semact.c
extern void fsymtab_remakealias(grammar *gr); // body in tmisc.c
extern void conform_strategies(int warn);


  extern int withrhs;	
#endif
#endif // end RUNTIME

