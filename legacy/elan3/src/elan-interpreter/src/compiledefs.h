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

#ifndef compiledefs_h
#define compiledefs_h

#include "termdefs.h"
#include "rtdatas.h"

#define MAXRTSIZE 500
#define MAXINT (0x7fffffff)
#define MAXNOFRULECOMBINE 2048     // size of hash tible for genexpmatch
#define MAXRSGENVAR 500            // maximal number of tmp vars building rside
//#define Var DEFAULTSYM   
#define Other (-1)


class transrule;

#ifdef MARIANS ////////////////////////////// JUNK
class matchtree;
struct matchfslist {
  unsigned fsym;
  term *rightside;          // assigned iff tree == NULL
  matchtree *tree;
  struct matchfslist *next;
};

class matchtree {
#ifdef GCMEM
: public gc
#endif
 private:
  unsigned maxvar,fathervar,fathersubi;
//  unsigned vartoswitch;
  struct matchfslist *sub;

  merge(unsigned skipUntilFather, matchtree *mt);
 public:
  matchtree();
  matchtree(unsigned oldmaxvar, unsigned fath, unsigned subti, term t);
  addterm(term t);
  closure();
  gen(FILE *f);
  dump();
};
#endif

/*
   something imported from my many-to-one matching code (file modrtu.c)
*/


			/***  globdef.h  ***/

extern int expmatch;    /* generate exponential many-to-one matching ? */
extern int Bins;        /* compile built-ins */
extern int earley_analyser; /* use earley features ? */
extern int islist;	/* robit listing ? */
extern int waserror;
extern int arity[];	/* arita identifikatora */

#define MAXIDENT 100	/* max poc ident */
#define IDENTLEN 30	/* max dlzka ident */

/*
struct term { 	int fs;
		struct term **st;};
typedef struct term *ter;
typedef ter  *subt;


#define terlist struct terlistn *
struct terlistn { ter head;
		  terlist tail;
		};
#undef terlist
typedef struct terlistn *terlist;
*/
struct rtlistn;
struct rtnode;

struct rtlistn { 
  struct rtnode * head;
  struct rtlistn * tail;
};


struct rtna { 
  unsigned ELIMset;
  int affvar,father,fatheri;          // variable names for code generation
  int isbuiltins;
  int nofSubTrees;                    // cardinality of setrt
  struct rtlistn setrt;
};

struct rtnode 	{ 
  int fsym;
  int infos;
  unsigned long OKset;
  struct rtna *subrt;      /* in fact struct rtna subrt[arity(fsym)] */
};

extern int optimize;        /* body in ldmain */
extern int determLink;        /* body in ldmain */

extern void addproto(struct rtnode * *prt,term *t);
extern void makert(struct rtnode *rrt,unsigned long oth);
extern void writert(struct rtnode * rrt);
extern void genfunbody(FILE *ff,struct rtnode *rrt,int isrecursive);
extern void genStratAppBody(FILE *ff,struct rtnode *rrt,int lab,int dontcare,
	int nofree);

#ifdef EARLEY
extern void genFunTab(FILE *ff);
#endif

extern void genfsymtab(FILE *ff);
extern void genpreambule(FILE *ff);
extern void genepilog(FILE *ff);
extern void genstratdeclar(FILE *ff);
extern void genmaintfile(FILE *ff,term maint,int mstrat);
//extern int parsereturnedterm(lstream *ff,term &t);
extern int writereturnedterm(lstream *ff,ochstream &of);
extern int variableaff;
extern int addRuleToCompile(FILE *ff, transrule *rul, struct rtnode *&rrt,
                int isdet,int isfunction);
extern void intend(FILE *ff,int i);
extern void genstratrulennappcode
       (FILE *ff,int deep,unsigned long ok,int lab,int dontcare,int isitlastrule);
extern void gennnappcode(FILE *ff,int deep,struct transrulelist *actr,
		    int islastr,unsigned long okmask,int isdet,
		    int isForStrat,int slab,int isitlastrule);
extern int genWheresCode(FILE *ff,int deep,struct wherelist *wh,int det,int isForStrat);
extern int isconditioned(unsigned long ok); // relative to static actrrule !!!!!!!
extern void inlwritefun(lexem lex);
extern void setrsff(FILE *ff);
extern void actcruleerror();
extern void genFail(FILE *ff,int deep);

extern void allocVarInit();
extern int allocVarFirstFree();
extern void allocVarSetUsed(int n);
extern void allocVarSetUnused(int n);
extern int allocVarIsUnused(int n);

extern int compile;        // am I compiling ?? defined in ldmain.c
extern int actnoruleapp;   // was generted goto to noruleapp?
extern unsigned long mask;
extern FILE *inloutfile;

#define emptyrt() NULL

#define HEADER \
  fprintf(ff,"struct term *fun%d(\n#ifdef __cplusplus\n",f); \
  for(i=0; i<a; i++) { \
    fprintf(ff,"struct term*v%d",rrt->subrt[i].affvar); \
    if (i<a-1) fprintf(ff,","); } \
  fprintf(ff,")\n#else\n"); \
  for(i=0; i<a; i++) { \
    fprintf(ff,"v%d",rrt->subrt[i].affvar); \
    if (i<a-1) fprintf(ff,","); } \
  fprintf(ff,")\n"); \
  for(i=0; i<a; i++) { fprintf(ff,"struct term *v%d;\n",rrt->subrt[i].affvar); } \
  fprintf(ff,"#endif\n");

#define ISFUNCONSTRUCTOR1(s) (s==DOUBLECONSTRUCT || \
   (trrules.getnnrules(s) == NULL && (fsymtab[s].get_semantic() == 0)) || \
   (trrules.getnnrules(s) == NULL && (fsymtab[s].get_semantic() < -100 || fsymtab[s].get_semantic() > 100) ))
#define ISFUNCONSTRUCTOR(s) (s==DOUBLECONSTRUCT || \
   (trrules.getnnrules(s) == NULL && fsymtab[s].get_semantic() == 0))
#define ISCONSTRUCTOR(s) (s==DOUBLECONSTRUCT || \
   (trrules.getnnrules(s) == NULL && fsymtab[s].get_semantic() == 0 && \
    fsymtab[s].isnotdefinedas()))
/*#define ISFUNCONSTRUCTOR(s) (s==DOUBLECONSTRUCT || \
   (s>=FSYMCODESBEG && trrules.getnnrules(s) == NULL))
#define ISCONSTRUCTOR(s) (s==DOUBLECONSTRUCT || \
   (s>=FSYMCODESBEG && trrules.getnnrules(s) == NULL && fsymtab[s].isnotdefinedas()))
*/

#define ISBUILTIN(l) ((l==booltype)?TNORMFS:((l==identype)?TIDENT:((l==numtype)?TNUMBER:((l==stringtype)?TSTRING:0))))
//#define ISBUILTIN(l) ((l==booltype)?TNORMFS:((l==identype)?TIDENT:((l==numtype)?TNUMBER:0)))

#endif







