import java.util.*;

public class DDNode {
    /**
     * chaque noeud contient un tableau qui associe a chaque symbole
     * un lien vers le noeud suivant :
     *
     * [a|b|c|*]
     *  | | |  \
     *  |
     * [a|b|c|*]
     */
  private DDNode[] next;
    /**
     *  lien vers l'arbre contenant ce noeud
     */
  private DDTree tree;
    /**
     * maskTerm est un tableau de termes (les motifs) en forme
     * aplatie
     */
  private Flatterm[] maskTerm;

    /**
     * indique si le noeud est une feuille du DDTree
     * i.e. contient une liste de motifs reconnus
     */
  private boolean isLeaf = false;
    /**
     * pour les feuilles uniquement
     * 
     */
  private Information[] infoArray;

    /**
     * lien vers le noeud contenant un symbole pere
     */
  private DDNode top;

  private static int labelCounter=1;
  private int label;
  private DDNode jumpNode;
  

  public DDNode(DDTree tree) {
    this.tree = tree;
    next = new DDNode[tree.getNodeSize()];
    maskTerm = new Flatterm[maskSize()];
    label=labelCounter++;
  }

  private int starNumber() {
    return tree.starNumber;
  }
  public int maskSize() {
    return tree.getBitSetSize();
  }

  public Flatterm getMaskTerm(int i) {
    return maskTerm[i];
  }

  private DDNode getTopNode() {
    return top;
  }

  private DDNode retrieve(Flatterm term,Flatterm nextTerm) {
      //System.out.println("term     = " + term);
      //System.out.println("nextTerm = " + nextTerm);
        
    if( term==null || term.equals(nextTerm) ) {
      return this;
    }

    if(term.getTerm().containsAC()) {
      throw new InternalError("this method is not implemented for term with AC symbols");
    }

      // A chaque symbol est associee une position dans next[]
    Integer noSymbol = (Integer)tree.getTable().get(term.getSymbol());
    DDNode nextNode = next[noSymbol.intValue()];
      //System.out.println("noSymbol = " + noSymbol);
        
    if(nextNode == null) {
      throw new InternalError("probleme : nextNode == null");
    }
    return nextNode.retrieve(term.getNext(),nextTerm);
  }

  public DDNode addTerm(Flatterm term,Flatterm nextTerm, Information info) {
   Integer noSymbol;
   DDNode nextNode;

     // System.out.println("DDNode.addTerm: " + term );
   if( term==null ) {
     if(infoArray==null) {
       infoArray = new Information[tree.getBitSetSize()];
       isLeaf=true;
     }

     if( infoArray[info.getNumber()] == null ) {
       infoArray[info.getNumber()] = info;
     } else {
       /* ce n'est pas grave */
//       System.out.println("merge infoArray[info.getNumber()]: " + infoArray[info.getNumber()]);
     }
     return this;
   } else if ( term.equals(nextTerm) ) {
     return this;
   }

   // A chaque symbol est associee une position dans next[]
   noSymbol = (Integer)tree.getTable().get(term.getSymbol());
   nextNode = next[noSymbol.intValue()];

     //System.out.println("noSymbol = " + noSymbol);
   
   
   boolean newNextNode=false;   
   if(nextNode == null) {
     nextNode = new DDNode(tree);
     next[noSymbol.intValue()] = nextNode;
     newNextNode=true;   
   }

     /*
      * Optimisation pour eviter d'inserer plusieurs fois le meme motif
      * seulement si le term ne contient pas de symbole AC
      * sinon il faudrait mettre a jour la hashtable dans ACDDTree
      */
     /*
   if(!term.getTerm().containsAC()) {
     for(int i=0 ; i<maskSize() ; i++) {
       if(maskTerm[i] != null &&
          maskTerm[i].getTerm().equals(term.getTerm())) {
         System.out.println("*** " + term + " est deja dans le DDTree");
         return retrieve(term,nextTerm);
       }
     }
   }
     */
   
   if(maskTerm[info.getNumber()] == null) {
     maskTerm[info.getNumber()]=term;
   } else {
     //throw new InternalError("re-insertion de : " + term);
     if(!maskTerm[info.getNumber()].equals(term)) {
       //System.out.println("re-insertion en [" + info.getNumber() + "] de : " + term);
       //maskTerm[info.getNumber()]=term;
     }
   }
     
   if( term.getSymbol().isVariable() ) {
     return addVariable(nextNode, term, nextTerm, info);
   } else if( term.getSymbol().isFree() ||
	      term.getSymbol().isBuiltin() ||
	      term.getSymbol().isValue()
	      ) {
     return addFreeSymbol(nextNode, newNextNode, term, nextTerm, info);
   } else if( term.getSymbol().isAC() ) {
     return addACSymbol(nextNode, newNextNode, term, nextTerm, info);
   } else {
     throw new InternalError("undefined symbol");
   }
  }

