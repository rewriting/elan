package rem;

import java.util.*;
import java.io.*;

import rem.compiler.*;
import rem.parser.*;
import rem.exception.*;

public class REM {

  private static void banner() {
    System.out.println("Reduce ELAN Machine v.3.2 (#4.1)");
    System.out.println("\t(c) LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)");
    System.out.println("\t    1998, 1999, 2000, 2001");
    System.out.println("Author: P.-E. Moreau");
  }

  private static void usage() {
    banner();
    System.out.println("Usage:");
    System.out.println("\tjava [options] REM inputfile");
    System.out.println("Options:");
    System.out.println("\t-output <name>");
    System.out.println("\t-verbose	");
    System.out.println("\t-nocode");
    System.out.println("\t-nosplit [default]");
    System.out.println("\t-split");
    System.out.println("\t-quiet");
    System.out.println("\t-debug");
    System.out.println("\t-noOptimiseChoicePoint [default]");
    System.out.println("\t-optimiseChoicePoint");
    System.out.println("\t-onlyC");
    System.out.println("\t-noC");
    System.out.println("\t-warning");
    System.out.println("\t-O");
    System.out.println("\t-O2");
    System.out.println("\t-lib");
    System.out.println("\t-aterm");
    System.out.println("\t-coq");
    System.out.println("\t-proofterm");
//    System.out.println("\t-noColor [default]");
//    System.out.println("\t-color");
//    System.out.println("\t-choicePointDebug");
//    System.out.println("\t-oldVariableAffectation");
//    System.out.println("\t-strategy <0|1|2>");
//    System.out.println("\t-withGoto");
  }

