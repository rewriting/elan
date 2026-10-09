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


#define RESULTLEN  4096
#define REFSTRINGLEN 50000

extern term *refstring2term(char *s);
extern int meta_reduce(char *str, char *t, 
		       int stratsort,
		       char *prog, char *spc,
		       int from, term *res);

extern int meta_reduce_bagof(int cons_fsym, int nil_fsym,
		       char *str, char *t, 
		       int stratsort,
		       char *prog, char *spc,
		       int from, int n, term *res);

// see Meta_strat.eln
/*
  @			: (Strategy) Strategies			code 131;
  @ || @		: (Strategy Strategies) Strategies	code 132;
  @			: (Label) Labels			code 133;
  @ @			: (Label Labels) Labels			code 134;
  dont care choose(@)	: (Labels) Strateg			code 135;
  dont know choose(@)	: (Labels) Strateg			code 136;
  dont care choose(@)	: (Strategies) Strateg			code 137;
  dont know choose(@)	: (Strategies) Strateg			code 138;
  repeat @ endrepeat	: (Strategy) Strateg			code 139;
  Iterate @ endIterate	: (Strategy) Strateg			code 140;
  @ 			: (Strateg) Strategy			code 141;
  @ @			: (Strateg Strategy) Strategy		code 142;
  identity              : Strategy                              code 143;
*/

extern int new_meta_apply(int typ,int cons_fsym, int nil_fsym,
		   term *str, term *t, int from, int n, term *res);
extern int meta_apply(int typ, term *str, term *t, int n, term *res);
extern void term2strategy(int typ,term *t, strategy **s);
extern void term2strateg(int typ, term *t, strategy *s);
extern void term2strategies(int typ, term *t, strlist **s);
extern void term2labels(int typ, term *t, namelist **s);
