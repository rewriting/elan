package completion;

import java.util.*;


/* This class is a tree witch ca represent several things.*
 * It is used for representing precedance between operator*
 * and expressions like f(x,y).*/

public class OpTree{

    /* The name of the node.*/
    private String name;

    /* When this class is used for parsing the critical   *
     * pairs, variables are renamed. So we need a new name*
     * for them. If a node is not a variable, this field  *
     * is set to null.*/
    private String newName = null;

    /* When we generate the critical pairs, a part of the *
     * first expression has to be displayed in color. So, *
     * we set the flag at true for the concerned nodes.*/
    private boolean flag;

    /* This vector contains the children of current node. *
     * These elements are OpTree too. When this class is  *
     * used for parsing expressions, children are the     *
     * arguments of the current function, if it is a      *
     * function. Else, this field is null. When this class*
     * is used for precedance, children are operators with*
     * more priority. Each child has the same priority.*/
    private Vector children;

    /* This field is only used for the precedance. It     *
     * regroups all descendants of the current operator.*/
    private Vector descendant;

    /* These fields are used for the critical pairs, when *
     * we write the first member with different colors.   *
     * They do not have a real signification for the      *
     * OpTree. read just means that this node is already  *
     * written, close*/
    private boolean read;
    private boolean close = false;
    private boolean comma = false;


    /* Constructors */

    public OpTree(String n){
	this(n, null, false, false);
    }

    public OpTree(String n, Vector v){
	this(n, v, false, false);
    }

    public OpTree(String n, Vector v, boolean f, boolean r){
	name = n;
	children = v;
	descendant = new Vector();
	flag = f;
	read = r;
	newName = null;
    }
    
    /* sets the children of this node */
    public void setChildren(Vector v){
	children = v;
    }

    /* sets the flag of the node. It means that this node  *
     * has to be displayed in color. */
    public void setFlag(){
	flag = true;
    }

    /* It's clear enough. */
    public boolean hasFlag(){
	return flag;
    }

    
    public void setClose(){
	close = true;
    }

   public void setComma(){
	comma = true;
    }

    /* Adds the node passed in argument to the list of     *
     * children. */
    public void addChildren(OpTree p){
	children.add(p);
    }

    /* Adds a list of children. This function check the    *
     * list to add, and if the element is not alerady a    *
     * child, add it.*/
    public void addChildren(Vector v){
	if(children == null)
	    children = new Vector();
	if(v != null && v.size() >  0){
	    for(int i = 0; i < v.size(); i++){
		boolean stop = false;
		int index = 0;
		if(children.size() > 0){
		    for(int j = 0; j < children.size(); j++){
			if(((OpTree) children.get(j)).getName().equals(((OpTree) v.get(i)).getName())){
			    stop = true;
			    index = j;
			    break; 
			}
		    }
		}	
		if(! stop)
		    children.add((OpTree) v.get(i));
	    }
	}
    }
    
    /* This function return all the descendant of the node. *
     * It calls the function getAllDescendant(Vector v)     *
     * whitch returns the descendants. Then, it checks if   *
     * the current node is in the list of descendants, and  *
     * if it is the case, throws an exception. This function*
     * is called for editing precedance between operators.  *
     * The exception is thrown if there is a loop in the    *
     * precedance. */
    public Vector getAllDescendant() throws Exception{
	descendant = new Vector();
	descendant = getAllDescendant(descendant);
	if(this.isIn(descendant) >= 0){
	    throw(new KbcException("loop for operator : " + name + "\n"));
	}
	return descendant;
    }

    /* This function returns the descendants of the current *
     * node.  */
    public Vector getAllDescendant(Vector v) throws Exception{
	descendant = v;
	if(children != null){
	    for(int i = 0; i < children.size(); i++){
		if(((OpTree) children.get(i)).isIn(descendant) < 0){
		    descendant.add((OpTree) children.get(i));
		    descendant = ((OpTree) children.get(i)).getAllDescendant(descendant);
		}
	    }
	}
	return descendant;
    }

    /* Checks if the current node is in the vector passed in *
     * argument. Return the rank of this one if it is the    *
     * case, -1 else.*/
    public int isIn(Vector v){
	int index = -1;
	if(v != null){
	    for(int i = 0; i < v.size(); i++){
		if(((OpTree) v.get(i)).getName().equals(name)){
		    index = i;
		    break;
		}
	    }
	}
	return index;
    }

    /*  */
    public int precDefined(Vector v){
	int index = -1;
	for(int i = 0; i < v.size(); i++){
	    if(((Operator) v.get(i)).getName().equals(name)){
		index = i;
		break;
	    }
	}
	return index;
    }

    /* This function edit the precedance for each operator. *
     * The arguments are a list of operators and an integer *
     * witch represents a priority. First, we check if the  *
     * operator is already in the vector. If not, we add it *
     * with priority p; else we set its priority with the   *
     * max between its priority and p. Then we call this    *
     * function for its children, wuth priority p+1. */
    public Vector doPrec(Vector v, int p){
	int r;
	if((r = precDefined(v)) < 0){
	    v.add(new Operator(name, p));
	}
	else{
	    Operator op = (Operator) v.get(r);
	    if(op.getPriority() < p)
		op.setPriority(p);
	}
	if(children != null){
	    for(int i = 0; i < children.size(); i++)
		v = ((OpTree) children.get(i)).doPrec(v, p+1);
	}
	return v;
    }

    public String getName(){
	return name;
    }

    public Vector getChildren(){
	return children;
    }