  public static void main(String args[]) {
    REFParser parser;
    String fileName = "";
    String fullFileName = "";
    String outputName = "a.out";
    Iterator it;

    if(args.length >= 1) {
      for(int i=0; i < args.length; i++) {
	if(args[i].charAt(0) == '-') {
	  if(args[i].equals("-verbose")) {
	    Flags.verbose = true;
	  } else if(args[i].equals("-nocode")) {
	    Flags.code = false;
	  } else if(args[i].equals("-nosplit")) {
	    Flags.split = false;
	  } else if(args[i].equals("-split")) {
	    Flags.split = true;
	  } else if(args[i].equals("-quiet")) {
	    Flags.quiet = true;
	  } else if(args[i].equals("-debug")) {
	    Flags.debug = true;
	  } else if(args[i].equals("-color")) {
	    Flags.color = true;
	  } else if(args[i].equals("-noColor")) {
	    Flags.color = false;
	  } else if(args[i].equals("-choicePointDebug")) {
	    Flags.choicePointDebug = true;
	  } else if(args[i].equals("-noOptimiseChoicePoint")) {
	    Flags.optimiseChoicePoint = false;
	  } else if(args[i].equals("-optimiseChoicePoint")) {
	    Flags.optimiseChoicePoint = true;
	  } else if(args[i].equals("-oldVariableAffectation")) {
	    Flags.newVariableAffectation = false;
	  } else if(args[i].equals("-onlyC")) {
	    Flags.onlyC = true;
	  } else if(args[i].equals("-noC")) {
	    Flags.onlyC = false;
	  } else if(args[i].equals("-withGoto")) {
	    Flags.withGoto = true;
	  } else if(args[i].equals("-output")) {
	    i++;
	    outputName = args[i];
          } else if(args[i].equals("-O")) {
	    Flags.optimiseChoicePoint = true;
          } else if(args[i].equals("-O2")) {
	    Flags.optimiseChoicePoint = true;
	    Flags.rwrCounter = false;
          } else if(args[i].equals("-warning")) {
	    Flags.warnings = true;
          } else if(args[i].equals("-coq")) {
	    Flags.coq = true;
          } else if(args[i].equals("-lib")) {
            Flags.lib = true;
          } else if(args[i].equals("-aterm")) {
	    Flags.aterm = true;
	  } else if(args[i].equals("-proofterm")) {// [NGUYEN: Apr  6 01] to trsnform named rules
	    Flags.proofterm = true;
	  } else if(args[i].equals("-strategy")) {
	    i++;
	    if(args[i].equals("0")) Flags.strat = 0;
	    else if(args[i].equals("1")) Flags.strat = 1;
	    else if(args[i].equals("2")) Flags.strat = 2;
	  } else {
	    System.out.println("!!! Undefined switch " + args[i] +"\n");
	  }
        }  else {
	  if( args[i].endsWith(".ref") ) {
	    fileName = args[i].substring(0,args[i].length()-(".ref".length()));
	    fullFileName = args[i];
	  } else {
	    fileName = args[i];
	    fullFileName = args[i] + ".ref";
	  }
        }
      }

      if(!Flags.quiet) {
        banner();
        System.out.println("Reduce ELAN Machine. Reading from file " + fullFileName + " . . .");
      }
      try {
	parser = new REFParser(new java.io.FileInputStream(fullFileName));
      } catch (java.io.FileNotFoundException e) {
        System.out.println("Reduce ELAN Code Machine. File " + fullFileName + " not found.");
        return;
      }
    } else {
      usage();
      return;
    }

    try {
      Tools tools = new Tools();

        // Creation du sous-repertoire
      String subdirName = ".elan." + fileName + "/" ;
      File subdir = new File(subdirName);
      subdir.mkdir();
        // Creation des fichiers et des makefiles
      OutputCode file_h = new OutputCode(
        new BufferedWriter(new FileWriter(subdirName + fileName + ".h~")));
      OutputCode file_c = new OutputCode(
        new BufferedWriter(new FileWriter(subdirName + fileName + ".c~")));
      OutputCode mainMakefile = new OutputCode(
        new BufferedWriter(new FileWriter(fileName + ".make")));
      OutputCode subMakefile = new OutputCode(
        new BufferedWriter(new FileWriter(subdirName + "Makefile")));


      OutputCode file_core_c = null;
        // fichier utilise pour les tables d'Earley
      OutputCode file_gram = new OutputCode();
      Query query;

        // list des noms de fichiers generes
      Collection listOfName = new ArrayList();

      String objectFile = fileName + ".o ";
      String command;


      if(!Flags.quiet) {
	System.out.print("Parsing");
	if(Flags.debug) {
	  System.out.println();
	}
      }

      query = parser.Start(file_gram);
      parser=null; /* liberation de la memoire du parser */

        /*
         * Generation des tables pour Earley
         */
      file_gram.write("int earleyQuerySort = " + query.getSourceSort() + ";\n");
      file_gram.write("int earleyQueryStrategy = " + query.getSourceStrategy() + ";\n");
      LexemIdentifier.genCode(file_gram);
      LexemSort.genCode(file_gram);
      LexemStrategyName.genCode(file_gram);

        /*
         * Compilation des regles non nommees
         */
      if(!Flags.quiet) {
	System.out.print("\nCompiling nonamed rules");
      }
      if(!Flags.split) {
	BufferedWriter main_core_c =
	  new BufferedWriter(
            new FileWriter(subdirName + fileName+".core.c"));
	objectFile+=fileName+".core.o ";
	listOfName.add(fileName + ".core");
	file_core_c = new OutputCode(main_core_c);
	file_core_c.write("#include \"" + fileName + ".h\"\n");
      }

      it = RewriteRule.valuesIterator();
      while(it.hasNext()) {
        Vector vector = (Vector)it.next();
        try {
	  RewriteRule rule = (RewriteRule) vector.firstElement();
	  if(!rule.isNamed()) {
	    if(!Flags.quiet) {
	      System.out.print(".");
	    }

	    if(Flags.verbose) {
	      System.out.println("Compilation of nonamed rules: " + rule);
	    }
	    if(Flags.split) {
	      Symbol headSymbol = rule.getLeftside().getSymbol();
	      String completeFileName = fileName + ".split.fun_" +
		headSymbol.getSymbolCode();
	      objectFile += completeFileName + ".o ";
	      listOfName.add(completeFileName);

	      OutputCode file_split_c = new OutputCode(
                new BufferedWriter(new FileWriter(subdirName + completeFileName + ".c~")));
              file_split_c.write("#include \"" + fileName + ".h\"\n");
	      RewriteRule.compileNoNamed(file_split_c,vector);
	      file_split_c.close();
	      Tools.fileUpdate(subdirName + completeFileName + ".c");
	      command = "${CC} ${CC_OPT} -c " + completeFileName + ".c" + "\n";
	    } else {
	      RewriteRule.compileNoNamed(file_core_c,vector);
	    }
	  }
        } catch (NoSuchElementException e) {
        }
      }

      if(!Flags.quiet) {
	System.out.print("\nCompiling strategies");
      }

      it = Strategy.valuesIterator();
      while(it.hasNext()) {
        Strategy strat = (Strategy)it.next();

	if(!Flags.quiet) {
	  System.out.print(".");
	}

        if(Flags.verbose) {
  	  System.out.println("Compilation of strategy: " + strat);
        }
	if(Flags.split) {
	  String completeFileName = fileName + ".split.str" + strat.getName();
	  objectFile += completeFileName + ".o ";
	  listOfName.add(completeFileName);
	  OutputCode file_split_c = new OutputCode(
	    new BufferedWriter(new FileWriter(subdirName + completeFileName + ".c~")));
	  file_split_c.write("#include \"" + fileName + ".h\"\n");
	  strat.compile(file_split_c);
	  file_split_c.close();
	  Tools.fileUpdate(subdirName + completeFileName + ".c");
	  command = "${CC} ${CC_OPT} -c " + completeFileName + ".c" + "\n";
	} else {
	  strat.compile(file_core_c);
	}
      }
      if(!Flags.quiet) {
	System.out.println();
      }

      tools.genHeader(file_h);
      tools.genMainFile(file_c,fileName,query);
      command = "${CC} ${CC_OPT} " + objectFile + "${LIB}\n";

        /*
         * Main Makefile
         */
      mainMakefile.write("SUBDIRS = " + subdirName + "\n\n");
      mainMakefile.write("all clean veryclean:\n\t@for DIR in $(SUBDIRS); do echo Make $@ in $$DIR; cd $$DIR && $(MAKE) $@; cp -f " + outputName + " ..; cd ..; done\n\n");

        /*
         * Sous Makefile
         */
      subMakefile.write("ARCH := $(shell uname -m)\n");

      subMakefile.write("ifeq '$(ARCH)' \"alpha\"\n");
      subMakefile.write("CC = cc\n");
      if(Flags.choicePointDebug) {
	subMakefile.write("#LIBELAN = -lDBelanmv -lexc\n");
	subMakefile.write("LIBELAN = -lDBelan\n");
      } else {
	subMakefile.write("#LIBELAN = -lelanmv -lexc\n");
	subMakefile.write("LIBELAN = -lelan\n");
      }
      if(Flags.debug) {
        subMakefile.write("FAST = -g3\n");
      } else {
        subMakefile.write("FAST = -fast\n");
      }

      subMakefile.write("YLIB = -ll -ly\n");
      subMakefile.write("else\n");
      subMakefile.write("ifneq (,$(findstring 86,$(ARCH)))\n");
      subMakefile.write("CC = gcc -pipe\n");
      if(Flags.choicePointDebug) {
        if(Flags.onlyC) {
          subMakefile.write("LIBELAN = -lDBCsetChoicePoint\n");
        } else {
          subMakefile.write("LIBELAN = -lDBelan\n");
        }
      } else {
        if(Flags.onlyC) {
          subMakefile.write("LIBELAN = -lchoice\n");
        } else {
          subMakefile.write("LIBELAN = -lelan\n");
        }
      }
      if(Flags.onlyC && !Flags.debug) {
        subMakefile.write("FAST = -O3 -march=i686\n");
      } else {
        subMakefile.write("FAST = \n");
      }

      subMakefile.write("YLIB = -lfl\n");
      subMakefile.write("else\n");
      subMakefile.write("CC = gcc -pipe\n");
      if(Flags.choicePointDebug) {
        if(Flags.onlyC) {
          subMakefile.write("LIBELAN = -lchoice-dbg\n");
        } else {
          subMakefile.write("LIBELAN = -lDBelan\n");
        }
      } else {
        if(Flags.onlyC) {
          subMakefile.write("LIBELAN = -lchoice\n");
        } else {
          subMakefile.write("LIBELAN = -lelan\n");
        }
      }
      if(Flags.debug) {
        subMakefile.write("FAST = -g3\n");
      } else {
        subMakefile.write("FAST = -O2\n");
      }
      subMakefile.write("YLIB = -ll -ly\n");
      subMakefile.write("endif\n");
      subMakefile.write("endif\n");

      subMakefile.write("INC = -I$(ELANLIB)/Compiler/ -I$(ELANLIB)/Compiler/$(ARCH)\n");

      if(Flags.aterm == false) {
        subMakefile.write("ATERM_OPT = -DBASETERM\n");
      } else {
        subMakefile.write("ATERM_OPT = -DATERM\n");
      }
      subMakefile.write("BASIC_OPT = -DCOMPUTE_HCODE -DHASHCODE -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM -DCOLOR $(ATERM_OPT)\n");

      if(Flags.onlyC) {
        if(Flags.debug) {
          subMakefile.write("CC_OPT = -DCSETCHP -DDEBUG -DPDEBUG -DBITSET32 $(BASIC_OPT) $(INC)\n");
        } else {
          subMakefile.write("CC_OPT = -DCSETCHP -DBITSET32 $(BASIC_OPT) $(INC)\n");
        }
      } else {
        if(Flags.debug) {
          subMakefile.write("CC_OPT = -DDEBUG -DPDEBUG -DBITSET32 $(BASIC_OPT) $(INC)\n");
        } else {
          subMakefile.write("CC_OPT = -DBITSET32 $(BASIC_OPT) $(INC)\n");
        }
      }
      if(Flags.coq) { //[Huy: Oct 18 00]
	  subMakefile.write("LIBCOQ= -L../ -lserver\n");
      } else {
	  subMakefile.write("LIBCOQ=\n");
      }

      if(Flags.aterm == false) {
        subMakefile.write("LIB = $(LIBCOQ) -L$(ELANLIB)/Compiler/$(ARCH) $(LIBELAN) -lTermIO -lRuntimeSupport -lACMatcher -learley $(YLIB) -lgc\n");
      } else {
        subMakefile.write("LIB = $(LIBCOQ) -L$(ELANLIB)/Compiler/$(ARCH) $(LIBELAN) -lTermIOATerm -lRuntimeSupportATerm -lACMatcher -learley $(YLIB) -lgc -lATerm\n");
      }
      String objList = "";
      String srcList = "";
      it = listOfName.iterator();
      while(it.hasNext()) {
	String objName = (String)it.next();
	objList += objName + ".o ";
	srcList += objName + ".c ";
      }
      subMakefile.write("OBJ = " + fileName + ".o " + objList +"\n");

      subMakefile.write(".c.o:\n\t$(CC) $(FAST) $(CC_OPT) -c -o $*.o $<\n");
      subMakefile.write("all: " + outputName + "\n\n");
      if(Flags.lib) {
        subMakefile.write(outputName + ": $(OBJ)\n\tar r " +
                          outputName + " $(OBJ)\n\tranlib " + outputName + "\n");
      } else {
        subMakefile.write(outputName + ": $(OBJ)\n\t$(CC) $(CC_OPT) -o " +
                          outputName + " $(OBJ) $(LIB)\n");
      }

      it = listOfName.iterator();
      while(it.hasNext()) {
	String objName = (String)it.next();
	subMakefile.write(objName + ".o: " + fileName + ".h\n");
      }
      subMakefile.write(fileName + ".o: " +
                        fileName + ".c " + fileName + ".h\n\t" +
                        "$(CC) $(CC_OPT) -c " + fileName + ".c\n");
      subMakefile.write("clean:\n\t/bin/rm -f " + outputName + " " +
                        fileName + ".o " + objList + "\n");
      subMakefile.write("veryclean: clean \n\t/bin/rm -f " +
                        fileName + ".c " + fileName + ".h " + srcList +
                        "../" + fileName + ".make " +
                        "../" + fileName + ".ref " + "\n");

      file_c.write(file_gram.stringDump());
      file_c.close();
      file_h.close();
      mainMakefile.close();
      subMakefile.close();
      if(!Flags.split) {
	file_core_c.close();
      }

      Tools.fileUpdate(subdirName + fileName + ".h");
      Tools.fileUpdate(subdirName + fileName + ".c");

      if(!Flags.code) {
	Runtime r = Runtime.getRuntime();
	Process exec = null;
	String gmakeCommand;
	String arch = System.getProperty("os.arch");

        try {
	  if(!Flags.quiet) {
	    System.out.println("Building " + outputName + "...");
	  }
          gmakeCommand = "make";
            //gmakeCommand = "/usr/bin/make";
            //System.out.println("arch = " + arch + " make = " + gmakeCommand);
          String execCompilePhase[] = { "/bin/sh" , "-c" , gmakeCommand + " --file " + fileName + ".make all"} ;
	  exec = r.exec(execCompilePhase);
          exec.waitFor();

          if(!Flags.quiet) {
	    System.out.println("Cleaning generated files...");
	  }
          String execCleanPhase[] = { "sh" , "-c" , gmakeCommand + " --file " + fileName + ".make veryclean" } ;
          exec = r.exec(execCleanPhase);
          exec.waitFor();
	} catch (Exception e){
	  System.out.println("Error in command execution!");
	}
      } else {
	System.out.println("execute the following line to build your program");
	System.out.println("\tgmake --file " + fileName + ".make");
      }
    } catch (ParseException e) {
      System.out.println("Reduce ELAN Code Machine. Encountered errors during parse.");
      e.printStackTrace();
    } catch (Exception e1) {
      System.out.println("Reduce ELAN Code Machine. Encountered errors during interpretation/tree building.");
      e1.printStackTrace();
    }
  }
}


