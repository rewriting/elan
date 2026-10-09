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
#define MAXPROFISTCK          1000
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

//---------------------------------- strategy.c
// ------------------------- strategy kind
#define DC_STRAT              1
#define DK_STRAT              2
#define ID_STRAT              3
#define FAIL_STRAT            4
#define CONCAT_STRAT          5
#define FSYM_STRAT            6
#define RULE_STRAT            7
#define ANY_IF                8
#define ANY_APPLY             9
#define IF_STRAT             10
#define LET_STRAT            11
#define SEQ_STRAT            12
#define DEF_STRAT            13
#define TERM_STRAT           14   // only for arguments of defined strats

struct Any_apply {
    term *var;
    struct strat *str; };

struct Any_if {
    term *cond; };

class Any
 {
public:
  int any_kind;
  union {
    struct Any_apply   any_apply;
    struct Any_if      any_if;
  };
  Any(term *con);
  Any(term *va, struct strat *st);
  void dump(ochstream &och);
};

class Alist
 {
    public:
    Any    *any;
    Alist  *next; 
    Alist(Any *an, Alist *inext) { any = an; next = inext; }
    void dump(ochstream &och);
};

class Slist
 {
    public:
  struct strat *s;
  Slist *next;

  Slist(strat *is, Slist *inext) { s = is; next = inext; }
  void dump(ochstream &och,int sep);
};

struct List_k { 
    int arity; 
    Slist *subs; };

class Appl
 {
    public:
  term *t;
  struct strat *s;
    Appl(term *tt, struct strat *ss) { t = tt; s = ss; }
  int dump(ochstream &och);
};

struct Let_k {
    term  *var;
    Appl  *appl;
    struct strat *instr; };

struct Rule_k {
    term  *left;
    term  *right;
    Alist *anys; };

struct If_k {
    term  *cond;
    struct strat *then;
    struct strat *els; };

struct Seq_k {
    Slist *slist; };

struct Fsym_k {
   int destr;
   int constr; 
   int arity;
   Slist *subs; };

struct Term_k {
    term *t; };

class strat
 {
  public:
    int kind;                 // kind of record
    union {
	struct List_k list_k;
	struct Let_k  let_k;
	struct Rule_k rule_k;
	struct If_k   if_k;
	struct Seq_k  seq_k;
        struct Fsym_k fsym_k;
	struct Term_k term_k;
    };
    strat(int ikind,int iarity,Slist *isubs)
    { kind = ikind; list_k.arity = iarity; list_k.subs = isubs; }
    strat(term *lef, term *rig, Alist *any)
    { kind = RULE_STRAT;
      rule_k.left = lef; rule_k.right = rig; rule_k.anys = any; }
    strat(term *con, strat *the, strat *els)
    { kind = IF_STRAT; if_k.cond = con; if_k.then = the; if_k.els = els; }
    strat(term *va, Appl *app, strat *inst)
    { kind = LET_STRAT; let_k.appl = app; let_k.instr = inst; let_k.var = va;}
    strat(Slist *slis)
    { kind = SEQ_STRAT; seq_k.slist = slis; }
    strat(int ikind,int destr, int constr,int iarity,Slist *isubs)
    { kind = ikind; // FSYM_STRAT ou DEF_STRAT
      fsym_k.destr = destr; fsym_k.constr = constr; 
      fsym_k.arity = iarity; fsym_k.subs = isubs; }
    strat(term *t) { kind = TERM_STRAT; term_k.t = t; } 
    void dump(ochstream &och);
};

//-----------------------------------
#define DEEPNESS    20

union u {
  term         *t;
  strat        *s;
  Slist        *sl;
  Any          *any;
  Alist        *anyl;
  Appl         *appl;
};

#define STTOP     (ststack[ststacki])
#define STPUSH_t(x) { ststack[++ststacki].t = x; }
#define STPUSH_s(x) { ststack[++ststacki].s = x; }
#define STPUSH_sl(x) { ststack[++ststacki].sl = x; }
#define STPUSH_any(x) { ststack[++ststacki].any = x; }
#define STPUSH_anyl(x) { ststack[++ststacki].anyl = x; }
#define STPUSH_appl(x) { ststack[++ststacki].appl = x; }
#define STPOP     (ststack[ststacki--])

extern union u ststack[DEEPNESS];
extern int ststacki;

//-------------------------------------
extern void crfstrat(int destr, int constr);
extern void crdefstrat(int rulenum);

extern void dump_list(Slist *sub);

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
extern const char *actmodname[MAXINCLDEEP];           // stack of the names of CP modules 
extern char *actfilemodname[MAXINCLDEEP];       // stack of the files where the CP modules are placed
//extern int newmodule[MAXINCLDEEP];
extern grammar grstack[MAXINCLDEEP];            // stack of the grammars of CP modules
extern int stacki;                              // index to modules stacks
extern int importrinfos[MAXINCLDEEP];           // kind (GLOBAL/LOCAL) of the CP import of a module
extern strategy *strstack[MAXINCLSTRAT];        // stack used while parsing nested strategies (REPEAT,ITERATE)
extern int strstacki;                         // index to strstack
extern struct strlist *strlstack[MAXINCLSTRAT]; // stack used while parsing nested list of strategies (DONT CARE/KNOW CHOOSE)
extern int strlstacki;                        // index to strlstack
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




extern int profistck[MAXPROFISTCK];           // stack of profiles
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
#define make_new_nonts(x,y,z,u) { sprintf(x,"<%s->%s>",z,u); }

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

extern struct profitab profit;                       // table of profiles

#define MAXSPAIR    1000
extern  int spairi;
struct  Spair { int from; int to; int stratsort; };  // cannot be move to inlinecodesi
extern struct Spair spair[];                     // pairs [x,y] if strat[x,y]

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

extern struct INLINES inlinecodes[];       
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

