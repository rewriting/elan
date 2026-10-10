/*
  
    REM - Reduce ELAN Machine

    Copyright (C) 2000-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
			     Nancy, France.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr

*/
package rem.compiler;

import java.util.*;

public class Query {
  private Lexem stratName;
  private Term queryTerm;
  private Term startTerm;
  private int sourceSort;
  
  public Query(int sourceSort, Lexem name, Term term) {
    this.sourceSort = sourceSort;
    stratName=name;
    startTerm=term;
    if(term.isGround()) {
      queryTerm=term;
    } else {
        //System.out.println("***\n*** The compiled query is not ground\n***");
      queryTerm=null;
        //throw new InternalError("");   
    }
  }

  public int getSourceSort() {
    return sourceSort;
  }
  public int getSourceStrategy() {
    if(stratName!=null) {
      return stratName.getCode();
    } else {
      return 0;
    }
  }
  
  public void genCode(OutputCode s,int deep) {
    BitSet b = new BitSet();

    if(queryTerm!=null) {
      
      queryTerm.setFullNumbering();
      queryTerm.rightsideVariableAffectation();
      queryTerm.rightsideVariableLiberation();
    
      s.write(deep,"{\n");
      s.write(deep+1,"/* " + queryTerm + " */\n");
    
        //queryTerm.markUsedVariable(b);
        //Tools.genDeclaration(s,deep+1,b);
      Tools.genDeclaration(s,deep+1, queryTerm.getMaxUsedVariable(0) );
    
      queryTerm.genTermConstruction(s,deep+1);
      if(stratName==null) {
        s.write(deep+1,"res=" + queryTerm.genCore() + ";\n");
      } else {
        Strategy strategy = Strategy.getStrategy(stratName);
        s.write(deep+1,"res=strTab[" + strategy.getCodeName() +
                "](" + queryTerm.genCore() + ");\n");
      }
      s.write(deep,"}\n");
    } else {
      s.write(deep,"printf(\"No ground term is defined\\n\");\n");
      s.write(deep,"printf(\"Do not use -noInput in this case\\n\");\n");
      s.write(deep,"exit(0);\n");
    }
    
  }

  /*
   * Gterm *main_query_start(Gterm *q): the start term of the query
   * (`start with (S) t`) where the variable `query` is bound to q, the term
   * read; the runtime applies S to it, as the interpreter does.
   */
  public void genStartTerm(OutputCode s,int deep) {
    s.write("Gterm *main_query_start(Gterm *q) {\n");
    if(startTerm.isVariable()) {
      s.write(deep+1,"return q;\n");
    } else {
      s.write(deep+1,"Gterm *res;\n");
      startTerm.setFullNumbering();
      startTerm.rightsideVariableAffectation();
      startTerm.rightsideVariableLiberation();
      s.write(deep+1,"{\n");
      s.write(deep+2,"/* " + startTerm + " */\n");
      Tools.genDeclaration(s,deep+2, startTerm.getMaxUsedVariable(0) );
      genBindQuery(startTerm,s,deep+2);
      startTerm.genTermConstruction(s,deep+2);
      s.write(deep+2,"res=" + startTerm.genCore() + ";\n");
      s.write(deep+1,"}\n");
      s.write(deep+1,"return res;\n");
    }
    s.write("}\n");
  }

  private static void genBindQuery(Term t,OutputCode s,int deep) {
    if(t.isVariable()) {
      s.write(deep,t.genCore() + "=q;\n");
    } else {
      for(int i=0 ; i<t.arity() ; i++) {
        genBindQuery(t.getSubterm(i),s,deep);
      }
    }
  }

}
