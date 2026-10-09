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
    s.write(deep,"TERM *EkerTerm[100];\n");
    s.write(deep,"void EkerTermInit() {\n");
    s.write(deep+1,"TERM_LIST *vlist[100];\n");
    s.write(deep+1,"AC_LIST *acvlist[100];\n");
    s.write(deep+1,"struct term *sv[100];\n");

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
