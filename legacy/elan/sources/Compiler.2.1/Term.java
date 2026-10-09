import java.util.*;

public class Term {
    /** head symbol */
private Symbol symbol;
    /** array of subterms */
private Vector subterms;
    /** father reference */
private Term father;
    /** multiplicity in Ordered Normal Form AC representation */
private int multiplicity;

    /** C variable number that stores the term */
public TermInformation termInfo;


  
public Term(Symbol termSymbol, Vector termSubterms) {
  symbol = termSymbol;
  subterms = termSubterms;
  multiplicity=1;
    // maj du lien vers le pere
  for(int i=0 ; i<arity() ; i++) {
    getSubterm(i).father=this;
  }
  termInfo = new TermInformation();
}

public int arity() {
  if( subterms == null ) {
    return 0;
  } else {
    return subterms.size();
  }
}

public Term deepCopy() {
  Vector copySubterms = new Vector(arity());
  for(int i=0 ; i<arity() ; i++) {
    copySubterms.addElement(getSubterm(i).deepCopy());
  }
  Term copy = new Term(symbol,copySubterms);
  copy.multiplicity=multiplicity;
  return copy;
}

public Symbol getSymbol() {
  return symbol;
}
public int getMultiplicity() {
  return multiplicity;
}
public Vector getSubterms() {
  return subterms;
}
public Term getSubterm(int i) {
  return (Term)subterms.elementAt(i);
}
public Term getFather() {
  return father;
}

public int getSemantic() {
  Symbol symb = this.getSymbol();
  if(symb instanceof SymbolBuiltin) {
    return ((SymbolBuiltin)symb).getSymbolSemantic();
  } else {
    return 0;
  }
}

public boolean isVariable() {
  return symbol.isVariable();
}
public boolean isExtensionVariable() {
  return symbol.isExtensionVariable();
}
public boolean isFree() {
  return symbol.isFree();
}
public boolean isAC() {
  return symbol.isAC();
}
public boolean isConstant() {
  return symbol.isConstant();
}
public boolean isFunction() {
  return symbol.isFunction();
}
public boolean isConstructor() {
  return getSymbol().isConstructor();
}
public void clearConstructor() {
  getSymbol().clearConstructor();
}
public boolean isBuiltinSort() {
  return getSymbol().isBuiltinSort();
}

public boolean containsAC() {
  boolean res=false;
  if(isAC()) {
    res=true;
  } else {
    for(int i=0 ; res==false && i<arity() ; i++) {
      res = res || getSubterm(i).containsAC();
    }
  }
  return res;
}

public int getNbACSubterms() {
  int res=0;
  if(isAC()) {
    res++;
  }
  for(int i=0 ; i<arity() ; i++) {
    res += getSubterm(i).getNbACSubterms();
  }
  return res;
}

public Term getFirstACPattern() {
  Term res=null;
  if(isAC()) {
    return this;
  } else {
    for(int i=0 ; res==null && i<arity() ; i++) {
      res = getSubterm(i).getFirstACPattern();
    }
  }
  return res;
}

public int getVarNumber() {
  return termInfo.varNumber;
}

public boolean isShared() {
  return termInfo.isShared();
}
public void setShared() {
  termInfo.setShared();
}

public boolean isPerfectShare() {
  ensureFullNumbering();
  return termInfo.isPerfectShare();
}
public void setPerfectShare() {
  termInfo.setPerfectShare();
}

public boolean isLeftsideDioVariable() {
  return termInfo.dioVariable;
}
public boolean isDioVariable() {
  Term leftside=this;
  if(isPerfectShare())
    leftside=termInfo.sharedTerm;
  return leftside.isLeftsideDioVariable();
}

public boolean isLeftsideInstantiated() {
  return termInfo.instantiated;
}
public boolean isInstantiated() {
  Term leftside=this;
  if(isPerfectShare())
    leftside=termInfo.sharedTerm;
  return leftside.isLeftsideInstantiated();
}

public boolean isReduced() {
  return termInfo.reduced;
}
public boolean isSaved() {
  return termInfo.saved;
}
public boolean isMatchingVariable() {
  return termInfo.matchingVariable;
}

public boolean isLeftsideNumbered() {
  return termInfo.numbered;
}
public boolean isNumbered() {
  Term leftside=this;
  if(isPerfectShare())
    leftside=termInfo.sharedTerm;
  return leftside.isLeftsideNumbered();
}

public void setSharedGenerated() {
  termInfo.isSharedGenerated=true;
}
public void clearSharedGenerated() {
  termInfo.isSharedGenerated=false;
}
public boolean isSharedGenerated() {
  return termInfo.isSharedGenerated;
}
public boolean isNotRhsLinear() {
  return termInfo.isNotRhsLinear();
}

public void setDioVariable() {
  termInfo.dioVariable=true;
}
public void setInstantiated() {
  termInfo.instantiated=true;
}
public void clearInstantiated() {
  termInfo.instantiated=false;
}
public void setReduced() {
  termInfo.reduced=true;
}
public void setSaved() {
  termInfo.saved=true;
}
public void setMatchingVariable() {
  termInfo.matchingVariable=true;
}
public void setNumbered() {
  termInfo.numbered=true;
}
public void clearNumbered() {
  termInfo.numbered=false;
}
public void clearSaved() {
  termInfo.saved=false;
}


public boolean dependFrom(Term t) {
  boolean res=false;
  if(!t.isVariable()) {
    throw new InternalError("dependFrom: not yet implemented");
  }

  if( t.isVariable() && symbol.equals(t.symbol) ) {
    res = true;
  } else if( isFunction() ) {
    for(int i=0 ; res==false && i<arity() ; i++) {
      res = res || getSubterm(i).dependFrom(t);
    }
  }
    
  return res;
}

    /*
     * check pattern class
     */

public PatternInfo getPatternInfo(BranchEvaluation branch) {
  PatternInfo patternInfo = new PatternInfo();
  if(isAC()) {
    int nbSingleACVariable=0;
    int nbMultiACVariable=0;
    int nbSingleACSubterm=0;
    int nbMultiACSubterm=0;
    for(int i=0 ; i<arity() ; i++) {
      if(getSubterm(i).isVariable()) {
        if(getSubterm(i).multiplicity == 1) {
          nbSingleACVariable++;
        } else {
          nbMultiACVariable++;
        }
        if( branch.dependFrom(getSubterm(i)) ) {
          patternInfo.setConstrainedVariable();
        }
      } else {
        if(getSubterm(i).multiplicity == 1) {
          nbSingleACSubterm++;
        } else {
          nbMultiACSubterm++;
        }
      }
    } 
    patternInfo.setNbSingleACVariable(nbSingleACVariable);
    patternInfo.setNbMultiACVariable(nbMultiACVariable);
    patternInfo.setNbSingleACSubterm(nbSingleACSubterm);
    patternInfo.setNbMultiACSubterm(nbMultiACSubterm);
  } else {
      //throw new InternalError("setPatternInfo: not yet implemented");
  }
  return patternInfo;
}

    /**
     * Pour savoir si le partage des sous termes a ete fait
     * et si les noeuds ont ete numerotes
     */
public void ensureFullNumbering() {
  if(!isFullNumbering()) {
    throw new InternalError("Term is not fully numbered:\n\t use RewriteRule.variableNumbering() before");
  }	
}
private boolean isFullNumbering() {
  return termInfo.fullNumbering;
}
public void setFullNumbering() {
  termInfo.fullNumbering=true;
  for(int i=0 ; i<arity() ; i++) {
    getSubterm(i).setFullNumbering();
  }
}

public boolean isUnderAC() {
  if(getFather()==null) {
    return false;
  } else {
    if(getFather().isAC()) {
      return true;
    } else {
      return getFather().isUnderAC();
    }
  }
}

public boolean isUnderConstructor() {
  if(getFather()==null) {
    return false;
  } else {
    return getFather().isConstructor();
  }
}

public boolean isLeftsideVariableDirectlyUnderAC() {
  if(isVariable()) {
    if( getFather()!=null && getFather().isAC() ) {
      return true;
    }
  }
  return false;
}
public boolean isVariableDirectlyUnderAC() {
  Term leftside=this;
  if(isPerfectShare())
    leftside=termInfo.sharedTerm;
  return leftside.isLeftsideVariableDirectlyUnderAC();
}

public boolean isLeftsideVariableUnderAC() {
  return (isVariable() && isUnderAC());
}
public boolean isVariableUnderAC() {
  Term leftside=this;
  if(isPerfectShare())
    leftside=termInfo.sharedTerm;
  return leftside.isLeftsideVariableUnderAC();
}

public boolean isLeftsideReducibleVariable() {
  return (isLeftsideVariableDirectlyUnderAC() &&
          getFather().getFather() == null);
}
public boolean isReducibleVariable() {
  Term leftside=this;
  if(isPerfectShare())
    leftside=termInfo.sharedTerm;
  return leftside.isLeftsideReducibleVariable();
}

public boolean dependFromVariableUnderAC() {
  boolean find = false;
  boolean res;
  if(isVariableUnderAC()) {
    res = true;
  } else {
    for(int i=0 ; i<arity() && !find ; i++) {
      find = getSubterm(i).dependFromVariableUnderAC();
    }
    res = find;
  }
  return res;
}
  

