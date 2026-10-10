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
     typecheck(f,ld.actimp->s);\
     pos = typet.addstr(ld.actimp->s); \
     /**/ CFRE(ld.actimp->s); CFRE(ld.actimp); ld.actimp=NULL;\
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

extern void fsymrule_add(struct sgrammrule *r1, struct sgrammrule *r2);
extern int fsymrule_exists(struct sgrammrule *r1, struct sgrammrule *r2);

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
// ld.strstack and ld.strlstack grow on demand (MAXINCLSTRAT = 30 until S3b)
extern void grow_strstack(int n);               // makes strstack[0..n-1] exist
extern void grow_strlstack(int n);              // makes strlstack[0..n-1] exist

// int mainstrategy;
// int & mainnewmod;

extern int pretydumpsitset(lstream *f,stringtab *types);
extern int acsymbolinleftside;		        // body in esemact.c   
                    //| is there an AC symbol in the left hand side of CP rule?
extern  int strategywasapplied;
extern void esemactinit();		             // body in esemact.c   | init term construction semantic actions
extern int pretydumpsitset(lstream *f,stringtab *types); // pretty dump of earley's situations table





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


struct  Spair { int from; int to; int stratsort; };  // cannot be move to inlinecodesi

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
extern int alpha_syntax;
extern int big;                                 // statistics level of -s/-S (driver/ldmain.cc)
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

struct RPair;                                   // msemact3.cc
struct tseq;                                    // rtdatas.h

// The state of the module parser (the semantic actions of msemact.cc and
// msemact3.cc, "CP" = currently parsed), until S3b about 110 separate
// globals. There is ONE instance, ld, shared by every level of import:
// importmod_inf reads an imported module (readmodules) in the middle of the
// importing one, and the inner SEMACT(0) resets part of this state
// (rulepri, finfos, actarity, actsemantic, actvtabi, vartabi, nestedvartabi,
// condition, actimp) and sets actcode = fsymtabi; the importing module then
// continues with the advanced actcode. Making it per-level would change
// that. The only per-level state is the module frames (modframes, above).
//
// Not here: the tables of the loaded program that the rest of the
// interpreter reads after loading (inlinecodes, visibilities, fsymtab...),
// and impmoduli and withrhs, which lex/mlstream.cc and parse/esemact.cc
// (lower modules, see tests/architecture) reach through extern declarations
// (rtdatas.h, termdefs.h).
struct LoaderState {
  // the CP function symbol (operator declaration)
  int rulen = 0, rulepri = 0;                   // the number and the priority of the CP rule
  int rinfos = 0, finfos2 = 0;                  // syntactic priority and associativity of the CP symbol
  int finfos = 0;                               // is the CP symbol AC ?
  int actarity = 0, actprofis = 0, actcode = 0; // arity of the CP symbol; CP profile; and current code number
  int actsemantic = 0;                          // semantic flag of the CP symbol
  lexem actleftside;                            // codomain of the CP profile
  lexem actruletype;                            // type of terms in the CP rule
  lexem sntype;
  int wasdefinedas = 0;                         // only for the COMPILER, is the symbol defined as an inlined function?
  struct sgrammrule *actalr = nullptr;          // CP functional symbol grammar rule
  int symbolcode = 0;                           // the value of a character defined by its ASCII code
  struct chlist *actarglist = nullptr, *actimp = nullptr; // used to pass the arguments (module names)

  // variables of the CP rule
  lexem actvartype, actvartype1;                // CP variables type
  struct nvlist *nestedvartabi = nullptr;       // a stack of var declarations (nested "rules" constructions)
  struct sgrammrule *vartab[MAXNOFVAR] = {};    // grammar rules for variables
  struct sgrammrule *dollar_vartab[MAXNOFVAR] = {}; // copy of vartab for the rules of variables with a dollar
  lexem actvtab[MAXNOFVAR];                     // names of variables
  int actvarrename[MAXNOFVAR] = {};             // table used during the renaming of variables inside RW-rules
  int actvarnum = 0;                            // the real number of variables inside an RW-rule
  int maxvarnum = 0;
  int actvtabi = 0, vartabi = 0;                // numbers of variables (they differ while parsing variable
                                                // declarations of the same type)
  lexem actlvtab[MAXNOFVAR];                    // for variables in labels
  int actlvtabi = 0;                            // pointer to actlvtab
  int RENAME_ALL_VARS = 0;
  int RENAME_IDENTITY = 0;

