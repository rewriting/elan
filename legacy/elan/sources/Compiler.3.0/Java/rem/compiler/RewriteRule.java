package rem.compiler;

import java.util.*;
import rem.exception.*;

public class RewriteRule {
  private Term leftside;           // membre gauche
  private Term rightside;          // membre droit
  private BranchEvaluation branch; // local evaluations

  private int maxVariableNumber;   // nombre de variables de la regle
  
  private Lexem name;
  private Lexem sort;

  // table : symbole de tete <--> vecteur de regles
  //    ou : nom de regle    <--> vecteur de regles 
  private static Map map = new TreeMap();

  /**
   * to know if local evaluations are deterministic 
   */
  private DetType detTypeEvaluation;
  static private int clear=0;
  static private int start=1;
  static private int done=2;
  private int detTypeState=clear;

    /**
     * numero du matchState associe a la regle
     */
  public int matchStateNumber=-1;
  
  public RewriteRule(Lexem ruleName,
                     Lexem ruleSort, 
		     Term lhs,
                     Term rhs,
                     BranchEvaluation localEvaluations,
                     int maxVarNumber) {
    name = ruleName;
    sort = ruleSort;
    leftside = lhs;
    if(!isNamed()) {
      leftside.clearConstructor();
    }
    rightside = rhs;
    branch=localEvaluations;
    maxVariableNumber = maxVarNumber;
  }

  public Term getLeftside() {
    return leftside;
  }

  public BranchEvaluation getBranch() {
    return branch;
  }
  public void setBranch(BranchEvaluation b) {
    branch = b;
  }
  
  public int getBranchEvaluationSize() {
    return getBranch().size();
  }
  
  
  /**
   * retourne les regles associees a un symbole de tete
   */
  public static Vector getTable(Object key) {
    return (Vector) map.get(key);
  }

  public static Iterator valuesIterator() {
    return map.values().iterator();
  }

  public boolean isNamed() {
    return name!=null;
  }

  public String toString() {
    String s;
    s = "[";
    if(name!=null) {
      s+=name;
    }
    s+= "] " + leftside + " => " + rightside ;
    s+= getBranch();
    return s;
  }

  /**
   * Ajoute une regle au systeme
   */
  public static void addElement(RewriteRule rule) {
    Vector vector;
    Object key;
    if(rule.isNamed()) {
      key = rule.name;
    } else {
      key = rule.leftside.getSymbol();
    }
      //System.out.println("addElement: " + key + " <--> " + rule);

    if(map.containsKey(key)) {
      vector = (Vector)map.get(key);
        //System.out.println("vector found for key: " + key);
    } else {
        //System.out.println("vector not found for key: " + key);
      vector = new Vector();
      map.put(key,vector);
    }
      //System.out.println("addElement: size = " + map.size());
    vector.addElement(rule);
  }

  public static void dump() {
    Iterator it = map.values().iterator();
    while(it.hasNext()) {
      Vector vector = (Vector)it.next();
      for(int i = 0 ; i < vector.size() ; i++) {
	System.out.println( vector.elementAt(i) );
      }
    }
  }

  public boolean hasACPattern() {
    return leftside.containsAC();
  }

  public boolean hasACDioPattern() {
    return leftside.containsACDio();
  }

  /**
   * identifiant de la regle : code du symbole de tete
   */
    
  public Lexem getName() {
      // [pem: Oct  5 00]
    return name;
    //return leftside.getSymbolCode();
  }

  public void orderedNormalForm() {
    leftside.orderedNormalForm();
    rightside.orderedNormalForm();
  }

  public DetType getTopLevelDetType(StrategyChooseRule sterm) {
    DetType detType;
    if(sterm==null) {
      detType = DetType.detType;
    } else {
      detType = sterm.getDetType();
    }

    if(hasACPattern()) {
      PatternInfo patternInfo = leftside.getPatternInfo(getBranch());
      if(patternInfo.getConstrainedVariable()) {
	detType=detType.and(DetType.multiDetType);
      }
    } 
    return detType;
  }

  public DetType getDetTypeEvaluation() {
    DetType detType;
    detType = getBranch().getDetType();
    return detType;
  }

  public boolean allDetEvaluation() {
    return getDetTypeEvaluation().isExactlyDet();
  }

  public boolean allSemiDetEvaluation() {
    return getDetTypeEvaluation().isSemiDet();
  }


  /**
   * Compilation du filtrage d'un systeme de regles nommees
   * apparaissant dans une strategie
   */
  public static void compileNamed(OutputCode s, int deep,
			 	  StrategyChooseRule sterm, Vector rules,
				  int strategyLab, 
				  OutputCode matchSubtermCode) {
    DDTree tree = new DDTree(sterm);

    // Construction de l'arbre
    boolean isFunctionTree=false;
    tree.construction(rules,isFunctionTree);
    /*
     * Creation du prefix
     */
    String prefix = sterm.getName();
    matchSubtermCode.write( tree.genMatchSubtermDeclaration(deep,prefix));
    // Filtrage des sous-termes
    tree.genMatchSubterm(matchSubtermCode,deep,prefix);
    tree.genVariableExtract(matchSubtermCode,deep,prefix);
    tree.genPatternListConstruction(matchSubtermCode,deep,prefix);
    /*
     * A ameliorer...
     */
    tree.rgenPatternListDeclaration(Tools.patternListDeclaration,deep,prefix);
    tree.rgenPatternListInit(Tools.patternListInit,deep,prefix);
    tree.rgenPatternListDelete(Tools.patternListDelete,deep,prefix);

    tree.genStrategyManyToOneMatching(s,deep,prefix,rules,sterm,strategyLab);
  }


  /**
   * Compilation d'un systeme de regles non nommees
   * commencant par le meme symbole de tete
   */

