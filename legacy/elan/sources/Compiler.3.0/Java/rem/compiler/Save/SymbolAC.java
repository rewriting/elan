import java.util.*;

public class SymbolAC extends SymbolCode {

  public SymbolAC(int code, Lexem[] lexArray, Lexem sort) {
    super(code,lexArray,sort);
  }

  public String genFsymInit(int deep) {
    StringBuffer s = new StringBuffer();
    int arity = -1;
    int semantic = 0 ;
    int defstrat = 0;
    Tools.indent(s,deep);
    s.append("fsym_init(code_" + getSymbolCode() + "," + arity + "," +
	     "\"" + this + "\"," + 
	     semantic + "," + defstrat + ", NULL" + ");\n");
    return s.toString();
  }

  public static Symbol getCreate(int id) {
    throw new InternalError("Not defined");
  }

  public String toString( Vector subterms ) {
    StringBuffer s = new StringBuffer();
    int i;
    s.append(this);
    s.append("[");
    for(i=0 ; i<subterms.size()-1 ; i++) {
      s.append( subterms.elementAt(i) );
      s.append( "." );
    }
    s.append( subterms.elementAt(i) );
    s.append("]");
    return s.toString();
  }

}



