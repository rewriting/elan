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
#include "termATerm.h"
#include "builtin.h"

//temporaire
//extern struct Gfsym  fsymtab[];
//int MAXPOS;


int myatoi(char* s) {
    //return 100*(*s) + 10*(*(s+1)) + (*(s+2)) - (111*'0') ;
  
  int n = 0;
  while(*s) { n = 10*n + (*s++) - '0'; }
    //for( ; *s ; s++) { n = 10*n + (*s - '0'); }
  return n;
}

#define notImplemented() printf("Not yet implemented\n"); assert(0);

#define max 100


void Gterm_init(int argc,char **argv, long *ptr_bottomOfStack) {
  ATinit(argc,argv,(ATerm*)ptr_bottomOfStack);
}

void Gfsym_init(int code, int a, char *n, char *s,
               int sem, int dstrat,
 	       Gterm* (*semaction)(Gterm *)) {
  AFun afun;
  char name[max];
    //unsigned char name[3];

  fsymtab[code].arity=a;
  fsymtab[code].name=n;
  fsymtab[code].sort=s;
  fsymtab[code].semantic=sem;
  fsymtab[code].modulo=0;
  fsymtab[code].defstrat=dstrat;
  fsymtab[code].semact=semaction;
    //sprintf(name,"%03d",code);
  sprintf(name,"%d",code);
    //name[0] = (unsigned char) 1+((code & 0x0000FF00) >>8);
    //name[1] = (unsigned char) (code & 0x000000FF);
    //name[2] = '\0';
  
    //printf("code = %d\t",code);
    //printf("name[0] = %d\t",name[0]);
    //printf("name[1] = %d\t",name[1]);
    //printf("code = %d\n",getCode(name));
    //printf("name = '%s'\n",name);
  
  afun = ATmakeAFun(name,a,ATfalse);

    //printf("afun = %d\tcode = %d\n",afun,code);
  ATprotectAFun(afun);
  fsymtab[code].afun=afun;

     //[QUANG: Sep 19 01] 
  fsymtab[code].prec=code;  //[QUANG: Sep 19 01] initially precedence = code
}

int intern_GgetSymb(Gterm *v1) {
  AFun afun;
  char *name;
  int code;
  
  afun = ATgetAFun((ATermAppl)v1);
  name = ATgetName(afun);
  code = getCode(name);
    //ATprintf("\n v1 = %t",v1);
    //ATprintf("s = '%s'\n",ATgetName(ATgetAFun((ATermAppl)v1)));
  return code;
}

void intern_Gmake_const(Gterm **ptr_dest,int code) {
  AFun afun;
  afun = fsymtab[code].afun;
  *ptr_dest=(Gterm*)ATmakeAppl0(afun);

    //printf("name = '%s'\tcode = %d\tgetCode =%d\n",ATgetName(afun), code, getCode(ATgetName(afun)));
  
  if(code != getCode(ATgetName(afun))) {
    printf("strange appl\n");
  }

    //printf("Gmake_const: ATprotect(%x)\n", ptr_dest);
  ATprotect(ptr_dest);
}


void intern_GmakeAppl0(Gterm **ptr_dest,int code) {
  AFun afun;
  afun = fsymtab[code].afun;
  if(code != getCode(ATgetName(afun))) {
    printf("strange appl\n");
  }
  *ptr_dest=(Gterm *)ATmakeAppl0(afun);
}

void intern_GmakeAppl1(Gterm **ptr_dest,int code,Gterm *subterm) {
  AFun afun;
  afun = fsymtab[code].afun;
  if(code != getCode(ATgetName(afun))) {
    printf("strange appl\n");
  }
    //printf("name(afun) = '%s'\n", ATgetName(afun));
  *ptr_dest=(Gterm *)ATmakeAppl1(afun,subterm);
    //printf("symb(*ptr_dest) = %d\n", GgetSymb(*ptr_dest));
}

