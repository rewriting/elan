package rem.compiler;

import java.util.*;

/*
 * A refaire serieusement en utilisant des concepts objets
 * PAS de static+case : utiliser la liaison dynamique
 */
public class StrategyEval {

  static int label=1;

  public static boolean isCompilable(Term t) {
    Symbol symb = t.getSymbol();
    if (symb instanceof SymbolApply) {
      Term str = t.getSubterm(0);
      Term trm = t.getSubterm(1);
      return (!str.isVariable()); }
    else {
      return false;
    }
  }

  public static void genEval(OutputCode s,int deep,
			       int exitLab, String dest, 
			       Term str, String src) {
    Symbol symb = str.getSymbol();
    
    if (Flags.verbose) {
      System.out.println("strategy term to compile " + dest + ":=" +
			 "[" + str.toString() + "]" + src + "*/\n");
    }

    if (symb instanceof SymbolBuiltin) {
      int sem = str.getSemantic();
      if (sem != 0) {
	genEvalSem(s,deep, exitLab, dest, str, sem, src);
      } else if (symb instanceof SymbolFsym) {
	genEvalFsym(s,deep, exitLab, dest, str, 
		    ((SymbolFsym)symb).getFsym1(),
		    ((SymbolFsym)symb).getFsym2(), src);
      } else if (symb instanceof SymbolDstr) {
	genEvalDstr(s,deep, exitLab, dest, str,
		    symb.getSymbolCode(), 
		    ((SymbolDstr)symb).getApplySymbol(),
			     src);
      } else { // if (symb instanceof SymbolLab) {
//	genEvalLab(s,deep, exitLab, dest, str.genCore(), symb.getSymbolCode(), src); //0206

	genEvalLab(s,deep, exitLab, dest, str, 
	symb.getSymbolCode(),((SymbolLab)symb).getApplyCode(),src);	
      } 
    } else {
      s.write("/* genEval - non-built-in strategy term to compile " + dest + ":=" + 
	      "[" + str.toString() + "]" + src + "*/\n");
      if (Flags.debug) {
	System.out.println("Eval non-built-in to compile = " + str.toString());
      }
      str.genTermConstruction(s,deep);
      s.write(dest + " = str_eval2(" + str.genCore() + "," + src + ");\n");
    }
  }

