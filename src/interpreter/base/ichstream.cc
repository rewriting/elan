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

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string>
#include "constants.h"
#include "streams.h"
#include "alloc.h"
#include "misc.h"
#include "options.h"

FILE *fopen_exact(const char *path)
{
  FILE *f = fopen(path, "r");
  if (f == NULL) return NULL;
  std::string p(path);
  std::string::size_type slash = p.rfind('/');
  std::string dir  = (slash == std::string::npos) ? "." : (slash == 0 ? "/" : p.substr(0, slash));
  std::string base = (slash == std::string::npos) ? p : p.substr(slash + 1);
  DIR *d = opendir(dir.c_str());
  if (d == NULL) return f;                 // cannot list the directory: keep fopen's answer
  bool exact = false;
  for (struct dirent *e; (e = readdir(d)) != NULL; )
    if (base == e->d_name) { exact = true; break; }
  closedir(d);
  if (!exact) { fclose(f); return NULL; }  // same file name up to case only
  return f;
}

ichstream::ichstream(const char *name)              // to open elan file in lib or .
{ char *fn;

  // the name as given (user's file, current directory): plain fopen, as in 2004;
  // the library lookups below are exact-case (any[X] vs Any on macOS)
  if ((file=fopen(name,"r"))) { fn = mstrdup(name); commonopen(fn,NFILE); return; }

  fn = addsuffix(perslib,name);
  if ((file=fopen_exact(fn))) { commonopen(fn,NFILE); return; }
  CFRE(fn);

  fn = addsuffix(elanlibcommon,name);
  if ((file=fopen_exact(fn))) { commonopen(fn,NFILE); return; }
  CFRE(fn);

  fn = addsuffix(elanlibqnq,name);
  if ((file=fopen_exact(fn))) { commonopen(fn,NFILE); return; }
  CFRE(fn);

  fn = addsuffix(elanlibstrat,name);
  if ((file=fopen_exact(fn))) { commonopen(fn,NFILE); return; }
  CFRE(fn);

  fn = addsuffix(elanlibref,name);
  if ((file=fopen_exact(fn))) { commonopen(fn,NFILE); return; }
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