void intern_GmakeAppl2(Gterm **ptr_dest,int code, Gterm *subterm0, Gterm *subterm1) {
  AFun afun;
    //char s_code[256];
    //sprintf(s_code,"%d",code);
  afun = fsymtab[code].afun;

  if(code != getCode(ATgetName(afun))) {
    printf("strange appl\n");
  }
  
    //printf("name(afun) = '%s'\n", ATgetName(afun));
  *ptr_dest=(Gterm *)ATmakeAppl2(afun,subterm0,subterm1);

    //*ptr_dest=(Gterm *)ATmakeAppl2(fsymtab[code].afun,subterm0,subterm1);
}

void intern_GmakeAppl3(Gterm **ptr_dest,int code,Gterm *subterm0
                ,Gterm *subterm1,Gterm *subterm2) {
  AFun afun;
  afun = fsymtab[code].afun;
  if(code != getCode(ATgetName(afun))) {
    printf("strange appl\n");
  }
  *ptr_dest=(Gterm *)ATmakeAppl3(afun,subterm0,subterm1,subterm2);
}

void intern_GmakeAppl4(Gterm **ptr_dest,int code,Gterm *subterm0
                ,Gterm *subterm1,Gterm *subterm2
                ,Gterm *subterm3) {
  AFun afun;
  afun = fsymtab[code].afun;
  if(code != getCode(ATgetName(afun))) {
    printf("strange appl\n");
  }
  *ptr_dest=(Gterm *)ATmakeAppl4(afun,subterm0,subterm1,subterm2,subterm3);
}

void intern_GmakeAppl5(Gterm **ptr_dest,int code,Gterm *subterm0
                ,Gterm *subterm1,Gterm *subterm2
                ,Gterm *subterm3,Gterm *subterm4) {
  AFun afun;
  afun = fsymtab[code].afun;
  if(code != getCode(ATgetName(afun))) {
    printf("strange appl\n");
  }
  *ptr_dest=(Gterm *)ATmakeAppl5(afun,subterm0,subterm1,subterm2,subterm3,subterm4);
}

void intern_GmakeAppl6(Gterm **ptr_dest,int code,Gterm *subterm0
                       ,Gterm *subterm1,Gterm *subterm2
                       ,Gterm *subterm3,Gterm *subterm4
                       ,Gterm *subterm5) {
  AFun afun;
  afun = fsymtab[code].afun;
  if(code != getCode(ATgetName(afun))) {
    printf("strange appl\n");
  }
  *ptr_dest=(Gterm *)ATmakeAppl6(afun,subterm0,subterm1,subterm2,subterm3,subterm4,subterm5);
}

void GmakeAppl(Gterm **ptr_dest,int code,int arity,...) {
  int i;//, arity = code_arity(code);
  Gterm *ArrayArgs[256];
  AFun afun;
  va_list args;
  va_start(args, arity);
  for(i=0; i<arity; i++) {
    ArrayArgs[i] = va_arg(args, Gterm *);
  }
  va_end(args);
  afun = fsymtab[code].afun;
  if(code != getCode(ATgetName(afun))) {
    printf("strange appl\n");
  }
  *ptr_dest=(Gterm*)ATmakeApplArray(afun,ArrayArgs);
}

void intern_GmakeAppl_Array(Gterm **ptr_dest,int code,Gterm *ArrayArgs[]){
  AFun afun;
  afun = fsymtab[code].afun;
  if(code != getCode(ATgetName(afun))) {
    printf("strange appl\n");
  }
  *ptr_dest=(Gterm*)ATmakeApplArray(afun,ArrayArgs);
}




int Gterm_cmp(register Gterm *t1, register Gterm *t2) {

  return(t1==t2);
}

void intern_GsetArgument(Gterm **ptr_dest,int pos, Gterm *t) {
  *ptr_dest = ((Gterm *)ATsetArgument((ATermAppl)*ptr_dest,t,pos));
}

Gterm *intern_GgetArgument(Gterm *v,int p) {
  return ATgetArgument(v,p) ;
}

