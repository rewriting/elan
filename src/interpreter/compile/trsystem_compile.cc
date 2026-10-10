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

// C code generation (elan -c) for rules, strategies and rewrite systems;
// moved out of trsystem.cc (spec S3a D7). Declarations are unchanged
// (rtdatas.h, compiledefs.h).

#include "rtdatas.h"
#include "module.h"
#include "compiledefs.h"
#include "strategy.h"
#include <string.h>
#include "command.h"

static transrule *actcompiledrule;

// Pour afficher un message une seule fois
static int flag_warning=0;

int builtintype(int typeofstr)
{ lexem lle;
  lle.crtypelex(typeofstr);
  return ISBUILTIN(lle); 
}

static int strategylabel;
static struct transrulelist *actrrules;

void genRulesAppInStrat(FILE *ff,int deep,struct namelist *np,
			int dontcare,int lab,
			int typeofstr,      // type of strategy
			int compilebuiltis, // if 1, left side should be a built in term
			int modul)          // valuable iff typeofstr is BUILIN
{ struct transrulelist *tr,*rrules,*rr,**arr;
/*int n,nameind,i;*/
/*  term l,r;*/
/*  char *name;*/
/*  struct wherelist *wh;*/
 int  anyBuiltins, addit; /* whichmatch, nrules;*/
  struct rtnode *rrt;
  lexem lle; 

  fprintf(ff,"/* genRulesApp-begin */\n");
  lle.crtypelex(typeofstr);
  anyBuiltins=0;
  rrules = NULL; arr = &rrules;
  for (;np!=NULL;np = np->next) {
    tr = trrules.getrules(np->strname,modul);
    if (tr==NULL) {
      if (!batch) {
      fprintf(stderr, "[warning] no rule %s defined\n",
	      trrules.rulename(np->strname)); }
    }
    for (;tr!=NULL;tr=tr->next) {
      if (typeofstr == -1) { //stout << "use marian's method\n" ;
	anyBuiltins =  anyBuiltins || tr->rule->isOnBuiltins() ; }
      else { //stout << "use boro's method\n" ;
//	if (tr->rule->ruletype() != lle) {
        if (tr->rule->ruletypeval() != typeofstr) {
	  if (!batch) {
	    sterr << "\n[warning] strategy " << typet.ide(typeofstr) <<
	      " contains a rule of type " <<
	      typet.ide(tr->rule->ruletypeval()) << "\n"; 
	    tr->rule->dump(5); sterr << "\n";
	  }
	}
	anyBuiltins =  anyBuiltins || builtintype(typeofstr); }
      addit = 1;
      if (addit) {
	AALLOS(rr,struct transrulelist);
	rr ->next = NULL;
	rr->rule = tr->rule->crStratAppRule();
	//   rr->rule->dump(2);
	*arr = rr;
	arr = &((*arr)->next);
      }
    }
  }
  actrrules = rrules; 
  rrt=emptyrt(); mask=1;

  //for (nrules=0,rr=rrules;rr!=NULL;nrules++) rr=rr->next;
  // stout << nrules << " named rules\n";

  for (rr=rrules;rr!=NULL;rr=rr->next) {
    addRuleToCompile(ff,rr->rule,rrt,dontcare,0);
  }

  if ((!dontcare) && (!anyBuiltins)) {
    // if one day strategy of nonBuiltin Rules applied on Built-in, here is a BUG
    intend(ff,deep);
    fprintf(ff,"  setBackAction(CARACTION,1,(void(*)())freeterm,(void*)v1);\n");
  }

  if (rrules==NULL || rrt == NULL) {
    intend(ff,deep); fprintf(ff,"{");
    if (trace && !batch) {
      fprintf(ff,"fprintf(%s,\"[trace] no rules in strategy ::\\n\");",OUTPUTS); 
      fprintf(ff,"fprintf(%s,\"\176\");\n",OUTPUTS);
      fprintf(ff,"fflush(%s);\n",OUTPUTS); }
    genFail(ff,deep);
    intend(ff,deep); fprintf(ff,"}");
  } else {
    makert(rrt,0); // ???anyBuiltins);      // PASSBINS
    if (!optimize) {
      fprintf(ff,"named_tried++;\n");//nofsrt
      if (statis)
	fprintf(ff,"  anofsrt[0]++;\n"); }
    genStratAppBody(ff,rrt,lab,dontcare,(anyBuiltins && !(compilebuiltis)));
    // topfree of leftside of rules
    //  FREELIST(rrules);
    //  freert(rrt);
  }
  fprintf(ff,"/* genRulesApp-end */\n");
}