    /**
     * Pour la compilation : code unique
     */
public String getSymbolCode() {
  return symbol.getSymbolCode();
}

    /**
     * returns i such that father.getSubterm(i)==this
     *        -1 if it has no father
     */
public int getChildNumber() {
  if(father == null) {
    return -1;
  } else {
    for(int i=0 ; i<father.arity() ; i++) {
      if( this == father.getSubterm(i) ) {
        return i;
      }
    }
  }
  throw new InternalError("my father does not know me: " + this);
}
    /*
public boolean equals(Term term) {
  boolean equal = true;
  if( this == term ) {
    return true;
  }
  if( symbol == term.getSymbol() && arity() == term.arity()
      && getMultiplicity() == term.getMultiplicity()) {
    for(int i=0 ; equal && i<arity() ; i++) {
      equal &= (getSubterm(i) == term.getSubterm(i));
    }
    equal &= termInfo.equals(term.termInfo);
    return equal;
  } else {
    return false;
  }
}
    */
  
public boolean deepEquals(Term term) {
  boolean equal = true;
  if( this.equals(term) )
    return true;
  if( symbol.equals(term.getSymbol())
      && arity()==term.arity()
      && getMultiplicity()==term.getMultiplicity()) {
    for(int i=0 ; equal && i<arity() ; i++) {
      equal  = equal && getSubterm(i).deepEquals(term.getSubterm(i));
    }
    return equal;
  } else {
    return false;
  }
}


public int cmp(Term term) {
  int r;
  if(this.equals(term)) {
    return 0;
  }
  r=symbol.cmp(term.symbol);
  if( r!=0 )
    return r;
    
  if(!isAC()) {
      /** lexicographic ordering on subterms */
    for(int i=0 ; i<arity() ; i++) {
      r=getSubterm(i).cmp(term.getSubterm(i));
      if( r!=0 )
        return r;
    }
  } else {
      /* multiset ordering on subterms */
    int a1 = arity();
    int a2 = term.arity();
    int mina = (a1<a2)?a1:a2;
    for(int i=0 ; i<mina ; i++) {
      r=getSubterm(i).cmp(term.getSubterm(i));
      if( r!=0 )
        return r;
      if(getSubterm(i).multiplicity<term.getSubterm(i).multiplicity)
        return -1;
      else if(getSubterm(i).multiplicity>term.getSubterm(i).multiplicity)
        return 1;
    }
    if(a1==a2)
      return 0;
    else if(a1<a2)
      return -1;
    else if(a1>a2)
      return 1;
  }
  return 0;
}

public int cmpRename(Term term) {
  int r;
  if(this.equals(term)) {
    return 0;
  }
  r=getSymbol().cmpRename(term.symbol);
  if( r!=0 )
    return r;
    
  if(!isAC()) {
      /** lexicographic ordering on subterms */
    for(int i=0 ; i<arity() ; i++) {
      r=getSubterm(i).cmpRename(term.getSubterm(i));
      if( r!=0 )
        return r;
    }
  } else {
      /* multiset ordering on subterms */
    int a1 = arity();
    int a2 = term.arity();
    int mina = (a1<a2)?a1:a2;
    for(int i=0 ; i<mina ; i++) {
      r=getSubterm(i).cmpRename(term.getSubterm(i));
      if( r!=0 )
        return r;
      if(getSubterm(i).multiplicity<term.getSubterm(i).multiplicity)
        return -1;
      else if(getSubterm(i).multiplicity>term.getSubterm(i).multiplicity)
        return 1;
    }
    if(a1==a2)
      return 0;
    else if(a1<a2)
      return -1;
    else if(a1>a2)
      return 1;
  }
  return 0;
}

public void flatten() {
    //System.out.println("term = " + this);
  for(int i=0 ; i<arity() ; i++) {
    getSubterm(i).flatten();
  }
  if(isAC()) {
    for(int i=0 ; i<arity() ; i++) {
      Term subterm = getSubterm(i);
      if(symbol.equals(subterm.getSymbol())) {
        for(int j=0 ; j<subterm.arity() ; j++) {
          subterms.addElement(subterm.getSubterm(j));
          subterm.getSubterm(j).father=this;
        }
        subterms.removeElementAt(i);
        i--;
      }
    }
  }
}

public void orderedNormalForm() {
  for(int i=0 ; i<arity() ; i++) {
    getSubterm(i).orderedNormalForm();
  }
  if(isAC()) {
    for(int i=1 ; i<arity() ; ) {
      i=insertOrderedNormalForm(i);
    }
  }
}

private int insertOrderedNormalForm(int index) {
  Term subterm = getSubterm(index);

  for(int i=0 ; i<index ; i++) {
    int r=getSubterm(i).cmp(subterm);
      //System.out.println(getSubterm(i) + " cmp " + subterm + " = " + r);
      /*
       * PEM 31.10.98 : if(r>0) {
       * si on remplace >0 par <0 cela fonctionne directement
       * avec EkerACMatcher mais cela nous oblige a maintenir 3
       * version de term_onf
       */
    if(r>0) {
	// insertion
      subterms.removeElementAt(index);
      subterms.insertElementAt(subterm,i);
      return index;
    } else if(r==0) {
	// modif multiplicity
      subterms.removeElementAt(index);
      getSubterm(i).multiplicity++;
      return index;
    }
  }
    // do nothing
  return index+1;
}

public int depth() {
  int max=0;
  int depth;
  for(int i=0 ; i<arity() ; i++) {
    depth=getSubterm(i).depth();
    if(depth>max)
      max=depth;
  }
  return 1+max;
}

    /**
     * Recherche des variables du rhs
     */
public void searchVariableShare(Term l) {
  if(l!=null) {  
    if( isVariable() && termInfo.numberOfShare==0) {
      markVariableShare(l);
    } else if( isFunction() ) {
      for(int i=0 ; i<arity() ; i++) {
        getSubterm(i).searchVariableShare(l);
      }
    }
  }
}

public void searchVariableShareFullNumbering(Term l) {
  termInfo.fullNumbering=true;
  if(l!=null) {
    if( isVariable() ) {
      markVariableShare(l);
    } else if( isFunction() ) {
      for(int i=0 ; i<arity() ; i++) {
        getSubterm(i).searchVariableShareFullNumbering(l);
      }
    }
  }
}


    /**
     * Marquage des variables du rhs
     */
private void markVariableShare(Term l) {
  rmarkVariableShare(l);
    //System.out.println("rhs = " + this + "\tlhs = " + l);
}

private void rmarkVariableShare(Term l) {
    // this is a variable
      
  if(this==l) {
      //System.out.println("identical terms");
    return;
  }

  if( l.isVariable() && !isPerfectShare() ) {
    if( symbol.equals(l.symbol) ) {
	// si l est deja partagee
	//if(l.isPerfectShare()) {
      if(l.termInfo.sharedTerm != null) {
        termInfo.sharedTerm = l.termInfo.sharedTerm;;
      } else {
        termInfo.sharedTerm = l;
      }
      termInfo.sharedTerm.termInfo.numberOfShare++;
	//PEM 29.10.98 : setShared();
      setShared();
	//termInfo.sharedTerm.setShared();

      setPerfectShare();
	//System.out.println("Bingo : " + this);
	//System.out.println("lhs : " + l);
    }
  } else if( l.isFunction() ) {
    for(int i=0 ; i<l.arity() ; i++) {
      rmarkVariableShare(l.getSubterm(i));
    }
  }
    //throw new InternalError("Unknown type");   
}

