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
/*
		(c)	INRIA-Lorraine & CRIN
			615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
			email: elan@loria.fr

	$Source: /CVS/aircube/elan3/src/elan-compiler/earley/termdefs.h,v $
	$Revision: 1.1.1.1 $
	$Date: 2001/11/29 08:28:44 $
	$Author: pem $
*/

#ifndef termdefs_h
#define termdefs_h
#include "commondefs.h"

#include <unistd.h> // pour pid_t


/* ------------------------------------------------------------- */

		// values of info record in fsymtab
#define FSNOINFO 0       
#define FSCOMM 1          // op is marked as commutative
#define FSASSOCCOM 2      // op is marked as assoc. comm.

		// values of info records in term structure, 
                // !!!!! can not be 0 and has to be smaller than FSYMCODESBEG
#define TVAR 1		// term is a variable
#define TNORMFS 2	// term is normal functional symbol
#define TIDENT 3	// term is of standart type identifier
#define TNUMBER 4	// term is of standart type number
#define TSTRING 5

// #define BINTaxpredicateset "axpredicateset"

	//		matching algorithm used
#define ACMATCH 0
#define NORMMATCH 1

#define NORENAME -1          /* value for no renamed variable in rule */

#define ADDRENAME 0          /* arguments for vars renaming functions */
#define CHECK 1

struct processdata {
  int counter;               // counter of callings
  pid_t pid;		     // pid of proc. 
  ochstream *pin;            // input of pipe
  lstream *s;               // output from pipe
  ichstream *is;            // output from pipe (just to can close it)
  struct processdatalist ** actplist;  // from which list of processus is taken
  int noblocking;              // read is blocking or not
};

struct processdatalist {
  struct processdata *pd;
  struct processdatalist *next;
};

#endif // end RUNTIME

