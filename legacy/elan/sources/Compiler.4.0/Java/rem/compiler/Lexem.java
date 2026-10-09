package rem.compiler;

import java.util.*;

public abstract class Lexem implements Comparable {
  protected int code;
  protected String name="<unnamed>";

  public abstract void put();

  public Lexem(int code, String name) {
    this.code = code;
    this.name = name;
  }

  public int getCode() {
    return code;
  }

  public String getName() {
    return name;
  }

  public int compareTo(Object o) {
    if(o instanceof Lexem) {
      String s1 = toStringCompare();
      String s2 = ((Lexem)o).toStringCompare();
        //System.out.println(s1 + ".compareTo(" + s2 + ") = " + s1.compareTo(s2));
      return s1.compareTo(s2);
    } else if(o instanceof Symbol) {
      String s1 = toStringCompare();
      String s2 = ((Symbol)o).toStringCompare();
        //System.out.println(s1 + ".compareTo(" + s2 + ") = " + s1.compareTo(s2));
      return s1.compareTo(s2);
    } else {
      throw new ClassCastException();
    }
  }

  public String toStringCompare() {
    return this.name + this.code;
  }
  
  public String toString() {
    // "<lex" + code + ":" + name + ">";
    if(name.equals("\\")) {
      /*
       * protection du backslash
       */
      return "\\" + name;
    } else {
      return name;
    }
  }
  
}