static int genIterateStrat(FILE *ff,int deep,strategy *sub,int typeofstr)
{
  intend(ff,deep);fprintf(ff,"for(;;){\n");
  intend(ff,deep);
  if (builtintype(typeofstr))
    fprintf(ff,"if(!setChoicePoint()) {break;}\n");             //Bins
  else
    fprintf(ff,"if(!setChoicePoint()) {v1->counter++;break;}\n");
  sub->compile(ff,deep+1,typeofstr);
  intend(ff,deep);fprintf(ff,"}\n");
  return 0;
}



static int genRepeatStrat(FILE *ff,int deep,strategy *sub,int typeofstr)
{
  intend(ff,deep);fprintf(ff,"{int *wr; struct term *tmp;\n");
  intend(ff,deep);fprintf(ff,"for(;;) {\n");
  intend(ff,deep);fprintf(ff,"  wr = (int*) allocStable(sizeof(int));\n");
  intend(ff,deep);fprintf(ff,"  *wr = 0;\n");
  intend(ff,deep);fprintf(ff,"  if(setChoicePoint()){if(*wr){\n");
  genFail(ff,deep+1);
  intend(ff,deep);fprintf(ff,"  } else break;}\n");
  intend(ff,deep);
  if (builtintype(typeofstr)) 
    fprintf(ff,"  tmp = v1; \n");
  else
    fprintf(ff,"  tmp = v1; v1->counter++;\n");
  sub->compile(ff,deep+1,typeofstr);
  intend(ff,deep);
  if (builtintype(typeofstr)) 
    fprintf(ff,"  if (*wr==0) {*wr = 1;}\n");
  else
    fprintf(ff,"  if (*wr==0) {*wr = 1;freeterm(tmp);}\n");
  intend(ff,deep);fprintf(ff,"}}\n");
  return 0;
}


static void genChooseStrat(FILE *ff,int deep,struct strlist *sl,int lab,int isdet,int typeofstr)
{
  if (isdet) {
    intend(ff,deep);
    fprintf(ff,"{int *wasr; struct term *ttmp; wasr=(int*) allocStable(sizeof(int)); *wasr=0;\n");
  }
  while (sl!=NULL) {
    if (sl->next!=NULL) {
      intend(ff,deep); fprintf(ff,"if (!setChoicePoint()) {\n");
/* Marian's original version - BUG-1
      intend(ff,deep); fprintf(ff,"v1->counter++;ttmp=v1;\n");
*/
      intend(ff,deep); 
      if (builtintype(typeofstr)) 
	  fprintf(ff,"\n");
      else
	  fprintf(ff,"v1->counter++;\n");
      if (isdet) {
	  intend(ff,deep); fprintf(ff,"ttmp=v1;\n"); }
/* end of `correction' BUG-1 */
    }
    sl->str->compile(ff,deep+1,typeofstr);
    if (sl->next!=NULL && isdet) {
      intend(ff,deep); 
      if (builtintype(typeofstr)) 
        fprintf(ff,"if (*wasr==0) {*wasr=1;}\n");
      else
        fprintf(ff,"if (*wasr==0) {*wasr=1;freeterm(ttmp);}\n");
    }
    intend(ff,deep); fprintf(ff,"goto slab%d;\n",lab);
    if (sl->next!=NULL) {
      intend(ff,deep); fprintf(ff,"}\n");/*,lab);*/
      if (isdet) {
	intend(ff,deep); fprintf(ff,"if (*wasr) {");
        if (trace && !batch) {
	    intend(ff,deep);
	    fprintf(ff,"fprintf(%s,\"[trace] no-alternatives ::\\n\");",OUTPUTS); 
	    fprintf(ff,"fprintf(%s,\"\176\");\n",OUTPUTS);
	    fprintf(ff,"fflush(%s);\n",OUTPUTS);
	}
        genFail(ff,0); fprintf(ff,"}\n");
      }
    }
    sl=sl->next;
  }
  if (isdet) {
    intend(ff,deep);
    fprintf(ff,"}\n");
  }
}


