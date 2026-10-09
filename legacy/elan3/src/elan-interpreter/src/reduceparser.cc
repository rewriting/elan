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


#define STTAB   reducesttab
#define PARSER  reducesyntan
#define SEMACT  reducesemact
#define RESWTAB reducerwt
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
{RWEndDef,shift,{6}}, {RWGrammarForSort,shift,{4}}, {EOFINPUT,accept,{0}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {RWEndDef,shift,{6}}, 
{RWGrammarForSort,shift,{4}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{DEFAULT,reduction,{708,37,1}}, {RWend,shift,{14}}, {RWdc,shift,{35}}, 
{RWiterate,shift,{32}}, {RWDK,shift,{37}}, {RWDC,shift,{38}}, 
{RWone,shift,{36}}, {RWMETA,shift,{18}}, {RWrepeat,shift,{33}}, 
{RWfail,shift,{19}}, {RWONE,shift,{39}}, {RWdkcall,shift,{24}}, 
{RWcall,shift,{21}}, {RWdccall,shift,{25}}, {RWdk,shift,{34}}, 
{RWid,shift,{20}}, {DEFAULT,reduction,{113,58,1}}, {NUMBER,shift,{40}}, 
{DEFAULT,reduction,{7,50,1}}, {RWEndDef,shift,{6}}, {RWGrammarForSort,shift,{4}}, 
{-':',shift,{42}}, {DEFAULT,reduction,{16,39,1}}, {DEFAULT,reduction,{719,44,2}}, 
{RWIDENT,shift,{47}}, {RWFSYM,shift,{45}}, {RWEVAR,shift,{49}}, 
{RWSTRING,shift,{46}}, {RWINT,shift,{48}}, {RWVAR,shift,{50}}, 
{RWend,shift,{52}}, {-';',shift,{51}}, {DEFAULT,reduction,{160,88,1}}, 
{DEFAULT,reduction,{197,87,1}}, {DEFAULT,reduction,{195,87,1}}, {DEFAULT,reduction,{196,87,1}}, 
{-'(',shift,{53}}, {-'(',shift,{54}}, {-'(',shift,{55}}, 
{-'(',shift,{56}}, {-'(',shift,{57}}, {-'(',shift,{58}}, 
{-'(',shift,{59}}, {-'(',shift,{60}}, {-'(',shift,{61}}, 
{-'(',shift,{62}}, {-'(',shift,{63}}, {-'*',shift,{64}}, 
{-'*',shift,{65}}, {DEFAULT,reduction,{6466,90,1}}, {DEFAULT,reduction,{6465,89,1}}, 
{DEFAULT,reduction,{6464,82,1}}, {DEFAULT,reduction,{163,85,1}}, {DEFAULT,reduction,{163,84,1}}, 
{DEFAULT,reduction,{163,81,1}}, {DEFAULT,reduction,{8,50,2}}, {RWRULE,shift,{68}}, 
{RWSWRULE,shift,{69}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{DEFAULT,reduction,{6471,98,4}}, {RWend,shift,{71}}, {-'(',shift,{72}}, 
{-'(',shift,{73}}, {-'(',shift,{74}}, {-'(',shift,{75}}, 
{-'(',shift,{76}}, {-'(',shift,{77}}, {RWdc,shift,{35}}, 
{RWiterate,shift,{32}}, {RWDK,shift,{37}}, {RWDC,shift,{38}}, 
{RWone,shift,{36}}, {RWMETA,shift,{18}}, {RWrepeat,shift,{33}}, 
{RWfail,shift,{19}}, {RWONE,shift,{39}}, {RWdkcall,shift,{24}}, 
{RWcall,shift,{21}}, {RWdccall,shift,{25}}, {RWdk,shift,{34}}, 
{RWid,shift,{20}}, {DEFAULT,reduction,{112,99,2}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {RWdc,shift,{35}}, {RWiterate,shift,{32}}, 
{RWDK,shift,{37}}, {RWDC,shift,{38}}, {RWone,shift,{36}}, 
{RWMETA,shift,{18}}, {RWrepeat,shift,{33}}, {RWfail,shift,{19}}, 
{RWONE,shift,{39}}, {RWdkcall,shift,{24}}, {RWcall,shift,{21}}, 
{RWdccall,shift,{25}}, {RWdk,shift,{34}}, {RWid,shift,{20}}, 
{RWdc,shift,{35}}, {RWiterate,shift,{32}}, {RWDK,shift,{37}}, 
{RWDC,shift,{38}}, {RWone,shift,{36}}, {RWMETA,shift,{18}}, 
{RWrepeat,shift,{33}}, {RWfail,shift,{19}}, {RWONE,shift,{39}}, 
{RWdkcall,shift,{24}}, {RWcall,shift,{21}}, {RWdccall,shift,{25}}, 
{RWdk,shift,{34}}, {RWid,shift,{20}}, {RWnil,shift,{85}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {RWnil,shift,{85}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {RWnil,shift,{90}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {RWnil,shift,{90}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {RWnil,shift,{90}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {RWdc,shift,{35}}, 
{RWiterate,shift,{32}}, {RWDK,shift,{37}}, {RWDC,shift,{38}}, 
{RWone,shift,{36}}, {RWMETA,shift,{18}}, {RWrepeat,shift,{33}}, 
{RWfail,shift,{19}}, {RWONE,shift,{39}}, {RWdkcall,shift,{24}}, 
{RWcall,shift,{21}}, {RWdccall,shift,{25}}, {RWdk,shift,{34}}, 
{RWid,shift,{20}}, {RWdc,shift,{35}}, {RWiterate,shift,{32}}, 
{RWDK,shift,{37}}, {RWDC,shift,{38}}, {RWone,shift,{36}}, 
{RWMETA,shift,{18}}, {RWrepeat,shift,{33}}, {RWfail,shift,{19}}, 
{RWONE,shift,{39}}, {RWdkcall,shift,{24}}, {RWcall,shift,{21}}, 
{RWdccall,shift,{25}}, {RWdk,shift,{34}}, {RWid,shift,{20}}, 
{RWdc,shift,{35}}, {RWiterate,shift,{32}}, {RWDK,shift,{37}}, 
{RWDC,shift,{38}}, {RWone,shift,{36}}, {RWMETA,shift,{18}}, 
{RWrepeat,shift,{33}}, {RWfail,shift,{19}}, {RWONE,shift,{39}}, 
{RWdkcall,shift,{24}}, {RWcall,shift,{21}}, {RWdccall,shift,{25}}, 
{RWdk,shift,{34}}, {RWid,shift,{20}}, {DEFAULT,reduction,{163,92,2}}, 
{DEFAULT,reduction,{163,91,2}}, {RWEndDef,shift,{6}}, {RWRULE,shift,{68}}, 
{RWSWRULE,shift,{69}}, {DEFAULT,reduction,{718,22,1}}, {-'(',shift,{101}}, 
{-'(',shift,{102}}, {-':',shift,{103}}, {DEFAULT,reduction,{111,100,2}}, 
{RWIDENT,shift,{47}}, {RWnil,shift,{106}}, {RWFSYM,shift,{45}}, 
{RWEVAR,shift,{49}}, {RWSTRING,shift,{46}}, {RWINT,shift,{48}}, 
{RWVAR,shift,{50}}, {RWnil,shift,{85}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{DEFAULT,reduction,{160,88,3}}, {-')',shift,{113}}, {DEFAULT,reduction,{182,77,1}}, 
{-';',shift,{51}}, {-')',shift,{114}}, {-';',shift,{51}}, 
{-')',shift,{115}}, {-',',shift,{116}}, {-'.',shift,{117}}, 
{DEFAULT,reduction,{710,41,1}}, {DEFAULT,reduction,{32,42,1}}, {-',',shift,{118}}, 
{-')',shift,{119}}, {-'.',shift,{120}}, {DEFAULT,reduction,{190,75,1}}, 
{DEFAULT,reduction,{192,52,1}}, {DEFAULT,reduction,{37,51,1}}, {-')',shift,{121}}, 
{-')',shift,{122}}, {-',',shift,{124}}, {-')',shift,{123}}, 
{-';',shift,{51}}, {-',',reduction,{177,86,1}}, {-')',reduction,{177,86,1}}, 
{-',',shift,{124}}, {-')',shift,{125}}, {-',',shift,{124}}, 
{-')',shift,{126}}, {DEFAULT,reduction,{717,22,2}}, {RWEndDef,shift,{6}}, 
{RWSTRATEGY,shift,{129}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {RWnil,shift,{134}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {-',',shift,{137}}, 
{-'.',shift,{138}}, {DEFAULT,reduction,{713,13,1}}, {-')',shift,{139}}, 
{-')',shift,{140}}, {-')',shift,{141}}, {-',',shift,{142}}, 
{DEFAULT,reduction,{18,34,1}}, {-',',shift,{143}}, {DEFAULT,reduction,{159,87,4}}, 
{DEFAULT,reduction,{165,87,4}}, {DEFAULT,reduction,{164,87,4}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {RWnil,shift,{85}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{DEFAULT,reduction,{174,87,4}}, {RWnil,shift,{90}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {DEFAULT,reduction,{173,87,4}}, {DEFAULT,reduction,{172,87,4}}, 
{DEFAULT,reduction,{176,87,4}}, {RWdc,shift,{35}}, {RWiterate,shift,{32}}, 
{RWDK,shift,{37}}, {RWDC,shift,{38}}, {RWone,shift,{36}}, 
{RWMETA,shift,{18}}, {RWrepeat,shift,{33}}, {RWfail,shift,{19}}, 
{RWONE,shift,{39}}, {RWdkcall,shift,{24}}, {RWcall,shift,{21}}, 
{RWdccall,shift,{25}}, {RWdk,shift,{34}}, {RWid,shift,{20}}, 
{DEFAULT,reduction,{175,87,4}}, {DEFAULT,reduction,{171,87,4}}, {RWQUERY,shift,{151}}, 
{DEFAULT,reduction,{6468,27,1}}, {-'(',shift,{152}}, {-',',shift,{153}}, 
{-',',shift,{154}}, {RWend,shift,{155}}, {-'.',shift,{156}}, 
{DEFAULT,reduction,{704,33,1}}, {-':',shift,{157}}, {DEFAULT,reduction,{17,48,1}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {RWIDENT,shift,{47}}, 
{RWnil,shift,{106}}, {RWFSYM,shift,{45}}, {RWEVAR,shift,{49}}, 
{RWSTRING,shift,{46}}, {RWINT,shift,{48}}, {RWVAR,shift,{50}}, 
{DEFAULT,reduction,{23,12,4}}, {DEFAULT,reduction,{22,12,4}}, {DEFAULT,reduction,{21,12,4}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {-',',shift,{164}}, {DEFAULT,reduction,{39,71,1}}, 
{DEFAULT,reduction,{711,41,3}}, {-',',shift,{165}}, {DEFAULT,reduction,{191,75,3}}, 
{-';',shift,{51}}, {-',',reduction,{178,86,3}}, {-')',reduction,{178,86,3}}, 
{DEFAULT,reduction,{6470,79,7}}, {-'(',shift,{166}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {RWEndDef,shift,{6}}, 
{RWGrammarForSort,shift,{4}}, {RWnil,shift,{134}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{-')',shift,{176}}, {DEFAULT,reduction,{25,63,1}}, {DEFAULT,reduction,{714,13,3}}, 
{-')',shift,{177}}, {DEFAULT,reduction,{19,35,1}}, {-')',shift,{178}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{-',',shift,{184}}, {DEFAULT,reduction,{179,76,1}}, {-',',shift,{185}}, 
{DEFAULT,reduction,{712,28,1}}, {-',',shift,{186}}, {DEFAULT,reduction,{709,37,8}}, 
{DEFAULT,reduction,{705,33,3}}, {-':',shift,{187}}, {DEFAULT,reduction,{14,46,1}}, 
{DEFAULT,reduction,{24,12,6}}, {DEFAULT,reduction,{20,12,6}}, {DEFAULT,reduction,{20,12,6}}, 
{-')',shift,{188}}, {DEFAULT,reduction,{180,69,1}}, {-')',shift,{189}}, 
{-',',shift,{190}}, {DEFAULT,reduction,{140,56,1}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {DEFAULT,reduction,{184,87,8}}, {DEFAULT,reduction,{183,87,8}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {-',',shift,{199}}, 
{-',',shift,{200}}, {DEFAULT,reduction,{38,53,1}}, {-',',shift,{201}}, 
{-':',shift,{202}}, {DEFAULT,reduction,{13,45,1}}, {-',',shift,{203}}, 
{DEFAULT,reduction,{141,57,1}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {-',',shift,{211}}, 
{-',',shift,{212}}, {-',',shift,{213}}, {-':',shift,{214}}, 
{DEFAULT,reduction,{9,31,1}}, {-',',shift,{215}}, {DEFAULT,reduction,{142,59,1}}, 
{RWdc,shift,{35}}, {RWiterate,shift,{32}}, {RWDK,shift,{37}}, 
{RWDC,shift,{38}}, {RWone,shift,{36}}, {RWMETA,shift,{18}}, 
{RWrepeat,shift,{33}}, {RWfail,shift,{19}}, {RWONE,shift,{39}}, 
{RWdkcall,shift,{24}}, {RWcall,shift,{21}}, {RWdccall,shift,{25}}, 
{RWdk,shift,{34}}, {RWid,shift,{20}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {RWIDENT,shift,{47}}, 
{RWFSYM,shift,{45}}, {RWEVAR,shift,{49}}, {RWSTRING,shift,{46}}, 
{RWINT,shift,{48}}, {RWVAR,shift,{50}}, {-';',shift,{51}}, 
{-')',shift,{224}}, {-',',shift,{225}}, {DEFAULT,reduction,{36,80,1}}, 
{-',',shift,{226}}, {-':',shift,{227}}, {DEFAULT,reduction,{15,47,1}}, 
{-',',shift,{228}}, {DEFAULT,reduction,{143,60,1}}, {RWend,shift,{230}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{RWIDENT,shift,{47}}, {RWFSYM,shift,{45}}, {RWEVAR,shift,{49}}, 
{RWSTRING,shift,{46}}, {RWINT,shift,{48}}, {RWVAR,shift,{50}}, 
{RWEndDef,shift,{6}}, {RWSTRATEGY,shift,{129}}, {DEFAULT,reduction,{181,70,1}}, 
{-',',shift,{239}}, {DEFAULT,reduction,{33,83,1}}, {-',',shift,{240}}, 
{-':',shift,{241}}, {DEFAULT,reduction,{10,72,1}}, {-')',shift,{242}}, 
{DEFAULT,reduction,{144,61,1}}, {DEFAULT,reduction,{6467,27,12}}, {RWIDENT,shift,{47}}, 
{RWFSYM,shift,{45}}, {RWEVAR,shift,{49}}, {RWSTRING,shift,{46}}, 
{RWINT,shift,{48}}, {RWVAR,shift,{50}}, {RWIDENT,shift,{47}}, 
{RWFSYM,shift,{45}}, {RWEVAR,shift,{49}}, {RWSTRING,shift,{46}}, 
{RWINT,shift,{48}}, {RWVAR,shift,{50}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {RWend,shift,{247}}, {-',',shift,{248}}, 
{DEFAULT,reduction,{34,66,1}}, {-',',shift,{249}}, {-':',shift,{250}}, 
{DEFAULT,reduction,{6469,62,13}}, {RWIDENT,shift,{47}}, {RWFSYM,shift,{45}}, 
{RWEVAR,shift,{49}}, {RWSTRING,shift,{46}}, {RWINT,shift,{48}}, 
{RWVAR,shift,{50}}, {RWSWITCH,shift,{254}}, {RWNOSWITCH,shift,{255}}, 
{RWChar,shift,{264}}, {RWnil,shift,{258}}, {RWNum,shift,{263}}, 
{RWIdent,shift,{261}}, {RWBlank,shift,{260}}, {RWString,shift,{262}}, 
{RWType,shift,{265}}, {-',',shift,{266}}, {DEFAULT,reduction,{41,67,1}}, 
{-')',shift,{267}}, {-'(',shift,{268}}, {-'(',shift,{269}}, 
{-':',shift,{270}}, {-'.',shift,{271}}, {DEFAULT,reduction,{706,30,1}}, 
{DEFAULT,reduction,{40,40,1}}, {DEFAULT,reduction,{6,29,1}}, {-'(',shift,{272}}, 
{-'(',shift,{273}}, {-'(',shift,{274}}, {-'(',shift,{275}}, 
{-'(',shift,{276}}, {RWWHERE,shift,{281}}, {RWnil,shift,{279}}, 
{RWPWHERE,shift,{280}}, {RWTRY,shift,{284}}, {RWIFF,shift,{283}}, 
{RWend,shift,{285}}, {RWWHERE,shift,{281}}, {RWnil,shift,{279}}, 
{RWPWHERE,shift,{280}}, {RWTRY,shift,{284}}, {RWIFF,shift,{283}}, 
{RWWHERE,shift,{281}}, {RWnil,shift,{279}}, {RWPWHERE,shift,{280}}, 
{RWTRY,shift,{284}}, {RWIFF,shift,{283}}, {RWnil,shift,{289}}, 
{RWChar,shift,{264}}, {RWnil,shift,{258}}, {RWNum,shift,{263}}, 
{RWIdent,shift,{261}}, {RWBlank,shift,{260}}, {RWString,shift,{262}}, 
{RWType,shift,{265}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{NUMBER,shift,{10}}, {-'-',shift,{9}}, {-')',shift,{296}}, 
{-'.',shift,{297}}, {DEFAULT,reduction,{715,15,1}}, {-'(',shift,{298}}, 
{-'(',shift,{299}}, {-'(',shift,{300}}, {-'(',shift,{301}}, 
{DEFAULT,reduction,{126,23,1}}, {DEFAULT,reduction,{136,20,19}}, {-',',shift,{302}}, 
{DEFAULT,reduction,{137,16,1}}, {-',',shift,{303}}, {DEFAULT,reduction,{11,32,17}}, 
{DEFAULT,reduction,{707,30,3}}, {-')',shift,{304}}, {-')',shift,{305}}, 
{-')',shift,{306}}, {-')',shift,{307}}, {-')',shift,{308}}, 
{RWend,shift,{309}}, {RWWHERE,shift,{281}}, {RWnil,shift,{279}}, 
{RWPWHERE,shift,{280}}, {RWTRY,shift,{284}}, {RWIFF,shift,{283}}, 
{RWIDENT,shift,{47}}, {RWFSYM,shift,{45}}, {RWEVAR,shift,{49}}, 
{RWSTRING,shift,{46}}, {RWINT,shift,{48}}, {RWVAR,shift,{50}}, 
{RWIDENT,shift,{47}}, {RWFSYM,shift,{45}}, {RWEVAR,shift,{49}}, 
{RWSTRING,shift,{46}}, {RWINT,shift,{48}}, {RWVAR,shift,{50}}, 
{RWWHERE,shift,{281}}, {RWnil,shift,{316}}, {RWPWHERE,shift,{280}}, 
{RWTRY,shift,{284}}, {RWIFF,shift,{283}}, {RWIDENT,shift,{47}}, 
{RWFSYM,shift,{45}}, {RWEVAR,shift,{49}}, {RWSTRING,shift,{46}}, 
{RWINT,shift,{48}}, {RWVAR,shift,{50}}, {RWIDENT,shift,{47}}, 
{RWnil,shift,{320}}, {RWFSYM,shift,{45}}, {RWEVAR,shift,{49}}, 
{RWSTRING,shift,{46}}, {RWINT,shift,{48}}, {RWVAR,shift,{50}}, 
{RWIDENT,shift,{47}}, {RWFSYM,shift,{45}}, {RWEVAR,shift,{49}}, 
{RWSTRING,shift,{46}}, {RWINT,shift,{48}}, {RWVAR,shift,{50}}, 
{DEFAULT,reduction,{5,29,4}}, {DEFAULT,reduction,{4,29,4}}, {DEFAULT,reduction,{3,29,4}}, 
{DEFAULT,reduction,{2,29,4}}, {DEFAULT,reduction,{1,29,4}}, {DEFAULT,reduction,{133,20,21}}, 
{DEFAULT,reduction,{716,15,3}}, {-',',shift,{325}}, {DEFAULT,reduction,{28,54,1}}, 
{-',',shift,{326}}, {-')',shift,{327}}, {-'.',shift,{328}}, 
{-'.',reduction,{715,15,1}}, {-',',reduction,{715,15,1}}, {-')',reduction,{127,18,1}}, 
{-')',shift,{329}}, {-')',shift,{330}}, {-'.',shift,{331}}, 
{DEFAULT,reduction,{138,95,1}}, {-',',shift,{332}}, {DEFAULT,reduction,{134,96,1}}, 
{-')',shift,{333}}, {DEFAULT,reduction,{35,68,1}}, {NUMBER,shift,{10}}, 
{-'-',shift,{9}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{DEFAULT,reduction,{129,14,4}}, {RWWHERE,shift,{281}}, {RWnil,shift,{316}}, 
{RWPWHERE,shift,{280}}, {RWTRY,shift,{284}}, {RWIFF,shift,{283}}, 
{DEFAULT,reduction,{30,14,4}}, {DEFAULT,reduction,{131,94,6}}, {RWIDENT,shift,{47}}, 
{RWnil,shift,{320}}, {RWFSYM,shift,{45}}, {RWEVAR,shift,{49}}, 
{RWSTRING,shift,{46}}, {RWINT,shift,{48}}, {RWVAR,shift,{50}}, 
{RWSWITCH,shift,{254}}, {RWNOSWITCH,shift,{255}}, {DEFAULT,reduction,{130,94,6}}, 
{-',',shift,{341}}, {DEFAULT,reduction,{46,17,1}}, {-',',shift,{342}}, 
{DEFAULT,reduction,{29,55,1}}, {DEFAULT,reduction,{128,18,3}}, {DEFAULT,reduction,{139,95,3}}, 
{DEFAULT,reduction,{135,97,3}}, {NUMBER,shift,{10}}, {-'-',shift,{9}}, 
{RWIDENT,shift,{47}}, {RWFSYM,shift,{45}}, {RWEVAR,shift,{49}}, 
{RWSTRING,shift,{46}}, {RWINT,shift,{48}}, {RWVAR,shift,{50}}, 
{-',',shift,{345}}, {-')',shift,{346}}, {RWIDENT,shift,{47}}, 
{RWFSYM,shift,{45}}, {RWEVAR,shift,{49}}, {RWSTRING,shift,{46}}, 
{RWINT,shift,{48}}, {RWVAR,shift,{50}}, {DEFAULT,reduction,{31,14,8}}, 
{-')',shift,{348}}, {DEFAULT,reduction,{45,14,10}}, {0,shift,{0}}};

 static struct sak {
     int sym,tostate;
 } apak[]={
{37,3}, {44,5}, {79,2}, {98,1}, {50,8}, {58,7}, {37,11}, {44,5}, 
{39,12}, {50,13}, {81,31}, {82,28}, {84,30}, {85,29}, {87,17}, {88,16}, 
{89,27}, {90,26}, {91,23}, {92,22}, {99,15}, {37,41}, {44,5}, {12,44}, 
{100,43}, {20,67}, {22,66}, {50,70}, {81,31}, {82,28}, {84,30}, {85,29}, 
{87,78}, {89,27}, {90,26}, {91,23}, {92,22}, {50,80}, {77,79}, {81,31}, 
{82,28}, {84,30}, {85,29}, {87,17}, {88,81}, {89,27}, {90,26}, {91,23}, 
{92,22}, {81,31}, {82,28}, {84,30}, {85,29}, {87,17}, {88,82}, {89,27}, 
{90,26}, {91,23}, {92,22}, {41,83}, {42,84}, {50,86}, {41,87}, {42,84}, 
{50,86}, {50,92}, {51,91}, {52,89}, {75,88}, {50,92}, {51,91}, {52,89}, 
{75,93}, {50,92}, {51,91}, {52,89}, {75,94}, {81,31}, {82,28}, {84,30}, 
{85,29}, {86,95}, {87,17}, {88,96}, {89,27}, {90,26}, {91,23}, {92,22}, 
{81,31}, {82,28}, {84,30}, {85,29}, {86,97}, {87,17}, {88,96}, {89,27}, 
{90,26}, {91,23}, {92,22}, {81,31}, {82,28}, {84,30}, {85,29}, {86,98}, 
{87,17}, {88,96}, {89,27}, {90,26}, {91,23}, {92,22}, {20,99}, {44,100}, 
{12,105}, {13,104}, {41,107}, {42,84}, {50,86}, {50,108}, {50,109}, {34,110}, 
{50,111}, {34,112}, {50,111}, {27,127}, {44,128}, {50,92}, {51,130}, {50,92}, 
{51,131}, {32,133}, {33,132}, {48,135}, {50,136}, {50,145}, {71,144}, {41,146}, 
{42,84}, {50,86}, {50,145}, {71,147}, {50,92}, {51,91}, {52,89}, {75,148}, 
{81,31}, {82,28}, {84,30}, {85,29}, {87,17}, {88,149}, {89,27}, {90,26}, 
{91,23}, {92,22}, {62,150}, {50,159}, {63,158}, {12,105}, {13,160}, {35,161}, 
{50,162}, {35,163}, {50,162}, {50,168}, {76,167}, {28,169}, {50,170}, {28,171}, 
{50,170}, {37,172}, {44,5}, {32,133}, {33,173}, {48,135}, {50,136}, {46,174}, 
{50,175}, {50,180}, {69,179}, {50,180}, {69,181}, {50,183}, {56,182}, {50,180}, 
{69,191}, {50,193}, {53,192}, {50,193}, {53,194}, {45,195}, {50,196}, {50,198}, 
{57,197}, {50,193}, {53,204}, {50,145}, {71,205}, {50,145}, {71,206}, {31,207}, 
{50,208}, {50,210}, {59,209}, {81,31}, {82,28}, {84,30}, {85,29}, {87,17}, 
{88,216}, {89,27}, {90,26}, {91,23}, {92,22}, {50,218}, {80,217}, {50,218}, 
{80,219}, {47,220}, {50,221}, {12,223}, {60,222}, {70,229}, {50,232}, {83,231}, 
{50,232}, {83,233}, {50,235}, {72,234}, {12,237}, {61,236}, {27,238}, {44,128}, 
{12,244}, {66,243}, {12,244}, {66,245}, {50,246}, {12,252}, {67,251}, {94,253}, 
{29,259}, {30,256}, {40,257}, {14,278}, {15,277}, {23,282}, {14,278}, {15,287}, 
{16,286}, {23,282}, {14,278}, {15,287}, {16,288}, {23,282}, {29,259}, {30,290}, 
{40,257}, {50,291}, {50,292}, {50,293}, {50,294}, {50,295}, {14,278}, {15,310}, 
{23,282}, {12,312}, {54,311}, {12,312}, {54,313}, {14,278}, {15,287}, {16,315}, 
{18,314}, {23,282}, {12,317}, {12,322}, {95,318}, {96,321}, {97,319}, {12,324}, 
{68,323}, {17,334}, {50,335}, {50,337}, {55,336}, {14,278}, {15,287}, {16,315}, 
{18,338}, {23,282}, {12,322}, {95,339}, {96,321}, {97,319}, {94,340}, {50,337}, 
{55,343}, {12,344}, {12,347}, {0,0}};

 struct sst {
	struct spr *pr;
	int npr;
	struct sak *ak;
	int nak;
 } STTAB[]={
{&appr[0],2,&apak[0],4}, {&appr[2],1,&apak[4],0}, {&appr[3],2,&apak[4],2}, 
{&appr[5],2,&apak[6],2}, {&appr[7],2,&apak[8],2}, {&appr[9],1,&apak[10],0}, 
{&appr[10],1,&apak[10],0}, {&appr[11],14,&apak[10],11}, {&appr[25],1,&apak[21],0}, 
{&appr[26],1,&apak[21],0}, {&appr[27],1,&apak[21],0}, {&appr[28],2,&apak[21],2}, 
{&appr[30],1,&apak[23],0}, {&appr[31],1,&apak[23],0}, {&appr[32],1,&apak[23],0}, 
{&appr[33],6,&apak[23],2}, {&appr[39],2,&apak[25],0}, {&appr[41],1,&apak[25],0}, 
{&appr[42],1,&apak[25],0}, {&appr[43],1,&apak[25],0}, {&appr[44],1,&apak[25],0}, 
{&appr[45],1,&apak[25],0}, {&appr[46],1,&apak[25],0}, {&appr[47],1,&apak[25],0}, 
{&appr[48],1,&apak[25],0}, {&appr[49],1,&apak[25],0}, {&appr[50],1,&apak[25],0}, 
{&appr[51],1,&apak[25],0}, {&appr[52],1,&apak[25],0}, {&appr[53],1,&apak[25],0}, 
{&appr[54],1,&apak[25],0}, {&appr[55],1,&apak[25],0}, {&appr[56],1,&apak[25],0}, 
{&appr[57],1,&apak[25],0}, {&appr[58],1,&apak[25],0}, {&appr[59],1,&apak[25],0}, 
{&appr[60],1,&apak[25],0}, {&appr[61],1,&apak[25],0}, {&appr[62],1,&apak[25],0}, 
{&appr[63],1,&apak[25],0}, {&appr[64],1,&apak[25],0}, {&appr[65],2,&apak[25],2}, 
{&appr[67],2,&apak[27],1}, {&appr[69],1,&apak[28],0}, {&appr[70],1,&apak[28],0}, 
{&appr[71],1,&apak[28],0}, {&appr[72],1,&apak[28],0}, {&appr[73],1,&apak[28],0}, 
{&appr[74],1,&apak[28],0}, {&appr[75],1,&apak[28],0}, {&appr[76],1,&apak[28],0}, 
{&appr[77],14,&apak[28],9}, {&appr[91],1,&apak[37],0}, {&appr[92],2,&apak[37],2}, 
{&appr[94],14,&apak[39],10}, {&appr[108],14,&apak[49],10}, {&appr[122],3,&apak[59],3}, 
{&appr[125],3,&apak[62],3}, {&appr[128],3,&apak[65],4}, {&appr[131],3,&apak[69],4}, 
{&appr[134],3,&apak[73],4}, {&appr[137],14,&apak[77],11}, {&appr[151],14,&apak[88],11}, 
{&appr[165],14,&apak[99],11}, {&appr[179],1,&apak[110],0}, {&appr[180],1,&apak[110],0}, 
{&appr[181],3,&apak[110],2}, {&appr[184],1,&apak[112],0}, {&appr[185],1,&apak[112],0}, 
{&appr[186],1,&apak[112],0}, {&appr[187],1,&apak[112],0}, {&appr[188],1,&apak[112],0}, 
{&appr[189],7,&apak[112],2}, {&appr[196],3,&apak[114],3}, {&appr[199],2,&apak[117],1}, 
{&appr[201],2,&apak[118],1}, {&appr[203],2,&apak[119],2}, {&appr[205],2,&apak[121],2}, 
{&appr[207],1,&apak[123],0}, {&appr[208],1,&apak[123],0}, {&appr[209],1,&apak[123],0}, 
{&appr[210],2,&apak[123],0}, {&appr[212],2,&apak[123],0}, {&appr[214],1,&apak[123],0}, 
{&appr[215],1,&apak[123],0}, {&appr[216],1,&apak[123],0}, {&appr[217],1,&apak[123],0}, 
{&appr[218],1,&apak[123],0}, {&appr[219],1,&apak[123],0}, {&appr[220],1,&apak[123],0}, 
{&appr[221],1,&apak[123],0}, {&appr[222],1,&apak[123],0}, {&appr[223],1,&apak[123],0}, 
{&appr[224],1,&apak[123],0}, {&appr[225],1,&apak[123],0}, {&appr[226],2,&apak[123],0}, 
{&appr[228],3,&apak[123],0}, {&appr[231],2,&apak[123],0}, {&appr[233],2,&apak[123],0}, 
{&appr[235],1,&apak[123],0}, {&appr[236],2,&apak[123],2}, {&appr[238],2,&apak[125],2}, 
{&appr[240],2,&apak[127],2}, {&appr[242],3,&apak[129],4}, {&appr[245],1,&apak[133],0}, 
{&appr[246],1,&apak[133],0}, {&appr[247],1,&apak[133],0}, {&appr[248],1,&apak[133],0}, 
{&appr[249],1,&apak[133],0}, {&appr[250],1,&apak[133],0}, {&appr[251],1,&apak[133],0}, 
{&appr[252],1,&apak[133],0}, {&appr[253],1,&apak[133],0}, {&appr[254],1,&apak[133],0}, 
{&appr[255],1,&apak[133],0}, {&appr[256],1,&apak[133],0}, {&appr[257],2,&apak[133],2}, 
{&appr[259],3,&apak[135],3}, {&appr[262],2,&apak[138],2}, {&appr[264],1,&apak[140],0}, 
{&appr[265],3,&apak[140],4}, {&appr[268],1,&apak[144],0}, {&appr[269],1,&apak[144],0}, 
{&appr[270],1,&apak[144],0}, {&appr[271],14,&apak[144],10}, {&appr[285],1,&apak[154],0}, 
{&appr[286],1,&apak[154],0}, {&appr[287],1,&apak[154],1}, {&appr[288],1,&apak[155],0}, 
{&appr[289],1,&apak[155],0}, {&appr[290],1,&apak[155],0}, {&appr[291],1,&apak[155],0}, 
{&appr[292],1,&apak[155],0}, {&appr[293],1,&apak[155],0}, {&appr[294],1,&apak[155],0}, 
{&appr[295],1,&apak[155],0}, {&appr[296],1,&apak[155],0}, {&appr[297],2,&apak[155],2}, 
{&appr[299],7,&apak[157],2}, {&appr[306],1,&apak[159],0}, {&appr[307],1,&apak[159],0}, 
{&appr[308],1,&apak[159],0}, {&appr[309],2,&apak[159],2}, {&appr[311],2,&apak[161],2}, 
{&appr[313],1,&apak[163],0}, {&appr[314],1,&apak[163],0}, {&appr[315],1,&apak[163],0}, 
{&appr[316],1,&apak[163],0}, {&appr[317],1,&apak[163],0}, {&appr[318],3,&apak[163],0}, 
{&appr[321],1,&apak[163],0}, {&appr[322],1,&apak[163],0}, {&appr[323],2,&apak[163],2}, 
{&appr[325],2,&apak[165],2}, {&appr[327],2,&apak[167],2}, {&appr[329],2,&apak[169],2}, 
{&appr[331],3,&apak[171],4}, {&appr[334],2,&apak[175],2}, {&appr[336],1,&apak[177],0}, 
{&appr[337],1,&apak[177],0}, {&appr[338],1,&apak[177],0}, {&appr[339],1,&apak[177],0}, 
{&appr[340],1,&apak[177],0}, {&appr[341],1,&apak[177],0}, {&appr[342],2,&apak[177],2}, 
{&appr[344],2,&apak[179],2}, {&appr[346],2,&apak[181],2}, {&appr[348],1,&apak[183],0}, 
{&appr[349],1,&apak[183],0}, {&appr[350],1,&apak[183],0}, {&appr[351],1,&apak[183],0}, 
{&appr[352],1,&apak[183],0}, {&appr[353],1,&apak[183],0}, {&appr[354],1,&apak[183],0}, 
{&appr[355],1,&apak[183],0}, {&appr[356],1,&apak[183],0}, {&appr[357],1,&apak[183],0}, 
{&appr[358],1,&apak[183],0}, {&appr[359],1,&apak[183],0}, {&appr[360],1,&apak[183],0}, 
{&appr[361],1,&apak[183],0}, {&appr[362],1,&apak[183],0}, {&appr[363],1,&apak[183],0}, 
{&appr[364],1,&apak[183],0}, {&appr[365],2,&apak[183],2}, {&appr[367],2,&apak[185],2}, 
{&appr[369],2,&apak[187],2}, {&appr[371],2,&apak[189],2}, {&appr[373],1,&apak[191],0}, 
{&appr[374],1,&apak[191],0}, {&appr[375],2,&apak[191],2}, {&appr[377],1,&apak[193],0}, 
{&appr[378],1,&apak[193],0}, {&appr[379],1,&apak[193],0}, {&appr[380],1,&apak[193],0}, 
{&appr[381],1,&apak[193],0}, {&appr[382],1,&apak[193],0}, {&appr[383],1,&apak[193],0}, 
{&appr[384],1,&apak[193],0}, {&appr[385],2,&apak[193],2}, {&appr[387],2,&apak[195],2}, 
{&appr[389],2,&apak[197],2}, {&appr[391],2,&apak[199],2}, {&appr[393],2,&apak[201],2}, 
{&appr[395],1,&apak[203],0}, {&appr[396],1,&apak[203],0}, {&appr[397],1,&apak[203],0}, 
{&appr[398],1,&apak[203],0}, {&appr[399],1,&apak[203],0}, {&appr[400],1,&apak[203],0}, 
{&appr[401],1,&apak[203],0}, {&appr[402],14,&apak[203],10}, {&appr[416],2,&apak[213],2}, 
{&appr[418],2,&apak[215],2}, {&appr[420],2,&apak[217],2}, {&appr[422],6,&apak[219],2}, 
{&appr[428],2,&apak[221],0}, {&appr[430],1,&apak[221],0}, {&appr[431],1,&apak[221],0}, 
{&appr[432],1,&apak[221],0}, {&appr[433],1,&apak[221],0}, {&appr[434],1,&apak[221],0}, 
{&appr[435],1,&apak[221],0}, {&appr[436],1,&apak[221],0}, {&appr[437],1,&apak[221],1}, 
{&appr[438],2,&apak[222],2}, {&appr[440],2,&apak[224],2}, {&appr[442],2,&apak[226],2}, 
{&appr[444],6,&apak[228],2}, {&appr[450],2,&apak[230],2}, {&appr[452],1,&apak[232],0}, 
{&appr[453],1,&apak[232],0}, {&appr[454],1,&apak[232],0}, {&appr[455],1,&apak[232],0}, 
{&appr[456],1,&apak[232],0}, {&appr[457],1,&apak[232],0}, {&appr[458],1,&apak[232],0}, 
{&appr[459],1,&apak[232],0}, {&appr[460],1,&apak[232],0}, {&appr[461],6,&apak[232],2}, 
{&appr[467],6,&apak[234],2}, {&appr[473],2,&apak[236],1}, {&appr[475],1,&apak[237],0}, 
{&appr[476],1,&apak[237],0}, {&appr[477],1,&apak[237],0}, {&appr[478],1,&apak[237],0}, 
{&appr[479],1,&apak[237],0}, {&appr[480],1,&apak[237],0}, {&appr[481],6,&apak[237],2}, 
{&appr[487],2,&apak[239],1}, {&appr[489],7,&apak[240],3}, {&appr[496],1,&apak[243],0}, 
{&appr[497],1,&apak[243],0}, {&appr[498],1,&apak[243],0}, {&appr[499],1,&apak[243],0}, 
{&appr[500],1,&apak[243],0}, {&appr[501],1,&apak[243],0}, {&appr[502],1,&apak[243],0}, 
{&appr[503],1,&apak[243],0}, {&appr[504],1,&apak[243],0}, {&appr[505],1,&apak[243],0}, 
{&appr[506],1,&apak[243],0}, {&appr[507],1,&apak[243],0}, {&appr[508],1,&apak[243],0}, 
{&appr[509],1,&apak[243],0}, {&appr[510],1,&apak[243],0}, {&appr[511],5,&apak[243],3}, 
{&appr[516],1,&apak[246],0}, {&appr[517],5,&apak[246],4}, {&appr[522],5,&apak[250],4}, 
{&appr[527],1,&apak[254],0}, {&appr[528],7,&apak[254],3}, {&appr[535],2,&apak[257],1}, 
{&appr[537],2,&apak[258],1}, {&appr[539],2,&apak[259],1}, {&appr[541],2,&apak[260],1}, 
{&appr[543],2,&apak[261],1}, {&appr[545],1,&apak[262],0}, {&appr[546],1,&apak[262],0}, 
{&appr[547],1,&apak[262],0}, {&appr[548],1,&apak[262],0}, {&appr[549],1,&apak[262],0}, 
{&appr[550],1,&apak[262],0}, {&appr[551],1,&apak[262],0}, {&appr[552],1,&apak[262],0}, 
{&appr[553],1,&apak[262],0}, {&appr[554],1,&apak[262],0}, {&appr[555],1,&apak[262],0}, 
{&appr[556],1,&apak[262],0}, {&appr[557],1,&apak[262],0}, {&appr[558],1,&apak[262],0}, 
{&appr[559],1,&apak[262],0}, {&appr[560],1,&apak[262],0}, {&appr[561],1,&apak[262],0}, 
{&appr[562],1,&apak[262],0}, {&appr[563],1,&apak[262],0}, {&appr[564],1,&apak[262],0}, 
{&appr[565],5,&apak[262],3}, {&appr[570],6,&apak[265],2}, {&appr[576],6,&apak[267],2}, 
{&appr[582],5,&apak[269],5}, {&appr[587],6,&apak[274],1}, {&appr[593],7,&apak[275],4}, 
{&appr[600],6,&apak[279],2}, {&appr[606],1,&apak[281],0}, {&appr[607],1,&apak[281],0}, 
{&appr[608],1,&apak[281],0}, {&appr[609],1,&apak[281],0}, {&appr[610],1,&apak[281],0}, 
{&appr[611],1,&apak[281],0}, {&appr[612],1,&apak[281],0}, {&appr[613],1,&apak[281],0}, 
{&appr[614],1,&apak[281],0}, {&appr[615],1,&apak[281],0}, {&appr[616],1,&apak[281],0}, 
{&appr[617],1,&apak[281],0}, {&appr[618],3,&apak[281],0}, {&appr[621],1,&apak[281],0}, 
{&appr[622],1,&apak[281],0}, {&appr[623],1,&apak[281],0}, {&appr[624],1,&apak[281],0}, 
{&appr[625],1,&apak[281],0}, {&appr[626],1,&apak[281],0}, {&appr[627],1,&apak[281],0}, 
{&appr[628],1,&apak[281],0}, {&appr[629],2,&apak[281],2}, {&appr[631],2,&apak[283],2}, 
{&appr[633],1,&apak[285],0}, {&appr[634],5,&apak[285],5}, {&appr[639],1,&apak[290],0}, 
{&appr[640],1,&apak[290],0}, {&appr[641],7,&apak[290],4}, {&appr[648],2,&apak[294],1}, 
{&appr[650],1,&apak[295],0}, {&appr[651],1,&apak[295],0}, {&appr[652],1,&apak[295],0}, 
{&appr[653],1,&apak[295],0}, {&appr[654],1,&apak[295],0}, {&appr[655],1,&apak[295],0}, 
{&appr[656],1,&apak[295],0}, {&appr[657],1,&apak[295],0}, {&appr[658],2,&apak[295],2}, 
{&appr[660],6,&apak[297],1}, {&appr[666],1,&apak[298],0}, {&appr[667],1,&apak[298],0}, 
{&appr[668],6,&apak[298],1}, {&appr[674],1,&apak[299],0}, {&appr[675],1,&apak[299],0}, 
{&appr[676],1,&apak[299],0}, {&appr[0],0,&apak[0],0}};





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



