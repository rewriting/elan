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

#define ACTIMP2POS {\
     typecheck(f,actimp->s);\
     pos = typet.addstr(actimp->s); \
     /**/ CFRE(actimp->s); CFRE(actimp); actimp=NULL;\
}
 
#ifndef __strategy_h
#define __strategy_h

#include <deque>
#include <vector>

struct nvlist {
  int i;
  lexem ruletype;
  struct nvlist *next;
};                          // a stack of var declarations (for the case of nested declarations using 
						//				nested "rules" construction)

extern int anysi;
extern int anys[];
extern int symbappli;
extern struct ilist *symbappl[];


#define AUXRULES 2
//  f : b;
//    b => f
//    <b> => f  -- aux 1
//    R_b => f  -- aux 2
//  f : (b) b;
//    b => f(b);
//    <b> => f(<b>) -- aux 1
//    R_(b)b => f    -- aux 2
//  s : <b>;
//    <b> => s;
//    R_<b> => s    -- aux 2;
//  s : (b) <b>;
//    <b> => s(<b>)
//    R(b)<b> => s;  -- aux 2;
//---------------------------------
//#define aux_rule(code,i)      (FSYMTABSIZE+AUXRULES*code+i-1)
//#define is_aux_rule(code)     (code >= FSYMTABSIZE && code < MAXCODE)
//#define which_aux_rule(code)  (((code-FSYMTABSIZE) % AUXRULES)+1)
//#define aux_fsym(code)        ((code-FSYMTABSIZE) / AUXRULES)

#define NOPROFIL              -9999           // bottom of the stack
#define MAXCODE               10000           // >= MAXCODE - codes of strats
#define START_RULE            "START_RULE"    // MAIN STRATEGY & RULE
#define START_STRATEGY        "START_STRATEGY"// MAIN STRATEGY & RULE
#define STRATEGY              "Strat"         // head of nonterminal
#define STRAT_MODNAME1        "strat"         // type preserving strategies
#define STRAT_MODNAME2        "tcstrat"       // type changing strategies
#define STRAT_MODNAME3        "strconc"       // for TRANSITIVE CLOSURE 
#define STRAT_SIGNATURE       "strsig"        // strsig.eln
//#define STRAT_MODNAME_AUX     "straux"
#define EVALSTR               "eval"          // name of ealuation strategy
#define ANY_MOD               "any"
#define SYMBOL_MODNAME        "symbol"        // symbol module
#define SYMBOL                 "Symbol"       // symbol prefix
#define DELTA_PRIOR            1000           // hack to eliminate
                                              // ambiguities because of
                                              // X => Strat[X,X] rule

#define ISSYMBOLMOD(x)  (!strcmp(SYMBOL_MODNAME,x))
#define ISANYMOD(x)     (!strcmp(ANY_MOD,x))
#define ISSTRAT1MOD(x)  (!strcmp(STRAT_MODNAME1,x))
#define ISSTRAT2MOD(x)  (!strcmp(STRAT_MODNAME2,x))
#define ISSTRATSIG(x)   (!strcmp(STRAT_SIGNATURE,x))
#define ISSTRATTYPE(x)  (!strcmp(STRATEGY,x))
//-----------------------------------

extern lexem Sif,Swhere,Send,Sstart;
extern lexem Scase, Sotherwise, Sthen, Sswitch, Schoose, Stry;

extern lexem Sif,Swhere,Send,Sstart;     // some usefull reserved words

extern int rulen,rulepri;                       // the number and the priority of the currently parsed (CP) rule 
extern int rinfos,finfos2;                      // syntactic priority and associativity of CP symbol
extern int finfos;                              // is the CP symbol AC ?
extern int actarity,actprofis,actcode;          // arity of the CP symbol; CP profil; and current code number
extern lexem actleftside;                       // codomain of the CP profile
extern lexem actruletype;                       // type of terms in the CP rul
extern void fsymrule_add(struct sgrammrule *r1, struct sgrammrule *r2);
extern int fsymrule_exists(struct sgrammrule *r1, struct sgrammrule *r2);