  public static void compileNoNamed(OutputCode s, Vector rules) {
    int deep=0;
    Symbol headSymbol = ((RewriteRule)rules.elementAt(0)).leftside.getSymbol();
    DDTree tree = new DDTree(headSymbol);

    // Construction de l'arbre
    boolean isFunctionTree=true;
    tree.construction(rules,isFunctionTree);

    /*
     * Creation du prefix
     */
    String prefix;
    if(headSymbol.isAC()) {
      prefix = "";
    } else {
      prefix = "_" + headSymbol.getSymbolCode();
    }
    s.write( tree.genMatchSubtermDeclaration(deep,prefix) );

    // Entete de la fonction
    // Numerote les variables du membre gauche
    headSymbol.genFunctionHeader(s,deep,tree,rules,
				 "fun_" + headSymbol.getSymbolCode(),true);
    
    StrategyChooseRule sterm=null;
    int strategyLab=-1;
    tree.genManyToOneMatching(s,deep+1,prefix,rules,sterm,strategyLab);

    boolean ACPattern=false;
    boolean allRuleDetEvaluation=true;
    for(int i = 0 ; i<rules.size() ; i++) {
      RewriteRule rule = (RewriteRule)rules.elementAt(i);
      ACPattern = ACPattern || rule.hasACPattern();
      allRuleDetEvaluation = allRuleDetEvaluation && rule.allDetEvaluation();
    }

    s.write("match_fail:\n");
    headSymbol.genMatchFail(s,deep+1);

    if(Flags.debug) {
      s.write(deep+1,"tab_rewrite_step[0][" + headSymbol.getSymbolCode() + "]++;\n");
    }

    s.write(deep+1,"goto end_no_rewrite;\n");

    s.write("end:\n");

    Tools.genRwrCounter(s,deep+1,"rewrite_step");
    
    if(Flags.debug) {
      s.write(deep+1,"tab_rewrite_step[1][" + headSymbol.getSymbolCode() + "]++;\n");
    }
    s.write("end_no_rewrite:\n");
    if(Flags.debug) {
      s.write(deep+1,"if(traceLevel>=1) {\n");
      s.write(deep+2,"doindent(indentlevel);\n");
      s.write(deep+2,"printf(\"rewrite[%u] \",rewrite_step);\n");
      s.write(deep+2,"internal_term_println(stdout,res,resultMode);\n");
      s.write(deep+1,"}\n");
    }
    if(rules.size()>32) {
      s.write(deep+1,"bitSet_delete(mask);\n");
    } else {
      s.write(deep+1,"bitSet32_stack_delete(mask32);\n");
    }

    if(ACPattern) {
        //s.write(deep+1,"if(ACPattern && ms!=NULL)\n");
        // Tools.indent(file,deep+2); file.write("MS_delete(ms[" + matchStateNumber +"]);\n");
    }

    /*
     * set a CUTCLOSE if a rule has an AC Pattern and is applied with
     * a one strategy
     */
    //final boolean isSemiDetApply = true;
    //if(isSemiDetApply && ACPattern) { // && !allRuleDetEvaluation) {
    //file.write(deep+1,"if(ACPattern)\n");
    //file.write(deep+2,"CUTCLOSE(); /* AC matching */\n");
    //}
    if(Flags.debug) {
      s.write(deep+1,"subindent();\n");
      s.write(deep+1,"restoreGlobalIndent();\n");
    }
    s.write(deep+1,"return res;\n");
    s.write(deep+1,"fail:\n");
    s.write(deep+1,"// printf(\"fail\\n\");\n");
    s.write(deep+1,"fail();\n");
    // Fin de la fonction
    s.write(deep,"}\n");

    // Filtrage des sous-termes
    tree.genMatchSubterm(s,deep,prefix);
    tree.genVariableExtract(s,deep,prefix);
    tree.genPatternListConstruction(s,deep,prefix);
    
    /*
     * A ameliorer...
     */
    tree.rgenPatternListDeclaration(Tools.patternListDeclaration,deep,prefix);
    tree.rgenPatternListInit(Tools.patternListInit,deep,prefix);
    tree.rgenPatternListDelete(Tools.patternListDelete,deep,prefix);
  }

  public void variableNumbering() {
    // Recherche du partage
    // right wrt. left
    rightside.searchVariableShareFullNumbering(leftside);
      //branch.searchVariableShare(leftside,rightside);
      // branch wrt. left
    getBranch().searchVariableShareFullNumbering(leftside);
      // branch wrt. branch
    getBranch().selfSearchVariableShare();
      // right wrt. branch
    getBranch().reverseSearchVariableShare(rightside);
    leftside.setFullNumbering();

    // marquage des variables dio
    leftside.markDioVariable();

    /*
     * Reservation des variables
     * Toutes les variables sont supposees etre libres
     */
    AllocVariable.init();
    /*
     * C'est subtil ici :
     * 1) numerotation des variables non utilissees a droite et
     *    apparaissant sous un symbol AC a gauche 
     * 2) numerotation du membre droit et des variables apparaissant
     *    sous un symbole AC du membre gauche 
     * 3) numerotation des LocalEvaluations
     * ATTENTION : la numerotation du membre droit libere les noeuds
     *             intermediaires c'est pourquoi il faut numeroter 
     *             les LocalEvaluation apres le membre droit
     * Sinon : une variable d'une LocalEvaluation peut etre liberee et
     *         utilisee pour numeroter une variable AC du lhs
     */
    int substIndex=leftside.leftsideACVariableAffectation(0);
    //rightside.rightsideVariableAffectation();  
    substIndex=getBranch().variableAffectation(substIndex);
    rightside.rightsideVariableAffectation();  

    // Liberation des variables
    leftside.leftsideACVariableLiberation();
    getBranch().variableLiberation();
    rightside.rightsideVariableLiberation();

  }

  /*
   * raz des flags des membres gauches :
   *  - lhs
   *  - local assignements
   */
  public void variableClear(boolean full) {
    leftside.leftsideVariableClear(full);
    getBranch().variableClear(full);
  }


  public int getMaxVariableNumber() {
    return maxVariableNumber;
  }
  public void setMaxVariableNumber(int max) {
    maxVariableNumber=max;
  }
  
  public void genDeclaration(OutputCode s,int deep) {

    //System.out.println("*** genDeclaration: " + this);

    int max = 0;
    int maxLeftside = leftside.getMaxUsedVariable(max);
    int maxRightside = rightside.getMaxUsedVariable(max);
    max=(maxLeftside>maxRightside)?maxLeftside:maxRightside;
    max = getBranch().getMaxUsedVariable(max);
    //System.out.println("max = " + max);
    Tools.genDeclaration(s,deep,max);

    int maxSubstIndex = leftside.getMaxSubstIndex(0);
    maxSubstIndex = getBranch().getMaxSubstIndex(maxSubstIndex);
    maxSubstIndex++;
    s.write(deep,"struct term *substitution[" + maxSubstIndex + "];\n");
    
  }