void intern_GmakeAppl_Arity(Gterm **ptr_dest,int arity,int code) {
  AFun afun;
  Gterm *args[256];
  int i;
  afun = fsymtab[code].afun;
  for(i=0; i<arity ; i++) {
      //args[i] = (Gterm *)ATparse("null");
    args[i] = ATmakeAppl0(AFUN_EMPTY_SUBTERM);
      //args[i] = (Gterm *) ATtrue;
      //printf("arg = %x\n",args[i]);
  }
  *ptr_dest=(Gterm *)ATmakeApplArray(afun,args);
}

void intern_GsetSymb(Gterm **ptr_dest,unsigned int code) {
  GmakeApplArity(*ptr_dest,code_arity(code),code);
}

void intern_GgetArguments_tab(Gterm ***ptr_arg[],Gterm* res) {
  int i,arity; 

  arity = term_arity(res);//fsymtab[GgetSymb(res)].arity;// 
  printf("\n arity = %d",arity); 
 // ATprintf("\n res=%t",res); 
  *ptr_arg = malloc(sizeof(arity * sizeof(Gterm *))) ; 
  for(i=0;i<arity;i++) { 
   //ATprintf("\nptr_arg[%d]= %t ",i,ATgetArgument((ATermAppl)res,i));  
   ptr_arg[i]=ATgetArgument((ATermAppl)res,i); 
  } 
}


/*HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
HHHHHHHHHHHHHHHHHHHHHHHHH DUPLICATION HHHHHHHHHHHHHHHHHHHHHHH
HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH*/

GtermList *GlistTermCreate(Gterm *term) {
  return ATmakeList1(term);
}

GtermList *GaddTermListTerm(GtermList *list , Gterm *term) {
  return ATappend(list,term);
}

Gterm *GlistGetHead(GtermList *list) {
  return ATgetFirst(list);
}

GtermList *GlistGetTail(GtermList *list) {
  return ATgetNext(list);
}

