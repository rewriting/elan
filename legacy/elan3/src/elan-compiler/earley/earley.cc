/*
  
    REM - Reduce ELAN Machine

    Copyright (C) 2000-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
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

    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr
    Christophe Ringeissen	e-mail: Christophe.Ringeissen@loria.fr
    Marian Vittek		e-mail: vittek@guma.ii.fmph.uniba.sk

*/

/*                     Parsing by Earley's algorithmes


Parser is realised by two main procedures  "earley" for the first pass and
"earleysecondpass" for the second pass. 

*/


#include "commondefs.h"
#ifndef RUNTIME // begin RUNTIME
#include "termdefs.h"	// because of earleycall(...)
#include "module.h"	// because of prety conflict warning message
#endif // end RUNTIME

int memnew = 0;
int memallo = 0;

//   static because of efficiency reasons but it has to work also for recursive calles

static struct earleystableslist {
  struct ssits *sitset;  	// Earley's sets for input lstream
  int sitseti;
  struct actchooselist *chooselist,**actchoose;
  int ambig_disabled;
  lexem startsymbol;
  struct earleystableslist *next;
} *top = NULL;

static  struct ssits *sitset=NULL;
static  int sitseti=0;
static  struct actchooselist *chooselist=NULL,**actchoose=NULL;
static  int ambig_disabled=0;
static  lexem startsymbol;


#define SEARCHENDSIT(endsit,seti,CONDITION) {struct psymlist *ps; lexem lex; \
  struct sitlist *sls;\
  ps=sitset[seti].sitparsym; lex.crendofstreamlex(); endsit=NULL;\
  FOUND(ps,lex);\
  if (ps!=NULL && lex==ps->symbol) {\
    s=ps->sit;\
    while (s!=NULL) {\
      if (CONDITION) {\
        NNEW(sls,struct sitlist);\
        sls->rule=s->rule; sls->pos=s->pos; sls->i=s->i;\
        sls->next=endsit; endsit=sls;\
      }\
      s=s->nextwss;\
    }\
  }\
}





int grammar::earleycall(lstream *f,lexem startsym,int (*isendofstream)(lexem))
{ int resan;
//  static term ter1;
  struct earleystables *tt;
//sterr << "[";
  resan = earley(*f,startsym,tt,isendofstream);
//sterr << ".";
//  ter1.stinit();
  if (resan) earleysecondpass(f,1,tt);
//sterr << ".";
  earleyfree(tt);
//sterr << "]";
  return(resan);
}

//       following three procedures save and unsaves "state of parser" into "table"
//       all this is because of possible recursive call of
//       earley throw ilex which can call earley in FOR EACH 

static void inittable(struct earleystables *&table)
{
  table->esitset = NULL;
  table->esitseti = 0;
  table->echooselist = NULL;
  table->eactchoose = NULL;
  table->eambig_disabled = 1;
  table->estartsymbol.crblanklex();
}

void grammar::earleyreturn(struct earleystables *&table)
{ struct earleystableslist *tt;
  table->esitset = sitset;
  table->esitseti = sitseti;
  table->echooselist = chooselist;
  table->eactchoose = actchoose;
  table->eambig_disabled = ambig_disabled;
  table->estartsymbol = startsymbol;
  sitset = top->sitset ;
  sitseti = top->sitseti ;
  chooselist = top->chooselist ;
  actchoose = top->actchoose ;
  ambig_disabled = top->ambig_disabled ;
  startsymbol = top->startsymbol ;
   tt = top->next; CFRE(top); 
   top = tt;
}

void grammar::earleyarrival(struct earleystables *&table)
{ struct earleystableslist *tt;
  NNEW(tt ,struct earleystableslist );
  tt -> next = top;
  top = tt;
  top->sitset =sitset ;
  top->sitseti =sitseti ;
  top->chooselist =chooselist ;
  top->actchoose =actchoose ;
  top->ambig_disabled = ambig_disabled;
  top->startsymbol = startsymbol;
  sitset = table->esitset ;
  sitseti = table->esitseti ;
  chooselist = table->echooselist ;
  actchoose = table->eactchoose ;
  ambig_disabled = table->eambig_disabled ;
  startsymbol = table->estartsymbol ;
}



