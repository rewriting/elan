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


#ifndef rtdatas_h
#define rtdatas_h
//    run-time data structures head file
//
//  sets of rewriting rules, transition rules and strategies

#include "termdefs.h"
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>

//  tseq ::= term seq |
//           { tseq case bool }^+
//             tseq otherwise seq
//  seq ::= [ where ... | if ... ]^* 


//********** TRY
struct WHEREbranches {
  struct wherelist *wherebranch;  // statements
//         term   test;             //condition
  struct WHEREbranches *next; };

//************************** SWITCH
struct branch {
  struct tseq   *tseq;
         term   test;
  struct branch *next; 
};

struct tseq {
  int is_case;
  struct wherelist *seq; 
  union {
    struct sone_branch {  // is_case = 0
      term *result; 
    } one_branch;
    struct smore_branches {  // is_case = 1
      struct branch *brlist;  
    } more_branches;
  } u;
};
void dump_brlist(ochstream &f, int deep, struct branch    *brlist);
void dump_tseq(ochstream &f, int deep, struct tseq      *rhs);
void dump_seq(ochstream &f, int deep, struct wherelist *seq);


class strategy;

#define IFVARN       (32000)
#define WHEREPATTERN (32001)
// RSWITCH#define SWITCHCASEEND (32002)
#define TRYCHOICEEND (32003)

struct wherelist {              // list of wheres in rules, for each...
    int leftvarn;		// index of left side variable in
				// the where assignement 
				// (if leftvarn == IFVARN then it was just 
				//  condition in this case strateg == NULL

    term pattern;               // if leftvarn == WHEREPATTERN
    int  pattype;               // type of pattern
    char *trail;                // trail of assigned vars

    strategy **strateg;	        // strategy in where
    term whereterm;
    term leftvarterm;           // variable sous forme de terme pour le compilateur
    //** for SWITCH
//RSWITCH    struct branch *branch_list;  // if leftvarn == SWITCHCASEEND
    //** for TRY
    struct WHEREbranches *wherebranch_list;
    struct wherelist *next;    
};


struct termlist {
  term *t;
  int  varn;
  int typ;                   // only for comprim
  struct termlist *next;
};

class termset
 {
  private:
    struct termlist *tlist;
  public:
    termset();
    struct termlist *getlst() { return tlist; }
    void setlst(struct termlist *l) { tlist = l; }
    void copy(termset &intothis) { intothis.tlist = tlist; }
    void single(term *t, /* int varn, */ int typ);
    void addterm(int subsume,term *t, /* int varn, */ int typ);
    void unione(termset tset);
    void remove_first();
    void ith_term(int i, term **t, int *varn, int *typ);
    int size();
    void dump();
    void empty();
    void freeset(); 
//    ~termset();
};

struct patterms {
  termset terms_to_compare;
  termset terms_compared; };


class Patterms
 {
  public:
   int Pattermsi;
   struct patterms Patterm[MAXNOFPATTERNS];
   term *where_to_replace[MAXNOFPATTERNS];
   void init();
   void empty();
   void copy(Patterms *p);
   void add(int redex, term *t, /*int varn, */ int typ);
   void dump();
   void compare(int module);
   void replace();
};

class transrule
 {
private:
  int varnum;// nb de variables dans la regle
  term leftside;
  term rightside;
  int whichmatch;               // symb AC dans leftside ?
  struct wherelist * wheres;
//  char *name;			// just and only for pretty tracing
  term rlabel;                  // term corresponding to rule label
  struct tseq *rhs;	        // extended rules, in this case, rightside=NULL
  int nameindex;                // index of rule name (for statistics)
	                        // -1 for non named rules
  int modul;
  int infos;
  int rule_counter;
public:
  int breaked;                  // break point;
  transrule(int varnum, term lefts, term rights,
	    int modul, int infos,
	    struct tseq *rhs,
	    int whichmatch, int nameindex,term rlabel,struct wherelist *wh);
  ~transrule();
  transrule *crExtRule();
  transrule *crStratAppRule();
  void setvarn(int);
  void setnameindex(int);
  void setwheres(struct wherelist *whs);
  void addwhere(int rev,int varn,strategy **tstrat,term t,lexem whtype);
  void addtrywhere(int rev,struct WHEREbranches *whbrs);
  void addpatternwhere(int rev,strategy **strat,term tl,term tr,int whtype);
  void getr(int &n,term &l,term &r,int &name, struct wherelist *&wh,
	    struct tseq *&rhs,
	    int &whichmatch,term &rlabel);
  void copyinstall(transrule *intothis, term *substarray, int varn, Patterms *pate1);
  strategy *addpartialrule(lstream *f,int partindex, int dknow, int typ);
  int addpartrule(lstream *f,int partindex);
  int no_false();
  int indexrule(int indx);
  //  int renamerule(int indx);
  void copy(transrule *intothis);
  term *getleft();
  void getname();
  int  get_nameindex() { return nameindex; }
  int  getvarn();
  int getmodule() { return modul; }
  int getinfos() { return infos; }
  void add_rule_counter() { rule_counter++; }
  int get_rule_counter() { return rule_counter; }
  int getvarnum();
  int  isOnBuiltins();
  void dump(int ods);
  void consistency();
  void Adump(ochstream &af,int ods);

    //lexem  ruletype();
  int ruletypeval();

};