  public static void genEvalSem(OutputCode s,int deep,
				int exitLab, String dest, Term str, 
				int sem, String src) {
    boolean is_det = true;
    Tools.indent(s,deep); 
    s.write("/* gen.Eval.Sem - strategy term to compile " + dest + ":=" + 
	    "[" + str.toString() + "]" + src + "*/\n");
    
    if (Flags.debug) {
      System.out.println("EvalSem to compile = " + str.toString() +
	  	         " sem = " + sem);
    }
    
    Tools.indent(s,deep); 
    switch (-sem) {
    case ElanConstants.DS_CONC:
      Tools.indent(s,deep+1); s.write("{ struct term *res" + deep + ";\n");
      genEval(s,deep+1, 0, "res" + deep , 
	      str.getSubterm(0),src); 
      genEval(s,deep+1, 0, dest, str.getSubterm(1), "res" + deep); 
      Tools.indent(s,deep+1); s.write("}\n");
      break;
    case ElanConstants.DS_DK:
      is_det = false;  // continue
    case ElanConstants.DS_DC:                // the same as in StrategyDCDKStrat.java
      Term strlist = str.getSubterm(0);
      int lab = StrategyEval.label++; //StrategyTerm.strategyLabel++;
      
      Tools.indent(s,deep); s.write("{ \n"); 
      if (is_det) {
	Tools.indent(s,deep+1); s.write("int *wasr=(int*) allocStable(sizeof(int));\n");
	Tools.indent(s,deep+1); s.write("*wasr=0;\n"); }
      
      while (strlist.getSemantic() != -ElanConstants.DS_EPSILON) {
        boolean is_last = (
			   (strlist.getSubterm(1)).getSemantic()== -ElanConstants.DS_EPSILON);
	  
	if(// ???? !Flags.optimiseChoicePoint || 
		!is_last) {
	  Tools.indent(s,deep); s.write("if (!setChoicePoint()) {\n"); 
	  deep++;
	}
	
	genEval(s,deep+1, 0, dest, strlist.getSubterm(0),src);  
	
	if (!is_last && is_det) {
	  Tools.indent(s,deep+1); s.write("if(*wasr==0) *wasr=1;\n"); }

	Tools.indent(s,deep); s.write("goto stratEvalLab" + lab +";\n");
	//	 ???? Tools.indent(s,deep); s.write("goto stratLab"+exitLab +";\n");

	if (// ???? !Flags.optimiseChoicePoint || 
		!is_last) {
	  deep--;
	  Tools.indent(s,deep+1); s.write("} \n");
	  if (is_det) {
	    Tools.indent(s,deep+1); s.write("if(*wasr!=0) { \n"); 
	    Tools.genFail(s,deep+2);
	    Tools.indent(s,deep+1); s.write("} \n");              
	  }
	}
	strlist = strlist.getSubterm(1);
      }
      Tools.indent(s,deep); s.write("stratEvalLab" + lab + ":;\n");
      Tools.indent(s,deep); s.write("}  \n"); 
      break;
    case ElanConstants.DS_ID:
      s.write(dest + " = " + src + ";\n");
      break;
    case ElanConstants.DS_IFTE:
	Tools.indent(s,deep); s.write("if ("+
				str.getSubterm(0).genCore()+
				"->symb) {\n");
	Tools.indent(s,deep+1);
	genEval(s,deep+1, 0, dest, str.getSubterm(1),src);  

	Tools.indent(s,deep); s.write("} else {\n");
	Tools.indent(s,deep+1);
	genEval(s,deep+1, 0, dest, str.getSubterm(2),src);  

	Tools.indent(s,deep); s.write("}\n");
	break;
    case ElanConstants.DS_IFTOE:
        int iflab = StrategyEval.label++; //StrategyTerm.strategyLabel++;

	Tools.indent(s,deep); s.write("{\n");
	Tools.indent(s,deep+1); s.write("int *wasr=(int*) allocStable(sizeof(int));\n");
	Tools.indent(s,deep+1); s.write("struct term *res" + deep + ";\n");
	Tools.indent(s,deep+1); s.write("*wasr=0;\n"); 
	Tools.indent(s,deep+1); s.write("if (!setChoicePoint()) {\n"); 

	genEval(s,deep+2, 0, "res" + deep, str.getSubterm(0),src);  
	Tools.indent(s,deep+2); s.write("if(*wasr==0) *wasr=1;\n"); 
	genEval(s,deep+2, 0, dest, str.getSubterm(1),"res" + deep); 
 	Tools.indent(s,deep+2); s.write("goto stratEvalLab" + iflab +";\n");
	Tools.indent(s,deep+1); s.write("}\n");

	Tools.indent(s,deep+1); s.write("if(*wasr!=0) { \n"); 
	Tools.genFail(s,deep+2);
	Tools.indent(s,deep+1); s.write("} \n");   

 	genEval(s,deep+1, 0, dest, str.getSubterm(2),src);
        Tools.indent(s,deep+1); s.write("stratEvalLab" + iflab + ":;\n");
        Tools.indent(s,deep); s.write("}  \n"); 
	break;
    case ElanConstants.DS_FAIL:
      s.write("fail();\n");
      break;
    default:
	Tools.indent(s,deep); s.write("/* unknown semantic symbol */\n");
        str.genTermConstruction(s,deep);
        s.write(dest + " = str_eval2(" + str.genCore() + "," + src + ");\n");
    }
  }