FILE *inloutfile;
void inlwritefun(lexem lex)
{
  fprintf(inloutfile,"%c",lex.charval());
}
void genInlineStrat(FILE *ff, lbuffer *bb)
{
  inloutfile = ff;
  bb->applyflush(0,inlwritefun);
  fprintf(ff,"\n");
}

void genstratrulennappcode
(FILE *ff,int deep,unsigned long ok,int lab,int dontcare,int isitlastrule)
{
  gennnappcode(ff,deep,actrrules,0,ok,dontcare,1,lab,isitlastrule);
}

void strategy::compile(FILE *ff,int deep, int typeofstr)
{ strategy *st;
/*struct strlist *sl;*/
  int lab, stri;
//dump();
  st = this;
 // stout << "\n ... compiling " << st->strname << " type= " << typeofstr << 
 // "\n"; st->dump();

  while (st !=NULL) {
    lab = strategylabel++; 
    switch (st->strname) {
    case STRNAMEDONTCARE : case STRNAMEDONTKNOW :
      // avec many-to-one matching
      /*
       * Filtrage des regles nommees
       */
      genRulesAppInStrat(ff,deep,st->u.cr.nm,st->strname==STRNAMEDONTCARE, 
			 lab,typeofstr,0,getmodule());

	break;
    case STRNAMEREPEAT: 
	genRepeatStrat(ff,deep,st->u.substrategy,typeofstr);
	break;
    case STRNAMEITERATE :
        genIterateStrat(ff,deep,st->u.substrategy,typeofstr);
	break;
    case STRNAMEONE: case STRNAMEONE2:
    case STRNAMEONECON: case STRNAMEONECON2:
        fprintf(stderr,
	    "[error] strategy one is not yet implemented in compiler\n");
	failexit();
    case STRNAMEDONTCARE2:    case STRNAMEDONTKNOW2:
    case STRNAMEDONTCARECON2: case STRNAMEDONTKNOWCON2:
      // sans many-to-one matching
	genChooseStrat(ff,deep,st->u.stl,lab,
		       st->strname==STRNAMEDONTCARE2,typeofstr);
	break;

    case STRNAMEDCPROCESSCALL: case STRNAMEDKPROCESSCALL:
	fprintf(stderr,"strategy CALL\n not yet implemented in compiler\n");
	interr();	
	break;
    case STRCALL:
        stri = st->subprocmaxn();
        if (trrules.getstrategy_refs(stri) == NULL) {
         sterr << "\n[compile] unknown strategy in call used\n";
         interr(); }
        fprintf(ff,"v1 = str_%s(v1);", CONVERT(stri));
	break;
    case STRMETA:
      fprintf(ff,"v1 = meta_capply(v1);\n");
      fprintf(ff,"if (v1) return v1; else fail();\n");
      break;    
    case STRIDENTITY:
        intend(ff,deep);fprintf(ff," /* identity */ ");
	break;
    case STRFAIL:
        intend(ff,deep);fprintf(ff," /* fail */ ");
	fprintf(ff,"fail();\n");
	break;
    case STRINLINE:
	genInlineStrat(ff,st->u.inlinedbuffer);
	break;
    default : dumpout << "\n[compile] unknown strategy, internal error\n";interr();
    }
    fprintf(ff,"slab%d:;\n",lab);
    if ((trace || tracelevel) && !batch) {
      fprintf(ff,"fprintf(%s,\"[trace] stratlab%d :: \\n\");",OUTPUTS,lab);
      fprintf(ff,"fflush(%s);\n",OUTPUTS);
      if (Bins) 
        fprintf(ff,"termwrite(v1,%d);\n",builtintype(typeofstr));
      else // Marian
        fprintf(ff,"termwrite(v1,STANDARDTERM);\n"); 
      fprintf(ff,"fprintf(%s,\"\176\");\n",OUTPUTS);
      fprintf(ff,"fflush(%s);\n",OUTPUTS);
    }
    st = st->next;
  }
}