  // the CP rewrite rule
  term lside, rside, condition;                 // left hand side, right hand side and condition of the CP RW rule
  term ter1, ter2;                              // temporary terms
  const char *acttrrulename = nullptr;          // the name of the CP RW rule
  transrule *acttrrule = nullptr;               // the CP RW rule
  transrule *acttrrulelab = nullptr;            // LAB_ RW rule
  lexem actwheretype;                           // type of the CP where affectation
  int actwherevar = 0, actwhstrategy = 0, actstratindex = 0; // CP variable of a where; its strategy; index of the strategy
  struct tseq *act_rhs = nullptr;
  int pattype = 0;
  int wherecount = 0;
  lexem actruletype_l;                          // type of terms in the CP rule (labels)
  int actruletypeindex = 0;
  int actruletypeindex_l = 0;
  term dstr_rs;                                 // right-hand side of a dstr rule

  // strategies
  std::vector<strategy *> strstack = std::vector<strategy *>(2); // nested strategies (REPEAT, ITERATE)
  int strstacki = 0;                            // index to strstack
  std::vector<struct strlist *> strlstack;      // nested lists of strategies (DONT CARE/KNOW CHOOSE)
  int strlstacki = 0;                           // index to strlstack
  const char *actstrategyname = nullptr;        // the name of MV strategy
  strategy *actstrategy = nullptr;              // the CP strategy
  int stratindex = 0;                           // type index of Marian's strategy
  int calledstr = 0;                            // strategy in call(..)
  int in_strategies = 0;
  int in_stratop = 0;
  int strattype = -1;
  int locstratlen = 0;
  int *locstrattable = nullptr;                 // table of local strategies
  struct ilist *locstrat = nullptr;             // local strategy of a symbol
  struct ilist **locstratend = &locstrat;       // local strategy end

  // strategy profiles and sorts <X->Y> (msemact3.cc)
  std::vector<int> profistck;                   // stack of profiles (grows on demand)
  int profistcki = -1;                          // top-pointer to profistck
  int profi_level = 0;                          // level of nesting of profiles
  int profi_lev = 0;
  int selectorn = 0;
  int numb_selectors = 0;
  int sel_poss = 0;
  struct selector selectors[MLENGRRULE] = {};
  char strategy_type[STRLEN] = {};              // name of the first type in profile
  int pos_l = 0;                                // index of type X, when parsing <X->Y>
  profitab profit = {};                         // table of profiles
  struct RPair *fsymrules = nullptr;            // list of rule indexes (r1,r2)
  int spairi = 0;
  std::vector<struct Spair> spair;              // pairs [x,y] if strat[x,y] (grows on demand)
  int Gtypestack[MAXGTYPESTACK] = {};
  int Gtypestacki = 0;

  // imports
  char *actargmodname = nullptr;                // the name of the CP module
  int in_stratmoduli = 0;                       // true if in str* module
  int stratmoduli_fromi = -1;                   // X of str* module
  int stratmoduli_toi = -1;                     // Y
  int is_explimpl = 0;                          // explode-implode module should be loaded
  int ignore = 0;                               // ignore deeper levels
  int explimpl_index = 0;                       // file counter
  int anysi = 0;                                // pointer to anys
  int anys[MAXANYS] = {};                       // modules for which any[X] has been imported
  int is_symbappl = 0;
  int symbappli = 0;
  struct ilist *symbappl[MAXSYMBAPPL] = {};
  int symbappl_index = 0;                       // file counter
};
extern LoaderState ld;                          // body in msemact.cc
#endif

