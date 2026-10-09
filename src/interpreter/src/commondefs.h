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

#ifndef __commondefs_h
#define __commondefs_h




#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdarg.h>

#include "mitab.h"
//#include "listmacr.h"

#include "stringtab.h"
#include <stdio.h>
#include <stdlib.h>
#include "mallo.h"

// FIN DES INCLUDES

// DEFINITION DES CONSTANTES POUR ELAN

#define NBITS 32	// number of bits in word in current arch.

                 // values of info record in term grammar
#define RNOINFO 0       
#define RGLOP 01	// global rule
#define RLOCOOP 02	// local rule


#define RIMPORTBIT 04  // was it imported 

#define RSTANDOP 010   // built in rule
#define RVAR 020       // variable
#define RSTATMSK 07

//#define NRESW 50        // size of table of res. words in ecolog
#define DEFAULTSYM -32000 // symbol for default action if parser's tables

/* ------------ some implementation constants -------------------*/
#define OUTS stderr       // 
#define OUTPUTS "stderr"  // outputs of executable
#define MAXNOFIMPORTS 200 // max. number of imported modules
#define MRWTSIZE 300      // number of res.word of ELAN (to accurate when def.)
#define MAXINCLDEEP 30    // max. deep of import graph
#define MAXNOFMAC 50	  // max. number of macro vars.
#define MAXNESTMAC 10	  // max. nested macro
#define MAXUNIFFSYM MAXNFSYM  // max. number of symbols in comm. with UNIF
#define MAXDEEPCONSTEXP 10 // max. deep of constant expression in prepro.
#define MAXNOFTRN 2000	  // max. number of trans. rule names
#define MAXNOFAXIOMS 10	  // max. number of parts in specification
#define MAXNOFSTRAT 500	  // max. number of strategies
#define MAXINCLSTRAT 30	  // max. number of rep./iterate nesting
#define MAXNOFSUBPROCESS 30 // max. number of subprocessus
#define MAXNNONT 500    // maximal number of nonterminals
//#define MAXTERMDEEP 10000     // max deep  of terms
#define MAXTERMDEEP 20000     // max deep  of terms
#define MAXNOFARITIES 100    // maximal number of different arities

//Marian's version 
#define VARSPRI 50      // variable priority (in term grammar)
//#define VARSPRI 4010      // variable priority (in term grammar)
//MARIAN's version
//#define STANDPRI 10000 // priority of standards (number & identifiers)
#define STANDPRI 4000       // priority of standards (number & identifiers)

#define STARTTYPE (NTYPES+1)  // codes for built in types 
#define RIGHTSTYPE (NTYPES+2)
#define STRATTYPE (NTYPES+3)



#define STRLEN   1000     // max. length of any string 

// CONSTANTES POUR ELAN ET LIBEARLEY

#define EXIT_FAILURE 1
#define ERRORIM 1         // return values for semact 
#define ACCEPTIM 2        // accept immediately
#define NORMCONT 3        // normal cont.
#define HANDERRORIM 4     // error, but handled by semact.

		// values of priority record in term grammar
#define RNOPRIOR 0
//#define RAXPREDPRIOR1 100	// priority of '&' in axpredicateset rule
//#define RAXPREDPRIOR2 50	// priority of (axpredicate)axpredicateset rule
#define RPRIORITYMSK 007777
#define RLEFTASSOC   010000	// left assoc.
#define RRIGHTASSOC  020000   	// right assoc.
#define RASSOCMSK    070000
#define RBINSTR      040000     // built in strategy

#define IDLEN 50          // max. length of dist. character in identifier
#define MAXNOFIDENT 3000  // max number of identifier used in all prog.
#define MLENGRRULE 200    // max. size of term grammar rule

//#define MAXRWINTERM 600 // max. number of res. word for term grammar
#define MAXNOFVAR 49      // max. number of variables in one rule (must be the 
                          //                   same as in acmatching procedure
#define MAXNFSYM 2000      // max number of functionals symbols

