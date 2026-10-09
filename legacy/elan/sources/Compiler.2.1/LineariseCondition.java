import java.util.*;

class LineariseCondition {
  private int maxVariableNumber=-1;
  private Vector conditions = new Vector();


  public int getMaxVariableNumber() {
    return maxVariableNumber;
  }
  
  public void updateMaxVariableNumber(int number) {
    if(number>maxVariableNumber) {
      maxVariableNumber=number;
    }
  }
  
  public Vector getConditions() {
    return conditions;
  }

  public Object conditionAt(int index) {
    return conditions.elementAt(index);
  }

  public void addCondition(Condition cond) {
    conditions.addElement(cond);
  }

  public void removeAllConditions() {
    conditions.removeAllElements();
  }

}
