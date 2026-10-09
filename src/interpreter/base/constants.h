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

// Implementation limits and codes shared by the whole interpreter
// (split from commondefs.h).

#ifndef __constants_h
#define __constants_h

// DEFINITION DES CONSTANTES POUR ELAN

#define NBITS 32	// number of bits in word in current arch.

                 // values of info record in term grammar
#define RNOINFO 0       
#define RGLOP 01	// global rule
#define RLOCOOP 02	// local rule


#define RIMPORTBIT 04  // was it imported 

#define RSTANDOP 010   // built in rule
#define RVAR 020       // variable
#define RSTATMSK 07

//#define NRESW 50        // size of table of res. words in ecolog
#define DEFAULTSYM -32000 // symbol for default action if parser's tables

/* ------------ some implementation constants -------------------*/
#define OUTS stderr       // 
#define OUTPUTS "stderr"  // outputs of executable
#define MAXNOFIMPORTS 200 // max. number of imported modules
#define MRWTSIZE 300      // number of res.word of ELAN (to accurate when def.)
#define MAXINCLDEEP 30    // max. deep of import graph
#define MAXNOFMAC 50	  // max. number of macro vars.
#define MAXNESTMAC 10	  // max. nested macro
#define MAXUNIFFSYM MAXNFSYM  // max. number of symbols in comm. with UNIF
#define MAXDEEPCONSTEXP 10 // max. deep of constant expression in prepro.
#define MAXNOFTRN 2000	  // max. number of trans. rule names
#define MAXNOFAXIOMS 10	  // max. number of parts in specification
#define MAXNOFSTRAT 500	  // max. number of strategies
#define MAXINCLSTRAT 30	  // max. number of rep./iterate nesting
#define MAXNOFSUBPROCESS 30 // max. number of subprocessus
#define MAXNNONT 500    // maximal number of nonterminals
//#define MAXTERMDEEP 10000     // max deep  of terms
#define MAXTERMDEEP 20000     // max deep  of terms
#define MAXNOFARITIES 100    // maximal number of different arities

//Marian's version 
#define VARSPRI 50      // variable priority (in term grammar)
//#define VARSPRI 4010      // variable priority (in term grammar)
//MARIAN's version
//#define STANDPRI 10000 // priority of standards (number & identifiers)
#define STANDPRI 4000       // priority of standards (number & identifiers)

#define STARTTYPE (NTYPES+1)  // codes for built in types 
#define RIGHTSTYPE (NTYPES+2)
#define STRATTYPE (NTYPES+3)



#define STRLEN   1000     // max. length of any string 

// CONSTANTES POUR ELAN ET LIBEARLEY

#define EXIT_FAILURE 1
#define ERRORIM 1         // return values for semact 
#define ACCEPTIM 2        // accept immediately
#define NORMCONT 3        // normal cont.
#define HANDERRORIM 4     // error, but handled by semact.

		// values of priority record in term grammar
#define RNOPRIOR 0
//#define RAXPREDPRIOR1 100	// priority of '&' in axpredicateset rule
//#define RAXPREDPRIOR2 50	// priority of (axpredicate)axpredicateset rule
#define RPRIORITYMSK 007777
#define RLEFTASSOC   010000	// left assoc.
#define RRIGHTASSOC  020000   	// right assoc.
#define RASSOCMSK    070000
#define RBINSTR      040000     // built in strategy

#define IDLEN 50          // max. length of dist. character in identifier
#define MAXNOFIDENT 3000  // max number of identifier used in all prog.
#define MLENGRRULE 200    // max. size of term grammar rule

//#define MAXRWINTERM 600 // max. number of res. word for term grammar
#define MAXNOFVAR 49      // max. number of variables in one rule (must be the 
                          //                   same as in acmatching procedure
#define MAXNFSYM 2000      // max number of functionals symbols

#define MAXLENNTERM 50000 // max. number of lexems in term 
//#define MAXLENNTERM 10000 // max. number of lexems in term 
extern  int MAXLENNTERMv; 
#define NTYPES 500        // max. number of types in ELAN
#define CHUNKSIZE 10    // size of chunk for lbuffer

#define FSYMCODESBEG 300	  // begin of codes of symbols
			// codes less then FSYMCODESBEG are for built-ins
#define RULECONSTRULE  (MAXNFSYM+1)   // sem. actions for standard rules
#define RIGHTSRULE     (MAXNFSYM+2)
#define STRATCONSTRULE (MAXNFSYM+3)
#define RULECONSTRULE1 (MAXNFSYM+4)
#define FSYMTABSIZE    (RULECONSTRULE1+1)

#define NNONTERMINALS (NTYPES+4) // max. number of nonterminals in the 
                          // term grammar 

#endif
