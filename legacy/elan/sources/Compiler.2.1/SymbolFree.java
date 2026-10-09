import java.util.*;

public class SymbolFree extends SymbolCode {

  public SymbolFree(int code, Lexem[] lexArray, Lexem sort) {
    super(code,lexArray,sort);
  }

  public String genFsymInit(int deep) {
    StringBuffer s = new StringBuffer();
    int semantic = 0;
    int defstrat = 0;
    Tools.indent(s,deep);
    s.append("fsym_init(code_" + getSymbolCode() + "," + getArity() + "," +
	     "\"" + this + "\"," +
	     semantic + "," + defstrat + ", NULL" + ");\n");
    return s.toString();
  }

  public static Symbol getCreate(int id) {
    throw new InternalError("Not defined");
  }

}

