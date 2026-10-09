import java.util.*;

public class DDTree {
  private Hashtable table = new Hashtable(); // table symbol --> numero
  private Vector vector = new Vector(); // table numero --> symbol
  protected DDNode node;

  protected Vector pattern = new Vector(); // liste des patterns

  public ACDDTree ACnet; // pour les symbols AC

  //private static Vector trees = new Vector();

  private static int label=0;

  private Object symbol; // symbol de tete de l'arbre
  // On suppose qu'il n'y en a qu'un

  private boolean isFunctionTree=true;


  public DDTree(Object symbol) {
    this.symbol=symbol;
  }

  /*
  public static void put(DDTree tree) {
    trees.addElement(tree);
  }
  */

  protected DDTree getRootFather() {
    return this;
  }

  public Hashtable getTable() {
    return table;
  }

  int getNodeSize() {
    return vector.size()+1; // pour l'*
  }
  int getBitSetSize() {
    return pattern.size();
  }

  public boolean isFunctionTree() {
    return isFunctionTree;
  }
  public void setFunctionTree() {
    isFunctionTree=true;
  }
  public void clearFunctionTree() {
    isFunctionTree=false;
  }

  public ACDDTree getACnet() {
    return ACnet;
  }

  public static int starNumber = 0;

  public Symbol getSymbol(int number) {
    return (Symbol)vector.elementAt(number-1); // decalage de l'*
  }

  public Symbol getSymbol() {
    if(isFunctionTree()) {
      return (Symbol) symbol;
    }
    throw new InternalError("getSymbol: bad usage");
  }
  public StrategyTerm getStrategyTerm() {
    if(!isFunctionTree()) {
      return (StrategyTerm) symbol;
    }
    throw new InternalError("getStrategyTerm: bad usage");
  }

  public String getName() {
    if(isFunctionTree()) {
      return "";
    } else {
      return getStrategyTerm().getName();
    }
  }

  private void initTable(Term current) {
    if( current.getSymbol().isVariable()  ) {
      if( table.get(current.getSymbol()) == null ) {
	table.put(current.getSymbol(), new Integer(starNumber));
      }	
    } else {
      if( table.get(current.getSymbol()) == null ) {
	vector.addElement(current.getSymbol());
	table.put(current.getSymbol(), new Integer(vector.size()));
      }
    }
    /*    
    System.out.println(current.getSymbol() + " --> " + 
		       ((Integer)table.get(current.getSymbol())) +
		       "\t(arity="+current.getSymbol().getArity()+")");
		       */	       
    for(int i=0 ; i<current.arity() ; i++) {
      initTable(current.getSubterm(i));
    }
  }

  public int init(Term term) {
    /*
    for(int i=0 ; i<pattern.size() ; i++) {
      if( term.cmp((Term)pattern.elementAt(i)) == 0 ) {
	return i;
      }
    } 
    */
    pattern.addElement(term);
    initTable(term);
    return pattern.size();
  }
  
  /**
   * Construit le DDT
   * Retourne le nombre de sous-termes inseres
   */
  public int build() {
    for(int i = 0 ; i < pattern.size() ; i++) {
      Flatterm ft = new Flatterm((Term)pattern.elementAt(i));
      addTerm(ft, i);
    }

    /*
     * Patch de correction
     */
    if(node!=null) {
      node.backChaining(new Stack(), new Stack());
      node.secondPass();
    }

    if(ACnet != null) {
      int n = ACnet.build();
      // Si l'ACnet est vide on le detruit
      if(n==0) {
	ACnet = null;
      }
    }

    //System.out.println("Tree =\n" + this);

    return pattern.size();
  }

  private void addTerm(Flatterm term, int number) {
    //System.out.println("addTerm("+term+","+number+")");
    Information info = new Information(this,number);
    if(node == null) {
      node = new DDNode(this);
    }
    node.addTerm(term,null,info);
  }

    /**
     * retourne le numero du pattern
     * par rapport a la liste de patterns
     */
  public int getNumber(Term term) {
    for(int i=0 ; i<pattern.size() ; i++) {
      if( term.cmpRename((Term)pattern.elementAt(i)) == 0 ) {
	return i;
      }
    } 
    //return -1;
    throw new InternalError("pattern not found:" + term);
  }

  public Term getPattern(int i) {
    return (Term)pattern.elementAt(i);
  }

