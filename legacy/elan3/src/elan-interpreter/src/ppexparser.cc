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


#define STTAB   ppexsttab
#define PARSER  ppexsyntan
#define SEMACT  ppexsemact
#define RESWTAB ppexrwt
#define ILEXEM  pilex

#include "commondefs.h"


#define RWAC (-132+BOFIDENT)
#define RWdc (-199+BOFIDENT)
#define RWid (-205+BOFIDENT)
#define RWdk (-207+BOFIDENT)
#define RWof (-213+BOFIDENT)
#define RWLPL (-232+BOFIDENT)
#define RWMETA (-295+BOFIDENT)
#define RWend (-311+BOFIDENT)
#define RWone (-322+BOFIDENT)
#define RWfor (-327+BOFIDENT)
#define RWpri (-331+BOFIDENT)
#define RWtry (-351+BOFIDENT)
#define RWcode (-411+BOFIDENT)
#define RWfail (-412+BOFIDENT)
#define RWif (-418+BOFIDENT)
#define RWbs (-424+BOFIDENT)
#define RWtall (-429+BOFIDENT)
#define RWthen (-431+BOFIDENT)
#define RWdont (-437+BOFIDENT)
#define RWtone (-438+BOFIDENT)
#define RWpart (-439+BOFIDENT)
#define RWwith (-444+BOFIDENT)
#define RWknow (-447+BOFIDENT)
#define RWsort (-456+BOFIDENT)
#define RWdcOne (-489+BOFIDENT)
#define RWcheck (-510+BOFIDENT)
#define RWalias (-522+BOFIDENT)
#define RWlocal (-523+BOFIDENT)
#define RWwhere (-539+BOFIDENT)
#define RWtsome (-552+BOFIDENT)
#define RWrules (-555+BOFIDENT)
#define RWstart (-558+BOFIDENT)
#define RWquery (-566+BOFIDENT)
#define RWdccall (-611+BOFIDENT)
#define RWdkcall (-619+BOFIDENT)
#define RWcare (-622+BOFIDENT)
#define RWcall (-623+BOFIDENT)
#define RWglobal (-625+BOFIDENT)
#define RWpublic (-639+BOFIDENT)
#define RWrepeat (-641+BOFIDENT)
#define RWmodule (-646+BOFIDENT)
#define RWsource (-657+BOFIDENT)
#define RWswitch (-658+BOFIDENT)
#define RWnormin (-659+BOFIDENT)
#define RWimport (-667+BOFIDENT)
#define RWresult (-671+BOFIDENT)
#define RWexport (-674+BOFIDENT)
#define RWdefined (-719+BOFIDENT)
#define RWdeclare (-720+BOFIDENT)
#define RWEpsilon (-730+BOFIDENT)
#define RWiterate (-750+BOFIDENT)
#define RWbuiltin (-759+BOFIDENT)
#define RWfirst (-763+BOFIDENT)
#define RWrewrite (-770+BOFIDENT)
#define RWstratop (-781+BOFIDENT)
#define RWnormout (-788+BOFIDENT)
#define RWcase (-834+BOFIDENT)
#define RWhandline (-835+BOFIDENT)
#define RWfirstOne (-842+BOFIDENT)
#define RWdcconcur (-849+BOFIDENT)
#define RWinline (-850+BOFIDENT)
#define RWchoose (-852+BOFIDENT)
#define RWdkconcur (-857+BOFIDENT)
#define RWimplicit (-859+BOFIDENT)
#define RWexplicit (-866+BOFIDENT)
#define RWqueryend (-877+BOFIDENT)
#define RWstrategy (-883+BOFIDENT)
#define RWdefinedAs (-899+BOFIDENT)
#define RWhardAlias (-905+BOFIDENT)
#define RWassocLeft (-932+BOFIDENT)
#define RWnormalise (-970+BOFIDENT)
#define RWoneconcur (-972+BOFIDENT)
#define RWprivate (-974+BOFIDENT)
#define RWnormalize (-977+BOFIDENT)
#define RWotherwise (-986+BOFIDENT)
#define RWoperators (-991+BOFIDENT)
#define RWassocRight (-1047+BOFIDENT)
#define RWstrategies (-1083+BOFIDENT)
#define RWdescription (-1188+BOFIDENT)
#define RWspecification (-1377+BOFIDENT)
#define EOFINPUT -256
#define NUMBER -13261
#define IDENTIFIER -259
#define DEFAULT -32000

