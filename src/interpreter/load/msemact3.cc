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

#include "commondefs.h"
#include "termdefs.h"
#include "module.h"
#include "strategy.h"
#include <string.h>
#include "codes.h"

extern int handlestrategydefinition(lstream *f);


 struct RPair { struct sgrammrule *r1, *r2; struct RPair *next; }; // ld.fsymrules

int    inlinecodesi = 0;
std::vector<struct INLINES> inlinecodes;
int    ninlines = 0;

int is_def_str(int from, int to)
{
  int i;
  for (i=0; i<inlinecodesi; i++)
    if (inlinecodes[i].from == from && inlinecodes[i].to == to) return i;
  return -1;
}

int apply_code(int from, int to) SEARCHCODE(applycode)
int inline_code(int from, int to) SEARCHCODE(inlin)

int inverse_apply_code(term t, int *from, int *to)
{
int i; 
int ind = t.head();
 for (i = 0; i < inlinecodesi; i++) 
   if (inlinecodes[i].applycode == ind) {
     *to = inlinecodes[i].to; *from = inlinecodes[i].from; return 1; }
 return 0;
}  

int is_inlin(int cde)      SEARCHINDEX(inlin,cde)
int is_inlineplus(int cde) SEARCHINDEX(inlineplus,cde)
int is_let(int cde)        SEARCHINDEX(let,cde)

int stratsort2index(int cde) { 
 int i; 
 for (i = 0; i < ld.spairi; i++) if (ld.spair[i].stratsort == cde) return i; 
  return -1; 
}

int add_strat_nont(int pos1,int pos2)
{int  r, i; /*pom,*/

  make_new_nonts(ld.strategy_type,STRATEGY,typet.ide(pos1),typet.ide(pos2)); 
  r = typet.addstr(ld.strategy_type);
  for(i = 0; i < ld.spairi; i++)
    if (ld.spair[i].from == pos1 && ld.spair[i].to == pos2) break;
  if (i >= ld.spairi) {
    if (ld.spairi >= (int)ld.spair.size()) ld.spair.resize(ld.spairi + 1);
    ld.spair[ld.spairi].from = pos1;
    ld.spair[ld.spairi].to = pos2;
    ld.spair[ld.spairi].stratsort = r; 
    ld.spairi++;
  }
  return r;
}

//------------------------------------------- PROFI_STACK 
void push_profistck(int p)
{
   ld.profistcki++;
   if (ld.profistcki >= (int)ld.profistck.size()) ld.profistck.resize(ld.profistcki + 1);
   ld.profistck[ld.profistcki] = p;
 }

int top_profistck()
{ return ld.profistck[ld.profistcki]; }

int pop_profistck()
{
  if (ld.profistcki >= 0)
    return ld.profistck[ld.profistcki--];
  else {
    sterr << "[profiles] int.err.\n"; failexit(); }
}
//-------------------------------------------- PROFI_TABLE

//--- compares two profiles
int profitab::equal(struct ilist *ilistptr1, struct ilist *ilistptr2)
{ 
  while (ilistptr1 != NULL && ilistptr2 != NULL) {
    if (ilistptr1->i > 0 && ilistptr2->i > 0) {   // should be a profile
       if (ilistptr1->i != ilistptr2->i) return FALSE; }
    else if  (ilistptr1->i <= 0 && ilistptr2->i <= 0) { // type
       if (!equal(tab[-ilistptr1->i],tab[-ilistptr2->i])) return FALSE; }
    else return FALSE;
    ilistptr1 = ilistptr1->next;
    ilistptr2 = ilistptr2->next;
  }
  return (ilistptr1 == NULL && ilistptr2 == NULL);
}

void profitab::profil2str(struct ilist *ilistptr, char *str)
{
  struct ilist *pom = ilistptr;
  str[0] = 0;
  strcat(str,SYMBOL); strcat(str,"[");
  while (pom) {
    if (strlen(str) + 10 > STRLEN) {
      sterr << "[fatal] profile too long\n"; failexit(); }
    //stout << typet.ide(pom->i) << "\n";
    if (pom->i > 0) strcat(str,typet.ide(pom->i));
    //// else QQQQQQQ not finished
    pom=pom->next;
    if (pom) strcat(str,",");
  }
  strcat(str,"]");
}