void grammar::inisitset()       // initialize set of situations sitset[sitseti]
{
  sitset[sitseti].actlex.crendofstreamlex();
  sitset[sitseti].sits=NULL;
  sitset[sitseti].aoflastsits= &(sitset[sitseti].sits);
  sitset[sitseti].sitparsym=NULL;
}


int grammar::addtosit(struct sgrammrule *rule, int position, int i)
//                 append situation [(rule, position),i] into sitset[sitseti]
{ register struct sitlist *s,**ss;
  struct psymlist **pp,*p;
  lexem sy;
//stout << "\n[addtosi] try to add rule :";  dumpgrrule(rule);
//stout << " into sitset[" << j<<"] with pos="<< position<<" i="<<i <<"\n";


  sy= rule->rside[position];
  p= sitset[sitseti].sitparsym;

  FOUND(p,sy);
  if (p!=NULL && p->symbol == sy) {
     s = p->sit;
     while (s!=NULL && (s->rule!=rule || s->pos!=position || s->i!=i)) {
       s = s->nextwss;
     }
     if (s!=NULL) return(0);
  }

  ss = sitset[sitseti].aoflastsits;
              // memallo += sizeof(struct sitlist); stout << " #1=" << memallo;
  AALLOS(s,struct sitlist);
  s->rule=rule; s->pos=position; s->i=i;
  s->next=NULL; 

  *(sitset[sitseti].aoflastsits) = s;
  sitset[sitseti].aoflastsits = &(s->next);


//  sy= rule->rside[position];
  if (p == NULL || p->symbol !=sy) {
      pp= &(sitset[sitseti].sitparsym);
      PLACE(pp,sy);
                 // memallo += sizeof(struct psymlist); stout << " #2=" << memallo;
      AALLOS(p,struct psymlist);
      p->next= *pp; p->symbol=sy; p->sit=NULL;
      *pp=p;
  }

  s->nextwss = p->sit;
  p->sit = s;
  return(1);

}

#ifndef RUNTIME // begin RUNTIME
void grammar::pretysitldump(stringtab *types,struct sitlist *p)
                                              // pretty dump of list of situations
{
  stout << "\n";
  while (p!=NULL) {
      stout << "[] at :" << p;
      stout <<" pos= "<< p->pos << "  i= "<< p->i;
      stout <<" nextwss = " << p->nextwss;
      stout << "\t"; writegrrule(stout,p->rule,types);
      p=p->next;
    }
}

void grammar::pretydumpsitset(stringtab *types)
                                              // pretty dump of sitset
{
  struct sitlist *p;
  struct psymlist *ps;
  sterr << "\n[sitsetdump] start of dump";
  for (int i=0; i<=sitseti; i++) {
    sterr << "\n\nsitset["<<i<<"] dump: lexem= " 
         << sitset[i].actlex.alfsy();
    p=sitset[i].sits;
    pretysitldump(types,p);
    ps=sitset[i].sitparsym;
    while (ps!=NULL) {
      sterr << "\n[psymlistdump] : symbol = "
           << ps->symbol.alfsy()
           << "\t sit = " << ps->sit;
      ps=ps->next;
    }
  }
  sterr << "\n[sitsetdump] end of dump\n\n";
}


void grammar::dumpsitset( int i)
                                              // dump of sitset
{
  struct sitlist *p;
  struct psymlist *ps;
  stout << "\n[sitsetdump] start of dump";
//  for (i=0; i<=sitseti; i++) {
    stout << "\nsitset["<<i<<"] dump: lexem= "; sitset[i].actlex.dump();
    p=sitset[i].sits;
    while (p!=NULL) {
      stout << "\n\n[sitlistdump] at :" << p;
      stout << "\n\t"; dumpgrrule(p->rule);
      stout <<"\t pos= "<< p->pos << "  i= "<< p->i;
      stout <<"\t nextwss = " << p->nextwss;
      p=p->next;
    }
    ps=sitset[i].sitparsym;
    while (ps!=NULL) {
      stout << "\n[psymlistdump] : symbol = "; ps->symbol.dump();
      stout << "\n\t sit = " << ps->sit;
      ps=ps->next;
    }
//  }
  stout << "\n[sitsetdump] end of dump\n\n";
}

