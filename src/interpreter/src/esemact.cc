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

#include <string.h>
#include "commondefs.h"
#include "termdefs.h"

#include "module.h"

static term te;

int strategywasapplied = 0;
static int lastIdentVal,lastNumVal;
static char *lastStringVal;
static int lookingforac =0;
int acsymbolinleftside = 0;

void esemactinit()
{
  lookingforac = 1;
  acsymbolinleftside = 0;
}


// Pour libearley : esemact est redefinie dans runtimeInit.c
int esemact(lstream *f,int rulenum,lexem lex,lexem sort)
{
  if (rulenum < 0) {
    te.crvar(-rulenum-1,sort);
    // stout << "var=" << -rulenum-1 << " sort= " << sort.typeval() << "\n";
  }  else {
    if (rulenum > MAXNFSYM) {
      switch (rulenum) {
        case RULECONSTRULE:    //  !!!!!!!!!!!!!!!!!!!!!
        case RULECONSTRULE1:    
	  withrhs = (rulenum == RULECONSTRULE);
	lookingforac = 0;
	break;
      case RIGHTSRULE:
      case STRATCONSTRULE:
        lookingforac = 1;
	acsymbolinleftside = 0;
	strategywasapplied = (rulenum == STRATCONSTRULE);
	break;
      default: { sterr << "\n unknown esemact rule \n"; failexit(); }
      }
    }
    else 
     { switch (rulenum) {
    case IDENTRULE : 
        strIdentVal = lex.idval();
        strIdentLex = lex;
        lastIdentVal = lex.idval();
	break;
    case NUMRULE : 
        lastNumVal = lex.numval();
	break;
    case NUMTOTERM:
	te.crstterm(lastNumVal,TNUMBER);
	break;
    case INTCONSTUMIN:
	te.crstterm(-lastNumVal,TNUMBER);
	break;
    case IDENTTOTERM:
	te.crstterm(lastIdentVal,TIDENT);
	break;
    case STRINGRULE:
        //stout << "ESEMACT STRINGRULE " << lex.stringval() << "\n";
        lastStringVal = lex.stringval(); 
      break;
    case STRINGTOTERM:
        // stout << "ESEMACT STRINGTOTERM " << lastStringVal << "\n";
        te.crststring(strdup(lastStringVal));
        break;
    case NUMTODOUBLE1:
	te.crdouble1();
	break;
    case NUMTODOUBLE2:
	te.crdouble2();
	break;
    case NUMTODOUBLE3a:
	te.crdouble3a();
	break;
    case NUMTODOUBLE3b:
	te.crdouble3b();
	break;
    case NUMTODOUBLE4:
	te.crdouble4();
	break;
    case NUMTODOUBLE5a:
	te.crdouble5a();
	break;
    case NUMTODOUBLE5b:
	te.crdouble5b();
	break;
    case MINNUMTODOUBLE1:
	te.crdouble1();
	te.crdoubleunmin();
	break;
    case MINNUMTODOUBLE2:
	te.crdouble2();
	te.crdoubleunmin();
	break;
    case MINNUMTODOUBLE3a:
	te.crdouble3a();
	te.crdoubleunmin();
	break;
    case MINNUMTODOUBLE3b:
	te.crdouble3b();
	te.crdoubleunmin();
	break;
    case MINNUMTODOUBLE4:
	te.crdouble4();
	te.crdoubleunmin();
	break;
    case MINNUMTODOUBLE5a:
	te.crdouble5a();
	te.crdoubleunmin();
	break;
    case MINNUMTODOUBLE5b:
	te.crdouble5b();
	te.crdoubleunmin();
	break;
    default :
	te.crterm(rulenum);
	if (lookingforac && fsyminfo(rulenum)==FSASSOCCOM)
	  acsymbolinleftside=1;
     }
     }
  }
  return(NORMCONT);
}