  public String toString() {
    StringBuffer s = new StringBuffer();
    for(int i = 0 ; i < pattern.size() ; i++) {
      s.append( "[" + i + "] " + pattern.elementAt(i) + "\n");
    }
    s.append( node.toString(0) );
    
    if(ACnet != null) {
      s.append("ACnet:\n" + ACnet);
    }

    return s.toString();

  }

  /**
   * Numerotation des nouvelles variables
   */
  public int leftsideVariableAffectation(int startVarNumber) {
    int res = node.leftsideVariableAffectation(startVarNumber);
    if(ACnet != null) {
      int ACstartVarNumber = 1;
      ACnet.leftsideVariableAffectation(ACstartVarNumber);
    }
    return res;
  }

  public void genMatchVariable(OutputCode s, int deep, int startVarNumber,
			       int strategyLab) {
    s.write(deep,"/* Begin syntactical matching */\n");

    boolean syntactic=true;
    node.genMatchVariable(s,deep,startVarNumber,syntactic,strategyLab);

    s.write(deep,"/* End syntactical matching */\n");
  }

  public void genACMatch(OutputCode s, int deep,
			 String prefix, String varCore,
                         int matchStateNumber) {
    if(ACnet!=null) {
      String name = prefix + "_" + ACnet.getSymbol().getSymbolCode();
      
      s.write(deep,"/* Begin AC matching */\n");
      if(ACnet.ACnet==null) {
	// 1 niveau
	s.write(deep,"necessary_link=0;\n");
      } else {
	// 2 niveaux
	s.write(deep,"necessary_link=1;\n");
      }

      // IL FAUT REMPLACER v0 PAR LE(S) SOUS-TERME(S) AC CONCERNE(S)
      s.write(deep,"indice = MS_init(&(ms[" + matchStateNumber +
              "]), match_subterm" + name +
	      ", no_pattern" + name +
	      "_niv_0, pattern_list" + name + ", nb_pattern" + name +
	      "_niv_1," + varCore +
	      ", necessary_link, max_nb_pattern_under" + name +");\n");
      s.write(deep,"/* End AC matching */\n");
    }
  }



  /**
   * Filtre les sous-termes d'un symbole AC
   */ 
  public void genMatchSubterm(OutputCode s,int deep, String prefix) {
     int ACstartVarNumber=1;

     if(ACnet == null) {
       return;
     }

     String name = prefix + "_" + ACnet.getSymbol().getSymbolCode();
       //System.out.println("prefix = " + prefix);
       //System.out.println("name   = " + name);
     s.write("\nint match_subterm" + name + "(struct term *v0,int no_arg_subject,int *mask,BG *cbg) {\n");

     int startVarNumber=1;
     int maxVarNumber = ACnet.leftsideVariableAffectation(startVarNumber);
     Term.genLeftsideDeclaration(s,deep+1,startVarNumber,maxVarNumber);

     s.write(deep+1,"int nb_bit=0;\n");
     //s.write(deep+1,"bitSet_init_clear(mask);\n");

     // AJOUTER UNE LIGNE nb_bit++ DANS rgenMatchVariable
     boolean isSyntacticCase=false;
     boolean incrementNbBit=true;
     /*
      * strategyLab ne sert a rien dans le cas AC
      */
     int strategyLab=0;
     ACnet.node.rgenMatchVariable(s,deep+1,ACstartVarNumber,
				  isSyntacticCase,incrementNbBit,0, name,
				  strategyLab);
     s.write(deep+1,"return nb_bit;\n");
     s.write("}\n");

     // Niveau suivant
     ACnet.genMatchSubterm(s,deep,name);
  }

  public String genMatchSubtermDeclaration(int deep, String prefix) {
    StringBuffer s = new StringBuffer();

    if(ACnet == null) {
      return "";
    }
    //String name = prefix + "_" + symbol.getSymbolCode();
    String name = prefix + "_" + ACnet.getSymbol().getSymbolCode();

    s.append("int match_subterm" + name + "(struct term *v0,int no_arg_subject, int *mask, BG *cbg);\n");
    s.append("void variable_extract" + name + "(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern, int base_id_pattern);\n");
    s.append("static int **pattern_list" + name + ";\n");
    s.append("static int no_pattern" + name + "_niv_0;\n");
    s.append("static int nb_pattern" + name + "_niv_0 = " + nb_pattern_level(0) + ";\n");
    s.append("static int nb_pattern" + name + "_niv_1 = " + nb_pattern_level(1) + ";\n");
    // ON PEUT AMELIORER LA BORNE MAX
    //s.append("#define max_nb_pattern_under" + name + " " + nb_pattern_level(1) + "\n"); 
    s.append("#define max_nb_pattern_under" + name + " " + node.maxNbPatternUnderAC() + "\n"); 

    // Niveau suivant
    s.append( ACnet.genMatchSubtermDeclaration(deep,name) );
    return s.toString();
  }


