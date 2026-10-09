import java.util.*;
import java.io.*;

class REM {

  
  private static void banner() {
    System.out.println("Reduce ELAN Machine v.2.1");
    System.out.println("\t(c) LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2), 1998, 1999");
    System.out.println("Author: P.-E. Moreau");
  }
  
    private static void usage() {
    System.out.println("Usage:");
    System.out.println("\tjava [options] REM inputfile");
    System.out.println("Options:");
    System.out.println("\t-output <name>");
    System.out.println("\t-verbose");
    System.out.println("\t-nocode");
    System.out.println("\t-nosplit");
    System.out.println("\t-quiet");
    System.out.println("\t-debug");
    System.out.println("\t-noOptimiseChoicePoint [default]");
    System.out.println("\t-optimiseChoicePoint");
    System.out.println("\t-onlyC [for PC Linux only]");
//    System.out.println("\t-color [default]");
//    System.out.println("\t-noColor");
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

    if(args.length >= 1) {
      for(int i=0; i < args.length; i++) { 
	if(args[i].charAt(0) == '-') {
	  if(args[i].equals("-verbose")) {
	    Flags.verbose = true;
	  } else if(args[i].equals("-nocode")) {
	    Flags.code = false;
	  } else if(args[i].equals("-nosplit")) {
	    Flags.split = false;
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
	  } else if(args[i].equals("-withGoto")) {
	    Flags.withGoto = true;
	  } else if(args[i].equals("-output")) {
	    i++;
	    outputName = args[i];
	  } else if(args[i].equals("-strategy")) {
	    i++;
	    if(args[i].equals("0")) Flags.strat = 0; 
	    else if(args[i].equals("1")) Flags.strat = 1; 
	    else if(args[i].equals("2")) Flags.strat = 2;
	  } else {
	    System.out.println("!!! Undefined switch " + args[i] +"\n");
	  }
	} else {
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
      BufferedWriter main_h    = new BufferedWriter(new FileWriter(fileName+".h~"));
      BufferedWriter main_c    = new BufferedWriter(new FileWriter(fileName+".c~"));
      BufferedWriter main_make = new BufferedWriter(new FileWriter(fileName+".make"));
      OutputCode file_h = new OutputCode(main_h);
      OutputCode file_c = new OutputCode(main_c);
      OutputCode file_make = new OutputCode(main_make);
      OutputCode file_core_c = null;
      OutputCode file_gram = new OutputCode();
      Query query;

      Vector listOfName = new Vector();
      //listOfName.addElement(fileName);

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
//      file_gram.close();
      
      
      if(!Flags.quiet) {
	System.out.print("Compiling nonamed rules");
      }
      if(!Flags.split) {
	BufferedWriter main_core_c =
	  new BufferedWriter(new FileWriter(fileName+".core.c"));
	objectFile+=fileName+".core.o ";
	listOfName.addElement(fileName + ".core");
	file_core_c = new OutputCode(main_core_c);
	file_core_c.write("#include \"" + fileName + ".h\"\n");
      }

      Collection s = RewriteRule.getSortedMap().values();
      Iterator eRule = s.iterator();
        //Enumeration eRule  = RewriteRule.getHashtable().keys();
      while(eRule.hasNext()) {
          //Object key = eRule.nextElement();
	Vector vector = (Vector) eRule.next();
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
	      listOfName.addElement(completeFileName);

	      
	      BufferedWriter main_split_c = new BufferedWriter(new FileWriter(completeFileName+".c~"));
	      OutputCode file_split_c = new OutputCode(main_split_c);
	      file_split_c.write("#include \"" + fileName + ".h\"\n");
	      RewriteRule.compileNoNamed(file_split_c,vector);
	      file_split_c.close();

	      Tools.fileUpdate(completeFileName + ".c");

	      //Runnable t = new RunCompileNoNamed(completeFileName,fileName,vector);  
	      //new Thread(t).start();
	      

	      command = "${CC} ${CC_OPT} -c " + completeFileName + ".c" + "\n";
	      //file_make.write("echo " + command);
	      //file_make.write(command);
	    } else {
	      RewriteRule.compileNoNamed(file_core_c,vector);
	    }
	  } else {
            if(Flags.verbose) {
                //System.out.println("Skip : " + key);
            }
          }
        } catch (NoSuchElementException e) {
        }
      }
      if(!Flags.quiet) {
	System.out.println("");
      }

      if(!Flags.quiet) {
	System.out.print("Compiling strategies");
      }
      Enumeration eStrat = Strategy.getHashtable().keys();
      while(eStrat.hasMoreElements()) {
        Lexem key = (Lexem)eStrat.nextElement();
        Strategy strat = Strategy.getStrategy(key);
	if(!Flags.quiet) {
	  System.out.print(".");
	}

        if(Flags.verbose) {
  	  System.out.println("Compilation of strategy: " + strat);
        }
	if(Flags.split) {
	  String completeFileName = fileName + ".split.str" + strat.getName();
	  objectFile += completeFileName + ".o ";
	  listOfName.addElement(completeFileName);
	  
	  BufferedWriter main_split_c =
	    new BufferedWriter(new FileWriter(completeFileName + ".c~"));
	  OutputCode file_split_c = new OutputCode(main_split_c);
	  file_split_c.write("#include \"" + fileName + ".h\"\n");
	  strat.compile(file_split_c);
	  file_split_c.close();
	  
	  Tools.fileUpdate(completeFileName + ".c");

	  //Runnable t = new RunCompileStrategy(completeFileName,fileName,strat);  
	  //new Thread(t).start();
	     

	  command = "${CC} ${CC_OPT} -c " + completeFileName + ".c" + "\n";
	  //file_make.write("echo " + command);
	  //file_make.write(command);
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
      //file_make.write("echo " + command);
      //file_make.write(command);


      file_make.write("ARCH := $(shell uname -m)\n");

      file_make.write("ifeq '$(ARCH)' \"alpha\"\n");
      file_make.write("CC = cc\n");
      if(Flags.choicePointDebug) {
	file_make.write("#LIBELAN = -lDBelanmv -lexc\n");
	file_make.write("LIBELAN = -lDBelan\n");
      } else {
	file_make.write("#LIBELAN = -lelanmv -lexc\n");
	file_make.write("LIBELAN = -lelan\n"); 
      }
      if(Flags.debug) {
        file_make.write("FAST = -g3\n");
      } else {
        file_make.write("FAST = -fast\n");
      }
      
      file_make.write("YLIB = -ll -ly\n");
      file_make.write("else\n");
//      file_make.write("ifeq '$(ARCH)' \"i686\"\n");
      file_make.write("ifneq (,$(findstring 86,$(ARCH)))\n");
      file_make.write("CC = gcc -pipe\n");
      if(Flags.choicePointDebug) {
        if(Flags.onlyC) {
          file_make.write("LIBELAN = -lDBCsetChoicePoint\n");
        } else {
          file_make.write("LIBELAN = -lDBelan\n");
        }
      } else {
        if(Flags.onlyC) {
          file_make.write("LIBELAN = -lCsetChoicePoint\n");
        } else {
          file_make.write("LIBELAN = -lelan\n");
        }
      }
      if(Flags.onlyC) {
        file_make.write("FAST = -O2\n");
      } else {
        file_make.write("FAST = \n");
      }
      
      file_make.write("YLIB = -lfl\n");
      file_make.write("else\n");
      file_make.write("CC = gcc -pipe\n");
      if(Flags.choicePointDebug) {
	file_make.write("LIBELAN = -lDBelan\n");
      } else {
	file_make.write("LIBELAN = -lelan\n");
      }
      if(Flags.debug) {
        file_make.write("FAST = -g3\n");
      } else {
        file_make.write("FAST = -O2\n");
      }
      file_make.write("YLIB = -ll -ly\n");
      file_make.write("endif\n");
      file_make.write("endif\n");

      file_make.write("INC = -I$(ELANLIB)/Compiler/ -I$(ELANLIB)/Compiler/$(ARCH)\n");
      if(Flags.onlyC) {
        if(Flags.debug) {
          file_make.write("CC_OPT = -DCSETCHP -DDEBUG -DPDEBUG -DBITSET32 -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM -DCOLOR $(INC)\n");
        } else {
          file_make.write("CC_OPT = -DCSETCHP -DBITSET32 -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM -DCOLOR $(INC)\n");
        }
      } else {
        if(Flags.debug) {
          file_make.write("CC_OPT = -DDEBUG -DPDEBUG -DBITSET32 -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM -DCOLOR $(INC)\n");
        } else {
          file_make.write("CC_OPT = -DBITSET32 -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM -DCOLOR $(INC)\n");
        }
      }
      
      
      /*
      if(arch.endsWith("x86")) {
	  file_make.write("LIB = -L$(ELANLIB)/Compiler/$(ARCH) -L$(ELANLIB)/Compiled/$(ARCH) $(LIBELAN) -lRuntimeSupport -lfl -lgc\n");
      } else {
	  file_make.write("LIB = -L$(ELANLIB)/Compiler/$(ARCH) -L$(ELANLIB)/Compiled/$(ARCH) $(LIBELAN) -lRuntimeSupport -ll -ly -lgc\n");
      }
      */
      file_make.write("LIB = -L$(ELANLIB)/Compiler/$(ARCH) $(LIBELAN) -lRuntimeSupport -lACMatcher -lTermIO -learley $(YLIB) -lgc\n");

      String objList = "";
      String srcList = "";
      for(int i=0 ; i<listOfName.size() ; i++) {
	String objName = (String)listOfName.elementAt(i);
	objList += objName + ".o ";
	srcList += objName + ".c ";
      }
      //file_make.write("OBJ = " + objectFile +"\n");
      file_make.write("OBJ = " + fileName + ".o " + objList +"\n");
      
      file_make.write(".c.o:\n\t$(CC) $(FAST) $(CC_OPT) -c -o $*.o $<\n");
      file_make.write(outputName + ": $(OBJ)\n\t$(CC) $(CC_OPT) -o " + outputName + 
		      " $(OBJ) $(LIB)\n");

      for(int i=0 ; i<listOfName.size() ; i++) {
	String objName = (String)listOfName.elementAt(i);
	file_make.write(objName + ".o: " + fileName + ".h\n");
      }
      file_make.write(fileName + ".o: " + fileName + ".c " +
		      fileName + ".h\n\t" +
		      "$(CC) $(CC_OPT) -c " + fileName + ".c\n");
      file_make.write("clean:\n\t/bin/rm -f " +
		      fileName + ".o " + objList + "\n");
      file_make.write("veryclean: clean \n\t/bin/rm -f " +
		      fileName + ".c " + fileName + ".h " + srcList +
		      fileName + ".make " + fileName + ".ref " + "\n");

      file_c.write(file_gram.stringDump());
      file_c.close();
      file_h.close();
      file_make.close();
      if(!Flags.split) {
	file_core_c.close();
      }

      Tools.fileUpdate(fileName + ".h");
      Tools.fileUpdate(fileName + ".c");

      if(!Flags.code) {
	Runtime r = Runtime.getRuntime();
	Process exec = null;
	String gmakeCommand;
	String arch = System.getProperty("os.arch");
	// System.out.println("arch = " + arch);
          /*
	if(arch.endsWith("x86")) {
	  gmakeCommand = "/usr/bin/make";
	} else {
	  gmakeCommand = "gmake";
	}
          */
        gmakeCommand = "gmake";
	String execCompilePhase[] = { "sh" , "-c" , gmakeCommand + " ARCH=`uname -m` --file " + fileName + ".make " + outputName } ;
	String execCleanPhase[] = { "sh" , "-c" , gmakeCommand + " ARCH=`uname -m` --file " + fileName + ".make veryclean" } ;
	try {
	  if(!Flags.quiet) {
	    System.out.println("Building " + outputName + "...");
	  }
	  exec = r.exec(execCompilePhase);
	  //plugTogether(System.in,  exec.getOutputStream());
	  //plugTogether(System.out, exec.getInputStream());
	  //plugTogether(System.err, exec.getErrorStream());
          exec.waitFor();
	  if(!Flags.quiet) {
	    System.out.println("Cleaning generated files...");
	  }
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