#endif // end RUNTIME

static unsigned isadded[NNONTERMINALS],addedt[NNONTERMINALS];

void grammar::completesit()
//              complete current sitset (i.e. sitset[sitseti]) with situations
{ struct sitlist *p,*p2;
  struct psymlist *ps;
  struct grammrulelist *gr;
  unsigned i,j,addedti;
struct sgrammrule *rig[10000];
unsigned rigi,rri;
  lexem l,sy;
    addedti =0;
    p=sitset[sitseti].sits;
    while (p!=NULL) {
      l= p->rule->rside[p->pos];
      if (l.nonterminal()) {
#ifdef PEM
	//	sterr <<"completesit : l.typeval="<<l.typeval()<<"\n";
	//	sterr <<"\t l.typeval="<<typet.ide(l.typeval())<<"\n";
#endif 
	i=l.typeval();
	j=isadded[i];
	if (j>=addedti || addedt[j] != i) { //not yet added, I have to do it
	  isadded[i] = addedti;
          addedt[addedti++] = i;
          gr=nontt[i];
          while (gr!=NULL) {
            addtosit(gr->r,0,sitseti);
            gr=gr->next;
          }
	}
      } else if (l.isblankk()) {
        addtosit(p->rule,p->pos+1,p->i);
      } else if (l.isendofstream()) {
         ps= sitset[p->i].sitparsym;
         sy=p->rule->leftside;
         FOUND(ps,sy);
         if (ps!=NULL && ps->symbol == sy) {
           p2=ps->sit;
           while (p2!=NULL) {
             addtosit(p2->rule,p2->pos+1,p2->i);
             p2=p2->nextwss;
           }
         }
      }
      p=p->next;
    }
//    dumpsitset(s);  
}

void grammar::oearleyerr(lstream &f,int sit,int (*isendofstream)(lexem))
{ lexem lex;
  int first,i;
  struct psymlist *ps;
  f.ilex(lex);
  f.oerr();
  sterr << "[earley] syntax error on symbol '" << lex.erralfsy() <<"'";
  sterr << "\n\tone of (";
  ps=sitset[sit].sitparsym; first=1;
  while (ps!=NULL) {
    if ((ps->symbol).terminal() && ps->symbol.isnotendofstream()) {
      sterr << (first?"'":" '") << ps->symbol.erralfsy() << "'";
//sterr << " i.e. lexem "; (ps->symbol).dump();
      first=0;
    }
    ps=ps->next;
  }
  sterr << ") expected";
  sterr << "\n\tin context ";
  for (i= (sit-8 < 1)?1:(sit-8); i<=sit; i++) 
     sterr << sitset[i].actlex.erralfsy() << " ";
  //  sterr << "     " << lex.erralfsy() << "      "; 
  sterr << " >>>" << lex.erralfsy() << "<<< "; 
  f.fulex(lex); sterr<<lex.erralfsy()<<" ";
  for (i=0; i<8 && ! (*isendofstream)(lex); i++) {
      f.ilex(lex); f.fulex(lex); sterr<<lex.erralfsy()<<" ";
  }
  sterr << "\n";
}


