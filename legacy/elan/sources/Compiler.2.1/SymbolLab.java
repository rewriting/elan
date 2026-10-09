import java.util.*;

public class SymbolLab extends SymbolBuiltin {
  private int defstrat;

  public SymbolLab(int code, Lexem[] lexArray, Lexem sort, int sem, int dstrat) {
    super(code,lexArray,sort,sem);
    defstrat = dstrat;
  }

  public int getSymbolDefstrat() {
    return defstrat;
  }	

  public int getRuleIndex() {
    return ElanConstants.indexLab(defstrat);
  }

  public int getApplyCode() {
    return ElanConstants.applySymbolLab(defstrat);
  }

  public String genFsymInit(int deep) {
    StringBuffer s = new StringBuffer();
    Tools.indent(s,deep);
    s.append("fsym_init(code_" + getSymbolCode() + "," + getArity() + "," +
	     "\"" + this + "\"," +
	     getSymbolSemantic() + "," + defstrat + 
	     ", (void*)(&(str_rule" + getSymbolCode() +
	     ")) );\n");
    return s.toString();
  }

  public static Symbol getCreate(int id) {
    throw new InternalError("Not defined");
  }

}