    /**
     * Marquage des variables d'un DioTerm
     */
public void markDioVariable() {
  if(isDio()) {
    for(int i=0 ; i<arity() ; i++) {
      getSubterm(i).setDioVariable();
    }
  } else {
    for(int i=0 ; i<arity() ; i++) {
      getSubterm(i).markDioVariable();
    }
  }
}

    /**
     * Si le symbole de tete est AC et tous 
     * les sous-termes sont des variables
     */
public boolean isDio() {
  if(isAC()) {
    boolean allVar=true;
    for(int i=0 ; i<arity() && allVar ; i++) {
      allVar = allVar && getSubterm(i).isVariable();
    }
    return allVar;
  } else {
    return false;
  }
}

public int numberOfVariableUnderAC() {
  int res=0;
  if(isAC()) {
    for(int i=0 ; i<arity() ; i++) {
      if(getSubterm(i).isVariable()) {
        res++;
      }
    }
  } else {
      //throw new InternalError("not yet implemented");   
  }
  return res;
}

    /**
     * Si le terme est/contient un sous-terme Dio
     */  
public boolean containsACDio() {
  boolean res=false;

  if(isDio()) {
    res = true;
  } else {
    for(int i=0 ; i<arity() ; i++) {
      res = res || getSubterm(i).containsACDio();
    }
  }
  return res;
}

    /**
     * is the term ground ?
     */
public boolean isGround() {
  boolean res=true;

  if(isVariable()) { 
    return false;
  } else {
    for(int i=0 ; res && i<arity() ; i++) {
      res = res &&  getSubterm(i).isGround();
    }
  }
  return res;
}
 

    /**
     * Numerotation des matching variables du lhs
     * 0 pour le terme principal
     *   - 1..n pour les fils directs dans le cas Syntactic (1 niveau seulement)
     *   - 0 pour tous les fils de noeuds AC (recursif)
     */
public void leftsideVariableInit(boolean initSubterm) {
    //System.out.println("*** leftsideVariableInit *** : " + this);
  termInfo.varNumber = 0;
  setMatchingVariable();
  if(initSubterm && !isAC()) {
    for(int i=0 ; i<arity() ; i++) {
      getSubterm(i).termInfo.varNumber = i+1;
      getSubterm(i).setMatchingVariable();
    }
  }
  rleftsideVariableACInit();
}
  
    // Mise a 0 recursive des sous-termes de noeud AC
private void rleftsideVariableACInit() {
    //    System.out.println("*** rleftsideVariableACInit *** : " + this);
  for(int i=0 ; i<arity() ; i++) {
    if( isAC() && !getSubterm(i).isLeftsideVariableDirectlyUnderAC() ) {
      getSubterm(i).termInfo.varNumber = 0;
    }
    getSubterm(i).rleftsideVariableACInit();
  }
}

    /**
     * Initialisation recursive d'un terme
     * - si full==true varNumber est reinitialise
     * - si full==false, seules les flags sont initialises
     */
  
public void leftsideVariableClear(boolean full) {

    //System.out.println("*** leftsideVariableClear *** : " + this);
    //if(!isLeftsideVariableDirectlyUnderAC()) {
  if(full & isMatchingVariable()) {
    termInfo.varNumber=-1;
    termInfo.matchingVariable=false;
  }

    //termInfo.substIndex=-1;
    //termInfo.dioVariable=false;
    /*
     * On ne peut pas utiliser isPerfectshare() parce que le term
     * n'est pas forcement numerote
     */
  clearInstantiated();
  termInfo.isSharedGenerated=false;  
  termInfo.reduced=false;
  termInfo.saved=false;
  if(termInfo.isPerfectShare()) {
    Term leftside=termInfo.sharedTerm;
    leftside.clearInstantiated();
    leftside.termInfo.isSharedGenerated=false;  
    leftside.termInfo.reduced=false;
    leftside.termInfo.saved=false;
  }
  
  for(int i=0 ; i<arity() ; i++) {
    getSubterm(i).leftsideVariableClear(full);
  }
}


    /**
     * Numerote les variables qui sont sous un symbole AC
     * de gauche a droite
     */
public void leftsideACVariableAffectation() {
  rleftsideACVariableAffectation(false,0);
}

public int leftsideACVariableAffectation(int substIndex) {
  return rleftsideACVariableAffectation(false,substIndex);
}

private int rleftsideACVariableAffectation(boolean underAC,
                                           int substIndex) {
    // System.out.println("*** rleftsideVariableAffectation *** : " + this);
    // Cas d'une variable AC non utilisee a droite

  if(isPerfectShare()) {
      /*
       * Only for non linear lhs (AC matching condition)
       * do nothing
       */ 
    return substIndex;
  }

  if(isVariable()) {
    if(underAC) {
      if(!isNumbered()) {
        termInfo.varNumber = AllocVariable.get();
        termInfo.matchingVariable = false;
        setNumbered();
      }
      termInfo.substIndex=substIndex;
      return substIndex+1;
    } else {
	/*
	 * do nothing
	 */
      return substIndex;
    }
  } else if(isAC()) {
    for(int i=0 ; i<arity() ; i++) {
      substIndex=getSubterm(i).rleftsideACVariableAffectation(true,substIndex);
    }
    return substIndex;
  } else if(isFunction()) {
    for(int i=0 ; i<arity() ; i++) {
      substIndex=getSubterm(i).rleftsideACVariableAffectation(underAC,substIndex);
    }
    return substIndex;
  }
  throw new InternalError("Unknown type");   
}

public void leftsideACVariableLiberation() {
  if(isPerfectShare()) {
    return;
  }
  if(isLeftsideVariableUnderAC() && isNumbered()) {
    AllocVariable.free( termInfo.varNumber );
    clearNumbered();
  } else {
    for(int i=0 ; i<arity() ; i++) {
      getSubterm(i).leftsideACVariableLiberation();
    }
  }
}

    /**
     * Numerotation des variables du rhs
     * A utiliser apres leftsideACVariableAffectation()
     */

public void rightsideVariableAffectation() {
  if(Flags.newVariableAffectation) {
    rightsideVariableAffectation2();
  } else {
    rightsideVariableAffectationWithNoReuse();
  }
}
public void rightsideVariableLiberation() {
  if(Flags.newVariableAffectation) {
    rightsideVariableLiberation2();
  } else {
    rightsideVariableLiberationWithNoReuse();
  }
}

public void rightsideVariableAffectationWithNoReuse() {
    //    System.out.println("*** rightsideVariableAffectation *** : " + this);
    /*
      if(isPerfectShare()) {
      System.out.println("isVariableUnderAC() = " + isVariableUnderAC());
      System.out.println("isNumbered()        = " + isNumbered());
      } 
      */

    // Reservation
  if(!isPerfectShare()) {
    if(isFunction()) {
      for(int i=arity()-1 ; i>=0 ; i--) {
        getSubterm(i).rightsideVariableAffectationWithNoReuse();
      }
      if(!isShared()) {
        termInfo.varNumber = AllocVariable.get();
      }
    } else if(isVariable()) {
	// where with pattern
      termInfo.varNumber = AllocVariable.get();
    }
  }
}

public void rightsideVariableLiberationWithNoReuse() {
    //System.out.println("*** rightsideVariableLiberationWithNoReuse ***");
    // Restitution
  if(!isPerfectShare()) {
    if( isFunction() ) {
      for(int i=arity()-1 ; i>=0 ; i--) {
        getSubterm(i).rightsideVariableLiberationWithNoReuse();
      }
      if(!isShared()) {
        AllocVariable.free( termInfo.varNumber );
      }
    }
  }
}

public void rightsideVariableAffectation2() {
    // Affectation/Liberation
  if(!isPerfectShare()) {
    if(isFunction()) {
	/*
	 * l'ordre depend de l'ordre utilise lors de la generation du terme
	 */
      for(int i=0 ; i<arity() ; i++) {
        getSubterm(i).rightsideVariableAffectation2();
      }
	
      if(!isShared()) {
        termInfo.varNumber = AllocVariable.get();
      }

      for(int i=0 ; i<arity() ; i++) {
        Term subterm = getSubterm(i);
        if(!subterm.isPerfectShare() &&
           subterm.isFunction() &&
           !subterm.isShared()) {
          AllocVariable.free( subterm.termInfo.varNumber );
        }
      }
    } else if(isVariable()) {
	// where with pattern
      termInfo.varNumber = AllocVariable.get();
	//throw new InternalError("Probleme");
    }
  }
}
  
public void rightsideVariableLiberation2() {
  if(!isPerfectShare() && isFunction() && !isShared()) {
    AllocVariable.free( termInfo.varNumber );
  } else {
    rRightsideVariableLiberation2();
  }
}

private void rRightsideVariableLiberation2() {
    //System.out.println("*** rightsideVariableLiberation ***");
    // Restitution
  if(!isPerfectShare()) {
    if( isFunction() ) {
      for(int i=0 ; i<arity() ; i++) {
	  //	for(int i=arity()-1 ; i>=0 ; i--) {
        getSubterm(i).rRightsideVariableLiberation2();
      }
    }
  }
}