void grammar::solveconflicts(struct sitlist *&sis)
{ struct sitlist **s1,**s2,*ss;
  struct sgrammrule *n1,*n2;
  int p1,p2;

  int j1,j2,ok;
  lexem ll1,ll2;

  s1 = &sis;
#ifdef PEM
  //  sterr << "solve conflicts\n";
#endif
  while (*s1!=NULL) {
cont1:
    n1 = (*s1)->rule; p1 = ((*s1)->rule->priority & RPRIORITYMSK);
    s2 = & ((*s1)->next);
    while (*s2!=NULL) {
      n2 = (*s2)->rule;
      p2 = (*s2)->rule->priority & RPRIORITYMSK;
      if ( (*s1)->i == (*s2)->i ) {
#ifdef PEM
// Il faut comparer les profils des operateurs
// on suppose qu ils sont egaux
     ok=1;
     if ( n1->rulenumber == n2->rulenumber)
       {
// comparaison des parties droites
	 j1=0; j2=0;
	 while(ok &&
	       n1->rside[j1].isnotendofstream() &&
	       n2->rside[j2].isnotendofstream())
	   {
//	     cout <<"ok="<<ok<<"j1="<<j1<<"j2="<<j2<<"\n";
	     ll1=n1->rside[j1];
	     ll2=n2->rside[j2];
	     if (ll1.nonterminal() && ll2.nonterminal())
	       {
		 /*
	       cout <<"ll1.alfsy="<<ll1.alfsy()
                    <<"\tll2.alfsy="<<ll2.alfsy()<<"\n";
	       cout <<"ll1.typeval="<<typet.ide(ll1.typeval())
                    <<"\tll2.typeval="<<typet.ide(ll2.typeval())<<"\n";
	       cout <<"ll1.typeval="<<ll1.typeval()
                    <<"\tll2.typeval="<<ll2.typeval()<<"\n";
		    */ 
	       ok&= (ll1.typeval()==ll2.typeval());
	       }
	     j1++;
	     j2++;
	   }
	 // Comparaison de la partie gauche
	      ok&=(n1->leftside.typeval() == n2->leftside.typeval());
       }
     else 
       ok=0;
//     cout<<"ok final="<<ok<<"\n";
     if(ok)
	{
	  ss = *s2; *s2 = ss->next; CFRE(ss); 
	  continue;
	}
#else
        if ( n1->rulenumber == n2->rulenumber ) {
	  ss = *s2; *s2 = ss->next; CFRE(ss); 
	  continue;
	}
#endif
#ifdef PEM
	//cout << "p1=" << p1 << "\t" << "p2=" << p2 << "\n";
	//dumpgrrule(n1);
	//dumpgrrule(n2);
	//sterr << "begin compare\n";
	//sterr << "\t";writegrrule(sterr,n1,&typet);
	//sterr << "\t";writegrrule(sterr,n2,&typet);
	//sterr << "end compare\n";
#endif
// Marian's version
        if ( p1 != RNOPRIOR && p2 != RNOPRIOR ) {
// I think this is a mistake (PEM)
//        if ( p1 != RNOPRIOR || p2 != RNOPRIOR ) {
// la modif perturbe l'execution...
           if (p1 < p2) { ss = *s2; *s2 = ss->next; CFRE(ss); 
                         continue; }
           else {ss = *s1; *s1 = ss->next; CFRE(ss); 
                         goto cont1;}
        } 
      }
      s2 = &((*s2)->next);
    }
    s1 = &((*s1)->next);
  }
}

void grammar::solveconflict2(struct sitlist *&sis,struct sgrammrule *r)
			// solve associativity conflicts 
{ struct sitlist *s,*minis,*ss,**si;
  struct sgrammrule *rn;
  int rp;
  rn = r;
  rp = r->priority & RASSOCMSK;
  if (rp && sis != NULL && sis->next !=NULL ) {
    s = sis; minis = NULL;
    while (s != NULL ) {
      if (s->rule->rulenumber == rn->rulenumber && (minis == NULL || s->i < minis->i) ) {
	minis = s;
      }
      s= s->next;
    }
    if (minis != NULL) {
      if (rp == RLEFTASSOC) {
        si = &sis;
	while (*si != NULL) {
	  if ((*si)->rule->rulenumber == rn->rulenumber) {
	    s = *si; *si = s->next; CFRE(s);
	    continue;
          }
	  si = & ((*si)->next);       
	}
      } else {
	s = sis;
	while (s!= NULL) {
	  ss=s; s= s->next;
	  if (ss != minis) { CFRE(ss);
	  }
	}
	sis = minis; sis->next = NULL;
      }
    }
  }
}


void grammar::oambigwarning(struct sitlist *si)
{
  if (warnings) {
    sterr << "\n[warning]earley: ambiguity can't be solved in string:\n";
    for (int i=1; i<=sitseti; i++) {
     sterr << " " 
       << sitset[i].actlex.alfsy();
    }
    sterr << "\n";
    sterr << "between operators :\n";
    while (si!=NULL) {
      sterr << "\t";writegrrule(sterr,si->rule,&typet);
      si=si->next;
    }
  }
}


int grammar::firsttestcond(struct sitlist *fins)
{
  return(fins->i == 0 && fins->rule->leftside == startsymbol);
}

