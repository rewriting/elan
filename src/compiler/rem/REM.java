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
package rem;

import java.util.*;
import java.io.*;

import rem.compiler.*;
import rem.parser.*;
import rem.exception.*;

public class REM {

  private static void banner() {
    System.out.println("Reduce ELAN Machine v.4.3f");
    System.out.println("\t(c) LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)");
    System.out.println("\t    1998-2003");
    System.out.println("Authors: P.-E. Moreau and H. Kirchner");
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
    System.out.println("\t-fast");
    System.out.println("\t-lib");
    System.out.println("\t-coq");
    System.out.println("\t-proofterm");
    //    System.out.println("\t-noColor [default]");
    //    System.out.println("\t-color");
    //    System.out.println("\t-choicePointDebug");
    //    System.out.println("\t-oldVariableAffectation");
    System.out.println("\t-strategy <0|1|2>");
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
          } else if(args[i].equals("-fast")) {
            Flags.fast = true;
          } else if(args[i].equals("-warning")) {
            Flags.warnings = true;
          } else if(args[i].equals("-coq")) {
            Flags.coq = true;
          } else if(args[i].equals("-lib")) {
            Flags.lib = true;
          } else if(args[i].equals("-aterm") || args[i].equals("-atermns")) {
            // 2026: the ATerm runtime variant is not built any more
            System.err.println("REM: " + args[i] + ": the ATerm runtime is not supported (removed in this version)");
            System.exit(1);
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
        System.exit(1);  // 2026: errors give a non-zero exit status
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

      query = REFParser.Start(file_gram);
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
      // a failed build of the subdirectory fails the main Makefile (it
      // returned 0 until 2026: the loop ended with `cd ..`)
      mainMakefile.write("all clean veryclean:\n\t@for DIR in $(SUBDIRS); do echo Make $@ in $$DIR; (cd $$DIR && $(MAKE) $@) || exit 1; [ $@ != all ] || cp -f $$DIR/" + outputName + " . || exit 1; done\n\n");

      /*
       * Sous Makefile
       */
      subMakefile.write("ARCH := $(shell uname -s)\n");
      // The compilers and the GC of the ELAN installation (passed by elanc):
      // the program must be linked by the C++ compiler that built libearley.
      // ELAN_SANITIZE: the sanitizer options of the libraries (sanitizer
      // build only). Defaults only: the environment can override them.
      String[] makefileTools = { "ELAN_CC", "ELAN_CXX", "GC_PREFIX", "ELAN_SANITIZE" };
      for(int t = 0; t < makefileTools.length; t++) {
        String value = System.getProperty(makefileTools[t]);
        if(value != null && value.length() > 0) {
          subMakefile.write(makefileTools[t] + " ?= " + value + "\n");
        }
      }
      subMakefile.write("ifneq (,$(findstring CYGWIN,$(ARCH)))\n");
      subMakefile.write("CC = gcc -static -pipe\n");
      subMakefile.write("CXX = g++ -Wl,--stack,0x2000000\n");
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
      if(Flags.onlyC && !Flags.debug) {
        if(Flags.fast) {
          subMakefile.write("FAST = -O0 -march=i686\n");
        } else {
          subMakefile.write("FAST = -O2 -march=i686\n");
        }
      } else {
        subMakefile.write("FAST = \n");
      }

      subMakefile.write("YLIB = -lfl\n");
      subMakefile.write("DLLIB = \n");
      subMakefile.write("else\n");
      // Linux (2026): pre-standard C mode, no static link, no libtool
      subMakefile.write("ifneq ($(ARCH),Darwin)\n");
      subMakefile.write("ELAN_CC ?= gcc\n");
      subMakefile.write("ELAN_CXX ?= g++\n");
      subMakefile.write("GC_PREFIX ?= /usr\n");
      subMakefile.write("endif\n");
      subMakefile.write("CC = $(ELAN_CC) -pipe -std=gnu89 -w -fcommon -fsigned-char -I$(GC_PREFIX)/include $(ELAN_SANITIZE)\n");
      subMakefile.write("CXX = $(ELAN_CXX) -L$(GC_PREFIX)/lib $(ELAN_SANITIZE)\n");
      //subMakefile.write("CXX = g++ \n");
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
      } else if(Flags.fast) {
        subMakefile.write("FAST = -O0\n");
      } else {
        subMakefile.write("FAST = -O2\n");
      }
      subMakefile.write("YLIB = -lfl\n");
      subMakefile.write("DLLIB = -ldl\n");
      subMakefile.write("endif\n");
      // macOS (arm64): Homebrew GCC + Boehm GC; overridable from the environment
      subMakefile.write("ifeq ($(ARCH),Darwin)\n");
      subMakefile.write("ELAN_CC ?= gcc-16\n");
      subMakefile.write("ELAN_CXX ?= g++-16\n");
      subMakefile.write("GC_PREFIX ?= /opt/homebrew/opt/bdw-gc\n");
      subMakefile.write("CC = $(ELAN_CC) -pipe -std=gnu89 -w -fcommon -I$(GC_PREFIX)/include $(ELAN_SANITIZE)\n");
      subMakefile.write("CXX = $(ELAN_CXX) -L$(GC_PREFIX)/lib $(ELAN_SANITIZE)\n");
      subMakefile.write("YLIB = -ll\n");
      subMakefile.write("DLLIB = \n");
      subMakefile.write("endif\n");

