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
char *queryName=NULL;  //[QUANG: Oct 20 01] 
int optQuery=0; //[QUANG: Oct 20 01] 
int testAC=0;  //[NGUYEN: Sep 10 01] 
FILE *file_query=NULL; //NGUYEN
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
  printf("    -sort=<sortName>\n");
  printf("    -strategy=<strategyName>\n");
  printf("    -filequery=<filename>\n");  //[QUANG: Dec 19 01] 
  printf("    -debug\n");
  printf("    -trace <number>\n");
  exit(1);
}

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

/* Procedure principale */
long *bp_main;
struct rusage before_self,after_self;
long diff_sec,diff_usec;
double total_time;
int main(int argc,char **argv) {
  long bp;
  Gterm *res;
  int i,j;
  char lazy_file[30];
  int queryMode=2;  /* 0:noInput  1:REFInput  2:Elan form 3:command line */
  int evaluationMode=0; /* 0:lgi 1:sort 2:strat:sort*/
  Gterm_init(argc,argv,&bp);
  //Ginit_builtin();

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
  
  choice_init(&bp);
  bp_main=&bp;

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
    if(!strcmp(argv[i],"-testAC")) { //[NGUYEN: Sep 10 01] 
      testAC=1;
    }
    if(!strcmp(argv[i],"-filequery")) {  //[QUANG: Oct 20 01] 
      if(i>=argc-1) {
	  printHelp();
      } else {
	  queryName=argv[++i];
	  optQuery=1;
      }
    }
    if(!strcmp(argv[i],"-printTrace")) {
      printMode=1;
    }

    if(!strcmp(argv[i],"-help") || !strcmp(argv[i],"-h")) {
      printHelp();
    } 

  }

  symbol_init();
  Ginit_builtin(); // should be done before tab_bijection_init
#ifdef ATERM
  tab_bijection_init();
#endif

  if(queryMode==1) {
    yyparse();
  }
  EkerTermInit();
  initTabRef();
  EarleyParserInit();
  
  getrusage(RUSAGE_SELF, &before_self);
    //backTrackInit();
  if(coqMode) {
	strcpy(trace_file,"/tmp/trace");
	if (optQuery){  //[QUANG: Dec 19 01] 
	    strcpy(query_file,queryName);
	} else {
	    strcpy(query_file,"/tmp/query");
	}
	strcpy(query_sort_file,"/tmp/query_sort");
	strcpy(lazy_file,argv[0]);
        lazy_annotation_read(strcat(lazy_file,".lazy"));
          // TODO
  }

  for(i=0 ; i<2 ; i++) {
    for(j=0 ; j<FSYM_TAB_SIZE ; j++) {
      tab_rewrite_step[i][j] = 0;
    }
  }
  
  if(!setChoicePoint()) {
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
        termOut(stdout,(Gterm*)term_unflatten(res));
        printf("\n");
        break;
      case 3:
        printf("\nresult = ");
        term_printnl(stdout,res);
        break;
    }
   //[NGUYEN: May  8 01]  only for testing group AC to be removed after
    if(coqMode) {
      if(file_query != NULL) {
        if(!testAC) {
          termOut(file_query,term_unflatten(res));
        }
        fclose(file_query);
      }
    }
    /*-------------------------------------------------*/

    if(resultMode!=1) {
        //backStatistics();
      globalStatistics();
    }
    fail();
  }
end:
  getrusage(RUSAGE_SELF, &after_self);
#ifdef DEBUG
    //backStatistics();
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
    if(coqMode) {
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
  exit(0);
}
