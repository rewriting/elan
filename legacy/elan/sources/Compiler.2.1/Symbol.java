import java.util.*;

public abstract class Symbol implements Comparable {
  private int arity=0;
  private Lexem sort;

  /* "sym"+code <---> Symbol  */
  private static Hashtable table = new Hashtable(40);
  private boolean isConstructor=true;

  public Symbol(Lexem sort) {
    super();
    this.sort = sort;
  }


  abstract int getCode();
  abstract String getSymbolCode();
  abstract Object getSymbolValue();

  /**
   * Construction du terme de depart
   */
  abstract void genMatchFail(OutputCode s,int deep);
  abstract void genFunctionHeader(OutputCode s,int deep, DDTree tree,
				  int nbRules,
				  String prefix, 
				  boolean isFunctionTree);

  abstract String genFsymInit(int deep);
  abstract String genInstance();
  abstract String toString(Vector subterms);
  abstract int cmp(Symbol sym);
  abstract int cmpRename(Symbol sym);

    /*
  public boolean equals(Object o) {
    if (!(o instanceof Symbol)) {
      return false;
    }
    Symbol sym = (Symbol) o;
    if(this==sym) {
      return true;
    }
    if(getArity()==sym.getArity() && getSort()==sym.getSort()) {
      return true;
    }
    return false;
  }
    */
  
  public int compareTo(Object o) {
    Symbol sym = (Symbol) o;
//    System.out.println(this + ".compareTo(" + sym + ")");
//    return ((String)toString()).compareTo((String)sym.toString());
    return this.cmp(sym);
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
    Enumeration e = table.elements();
    int max=0;
    while(e.hasMoreElements()) {
      Symbol sym = (Symbol) e.nextElement();
      if(sym.getCode() > max)
	max=sym.getCode();
    }
    return max;
  }

  public static Symbol get(int code) {
    return (Symbol) table.get("sym" + code);
  }

  public void put() {
    Symbol oldSym = (Symbol)table.put("sym" + getSymbolCode(),this);
    if( oldSym != null ) {
      if (Flags.warnings) {
        System.out.println("Warning: '" + oldSym + "' and '" + this + "' have the same code");
      }
    }
  }

  public static void dump() {
    Enumeration e = table.keys();
    while(e.hasMoreElements()) {
      String key = (String) e.nextElement();
      Symbol sym = (Symbol) table.get(key);
      if (Flags.verbose) {
        System.out.println("<" + key + ":" + sym + ">");
      }
    }
  }