int profitab::add_profil(struct ilist **ilistptrptr, int *found)
  {
   int i = 0;
   struct ilist *ilistptr = *ilistptrptr;
   struct ilist *pom;
   *found = FALSE;
   if (profinum +1 >= PROFITABSIZE) {
      sterr << "[profil.add_profil] int.err\n"; failexit();
    }
   while (i < profinum) {
       if (!equal(tab[i],ilistptr)) i++; else break; }
    if (i < profinum) {                 // found
      while (ilistptr) {
	pom = ilistptr->next; CFRE(ilistptr); ilistptr = pom;
      }
      *ilistptrptr = tab[i]; *found = TRUE;
      return -i; }
    else {
      tab[profinum] = ilistptr; 
      return -(profinum++);
    }
  }
//-------------------------------------------------------
int handlestrategybody(lstream *f)
{ lexem le, applex, selflex, reslex;
  term ls, rs, resvar, *rlab;
  struct sgrammrule *mainrule1,*rightsrule1;
  struct sgrammrule *mainrule2,*rightsrule2;
  int resan; /* ,whi;*/
  char *strnam;                      // temp variable for rulenames

  applex.crtypelex(add_strat_nont(ld.actruletypeindex_l,ld.actruletypeindex));
  selflex.crtypelex(ld.actruletypeindex_l);
  reslex.crtypelex(ld.actruletypeindex);

  // right1 => Y
  modframes[stacki].gr.addsymbol(reslex); le.crtypelex(RIGHTSTYPE);
  rightsrule1=modframes[stacki].gr.addrule(le,RNOPRIOR,RNOINFO,STRATCONSTRULE);
  // rule1 => right1 =>right1
  le.crtypelex(RIGHTSTYPE); modframes[stacki].gr.addsymbol(le);
  le.crcharlex('='); modframes[stacki].gr.addsymbol(le);
  le.crcharlex('>'); modframes[stacki].gr.addsymbol(le);
  le.crtypelex(RIGHTSTYPE); modframes[stacki].gr.addsymbol(le);
    le.crtypelex(STARTTYPE); 
  mainrule1=modframes[stacki].gr.addrule(le,RNOPRIOR,RNOINFO,RULECONSTRULE); 

  // right2 => <X->Y>
  modframes[stacki].gr.addsymbol(applex); le.crtypelex(STRATTYPE);
  rightsrule2=modframes[stacki].gr.addrule(le,RNOPRIOR,RNOINFO,RIGHTSRULE);
  // rule2 => right2 =>right2
  le.crtypelex(STRATTYPE); modframes[stacki].gr.addsymbol(le);
  le.crcharlex(']'); modframes[stacki].gr.addsymbol(le);
  le.crtypelex(STRATTYPE); modframes[stacki].gr.addsymbol(le);
    le.crtypelex(STARTTYPE); 
  mainrule2=modframes[stacki].gr.addrule(le,RNOPRIOR,RNOINFO,RULECONSTRULE);

  esemactinit();
  resan = modframes[stacki].gr.earleycall(f,le,endofin);
  if (! resan) return(resan);

///...ADDING
  ld.lside.popt(); ld.rside.popt(); 

  ld.rside.remove_inlines();

  //stout << "strategy was applied = " << strategywasapplied << "\n";

  if (!strategywasapplied) {
    ls.stinit(); 
    ls.crvar(0,selflex); // should be self 
    ls.pusht(ld.lside); 
    ls.crterm(apply_code(ld.actruletypeindex_l,ld.actruletypeindex),2);
    ls.popt();
    //ls.write(stout); stout.flush();
    //--------------
    rs.stinit(); 
    rs.crvar(0,selflex); // should be self 
    rs.pusht(ld.rside); 
    rs.crterm(apply_code(ld.actruletypeindex_l,ld.actruletypeindex),2);
    rs.popt();
  } else { ls = ld.lside; rs = ld.rside; }
  NNEW(rlab, term);
  strnam = attach_type("DSTR",ld.actruletypeindex); 

  if (strategywasapplied) {

  ld.acttrrule = trrules.addrule(strnam,ld.actvtabi,ls,rs,
		    stratmoduli(ld.actruletypeindex_l,ld.actruletypeindex),
		    RGLOP,     // AAA
	            NULL,
	            (acsymbolinleftside?ACMATCH:NORMMATCH),*rlab,NULL);  
  //stout << "##3 ##"; acttrrule->dump(0); stout << "\n";
  }
  else {
  //------------------------ to be able to remove repeat from the strategy eval
  resvar.stinit();
  resvar.crvar(ld.actvtabi,reslex);
  resvar.popt();
  ld.acttrrule = trrules.addrule(strnam,ld.actvtabi+1,ls,resvar,
		    stratmoduli(ld.actruletypeindex_l,ld.actruletypeindex),
		    RGLOP,     // AAA
	            NULL,
	            (acsymbolinleftside?ACMATCH:NORMMATCH),*rlab,NULL);  
  //stout << "##4 ##"; acttrrulelab->dump(0); stout << "\n";
  ld.dstr_rs = rs;
  }
  modframes[stacki].gr.deleterule(mainrule1);
  modframes[stacki].gr.deleterule(rightsrule1); 
  modframes[stacki].gr.deleterule(mainrule2);
  modframes[stacki].gr.deleterule(rightsrule2);
  return(resan);
}