int grammar::earleyaddnewsits(lexem lex)
{  struct sitlist *s;
    struct psymlist *ps;
    ps=sitset[sitseti-1].sitparsym;
    FOUND(ps,lex);
    if (ps!=NULL && lex==ps->symbol) {
      s=ps->sit;
      while (s!=NULL) {
        addtosit(s->rule,s->pos+1,s->i);
        s=s->nextwss;
      }
      return(1);
    } else return(0);
}


int grammar::earley(lstream &f,lexem starts,struct earleystables *&tables,int (*isendofstream)(lexem))
{ lexem lex,llex,le;;
  int sadded;
  struct sitlist *s;
  struct grammrulelist *rulel;
  NNEW(tables ,struct earleystables); inittable(tables);

//NONBLOCK: stout << "EARLEY\n";

  earleyarrival(tables);
  startsymbol = starts;
             // stout << "(" << (int)((MAXLENNTERMv * sizeof(struct ssits))/1024) << "k)"; stout << "#3 ";
  AALLOSS(sitset ,MAXLENNTERMv, struct ssits);
 

  // stout << startsymbol.typeval() ;

  rulel = nontt[startsymbol.typeval()];
  sitseti=0; inisitset();

  while (rulel!=NULL) {
    addtosit(rulel->r,0,0);
    rulel=rulel->next;
  }
  completesit();
  f.fulex(lex); 

//NONBLOCK
  if (f.isblock() && lex.isendofstream()) {
      f.ilex(lex); }
//NONBLOCK


  do {
    do {
//      sterr << "[earley] lexem readed "; lex.dump(); stout << " !!!! \n";
      sitseti++; 
      if (sitseti >= MAXLENNTERMv) {
       sterr << "term is longer then MAXLENNTERM=" << MAXLENNTERMv
            << " lexems\n\tsorry, FATAL !!!\n";
       failexit();
      }
      inisitset();
      sitset[sitseti].actlex = lex; 

// stout << "[beafore-earley] lexem "; lex.dump(); sterr << "readed\n";

      sadded = earleyaddnewsits(lex); 
//    add situations for identifiers or numbers in general  
      if (lex.isrealid()) {le.cridlex(); sadded=earleyaddnewsits(le)||sadded; }
      if (lex.isrealnum()){le.crnumlex();sadded=earleyaddnewsits(le)||sadded;}
#ifdef STRINGS
      if (lex.isrealstring()){le.crstringlex();sadded=earleyaddnewsits(le)||sadded;}
#endif
      if (! sadded){
        if (!in_runtime) oearleyerr(f,sitseti-1,isendofstream);
        earleyreturn(tables);  
	return(0);
      }
      completesit();
      f.ilex(lex); f.fulex(lex);

// stout << "[earley] lexem "; lex.dump(); sterr << "readed\n";

    } while (! (*isendofstream)(lex));
                         // situations were created, now found a startrules
    //  stout << "$4 ";
    NNEW(chooselist,struct actchooselist);
    chooselist->next=NULL;
    SEARCHENDSIT((chooselist->sits),sitseti,firsttestcond(s));
    solveconflicts(chooselist->sits);
    if (chooselist->sits == NULL) {
      CFRE(chooselist); 
      chooselist=NULL;
    }
  } while (chooselist == NULL);          // no start situation continue trying
                         // add one fictive sits (will be deleted by incrchoose
  struct sitlist *sl;
  // stout << "$5 ";
  NNEW(sl,struct sitlist);
  sl->next = chooselist->sits; chooselist->sits = sl;
  earleyreturn(tables);  return(1);
}

int grammar::testcond(struct sitlist *fins,struct sitlist *acts,int k, lexem xk)
{ struct psymlist *ps;
  struct sitlist *s;
  if (fins->rule->leftside != xk) {
    return(0);
  }
  ps=sitset[fins->i].sitparsym;
  FOUND(ps,xk);
  if (ps!=NULL && xk==ps->symbol) {
    s=ps->sit;
    while (s!=NULL) {
      if (acts->rule->rulenumber == s->rule->rulenumber 
        && s->pos == k && s->i == acts->i) {
          return(1);
      }
      s=s->nextwss;
    }
  } 
  return(0);
} 