    /**
     * Recensement des variables sv utilisees
     */
public void markUsedVariable(BitSet b) {
    //System.out.println("\n\nmark : " + this);
  if( isPerfectShare() ) {
    Term leftside = termInfo.sharedTerm;
    if( leftside.termInfo.varNumber != -1) {
      b.set( leftside.termInfo.varNumber );
    }
  } else if(isVariableUnderAC() && termInfo.numberOfShare==0) {
    if( termInfo.varNumber != -1) {
      b.set( termInfo.varNumber );
    } else {
      throw new InternalError("Probleme");
    }
  } else if(!isMatchingVariable()) {
    if( termInfo.varNumber != -1) {
      b.set( termInfo.varNumber );
    } else {
      throw new InternalError("Probleme");
    }
  }
  for(int i=0 ; i<arity() ; i++) {
    getSubterm(i).markUsedVariable(b);
  }
}

    /*
     * Retourne le plus grand numero de variable utilise
     * en eliminant les variables de filtrage [mv]
     */
public int getMaxUsedVariable(int max) {
  if( isPerfectShare()) {
    Term leftside = termInfo.sharedTerm;
    if( !leftside.isMatchingVariable() ) {
      if( leftside.termInfo.varNumber != -1) {
        max=(leftside.termInfo.varNumber>max)?leftside.termInfo.varNumber:max;
      }
    } else {
	/* do nothing */
    }
  } else if(isVariableUnderAC() && termInfo.numberOfShare==0) {
    if( termInfo.varNumber != -1) {
      max=(termInfo.varNumber>max)?termInfo.varNumber:max;
    } else {
      System.out.println("Term = " + this);
      throw new InternalError("Internal Problem");
    }
  } else if(isUnderAC() && !isVariable()) {
      /*
       * Dans un lhs : les termes sous un symbole AC ne sont pas numerotes
       * sauf le premier sous-terme et les variables
       * Dans un rhs : il n'y a pas cette restriction
       */
    max=(termInfo.varNumber>max)?termInfo.varNumber:max;
  } else if(!isMatchingVariable()) {
    if( termInfo.varNumber != -1) {
      max=(termInfo.varNumber>max)?termInfo.varNumber:max;
    } else {
      System.out.println("Term = " + this);
      throw new InternalError("Internal Problem");
    }
  }
  for(int i=0 ; i<arity() ; i++) {
    int r = getSubterm(i).getMaxUsedVariable(max);
    max=(r>max)?r:max;
  }
  return max;
}

public int getMaxSubstIndex(int max) {
  Term leftside = this;
  if(isPerfectShare()) {
    leftside = termInfo.sharedTerm;
  }
  if(isVariable()) {
    if(leftside.termInfo.substIndex>max) {
      max = leftside.termInfo.substIndex;
    }
  } else {
    for(int i=0 ; i<arity() ; i++) {
      int r = getSubterm(i).getMaxSubstIndex(max);
      max=(r>max)?r:max;
    }
  }
  return max;
}

    /**
     * Recuperation d'un terme
     */

public String genCore() {
  String s;
  Term leftside;
  if(isPerfectShare()) {
    leftside = termInfo.sharedTerm;
    if( leftside.isLeftsideVariableUnderAC() ||
        (leftside.isVariable() && !leftside.isMatchingVariable())
        ) {
      s = "sv[" + leftside.termInfo.varNumber + "]";
    } else if( leftside.isVariable() && leftside.isMatchingVariable()) {
      s = "v" + leftside.termInfo.varNumber;
    } else {
      throw new InternalError("problem in genCore");
    }
  } else {
    if( isVariableUnderAC() ||
	  // SOLUTION PROVISOIRE POUR LES WHERES
        (isVariable() && !isMatchingVariable())
        ) {
      s = "sv[" + termInfo.varNumber + "]";
    } else if( isVariable() && isMatchingVariable()) {
      s = "v" + termInfo.varNumber;
    } else if( arity() != 0 && !isMatchingVariable()) {
      s = "sv[" + termInfo.varNumber + "]";
    } else if( arity() != 0 && isMatchingVariable()) {
      s = "v" + termInfo.varNumber;
    } else if( isConstant() && getSymbol().isConstructor() ) {
      if(getSymbol().isValue()) {
        s = "(" + ((SymbolValue)getSymbol()).setTag() +
          "(" + getSymbol().getSymbolValue() + "))";
	  //s = "(setTag(" + getSymbol().getSymbolValue() + "))";
      } else { 
        s = "con_" + getSymbolCode();
      }
    } else {
	//s = "fun_" + getSymbolCode() + "()");
      s = "sv[" + termInfo.varNumber + "]";
    }
  }
  return s;
}

    /**
     * Recuperation de substitution[]
     * Utilise dans le traitement des variable Dio et
     * dans les matching condition AC
     */ 
public void genVariableInstantiation(OutputCode s,int deep) {
  Term leftside=this;
  if(isPerfectShare())
    leftside = termInfo.sharedTerm;

    /* 
       System.out.println("t=" + this +
       "\tisVariableUnderAC=" + isVariableUnderAC() +
       "\tisDioVariable=" + isDioVariable());
       */

  if(leftside.isLeftsideVariableUnderAC() //&& !leftside.isDioVariable()
     && !leftside.isLeftsideInstantiated()) {
    int index = leftside.termInfo.substIndex;
    leftside.setInstantiated();
    s.write(deep, genCore() + "=substitution[" + index + "];\n");
    if(!isBuiltinSort()) { 
      s.write(deep, "setShared(" + genCore() + ");\n");
    }
  } else {
    for(int i=0 ; i<arity() ; i++) {
      getSubterm(i).genVariableInstantiation(s,deep);
    }
  }
}

