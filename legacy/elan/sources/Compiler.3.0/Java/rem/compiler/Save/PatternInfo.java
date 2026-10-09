import java.util.*;

class PatternInfo {
    // nombre de variables de multiplicite 1
  private int nbSingleACVariable=0;
    // nombre de variables de multiplicite n>1
  private int nbMultiACVariable=0;
    // nombre de sous-termes de multiplicite 1
  private int nbSingleACSubterm=0;
      // nombre de sous-termes de multiplicite n>1
  private int nbMultiACSubterm=0;
    // contient une variable apparaissant dans une localEvaluation
  private boolean constrainedVariable=false;  


  public void setNbSingleACVariable(int n) {
    nbSingleACVariable=n;
  }

  public void setNbMultiACVariable(int n) {
    nbMultiACVariable=n;
  }

  public void setConstrainedVariable() {
    constrainedVariable=true;
  }

  public int getNbSingleACVariable() {
    return nbSingleACVariable;
  }

  public int getNbMultiACVariable() {
    return nbMultiACVariable;
  }

  public boolean getConstrainedVariable() {
    return constrainedVariable;
  }

  public void setNbSingleACSubterm(int n) {
    nbSingleACSubterm=n;
  }

  public void setNbMultiACSubterm(int n) {
    nbMultiACSubterm=n;
  }

  public int getNbSingleACSubterm() {
    return nbSingleACSubterm;
  }

  public int getNbMultiACSubterm() {
    return nbMultiACSubterm;
  }
  

  public String toString() {
    String s = new String();
    s += "nbSingleACVariable  = " + nbSingleACVariable  + "\n";
    s += "nbMultiACVariable   = " + nbMultiACVariable   + "\n";
    s += "nbSingleACSubterm   = " + nbSingleACSubterm   + "\n";
    s += "nbMultiACSubterm    = " + nbMultiACSubterm    + "\n";
    s += "constrainedVariable = " + constrainedVariable + "\n";
    return s;
  }


}
