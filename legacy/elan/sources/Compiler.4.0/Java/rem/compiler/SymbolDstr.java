package rem.compiler;

import java.util.*;

public class SymbolDstr extends SymbolBuiltin {
  private int defstrat;

  public SymbolDstr(int code, Lexem[] lexArray, Lexem sort, int sem, int dstrat) {
    super(code,lexArray,sort,sem);
    defstrat = dstrat;
  }

  public int getSymbolDefstrat() {
    return defstrat;
  }	

  public int getRuleIndex() {
    return ElanConstants.indexDstr(defstrat);
  } 

  public int getApplySymbol() {
    return ElanConstants.applySymbolDstr(defstrat);
  }

  public String genFsymInit(int deep) {
    StringBuffer s = new StringBuffer();
    Tools.indent(s,deep);
    s.append("Gfsym_init(code_" + getSymbolCode() + "," + getArity() + "," +
	     "\"" + this + "\"," +
             "\"" + getSort().getName() + "\"," +  // [hassen: Jun 28 01]
	     getSymbolSemantic() + "," + defstrat + 
	     ", (void*)(&(str_dstr" + getSymbolCode() +
	     ")) );\n");
    return s.toString();
  }

  public static Symbol getCreate(int id) {
    throw new InternalError("Not defined");
  }

}

