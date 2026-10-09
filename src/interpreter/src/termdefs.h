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

#ifndef termdefs_h
#define termdefs_h
#include "commondefs.h"

#include <unistd.h> // pour pid_t

#include "acmatchdefs.h"
#include "codes.h"
#include <math.h>


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
#define TSTRING 5

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


class fsym
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
  int breaked;
  fsym();
  fsym(int arity, struct sgrammrule *,int inf);
  void set_aliased();  
  void set_definedas();
  int arity();
  int isnotdefinedas();
  struct sgrammrule *textform();
  int infos();
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
breaked=0;
};
inline fsym::fsym(int a, struct sgrammrule *g,int i) 
                {arit=a;textf=g;inf=i;aliased=0;definedas=0;
semantic = 0; locstrattable = NULL; locstratlen= 0;
breaked=0;
};
inline void fsym::set_aliased() {aliased=1;};
inline void fsym::set_definedas() {definedas=1;};
inline int fsym::arity() {return arit;};
inline int fsym::isnotdefinedas() {return(definedas==0);};
inline struct sgrammrule *fsym::textform() {return textf;};
inline int fsym::infos() {return inf;};
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
	breaked=s.breaked;
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
  int isVarExt;                   // est-ce une variable d'extension ?
  struct leftside {               // leftside/rightside would bo union
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
 {
 private:
  struct hterm *t;

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
  void rlastrecursion(unsigned fsym);
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
   int equalUnify(term with, int nbVar, term *tabVar, term *tabRes);
   void buildResult(term &dest, int n, term *tabRes);
   void buildMGUResult(term &dest, int n, 
		       int nbVar, term *tabVar, term *tabRes);

 public:
  term();
   // builtin syntactic matching
   void syntacticMatching(int unify,term &dest,term subject, term listVar, term fail);
  int  isvalidterm();
  int  varnumbers();
  int  cont_var(unsigned varnum);
  int  ren_var(unsigned old, int nova);
  void  remove_inlines();
  void add_inline_symbol(int extended, int j, term t1, term anies, term t2);
  void add_let_symbol(int j, term asses, term exp);
  void consistency();             // check validity of a term
  void stinit();                  // functions for creating a term
  void crstterm(int val,int type);// from R-derivation produced 
  void crststring(char *val);
  char *getstring();
  void shift_vars(int delta);
  int leave_constructors(int varn);
  void crvar(int );		  // by parser (i.e. using own stack)
  void crterm(int fsi);		  // for ex. term f(a,X) will be
  void popt();			  // created into t by
  void pusht(term);		// t.stinit(); t.crvar('X'); t.crterm('a');
                        // t.crterm('f'); t.popt();
  void  crterm(int fsi,int arity);
  void  crterm_reverse(int fsi,int arity);         // args are in reversed order

  void crvar(int nv, lexem sort);
  void crExtVar(int nv, lexem sort);
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

  int head();                       // head symbol
  int headarity();                  // arity of head
  int inf();                        // info record
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
  term(const term &) = default;	// copies the pointer, as the implicit one did
  int contains_AC();               // true, if it contains at least one AC symbol

  int match(term gt,term *substarray,int varn,struct vilist *&iv);  
					  // syntactic matching
  int matchrec(term gt,term *substarray); 
  int unify(term gt,term *substarray,int varn,struct vilist *&iv);  
  term *deref(term *substarray, int &varn);
					  // syntactic matching
  int unifyrec(term gt,term *substarray); 
  term *derefVariable(int nbVar, term *tabVar,term *tabRes, int &varn);
					 // using partially installed vars
  term acextend();                       // a+b  => x+a+b
  int equal_nground(term t);             // equality of non-ground terms
  int equal(term t);                      // equality modulo AC (terms 
                                          //     must be grounds !!!!!!!!!! )
  int tcmp(term t);                      // ordering on terms without AC
                                          //     must be grounds !!!!!!!!!! )
  int ren_vars_lin(int check, term &r, struct wherelist **whs);
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
  void Awrite(ochstream &);
  void Awriterec(ochstream &);
  void dump();
  void topdump();
  // f(4,a) -> FSYM(INT(5).FSYM(nil,'a').nil,'f')
  char *term2refstring();  // exporting
  void term2refstring(char *buff);
//  void writetostring(char *&strin);
//  void writerectostring(char *&strin);
    void writetostring(char *strin);
    void writerectostring(char *strin);
                         // functions of input/output to AC-matching program

//  int unifscount();
//  int uniftout(char *);
//  void ekeruniftout(ochstream &);

    TERM * toacform();        // conversion to Eker ac form
    void tomyform(TERM * tt); // inverse conversion, result obtained by popt()

};


struct vilist {                  // list of instantiated variables
          term tt;
          struct vilist *next;
};

extern void initAssignment();

class match_state
 {          //      bodies in match.c
private:
  int whichmatch,varnum;
  term vt,gt;
  struct vilist *vis;
  union {
    void *match_state;              // data for ac matching
    int state;                      // data for synt. matching
  } u;
  void expect(char c);
  int handnum(char *s);
  int handvar();
  void handsym();
  void handterm(term &t);
  void freeinstv();
public:
  match_state(term,term,int matchtype,int varn);
  term get_lt();
  term get_rt();
  ~match_state();
//  void freee();                          //  !!!!!!!!!!!! just to debug
  int isnextsol(term *);
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

extern int early1call(lstream *f,grammar *gr,
                 int (*end)(lexem));	// body in earley.c
extern int earlycall(lstream *f,grammar *gr,
                 int (*end)(lexem));	// body in earley.c
extern void readmodules(lstream *f, char *name);           // body in semact.c
extern void fsymtab_remakealias(grammar *gr); // body in tmisc.c
extern void conform_strategies(int warn);


  extern int withrhs;	
#endif // end RUNTIME

