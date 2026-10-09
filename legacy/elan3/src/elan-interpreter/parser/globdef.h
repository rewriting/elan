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



#define RULTL 9000		/* totalna velkost pravidiel */
#define NNET (1024+NSTNET)	/* pocet neterminalov		*/
#define NTER (BRESW+NRWORD)	/* pocet terminalov		*/
#define POLTL 6000		/* totalna velkost poloziek v stavoch 11000*/
#define NSTAT 900		/* pocet stavov			*/
#define HASHSIZE 1024            /* velkost hash tab  (viac ako NSTAT)*/
#define MAXLP 400		/* max. pocet poloziek v stave	*/


#define NSTNET 4                /* pocet a kody system neterminalov */
#define STAR1NET 1024
#define STAR2NET 1025
#define SWHTNET  1026
#define SWHT2NET 1027

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1  
#endif

#define STSIZE 500			/* velkost zasobnika pri synt. analyze 	*/

#define DEFAULTSYM -32000	/* terminalny symbol pre neuvedene symboly */
						/* pri akcii v stave 	*/
#define NRWORD 200			/* max. pocet rezervovanych slov	*/
#define NUIDENT 100			/* max. pocet nedefinovanych ident.	*/

#define ZEROCHAR 1	/* lexemy : 0,...,LASTCHAR = znaky, */
#define LASTCHAR 255	
#define APOSCHAR 256
#define DBOD     257
#define ARGUM    258
#define ENDOI    259
#define SIPK     260
#define NUMBER   261
#define REALNUM  262
#define UNDID    263
#define UNDID_   264
#define BRESW    265	/* BRESW,...,BRESW+NRWORD=rezervovane slova */



#define NORMCONT 0                 /* navratove hodnoty semaction */
#define ERRORIM  1
#define ACCEPTIM 2


#define NMAC   100      /* max pocet makier */

                        /* kody rezervovanych slov v TRAPASE */

 /* !!!!!!!!!!! pozor, tieto hodnoty sa viazu k tabulke rwt z typantab.c !!!*/
 /* !!!!!!!!!!!        pri pregenerovani gramatiky ich treba opravit !!!!!!!*/

#define STdefine    (19+BRESW)
#define STinclude   (20+BRESW)
#define STleftasoc  ( 9+BRESW)
#define STrightasoc ( 4+BRESW)
#define STnoasoc    (14+BRESW)
#define STtype      ( 0+BRESW)
#define STextension (29+BRESW)
#define STtypeimport   ( 3+BRESW)
#define STopimport     (21+BRESW)
#define STopimporti    ( 5+BRESW)
#define STop        (13+BRESW)
#define STimport    ( 7+BRESW)
#define STalias     (12+BRESW)
#define STlink      (10+BRESW)
#define STvar       ( 1+BRESW)
#define STaxiom     ( 2+BRESW)
#define STend       (11+BRESW)
#define STpri       ( 6+BRESW)
#define STlocop     ( 8+BRESW)
#define STcode      (22+BRESW)

#define UNDEFPRI  0177777	/* konstanta pre nedefinovanu prioritu */
				/* a asociativitu !! nesmie byt rovna 0	*/
#define MAXPRI    0037776
#define ASOCMSK	  0140000
#define PRIMSK    0037777
#define NOASOC    0140000
#define LEFTASOC  0100000
#define RIGHTASOC 0040000

#define MLENMAC   500    /* max dlzka tela makra */
#define MAXARG    31     /* max pocet argumentov makra (max=31) */
#define MAXINMAC  10     /* max vnorenie makier  */




typedef struct {
	  int instr;			/* 0-nie je v retazci */
	  int fch;			/* prvy znak */
	  FILE *f;			/* ostatne znaky */
          char *name;                   /* meno suboru */
          int line,pos;                 /* akt. pozicia v subore */
	  } INFILE;

