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

#include "termdefs.h"
#include "module.h"
#include "compiledefs.h"
#include "strategy.h"
extern lexem sourcetype;

void genFail(FILE *ff,int deep)
{
  intend(ff,deep);
  if (optimize) fprintf(ff,"fail();return(NULL);\n");
  else fprintf(ff,"number_of_fail++;fail();return(NULL);\n");//noff
}

void genstattabs(FILE *ff)
{ int i;

  fprintf(ff,"int FSYMTABSIZE = %d;\n",fsymtabi);
  fprintf(ff,"int RTABSIZE = %d;\n",MAXNOFTRN);

  fprintf(ff,"char *RNAMES[] = {");
  if (statis) {
    for (i=0; i<MAXNOFTRN; i++) {
      if (!(i%15)) fprintf(ff,"\n");
      if (trrules.rulename(i))
	fprintf(ff,"\"%s\",",trrules.rulename(i));
      else
	fprintf(ff,"NULL,"); }
  }
  fprintf(ff,"0};\n\n");

  fprintf(ff,"int anofreduction[%d];\n",fsymtabi);
  fprintf(ff,"int anofrt[%d];\n",fsymtabi);
  fprintf(ff,"int anofr2[%d];\n",fsymtabi);
  fprintf(ff,"int anofsreduction[%d];\n",MAXNOFTRN);
  fprintf(ff,"int anofsrt[%d];\n",MAXNOFTRN);
}

void genfsymtab(FILE *ff)
{ 
  int i;
  fprintf(ff,"struct fsym fsymtab[] = {");
  for (i=0; i<fsymtabi; i++) {
    if (!(i%15)) fprintf(ff,"\n");
    fprintf(ff,"{%d},",fsymtab[i].arity());
  }
  fprintf(ff,"{0}};\n\n");

  if(earley_analyser) {
    // arity
    fprintf(ff,"int arity[] = {");
    for (i=0; i<fsymtabi; i++) {
      if (!(i%25)) fprintf(ff,"\n");
      fprintf(ff,"%d,",fsymtab[i].arity());
    }
    fprintf(ff,"0};\n\n");
    // isconstructor
    fprintf(ff,"int isconstructor[] = {");
    for (i=0; i<fsymtabi; i++) {
      if (!(i%25)) fprintf(ff,"\n");
      fprintf(ff,"%d,",ISCONSTRUCTOR(i));
    }
    fprintf(ff,"0};\n\n");
  }

}

void genlabelswitch(FILE *ff)
{
int i;
lexem labellex;
labellex.crtypelex(typet.addstr("Label"));
  fprintf(ff,"void *labelswitch(int label){\n");
  fprintf(ff,"  switch(label) {\n");
    for (i=0; i<fsymtabi; i++) {
    if (fsymtab[i].textform()!=NULL &&
	fsymtab[i].textform()->leftside == labellex)
      fprintf(ff,"\tcase %d :\treturn &(str_%s);\n",i,
	      (fsymtab[i].textform()->rside[0]).alfsy());
    }
    fprintf(ff,"\tdefault : \treturn NULL;\n");
  fprintf(ff,"  }\n");
  fprintf(ff,"}\n");
}

int cross_refs(FILE *ff,const char *ss)
{ int i,j; /*,k,a;*/
  lexem *p;
  j=0;
 j=0;
  fprintf(ff,"unsigned *%ssprofil[] = {",ss);
  for (i=0; i<fsymtabi; i++) {
    if (!(i%5)) fprintf(ff,"\n");
    fprintf(ff,"&(%sppfs[%d]),",ss,j);
//    if (ISCONSTRUCTOR(i)) {
      if (fsymtab[i].textform() != NULL) {
	p = (fsymtab[i].textform())->rside;
	if (strlen(ss)) j++; // lefthandside
	while ( p->isnotendofstream()) {
	  if (p->nonterminal()) j++;
	  p++;
	}
      }
      j++;
      // }
  }
  fprintf(ff,"NULL};\n\n");
  return 0;
}


