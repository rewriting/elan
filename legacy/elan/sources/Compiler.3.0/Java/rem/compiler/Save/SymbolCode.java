import java.util.*;

public abstract class SymbolCode extends Symbol {
  protected int code;
  protected Lexem[] lexArray;


  public SymbolCode(int code, Lexem[] lexArray, Lexem sort) {
    super(sort);
    this.code = code;
    this.lexArray = lexArray;
    int ar = getArity();
    for(int i=0 ; i<lexArray.length ; i++) {
      if( lexArray[i] instanceof LexemSort) {
	ar++;
      }
    }
    setArity(ar);
  }

  public int getCode() {
    return code;
  }

  public String getSymbolCode() {
    return "" + code;
  }

  public Object getSymbolValue() {
    throw new InternalError("Not a SymbolValue");
  }

  public boolean equals(Symbol sym) {
    if(!super.equals(sym)) {
      return false;
    }
    if(sym instanceof SymbolCode && getCode()==sym.getCode()) {
      return true;
    }
    return false;
  }
  
  
  public int cmp(Symbol sym) {
    if(this.equals(sym)) {
      return 0;
    } else if(sym instanceof SymbolCode) {
      SymbolCode symCode=(SymbolCode) sym;
      if(this.isVariable() && !symCode.isVariable()) {
	return -1;
      } else if(!this.isVariable() && symCode.isVariable()) {
	return 1;
      } else if(this.code < symCode.code) {
	return -1;
      } else if(this.code > symCode.code) {
	return 1;
      } else {
	throw new InternalError(this + " and " + sym + " are not comparable");
      }
    } else if(sym instanceof SymbolValue) {
      return 1;
    } else {
      throw new InternalError(this + " and " + sym + " are not comparable");
    }
  }

  public int cmpRename(Symbol sym) {
    if(sym instanceof SymbolCode) {
      if(this.isVariable() && ((SymbolCode)sym).isVariable()) {
	return 0;
      } else {
	return this.cmp(sym);
      }
    } else {
      return this.cmp(sym);
    }
  }

  public String genInstance() {
    return "code_" + getSymbolCode();
  }


  /**
   * Construction du terme de depart
   */
  public void genMatchFail(OutputCode s,int deep) {
    if(isConstant() && isConstructor()) {
      s.write(deep,"res = con_" + getSymbolCode() + ";\n");
    } else if(isConstant() && !isConstructor()) {
      s.write(deep,"TERM_ALLOC(res,term1,code_" + getSymbolCode() + ");\n");
      /*
      s.write(deep,"fprintf(stderr,\"Match Fail error in fun_" +
	      getSymbolCode() + "\\n\");\n");
      s.write(deep,"exit(0);\n");
      */
    } else {
      if(isAC()) {
	s.write(deep,"res=v0;\n");
        if(Flags.color) {
          s.write(deep,"setMonoColor(res);\n");
        }
      } else {
	s.write(deep,"TERM_ALLOC(res,term" + getArity() +
		", " + getSymbolCode() + ");\n");
	for(int i=0 ; i<getArity() ; i++) {
	  s.write(deep,"res->sub[" + i + "] = v" + (i+1) + ";\n");  
	}
      }
    }
  }

