import java.util.*;

interface Symbol  {
  Symbol create(String name, int arity);
  String getName();
  int getArity();
}

class FreeSymbol implements Symbol {
  String name;
  int arity;
  private static Symbol dummySymbol = new FreeSymbol();
  
  public String getName() {
    return name;
  }

  public int getArity() {
    return arity;
  }

  public Symbol create(String name, int arity) {
    FreeSymbol s = new FreeSymbol();
    s.name = name;
    s.arity = arity;
    return s;
  }

  public static Symbol build(String name, int arity) {
    return SymbolFactory.build(dummySymbol,name,arity);
  }

  public String toString() {
    return "[" + getName() + "," + getArity() + "]";
  }
  
  static public void main(String[] args) {
    System.out.println("Test FreeSymbol");
    Symbol s[] = new Symbol[10];
    int cpt=0;
    System.out.println("Creation de (F,1)");
    s[cpt++] = build("F",1);
    System.out.println("Creation de (G,2)");
    s[cpt++] = build("G",2);
    System.out.println("Creation de (H,3)");
    s[cpt++] = build("H",3);

    System.out.println("re-Creation de (F,1)");
    s[cpt++] = build("F",1);
    System.out.println("re-Creation de (G,2)");
    s[cpt++] = build("G",2);
    System.out.println("re-Creation de (H,3)");
    s[cpt++] = build("H",3);
    
    System.out.println("Creation de (F,3)");
    s[cpt++] = build("F",3);
    System.out.println("re-Creation de (G,2)");
    s[cpt++] = build("G",2);
    System.out.println("Creation de (H,1)");
    s[cpt++] = build("H",1);
  }

  
}

