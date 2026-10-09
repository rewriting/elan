package completion;
import java.util.*;

/* This class represent a substitution. It matches *
 * a value to a variable. */
public class Substitution{
    
    /* The variable name. */
    private String variable;

    /* The value of the variable. */
    private OpTree value;

    /* The name of the variable after renaming. */
    private String newName;

    /* The member the substitution is about. The*
     * first one or the second one. */
    private int member;


    // Constructors

    public Substitution(String n, OpTree v){
	this(n, v, 0);
    }
    
    public Substitution(String n, OpTree v, int m){
	variable = n;
	value = v;
	member = m;
	newName = null;
    }

    /* This function sets the new name of the      *
     * variable. It checks each vector in argument,*
     * and if the variable's name in, then it sets *
     * its new name. */
    public void renameVariables(Vector l, Vector r){
	boolean found = false;
	for(int i = 0; i < l.size(); i++){
	    if(variable.equals(((Substitution) l.get(i)).getValue().toString())){
		newName = ((Substitution) l.get(i)).getVariable();
		found = true;
		break;
	    }
	}
	if(! found){
	    for(int i = 0; i < r.size(); i++){
		if(variable.equals(((Substitution) r.get(i)).getValue().toString())){
		    newName = ((Substitution) r.get(i)).getVariable();
		    break;
		}
	    }
	}
	value.renameVariables(l);
	value.renameVariables(r);
    }


    /* Methods to access the fields */
    public String getVariable(){
	return variable;
    }

    public String getNewName(){
	return newName;
    }

    public OpTree getValue(){
	return value;
    }

    public int getMember(){
	return member;
    }

   
    /* Methods to set the fields.*/
    public void setMember(int m){
	member = m;
    }

    public void setMember(OpTree f, OpTree s){
	if(f.has(variable))
	    member = 1;
	else
	    member = 2;
    }

    

}
