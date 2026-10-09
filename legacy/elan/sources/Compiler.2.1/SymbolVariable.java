import java.util.*;

public class SymbolVariable extends SymbolCode {
  private static Hashtable table = new Hashtable(40);

  public SymbolVariable(int code, Lexem[] lexArray, Lexem sort) {
    super(code,lexArray,sort);
  }

  public String genFsymInit(int deep) {
    return "";
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
    Lexem[] arrayLex = new Lexem[1];
    Symbol sym = SymbolVariable.get(id,sort);
    if(sym==null) {
      arrayLex[0] = LexemVariableName.get(id);
      sym = new SymbolVariable(id, arrayLex, sort);
      sym.put();
    }
    return sym;
  }

  public SymbolVariable copyRename(int idVar) {
    Lexem lex = LexemVariableName.get(idVar);
    Lexem sort = this.getSort();
    if(lex==null) {
      lex = new LexemVariableName(idVar, "var"+idVar);
      lex.put();
    }
    return (SymbolVariable)SymbolVariable.getCreate(idVar,sort);
  }

}

