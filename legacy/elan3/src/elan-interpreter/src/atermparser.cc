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


#define STTAB   atermsttab
#define PARSER  atermsyntan
#define SEMACT  atermsemact
#define RESWTAB atermrwt
#define ILEXEM  ilex 

#include "commondefs.h"


#define RWDC (-135+BOFIDENT)
#define RWDK (-143+BOFIDENT)
#define RWdc (-199+BOFIDENT)
#define RWid (-205+BOFIDENT)
#define RWdk (-207+BOFIDENT)
#define RWIFF (-213+BOFIDENT)
#define RWONE (-226+BOFIDENT)
#define RWVAR (-233+BOFIDENT)
#define RWINT (-235+BOFIDENT)
#define RWTRY (-255+BOFIDENT)
#define RWMETA (-295+BOFIDENT)
#define RWEVAR (-302+BOFIDENT)
#define RWNum (-304+BOFIDENT)
#define RWend (-311+BOFIDENT)
#define RWRULE (-312+BOFIDENT)
#define RWFSYM (-319+BOFIDENT)
#define RWone (-322+BOFIDENT)
#define RWnil (-323+BOFIDENT)
#define RWIDENT (-372+BOFIDENT)
#define RWWHERE (-379+BOFIDENT)
#define RWChar (-382+BOFIDENT)
#define RWQUERY (-406+BOFIDENT)
#define RWcall (-412+BOFIDENT)
#define RWType (-418+BOFIDENT)
#define RWPWHERE (-459+BOFIDENT)
#define RWSWITCH (-466+BOFIDENT)
#define RWSTRING (-471+BOFIDENT)
#define RWSWRULE (-482+BOFIDENT)
#define RWBlank (-488+BOFIDENT)
#define RWIdent (-500+BOFIDENT)
#define RWEndDef (-550+BOFIDENT)
#define RWdccall (-611+BOFIDENT)
#define RWdkcall (-619+BOFIDENT)
#define RWNOSWITCH (-623+BOFIDENT)
#define RWSTRATEGY (-627+BOFIDENT)
#define RWString (-631+BOFIDENT)
#define RWrepeat (-641+BOFIDENT)
#define RWiterate (-750+BOFIDENT)
#define RWfail (-834+BOFIDENT)
#define RWGrammarForSort (-1430+BOFIDENT)
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
{RWEndDef,shift,{5}}, {RWGrammarForSort,shift,{3}}, {EOFINPUT,accept,{0}}, 
{RWEndDef,shift,{5}}, {RWGrammarForSort,shift,{3}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {DEFAULT,reduction,{708,37,1}}, {RWend,shift,{11}}, 
{RWEndDef,shift,{5}}, {RWGrammarForSort,shift,{3}}, {-':',shift,{13}}, 
{DEFAULT,reduction,{16,39,1}}, {NUMBER,shift,{14}}, {DEFAULT,reduction,{7,50,1}}, 
{DEFAULT,reduction,{719,44,2}}, {RWRULE,shift,{17}}, {RWSWRULE,shift,{18}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {DEFAULT,reduction,{8,50,2}}, 
{RWEndDef,shift,{5}}, {RWRULE,shift,{17}}, {RWSWRULE,shift,{18}}, 
{DEFAULT,reduction,{718,22,1}}, {-'(',shift,{22}}, {-'(',shift,{23}}, 
{-':',shift,{24}}, {DEFAULT,reduction,{717,22,2}}, {RWEndDef,shift,{5}}, 
{RWSTRATEGY,shift,{27}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {RWnil,shift,{33}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {RWQUERY,shift,{37}}, 
{DEFAULT,reduction,{6468,27,1}}, {-'(',shift,{38}}, {-',',shift,{39}}, 
{DEFAULT,reduction,{37,51,1}}, {-',',shift,{40}}, {RWend,shift,{41}}, 
{-'.',shift,{42}}, {DEFAULT,reduction,{704,33,1}}, {-':',shift,{43}}, 
{DEFAULT,reduction,{17,48,1}}, {DEFAULT,reduction,{6470,79,7}}, {-'(',shift,{44}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{RWEndDef,shift,{5}}, {RWGrammarForSort,shift,{3}}, {RWnil,shift,{33}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{-',',shift,{56}}, {DEFAULT,reduction,{179,76,1}}, {-',',shift,{57}}, 
{DEFAULT,reduction,{712,28,1}}, {-',',shift,{58}}, {DEFAULT,reduction,{709,37,8}}, 
{DEFAULT,reduction,{705,33,3}}, {-':',shift,{59}}, {DEFAULT,reduction,{14,46,1}}, 
{-',',shift,{60}}, {DEFAULT,reduction,{140,56,1}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{-',',shift,{70}}, {DEFAULT,reduction,{180,69,1}}, {-',',shift,{71}}, 
{DEFAULT,reduction,{38,53,1}}, {-',',shift,{72}}, {-':',shift,{73}}, 
{DEFAULT,reduction,{13,45,1}}, {-',',shift,{74}}, {DEFAULT,reduction,{141,57,1}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {-',',shift,{83}}, {-',',shift,{84}}, 
{DEFAULT,reduction,{39,71,1}}, {-',',shift,{85}}, {-':',shift,{86}}, 
{DEFAULT,reduction,{9,31,1}}, {-',',shift,{87}}, {DEFAULT,reduction,{142,59,1}}, 
{RWdc,shift,{107}}, {RWiterate,shift,{104}}, {RWDK,shift,{109}}, 
{RWDC,shift,{110}}, {RWone,shift,{108}}, {RWMETA,shift,{90}}, 
{RWrepeat,shift,{105}}, {RWfail,shift,{91}}, {RWONE,shift,{111}}, 
{RWdkcall,shift,{96}}, {RWcall,shift,{93}}, {RWdccall,shift,{97}}, 
{RWdk,shift,{106}}, {RWid,shift,{92}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {RWIDENT,shift,{121}}, 
{RWFSYM,shift,{119}}, {RWEVAR,shift,{123}}, {RWSTRING,shift,{120}}, 
{RWINT,shift,{122}}, {RWVAR,shift,{124}}, {-';',shift,{125}}, 
{-')',shift,{126}}, {DEFAULT,reduction,{160,88,1}}, {DEFAULT,reduction,{197,87,1}}, 
{DEFAULT,reduction,{195,87,1}}, {DEFAULT,reduction,{196,87,1}}, {-'(',shift,{127}}, 
{-'(',shift,{128}}, {-'(',shift,{129}}, {-'(',shift,{130}}, 
{-'(',shift,{131}}, {-'(',shift,{132}}, {-'(',shift,{133}}, 
{-'(',shift,{134}}, {-'(',shift,{135}}, {-'(',shift,{136}}, 
{-'(',shift,{137}}, {-'*',shift,{138}}, {-'*',shift,{139}}, 
{DEFAULT,reduction,{6466,90,1}}, {DEFAULT,reduction,{6465,89,1}}, {DEFAULT,reduction,{6464,82,1}}, 
{DEFAULT,reduction,{163,85,1}}, {DEFAULT,reduction,{163,84,1}}, {DEFAULT,reduction,{163,81,1}}, 
{-',',shift,{140}}, {DEFAULT,reduction,{36,80,1}}, {-',',shift,{141}}, 
{-':',shift,{142}}, {DEFAULT,reduction,{15,47,1}}, {-',',shift,{143}}, 
{DEFAULT,reduction,{143,60,1}}, {-'(',shift,{144}}, {-'(',shift,{145}}, 
{-'(',shift,{146}}, {-'(',shift,{147}}, {-'(',shift,{148}}, 
{-'(',shift,{149}}, {RWdc,shift,{107}}, {RWiterate,shift,{104}}, 
{RWDK,shift,{109}}, {RWDC,shift,{110}}, {RWone,shift,{108}}, 
{RWMETA,shift,{90}}, {RWrepeat,shift,{105}}, {RWfail,shift,{91}}, 
{RWONE,shift,{111}}, {RWdkcall,shift,{96}}, {RWcall,shift,{93}}, 
{RWdccall,shift,{97}}, {RWdk,shift,{106}}, {RWid,shift,{92}}, 
{RWend,shift,{152}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{RWdc,shift,{107}}, {RWiterate,shift,{104}}, {RWDK,shift,{109}}, 
{RWDC,shift,{110}}, {RWone,shift,{108}}, {RWMETA,shift,{90}}, 
{RWrepeat,shift,{105}}, {RWfail,shift,{91}}, {RWONE,shift,{111}}, 
{RWdkcall,shift,{96}}, {RWcall,shift,{93}}, {RWdccall,shift,{97}}, 
{RWdk,shift,{106}}, {RWid,shift,{92}}, {RWdc,shift,{107}}, 
{RWiterate,shift,{104}}, {RWDK,shift,{109}}, {RWDC,shift,{110}}, 
{RWone,shift,{108}}, {RWMETA,shift,{90}}, {RWrepeat,shift,{105}}, 
{RWfail,shift,{91}}, {RWONE,shift,{111}}, {RWdkcall,shift,{96}}, 
{RWcall,shift,{93}}, {RWdccall,shift,{97}}, {RWdk,shift,{106}}, 
{RWid,shift,{92}}, {RWnil,shift,{159}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {RWnil,shift,{159}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {RWnil,shift,{164}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {RWnil,shift,{164}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {RWnil,shift,{164}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {RWdc,shift,{107}}, {RWiterate,shift,{104}}, 
{RWDK,shift,{109}}, {RWDC,shift,{110}}, {RWone,shift,{108}}, 
{RWMETA,shift,{90}}, {RWrepeat,shift,{105}}, {RWfail,shift,{91}}, 
{RWONE,shift,{111}}, {RWdkcall,shift,{96}}, {RWcall,shift,{93}}, 
{RWdccall,shift,{97}}, {RWdk,shift,{106}}, {RWid,shift,{92}}, 
{RWdc,shift,{107}}, {RWiterate,shift,{104}}, {RWDK,shift,{109}}, 
{RWDC,shift,{110}}, {RWone,shift,{108}}, {RWMETA,shift,{90}}, 
{RWrepeat,shift,{105}}, {RWfail,shift,{91}}, {RWONE,shift,{111}}, 
{RWdkcall,shift,{96}}, {RWcall,shift,{93}}, {RWdccall,shift,{97}}, 
{RWdk,shift,{106}}, {RWid,shift,{92}}, {RWdc,shift,{107}}, 
{RWiterate,shift,{104}}, {RWDK,shift,{109}}, {RWDC,shift,{110}}, 
{RWone,shift,{108}}, {RWMETA,shift,{90}}, {RWrepeat,shift,{105}}, 
{RWfail,shift,{91}}, {RWONE,shift,{111}}, {RWdkcall,shift,{96}}, 
{RWcall,shift,{93}}, {RWdccall,shift,{97}}, {RWdk,shift,{106}}, 
{RWid,shift,{92}}, {DEFAULT,reduction,{163,92,2}}, {DEFAULT,reduction,{163,91,2}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{RWIDENT,shift,{121}}, {RWFSYM,shift,{119}}, {RWEVAR,shift,{123}}, 
{RWSTRING,shift,{120}}, {RWINT,shift,{122}}, {RWVAR,shift,{124}}, 
{RWIDENT,shift,{121}}, {RWnil,shift,{181}}, {RWFSYM,shift,{119}}, 
{RWEVAR,shift,{123}}, {RWSTRING,shift,{120}}, {RWINT,shift,{122}}, 
{RWVAR,shift,{124}}, {RWnil,shift,{159}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{DEFAULT,reduction,{160,88,3}}, {RWEndDef,shift,{5}}, {RWSTRATEGY,shift,{27}}, 
{DEFAULT,reduction,{181,70,1}}, {-')',shift,{189}}, {DEFAULT,reduction,{182,77,1}}, 
{-';',shift,{125}}, {-')',shift,{190}}, {-';',shift,{125}}, 
{-')',shift,{191}}, {-',',shift,{192}}, {-'.',shift,{193}}, 
{DEFAULT,reduction,{710,41,1}}, {DEFAULT,reduction,{32,42,1}}, {-',',shift,{194}}, 
{-')',shift,{195}}, {-'.',shift,{196}}, {DEFAULT,reduction,{190,75,1}}, 
{DEFAULT,reduction,{192,52,1}}, {-')',shift,{197}}, {-')',shift,{198}}, 
{-',',shift,{200}}, {-')',shift,{199}}, {-';',shift,{125}}, 
{-',',reduction,{177,86,1}}, {-')',reduction,{177,86,1}}, {-',',shift,{200}}, 
{-')',shift,{201}}, {-',',shift,{200}}, {-')',shift,{202}}, 
{-',',shift,{203}}, {DEFAULT,reduction,{33,83,1}}, {-',',shift,{204}}, 
{-':',shift,{205}}, {DEFAULT,reduction,{10,72,1}}, {-')',shift,{206}}, 
{DEFAULT,reduction,{144,61,1}}, {-',',shift,{207}}, {-'.',shift,{208}}, 
{DEFAULT,reduction,{713,13,1}}, {-')',shift,{209}}, {-')',shift,{210}}, 
{-')',shift,{211}}, {-',',shift,{212}}, {DEFAULT,reduction,{18,34,1}}, 
{-',',shift,{213}}, {DEFAULT,reduction,{6467,27,12}}, {DEFAULT,reduction,{159,87,4}}, 
{DEFAULT,reduction,{165,87,4}}, {DEFAULT,reduction,{164,87,4}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {RWnil,shift,{159}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{DEFAULT,reduction,{174,87,4}}, {RWnil,shift,{164}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {DEFAULT,reduction,{173,87,4}}, {DEFAULT,reduction,{172,87,4}}, 
{DEFAULT,reduction,{176,87,4}}, {RWdc,shift,{107}}, {RWiterate,shift,{104}}, 
{RWDK,shift,{109}}, {RWDC,shift,{110}}, {RWone,shift,{108}}, 
{RWMETA,shift,{90}}, {RWrepeat,shift,{105}}, {RWfail,shift,{91}}, 
{RWONE,shift,{111}}, {RWdkcall,shift,{96}}, {RWcall,shift,{93}}, 
{RWdccall,shift,{97}}, {RWdk,shift,{106}}, {RWid,shift,{92}}, 
{DEFAULT,reduction,{175,87,4}}, {DEFAULT,reduction,{171,87,4}}, {RWIDENT,shift,{121}}, 
{RWFSYM,shift,{119}}, {RWEVAR,shift,{123}}, {RWSTRING,shift,{120}}, 
{RWINT,shift,{122}}, {RWVAR,shift,{124}}, {RWIDENT,shift,{121}}, 
{RWFSYM,shift,{119}}, {RWEVAR,shift,{123}}, {RWSTRING,shift,{120}}, 
{RWINT,shift,{122}}, {RWVAR,shift,{124}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {RWend,shift,{223}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {RWIDENT,shift,{121}}, {RWnil,shift,{181}}, 
{RWFSYM,shift,{119}}, {RWEVAR,shift,{123}}, {RWSTRING,shift,{120}}, 
{RWINT,shift,{122}}, {RWVAR,shift,{124}}, {DEFAULT,reduction,{23,12,4}}, 
{DEFAULT,reduction,{22,12,4}}, {DEFAULT,reduction,{21,12,4}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{-',',shift,{230}}, {DEFAULT,reduction,{711,41,3}}, {-',',shift,{231}}, 
{DEFAULT,reduction,{191,75,3}}, {-';',shift,{125}}, {-',',reduction,{178,86,3}}, 
{-')',reduction,{178,86,3}}, {-',',shift,{232}}, {DEFAULT,reduction,{34,66,1}}, 
{-',',shift,{233}}, {-':',shift,{234}}, {DEFAULT,reduction,{6469,62,13}}, 
{-')',shift,{235}}, {DEFAULT,reduction,{25,63,1}}, {DEFAULT,reduction,{714,13,3}}, 
{-')',shift,{236}}, {DEFAULT,reduction,{19,35,1}}, {-')',shift,{237}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {RWIDENT,shift,{121}}, {RWFSYM,shift,{119}}, 
{RWEVAR,shift,{123}}, {RWSTRING,shift,{120}}, {RWINT,shift,{122}}, 
{RWVAR,shift,{124}}, {RWSWITCH,shift,{243}}, {RWNOSWITCH,shift,{244}}, 
{RWChar,shift,{253}}, {RWnil,shift,{247}}, {RWNum,shift,{252}}, 
{RWIdent,shift,{250}}, {RWBlank,shift,{249}}, {RWString,shift,{251}}, 
{RWType,shift,{254}}, {DEFAULT,reduction,{24,12,6}}, {DEFAULT,reduction,{20,12,6}}, 
{DEFAULT,reduction,{20,12,6}}, {-')',shift,{255}}, {-')',shift,{256}}, 
{-',',shift,{257}}, {DEFAULT,reduction,{41,67,1}}, {-')',shift,{258}}, 
{-'(',shift,{259}}, {-'(',shift,{260}}, {-':',shift,{261}}, 
{-'.',shift,{262}}, {DEFAULT,reduction,{706,30,1}}, {DEFAULT,reduction,{40,40,1}}, 
{DEFAULT,reduction,{6,29,1}}, {-'(',shift,{263}}, {-'(',shift,{264}}, 
{-'(',shift,{265}}, {-'(',shift,{266}}, {-'(',shift,{267}}, 
{DEFAULT,reduction,{184,87,8}}, {DEFAULT,reduction,{183,87,8}}, {RWWHERE,shift,{272}}, 
{RWnil,shift,{270}}, {RWPWHERE,shift,{271}}, {RWTRY,shift,{275}}, 
{RWIFF,shift,{274}}, {RWend,shift,{276}}, {RWWHERE,shift,{272}}, 
{RWnil,shift,{270}}, {RWPWHERE,shift,{271}}, {RWTRY,shift,{275}}, 
{RWIFF,shift,{274}}, {RWWHERE,shift,{272}}, {RWnil,shift,{270}}, 
{RWPWHERE,shift,{271}}, {RWTRY,shift,{275}}, {RWIFF,shift,{274}}, 
{RWnil,shift,{280}}, {RWChar,shift,{253}}, {RWnil,shift,{247}}, 
{RWNum,shift,{252}}, {RWIdent,shift,{250}}, {RWBlank,shift,{249}}, 
{RWString,shift,{251}}, {RWType,shift,{254}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{-')',shift,{287}}, {-'.',shift,{288}}, {DEFAULT,reduction,{715,15,1}}, 
{-'(',shift,{289}}, {-'(',shift,{290}}, {-'(',shift,{291}}, 
{-'(',shift,{292}}, {DEFAULT,reduction,{126,23,1}}, {DEFAULT,reduction,{136,20,19}}, 
{-',',shift,{293}}, {DEFAULT,reduction,{137,16,1}}, {-',',shift,{294}}, 
{DEFAULT,reduction,{11,32,17}}, {DEFAULT,reduction,{707,30,3}}, {-')',shift,{295}}, 
{-')',shift,{296}}, {-')',shift,{297}}, {-')',shift,{298}}, 
{-')',shift,{299}}, {RWend,shift,{300}}, {RWWHERE,shift,{272}}, 
{RWnil,shift,{270}}, {RWPWHERE,shift,{271}}, {RWTRY,shift,{275}}, 
{RWIFF,shift,{274}}, {RWIDENT,shift,{121}}, {RWFSYM,shift,{119}}, 
{RWEVAR,shift,{123}}, {RWSTRING,shift,{120}}, {RWINT,shift,{122}}, 
{RWVAR,shift,{124}}, {RWIDENT,shift,{121}}, {RWFSYM,shift,{119}}, 
{RWEVAR,shift,{123}}, {RWSTRING,shift,{120}}, {RWINT,shift,{122}}, 
{RWVAR,shift,{124}}, {RWWHERE,shift,{272}}, {RWnil,shift,{307}}, 
{RWPWHERE,shift,{271}}, {RWTRY,shift,{275}}, {RWIFF,shift,{274}}, 
{RWIDENT,shift,{121}}, {RWFSYM,shift,{119}}, {RWEVAR,shift,{123}}, 
{RWSTRING,shift,{120}}, {RWINT,shift,{122}}, {RWVAR,shift,{124}}, 
{RWIDENT,shift,{121}}, {RWnil,shift,{311}}, {RWFSYM,shift,{119}}, 
{RWEVAR,shift,{123}}, {RWSTRING,shift,{120}}, {RWINT,shift,{122}}, 
{RWVAR,shift,{124}}, {RWIDENT,shift,{121}}, {RWFSYM,shift,{119}}, 
{RWEVAR,shift,{123}}, {RWSTRING,shift,{120}}, {RWINT,shift,{122}}, 
{RWVAR,shift,{124}}, {DEFAULT,reduction,{5,29,4}}, {DEFAULT,reduction,{4,29,4}}, 
{DEFAULT,reduction,{3,29,4}}, {DEFAULT,reduction,{2,29,4}}, {DEFAULT,reduction,{1,29,4}}, 
{DEFAULT,reduction,{133,20,21}}, {DEFAULT,reduction,{716,15,3}}, {-',',shift,{316}}, 
{DEFAULT,reduction,{28,54,1}}, {-',',shift,{317}}, {-')',shift,{318}}, 
{-'.',shift,{319}}, {-'.',reduction,{715,15,1}}, {-',',reduction,{715,15,1}}, 
{-')',reduction,{127,18,1}}, {-')',shift,{320}}, {-')',shift,{321}}, 
{-'.',shift,{322}}, {DEFAULT,reduction,{138,95,1}}, {-',',shift,{323}}, 
{DEFAULT,reduction,{134,96,1}}, {-')',shift,{324}}, {DEFAULT,reduction,{35,68,1}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {DEFAULT,reduction,{129,14,4}}, {RWWHERE,shift,{272}}, 
{RWnil,shift,{307}}, {RWPWHERE,shift,{271}}, {RWTRY,shift,{275}}, 
{RWIFF,shift,{274}}, {DEFAULT,reduction,{30,14,4}}, {DEFAULT,reduction,{131,94,6}}, 
{RWIDENT,shift,{121}}, {RWnil,shift,{311}}, {RWFSYM,shift,{119}}, 
{RWEVAR,shift,{123}}, {RWSTRING,shift,{120}}, {RWINT,shift,{122}}, 
{RWVAR,shift,{124}}, {RWSWITCH,shift,{243}}, {RWNOSWITCH,shift,{244}}, 
{DEFAULT,reduction,{130,94,6}}, {-',',shift,{332}}, {DEFAULT,reduction,{46,17,1}}, 
{-',',shift,{333}}, {DEFAULT,reduction,{29,55,1}}, {DEFAULT,reduction,{128,18,3}}, 
{DEFAULT,reduction,{139,95,3}}, {DEFAULT,reduction,{135,97,3}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {RWIDENT,shift,{121}}, {RWFSYM,shift,{119}}, 
{RWEVAR,shift,{123}}, {RWSTRING,shift,{120}}, {RWINT,shift,{122}}, 
{RWVAR,shift,{124}}, {-',',shift,{336}}, {-')',shift,{337}}, 
{RWIDENT,shift,{121}}, {RWFSYM,shift,{119}}, {RWEVAR,shift,{123}}, 
{RWSTRING,shift,{120}}, {RWINT,shift,{122}}, {RWVAR,shift,{124}}, 
{DEFAULT,reduction,{31,14,8}}, {-')',shift,{339}}, {DEFAULT,reduction,{45,14,10}}, 
{0,shift,{0}}};

 static struct sak {
     int sym,tostate;
 } apak[]={
{37,2}, {44,4}, {79,1}, {37,6}, {44,4}, {39,7}, {50,8}, {37,12}, 
{44,4}, {20,16}, {22,15}, {50,19}, {20,20}, {44,21}, {27,25}, {44,26}, 
{50,29}, {51,28}, {50,29}, {51,30}, {32,32}, {33,31}, {48,34}, {50,35}, 
{62,36}, {50,46}, {76,45}, {28,47}, {50,48}, {28,49}, {50,48}, {37,50}, 
{44,4}, {32,32}, {33,51}, {48,34}, {50,35}, {46,52}, {50,53}, {50,55}, 
{56,54}, {50,62}, {69,61}, {50,64}, {53,63}, {50,64}, {53,65}, {45,66}, 
{50,67}, {50,69}, {57,68}, {50,64}, {53,75}, {50,77}, {71,76}, {50,77}, 
{71,78}, {31,79}, {50,80}, {50,82}, {59,81}, {81,103}, {82,100}, {84,102}, 
{85,101}, {87,89}, {88,88}, {89,99}, {90,98}, {91,95}, {92,94}, {50,113}, 
{80,112}, {50,113}, {80,114}, {47,115}, {50,116}, {12,118}, {60,117}, {81,103}, 
{82,100}, {84,102}, {85,101}, {87,150}, {89,99}, {90,98}, {91,95}, {92,94}, 
{70,151}, {50,154}, {77,153}, {81,103}, {82,100}, {84,102}, {85,101}, {87,89}, 
{88,155}, {89,99}, {90,98}, {91,95}, {92,94}, {81,103}, {82,100}, {84,102}, 
{85,101}, {87,89}, {88,156}, {89,99}, {90,98}, {91,95}, {92,94}, {41,157}, 
{42,158}, {50,160}, {41,161}, {42,158}, {50,160}, {50,29}, {51,165}, {52,163}, 
{75,162}, {50,29}, {51,165}, {52,163}, {75,166}, {50,29}, {51,165}, {52,163}, 
{75,167}, {81,103}, {82,100}, {84,102}, {85,101}, {86,168}, {87,89}, {88,169}, 
{89,99}, {90,98}, {91,95}, {92,94}, {81,103}, {82,100}, {84,102}, {85,101}, 
{86,170}, {87,89}, {88,169}, {89,99}, {90,98}, {91,95}, {92,94}, {81,103}, 
{82,100}, {84,102}, {85,101}, {86,171}, {87,89}, {88,169}, {89,99}, {90,98}, 
{91,95}, {92,94}, {50,173}, {83,172}, {50,173}, {83,174}, {50,176}, {72,175}, 
{12,178}, {61,177}, {12,180}, {13,179}, {41,182}, {42,158}, {50,160}, {50,183}, 
{50,184}, {34,185}, {50,186}, {34,187}, {50,186}, {27,188}, {44,26}, {50,77}, 
{71,214}, {41,215}, {42,158}, {50,160}, {50,77}, {71,216}, {50,29}, {51,165}, 
{52,163}, {75,217}, {81,103}, {82,100}, {84,102}, {85,101}, {87,89}, {88,218}, 
{89,99}, {90,98}, {91,95}, {92,94}, {12,220}, {66,219}, {12,220}, {66,221}, 
{50,222}, {50,225}, {63,224}, {12,180}, {13,226}, {35,227}, {50,228}, {35,229}, 
{50,228}, {50,62}, {69,238}, {50,62}, {69,239}, {12,241}, {67,240}, {94,242}, 
{29,248}, {30,245}, {40,246}, {14,269}, {15,268}, {23,273}, {14,269}, {15,278}, 
{16,277}, {23,273}, {14,269}, {15,278}, {16,279}, {23,273}, {29,248}, {30,281}, 
{40,246}, {50,282}, {50,283}, {50,284}, {50,285}, {50,286}, {14,269}, {15,301}, 
{23,273}, {12,303}, {54,302}, {12,303}, {54,304}, {14,269}, {15,278}, {16,306}, 
{18,305}, {23,273}, {12,308}, {12,313}, {95,309}, {96,312}, {97,310}, {12,315}, 
{68,314}, {17,325}, {50,326}, {50,328}, {55,327}, {14,269}, {15,278}, {16,306}, 
{18,329}, {23,273}, {12,313}, {95,330}, {96,312}, {97,310}, {94,331}, {50,328}, 
{55,334}, {12,335}, {12,338}, {0,0}};

 struct sst {
	struct spr *pr;
	int npr;
	struct sak *ak;
	int nak;
 } STTAB[]={
{&appr[0],2,&apak[0],3}, {&appr[2],1,&apak[3],0}, {&appr[3],2,&apak[3],2}, 
{&appr[5],2,&apak[5],2}, {&appr[7],1,&apak[7],0}, {&appr[8],1,&apak[7],0}, 
{&appr[9],2,&apak[7],2}, {&appr[11],1,&apak[9],0}, {&appr[12],1,&apak[9],0}, 
{&appr[13],1,&apak[9],0}, {&appr[14],1,&apak[9],0}, {&appr[15],1,&apak[9],0}, 
{&appr[16],2,&apak[9],2}, {&appr[18],2,&apak[11],1}, {&appr[20],1,&apak[12],0}, 
{&appr[21],3,&apak[12],2}, {&appr[24],1,&apak[14],0}, {&appr[25],1,&apak[14],0}, 
{&appr[26],1,&apak[14],0}, {&appr[27],1,&apak[14],0}, {&appr[28],1,&apak[14],0}, 
{&appr[29],2,&apak[14],2}, {&appr[31],2,&apak[16],2}, {&appr[33],2,&apak[18],2}, 
{&appr[35],3,&apak[20],4}, {&appr[38],1,&apak[24],1}, {&appr[39],1,&apak[25],0}, 
{&appr[40],1,&apak[25],0}, {&appr[41],1,&apak[25],0}, {&appr[42],1,&apak[25],0}, 
{&appr[43],1,&apak[25],0}, {&appr[44],1,&apak[25],0}, {&appr[45],1,&apak[25],0}, 
{&appr[46],1,&apak[25],0}, {&appr[47],1,&apak[25],0}, {&appr[48],1,&apak[25],0}, 
{&appr[49],1,&apak[25],0}, {&appr[50],1,&apak[25],0}, {&appr[51],2,&apak[25],2}, 
{&appr[53],2,&apak[27],2}, {&appr[55],2,&apak[29],2}, {&appr[57],2,&apak[31],2}, 
{&appr[59],3,&apak[33],4}, {&appr[62],2,&apak[37],2}, {&appr[64],2,&apak[39],2}, 
{&appr[66],1,&apak[41],0}, {&appr[67],1,&apak[41],0}, {&appr[68],1,&apak[41],0}, 
{&appr[69],1,&apak[41],0}, {&appr[70],1,&apak[41],0}, {&appr[71],1,&apak[41],0}, 
{&appr[72],1,&apak[41],0}, {&appr[73],1,&apak[41],0}, {&appr[74],1,&apak[41],0}, 
{&appr[75],1,&apak[41],0}, {&appr[76],1,&apak[41],0}, {&appr[77],2,&apak[41],2}, 
{&appr[79],2,&apak[43],2}, {&appr[81],2,&apak[45],2}, {&appr[83],2,&apak[47],2}, 
{&appr[85],2,&apak[49],2}, {&appr[87],1,&apak[51],0}, {&appr[88],1,&apak[51],0}, 
{&appr[89],1,&apak[51],0}, {&appr[90],1,&apak[51],0}, {&appr[91],1,&apak[51],0}, 
{&appr[92],1,&apak[51],0}, {&appr[93],1,&apak[51],0}, {&appr[94],1,&apak[51],0}, 
{&appr[95],1,&apak[51],0}, {&appr[96],2,&apak[51],2}, {&appr[98],2,&apak[53],2}, 
{&appr[100],2,&apak[55],2}, {&appr[102],2,&apak[57],2}, {&appr[104],2,&apak[59],2}, 
{&appr[106],1,&apak[61],0}, {&appr[107],1,&apak[61],0}, {&appr[108],1,&apak[61],0}, 
{&appr[109],1,&apak[61],0}, {&appr[110],1,&apak[61],0}, {&appr[111],1,&apak[61],0}, 
{&appr[112],1,&apak[61],0}, {&appr[113],1,&apak[61],0}, {&appr[114],14,&apak[61],10}, 
{&appr[128],2,&apak[71],2}, {&appr[130],2,&apak[73],2}, {&appr[132],2,&apak[75],2}, 
{&appr[134],6,&apak[77],2}, {&appr[140],2,&apak[79],0}, {&appr[142],1,&apak[79],0}, 
{&appr[143],1,&apak[79],0}, {&appr[144],1,&apak[79],0}, {&appr[145],1,&apak[79],0}, 
{&appr[146],1,&apak[79],0}, {&appr[147],1,&apak[79],0}, {&appr[148],1,&apak[79],0}, 
{&appr[149],1,&apak[79],0}, {&appr[150],1,&apak[79],0}, {&appr[151],1,&apak[79],0}, 
{&appr[152],1,&apak[79],0}, {&appr[153],1,&apak[79],0}, {&appr[154],1,&apak[79],0}, 
{&appr[155],1,&apak[79],0}, {&appr[156],1,&apak[79],0}, {&appr[157],1,&apak[79],0}, 
{&appr[158],1,&apak[79],0}, {&appr[159],1,&apak[79],0}, {&appr[160],1,&apak[79],0}, 
{&appr[161],1,&apak[79],0}, {&appr[162],1,&apak[79],0}, {&appr[163],1,&apak[79],0}, 
{&appr[164],1,&apak[79],0}, {&appr[165],1,&apak[79],0}, {&appr[166],1,&apak[79],0}, 
{&appr[167],1,&apak[79],0}, {&appr[168],1,&apak[79],0}, {&appr[169],1,&apak[79],0}, 
{&appr[170],1,&apak[79],0}, {&appr[171],1,&apak[79],0}, {&appr[172],1,&apak[79],0}, 
{&appr[173],1,&apak[79],0}, {&appr[174],1,&apak[79],0}, {&appr[175],1,&apak[79],0}, 
{&appr[176],1,&apak[79],0}, {&appr[177],1,&apak[79],0}, {&appr[178],14,&apak[79],9}, 
{&appr[192],1,&apak[88],1}, {&appr[193],2,&apak[89],2}, {&appr[195],14,&apak[91],10}, 
{&appr[209],14,&apak[101],10}, {&appr[223],3,&apak[111],3}, {&appr[226],3,&apak[114],3}, 
{&appr[229],3,&apak[117],4}, {&appr[232],3,&apak[121],4}, {&appr[235],3,&apak[125],4}, 
{&appr[238],14,&apak[129],11}, {&appr[252],14,&apak[140],11}, {&appr[266],14,&apak[151],11}, 
{&appr[280],1,&apak[162],0}, {&appr[281],1,&apak[162],0}, {&appr[282],2,&apak[162],2}, 
{&appr[284],2,&apak[164],2}, {&appr[286],2,&apak[166],2}, {&appr[288],6,&apak[168],2}, 
{&appr[294],7,&apak[170],2}, {&appr[301],3,&apak[172],3}, {&appr[304],2,&apak[175],1}, 
{&appr[306],2,&apak[176],1}, {&appr[308],2,&apak[177],2}, {&appr[310],2,&apak[179],2}, 
{&appr[312],1,&apak[181],0}, {&appr[313],2,&apak[181],2}, {&appr[315],1,&apak[183],0}, 
{&appr[316],1,&apak[183],0}, {&appr[317],1,&apak[183],0}, {&appr[318],2,&apak[183],0}, 
{&appr[320],2,&apak[183],0}, {&appr[322],1,&apak[183],0}, {&appr[323],1,&apak[183],0}, 
{&appr[324],1,&apak[183],0}, {&appr[325],1,&apak[183],0}, {&appr[326],1,&apak[183],0}, 
{&appr[327],1,&apak[183],0}, {&appr[328],1,&apak[183],0}, {&appr[329],1,&apak[183],0}, 
{&appr[330],1,&apak[183],0}, {&appr[331],1,&apak[183],0}, {&appr[332],1,&apak[183],0}, 
{&appr[333],2,&apak[183],0}, {&appr[335],3,&apak[183],0}, {&appr[338],2,&apak[183],0}, 
{&appr[340],2,&apak[183],0}, {&appr[342],1,&apak[183],0}, {&appr[343],1,&apak[183],0}, 
{&appr[344],1,&apak[183],0}, {&appr[345],1,&apak[183],0}, {&appr[346],1,&apak[183],0}, 
{&appr[347],1,&apak[183],0}, {&appr[348],1,&apak[183],0}, {&appr[349],1,&apak[183],0}, 
{&appr[350],1,&apak[183],0}, {&appr[351],1,&apak[183],0}, {&appr[352],1,&apak[183],0}, 
{&appr[353],1,&apak[183],0}, {&appr[354],1,&apak[183],0}, {&appr[355],1,&apak[183],0}, 
{&appr[356],1,&apak[183],0}, {&appr[357],1,&apak[183],0}, {&appr[358],1,&apak[183],0}, 
{&appr[359],1,&apak[183],0}, {&appr[360],1,&apak[183],0}, {&appr[361],1,&apak[183],0}, 
{&appr[362],2,&apak[183],2}, {&appr[364],3,&apak[185],3}, {&appr[367],2,&apak[188],2}, 
{&appr[369],1,&apak[190],0}, {&appr[370],3,&apak[190],4}, {&appr[373],1,&apak[194],0}, 
{&appr[374],1,&apak[194],0}, {&appr[375],1,&apak[194],0}, {&appr[376],14,&apak[194],10}, 
{&appr[390],1,&apak[204],0}, {&appr[391],1,&apak[204],0}, {&appr[392],6,&apak[204],2}, 
{&appr[398],6,&apak[206],2}, {&appr[404],2,&apak[208],1}, {&appr[406],1,&apak[209],0}, 
{&appr[407],2,&apak[209],2}, {&appr[409],7,&apak[211],2}, {&appr[416],1,&apak[213],0}, 
{&appr[417],1,&apak[213],0}, {&appr[418],1,&apak[213],0}, {&appr[419],2,&apak[213],2}, 
{&appr[421],2,&apak[215],2}, {&appr[423],1,&apak[217],0}, {&appr[424],1,&apak[217],0}, 
{&appr[425],1,&apak[217],0}, {&appr[426],1,&apak[217],0}, {&appr[427],3,&apak[217],0}, 
{&appr[430],1,&apak[217],0}, {&appr[431],1,&apak[217],0}, {&appr[432],1,&apak[217],0}, 
{&appr[433],1,&apak[217],0}, {&appr[434],1,&apak[217],0}, {&appr[435],1,&apak[217],0}, 
{&appr[436],1,&apak[217],0}, {&appr[437],1,&apak[217],0}, {&appr[438],1,&apak[217],0}, 
{&appr[439],1,&apak[217],0}, {&appr[440],1,&apak[217],0}, {&appr[441],2,&apak[217],2}, 
{&appr[443],2,&apak[219],2}, {&appr[445],6,&apak[221],2}, {&appr[451],2,&apak[223],1}, 
{&appr[453],7,&apak[224],3}, {&appr[460],1,&apak[227],0}, {&appr[461],1,&apak[227],0}, 
{&appr[462],1,&apak[227],0}, {&appr[463],1,&apak[227],0}, {&appr[464],1,&apak[227],0}, 
{&appr[465],1,&apak[227],0}, {&appr[466],1,&apak[227],0}, {&appr[467],1,&apak[227],0}, 
{&appr[468],1,&apak[227],0}, {&appr[469],1,&apak[227],0}, {&appr[470],1,&apak[227],0}, 
{&appr[471],1,&apak[227],0}, {&appr[472],1,&apak[227],0}, {&appr[473],1,&apak[227],0}, 
{&appr[474],1,&apak[227],0}, {&appr[475],1,&apak[227],0}, {&appr[476],1,&apak[227],0}, 
{&appr[477],1,&apak[227],0}, {&appr[478],1,&apak[227],0}, {&appr[479],1,&apak[227],0}, 
{&appr[480],1,&apak[227],0}, {&appr[481],1,&apak[227],0}, {&appr[482],5,&apak[227],3}, 
{&appr[487],1,&apak[230],0}, {&appr[488],5,&apak[230],4}, {&appr[493],5,&apak[234],4}, 
{&appr[498],1,&apak[238],0}, {&appr[499],7,&apak[238],3}, {&appr[506],2,&apak[241],1}, 
{&appr[508],2,&apak[242],1}, {&appr[510],2,&apak[243],1}, {&appr[512],2,&apak[244],1}, 
{&appr[514],2,&apak[245],1}, {&appr[516],1,&apak[246],0}, {&appr[517],1,&apak[246],0}, 
{&appr[518],1,&apak[246],0}, {&appr[519],1,&apak[246],0}, {&appr[520],1,&apak[246],0}, 
{&appr[521],1,&apak[246],0}, {&appr[522],1,&apak[246],0}, {&appr[523],1,&apak[246],0}, 
{&appr[524],1,&apak[246],0}, {&appr[525],1,&apak[246],0}, {&appr[526],1,&apak[246],0}, 
{&appr[527],1,&apak[246],0}, {&appr[528],1,&apak[246],0}, {&appr[529],1,&apak[246],0}, 
{&appr[530],1,&apak[246],0}, {&appr[531],1,&apak[246],0}, {&appr[532],1,&apak[246],0}, 
{&appr[533],1,&apak[246],0}, {&appr[534],1,&apak[246],0}, {&appr[535],1,&apak[246],0}, 
{&appr[536],5,&apak[246],3}, {&appr[541],6,&apak[249],2}, {&appr[547],6,&apak[251],2}, 
{&appr[553],5,&apak[253],5}, {&appr[558],6,&apak[258],1}, {&appr[564],7,&apak[259],4}, 
{&appr[571],6,&apak[263],2}, {&appr[577],1,&apak[265],0}, {&appr[578],1,&apak[265],0}, 
{&appr[579],1,&apak[265],0}, {&appr[580],1,&apak[265],0}, {&appr[581],1,&apak[265],0}, 
{&appr[582],1,&apak[265],0}, {&appr[583],1,&apak[265],0}, {&appr[584],1,&apak[265],0}, 
{&appr[585],1,&apak[265],0}, {&appr[586],1,&apak[265],0}, {&appr[587],1,&apak[265],0}, 
{&appr[588],1,&apak[265],0}, {&appr[589],3,&apak[265],0}, {&appr[592],1,&apak[265],0}, 
{&appr[593],1,&apak[265],0}, {&appr[594],1,&apak[265],0}, {&appr[595],1,&apak[265],0}, 
{&appr[596],1,&apak[265],0}, {&appr[597],1,&apak[265],0}, {&appr[598],1,&apak[265],0}, 
{&appr[599],1,&apak[265],0}, {&appr[600],2,&apak[265],2}, {&appr[602],2,&apak[267],2}, 
{&appr[604],1,&apak[269],0}, {&appr[605],5,&apak[269],5}, {&appr[610],1,&apak[274],0}, 
{&appr[611],1,&apak[274],0}, {&appr[612],7,&apak[274],4}, {&appr[619],2,&apak[278],1}, 
{&appr[621],1,&apak[279],0}, {&appr[622],1,&apak[279],0}, {&appr[623],1,&apak[279],0}, 
{&appr[624],1,&apak[279],0}, {&appr[625],1,&apak[279],0}, {&appr[626],1,&apak[279],0}, 
{&appr[627],1,&apak[279],0}, {&appr[628],1,&apak[279],0}, {&appr[629],2,&apak[279],2}, 
{&appr[631],6,&apak[281],1}, {&appr[637],1,&apak[282],0}, {&appr[638],1,&apak[282],0}, 
{&appr[639],6,&apak[282],1}, {&appr[645],1,&apak[283],0}, {&appr[646],1,&apak[283],0}, 
{&appr[647],1,&apak[283],0}, {&appr[0],0,&apak[0],0}};




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