inline int transrule::getvarnum() {return varnum;}

struct transrulelist {
  struct transrulelist *next;
  transrule *rule;
};

#define STRNAMEDONTCARE      1
#define STRNAMEDONTKNOW      2
#define STRNAMEDONTCARE2     3
#define STRNAMEDONTKNOW2     4
#define STRNAMEREPEAT        5
#define STRNAMEITERATE       6
#define STRNAMEDCPROCESSCALL 8
#define STRNAMEDKPROCESSCALL 9
#define STRINLINE           10
//#define STRBUILTIN          11
#define STRMETA             11
#define STRIDENTITY         12
#define STRCALL             13
#define STRNAMENORMALISE    14
#define STRFAIL             15
#define STRNAMEONE          16
#define STRNAMEONE2         17
#define STRNAMENORMALISE2   18
#define STRNAMEDONTKNOWCON2     19
#define STRNAMEDONTKNOWCON      20
#define STRNAMEDONTCARECON2     21
#define STRNAMEDONTCARECON      22
#define STRNAMEONECON2     23
#define STRNAMEONECON      24
#define STRNORM_IN         25  // [Huy: May  4 00] 
#define STRNORM_OUT        26  // [Huy: May  4 00] 
#define STRNAMETALL        27  // [pem: Oct 26 00]
#define STRNAMETONE        28  // [pem: Oct 26 00]
#define STRNAMETSOME       29  // [pem: Oct 26 00]
#define STRNAMEREWRITE     30  // [pem: Apr  8 02]


struct strlist{
      strategy *str;
      struct strlist *next;
};

struct namelist {
      int strname;
      struct namelist *next;
};

class strategy
 {			// bodies in trsystem.c
private:
  int strname;          // name of strategy
  int strmod;           // module of definition or -1 (when unknown)
  int typeofstrategy;
  strategy *next;
  union {
    struct {
      struct namelist *nm;
    } cr;
    strategy *substrategy;
    struct {
      const char *pname;
      int maxn;
      grammar *locgr;
      int restype;
    } procc;
    struct strlist *stl;
    lbuffer *inlinedbuffer;
  } u;

public:
    int  breaked;
  strategy();
//  strategy(int n,strategy *sub);
  void appendrname(int nam);
  void setnext(strategy *n);
  void setname(int n, int modul);
  int  typeofstr();
  void settypeof(int typ);
  void setsubst(strategy *);
  void setstl(struct strlist *);
  void setstl_no_test(struct strlist *);
  void setnamelist(struct namelist *);
  void setcalledstr(strategy **);
  void setprocname(const char *);
  void setprocmaxn(int n);
  void setprocgr(grammar *);
  void setproctype(int l);
  void setinlinedbuffer(lbuffer *);
  lbuffer *getinlinedbuffer();
  int strnam() { return strname; }
  strategy * nex() {return(next);}
  strategy **nex_addr() { return (&next); }
  struct namelist *rulenamelist() {return(u.cr.nm);}
  strategy *substrateg() {return(u.substrategy);}
  struct strlist *substrlist() {return(u.stl);}
  const char *subprocname() {return(u.procc.pname);}
  int subprocmaxn() {return(u.procc.maxn);}
  int subproctype() {return(u.procc.restype);}
  grammar * subprocgr() {return(u.procc.locgr);}
  int getmodule() {return strmod;}
  void compile(FILE *ff,int deep, int typeofstr);
  void dump();
  void dump2(int n);
  void simpledump(int n);
  void Adump(ochstream &af);
  void Asimpledump(ochstream &af);
  int conform(int warn);        // transform a strategy from intermed form to MV form
  int conform2(int n,int warn, int typ);
  int simpleconform(int n,int warn, int typ);
  int infertypes(int warn, int &sort);
  int infertype(int warn, int &sort);
  int is_call();
  strategy *copy();
  void Delete();
  void expand_strategy();
  void expand_str();
  void count_rules();
  void count_rul();
};
extern struct strlist *copies(struct strlist *sl);
extern void DeletE(struct strlist *sl);
extern void addtrywheretolist(int rev, struct WHEREbranches *whbrs, 
            struct wherelist **p);

