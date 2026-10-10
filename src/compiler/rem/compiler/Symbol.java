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

public abstract class Symbol implements Comparable {
  private int arity=0;
  private Lexem sort;

  /* "sym"+code <---> Symbol  */
  private static Map map = new HashMap(40);
  private boolean isConstructor=true;

  public Symbol(Lexem sort) {
    super();
    this.sort = sort;
  }


  public abstract int getCode();
  public abstract String getSymbolCode();
  public abstract Object getSymbolValue();

  /**
   * Construction du terme de depart
   */
  public abstract void genMatchFail(OutputCode s,int deep);
  public abstract void genFunctionHeader(OutputCode s,int deep, DDTree tree,
				  Vector rules,
				  String prefix, 
				  boolean isFunctionTree);

  public abstract String genFsymInit(int deep);
  public abstract String genInstance();
  public abstract String toString(Vector subterms);
  public abstract int cmp(Symbol sym);
  public abstract int cmpRename(Symbol sym);

//abstract boolean equals(Symbol sym);
  public boolean equals(Symbol sym) {
    if(this==sym) {
      return true;
    }
    if(getArity()==sym.getArity() && getSort()==sym.getSort()) {
      return true;
    }
    return false;
  }

  protected LexemSort getSort() {
    return (LexemSort)sort;
  }
  public boolean isBuiltinSort() {
    return getSort().isBuiltin();
  }

  public String genAccess() {
    return getSort().genAccess();
  }

  public boolean isFree() {
    return this instanceof SymbolFree;
  }
  public boolean isVariable() {
    return this instanceof SymbolVariable || isExtensionVariable();
  }
  public boolean isExtensionVariable() {
    return this instanceof SymbolExtensionVariable;
  }
  public boolean isAC() {
    return this instanceof SymbolAC;
  }
  public boolean isBuiltin() {
    return this instanceof SymbolBuiltin;
  }
  public boolean isValue() {
    return this instanceof SymbolValue;
  }

  public void clearConstructor() {
    isConstructor=false;
  }

  public boolean isConstructor() {
//    if(isBuiltin() && getArity()>0) {
    if(isBuiltin() && ((SymbolBuiltin)this).getSymbolSemantic() != 0) {
      return false;
    } else {
      return isConstructor;
    }
  }

  public boolean isConstant() {
    return (!isVariable() && getArity()==0);
  }

  public boolean isFunction() {
    return isFree() || isAC() || isBuiltin() || isConstant();
  }

  protected void setArity(int v) {
    arity = v;
  }

  public int getArity() {
    //System.out.println("symbol " + this + " isAC = " + isAC() + " arity = " + arity);
    return arity;
  }

  /*
   * retourne 0 pour les symboles AC
   */
  public int getACArity() {
    if(isAC()) {
      return 0;
    } else {
      return getArity();
    }
  }

  public int getStartVarNumber() {
    if(isAC()) {
      return 1;
    } else {
      return getArity()+1;
    }
  }


  /**
   * Static functions
   */
  public static int getMaxCode() {
    int max=0;
    Iterator it = map.values().iterator();
    while(it.hasNext()) {
      Symbol sym = (Symbol)it.next();
      if(sym.getCode() > max)
	max=sym.getCode();
    }
    return max;
  }

  public static Symbol get(int code) {
    return (Symbol) map.get("sym" + code);
  }

  public void put() {
    Symbol oldSym = (Symbol)map.put("sym" + getSymbolCode(),this);
    if( oldSym != null ) {
      if (Flags.warnings) {
        System.out.println("Warning: '" + oldSym + "' and '" + this + "' have the same code");
      }
    }
  }

  public static void dump() {
    Iterator it = map.entrySet().iterator();
    while(it.hasNext()) {
      String key = (String) it.next();
      Symbol sym = (Symbol) map.get(key);
      if (Flags.verbose) {
        System.out.println("<" + key + ":" + sym + ">");
      }
    }
  }

  public static String genStructure(int deep) {
    StringBuffer s = new StringBuffer();
    StringBuffer struct = new StringBuffer();
    BitSet b = new BitSet();

    s.append("\n/* Codes */\n");
    Iterator it = map.values().iterator();
    while(it.hasNext()) {
      Symbol sym = (Symbol)it.next();
      int arity = sym.getArity();
      /*
       * term1 et term2 sont definis dans term.h
       */
      /********************** EHM Modification mise en commentaire *****************/
      /*if(arity>2 && !b.get(arity)) {
	Tools.indent(struct,deep);
	struct.append("TERMSTR(term" + arity + "," + arity + ");\n");
	b.set(arity);
      } */
      if( !sym.isVariable() ) {
	s.append("#define code_" + sym.getSymbolCode() + " " +
		 sym.getSymbolCode() + "\n");
      }
    }
    s.append("\n/* Structures */\n");
    s.append(struct);
    return s.toString();
  }