typedef struct {			/* tabulka identifikatorov */
	  int maxid,nidin;		/* rozmer tabulky,pocet id. v tab. */
	  char **id;
	  } TABID;





struct rslist {
	struct rslist *next;
	int rside;
	};

struct flist {
	struct flist *next;
	int symbol;
	};

struct usedmodl {                    /* zoznam pouzitych modulov pre make */
        struct usedmodl *next;
        int modn;                    /* cislo modulu */
        struct flist *moimi;         /* zoznam opimporti modulov (+alias) */
        };


struct setpol {
	int number;		/* pocet poloziek od pol */
	int *pol;                /* smernik do polbody    */
	};

struct actlist {			/* prechodova fcia v stave */
	struct actlist *next;
	int symbol;                     /* terminalny symbol */
	enum actenum {reduction,accept,shift,srerr,rrerr} action;
	union { struct { int num,left,lenght;
		       } redpar;
		int shiftstate;
	      } act;
	};

struct gotolist {
	struct gotolist *next;
	int symbol;
	int tostate;
	};

struct lalist {
	struct lalist *next;
	int symbol;
	struct flist *syms;
	};

struct state {
	struct setpol    polozky;
	struct lalist	 *laset;
	struct gotolist  *gotos;
	struct actlist   *actions;
	};

struct laaddl {
        struct flist **tolas;
        struct lalist *froml;
        struct laaddl *next;
        };

  struct spr {
     int sym;
     enum actenum action;
     struct { int num,left,lenght;
            } actpar;
  };

 struct sak {
     int sym,tostate;
 }; 

 struct sst {
     struct spr *pr;
     int npr;
     struct sak *ak;
     int nak;
 }; 


extern int warni;		/* flag, ci ma hlasit warningy */


extern struct rslist **rulenet;	/* rulenet[NNET]: zoznamy pravidiel pre */
				/*  	          neterminaly		*/

extern int *rbody;		/* rbody[RULTL]: tela pravidiel */
extern int irbody;

extern int *polbody;		/* polbody[POLTL]: tela poloziek  ==	*/
				/* indexy do rbody 	*/
extern int ipolbody;

extern struct state *sttab;	/* sttab[NSTAT]: tabulka stavov */
extern int isttab;


extern unsigned *terpri;     /* terpri[NTER]: priority terminalnych symbolov */

extern TABID *nultab,*prwt,*uit,*mactab;
                             /* tabulky identifikatorov */

extern int Send,Sfor,Sotherwise,Swhen;
                            /* kody preddefinovanych slov */

extern int posid;            /* pozicia identifikatora (nadstavuje idmember) */

extern int optim;            /* flag pre macc, ci ma optimalizovat redukcie */

extern int *st1net,*st2net,*st3net,*st4net,
           *st5net,*st6net; /* adresy, kam treba doplnit typ axiom */

extern TABID *rwt;            /* rez. slova trapasu */
extern struct spr ppr[];     /* prechody z bootstr. gr */
extern struct sak pak[];     /* akcie */
extern struct sst sttb[];    /* tabulka stavov */

extern struct flist *euselist,*eiuselist; /* zoznamy useovanych modulov */
extern unsigned wasinimp[1024/NBITS];     /* flagy importov pre typy */

extern INFILE *rinf;         /* vst subor pre citanie gramatiky */
extern INFILE inf;           /* vst subor pre sa modulu */
extern INFILE *aff;          /* aktualny subor pre ich */
extern int begru;            /* index lavej str. pravidla (nadst. hruleb) */
extern int lstsat;           /* posledny znak po analyze typu */

extern int inruleb;          /* pre lexan, ci citam telo gr. pravidla */
extern int inimport;         /* pre ntyp, ci ma hlasit fatal na ned. typ */

extern int nval,uival;
extern char chval;
extern float rnval;          /* vedlajsie hodnoty lexem (nadst, lexan) */

extern char amacb[];
extern int amacbi;          /* bufer, pri sa typu */

