#include "main_coq_skeleton.h"
#include "signal.h"
#include "soct.h"

int service(int nsock,int argc, char ** argv);
int main_elan_init(int argc,char **argv);
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
extern char trace_file[];
extern char query_file[];
extern char query_sort_file[]; 

/* Procedure principale */
long *bp_main;
struct rusage before_self,after_self;
long diff_sec,diff_usec;
double total_time;

main(int argc, char **argv)
{
 int sock;
 int sock_client;
 struct sockaddr_in server;
 int ret;
 char buf_out[20];

 sock = socket(AF_INET, SOCK_STREAM, 0);
 bzero(&server, sizeof(server));
 server.sin_family = AF_INET;
 server.sin_addr.s_addr = INADDR_ANY;
 server.sin_port = htons(atoi(argv[argc-1]));
 ret = bind(sock, (struct sockaddr *)&server, sizeof(server));
 if (ret != 0) {
     fprintf(stderr,"Error : failed in starting server %s. Port %s is busy.\n",argv[0], argv[argc-1]);
     exit(1);
 }
 main_elan_init(argc-1,argv);
 strcpy(trace_file,FILE_TRACE);
 strcpy(query_file,FILE_QUERY);
 strcpy(query_sort_file,FILE_QUERY_ARGS);
 
 fprintf(stderr,"Server %s started, port %s...\n",argv[0],argv[argc-1]);

 listen(sock,3);
 for(;;){
     sock_client = accept(sock, (struct sockaddr *) 0, (int *) 0);
     if ((ret = service(sock_client,argc,argv)) == 1) { 
	 fprintf(stderr,"Server %s stopped ...\n",argv[0]);
	 elan_reply(sock_client,REP_NIL,QUIT_OK);
	 close(sock_client); 
	 break;
     }
     close(sock_client);
 }
 exit(0);
}

int service(int nsock,int argc, char ** argv)
{
 int ret;
 char buf[BUF_SIZE];

 if (elan_readline(nsock, buf) < 0) return 0;
 if (elan_match(buf,REQ_QUIT) == 0) return 1;  /*signal QUIT to stop server*/
 if (elan_match(buf,REQ_NORM) != 0) return 0; 
 if ((ret = main_elan_run(argc-2,argv)) ==0){
     elan_reply(nsock, REP_NIL, TERM_OK);
 }
 else
  {
      elan_reply(nsock,REP_NIL, TERM_KO); 
  }

 return 0; 
}

int main_elan_init(int argc,char **argv) {
  long bp;
  struct term *res;
  int i,j;
  choice_init(&bp);
  bp_main=&bp;
  GC_free_space_divisor=2;

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
    /* Initialisation des pattern_list */
  if(queryMode==1) {
    yyparse();
  }
  EkerTermInit();
  initTabRef();
  backTrackInit();
  EarleyParserInit();

  if(coqMode) {
    lazy_annotation_read(strcat(argv[0],".lazy"));
  }

}

int main_elan_run() {
  struct term *res;

  rewrite_step = 0;
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
        printf("\nresult = ");
        termOut(stdout,term_unflatten(res));
        printf("\n");
        break;
      case 3:
        printf("\nresult = ");
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