void trsystem::compile(FILE *ff)
{
  compilerules(ff);
  compilestrats(ff);
}

void trsystem::compilestrats(FILE *ff)
{ int i;
  for (i=0; i<MAXNOFSTRAT; i++) {
    if (strategies_defs[i]!=NULL) {
   //if (!batch) stout << "compile strategy " << i << "\n";
      fprintf(ff,"\nstruct term *str_%s(\n",
	      remove_underscores(
		trrules.strategyname_defs(&(strategies_defs[i]))));
      ///??? connery --- CONVERT(&(strategies_refs[i])));
      ///??? connery --- CONVERT(&(strategies_defs[i])));
      fprintf(ff,"#ifdef __cplusplus\n");
      fprintf(ff,"struct term *arg)\n#else\n");
      fprintf(ff,"arg)\nstruct term *arg;\n#endif\n");
      fprintf(ff,"{ unsigned long ok; struct term *v1 = arg;\n");
      strategylabel = 0;
      if (trace && !batch) {
        fprintf(ff,"fprintf(%s,\"[trace] entry-%s ::\");",
		OUTPUTS,strategyname_refs(&(strategies_defs[i])));
        if (Bins) 
          fprintf(ff,"termwrite(v1,%d);\n",builtintype(typeofstrategies_defs[i]));
        else // Marian
 	  fprintf(ff,"termwrite(v1,STANDARDTERM);\n");
	fprintf(ff,"fprintf(%s,\"\176\");\n",OUTPUTS);
	fprintf(ff,"fflush(%s);\n",OUTPUTS); }
      // ----
      strategies_defs[i]->compile(ff,0,typeofstrategies_defs[i]);
      // ----
      if (trace && !batch) {
        fprintf(ff,"fprintf(%s,\"[trace] exit-%s ::\");",
		OUTPUTS,strategyname_refs(&(strategies_defs[i])));
        if (Bins) 
          fprintf(ff,"termwrite(v1,BUILTINTERM);\n");
        else
          fprintf(ff,"termwrite(v1,STANDARDTERM);\n"); 
	fprintf(ff,"fprintf(%s,\"\176\");\n",OUTPUTS);
	fprintf(ff,"fflush(%s);\n",OUTPUTS); } 
      fprintf(ff,"return(v1);}\n");
    }
  }
}

void actcruleerror()
{
  fprintf(stderr,"\t in rule:\n");
  actcompiledrule->dump(2);
}