extern int enabmac;         /* flag pre lexan, ci moze rozvijat makra */
extern int amodn,actmon,actopn;   /* aktualne cislo modulu a operacie */
extern char gramfile[],*gramfiln;  /* meno pre subory pre gramatiku */


char *alfa(int sy);			/* prevedie lexemu na retazec */

int idmember(char *id, TABID *t);
int idadd(char *id, int l, TABID *t);
TABID *idtaballoc(int n);             /* operacie s tabulkami ident */

void oferr(INFILE *f, char *t, ...);
void oerr(INFILE *f, int w, char *t, ...);
void oadderr(char *t, ...);        /* vypisy chybovych hlasok */

void statedump(void);
void stbdump(int i);
void stategen(void);
void compla(void);
void completestate(void);
void gensa(FILE *saf, int name);
/*int ssyntan();*/
void tiddump(FILE *f, TABID *t, char *name);
void statistics(void);   /* operacie z generatora synt. tab*/

void freesttabl(void);                    /* uvolnenie pam. zo zoznamov v sttab */
void freerulenet(void);
void freidt(void);
void freeiniallo(void);                   /* uvolnovanie pamate */
void iniallo(void);
void init(void);
void reinit(void);/* inicializacia a reinicializacia tab. sa*/

/*int semaction();*/                 /* bude to stavba termu pri sa pravidiel*/

char *envirgetname(int s);
void envirread(void);
int enviradd(int t, char *gt);
int envirnumallo(int tn, int ln);
/*int envirgrtget();*/
void envirsetal(int i, char *gt);
int isinenvir(char *gt);
int enviraliget(int n);
void envirwrite(void);
int envirget(int t, char *gt);        /* operacie s environmentom */

/*int syntan();*/
/*int semact();*/              /* op pri sa typu z bootstrapovaneho sa*/

void readgram(char *n, int t);
int ntyp(void);
int hruleb(int t, int reallyimp);
int nsym(void);/* op citania gramat. (citaju z *rinf) */

void outruleb(int b, int e, FILE *f);                       /* vystup gramat */

void exp(int s);
void trapsa(char *name);
int hasocpri(INFILE *f, int sy);        /* synt analyza modulu */

void hcomline(char *name, char *fnam, int *aar, int *mmodn);
/*char *grname();*/

int opeinfile(char *s, INFILE *ff);
void cloinfile(INFILE *ff);
int ich(void);
                           /* praca so vstupnym suborom */

int sast(TABID *rw, TABID *ui, INFILE *f);
int prodef(INFILE *f, int sy);
void pushmac(char *t, int i);
void popmac(void);
int handlar(TABID *rw, TABID *ui, INFILE *f);
void handlfarg(int ar);
int lexan(TABID *rw, TABID *ui, INFILE *ff); /* pomoc pri sa modulu */

void resetmactab(void);                  /* vycisti tabulku makier */

char *alfsy(int s, TABID *t);
char *alfstter(int s);                /* text symbolu */

/*int setsyminf();*/				/* nadstavi informaciu o fun.symbole */

void init0(void);
void addENDOIaccept(void);
void idwrite(FILE *f, TABID *t);

#define TIDFRE(t) {CFRE(t->id); CFRE(t);}
#define FREEIDT(p) {register int k;for(k=0;k<p->maxid;k++){ \
				  CFRE(p->id[k]);p->id[k]=NULL;\
			       } p->nidin=0;}


#define RBODYALLO(n) ALLOSS(n,int)
#define POLBODYALLO(n) ALLOSS(n,int)
#define STTABALLO(n) ALLOSS(n,struct state)
#define TERPRIALLO(n) ALLOSS(n,unsigned)
#define RULENETALLO(n) ALLOSS(n,struct rslist *)

#define BIFOUND(p,s,i,j) \
  { while (j!=i+1) { k=(i+j)/2; if (p[k].sym<=s) i=k; else j=k; }; \
    p= &p[i]; \
  }

