/* A 3-state excerpt in the form of the generated ppexparser.cc (test of front/lr/gen). */
#define STTAB   ppexsttab
#define PARSER  ppexsyntan
#define SEMACT  ppexsemact

#include "commondefs.h"

#define RWAC (-132+BOFIDENT)
#define RWdc (-199+BOFIDENT)
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
{IDENTIFIER,shift,{12}}, {NUMBER,shift,{10}}, {-'\'',shift,{9}}, 
{DEFAULT,reduction,{704,16,2}}, {RWAC,reduction,{1,17,2}}, {0,shift,{0}}};

 static struct sak {
     int sym,tostate;
 } apak[]={
{16,1}, {17,2}, {13,6}, 
{0,0}};

 struct sst {
	struct spr *pr;
	int npr;
	struct sak *ak;
	int nak;
 } STTAB[]={
{&appr[0],1,&apak[0],2}, {&appr[1],1,&apak[2],0}, {&appr[2],6,&apak[2],1}};