#define MAXLENNTERM 50000 // max. number of lexems in term 
//#define MAXLENNTERM 10000 // max. number of lexems in term 
extern  int MAXLENNTERMv; 
#define NTYPES 500        // max. number of types in ELAN
#define CHUNKSIZE 10    // size of chunk for lbuffer

#define FSYMCODESBEG 300	  // begin of codes of symbols
			// codes less then FSYMCODESBEG are for built-ins
#define RULECONSTRULE  (MAXNFSYM+1)   // sem. actions for standard rules
#define RIGHTSRULE     (MAXNFSYM+2)
#define STRATCONSTRULE (MAXNFSYM+3)
#define RULECONSTRULE1 (MAXNFSYM+4)
#define FSYMTABSIZE    (RULECONSTRULE1+1)

#define NFILE 0		// type of ichstream. internal ichstream constants.
#define PIPE 1
#define NNONTERMINALS (NTYPES+4) // max. number of nonterminals in the 
                          // term grammar 

// STRUCTURES ET CLASSES

struct chlist{
  char *s;
  struct chlist*next;
};

//#include "ochstream.h"

class ichstream
 {
 private:
  int fchar;            // first char in ichstream
  int type;		// type of stream 'file/pipe'
  FILE *file;           // rest of ichstream
  char *fname;          // name of file
  int line,pos;         // actual line and position in file
  int read_block;       // read blocks or not
  void commonopen(char *name,int type);
 public:

#define EOFICHSTR EOF

  ichstream(const char *name);
  ichstream(FILE *file,const char *name);
  ichstream(FILE *file,const char *name, int block);
  ichstream(const char *command,const char *name);
  
  ~ichstream();


  void fuch(int &c);    // future character (which will be returned by ich())
                        // doesn't change the ichstream
  void ich(int &c);     // next character from strem
                        // remove the character from ichstrem
  void blankskip();     // skip the blanks in the stream
  int isready();
  int actline();
  int actpos();
  void setposition(int actl,int actp);
  char *actname();
  char *readstring();
};

class ochstream
 {	// I don't know why the ostream doesn't work
			// so, i have made this module
private:
  FILE *file;
public:
  ochstream(const char *name);
  ochstream(FILE *);
  ~ochstream();

  void flush();
  ochstream& operator << (char );
  ochstream& operator << (int  );
  ochstream& operator << (unsigned  );
  ochstream& operator << (unsigned long );
  ochstream& operator << (float  );
  ochstream& operator << (double  );
  ochstream& operator << (char *);
  ochstream& operator << (const char *);
  ochstream& operator << (void *);
};


class lexem
 {		// !!! bodies of functions are in 'lstream.c'
 private:
  int lex;              // value of lexem

 public:

			// 00 .. 0 reserved for constants
                        // -1 .. -255 lexem for characters
#define NOLEXEM -256     // no lexem, label for end of string of lexems
#define BLANKLEXEM -257	 // blank lexem, with special use in mlstream
                         // and as epsilon rule for parser
#define IDENT -259       // just an identifier
#define BOFIDENT -260       // BOFIDENT ... JUSTBUMBER    identifiers
#define MAXNOFSTRING       10000
#define STRING     (BOFIDENT-MAXNOFIDENT)
#define BOFSTRING  (BOFIDENT-MAXNOFIDENT-1)
#define JUSTNUMBER (BOFSTRING-MAXNOFSTRING)
//#define JUSTNUMBER (BOFIDENT-MAXNOFIDENT)
                            // just a number
#define BOFTYPES (JUSTNUMBER-1)
                            // BOFTYPES ... user's sortes

  lexem();
  lexem(lexem &);

  void crendofstreamlex();

  void cridlex(int n);       // position in tabofident
  void crtypelex(int );
  int typeval();

  void cridlex(const char *ide);
  void craidlex(const char *ide);
  void cridlex();
  void crnumlex(int n);
  
  void crnumlex();
  void crcharlex(int c);
  void crblanklex();
  void dump();					// body in the 'lstream.c'

  int terminal();    // true if terminal symbol
  int nonterminal(); // true if nonterminal symbol
  int isendofstream(); // true if end of stream
  int isnotendofstream();
  int isblankk();
  int isident(); // true if identifier (both ident and res. word)
  int isrealid();   // true if it is rw (i.e. defined identifier)
  int idval();             // index of identifier in tabofident
  int isnum();   // true if number
  int isrealnum();
  int numval();  // value of number
  int ischar();   // true if character
  int charval();  // value of character
  int isstring();
  int isrealstring();
  char *stringval();
  void crstringlex();
  void crstringlex(char *s);
  int notendoftermlex(); // true if not a lexem for end of term
  const char *alfsy(); // body is in the 'lstream.c'
  const char *erralfsy(); // body is in the 'lstream.c'
  void operator =(lexem );
  int operator ==(char );
  int operator !=(char );
  int operator ==(lexem );
  int operator !=(lexem );
  int operator >(lexem );     // an ordering on lexems to can have ordered lists

//                  some functions to facilitate parsing 
  void maccsymtolex(int );// conversion from int used in parsed 
                          // automat to lexem
  int notEqualMaccsym(int s); // compare with a parser symbol
  int Adump(ochstream &af);
};

