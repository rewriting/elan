package rem.compiler;

import java.util.*;

class AllocVariable {
  private static BitSet used = new BitSet();

  public static void init() {
    for(int i=0 ; i<used.size() ; i++) {
      used.clear(i);
    }
  }

  public static int get() {
    for(int i=0 ; i<1+used.size() ; i++) {
      if( used.get(i)==false ) {
	//System.out.println("get:  used = " + used + " : " + i);
	used.set(i);
	return i;
      }
    }
    throw new InternalError("No free variable found");   
  }

  public static void free(int n) {
    //System.out.println("free: " + n);
    if( used.get(n)==false ) {
      throw new InternalError("variable freed twice");   
    }
    used.clear(n);
  }
}
 
