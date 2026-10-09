import java.util.*;

public class SymbolInteger extends SymbolValue {
  private static Hashtable table = new Hashtable(40);

  public SymbolInteger(int intValue, Lexem sort) {
    super(new Integer(intValue),sort);
  }

  public String setTag() {
    return "setIntegerTag";
  }

  public static Symbol get(int intValue) {
    return (Symbol) table.get("sym" + intValue);
  }

  public void put() {
    int intValue = ((Integer)getSymbolValue()).intValue();
    Symbol oldSym = (Symbol)table.put("sym" + intValue, this);
    if( oldSym != null ) {
      if (Flags.warnings) {
        System.out.println("Warning: '" + oldSym + "' and '" + this + "' have the same code");
      }
    }
  }



}