  public static String genDeclarFsym(int deep) {
    StringBuffer s = new StringBuffer();

    s.append("\n/* Entetes */\n");
    Iterator it = map.values().iterator();
    while(it.hasNext()) {
      Symbol sym = (Symbol) it.next();
      Tools.indent(s,deep);
      if(sym.isAC()) {
	s.append("extern Gterm *fun_" + sym.getSymbolCode() +
		 "(Gterm *t);\n");
      } else if(sym.isConstant() && sym.isConstructor()) {
	s.append("extern Gterm *con_" + sym.getSymbolCode() +
		 ";\n");
      } else if(sym.isFree() && !sym.isConstructor()) {
	// Entete de la fonction
	s.append("extern Gterm* fun_" + sym.getSymbolCode() + "(");
	for(int i=0 ; i<sym.getArity() ; i++) {
	  s.append("Gterm *v" + (i+1));
	  if(i<sym.getArity()-1) {
	    s.append(",");
	  }
	}
	s.append(");\n");
      } else if(sym.isBuiltin()) {
	int sem = ((SymbolBuiltin)sym).getSymbolSemantic();
	if (sem < 0) {
	  s.append("extern Gterm *fun_" + sym.getSymbolCode() + "(");
	  for(int i=0 ; i<sym.getArity() ; i++) {
	    s.append("Gterm *v" + i);
	    if (i+1 < sym.getArity()) {
	      s.append(", "); 
	    }
	  }
	  s.append(");\n");
	}
      }
    }
    return s.toString();
  }

  public static void genRedirectFsym(OutputCode s,int deep) {
    s.write(deep,"\n/* tableau de pointeurs de fonctions qui retournent */\n");
    s.write(deep,"/* un pointeur sur un term */\n");
    s.write("funTabType funTab[] = {\n");
    int lineLength=0;
    int maxLineLength=70;
    String redirection="";
    for(int i=0 ; i <= getMaxCode() ; i++) {
      Symbol sym=get(i);
      if(sym==null) {
	redirection="NULL";
      } else {
	if(sym.isAC() && !sym.isConstructor()) {
	  redirection="(funTabType) &fun_" + sym.getSymbolCode();
	} else if(sym.isConstant() && sym.isConstructor()) {
	  //redirection="(funTabType) &con_" + sym.getSymbolCode();
	  redirection="NULL";
	} else if(sym.isFree() && !sym.isConstructor()) {
	  redirection="(funTabType) &fun_" + sym.getSymbolCode();
	} else {
	  redirection="NULL";
	}
      }
      lineLength+=2+redirection.length();
      if(lineLength>maxLineLength) {
	s.write("\n");
	lineLength=2+redirection.length();
      }
      s.write(redirection+", ");
    }
    s.write("NULL};\n");
  }

  public static void genCreateFsym(OutputCode s, int deep) {
    s.write(deep,"//init_alloc();\n");
    s.write(deep,"for(i=0 ; i<FSYM_TAB_SIZE ; i++) {\n");
    s.write(deep+1,"Gfsym_init(i,0,\"nullString\",\"nullSort\",0,0,NULL);\n");
    s.write(deep,"}\n");
    Iterator it = map.values().iterator();
    while(it.hasNext()) {
      Symbol sym = (Symbol) it.next();
      s.write( sym.genFsymInit(deep) );
    }
  }

  public static String genFreeFsym(int deep) {
    StringBuffer s = new StringBuffer();

    Iterator it = map.values().iterator();
    while(it.hasNext()) {
      Symbol sym = (Symbol) it.next();
      if( !sym.isVariable() ) {
	Tools.indent(s,deep);
	// C'est le meme FREE que celui de strdup dans tools.h
	s.append("FREE(fsymtab[code_" + sym.getSymbolCode() + "].name);\n");
      }
    }
    return s.toString();
  }

  public static String genCreateConstant(int deep) {
    StringBuffer s = new StringBuffer();
    Tools.indent(s,deep); s.append("\n/* Constantes */\n");

    Iterator it = map.values().iterator();
    while(it.hasNext()) {
      Symbol sym = (Symbol) it.next();
      if( sym.isConstant() && sym.isConstructor() ) {
	Tools.indent(s,deep);
	s.append("Gterm *con_" + sym.getSymbolCode() + ";\n");
      }
    }
    return s.toString();
  }

