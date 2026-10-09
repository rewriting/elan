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
#include "module.h"
#include "termdefs.h"

#include "strategy.h"

union u ststack[];
int ststacki = 0;

void strat::dump(ochstream &och)
{
  switch (kind) {
    case ID_STRAT:
      och << "id";
      break;      
    case FAIL_STRAT:
      och << "fail";
      break;      
    case DC_STRAT:
    case DK_STRAT:
      if (kind == DK_STRAT) och << "dk("; else och << "dc(";
      list_k.subs->dump(och,',');
      och << ")";
      break;
  case FSYM_STRAT:
  case DEF_STRAT:
      och << fsym_k.destr << "/" << fsym_k.constr << "(";
      fsym_k.subs->dump(och,',');
      och << ")";
      break;
  case IF_STRAT:
      och << " if "; if_k.cond->write(och);
      och << " then "; if_k.then->dump(och);
      och << " else "; if_k.els->dump(och);
      break;
  case LET_STRAT:
      och << " let "; let_k.var->write(och);
      och << ":="; let_k.appl->dump(och);
      och << " in "; let_k.instr->dump(och);
      break;
  case RULE_STRAT:
      och << "["; och.flush();
      rule_k.left->write(och);
      och << "=>"; och.flush(); 
      rule_k.anys->dump(och);
      och << "=>"; och.flush(); 
      rule_k.right->write(och);
      och << "]"; och.flush();
      break;
  case SEQ_STRAT:
      seq_k.slist->dump(och,';');
      break;
  case TERM_STRAT:
      term_k.t->write(och);
      break;
  default:
	sterr << "[int.err.2]\n";   
    }
}

Any::Any(term *con)
{
  any_kind = ANY_IF;
  any_if.cond = con;
}

Any::Any(term *va, strat *st)
{
  any_kind = ANY_APPLY;
  any_apply.var = va;
  any_apply.str = st; 
}

void Any::dump(ochstream &och)
{
    switch(any_kind) {
    case ANY_IF: 
	och << " if "; 
        any_if.cond->write(och); och.flush();
	break;
    case ANY_APPLY:
        any_apply.var->write(och); och.flush();
	och << ":"; 
        any_apply.str->dump(och); och.flush();
	break;
    default:
	sterr << "[int.err.2]\n";   
    }
}
    
void Slist::dump(ochstream &och,int sep)
{
Slist *sub = this;
  if (!sub) return;
  while (sub) {
    (sub->s)->dump(och);
    sub = sub->next; 
    if (sub) och << (char)sep; 
  }
}

void Alist::dump(ochstream &och)
{
Alist *sub = this;
  if (!sub) return;
  while (sub) {
    (sub->any)->dump(och);
    sub = sub->next; 
  }
}

void crdefstrat(int rulenum)
{
Slist *strlist = NULL;
strat *str;
int ar,i;
term *t;
lexem *right;
  ar = fsymtab[rulenum].arity(); 
  // QQQ stout << "crdefstrat arity = " << ar << " reulenum = " << rulenum << "\n";

  right = fsymtab[rulenum].textform()->rside; i = 0;
 // QQQ dumpgrrule(fsymtab[rulenum].textform());

  while(ar-- > 0) {
    while (!(right[i].nonterminal())) {
      //QQQ stout << "terminal " << i <<  ","; stout.flush();
	i++;  }

    //QQQ stout << "non-terminal " << i <<  ","; stout.flush();
    
    if (ISSTRATTYPE(typet.ide(right[i].typeval()))) {
    // QQQ stout << "strat-nont " << i << " typeval " << right[i].typeval() << "\n";
    // QQQ stout.flush();
      if (ststacki == 0) {
        sterr << "[crelemstrat] -- ststack underflow\n"; failexit(); }
      NNEW(strlist,Slist(STPOP.s,strlist)); }
    else { // term arument
    //QQQ stout << "term-nont " << i << " typeval " << right[i].typeval() << "\n";
    //QQQ stout.flush();
      NNEW(t,term); t->popt(); NNEW(str,strat(t));
      NNEW(strlist,Slist(str,strlist)); }
    i++;  // next nonterminal
  }
  NNEW(str,strat(DEF_STRAT,rulenum,rulenum,ar,strlist));
  STPUSH_s(str);
}

void crfstrat(int destr, int constr)
{
Slist *strlist = NULL;
strat *str;
int ar1,ar2;
  ar1 = fsymtab[destr].arity(); 
  ar2 = fsymtab[constr].arity(); 
  if (ar1 != ar2) { sterr << "[elemstr] arities are different\n"; failexit();}

  // QQQ stout << "crfstrat arity = " << ar1 
  // << " destr = " << destr 
  // << " constr = " << constr << "\n";

  while(ar1-- > 0) {
      if (ststacki == 0) {
        sterr << "[crelemstrat] -- ststack underflow\n"; failexit(); }
      NNEW(strlist,Slist(STPOP.s,strlist)); }
  NNEW(str,strat(FSYM_STRAT,destr,constr,ar1,strlist));
  STPUSH_s(str);
}

int Appl::dump(ochstream &och)
{
  och << "(";
  s->dump(och);
  och << ")";
  t->write(och);
}

// import strat[intype,outtype]
void load_strat_mod(lstream *f, int sou, int res)
{ char stratmod[IDLEN];
  if (sou == res)
    sprintf(&(stratmod[0]),"%s[%s]",STRAT_MODNAME1,typet.ide(sou));/*,
								     typet.ide(res));  */
  else
    sprintf(&(stratmod[0]),"%s[%s,%s]",STRAT_MODNAME2,typet.ide(sou),
	    typet.ide(res));
  if (! import.member(stratmod)) readmodules(f,stratmod); 
  if (! import.member(stratmod)) {
      sterr << "[load_strat_mod] int.err.\n"; failexit();      }
  topgrammar->addgrammar(* importglobgr[import.posid],RGLOP,RGLOP);
}