  /**
   * Application of a syntactic rule (without AC)
   */
  public void genApplication(OutputCode s,int deep, int number, 
			     StrategyChooseRule sterm, int strategyLab,
			     boolean isFirst,boolean isLast,
			     boolean ACPattern) {

    // rule is applied under nonamed/one/dc/dk ?
    boolean isOneApply=false;
    boolean isDcApply=false;
    boolean isDkApply=false;
    if(sterm==null || sterm.isOne()) {
      isOneApply=true;
    } else if(sterm.isDc()) {
      isDcApply=true;
    } else if(sterm.isDk()) {
      isDkApply=true;
    } else if(sterm.isNorm()) {        // HUY 18/02/00
      isOneApply=true;
    } else {
      throw new InternalError("unknown StrategyChooseRule type");
    }
    
    /*
    System.out.println("genApplication : " + this);
    System.out.println("One=" + isOneApply +
		       " Dc=" + isDcApply +
		       " Dk=" + isDkApply);
		       */

    /*
     * recherche du partage et numerotation des variables
     */
    genDeclaration(s,deep);
    if(sterm != null){
      s.write(deep,"long tmp_step;\n"); // [Huy: Sep 27 00]  
      if(sterm.isNorm() && getBranch().size()>=2){ // [Huy: May  7 00] 
        s.write(deep,"POS position_tmp;\n");
	s.write(deep,"int index=0, pos = MAXPOS,i,strCalltmp=strCall;\n");
	s.write(deep,"TR_COQ *head_tmp = head_tr, *tail_tmp = tail_tr, *head_cond_tr[2] = {NULL,NULL};\n");
      	s.write(deep,"if(coqMode) {\n");
	s.write(deep+1,"for (i=0;i<MAXPOS; i++) position_tmp[i]=position[i];\n");
	s.write(deep,"}\n");
      }
    }

    
    if(isDcApply) {
      Tools.indent(s,deep); s.write("int *wasr;\n");
    }
    s.write(deep,"/* lhs: " + leftside +" */\n");

    if(Flags.debug && getName() != null) {
      s.write(deep+0,"if(traceLevel>=3) {\n");
      s.write(deep+1,"doindent(indentlevel);\n");
      s.write(deep+1,"printf(\"TRY RULE: " + getName() + "\\n\");\n");
      s.write(deep+1,"if(traceLevel>=4) {\n");
      s.write(deep+2,"doindent(indentlevel);\n");
      s.write(deep+2,"printf(\"on \");\n");
      s.write(deep+2,"internal_term_println(stdout,v0,resultMode);\n");
      s.write(deep+1,"}\n");
      s.write(deep+0,"}\n");
    }
    /*
     * backtracking management
     */

    s.write(deep,"/* allDetEvaluation: " + getDetTypeEvaluation() + " */\n");

    /*
     * we set a CUTOPEN if the top level is semiDeterministic
     * (i.e. 0 or 1 result) and if local evaluations do not
     * forget any choice point
     */
    if(Flags.optimiseChoicePoint) {
      //if( getTopLevelDetType(sterm).isExactlyDet() && !allDetEvaluation() ) {
	/* A ETUDIER ... */
      if( getTopLevelDetType(sterm).isSemiDet() && !allDetEvaluation() ) {
	s.write(deep,"CUTOPEN(); /* Wheres */\n");
      }
    } else {
      if( getTopLevelDetType(sterm).isSemiDet() ) {
	s.write(deep,"CUTOPEN(); /* Wheres */\n");
      }
    }

    if(isDcApply) {
      s.write(deep,"wasr=(int*) allocStable(sizeof(int));\n"); 
      s.write(deep,"*wasr=0;\n");
    }

    /*
     * we set a choicePoint if the top level returns more then one result
     * (i.e. if there is an AC operator)
     * or if local evaluations may fail
     * and if it is not the last rule
     */
    //    if( (!getTopLevelDetType(sterm).isExactlyDet() || !allDetEvaluation())
    
    if( Flags.optimiseChoicePoint==false // && (isFirst || !isLast)
	||
	(!getTopLevelDetType(sterm).isSemiDet() || !allDetEvaluation()
	 && (isFirst || !isLast)
         && (sterm==null || !sterm.isNorm()) // [pem: Oct 16 00] A VERIFIER
         )) {
      if( isDcApply ) {
	s.write(deep,"if(setChoicePoint()) {\n");
	s.write(deep+1,"/* local evaluations failed */\n");
	s.write(deep+1,"if(*wasr!=0) {\n");	s.write(deep+2,"/* If the rule succeed one then re-generate a fail */\n");
	Tools.genFail(s,deep+2);
	s.write(deep+1,"}\n");
	s.write(deep+1,"/* Else, try next rule */\n");
	s.write(deep+1,"goto myend" + number + ";\n");
	s.write(deep,"}\n");
      } else {
	s.write(deep,"if(setChoicePoint()) {\n");
	s.write(deep+1,"/* local evaluations failed, try next rule */\n");
	s.write(deep+1,"goto myend" + number + ";\n");
	s.write(deep,"}\n");
      }
    }

    // local evaluations
    getBranch().genCode(s,deep,this,number,sterm,isFirst,isLast);

    if(sterm != null && sterm.isNorm()) {
      s.write(deep,"if(coqMode) {\n");
      s.write(deep+1,"strcpy(rname,\"" + name.getName().substring(0,
                             name.getName().indexOf(':')) +"\");\n");
      if(getBranch().size()>=2) {
        s.write(deep+1,"add_trace(head_cond_tr[0],head_cond_tr[1]);\n");
      } else {
        s.write(deep+1,"add_trace(NULL,NULL);\n");
      }
      s.write(deep,"}\n");
    }
    
    rightside.genRightside(s,deep);
    /*
     * Pour bien compter le nombre de regles appliquees dans les strategies
     */
    if(sterm!=null) {
      Tools.genRwrCounter(s,deep,"rewrite_step");
      Tools.genRwrCounter(s,deep,"rewrite_label_step");
      if(sterm.isNormOut()) {
        Tools.genRwrCounter(s,deep,"rewrite_real_step");
      }
    }

    //if(isDcApply && !isLast) {
    if(isDcApply) {
      // pour compiler le Choice      s.write(deep,"/* The rule succeed once */\n");
      s.write(deep,"*wasr=1;\n");
    }
    if(Flags.optimiseChoicePoint) {
      //if( getTopLevelDetType(sterm).isExactlyDet() && !allDetEvaluation() ) {
      if( getTopLevelDetType(sterm).isSemiDet() && !allDetEvaluation() ) {
	s.write(deep,"CUTCLOSE(); /* Wheres */\n");
      }
    } else {
      if( getTopLevelDetType(sterm).isSemiDet() ) {
	s.write(deep,"CUTCLOSE(); /* Wheres */\n");
      }
    }

    /*
     * set a CUTCLOSE if a rule has an AC Pattern and is applied with
     * a one strategy
     */
    if(getTopLevelDetType(sterm).isSemiDet() && ACPattern) {
      //s.write(deep+1,"if(ACPattern)\n");
      //s.write(deep+2,"CUTCLOSE(); /* AC matching */\n");
    }

    if(strategyLab>=0) {
      s.write(deep,"goto stratLab" + strategyLab + ";\n");
    } else {
      s.write(deep,"goto end;\n");
    }
  }

