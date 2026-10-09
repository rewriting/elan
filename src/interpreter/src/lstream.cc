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
#include <ctype.h>

lstream::lstream(const char *name)
{
  NNEW(istr ,ichstream (name));
  type = OWNAME | UNDERID;
  lastinline = NULL; block = 0;
  ilex(flex);
}

lstream::lstream(const char *name,int typ)
{
  NNEW(istr ,ichstream (name));
  type = OWNAME | typ;
  lastinline = NULL; block = 0;
  ilex(flex);
}

lstream::lstream(ichstream *fil)
{
  istr = fil;
  type = OWSTREAM | UNDERID;
  lastinline = NULL; block = 0;
  ilex(flex);
}

lstream::lstream(ichstream *fil,char c)
{
  istr = fil;
  type = OWSTREAM | UNDERID;
  lastinline = NULL; block = 0;
  flex.crcharlex(c);
}

lstream::lstream(ichstream *fil,char c, int bloque)
{
  istr = fil;
  type = OWSTREAM | UNDERID;
  lastinline = NULL; block = bloque;
  flex.crcharlex(c);
}

lstream::lstream(const char *command,const char *name)
{
  NNEW(istr ,ichstream (command,name));
  type=OWNAME | UNDERID;
  lastinline = NULL; block = 0;
  ilex(flex);
}


lstream::~lstream()
{
  if ((type&OWTYPEMSK)!=OWSTREAM) DELETE1(istr);
}

void lstream::fulex(lexem &l)
{
  l=flex;
}

char strconst[STRLEN];
char *stringconstants[MAXNOFSTRING];
int  stringconstantsi = 0;

char *lexem::stringval() 
{ 
//  stout << "string " << BOFSTRING-lex << " is " <<  stringconstants[BOFSTRING-lex] << "\n";
  return(stringconstants[BOFSTRING-lex]); 
}

void lexem::crstringlex(char *s) {
  int i = stringconstantsi;
  if (stringconstantsi < MAXNOFSTRING) {
    stringconstants[stringconstantsi]=strdup(s); 
    stringconstantsi++;
    lex=BOFSTRING-i; 
  }
  else {
    sterr << "\ntoo many string constants\n"; failexit(); }
}

//#define LEX_BLABLABLA

void lstream::ilex(lexem &l)
{ char ide[IDLEN];
  int i,n,c,fc,tc;
  lexem ll;

  l=flex;
  istr->blankskip();
  istr->ich(c); istr->fuch(fc);

/*
 * c : caractere courant, fc : caractere suivant
 * Skip les commentaires et les inlines
 */

  while (c=='/' && (fc=='*' || fc=='/' || fc=='#')) {
    if (fc=='/') {
      while (fc!='\n') {
         istr->ich(c); istr->fuch(fc);
         if ( fc== EOFICHSTR ) {
            oerr("[lexan] comment throw end of input stream\n");
            failexit();
         }         
      }
    } else if (fc=='#') {
      if (lastinline !=NULL) {
       owarn("\ttwo succesives inline definitions,\n\tprevious one will be lost\n");
      }
      lastinline = new lbuffer();
      istr->ich(c); istr->ich(c); istr->fuch(fc);  
      while (c!='#' || fc!='/') {
	 ll.crcharlex(c);lastinline->put(ll);
         istr->ich(c); istr->fuch(fc);       
         if ( fc== EOFICHSTR ) {
            oerr("[lexan] inline throw end of input stream\n");
            failexit();
         }
      }
    } else {
      istr->ich(c); istr->ich(c); istr->fuch(fc);  
      while (c!='*' || fc!='/') {
         istr->ich(c); istr->fuch(fc);       
         if ( fc== EOFICHSTR ) {
            oerr("[lexan] comment throw end of input stream\n");
            failexit();
         }
      }
    }
    istr->ich(c); istr->blankskip(); istr->ich(c); istr->fuch(fc);
  }
  if (c=='"') {
    int  i = 0;
    istr->ich(c); istr->fuch(fc);
    while (c != '"') {
      if (c == '\\' && fc == 'n') { strconst[i++] = '\n'; istr->ich(c); istr->fuch(fc); }
      else if (c == '\\' && fc == 't') {strconst[i++] = '\t'; istr->ich(c); istr->fuch(fc);}
      else if (c == '\\' && fc == '\\') {strconst[i++] = '\\'; istr->ich(c); istr->fuch(fc);}
      else if (c == '\\' && fc == '"') {strconst[i++] = '"'; istr->ich(c); istr->fuch(fc);}
      else strconst[i++] = c; 
      if (i >= STRLEN) { sterr << "\nstring constant too long\n"; failexit(); }
      istr->ich(c); istr->fuch(fc);
    }  
    strconst[i] = 0;
    flex.crstringlex(strconst);   
    return;
  }
  if (c == '`') {
    istr->ich(c); istr->fuch(fc);
    if (c == '\\' && fc == 'n') { istr->ich(c); istr->fuch(fc); n = '\n'; }
    else if (c == '\\' && fc == 't') { istr->ich(c); istr->fuch(fc); n = '\t'; }
    else if (c == '\\' && fc == '\\') { istr->ich(c); istr->fuch(fc); n = '\\'; }
    else if (c == '\\' && fc == '"') { istr->ich(c); istr->fuch(fc); n = '"'; }
    else n = c;
    istr->ich(c); istr->fuch(fc);
    if (c != '`') { sterr << "\nwrong string constant\n"; failexit(); }
    flex.crnumlex(n);
    return;
  }
//--- BORO's hacks
  if (c == '\\') {
    if(fc == 'n') {
     istr->ich(c); ide[0] = '\n'; ide[1] = 0; flex.cridlex(ide); return; }
    if(fc == 't') {
     istr->ich(c); ide[0] = '\t'; ide[1] = 0; flex.cridlex(ide); return; }
  }
//--- up to here
// identifier
//  if (c == '$' || isalpha(c)) {
  if (isalpha(c)) {
    i=1; ide[0]=c;
    while (isalpha(fc) || isdigit(fc) || (fc=='_' && (type&IDMSK)==UNDERID)) { 
      istr->ich(tc); ide[i>=(IDLEN-1)?(IDLEN-1):i++]=tc; 
      istr->fuch(fc);
    }
    ide[i]=0;
    if (aterm_parse)
      flex.craidlex(ide);
    else
      flex.cridlex(ide);
    return;
  }
// number
  if (isdigit(c)) {
    n= c - '0';
    while (isdigit(fc)) {
      istr->ich(c); istr->fuch(fc);
      n=n*10+c-'0';
    }
    flex.crnumlex(n);
    return;
  }
  if (c==EOFICHSTR) {
//    if (block) {
//      flex.crendofstreamlex();
//      return; }
//    else {
      flex.crendofstreamlex(); 
      return; 
//}
  }
  flex.crcharlex(c);
  return;
}