    /**
     * Instanciation des dioVariables
     * A appliquer sur leftside
     */
public void genDioMatching(OutputCode s,int deep, int number, boolean detRule) {
  if(!isDio()) {
    for(int i=0 ; i<arity() ; i++) {
      getSubterm(i).genDioMatching(s,deep,number, detRule);
    }
  }
    
    /*
     * pattern de la forme f(Y,X^n)
     */
  if(numberOfVariableUnderAC()==2) {

    genVariableInstantiation(s,deep);

      //getSubterm(0).setInstantiated();
      //getSubterm(1).setInstantiated();

      /*
       * Si une variable d'extension est ajoutee, elle a une multiplicite 1
       * Elle a generalement un numero plus grand que les autres
       * Il faut la reconnaitre parce qu'elle doit pouvoir etre instanciee
       * par le terme vide
       */

    String subject, list_x, list_y;
    int maxMult;

    int r=0;
    if(getSubterm(0).multiplicity > getSubterm(1).multiplicity) {
      r=1;
    } else if(getSubterm(0).multiplicity < getSubterm(1).multiplicity) {
      r=-1;
    } else if(getSubterm(0).cmp(getSubterm(1))==1) {
      r=-1;
    } else if(getSubterm(0).cmp(getSubterm(1))==-1) {
      r=1;
    } else {
      throw new InternalError("variables are not comparable");
    }
      
    if(r==1) {
      list_x = getSubterm(0).genCore();
      list_y = getSubterm(1).genCore();
      maxMult = getSubterm(0).multiplicity;
    } else {
      list_x = getSubterm(1).genCore();
      list_y = getSubterm(0).genCore();
      maxMult = getSubterm(1).multiplicity;
    }

    subject = list_x;
      
    s.write(deep,"{\n");
    s.write(deep+1,"int *E,*sol;\n");
    s.write(deep+1,"int nb_arg_subject;\n");
    s.write(deep+1,"int no_arg_subject;\n");
    s.write(deep+1,"int indice=0;\n");
    s.write(deep+1,"struct cell_term *cell;\n");
    s.write(deep+1,"struct cell_term *copy_cell;\n");

    s.write(deep+1,"for(cell=term_first(" + subject + "), nb_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), nb_arg_subject++);\n");
      /*
       * il faut mettre long a la place de int sinon
       * il y a un plantage dans le BDW GC
       */
    s.write(deep+1,"E=(int*)AMALLOC(nb_arg_subject*sizeof(long));\n");
    s.write(deep+1,"sol=(int*)AMALLOC(nb_arg_subject*sizeof(long));\n");
      //s.write(deep+1,"E=(int*)AMALLOC(nb_arg_subject*sizeof(int));\n");
      //s.write(deep+1,"sol=(int*)AMALLOC(nb_arg_subject*sizeof(int));\n");

    s.write(deep+1,"for(cell=term_first(" + subject + "), no_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), no_arg_subject++) {\n");
    s.write(deep+2,"E[no_arg_subject]=getMult(cell);\n");
    s.write(deep+2,"sol[no_arg_subject]=0;\n");
    s.write(deep+1,"}\n");

      // version classique
      /*
        s.write(deep,"chp" + number + ":\n");
        if(maxMult > 1 ) {
        s.write(deep+1,"indice=next_pe_extract2(0,nb_arg_subject-1,E,sol," + maxMult + ");\n");
	
        s.write(deep+1,"if(indice > 0 ) {\n");
        } else {
        s.write(deep+1,"indice=next_pe_extract(nb_arg_subject-1,E,sol," + maxMult + ");\n");
        s.write(deep+1,"if(indice >= 0 ) {\n");
        }
      
        s.write(deep+2,"if(setChoicePoint()) {\n");
        s.write(deep+3,"// Ask for the next solution \n");
        s.write(deep+3,"goto chp" + number + ";\n");
        s.write(deep+2,"}\n");
        */

      // version utilisant extract_fail
    s.write(deep+1,"if(" + ((maxMult>1)?"maximal":"minimal") +
            "_extract_fail(nb_arg_subject-1,E,sol," + maxMult + ")) {\n");

      // partie commune aux deux versions
    s.write(deep+2,"setShared(" + list_x + ");\n");

      // Le contexte est dans la 1ere variable
    s.write(deep+2,"extract_xy_from_pe(" + subject + ",E,sol," +
            maxMult + ",&" + list_x + ",&" + list_y + ");\n");

    s.write(deep+2,"// printf(\"list_x = " + list_x + " = \"); ");
    s.write(deep+2,"term_printnl(stdout," + list_x + ");\n");
    s.write(deep+2,"// printf(\"list_y = " + list_y + " = \"); ");
    s.write(deep+2,"term_printnl(stdout," + list_y + ");\n");


      /*
       * il faut savoir si y peut etre vide ou non
       * cas F(y,x^n)   : y!=0 si F est un sous-terme ou regle nommee
       * cas F(y,x^n,t) : y!=0 si F est un sous-terme ou regle nommee
       * cas F(y,x)     : y!=0
       * cas F(y,x,t)   : y est une variable d'extension
       */
    if( (maxMult >  1 && getFather()!=null) || 
        (maxMult == 1 && numberOfVariableUnderAC()==arity()) ) {
      s.write(deep+2,"if(term_first(" + list_y + ")==NULL) {\n");
      s.write(deep+3,"/* Avoid an empty context */\n");
      Tools.genFail(s,deep+3);
      s.write(deep+2,"}\n");
    }
    s.write(deep+1,"} else {\n");
    s.write(deep+2,"/*  There is no more solution */\n");
//    [pem: Apr 16 99]
    if(Flags.optimiseChoicePoint && detRule) {
      s.write(deep+2,"goto myend" + number + ";\n");
    } else {
      Tools.genFail(s,deep+2);
    }
    s.write(deep+1,"}\n");
    s.write(deep,"}\n");

  } else if(numberOfVariableUnderAC()==1 && getSubterm(0).multiplicity>1) {
      /*
       * pattern de la forme f(X^n)
       */
    genVariableInstantiation(s,deep);
    String subject = getSubterm(0).genCore();
    s.write(deep,"if(!is_maximal_identical_element(" + subject + "," +
            getSubterm(0).multiplicity + ",&" + subject + ")) {\n");
    Tools.genFail(s,deep+1);
    s.write(deep,"}\n");
  }
}
 
    /*
     * marquage des variables utilisees plusieurs fois dans le rhs
     * Rq : cela prend en compte les conditions
     */
public void genSetShared(OutputCode s,int deep) {
  if( isNotRhsLinear() && !isBuiltinSort() ) {
      //      System.out.println("leftside = " + this + "[" + isBuiltinSort() + "]");
    s.write(deep,"setShared(" + genCore() + ");\n");
  } else {
    for(int i=0 ; i<arity() ; i++) {
      getSubterm(i).genSetShared(s,deep);
    }
  }
}


    /**
     * Construction de la partie droite
     */
public void genRightside(OutputCode s,int deep) {
  rgenRightside(s,deep);
  s.write(deep,"res = " + genCore() + " ;\n");
}


    //private static int lineSplit = 1000 ;
    /**
     * Construction recursive de la partie droite
     */
public void rgenRightside(OutputCode s,int deep) {
  if( !isPerfectShare() ) {
    if(isFunction()) {
      if(arity()!=0 || (isConstant() && !getSymbol().isConstructor()) ) {
        for(int i=0 ; i<arity() ; i++) {
          getSubterm(i).rgenRightside(s,deep);
        }
        if(getSymbol().isConstructor()) {
          genNoShared(s,deep);
        } else {
          if(isAC()) {
	      // Appel d'une fonction AC
            genNoShared(s,deep);
            s.write(deep, "sv[" + termInfo.varNumber  + "]" + 
                    " = fun_" + getSymbolCode() + "( " + genCore() + " );\n");
          } else {
	      // Cas syntaxique
            s.write(deep, "sv[" + termInfo.varNumber  + "]" +
                    " = fun_" + getSymbolCode() + "( ");
            for(int i=0 ; i<arity() ; i++) {
              s.write( getSubterm(i).genCore() );
              if(i<arity()-1) {
                s.write(",");
              }
            }
            s.write(" );\n");
              /*
               * [pem: Feb 11 00]
               * Placer l'appel a normalize par ici
               */
          }
        }
      }
    }
  } else if( isPerfectShare() ) {
      // Traitement des variables apparaissant dans un pattern AC
    Term leftside = termInfo.sharedTerm;

    if(leftside.isLeftsideVariableUnderAC() && !leftside.isLeftsideInstantiated()) {
      int index = leftside.termInfo.substIndex;
      leftside.setInstantiated();
      s.write(deep, genCore() + "=substitution[" + index + "];\n");
      if(!isBuiltinSort()) {
        s.write(deep, "setShared(" + genCore() + ");\n");
      }

      if(isVariableDirectlyUnderAC() && !isExtensionVariable()
         && leftside.getFather().arity()>2) {
        s.write(deep,"if(term_first(" + genCore() + ")==NULL) {\n");
        s.write(deep+1,"/* Avoid an empty instance */\n");
        Tools.genFail(s,deep+1);
        s.write(deep,"}\n");
      }

    }

    if( isReducibleVariable() && !leftside.isReduced() 
        && !leftside.isUnderConstructor()) {
	// normalisation eventuelle
      leftside.setReduced();
      s.write(deep,"tmp = term_removeTopSymbol(" + genCore() + ");\n");
      s.write(deep,"if(tmp == NULL)\n");
      if(Flags.color) {
        s.write(deep+1,"if(!isMonoColor(" + genCore() + ")) {\n");
        s.write(deep+2,genCore() + "=fun_" +
                leftside.getFather().getSymbolCode() + 
                "( " + genCore() + " );\n");
        s.write(deep+1,"} else {\n");
        s.write(deep+2,"// printf(\"monoColor: \"); term_printnl(stdout," +
                genCore() + ");\n");
        s.write(deep+1,"}\n");

      } else {
        s.write(deep+1,genCore() + "=fun_" +
                leftside.getFather().getSymbolCode() + 
                "( " + genCore() + " );\n");
      }
      s.write(deep,"else\n");
      s.write(deep+1,genCore() + "=tmp;\n");


    } else if((isVariableDirectlyUnderAC() || isReducibleVariable())
		// isReducibleVariable() est redondant
              && !leftside.isReduced()) {
	// elimination eventuelle du symbole de tete
      leftside.setReduced();
      s.write(deep,"tmp = term_removeTopSymbol(" + genCore() + ");\n");
      s.write(deep,"if(tmp != NULL)\n");
      s.write(deep+1,genCore() + "=tmp;\n");
    }
      
      /*
       * marquage des variables non lineaires droite
       */
    if(leftside.isNotRhsLinear() && !leftside.isBuiltinSort() &&
       !leftside.isSharedGenerated()) {
	// C'est le setShared active !!!
	// System.out.println("leftside = " + this + "[" + isBuiltinSort() + "]");
      s.write(deep,"setShared(" + genCore() + ");\n");
      leftside.setSharedGenerated();
    }
  }
}