  /**
   * Application of an AC rule (only 1 AC variable)
   */
  public void genACApplication(OutputCode s,int deep,
			       int number, int cbgNumber, 
			       boolean hasACnet,
			       StrategyChooseRule sterm, 
			       int strategyLab,
			       boolean isFirst, boolean isLast,
			       String prefix,
			       boolean allRuleDetEvaluation) {
      /*
       * [pem: Sep  9 00]
       * re-initialisation des flags
       */
    variableClear(false);
    
    int nbVar=leftside.nbVariable();
    int nbACVar=leftside.nbVariableUnderACSymbol();

    // rule is applied under nonamed/one/dc/dk ?
    boolean isOneApply=false;
    boolean isDcApply=false;
    boolean isDkApply=false;
    if(sterm==null || sterm.isOne()) {
      isOneApply=true;
    } else if(sterm.isDc()) {
      isDcApply=true;
    } else if(sterm.isDk()) {
      isDkApply=true;
    } else {
      throw new InternalError("unknown StrategyChooseRule type");
    }
    boolean isSemiDetApply=false;
    if(sterm==null || sterm.isSemiDet()) {
      isSemiDetApply=true;
    }
// [pem: Dec 23 98]
    if( Flags.optimiseChoicePoint &&
        isSemiDetApply && allRuleDetEvaluation) {
    } else {
    if(isDcApply) {
      s.write(deep,"int *wasr=(int*) allocStable(sizeof(int));\n");

      if(Flags.debug && getName() != null) {
        s.write(deep+0,"if(traceLevel>=3) {\n");
        s.write(deep+1,"doindent(indentlevel);\n");
        s.write(deep+1,"printf(\"TRY RULE: " + getName() + "\\n\");\n");
        s.write(deep+1,"if(traceLevel>=4) {\n");
        s.write(deep+2,"doindent(indentlevel);\n");
        s.write(deep+2,"printf(\"on \");\n");
        s.write(deep+2,"internal_term_println(stdout,v0,resultMode);\n");
        s.write(deep+1,"}\n");
        s.write(deep+0,"}\n");
      }

      
      s.write(deep,"if(debugMode) {\n");
      s.write(deep+1,"printf(\"try pattern: [" + name + "] " +
	      leftside + "\\n\");\n");
      s.write(deep+1,"internal_term_println(stdout,v0,resultMode);\n");
      s.write(deep,"}\n");

      s.write(deep,"*wasr=0;\n");
      s.write(deep,"if(setChoicePoint()) {\n");
      s.write(deep+1,"/* AC matching failed */\n");
      s.write(deep+1,"if(*wasr!=0) {\n");
      s.write(deep+2,"/* If the rule succeed one then re-generate a fail */\n");
      Tools.genFail(s,deep+2);
      s.write(deep+1,"}\n");

      s.write(deep+1,"/* Else, try next rule */\n");
      s.write(deep+1,"goto myend" + number + ";\n");
      s.write(deep,"}\n");
    } else {
      if(Flags.debug && getName() != null) {
        s.write(deep+0,"if(traceLevel>=3) {\n");
        s.write(deep+1,"doindent(indentlevel);\n");
        s.write(deep+1,"printf(\"TRY RULE: " + getName() + "\\n\");\n");
        s.write(deep+1,"if(traceLevel>=4) {\n");
        s.write(deep+2,"doindent(indentlevel);\n");
        s.write(deep+2,"printf(\"on \");\n");
        s.write(deep+2,"internal_term_println(stdout,v0,resultMode);\n");
        s.write(deep+1,"}\n");
        s.write(deep+0,"}\n");
      }
      s.write(deep,"if(setChoicePoint()) {\n");
      s.write(deep+1,"/* AC matching failed, try next rule */\n");
      s.write(deep+1,"goto myend" + number + ";\n");
      s.write(deep,"}\n");
    }
    }
    
    /*
     * Recherche d'une solution DU match_state 
     * Il n'y a qu'un seul pattern AC par regle
     */
    
    /*
      if(cbgNumber>=0) { 
	String varName = leftside.getFirstACPattern().genCore();
	s.write(deep,"indice = MS_reinit(ms," + varName + "," + cbgNumber + ");\n");
      s.write(deep,"if(indice>=0) {\n");
      s.write(deep+1,"if(ACPattern && ms!=NULL) {\n");
      if( isSemiDetApply && allRuleDetEvaluation) {
	s.write(deep+2,"indice = MS_solve(ms,mode);\n");
      } else {
	s.write(deep+2,"start" + number + ":;\n");
	s.write(deep+2,"indice = MS_solve(ms,mode);\n");
	if(Flags.debug) {
	  s.write(deep+2,"// printf(\"\\nsetChoicePoint\\n\");\n");
	}
	Tools.genComment(s,deep+2,"choicePoint AC matching");
	s.write(deep+2,"if(indice>=0 && setChoicePoint()) {\n");
	if(Flags.debug) {
	  s.write(deep+3,"// printf(\"on revient d'un fail\\n\");\n");
	}
	s.write(deep+3,"goto start" + number + ";\n");
	s.write(deep+2,"}\n");
      }
      s.write(deep+1,"} else {\n");
      s.write(deep+2,"indice = 0;\n");
      s.write(deep+1,"}\n");
      s.write(deep,"} else {\n");
      Tools.genFail(s,deep+1);
      s.write(deep,"}\n");
    }
    */

    Term lhsACPattern = leftside.getFirstACPattern();
    if(cbgNumber>=0) {
      String varName = lhsACPattern.genCore();
      
      if(isSemiDetApply && allRuleDetEvaluation) {
	s.write(deep,"if(ACPattern && MS_reinit(ms[" + matchStateNumber +"], (struct termac*)" + varName + "," + cbgNumber + ")>0 && MS_solve_rule(ms[" + matchStateNumber +"])>=0) {\n");
	s.write(deep+1,"// do nothing \n");
	s.write(deep,"} else {\n");
          // [pem: Dec 23 98]
        if(Flags.optimiseChoicePoint) {
          s.write(deep+1,"goto myend" + number + ";\n");
        } else {
          Tools.genFail(s,deep+1);
        }
        s.write(deep,"}\n");
      } else {
	s.write(deep,"if(ACPattern && MS_reinit(ms[" + matchStateNumber +"], (struct termac*)" + varName + "," + cbgNumber + ")>0) {\n");
	s.write(deep+1,"while(1) {\n");
	s.write(deep+2,"if(MS_solve_rule(ms[" + matchStateNumber +"])<0) {\n");
	// PEM: mydend
	Tools.genFail(s,deep+3);
	//s.write(deep+3,"goto myend" + number + ";\n");
	s.write(deep+2,"}\n");
	if(Flags.debug) {
	  s.write(deep+2,"// printf(\"setChoicePoint\\n\");\n");
	}
	Tools.genComment(s,deep+2,"choicePoint AC matching");
	s.write(deep+2,"if(!setChoicePoint()) {\n");
	s.write(deep+3,"break;\n");
	s.write(deep+2,"}\n");
	if(Flags.debug) {
	  s.write(deep+2,"// printf(\"on revient d'un fail\\n\");\n");
	}
	s.write(deep+1,"}\n");
	s.write(deep,"} else {\n");
	// PEM: mydend
	Tools.genFail(s,deep+1);
	//s.write(deep+1,"goto myend" + number + ";\n");
	s.write(deep,"}\n");
      }
    }


    // cbgNumber : Numero de la regle dans le CBG
    if(hasACnet && !leftside.isDio()) {
      //s.write(deep,"if(indice>=0 && ms->no_rule==" + cbgNumber + ") {\n");
      s.write(deep,"if(1) {\n");
    } else {
      s.write(deep,"if(1) {\n");
    }

    s.write(deep+1,"int nb_variable=" + nbVar + ";\n");
    s.write(deep+1,"int nb_variable_ac=" + nbACVar + ";\n");
    /*
     * Ne marche pas parce qu'il faut prendre en compte les variables des 
     * matching conditions AC
     * la declaration est faite dans genDeclaration()
     * s.write(deep+1,"struct term *substitution[" + nbVar + "];\n");
     */
    s.write(deep+1,"int i;\n");

    /**
     * numerotation des variables de la substitution
     */    
    genDeclaration(s,deep+1);
    s.write(deep+1,"/* lhs: " + leftside +" */\n");
    if(Flags.debug) {
      int max = 0;
      int maxLeftside = leftside.getMaxUsedVariable(max);
      int maxRightside = rightside.getMaxUsedVariable(max);
      max=(maxLeftside>maxRightside)?maxLeftside:maxRightside;
      max = getBranch().getMaxUsedVariable(max);
      s.write(deep+1,"{\n");
      s.write(deep+2,"int i;\n");
      s.write(deep+2,"for(i=0;i<=" + max + ";i++) sv[i]=NULL;\n");
      s.write(deep+1,"}\n");
    }
      
    /*
     * 08/07/1998 : je pense que cela ne sert a rien    
     if(isDcApply) {
     s.write(deep+1,"int *wasr;\n");
     }
    */


    /**
     * le variables AC sont stockee de 0 a m (avec la restriction m=0)
     * dans l'ordre d'apparition
     * et les variables non AC de m+1 a n
     */
    if(hasACnet && !lhsACPattern.isDio()) {
      String name = prefix; // + "_" + leftside.getFirstACPattern().getSymbolCode();
      String varName = lhsACPattern.genCore();
      s.write(deep+1,"/* To protect the term */\n");
      if(lhsACPattern.usedContextInRhs()) {
	s.write(deep+1,"substitution_build((struct termac*)" + varName +
		",ms[" + matchStateNumber +"],nb_variable,substitution,nb_variable_ac,variable_extract" + name +
		"," + cbgNumber + ");\n");
      } else {
	s.write(deep+1,"substitution_build_without_context((struct termac*)" + varName +
		",ms[" + matchStateNumber +"],nb_variable,substitution,nb_variable_ac,variable_extract" + name +
		"," + cbgNumber + ");\n");
      }
    } else {
      String varName = lhsACPattern.genCore();
      //      s.write(deep+1,"for(i=0 ; i<nb_variable ; i++) {\n");
      s.write(deep+1,"for(i=0 ; i<sizeof(substitution)/sizeof(struct term*) ; i++) {\n");
      s.write(deep+2,"substitution[i]=" + varName + ";\n");
      s.write(deep+1,"}\n");
    }

    /*
     * Decoupage eventuel des dioVariables
     */
    leftside.genDioMatching(s,deep+1,number, isSemiDetApply && allRuleDetEvaluation);

    /*
     * backtracking management
     */ 
    s.write(deep+1,"/* allDetEvaluation: " + getDetTypeEvaluation() + " */\n");

    /*
     * we set a choicePoint if the top level returns more then one result
     * (i.e. if there is an AC operator)
     * or if local evaluations may fail
     * and if it is not the last rule
     */
    /*
     * CA NE SERT A RIEN ICI !!!
    if( (!Flags.optimiseChoicePoint && (isFirst || !isLast))
	||
	(!getTopLevelDetType(sterm).isSemiDet() || !allDetEvaluation())
	&& (isFirst || !isLast)) {
      s.write(deep+1,"if(setChoicePoint()) {\n");
      Tools.genComment(s,deep+2,"local evaluations failed, try another match");
      //s.write(deep+1,"goto myend" + number + ";\n");
      Tools.genFail(s,deep+2);
      s.write(deep+1,"}\n");
    }
    */
    // local evaluations
    getBranch().genCode(s,deep+1,this,number,sterm,isFirst,isLast);
    rightside.genRightside(s,deep+1);

    /* 
     * Pour bien compter le nombre de regles appliquees dans les strategies
     */
    if(sterm!=null) {
      Tools.genRwrCounter(s,deep,"rewrite_step");
    }

    if(isDcApply) {
      // pour compiler le Choice
      s.write(deep+1,"/* The rule succeed once */\n");
      s.write(deep+1,"*wasr=1;\n");
    }


    /*
     * set a CUTCLOSE if a rule has an AC Pattern and is applied with
     * a one strategy
     */
    if(isSemiDetApply) {
      s.write(deep+1,"if(ACPattern)\n");
      s.write(deep+2,"CUTCLOSE(); /* AC matching */\n");
    }

    if(strategyLab>=0) {
      s.write(deep+1,"goto stratLab" + strategyLab + ";\n");
    } else {
      s.write(deep+1,"goto end;\n");
    }
    s.write(deep,"} else {\n");
      // [pem: Dec 23 98]
    if( Flags.optimiseChoicePoint &&
        isSemiDetApply && allRuleDetEvaluation) {
      s.write(deep+1,"goto myend" + number + ";\n");
    } else {
      Tools.genFail(s,deep+1);
    }
    s.write(deep,"}\n");
  }

