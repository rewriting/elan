package rem.compiler;

import java.util.*;

class TermInformation {
  public int varNumber=-1;
  public int ekerVarNumber=-1;

  /**
   * if it is used at least once in the rhs
   */  
  private boolean shared;
  private boolean perfectShare;
  public Term sharedTerm;
  public int numberOfShare=0;

  // where the variable instantiation is stored in AC matching
  public int substIndex=-1; 
  public boolean dioVariable;
  public boolean instantiated;
  public boolean reduced;
  public boolean saved;
  public boolean matchingVariable;
  public boolean numbered;
  public boolean fullNumbering;
  public boolean linearVariable=false;// [NGUYEN: Mar 26 01] 
  public boolean abstractACVariable=false;// [NGUYEN: Mar 26 01] 

  public boolean isNotRhsLinear() {
    return (numberOfShare>=2);
  }

  public boolean isShared() {
      return shared;
  }
  public void setShared() {
      shared=true;
  }
  public boolean isPerfectShare() {
      return perfectShare;
  }
  public void setPerfectShare() {
      perfectShare=true;
  }

  public String toString() {
    StringBuffer s = new StringBuffer();
    if(matchingVariable) {
      s.append("[mv]");
    }
        s.append("[var=" + varNumber +
	     ", subst=" + substIndex +
	     //	     ", eker=" + ekerVarNumber +
	     //", linear=" + linearVariable + // [NGUYEN: Mar 26 01]
	     ", instanciated=" + instantiated +
	     ", " + shareTypeToString());
    if(numberOfShare>0) {
      s.append(", " + numberOfShare);
    }
    s.append("]");
    return s.toString();
  }

  private String shareTypeToString() {
    if(isPerfectShare()) {
      return "PERFECTSHARE";
    } else if(!shared) {
      return "NOSHARE";
    } else {
      return "UNDEFINED";
    }
  }
}
