#include "main_coq_skeleton.h"

int main_elan_init(int ,char **, char* ,char*, char*,char*);
int main_elan_run();

extern struct term *query;

/* Constantes d'execution */
unsigned long rewrite_step=0;
unsigned long rewrite_label_step=0;
unsigned long rewrite_real_step=0;
int global_indentlevel=0;
int debugMode=0;
int quietMode=0;
int coqMode=0;
int printMode=0;
int queryMode=2;  /* 0:noInput  1:REFInput  2:Elan form 3:command line */
int ResultMode=2; /* 0:noOutput 1:REFOutput 2:Elan form 3:internalOutput */
int evaluationMode=0; /* 0:lgi 1:sort 2:strat:sort*/
char *sortName=NULL;
char *strategyName=NULL;

/* Procedure principale */
long *bp_main;
struct rusage before_self,after_self;
long diff_sec,diff_usec;
double total_time;

int main_elan_init(int argc,char **argv, char *port,
		   char *file_query, char* file_query_args, char *file_trace) {
  long bp;
  struct term *res;
  int i,j;
  char lazy_file[30];

  choice_init(&bp);
  bp_main=&bp;
  GC_free_space_divisor=2;
  sprintf(trace_file,"%s%s",file_trace,port);
  sprintf(query_file,"%s%s",file_query,port);
  sprintf(query_sort_file,"%s%s",file_query_args,port);

  for(i=1 ; i<argc && *argv[i]=='-' ; i++) {
    if(!strcmp(argv[i],"-coq")) {
      coqMode=1;
    }
    if(!strcmp(argv[i],"-printTrace")) {
      printMode=1;
    }
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
      ResultMode=0;
    }
    if(!strcmp(argv[i],"-REFOutput")) {
      ResultMode=1;
    }
    if(!strcmp(argv[i],"-internalOutput")) {
      ResultMode=3;
    }
    if(!strcmp(argv[i],"-debug")) {
      debugMode=1;
    }
    if(!strcmp(argv[i],"-quiet")) {
      quietMode=1;
    }
    if(!strcmp(argv[i],"-sort")) {
      evaluationMode=1;
      if (argc>i+1)
        sortName=argv[++i];
      else exit(0);
    }
    if(!strcmp(argv[i],"-strategy")) {
      evaluationMode=2;
      if (argc>i+1)
        strategyName=argv[++i];
      else {
        printf("the '-strategy' option should be followed by:
	<strategy>:<sort>");
        exit(0);
      }
    }
  }

  symbol_init();
  Ginit_builtin(); // should be done before tab_bijection_init
#ifdef ATERM
  tab_bijection_init();
#endif

    /* Initialisation des pattern_list */
  if(queryMode==1) {
    yyparse();
  }
  EkerTermInit();
  initTabRef();
  backTrackInit();
  EarleyParserInit();

  if(coqMode) {
      strcpy(lazy_file,argv[0]);
      lazy_annotation_read(strcat(lazy_file,".lazy"));
  }
}

int main_elan_run() {
  struct term *res;

  rewrite_step = 0;
  rewrite_label_step = 0;
  getrusage(RUSAGE_SELF, &before_self);
  if (!setChoicePoint()) {
    /* Input */
      if ((res=termParser(queryMode,evaluationMode)) == NULL) return 1;
      /* Output */
      switch(ResultMode) {
      case 0:
        break;
      case 1:
        term_printREFln(stdout,res);
        break;
      case 2:
        printf("\nresult = ");printf("\nin main_coq_skeleton.c line 125\n")
	printf("\n in main_elan_run arity = %d",fsymtab[322].arity);
        termOut(stdout,term_unflatten(res));
        printf("\n");
        break;
      case 3:
        printf("\nresult = ");printf("\nin main_coq_skeleton.c line 131\n");
        term_printnl(stdout,res);
        break;
    }
    if(ResultMode!=1) {
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
  if(ResultMode!=1) {
      if (coqMode){
	  printf("\nrewrite_step = %u where traced steps: %u\n",rewrite_step, rewrite_label_step);
      } else {
	  printf("\nrewrite_step = %u\n",rewrite_step);
      }
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
  return 0;
}
