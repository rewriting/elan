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


#include "mitab.h"
#include <stdio.h>
#include "alloc.h"
#include "streams.h"
#include <stdarg.h>

void mitab::stinit(int size)
{
  AALLOSS(id,size , struct recstr);
  tsize= size;
  init();
}

mitab::mitab(int size)
{
  stinit(size);
}

mitab::mitab(int size, int strs ... )
{ 
  va_list ap;
  va_start (ap,strs);
  int p;
  stinit(size);
  p=strs;
  while (p>0) {
   addn(p);
   p=va_arg(ap,int);
  }
  va_end(ap);
}

 mitab::~mitab()
{
  CFRE(id);
}

void mitab::init()
{ int i;
  for (i=0; i<tsize; i++) { id[i].n = NOINT;id[i].nofoc=0;}
  nin=0;
}

int mitab::member(int n)
{
  posid=n;
  posid=posid % tsize;
  while (id[posid].n != NOINT) {
    if (id[posid].n == n) return(1);
    posid=(posid+211) % tsize;
  }
  return(0);
}

int mitab::addn(int s)
{
  if (s == NOINT) {
     sterr << "[addn] ERROR trying to add NOINT value into mitab\n"
          << " int. err.\n";
     failexit();     
  }
  if (!member(s)) {
    if (nin+1 >= tsize) {
      sterr << "[addn] ERROR, mi tab overflowed over " << tsize
           << "\n int.err.\n";
      failexit();
    }
    nin++;
    id[posid].n = s;
    id[posid].nofoc = 0;
  }
  id[posid].nofoc ++;
  return(posid);
}

void mitab::removenonp(int p)  
                             // remove has to be made in reversed order to add
{
  id[p].nofoc--;
  if (id[p].nofoc < 0) {
	sterr << "[removen] trying to remove non ex. number\n"
             << "int.err.\n";
	failexit();
  }
  if (id[p].nofoc) return;
  id[p].n = NOINT;
  nin--;
}

void mitab::removen(int n)  
                             // remove has to be made in reversed order to add
{
  member(n);
  removenonp(posid);
}

/*
void mitab::write(char *bef, char *aft)
{ int i;
  for (i=0; i<tsize; i++)
    if (id[i]!=NULL) 
       stout <<"\n#define RW" << id[i] << " " << bef << i << aft;
}
*/

void mitab::dump(char *name)
{ int i;
  stout << "mitab " << name <<  "(" << tsize << "," << nin << "\n"; 
  for (i=0; i<tsize; i++) {
    if (id[i].n==NOINT) stout << ",";
    else stout << "," << id[i].n;
    if (!((i+1)%20)) stout << '\n';
  } 
  stout << ");\n";
}


void mitab::forinit()
{
  forpos= -1;
  fornext();
}

void mitab::fornext()
{
  if (forpos<tsize) forpos++;
  while (forpos<tsize && id[forpos].n == NOINT ) forpos++;
}

int mitab::forcond()
{
  return(forpos<tsize);
}

int mitab::foractval()
{
  return(id[forpos].n);
}
