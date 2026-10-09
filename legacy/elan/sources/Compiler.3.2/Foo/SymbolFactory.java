import java.util.*;
import java.lang.ref.*;

class HashedWeakRef extends WeakReference {
  protected HashedWeakRef next;

  public HashedWeakRef(Object object, HashedWeakRef next) {
    super(object);
    this.next = next;
  }
}

public class SymbolFactory {
  protected static int DEBUG_LEVEL = 1;

  private static int DEFAULT_SYMBOL_TABLE_SIZE = 2003;;
  private static int symbol_table_size = DEFAULT_SYMBOL_TABLE_SIZE;
  private static HashedWeakRef[] symbol_table = new HashedWeakRef[symbol_table_size];

  private static int hashFunction(String name, int arity) {
    int res = name.hashCode() * arity;
    if(DEBUG_LEVEL >= 2) {
      System.out.println("hashFunction(" + name + "," + arity + ") = " +
                         res);
    }
    return res;
  }

  public static Symbol build(Symbol symbol, String name, int arity) {
    Symbol sym;
    int hnr = SymbolFactory.hashFunction(name, arity);
    int idx = hnr % symbol_table_size;

    name = name.intern();
    HashedWeakRef prev, cur;
    prev = null;
    cur  = symbol_table[idx];
    while (cur != null) {
      sym = (Symbol) cur.get();
      if(sym == null) {
          // Found a reference to a garbage collected term
          // remove it to speed up lookups.
        if(DEBUG_LEVEL >= 1) {
          System.out.println("Found a reference to a garbage collected term");
          System.out.println("remove it to speed up lookups");
        }
	if(prev == null) {
	  symbol_table[idx] = cur.next;
	} else {
	  prev.next = cur.next;
	}
      } else {
          // use == because name is interned.
	if (sym.getName() == name
            && sym.getArity() == arity) {
          if(DEBUG_LEVEL >= 1) {
            System.out.println("Symbol found: " + sym);
          }
	  return sym;
	}
      }
      cur = cur.next;
      if(DEBUG_LEVEL >= 1) {
        System.out.println("*** Try next cell");
      }
    }
    
    // No similar Symbol found, so build a new one
    if(DEBUG_LEVEL >= 1) {
      System.out.println("No similar symbol found");
    }
    sym = symbol.create(name, arity);
    cur = new HashedWeakRef(sym, symbol_table[idx]);
    symbol_table[idx] = cur;

    return sym;
  }
  
}
