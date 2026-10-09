import java.util.*;
import java.lang.ref.*;

class HashedWeakRef extends WeakReference {
  protected HashedWeakRef next;

  public HashedWeakRef(Object object, HashedWeakRef next) {
    super(object);
    this.next = next;
  }
}

class Factory {
  protected static int DEBUG_LEVEL = 0;

  private int table_size;
  private HashedWeakRef[] table;

  public Factory(int table_size) {
    this.table_size = table_size;
    this.table = new HashedWeakRef[table_size];
  }
  
  public Object build(BaseObject instance, Object[] o) {
    BaseObject bObj;
    int hnr = instance.hashFunction(o);
    int idx = hnr % table_size;
    if(idx<0) {
      idx = -idx;
    }

//    name = name.intern();
    HashedWeakRef prev, cur;
    prev = null;
    cur  = table[idx];
    while (cur != null) {
      bObj = (BaseObject) cur.get();
      if(bObj == null) {
          // Found a reference to a garbage collected term
          // remove it to speed up lookups.
        if(DEBUG_LEVEL >= 1) {
          System.out.println("Found a reference to a garbage collected term");
          System.out.println("remove it to speed up lookups");
        }
	if(prev == null) {
	  table[idx] = cur.next;
	} else {
	  prev.next = cur.next;
	}
      } else {
          // Found a reference
        if(DEBUG_LEVEL >= 1) {
          System.out.println("Found a reference");
        }
        if(bObj.equals(o)) {
          if(DEBUG_LEVEL >= 1) {
            System.out.println("BaseObject found: " + bObj);
          }
	  return bObj;
        }
      }
      cur = cur.next;
      if(DEBUG_LEVEL >= 1) {
        System.out.println("*** Try next cell");
      }
    }
    
    // No similar BaseObject found, so build a new one
    if(DEBUG_LEVEL >= 1) {
      System.out.println("No similar BaseObject found");
    }
    bObj = instance.create(o);
    cur = new HashedWeakRef(bObj, table[idx]);
    table[idx] = cur;

    return bObj;
  }
  
}

/*
abstract class BaseObject  {
  abstract int hashFunction(Object[] o);
  abstract BaseObject create(Object[] o);
  abstract boolean equals(Object[] o);
}
abstract class Symbol extends BaseObject {
  abstract String getName();
  abstract int getArity();
}
*/

interface BaseObject  {
  int hashFunction(Object[] o);
  BaseObject create(Object[] o);
  boolean equals(Object[] o);
}
//interface Symbol extends BaseObject {
interface Symbol {
  abstract String getName();
  abstract int getArity();
}


class FreeSymbol 
  // extends BaseObject
  //  extends Symbol {
  implements Symbol, BaseObject {

  private String name;
  private int arity;
  private int hashCode;

  private static int DEFAULT_SYMBOL_TABLE_SIZE = 2003;;
  private static Factory factory = new Factory(DEFAULT_SYMBOL_TABLE_SIZE);
  private static BaseObject instance = new FreeSymbol();
  protected static int DEBUG_LEVEL = 0;
  
  public static Symbol build(Factory factory, Object[] o) {
    return (Symbol) factory.build(instance,o);
  }

  private void initHashCode() {
    hashCode = name.hashCode() * (1+arity);
   }
  
  public int hashCode() {
    return hashCode;
  }
  
  public int hashFunction(Object[] o) {
    String name = (String) o[0];
    int arity = ((Integer)o[1]).intValue();
    int res = name.hashCode() * (1+arity);
    if(DEBUG_LEVEL >= 2) {
      System.out.println("hashFunction --> " + res);
    }
    return res;
  }

  public String getName() {
    return name;
  }

  public int getArity() {
    return arity;
  }

  public BaseObject create(Object[] o) {
      // [name,Integer(arity)]
    FreeSymbol s = new FreeSymbol();
    s.name = (String) o[0];
    s.arity = ((Integer)o[1]).intValue();
    s.initHashCode();
    return s;
  }

  public boolean equals(Object[] o) {
      // [name,Integer(arity)]
    boolean res;
    if(DEBUG_LEVEL >= 2) {
      System.out.print("equals( " + this + "," );
      for(int i=0 ; i<o.length ; i++) System.out.print(o[i] + " ");
      System.out.print(") = ");
    }
    
    if(o.length != 2) {
      res = false;
    } else if (o[0] != name) {
      res = false;
    } else {
      res =  (arity == ((Integer)o[1]).intValue());
    }
    if(DEBUG_LEVEL >= 2) {
      System.out.println(res);
    }
    return res;
  }

  public String toString() {
    return "[" + getName() + "," + getArity() + "]";
  }

  public static Symbol build(String name, int arity) {
    return (Symbol) factory.build(instance, new Object[] {name.intern(),
                                                          new Integer(arity)});
  }

}  

interface Term {
  abstract Symbol getSymbol();
  abstract Term getSubterm(int i);
}