    public static void genEvalFsym(OutputCode s,int deep,
				   int exitLab, String dest, Term str, 
				   int f1, int f2, String src) {
    int arty = (str.getSymbol()).getArity();
    Tools.indent(s,deep); 
    s.write("/* genEvalFsym - strategy term to compile " + dest + ":=" + 
	     "[" + str.toString() + "]" + src + "*/\n");

    if (Flags.debug) {
      System.out.println("EvalFsym to compile = " + str.toString());
    }

    Tools.indent(s,deep+1); s.write("if (" + src + "->symb == " + f1 + ") { /*3*/\n");

    if (arty == 0 && f1 == f2) {
      Tools.indent(s,deep+2);  s.write(dest + " = " + src + ";\n"); }
    else { 
      Tools.indent(s,deep+2); 
        s.write("struct term *res_" + deep + ";\n");
      for(int i=0; i<arty; i++) {
	Tools.indent(s,deep+2); 
	  s.write("struct term *sub_" + deep + "_" + i + " = " + 
		   src + "->sub[" + i + "];\n");
      }
      Tools.indent(s,deep+2); 
      s.write("TERM_ARITY_ALLOC(res_" + deep + ", " + arty + 
	       ", " + f2 +");\n");
      for(int i=0; i<arty; i++) {
	genEval(s,deep+2, exitLab, "res_" + deep + "->sub[" + i + "]",
		str.getSubterm(i), "sub_" + deep + "_" + i);
      }
      Tools.indent(s,deep+1); s.write(dest + " = res_" + deep + ";");
    }
    Tools.indent(s,deep+1); s.write("} /*3*/ else {  /*4*/\n");
    Tools.genFail(s,deep+2);
    Tools.indent(s,deep+1); s.write("} /*4*/\n");
  }

    public static void genEvalLab(OutputCode s,int deep,
				  int exitLab, String dest, Term str, 
				  String symbCode, 
				  int applyCode,
				  String src) {
    if (Flags.debug) {
      System.out.println("EvalLab to compile = " + str.toString());
    }

    Tools.indent(s,deep); 
    s.write("/* genEvalLab - strategy term to compile " + dest + ":=" + 
	     "[" + str.toString() + "]" + src + "*/\n");

//0206 --- from here
    Tools.indent(s,deep); s.write("{ /*6*/ struct term *res" + deep + ";\n");

    str.genTermConstruction(s,deep);

    Tools.indent(s,deep+1); s.write("TERM_ALLOC(res"+ deep + ", term2, " +
				   applyCode + ");\n");

    Tools.indent(s,deep+1); s.write("res" + deep + "->sub[0] = " +
				   str.genCore() + ";\n");
    Tools.indent(s,deep+1); s.write("res" + deep + "->sub[1] = " +
				   src + ";\n");
    Tools.indent(s,deep+1); s.write(dest + " = str_rule" + symbCode + "(res" + deep + ");\n");
    Tools.indent(s,deep+1); s.write("} /*6*/");
//0206 ---- up to here

//0206   Tools.indent(s,deep); s.write(dest + " = str_rule" + symbCode + "(" + src + ");\n");
  }

  public static void genEvalDstr(OutputCode s,int deep,
				 int exitLab, String dest, Term str, 
				 String symbCode, int applyCode,
				 String src) {
    if (Flags.debug) { 
      System.out.println("EvalDstr to compile = " + str.toString());
    }

    Tools.indent(s,deep); 
    s.write("/* genEvalDstr - strategy term to compile " + dest + ":=" + 
	     "[" + str.toString() + "]" + src + "*/\n");

    Tools.indent(s,deep); s.write("{ /*5*/ struct term *res" + deep + ";\n");

    str.genTermConstruction(s,deep);

    Tools.indent(s,deep+1); s.write("TERM_ALLOC(res"+ deep + ", term2, " +
				   applyCode + ");\n");

    Tools.indent(s,deep+1); s.write("res" + deep + "->sub[0] = " +
				   str.genCore() + ";\n");
    Tools.indent(s,deep+1); s.write("res" + deep + "->sub[1] = " +
				   src + ";\n");
    Tools.indent(s,deep+1);
    s.write(dest + " = str_dstr" + symbCode + "(res" + deep + ");\n");

    Tools.indent(s,deep+1); s.write("} /*5*/");

  }


  }