class trsystem
 {
private:
  // indexed by definition index:
  stringtab *strategynames_defs;      // definitions of strategies
  int strategyinfos_defs[MAXNOFSTRAT];// global/local
  strategy *strategies_defs[MAXNOFSTRAT];  // strategies via strategynames_defs
  int typeofstrategies_defs[MAXNOFSTRAT];

  // indexed by reference index:
  stringtab *strategynames_refs;      // references of strategies
  strategy *strategies_refs[MAXNOFSTRAT];  // strategies via strategynames_refs
  int strategies_cross[MAXNOFSTRAT];

  stringtab *rulenames;               // set of rule and strategie names
  struct transrulelist *rules[MAXNOFTRN];
  struct transrulelist  **lastrule[MAXNOFTRN];  
  struct transrulelist  *nnrules[MAXNFSYM];
  struct transrulelist  **lastnnrule[MAXNFSYM];  
                        // nonamed rules divides w.r.t. functional symbol
			//	at the head of left side
  int islastrec[MAXNFSYM];  // compiler
public:
  trsystem();
  transrule *addrule(char *name, int lefthead, transrule *rr);
  transrule *addrule(char *name, int varnum,term lefts,term rights,
		     int modul, int infos,
		     struct tseq *rhs,
		     int whichm, term rlabel,struct wherelist *wh);
  int trruleindex(char *name);
  int trrulemember(char *name);
  char *rulename(int n);
  void remove_rulename(int n);
  struct transrulelist *getrules(int ruleindex);
  struct transrulelist *getrules(int ruleindex, int modul);
  struct transrulelist *getnnrules(int fsym) {return(nnrules[fsym]);}

  //------------ MIX:
  int strategy_refs_into_defs(int iname); 
  int strat_refs_into_defs(char *name);

  void set_strategies_cross(int strindex_refs, int strindex, strategy *str);
  void assign_all_refs(int warn);           // calls assign_one_ref
  void assign_one_ref(int warn, int ref);                       
  int searchmatch_defs(char *name, int typ, int modu);
  int searchcall_defs(const char *name, const char *sort);
  // ------------- DEFS:
  const char *strategyname_defs(int n);              
  int strategyindex_defs(char *name, int infos);   
  int strategymember_defs(char *name);             
  const char *strategyname_defs(strategy **);        
  int setstrategy_defs(int n, strategy *st);        // return 0=O.K.; 1=error
  void settypeofstrategy_defs(int n, int typ);
  int  typeofstrategy_defs(int n);
  strategy *getstrategy_defs(int stratindex);
  strategy **getstrategyadr_defs(int stratindex);
  // ------------- REFS:
  const char *strategyname_refs(strategy **);
  int strategymember_refs(char *name);
  int strategyindex_refs(const char *name);
  const char *strategyname_refs(int n);
  int strategyremove_refs(int n);
  strategy *getstrategy_refs(int stratindex);
  strategy **getstrategyadr_refs(int stratindex);
  //---------------
  void addExtRule(transrule *rrule);
  int nnrewrited(term &t);
  void compile(FILE *ff);
  void compilerules(FILE *ff);
  void compilestrats(FILE *ff);
  void genrwrulennappcode
    (FILE *ff,int deep,int actfunction,unsigned long ok,int isitlastrule);
  void dump();
  void consistency();
  void Adump(ochstream &af);
  void Adump_tabs(ochstream &af);
  void Aread_tabs(lstream *f);
  void Aread(lstream *f);
  void joinrdefs();
  void localize1();
  void localize2();
  void rulelistdump(ochstream &af, int rindex, int module);
  void breakk(int breaked);
  void peval(lstream *f);
  void peval(lstream *f,int module);
  void expand_strategies();
  int renamerule(int nameindex, int indx, transrule *rr);
};

