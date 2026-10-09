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

#include "commondefs.h"
#include <string.h>
#include "command.h"
#include "codes.h"


grammar::grammar()
{ int i;
  for (i=0; i<NNONTERMINALS; i++) {
    nontt[i]=NULL;
    alastnontt[i] = &(nontt[i]);
  }
  buffi=ntbuffi=0; 
  buff[buffi].crendofstreamlex();
}

grammar::~grammar()
{ //if (warnings) {
//    sterr << "[warning] ~grammar() sorry not yet implemented, but don't panic\n";
  //}
}

struct sgrammrule *grammar::addrule(lexem &leftside, int priority, int infos, int num)
{ 
  return addrule_(buff,&buffi,leftside,priority,infos,num);
}

struct sgrammrule *grammar::addrule_(lexem *ebuff,int *ebuffi,lexem &leftside, int priority, int infos, int num)
{ int i;
  lexem *rs;  
  if (num<0) {
    sterr <<"[grammar.c] adding variable by addrule";
    interr();
  }
  AALLOSS(rs ,(*ebuffi)+1,lexem);
  for (i=0; i<=*ebuffi; i++) rs[i]=ebuff[i];
  *ebuffi=ntbuffi=0;
  return(addrul(leftside,rs,priority,infos,num));
}

void grammar::addsymbol(lexem &l) { addsymbol_(buff,&buffi,l); }
void grammar::addsymbol_(lexem *ebuff,int *ebuffi,lexem &l)
 {
  if ((*ebuffi)+2 >= MLENGRRULE) {
     sterr << "length of rule is overflow over MLENGRRULE="
          << MLENGRRULE << "\n\t FATAL !!!\n";
     failexit();
  }
  ebuff[(*ebuffi)++]=l; ebuff[(*ebuffi)].crendofstreamlex();
}

struct sgrammrule * grammar::addrul(lexem &leftside,lexem *body,int priority, int infos ,int num)
{
  struct sgrammrule *gr;
  struct grammrulelist *gl,**ggl;
  AALLOS(gr,struct sgrammrule);
  gr->leftside= leftside;
  gr->priority=priority;
  gr->rulenumber=num;
  gr->rside = body;
  gr->semantic=0; //31
  gr->defstrat=0;
  gr->fsymcode = -1;
  AALLOS(gl ,struct grammrulelist);
  gl->r = gr;
  gl->infos=infos;
/* 
  gl->next = nontt[leftside.typeval()];
  nontt[leftside.typeval()] = gl;
*/


  gl->next= NULL;
  *(alastnontt[leftside.typeval()]) = gl;
  alastnontt[leftside.typeval()] = &(gl->next);

/*
  gl->next= NULL;
  ggl =  & (nontt[leftside.typeval()]);
  while (*ggl != NULL) ggl = &((*ggl)->next);
  *ggl = gl;
/* */
  return(gr);
}


void grammar::addnont(lexem &l) { addnont_(buff,&buffi,l); }
void grammar::addnont_(lexem *ebuff,int *ebuffi,lexem &l)
{
  int ntbuffi = 0;
  while (ntbuffi<(*ebuffi) && ebuff[ntbuffi].isnotendofstream()) ntbuffi++;
  if (ebuff[ntbuffi].isnotendofstream()) 
     sterr<<"[grammar::addnont] something is wrong, int.err.\n";
  ebuff[ntbuffi++]=l;
  if (ntbuffi>*ebuffi) { *ebuffi=ntbuffi; ebuff[*ebuffi].crendofstreamlex(); }
}



void writegrrule(ochstream &cou,struct sgrammrule *gr,stringtab *typet)
{ int j,wast;
  lexem ll;
  j=0;
  while (gr->rside[j].isnotendofstream()) {
    ll=gr->rside[j];
    if (ll.nonterminal()) cou << " @ ";
    else cou << "'"<< ll.alfsy() << "' ";
    j++;
  }
  cou << "\t : ";
  j=0; wast=0;
  while (gr->rside[j].isnotendofstream()) {
    ll=gr->rside[j];
    if (ll.nonterminal()) {
      cou << (wast?" ":"(") << typet->ide(ll.typeval());
      wast=1;
    }
    j++;
  }
  if (wast) cou << ")";
  cou << typet->ide(gr->leftside.typeval());
  if(gr->rulenumber >= 0 )
    {
      if ((gr->priority & RASSOCMSK) == RLEFTASSOC)
	cou << "\t assocLeft ";
      if ((gr->priority & RASSOCMSK) == RRIGHTASSOC)
	cou << "\t assocRight ";
      cou << "\t pri " << (gr ->priority & RPRIORITYMSK);
      cou << " code " << gr->rulenumber;
      if ( gr->semantic) { cou << " sem " << gr->semantic; }
    }
  else
    {
      cou << "\t VAR(" << -gr->rulenumber << ")";
      cou << "\t (pri " << (gr ->priority & RPRIORITYMSK)<<")";
    }
    cou << ";\n";
}

