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


void banner()
{
  stout<<"\n";
  stout<< "ELAN version " << VERSION << "\n\n";
  stout<< "Copyright (C) 1994-2003  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)\n";
  stout<< "                         Nancy, France.\n";

#ifndef TO_BE_DISTRIBUTED
  stout<<"\n\t Options: \t";
  stout<<"+Strategies";
#ifdef PEM
  stout<<"+Pem";
#endif
#ifdef STORM
  stout<<"+Storm";
#endif
#ifdef VISIGRAPH
  stout<<"+VisiGraph";
#endif
#ifdef COMMAND
  stout<<"+CLang";
#endif
  stout<<"+I/Os";
#ifdef PICALC
  stout<<"+PICALC";
#endif  
#ifdef BINS
  stout<<"+Built-ins";
#endif  
  stout<<"+Normalise";
  stout<<"\n\t\t\t";

#ifdef __DECCXX
  stout<<"+Dec Compiler";
#endif  
  stout<<"+Meta";
#ifdef ANYS
  stout<<"+Anys";
#endif
#ifdef SYMBS
  stout<<"+Symbs";
#endif
  stout<<"+Select";
  stout<<"+Strings";
  stout<<"+Case&Patterns";
  stout<<"\n\t\t\t";
#ifdef EARLEY
  stout<<"+Earley";
#endif
  stout<<"+Peval";
  stout<<"\n";
#endif
}

void usage()
{
    //<< "elan usage : elan [options] LPL_description_file"
    //<< " specification_file\n"

  sterr << "\n" 
	<< "Usage:\n"
        << "\telan [options] lgi_file [spc_file]\n"
  << "Options:\n"
  <<"\t--dump, -d\t\t: dump of signature, strategies and rules\n"
  <<"\t--trace, -t [num]\t: tracing of execution (default max)\n"
  <<"\t--statistic, -s/-S\t: statistics: short/long\n"
  <<"\t--warningsoff, -w\t: suppress warning messages of the parser\n"
  <<"\t--quiet, -q\t\t: quiet regime of execution\n"
  <<"\t--batch, -b\t\t: batch regime (no messages at all)\n"
  <<"\t--elanlib\t\t: elanlib\n"
  <<"\t--secondlib, -l lib\t: second elan library (by default ..)\n"
  <<"\t--command, -C\t\t: command language\n" 
  <<"\t--export, --cexport\t: export to a .ref file\n" 
  <<"\t--import  \t\t: import from a .ref file\n" 
#ifndef TO_BE_DISTRIBUTED
  <<"\t--lis/los\t\t: left-most inner/outer-most normalisation\n"
  <<"\t--reduce\t\t: import and run from a .ref file (for meta_apply)\n"
  <<"\t--peval\t\t: partial evaluation\n"
  <<"\t--compr\t\t: compressing of terms (for --peval)\n"
  <<"\t--extend\t\t: extend equally named rules (for --peval)\n"
  <<"\t--no_more\t\t: write no_more instead of end\n"
  <<"\t--reduce\t\t: import and run from a .ref file (for meta_apply)\n"
  <<"\t--reduce\t\t: import and run from a .ref file (for meta_apply)\n"
#endif
// NO LONGER SUPPORTED in version 3.* <<"\t--compiler, -c\t\t: use compiler\n"
// NO LONGER SUPPORTED in version 3.*  <<"\t--optimise, -O\t\t: optimise compiled code\n"
// NO LONGER SUPPORTED in version 3.* <<"\t--deterministic, -n\t: use deterministic library (for compiler only)\n"
    // Ne marche plus
    //<<"\t--expmatch, -e\t\t: generate faster many-to-one matching alg.\n"
#ifdef EARLEY
//  NO LONGER SUPPORTED in version 3.* <<"\t--exe\t\t\t: compile into an independent executable\n"
//  NO LONGER SUPPORTED in version 3.* <<"\t--earley\t\t: compile into an independent executable\n"
//  NO LONGER SUPPORTED in version 3.* <<"\t--output, -o name\t: give a name to the compiled executable file\n"
//  NO LONGER SUPPORTED in version 3.* <<"\t--code\t\t\t: generate also intermediate .c and .h files\n"
#endif
  ;
  failexit();
}


void usage_vars()
{
  sterr 
    << "\n\n"
    << "elan variables \n\n"
    <<"\t--MAXLENNTERM\t\t: max. number of lexems in term [" << MAXLENNTERM <<"] \n"

    // this list should be much more longer
    // every elan constant should be variabalized by a default value
    // it is necessary to touch all static allocations of arrays,
    // and it is shitty job ...

    ;
}
