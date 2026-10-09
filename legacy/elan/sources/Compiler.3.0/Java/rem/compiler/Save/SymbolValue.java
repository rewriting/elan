import java.util.*;

public abstract class SymbolValue extends Symbol {
  protected Object value;

  public SymbolValue(Object v, Lexem sort) {
    super(sort);
    value = v;
  }

  public int getCode() {
    throw new InternalError("Not a SymbolCode");
  }
  public String getSymbolCode() {
    throw new InternalError("Not a SymbolCode");
  }
  public Object getSymbolValue() {
    return value;
  }

  public abstract String setTag();

  public boolean equals(Symbol sym) {
    if(!super.equals(sym)) {
      return false;
    }
    if(sym instanceof SymbolValue &&
       getSymbolValue().equals(sym.getSymbolValue())) {
      return true;
    }
    return false;
  }

  public int cmp(Symbol sym) {
    if(this.equals(sym)) {
      return 0;
    } else if(sym instanceof SymbolCode) {
      return -1;
    } else if(sym instanceof SymbolValue) {
      // A REFAIRE !!!
      if(Flags.debug) {
        System.out.println("Warning : improve the implementation");
      }
      

      Integer i1 = (Integer) this.getSymbolValue();
      Integer i2 = (Integer) ((SymbolValue)sym).getSymbolValue();

      
      if(i1.equals(i2)) {
        System.out.println("coucou");
        System.out.println("sym1 = " + this + "\ti1 = " + i1);
        System.out.println("sym2 = " + sym  + "\ti2 = " + i2);
        
        System.out.println("arity = " + getArity() + "\tsort = " + getSort());
        System.out.println("arity = " + sym.getArity() + "\tsort = " + sym.getSort());
      }
      
      
      if( i1.intValue() < i2.intValue() ) {
	return -1;
      } else if( i1.intValue() > i2.intValue() ) {
	return 1;
      } else {
          /*
           * on est oblige de mettre cette condition parce que le
           * equals ne fonctionne pas comme attendu
           */
        return 0;
      }
      
      //throw new InternalError("Not yet implemented");
    } else {
      throw new InternalError(this + " and " + sym + " are not comparable");
    }
  }

  public int cmpRename(Symbol sym) {
    return cmp(sym);
  }

  public String genInstance() {
    return value.toString();
  }

  /**
   * Construction du terme de depart
   */
  public void genMatchFail(OutputCode s,int deep) {
    throw new InternalError("Cannot be a head symbol");
  }

  public void genFunctionHeader(OutputCode s,int deep, DDTree tree,
                                Vector rules,
				String prefix, boolean isFunctionTree) {
    throw new InternalError("Cannot be a head symbol");
  }

  public String genFsymInit(int deep) {
    return "";
  }

  public String toString() {
    return value.toString();
  }

  public String toString( Vector subterms ) {
    return this.toString();
  }

  public String toStringCompare() {
    return this.toString();
  }

}