void lstream::oerr()
{
  sterr<<"\n[error] :";
  sterr<<"file="<<istr->actname()<<
        "\tline=" << istr->actline() << "\tpos=" << istr->actpos()<<"\n";
}

void lstream::owarn()
{
  if (warnings) {
    sterr<<"\n[warning] :";
    sterr<<"file="<<istr->actname()<<
        "\tline=" << istr->actline() << "\tpos=" << istr->actpos()<<"\n";
  }
}

void lstream::oerr(const char *ch )
{
  oerr();
  sterr<<ch;
}

void lstream::owarn(const char *ch )
{
  if (warnings) {
    owarn();
    sterr<<ch;
  }
}

void lstream::oerr(const char *ch, const char *s ... )
{ va_list ap;
  oerr();
  va_start (ap,s);
  sterr << ch;
  while (s!=NULL) {
    sterr << s;
    s=va_arg(ap,const char *);
  }
  va_end(ap);
}

void lstream::owarn(const char *ch, const char *s ... )
{  va_list ap;
   if (warnings) {
    owarn();
    va_start (ap,s);
    sterr << ch;
    while (s!=NULL) {
      sterr << s;
      s=va_arg(ap,const char *);
    }
    va_end(ap);
  }
}


void lstream::beforemess(lexem s)
{ int n;
  sterr << "\n\t before  ";
  for (n=0; n<7 && s.isnotendofstream(); n++) { 
    ilex(s);
    sterr << s.alfsy() << " ";
  }
  sterr << " \n";
}

void lstream::setposition(int actl,int actp)
{
  istr->setposition(actl,actp);
}

lbuffer *lstream::getlastinlineAndinit()
{ lbuffer *ll;
  ll = lastinline;
  lastinline = NULL;
  return(ll);
}

/* ----------- here is moreover the lexem ------------*/


static char alfs[]=" ";
static char alfs2[100];

const char *lexem::alfsy()
{
 if (nonterminal()) {
   snprintf(alfs2,sizeof(alfs2),"sort(%d)",BOFTYPES-lex);return(alfs2);
 }
 else if (lex == BLANKLEXEM) return("BLANKLEXEM");
 else if (lex == NOLEXEM) return("ENDOFLSTREAM");
 else if (lex == IDENT) return("IDENTIFIER");
 else if (lex == JUSTNUMBER) return("NUMBER");
 else if (lex == STRING)
   return "???";
 else if ((lex <= BOFSTRING && lex > BOFSTRING-MAXNOFSTRING )) {
   return stringconstants[BOFSTRING-lex]; }
 else if (lex >= 0) {snprintf(alfs2,sizeof(alfs2),"%d",lex);return(alfs2);}
 else if (lex <= BOFIDENT) return(tabofident.ide(BOFIDENT-lex));
 else {
   alfs[0]= -lex; return(alfs);
 }
}

const char *lexem::erralfsy()
{ 
 if (lex <= 0 && lex >= -32) {
   snprintf(alfs2,sizeof(alfs2),"\'\\%d\'",-lex);return(alfs2);
 } else return(alfsy());
}

void lexem::dump() 
{
//NONBLOCK: stout << "lex<" << lex <<">";
  sterr << "lex<" << lex <<">";
}

int lstream::isready()
{
  return istr->isready(); 
}

int lstream::isblock()
{
  return block; 
}