class FreeTerm implements Term, BaseObject {
  protected Symbol symbol;
  protected Term subterm[];
  private int hashCode;

  private static int DEFAULT_TERM_TABLE_SIZE = 43117;;
  private static Factory factory = new Factory(DEFAULT_TERM_TABLE_SIZE);
  private static BaseObject instance = new FreeTerm();
  
  protected static int DEBUG_LEVEL = 0;

  public void initHashCode() {
    hashCode =  getSymbol().hashCode();
    for(int i=0 ; i<getSymbol().getArity() ; i++) {
      hashCode = getSubterm(i).hashCode() ^ (hashCode << 1) ^ (hashCode >> 1);
    }
  }

  public int hashCode() {
    return hashCode;
  }
  
  public int hashFunction(Object[] o) {
      // [symbol,arg1,...,argn]
    int res =  o[0].hashCode();
    for(int i=1 ; i< o.length ; i++) {
      res = o[i].hashCode() ^ (res << 1) ^ (res >> 1);
    }
    if(DEBUG_LEVEL >= 2) {
      System.out.println("hashFunction --> " + res);
    }
    return res;
  }

  public boolean equals(Object[] o) {
      // [symbol,arg1,...,argn]
    boolean res = false;
    if(DEBUG_LEVEL >= 2) {
      System.out.print("equals( " + this + "," );
      for(int i=0 ; i<o.length ; i++) System.out.print(o[i] + " ");
      System.out.print(") = ");
    }
    
    if(o.length != 1 + ((Symbol)o[0]).getArity()) {
      res = false;
    } else if (o[0] != getSymbol()) {
      res = false;
    } else {
      boolean found = true;
      for(int i=0 ; found && i<getSymbol().getArity() ; i++) {
        found = found && getSubterm(i) == o[i+1];
      }
      if(found) {
        res = true;
      }
    }
    if(DEBUG_LEVEL >= 2) {
      System.out.println(res);
    }
    return res;
  }

  public Symbol getSymbol() {
    return symbol;
  }

  public Term getSubterm(int n) {
    return (Term) subterm[n];
  }

  public BaseObject create(Object[] o) {
    FreeTerm t = new FreeTerm();
    t.symbol = (Symbol) o[0];
    int arity = o.length-1;
    t.subterm = new Term[arity];
    for(int i=0 ; i<arity ; i++) {
      t.subterm[i] = (Term) o[i+1];
    }
    t.initHashCode();
    return t;
  }

  public String toString() {
    String res = getSymbol() + "(";
    if(getSymbol().getArity() > 0 ) {
      res += getSubterm(0);
    }
    for(int i=1 ; i<getSymbol().getArity() ; i++) {
      res += "," + getSubterm(i);
    }
    res += ")";
    return res;
  }

  public static Term build(Object[] o) {
    return (Term) factory.build(instance,o);
  }

  public static Term build0(Symbol s) {
    return (Term) factory.build(instance,new Object[] {s});
  }

  public static Term build1(Symbol s, Term t1) {
    return (Term) factory.build(instance,new Object[] {s,t1});
  }

  public static Term build2(Symbol s, Term t1, Term t2) {
    return (Term) factory.build(instance,new Object[] {s,t1,t2});
  }
}

class Test {
  static public void main(String[] args) {
    Test m = new Test();
      //m.simpleTest();
      //m.testSymbol();
      //m.testAppl();
    m.testFib();
    
  }

    
  public void simpleTest() {
    System.out.println("Test FreeSymbol");
    Symbol s[] = new Symbol[10];
    int cpt=0;

    System.out.println("Creation de (F,1)");
    s[cpt++] = FreeSymbol.build("F",1);
    System.out.println("Creation de (G,2)");
    s[cpt++] = FreeSymbol.build("G",2);
    Symbol s_F,s_a;
    System.out.println("Creation de (F,1)");
    s_F = FreeSymbol.build("F",1);
    System.out.println("Creation de (a,0)");
    s_a = FreeSymbol.build("a",0);

    Term t_a, t_Fa;
    System.out.println("Creation de a)");
    t_a = FreeTerm.build0(s_a);

    System.out.println("Creation de f(a))");
    t_Fa = FreeTerm.build1(s_F,t_a);
    
    System.out.println("re-Creation de f(a))");
    t_Fa = FreeTerm.build1(s_F,t_a);
  }

  void testSymbol() {
    int NB_TEST_SYMBOL = 3*65535/2;
    Symbol foo[] = new Symbol[NB_TEST_SYMBOL];

    long start = System.currentTimeMillis();
    
    for(int i=0 ; i<NB_TEST_SYMBOL ; i++) {
      foo[i] = FreeSymbol.build("xxx"+i , i%3);
    }
    
    for(int i=0 ; i<NB_TEST_SYMBOL ; i++) {
      if(foo[i] != FreeSymbol.build("xxx"+i , i%3)) {
        System.out.println("test failed: " + i);
      }
    }

    long end   = System.currentTimeMillis();
    System.out.println("testSymbol OK in " + (end-start) + " ms");
   
  }
  