void genppfs(FILE *ff,const char *ss)
{ int i; /*,j,k,a;*/
  lexem *p,lf;
  fprintf(ff,"static unsigned %sppfs[] = {",ss);
  for (i=0; i<fsymtabi; i++) {
//    if (ISCONSTRUCTOR(i)) {
    if (!(i%20)) fprintf(ff,"\n");
    if (fsymtab[i].textform()!=NULL) {
      if (strlen(ss)) {
	lf = (fsymtab[i].textform())->leftside; fprintf(ff,"%d,",lf.typeval()); }
      p = (fsymtab[i].textform())->rside;
      //fprintf(ff,"/*%d=*/",i);
      while ( p->isnotendofstream()) {
	if (p->nonterminal()) {
//          if (!(j%20)) fprintf(ff,"\n");
          fprintf(ff,"%d,",
		  (!strlen(ss))?ISBUILTIN(*p):p->typeval());
	}
	p++;
      }
    }
     fprintf(ff,"EPM,");
//    }
  }
  fprintf(ff,"0};\n\n");
}

void genprofils(FILE *ff)
{ 
  genppfs(ff,""); cross_refs(ff,"");
//// eh, voila, on a besion un vrai fymtab
//// SHOULD BE UNIFIED WITH ppfs, otherwise ...
  genppfs(ff,"real_"); cross_refs(ff,"real_");
//// SHIT OF ALL SHITS
//// UP TO THIS, THE CODE IS IDENTICAL, EXCEPT ISBUILIN

}

static int arities[MAXNOFARITIES];
static int aritiesi=0;

static void genfreelist(FILE *ff,const char *prefix,int a)
{ int i;
  for (i=0;i<aritiesi;i++) if (arities[i]==a) return;
  arities[aritiesi++] = a;
  fprintf(ff,"%s struct term *f%dlist",prefix,a);
  if (!(*prefix)) fprintf(ff," = NULL");
  fprintf(ff,";\n");  
  //  fprintf(ff,"%s TERMSTR(term%d,%d);\n",prefix,a,a);  
  fprintf(ff,"TERMSTR(term%d,%d);\n",a,a);
}


//extern int earleyPrettyDumpGrammarRule(ochstream &stout,struct sgrammrule *gr);
void genFunTab(FILE *ff)
{
  int i,a;

  fprintf(ff,"// tableau de pointeurs de fonctions qui retournent\n");
  fprintf(ff,"// un pointeur sur un term\n");
  fprintf(ff,"#ifdef __cplusplus\n");
  fprintf(ff,"typedef struct term* (*funTabType)(...);\n");
  fprintf(ff,"#else\n");
  fprintf(ff,"typedef struct term* (*funTabType)();\n");
  fprintf(ff,"#endif\n");

  fprintf(ff,"funTabType funTab[] = {");
  for (i=0 ; i<fsymtabi ; i++) 
    {
      if (!(i%5)) fprintf(ff,"\n");
      if(fsymtab[i].textform()!=NULL
	 && i!=TRUEVAL
	 && i!=FALSEVAL
	 && i!=IDENTEQUAL
	 && i!=BOOLXOR
	 && i!=NUMTOTERM
	 && i!=IDENTTOTERM
	 && i!=STRINGTOTERM
	 && i!=INTCONSTUMIN
	 && i!=COMPILMAIN
	 )
	{
	  //earleyPrettyDumpGrammarRule(stout,fsymtab[i].textform());
	  //stout << "\n";
	  //stout << "ISBUILTIN=" << ISBUILTIN(fsymtab[i].textform()->leftside) << "\n";


	  a = fsymtab[i].arity();
	  if(a==0)
	    {
	      if(ISCONSTRUCTOR(i))
		fprintf(ff,"(funTabType)&con%d,",i);
	      else
		if(i<FSYMCODESBEG)
		  fprintf(ff,"(funTabType)&earley_fun%d,",i);
		else
		  fprintf(ff,"(funTabType)&fun%d,",i);
	    }
	  else
	    {

//stout << i << " is constructor " << ISCONSTRUCTOR(i) << "\n";

	      if(ISCONSTRUCTOR(i))
		fprintf(ff,"0,");
	      else
		if(i<FSYMCODESBEG)
		  fprintf(ff,"(funTabType)&earley_fun%d,",i);
		else
		  fprintf(ff,"(funTabType)&fun%d,",i);
	    }
	}
      else
	fprintf(ff,"0,");
    }
  fprintf(ff,"0};\n\n");
}

#define HEADER1(f) \
  fprintf(ff,"\nstruct term *fun%d(\n#ifdef __cplusplus\n",f); \
  for(j=0; j<a; j++) { fprintf(ff,"struct term*v%d",j); \
    if (j<a-1) fprintf(ff,","); } \
  fprintf(ff,")\n#else\n"); \
  for(j=0; j<a; j++) { fprintf(ff,"v%d",j); \
    if (j<a-1) fprintf(ff,","); } \
  fprintf(ff,")\n"); \
  for(j=0; j<a; j++) { fprintf(ff,"struct term *v%d;\n",j); } \
  fprintf(ff,"#endif\n");