int addRuleToCompile(FILE * /*ff*/,transrule *rule, struct rtnode *&rrt,int isdet,
		     int /*isfunction*/)
{
  /*term wvart;*/
  term rlabel;
  /*strategy *sl;*/
  int varn,name,match,mwhvar,lastrec; /* i, sharetype*/
  struct wherelist *wh,*wwh,*whh;
  struct tseq *rhs;
  term left,right;
  rule->getr(varn,left,right,name,wh,
	     rhs,
	     match,rlabel);
  left.setcompilevar(0);           // mobed up - because of follwoing if
  // rule->dump(1);
  actcompiledrule = rule;
//trrules.dump();
  if (match==ACMATCH) {
    fprintf(stderr,"\n[error] compile: I don't know compile AC matching\n");
    actcruleerror();
    interr();
  }
//rule->dump(0);

//--- I have moved the following line up
//  left.setcompilevar(0);           // set head variable name
  addproto(&rrt,&left);            // add pattern and set left side variables
  for(wwh=wh;wwh!=NULL;wwh=wwh->next) {  // set wheres variables
    if (wwh->leftvarn == WHEREPATTERN) {
      sterr << "\n[fatal] pattern wheres are not compiled yet\n";
      failexit();
    }
    if (wwh->leftvarn != IFVARN) {
      mwhvar = allocVarFirstFree();
      wwh->leftvarterm.setcompilevar(mwhvar);
      allocVarSetUsed(mwhvar);
    }
  }
  for(wwh=wh;wwh!=NULL;wwh=wwh->next) {  // set where terms variables
    wwh->whereterm.marknoshares();	  
    wwh->whereterm.affrsidevars();	  
  }
  right.marknoshares();
  left.searchpfshares(right,1);
  /*
   * Pour eviter un bug
   */
  lastrec=0;
  if(flag_warning==0)
    {
      if (!batch) {
	fprintf(stderr,"*** Warning ***");
	fprintf(stderr,"\tPEM modification, no lastrecursion optimization\n");
      }
      flag_warning=1;
    }
  if (optimize && isdet) 
    left.searchshares(right,FIRSTSTRSHARE);
  right.affrsidevars();                 // set variables of right hand side
  wwh=wh;
  while (wwh!=NULL) {
    left.searchpfshares(wwh->whereterm,0);
    if (wwh->leftvarn == WHEREPATTERN) {
      sterr << "\n[fatal] pattern wheres are not compiled yet\n";
      failexit();
    }
    if (wwh->leftvarn != IFVARN) {
      for(whh=wwh->next;whh!=NULL;whh=whh->next) {
	wwh->leftvarterm.searchpfshares(whh->whereterm,0);
      }
      wwh->leftvarterm.searchpfshares(right,1);
    }
    wwh=wwh->next;
  }
//left.dump(); sterr << "=>"; right.dump(); sterr<< "\n wheres\n";
//for(wwh=wh;wwh!=NULL;wwh=wwh->next) {wwh->whereterm.dump(); sterr<<"\n";}
//sterr << "\n\n";
  mask=(mask<<1) & ~1;
  if (! mask) {
    fprintf(stderr,"[error] sorry, too much rules in many to one matching\n");
    fprintf(stderr,"\tlast rule processed:\n");
    rule->dump(0);
    interr();
  }
  return(lastrec);
}

void trsystem::compilerules(FILE *ff)  // for no-name rules
{ struct transrulelist *tl;
  struct transrulelist *ttl;
  /*term wvart;*/
  term  rlabel;
  /*strategy *sl;*/
  int i,varn,name,match; /*,rulestype,nrules;*/
  struct wherelist *wh; /*,*wwh,*whh;*/
  struct tseq *rhs;
  term left,right;
  struct rtnode * rrt;
  for (i=0; i<MAXNFSYM; i++) {
    tl= nnrules[i]; 
    islastrec[i] = 0; 
    actrrules = nnrules[i]; 
    rrt=emptyrt();
    mask=1; 
    if (tl != NULL) {
      for(ttl=tl; ttl != NULL; )
	ttl=ttl->next;
      //stout << nrules << " of no-named rules for symbol " << i << "\n";
      while (tl!=NULL) {
	tl->rule->getr(varn,left,right,name,wh,
		       rhs,
		       match,rlabel);
        left.termtype(); // rulestype = left.termtype().typeval();
//stout << "compile rule \n --------------------\n" ;
//tl->rule->dump(0);
//stout << "----------------------------\n";
	islastrec[i] |= addRuleToCompile(ff,tl->rule,rrt,1,1);
        tl=tl->next;
      }
      makert(rrt,0); /////////// ????? PASSBINS
//writert(rrt);fprintf(stderr,"\n");
      genfunbody(ff,rrt,islastrec[i]);
      fprintf(ff,"\n");
//    freert(rrt);
    }
  }
}