void grammar::earleysecprec(lstream *f,struct sitlist *si,int j)
{ int k,l;
  struct sitlist *s,*ss;
  lexem xk;
  if (si->next != NULL && ambig_disabled) 
    if (!in_runtime)
      oambigwarning(si);
  k=si->pos-1; l=j;
  while (k>=0) {
    xk = si->rule->rside[k];
    if (xk.terminal()) { k--; if (! xk.isblankk()) l--;}
    else {
      if (*actchoose == NULL) {
        SEARCHENDSIT(ss,l,testcond(s,si,k,xk));
	solveconflicts(ss);
	solveconflict2(ss,si->rule);
	if (ss==NULL) {sterr << "[earleysecprec] int.err\n";failexit();}
        // stout << "$6 ";
	NNEW(*actchoose ,struct actchooselist);
	(*actchoose)->sits = ss;
	actchoose = &((*actchoose)->next);
	*actchoose = NULL;
      } else {
	ss=(*actchoose)->sits;
	actchoose = &((*actchoose)->next);
      }
      earleysecprec(f,ss,l);
      k--; l=ss->i;
    }
  }

  esemact(f,si->rule->rulenumber,sitset[j].actlex,si->rule->leftside);
    // esemact at the end, because I wish the derivation in reversed order !!! 
}

static int incrchoose(struct actchooselist *act)
{
  if (act == NULL) return(0);
  if (incrchoose(act->next)) return(1);
  struct sitlist *s;
  s = act->sits->next;  CFRE(act->sits);  
  act->sits = s;
  if (s == NULL) {
    CFRE(act);
    return(0);
  } else {
    act->next = NULL;
    return(1);
  }
}


int grammar::earleysecondpass(lstream *f,int ambigdis,struct earleystables *&table)
{ 
  earleyarrival(table);
  ambig_disabled = ambigdis;
  actchoose= &(chooselist->next);
  if (incrchoose(chooselist)) {
    earleysecprec(f,chooselist->sits,sitseti);
    earleyreturn(table);
    return(1);
  } else {
    chooselist = NULL;
    earleyreturn(table);
    return(0);
  }
}

void grammar::earleyfree(struct earleystables *&table)
{ int i;
  struct psymlist *ps,*ps1;
  struct sitlist *s,*s1;
  earleyarrival(table);
  while (chooselist!=NULL) {
    while (chooselist->sits !=NULL) {
      struct sitlist *c; 
      c=chooselist->sits->next; CFRE(chooselist->sits); 
      chooselist->sits=c;
    }
    struct actchooselist *ac;
    ac = chooselist->next; CFRE(chooselist); 
    chooselist = ac;
  }
  for(i=0; i<=sitseti; i++) {
    s=sitset[i].sits;
    FREELIST(s,s1);
    ps=sitset[i].sitparsym;
    FREELIST(ps,ps1);
  }
  CFRE(sitset);
  // Ajoute par PEM (25/06)
  // sitset = NULL;

  earleyreturn(table);
  CFRE(table);
  table = NULL;  
}


/*
grammar::addtosit(int j,struct sgrammrule *rule, int position, int i)
                                          // append situation [(rule, position),i] into sitset[i]
{ register struct sitlist *s,**ss;
  struct psymlist **pp,*p;
  lexem sy;
//stout << "\n[addtosi] try to add rule :";  dumpgrrule(rule);
//stout << " into sitset[" << j<<"] with pos="<< position<<" i="<<i <<"\n";
  ss= &(sitset[j].sits); s = *ss;
  while (s!=NULL && (s->rule!=rule || s->pos!=position || s->i!=i)) {
    ss= &(s->next); s= *ss;
  }
  if (s!=NULL) return(0);             // existing position, nothing to add
sterr << j;
  AALLOS(s,struct sitlist);
  s->rule=rule; s->pos=position; s->i=i;
  s->next=NULL; *ss=s;
  sy= rule->rside[position];
  pp= &(sitset[j].sitparsym);
  PLACE(pp,sy);
  if (*pp == NULL || (*pp)->symbol !=sy) {
        AALLOS(p,struct psymlist);
        p->next= *pp; p->symbol=sy; p->sit=NULL;
        *pp=p;
  }
  *ss=s;
  s->nextwss = (*pp)->sit;
  (*pp)->sit = s;
  return(1);
}
*/