extern stringtab tabofident;     // body is generated with
                                 //reserved word with parser generation
                                 // in file tabofident.c
extern stringtab atabofident;    // pour REFTERM



inline lexem::lexem() {}
inline lexem::lexem(lexem &l) {lex = l.lex;}
inline void lexem::operator =(lexem l) {lex=l.lex; }
inline void lexem::crcharlex(int c){ lex = -c;}

inline int lexem::terminal() {return (lex>BOFTYPES);}
inline int lexem::nonterminal() {return (lex<=BOFTYPES);}
inline int lexem::isendofstream() {return(lex==NOLEXEM);}
inline int lexem::isnotendofstream() {return(lex!=NOLEXEM);}
inline int lexem::isblankk() {return(lex==BLANKLEXEM);}
inline int lexem::isident() {return((lex<=BOFIDENT && lex>BOFIDENT-MAXNOFIDENT)||lex==IDENT);}
inline int lexem::isrealid() {return(lex<=BOFIDENT && lex > BOFIDENT-MAXNOFIDENT);}
//inline lexem::isident() {return((lex<=BOFIDENT && lex>BOFTYPES)||lex==IDENT);}
//inline lexem::isrealid() {return(lex<=BOFIDENT && lex > BOFTYPES);}
extern char   *stringconstants[];
extern int    stringconstantsi;
inline int lexem::isstring()  { return((lex<=BOFSTRING && lex>BOFSTRING-MAXNOFSTRING ) || lex == STRING); }
inline int lexem::isrealstring()  { return(lex<=BOFSTRING && lex>BOFSTRING-MAXNOFSTRING); }
inline void lexem::crstringlex() { lex = STRING; }
inline int lexem::idval() {return(BOFIDENT-lex);}
inline int lexem::isnum() {return(lex>=0 || lex == JUSTNUMBER);}
inline int lexem::isrealnum() {return(lex>=0);}
inline int lexem::numval() {return(lex);}
inline int lexem::ischar() {return(lex<0 && lex >= -255);}
inline int lexem::charval() {return(-lex);}

inline int lexem::notendoftermlex() {return (lex!=NOLEXEM);}
inline void lexem::cridlex(const char *ide){ lex = BOFIDENT-tabofident.addstr(ide);}
inline void lexem::craidlex(const char *ide){ lex = BOFIDENT-atabofident.addstr(ide);}
inline void lexem::crnumlex(int n){ lex = n;}
inline void lexem::cridlex(){ lex = IDENT;}
inline void lexem::crnumlex(){ lex = JUSTNUMBER;}


