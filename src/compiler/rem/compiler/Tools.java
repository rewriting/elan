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
import java.io.*;

public final class Tools {

  public static String indent(int deep) {
    StringBuffer s = new StringBuffer();
    for(int j=0 ; j<deep ; j++) {
      s.append("  ");
    }
    return s.toString();
  }

  public static void indent(StringBuffer s, int deep) {
    for(int i=0 ; i<deep ; i++) {
      s.append("  ");
    }
  }

  public static void indent(OutputCode file, int deep) {
    for(int i=0 ; i<deep ; i++) {
      file.write("  ");
    }
  }

  public static void genDeclaration(OutputCode s, int deep, BitSet b) {
    Tools.indent(s,deep); s.write("Gterm *tmp");
    for(int i=0 ; i<b.size() ; i++) {
      if(b.get(i)) {
	s.write(", *sv"+i);
      }
    }
    s.write(";\n");
  }

  public static void genDeclaration(OutputCode s, int deep, int max) {
    int max2=max+1;
    s.write(deep,"Gterm *tmp, *sv[" + max2 + "];\n");
      // [pem: Nov  2 00] deplace de Term.java 
    s.write(deep,"multiplicityType *E,*sol;\n");

  }

  /*
  public static String genFail(int deep) {
    StringBuffer s = new StringBuffer();
    Tools.indent(s,deep); s.append("fail();\n");
    return s.toString();
  }
  */

  public static void genFail(OutputCode s,int deep) {
    s.write(deep,"fail();\n");
      //s.write(deep,"goto fail;\n");
  }

  public static void genComment(OutputCode s,int deep,String comment) {
    s.write(deep,"/* " + comment + " */\n");
  }

  public static void genRwrCounter(OutputCode s,int deep,String counter) {
    if(Flags.rwrCounter) {
      s.write(deep,counter + "++;\n");
    }
  }

  public void genHeader(OutputCode s) {
    int deep=0;
    s.write("#ifndef __main_header_h\n");
    s.write("#define __main_header_h\n");

    if(Flags.coq) {
      s.write("#include \"main_coq_skeleton.h\"\n");
    } else if(Flags.lib) {
      s.write("#include \"main_skeleton_lib.h\"\n");
    } else {
      s.write("#include \"main_skeleton.h\"\n");
    }

      /* structure and code generation */
    s.write(Symbol.genStructure(deep));

      /* Constantes d'execution */
    s.write("\n/* Constantes d'execution */\n");
    s.write("#define FSYM_TAB_SIZE " + (Symbol.getMaxCode()+1) + "\n");
    s.write("extern int fsymtabSize;\n");
    s.write("/* 0: no trace, 1: result, 2: start with */\n");
    s.write("extern unsigned long tab_rewrite_step[2][FSYM_TAB_SIZE];\n");
    
    s.write(Symbol.genDeclarFsym(deep));
    s.write(Strategy.genDeclar(deep));
    if(Flags.strat > 0) {
      s.write(Symbol.genDeclarStrategyFunctions(deep));
    }
    s.write("#endif\n");
  }

  public static void genStrategyProlog(OutputCode s, int deep, String name) {

    // Declaration de mask, res
    Tools.indent(s,deep+1); s.write("bitSet *mask;\n");
    s.write(deep+1,"Gterm *res=v0;\n");

    if(Flags.debug) {
        /* Il ne faut pas de ; ici */
      s.write(deep+1,"declareIndentLevel()\n");
      s.write(deep+1,"addindent();\n");
      s.write(deep+1,"saveGlobalIndent();\n");
      s.write(deep+1,"if(traceLevel>=2) {\n");
      s.write(deep+2,"doindent(indentlevel);\n");
      s.write(deep+2,"printf(\"start with: \");\n");
      s.write(deep+2,"printf(\"str" + name + " on \");\n");
      s.write(deep+2,"internal_term_println(stdout,v0,resultMode);\n");
      s.write(deep+1,"}\n");
    }
  }	

  public static void genStrategyEpilog(OutputCode s, int deep, String name) {

    //s.write(Tools.genFail(deep)); /* PEM TEST */

    s.write("end:\n");
    /*
     * le comptage des regles appliquees est deplacee dans
     * genApplication et genACApplication
     * il est aussi possible de le faire apres les StratLab
     */
    //Tools.indent(s,deep+1); s.write("rewrite_step++;\n");
    //s.write("end_no_rewrite:\n");
    if(Flags.debug) {
      s.write(deep+1,"if(traceLevel>=1) {\n");
      s.write(deep+2,"doindent(indentlevel);\n");
      s.write(deep+2,"printf(\"rewrite(str" + name + ")[%u] \",rewrite_step);\n");
      s.write(deep+2,"internal_term_println(stdout,res,resultMode);\n");
      s.write(deep+1,"}\n");
    }

    if(Flags.debug) {
      Tools.indent(s,deep+1); s.write("subindent();\n");
      Tools.indent(s,deep+1); s.write("restoreGlobalIndent();\n");
    }
    s.write(deep+1,"return res;\n");
    s.write(deep+1,"fail:\n");
    s.write(deep+1,"// printf(\"fail\\n\");\n");
    s.write(deep+1,"fail();\n");
  }

