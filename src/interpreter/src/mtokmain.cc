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

stringtab tabofident(MAXNOFIDENT);
stringtab atabofident(MAXNOFIDENT);

struct iilist {
  int i; 
  struct iilist *next;
};

int main()
{ int i;
  ichstream mainin(stdin,"stdin");
  struct iilist *ll,*l;
  lstream ff(&mainin);
  lexem lex;

  //init_alloc();

  ff.ilex(lex);
  i=0;
  ll=NULL;
  stout << "\n#include \"commondefs.h\"\n\n";
  stout << "stringtab tabofident(" <<  MAXNOFIDENT;
  while (! lex.isendofstream()) {
    if (!lex.isident()) {
       ff.oerr("[mtokmain] identifier expected, int. err.");
       failexit();
    }
    stout << ",\"" << tabofident.ide(lex.idval()) << "\"";
    AALLOS(l ,struct iilist);
    l->next = ll; l->i = lex.idval(); ll= l;
    if (!((++i)%5)) stout << "\n";
    ff.ilex(lex);
  }
  stout << ", NULL);\n";
  stout << "\n\nmitab mrwt(MRWTSIZE,";
  while (ll!=NULL) {
    stout << ll->i << ",";
    ll = ll->next;
  }
  stout << "NOINT);\n";
  return(0);
}