static void genwherevars(FILE *ff,int deep,struct wherelist *wh)
{ const char *preff;
  intend(ff,deep);
  preff = "struct term ";
  for(;wh!=NULL;wh=wh->next) {
    if (wh->leftvarn == WHEREPATTERN) {
      sterr << "\n[fatal] pattern wheres are not compiled yet\n";
      failexit();
    }
    if (wh->leftvarn!=IFVARN) {
      fprintf(ff,"%s*v%d",preff,wh->leftvarterm.givlsvarnum());
      preff = ",";
    }
  }
  if (*preff == ',') fprintf(ff,";\n");
  else fprintf(ff,"\n");
}

#define GENBACKFREEING()\
  if ((!detWheres) && (!(wh->leftvarterm.isofbuiltintype()))) {\
    intend(ff,deep+1);\
    fprintf(ff,"setBackAction(%sACTION,1,(void(*)())freeterm,(void*)v%d );\n",\
	        (det)?"REV":"CAR",wh->leftvarterm.givlsvarnum());\
  }

static int rulelabel=0;


void genWheresCode(FILE *ff,int deep,struct wherelist *wheres,int det,int lastRule,int isForStrat,int detWheres,int lab)
{ 
  struct wherelist *wh,*wwh;
  wh = wheres;
  intend(ff,deep);fprintf(ff,"{\n");
  genwherevars(ff,deep,wh);
  if (((!det) || (! detWheres)) && !lastRule) {
	intend(ff,deep); 
	fprintf(ff,"if (setChoicePoint()) goto r%dfin;\n",lab);
  }
  while (wh!=NULL) {
//wh->whereterm.dump();
    wh->whereterm.genSaveUnsavePF(ff,deep+1);
    wh->whereterm.genrsidedecl(ff,deep);
    wh->whereterm.genbuildterm(ff,deep+1);
    if (wh->leftvarn == WHEREPATTERN) {
      sterr << "\n[fatal] pattern wheres are not compiled yet\n";
      failexit();
    }
    if (wh->leftvarn == IFVARN) {
      intend(ff,deep+1); 
      if (Bins) {
//BUG   fprintf(ff,"if ( !(((int)"); wh->whereterm.gencorrvalue(); fprintf(ff,"&1))){"); }
	fprintf(ff,"if ( ((int)"); wh->whereterm.gencorrvalue(); fprintf(ff,")==1){"); }
      else {
	//fprintf(ff,"if ( !("); wh->whereterm.gencorrvalue(); fprintf(ff,")) {"); }
	// if(c==false)
	fprintf(ff,"if ( getInt("); wh->whereterm.gencorrvalue(); fprintf(ff,") == 0) {"); }
      if (detWheres) {
	for(wwh=wheres;wwh!=wh;wwh=wwh->next) {
	  if (wwh->leftvarn == WHEREPATTERN) {
	    sterr << "\n[fatal] pattern wheres are not compiled yet\n";
	    failexit();
	  }
	  if (wwh->leftvarn != IFVARN && ! wwh->leftvarterm.isofbuiltintype()) {
	    fprintf(ff,"freeterm( v%d ); ",wwh->leftvarterm.givlsvarnum()); }
	}
	if (trace && !batch) {
	  fprintf(ff,"fprintf(%s,\"[trace] fail in IF ::\\n\");",OUTPUTS); 
	  fprintf(ff,"fprintf(%s,\"\176\");\n",OUTPUTS);
	  fprintf(ff,"fflush(%s);\n",OUTPUTS); }
	if (lastRule) {
	  if (isForStrat) {
	    if (det)
	      fprintf(ff,"freeterm(v1);");  
	    genFail(ff,0);
	  } else {
	    fprintf(ff,"goto norulelab;\n");actnoruleapp =1;
	  }
	} else { 
	  if (trace && !batch) {
	    fprintf(ff,"fprintf(%s,\"[trace] fail in WHERE ::\\n\");",OUTPUTS); 
	    fprintf(ff,"fprintf(%s,\"\176\");\n",OUTPUTS);
	    fprintf(ff,"fflush(%s);\n",OUTPUTS);
	  }
	  if (det) /* CP was not generated */ 
	    fprintf(ff,"goto r%dfin;\n",rulelabel);
	  else /* CP was generated */
	    genFail(ff,0);
	}
      } else {
	 if (trace && !batch) {
	   fprintf(ff,"fprintf(%s,\"[trace] fail in IF/WHERE ::\\n\");",OUTPUTS); 
	   fprintf(ff,"fprintf(%s,\"\176\");\n",OUTPUTS);
	   fprintf(ff,"fflush(%s);\n",OUTPUTS);
	 }
	 genFail(ff,0);
      }
      fprintf(ff,"}\n");
    } else if (wh->strateg==NULL || *(wh->strateg) == NULL) {
      if (wh->strateg!=NULL) {
	fprintf(stderr,"[error] strategy \'%s\' is not defined\n",
		         trrules.strategyname_refs(wh->strateg));
	failexit();
      }
      intend(ff,deep+1); 
      fprintf(ff,"v%d = ",wh->leftvarterm.givlsvarnum());
      wh->whereterm.gencorrvalue();
      fprintf(ff,";\n");
      GENBACKFREEING();
    } else {
      intend(ff,deep+1); 
      fprintf(ff,"v%d = str_%s(",wh->leftvarterm.givlsvarnum(),CONVERT(wh->strateg));
      wh->whereterm.gencorrvalue();
      fprintf(ff,");\n");
      GENBACKFREEING();
    }
    intend(ff,deep);fprintf(ff,"}\n");      // somewhere rested opened par.
    wh = wh->next;
  }
}


