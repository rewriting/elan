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


lbuffer::lbuffer()
{
  firstch.next = NULL; 
  lastch = &firstch;
  lastch->b = lastch->e = 0;
  NNEW(lastch->l ,lexem[CHUNKSIZE]);
}

lbuffer::~lbuffer()
{
  clear();
}

void lbuffer::put(lexem le)
{
//sterr << "putting lexem :"; le.dump(); sterr << "\n";
  if (lastch->e == CHUNKSIZE) {
    NNEW(lastch->next ,struct lbufchunk);
    NNEW(lastch->next->l ,lexem[CHUNKSIZE]);
    lastch = lastch->next;
    lastch->b = lastch->e = 0;
    lastch->next = NULL;
  }
  lastch->l[(lastch->e)++] = le; 
}

void lbuffer::clear()
{ struct lbufchunk *b,*bb;
//sterr << "[lbuffer::clear]\n";
  bb=firstch.next;
  while (bb!=NULL) {
    delete [] bb->l;
    b=bb; bb=bb->next; DELETE1((b));
  }
  firstch.next=NULL; 
  lastch = &firstch;
  lastch->b = lastch->e = 0;
}

int lbuffer::isempty()
{
  return(lastch == &firstch && firstch.b==firstch.e);
}

void lbuffer::get(lexem &le)
{ struct lbufchunk *lb;
//sterr << "getting lexem :"; firstch.l[(firstch.b)].dump();
//sterr<<"\t b = " << firstch.b << "\n";
  if (firstch.b == firstch.e) {
    sterr << "[lbuffer::get] try to get from empty buff, internall error\n";
    failexit();
  }
  le = firstch.l[(firstch.b)++];
  if (firstch.b == firstch.e ) {
   if (firstch.next ==NULL )  		// buffer is empty
     firstch.b = firstch.e = 0;
   else {	
    lb = firstch.next;
				//    firstch = *lb;
    delete [] firstch.l;
    firstch.l = lb->l;
    firstch.b = lb->b;
    firstch.e = lb->e;
    firstch.next = lb->next;
    if (firstch.next == NULL) lastch = &firstch;
    DELETE1((lb));
  }
  }
}

/*
		in following two functions int parameter is 
		unused, it was added  because of bug in gnu C++ compiler
*/

void lbuffer::applyflush(int /*nothing*/,void (*f)(lexem))
{ struct lbufchunk *lb;
  int i;
//sterr << "[lbuffer::applyflush]flushing \n";
  lb = &firstch;
  if (! isempty()) {
    while (lb != NULL) {
      for(i=lb->b; i<lb->e; i++) {
//sterr << "[lbuffer::applyflush] calling f with ";lb->l[i].dump();sterr<<"\n";
	(*f)(lb->l[i]);
      }
      lb=lb->next;
    }
  }
}

void lbuffer::flush(int /*nothing*/,void (*f)(lexem))
{
  applyflush(0,f);					//!!!!!!!!!!!!!!!!!!!!!
  clear();
}

void lbuffer::copy(lbuffer &into)
{ struct lbufchunk *lb,*lb2;
  int i;
  into.clear();
//sterr << "[lbuffer::copy]\n";
  lb= &firstch; lb2 = &(into.firstch);
  while (lb!=NULL) {
    lb2->b  = lb->b; lb2->e = lb->e;
    for(i= 0; i<CHUNKSIZE; i++) lb2->l[i] = lb->l[i];
    lb = lb->next;
    if (lb!=NULL) {
      NNEW(lb2->next ,struct lbufchunk);
      NNEW(lb2->next->l ,lexem[CHUNKSIZE]);
      lb2 = lb2->next;
    }
  }
  lb2->next = NULL;
  into.lastch = lb2;
}

void lbuffer::append(lbuffer &appendto)
{ struct lbufchunk *lb;
  int i;
  lb= &firstch;
  while (lb!=NULL) {
    for(i= lb->b; i<lb->e; i++) appendto.put(lb->l[i]);
    lb = lb->next;
  }
}

void lbuffer::dump()
{ struct lbufchunk *lb;
  int i;
  sterr << "[lbuffer::dump] start\n";
  lb= &firstch;
  while (lb!=NULL) {
    sterr << "b = " << lb->b << "  e = " << lb->e << "\n";
    for(i= 0; i<CHUNKSIZE; i++) sterr << lb->l[i].alfsy() << " "; 
 	/* !!!!!!!!!!!! dumping all size, whichever are the bounds !! */
    sterr << "\nnext chunk\n";
    lb = lb->next;
  }
  sterr << "[lbuffer::dump] end\n";
}