    /**
     * Construction d'un terme pour l'outil de filtrage AC
     */
public void genTermForACMatcher(OutputCode s,int deep) {
  s.write(deep,"/* AC pattern construction phase */\n");
  rgenTermForACMatcher(s,deep,0);
}

    /**
     * Construction recursive d'un terme pour l'outils de filtrage AC
     */
private int rgenTermForACMatcher(OutputCode s,int deep,
                                 int varIndex) {
  String typeName;
  if(isVariable()) {
    Term t=this;
    if(isPerfectShare()) {
      t=termInfo.sharedTerm;
    }
      /*
       * Dans tous les cas il faut numeroter la variable
       */
    if(t.termInfo.ekerVarNumber<0) {
      t.termInfo.ekerVarNumber=varIndex++;
    }
      /*
        System.out.println("varNumber = " + t.termInfo.varNumber);
        System.out.println("ekerVar   = " + t.termInfo.ekerVarNumber);
        */
    typeName="VAR";
    s.write(deep,"sv[" + t.termInfo.varNumber +
            "] = (struct term*) make_term(" + t.termInfo.ekerVarNumber +
            ",NULL," + typeName + ");\n");
  } else if(arity()==0) {
    if(isBuiltinSort()) {
      typeName="BUILTIN";
      s.write(deep,"sv[" + termInfo.varNumber +
              "] = (struct term*) make_term((long)" +
              ((SymbolValue)getSymbol()).setTag() +
              "(" + getSymbol().getSymbolValue() + ")" +
              ",NULL," + typeName + ");\n");
    } else if(isConstant() && getSymbol().isConstructor()) {
      typeName="CONSTAN";
      s.write(deep,"sv[" + termInfo.varNumber +
              "] = (struct term*) make_term(" + 
              getSymbolCode() + ",NULL," + typeName + ");\n");
    } else {
      throw new InternalError("Lhs contains a defined constant:" + this);
    }
  } else {
    int varListNumber=0;
    for(int i=0 ; i<arity() ; i++) {
      varIndex=getSubterm(i).rgenTermForACMatcher(s,deep,varIndex);
      Term subterm=getSubterm(i);
      if(subterm.isPerfectShare()) {
        subterm=subterm.termInfo.sharedTerm;
      }
      int varNumber=subterm.termInfo.varNumber;
      if(!isAC()) {
        if(i==0) {
          varListNumber=AllocVariable.get();
          s.write(deep,"vlist[" + varListNumber +
                  "] = make_term_list((TERM*) sv[" + varNumber +
                  "],(TERM_LIST *) NULL);\n");
        } else {
          s.write(deep,"vlist[" + varListNumber + 
                  "] = make_term_list((TERM*) sv[" + varNumber +
                  "],vlist[" + varListNumber + "]);\n");
        }
      } else {
        if(i==0) {
          varListNumber=AllocVariable.get();
          s.write(deep,"acvlist[" + varListNumber +
                  "] = make_ac_list((TERM*) sv[" + varNumber +
                  "]," + getSubterm(i).getMultiplicity() + ",(AC_LIST *) NULL);\n");
        } else {
          s.write(deep,"acvlist[" + varListNumber + 
                  "] = make_ac_list((TERM*) sv[" + varNumber +
                  "]," + getSubterm(i).getMultiplicity() + ",acvlist[" + varListNumber + "]);\n");
        }

      }
    }
    if(!isAC()) {
      s.write(deep,"sv[" + termInfo.varNumber +
              "] = (struct term*) make_term(" + getSymbolCode() +
              ",vlist[" + varListNumber + "],FUNC);\n");
    } else {

      s.write(deep,"sv[" + termInfo.varNumber +
              "] = (struct term*) make_ac_term(" + getSymbolCode() +
              ",acvlist[" + varListNumber + "],ACFUNC);\n");
    }
    AllocVariable.free(varListNumber);
  }
  return varIndex;
}
    
    /*
     * Compilation des wheres avec pattern
     */

  public boolean containsOnlyFreshVariable() {
    boolean res=true;
    
    if(isVariable()) {
        /* do nothing */;
        if(isPerfectShare()) {
          System.out.println("Warning: " + this + " is not ``fresh''");
          return false;
        }
    } else {
      for(int i=0 ; res && i<arity() ; i++) {
        res &= getSubterm(i).containsOnlyFreshVariable();
      }
    }
    return res;
  }
  
public void genOneToOneMatching(OutputCode s, int deep) {
    // the current term is the pattern
    // Le terme a filtrer est contenu dans le membre gauche !
  if(isVariable()) {
      /* do nothing */;
  } else if(containsAC()) { 
      /*
       * nbVar est surdimensionne parce que les variables identiques
       * sont comptes plusieurs fois
       */
    int nbVar=nbVariable();
    s.write(deep,"{\n");
      /* construction du pattern */
      //s.write(deep+1,"/* TODO: modifier la taille du tableau */\n");
      //s.write(deep+1,"TERM_LIST *vlist[10];\n");
      //s.write(deep+1,"AC_LIST *acvlist[10];\n");
    s.write(deep+1,"match_state *ACmatch;\n");
    s.write(deep+1,"/* TODO: modifier la taille du tableau */\n");
    s.write(deep+1,"TERM *assignment[" + nbVar + "];\n");
    s.write(deep+1,"int i;\n");
    s.write(deep+1,"for(i=0 ; i<" + nbVar + " ; i++) assignment[i]=NULL;\n");

      //genTermForACMatcher(s,deep+1);

    PatternCode pattern = PatternCode.get(this);
    if(pattern == null) {
      pattern = new PatternCode(this);
      pattern.put(this);
    }
    s.write(deep+1,"/* pattern number " + pattern.getNumber() + " */\n");
    s.write(deep+1,"sv[" + termInfo.varNumber + 
            "] = (struct term*) EkerTerm[" + pattern.getNumber() + "];\n");

      /* filtrage AC */
    s.write(deep+1,"/* TODO: modifier le nb de variables */\n");
    s.write(deep+1,"tmp = (struct term*)toEkerForm(tmp);\n");
    s.write(deep+1,"ac_sort((TERM*)tmp);\n");

    s.write(deep+1,"/*\n");
    s.write(deep+1,"printf(\"sv  = \"); eker_print_term((TERM*)" +
            genCore() + ");\n");
    s.write(deep+1,"printf(\"\\n\");\n");
    s.write(deep+1,"printf(\"tmp  = \"); eker_print_term((TERM*)tmp);\n");
    s.write(deep+1,"printf(\"\\n\");\n");
    s.write(deep+1,"printf(\"\\n\");\n");
    s.write(deep+1,"*/\n");

    s.write(deep+1,"ACmatch = build_match((TERM*)" +
            genCore() + ",(TERM*)tmp," + nbVar + ");\n");


    s.write(deep+1,"while(1) {\n");
    s.write(deep+2,"if(!extract_match(ACmatch, assignment)) {\n");
    s.write(deep+3,"destroy_match(ACmatch);\n");
    Tools.genFail(s,deep+3);
    s.write(deep+2,"}\n");
    if(Flags.debug) {
      s.write(deep+2,"// printf(\"setChoicePoint\\n\");\n");
    }
    Tools.genComment(s,deep+2,"choicePoint Eker AC matching");
    s.write(deep+2,"if(!setChoicePoint()) {\n");
    s.write(deep+3,"break;\n");
    s.write(deep+2,"}\n");
    if(Flags.debug) {
      s.write(deep+2,"// printf(\"on revient d'un fail\\n\");\n");
    }
    s.write(deep+1,"}\n");

      /*
        s.write(deep+1,"if(extract_match(ACmatch, assignment)) {\n");
        s.write(deep+2,"printf(\"Au moins une solution\\n\");\n");
        s.write(deep+1,"} else {\n");
        s.write(deep+2,"printf(\"Pas de solution\\n\");\n");
        s.write(deep+1,"}\n");
          //s.write(deep+1,"destroy_match(ACmatch);\n");
          */

      /* recopie de assignment[] dans substitution[] */
    genAssignment2Substitution(s,deep+1);
    genVariableInstantiation(s,deep+1);

    s.write(deep,"}\n");
  } else {
    s.write(deep,"if(" + getSymbol().genInstance() +
            " != " + getSymbol().genAccess() +"(sv[" +
            termInfo.varNumber + "])) {\n");
    Tools.genFail(s,deep+1);
    if(arity()>0) {
      s.write(deep,"} else {\n");
      for(int i=0 ; i<arity() ; i++) {
        int number=-1;
        if(getSubterm(i).isPerfectShare()) {
          number=getSubterm(i).termInfo.sharedTerm.termInfo.varNumber;
        } else {
          number=getSubterm(i).termInfo.varNumber;
        }

        s.write(deep+1,"sv[" + number + "] = " + 
                genCore() + "->sub[" + i + "];\n");
        getSubterm(i).genOneToOneMatching(s,deep+1);
      }
    }
    s.write(deep,"}\n");
  }
}