class stateofexecution;

#define APPFBEFFIRST 0	// flags of state of AC matching
#define APPFINMATCH 1
#define APPFAFTLAST 2



// run time
class contrule
 {
private:
    int appflag,       // flag before first match <-> inside matchings
        whflag;        // flag before first result of wheres 
    transrule *trule;
    match_state *match;
    term *substarray;
    struct wheress *lastws;
public:
    int nbacktr;
    struct wherelist *lastwheres;
    class term       *lastresult;
    contrule(){ nbacktr = 0; }
  contrule(transrule *r,term mt);
  ~contrule();
//  void deletematch();                            //  !!!!!!!!!!!! just to debug
//  void getactrule(int &n,term &l,term &r,term &c,char *&name);
  void gettraceinfo(char *&name,int &appflag
, int &breakflag
);
  int nextapp(term &mtt,int trace);
  contrule *copy();
  void dump();
};

struct contrulelist {
  contrule *rule;
  struct contrulelist *next;
};

struct statelist {
        stateofexecution *s;
        struct statelist *next;
};

union simple_str_state {
    struct statelist *rep_iter;
    struct {
       struct processdata *pd;
       grammar *gr;
       int stsym;
    } proc;
    struct {
       int nofsubprocesses;
       struct processdata **subprocesses;
       grammar *gr;
       int stsym;
    } processes;
    struct {
      contrule *cr;
      struct transrulelist *r;
      struct namelist *ns;
    } choose;
    struct {
      stateofexecution *st;
      strlist *s;
    } choose2;
};
 

class locstatistics
 {
  int succNodes,totalNodes;
  int nonCountednodes,wasSucces;
  int tmpSucc,tmpTotal;
public:
  locstatistics();
  void fromThisWasSucces();
  void backFrom(locstatistics *);
  void forwardFrom(locstatistics *);
  void write();
  void dump();
};


struct strstatelist {
  term trt;
//  locstatistics *statist;
  strategy *actst;                    // == NULL if st. normalization strategy
  struct strlist *strstore; //$$$     // stack of strategies to finish
  struct strstatelist *next,*prev;
  union simple_str_state *u;          // == NULL before first call of incr_ac..
};


class stateofexecution
 {
private:

 struct strstatelist *sl,*actsl;

 void init_actsl(term t);
 int incr_actsl(term &res);
 void free_pp(struct strstatelist *pp);
 int repeat_incr(term &res);
 void rep_iter_add_state(term t,int afterdeterm);

public:
  stateofexecution(term &t,strategy *st);
  ~stateofexecution();
  void free();
  int nextsolution(term &res);	// ==1 if exist next solution.
  void dump(int n);
};

struct wheress {                        // wheres evaluation state
        struct wherelist *actwh;
        stateofexecution *actexstate;	// state of ex. of actual where
	struct wheress *prev;
        char *trail;                // trail of assigned vars // MMM
        match_state *mstate;
//RSWITCH        struct branch *branch_list;   // for SWITCH
	struct WHEREbranches *wherebranches;  // for TRY-CHOICE
	int  len; int TRYCHOICE;
};

class statistics
 {
  int total_num_of_cpoints;
  int num_of_cpoints;
  int max_of_cpoints;
  int total_num_of_mpoints;
  int num_of_mpoints;
  int max_of_mpoints;
  int transrules,rwrules;
  int transrtried;
  int rulesapp[MAXNOFTRN+1];
  int rulestried[MAXNOFTRN+1];
  struct rusage before_self,after_self; // defined in <sys/resource.h>
  struct rusage before_children,after_children; // defined in <sys/resource.h>
  int numberOfAllocTerm;
  
 public:
  void init();
  void timestart();
  void timestop();
  void transruleapp(int ri, term l);
  void transruletry(int ri, term l);
  void rwruleapp();
  void write(int level,int big);
  void inc_ncp();
  void dec_ncp();
  void dec_manyp();
  void inc_manyp();
  void incNumberOfAllocTerm();
 };



extern statistics statistic;
extern int reverse_wheres;
extern void reduce(term&,int trace);	// body in rtmisc.c

extern void wherelidump(ochstream &f,struct wherelist *wh,int ods);   
  // body in trsystem.c