int semact3(int n,lexem l,lstream *f)
{ lexem le,lle;
  int pos, infos, apli;
  /* struct sgrammrule *gr;*/
  switch (n) {
  case 318:
     ld.selectorn = typet.addstr(ld.actimp->s);
     CFRE(ld.actimp->s); CFRE(ld.actimp); ld.actimp=NULL;
    break;
  case 319:// __ :: (imodule1) imodule2;  -- parsing a simple type profi
    ld.selectorn = 0;				      
    [[fallthrough]];
  case 320:      // case 319 should be without break;
     ACTIMP2POS;
     if (ld.selectorn) {
       ld.selectors[ld.numb_selectors].type = pos;
       ld.selectors[ld.numb_selectors].possition = ld.sel_poss;
       ld.selectors[ld.numb_selectors].name = ld.selectorn;
       ld.numb_selectors++; }
     ld.sel_poss++;
     push_profistck(pos);
     if (ld.profi_level <= 1 /*only topmost level*/) {
       if (ld.profi_lev == 1 /*arg type*/) {
         ld.actprofis++;
         le.crtypelex(pos);
         modframes[stacki].gr.addnont(le);
       } else {
         ld.actleftside.crtypelex(pos);
         if (ld.actarity!=ld.actprofis) {
           sterr <<
	   "\n[semact] arity of definition is not compatible with the profil";
           return(ERRORIM); }
       } }
     break;
  case 321: case 323: // <__> :: (imodule1) imodule2;  -- strategy profile
		      // <__ -> __> :: (imodule1 imodule1) imodule2;
    {
      /*struct ilist *pom = NULL;*/
     ACTIMP2POS;

     // syntactic convention removed ..........
     if (n == 321) ld.pos_l = pos;  // if <X> ....
 
     pos = add_strat_nont(ld.pos_l,pos); // code of the sprofil
     push_profistck(pos);
     le.crtypelex(pos);
     if (ld.profi_level <= 1 /*only topmost level*/) {
       if (ld.profi_lev == 1 /*arg type*/) {
         ld.actprofis++;
         modframes[stacki].gr.addnont(le);
       } else {
         ld.actleftside = le;
       }
     }
     break;
    }
  case 324: // symbol -> in <X->Y>
     ACTIMP2POS;
     ld.pos_l = pos;
     break;
  case 329: 
    ld.numb_selectors = 0; ld.sel_poss = 0;
    break;
  case 330:
    if (!alpha_syntax) {
      f->owarn("\n[syntax] strategy definition probably in the old syntax\n",NULL);
      return(ERRORIM); }
    break;
    /********* 0606
 case 331:  // ( :: lsprofil                // begin of profile
    push_profistck(NOPROFIL);               // bottom of the stack
    profi_level++;
    profi_lev++;
    numb_selectors = 0; sel_poss = 0;
    break;
 case 332: // ) :: rsprofil                 // end arity of profile
    profi_lev--;
    break;
    **********/
 case 333:
    f->fulex(le);

    ld.acttrrulelab = NULL; ld.acttrrule = NULL;

    ld.stratindex = ld.strattype; 
    pos = modframes[stacki].gr.lookbuiltincode(ld.actruletypeindex_l, ld.actruletypeindex, le, &infos);
    ld.rinfos = infos;
    if (pos > 0) { term ls, rs, resvar, *rlab;
      lexem applex, selflex, reslex;
      char *strnam;             
      apli = apply_code(ld.actruletypeindex_l,ld.actruletypeindex);
      if (apli != -1) { // the module strat[x,y] has been used
	// modframes[stacki].gr.add_apply_code(actruletypeindex_l,actruletypeindex);
        // add_stratmoduli(actruletypeindex_l,actruletypeindex);
	// apli = apply_code(actruletypeindex_l,actruletypeindex); }
        applex.crtypelex(add_strat_nont(ld.actruletypeindex_l,ld.actruletypeindex));
	selflex.crtypelex(ld.actruletypeindex_l);
	reslex.crtypelex(ld.actruletypeindex);
      
	ls.stinit(); 
	ls.crvar(0,selflex); // should be self 
	ls.crterm(pos,0);

	ls.crterm(apli,2);
	ls.popt();
	//ls.write(stout); stout.flush();
	//-------------
	rs.stinit(); 
	rs.crvar(0,selflex); // should be self 
	rs.popt();
	NNEW(rlab, term);
	strnam = attach_type("DSTR",ld.actruletypeindex);

	resvar.stinit();
	resvar.crvar(1,reslex);
	resvar.popt();
      
	ld.acttrrule = trrules.addrule(strnam,2,ls,resvar,
		    stratmoduli(ld.actruletypeindex_l,ld.actruletypeindex),
		    RGLOP,     // AAA
	            NULL,
	            (acsymbolinleftside?ACMATCH:NORMMATCH),*rlab,NULL);  
        // stout << "QQQQQ" << le.alfsy() << "--" << typet.ide(actruletypeindex_l) << "--" << import.ide(impmoduli) << "\n";
	//stout << "##5 ##"; acttrrule->dump(0); stout << "\n";

        ld.actwhstrategy = trrules.strategyindex_refs(
		attach_type_mod(le.alfsy(),ld.actruletypeindex_l,impmoduli));
        ld.acttrrule->addwhere(reverse_wheres,1,(ld.actwhstrategy==-1)?((strategy**)NULL):
			  trrules.getstrategyadr_refs(ld.actwhstrategy),
			  rs,reslex);
	ld.actwhstrategy = -1; // 3005
      // stout << " CONVERT RULE \n"; acttrrule->dump(0); 
    }
    }
    else { // defined
      if (! handlestrategydefinition(f)) return(HANDERRORIM);
    }
    break;
 case 334:                                // end of topmost profile
    //   __ : __  __    ::(ssymbollist,sprofil,atribl) strdef1      
    //   __ : __        ::(ssymbollist,sprofil) strdef1             
    if (ld.profi_level > 0 || ld.actarity != ld.actprofis) {
      f->owarn("\n[semact] arity of strategy is not compatible with the profil\n",NULL);
      return(ERRORIM); }
    ld.profi_level = 0; ld.actprofis = 0;
    break;
 case 335: // operators
   ld.in_stratop = 0;
   break;
 case 336: // operators
   ld.in_stratop = 1;
   break;
 case 340:                               // body in strategy definition
    // strategy __ endstr body :: (rwstr,strdeflist) rest2    
    // body              :: rest2                    
    ld.acttrrulelab = NULL; ld.acttrrule = NULL;
    if (! handlestrategybody(f)) return(HANDERRORIM);
    break;
 case 341:  case 342:                           // type of strategy
  // <__ -> __>     :: (imodule1,parrow,imodule1) stype
  //  __           	:: (imodule1) stype
     ACTIMP2POS;

     // syntactic convention removed ..........
     if (n == 342) {
      ld.pos_l = pos;  
      ld.stratindex = pos; // KKKKKKKKK
      ld.strattype = pos;
     }// if <X> ....
 
     ld.actruletypeindex_l = ld.pos_l; ld.actruletype_l.crtypelex(ld.pos_l); //:s
     ld.actruletypeindex = pos;   ld.actruletype.crtypelex(pos);

     if (ld.actvtabi+1 >= MAXNOFVAR) {
       f->owarn("\n[semact] too much local variables in rule\n",
       "more then MAXNOFVAR\n",NULL);
       return(ERRORIM);
     }
     lle.cridlex("self"); ld.actvtab[ld.actvtabi] = lle;
     ld.vartab[ld.actvtabi]=modframes[stacki].gr.addvarrule(ld.actruletype_l,// self : s
                      ld.actvtab[ld.actvtabi],VARSPRI,RVAR,-ld.actvtabi-1);
     ld.dollar_vartab[ld.vartabi]=modframes[stacki].gr.adddollarvarrule(ld.actruletype_l,
			    ld.actvtab[ld.vartabi],VARSPRI,RVAR,-ld.vartabi-1);
     ld.actvtabi++;
    ld.vartabi++;
     break; 
 case 351:                               // varident in the rule label
				         //  __ ::( identifier ) varname1
   if (ld.actlvtabi >= MAXNOFVAR) {
     f->owarn("\n[semact] too much local variables in rule\n","more then MAXNOFVAR\n",NULL);
     return(ERRORIM); }
   ld.actlvtab[ld.actlvtabi++]=l;
   break;
                                      //__ := [       ::(beforewhere2)beforewhere   
 case 353:                                     // processing of new where
   { int resan,strx,stry;
   /*char *strnam;*/
   resan = modframes[stacki].gr.earleycall(f,ld.actwheretype,endofin);
   if (! resan) return(HANDERRORIM);
   ld.ter1.popt();
   // S2: strx/stry were used uninitialised when ter1 is not an applied code
   if (!inverse_apply_code(ld.ter1,&strx,&stry)) interr();
   trrules.strategyremove_refs(ld.actstratindex);	
   ld.actwhstrategy = trrules.strategyindex_refs(
               attach_type_mod(EVALSTR,ld.actwheretype.typeval(),
			   evalmoduli(strx,stry)));
//EEEE			   evalmoduli(actwheretype.typeval())));
   if (ld.acttrrule) { // elan rule
     ld.acttrrule->addwhere(reverse_wheres,ld.actwherevar,(ld.actwhstrategy==-1)?((strategy**)NULL):
			 trrules.getstrategyadr_refs(ld.actwhstrategy),
			 ld.ter1,ld.actwheretype);
   }
   if (ld.acttrrulelab) { // strategy LAB_ rule
     ld.ter1.copyrec(ld.ter2);
     ld.acttrrulelab->addwhere(reverse_wheres,ld.actwherevar,(ld.actwhstrategy==-1)?((strategy**)NULL):
		    trrules.getstrategyadr_refs(ld.actwhstrategy),ld.ter2,ld.actwheretype);
   } 
   ld.actwhstrategy = -1;
   break;
  }
 case 360:                                      // begin of strategy
                                                //  strategy :: RWstrategy
//  KKKK
   ld.rinfos=RLOCOOP;modframes[stacki].importrinfos = RLOCOOP;
   ld.in_strategies = 1;
   create_nested();
//   listofrules = NULL;
   break;

  case 361:
  {
    ACTIMP2POS;
    if (ld.Gtypestacki >= MAXGTYPESTACK) {
      sterr << "\n[semact] sorts nested too deeply, more than MAXGTYPESTACK="
            << MAXGTYPESTACK << "\n\t fatal\n";
      failexit(); }
    ld.Gtypestack[ld.Gtypestacki++] = pos;
    break;
  }
  case 362: 
  {
    int pos1;
    pos1 = ld.Gtypestack[--ld.Gtypestacki];
    pos = add_strat_nont(pos1,pos1);
    ld.Gtypestack[ld.Gtypestacki++] = pos;
    break;
  }
  case 363:
  {
    int pos1, pos2;
    pos2 = ld.Gtypestack[--ld.Gtypestacki];
    pos1 = ld.Gtypestack[--ld.Gtypestacki];
    pos = add_strat_nont(pos1,pos2);
    ld.Gtypestack[ld.Gtypestacki++] = pos;
    break;
  }
  case 364:
  {
    ld.actvartype.crtypelex(ld.Gtypestack[--ld.Gtypestacki]);
    break;
  }
  case 365:  case 366:
  { int pos1, pos2;

     if (n == 365) {
       pos1 = pos2 = ld.Gtypestack[--ld.Gtypestacki];
     } else {
       pos2 = ld.Gtypestack[--ld.Gtypestacki];
       pos1 = ld.Gtypestack[--ld.Gtypestacki];
     }
     ld.stratindex = pos2;
     ld.strattype = pos2;
 
     ld.actruletypeindex_l = pos1; ld.actruletype_l.crtypelex(pos1); //:s
     ld.actruletypeindex = pos2;   ld.actruletype.crtypelex(pos2);

     if (ld.actvtabi+1 >= MAXNOFVAR) {
       f->owarn("\n[semact] too much local variables in rule\n",
       "more then MAXNOFVAR\n",NULL);
       return(ERRORIM);
     }
     lle.cridlex("self"); ld.actvtab[ld.actvtabi] = lle;
     ld.vartab[ld.actvtabi]=modframes[stacki].gr.addvarrule(ld.actruletype_l,// self : s
                      ld.actvtab[ld.actvtabi],VARSPRI,RVAR,-ld.actvtabi-1);
     ld.dollar_vartab[ld.vartabi]=modframes[stacki].gr.adddollarvarrule(ld.actruletype_l,
			    ld.actvtab[ld.vartabi],VARSPRI,RVAR,-ld.vartabi-1);
     ld.actvtabi++;
    ld.vartabi++;
    break; 
  }
  case 367: case 368: 
   { int pos1, pos2;
    if (n == 367) {
       pos1 = pos2 = ld.Gtypestack[--ld.Gtypestacki];
     } else {
       pos2 = ld.Gtypestack[--ld.Gtypestacki];
       pos1 = ld.Gtypestack[--ld.Gtypestacki];
     }
     ld.actruletypeindex_l = pos1; ld.actruletype_l.crtypelex(pos1);
     ld.actruletypeindex = pos2;   ld.actruletype.crtypelex(pos2);
     break;
   }
  case 369:
    ld.selectorn = typet.addstr(l.alfsy());
    break;
  case 370:
     typecheck(f,ld.actimp->s);
     pos = typet.addstr(ld.actimp->s); 
     if (ld.selectorn) {
       ld.selectors[ld.numb_selectors].type = pos;
       ld.selectors[ld.numb_selectors].possition = ld.sel_poss;
       ld.selectors[ld.numb_selectors].name = ld.selectorn;
       ld.numb_selectors++; }
//     sel_poss++;
     break;
 }
 return(NORMCONT);
}