int GlistIsEmpty(GtermList *list) {
  return ATisEmpty(list);
}
/*
//int Ginitialise_trace() {
//  notImplemented();
//    struct NODE *node;

//    head_tr = NULL;
//    tail_tr = NULL;
//    if((node=(struct NODE*)MALL
//	node->father = NULL;
//	node->next =  NULL;
//	node->son = NULL;
//	node->ps = 2;
//	racine = node;
//    }
//    else
//	return 1;

//    return 0;
//}
//Gterm *GcoqEarleyParser(char *querySortName) {
//  notImplemented();
/*    int e ;
    struct term *query;
    FILE *fp_query = NULL;

    earleyInit() ;
    if (*querySortName != 0) {
	if ((earleyQuerySort = findTab(tabSort,TABOFSORT_SIZE,querySortName))==-1) return NULL;
    }

    if ((fp_query = fopen(query_file,"r"))!= NULL){
	e = earleyCall(fp_query,earleyQuerySort) ;
	if (e) {
	    query = computeTerm(&earleyPos) ;
	    printf("\n") ;
	    fflush(stdout) ;
	    fclose(fp_query);
	} else {
	    fclose(fp_query);
	    return NULL;
	}
    }
    // [Huy: Apr 18 00]
    else{
	e = earleyCall(NULL,earleyQuerySort) ;
	if (e) {
	    query = computeTerm(&earleyPos) ;
	    printf("\n") ;
	    fflush(stdout) ;
	} else{
	    return NULL;
	}
    }
    return(query) ;
//}

void GsetTermNoReduced(Gterm *t){
  notImplemented();
    int code, arity, i;

    if(isIntegerTagged(t) || isIdentifierTag
	return;
    }


  arity = term_arity(t);
  for(i=0 ; i<arity ; i++) {
    setTermNoReduced(GgetArgument(t,i));
  }
  GclearReduced(t);
}
int GcoqprefixParser(char *s1, char *s2) {
  notImplemented();
    char c;
    FILE *fp_query_sort ;  //ce qui peut remplacer le stdin

    if (!coqMode) {
	fprintf(stdout,"Enter a query of the form: ([[<strat>:]<sort>]) <term> end\n");
	fflush(stdout);
}

    s1[0]='\0';
    s2[0]='\0';
    if ((fp_query_sort=fopen(query_sort_file,"r")) == NULL){
	fp_query_sort = stdin;
    }
    if(fscanf(fp_query_sort," ( %c",&c) != 1) {
	printf("'(' expected\n");
	return 1;
    }
    if(c==')') {
	// () ;
    } else {
	ungetc(c,fp_query_sort); //stdin
	if(fscanf(fp_query_sort," %[^ :)] %c",s1,&c) != 2) {
	    printf("identifier expected\n");
	    return 1;
	}

	if(c==')') {
	    // ( id ) ;
	} else {
	    if(c!=':') {
		printf("':' expected\n",c);
		return 1;
	    }
	    if(fscanf(fp_query_sort," %[^ )] %c",s2,&c) != 2) {
		printf("identifier expected\n");
	    }
	    // ( id : id ) //
	}
    }
    if (fp_query_sort != stdin) fclose(fp_query_sort);
    return 0;

}

//typedef char RNAME[64];
/*typedef struct TR_COQ{
 RNAME rname;
 struct NODE *rnode;
 struct TR_COQ *next;
 struct COND_NODE *cond_node; //pointeur vers la trace d'une condition
} TR_COQ;



int Gtrace_pretty_print(TR_COQ * head,int deep, FILE * fp_trace) {
  notImplemented();
/*    TR_COQ *tr;
    NODE *node;
    char tmp[5],rp[2];
    char output[1000],output1[1000];
    int i,ret,j;

    if (coqMode == 0) return 2;
    i = 0;
    tr = head;


    while (tr != NULL)
	{
	    memset(output,'\0',sizeof(output));
	    node = tr->rnode;
	    while (node != racine)
		{
		    i = i+1;
		    if (node->father == racine)
			{
			    sprintf(tmp,"%d",node->ps);
			}
		    else
			{
			    sprintf(tmp,";%d",node->ps);
			}
		    sprintf(output1,"%s%s",tmp,output);
		    strcpy(output,output1);
		    node = node->father;
		}
	    for (j=1; j<=deep; j++) fprintf(fp_trace," ");
	    fprintf(fp_trace, "%s @ [%s]\n",tr->rname,output);
	    if (tr->cond_node->leftside != NULL)
		trace_pretty_print(tr->cond_node->leftside,deep+2,fp_trace);
	    if (tr->cond_node->rightside != NULL)
		trace_pretty_print(tr->cond_node->rightside,deep+2,fp_trace);
	    tr = tr->next;
	}
    //write(nsock,"END",3);
    //printf("\n# position:%d\n",i);

    return 0;
}*/

int hashTerm(Gterm *t) {
    //return (int)t;
  return (int)t & 0x0000FFFF;
}

/*
 * Array
 */

Gterm *term_newArray(int size, Gterm *t) {
  Gterm **array;
  Gterm *res;
  ATermBlob blob;
  ATermInt atInt;
  int i;
  
  array = (Gterm**) malloc(size * sizeof(Gterm*));
    //printf("NEW: array = %x\n",array);
 
  atInt = ATmakeInt(size);
  for(i=0 ; i<size ; i++) {
    array[i] = t;
  }

    //printf("ATprotectArray(%x,%d)\n", array,size);
  ATprotectArray(array,size);
    //ATprotectMemory(array,size * sizeof(Gterm*));
  
  blob = ATmakeBlob(size * sizeof(Gterm*), array);
  
    //GmakeAppl2(res,CODE_ARRAY,atInt,blob);
  res = (Gterm*)ATmakeAppl2(AFUN_ARRAY,(Gterm*)atInt,(Gterm*)blob);
  
    //printf("NEW: res= %d\n",res);
  return res;
}