class lbuffer
 {
private:
  struct lbufchunk {
    lexem *l;                   // l[CHUNKSIZE];
    int b,e;			// begin and end index of valid lexems
    struct lbufchunk *next;
  } firstch, *lastch;

public:
  lbuffer();
  ~lbuffer();
  void put(lexem);
  void clear();
  int isempty();
  void put(char );
  void get(lexem &);
  void flush(int i,void (*f)(lexem)); // apply f on each lexem and clear buff. 
  void applyflush(int i,void (*f)(lexem)); // just apply f on each lexem
		// argument i in both cases is not used !!!!!!!!!!!!!!!!!!!!!!
		// it was added, because of a bug in the gnu C++ compiler !!!!
  void copy(lbuffer &into);
  void append(lbuffer &appendto);
  void dump();
};


#define OWNAME 00		// flags for lstream;
#define OWSTREAM 01		// bit 0 :type of stream file/pipe
#define OWTYPEMSK 01
#define UNDERID 00		// bit 1 :uderscore is part of
#define NONUNDERID 02           //        identifier yes/no
#define IDMSK 02

//#include "lstream.h"

class lstream
 {    // stream of lexems (input for parser)
 private:  
  lexem flex;
  ichstream *istr;
  int type;
  lbuffer *lastinline;
  int block;
 public:
  lstream(const char *name);
  lstream(const char *name,int idtype);
  lstream(ichstream *file);
  lstream(ichstream *file,char c);
  lstream(ichstream *file,char c, int block);
  lstream(const char *command,const char *name);
  virtual ~lstream();
  virtual void fulex(lexem &l);
  virtual void ilex(lexem &l);
  void oerr();
  void oerr(const char *);
  void owarn();
  void owarn(const char *);
  int isready();
  int isblock();
  void oerr(const char *,const char * ...);
  void owarn(const char *,const char * ...);
  lbuffer *getlastinlineAndinit();
  virtual void beforemess(lexem s);
  void setposition(int actl,int actp);
  int actline();
  int actpos();
				// follows functions active only in macro p.
  virtual void pilex(lexem &) {};
  virtual void addmac(int ,lbuffer &) {};
/*
  virtual void addsimiter(int , lbuffer &) {};
  virtual void addincriter(int , int , int , lbuffer &) {};
  virtual void addforeach(lbuffer &) {};
  virtual void addmac(int ,lbuffer &) {};
  virtual void deletemac(int ) {};
  virtual void prepend(lbuffer &) {};
*/
};   


#define SIMITERACTION 0
#define INCRITERACTION 1
#define NEXTMATCHACTION 2
#define IDITERACTION 3
#define SIMPLECOPYACTION 4
#define MAXPREPEND 10	// max. number of prepends, internall constant
			// can be fully calculate later


class term;

class mlstream: public lstream {
private:
  lexem mflex;				// first lexem in mlstream
  mitab *mactab;			// table of names of macros
  lbuffer macb[MAXNOFMAC];		// table of bodies of macros
  struct macactelem {
    lbuffer currentb;
    lexem lastlexem;
    int whichaction;
    union {
      struct ssimiter {		// X~N  or body of macro
        int i;
      } simiter;
      struct sincriter {	// {X}_I=1...N
        int b,e;
        int mname;
      } incriter;
      struct snextmatch {	// FOR EACH X:t SUCH THAT X:=T : {...X...}
        struct wheress *lastws;
        struct wherelist *whl;
	term *substarray;
        int *varnames;
        int varnum;
      } nextmatch;
      struct siditer {		// X_1,...,X_N
        int b,e,ch;
      } iditer;
    } actdata;
    lbuffer actionbuf;
  } macactstack[MAXNESTMAC];		// stack of actions to prepend
  int macactstacki;

/*
	The stream of lexems in mlstream is as follows:
'mflex' is the first lexem to output; then follows lexems in 
'macactstack[macactstacki-1].currentb', then lexems coded in the action
'macactstack[macactstacki-1].actionbuf', then lexem
'macactstack[macactstacki-1].lastlexem', then
...,
'macactstack[0].currentb' and 
'macactstack[0].actionbuf', and
'macactstack[0].lastlexem',
and then the rest of lexems from superclass lstream.
*/

