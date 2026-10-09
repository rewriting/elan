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

	$Source: /CVS/aircube/elan3/src/elan-compiler/earley/commondefs.h,v $
	$Revision: 1.1.1.1 $
	$Date: 2001/11/29 08:28:44 $
	$Author: pem $
*/

#ifndef __commondefs_h
#define __commondefs_h


#include "stringtab.h"
#include <stdio.h>
#include <stdlib.h>
#include "mallo.h"

// FIN DES INCLUDES

// DEFINITION DES CONSTANTES POUR ELAN


#define STRLEN   1000     // max. length of any string 

// CONSTANTES POUR ELAN ET LIBEARLEY

#define RSTANDOP 010   // built in rule
#define STANDPRI 4000       // priority of standards (number & identifiers)

// the two previous define are new in this section

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

#define FSYMCODESBEG 200	  // begin of codes of symbols
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

  ichstream(char *name);
  ichstream(FILE *file,const char *name);
  ichstream(FILE *file,const char *name, int block);
  ichstream(char *command,char *name);
  
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
};

class ochstream
 {	// I don't know why the ostream doesn't work
			// so, i have made this module
private:
  FILE *file;
public:
  ochstream(char *name);
  ochstream(FILE *);
  ~ochstream();

  void flush();
  ochstream& operator << (char );
  ochstream& operator << (int  );
  ochstream& operator << (unsigned  );
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
  void craidlex(char *ide);
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
extern stringtab atabofident;    // pour ATERM

extern stringtab typet;


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
inline void lexem::craidlex(char *ide){ lex = BOFIDENT-atabofident.addstr(ide);}
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
  lstream(char *name);
  lstream(char *name,int idtype);
  lstream(ichstream *file);
  lstream(ichstream *file,char c);
  lstream(ichstream *file,char c, int block);
  lstream(char *command,char *name);
  virtual ~lstream();
  virtual void fulex(lexem &l);
  virtual void ilex(lexem &l);
  void oerr();
  void oerr(const char *);
  void owarn();
  void owarn(const char *);
  int isready();
  int isblock();
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


};

// ELAN + LIBEARLEY
extern char *mstrdup(const char *s);         // body in 'misc.c'

extern void *allo(unsigned n, unsigned s);// body in 'mallo.c'
extern void  fre(void *p);

extern char *addsuffix (char *,char *); // body in 'misc.c'
extern char *addsuffixs (char * ... ); // body in 'misc.c'
extern void writegrrule(ochstream &,struct sgrammrule *,
                        stringtab*types);

extern int esemact (lstream *f,int n,lexem l,lexem type);//semaction for building term

extern void failexit();          // body in specials.c and module.c
extern void interr();            // body in commondefs.c
extern ochstream stout,sterr,graphout,dumpout,traceout;
extern char *elanlib,*perslib;   // bodies in commondefs.c

extern int  quote;
extern char elanlibqnq[];
extern char elanlibcommon[];
extern char elanlibstrat[];
// ......
extern int warnings,trace,dump,quiet,batch,statis;
extern int in_runtime;
extern int adump;        // export to aterm form
extern int aimport;      // import from  aterm form
extern int aterm_parse;

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

#endif
