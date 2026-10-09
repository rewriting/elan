/*
  
    ELAN

    Copyright (C) 1994-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
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

    Peter Borovansky		e-mail: borovan@fmph.uniba.sk
    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr
    Marian Vittek		e-mail: vittek@guma.ii.fmph.uniba.sk

*/


#include "commondefs.h"
#include "module.h"
#include "rtdatas.h"
#include "termdefs.h"
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include "meta.h"
#include "strategy.h"
#include "string.h"

void skip_chars(char *s,int *si)
{
  while (s[*si] <= ' ' && s[*si] != 0) (*si)++;
}

char *get_symbol(char *&s)
{
char lexem[STRLEN];
int  li = 0;
int  si = 0;

 skip_chars(s,&si);
 if (isalpha(s[si])) {
    do { 
      lexem[li++] = s[si++];
    } while (isalpha(s[si]));
  } else if (isdigit(s[si])) {
    do { 
      lexem[li++] = s[si++];
    } while (isdigit(s[si]));
  } else
    lexem[li++] = s[si++];
  s += si; lexem[li] = 0;

  //stout << ":" << lexem << ":" << si << "\t" << s << "\n";

  return strdup(lexem);
}

term *refstring2term(char *&s, char *lookahead) 
{
char *lex;
term *tt, *rt;
lexem typel;
int  varn;

  // stout << "refstring2term " << s << "," << lookahead << "\n";

  if (lookahead == NULL)
    lex = get_symbol(s);
  else
    lex = lookahead;
  NNEW(rt,term); rt->stinit();
  if (!strcmp(lex,"FSYM")) {
    lex = get_symbol(s);  // '('
    lex = get_symbol(s);  varn = 0;
    while (strcmp(lex,"nil")) {
      tt = refstring2term(s,lex);
      rt->pusht(*tt); varn++;
      lex = get_symbol(s); // '.'
      lex = get_symbol(s); }
    lex = get_symbol(s); // ','
    lex = get_symbol(s); // fsymi
    rt->crterm_reverse(atoi(lex),varn);
    lex = get_symbol(s); // ')'
  } else if (!strcmp(lex,"INT")) {
    lex = get_symbol(s); // '('
    lex = get_symbol(s);
    rt->crstterm(atoi(lex),TNUMBER); 
    lex = get_symbol(s); // ')' 
  } else if (!strcmp(lex,"TVAR")) {
    lex = get_symbol(s); // '('
    lex = get_symbol(s); varn = atoi(lex);
    lex = get_symbol(s); // ','
    lex = get_symbol(s); 
    typel.crtypelex(atoi(lex));
    rt->crvar(varn,typel); 
    lex = get_symbol(s); // ')'
  } else if (!strcmp(lex,"IDENT")) {
    lex = get_symbol(s); // '('
    lex = get_symbol(s);
    rt->crstterm(atoi(lex),TIDENT); 
    lex = get_symbol(s); // ')'
 }  else { 
    sterr << "\n[fatal] refstring2term - error in refstring\n";
    failexit(); }
  rt->popt(); 
  rt->incrcount();

  //*** stout << "refstring2term =";  rt->write(stout); stout << "\n";

  return rt;
}

// "FSYM(INT(5).FSYM(nil,'a').nil,'f')" -> f(4,a)
term *refstring2term(char *s) 
{
  return refstring2term(s,NULL);
}

void term::term2refstring(char *buff)
{ 
  int i;
  /*lexem *p; */
  char *ch;
  char  tempbuff[STRLEN];

  if (strlen(buff) > REFSTRINGLEN - 500) {
    sterr << "\n[fatal] term2refstring - string too long\n";
    failexit(); }
  switch (t->infos) {
  case TVAR:	
    sprintf(tempbuff,"VAR(%d,%d)",t->fsymi,t->compif.varsort.typeval());
    strcat(buff,tempbuff);
    break;
  case TNUMBER:
    sprintf(tempbuff,"INT(%d)",(int) t->fsymi);
    strcat(buff,tempbuff);
    break;
  case TSTRING: 
    ch = (char*)(t->subt);
    strcat(buff,"STRING(");
    for(i=0; ch[i]; i++) {
      sprintf(tempbuff,"%d.",(int)(ch[i]));
      strcat(buff,tempbuff); }
    strcat(buff,"nil");
    break;
  case TIDENT: 
    sprintf(tempbuff,"IDENT(%s)",tabofident.ide(t->fsymi));
    strcat(buff,tempbuff);
    break;
  case TNORMFS: 
    strcat(buff,"FSYM(");
    for(i=0; i < headarity(); i++) {
      t->subt[i].term2refstring(buff); 
      strcat(buff,".");}
    sprintf(tempbuff,"nil, %d)",t->fsymi); 
    strcat(buff,tempbuff); 
    break;
  default :     interr();
  }
}