extern lexem actvartype,actvartype1;            // CP variables type
extern lexem actwheretype;                      // type of the CP where affectation
extern int actwherevar,actwhstrategy,actstratindex;  // CP variable of a where; its strategy; and index of the strategy 
extern int wasdefinedas;                        // only for the COMPILER, is the symbol defined as an inlined function?
extern struct sgrammrule *actalr,*actalr1;      // CP functional symbol grammar rule

extern struct nvlist *nestedvartabi;
extern struct sgrammrule *vartab[MAXNOFVAR];    // grammar rules for variables
extern transrule *acttrrulelab;                 // the CP RW rule
extern lexem actvtab[MAXNOFVAR];                // names of variables
extern int actvarrename[MAXNOFVAR];             // table used during the renaming of variables inside RW-rules
extern int actvarnum;                           // the real number of variables inside an RW-rule
extern int selectorn;
extern int numb_selectors, sel_poss;
extern struct selector selectors[];
extern int actvtabi,vartabi;                    // numbers of variables (two values differ during parsing variable declarations
extern int big;						//			 for the same type)
extern term lside,rside,condition;              // left hand side, right hand side and the condition of the CP RW rule
// One frame per module being read: an import reads the imported module one
// level deeper (stacki+1). The frames of a level are reused by the next
// module read at that level. The stack grows on demand (imports of any
// depth; MAXINCLDEEP = 30 until S3b); a std::deque keeps the existing frames
// in place when it grows, but code holds indices (stacki), not references.
struct ModuleFrame {
  const char *modname = nullptr;                // name of the CP module
  char *filemodname = nullptr;                  // file where the CP module is placed
  grammar gr;                                   // grammar of the CP module
  int importrinfos = 0;                         // kind (GLOBAL/LOCAL) of the CP import of the module
};
extern std::deque<ModuleFrame> modframes;       // modframes[0..stacki]: the modules being read
extern int stacki;                              // index to modframes
extern void grow_modframes(int level);          // makes modframes[level] exist
extern std::vector<strategy *> strstack;        // stack used while parsing nested strategies (REPEAT,ITERATE)
extern int strstacki;                         // index to strstack
extern std::vector<struct strlist *> strlstack; // stack used while parsing nested list of strategies (DONT CARE/KNOW CHOOSE)
extern int strlstacki;                        // index to strlstack
// the two stacks above grow on demand (MAXINCLSTRAT = 30 until S3b)
extern void grow_strstack(int n);               // makes strstack[0..n-1] exist
extern void grow_strlstack(int n);              // makes strlstack[0..n-1] exist
extern term ter1, ter2;                         // a temporary term variable
extern const char *acttrrulename;                     // the name of the CP RW rule
extern transrule *acttrrule;                    // the CP RW rule
extern strategy *actstrategy;                   // the CP strategy
extern char *actargmodname;                     // the name of the CP module
extern int symbolcode;                          // the value of a character defined by its ASCII code
extern struct chlist *actarglist,*actimp;	// used to pass the arg. also
extern int strnam[];                          

// int mainstrategy;
// int & mainnewmod;

extern int pretydumpsitset(lstream *f,stringtab *types);
extern int acsymbolinleftside;		        // body in esemact.c   
                    //| is there an AC symbol in the left hand side of CP rule?
extern  int strategywasapplied;
extern void esemactinit();		             // body in esemact.c   | init term construction semantic actions
extern int pretydumpsitset(lstream *f,stringtab *types); // pretty dump of earley's situations table




extern std::vector<int> profistck;           // stack of profiles (grows on demand)
extern int profistcki;             // top-pointer to the profistck
extern int in_strat_def;          // true iff parsing str body
extern int in_strat_module;           // true if parse strat.eln
extern int opdefinition;                  // parsing op or strategy def
extern lexem actlvtab[MAXNOFVAR];             // for variables in labels
extern int actlvtabi;                     // pointer to actlvtab
extern lexem actruletype_l;                   // type of terms in the CP rule
extern int actruletypeindex;              //
extern int actruletypeindex_l;            //
extern term rlabel;                           // term corresponding to a label
extern struct grammrulelist *listofrules;// list of rules added in strategy body
extern int profi_level;                   // level of nesting of profiles
extern int profi_lev;
extern char strategy_type[];             // name of the first type in profile
extern int complex_label;                    // rule label is != ident
extern int pos_l;                         // index of type X, when parsing <X->Y>
extern term dstr_rs;                     // right-hand side of a dstr rule

