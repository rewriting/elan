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

*/
package rem.compiler;

import java.util.*;

class PatternCode {
    /* AC pattern <---> [index,code]  */
    private static Hashtable table = new Hashtable(10);
    private static int currentIndex=0;

    private int index; /* position in the array */
    private String code; /* generated C code */
    private Term term; /* associated term */

    public PatternCode(Term term) {
	index=currentIndex++;
	this.term=term;

	OutputCode s = new OutputCode();
	int deep=0;
	term.genTermForACMatcher(s,deep+1);
	code=s.stringDump();
    }

    public int getNumber() {
	return index;
    }

    public Term getTerm() {
	return term;
    }

    public String getCode() {
	return code;
    }

    public static PatternCode get(Term pattern) {
	return (PatternCode) table.get(pattern);
    }

    public void put(Term term) {
	PatternCode oldPatternCode = (PatternCode) table.put(term,this);
	if( oldPatternCode != null ) {
	    if (Flags.warnings) {
		/* warning */
	    }
	}
    }

  public static void genInitialisation(OutputCode s, int deep) {
    s.write(deep,"\n/* Initialisation des patterns AC */\n");
    Enumeration e  = table.keys();
    // mettre le bon nb d'elements
    //ehm modification a faire aprés AC
    if(Flags.aterm == false) {
    s.write(deep,"TERM *EkerTerm[100];\n");
    s.write(deep,"void EkerTermInit() {\n");
    s.write(deep+1,"TERM_LIST *vlist[100];\n");
    s.write(deep+1,"AC_LIST *acvlist[100];\n");
    s.write(deep+1,"Gterm *sv[100];\n");
    } else {
    s.write(deep,"//TERM *EkerTerm[100];\n");
    s.write(deep,"void EkerTermInit() {\n");
    s.write(deep+1,"//TERM_LIST *vlist[100];\n");
    s.write(deep+1,"//AC_LIST *acvlist[100];\n");
    s.write(deep+1,"Gterm *sv[100];\n");
    }    
    while(e.hasMoreElements()) {
      Term key = (Term)e.nextElement();
      PatternCode pattern = get(key);

      //key.genTermForACMatcher(s,deep+1);
      s.write(deep+1,"/* " + key + " */\n");
      s.write(deep+1,pattern.getCode());
      /*
	s.write(deep+1,"ac_compress((TERM*)sv[" + 
	key.getVarNumber() + "]);\n");
      */
      s.write(deep+1,"//eker_print_term((TERM*)sv[" + key.getVarNumber() + "]);\n");
      s.write(deep+1,"//printf(\"\\n\");\n");
      

      s.write(deep+1,"ac_sort((TERM*)sv[" + 
	      key.getVarNumber() + "]);\n");
      s.write(deep+1,"EkerTerm[" + pattern.getNumber() +
	      "] = (TERM*)sv[" +
	      key.getVarNumber() + "];\n");

    }
    s.write(deep,"}\n");
  }


    
}
