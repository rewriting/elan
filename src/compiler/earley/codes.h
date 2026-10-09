/*
  
    REM - Reduce ELAN Machine

    Copyright (C) 2000-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
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

    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr
    Christophe Ringeissen	e-mail: Christophe.Ringeissen@loria.fr
    Marian Vittek		e-mail: vittek@guma.ii.fmph.uniba.sk

*/
#ifndef __codes_h
#define __codes_h



/* -------------- values of standard func. symbols ------------- */
/*  values can range from 0 to FSYMCODESBEG (from commondefs.h)  */
/*  and have to correspond with the code values in .eln modules   */


//
//  The intention behind this module is to minimise the bordel with different
//  codes in different *.h modules
//

#define FALSEVAL 0        // if you change this, you have to reimplement bool// !!! BUILTIN
#define TRUEVAL 1         // if you change this, you have to reimplement bool// !!! BUILTIN
//---------------------------------------------------------------------------
#define PLUS                3
#define MINUS               4
#define TIMES               5
#define DIV                 6
//#define MAKEIDENT  7
#define INTEQUAL            8
#define INTNONEQUAL         9
#define INTLESS            10
#define INTLESSOREQ        11
#define INTGREATER         12
#define INTGREATEROREQ     13
#define IDENTEQUAL         14
#define IDENTNONEQUAL      15
#define REPLACE            16
#define OCCUR              17
#define EQUALL             18
#define NEQUALL            19
#define UNMIN              20
#define BOOLAND            21
#define BOOLOR             22
#define BOOLXOR            23
#define BOOLNOT            24
#define BOOLTOINT          25
#define INTTOBOOL          26
#define MOD                27
#define INTAND             28
#define INTOR              29
#define LESS               30
#define LESSEQ             31
#define GREATER            32
#define GREATEREQ          33
#define NUMTODOUBLE1       34          //  functions on double precision reals
#define NUMTODOUBLE2       35
#define NUMTODOUBLE3a      36
#define NUMTODOUBLE3b      37
#define DOUBLEPLUS         38
#define DOUBLEMINUS        39
#define DOUBLEMULT         40
#define DOUBLEDIV          41
#define DOUBLEUNMIN        42
#define DOUBLEEQ           43
#define DOUBLENEQ          44
#define DOUBLELESS         45
#define DOUBLELESSEQ       46
#define DOUBLEGREATER      47
#define DOUBLEGREATEREQ    48
#define DOUBLECONSTRUCT    49          // constructor of doubles constants 
#define DOUBLEEXP          50
#define DOUBLEEXP2         51
#define DOUBLEEXP10        52
#define DOUBLELOG          53
#define DOUBLELOG2         54
#define DOUBLELOG10        55
#define DOUBLEPOW          56
#define DOUBLESIN          57
#define DOUBLECOS          58
#define DOUBLETAN          59
#define DOUBLEASIN         60
#define DOUBLEACOS         61
#define DOUBLEATAN         62
#define INTTODOUBLE        63
#define NUMTODOUBLE4       64
#define NUMTODOUBLE5a      65
#define NUMTODOUBLE5b      66
#define NUMTOTERM          67       //  functions to construction of numbers and identifiers
#define IDENTTOTERM        68
#define NUMRULE            69
#define IDENTRULE          70
#define MINNUMTODOUBLE1    71       // negative double constants
#define MINNUMTODOUBLE2    72
#define MINNUMTODOUBLE3a   73
#define MINNUMTODOUBLE3b   74
#define MINNUMTODOUBLE4    75
#define MINNUMTODOUBLE5a   76
#define MINNUMTODOUBLE5b   77
#define DOUBLESQRT         78
#define INTCONSTUMIN       79
#define PIO                80       // #ifdef PICALC
#define STRINGTOTERM       81       // #ifdef STRINGS
#define STRINGRULE         82       // #ifdef STRINGS
#define QUIT               90
#define LOAD               91
#define SORTS              92
#define STARTWITH          93
#define RUN                94
#define DUMP               95
#define STAT               96
#define TRACE              97
#define PRINTWITH          98
#define COMPILMAIN         99       // used to compile dont care/know of rules in strategies
#define QUERIES           100       // #ifdef Command language
#define CHECKWITH         102
#define RESULTS           101                    
#define DISPLAY           103
#define BREAK_I           104
#define UNBREAK_I         105
#define BREAKS            106
#define BATCH             107
#define DUMP_I            108
#define DUMP_N            109
#define BREAK_N           110
#define UNBREAK_N         111
#define HELP              112       // #endif Command language
#define BUILTGETC         113
#define BUILTPUTC         114
#define BUILTCREATE       115       // #ifdef IOS
#define BUILTOPEN         116
#define BUILTCREATENOBLOCK       117
#define BUILTCLOSE        118
#define BUILTWRITE        119
#define BUILTREAD         120
#define BUILTPIDCONSTR    121                                         // !!! BUILTIN
#define BUILTERRORPIPCONSTR 122                                       // !!! BUILTIN
#define BUILTERRORXCONSTR 123       // #endif IOS                     // !!! BUILTIN
#define NEW_META_APPLY    128
#define META_APPL 	  129       // META
#define META_APPLY 	  130       // META
#define One_Strategy      131
#define More_Strategy     132
#define One_Rule          133
#define More_Rules        134
#define DC_Labels         135
#define DK_Labels         136
#define DC_Strategies     137
#define DK_Strategies     138
#define Repeat_Strategy   139
#define Iterate_Strategy  140
#define One_Strateg       141
#define More_Strateg      142
#define Identity          143
#define Call              144
#define STRANYIF          145
#define STRANYNIL         146
#define STRANYCONS        147
#define STRLENGTH         150      // #ifdef STRINGS
#define STRAPPEND         151
#define STRINDEX          152
#define STRMODIF          153
#define STRSUBSTR         154
#define STRSTR            155
#define STRSPN            156
#define STRCMP            157
#define STRNG             158     // #endif STRINGS
//-------------------------------------
#define DS_APPLY          180
#define DS_DC             181
#define DS_DK             182
#define DS_FAIL           183
#define DS_IFTE           184
#define DS_COMMA          185
#define DS_ID             186
#define DS_IFTOE          187
#define DS_CONC           188
#define DS_EPSILON        189
#define DS_ONE            190
//-------------------------------------
#define SYNTACTICMATCHING 191
//-------------------------------------
#define LISTNULL          192
#define LISTCONS          193
#define LISTHEAD          194
#define LISTTAIL          195
#define LISTPREFIX        196
#define LISTLAST          197
#define LISTNOTEMPTY      198
#define LISTSINGLEELEMENT 199