//... ADDING
void create_fsymterm(lexem *rside, term &t, int &varc)
{
lexem lle;
  if (rside->isnotendofstream()) {
      create_fsymterm(rside+1,t,varc);
      lle.crtypelex(rside->typeval());
      if (rside->nonterminal()) t.crvar(varc++,lle);
  }
}

// ... TRANSITIVE CLOSURE
void trclos(lstream *f)
{ int i,j,k;
char strnam[STRLEN];
    for (i=0; i<inlinecodesi; i++)
	 for (j=0; j<inlinecodesi; j++) {
	     if (inlinecodes[i].to == inlinecodes[j].from) {
		 for (k=0; k<inlinecodesi; k++)
		     if (inlinecodes[j].to == inlinecodes[k].from && 
			 inlinecodes[i].from == inlinecodes[k].to) {
			 snprintf(strnam,sizeof(strnam),"%s[%s,%s,%s]",STRAT_MODNAME3,
			                       typet.ide(inlinecodes[i].from),
			                       typet.ide(inlinecodes[j].from),
			                       typet.ide(inlinecodes[k].from));
//			 if (!
//			     (inlinecodes[i].from == inlinecodes[j].from) && 
//			     (inlinecodes[j].from == inlinecodes[k].from)) {

		           importmod_inf(strnam,f,
					 stratmoduli(inlinecodes[i].from,
						inlinecodes[k].from),RGLOP); 
//}
		     }
	     }
	 }
}