  // *** Peter
  public static Vector getAllRulesF1F2(int f1, int f2) {
    Vector subrules = new Vector();
    System.out.print("(");
    
    Iterator it = map.values().iterator();
    while(it.hasNext()) {
      Vector vector = (Vector)it.next();
      int symb1, symb2;
      Term lhs;

        //**RewriteRule rule = (RewriteRule) vector.firstElement();
      //System.out.print("size="+vector.size());
      for(int i=0; i<vector.size(); i++) {
	RewriteRule rule = (RewriteRule)vector.elementAt(i);
	//System.out.println("At "+i+"::");
	if(rule.isNamed()) {
	  lhs = rule.leftside;
	  //System.out.println("[1] "+lhs.getSymbol().getCode()+"=="+f1);
	  if (lhs.getSymbol().getCode() == f1) {
	    //System.out.println("[2] "+lhs.getSubterm(0).getSymbol().getCode()+"=="+f2);
	    if (lhs.getSubterm(0).getSymbol().getCode() == f2) {
	      System.out.print(":");
	      StrategyRuleName rterm = new StrategyRuleName(rule.name);
	      subrules.addElement(rterm);
	    }
	  }
	}
      }
    }
    System.out.print(")");
    return subrules;
  }

  public boolean isCompilable() throws CompileError,ACPatternNotCompiled {
    if(leftside.containsAC()) {
      Term lhsACPattern = leftside.getFirstACPattern();
      int lhsArity      = lhsACPattern.arity();
      int nbACSubterms  = leftside.getNbAllLevelACSubterms();

      if(Flags.debug) {
	if(!leftside.isAC()) {
	  System.out.println("\nleftside = " + leftside);
	}
	System.out.println("\nlhsACPattern = " + lhsACPattern);
	System.out.println("\nnbACSubterms = " + nbACSubterms);
      }

      PatternInfo patternInfo     = lhsACPattern.getPatternInfo(getBranch());
      int nbSingleACVariable      = patternInfo.getNbSingleACVariable();
      int nbMultiACVariable       = patternInfo.getNbMultiACVariable();
      int nbSingleACSubterm       = patternInfo.getNbSingleACSubterm();
      int nbMultiACSubterm        = patternInfo.getNbMultiACSubterm();
      boolean constrainedVariable = patternInfo.getConstrainedVariable();

      if(Flags.debug) {
	System.out.println(patternInfo);
      }

        /*
         * On ne peut pas compiler s'il y a >1 sous-termes AC
         */
      if(leftside.getNbFirstLevelACSubterms() > 1) {
        throw new ACPatternWithSeveralACSubterms(
          "There are 2 AC subterms in this pattern");
      }
      
      /*
       * un seul niveau est compile
       * F(x,G(y,z))
       */
      for(int i=0 ; i<lhsArity ; i++) {
	if(lhsACPattern.getSubterm(i).containsAC()) {
	  Term subACTerm = lhsACPattern.getSubterm(i).getFirstACPattern();
	  if(subACTerm.isDio()) {
	    if(subACTerm.arity()>2) {
	      throw new CompileError(
                "Second level contains more than 2 variables");
	    }
	    if(Flags.debug) {
	      System.out.println("Two levels of AC symbols");
	      System.out.println("*** leftside = " + leftside);
	    }
	  } else { 
	    /*
	     * On ne peut pas compiler un sous-terme non-dio
	     */
	      throw new CompileError(
                "Second level contains a non variable term");
	  }
	}
      }      

      /*
       * cas d'une regle nommee ou d'un sous-pattern AC
       */
      if(isNamed() || !leftside.isAC()) {
	/*
	 * On peut compiler
         *
	 * F(x^n)
         * F(t^n)
         *
	 * F(x,y)
	 * F(x,y^n)
         * F(x,t)
         * F(t1,t2)
         *
	 * F(t1,...,tn)
	 * F(x,t1,...,tn)
	 */	
	if(!(
          (lhsArity==1 && nbSingleACVariable==0 && nbMultiACVariable==1) ||
          (lhsArity==1 && nbSingleACVariable==0 && nbMultiACVariable==0 && nbMultiACSubterm==1) ||

          (lhsArity==2 && nbSingleACVariable==2 && nbMultiACVariable==0) ||
          (lhsArity==2 && nbSingleACVariable==1 && nbMultiACVariable==1) ||
          
          (lhsArity>=2 && nbSingleACVariable==0 && nbMultiACVariable==0) ||
          (lhsArity>=2 && nbSingleACVariable==1 && nbMultiACVariable==0)
	     )) {
	  throw new CompileError(
            "too many variables in the first level");
	}
      } else {
	/*
	 * cas d'une regle non nommee et d'un symbole AC en tete
	 */
	if(!constrainedVariable) {
	  /*
	   * si toutes les variables sont libres on peut compiler
	   * F(x,y)
	   * F(x,y^n)
	   * F(x,t1,...,tn)
	   */
	  if(!(
            (lhsArity==2 && nbSingleACVariable==2 && nbMultiACVariable==0) ||
            (lhsArity==2 && nbSingleACVariable==1 && nbMultiACVariable==1) ||
            (lhsArity>=2 && nbSingleACVariable==1 && nbMultiACVariable==0) 
            )) {
	    throw new ACPatternNotCompiled();
	  }
	} else {
	  /*
	   * si une variable est liee aux conditions on peut compiler
	   * F(x,y,z) ne sert a rien !!! a remplacer par F(x,y)
	   * F(x,y^n)
	   * F(x,y,t1,...,tn)
	   */
	  if(!(
              // F(x,y)
            (lhsArity==2 && nbSingleACVariable==2 && nbMultiACVariable==0) ||
            (lhsArity==2 && nbSingleACVariable==1 && nbMultiACVariable==1) ||
            (lhsArity>=3 && nbSingleACVariable==2 && nbMultiACVariable==0) 
            )) {
	    throw new ACPatternNotCompiled();
	  }
	}
      }
    } else {

    }
    return true;
  }