  void addactionprelim();
  int buffilex(lexem &l);
  void collpilex(lexem &l);  
//  void milex(lexem &l);  
  void expect(char );
  void expectident();
  void insteaderr();
  int constexpexp();
  int getmflex(lexem &l);
  void fillmacbuf(lbuffer &tmpb);
  int macroexp();
  void collident(); 
  void addvars(const char *tn);
  void addvarid(lexem l);
  void addsimplecopy(lbuffer &b);
  void deleteforeachmacs(int *names, int n);
  void addforeachmacs(int *names, int n, term *inst);
  void parsetype(const char *&tn);
public:
  mlstream(char *name);
  virtual ~mlstream();
  virtual void beforemess(lexem s);
  virtual void fulex(lexem &l);
  virtual void ilex(lexem &l);  
  virtual void pilex(lexem &l);
  void addmac(int name,lbuffer &body);
  void deletemac(int name);
  void addsimiter(int n, lbuffer &b);
  void addiditer(int b, int e, int ch, lbuffer &buf);
  void addincriter(int mn, int b, int e, lbuffer &buf);
  void addforeach(lbuffer &buf,struct wherelist *wh,int bvarnamesi);
  void actionstackdump();
  int macrotest();
};


struct sgrammrule {
  int priority;              // priority of rule 
  int rulenumber;            // number of this rule
  lexem leftside;            // nonterminal on the left side of rule  
  lexem *rside;              // array of lexems; right side of rule
                             // finished by 'nolexem'.
  int semantic;              // index of semantic's action
  int defstrat;
  int topglobgram;           // 1 in topgrammar, 2 in globgram, 3 in both
  int fsymcode;              // index to fsymtab 
};

#define INTOPGRAM     1
#define INGLOBGRAM     2

struct grammrulelist {
  struct sgrammrule *r;
  int infos;                 // infos, such that local/global op
  struct grammrulelist *next;
};

extern void dumpgrrule(struct sgrammrule *);	// bodies in 'grammar.c'

struct sitlist {         	// list of situations
  struct sgrammrule *rule;     // rule from term grammar
  int pos;                     // position of '.' in the rule
  int i;                       // int from earley's situation
  struct sitlist *next;        // next situation 
  struct sitlist *nextwss;     // next situation with the same symbol after '.'
};

struct psymlist {          // lists of situations par symbols
  lexem symbol;
  struct sitlist *sit;
  struct psymlist *next;
};

struct ssits {             
  lexem actlex;
  struct sitlist *sits,**aoflastsits;
  struct psymlist *sitparsym;
};

struct actchooselist {             
  struct sitlist *sits;
  struct actchooselist *next;
};

struct earleystables {
  struct ssits *esitset;  	// Earley's sets for input lstream
  int esitseti;
  struct actchooselist *echooselist,**eactchoose;
  int eambig_disabled;
  lexem estartsymbol;
};

