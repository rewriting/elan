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

// Grammars and the Earley parser (grammar.cc, earley.cc, esemact.cc) (split
// from commondefs.h).

#ifndef __grammar_h
#define __grammar_h

#include "constants.h"
#include "stringtab.h"
#include "streams.h"
#include "lexem.h"
#include "lstream.h"

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

extern void writegrrule(ochstream &,struct sgrammrule *,
                        stringtab*types);

extern int esemact (lstream *f,int n,lexem l,lexem type);//semaction for building term

#endif
