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


#define UNUSED   0
#define STATROPEN 1
#define STATWOPEN 2
#define STATAOPEN 3
#define STATROPENEND  4       // read file, end should be read

#ifndef FOPEN_MAX
#define FOPEN_MAX 30
#endif
struct {
    FILE *f;
    lstream *ls;
    int nofreads;
    ochstream *och;
    int  status; } FILES[FOPEN_MAX];

void init_files()
{
int i;
  for(i = 3; i < FOPEN_MAX; i++) FILES[i].status = UNUSED;
  FILES[STDIN_FILENO].nofreads = 1;
}

void close_pid(int pid)
{
  if (FILES[pid].status == UNUSED)
    return;
  if (FILES[pid].status == STATROPEN) {
    if (FILES[pid].ls)
      DELETE1(FILES[pid].ls); 
  } else {
    if (FILES[pid].och)  
      DELETE1(FILES[pid].och); 
  }
  FILES[pid].status = UNUSED;
}

void close_files()
{
int i;
  for(i = 3; i < FOPEN_MAX; i++) close_pid(i);
}

int find_fid()
{
int i;
  for(i = 3; i < FOPEN_MAX; i++)
    if (FILES[i].status == UNUSED) return i;
  return 0;
}