  private void merge(DDNode node) {
      // System.out.println("merge : isLeaf = " + isLeaf);
    if(isLeaf) {
      
      for(int i=0 ; i<node.infoArray.length ; i++) {
	if(this.infoArray[i] == null) {
	  this.infoArray[i] = node.infoArray[i];
	}
      }

      if(node.isLeaf==false) {
	throw new InternalError("merge: leaf problem");
      }
    }

    for(int i=0 ; i<node.maskSize() ; i++) {
      if( node.maskTerm[i] != null ) {
	Information info = new Information(tree,i);
	this.addTerm(node.maskTerm[i],null,info);
      }
    }
  }

  private DDNode addFreeSymbol(DDNode nextNode, boolean newNode,
			       Flatterm term, Flatterm nextTerm,
			       Information info) {
    
    //if(next[starNumber()] == null) {
    // s'il n'y a pas de variable

    // Version corrigee de l'algo en s'inspirant de l'algo d'A. Graf
      /*
    boolean differentTopSymbol=false;
    if(term.getNext()!=null) {
      differentTopSymbol=!term.getSymbol().equals(term.getNext().getSymbol());
    }
      */
    //System.out.println("Ajout du symbole " + term.getSymbol());
    if(next[starNumber()] == null) {
	/*
	 * //|| !newNode) { // || differentTopSymbol) {
	 * 17/08/98 : ces optimisations me semblent fausses !!!
	 */
      // s'il n'y a pas de variable
      // ou si L/s!=emptyset 
      // ou si {p}/s==emptyset
      return nextNode.addTerm( term.getNext(), nextTerm, info );
    } else {
      // s'il y a deja une variable
      DDNode tmpNode;
      DDNode starNode=next[starNumber()];
      // Insertion du sous-terme
        // System.out.println("Insertion du sous-terme : " + term.getNext());
      tmpNode=nextNode.addTerm( term.getNext(),
				term.getEnd().getNext(), info );
      // Merge des branches sous l'*
        // System.out.println("Merge des branches sous l'*");
      tmpNode.merge(starNode);
      // L'insertion continue
      return tmpNode.addTerm( term.getEnd().getNext(), nextTerm, info );
    }
  }

  private DDNode addACSymbol(DDNode nextNode, boolean newNode,
			     Flatterm term, Flatterm nextTerm,
			     Information info) {
    int numberRule;
    int tmp;
    
    if( tree.ACnet==null) {
      tree.ACnet = new ACDDTree(term.getTerm().getSymbol(),tree.getRootFather());
    }
    // insertion d'une copie des sous-termes AC dans tree.ACnet
    Term copy = term.getTerm().deepCopy();
    tmp = tree.ACnet.ACinit(copy);

    if(tmp>=0) {
        // maj de information
      info.addInfo(tree.ACnet,copy);
    }
    
    if(next[starNumber()] == null) {
      // insertion du reste
      return nextNode.addTerm( term.getEnd().getNext(), nextTerm, info );
    } else {
        // s'il y a deja une variable
      DDNode starNode=next[starNumber()]; 
        /*
         * [pem: May 20 99]
         * si le symbole AC est une feuille
         */
      if(term.getEnd().getNext() == null) {
          // System.out.println("Insertion du sous-terme vide");
        nextNode.addTerm(null,null, info );
      }

        // Merge des branches sous l'*
        // System.out.println("ACMerge des branches sous l'*");
      nextNode.merge(starNode); 
      // L'insertion continue
      return nextNode.addTerm( term.getEnd().getNext(), nextTerm, info ); 
    }
  }


  private DDNode addVariable(DDNode nextNode,
			     Flatterm term, Flatterm nextTerm,
			     Information info) {
    // System.out.println("Ajout d'une variable");
    for(int i=1 ; i<tree.getNodeSize() ; i++) {
      if(next[i] != null) {
	next[i].addSkipTerm(tree.getSymbol(i).getACArity(),term.getNext(), info);
      }
    }
    return nextNode.addTerm(term.getNext(), nextTerm, info );
  }