    public int nbChildren(){
	if(children != null)
	    return children.size();
	else
	    return 0;
    }

    public void setFlag(boolean b){
	flag = b;
    }

    /* This function is used during the creation of critical*
     * pairs. It sets the flag of operators that have to be *
     * displayed in color. */
    public void setFlag(Vector v){
	if(v == null || v.size() == 0){
	    flag = true;
	    setChildrenFlag();
	}
	else{
	    int val = Integer.parseInt((String) v.get(0));
	    v.removeElementAt(0);
	    ((OpTree) children.get(val - 1)).setFlag(v);
	}
    }

    /* It sets the flags for all the children.*/
    public void setChildrenFlag(){
	if(children != null){
	    for(int i = 0; i < children.size(); i++){
		((OpTree) children.get(i)).setFlag();
		((OpTree) children.get(i)).setChildrenFlag();
	    }
	}
    }
   
    /* Returns a vector of String. Each string is a string   *
     * representation of a part of the expressionThe second  *
     * one represents the member to be displayed in color. */
    public Vector doString(){
	Vector res = new Vector();
	res.add(readFirst());
	res.add(readSecond());
	res.add(readThird());
	return res;
    }

    /* The string representation of the tree's part witch is *
     * before the flags. */
    public String readFirst(){
	String res = "";
	if(! flag){
	    if(newName != null)
		res += newName;
	    else
		res += name;
	    read = true;
	    if(children != null){
		res += "(";
		for(int i = 0; i < children.size(); i++){
		    if(((OpTree) children.get(i)).hasFlag()){
			if(i == children.size() -1)
			    ((OpTree) children.get(i)).setClose();
			else
			    ((OpTree) children.get(i)).setComma();
			break;
		    }
		    else{
			res += ((OpTree) children.get(i)).readFirst();
			if(i < children.size()-1)
			    res += ",";
			else
			    res += ")";
		    }
		}
	    }
	}
	return res;
    }

    /* The string representation of the tree's part witch has*
     * flags. */
    public String readSecond(){
	String res = "";
	if(! read){
	    if(flag){
		if(newName != null)
		    res += newName;
		else
		    res += name;
		read = true;
		if(children != null){
		    res += "(";
		    for(int i = 0; i < children.size(); i++){
			res += ((OpTree) children.get(i)).readSecond();
			if(i < children.size()-1)
			    res += ",";
			else
			    res += ")";
		    }
		}
		if(close)
		    res += ")";
		else if(comma)
		    res += ",";
	    }
	}
	else{
	    if(children != null){
		for(int i = 0; i < children.size(); i++){
		    res += ((OpTree) children.get(i)).readSecond();
		}	
	    }
	}
	return res;
    }

    /* The string representation of the tree's part witch is *
     * after the flags. */
    public String readThird(){
	String res = "";
	if(! read){
	    if(newName != null)
		res += newName;
	    else
		res += name;
	    read = true;
	    if(children != null){
		res += "(";
		for(int i = 0; i < children.size(); i++){
		    res += ((OpTree) children.get(i)).readThird();
		    if(i < children.size()-1)
			res += ",";
		    else
			res += ")";
		}
	    }
	    if(close)
		res += ")";
	}
	else{
	    if(children != null){
		for(int i = 0; i < children.size(); i++){
		    if(! ((OpTree) children.get(i)).hasFlag())
			res += ((OpTree) children.get(i)).readThird();
		    else{
			((OpTree) children.get(children.size() - 1)).setClose();
		    }
		}	
	    }
	}
	return res;
    }


    /* Returns a string representation of the object.*/
    public String intoString(){
	String res = "";
	res += name;
	if(children != null){
	    res += "(";
	    for(int i = 0; i < children.size(); i++){
		res += ((OpTree) children.get(i)).intoString();
		if(i < children.size()-1)
		    res += ",";
		else
		    res += ")";
	    }
	}
	return res;
    }

    /* Returns a string representation of the object after*
     * renaming. */
    public String intoStringNewName(){
	String res = "";
	if(newName != null)
	    res += newName;
	else
	    res += name;
	if(children != null){
	    res += "(";
	    for(int i = 0; i < children.size(); i++){
		res += ((OpTree) children.get(i)).intoStringNewName();
		if(i < children.size()-1)
		    res += ",";
		else
		    res += ")";
	    }
	}
	return res;
    }

    public String toStringNewName() {
	return intoStringNewName();
    }

    public String toString() {
	return intoString();
    }

    /* This method renames the variables of the node.*
     * It is called for the critical pairs. */
    public void renameVariables(Vector v){
	for(int i = 0; i < v.size(); i++){
	    if(name.equals(((Substitution) v.get(i)).getValue().toString())){
		newName = ((Substitution) v.get(i)).getVariable();
		break;
	    }		
	}
	if(children != null){
	    for(int i = 0; i < children.size(); i++)
		((OpTree) children.get(i)).renameVariables(v);
	}
    }

    /* Returns true if the argument is the name of the current node*
     * or the name of one of its children.*/
    public boolean has(String n){
	boolean b = false;
	if(name.equals(n))
	    b = true;
	else if(children != null){
	    for(int i = 0; i < children.size(); i++){
		if(((OpTree) children.get(i)).has(n)){
		    b = true;
		    break;
		}
	    }
	}
	return b;
    }

    /* Returns true if the two OpTrees have the same name.*/
    public boolean equals(OpTree tree){
	boolean b = false;
	if(this.toString().equals(tree.toString()))
	    b = true;
	return b;
    }
    
}