  public static OutputCode patternListDeclaration = new OutputCode();
  public static OutputCode patternListInit = new OutputCode();
  public static OutputCode patternListDelete = new OutputCode();

  public void genMainFile(OutputCode s, String fileName,Query query) {
    int deep=0;
    s.write("#include \"" + fileName + ".h\"\n");

    if(Flags.coq) {
      s.write("#include \"main_coq_skeleton.c\"\n");
    } else if(Flags.lib) {
      s.write("#include \"main_skeleton_lib.c\"\n");
    } else {
      s.write("#include \"main_skeleton.c\"\n");
    }

      /* Constantes d'execution */
    s.write("\n/* Constantes d'execution */\n");
    s.write("unsigned long tab_rewrite_step[2][FSYM_TAB_SIZE];\n");
    s.write("int fsymtabSize = FSYM_TAB_SIZE;\n");

    s.write("\n/* Table des symboles */\n");
    s.write("Gfsym fsymtab[FSYM_TAB_SIZE];\n");

    DDTree.genPatternListDeclaration(s,deep);

    s.write(Symbol.genCreateConstant(deep) );
    s.write(Symbol.genRedirectBuiltins(deep));	// Peter

    /*
     * Redirection des fonctions et constantes
     */
    Symbol.genRedirectFsym(s,deep);
    /*
     * Redirection des strategies
     */
    Strategy.genRedirectStrategy(s,deep);

    if(Flags.strat > 0) {
      s.write("\n#ifdef BORO\n");			// Peter
      Symbol.genLabeledRules(s,deep);    		// Peter
      Symbol.genDefinedStrategies(s,deep);      	// Peter
      s.write("\n#endif\n");				// Peter
    }

    /*
     * Preconstruction des patterns AC
     */
    PatternCode.genInitialisation(s,deep);

      /* Query */
    s.write("\n/* Query */\n");
    s.write("Gterm *main_query() {\n");
    s.write(deep+1,"Gterm *res;\n");
    s.write(deep+1,"/* TERME DE DEPART */\n");
    query.genCode(s,deep+1);
    s.write(deep+1,"return res;\n");
    s.write("}\n");
    query.genStartTerm(s,deep);

      /* symbol_init */
    s.write("void symbol_init() {\n");
    s.write(deep+1,"int i;\n");
    Symbol.genCreateFsym(s,deep+1);
    s.write(Symbol.genInitConstant(deep+1));
    DDTree.genPatternListInit(s,deep+1);
    s.write("}\n");
    
  }


  public static void fileUpdate(String fileName) {
    String generatedFileName = fileName + "~";

    File file = new File(".",fileName);
    File generatedFile = new File(".",generatedFileName);

    if(!generatedFile.exists()) {
      throw new InternalError(generatedFile.getName() + "does not exist !");
    }
      //System.out.println("name   = " + file.getName());
      //System.out.println("exists = " + file.exists());
    if(!file.exists()) {
      generatedFile.renameTo(file);
    } else {
      /*
       * Comparaison de deux fichiers : a ameliorer
       */
      boolean equal=true;
      try {
	BufferedReader r1 = new BufferedReader(new FileReader(fileName));
	BufferedReader r2 = new BufferedReader(new FileReader(generatedFileName));
	
	try {
	  while(equal && r1.ready() && r2.ready()) {
	    String l1 = r1.readLine();
	    String l2 = r2.readLine();
	    equal = equal && l1.equals(l2);
            if(!equal){
                //System.out.println("l1 = " + l1);
                //System.out.println("l2 = " + l2);
            }
          }
            //System.out.println("eq1    = " + equal);
	  if((r1.ready() && !r2.ready()) || (!r1.ready() && r2.ready())) {
	    equal=false;
	  }
            //System.out.println("eq2    = " + equal);
	} catch (IOException e) {
	} 
	r1.close();
	r2.close();
	} catch (IOException e) {
	// } catch (FileNotFoundException e) {
      }
      if(equal) {
	generatedFile.delete();
      } else {
	generatedFile.renameTo(file);
      }
    }
  }
}