//#define make_new_nonts(x,y,z,u) { sprintf(x,"%s[%s,%s]",y,z,u); }
#define make_new_nonts(x,y,z,u) { snprintf(x,sizeof(x),"<%s->%s>",z,u); }

extern void add_to_fsymtab(int actarityy,struct sgrammrule *gr, int defstrat);
extern int add_strat_nont(int pos1,int pos2);

struct ilist {
    int i;
    struct ilist *next; };

#define PROFITABSIZE  1000
class profitab
 {
  //private:
  public:
    struct ilist *tab[PROFITABSIZE]; 
    int profinum; 
  public:
    int add_profil(struct ilist **ilistptr, int *found);
    void profil2str(struct ilist *ilistptr, char *str);
    int equal(struct ilist *ilistptr1, struct ilist *ilistptr2); 
};

extern class profitab profit;                        // table of profiles

extern  int spairi;
struct  Spair { int from; int to; int stratsort; };  // cannot be move to inlinecodesi
extern std::vector<struct Spair> spair;          // pairs [x,y] if strat[x,y] (grows on demand)

struct selector {
         int type;
         int possition;
         int name;
       };

extern  int inlinecodesi;

struct INLINES {
           int from;  // means x
           int to;    // means y
//           int stratsort;                        // index of the sort <x->y>
           int applycode;                        // []
           int inlin;                           // codes of ['x' => 'y']
           int inlineplus;                       // [ x => .... => y ]
           int let; };                            // let ...

#define SEARCHCODE(xxx) { \
  int i; \
  for (i = 0; i < inlinecodesi; i++) \
    if (inlinecodes[i].from == from && inlinecodes[i].to == to) return inlinecodes[i].xxx;\
  return -1; \
} 


#define SEARCHINDEX(xxx,cde) { \
  int i; \
  for (i = 0; i < inlinecodesi; i++) if (inlinecodes[i].xxx == cde) return i; \
  return -1; \
}

extern std::vector<struct INLINES> inlinecodes; // grows on demand (MAXSPAIR = 1000 until S3b)
extern int ninlines;
extern int is_inlin(int cde);
extern int is_inlineplus(int cde);
extern int is_let(int cde);
extern int stratsort2index(int cde);
extern int stratindex;
extern int is_def_str(int from, int to);
extern int apply_code(int from, int to);
extern int inverse_apply_code(term t, int *from, int *to);
//extern int inverse_apply_code(int ind, int *from, int *to);

//extern unsigned stratype[NTYPES/NBITS+1][NTYPES];  // bit-array for strategies
//#define set_strategy_type(x,y) SETBIT(stratype[y],x)
//#define is_strategy_type(x,y) ISSETBIT(stratype[y],x)

extern void create_nested();
extern int add_strat_nont(int pos1,int pos2);
extern int endofin(lexem le);
extern void push_profistck(int p);
extern int top_profistck();
extern int pop_profistck();
extern int handlestrategybody(lstream *f);
extern int semact3(int n,lexem l,lstream *f);
extern void typecheck(lstream *f,char *s);
extern void create_fsymterm(lexem *rside, term &t, int &varc);
extern int add_stratmoduli(int x, int y);
extern int in_strategies;
extern int in_stratop;
extern int strattype;
extern int alpha_syntax;
extern int calledstr;
extern void appactstrat();

void importmod(char *impmodule,lstream *f,int supermodule);
void importmod_inf(const char *impmodule,lstream *f,int supermodule, int rinf);

void trclos(lstream *f);
int gr_compatible(struct sgrammrule *r1, struct sgrammrule *r2);
struct sgrammrule *add_fsymrule(struct sgrammrule *gr1,struct sgrammrule *gr2,
				int infos);
void add_frule(struct sgrammrule *gr, 
	       struct sgrammrule *gr1,
	       struct sgrammrule *gr2);
char *remove_underscores(const char *s);
void load_strat_mod(lstream *f, int sou, int res);
extern int flowcheckrule(lstream *f,transrule *trrule);
extern int equal_non_ground;
extern int peval_switch;
extern int peval_compression;
extern term *defer(term *trm, term *substarray, int &varn);
extern void peval_init();
#endif

