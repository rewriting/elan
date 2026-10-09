public class SimpleNode implements Node {
  protected Node parent;
  protected java.util.Vector children;
  protected String identifier;
  protected Object info;
  
  public SimpleNode(String id) {
    identifier = id;
  }

  public static Node jjtCreate(String id) {
    return new SimpleNode(id);
  }
  
  public void jjtOpen() {}
  public void jjtClose() {
    if (children != null) {
      children.trimToSize();
    }
  }
  
  public void jjtSetParent(Node n) { parent = n; }
  public Node jjtGetParent() { return parent; }

  public void jjtAddChild(Node n) {
    if (children == null) {
      children = new java.util.Vector();
    }
    children.addElement(n);
  }
  public Node jjtGetChild(int i) {
    return (Node)children.elementAt(i);
  }

  public int jjtGetNumChildren() {
    return (children == null) ? 0 : children.size();
  }

  /* These two methods provide a very simple mechanism for attaching
     arbitrary data to the node. */

  public void setInfo(Object i) { info = i; }
  public Object getInfo() { return info; }

  /* You can override these two methods in subclasses of SimpleNode to
     customize the way the node appears when the tree is dumped.  If
     your output uses more than one line you should override
     toString(String), otherwise overriding toString() is probably all
     you need to do. */

  public String toString() { return identifier; }
  public String toString(String prefix) { return prefix + toString(); }

  /* Override this method if you want to customize how the node dumps
     out its children. */

  public void dump(String prefix) {
    System.out.println(toString(prefix));
    if (children != null) {
      for (java.util.Enumeration e = children.elements();
	   e.hasMoreElements();) {
	SimpleNode n = (SimpleNode)e.nextElement();
	n.dump(prefix + " ");
      }
    }
  }

  /************************* Added by Sreeni. *******************/

  /** Symbol table */
  protected static java.util.Hashtable symtab = new java.util.Hashtable();

  /** Stack for calculations. */
  protected static Object[] stack = new Object[1024];
  protected static int top = -1;

  public void interpret()
  {
     throw new Error(); // It better not come here.
  }

}

