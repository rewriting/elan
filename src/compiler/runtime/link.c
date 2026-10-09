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
#include "link.h"
#include "match_state.h"
#include "tools.h"

LINK *LINK_create(int size)
{
  LINK *res;
  int i;
  res=(LINK*) IMALLOC(sizeof(LINK));
  res->size=size;
  if(size==0)
    res->ms_tab=(struct match_state**)0;
  else
    res->ms_tab=(struct match_state**) IMALLOC(size*sizeof(struct match_state*));
  for(i=0 ; i<size ; i++)
    res->ms_tab[i]=(struct match_state*)0;
  return res;
}

void LINK_delete(LINK *link)
{
  int i;
  Verif_void(link,"LINK_delete(link)");
  Verif_void(link->ms_tab,"LINK_delete(link->ms_tab)");
  //printf("Destruction du link\n");
  for(i=0 ; i<link->size ; i++)
    if(link->ms_tab[i] != NULL)
      {
	MS_pattern_list_free(link->ms_tab[i]->pattern_list,link->ms_tab[i]->nb_rule); 
	MS_delete(link->ms_tab[i]);
      }
  IFREE(link->ms_tab);
  IFREE(link);
}


void LINK_print(LINK *link)
{
  int i;
  //  Verif_void(link,"LINK_print(link)");
  if(link==0)
    {
      printf(" no link ");
      return;
    }

  for(i=0 ; i<LINK_size(link) ; i++)
    if(LINK_get(link,i) != 0)
      {
	//MS_print(LINK_get(link,i));
	printf("ms");
	printf(" . ");
      }
  else
    printf(" null . ");
}

