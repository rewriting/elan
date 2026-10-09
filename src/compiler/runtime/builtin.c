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
#include "builtin.h"


struct TabFile* tabfile;

void Ginit_builtin() {
    int i;
  tabfile = (struct TabFile*) malloc(sizeof(struct TabFile));
  for (i=0; i<MAXFILE;i++){  //[QUANG: Dec 19 01] 
      tabfile_free_case[i]=1;
  }
  tabfile->files[0] = stdin;
  tabfile_free_case[0]=0;
  tabfile->files[1] = stdout;
  tabfile_free_case[1]=0;
  tabfile->files[2] = stderr;
  tabfile_free_case[2]=0;

}