class grammar
 {
 private:
  // ELAN + LIBEARLEY
  struct grammrulelist *nontt[NNONTERMINALS]; 
  struct grammrulelist **alastnontt[NNONTERMINALS]; 
                              // array of rules for each nonterminal
  lexem buff[MLENGRRULE];     // buffer used for building the rule
  int buffi,ntbuffi;

  void addsymbol_(lexem *ebuff,int *ebuffi,lexem &);
  struct sgrammrule *addrule_(lexem *ebuff,int *ebuffi,lexem &leftside, int priority, int infos, int num);
  struct sgrammrule * addrul(lexem &leftside,lexem *body,int priority, int infos ,int num);
  void addnont_(lexem *ebuff,int *ebuffi,lexem &);

  void earleyreturn(struct earleystables *&table);
  void earleyarrival(struct earleystables *&table);
  void inisitset();
  int addtosit(struct sgrammrule *rule, int position, int i);
  void completesit();
  void oearleyerr(lstream &f,int sit,int (*g)(lexem));
  void solveconflicts(struct sitlist *&sis);
  void solveconflict2(struct sitlist *&sis, struct sgrammrule *r);
  int firsttestcond(struct sitlist *fins);
  int  earleyaddnewsits(lexem l);
  int testcond(struct sitlist *fins,struct sitlist *acts,int k, lexem xk);
  void earleysecprec(lstream *f,struct sitlist *,int);
  // ELAN
  void dumpsitset( int i);
  void pretydumpsitset(stringtab *types);
  void pretysitldump(stringtab *types,struct sitlist *);
  int rbodyeq(lexem *b);
  int hardcompatible(lexem *b);
  void markusedtype(int );
  void lextoelex(lexem, lexem &);     // conversion of lexem for earley alg.

 public:
  // ELAN + LIBEARLEY
  grammar();
  ~grammar();

  void  addsymbol(lexem &);
  struct sgrammrule * addrule(lexem &leftside,int priority, int infos ,int num);
  void  addnont(lexem &);  
  inline struct grammrulelist *get_rule_list(int i) { return nontt[i]; }

  void earleyPrettyDump(ochstream &);
  void earleyDump(ochstream &);

  int earleycall(lstream *f,lexem startsym,int (*isendofstream)(lexem));
  int earley(lstream &f,lexem startsym,struct earleystables *&tables,int (*)(lexem ));
  int earleysecondpass(lstream *f,int ambigdis,struct earleystables *&table);
  void earleyfree(struct earleystables *&table);
  void oambigwarning(struct sitlist *si);

  void combine();

  struct sgrammrule * addvarrule(lexem &leftside,lexem &l,int priority, int infos ,int num);
  struct sgrammrule * adddollarvarrule(lexem &leftside,lexem &l,int priority, int infos ,int num);
  void preprocdeleterule(struct sgrammrule *);
  void deleterule(struct sgrammrule *);
  int addalias(struct sgrammrule *, lexem &leftside);
  int addhardalias(struct sgrammrule *, lexem &leftside);
//  void setstartsym(lexem &l);
//  void addrw(lexem l);             // add identifier to rwtab
//  void removerw(lexem l);          // identifiers have to be removed 
                                   // in reversed order than added
  void genglobgr(ochstream &,char *modname,stringtab*t);
  void freetopl();
  void addgrammar(grammar &, int which, int as_which);
  void gr_rule_mapp(int nothing, void (*)(struct sgrammrule *));
  void dump();
  void rmark(int set, int flag);
  void Adump(ochstream &af, int flag);
  void pretydump(stringtab *typt);
  void  anycode_ops(ochstream &anymod);
  void  anycode_rules(ochstream &anymod);
  int  anysymbol_exists();
  void  any_code(char *anymodstr, char *anymodfname, const char *name);
  void  symbapplcode_ops(ochstream &symbapplmod);
  void  symbapplcode_rules(ochstream &symbapplmod);
  int  symbappl_exists();
  void  symbappl_code(char *symbapplmodstr, char *symbapplmodfname, const char *name);
  void  lookinlinecode(int x, int y);
  int   lookbuiltincode(int x, int y, lexem le, int *infos);
  void   add_apply_code(int x, int y);
  void write(ochstream &,int winfos,
             stringtab *typt,const char *before ,const char *after);

};

// ELAN + LIBEARLEY
extern char *mstrdup(const char *s);         // body in 'misc.c'

extern void *allo(unsigned n, unsigned s);// body in 'mallo.c'
extern void  fre(void *p);

extern char *addsuffix (const char *,const char *); // body in 'misc.c'
extern char *addsuffixs (const char * ... ); // body in 'misc.c'
extern void writegrrule(ochstream &,struct sgrammrule *,
                        stringtab*types);

extern int esemact (lstream *f,int n,lexem l,lexem type);//semaction for building term

[[noreturn]] extern void failexit(); // body in specials.c and module.c
[[noreturn]] extern void interr();   // body in commondefs.c
extern ochstream stout,sterr,graphout,dumpout,traceout;
extern const char *elanlib,*perslib;   // bodies in commondefs.c

extern int  quote;
extern char elanlibqnq[];
extern char elanlibcommon[];
extern char elanlibstrat[];
extern char elanlibref[];
// ......
extern int warnings,trace,dump,quiet,batch,statis;
extern int in_runtime;
extern int adump;        // export to aterm form
extern int aimport;      // import from  aterm form
extern int aterm_parse;
extern int reduceimport;      // import from  reduce