      String prefix = System.getProperty("PREFIX");
      String elanlib = System.getProperty("ELANLIB");
      if(elanlib==null) {
        elanlib=prefix;
      }
      //System.out.println("prefix = " + prefix);
      //System.out.println("elanlib = " + elanlib);

      String include = elanlib + "/include";
      String quote = "\"";
      subMakefile.write("INC = " + "-I" +quote + include + quote + " -I" +
                        quote + include + "/elan-compiler" + quote + "\n"); 
      if(Flags.atermns) {
        subMakefile.write("ATERM_OPT = -DATERM -DNO_SHARING\n");
      } else if(Flags.aterm) {
        subMakefile.write("ATERM_OPT = -DATERM\n");
      } else {
        subMakefile.write("ATERM_OPT = -DBASETERM\n");
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
      if(Flags.coq) { //[QUANG: Sep 19 01] 
        subMakefile.write("LIBCOQ= -L$(ERP_SERVER)/src -lserver\n");
      } else {
        subMakefile.write("LIBCOQ=\n");
      }

      if(Flags.debug) {
        if(Flags.aterm) {
          subMakefile.write("LIB = $(LIBCOQ) -L" + elanlib + "/lib $(LIBELAN) -lruntime-aterm-dbg -learley $(YLIB) $(DLLIB) -lgc -lATerm-dbg\n");
        } else if(Flags.atermns) {
          subMakefile.write("LIB = $(LIBCOQ) -L" + elanlib + "/lib $(LIBELAN) -lruntime-aterm-dbg -learley $(YLIB) $(DLLIB) -lgc -lATerm-dbg\n");
        } else {
          subMakefile.write("LIB = $(LIBCOQ) -L" + elanlib + "/lib $(LIBELAN) -lruntime-base-dbg -learley $(YLIB) $(DLLIB) -lgc\n");
        }
      } else {
        if(Flags.aterm) {
          subMakefile.write("LIB = $(LIBCOQ) -L" + elanlib + "/lib $(LIBELAN) -lruntime-aterm -learley $(YLIB) $(DLLIB) -lgc -lATerm\n");
        } else if(Flags.atermns) {
          subMakefile.write("LIB = $(LIBCOQ) -L" + elanlib + "/lib $(LIBELAN) -lruntime-aterm -learley $(YLIB) $(DLLIB) -lgc -lATerm-ns\n");
        } else {
          subMakefile.write("LIB = $(LIBCOQ) -L" + elanlib + "/lib $(LIBELAN) -lruntime-base -learley $(YLIB) $(DLLIB) -lgc\n");
        }
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
        subMakefile.write(outputName + ": $(OBJ)\n\t$(CXX) $(CC_OPT) -o " +
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
          gmakeCommand = "gmake";
          //gmakeCommand = "/usr/bin/make";
          //System.out.println("arch = " + arch + " make = " + gmakeCommand);
          String execCompilePhase[] = { "/usr/bin/sh" , "-c" , gmakeCommand + " --file " + fileName + ".make all"} ;
          exec = r.exec(execCompilePhase);
          exec.waitFor();

          if(!Flags.quiet) {
            System.out.println("Cleaning generated files...");
          }
          String execCleanPhase[] = { "/usr/bin/sh" , "-c" , gmakeCommand + " --file " + fileName + ".make veryclean" } ;
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
      System.exit(1);
    } catch (Exception e1) {
      System.out.println("Reduce ELAN Code Machine. Encountered errors during interpretation/tree building.");
      e1.printStackTrace();
      System.exit(1);
    }
  }
}


