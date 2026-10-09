package rem.compiler;

import java.util.*;

public class SymbolApply extends SymbolBuiltin {
  private int defstrat;

  public SymbolApply(int code, Lexem[] lexArray, Lexem sort, int sem, int dstrat) {
    super(code,lexArray,sort,sem);
    defstrat = dstrat;
  }

  public String genApplyInit(int deep) {
    StringBuffer s = new StringBuffer();
    Tools.indent(s,deep);
    s.append("Gfsym_init(code_" + getSymbolCode() + "," + getArity() + "," +
	     "\"" + this + "\"," +
	     getSymbolSemantic() + "," + defstrat + ", NULL" + ");\n");
    return s.toString();
  }

  public static Symbol getCreate(int id) {
    throw new InternalError("Not defined");
  }

}


