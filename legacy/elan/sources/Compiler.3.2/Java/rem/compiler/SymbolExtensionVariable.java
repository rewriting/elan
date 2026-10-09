package rem.compiler;

import java.util.*;

public class SymbolExtensionVariable extends SymbolVariable {
  private static Hashtable table = new Hashtable(10);

  public SymbolExtensionVariable(int code, Lexem[] lexArray, Lexem sort) {
    super(code,lexArray,sort);
  }

  public static Symbol get(int code, Lexem sort) {
    return (Symbol) table.get("sym" + code + "_" + sort.getName());
  }

  public void put() {
    Symbol old = (Symbol)table.put("sym" + getSymbolCode() + "_" +
				   getSort().getName() ,this);
    if( old != null ) {
      if (Flags.warnings) {
        System.out.println("Warning: '" + old + "' and '" + this + "' have the same code");
      }
    }
  }

  public static Symbol getCreate(int id, Lexem sort) {
    Lexem lex = LexemVariableName.get(id);
    if(lex==null) {
      lex = new LexemVariableName(id, "var"+id);
      lex.put();
    }

    Symbol sym = SymbolExtensionVariable.get(id,sort);
    if(sym==null) {
      sym = new SymbolExtensionVariable(id, new Lexem[] {lex}, sort);
      sym.put();
    }
    return sym;
  }


}