  public static String genInitConstant(int deep) {
    StringBuffer s = new StringBuffer();

    Tools.indent(s,deep); s.append("\n/* Initialisation des constantes */\n");
    Iterator it = map.values().iterator();
    while(it.hasNext()) {
      Symbol sym = (Symbol) it.next();
      if( sym.isConstant() && sym.isConstructor()) {
	Tools.indent(s,deep);
	s.append("Gmake_const(con_" + sym.getSymbolCode() +
		 ", code_" +  sym.getSymbolCode() + ");\n");
      }
    }
    return s.toString();
  }

  public static String genFreeConstant(int deep) {
    StringBuffer s = new StringBuffer();

    Iterator it = map.values().iterator();
    while(it.hasNext()) {
      Symbol sym = (Symbol) it.next();
      if( sym.isConstant() && sym.isConstructor() ) {
	Tools.indent(s,deep);
	s.append("TERM_FREE(con_" + sym.getSymbolCode() + ");\n");
      }
    }
    return s.toString();
  }

  // *** Peter
  // arguments of a rewrite rule has to been added
  /*
   * Prototypes of the functions of genLabeledRules and genDefinedStrategies
   * (called by the strategy terms compiled by StrategyEval, -strategy 2)
   */
  public static String genDeclarStrategyFunctions(int deep) {
    StringBuffer s = new StringBuffer();
    s.append("\n#ifdef BORO\n");
    Iterator it = map.values().iterator();
    while(it.hasNext()) {
      Symbol sym = (Symbol) it.next();
      if(sym instanceof SymbolLab) {
        Tools.indent(s,deep);
        s.append("extern Gterm* str_rule" + sym.getSymbolCode() + "(Gterm *v0);\n");
      } else if(sym instanceof SymbolDstr) {
        Tools.indent(s,deep);
        s.append("extern Gterm* str_dstr" + sym.getSymbolCode() + "(Gterm *v0);\n");
      }
    }
    s.append("#endif\n");
    return s.toString();
  }

  public static void genLabeledRules(OutputCode s,int deep) {
    StrategyRuleName rterm;
    StrategyTerm sterm;
    Vector subterms;
    OutputCode matchSubtermCode = new OutputCode();

    Lexem name;
    Tools.indent(s,deep); s.write("\n/* substrategies for streval */\n");

    Iterator it = map.values().iterator();
    while(it.hasNext()) {
      Symbol sym = (Symbol) it.next();
      if(sym instanceof SymbolLab) {
	s.write("/*  labeled rule " + sym.getSymbolCode() + " */\n");

	// Entete de la fonction
	s.write("\nGterm* str_rule" + sym.getSymbolCode() + "( Gterm *v0 ) {\n");

        if (Flags.debug) {
	 System.out.println(" /* LAB " + sym.getSymbolCode() + 
	" RULE = " + ((SymbolLab)sym).getRuleIndex() + ", " +
	// " APPLY = " + ((SymbolLab)sym).getApplySymbol() +
	" */ \n" );
        } 

	Tools.genStrategyProlog(s,deep, sym.getSymbolCode());

	subterms = RewriteRule.getAllRulesF1F2(
		((SymbolLab)sym).getApplyCode(),
		((SymbolLab)sym).getRuleIndex());

/******* 0106
	name =  LexemRuleName.get(((SymbolLab)sym).getRuleIndex());

	rterm = new StrategyRuleName(name);

System.out.println("Rulename="+name+" "+RewriteRule.getTable(name)+"\n");

	if (RewriteRule.getTable(name) != null) {
	  subterms = new Vector();	
	  subterms.addElement(rterm);
********/
	if (subterms.size() > 0) {
	  sterm = new StrategyDkRule(subterms);
	  Strategy strat = 
	    new Strategy(LexemStrategyName.newStrategyName(),sterm);
	  if (Flags.debug) {
	    System.out.println(strat);
	  }
	  sterm.compile(s,deep+1,matchSubtermCode);
	}
	Tools.genStrategyEpilog(s,deep, sym.getSymbolCode());
	s.write("}\n");
      }
    }
    s.write(matchSubtermCode.stringDump());
  }

  // *** Peter
  // *** filter rules of the form
  // ***  apply_code(dstr_code(.,.,.),.)
  public static Vector filter(int apply_code, int dstr_code, Vector rules) {
    Vector nrules = new Vector();
    
    for(int i=0; i < rules.size(); i++) {
      RewriteRule rule = (RewriteRule)rules.elementAt(i);
      if (((rule.getLeftside()).getSymbol()).getCode() == apply_code &&
	((rule.getLeftside().getSubterm(0)).getSymbol()).getCode() == 
	dstr_code) {
	nrules.addElement(rule);
      }
    }
    return nrules;
  }