    /*
     * abstract a term by a new variable and return this variable
     */
  private void abstractACSubterm(Term subterm) {
    List list = new ArrayList();
    list.add(subterm);
    abstractACSubterm(list);
  }

  private void abstractACSubterm(List ACSubterms) {
    Term variable = variableAbstractACSubterm(ACSubterms);
  }

  private Term variableAbstractACSubterm(List ACSubterms) {
      // get a fresh number and update the MaxVariableNumber
    int varNumber = maxVariableNumber++;
      // create a new variable
    Term term = (Term)ACSubterms.get(0);
    Symbol fatherSymbol = term.getFather().getSymbol();
    Symbol source = term.getSymbol();
    Symbol symb = SymbolVariable.getCreate(varNumber,source.getSort());
    Term variable = new Term(symb,null);
      // replace the list of terms by a variable
    leftside.abstractACSubterm(ACSubterms,variable);
      // collect variables of lhs
    Collection listVar = new HashSet();
    leftside.collectSymbolVariable(listVar);
    LocalAffectation newWhere;

    if(ACSubterms.size() > 1) {
      Vector subterms = new Vector();
      Iterator it = ACSubterms.iterator();
      while(it.hasNext()) {
        subterms.addElement(it.next());
      }
      term = new Term(fatherSymbol,subterms);
      term.orderedNormalForm();
    }
    newWhere = new LocalAffectation(term,null,variable.deepCopy());
    
      /*
       * Il faut renommer les variables du pattern AC introduit
       * pour eviter des variables non fraiches par rapport
       * au membre gauche de la regle
       */

      // rename variable of the abstracted term
    Set listPair = newWhere.getLeftside().renameVariable(listVar,maxVariableNumber,null);
      // build and insert conditions
    maxVariableNumber += listPair.size();
    Iterator it = listPair.iterator();
    while(it.hasNext()) {
      Pair pair = (Pair)it.next();
        // System.out.println(pair);
      Term t1 = new Term((Symbol)pair.getX(),null);
      Term t2 = new Term((Symbol)pair.getY(),null);
      Condition cond = Condition.buildEquality(t1,t2);
      getBranch().addFirst(cond);
    }
      // insertion de la matching condition
    getBranch().addFirst(newWhere);
    return variable;
  }

