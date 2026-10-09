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

#include "globuse.h"

#define RTSTACKSIZE ((long) 16000)
#define RTPOOLSIZE  ((long) 512000)

#define NOPINMN 16	/* max. pocet kodov funkcnych symbolov so spolocnou
			   predponou */
#define MAXARI NBITS	/* max. arita funkcneho symbolu */


	  /* cisla akcii pre zabudovane gramaticke pravidla */
#define GA1 1		/* 	START	::= . S2 			*/
#define GA2 2		/*	S2	::= STYPE -> SWH ;		*/
#define GA3 3		/*	S2	::= S2 STYPE -> SWH ;		*/
#define GA4 4		/*	SWH	::= STYPE			*/
#define GA5 5		/*	SWH	::= SWH2 STYPE otherwise 	*/
#define GA6 6		/*	SWH2	::= STYPE when BOOLT ->		*/
#define GA7 7		/*	SWH2	::= SWH2 STYPE when BOOLT ->	*/
#define GA8 8		/*	BOOLT	::= BOOL	code 8		*/
#define GA9 9		/*	BOOL	::= true	code 9		*/
#define GA10 10		/*	BOOL	::= false	code 10		*/
#define GA11 11		/*	CHAR	::= "charact"		*/
#define GA12 12		/*	NUMB	::= "number" 		*/
#define GA13 13		/*	REAL 	::= "realnum"		*/
#define GA14 14		/*	IDEN	::= "undid"		*/

	/* kody pre konstantu alebo premennu na mieste st. typu */
#define STCON 15	/*      sttyp ::= konstanta		*/
#define STVAR 16	/* 	sttyp ::= premenna		*/
#define IDENTITY	/* (X)X               	code   */ 17

	/* standardne funkcie a ich kody !!!!
           !!!!!!!! musia korespondovat s code v st. moduloch !!!!!!!!! */
	/* module int.t */
#define  PLUS 	/* (number,number)number	code */ 64
#define  MINUS 	/* (number,number)number	code */ 65
#define  TIMES 	/* (number,number)number	code */ 66
#define  DIVIDE /* (number,number)number	code */ 67
#define  UNMINUS /*(number)number		code */ 68
  
#define  DIV2 	/* (number)number		code */ 69
#define  MUL2 	/* (number)number		code */ 70

#define  NLESS 		/* (number,number)boolean code */ 71
#define  NGREATER 	/* (number,number)boolean code */ 72
#define  NLESSEQ 	/* (number,number)boolean code */ 73
#define  NGREATEREQ 	/* (number,number)boolean code */ 74
#define  NEQ 		/* (number,number)boolean code */ 75
#define  NNEQ 		/* (number,number)boolean code */ 76
#define  NODD 		/* (number) boolean	code */ 77
#define  CHCHTOI	/* (character)number    code */ 78
#define  CHITOCH	/* (number)character    code */ 79

        /*---------------------------------------*/
#define STOPBEG 64	/* zaciatok a koniec kodov pre */
#define STOPEND 192	/* standardne operacie		*/

#define VARIABEG 192  /*  zaciatok kodov pre premenne */
#define VARIAEND 384 /*  koniec premennych */

#define TRUEVAL 1
#define FALSEVAL 0

#define IREDTERM 01000000
#define NULLARY  02000000
#define UNARY    04000000
#define MORE     06000000
#define ARIMSK   06000000  /* bity v symbole */
#define FSMASK   00177777

	/* typy informacii v pravej strane pravidla */
#define VAR 0   /*premenna*/
#define CON 1   /*konstanta (alebo funkcny symbol)*/
#define OFF 2   /*offset*/

#define P(memi) ((MEMEL *) (memi))
#define I(memi) ((int) (memi))
#define CH(memi) ((char) (memi))
#define M(memi) ((MEMEL) (memi))
#define PUSHB(inf) if (top>=stufp) stackover();\
			    else (top++)->t = P(inf)
#define MTEST(n) {tstsize=n; memtst();}

#define ARITA(q) (((q)>=VARIABEG&&(q)<VARIAEND)?0:(arity[((q)>>6)&1023][(q)&63]))
#define BMASK(q) (bitmsk[(q>>6)&1023][q&63])

#define ISUBT(p,i) P(P(p[1])[i-1])
#define AISUBT(p,i) (P(p[1])+i-1)

#define SIZEMEMEL sizeof(			\
  union {					\
	int inum;				\
	char charact;				\
	float realnum;				\
	int bool;				\
	int undid;				\
	MEMEL *pointer;				\
	long symb; /* bity 0..15 cislo, 16..31 flagy */			\
  })

struct rtnode 	{ unsigned fsym;
		  unsigned OKset;
		  struct rtna { unsigned ELIMset;
				int varpos;
				struct rtlistn { struct rtnode * head;
						 struct rtlistn* tail;
						} setrt;
			       } subrt;
		};
typedef struct rtlistn *RTLIST;
typedef struct rtnode *RT;

#define HRTSIZE (sizeof(struct rtnode)-sizeof(struct rtna))
#define SRTSIZE sizeof(struct rtna)
#define Other 0

typedef unsigned long MEMEL;	/* !!! je to akoby union memelun,
				  ktory sa da priradit !!!!! */
struct stackel {
  MEMEL *t;
  unsigned n;
};

struct rsformat {
  char bits;
  MEMEL el;
};

struct slist {
	struct slist *next;
	int symbol;
	};


struct rulelist {
  struct rulelist *next;
  unsigned symbol;		/* koren symbol lavych stran */
  int numrul;			/* pocet pravidiel v rulist  */
  MEMEL *rulist;
};

struct stfevall {
  struct stfevall *next;
  unsigned offs;
  MEMEL *st;
};

extern MEMEL *dfp,*sfp,*pdfp,*pufp,*ufp,*actp,*actuf;
extern struct stackel *top,*toop,*stufp;
extern int actfreepart;		/* premenne pre spravu pamate (vid.memor.c*/
extern int tstsize; 		/* kolko volneho miesta treba pri memtst() */
extern int hruleari;        /* arita nacitaneho tela pravidla (nadst. hruleb)*/
extern int numvars;         /* pocet premennych v module */
extern int varposi;         /* velkost var framu */
extern unsigned hrulemsk;   /* bitmaska typov podtermov (stand.<-> nest.)*/
extern unsigned *arity[1024];	  /* pole poli arit funkcnych symb. */
extern unsigned *bitmsk[1024];    /* pole bitmasiek typu podtermov */
extern struct slist **canred[1024];  /* pole zoznamov, kde je redukcia */
extern unsigned *toutterm[1024];	/* pole pre vystup termu */
extern int maxlsdeep,maxrslen;    /* max. hlbka lavej strany,
				     max dlzka pravej strany */
extern int maxvarp;
extern int vartyp[VARIAEND-VARIABEG];/* vartyp[varn], typ premennej */
extern int *nwhen;		/* nwhen[n] urcuje pocet when-ov v pravidle*/
extern int *vardivi;		/* vardivi[varn,ruln] urcuje poziciu
				   premennej v stack frame */

/*RT buildrrt();*/
/*MEMEL *ccopy();*/
/*MEMEL *reduce();*/


extern int snval,suival;
extern char schval;
extern float srnval;		/* hodnoty predposlednych lexem */
