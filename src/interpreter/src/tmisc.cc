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


void fsymtabdump()
{ int i;
  stout << "[fsymtabdump]begin\n";
  for (i=FSYMCODESBEG; i<fsymtabi; i++) {
    stout << "i="<< i<< " "; fsymtab[i].dump();
    writegrrule(stout,fsymtab[i].textform(),&typet);
    stout << "\n";
  }
  stout << "[fsymtabdump]end\n";
}
/*
 * sert a afficher un terme dans la syntaxe locale de la grammaire
 * du module qui appel un processus
 */

void remake_alias(struct sgrammrule *gr)
{
// toto mi robilo problemy, lebo ja mam pravidla s kodmi viac ako FSYMTABSIZE,
// a tvoja valcovacia rutina na to kasle, ale mozno sa mylim
//#ifdef BUGS
  if (gr->rulenumber < FSYMTABSIZE)
//#endif
    fsymtab[gr->rulenumber].new_textf_for_alias(gr); 
}

void fsymtab_remakealias(grammar *gr)
{
  gr->gr_rule_mapp(0,remake_alias);
}





