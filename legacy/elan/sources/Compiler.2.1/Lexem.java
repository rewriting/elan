import java.util.*;

public abstract class Lexem implements Comparable {
  protected int code;
  protected String name="<unnamed>";

  abstract void put();

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

    /*
  public boolean equals(Object o) {
    if (!(o instanceof Lexem)) {
      return false;
    }
    Lexem lex = (Lexem) o;
    if(this==lex || (code==lex.code && name.equals(lex.name))) {
      return true;
    }
    return false;
  }
    */
  
  public int compareTo(Object o) {
    Lexem lex = (Lexem) o;
    String s1 = this.name + this.code;
    String s2 = lex.name + lex.code;
    return s1.compareTo(s2);
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