Gterm *term_getArray(Gterm *t, int n) {
  Gterm **array;
  int size;
  ATermBlob blob;
  ATermInt atInt;
  Gterm *res;
  
  if(GgetSymb(t) != CODE_ARRAY) {
    printf("getArray error: symb = %d\n",GgetSymb(t));
    exit(1);
  }

  atInt = (ATermInt)GgetArgument(t,0);
  size  = (int) ATgetInt(atInt);
  blob  = (ATermBlob)GgetArgument(t,1);
  array = ATgetBlobData(blob);

    //printf("GET: array = %d\tblob = %d\n",array,blob);
    //printf("n = %d\tsize = %d\n",n,size);
  if(n<0 || n>=size) {
    printf("getArray error: size = %d\tn = %d\n",size,n);
    exit(1);
  }

    //res = array[n];
    //printf("res = %d\n",res);
  
    //printf("array[%d] = ",n); term_println(stdout,res);
  
  return array[n];
}

Gterm *term_setArray(Gterm *t, int n, Gterm *subterm) {
  Gterm **array;
  int size;
  Gterm *res;
  
  ATermBlob blob;
  ATermInt atInt;
  if(GgetSymb(t) != CODE_ARRAY) {
    printf("setArray error: symb = %d\n",GgetSymb(t));
    exit(1);
  }
  atInt = (ATermInt)GgetArgument(t,0);
  size  = (int) ATgetInt(atInt);
  blob  = (ATermBlob)GgetArgument(t,1);
  array = ATgetBlobData(blob);
  if(n<0 || n>=size) {
    printf("setArray error: size = %d\tn = %d\n",size,n);
    exit(1);
  }
  array[n] = subterm;
  blob = ATmakeBlob(size * sizeof(Gterm*), array);
    //GmakeAppl2(res,CODE_ARRAY,atInt,blob);
  res = (Gterm*)ATmakeAppl2(AFUN_ARRAY,(Gterm*)atInt,(Gterm*)blob);
    //printf("array[%d] <- ",n); term_println(stdout,array[n]);
    //printf("SET: res= %d\n",res);
  return res;
}

int term_getLength(Gterm *t) {
  ATermInt atInt;

  AFun afun = ATgetAFun(t);
    //printf("afun = %d\tcode = %d\n",afun,tab_bijection[afun]);
  
  if(GgetSymb(t) != CODE_ARRAY) {
    printf("getLength error: symb = %d\n",GgetSymb(t));
    exit(1);
  }
  atInt = (ATermInt)GgetArgument(t,0);

    //printf("length = %d\n",ATgetInt(atInt));
  
  return (int) ATgetInt(atInt);
}

Gterm *term_newString(char *string) {
  ATermInt atInt;
  ATermAppl atString;
  Gterm *res;
  
  atInt    = ATmakeInt(strlen(string));
  atString = ATmakeAppl0(ATmakeAFun(string,0,ATfalse));
  res = (Gterm*)ATmakeAppl2(AFUN_STRING, (Gterm*)atInt, (Gterm*)atString);
  return res;
}

char *term_getString(Gterm *t) {
    //ATermInt atInt;
  ATermAppl atString;
  char *res;
  
  if(GgetSymb(t) != CODE_STRING) {
    printf("getString error: symb = %d\n",GgetSymb(t));
    exit(1);
  }
    //atInt    = (ATermInt)GgetArgument(t,0);
  atString = (ATermAppl)GgetArgument(t,1);
  res = ATgetName(ATgetAFun(atString));
  return res;
}







void tab_bijection_init() {
  int i,code_max;
  
  code_max = 0;
  for(i=0; i<fsymtabSize ; i++) {
    if(fsymtab[i].afun > code_max)
      code_max = fsymtab[i].afun;
  }
    //printf("code_max = %d\n",code_max);
  tab_bijection = malloc((code_max+1)*sizeof(int));
  for(i=0; i<fsymtabSize ; i++) {
      //printf("code = %d	afun = %d\n",i,fsymtab[i].afun);
    tab_bijection[fsymtab[i].afun]=i;
  }

    //printf("code = %d	afun = %d\n",204,fsymtab[204].afun);
    //printf("code = %d	afun = %d\n",204,tab_bijection[221]);
}
