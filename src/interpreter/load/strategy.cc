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

// import strat[intype,outtype]
void load_strat_mod(lstream *f, int sou, int res)
{ char stratmod[IDLEN];
  if (sou == res)
    snprintf(&(stratmod[0]),sizeof(stratmod),"%s[%s]",STRAT_MODNAME1,typet.ide(sou));/*,
								     typet.ide(res));  */
  else
    snprintf(&(stratmod[0]),sizeof(stratmod),"%s[%s,%s]",STRAT_MODNAME2,typet.ide(sou),
	    typet.ide(res));
  if (! import.member(stratmod)) readmodules(f,stratmod); 
  if (! import.member(stratmod)) {
      sterr << "[load_strat_mod] int.err.\n"; failexit();      }
  topgrammar->addgrammar(* importglobgr[import.posid],RGLOP,RGLOP);
}



