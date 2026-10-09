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


/* Help */
printHelp() {
  printf("  Options:\n");
  printf("    -noInput\n");
  printf("    -REFInput\n");
  printf("    -commandLine\n");
  printf("    -noOutput\n");
  printf("    -REFOutput\n");
  printf("    -internalOutput\n");
  printf("    -quiet\n");
  printf("    -sort <sortName>\n");
  printf("    -strategy <strategyName>\n");
  printf("    -debug\n");
  printf("    -trace <number>\n");
  exit(1);
}
/* Procedure principale */
long *bp_main;
struct rusage before_self,after_self;
long diff_sec,diff_usec;
double total_time;
int main(int argc,char **argv) {
  long bp;
  struct term *res;
  int i,j;
  int queryMode=2;  /* 0:noInput  1:REFInput  2:Elan form 3:command line */
  int evaluationMode=0; /* 0:lgi 1:sort 2:strat:sort*/
  choice_init(&bp);
  bp_main=&bp;
  GC_free_space_divisor=2;
  for(i=1 ; i<argc && *argv[i]=='-' ; i++) {
    if(!strcmp(argv[i],"-noInput")) {
      queryMode=0;
    }
    if(!strcmp(argv[i],"-REFInput")) {
      queryMode=1;
    }
    if(!strcmp(argv[i],"-commandLine")) {
      queryMode=3;
    }
    if(!strcmp(argv[i],"-noOutput")) {
      resultMode=0;
    }
    if(!strcmp(argv[i],"-REFOutput")) {
      resultMode=1;
    }
    if(!strcmp(argv[i],"-internalOutput")) {
      resultMode=3;
    }
    if(!strcmp(argv[i],"-debug")) {
      debugMode=1;
    }
    if(!strcmp(argv[i],"-quiet")) {
      quietMode=1;
    }
    if(!strcmp(argv[i],"-sort=")) {
      evaluationMode=1;
      if(i>=argc-1) {
        printHelp();
      } else {
        sortName=argv[++i];
      }
    }
    if(!strcmp(argv[i],"-strategy=")) {
      evaluationMode=2;
      if(i>=argc-1) {
        printHelp();
      } else {
        strategyName=argv[++i];
      }
    }
    if(!strcmp(argv[i],"-trace")) {
      traceLevel=2;
      if(i>=argc-1) {
        printHelp();
      } else {
        traceLevel=atoi(argv[++i]);
      }
    }
    if(!strcmp(argv[i],"-coq")) {
      coqMode=1;
    }
    if(!strcmp(argv[i],"-printTrace")) {
      printMode=1;
    }

    if(!strcmp(argv[i],"-help") || !strcmp(argv[i],"-h")) {
      printHelp();
    } 

  }

  symbol_init();

  if(queryMode==1) {
    yyparse();
  }
  EkerTermInit();
  initTabRef();
  EarleyParserInit();
  
  getrusage(RUSAGE_SELF, &before_self);
  backTrackInit();

  if(coqMode) {
    lazy_annotation_read(strcat(argv[0],".lazy"));
  }
  
  for(i=0 ; i<2 ; i++) {
    for(j=0 ; j<FSYM_TAB_SIZE ; j++) {
      tab_rewrite_step[i][j] = 0;
    }
  }
  
  if (!setChoicePoint()) {
    /* Input */
    res=termParser(queryMode,evaluationMode);
    /* Output */
    switch(resultMode) {
      case 0:
        break;
      case 1:
        term_printREFln(stdout,res);
        break;
      case 2:
        printf("\nresult = ");
        termOut(stdout,term_unflatten(res));
        printf("\n");
        break;
      case 3:
        printf("\nresult = ");
        term_printnl(stdout,res);
        break;
    }
    if(resultMode!=1) {
      backStatistics();
      globalStatistics();
    }
    fail();
  }
end:
  getrusage(RUSAGE_SELF, &after_self);
#ifdef DEBUG
  backStatistics();
#endif
  if(resultMode!=1) {
    int printedHeader = 0;
    for(j=0 ; j<FSYM_TAB_SIZE ; j++) {
      if(tab_rewrite_step[0][j] > 0 || tab_rewrite_step[1][j] > 0) {
        if(printedHeader==0) {
          printf("                      \tfails\tsuccess\n");
          printedHeader=1;
        }

        printf("tab_rewrite_step[%d] :\t%u\t%u\n",
               j,tab_rewrite_step[0][j],tab_rewrite_step[1][j]);
      }
    }

    printf("\nrewrite_step = %u\n",rewrite_step);
    diff_sec  = after_self.ru_utime.tv_sec  - before_self.ru_utime.tv_sec;
    diff_usec = after_self.ru_utime.tv_usec - before_self.ru_utime.tv_usec;
    total_time= (double)diff_sec + (((double)diff_usec)/1000000.0);
    if(!quietMode) {
      printf("total time    = %.3f sec\n",total_time);
      if(diff_sec > 0) {
        printf("average speed = %d rwr/sec\n",(long)(((double)rewrite_step)/total_time));
      } else if(diff_usec > 0) {
        printf("average speed = %d rwr/sec\n",(long)(((double)1000000*rewrite_step)/((double)diff_usec)));
      }
    }
  }
  exit(0);
}
