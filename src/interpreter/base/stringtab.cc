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

#include "stringtab.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "alloc.h"
#include "streams.h"
#include "misc.h"

void stringtab::stinit(int size)
{
  AALLOSS(id,size , char *);
  tsize= size;
  init();
}

stringtab::stringtab(int size)
{
  stinit(size);
}

stringtab::stringtab(int size, const char *strs ... )
{ 
  va_list ap;
  va_start (ap,strs);
  const char *p;
  stinit(size);
  p=strs;
  while (p!=NULL) {
   addstr(p);
   p=va_arg(ap,const char *);
  }
  va_end(ap);
}

 stringtab::~stringtab()
{
  CFRE(id);
}

void stringtab::init()
{ int i;
  for (i=0; i<tsize; i++) id[i]=NULL;
  strin=0;
}

int stringtab::member(const char *s)
{ const char *p;
  p=s; posid=0;
  while (*p) posid=posid+ *(p++);
  posid=posid % tsize;
  while (id[posid] != NULL) {
    if (!strcmp(s,id[posid])) return(1);
    posid=(posid+211) % tsize;
  }
  return(0);
}

int stringtab::index(const char *s)
{ 
  if (member(s)) return posid;
  else return -1;
}

int stringtab::addstr(const char *s)
{
  if (!member(s)) {
    if (strin+1 >= tsize) {
      sterr << "[addstr] ERROR, string tab overflowed over " << tsize
           << "!!! \t !!!!!!!!!\n";
      failexit();
    }
  strin++;
  id[posid] = mstrdup(s);
  }
  return(posid);
}

void stringtab::removestr(int n)  
                             // remove has to be made in reversed order to add
{
// stout << "\tremoving string " << id[n] << "from idt\n";
  strfree(id[n]); id[n]=NULL;
  strin--;
}

void stringtab::write(const char *bef, const char *aft)
{ int i;
  for (i=0; i<tsize; i++)
    if (id[i]!=NULL) 
       stout <<"\n#define RW" << id[i] << " " << bef << i << aft;
}

void stringtab::dump(const char *name)
{ int i;
  stout << "stringtab " << name <<  "(" << tsize << "," << strin << "\n"; 
  for (i=0; i<tsize; i++) {
    if (id[i]==NULL) stout << ",NULL";
    else stout << ",\"" << id[i] << "\"";
    if (!(i%5)) stout << '\n';
  } 
  stout << ");\n";
}

void stringtab::earleyDump(ochstream &stout,const char *name)
{
  int i;
  stout << "char *"<< name<<"[" <<tsize <<"] = {\n";

  if (id[0]==NULL) stout << "\"\"";
  else stout << "\"" << id[0] << "\"";
  for (i=1; i<tsize; i++) {
    stout << ",";
    if (id[i]==NULL) stout << "\"\"";
    else stout << "\"" << id[i] << "\"";
    if (!(i%15)) stout << '\n';
  } 
  stout << "\n};\n";
  // #define toto_size 500
  stout << "#define " << name << "_" << "size " << tsize << "\n"; 
}

void stringtab::forinit()
{
  forpos= -1;
  fornext();
}

void stringtab::fornext()
{
  if (forpos<tsize) forpos++;
  while (forpos<tsize && id[forpos]==NULL ) forpos++;
}

int stringtab::forcond()
{
  return(forpos<tsize);
}

int stringtab::forindex()
{
  return(forpos);
}

char *stringtab::foractval()
{
  return(id[forpos]);
}