int gr_compatible(struct sgrammrule *r1, struct sgrammrule *r2)
{
  lexem *p1,*p2;
  if (r1 == NULL || r2 == NULL) return FALSE;
  if (r1->priority != r2->priority) return FALSE;
  if (is_def_str(r1->leftside.typeval(),r2->leftside.typeval())== -1)
    return FALSE;

//dumpgrrule(r1);
//dumpgrrule(r2);

//  stout << " .....> " << 
//      "{" << r1 << "," << r2 << "}" <<
//      "<" << r1->leftside.typeval()<<","<< r2->leftside.typeval() <<
//      "[" << r1->rulenumber << ","<< r2->rulenumber << "]\n";


  //stout << "++";

  p1 = r1->rside; p2 = r2->rside;
  while (p1->isnotendofstream() && p2->isnotendofstream()) {

    if (p1->nonterminal() && p2->nonterminal()) {
      if (is_def_str(p1->typeval(),p2->typeval())==-1) return FALSE;
    } else if (*p1 != *p2) return FALSE;
    p1++; p2++;
  }
  if (p1->isendofstream() && p2->isendofstream()) return TRUE;
  else return FALSE;
}

int fsymrule_exists(struct sgrammrule *r1, struct sgrammrule *r2)
{
struct RPair *p;
  for(p = ld.fsymrules; p; p=p->next)
    if (p->r1 == r1 && p->r2 == r2) return 1;
//    else stout << "[" << p->r1 << "," << p->r2 << "]"
//		 << "!=[" << r1 << "," <<     r2 << "]\n";
  return 0;
}

