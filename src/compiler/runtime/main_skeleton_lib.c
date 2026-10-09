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
extern Gterm *query;

/* Constantes d'execution */
unsigned long rewrite_step=0;
unsigned long rewrite_label_step=0;
unsigned long rewrite_real_step=0;

int global_indentlevel=0;
int debugMode=0;
int quietMode=0;
int resultMode=ELAN_IO;
char *sortName=NULL;
char *strategyName=NULL;
int traceLevel=0;
int coqMode=0;
int printMode=0;
int testAC=0;  //[NGUYEN: Sep 10 01] 
FILE *file_query=NULL; //NGUYEN

#ifdef ATERM
void *at_malloc_protect(int size) {
  void *start = malloc(size);
  ATprotectMemory(start,size);
  return start;
}

void *at_realloc(char *old,int size) {
  void *start = realloc(old,size);
  if(!start) {
    printf("at_realloc: out of memory\n");
    exit(1);
  }
  return start;
}

void *at_realloc_protect(char *old,int size) {
  void *start = realloc(old,size);
  if(!start) {
    printf("at_realloc_protect: out of memory\n");
    exit(1);
  }
  ATunprotectMemory(old);
  ATprotectMemory(start,size);
  return start;
}
#endif


int initElanLib(long *bp) {
  int i,j;

    //Gterm_init(argc,argv,&bp);
  Gterm_init(NULL,NULL,&bp);
#ifdef ATERM
  CPL_init_malloc_protect(at_malloc_protect);
  CPL_init_malloc(malloc);
  CPL_init_realloc_protect(at_realloc_protect);
  CPL_init_realloc(at_realloc);
#else
  CPL_init_malloc_protect(GC_malloc);
  CPL_init_malloc(GC_malloc_atomic);
  CPL_init_realloc_protect(GC_realloc);
  CPL_init_realloc(GC_realloc);
#endif

  choice_init(bp);

  symbol_init();
  Ginit_builtin(); // should be done before tab_bijection_init
#ifdef ATERM
  tab_bijection_init();
#endif

  EkerTermInit();
  initTabRef();
  EarleyParserInit();
    //backTrackInit();

  for(i=0 ; i<2 ; i++) {
    for(j=0 ; j<FSYM_TAB_SIZE ; j++) {
      tab_rewrite_step[i][j] = 0;
    }
  }
}

int getStrategyIndex(char *strategyName) {
  int i,strIndex;

  for(i=0, strIndex=0 ; strIndex==0 && i<tabStrategySize ; i++) {
    if((strlen(tabStrategyStr[i])==strlen(strategyName)) &&
       !strcmp(strategyName,tabStrategyStr[i])) {
      strIndex = tabStrategyIndex[i];
    }
  }
  if(strIndex==0) {
    printf("Strategy '%s' not found\n",strategyName); 
    exit(1);
  }
  return strIndex;

}

int getPrefixStrategyIndex(char *strategyName) {
  int i,strIndex;

  for(i=0, strIndex=0 ; strIndex==0 && i<tabStrategySize ; i++) {
    if(!strncmp(strategyName,tabStrategyStr[i],strlen(strategyName))) {
      strIndex = tabStrategyIndex[i];
    }
  }
  if(strIndex==0) {
    printf("Strategy '%s' not found\n",strategyName); 
    exit(1);
  }
  return strIndex;
}

int getSymbolIndex(char *symbolName) {
  int i,symbolIndex;

  for(i=0, symbolIndex=0 ; symbolIndex==0 && i<FSYM_TAB_SIZE ; i++) {
    if((strlen(fsymtab[i].name)==strlen(symbolName)) &&
       !strcmp(symbolName,fsymtab[i].name)) {
      symbolIndex = i;
    }
  }
  if(symbolIndex==0) {
    printf("Symbol '%s' not found\n",symbolName); 
    exit(1);
  }
  return symbolIndex;
}

int getPrefixSymbolIndex(char *symbolName) {
  int i,symbolIndex;

  for(i=0, symbolIndex=0 ; symbolIndex==0 && i<FSYM_TAB_SIZE ; i++) {
    if(!strncmp(symbolName,fsymtab[i].name,strlen(symbolName))) {
      symbolIndex = i;
    }
  }
  if(symbolIndex==0) {
    printf("Symbol '%s' not found\n",symbolName); 
    exit(1);
  }
  return symbolIndex;
}

Gterm *makeConstructor(int code) {
  Gterm *t;
  if(arity[code] == 0) {
    if(code == 0) {
      return con_0;
    } else if(code == 1) {
      return con_1;
    } else {
      Gmake_const(t,code);
      return t;
    }
  } else {
    if(symb_isAC(code)) {
      TERMAC_ALLOC(t,arity[code],code);
      return t;
    } else {
      GmakeApplArity(t,arity[code],code);
      return t;
    }
  }
}