  public static String genStructure(int deep) {
    StringBuffer s = new StringBuffer();
    StringBuffer struct = new StringBuffer();
    Enumeration e = table.elements();
    BitSet b = new BitSet();

    s.append("\n/* Codes */\n");
    while(e.hasMoreElements()) {
      Symbol sym = (Symbol) e.nextElement();
      int arity = sym.getArity();
      /*
       * term1 et term2 sont definis dans term.h
       */
      if(arity>2 && !b.get(arity)) {
	Tools.indent(struct,deep);
	struct.append("TERMSTR(term" + arity + "," + arity + ");\n");
	b.set(arity);
      }
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
    Enumeration e = table.elements();

    s.append("\n/* Entetes */\n");
    while(e.hasMoreElements()) {
      Symbol sym = (Symbol) e.nextElement();
      Tools.indent(s,deep);
      if(sym.isAC()) {
	s.append("extern struct term *fun_" + sym.getSymbolCode() +
		 "(struct term *t);\n");
      } else if(sym.isConstant() && sym.isConstructor()) {
	s.append("extern struct term *con_" + sym.getSymbolCode() +
		 ";\n");
      } else if(sym.isFree() && !sym.isConstructor()) {
	// Entete de la fonction
	s.append("extern struct term* fun_" + sym.getSymbolCode() + "(");
	for(int i=0 ; i<sym.getArity() ; i++) {
	  s.append("struct term *v" + (i+1));
	  if(i<sym.getArity()-1) {
	    s.append(",");
	  }
	}
	s.append(");\n");
      } else if(sym.isBuiltin()) {
	int sem = ((SymbolBuiltin)sym).getSymbolSemantic();
	if (sem < 0) {
	  s.append("extern struct term *fun_" + sym.getSymbolCode() + "(");
	  for(int i=0 ; i<sym.getArity() ; i++) {
	    s.append("struct term *v" + i);
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
    Enumeration e = table.elements();

    s.write(deep,"\n/* tableau de pointeurs de fonctions qui retournent */\n");
    s.write(deep,"/* un pointeur sur un term */\n");
    s.write("#ifdef __cplusplus\n");
    s.write(deep,"typedef struct term* (*funTabType)(...);\n");
    s.write("#else\n");
    s.write(deep,"typedef struct term* (*funTabType)();\n");
    s.write("#endif\n\n");

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
    Enumeration e = table.elements();

    s.write(deep,"init_alloc();\n");
    s.write(deep,"for(i=0 ; i<fsymtabSize ; i++) {\n");
    s.write(deep+1,"fsym_init(i,0,\"nullString\",0,0,NULL);\n");
    s.write(deep,"}\n");

    while(e.hasMoreElements()) {
      Symbol sym = (Symbol) e.nextElement();
      s.write( sym.genFsymInit(deep) );
    }
  }

  public static String genFreeFsym(int deep) {
    StringBuffer s = new StringBuffer();
    Enumeration e = table.elements();

    while(e.hasMoreElements()) {
      Symbol sym = (Symbol) e.nextElement();
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
    Enumeration e = table.elements();
    Tools.indent(s,deep); s.append("\n/* Constantes */\n");
    while(e.hasMoreElements()) {
      Symbol sym = (Symbol) e.nextElement();
      if( sym.isConstant() && sym.isConstructor() ) {
	Tools.indent(s,deep);
	s.append("struct term *con_" + sym.getSymbolCode() + ";\n");
      }
    }
    return s.toString();
  }

  public static String genInitConstant(int deep) {
    StringBuffer s = new StringBuffer();
    Enumeration e = table.elements();

    Tools.indent(s,deep); s.append("\n/* Initialisation des constantes */\n");
    while(e.hasMoreElements()) {
      Symbol sym = (Symbol) e.nextElement();
      if( sym.isConstant() && sym.isConstructor()) {
	Tools.indent(s,deep);
	s.append("TERM_CONST_ALLOC(con_" + sym.getSymbolCode() +
		 ", code_" +  sym.getSymbolCode() + ");\n");
      }
    }
    return s.toString();
  }

  public static String genFreeConstant(int deep) {
    StringBuffer s = new StringBuffer();
    Enumeration e = table.elements();

    while(e.hasMoreElements()) {
      Symbol sym = (Symbol) e.nextElement();
      if( sym.isConstant() && sym.isConstructor() ) {
	Tools.indent(s,deep);
	s.append("TERM_FREE(con_" + sym.getSymbolCode() + ");\n");
      }
    }
    return s.toString();
  }

  // *** Peter
  // arguments of a rewrite rule has to been added
  public static void genLabeledRules(OutputCode s,int deep) {
    Enumeration e = table.elements();
    StrategyRuleName rterm;
    StrategyTerm sterm;
    Vector subterms;
    OutputCode matchSubtermCode = new OutputCode();

    Lexem name;
    Tools.indent(s,deep); s.write("\n/* substrategies for streval */\n");
    while(e.hasMoreElements()) {
      Symbol sym = (Symbol) e.nextElement();
      if(sym instanceof SymbolLab) {
	s.write("/*  labeled rule " + sym.getSymbolCode() + " */\n");

	// Entete de la fonction
	s.write("\nstruct term* str_rule" + sym.getSymbolCode() + "( struct term *v0 ) {\n");

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
    Enumeration e = table.elements();
    StrategyRuleName rterm;
    StrategyDkRule sterm;
    Vector subterms;
    OutputCode matchSubtermCode = new OutputCode();

    Lexem name;
    Tools.indent(s,deep); s.write("\n/* defined strategies */\n");
    while(e.hasMoreElements()) {
      Symbol sym = (Symbol) e.nextElement();
      if(sym instanceof SymbolDstr) {
	s.write("/*  defined strategy " + sym + "==" + sym.getSymbolCode() + " */\n");

	// Entete de la fonction
	s.write("\nstruct term* str_dstr" + sym.getSymbolCode() + "( struct term *v0 ) {\n");

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
    Enumeration e = table.elements();
    Tools.indent(s,deep); s.append("\n/* Redirection de built-ins */\n");
    while(e.hasMoreElements()) {
      Symbol sym = (Symbol) e.nextElement();
      if(sym.isBuiltin() ) {
	int sem = ((SymbolBuiltin)sym).getSymbolSemantic();
	if (sem < 0) {
	  Tools.indent(s,deep);
	  s.append("struct term *fun_" + sym.getSymbolCode() + "(");
	  for(int i=0 ; i<sym.getArity() ; i++) {
	    s.append("struct term *v" + i);
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

}