  /**
   * Pour construire une substitution a partir d'un filtre AC
   */
  public void genVariableExtract(OutputCode s,int deep, String prefix) {
     if(ACnet != null) {
       //String name = prefix + "_" + symbol.getSymbolCode();
       String name = prefix + "_" + ACnet.getSymbol().getSymbolCode();
     
       s.write(deep,"\nvoid variable_extract" + name + "(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern, int base_id_pattern) {\n");

       s.write(deep+1,"switch(id_pattern) {\n");

       for(int i=0 ; i<ACnet.node.maskSize() ; i++) {
	 if( ACnet.node.getMaskTerm(i) != null ) {
	   s.write(deep+2,"/* " + ACnet.node.getMaskTerm(i).getTerm() + " */\n");
	   s.write(deep+1,"case " + i + ":\n");
	   String name2 = name;
	   if(ACnet.ACnet != null) {
	     name2 = name + "_" + ACnet.ACnet.getSymbol().getSymbolCode();
	   }
	   ACnet.node.getMaskTerm(i).getTerm().genVariableExtract(s,deep+2,name2);

	   s.write(deep+2,"break;\n");
	 }
       }

       s.write(deep+1,"default:\n");
       s.write(deep+2,"fprintf(stderr,\"variable_extract" + name + ": bad pattern number\\n\");\n"); 
       s.write(deep+2,"exit(0);\n");
       s.write(deep+1,"}\n");
       s.write(deep,"}\n");
       ACnet.genVariableExtract(s,deep,name);
     }
  }

  /**
   * Creation des pattern_list
   */ 

  public int nb_pattern_level(int level) {
    if(ACnet==null) {
      return -1;
    }

    if(level==0) {
      //return ACnet.getACPatternSize(); // nb de pattern AC
      return ACnet.getNumberOfPattern(); // nb de pattern AC
    } else if(level==1) {
      return ACnet.getBitSetSize(); // nb de sous-pattern utilses
    } else {
      return -1;
    }

    /*
    if(level==0) {
      return node.maskSize();
    } else if(ACnet != null) {
      return ACnet.nb_pattern_level(level-1);
    } else {
      return 0;
    }
    */
  }

  public Information[] collectInformation() {
    return node.collectInformation();
  }

  public void genPatternListConstruction(OutputCode s,int deep, String prefix) {
    if(ACnet == null) {
      return ;
    }
    //String name = prefix + "_" + symbol.getSymbolCode();
    String name = prefix + "_" + ACnet.getSymbol().getSymbolCode();

    s.write(deep,"\nvoid init_pattern_list" + name + "() {\n");
    s.write(deep+1,"int pattern_tab[max_nb_pattern_under" + name + "];\n");
    s.write(deep+1,"no_pattern" + name + "_niv_0=0;\n");
    s.write(deep+1,"pattern_list" + name +
	    "=MS_pattern_list_create(nb_pattern" + name + "_niv_0);\n");

    //    s.write( ACnet.node.genPatternListConstruction(deep,name,level) );
      //node.genPatternListConstruction(s,deep+1,name);
    ACnet.genPatternListConstruction(s,deep,name);
    
    s.write(deep,"}\n");
    s.write(deep,"\nvoid delete_pattern_list" + name + "() {\n");
    s.write(deep+1,"MS_pattern_list_free(pattern_list" + name + ",no_pattern" +
	    name + "_niv_0);\n");
    s.write(deep,"}\n"); 

      // pour le deuxieme niveau
      // ACnet.genPatternListConstruction(s,deep,name);
  }

