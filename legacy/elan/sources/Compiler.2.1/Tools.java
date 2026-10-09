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
    Tools.indent(s,deep); s.write("struct term *tmp");
    for(int i=0 ; i<b.size() ; i++) {
      if(b.get(i)) {
	s.write(", *sv"+i);
      }
    }
    s.write(";\n");
  }

  public static void genDeclaration(OutputCode s, int deep, int max) {
    int max2=max+1;
    s.write(deep,"struct term *tmp, *sv[" + max2 + "];\n");
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

  public void genHeader(OutputCode s) {
    int deep=0;
    s.write("#ifndef __main_header_h\n");
    s.write("#define __main_header_h\n");

    s.write("#include <stdio.h>\n");
    s.write("#include <stdlib.h>\n");
    s.write("#include <sys/time.h>\n");
    s.write("#include <sys/resource.h>\n");
    s.write("#include <sys/types.h>\n");
    s.write("#include \"tools.h\"\n");
    s.write("#include \"term.h\"\n"); 
    s.write("#include \"bitset.h\"\n");
    s.write("#include \"bgraph.h\"\n") ;
    s.write("#include \"match_state.h\"\n");
    s.write("#include \"back.h\"\n");
    s.write("#include \"choice.h\"\n");
    s.write("#include \"builtin.h\"\n");
    s.write("#include \"builtinMatching.h\"\n"); 
    s.write("#include \"streval.h\"\n");  // Peter
    s.write("#include \"gc.h\"\n");
    /* For Eker AC matcher */
    s.write("#include \"acmatchdefs.h\"\n");
      /* For Earley IO */
    s.write("#include \"termIn.h\"\n"); 
    s.write("#include \"termOut.h\"\n"); 

    s.write(Symbol.genStructure(deep));
    s.write("\n/* Constantes d'execution */\n");
    s.write("/* 0: no trace, 1: result, 2: start with */\n");
    if(Flags.debug) {
      s.write("extern int trace;\n");
    } else {
      s.write("#define trace 0\n");
    }
    s.write("extern int debugMode;\n");
    s.write("extern int quietMode;\n");
    s.write("extern unsigned long rewrite_step;\n");
    if(Flags.debug) {
    s.write("extern unsigned long tab_rewrite_step[2][" + Symbol.getMaxCode() +"];\n");
    }
    s.write("extern int global_indentlevel;\n");
    s.write("extern TERM *EkerTerm[];\n");

    s.write("\n/* Macros */\n");
    s.write("#define MAX_TERM_SIZE 1000 /* nb max du sous-termes d'un symbole AC */\n");
    s.write("#define MAX_CBG_SIZE  100  /* nb max de patterns dans un CBG */\n"); 
    s.write("#ifdef DEBUG\n");
    s.write("#define declareIndentLevel() int indentlevel;\n");
    s.write("#define addindent() global_indentlevel++;\n");
    s.write("#define subindent() indentlevel--;\n");
    s.write("#define doindent(deep) indent(deep);\n");
    s.write("#define saveGlobalIndent() indentlevel=global_indentlevel;\n");
    s.write("#define restoreGlobalIndent() global_indentlevel=indentlevel;\n");
    s.write("#else\n");
    s.write("#define declareIndentLevel()\n");
    s.write("#define addindent()\n");
    s.write("#define subindent()\n");
    s.write("#define doindent(deep)\n");
    s.write("#define saveGlobalIndent()\n");
    s.write("#define restoreGlobalIndent()\n");
    s.write("#endif\n");
    s.write("#define allocStable(x) MALLOC(x)\n");

    
    s.write(Symbol.genDeclarFsym(deep));
    s.write(Strategy.genDeclar(deep));
    s.write("#endif\n");
  }

  public static void genStrategyProlog(OutputCode s, int deep, String name) {

    // Declaration de mask, res
    Tools.indent(s,deep+1); s.write("bitSet *mask;\n");
    s.write(deep+1,"struct term *res=v0;\n");

    /* Il ne faut pas de ; ici */
    s.write(deep+1,"declareIndentLevel()\n");
    s.write(deep+1,"addindent();\n");
    s.write(deep+1,"saveGlobalIndent();\n");
    s.write(deep+1,"if(trace>=2) {\n");
    s.write(deep+2,"doindent(indentlevel);\n");
    s.write(deep+2,"printf(\"start with: \");\n");
    s.write(deep+2,"printf(\"str" + name + "(\");\n");
    s.write(deep+2,"term_print(stdout,v0);\n");
    s.write(deep+2,"printf(\")\\n\");\n");
    s.write(deep+1,"}\n");
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
    Tools.indent(s,deep+1); s.write("if(trace>=1) {\n");
    Tools.indent(s,deep+2); s.write("doindent(indentlevel);\n");
    Tools.indent(s,deep+2); s.write("printf(\"rewrite(str" + name + ")[%u] \",rewrite_step);\n");
    Tools.indent(s,deep+2); s.write("term_printnl(stdout,res);\n");
    Tools.indent(s,deep+1); s.write("}\n");
    Tools.indent(s,deep+1); s.write("subindent();\n");
    Tools.indent(s,deep+1); s.write("restoreGlobalIndent();\n");
    s.write(deep+1,"return res;\n");
    s.write(deep+1,"fail:\n");
    s.write(deep+1,"// printf(\"fail\\n\");\n");
      //s.write(deep+1,"subindent();\n");
      //s.write(deep+1,"restoreGlobalIndent();\n");
    s.write(deep+1,"fail();\n");
  }

  public static OutputCode patternListDeclaration = new OutputCode();
  public static OutputCode patternListInit = new OutputCode();
  public static OutputCode patternListDelete = new OutputCode();

  public void genMainFile(OutputCode s, String fileName,Query query) {
    int deep=0;
    s.write("#include \"" + fileName + ".h\"\n"); 
    s.write("extern struct term *query;\n");

    s.write("\n/* Constantes d'execution */\n");
    //s.write("/* 0: no trace, 1: result, 2: start with */\n");
    //s.write("#define trace 0\n");
    s.write("unsigned long rewrite_step=0;\n");
    if(Flags.debug) {
      s.write("unsigned long tab_rewrite_step[2][" + Symbol.getMaxCode() +"];\n");
    s.write("int trace=0;\n");
    }
    s.write("int global_indentlevel=0;\n");
    s.write("int debugMode=0;\n");
    s.write("int quietMode=0;\n");
    s.write("char *sortName=NULL;\n");
    s.write("char *strategyName=NULL;\n");

    s.write("\n/* Table des symboles */\n");
    s.write("int fsymtabSize = " + (Symbol.getMaxCode()+1) + ";\n");
    s.write("fsym fsymtab[" + (Symbol.getMaxCode()+1) + "];\n");

    DDTree.genPatternListDeclaration(s,deep);

    s.write( Symbol.genCreateConstant(deep) );
    s.write( Symbol.genRedirectBuiltins(deep));	// Peter

    /*
     * Redirection des fonctions et constantes
     */
    Symbol.genRedirectFsym(s,deep);
    /*
     * Redirection des strategies
     */
    Strategy.genRedirectStrategy(s,deep);

    if (Flags.strat > 0) {
      s.write("\n#ifdef BORO\n");			// Peter
      Symbol.genLabeledRules(s,deep);    		// Peter
      Symbol.genDefinedStrategies(s,deep);      	// Peter
      s.write("\n#endif\n");				// Peter
    }

    /*
     * Preconstruction des patterns AC
     */
    PatternCode.genInitialisation(s,deep);


    s.write("\n/* Query */\n");
    s.write("struct term *main_query() {\n");
    s.write(deep+1,"struct term *res;\n");
    s.write(deep+1,"/* TERME DE DEPART */\n");
    query.genCode(s,deep+1);
    s.write(deep+1,"return res;\n");
    s.write("}\n");

 
    s.write("\n/* Procedure principale */\n");
    s.write(0,"#ifndef CSETCHP\n");
    s.write("long *bp_main;\n");
    s.write(0,"#endif\n");
    s.write(0,"struct rusage before_self,after_self;\n");
    s.write(0,"long diff_sec,diff_usec;\n");
    s.write(0,"double total_time;\n");
    
    s.write("int main(int argc,char **argv) {\n");
    s.write(0,"#ifdef CSETCHP\n");
    s.write(deep+1,"char bp;\n");
    s.write(0,"#else\n");
    s.write(deep+1,"long bp;\n");
    s.write(0,"#endif\n");
    s.write(deep+1,"struct term *res;\n");
    s.write(deep+1,"int i,j;\n");
    s.write(deep+1,"int queryMode=2;  /* 0:noInput  1:REFInput  2:Elan form 3:command line */\n");
    s.write(deep+1,"int ResultMode=2; /* 0:noOutput 1:REFOutput 2:Elan form 3:internalOutput */\n");
    s.write(deep+1,"int evaluationMode=0; /* 0:lgi 1:sort 2:strat:sort*/\n");
    
    s.write(0,"#ifdef CSETCHP\n");
    s.write(deep+1,"choice_init(&bp);\n");
    s.write(0,"#else\n");
    s.write(deep+1,"bp_main=&bp;\n");
    s.write(0,"#endif\n");
    s.write(deep+1,"GC_free_space_divisor=2;\n");

    s.write(deep+1,"for(i=1 ; i<argc && *argv[i]=='-' ; i++) {\n");
    s.write(deep+2,"if(!strcmp(argv[i],\"-noInput\")) {\n");
    s.write(deep+3,"queryMode=0;\n");
    s.write(deep+2,"}\n");
    s.write(deep+2,"if(!strcmp(argv[i],\"-REFInput\")) {\n");
    s.write(deep+3,"queryMode=1;\n");
    s.write(deep+2,"}\n");
    s.write(deep+2,"if(!strcmp(argv[i],\"-commandLine\")) {\n");
    s.write(deep+3,"queryMode=3;\n");
    s.write(deep+2,"}\n");
    s.write(deep+2,"if(!strcmp(argv[i],\"-noOutput\")) {\n");
    s.write(deep+3,"ResultMode=0;\n");
    s.write(deep+2,"}\n");
    s.write(deep+2,"if(!strcmp(argv[i],\"-REFOutput\")) {\n");
    s.write(deep+3,"ResultMode=1;\n");
    s.write(deep+2,"}\n");
    s.write(deep+2,"if(!strcmp(argv[i],\"-internalOutput\")) {\n");
    s.write(deep+3,"ResultMode=3;\n");
    s.write(deep+2,"}\n");
    s.write(deep+2,"if(!strcmp(argv[i],\"-debug\")) {\n");
    s.write(deep+3,"debugMode=1;\n");
    s.write(deep+2,"}\n");
    s.write(deep+2,"if(!strcmp(argv[i],\"-quiet\")) {\n");
    s.write(deep+3,"quietMode=1;\n");
    s.write(deep+2,"}\n");
    s.write(deep+2,"if(!strcmp(argv[i],\"-sort\")) {\n");
    s.write(deep+3,"evaluationMode=1;\n");
    s.write(deep+3,"sortName=argv[++i];\n");
    s.write(deep+2,"}\n");
    s.write(deep+2,"if(!strcmp(argv[i],\"-strategy\")) {\n");
    s.write(deep+3,"evaluationMode=2;\n");
    s.write(deep+3,"strategyName=argv[++i];\n");
    s.write(deep+2,"}\n");
    if(Flags.debug) {
      s.write(deep+2,"if(!strcmp(argv[i],\"-trace\")) {\n");
      s.write(deep+3,"trace=2;\n");
      s.write(deep+2,"}\n");
    }
    s.write(deep+1,"}\n");

    Symbol.genCreateFsym(s,deep+1);
    s.write( Symbol.genInitConstant(deep+1) );
    DDTree.genPatternListInit(s,deep+1);

    s.write(deep+1,"if(queryMode==1) {\n");
    s.write(deep+2,"yyparse();\n");
    s.write(deep+1,"}\n");

    s.write(deep+1,"EkerTermInit();\n");
    s.write(deep+1,"initTabRef();\n");
    s.write("\n");
    s.write(deep+1,"getrusage(RUSAGE_SELF, &before_self);\n");
    
    s.write(deep+1,"backTrackInit();\n");

    if(Flags.debug) {
      s.write(deep+1,"for(i=0 ; i<2 ; i++) {\n");
      s.write(deep+2,"for(j=0 ; j<" + Symbol.getMaxCode() + "; j++) {\n");
      s.write(deep+3,"tab_rewrite_step[i][j] = 0;\n");
      s.write(deep+2,"}\n");
      s.write(deep+1,"}\n");
      s.write("\n");
    }

    s.write(deep+1,"if (!setChoicePoint()) {\n");

    genComment(s,deep+2,"Input");
    s.write(deep+2,"res=termParser(queryMode,evaluationMode);\n");
    
    genComment(s,deep+2,"Output");
    s.write(deep+2,"switch(ResultMode) {\n");
    s.write(deep+3,"case 0:\n");
    s.write(deep+4,"break;\n");
    s.write(deep+3,"case 1:\n");
    s.write(deep+4,"term_printREFln(stdout,res);\n");
    s.write(deep+4,"break;\n");
    s.write(deep+3,"case 2:\n");
    s.write(deep+4,"printf(\"\\nresult = \");\n");
    s.write(deep+4,"termOut(stdout,term_unflatten(res));\n");
    s.write(deep+4,"printf(\"\\n\");\n");
    s.write(deep+4,"break;\n");
    s.write(deep+3,"case 3:\n");
    s.write(deep+4,"printf(\"\\nresult = \");\n");
    s.write(deep+4,"term_printnl(stdout,res);\n");
    s.write(deep+4,"break;\n");
    s.write(deep+2,"}\n");

    s.write(deep+2,"if(ResultMode!=1) {\n");
    s.write(deep+3,"backStatistics();\n");
    s.write(deep+3,"globalStatistics();\n");
    s.write(deep+2,"}\n");
    s.write(deep+2,"fail();\n");
    Tools.indent(s,deep+1); s.write("}\n");

    s.write("end:\n");
    s.write(deep+1,"getrusage(RUSAGE_SELF, &after_self);\n");
    //Tools.indent(s,deep+1); s.write("printf(\"\\nresult[%u] = \",rewrite_step);\n");
    //Tools.indent(s,deep+1); s.write("term_printnl(stdout,res);\n");
    s.write("destruction:\n");
    s.write( Symbol.genFreeFsym(deep+1) );
    s.write( Symbol.genFreeConstant(deep+1) );
    DDTree.genPatternListDelete(s,deep+1);
    s.write("#ifdef DEBUG\n");
    s.write(deep+1,"backStatistics();\n");
    s.write("#endif\n");

    s.write(deep+1,"if(ResultMode!=1) {\n");
    if(Flags.debug) {
      s.write(deep+2,"printf(\"                      \tfails\tsuccess\\n\");\n");
      s.write(deep+2,"for(j=0 ; j<" + Symbol.getMaxCode() + "; j++) {\n");
      s.write(deep+3,"if(tab_rewrite_step[0][j] > 0 || tab_rewrite_step[1][j] > 0)\n");
      s.write(deep+4,"printf(\"tab_rewrite_step[%d] :\t%u\t%u\\n\",j,tab_rewrite_step[0][j],tab_rewrite_step[1][j]);\n");
      s.write(deep+2,"}\n");
    }
    s.write(deep+2,"printf(\"\\nrewrite_step = %u\\n\",rewrite_step);\n");
    s.write(deep+2,"diff_sec  = after_self.ru_utime.tv_sec  - before_self.ru_utime.tv_sec;\n");
    s.write(deep+2,"diff_usec = after_self.ru_utime.tv_usec - before_self.ru_utime.tv_usec;\n");
    s.write(deep+2,"total_time= (double)diff_sec + (((double)diff_usec)/1000000.0);\n");


    s.write(deep+2,"if(!quietMode) {\n");
    s.write(deep+3,"printf(\"total time    = %.3f sec\\n\",total_time);\n");
    s.write(deep+3,"if(diff_sec > 0) {\n");
    s.write(deep+4,"printf(\"average speed = %d rwr/sec\\n\",(long)(((double)rewrite_step)/total_time));\n");
    s.write(deep+3,"} else if(diff_usec > 0) {\n");
    s.write(deep+4,"printf(\"average speed = %d rwr/sec\\n\",(long)(((double)1000000*rewrite_step)/((double)diff_usec)));\n");
    s.write(deep+3,"}\n");
    s.write(deep+2,"}\n");
    s.write(deep+1,"}\n");
        
    // Pour corriger un BUG sous Linux
    s.write(deep+1,"exit(0);\n");
    s.write("}\n");

    s.write("#include \"ac_tools.c\"\n");
    //s.write("#include \"" + fileName + ".core.c\"\n"); 
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