// f(4,a) -> "FSYM(INT(5).FSYM(nil,'a').nil,'f')"
char *term::term2refstring()  // exporting
{
char *buff, *s;
  AALLOSS(buff,REFSTRINGLEN,char); buff[0] = 0;
  term2refstring(buff);
  s = strdup(buff);
  DELETE2(buff);
  return s;
}

#define  MAKE_REF_FILE \
if (spc) {  \
    sprintf(buff,"elan -b --export %s %s %s",aux_file_name,prog,spc); \
    system(buff); } \
  else if (strlen(prog) < STRLEN) \
    strcpy(aux_file_name,prog); \
  else { \
    tmp_file = fopen(aux_file_name,"w"); \
    if (tmp_file == NULL) { \
      sterr << "\n[META-REDUCE] can not open temporary file " << aux_file_name << "\n"; sterr.flush(); } \
    fprintf(tmp_file,"%s\n",prog); \
    fclose(tmp_file); \
  } \
  tmp_file = fopen(aux_file_name,"a"); \
  if (tmp_file == NULL) { \
    sterr << "\n[META-REDUCE] can not open temporary file " << aux_file_name << "\n"; sterr.flush(); } \
  fprintf(tmp_file,"\n%d\n",stratsort); \
  fprintf(tmp_file,"\n%s end\n",str); \
  fprintf(tmp_file,"\n%s end\n",t); \
  fclose(tmp_file);


int meta_reduce_bagof(int cons_fsym, int nil_fsym,
		       char *str, char *t, int stratsort, char *prog, char *spc,
		       int from, int n, term *res)
{
  int det, all = 0;
  term rt,  beg_list, *list_rt2; /* tt,  *list_rt, *list_rt1,*/
  char RESULT[RESULTLEN];
  struct processdata *pd;
  lexem le, ENDlex, NOMORElex;
  RESULT[0] = 0; ENDlex.cridlex("end"); NOMORElex.cridlex("no_more");
  char buff[STRLEN];
  FILE *tmp_file;
  char aux_file_name[STRLEN] = "Tmp.ref";

  if (n == 0) all=1; else n+=from;

  //*** stout << "META-REDUCE_BAGOF <<" << str << ", " << t   << ", "  << from << "," <<  n << "," << nil_fsym << "," << cons_fsym << ")\n";
  
  list_rt2 = &beg_list;

  MAKE_REF_FILE
  pd = newprocess("elan","--no_more","-b","--reduce",aux_file_name,NULL,NULL,NULL,NULL,9999,0); 

  if (pd == NULL) {
    if (!batch) { sterr << "\n[META-REDUCE] cannot run another ELAN\n"; sterr.flush(); }
    return(0); }
  /*
  *pd->pin << t << " end \n\n";
  pd->pin->flush();
  */
 (*pd->s).ilex(le);  // '#'
 (*pd->s).ilex(le);  // '#'

  while ((n > 0 || all)) {
    RESULT[0] = 0; 
    for(;;) {
      (*pd->s).ilex(le);  // stout << le.alfsy();
      if (le == NOMORElex) { det = 0; break; }
      if  (le == ENDlex) { det = 1; break; }
      if (!from) {
	strcat(RESULT, le.alfsy());
	if (strlen(RESULT) > RESULTLEN - 100) {
	  sterr << "\n[META-REDUCE] result too long\n"; sterr.flush(); } }  }
    n--;
    if (from > 0) { from--; continue; }

    if (det == 0) break;
    list_rt2->crterm(nil_fsym,0);  list_rt2->incrcount();// n'importe quoi

    rt.stinit();
    rt.crststring(strdup(RESULT));
    rt.incrcount();

    list_rt2->pusht(rt);

    list_rt2->crterm(cons_fsym,2); list_rt2->incrcount();
    list_rt2 = list_rt2->subterm(1);   }
  list_rt2->crterm(nil_fsym,0); list_rt2->incrcount();
  *res = beg_list;

  //*** stout << "META-REDUCE-BAGOF >>"; res->write(stout); stout << "\n";

  // delete file
  //system("/bin/rm -f Tmp.ref ");     

  return 0;
}


