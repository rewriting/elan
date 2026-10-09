package completion;

/* This class represents an oriented rule or *
 * an equation. */ 
public class Rule{
    
    /* The left part of the rule.*/
    private OpTree left;

    /* The right part of the rule.*/
    private OpTree right;

    /* A flag set to true if this is an oriented  *
     * rule, and if its orientation is the        *
     * opposite of the orientation of the rule    *
     *corresponding in the system. */
    private boolean flag = false;


    /* Constructor */
    public Rule(OpTree l, OpTree r){
	left = l;
	right = r;
    }

    /* Returns true if the two rules aresimilar. */
    public boolean equals(Rule rule){
	boolean b = false;
	if(left.equals(rule.getLeft())){
	    if(right.equals(rule.getRight()))
		b = true;
	}
	return b;
    }
    
    /* Returns true if the rules are opposit. */
    public boolean isOpposit(Rule rule){
	boolean b = false;
	if(left.equals(rule.getRight())){
	    if(right.equals(rule.getLeft()))
		b = true;
	}
	return b;
    }

    /* Methods to access fields. */
    public OpTree getLeft(){
	return left;
    }

    public OpTree getRight(){
	return right;
    }

    /* Sets the flag of the rule, and its *
     * left and right parts.*/
    public void setFlag(boolean b){
	flag = b;
	left.setFlag(b);
	right.setFlag(b);
    }

    /* returns a string representation of the rule. */
    public String toString(){
	return left.toString() + "->" + right.toString();
    }

}
