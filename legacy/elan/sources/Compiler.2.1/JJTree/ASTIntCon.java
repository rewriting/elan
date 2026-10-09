/* JJT: 0.2.2 */




public class ASTIntCon extends SimpleNode {
  private int val;

  ASTIntCon(String id) {
    super(id);
  }

  public static Node jjtCreate(String id) {
    return new ASTIntCon(id);
  }


  public void setVal(int n) {
    val = n;
  }

  public String toString() {
    return "IntCon: " + val;
  }

}