int meta_reduce(char *str, char *t, 
                int stratsort,
                char *prog, char *spc, 
		int from, term *res)
{
  struct processdata *pd;
  /*int det, all = 0;*/
  lexem le, ENDlex, NOMORElex;
  char RESULT[RESULTLEN];
  char buff[STRLEN];
  FILE *tmp_file;
  char aux_file_name[STRLEN] = "Tmp.ref";

 
  RESULT[0] = 0; ENDlex.cridlex("end"); NOMORElex.cridlex("no_more");

  if (from < 0) return 0;

  //***stout << "META-REDUCE <<" << str << ", " << t   << ", "  << from << prog << ")\n"; }

  MAKE_REF_FILE
  pd = newprocess("elan","--no_more","-b","--reduce",aux_file_name,NULL,NULL,NULL,NULL,9999,0); 
  if (pd == NULL) {
    if (!batch) { sterr << "\n[META-REDUCE] cannot run another ELAN\n"; sterr.flush(); }
    return(0); }

 (*pd->s).ilex(le);  // '#'
 (*pd->s).ilex(le);  // '#'

  do {
    for(;;) {
      (*pd->s).ilex(le);  
      if (le == NOMORElex) return 0;
      if  (le == ENDlex) break;
      if (!from) {
	strcat(RESULT, le.alfsy());
	if (strlen(RESULT) > RESULTLEN - 100) {
	  sterr << "\n[META-REDUCE] result too long\n"; sterr.flush(); } }
    }
    from--;
  } while (from >= 0);

  res->crststring(strdup(RESULT));
  res->incrcount();

  // delete file
  //system("/bin/rm -f Tmp.ref ");     

  //*** stout << "META-REDUCE >>"; res->write(stout); stout << "\n";

  return -1;
}

int new_meta_apply(int typ,int cons_fsym, int nil_fsym,
		   term *str, term *t, int from, int n, term *res)
{
  int det, all = 0;
  strategy *s;
  /* list_rt2 initialised to avoid warning */
  term rt, tt,  *list_rt2 = NULL, beg_list;/* *list_rt, *list_rt1,  */

  if (n == 0) all=1;
  else n+=from;

   //stout << "NEW_META-APPLY("; str->write(stout); stout << ", ";
   //t->write(stout); stout << ", " << from << ")\n";
   //stout << ", " << n << ")\n";

  term2strategy(typ,str,&s);

  //s->dump(); stout << "\n";
  
  tt = *t;
//  reduce(tt,trace);
  {
  stateofexecution execst(tt,s);
  
  list_rt2 = &beg_list;

  // execst.dump(0);

  while ((n > 0 || all) && (det = execst.nextsolution(rt))) {
    n--;
    if (from > 0) { from--; continue; }
    list_rt2->crterm(nil_fsym,0);  list_rt2->incrcount();// n'importe quoi

    list_rt2->pusht(rt);

    list_rt2->crterm(cons_fsym,2); list_rt2->incrcount();
    list_rt2 = list_rt2->subterm(1); 
  }

  list_rt2->crterm(nil_fsym,0); list_rt2->incrcount();
  execst.free();

  //stout << "NEW_META-APPLY_RESULT\n"; beg_list.write(stout); 
  // beg_list.dump();  stout << "\n";
  }
  *res = beg_list;

  s->Delete();

  return 0;
}


int meta_apply(int typ,term *str, term *t, int n, term *res)
{
  term rt, tt;
  strategy *s;
  int det=0;/* initialised to avoid warning */
  //stout << "META-APPLY("; str->write(stout); stout << ", ";
  //t->write(stout); stout << ", " << n << ")\n";
  term2strategy(typ,str,&s);
  //s->dump();
  tt = *t;
//  reduce(tt,trace);
  stateofexecution execst(tt,s);
  while (n >= 0 && (det = execst.nextsolution(rt))) {
    if (n != 0) 
      rt.tdelete();
    n--;
  }
  execst.free();
  if (n < 0) {
    *res = rt; return det; }
  else return 0;
}

// should allocate s
void term2strategy(int typ,term *t, strategy **s)
{
  strategy *ss;
  NNEW(*s, strategy);
  switch (t->semantic()) {
    case One_Strateg:
      term2strateg(typ,t->subterm(0),*s);
      (*s)->setnext(NULL);
      break;  
    case More_Strateg:
      term2strateg(typ,t->subterm(0),*s);
      term2strategy(typ,t->subterm(1),&ss);
      (*s)->setnext(ss);
      break;
    default:
      sterr << "term2strategy internal error \n"; failexit();
  }
}


int Strategyname_to_ref_index(char *strname, int typ)
{
    char   *new_ss, *ss, *name, *type, *modul;
    int iref, good_type;
    // BAD SOL     s->setprocmaxn(trrules.searchmatch_defs(strname,typ,-1));

    //ss = trrules.strategyname_defs(trrules.searchmatch_defs(strname,typ,-1));
      //[pem: Apr 13 03] 'typ' cannot be used because its code may be wrong
    ss = trrules.strategyname_defs(trrules.searchmatch_defs(strname,-1,-1));
 
    detach_name_type_module(ss, &name, &type, &modul);

      //stout << "type = " << type << "\n";
    good_type = typet.index(type);
    new_ss =  attach_type_mod(strname,good_type,import.addstr(modul));
      //stout << "new_ss = " << new_ss << "\n";
   
    iref = trrules.strategyindex_refs(new_ss);
    return iref;
}