static void genFreeing(FILE *ff,int deep,term left,struct wherelist *wh)
{
  left.genfreeleft(ff,deep);
  for(;wh!=NULL;wh=wh->next) {
    if (wh->leftvarn == WHEREPATTERN) {
      sterr << "\n[fatal] pattern wheres are not compiled yet\n";
      failexit();
    }
    if (wh->leftvarn != IFVARN) wh->leftvarterm.genfreeleft(ff,deep);
  }
} 

static int areAllDet(struct wherelist *wh)
{ 
  for(;wh!=NULL;wh=wh->next) {
    if (wh->leftvarn == WHEREPATTERN) {
      sterr << "\n[fatal] pattern wheres are not compiled yet\n";
      failexit();
    }
    if (wh->leftvarn!=IFVARN && wh->strateg!=NULL)
      return(0);
  }
  return(1);
}

/* supposing,that ok has only one bit set !!!!!!!!!!!!!!!!!!!!!!!!!! */
/* so generate application of one rule                               */

void gennnappcode(FILE *ff,
		  int deep,
		  struct transrulelist *actr,
		  int istailrec,
		  unsigned long okmask,
		  int isdet,
		  int isForStrat,
		  int slab,
		  int isItLastRule)
{
  int detWheres;
  int varn,name,match; /* i */
  struct wherelist *wwh; /*,*wh,*wwwh;*/
  term left,right;
  term rlabel;
  /*  char *preff;*/
  struct tseq *rhs;
  while ((okmask & 1)==0) {
    if (actr == NULL || actr->next==NULL) {
      fprintf(stderr,"[trsystem.c] gennappcode, something wrong"); interr();
    }
    actr= actr->next;
    okmask = (okmask& ~1) >> 1;
  }
  rulelabel ++;
//actr->rule->dump(deep);
  actr->rule->getr(varn,left,right,name,wwh,
		   rhs,
		   match,rlabel);
//left.dump();
//fprintf(stderr,"==>\n");
//right.dump();
//for(wh=wwh;wh!=NULL;wh=wh->next) wh->whereterm.dump();
  /* wh = wwh; */
  detWheres = areAllDet(wwh);
  if(isdet) {
    if(!detWheres) {intend(ff,deep+1); fprintf(ff,"CUTOPEN();\n");} 
  }
  genWheresCode(ff,deep,wwh,isdet,isItLastRule,isForStrat,detWheres,rulelabel);
  right.genrsidedecl(ff,deep);
  if(isdet) {
    if(!detWheres) {
      intend(ff,deep+1); fprintf(ff,"CUTCLOSE();\n");
    } 
    genFreeing(ff,deep+1,left,wwh);
/*
  } else 
    if (isItLastRule) {                
    if(! detWheres) {intend(ff,deep); fprintf(ff,"if (noMoreChoicePointsOnTheLevel()) {\n");}
    genFreeing(ff,deep+1,left,wwh);
    if(! detWheres) {intend(ff,deep); fprintf(ff,"} else {\n");}
    right.genSaveUnsavePF(ff,deep);
    if(! detWheres) {intend(ff,deep); fprintf(ff,"}\n");}
*/
  } else {
    right.genSaveUnsavePF(ff,deep);
  }
  if (isForStrat) {
    if (!optimize) {intend(ff,deep); fprintf(ff,"nofsreductions++;\n");
      if (statis)
	fprintf(ff,"  anofsreduction[%d]++;\n",name); }
    right.genbuildterm(ff,deep);
    intend(ff,deep);
    fprintf(ff,"v1 = ");
    right.gencorrvalue();
    fprintf(ff,";\n");
    intend(ff,deep);
    fprintf(ff,"} goto slab%d;\n",slab);
  } else {
    if (!optimize) {
      intend(ff,deep); fprintf(ff,"nofr2++;\n");
      if (statis) {
	  if (name == -1) fprintf(ff,"  anofr2[%d]++;\n",left.head());
          else { sterr << "\n[gennnappcode] - int.err. \n"; failexit(); }
      }
    }
    right.genrside(ff,deep+1,istailrec);
  }
  intend(ff,deep); fprintf(ff,"}\nr%dfin:;\n",rulelabel);
  if(isdet && ! detWheres) {
    intend(ff,deep); fprintf(ff,"CUTCLOSE();\n");
  } 
}


void trsystem::genrwrulennappcode(FILE*ff,int deep,int actfunction,unsigned long ok,int isitlastrule)
{
  if (actrrules != nnrules[actfunction]) {
    fprintf(stderr,"[genrwrulennappcode] trsystem.c ");interr();
  }
  gennnappcode(ff,deep,nnrules[actfunction],islastrec[actfunction],
	       ok,1,0,0,isitlastrule);
}

/* supposing, that ok has only one bit set */

int isconditioned(unsigned long ok)
{ 
  struct transrulelist *ar;
  int varn,name,match;
  struct wherelist *wh;
  term left,right;
  term rlabel;
  struct tseq *rhs;
  ar = actrrules;
  mask = 1;
  if (!ok) {
    fprintf(stderr,"[isconditioned] trsystem.c ");interr();
  }
  while (! (ok & 1)) {
    ar= ar->next;    
    ok = (ok & ~1) >> 1;
  }
  ar->rule->getr(varn,left,right,name,wh,
		 rhs,
		 match,rlabel);
  return(wh!=NULL);
}