  private void addSkipTerm(int level, Flatterm term, Information info) {
//    System.out.println("level = " + level);
//    System.out.println("term  = " + term);
    
    if(level == 0) {
      this.addTerm(term,null,info);
      return ;
    } else {
      for(int i=0 ; i<tree.getNodeSize() ; i++) {
	if(next[i] != null) {
	  int arity;
	  if(i==starNumber()) {
	    arity=0;
	  } else {
	    arity=tree.getSymbol(i).getACArity();
	  }
	  next[i].addSkipTerm(level-1+arity, term, info);
	}
      }
    }
  }

  /*
   * Correction d'un probleme : il manquait des branches dans le DDTree
   * Chaque noeud sans * va faire un saut en cas d'echec
   * Le saut se fait vers le noeud correspondant au pere du noeud courant.
   */
  public void secondPass() {
      //System.out.println("secondPass: " + this);
    if(next[starNumber()] == null) {
      if(getTopNode()==null) {
	  //System.out.println("*** topNode       = " + getTopNode());
	jumpNode=null;
      } else {
	  //System.out.println("*** topNode       = " + getTopNode());
	jumpNode = getTopNode().jumpNode;
	/*
	 * Il faut remonter les jumpNode ICI
	 * arret si jumpNode==this ou jumpNode==null
	 */
	while(jumpNode!=null && jumpNode!=jumpNode.jumpNode) {
	  jumpNode=jumpNode.jumpNode;
	}
      }
    } else {
      jumpNode=this;
    }
    /*
     * appel recursif sur les fils
     */
    for(int i=0 ; i<tree.getNodeSize() ; i++) {
      if(next[i] != null) {
	next[i].secondPass();
	//System.out.println("*** next branch");
      }
    }
  }

  private static void stackCopy(Stack dest, Stack source) {
    if(!source.empty()) {
      Object obj = source.pop();
      stackCopy(dest,source);
      dest.push(obj);
    }
  }

  public void backChaining(Stack paramStackNode, Stack paramStackLevel) {
    Stack stackNode, stackLevel;

    for(int i=0 ; i<tree.getNodeSize() ; i++) {
      if(next[i] != null) {
	stackNode = (Stack) paramStackNode.clone();
	stackLevel= (Stack) paramStackLevel.clone();

	//System.out.println("stackNode:  " + paramStackNode);
	//System.out.println("stackLevel: " + paramStackLevel);

	int arity;
	if(i==starNumber()) {
	  arity=0;
	  //System.out.println("symbol var");
	} else {
	  arity=tree.getSymbol(i).getACArity();
	  //System.out.println("symbol: " + tree.getSymbol(i));
	}

	if(arity > 0) {
	  stackNode.push(this);
	  stackLevel.push(new Integer(arity));
	  //System.out.println("on empile:      [" + this + "," + arity + "]");
	  next[i].backChaining(stackNode, stackLevel);
	} else {
	  try {
	    this.top=(DDNode)stackNode.peek();
	    //System.out.println("on chaine vers: " + this.top);

	    int level=((Integer)stackLevel.pop()).intValue();
	    DDNode node=(DDNode)stackNode.pop();
	    //System.out.println("on depile:      [" + node + "," + level + "]");

	    while(level==1) {
	      //System.out.println("boucle");
	      node.top=(DDNode)stackNode.peek();
	      //System.out.println("on chaine (" + node + ") vers: " + node.top);
	      node=(DDNode)stackNode.pop();
	      level=((Integer)stackLevel.pop()).intValue();
	      //System.out.println("on depile:      [" + node + "," + level + "]");
	    }
	    
	    stackNode.push(node);
	    stackLevel.push(new Integer(level-1));
	    //System.out.println("modif sommet de pile et recursion");
	      
	    //node  = (DDNode)stackNode.peek();
	    //level = ((Integer)stackLevel.peek()).intValue();
	    //System.out.println("on re-empile:      [" + node + "," + level + "]");
	    next[i].backChaining(stackNode, stackLevel);

	  } catch(EmptyStackException e) {
              //System.out.println("pile vide");
	  }
	}
      }
    }
    //System.out.println("return");
  }

  private int nbFils() {
    int res=0;
    for(int i=0 ; i<tree.getNodeSize() ; i++) {
      if(next[i] != null) {
	res++;
      }
    }
    return res;
  }