  public void transformation()
    throws RuleCompiled, RuleTransformed, CompileError {

      // remove extra AC-subterms
    while(transformation1()) { /* do nothing */ }
      // remove extra AC-levels
    transformation2();
      // remove extra AC-variables and subterms
    transformation3_1();
    transformation3_2();

      /*
       * No more transformation can be applied: the rule can be compiled
       */
      //    throw new RuleCompiled();
  }

  private boolean transformation1() {
    boolean transformationDone = false;
      /*
       * Cas 1
       *******
       * S'il y a plusieurs sous-termes AC de niveau 1
       *     f(...,F(...),...,G(...),...)
       * on abstrait un sous terme et on ajoute une matching condition
       *     f(...,X,...,G(...),...)
       *     where F(...) :=() X
       */
    if(leftside.containsAC()) {
      if(!leftside.isAC() && leftside.getNbFirstLevelACSubterms() > 1) {
        if(Flags.debug) {
          System.out.println("Case 1: " + this);
        }
        Term ACSubterm = leftside.getFirstACSubterm();
        abstractACSubterm(ACSubterm);
        transformationDone = true;
      }
    }
    if(transformationDone && Flags.debug) {
      System.out.println("After: " + this);
    }
    return transformationDone;
  }

  private boolean transformation2() {
    boolean transformationDone = false;
      /*
       * Cas 2
       *******
       * Le pattern ne content qu'un seul sous-terme AC
       * S'il y plus d'un niveau de symbol AC
       *      f(...,F(...,G1(...),...,Gn(...),...),...)
       * on abstrait le 2nd niveau
       *      f(...,F(...,Xn,...,Xn,...),...)
       *      where G1(...) :=() X1
       *      where Gn(...) :=() Xn
       *
       * sauf pour les patterns
       *     f(...,F(x,G(y,z)),...)
       *     f(...,F(x,g(...,G(y,z),...),...),...)
       *
       */
    if(leftside.containsAC()) {
      int nbAllLevelACSubterms = leftside.getNbAllLevelACSubterms();
      if(( leftside.isAC() && nbAllLevelACSubterms > 0) ||
         (!leftside.isAC() && nbAllLevelACSubterms > 1)) {
        Term ACTerm = leftside.getFirstACPattern();
        Term ACSubterm = ACTerm.getFirstACSubterm();
          //System.out.println("ACTerm    = " + ACTerm);
          //System.out.println("ACSubterm = " + ACSubterm);
        if(ACTerm.getNbFirstLevelACSubterms()==1 &&
           ACSubterm.isDio() && ACSubterm.arity() <= 2) {
            /*
             * pas de transformation necessaire
             */
        } else {
          if(Flags.debug) {
            System.out.println("Case 2: " + this);
          }
          Collection set = new HashSet();
          ACTerm.collectACSubterm(set);
          Iterator it = set.iterator();
          while(it.hasNext()) {
            Term subterm = (Term)it.next();
            abstractACSubterm(subterm);
          }
          transformationDone = true;
        }
      }
    }
    if(transformationDone && Flags.debug) {
      System.out.println("After: " + this);
    }
    return transformationDone;
  }