void fsymrule_add(struct sgrammrule *r1, struct sgrammrule *r2)
{
struct RPair *p;
  NNEW(p, struct RPair);
  p->r1 = r1; p->r2 = r2; p->next = ld.fsymrules; ld.fsymrules = p;
}

struct sgrammrule *add_fsymrule(struct sgrammrule *gr1,struct sgrammrule *gr2,
				int /*infos*/)
{
  lexem *rs1,*rs2,lle;
  struct sgrammrule *gr;
  int ari = 0;

  rs1 = gr1->rside; rs2 = gr2->rside;
  while (rs1->isnotendofstream()) { 
    if (rs1->nonterminal()) {
        ari++;
//      stout << "<" << rs1->typeval() << "," << rs2->typeval() << ">";
      lle.crtypelex(add_strat_nont(rs1->typeval(),rs2->typeval()));
      modframes[stacki].gr.addsymbol(lle); }
    else
	modframes[stacki].gr.addsymbol(*rs1); 
    rs1++; rs2++;
  }
//  stout << " => " << 
//       "<" << gr1->leftside.typeval()<<","<< gr2->leftside.typeval() << ">";
  lle.crtypelex(
      add_strat_nont(gr1->leftside.typeval(),gr2->leftside.typeval()));
  gr=modframes[stacki].gr.addrule(lle,
			     gr1->priority+DELTA_PRIOR,ld.rinfos,
			     /////////rulepri+DELTA_PRIOR | finfos2,rinfos,
			     ld.actcode);

// stout << " => " << gr << "rule = " << actcode-1 << "\n";

  add_to_fsymtab(ari,gr,FSYM_FLAG(gr1->rulenumber,gr2->rulenumber));

  return gr;
}

