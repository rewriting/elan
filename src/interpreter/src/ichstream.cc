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

ichstream::ichstream(const char *name)              // to open elan file in lib or .
{ char *fn;

  if ((file=fopen(name,"r"))) { fn = mstrdup(name); commonopen(fn,NFILE); return; }

  fn = addsuffix(perslib,name);
  if ((file=fopen(fn,"r"))) { commonopen(fn,NFILE); return; }
  CFRE(fn);

  fn = addsuffix(elanlibcommon,name);
  if ((file=fopen(fn,"r"))) { commonopen(fn,NFILE); return; }
  CFRE(fn);

  fn = addsuffix(elanlibqnq,name);
  if ((file=fopen(fn,"r"))) { commonopen(fn,NFILE); return; }
  CFRE(fn);

  fn = addsuffix(elanlibstrat,name);
  if ((file=fopen(fn,"r"))) { commonopen(fn,NFILE); return; }
  CFRE(fn);

  fn = addsuffix(elanlibref,name);
  if ((file=fopen(fn,"r"))) { commonopen(fn,NFILE); return; }
  CFRE(fn);

  sterr << "[error] can't open input file '"<< name << "'\n\tfatal\n"; failexit(); 
}


ichstream::ichstream(FILE *fil,const char *name)
{ 
  file = fil;
  commonopen(mstrdup(name),NFILE);
}

ichstream::ichstream(FILE *fil,const char *name, int block)
{ 
  file = fil;
  commonopen(mstrdup(name),NFILE);
  read_block = block;
}

ichstream::ichstream(const char *pcommand, const char *name)
{ 
  file = popen(pcommand,"r");
  commonopen(mstrdup(name),PIPE);
}

ichstream::~ichstream()
{
  CFRE(fname);
  if (type==NFILE) fclose(file);
  else 
    {
      // BUG ??????????????
      //pclose(file);
    }
}

void ichstream::commonopen(char *name,int ftype)
{ int c;
  if (file==NULL) {
    sterr << "[error] can't open input file '"<< name << "'\n\tfatal\n";
    failexit();
  }
  line=0; pos=0; type = ftype; fchar=1;
  fname=name; read_block = 0;
  ich(c);
}

#include <errno.h>
#include <sys/errno.h>

#include <sys/errno.h>

char *ichstream::readstring()
{
char buffer[30000];
  (void)!fscanf(file,"%s",buffer);
  return strdup(buffer);
}

void ichstream::ich(int &c)
{ 
  /*int eno;*/
  c=fchar;
  if ((read_block) || (c!=EOF)) { 
    fchar= fgetc(file);

    if (fchar=='\n') {line++; pos=0;}
    else pos++;
    if (fchar==EOF) { fchar=EOFICHSTR;
    } }
//NONBLOCK: if (read_block) stout << "ICH(" << c << ")\n";
}

void ichstream::fuch(int &c)
{ c=fchar;
}

void ichstream::blankskip()
{ int c;
  while (fchar <= ' ' && fchar!=EOF) ich(c);
}

void ichstream::setposition(int actl,int actp)
{
  line = actl; pos = actp;
}

int ichstream::isready()
{
  int ch = fgetc(file);
  if (ch != EOF) {
      ungetc(ch,file);
// stout << "ISREADY " << ch << "\n";
//      fchar = ch;
      return 1; }
  else {
//stout << "NOTREADY\n";
    return 0; }
}
