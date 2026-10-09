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
    Marian Vittek		e-mail: vittek@guma.ii.fmph.uniba.sk

*/

/*
		(c) 	Marian Vittek, 1991
			Bratislava, Slovakia
*/
#include <stdarg.h>

#include "globdef.h"

/*...........................................................................*/

int idmember(char *id, TABID *t)
{ char *p;
  posid=0; p=id;
  while (*p) posid=posid+*(p++);
  posid=posid % t->maxid;
  while (t->id[posid] != NULL) {
    if (!strcmp(id,t->id[posid])) return(1);
    posid=(posid+211)% t->maxid;
  }
  return(0);
}

int idadd(char *id, int l, TABID *t)
{ if (!idmember(id,t)) {
    if (((t->nidin+1)*10)/9 >= t->maxid) {
      oerr(NULL,'e',"[idadd] id tab overflowed over %d\n ",t->maxid);
      return(-1);
    }
    (t->nidin)++;
    t->id[posid] = ALLOSS(l+1,char);
    strcpy(t->id[posid],id);
  }
  return(posid);
}


void idwrite(FILE *f,TABID *t)
{ int i;
  for (i=0; i<t->maxid; i++)
    if (t->id[i]!=NULL) fprintf(f,"%s\n",t->id[i]);
}

static void idinit(TABID *t)
{ int i;
  for(i=0; i < t->maxid; i++) t->id[i]=NULL;
  t->nidin=0;
}

TABID * idtaballoc(int n)
{ TABID *t;
  t=ALLOS(TABID);
  t->id = ALLOSS(n,char *);
  t->maxid=n;
  idinit(t);
  return(t);
}

void tiddump(FILE *f, TABID *t, char *name)
{ int i;
  fprintf(f,"static char * %s0[]={",name);
  for(i=0; i<(t->maxid-1);i++) {
    if (t->id[i]==NULL) fprintf(f,"NULL,");
    else fprintf(f,"\"%s\",",t->id[i]);
    if (!(i%5)) fprintf(f,"\n");
  }
  if (t->id[i]==NULL) fprintf(f,"NULL};\n");
  else fprintf(f,"\"%s\"};\n",t->id[i]);
  fprintf(f,"static TABID %s1 = {%d,%d,&%s0[0]};\n",name,t->maxid,t->nidin,name);
  fprintf(f,"TABID * %s = & %s1;\n",name,name);
}
/*........................................................................*/

int lasterr;

void oferr(INFILE *f,char *t,...)
{ 
  va_list args;

  va_start(args, t);
  oerr(f,'e',t, args); oadderr("  !! fatal !!\n");
  exit(EXIT_FAILURE);

  va_end (args);
}

void oerr(INFILE *f,int w,char *t,...)
{
  va_list args;

  lasterr= w;
  if (w=='w' && warni==0) return;
  if (w=='w') oadderr("\n[warning] :");
  else        oadderr("\n[error] :");
  if (f!=NULL)
    oadderr(" file=%s\tline=%d\tposition=%d",f->name,f->line,f->pos);
  if (w!='w') oadderr("\n");
  else oadderr("\t");

  va_start(args, t);

  /*  if (f!=NULL)*/
    oadderr(t,args);
    /* else
       {*/
      /*printf("f=NULL\n");
    fprintf(stderr,"t=%s\n",t);
    fprintf(stderr,"a1=%d\n",a1);
    fprintf(stderr,"a2=%c\n",a2);
    */
    /*      oadderr(t,args);
	    }*/

  va_end (args);
}

void oadderr(char *t,...)
{ 
  va_list args;

  if (lasterr=='w' && warni==0) 
    return;
  
  va_start(args, t);
  
  fprintf(stderr,t,args);

  va_end (args);
}