    /**
     * Recuperation de substitution[] a partir d'assignment[]
     */ 
private void genAssignment2Substitution(OutputCode s,int deep) {
  if(isVariable()) {
    if(isUnderAC()) {
      if(!isPerfectShare()) {
        s.write(deep,"substitution[" + termInfo.substIndex +
                "]=fromEkerForm(assignment[" + 
                termInfo.ekerVarNumber + "]);\n");
        s.write(deep,"destroy_term(assignment[" +
                termInfo.ekerVarNumber + "]);\n");
        if(termInfo.ekerVarNumber<0) {
          throw new InternalError("pattern not numbered");
        }
      }
    } else {
      s.write(deep,genCore() + "=fromEkerForm(assignment[" + 
              termInfo.ekerVarNumber + "]);\n");
      s.write(deep,"destroy_term(assignment[" +
              termInfo.ekerVarNumber + "]);\n");
      if(termInfo.ekerVarNumber<0) {
        throw new InternalError("pattern not numbered");
      }
    }
  } else {
    for(int i=0 ; i<arity() ; i++) {
      getSubterm(i).genAssignment2Substitution(s,deep);
    }
  }
}

    /**
     * Creation d'un terme
     */
public String getCoreNumber() {
  String s="";
  Term leftside;
  if(isPerfectShare()) {
    leftside = termInfo.sharedTerm;
    s += leftside.termInfo.varNumber;
  } else {
    if( isConstant() && getSymbol().isConstructor() ) {
      if(getSymbol().isValue()) {
	  //s = getSymbol().getSymbolValue();
        s += "bicolor";
      } else { 
        s += getSymbolCode();
      }
    } else {
      s += termInfo.varNumber;
    }
  }
  return s;
}


public void genNoShared(OutputCode s,int deep) {
  int arity=arity();
  if(isAC()) {
    arity=2;
  }

  if(Flags.strat >= 3 && isStrategy()) {
    s.write(deep,"TERM_ALLOC(sv[" + termInfo.varNumber  + "]" + ",term" + 
            (int)(arity+1) + ",code_" + getSymbolCode() + ");\n");
    Tools.indent(s,deep);
    s.write(deep,"sv[" + termInfo.varNumber  + "]" + "->sub[" +
            arity + "] = NULL /* #proc */;\n");
  } else {
    int maxNumberMacro = 10;
    if(termInfo.varNumber < maxNumberMacro) {
      s.write(deep,"TERM_ALLOC(sv[" + termInfo.varNumber  + "]" +
              ",term" + arity + ",code_" + getSymbolCode() + ");\n");
    } else {
      s.write(deep,"term_alloc(&sv[" + termInfo.varNumber  + "]" +
              ",sizeof(struct term" + arity + "),code_" + getSymbolCode() + ");\n"); 
    }
  }

    //Peter, THIS part is bizard !!!
  if(isAC()) {
    int compteur=1;
    for(int i=0 ; i<arity() ; i++) {
      for(int j=0 ; j<getSubterm(i).multiplicity ; j++) {
        if(Flags.color) {
          String color = getSubterm(i).getCoreNumber();
          s.write(deep,"term_add_onf_term_color(sv[" + termInfo.varNumber  + "]" +
                  "," + getSubterm(i).genCore() + "," + compteur++ +");\n");
        } else {
          s.write(deep,"term_add_onf_term(sv[" + termInfo.varNumber  + "]" +
                  "," + getSubterm(i).genCore() + ");\n");
        }
      }
    }
  } else {
    if(isFree() || isConstructor()) {
	// isConstructor(): for builtins
      for(int i=0 ; i<arity() ; i++) {
        s.write(deep,"sv[" + termInfo.varNumber  + "]" + "->sub[" +
                i + "] = " + getSubterm(i).genCore() + ";\n"); 
      }
        /*
         * [pem: Feb 11 00]
         * Placer l'appel a normalize par ici
         * si getSymbolCode() correspond a un symbole de tete
         *  de la strategie normalise
         */
    }
  }
    //Peter, up to here, discute with Pierrot	

    /*
     * marquage des variables non lineaires droite
     */
    
  for(int i=0 ; i<arity() ; i++) {
    if(getSubterm(i).isPerfectShare()) {
      Term leftside = getSubterm(i).termInfo.sharedTerm;
      if(leftside.isNotRhsLinear() && !leftside.isBuiltinSort() &&
         !leftside.isSharedGenerated()) {
        Tools.indent(s,deep); 
        s.write("setShared2(" + getSubterm(i).genCore() + ");\n");
        leftside.setSharedGenerated();
      }
    }
  }


}

    /**
     * Declaration des variables v utilisees
     */
public static void genLeftsideDeclaration(OutputCode s, int deep,
                                          int startVarNumber, 
                                          int maxVarNumber) {
  String prefix = Tools.indent(deep) + "struct term ";
  boolean declaration=false;

  for(int i=startVarNumber ; i<=maxVarNumber ; i++) {
    s.write(prefix + "*v"+i);
    prefix=",";
    declaration=true;
  }
  if(declaration) {
    s.write(";\n");
  }
}