extern int handlecondition(term c, term *substarray);	// body in rtmisc.c
extern void transred(int,term,int);		// body in rtmisc.c

extern struct processdata *newprocess(const char *command,
	const char *arg1, const char *arg2, const char *arg3, const char *arg4, const char *arg5,
        const char *arg6, const char *arg7, const char *arg8, int maxn, int noblock);
extern void freeprocess(struct processdata *);
extern int isNextSolInProcess(struct processdata *pd);
extern int nextsolprocess(struct processdata *pd,grammar *gr,int stsym,term &res);
extern void kill_all_processus();
extern void kilproc(struct  processdata *pd);
extern void freewherelist(struct wherelist *ll);     // body in trsystem.c
extern struct processdata *pid2processdata(int pid);
extern int write2process(int pid, term &t);
extern void wherelisdump(ochstream &f,struct wherelist *wl,int deep);
extern int qendofin(lexem le);
extern void out_symbol(ochstream &anymod, struct sgrammrule *actr, int kind);
extern void conform_strategies(int warn);
extern char *attach_type(const char *s,int t);
extern char *attach_mod(const char *s,int modu);
extern char *attach_type_mod(const char *s,int t, int modu);
extern char *attach_mod_loc(const char *s, int modul, int infos);
extern int detach_name_type(const char *s,char **name, char **type);
extern int detach_name_module(const char *s,char **name, char **type);
extern int detach_name_type_module(const char *s, char **name, char **type, char **modu);
extern int is_prefix_of_name(const char *s1, const char *s2);
extern int impmoduli;
extern void init_visi(int modul);
extern int stratmoduli(int x, int y);
//extern int strauxmoduli(int x);
extern int evalmoduli(int x,int y);
#define TYPE_SEPARATOR    ":"
#define TYPE_SEPAR    ':'
#define MODULE_SEPARATOR    "/"
#define MODULE_SEPAR    '/'
#define LOC_SEPARATOR  "!"
extern int visible(int i, int j, int infos);
#define CONVERT(ssa) \
 ( remove_underscores(trrules.strategyname_defs( \
	trrules.strategy_refs_into_defs( \
        trrules.strategyindex_refs( \
	trrules.strategyname_refs(ssa))))) )
extern int lis_norm;
extern int alg_normalisation;
extern strategy *gen_normalisation1(int norm_index,int modu, int sort, struct strlist *sl);
extern struct namelist *gen_normalisation2(int norm_index,int sort, strategy *normstr);
extern void addwheretolist(int rev,int varn,strategy **strat,term t,lexem whtype, struct wherelist **p);
extern void addpatternwheretolist(int rev,strategy **strat,term tl,term tr,
				       int whtype,struct wherelist **p);
extern void Awherelisdump(ochstream &f,struct wherelist *wl);
extern void Awherelidump(ochstream &f,struct wherelist *wl);
extern void Aswitchdump(ochstream &f, struct tseq *tseq);
extern void Abranchlistdump(ochstream &f, struct branch *brlist);
extern int builtintype(int typeofstr);

extern void RSambiguity(int warn, char *name, char *type, char *modu);
extern void Sundefined(int warn, char *name, char *type, char *modu);
extern void RSundefined(int warn, char *name, char *type, char *modu);
extern void SSambiguity(int warn, char *name, char *type, char *modu);

// backtracking of where clauses and rule right-hand sides, bodies in
// stateofexecution.cc (moved from commondefs.h)
extern void freeWhereBacktrack(int varn,struct wheress * &lastws,term *substarray);
extern int isWhereBacktrackNextSol(struct wherelist *wheres,term *substarray,
                   struct wheress * &lastws, int trace, int varn);
extern int isTseqBacktrackNextSol(term &res, struct tseq *rhs, term *substarray,
                   struct wheress * &lastws, int notbatch, int nback,
	           struct wherelist *&lastwheres, class term *&lastresult,
	           int varn);

#endif
extern struct wherelist *appendwherelists(struct wherelist *a,
				   struct wherelist *b, int *len);
extern int LEN(struct wherelist *a);
extern struct wherelist *delete_n_wheres(int len, struct wherelist *a);

extern struct wherelist *copy_wherelist(struct wherelist *wl);
extern struct WHEREbranches *copy_WHEREbranches(struct WHEREbranches * wb);
extern struct wherelist *new_wherelist();
extern void init_files();