/*
#define FSYMTABSIZE    (RULECONSTRULE1+1)
#define RULECONSTRULE  (MAXNFSYM+1)   // sem. actions for standard rules
#define RIGHTSRULE     (MAXNFSYM+2)
#define STRATCONSTRULE (MAXNFSYM+3)
#define RULECONSTRULE1 (MAXNFSYM+4
*/

// defstrat word:
#define DS_FSYM     0x1000000                 // MAXFSYM*f1+f2
#define FSYM_FLAG(f1,f2) (DS_FSYM | (((f1)<<12)+(f2)))
#define IS_FSYM_FLAG(x)   (( (x) & 0xf000000) == DS_FSYM)
#define FSYM_F1(x)        (((x) & 0xfff000)>>12)
#define FSYM_F2(x)        ((x) & 0xfff)

#define DS_LAB      0x2000000                 // rule_index
#define LAB_FLAG(l) (DS_LAB | (l))
#define IS_LAB_FLAG(x) (( (x) & 0xf000000) == DS_LAB)

#define DS_DSTR     0x4000000
#define DSTR_FLAG(f,lab)   (DS_DSTR | (((f)<<12)+(lab)))
#define DSTR_F(x)          (((x) & 0xfff000)>>12)
#define DSTR_LAB(x)        ((x) & 0xfff)

#define IS_DSTR_FLAG(x) (( (x) & 0xf000000) == DS_DSTR)

#define DS_APPL    0x8000000
#define APPLY_FLAG(x)   (DS_APPL | (x))
#define IS_APPLY_FLAG(x) (( (x) & 0xf000000) == DS_APPL)

/*
#define DS_PRIMAL    0x10000000
#define PRIMAL_FLAG   (DS_PRIMAL)

#define IS_PRIMAL_SYMBOL(x) \
          ( x == DS_DC || x == DS_DK || x == DS_FAIL || x == DS_ID || \
	    x == DS_IFTE || x == DS_IFTOE || x == DS_CONC )
*/

#endif
