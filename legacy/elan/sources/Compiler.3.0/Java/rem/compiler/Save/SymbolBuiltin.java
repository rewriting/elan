import java.util.*;

public class SymbolBuiltin extends SymbolCode {

  private int semantic;
  private int theory=0;

  public SymbolBuiltin(int code, Lexem[] lexArray, Lexem sort, int sem) {
    super(code,lexArray,sort);
    semantic=sem;
  }

  public SymbolBuiltin(int code, Lexem[] lexArray, Lexem sort, int sem,
		       int intTheory) {
    this(code,lexArray,sort,sem);
    theory=intTheory;
  }

  public int getSymbolSemantic() {
    return semantic;
  }	

  public String genFsymInit(int deep) {
    StringBuffer s = new StringBuffer();
    Tools.indent(s,deep);
    int defstrat = 0;
    int arity=0;
    if(theory==2) {
      arity=-1;
    } else {
      arity=getArity();
    }
    s.append("fsym_init(code_" + getSymbolCode() + "," + arity + "," +
	     "\"" + this + "\"," +
	     semantic + "," + defstrat + ", NULL" + ");\n");
    return s.toString();
  }

  public static Symbol getCreate(int id) {
    throw new InternalError("Not defined");
  }

}

