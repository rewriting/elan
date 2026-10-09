extern struct term *query;

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

int initElanLib(long *bp) {
  int i,j;
  choice_init(bp);
  GC_free_space_divisor=2;

  symbol_init();
  EkerTermInit();
  initTabRef();
  EarleyParserInit();
  backTrackInit();

  for(i=0 ; i<2 ; i++) {
    for(j=0 ; j<FSYM_TAB_SIZE ; j++) {
      tab_rewrite_step[i][j] = 0;
    }
  }
}

int getStrategyIndex(char *strategyName) {
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

struct term *makeConstructor(int code) {
  struct term *t;
  if(arity[code] == 0) {
    if(code == 0) {
      return con_0;
    } else if(code == 1) {
      return con_1;
    } else {
      TERM_CONST_ALLOC(t,code);
      return t;
    }
  } else {
    if(symb_isAC(code)) {
      TERMAC_ALLOC(t,arity[code],code);
      return t;
    } else {
      TERM_ARITY_ALLOC(t,arity[code],code);
      return t;
    }
  }
}


