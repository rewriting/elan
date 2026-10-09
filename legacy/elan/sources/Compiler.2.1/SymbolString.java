import java.util.*;

public class SymbolString extends SymbolValue {
  private static Hashtable table = new Hashtable(40);

  public SymbolString(String stringValue, Lexem sort) {
    super(new String(stringValue),sort);
  }

  public String setTag() {
    return "setStringTag";
  }

  public static Symbol get(String stringValue) {
    return (Symbol) table.get("sym" + stringValue);
  }

  public void put() {
    String stringValue = (String)getSymbolValue();
    Symbol oldSym = (Symbol)table.put("sym" + stringValue, this);
    if( oldSym != null ) {
      if (Flags.warnings) {
        System.out.println("Warning: '" + oldSym + "' and '" + this + "' have the same code");
      }
    }
  }



}