void add_frule(struct sgrammrule *gr, 
	       struct sgrammrule *gr1,
	       struct sgrammrule *gr2)
{
  term ter,t1,t2,t3,ls,*rlab;                         // t1[t2] => t3
  int varc,j1,j2,j3;
  char *strnam;
  int whstr;
  lexem *rsid1,*rsid2, lle, whtype, strlex; /* *rsid0, */
  transrule *tr;

  varc = 0;
  t1.stinit(); create_fsymterm(gr->rside,t1,varc); 
  t1.crterm(gr->rulenumber); t1.popt(); 
  //stout << "r1 = "; t1.write(stout); stout << "\n";

  t2.stinit(); create_fsymterm(gr1->rside,t2,varc); 
  t2.crterm(gr1->rulenumber);t2.popt();                       
  //stout << "r2 = "; t2.write(stout); stout << "\n";

  t3.stinit(); create_fsymterm(gr2->rside,t3,varc); 
  t3.crterm(gr2->rulenumber);t3.popt();    
  //stout << "r3 = "; t3.write(stout); stout << "\n";

  ls.stinit(); ls.pusht(t2); ls.pusht(t1);
  ls.crterm(apply_code(gr1->leftside.typeval(),gr2->leftside.typeval()),2); 
                           // arity is important, 
			   // because APPLY_CODE is not defined yet
  ls.popt();
  // stout << "[" << gr1->rulenumber << "(" << gr1 << ")" 
  //	<< ":" << gr2->rulenumber << "(" << gr2 << ")\n";
  // ls.write(stout); stout << "===>"; t3.write(stout); stout << "\n";

  strnam = attach_type("FSYM",gr2->leftside.typeval());
  NNEW(rlab,term);