  public static void genPatternListDeclaration(OutputCode s,int deep) {
    DDTree tree;

    s.write(deep,"/* Declaration des pattern_list */\n");
    /*
    for(int i=0 ; i<trees.size() ; i++) {
      tree=(DDTree)trees.elementAt(i);
      s.append(tree.rgenPatternListDeclaration(deep,tree.getName()));
    }
    */
    s.write(deep,Tools.patternListDeclaration.stringDump());
  }

  public void rgenPatternListDeclaration(OutputCode s,int deep, String prefix) {
    if(ACnet != null) {
      //String name = prefix + "_" + symbol.getSymbolCode();
      String name = prefix + "_" + ACnet.getSymbol().getSymbolCode();
      s.write(deep,"void init_pattern_list" + name + "();\n");
      s.write(deep,"void delete_pattern_list" + name + "();\n");
      ACnet.rgenPatternListDeclaration(s,deep,name);
    }
  }
  

  public static void genPatternListInit(OutputCode s,int deep) {
    DDTree tree;
    s.write(deep,"/* Initialisation des pattern_list */\n");
    /*
    for(int i=0 ; i<trees.size() ; i++) {
      tree=(DDTree)trees.elementAt(i);
      s.append(tree.rgenPatternListInit(deep,tree.getName()));
    }
    */
    s.write(Tools.patternListInit.stringDump());
  }

  public void rgenPatternListInit(OutputCode s,int deep, String prefix) {
    if(ACnet != null) {
      //String name = prefix + "_" + symbol.getSymbolCode();
      String name = prefix + "_" + ACnet.getSymbol().getSymbolCode();
      s.write(deep,"init_pattern_list" + name + "();\n");
      ACnet.rgenPatternListInit(s,deep,name);
    }
  }

  public static void genPatternListDelete(OutputCode s,int deep) {
    DDTree tree;

    s.write(deep,"/* Destruction des pattern_list */\n");
    /*
    for(int i=0 ; i<trees.size() ; i++) {
      tree=(DDTree)trees.elementAt(i);
      s.append(tree.rgenPatternListDelete(deep,tree.getName()));
    }
    */
    s.write(Tools.patternListDelete.stringDump());
  }

  public void rgenPatternListDelete(OutputCode s,int deep, String prefix) {
    if(ACnet != null) {
      //String name = prefix + "_" + symbol.getSymbolCode();
      String name = prefix + "_" + ACnet.getSymbol().getSymbolCode();
      s.write(deep,"delete_pattern_list" + name + "();\n");
      ACnet.rgenPatternListDelete(s,deep,name);
    }
  }


