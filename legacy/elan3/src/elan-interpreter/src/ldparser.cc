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


#define STTAB   ldsttab
#define PARSER  ldsyntan
#define SEMACT  ldsemact
#define RESWTAB ldrwt
#define ILEXEM  ilex 

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
{RWLPL,shift,{2}}, {EOFINPUT,accept,{0}}, {IDENTIFIER,shift,{4}}, 
{RWdescription,shift,{5}}, {DEFAULT,reduction,{2,17,1}}, {RWspecification,shift,{8}}, 
{RWquery,shift,{7}}, {RWpart,shift,{12}}, {RWof,shift,{15}}, 
{RWdescription,shift,{16}}, {RWend,shift,{17}}, {RWend,reduction,{5,21,1}}, 
{RWpart,shift,{12}}, {DEFAULT,reduction,{704,13,1}}, {IDENTIFIER,shift,{20}}, 
{RWend,shift,{21}}, {RWresult,shift,{23}}, {RWsort,shift,{24}}, 
{DEFAULT,reduction,{15,26,2}}, {RWquery,shift,{25}}, {DEFAULT,reduction,{705,13,2}}, 
{RWof,shift,{15}}, {DEFAULT,reduction,{1,16,1}}, {DEFAULT,reduction,{3,12,6}}, 
{RWimport,shift,{28}}, {RWof,shift,{29}}, {IDENTIFIER,shift,{33}}, 
{NUMBER,shift,{34}}, {-'<',shift,{31}}, {RWof,shift,{15}}, 
{RWimport,shift,{37}}, {RWstart,shift,{40}}, {RWcheck,shift,{41}}, 
{RWlocal,shift,{47}}, {RWglobal,shift,{46}}, {IDENTIFIER,shift,{33}}, 
{NUMBER,shift,{34}}, {-'<',shift,{31}}, {RWsort,shift,{48}}, 
{DEFAULT,reduction,{13,24,3}}, {IDENTIFIER,shift,{33}}, {NUMBER,shift,{34}}, 
{-'<',shift,{31}}, {RWstart,reduction,{713,32,1}}, {RWlocal,reduction,{713,32,1}}, 
{RWend,reduction,{713,32,1}}, {RWcheck,reduction,{713,32,1}}, {RWresult,reduction,{713,32,1}}, 
{RWimport,reduction,{713,32,1}}, {RWpart,reduction,{713,32,1}}, {IDENTIFIER,reduction,{713,32,1}}, 
{NUMBER,reduction,{713,32,1}}, {-'|',reduction,{713,32,1}}, {-']',reduction,{713,32,1}}, 
{-'[',shift,{50}}, {-'>',reduction,{713,32,1}}, {-'<',reduction,{713,32,1}}, 
{-'-',reduction,{713,32,1}}, {-',',reduction,{713,32,1}}, {DEFAULT,reduction,{16,29,1}}, 
{DEFAULT,reduction,{16,29,1}}, {RWend,shift,{51}}, {RWend,reduction,{20,15,4}}, 
{RWcheck,shift,{53}}, {RWpart,reduction,{20,15,4}}, {RWlocal,shift,{47}}, 
{RWglobal,shift,{46}}, {IDENTIFIER,shift,{33}}, {NUMBER,shift,{34}}, 
{-'<',shift,{31}}, {DEFAULT,reduction,{6,18,4}}, {RWstart,shift,{40}}, 
{RWwith,shift,{58}}, {RWwith,shift,{59}}, {RWstart,reduction,{715,25,2}}, 
{RWcheck,reduction,{715,25,2}}, {IDENTIFIER,shift,{33}}, {NUMBER,shift,{34}}, 
{-'<',shift,{31}}, {IDENTIFIER,shift,{33}}, {NUMBER,shift,{34}}, 
{-'<',shift,{31}}, {IDENTIFIER,shift,{33}}, {NUMBER,shift,{34}}, 
{-'<',shift,{31}}, {DEFAULT,reduction,{4,35,1}}, {DEFAULT,reduction,{21,27,1}}, 
{DEFAULT,reduction,{22,28,1}}, {IDENTIFIER,shift,{33}}, {NUMBER,shift,{34}}, 
{-'<',shift,{31}}, {-'>',shift,{64}}, {-'-',shift,{65}}, 
{IDENTIFIER,shift,{33}}, {NUMBER,shift,{34}}, {-'<',shift,{31}}, 
{DEFAULT,reduction,{3,12,9}}, {DEFAULT,reduction,{706,15,5}}, {RWwith,shift,{68}}, 
{RWend,reduction,{707,14,2}}, {RWcheck,reduction,{707,14,2}}, {RWpart,reduction,{707,14,2}}, 
{IDENTIFIER,shift,{33}}, {NUMBER,shift,{34}}, {-'<',shift,{31}}, 
{IDENTIFIER,shift,{33}}, {NUMBER,shift,{34}}, {-'<',shift,{31}}, 
{IDENTIFIER,shift,{33}}, {NUMBER,shift,{34}}, {-'<',shift,{31}}, 
{DEFAULT,reduction,{7,18,5}}, {-'[',shift,{72}}, {-'(',shift,{71}}, 
{DEFAULT,reduction,{11,22,2}}, {DEFAULT,reduction,{4,35,2}}, {RWstart,reduction,{716,25,3}}, 
{RWlocal,shift,{47}}, {RWcheck,reduction,{716,25,3}}, {IDENTIFIER,shift,{33}}, 
{NUMBER,shift,{34}}, {-'<',shift,{31}}, {RWstart,reduction,{717,25,3}}, 
{RWcheck,reduction,{717,25,3}}, {IDENTIFIER,shift,{33}}, {NUMBER,shift,{34}}, 
{-'<',shift,{31}}, {DEFAULT,reduction,{9,19,4}}, {DEFAULT,reduction,{23,32,3}}, 
{-'>',shift,{74}}, {-'|',shift,{77}}, {-']',shift,{75}}, 
{-',',shift,{76}}, {DEFAULT,reduction,{714,33,1}}, {DEFAULT,reduction,{19,34,2}}, 
{RWlocal,shift,{47}}, {RWend,reduction,{708,14,3}}, {RWcheck,reduction,{708,14,3}}, 
{RWpart,reduction,{708,14,3}}, {IDENTIFIER,shift,{33}}, {NUMBER,shift,{34}}, 
{-'<',shift,{31}}, {RWend,reduction,{709,14,3}}, {RWcheck,reduction,{709,14,3}}, 
{RWpart,reduction,{709,14,3}}, {IDENTIFIER,shift,{33}}, {NUMBER,shift,{34}}, 
{-'<',shift,{31}}, {IDENTIFIER,shift,{81}}, {-')',shift,{80}}, 
{DEFAULT,reduction,{14,20,3}}, {IDENTIFIER,shift,{33}}, {NUMBER,shift,{34}}, 
{-'<',shift,{31}}, {IDENTIFIER,shift,{33}}, {NUMBER,shift,{34}}, 
{-'<',shift,{31}}, {DEFAULT,reduction,{17,32,4}}, {IDENTIFIER,shift,{33}}, 
{NUMBER,shift,{34}}, {-'<',shift,{31}}, {IDENTIFIER,shift,{33}}, 
{NUMBER,shift,{34}}, {-'<',shift,{31}}, {IDENTIFIER,shift,{33}}, 
{NUMBER,shift,{34}}, {-'<',shift,{31}}, {-')',shift,{87}}, 
{DEFAULT,reduction,{10,20,4}}, {DEFAULT,reduction,{12,23,1}}, {RWstart,reduction,{718,25,5}}, 
{RWcheck,reduction,{718,25,5}}, {IDENTIFIER,shift,{33}}, {NUMBER,shift,{34}}, 
{-'<',shift,{31}}, {-'>',shift,{88}}, {DEFAULT,reduction,{18,33,3}}, 
{DEFAULT,reduction,{18,33,3}}, {RWend,reduction,{710,14,5}}, {RWcheck,reduction,{710,14,5}}, 
{RWpart,reduction,{710,14,5}}, {IDENTIFIER,shift,{33}}, {NUMBER,shift,{34}}, 
{-'<',shift,{31}}, {DEFAULT,reduction,{10,20,5}}, {DEFAULT,reduction,{24,32,6}}, 
{0,shift,{0}}};

 static struct sak {
     int sym,tostate;
 } apak[]={
{12,1}, {17,3}, {26,6}, {13,10}, {15,11}, {21,9}, {18,13}, {24,14}, 
{15,18}, {16,19}, {19,22}, {24,26}, {25,27}, {29,32}, {32,30}, {18,35}, 
{24,14}, {14,36}, {20,38}, {22,39}, {27,43}, {28,44}, {29,32}, {32,45}, 
{35,42}, {29,32}, {32,49}, {34,52}, {27,55}, {28,56}, {29,32}, {32,45}, 
{35,54}, {20,57}, {29,32}, {32,60}, {29,32}, {32,45}, {35,61}, {29,32}, 
{32,45}, {35,62}, {29,32}, {32,63}, {29,32}, {32,67}, {33,66}, {29,32}, 
{32,60}, {29,32}, {32,45}, {35,69}, {29,32}, {32,45}, {35,70}, {28,73}, 
{29,32}, {32,60}, {29,32}, {32,60}, {28,78}, {29,32}, {32,60}, {29,32}, 
{32,60}, {23,79}, {29,32}, {32,45}, {35,82}, {29,32}, {32,83}, {29,32}, 
{32,84}, {29,32}, {32,85}, {29,32}, {32,45}, {35,86}, {29,32}, {32,60}, 
{29,32}, {32,60}, {0,0}};

 struct sst {
	struct spr *pr;
	int npr;
	struct sak *ak;
	int nak;
 } STTAB[]={
{&appr[0],1,&apak[0],1}, {&appr[1],1,&apak[1],0}, {&appr[2],1,&apak[1],1}, 
{&appr[3],1,&apak[2],0}, {&appr[4],1,&apak[2],0}, {&appr[5],2,&apak[2],1}, 
{&appr[7],1,&apak[3],3}, {&appr[8],1,&apak[6],2}, {&appr[9],1,&apak[8],0}, 
{&appr[10],1,&apak[8],0}, {&appr[11],2,&apak[8],1}, {&appr[13],1,&apak[9],0}, 
{&appr[14],1,&apak[9],1}, {&appr[15],1,&apak[10],0}, {&appr[16],1,&apak[10],1}, 
{&appr[17],1,&apak[11],0}, {&appr[18],1,&apak[11],0}, {&appr[19],1,&apak[11],0}, 
{&appr[20],1,&apak[11],0}, {&appr[21],1,&apak[11],1}, {&appr[22],1,&apak[12],0}, 
{&appr[23],1,&apak[12],0}, {&appr[24],1,&apak[12],1}, {&appr[25],1,&apak[13],0}, 
{&appr[26],3,&apak[13],2}, {&appr[29],1,&apak[15],2}, {&appr[30],1,&apak[17],1}, 
{&appr[31],2,&apak[18],2}, {&appr[33],5,&apak[20],5}, {&appr[38],1,&apak[25],0}, 
{&appr[39],1,&apak[25],0}, {&appr[40],3,&apak[25],2}, {&appr[43],16,&apak[27],0}, 
{&appr[59],1,&apak[27],0}, {&appr[60],1,&apak[27],0}, {&appr[61],1,&apak[27],0}, 
{&appr[62],3,&apak[27],1}, {&appr[65],5,&apak[28],5}, {&appr[70],1,&apak[33],0}, 
{&appr[71],1,&apak[33],1}, {&appr[72],1,&apak[34],0}, {&appr[73],1,&apak[34],0}, 
{&appr[74],5,&apak[34],2}, {&appr[79],3,&apak[36],3}, {&appr[82],3,&apak[39],3}, 
{&appr[85],1,&apak[42],0}, {&appr[86],1,&apak[42],0}, {&appr[87],1,&apak[42],0}, 
{&appr[88],3,&apak[42],2}, {&appr[91],2,&apak[44],0}, {&appr[93],3,&apak[44],3}, 
{&appr[96],1,&apak[47],0}, {&appr[97],1,&apak[47],0}, {&appr[98],1,&apak[47],0}, 
{&appr[99],6,&apak[47],2}, {&appr[105],3,&apak[49],3}, {&appr[108],3,&apak[52],3}, 
{&appr[111],1,&apak[55],0}, {&appr[112],2,&apak[55],0}, {&appr[114],1,&apak[55],0}, 
{&appr[115],1,&apak[55],0}, {&appr[116],6,&apak[55],3}, {&appr[122],5,&apak[58],2}, 
{&appr[127],1,&apak[60],0}, {&appr[128],1,&apak[60],0}, {&appr[129],1,&apak[60],0}, 
{&appr[130],3,&apak[60],0}, {&appr[133],1,&apak[60],0}, {&appr[134],1,&apak[60],0}, 
{&appr[135],7,&apak[60],3}, {&appr[142],6,&apak[63],2}, {&appr[148],2,&apak[65],1}, 
{&appr[150],1,&apak[66],0}, {&appr[151],3,&apak[66],3}, {&appr[154],3,&apak[69],2}, 
{&appr[157],1,&apak[71],0}, {&appr[158],3,&apak[71],2}, {&appr[161],3,&apak[73],2}, 
{&appr[164],3,&apak[75],3}, {&appr[167],1,&apak[78],0}, {&appr[168],1,&apak[78],0}, 
{&appr[169],1,&apak[78],0}, {&appr[170],5,&apak[78],2}, {&appr[175],1,&apak[80],0}, 
{&appr[176],1,&apak[80],0}, {&appr[177],1,&apak[80],0}, {&appr[178],6,&apak[80],2}, 
{&appr[184],1,&apak[82],0}, {&appr[185],1,&apak[82],0}, {&appr[0],0,&apak[0],0}};


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