  tr=trrules.addrule(strnam,varc,ls,t3,
		     stratmoduli(gr1->leftside.typeval(),gr2->leftside.typeval()),
		     RGLOP,     // AAA
		     NULL,
		     (acsymbolinleftside?ACMATCH:NORMMATCH), *rlab,NULL);  
  //tr->dump(5);
  /* rsid0 = gr->rside; */ rsid1 = gr1->rside; rsid2 = gr2->rside; 
  j1 = varc/3; j2 = 2*j1; j3 = varc;
  while (rsid1->isnotendofstream()) { 
    if (rsid1->nonterminal()) {
      j1--; j2--; j3--;
      whstr = trrules.strategyindex_refs(
           attach_type_mod(EVALSTR,rsid2->typeval(),
			   evalmoduli(rsid1->typeval(),rsid2->typeval())));
// EEEE			   evalmoduli(rsid2->typeval())));
      whtype.crtypelex(rsid2->typeval()); 
      lle.crtypelex(rsid1->typeval()); 
      strlex.crtypelex(add_strat_nont(rsid1->typeval(),rsid2->typeval()));
      ter.stinit(); 
      ter.crvar(j2,lle); 
      ter.crvar(j1,strlex); 
      ter.crterm(apply_code(rsid1->typeval(),rsid2->typeval()),2);
      ter.popt();
      tr->addwhere(0,j3,(whstr==-1)?((strategy**)NULL):
		       trrules.getstrategyadr_refs(whstr),ter,whtype);
    }
    /* rsid0++; */ rsid1++; rsid2++;
  }
}


char *remove_underscores(const char *s)
{
  char *r,*rr;
  AALLOSS(r ,strlen(s)+1,char);
  rr = r; strcpy(r,s); 
  while (*rr) {
   if (*rr == '[' || *rr == ']' || *rr == ',' || *rr == '.' || 
       *rr == TYPE_SEPAR || *rr == MODULE_SEPAR)
     *rr = '_';
   rr++; }
  return r;
}

int handlestrategydefinition(lstream *f)
{ lexem le, applex, selflex, reslex;
  term ls, rs, resvar, *rlab;
  struct sgrammrule *mainrule2,*rightsrule2;
  int resan; /*,whi;*/
  char *strnam;                      // temp variable for rulenames

  applex.crtypelex(add_strat_nont(ld.actruletypeindex_l,ld.actruletypeindex));
  selflex.crtypelex(ld.actruletypeindex_l);
  reslex.crtypelex(ld.actruletypeindex);

  // right2 => <X->Y>
  modframes[stacki].gr.addsymbol(applex); le.crtypelex(STRATTYPE);
  rightsrule2=modframes[stacki].gr.addrule(le,RNOPRIOR,RNOINFO,RIGHTSRULE);
  // rule2 => right2 =>right2
  le.crtypelex(STRATTYPE); modframes[stacki].gr.addsymbol(le);
  le.crcharlex('='); modframes[stacki].gr.addsymbol(le);
  le.crcharlex('>'); modframes[stacki].gr.addsymbol(le);
  le.crtypelex(STRATTYPE); modframes[stacki].gr.addsymbol(le);
  le.crtypelex(STARTTYPE); 
  mainrule2=modframes[stacki].gr.addrule(le,RNOPRIOR,RNOINFO,RULECONSTRULE);

  esemactinit();
  resan = modframes[stacki].gr.earleycall(f,le,endofin);
  if (! resan) return(resan);

///...ADDING
  ld.lside.popt(); ld.rside.popt(); 

  ld.rside.remove_inlines();

  ls.stinit(); 
  ls.crvar(0,selflex); // should be self 
  ls.pusht(ld.lside); 
  ls.crterm(apply_code(ld.actruletypeindex_l,ld.actruletypeindex),2);
  ls.popt();
  //ls.write(stout); stout.flush();
  //--------------
  rs.stinit(); 
  rs.crvar(0,selflex); // should be self 
  rs.pusht(ld.rside); 
  rs.crterm(apply_code(ld.actruletypeindex_l,ld.actruletypeindex),2);
  rs.popt();
  NNEW(rlab, term);
  strnam = attach_type("DSTR",ld.actruletypeindex); 

  //------------------------ to be able to remove repeat from the strategy eval
  resvar.stinit();
  resvar.crvar(ld.actvtabi,reslex);
  resvar.popt();

  ld.acttrrule = trrules.addrule(strnam,ld.actvtabi+1,ls,resvar,
		    stratmoduli(ld.actruletypeindex_l,ld.actruletypeindex),
		    RGLOP,     // AAA
	            NULL,
	            (acsymbolinleftside?ACMATCH:NORMMATCH),*rlab,NULL);  

  // stout << "##7 ##"; acttrrule->dump(0); stout << "\n";


  /*
  stout << "WWWWWWWWW\n";
  rs.write(sterr); sterr << "\n";
  acttrrule->dump(0);
  stout << "MMMMMMMMM\n";
  */
  ld.dstr_rs = rs;

  modframes[stacki].gr.deleterule(mainrule2);
  modframes[stacki].gr.deleterule(rightsrule2);
  return(resan);
}