  void testAppl() {
    Symbol foo[] = new Symbol[4];
    Term   t[]   = new Term[16];
    foo[0] = FreeSymbol.build("f0" , 0);
    foo[1] = FreeSymbol.build("f1" , 1);
    foo[2] = FreeSymbol.build("f6" , 6);
    foo[3] = FreeSymbol.build("f10" , 10);

    System.out.println("t0");
    t[0] = FreeTerm.build0(foo[0]);
    System.out.println("t1");
    t[1] = FreeTerm.build1(foo[1], t[0]);
    System.out.println("t2");
    t[2] = FreeTerm.build1(foo[1], t[1]);
    System.out.println("t3");
    t[3] = FreeTerm.build1(foo[1], t[0]);
    System.out.println("t4");
    t[4] = FreeTerm.build(new Object[] {foo[2],
                                        t[0], t[0], t[1],
                                        t[0], t[0], t[1]});
    System.out.println("t5");
    t[5] = FreeTerm.build(new Object[] {foo[3],
                                        t[0], t[1], t[0],
                                        t[1], t[0], t[1],
                                        t[0], t[1], t[0],
                                        t[1]});

    if(t[1] != t[3]) System.out.println("error 2");
    if(t[2] == t[1]) System.out.println("error 3");
    if(t[2] == t[6]) System.out.println("error 4");
    if(t[1] == t[2]) System.out.println("error 5");
    if(t[2] == t[3]) System.out.println("error 6");
    if(t[0] == t[1]) System.out.println("error 7");
    System.out.println("testAppl OK");
  }

  public void testFib() {
      TestFib t = new TestFib();
      t.test1();
      t.test2();
      t.test3(20);
      t.test3(30);
      
  }

  
}

class TestFib {
  private static Symbol zero = FreeSymbol.build("zero" , 0);
  private static Symbol suc = FreeSymbol.build("suc" , 1);
  private static Symbol plus = FreeSymbol.build("plus" , 2);
  private static Symbol fib = FreeSymbol.build("fib" , 1);

  private static Term tzero = FreeTerm.build0(zero);
  
  public void test1() {
    System.out.println("test 1");
    Term res = normalizePlus(
      FreeTerm.build2(plus,
                      FreeTerm.build1(suc,FreeTerm.build1(suc,tzero)),
                      FreeTerm.build1(suc,FreeTerm.build1(suc,tzero))));
    System.out.println("res = 2+2 = " + res);
  }

  public void test2() {
    System.out.println("test 2");
    Term res = normalizeFib(
      FreeTerm.build1(fib,
                      FreeTerm.build1(suc,
                      FreeTerm.build1(suc,
                      FreeTerm.build1(suc,
                      FreeTerm.build1(suc,tzero))))));

    System.out.println("res = fib(4) = " + res);
  }

  public void test3(int n) {
    System.out.println("test 3");
        
    Term N = tzero;
    for(int i=0 ; i<n ; i++) {
      N = FreeTerm.build1(suc,N);
    }
      //System.out.println("N = " + N);
    
    long start   = System.currentTimeMillis();
    Term res = normalizeFib(FreeTerm.build1(fib,N));
    long end   = System.currentTimeMillis();
      //System.out.println("fib(" + n + ") = " + res);
    System.out.println("fib(" + n + ") in " + (end-start) + "ms");
  }
  

  public Term normalizePlus(Term t) {
    Term res = t;

    while(true) {
        // plus(0,x) = x
      Term v0 = res.getSubterm(0);
      if(v0.getSymbol() == zero) {
        res = res.getSubterm(1);
        break;
      }

        // plus(s(x),y) => plus(x,s(y))
      if(v0.getSymbol() == suc) {
        res = FreeTerm.build2(plus,
                              v0.getSubterm(0),
                              FreeTerm.build1(suc,res.getSubterm(1)));
        continue;
      }
      break;
    }
    return res;
   }

  public Term normalizeFib(Term t) {
    Term res = t;
    while(true) {
        // fib(0) = suc(0)
      Term v0 = res.getSubterm(0);
      if(v0.getSymbol() == zero) {
        res = FreeTerm.build1(suc,v0);
        break;
      }
        // fib(suc(0)) => suc(0)
      if(v0.getSymbol() == suc) {
        Term v1 = v0.getSubterm(0);
        if(v1.getSymbol() == zero) {
          res = v0;
          break;
        }
      }
        //  fib(s(s(x))) => plus(fib(x),fib(s(x)))
      if(v0.getSymbol() == suc) {
        Term v1 = v0.getSubterm(0);
        if(v1.getSymbol() == suc) {
          Term v2 = v1.getSubterm(0);
          Term fib1 = normalizeFib(FreeTerm.build1(fib,v2));
          Term fib2 = normalizeFib(FreeTerm.build1(fib,v1));
          res = normalizePlus(FreeTerm.build2(plus,fib1,fib2));
          break;
        }
      }
      break;
    }
    return res;
  }

  
}