#define reduction 0
#define accept 1
#define shift 2
#define srerr 3
#define rrerr 4
static struct spr {
     int sym;
     int action;
     struct { int num,left,lenght;
            } actpar;
} appr[]={
{-'(',shift,{3}}, {EOFINPUT,accept,{0}}, {-')',shift,{4}}, 
{IDENTIFIER,shift,{12}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{-'(',shift,{8}}, {DEFAULT,reduction,{704,16,2}}, {-')',reduction,{1,17,2}}, 
{-'&',shift,{13}}, {-'>',shift,{17}}, {-'=',shift,{14}}, 
{-'<',shift,{16}}, {-'/',shift,{21}}, {-'-',shift,{19}}, 
{-'+',shift,{18}}, {-'*',shift,{20}}, {-')',reduction,{1,17,2}}, 
{-'%',shift,{22}}, {-'!',shift,{15}}, {DEFAULT,reduction,{705,14,1}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {-'(',shift,{8}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {-'(',shift,{8}}, 
{DEFAULT,reduction,{2,13,1}}, {-'=',shift,{25}}, {-'!',shift,{26}}, 
{DEFAULT,reduction,{16,18,1}}, {IDENTIFIER,shift,{12}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {-'(',shift,{8}}, {-'=',shift,{29}}, 
{-'=',shift,{30}}, {NUMBER,shift,{10}}, {-'=',shift,{31}}, 
{-'-',shift,{9}}, {-'(',shift,{8}}, {NUMBER,shift,{10}}, 
{-'=',shift,{33}}, {-'-',shift,{9}}, {-'(',shift,{8}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {-'(',shift,{8}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {-'(',shift,{8}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {-'(',shift,{8}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {-'(',shift,{8}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {-'(',shift,{8}}, 
{-'/',shift,{21}}, {-'-',shift,{19}}, {-'+',shift,{18}}, 
{-'*',shift,{20}}, {-')',shift,{40}}, {-'%',shift,{22}}, 
{-'>',reduction,{8,13,2}}, {-'=',reduction,{8,13,2}}, {-'<',reduction,{8,13,2}}, 
{-'/',reduction,{8,13,2}}, {-'-',reduction,{8,13,2}}, {-'+',reduction,{8,13,2}}, 
{-'*',reduction,{8,13,2}}, {-')',reduction,{8,13,2}}, {-'&',reduction,{8,13,2}}, 
{-'%',reduction,{8,13,2}}, {-'!',reduction,{8,13,2}}, {-'=',shift,{41}}, 
{-'=',shift,{42}}, {DEFAULT,reduction,{9,14,3}}, {-'>',shift,{17}}, 
{-'=',shift,{14}}, {-'<',shift,{16}}, {-'/',shift,{21}}, 
{-'-',shift,{19}}, {-'+',shift,{18}}, {-'*',shift,{20}}, 
{-'%',shift,{22}}, {-'!',shift,{15}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {-'(',shift,{8}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {-'(',shift,{8}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {-'(',shift,{8}}, {-'/',shift,{21}}, 
{-'-',shift,{19}}, {-'+',shift,{18}}, {-'*',shift,{20}}, 
{-')',reduction,{15,15,3}}, {-'&',reduction,{15,15,3}}, {-'%',shift,{22}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {-'(',shift,{8}}, 
{-'/',shift,{21}}, {-'-',shift,{19}}, {-'+',shift,{18}}, 
{-'*',shift,{20}}, {-')',reduction,{14,15,3}}, {-'&',reduction,{14,15,3}}, 
{-'%',shift,{22}}, {-'>',reduction,{3,13,3}}, {-'=',reduction,{3,13,3}}, 
{-'<',reduction,{3,13,3}}, {-'/',shift,{21}}, {-'-',reduction,{3,13,3}}, 
{-'+',reduction,{3,13,3}}, {-'*',shift,{20}}, {-')',reduction,{3,13,3}}, 
{-'&',reduction,{3,13,3}}, {-'%',shift,{22}}, {-'!',reduction,{3,13,3}}, 
{-'>',reduction,{4,13,3}}, {-'=',reduction,{4,13,3}}, {-'<',reduction,{4,13,3}}, 
{-'/',shift,{21}}, {-'-',reduction,{4,13,3}}, {-'+',reduction,{4,13,3}}, 
{-'*',shift,{20}}, {-')',reduction,{4,13,3}}, {-'&',reduction,{4,13,3}}, 
{-'%',shift,{22}}, {-'!',reduction,{4,13,3}}, {-'>',reduction,{5,13,3}}, 
{-'=',reduction,{5,13,3}}, {-'<',reduction,{5,13,3}}, {-'/',reduction,{5,13,3}}, 
{-'-',reduction,{5,13,3}}, {-'+',reduction,{5,13,3}}, {-'*',reduction,{5,13,3}}, 
{-')',reduction,{5,13,3}}, {-'&',reduction,{5,13,3}}, {-'%',reduction,{5,13,3}}, 
{-'!',reduction,{5,13,3}}, {-'>',reduction,{6,13,3}}, {-'=',reduction,{6,13,3}}, 
{-'<',reduction,{6,13,3}}, {-'/',reduction,{6,13,3}}, {-'-',reduction,{6,13,3}}, 
{-'+',reduction,{6,13,3}}, {-'*',reduction,{6,13,3}}, {-')',reduction,{6,13,3}}, 
{-'&',reduction,{6,13,3}}, {-'%',reduction,{6,13,3}}, {-'!',reduction,{6,13,3}}, 
{-'>',reduction,{7,13,3}}, {-'=',reduction,{7,13,3}}, {-'<',reduction,{7,13,3}}, 
{-'/',reduction,{7,13,3}}, {-'-',reduction,{7,13,3}}, {-'+',reduction,{7,13,3}}, 
{-'*',reduction,{7,13,3}}, {-')',reduction,{7,13,3}}, {-'&',reduction,{7,13,3}}, 
{-'%',reduction,{7,13,3}}, {-'!',reduction,{7,13,3}}, {DEFAULT,reduction,{706,13,3}}, 
{IDENTIFIER,shift,{12}}, {IDENTIFIER,shift,{12}}, {-'/',shift,{21}}, 
{-'-',shift,{19}}, {-'+',shift,{18}}, {-'*',shift,{20}}, 
{-')',reduction,{10,15,4}}, {-'&',reduction,{10,15,4}}, {-'%',shift,{22}}, 
{-'/',shift,{21}}, {-'-',shift,{19}}, {-'+',shift,{18}}, 
{-'*',shift,{20}}, {-')',reduction,{11,15,4}}, {-'&',reduction,{11,15,4}}, 
{-'%',shift,{22}}, {-'/',shift,{21}}, {-'-',shift,{19}}, 
{-'+',shift,{18}}, {-'*',shift,{20}}, {-')',reduction,{12,15,4}}, 
{-'&',reduction,{12,15,4}}, {-'%',shift,{22}}, {-'/',shift,{21}}, 
{-'-',shift,{19}}, {-'+',shift,{18}}, {-'*',shift,{20}}, 
{-')',reduction,{13,15,4}}, {-'&',reduction,{13,15,4}}, {-'%',shift,{22}}, 
{DEFAULT,reduction,{10,15,4}}, {DEFAULT,reduction,{11,15,4}}, {0,shift,{0}}};

 static struct sak {
     int sym,tostate;
 } apak[]={
{16,1}, {17,2}, {13,6}, {14,5}, {15,7}, {18,11}, {13,23}, {13,24}, 
{13,28}, {15,27}, {18,11}, {13,32}, {13,34}, {13,35}, {13,36}, {13,37}, 
{13,38}, {13,39}, {13,43}, {13,44}, {13,45}, {13,46}, {18,47}, {18,48}, 
{0,0}};

 struct sst {
	struct spr *pr;
	int npr;
	struct sak *ak;
	int nak;
 } STTAB[]={
{&appr[0],1,&apak[0],2}, {&appr[1],1,&apak[2],0}, {&appr[2],1,&apak[2],0}, 
{&appr[3],4,&apak[2],4}, {&appr[7],1,&apak[6],0}, {&appr[8],2,&apak[6],0}, 
{&appr[10],10,&apak[6],0}, {&appr[20],1,&apak[6],0}, {&appr[21],3,&apak[6],1}, 
{&appr[24],3,&apak[7],1}, {&appr[27],1,&apak[8],0}, {&appr[28],2,&apak[8],0}, 
{&appr[30],1,&apak[8],0}, {&appr[31],4,&apak[8],3}, {&appr[35],1,&apak[11],0}, 
{&appr[36],1,&apak[11],0}, {&appr[37],4,&apak[11],1}, {&appr[41],4,&apak[12],1}, 
{&appr[45],3,&apak[13],1}, {&appr[48],3,&apak[14],1}, {&appr[51],3,&apak[15],1}, 
{&appr[54],3,&apak[16],1}, {&appr[57],3,&apak[17],1}, {&appr[60],6,&apak[18],0}, 
{&appr[66],11,&apak[18],0}, {&appr[77],1,&apak[18],0}, {&appr[78],1,&apak[18],0}, 
{&appr[79],1,&apak[18],0}, {&appr[80],9,&apak[18],0}, {&appr[89],3,&apak[18],1}, 
{&appr[92],3,&apak[19],1}, {&appr[95],3,&apak[20],1}, {&appr[98],7,&apak[21],0}, 
{&appr[105],3,&apak[21],1}, {&appr[108],7,&apak[22],0}, {&appr[115],11,&apak[22],0}, 
{&appr[126],11,&apak[22],0}, {&appr[137],11,&apak[22],0}, {&appr[148],11,&apak[22],0}, 
{&appr[159],11,&apak[22],0}, {&appr[170],1,&apak[22],0}, {&appr[171],1,&apak[22],1}, 
{&appr[172],1,&apak[23],1}, {&appr[173],7,&apak[24],0}, {&appr[180],7,&apak[24],0}, 
{&appr[187],7,&apak[24],0}, {&appr[194],7,&apak[24],0}, {&appr[201],1,&apak[24],0}, 
{&appr[202],1,&apak[24],0}, {&appr[0],0,&apak[0],0}};





#define SATSTS 1500        /* velkost stacku pre s.a. */

#define FFOUND(p,s,i,j) { \
  k=i; p--; do { p++; k++; } while (k<j && s.notEqualMaccsym(p->sym));}
#define RFOUND(p,s,i,j) { \
  k=i; p--; do { p++; k++; } while (k<j &&  s!=p->sym); }


extern int SEMACT(int n,lexem l,lstream *f);

static void errcommon(lstream &f)
{ lexem s;
  /* int n; */
  f.ilex(s);
  sterr << "\ton symbol \'" << s.alfsy() << "\'" /* <<" in state " << st */;
  f.beforemess(s);
}


static void syerror(lstream &f,int st)
{ int n;
  lexem s;
  struct spr *p;
  f.oerr();
  sterr << "[PARSER] syntax error ";
  errcommon(f);
  sterr << "\tone of (";
  p=STTAB[st].pr; n=STTAB[st].npr;
  while (n--) {
    s.maccsymtolex((p++)->sym);
    sterr << " \'" << s.alfsy() << "\' ";
  }
  sterr << ") expected \n";
}

int PARSER(lstream &f,int state,void (*lextomodlex)(lexem lex,lexem &modlex))
                        /* LR(1) synt analyza podla STTAB */
			/* vysledok 0 = syntax error */
{ int i,j,k,istack;
  int stack[SATSTS];
  lexem ls,nnls;
  lexem lastl; lastl.crendofstreamlex();
  struct spr *p,*pp;
  struct sak *g;
  istack=0;
  f.fulex(nnls); lextomodlex(nnls,ls);
  SEMACT(0,lastl,&f);
  for(;;) {
    p=pp=STTAB[state].pr;
//cout << "\n state = " << state << "ls = "; ls.dump();
    i=0; j=STTAB[state].npr;
    if (j) {
      FFOUND(p,ls,i,j);
      if (ls.notEqualMaccsym(p->sym)) {
	/* ls nie je v zozname akcii */
	if (pp->sym == DEFAULTSYM) p=pp;
	else { syerror(f,state);return(0); };
      };
    }
    else { syerror(f,state);return(0); };
    switch (p->action) {
    case accept	: SEMACT(-2,lastl,&f); return(1);
    case srerr  : case rrerr:
                  f.oerr("[PARSER] syntax error\n");
                  sterr << "\ton symbol \'" << lastl.alfsy() << "\' ";
                  sterr << "in state " << state << ";\n",
                  sterr << "\t there was an " 
                       << ((p->action==srerr)?"shift":"reduce")
                       << "/reduce conflict in this place";
/*                  stbdump(state);                                */
                  return(0);

    case shift	:if (istack>=SATSTS) {
		   f.oerr("[PARSER] stack overflow ");
		   return(0);
		 };
		 stack[istack++]=state;
		 state=p->actpar.num;
		 f.ILEXEM(lastl); f.fulex(nnls); lextomodlex(nnls,ls);
		 break;
    case reduction:
	switch(SEMACT(p->actpar.num,lastl,&f)) {
          case ERRORIM: f.oerr();
                        sterr << "[PARSER] semaction error ";
			errcommon(f);
                       return(0);
          case ACCEPTIM:return(1);
          case HANDERRORIM : return(0);
        }     
        f.fulex(nnls); lextomodlex(nnls,ls);
        {register int l;
	l=p->actpar.left;
	istack-=p->actpar.lenght;
        state=stack[istack++];
	g=STTAB[state].ak;
	i=0;j=STTAB[state].nak;
	RFOUND(g,l,i,j);
        }
	state=g->tostate;
//cout << "\n reduction into state " << state;
    };
  };
};



