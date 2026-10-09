package rem.compiler;

import java.util.*;

public class LexemIdentifier extends Lexem {
  private static Hashtable table = new Hashtable(40);
  public static LexemIdentifier blank = new LexemIdentifier(-1,"");


  public LexemIdentifier(int code, String name) {
    super(code,name);
  }

  public void put() {
       Lexem old = (Lexem) table.put("lex" + code, this);
    if( old != null ) {
      if (Flags.warnings) {
	System.out.println("Warning: '" + old + "' and '" + this + "' have the same code");
      }
    }
  }

  public static Lexem get(int code) {
    return (Lexem)table.get("lex" + code);
  }

  public static void dump() {
    Enumeration e = table.keys();
    while(e.hasMoreElements()) {
      String key = (String) e.nextElement();
      Lexem lex = (Lexem)table.get(key);
      if (Flags.verbose) {
        System.out.println("<" + key + ":" + lex + ">");
      }
    }
  }

  public static void genCode(OutputCode s) {
    String sIdentifier = "char *tabIdentStr[] = {\n";
    String sIndex = "int tabIdentIndex[] = {\n";
    int cpt = 0;
    
    Enumeration e = table.keys();
    while(e.hasMoreElements()) {
      String key = (String) e.nextElement();
      Lexem lex = (Lexem)table.get(key);
      sIdentifier += "\"" + lex + "\",\n";
      sIndex += lex.code + ",\n";
      cpt++;
    }
    sIdentifier += "\"\"};\n";
    sIndex += "0};\n";
    s.write(sIdentifier);
    s.write(sIndex);
    s.write("int tabIdentSize = " + cpt + ";\n");
  }

  
}

