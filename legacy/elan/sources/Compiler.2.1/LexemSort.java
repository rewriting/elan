import java.util.*;

public class LexemSort extends Lexem {
  private static Hashtable table = new Hashtable(40);

  public static LexemSort sortInteger;
  public static LexemSort sortIdentifier;
  public static LexemSort sortString;

  private boolean isBuiltin=false;

  public LexemSort(int code, String name) {
    super(code,name);
    if(name.equals("builtinInt")) {
      //if(name.equals("int")) {
      if(sortInteger==null) {
	LexemSort.sortInteger=this;
      } else {
	System.out.println("multiple definition of builtinInt");
      }
    } else if(name.equals("ident")) {
      if(sortString==null) {
	LexemSort.sortIdentifier=this;
      } else {
	System.out.println("multiple definition of builtinString");
      }
      //sortIdentifier=this;
    } else if(name.equals("builtinString")) {
      if(sortString==null) {
	LexemSort.sortString=this;
      } else {
	System.out.println("multiple definition of builtinString");
      }
    }
  }

  public boolean isSortInteger() {
    return this == LexemSort.sortInteger;
  }
  private boolean isSortIdentifier() {
    return this == LexemSort.sortIdentifier;
  }
  private boolean isSortString() {
    return this == LexemSort.sortString;
  }

  public void setBuiltin() {
    isBuiltin=true;
  }

  public boolean isBuiltin() {
    // System.out.println("sort = " + name + " ");
    return isBuiltin;
  }

  public String toString() {
    return  ""; 
      //return  "<" + name + ">" ;
  }
  
  public String genAccess() {
    if(isSortInteger()) {
      return "getInt";
    } else if(isSortIdentifier()) {
      return "getIdentifier";
    } else if(isSortString()) {
      return "getString";
    } else {
      return "getSymb";
    }
  }

 public void put() {
       Lexem old = (Lexem) table.put("lex" + code, this);
    if( old != null ) {
      if (Flags.warnings) {
        System.out.println("Warning: '" + old + "' and '" + this + "' have the same code");
      }
    }
  }

  public static LexemSort get(int code) {
    return (LexemSort) table.get("lex" + code);
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
    String sIdentifier = "char *tabSortStr[] = {\n";
    String sIndex = "int tabSortIndex[] = {\n";
    int cpt = 0;
    
    Enumeration e = table.keys();
    while(e.hasMoreElements()) {
      String key = (String) e.nextElement();
      Lexem lex = (Lexem)table.get(key);
      sIdentifier += "\"" + lex.name + "\",\n";
      sIndex += lex.code + ",\n";
      cpt++;
    }
    sIdentifier += "\"\"};\n";
    sIndex += "0};\n";
    s.write(sIdentifier);
    s.write(sIndex);
    s.write("int tabSortSize = " + cpt + ";\n");
  }

}