#define BODY(ajout,f) \
  fprintf(ff,"return fun%d(",f); \
  if (ajout) fprintf(ff,"%d",i); \
  for(j=0; j<a; j++) { \
    if (ajout || j) fprintf(ff,","); \
    fprintf(ff,"v%d",j); } \
  fprintf(ff,");"); 

void genfsym(FILE *ff,const char *prefix,int i)
{
    int j,a;
    a = fsymtab[i].arity();
    if (!(a)) {
      // to write out a real fsym-tab ...
      if (strlen(prefix) == 0 && fsymtab[i].get_semantic() != 0) {
	HEADER1(i);
	fprintf(ff,"{");
	BODY(fsymtab[i].add_sort(),fsymtab[i].get_semantic());
	fprintf(ff,"}\n\n");
      } else {
	  fprintf(ff,"%s struct term ccon%d",prefix,i);
	  if (!(*prefix)) fprintf(ff," = {0,NULL,%d}",i);
	  fprintf(ff,";\n%s struct term *con%d",prefix,i);
	  if (!(*prefix)) fprintf(ff," = &ccon%d",i);
	  fprintf(ff,";\n");
	  if (!(ISCONSTRUCTOR(i))) {
	    fprintf(ff,"%s struct term *fun%d();\n",prefix,i);
	  }
	}
    } else {
      genfreelist(ff,prefix,a);
      if (!(ISCONSTRUCTOR(i))) {
	fprintf(ff,"extern struct term *fun%d(\n",i);
	fprintf(ff,"#ifdef __cplusplus\n");
	for(j=0;j<a;j++) fprintf(ff,"struct term*%c",((j==a-1)?' ':','));
	fprintf(ff,"\n#endif\n);\n");
      }
      // to write out a real fsym-tab ...
      if (strlen(prefix) == 0 && fsymtab[i].get_semantic() != 0) {
	HEADER1(i);
	fprintf(ff,"{");
	BODY(fsymtab[i].add_sort(),fsymtab[i].get_semantic());
	fprintf(ff,"}\n\n");
      }
    }
}

void genconstants(FILE *ff,const char *prefix)
{ 
  int i;
  struct definedaslist *ddt;
  const char *fn;
  aritiesi = 0;

// I will probably need free list for arities 1,2 and 3 for some builtins

  genfreelist(ff,prefix,1);
  genfreelist(ff,prefix,2);
  genfreelist(ff,prefix,3);

// than ordinary functional symbols

  for (i=FSYMCODESBEG; i<fsymtabi; i++) 
      genfsym(ff,prefix,i);
  fprintf(ff,"\n");

  while (definedasl != NULL) {
    fn = definedasl->fname.alfsy();
    fprintf(ff,"#define %s fun%d\n#define CODE_%s %d\n",
	            fn,definedasl->code,fn,definedasl->code);
    ddt = definedasl->next; CFRE(definedasl); definedasl=ddt;
  }

}

void genstratdeclar(FILE *ff)
{ int i;
  strategy *s,**ssa;
  for (i=0;i<MAXNOFSTRAT;i++) {
    s = trrules.getstrategy_defs(i); 
    ssa = trrules.getstrategyadr_defs(i);
    if (s!=NULL) {
      fprintf(ff,"extern struct term *str_%s(\n", 
	       remove_underscores(trrules.strategyname_defs(ssa))
	      );
      fprintf(ff,"#ifdef __cplusplus\n");
      fprintf(ff,"struct term*");
      fprintf(ff,"\n#endif\n);\n");
    }
  }
  fprintf(ff,"\n");
}



static void geninlines(FILE *ff)
{ struct inlineslist *inl;
  while (inlinesl!=NULL) {
    fprintf(ff,"\n");
    inloutfile = ff;
    inlinesl->text->applyflush(0,inlwritefun);
    fprintf(ff,"\n");
    inl = inlinesl->next; CFRE(inlinesl); inlinesl=inl;
  }
}

void genpreambule(FILE *ff)
{

  if (Bins) fprintf(ff,"\n#define BINS %d\n",Bins);

  fprintf(ff,"#include \"RTCommons.h\"\n\n");
  fprintf(ff,"int nofreductions=0;\n");
  fprintf(ff,"int nofsreductions=0;\n");
  fprintf(ff,"int nonamed_tried=0;\n");  //nofrt
  fprintf(ff,"int nofr2=0;\n");
  fprintf(ff,"int named_tried=0;\n"); //nofsrt
  fprintf(ff,"int number_of_fail=0;\n");//noff
  genstattabs(ff);
  genfsymtab(ff);
  genprofils(ff);
  genconstants(ff,"");
  genstratdeclar(ff);
  geninlines(ff);
  genlabelswitch(ff);
  if(earley_analyser)
    genFunTab(ff);
}