  private boolean transformation3_1() {
    boolean transformationDone = false;
      /*
       * Cas 3.1
       *********
       * Le terme ne contient qu'un seul sous-terme AC
       *  - d'un seul niveau
       *  - ou de la forme F(x,g(...,G(y,z),...),...)
       *
       * Pas de variable d'extension (regle nommee ou sous-terme AC strict)
       *
       */
    if(leftside.containsAC() && (isNamed() || !leftside.isAC())) {
      Term ACSubterm          = leftside.getFirstACPattern();
      int lhsArity            = ACSubterm.arity();
      PatternInfo patternInfo = ACSubterm.getPatternInfo(getBranch());
      int nbSingleACVariable  = patternInfo.getNbSingleACVariable();
      int nbMultiACVariable   = patternInfo.getNbMultiACVariable();
      int nbSingleACSubterm   = patternInfo.getNbSingleACSubterm();
      int nbMultiACSubterm    = patternInfo.getNbMultiACSubterm(); 
      if(lhsArity == 2) {
        if(nbMultiACVariable==2 ||
           (nbMultiACVariable==1 && nbSingleACSubterm==1) ||
           (nbMultiACVariable==1 && nbMultiACSubterm==1)) {
            /*
             * F(x^n,y^m) => F(X,y^m) where x^n :=() X 
             * F(x^n,t)   => F(X,t)   where x^n :=() X 
             * F(x^n,t^m) => F(X,t^m) where x^n :=() X
             */
          if(Flags.debug) {
            System.out.println("Case 3.1 (1): " + this);
          }
          List list = new ArrayList();
          for(int i=0 ; list.isEmpty() && i<ACSubterm.arity() ; i++) {
            Term t = ACSubterm.getSubterm(i);
            if(t.isVariable() && t.getMultiplicity()>1) {
              list.add(t);
            }
          }
          abstractACSubterm(list);
          transformationDone = true;
        }
      } else if(lhsArity > 2) { 
        if((nbSingleACVariable + nbMultiACVariable > 1) ||
           (nbMultiACSubterm > 0)) {
            /*
             * F(x1^alpha_1,...,xn^alpha_n,t1^beta_1,...,tm^beta_m)
             * avec n > 1 ou beta_i != 1
             * =>
             * F(X,ti^beta_i=1,...,tj^beta_j=1)
             *   where F(x1^alpha_1,...,xn^alpha_n,...,tk^beta_k>1,...) :=() X
             */
          if(Flags.debug) {
            System.out.println("Case 3.1 (2): " + this);
          }
          List list = new ArrayList();
          for(int i=0 ; i<ACSubterm.arity() ; i++) {
            Term t = ACSubterm.getSubterm(i);
            if(t.isVariable() || t.getMultiplicity()>1) {
              list.add(t);
            }
          }
          abstractACSubterm(list);
          transformationDone = true;
        }
      }
    }
    if(transformationDone && Flags.debug) {
      System.out.println("After: " + this);
    }
    return transformationDone;
  }

  private boolean transformation3_2() {
    boolean transformationDone = false;
      /*
       * Cas 3.2
       *********
       * Le terme ne contient qu'un seul sous-terme AC
       *  - d'un seul niveau
       *  - ou de la forme F(x,g(...,G(y,z),...),...)
       *
       * Regle non nommee avec symbole AC en tete
       * Ajout d'une variable d'extension
       */
    if(leftside.containsAC() && !isNamed() && leftside.isAC()) {
      int lhsArity            = leftside.arity();
      PatternInfo patternInfo = leftside.getPatternInfo(getBranch());
      int nbSingleACVariable  = patternInfo.getNbSingleACVariable();
      int nbMultiACVariable   = patternInfo.getNbMultiACVariable();
      int nbSingleACSubterm   = patternInfo.getNbSingleACSubterm();
      int nbMultiACSubterm    = patternInfo.getNbMultiACSubterm();
      boolean constrainedVariable = patternInfo.getConstrainedVariable();

      if(lhsArity == 1 && nbMultiACVariable==1) {
          /*
           * F(x^n)     => F(X,x^n)
           */
        if(Flags.debug) {
          System.out.println("Case 3.2 (1): " + this);
        }
        addExtensionVariable();
        transformationDone = true;
      } else if(lhsArity <= 2 && nbMultiACSubterm==1 && !constrainedVariable) {
            /*
             * F(t^n)       => F(X,Y^n) where t:=()Y
             * F(x,t^n)     => F(x,Y^n) where t:=()Y
             */
          if(Flags.debug) {
            System.out.println("Case 3.2 (2): " + this);
          }

          Term t = !leftside.getSubterm(0).isVariable()?
                   leftside.getSubterm(0):
                   leftside.getSubterm(1);
          int mult = t.getMultiplicity();
          t.setMultiplicity(1);
          List list = new ArrayList();
          list.add(t);
          Term variable = variableAbstractACSubterm(list);
            // mise a jour la multiplicite de la nouvelle variable
          variable.setMultiplicity(mult);
          if(lhsArity == 1) {
            addExtensionVariable();
          }
          transformationDone = true;
      } else if(lhsArity >= 2) { 
        if(nbSingleACVariable  == 1     &&
           nbMultiACVariable   == 0     &&
           nbMultiACSubterm    == 0     &&
           constrainedVariable == true) {
            /*
             * F(x,t1,...,tm)
             * avec x contraint
             * =>
             * F(X,x,t1,...,tm)
             */
          if(Flags.debug) {
            System.out.println("Case 3.2 (3): " + this);
          }
          addExtensionVariable();
          transformationDone = true;
        } else if((nbSingleACVariable + nbMultiACVariable > 1) ||
                  (nbMultiACSubterm > 0)) {
            /*
             * F(x1^alpha_1,...,xn^alpha_n,t1^beta_1,...,tm^beta_m)
             * avec n > 1 ou beta_i != 1
             * =>
             * F(X,Y,ti^beta_i=1,...,tj^beta_j=1)
             *   where F(x1^alpha_1,...,xn^alpha_n,...,tk^beta_k>1,...) :=() Y
             */
          if(Flags.debug) {
            System.out.println("Case 3.2 (4): " + this);
          }
          List list = new ArrayList();
          for(int i=0 ; i<leftside.arity() ; i++) {
            Term t = leftside.getSubterm(i);
            if(t.isVariable() || t.getMultiplicity()>1) {
              list.add(t);
            }
          }
          addExtensionVariable();
          abstractACSubterm(list);
          transformationDone = true;
        }
      }
    }

    if(transformationDone && Flags.debug) {
      System.out.println("After: " + this);
    }
    return transformationDone;
  }


  private void addExtensionVariable() {
      // get a fresh number and update the MaxVariableNumber
    int varNumber = maxVariableNumber++;
      // create an extension variable
    Symbol source = leftside.getSymbol();
    Symbol symb = SymbolExtensionVariable.getCreate(varNumber,source.getSort());
    Term extensionVariable = new Term(symb,null);
    leftside.getSubterms().addElement(extensionVariable);
    extensionVariable.setFather(leftside);
    leftside.orderedNormalForm();
    Vector subterms = new Vector();
    subterms.addElement(extensionVariable.deepCopy());
    subterms.addElement(rightside);
    rightside = new Term(source,subterms);
    rightside.flatten();
    rightside.orderedNormalForm();
  }
  
    /*
      private void addExtensionVariable(Term variable) {
      Symbol source = leftside.getSymbol();
        // ne rien faire si la variable apparait dans le rhs
          // sous le meme symbole AC
          if(source.equals(rightside.getSymbol())) {
          for(int i=0 ; i<rightside.arity() ; i++) {
          if(rightside.getSubterm(i).getSymbol().equals(variable.getSymbol())) {
          return;
          }
          }
          }
          Vector subterms = new Vector();
          subterms.addElement(variable.deepCopy());
          subterms.addElement(rightside);
          rightside = new Term(source,subterms);
          rightside.orderedNormalForm();
          }
    */
  
} // end of class