void earleyPrettyDumpGrammarRule(ochstream &stout,struct sgrammrule *gr)
{
  int j;
  stout << "priority ";
  stout << gr ->priority << " "; 
  stout << "rulenumber ";
  stout << gr->rulenumber << " ";

  if (gr->leftside.nonterminal())
    {
      // un type : term, bool ...
      stout << typet.ide(gr->leftside.typeval());
      //stout << "[" << gr->leftside.typeval() <<"]";
      stout << "[" << gr->leftside.numval() <<"]";
    }
  else
    {
      // un symbole : f,g,a,( ...
      stout << gr->leftside.alfsy();
      stout << "cela ne devrait pas arriver\n";
      exit(1);
    }
  stout << " ";
  stout << " -->  ";
  j=0;
  while (gr->rside[j].isnotendofstream())
    {
      if (gr->rside[j].nonterminal()) 
	{
	  // un type : term, bool ...
	  stout << typet.ide(gr->rside[j].typeval()); 
	  //stout << "[" << gr->rside[j].typeval() <<"]";
	  stout << "[" << gr->rside[j].numval() << "]";
	}
      else
	{
	  // un symbole : f,g,a,( ...
	  stout << gr->rside[j].alfsy(); 
	  stout << "[" << gr->rside[j].idval()
		<< "(" << gr->rside[j].numval() << ")" << "]";


	  //stout << "[" << gr->rside[j].idval() <<"]";


	}
      j++;
    }
  stout << "\n";
}

void grammar::earleyPrettyDump(ochstream &stout)
{ struct grammrulelist *gr;
  int i;
  //  stout << "\nrules :\n";
  for (i=0; i<NNONTERMINALS; i++) 
    if (nontt[i]!=NULL) {
      stout <<" for nonterminal ";
      stout << i << "\n";
       gr=nontt[i];
       while (gr!=NULL) { 
	 stout << " info ";
	 stout << gr->infos << "\n";
         earleyPrettyDumpGrammarRule(stout,gr->r);
         gr=gr->next;
       }
    }
  stout << "[end of grammar dump]\n";
}

int earleyDumpGrammarRule(ochstream &stout,struct sgrammrule *gr)
{
  int j;
  int compteur;
  stout << gr ->priority << " , "; 
  stout << gr->rulenumber << " , ";

  if (gr->leftside.nonterminal())
    {
      // un type : term, bool ...
      stout << gr->leftside.numval() << " , ";
    }
  else
    {
      // un symbole : f,g,a,( ...
      stout << gr->leftside.alfsy();
      stout << "cela ne devrait pas arriver\n";
      exit(1);
    }
  // on compte le nombre de membres droits
  compteur=0;
  while (gr->rside[compteur].isnotendofstream())
    compteur++;
  stout << compteur << " , ";

  j=0;
  while (gr->rside[j].isnotendofstream())
    {
      if (gr->rside[j].nonterminal()) 
	{
	  // un type : term, bool ...
	  stout << gr->rside[j].numval() <<" , ";
	}
      else
	{
	  // un symbole : f,g,a,( ...
	  stout << gr->rside[j].numval() <<" , ";
	}
      j++;
    }
  return 4+1+compteur;
}

void grammar::earleyDump(ochstream &stout)
{ struct grammrulelist *gr;
  int i;
  int size=0;
  
  // dump de la  table de symboles
  tabofident.earleyDump(stout,"char_tabofident");

  // dump de la  table de types
  typet.earleyDump(stout,"char_typet");

  // dump des arites
  // dump des symboles constructeurs
  // c'est fait dans compilemisc.c : genfsymtab(FILE *ff)

  // dump de la grammaire
  stout << "int grammar[] = {\n";
  for (i=0; i<NNONTERMINALS; i++) 
    if (nontt[i]!=NULL) {
       gr=nontt[i];
       while (gr!=NULL) { 
	 stout << gr->infos << " , ";
         size+=earleyDumpGrammarRule(stout,gr->r);
	 stout << "\n";
         gr=gr->next;
       }
    }
  // 0 pour finir
  stout << "0};\n";
  stout << "#define GRAMMAR_SIZE " << size << "\n";
}


