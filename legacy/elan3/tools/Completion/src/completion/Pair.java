package completion;
import java.util.*;

/* This class represents a critical pair */
public class Pair{

    // Fields

    /* The first members of the two rules that *
     * create the critical pair. */
    private OpTree first;
    private OpTree second;

    /* The critical pair */
    private OpTree firstRewrite;
    private OpTree secondRewrite;

    /* The substitution that make the          *
     * unification of the rules. */
    private Vector substitution;

    /* Variable's new names of each rules. */
    private Vector firstRename;
    private Vector secondRename;

    /* Numbers of the rules that create the    *
     * critical pair. */
    private int firstNumber;
    private int secondNumber;
    

    // Constructor

    public Pair(OpTree o1, OpTree o2, OpTree o3, OpTree o4, Vector v, Vector f, Vector s, int n1, int n2){
	first = o1;
	second = o2;
	firstRewrite = o3;
	secondRewrite = o4;
	substitution = v;
	firstRename = f;
	secondRename = s;
	firstNumber = n1;
	secondNumber = n2;
	setMemberForSubstitution();
	renameVariables();
    }


    /* This method renames the variables of each tree. */
    private void renameVariables(){
	first.renameVariables(firstRename);
	second.renameVariables(secondRename);
	firstRewrite.renameVariables(firstRename);
	firstRewrite.renameVariables(secondRename);
	secondRewrite.renameVariables(secondRename);	
	secondRewrite.renameVariables(firstRename);
	if(substitution != null){
	    for(int i = 0; i < substitution.size(); i++)
		((Substitution) substitution.get(i)).renameVariables(firstRename, secondRename);
	    
	}
    }


    /* Sets the substitution according to the rule it is about. */
    private void setMemberForSubstitution(){
	if(substitution != null){
	    for(int i = 0; i < substitution.size(); i++)
		((Substitution) substitution.get(i)).setMember(first, second);
	}
    }

    /* Methods to access the fields. */

    public OpTree getFirst(){
	return first;
    }

    public OpTree getSecond(){
	return second;
    }

    public Vector getSubstitution(){
	return substitution;
    }

    public int getFirstNumber(){
	return firstNumber;
    }

    public int getSecondNumber(){
	return secondNumber;
    }

    public OpTree getFirstRewrite(){
	return firstRewrite;
    }

    public OpTree getSecondRewrite(){
	return secondRewrite;
    }

}