  public void genFunctionHeader(OutputCode s,int deep, DDTree tree,
				Vector rules,
				String prefix,
				boolean isFunctionTree) {
    // Entete de la fonction
    s.write("\nstruct term* " + prefix + "(");
    if(isAC() || !isFunctionTree) {
      s.write("struct term *v0");
    } else {
      for(int i=0 ; i<getArity() ; i++) {
	s.write("struct term *v" + (i+1));
	if(i<getArity()-1) {
	  s.write(",");
	}
      }
    }
    s.write(" ) {\n");

    // Declaration des variables
    int startVarNumber = (isFunctionTree)?getStartVarNumber():1;
    
    int maxVarNumber = tree.leftsideVariableAffectation(startVarNumber);
    Term.genLeftsideDeclaration(s,deep+1,startVarNumber,maxVarNumber);

    // Declaration de mask, res
    if(rules.size()>32) {
      s.write(deep+1,"bitSet *mask;\n");
    } else {
      //s.write(deep+1,"bitSet32 *mask32;\n");
    }
    s.write(deep+1,"struct term *res;\n");
    if(Flags.debug) {
        /* Il ne faut pas de ; ici */
      s.write(deep+1,"declareIndentLevel()\n");
    }
    // Pour le filtrage AC
    /*
     * Faut savoir si le lhs contient un pattern AC
     * true || ... implantation temporaire (non optimisee)
     */
    if(true || isAC()) {
      s.write(deep+1,"match_state **ms=NULL;\n");
      s.write(deep+1,"int ACPattern=0;\n");
    }
    //s.write(deep+1,"int mode;\n");
    //s.write(deep+1,"int necessary_link;\n");
    s.write(deep+1,"int indice=-1;\n");
   
    
    if(rules.size()>32) {
      s.write(deep+1,"bitSet_create(mask," + rules.size() + ");\n");
      s.write(deep+1,"bitSet_init_clear(mask);\n");
    } else {
      s.write(deep+1,"bitSet32_stack_create(mask32," + rules.size() + ");\n");
      s.write(deep+1,"bitSet32_init_clear(mask32);\n");
    }

      // Mode trace
    if(Flags.debug) {
      s.write(deep+1,"addindent();\n");
      s.write(deep+1,"saveGlobalIndent();\n");
      if(isAC()) {
        s.write(deep+1,"if(traceLevel>=2) {\n");
        s.write(deep+2,"doindent(indentlevel);\n");
        s.write(deep+2,"printf(\"start with: \");\n");
        s.write(deep+2,"printf(\"fun_" + getSymbolCode() + "_%s: " + 
                "\",fsymtab["+ getSymbolCode() + "].name);\n");
        s.write(deep+2,"internal_term_println(stdout,v0,resultMode);\n");
        s.write(deep+1,"}\n");
      } else {
        s.write(deep+1,"if(traceLevel>=2) {\n");
        s.write(deep+2,"doindent(indentlevel);\n");
        s.write(deep+2,"printf(\"start with: \");\n");
        s.write(deep+2,"printf(\"fun_" + getSymbolCode() + "_%s: " + 
                "\",fsymtab["+ getSymbolCode() + "].name);\n");
          // construction du terme
        this.genMatchFail(s,deep+2);
        s.write(deep+2,"internal_term_println(stdout,res,resultMode);\n");
          /*
            for(int i=0 ; i<getArity() ; i++) {
            s.write(deep+2,"term_print(stdout,v" + (i+1) + ");\n");
            if(i<getArity()-1) {
            s.write(deep+2,"printf(\",\");\n");
            }
            }
            s.write(deep+2,"printf(\")\\n\");\n");
          */      
        s.write(deep+1,"}\n");
      }
    }
    
    // Elimination eventuelle du symbole de tete
    if(isAC()) {
      s.write(deep+1,"if(getArity(((struct termac*)v0))==1 && getMult(((struct termac*)v0),0)==1) {\n");
      s.write(deep+2,"res=getSubterm(((struct termac*)v0),0);\n");
      s.write(deep+2,"goto end_no_rewrite;\n");
      s.write(deep+1,"}\n");
      if(Flags.color) {
          /*
        s.write(deep+1,"if(isMonoColor(v0)) {\n");
          //s.write(deep+2,"printf(\"monoColor\\n\");\n");
        s.write(deep+2,"res=v0;\n");
        s.write(deep+2,"goto end_no_rewrite;\n");
        s.write(deep+1,"}\n");
          */
      }
    }
  }

  public String toString() {
    StringBuffer s = new StringBuffer();
    for(int i=0 ; i < lexArray.length ; i++) {
      s.append( lexArray[i] );
    }
    if(isExtensionVariable()) {
      s.append("$");
    }
      //s.append(getSort());
    return s.toString();
  }

  public String toStringCompare() {
    String s = "";
    for(int i=0 ; i < lexArray.length ; i++) {
      s += lexArray[i].toStringCompare();
    }
    if(isExtensionVariable()) {
      s += "$";
    }
    s += getSort().toStringCompare();
    return s;
  }

  public String toString( Vector subterms ) {
    StringBuffer s = new StringBuffer();
    int pos=0;

    for(int i=0 ; i < lexArray.length ; i++) {
      if( lexArray[i] instanceof LexemSort) {
	s.append( subterms.elementAt(pos) );
	pos++;
      }
      else {
	s.append( lexArray[i] );
      }
    }
    return s.toString();
  }

  
}