    /**
     * Cette fonction s'applique sur une variable pour creer le chemin
     * ...->sub[i]->...->sub[k]
     * qui va de la racine (si elle n'est pas AC) jusqu'a la variable
     * genPathAcces applique a `x' de F(g(a,f(x))), retourne
     * sub[1]->sub[0]
     */ 

private String genPathAccess() {
  return genPathAccess(null);
}

private String genPathAccess(Term root) {
  StringBuffer s = new StringBuffer();
  if( father == null || father == root) {
    s.append("");
  } else {
    if(father.isAC()) {
      s.append("");
    } else {
      s.append( father.genPathAccess() + "->sub[" + getChildNumber() + "]" );
    }
  }
  return s.toString();
}

public void genVariableExtract(OutputCode s,int deep, String name) {
  if(isVariable()) {
    s.write(deep,"extract_substitution[*indice]=v0" +
            genPathAccess() + ";\n");
    s.write(deep,"(*indice)++;\n");
  } else if(isFree()) {
    for(int i=0 ; i<arity() ; i++) {
      getSubterm(i).genVariableExtract(s,deep,name);
    }
  } else if(isAC()) {
    int nbVar=nbVariable();
      //int nbACVar=(nbVariableUnderACSymbol()>0)?1:0;
    int nbACVar=nbVariableUnderACSymbol();
    s.write(deep,"{\n");
    s.write(deep+1,"/* Not tested */\n");
    s.write(deep+1,"int nb_variable=" + nbVar + ";\n");
    s.write(deep+1,"int nb_variable_ac=" + nbACVar + ";\n");
    s.write(deep+1,"struct term *substitution[" + nbVar + "];\n");
    s.write(deep+1,"LINK *link;\n");
    s.write(deep+1,"match_state *msbg;\n");
    s.write(deep+1,"int i;\n");
    s.write(deep+1,"//link=BG_link_get(ms->cbg,base_id_pattern+id_pattern);\n");

    s.write(deep+1,"if(1 || link==NULL) {\n");
    s.write(deep+2,"for(i=0 ; i<nb_variable ; i++) {\n");
      //      s.write(deep+3,"extract_substitution[*indice]=v0;\n");
    String varName = "v0" + genPathAccess();
    s.write(deep+3,"extract_substitution[*indice]=" + varName + ";\n");
    s.write(deep+3,"(*indice)++;\n");
    s.write(deep+2,"}\n");
    s.write(deep+1,"} else {\n");
    s.write(deep+2,"msbg=LINK_get(link,no_arg_subject);\n");
    s.write(deep+2,"substitution_build(" + varName +
            ",msbg,nb_variable,substitution,nb_variable_ac,variable_extract" +
            name + ",base_id_pattern);\n");
    s.write(deep+2,"for(i=0 ; i <nb_variable ; i++) {\n");
    s.write(deep+3,"extract_substitution[*indice]=substitution[i];\n");
    s.write(deep+3,"setShared(substitution[i]);\n");
    s.write(deep+3,"(*indice)++;\n");
    s.write(deep+2,"}\n");
    s.write(deep+1,"}\n");
    s.write(deep,"}\n");
  }
}

    /**
     * Compte le nombre de variables directement sous un symbol AC
     */ 
public int nbVariableUnderACSymbol() {
  int res=0;
  if(isAC()) {
    for(int i=0 ; i<arity() ; i++) {
      if(getSubterm(i).isVariable()) {
        res++;
      }
    }
  } else {
    for(int i=0 ; i<arity() ; i++) {
      res = res + getSubterm(i).nbVariableUnderACSymbol();
    }
  }
  return res;
}

    /*
      public int nbVariableUnderACSymbol() {
      int res=0;
      if( isVariable() && father.isAC() ) {
      res = 1;
      } else {
      for(int i=0 ; i<arity() ; i++) {
      res = res + getSubterm(i).nbVariableUnderACSymbol();
      }
      }
      return res;
      }
      */

    /**
     * Nombre de variables differentes apparaissant dans un terme
     * Nombre de variables apparaissant dans un terme
     */
public int nbVariable() {
  int res=0;
    // PEM 29.10.98 : if( isVariable()) {
    // PEM 06.11.98 : if( isVariable() && !isPerfectShare()) {
  if( isVariable()) {
    res = 1;
  } else {
    for(int i=0 ; i<arity() ; i++) {
      res = res + getSubterm(i).nbVariable();
    }
  }
  return res;
}


public boolean usedContextInRhs() {
  if(isAC()) {
    for(int i=0 ; i<arity() ; i++) {
      if(getSubterm(i).isVariable() && getSubterm(i).termInfo.numberOfShare>0) {
        return true;
      }
    }
  } else {
    throw new InternalError("not an AC term");
  }
  return false;
}


    /*
     * C'est quoi ce bordel ???
     */
    // Peter
public boolean isStrategy() {
  if (ElanConstants.isPrimalSymbol(-getSemantic()))
    return true;
  else if ( (symbol instanceof SymbolLab) ||
            (symbol instanceof SymbolFsym) ||
            (symbol instanceof SymbolDstr) )
    return true;
  else
    return false;
}
    // Peter

public int getMaxVariableNumber(int max) {
  if(isVariable()) {
    int code = getSymbol().getCode();
    if(code>max) {
      max = code;
    }
  } else {
    for(int i=0 ; i<arity() ; i++) {
      max = getSubterm(i).getMaxVariableNumber(max);
    }
  }
  return max;
}


public void linearise(LineariseCondition linearConditions) {
  Vector variables = new Vector();
  if(linearConditions.getConditions().size()>0) {
    System.out.println("Warning: initial linearConditions is not empty");
  }
    // System.out.println("\n\n\nDebut linearise on " + this);
  rlinearise(linearConditions, variables);
    // System.out.println("\n\n\nFin linearise");
    /*
  if(linearConditions.getConditions().size()>0) {
    System.out.println("linearise term: " + this);
  }
    */
}

private void rlinearise(LineariseCondition linearConditions,
                        Vector variables) {
  if( isVariable() ) {
      /*
       * Recherche de la variable
       */
    boolean occurVariable=false;
    int indexVariable=0;
    for( int i=0 ; !occurVariable && i<variables.size() ; i++ ) {
      Symbol symb = ((Term)variables.elementAt(i)).getSymbol();
      if( symb.equals(getSymbol()) ) {
        occurVariable = true;
        indexVariable = i;
      }
    }

//    if( occurVariable ) {
    if( occurVariable && !isLeftsideVariableDirectlyUnderAC()) {
        /*
         * [pem: May 19 00]
         * la condition suivante pose des problemes avec le pattern
         * F(g(x),x) => ...
         * 
         */
	/*
	 * Create a new variable
	 */
      int max = linearConditions.getMaxVariableNumber()+1;
      SymbolVariable symb = ((SymbolVariable) getSymbol()).copyRename(max);
      linearConditions.updateMaxVariableNumber(max);
      	//System.out.println("Create a new variable : " + symb);
	/*
	 * Modification of the variable symbol
	 */
      this.symbol=symb;
	/*
	 * Add a conditon
	 */
      Vector subterms = new Vector(2);
	/*
	 * une copies est necessaire pour ne pas modifier
	 * le pere de la variable du lhs
	 */
      Term foundVariable = (Term) variables.elementAt(indexVariable);
      subterms.addElement(foundVariable.deepCopy());
      subterms.addElement(this.deepCopy());
      int idFsym;
      if(this.getSymbol().getSort().isSortInteger()) {
        idFsym = 8; // Code of equality for builtinInt
      } else {
        idFsym = 18; // Code of equality for terms
      }
      Symbol eqSymbol=Symbol.get(idFsym);
      if(eqSymbol == null) {
        System.out.println("You need to import the equality module on sort: "
+ this.getSymbol().getSort().getName());
        throw new InternalError("symbol sym" + idFsym + " not found");
      }
      Term eqTerm = new Term(eqSymbol , subterms);
      Condition cond = new Condition( eqTerm );
      linearConditions.addCondition( cond );
    } else {
      variables.addElement(this);
    }
  } else {
    for(int i=0 ; i<arity() ; i++) {
      getSubterm(i).rlinearise(linearConditions,variables);
    }
  }
}

public String toString() {
  StringBuffer s = new StringBuffer();
    
  s.append( symbol );
    //s.append( symbol.toString(subterms) );

  if(multiplicity>1) {
    s.append("[" + multiplicity + "]");
  }

//    s.append( termInfo );

    
  if( subterms != null && subterms.size()>0 ) {
    int i;
    s.append("(");
    for(i=0 ; i<subterms.size()-1 ; i++) {
      s.append( subterms.elementAt(i) );
      s.append( "," );
    }
    s.append( subterms.elementAt(i) );
    s.append(")");
  }
  return s.toString();
}
  
} // end class