void genepilog(FILE * /*ff*/)
{
  // Elan est mort, vive Elan !!!
  // qui est l'auteur ???
}

void genmaintfile(FILE *ff,term maint,int mainstrategy)
{ strategy *ss,**ssa;
 int bis = 0; /* initialised to avoid warning */

 if (mainstrategy != -1) {
    ss = trrules.getstrategy_refs(mainstrategy); 
    ssa = trrules.getstrategyadr_refs(mainstrategy); }
  else {
    ss = NULL; ssa = NULL; }

  if (Bins) fprintf(ff,"\n#define BINS %d\n",Bins);

  fprintf(ff,"#include\"RTCommons.h\"\n\n");
  if(earley_analyser)
    fprintf(ff,"#include\"runtimeInit.h\"\n\n");
  else
    genconstants(ff,"extern ");
  genstratdeclar(ff);
  fprintf(ff,"extern int nofreductions,nofsreductions,nonamed_tried,nofr2,named_tried,number_of_fail;\n"); //nofrt,nofr2,nofsrt,noff

  /*
#ifdef EARLEY
  if(earley_analyser) {
    fprintf(ff,"extern int arity[];\n");
    fprintf(ff,"extern int isconstructor[];\n");
    
    fprintf(ff,"#ifdef __cplusplus\n");
    fprintf(ff,"typedef struct term* (*funTabType)(...);\n");
    fprintf(ff,"#else\n");
    fprintf(ff,"typedef struct term* (*funTabType)();\n");
    fprintf(ff,"#endif\n");
    fprintf(ff,"extern funTabType funTab[];");
  }
#endif
*/

  // Creation du terme de depart
  if(!earley_analyser) {
    fprintf(ff,"struct term *mainterm()\n");
    maint.genrsidedecl(ff,0);
    maint.genrside(ff,0,0);
    fprintf(ff,"\n\n");
  }

  // Variables globales
  fprintf(ff,"\nint Bins = %d;\n\n",Bins);
  fprintf(ff,"\nlong *bp_main;\n\n");

  // Main
  fprintf(ff,"main()\n");
  fprintf(ff,"{\n");
  fprintf(ff,"  long bp;\n");
  fprintf(ff,"  struct term *pp;\n");
  fprintf(ff,"  bp_main=&bp;\n");

  if (!determLink)
    fprintf(ff,"  backTrackInit();\n");

  if(earley_analyser) {
    fprintf(ff,"  tabofidentInit(char_tabofident,char_tabofident_size);\n");
    fprintf(ff,"  typetInit(char_typet,char_typet_size);\n");
    fprintf(ff,"  grammarInit(grammar,GRAMMAR_SIZE);\n");
  }

  fprintf(ff,"  timestart();\n");
  if (!batch) {
    if(earley_analyser)
      fprintf(ff,"  fprintf(%s,\"Query: \\n\");\n",OUTPUTS);
    else {
      fprintf(ff,"  fprintf(%s,\"\\176 [main] start:\\176\");\n",OUTPUTS);
      fprintf(ff,"  fflush(%s);\n",OUTPUTS);
  }
  }
  if (!determLink)
    fprintf(ff,"  if (! setChoicePoint()) {\n");
  if(earley_analyser) {
    fprintf(ff,"  if(!earleyCall(stdin,SOURCETYPE))\n");
    fprintf(ff,"  {fprintf(%s,\"Earley Error\\n\") ; exit(1);}\n",OUTPUTS);
    fprintf(ff,"  pp=popResult();\n");
  }
  else
    fprintf(ff,"  pp=mainterm();\n");
  if ( ss == NULL) {
    if (mainstrategy != -1) {
      fprintf(stderr,"[warning] main strategy was not defined \t!\n");
      fprintf(stderr,"\t empty strategy used\n\n"); }
  } else {
    fprintf(ff,"  pp=str_%s(pp);\n", CONVERT(ssa)); }
  if (!batch) {
  fprintf(ff,"  fprintf(%s,\"\\n[] result term: \\n    \");\n",OUTPUTS);
  fprintf(ff,"  fflush(%s);\n",OUTPUTS);
  }
  if(!earley_analyser)
    bis = maint.isofbuiltintype();
  if(earley_analyser)
    {
      //stout << " builin?=" << ISBUILTIN(sourcetype) << "\n";
      bis=ISBUILTIN(sourcetype);
      fprintf(ff,"  earleyTermWrite(stdout,pp,%d);\n",bis);
      fprintf(ff,"  fflush(stdout);\n");
    }
  else
    {
      fprintf(ff,"  termwrite(pp,%d);\n",bis);
      fprintf(ff,"  fprintf(stdout,\"\\176\");\n");
      fprintf(ff,"  fflush(stdout);\n");
    }
  if (!bis) fprintf(ff,"  freeterm(pp);\n");
  if (!determLink) {
  if (trace && !batch) {
      fprintf(ff,"fprintf(%s,\"[trace] next-solution ::\\n\");",OUTPUTS); 
      fprintf(ff,"fprintf(%s,\"\176\");\n",OUTPUTS);
      fprintf(ff,"fflush(%s);\n",OUTPUTS); }
   genFail(ff,1);
   fprintf(ff,"  } else {\n");
  }
  if (!batch && statis) {
  fprintf(ff,"    fprintf(%s,\"\\n[] end\\n\");\n",OUTPUTS);
  fprintf(ff,"    timestop();\n");
  fprintf(ff,"    statistics(%s);\n",OUTPUTS);
  if (statis) fprintf(ff,"    rulestatistics(%d);\n",big+1); 
  if (!determLink && big)
    fprintf(ff,"    backStatistics();\n");
  fprintf(ff,"    fprintf(%s,\"\\176\");\n",OUTPUTS);
  fprintf(ff,"    fflush(%s);\n",OUTPUTS);
  fprintf(ff,"    exit(0);\n");
  }
  if (!determLink) fprintf(ff,"  }\n"); 
  fprintf(ff,"}\n\n");
}

