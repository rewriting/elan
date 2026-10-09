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
#include "stringtab.h"
#include "codes.h"
#include "termdefs.h"
#include "RTCommons.h"
#include <errno.h>
#include <unistd.h>

#define IDENTCODE 0
#define TYPECODE 1
#define CHARCODE 2
#define NUMCODE 3


#define STANDPRI 4000  // priority of standards (number & identifiers) 
#define RSTANDOP 010   // built in rule

// PAS NORMAL: devrait etre dans commondefs.h(pourquoi le #ifndef RUNTIME ???)


lexem internIdentType,internIntType ;
lexem internStringType ;


//#include "runtimeInit.h"
extern void esemactinit();

//static stringtab tabofident(3000);
//static stringtab typet(500);
// sizes = MAXNOFIDENT, NTYPES of the interpreter (tests/architecture/check_limits.py)
stringtab tabofident(3000);
stringtab atabofident(3000);
stringtab typet(500);

#include <signal.h>
#include <sys/wait.h>
#include "files.c"

// STANDARD INPUT FILE
ichstream *mainiin = NULL;
lstream   *mainstrm = NULL;

int writewasident = 1;
#define TERMWRITEBLANK() (writewasident?" ":"")

grammar topGrammar;


extern "C" void addstandards()
{ lexem le;

      internIdentType.crtypelex(220);
      internIntType.crtypelex(19);
      internStringType.crtypelex(351);

      le.cridlex(); topGrammar.addsymbol(le); 
      topGrammar.addrule(internIdentType,STANDPRI,RSTANDOP,IDENTRULE);
      le.crnumlex(); topGrammar.addsymbol(le);
      topGrammar.addrule(internIntType,STANDPRI,RSTANDOP,NUMRULE);
      le.crstringlex(); topGrammar.addsymbol(le); 
      topGrammar.addrule(internStringType,STANDPRI,RSTANDOP,STRINGRULE);
}



int qendofin(lexem le)
{
  lexem Send;
  Send.cridlex("end");
  return(le==Send || le.isendofstream());
}

extern "C" void tabofidentInit(char *tab[],int max)
{
  int i;
  for(i=0 ; i<max ; i++)
    tabofident.addstr(tab[i]);
}

extern "C" void typetInit(char *tab[],int max)
{
  int i;
  for(i=0 ; i<max ; i++)
    typet.addstr(tab[i]);
}


extern "C" void grammarInit(int gram[],int max)
{
  int i,pos,nb_rhs;
  lexem le;

  pos=0;
  while(pos < max)
    {
      // partie droite
      nb_rhs=gram[pos+4];
      for(i=0 ; i<nb_rhs ; i++)
	{
       

          switch(gram[pos+5+i*2]) {

          case IDENTCODE: le.cridlex(gram[pos+5+i*2+1]) ;
                          break ;

          case CHARCODE: le.crcharlex(gram[pos+5+i*2+1]) ;
                         break ;

          case TYPECODE: le.crtypelex(gram[pos+5+i*2+1]) ;
                         break ;

          case NUMCODE: le.crnumlex(gram[pos+5+i*2+1]) ;
                         break ;
          }

	  topGrammar.addsymbol(le);
	}
      // partie gauche
      le.crtypelex(gram[pos+3]); 
      topGrammar.addrule(le,gram[pos+1],gram[pos+0],gram[pos+2]);

      pos+=5+nb_rhs*2;
    }
}

/*
 * Actions semantiques d'Earley
 */

/*
extern "C" void esemactinit()
{
  lookingforac = 1;
  acsymbolinleftside = 0;
}
*/

#define EARLEYSIZE 100000
#define DEFAULTRULE 0

int earleyRes[EARLEYSIZE] ;

int earleyKind[EARLEYSIZE] ;

int earleyPos = 0 ;
 
static int lastIdentVal,lastNumVal;

char *earleyString[EARLEYSIZE] ;
char *lastStringVal ;


int esemact(lstream *f,int rulenum,lexem lex,lexem sort)
{
  (void)f; (void)sort;
  if (earleyPos == EARLEYSIZE) {
    sterr << "[esemact] Not enough memory...\n" ;
    exit(-1) ;
  }  else {

    switch (rulenum) {
    case IDENTRULE : 
        lastIdentVal = lex.idval();
	earleyKind[earleyPos] = IDENTRULE ;
	break;
    case NUMRULE : 
        lastNumVal = lex.numval();
	earleyKind[earleyPos] = NUMRULE ;
	break;

    case NUMTOTERM:
	earleyRes[earleyPos++] = lastNumVal ;
	break;

     case IDENTTOTERM:
        earleyRes[earleyPos++] = lastIdentVal ;
	break;

    case STRINGRULE :
        lastStringVal = lex.stringval(); 
	earleyKind[earleyPos] = STRINGRULE ;
        earleyString[earleyPos] = lastStringVal ; // [pem: Jun 15 01]
          //printf("earley STRINGRULE\n");
        break ;

    case STRINGTOTERM:
      lastStringVal = lex.stringval(); // [pem: Jun 15 01]
        earleyString[earleyPos++] = lastStringVal ;
        break ;
    
    default :
       earleyKind[earleyPos] = DEFAULTRULE ;
       earleyRes[earleyPos] = rulenum ; 
       earleyPos++ ;

    }
  }
  return(NORMCONT);
}


int earleyCounter ; // count the number of earleyCall


extern "C" void earleyInit() {
  earleyCounter = 0 ;
}

// [Huy: Oct 17 00] 
extern "C" int earleyCall(FILE *fp,int sort)
{

  lexem le;
  lexem Send ;

  le.crtypelex(sort); 
  esemactinit(); 

  FILES[STDIN_FILENO].status = STATROPENEND;
  if(fp == NULL) {
    NNEW(mainiin,ichstream(stdin,"standard input"));
  } else {
    NNEW(mainiin,ichstream(fp,"file input"));
  }
  NNEW(mainstrm,lstream(mainiin));

  if (earleyCounter>0) mainstrm->ilex(Send) ; // skip the remaining end

  earleyCounter++ ;

  return topGrammar.earleycall(mainstrm,le,qendofin) ;
}



