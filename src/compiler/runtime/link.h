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

*/
#ifndef _link_h
#define _link_h
#include "termCommon.h"
//#include "match_state.h" CA NE MARCHE PAS AVEC !!!

/*
 * un link (LINK *) est un tableau de match_state*
 */

typedef struct LINK
{
  int size;
  struct match_state **ms_tab;
} LINK;


extern LINK *LINK_create(int size);

extern void LINK_delete(LINK *link);
#define LINK_set(link,pos,ms) (link)->ms_tab[pos]=(ms);
#define LINK_get(link,pos) ((link)->ms_tab[pos])
#define LINK_clear(link,pos) (link)->ms_tab[pos]=0;

extern void LINK_print(LINK *link);

#define LINK_size(link) ((link)->size)
#define LINK_set_size(link,s) (link)->size=(s);

#endif