  // *** Peter
  // very preliminaly version
  public static void genDefinedStrategies(OutputCode s,int deep) {
    StrategyRuleName rterm;
    StrategyDkRule sterm;
    Vector subterms;
    OutputCode matchSubtermCode = new OutputCode();
    Lexem name;
    Tools.indent(s,deep); s.write("\n/* defined strategies */\n");

    Iterator it = map.values().iterator();
    while(it.hasNext()) {
      Symbol sym = (Symbol) it.next();
      if(sym instanceof SymbolDstr) {
	s.write("/*  defined strategy " + sym + "==" + sym.getSymbolCode() + " */\n");

	// Entete de la fonction
	s.write("\nGterm* str_dstr" + sym.getSymbolCode() + "( Gterm *v0 ) {\n");

        if (Flags.debug) {
	 System.out.println(" /* DSTR " + sym.getSymbolCode() + 
	" RULE = " + ((SymbolDstr)sym).getRuleIndex() + ", " +
	" APPLY = " + ((SymbolDstr)sym).getApplySymbol() + " */" );
        } 

	Tools.genStrategyProlog(s,deep, sym.getSymbolCode());

	subterms = RewriteRule.getAllRulesF1F2(
		((SymbolDstr)sym).getApplySymbol(),
		((SymbolDstr)sym).getRuleIndex());
/********* 0106
	name =  LexemRuleName.get(((SymbolDstr)sym).getRuleIndex());
	rterm = new StrategyRuleName(name);
	subterms = new Vector();
	subterms.addElement(rterm);
	sterm = new StrategyDkRule(subterms);
        Strategy strat = 
	  new Strategy(LexemStrategyName.newStrategyName(),sterm);
********/
	if (subterms.size() > 0) {
	  sterm = new StrategyDkRule(subterms);
	  Strategy strat = 
	    new Strategy(LexemStrategyName.newStrategyName(),sterm);
	  if (Flags.debug) {
	    System.out.println(strat);
	  }

	  //sterm.strategyLabel=0;
	  Vector rules = new Vector();
	  sterm.compileProlog(s,deep+1,matchSubtermCode, rules);
	  
	  // System.out.println(rules.size() + " rules found \n");
	  
	  rules = filter(((SymbolDstr)sym).getApplySymbol(),
			 sym.getCode(), rules);
	  
	  // System.out.println(rules.size() + " rules left \n");
	  sterm.compileEpilog(s,deep+1,matchSubtermCode, rules);
	  
	}
	Tools.genStrategyEpilog(s,deep, sym.getSymbolCode());
	s.write("}\n");
      }
    }
    s.write(matchSubtermCode.stringDump());
  }

  // *** Peter
  public static String genRedirectBuiltins(int deep) {
    StringBuffer s = new StringBuffer();
    Tools.indent(s,deep); s.append("\n/* Redirection de built-ins */\n");

    Iterator it = map.values().iterator();
    while(it.hasNext()) {
      Symbol sym = (Symbol) it.next();
      if(sym.isBuiltin() ) {
	int sem = ((SymbolBuiltin)sym).getSymbolSemantic();
	if (sem < 0) {
	  Tools.indent(s,deep);
	  s.append("Gterm *fun_" + sym.getSymbolCode() + "(");
	  for(int i=0 ; i<sym.getArity() ; i++) {
	    s.append("Gterm *v" + i);
	    if (i+1 < sym.getArity()) {
	      s.append(", "); 
	    }
	  }
	  s.append(")\n { return fun_" + (-sem) +
		   "("+ sym.getSymbolCode());
	  for(int i=0 ; i<sym.getArity() ; i++) {
	    s.append(", v" + i); 
	  }
	  s.append("); }\n\n");
	}
      }
    }
    return s.toString();
  }

  public int compareTo(Object o) {
    if(o instanceof Lexem) {
      String s1 = toStringCompare();
      String s2 = ((Lexem)o).toStringCompare();
        //System.out.println(s1 + ".compareTo(" + s2 + ") = " + s1.compareTo(s2));
      return s1.compareTo(s2);
    } else if(o instanceof Symbol) {
      String s1 = toStringCompare();
      String s2 = ((Symbol)o).toStringCompare();
        //System.out.println(s1 + ".compareTo(" + s2 + ") = " + s1.compareTo(s2));
      return s1.compareTo(s2);
    } else {
      throw new ClassCastException();
    }
  }

  public abstract String toStringCompare();
  
}