static int wasident=0;
#define OPTWRITEBLANK() (wasident?" ":"")

int getnum(lstream *ff)
{ lexem lex;
  int sign;
  ff->ilex(lex);
  if (lex == '-') {sign = -1; ff->ilex(lex);} else sign = 1;
  return(sign * lex.numval());
}


/// for pipe

int writereturnedterm(lstream *ff,ochstream &of)
{ lexem lex,*p;
 int lv1; /*,lv2,sign;*/
  int doublep[2];
  ff->ilex(lex);
  while (! lex.isnum() && lex.isnotendofstream()) {
    if (lex == '\177') ff->ilex(lex);
    while (lex!='\177'&& lex.isnotendofstream()) {
      if (lex =='\176') {of << "\n  "; wasident=0;}
      else {
	if (lex.isident()) {
	  of << " " << lex.alfsy();  wasident=1;
	} else {
	  of << OPTWRITEBLANK() << lex.alfsy();  wasident=0;
	}
      }
      of.flush();
      ff->ilex(lex);
    }
    ff->ilex(lex);
  }
  of.flush();
  if (lex.isendofstream()) return(0);
  lv1 = lex.numval();
  if (lv1 < FSYMCODESBEG) {
    if (lv1 == DOUBLECONSTRUCT) {
      ff->ilex(lex);
      // S2: doublep was char*[2]; on LP64 the double was read from the
      // first (sign-extended) pointer only.  The two ints now form the
      // double as they did on the 32-bit hosts of 2004.
      doublep[0] = getnum(ff);
      ff->ilex(lex);
      doublep[1] = getnum(ff);
      double d;
      memcpy(&d,doublep,sizeof(d));
      of << d;
    } else if (lv1 == TIDENT) {
      of << OPTWRITEBLANK() << tabofident.ide(getnum(ff));   wasident=1;
    } else if (lv1 == TNORMFS) {         // a builtin normfs==boolean
      of << OPTWRITEBLANK() << (getnum(ff)?"true":"false");   wasident=1;
    } else {
      of << getnum(ff);   wasident=0;
    }
  } else {
    p = fsymtab[lv1].textform()->rside;
    while ( p->isnotendofstream()) {
      if (p->nonterminal())
	writereturnedterm(ff,of);
      else if (p->isident())
	{
	  of << OPTWRITEBLANK() << p->alfsy();
	  wasident=1;
	}
      else if (!p->isblankk())
	{
	  of << p->alfsy();
	  wasident=0;
	}
      // C'est ici qu'on avance dans la grammaire
      // a la recherche des identificateurs
      p++;
    }
  }
  of.flush();
  return(1);
}