  public void genManyToOneMatching(OutputCode s, int deep, String prefix,
				   Vector rules,
				   StrategyChooseRule sterm, int strategyLab) {
    int startVarNumber;

    boolean isSemiDetApply=false;
    if(sterm==null || sterm.isSemiDet()) {
      isSemiDetApply=true;
    }

    // syntactic many-to-one matching
    if(isFunctionTree()) {
      startVarNumber=getSymbol().getStartVarNumber();
    } else {
      startVarNumber=1;
    }

    /*
     * s'il y a une strategie, il faut filtrer le symbole de tete
     */
    if(sterm==null) {
      genMatchVariable(s,deep,startVarNumber,strategyLab);
    } else {
      boolean syntactic=true;
      boolean incrementNbBit=false;
      node.rgenMatchVariable(s, deep,startVarNumber,syntactic,incrementNbBit,0,prefix,
			     strategyLab);
    }

    // Numerotation des sous-termes et recherche des pattern AC
    boolean ACPattern=false;
    // tous les ifs/wheres sont deterministes
    boolean allRuleDetEvaluation=true;
    for(int i = 0 ; i < rules.size() ; i++) {
      RewriteRule rule = (RewriteRule)rules.elementAt(i);
      ACPattern = ACPattern || rule.hasACPattern();
      allRuleDetEvaluation = allRuleDetEvaluation && rule.allDetEvaluation();
    }


    /*
     * Preparation du filtrage des regles avec pattern AC
     */
    if(ACPattern) {
      /*
       * Recherche de toutes les positions de patterns AC
       * Hashtable varNumber --> BitSet de regles applicables
       */
      Hashtable tableOfBitsets = new Hashtable();

      for(int i = 0 ; i < rules.size() ; i++) {
	RewriteRule rule = (RewriteRule)rules.elementAt(i);
	if(rule.hasACPattern()) {
	  Term firstACPattern = rule.getLeftside().getFirstACPattern();
	  Integer varNumber = new Integer(firstACPattern.getVarNumber());
	  if(!tableOfBitsets.containsKey(varNumber)) {
	    tableOfBitsets.put(varNumber,new BitSet());
	  }
	  ((BitSet)tableOfBitsets.get(varNumber)).set(i);
	}
      }
      
      /*
       * Generation de la construction des match_state
       */
      int matchStateNumber=0;
      OutputCode matchCode = new OutputCode();
              
      Enumeration e = tableOfBitsets.keys();
      while(e.hasMoreElements()) {
	Integer varNumber = (Integer) e.nextElement();
	BitSet mask = (BitSet) tableOfBitsets.get(varNumber);
	Tools.indent(matchCode,deep);
	String tmpString = "if( ";
	for(int noRule=0 ; noRule<mask.size() ; noRule++) {
	  if(mask.get(noRule)) {
	    RewriteRule rule = (RewriteRule)rules.elementAt(noRule);
            rule.matchStateNumber=matchStateNumber;
            if(sterm!=null || rules.size()>32) {
	      matchCode.write(tmpString + "bitSet_get(mask," + noRule + ")");
	    } else {
	      matchCode.write(tmpString + "bitSet32_get(mask32," + noRule + ")");
	    }
	    tmpString = " || ";
	  }
	}
        
        matchCode.write(" ) {\n");
	matchCode.write(deep+1,"int necessary_link;\n");
	matchCode.write(deep+1,"ACPattern=1;\n");
	/*
	 * Procedure de filtrage AC : construction du probleme
	 * On recherche le premier pattern AC dans le CBG
	 */
	String varCore = "v" + varNumber.intValue();
	genACMatch(matchCode,deep+1,prefix,varCore,matchStateNumber);
        matchStateNumber++;
	matchCode.write(deep,"}\n");

      }
        // Allocation de ms[]
      if(matchStateNumber>0) {
        String tmpString = "ms=(match_state**) MALLOC(" +
        matchStateNumber + "*sizeof(match_state*));\n";
        s.write(deep,tmpString+matchCode.stringDump());
      } else {
        throw new InternalError("ms[] alloc problem");
      }
      
      
      
      /*
       * AC matching
       * set a CUTOPEN if a rule has an AC Pattern and is applied with
       * a one strategy
       */
      //if(isSemiDetApply && ACPattern) { // && !allRuleDetEvaluation) {
      //s.write(deep,"if(ACPattern)\n");
      //s.write(deep+1,"CUTOPEN(); /* AC matching */\n");
      //}
      
      /*
       * Recherche d'une solution... was here
       */
    }
    /*
     * A cet endroit, la generation du filtrage est terminee
     */


    // Choix d'une regle a appliquer et Construction du rhs
    for(int i = 0 ; i < rules.size() ; i++) {
      RewriteRule rule = (RewriteRule)rules.elementAt(i);
      boolean isLast = (i+1==rules.size()) ;
      boolean isFirst = (i==0) ;
      int myendLab = DDTree.label++;

      /*
       * Pour optimiser filtrage/application Rhs
       */
      if(Flags.withGoto && !rule.hasACPattern()) {
	if(strategyLab>=0) {
	  s.write(deep,"labelRhs" + strategyLab + "_" + i + ":;\n");
	} else {
	  s.write(deep,"labelRhs" + i + ":;\n");
	}
      }

      if(sterm!=null || rules.size()>32) {
	s.write(deep,"if(bitSet_get(mask," + i + ")) {\n");
      } else {
	s.write(deep,"if(bitSet32_get(mask32," + i + ")) {\n");
      }
      if(!rule.hasACPattern()) {
	/*
	 * Syntactic case
	 */
	rule.genApplication(s,deep+1,myendLab,sterm,strategyLab,isFirst,isLast,
			    ACPattern);
      } else if(rule.hasACPattern()) {
	/*
	 * AC matching
	 * set a CUTOPEN if a rule has an AC Pattern and is applied with
	 * a one strategy
	 */
	if(isSemiDetApply) {
	  s.write(deep+1,"if(ACPattern)\n");
	  s.write(deep+2,"CUTOPEN(); /* AC matching */\n");
	}
	
        if (Flags.debug) {
	  s.write(deep+1,"// printf(\"try rule " + i + "\\n\");\n");
	}

	/*
	 * AC case : Application de la regle
	 */
	boolean hasACnet=false;
	// Numero de la regle dans le CBG
	String name = prefix;
	int cbgNumber = -1;
	if(getACnet() != null) {
	  hasACnet=true;
	  Term firstACPattern = rule.getLeftside().getFirstACPattern();
	  cbgNumber = ACnet.getCBGNumber(firstACPattern);
	  //System.out.println(cbgNumber + " : " + rule);
	  name += "_" + getACnet().getSymbol().getSymbolCode();
	}
	rule.genACApplication(s,deep+1,myendLab,cbgNumber,hasACnet,
			      sterm,strategyLab,isFirst,isLast,name,
			      allRuleDetEvaluation);
      }

      s.write(deep+1,"myend" + myendLab +":;\n");
        /*
         * [pem: May  7 99]
         * pour bien resynchroniser l'indentation lorsqu'un echec de
         * strategie provoque un changement de regle
         */
      s.write(deep+1,"restoreGlobalIndent();\n");

      if(!rule.hasACPattern()) {
	if(Flags.optimiseChoicePoint) {
	  if( rule.getTopLevelDetType(sterm).isSemiDet() &&
	      !rule.allDetEvaluation() ) {
	    s.write(deep+1,"CUTCLOSE(); /* Wheres */\n");
	  }
	} else {
	  if( rule.getTopLevelDetType(sterm).isSemiDet() ) {
	    s.write(deep+1,"CUTCLOSE(); /* Wheres */\n");
	  }
	}
      } else {
	/*
	 * AC matching
	 * set a CUTOPEN if a rule has an AC Pattern and is applied with
	 * a one strategy
	 */
	if(isSemiDetApply) {
	  s.write(deep+1,"if(ACPattern)\n");
	  s.write(deep+2,"CUTCLOSE(); /* AC matching */\n");
	}
      }
      s.write(deep,"}\n");
    }
  }

