package completion;
import java.lang.reflect.*;
import java.util.*;


/* This class represents an operator. */
public class Operator{

    // Fields

    /* The name of the operator */
    private String name;

    /* The arity of the operator */
    private int arity;

    /* the priority of the operator */
    private int priority;


    // Constructors

    public Operator(String s){
	this(s, -1, -1);
    }

    public Operator(String s, int p){
	this(s, -1, p);
    }

    public Operator(OpTree op){
	this(op.getName(), op.nbChildren(), -1);
    }
    
    public Operator(String s, int n, int p){
	name = s;
	arity = n;
	priority = p;
    }

    // Methods to access the fields

    public String getName(){
	return name;
    }

    public int getArity(){
	return arity;
    }

    public int getPriority(){
	return priority;
    }


    // Methods to set the fields

    public void setArity(int n){
	arity = n;
    }

    public void setPriority(int p){
	priority = p;
    }

}