  public String toString(int deep) {
    StringBuffer s = new StringBuffer();

    for(int i=tree.getNodeSize()-1 ; i>starNumber() ; i--) {
      if(next[i] != null) {
	Tools.indent(s,deep);
	s.append("case " + tree.getSymbol(i).genInstance() +
		 ": /* " + tree.getSymbol(i) + " */\n");

	s.append(next[i].toString(deep+1));
      }
    }

    if(isLeaf) {
      for(int i=0 ; i<infoArray.length ; i++) {
	if(infoArray[i] != null) {
	  Tools.indent(s,deep); s.append(infoArray[i] + "\n");
	}
      }
    }

    if(next[starNumber()] != null) {
      Tools.indent(s,deep); s.append("default:\n");
      Tools.indent(s,deep); s.append("label" + label + ":\n");
      s.append(next[starNumber()].toString(deep+1));
    } else {
      if(jumpNode!=null) {
	Tools.indent(s,deep);   s.append("default:\n");
	Tools.indent(s,deep+1); s.append("goto label" + jumpNode.label + ";\n");
      }
    }
    

    return s.toString();
  }

  /*
  public String toString() {
    return toString(0);
  }
  */

  /**
   * Numerotation des variables du lhs
   */
 public int leftsideVariableAffectation(int startVarNumber) {
    int max = startVarNumber;

    // Noeud qui n'est pas une feuille
    if(!isLeaf) {
     //System.out.println("startVarNumber = " + startVarNumber);
      
      /*
       * Numerotation des sommets des termes de maskTerm
       * Danger : 2 elements du meme maskTerm peuvent avoir des no differents
       */
      for(int i=0 ; i<maskSize() ; i++) {
        if(maskTerm[i] != null) {

          //System.out.println("maskTerm["+i+"] = " + maskTerm[i].getTerm());

          if( maskTerm[i].getTerm().getVarNumber() == -1 ) {
            maskTerm[i].getTerm().setVarNumber(startVarNumber);
            maskTerm[i].getTerm().setMatchingVariable();
            if(startVarNumber>=max) {
              max = startVarNumber;
            }
	    // si le term est deja numerote on ne fait rien
          } else if(maskTerm[i].getTerm().getVarNumber() >= max) {
            max = maskTerm[i].getTerm().getVarNumber();
          }
        }
      }

      max++;
      // max est le numero libre suivant

      /*
       * on numerote recursivement les sous-termes a partir de max
       */
      for(int i=tree.getNodeSize()-1 ; i>=0 ; i--) {
        if(next[i] != null) {
          int r = next[i].leftsideVariableAffectation(max);
          if(r>max) {
            max=r;
          }
        }
      }
    }
    return max;
  }

  /**
   * debut de generation du filtrage avec memorisation des sous-termes
   */
  public void genMatchVariable(OutputCode s, int deep, int startVarNumber,
			       boolean isSyntacticCase, int strategyLab,
                               Vector rules) {
    for(int i=tree.getNodeSize()-1 ; i>=0 ; i--) {
      if(next[i] != null) {
	boolean incrementNbBit=false;
	String prefix ="";
	next[i].rgenMatchVariable(s,deep,startVarNumber,
				  isSyntacticCase,
				  incrementNbBit,
				  0,
				  prefix,
				  strategyLab,
                                  rules); 
      }
    }
  }