// 's' should be preallocated
void term2strateg(int typ,term *t, strategy *s)
{
  strategy *ss;
  struct strlist *stl;
  struct namelist *nml;
  /*int    stri;*/
  /*char   *strname;*/
  switch (t->semantic()) {
    case DC_Labels:
    case DK_Labels:
      s->setname(((t->semantic()==DC_Labels)?STRNAMEDONTCARE:STRNAMEDONTKNOW),-1);
      term2labels(typ,t->subterm(0),&(nml));
      s->setnamelist(nml);
      break;
    case DC_Strategies:
    case DK_Strategies:
      s->setname(((t->semantic()==DC_Strategies)?STRNAMEDONTCARE2:STRNAMEDONTKNOW2),-1);
      term2strategies(typ,t->subterm(0),&stl);
      s->setstl(stl);
      break;
    case Repeat_Strategy:
    case Iterate_Strategy:
      s->setname(
         ((t->semantic()==Repeat_Strategy)?STRNAMEREPEAT:STRNAMEITERATE),-1);
      term2strategy(typ,t->subterm(0),&ss);
      s->setsubst(ss);
      break;
    case Identity:
      s->setname(STRIDENTITY,-1);
      break;
  case Call: {
	int iref;
	s->setname(STRCALL,-1);
        
        iref = Strategyname_to_ref_index(t->subterm(0)->getstring(),typ);
        
          //iref = Strategyname_to_ref_index(t->subterm(0)->subterm(0)->getstring(),typ);
	s->setprocmaxn(iref);
	trrules.strategy_refs_into_defs(iref);
      break;
  }
   default:     
      sterr << "term2strateg internal error \n"; failexit();
  }
}

// should allocate s
void term2strategies(int typ,term *t, struct strlist **s)
{
  NNEW(*s,struct strlist);
  switch (t->semantic()) {
    case One_Strategy:
      term2strategy(typ,t->subterm(0),&((*s)->str));
      (*s)->next = NULL;
      break;
    case More_Strategy:
      term2strategy(typ,t->subterm(0),&((*s)->str));
      term2strategies(typ,t->subterm(1),&((*s)->next));
      break;
    default:
      sterr << "term2strategies internal error \n"; failexit();
  }
}
// should allocate s
void term2labels(int typ,term *t, struct namelist **s)
{
  char *rulename;
  NNEW(*s,struct namelist);
  switch (t->semantic()) {
    case One_Rule:
    case More_Rules:

      rulename = t->subterm(0)->getstring(); // with string

//with Label      rulename = fsymtab[t->subterm(0)->head()].textform()->rside[0].alfsy();
      (*s)->strname = trrules.trruleindex(rulename);
      if (t->semantic() == One_Rule) 
        (*s)->next = NULL;
      else
        term2labels(typ,t->subterm(1),&((*s)->next));
      break;
    default:
      sterr << "term2labels internal error \n"; failexit();
  }
}


void SSambiguity1(int warn, char *name, char *type, char *modu)
{
  if (!batch) { 
    sterr << "\nthere is an strategy/strategy ambiguity because of a reference " << name 
	  << " for " << type << " in module " << modu << "\n"; }
}

// typ may be =-1 and also may be modu = -1
int trsystem::searchmatch_defs(char *name, int typ, int modu) {
  char *strategyName, *rr;
  int indx=0, count = 0;
  if (typ == -1) {
    strategyName = strdup(name);
  } else if (modu == -1) {
    strategyName = attach_type(name,typ);
  } else {
    strategyName = attach_type_mod(name,typ,modu);
  }

  for(strategynames_defs->forinit();
      strategynames_defs->forcond();
      strategynames_defs->fornext()) {
    rr = strategynames_defs->foractval();
   
    if (!(strncmp(strategyName,rr,strlen(strategyName)))) {
        //stout << "strategyName = " << strategyName << " rr = " << rr << " len=" << strlen(strategyName) << "\n";
      count ++;
      indx = strategynames_defs->forindex();
    }
  }
  CFRE(strategyName);
  if (count > 1) {
    SSambiguity1(1,name,
                 ((typ == -1) ? (char*)"???" : typet.ide(typ)),
                 ((modu == -1) ? (char*)"???" : import.ide(modu)));
  } else if (count == 0) {
    Sundefined(1,name, 
               ((typ == -1) ? (char*)"???" : typet.ide(typ)),
               ((modu == -1) ? (char*)"???" : import.ide(modu)));
  }
  return indx;
}