  public void genStrategyManyToOneMatching(OutputCode s,
					   int deep,
					   String prefix,
					   Vector rules,
					   StrategyChooseRule sterm,
					   int strategyLab) {
    // Declaration des variables
    int startVarNumber = 1;

    // on numerote les variables du membre gauche
    int maxVarNumber = leftsideVariableAffectation(startVarNumber);
    
    Term.genLeftsideDeclaration(s,deep,startVarNumber,maxVarNumber);

    boolean ACPattern=false;
    for(int i = 0 ; i<rules.size() && ACPattern==false ; i++) {
      RewriteRule rule = (RewriteRule)rules.elementAt(i);
      if(rule.hasACPattern()) {
	   ACPattern=true;
      }
    }
    // Pour le filtrage AC
    if(ACPattern) {
      s.write(deep,"match_state **ms=NULL;\n");
      //s.write(deep,"int mode;\n");
      s.write(deep,"int necessary_link;\n");
      s.write(deep,"int indice=-1;\n");
      s.write(deep,"int ACPattern=0;\n");
    }
    
    s.write(deep,"bitSet_GC_create(mask," + rules.size() + ");\n");
    s.write(deep,"bitSet_init_clear(mask);\n");
    //Tools.indent(s,deep); s.write("addindent();\n");

    genManyToOneMatching(s,deep,prefix,rules,sterm,strategyLab);
  }

  public void construction(Vector rules, boolean isFunctionTree) {
    //DDTree.put(this);

    this.isFunctionTree=isFunctionTree;
    // Calcul des variables v necessaires et initialisation de l'arbre
    for(int i = 0 ; i < rules.size() ; i++) {
      RewriteRule rule = (RewriteRule) rules.elementAt(i);
      Term ls = rule.getLeftside();
      /*
       * 0 pour ls
       * 1..n pour les fils
       */
      rule.variableClear();
      //ls.leftsideVariableClear();
      ls.leftsideVariableInit(isFunctionTree()); // il faut numeroter les sous-termes
      init( ls );
    }
    // Construction de l'arbre
    build();
    /*
     * A faire apres la construction de l'arbre, sinon l'ACnet est null
     */
    if(ACnet != null) {
      // il faut propager l'info aux sous-arbres
      //ACnet.isFunctionTree=this.isFunctionTree;
      ACnet.leftsideVariableClear();
      ACnet.leftsideVariableInit(isFunctionTree());
    }
  } 

}