inline int ichstream::actline() {return line;}
inline int ichstream::actpos() {return pos;}
inline char * ichstream::actname() {return fname;}

inline int lexem::typeval() {return(BOFTYPES-lex);}
inline void lexem::cridlex(int n){ lex = BOFIDENT-n;}
inline void lexem::crendofstreamlex() {  lex = NOLEXEM;}
inline void lexem::crtypelex(int n) {lex= BOFTYPES-n;}
inline int lexem::operator ==(lexem le) {return (lex==le.lex);}
inline void lexem::crblanklex() { lex = BLANKLEXEM;}
inline int lexem::operator !=(lexem le) {return (lex!=le.lex);}
inline int lexem::operator >(lexem le) {return (lex>le.lex);}
inline void lexem::maccsymtolex(int ps){lex = ps;} 
inline int lexem::notEqualMaccsym(int s) {return(lex != s);}
inline int lexem::operator ==(char ch) {return (lex== -ch);}
inline int lexem::operator !=(char ch) {return (lex!= -ch);}


#define PLACE(ss,sy) {while((*ss!=NULL)&&(sy>(*ss)->symbol))ss= &((*ss)->next);}
#define FOUND(p,s) {while (p!=NULL && s>p->symbol) p= p->next;}
#define FREELIST(p,pp) while(p!=NULL){pp= p;p= p->next;CFRE(pp);}
#define APPEND(fst,snd,pp) {pp= &(fst);while(*pp!=NULL)pp= &((*pp)->next);*pp=snd;}

#define strfree(s) { CFRE(s);}

#define NNEW(p,t) {p= (new t); if(p==NULL){sterr << "\n\n[new] sorry, no memory\n"; failexit();}}
#define DELETE1(p)  DELETE2(p)

#define DELETE2(p) { delete p; }


#define CFRE(p) { fre(p);}

#define AALLOSS(p,n,t) {p= (t*) allo(n,sizeof(t)); /*allotest1(p);*/}
#define AALLOS(p,t) AALLOSS(p,1,t)

// ELAN 
inline void lbuffer::put(char ch) {lexem l; l.crcharlex(ch); put(l);}
//inline void grammar::setstartsym(lexem &l) {startsym=l;}

inline int lstream::actline() { return istr->actline();}
inline int lstream::actpos()  { return istr->actpos();}


extern int semact(int, lexem ,lstream *);  //semaction for compiling module

extern void odsek(ochstream &,int n);

extern void freeWhereBacktrack(int varn,struct wheress * &lastws,term *substarray);
extern int isWhereBacktrackNextSol(struct wherelist *wheres,term *substarray,
                   struct wheress * &lastws, int trace, int varn);
extern int isTseqBacktrackNextSol(term &res, struct tseq *rhs, term *substarray,
                   struct wheress * &lastws, int notbatch, int nback,
	           struct wherelist *&lastwheres, struct term *&lastresult,
	           int varn);

extern int traceind,tracelevel;
extern mitab mrwt;               // res. words for modules table containes 
                                 // indexes to tabofident ans is generated 
                                 // into file tabofident.cc
extern mitab amrwt;              // pour REFTERM


#define indent() odsek(traceout,traceind*2)

#define CHARSYM(s,rwt,uit,tyt) ( ((s).terminal())?((s).alfsy(rwt,uit)):(tyt->ide(s))) 

extern void allotest(void *);  // body in mallotest.c  !!! in comment !!!!!!!
extern void freetest(void *);  // body in mallotest.c  !!! in comment !!!!!!!

#define ISSETBIT(bitarr,s) ((bitarr[s/NBITS]>>(s%NBITS))&1)
#define SETBIT(bitarr,s) {bitarr[s/NBITS]|= 1<<(s%NBITS);}
#define NULLBIT(bitarr,s) {bitarr[s/NBITS]&= ~(1<<(s%NBITS));}


#endif
