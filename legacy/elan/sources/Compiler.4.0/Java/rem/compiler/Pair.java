package rem.compiler;

public class Pair {
  private Object x;
  private Object y;

  public Pair(Object x, Object y) {
    this.x = x;
    this.y = y;
  }

  public Object getX() {
    return x;
  }
  public Object getY() {
    return y;
  }
  public void setX(Object o) {
    x=o;
  }
  public void setY(Object o) {
    y=o;
  }

  public int hashCode() {
    return getX().hashCode() + getY().hashCode();
  }
  
  public boolean equals(Object o) {
    if(o instanceof Pair) {
      Pair p = (Pair)o;
/*
      System.out.println(getX()+"=="+p.getX() + " --> " +
                         getX().equals(p.getX()));
      System.out.println(getY()+"=="+p.getY() + " --> " +
                         getY().equals(p.getY()));
*/
      return getX().equals(p.getX()) && getY().equals(p.getY());
    } else {
      return false;
    }
  }
  public String toString() {
    return "["+x+","+y+"]";
  }
}