  public void rgenMatchVariable(OutputCode s, int deep, int startVarNumber,
				boolean isSyntacticCase,
				boolean incrementNbBit,
				int fatherVarNumber,
				String prefix,
				int strategyLab,
                                Vector rules) {
    /*
     * fatherVarNumber est utilise pour compiler les feuilles AC
     */
    Flatterm term=null;

    // Noeud qui n'est pas une feuille
    if(!isLeaf) {
      BitSet affectedVar = new BitSet();
      for(int i=0 ; i<maskSize() ; i++) {
	if(maskTerm[i] != null) {
	  Term father = maskTerm[i].getTerm().getFather();
	  int varNumber = maskTerm[i].getTerm().getVarNumber();
	  // term est un des patterns possibles
	  term = maskTerm[i];
	  // si son numero est nouveau, on affecte la variable
	  // DANGER : les peres n'ont pas forcement le meme numero
	  if(varNumber >= startVarNumber && !affectedVar.get(varNumber)) {
	    affectedVar.set(varNumber);
	    s.write(deep,"v" + varNumber + "=" + "v" + 
		    father.getVarNumber() +
		    "->sub[" + maskTerm[i].getTerm().getChildNumber() +"];\n");
	  }
	}
      }
      if(term==null) {
	throw new InternalError("empty maskTerm");
      }

      s.write(deep,"switch(" + term.getTerm().getSymbol().genAccess() +
	      "(v" + term.getTerm().getVarNumber() + ")) {\n");
      int currentVarNumber = term.getTerm().getVarNumber();

      for(int i=tree.getNodeSize()-1 ; i>starNumber() ; i--) {
	if(next[i] != null) {
	  s.write(deep,"case " + tree.getSymbol(i).genInstance() +
		  ": /* " + tree.getSymbol(i) + " */\n");
	  
	  next[i].rgenMatchVariable(s, deep+1, startVarNumber,
				    isSyntacticCase,
				    incrementNbBit,
				    currentVarNumber,prefix,
				    strategyLab,rules);
	  s.write(deep+1,"break;\n");
	}
      }
      
      if(next[starNumber()] != null) {
	   s.write(deep,"default:\n");
	   s.write(deep,"label" + label + ":\n");
	   next[starNumber()].rgenMatchVariable(s, deep+1, startVarNumber,
						isSyntacticCase,
						incrementNbBit,
						currentVarNumber,
						prefix,
						strategyLab,rules);
      } else {
	if(jumpNode!=null) {
	  s.write(deep,"default:\n");
	  s.write(deep+1,"goto label" + jumpNode.label + ";\n");
	} else {
	  s.write(deep,"/* matching is not complete: jumpNode is null */\n");
	}
      }
	 
      s.write(deep,"}\n");
	 
    } else {
      // traitement des feuilles

      int minNumber=infoArray.length+1;
      for(int i=0 ; i<infoArray.length ; i++) {
	if(infoArray[i] != null) {
	  //Tools.indent(s,deep);
	  //s.write("mask.set(" + infoArray[i] + ");\n");
	  if(infoArray[i].getNumber() < minNumber) {
	    minNumber = infoArray[i].getNumber();
	  }

	  String name = prefix;
	  if(tree.getACnet() != null) {
	    name += "_" + tree.getACnet().getSymbol().getSymbolCode();
	  }
 
	  //System.out.println("\tisFunctionTree = " + tree.isFunctionTree() + "\tname = " + name );



            /*
             * [pem: Jul 12 00]
             * Pour optimiser filtrage/application Rhs
             */
          boolean allRulesWithoutLocalEvaluation = true;
          if(Flags.withGoto) {
            // RewriteRule rule = (RewriteRule) rules.elementAt(infoArray[i].getNumber());
            for(int j=0 ; j<rules.size() ; j++) {
              RewriteRule rule = (RewriteRule) rules.elementAt(j);
              allRulesWithoutLocalEvaluation = allRulesWithoutLocalEvaluation &&
                (rule.getBranchEvaluationSize()==0);
            }
          }
          
          if(Flags.withGoto &&
             isSyntacticCase && allRulesWithoutLocalEvaluation) {
            if(strategyLab<0) {
              s.write(deep,"goto labelRhs" + infoArray[i].getNumber() + ";\n");
            }
              /*
                if(strategyLab>=0) {
                s.write(deep,"goto labelRhs" + strategyLab + "_" + minNumber + ";\n");
                } else {
                s.write(deep,"goto labelRhs" + minNumber + ";\n");
                }*/
          }
          infoArray[i].compile(s,deep, name, isSyntacticCase,incrementNbBit,
                                 fatherVarNumber);
        }
      }
    }
  }
  
    /*
     * recupere les informations associees a toutes les feuilles
     * les trie en fonction du ruleNumber (info.getNumber())
     */
  public Information[] collectInformation() {
    Information[] list = new Information[tree.getBitSetSize()];
    rcollectInformation(list);
    return list;
  }

  private void rcollectInformation(Information[] list) {
    for(int i=tree.getNodeSize()-1 ; i>=0 ; i--) {
      if(next[i] != null) {
	next[i].rcollectInformation(list);
      }
    }
    if(isLeaf) {
      for(int i=0 ; i<infoArray.length ; i++) {
    	if(infoArray[i] != null) {
          int cbgNumber = infoArray[i].getCBGNumber();
          if(cbgNumber>=0) {
            if(list[cbgNumber] == null) {
              list[cbgNumber]=infoArray[i];
            } else {
              list[cbgNumber].merge(infoArray[i]);
            }
          }
        }
      }
    }
  }
  

  public int maxNbPatternUnderAC() {
    int max=0;
    int res;
    for(int i=tree.getNodeSize()-1 ; i>=0 ; i--) {
      if(next[i] != null) {
	res = next[i].maxNbPatternUnderAC();
	if(res>max)
	  max=res;
      }
    }
    if(isLeaf) {
      for(int i=0 ; i<infoArray.length ; i++) {
	if(infoArray[i] != null) {
	  res = infoArray[i].maxNbPatternUnderAC();
	  if(res>max)
	    max=res;
	}
      }
    }
    return max;
  }

} // end class DDNode




