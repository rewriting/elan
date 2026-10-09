package rem.compiler;

import java.util.*;

public class SymbolFsym extends SymbolBuiltin {
  private int defstrat;

  public SymbolFsym(int code, Lexem[] lexArray, Lexem sort, int sem, int dstrat) {
    super(code,lexArray,sort,sem);
    defstrat = dstrat;
  }

  public int getSymbolDefstrat() {
    return defstrat;
  }	

  public int getFsym1() {
    return ElanConstants.fsymF1(defstrat);
  }

  public int getFsym2() {
    return ElanConstants.fsymF2(defstrat);
  }
	
  public String genFsymInit(int deep) {
    StringBuffer s = new StringBuffer();
    Tools.indent(s,deep);
    s.append("fsym_init(code_" + getSymbolCode() + "," + getArity() + "," +
	     "\"" + this + "\"," +
	     getSymbolSemantic() + "," + defstrat + ", NULL" + ");\n");
    return s.toString();
  }

  public static Symbol getCreate(int id) {
    throw new InternalError("Not defined");
  }

}


