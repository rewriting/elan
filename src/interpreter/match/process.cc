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


// include to match.c & runtime.c

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>
#include <sys/errno.h>
#include <stdio.h>

int pipe_index = 0;


int j;

struct processdata *pid2processdata(int pid)
{
struct processdatalist *pdl;
  for(processtab.forinit(); processtab.forcond(); processtab.fornext()) {
    j = processtab.addstr(processtab.foractval());
    for(pdl = processlists[j]; pdl; pdl = pdl->next)
      if (pdl->pd->pid == pid) 
        return pdl->pd; }
  return NULL;
}

void pipe_names(char *pipe1_name, char *pipe2_name)
{
  snprintf(pipe1_name,STRLEN,"/bin/rm -f .internal.pipe1.%d.%d",getpid(),pipe_index);
  snprintf(pipe2_name,STRLEN,"/bin/rm -f .internal.pipe2.%d.%d",getpid(),pipe_index);
  (void)!system(pipe1_name);
  (void)!system(pipe2_name);
  snprintf(pipe1_name,STRLEN,".internal.pipe1.%d.%d",getpid(),pipe_index);
  snprintf(pipe2_name,STRLEN,".internal.pipe2.%d.%d",getpid(),pipe_index);

  pipe_index++;
    if (mknod(pipe1_name,S_IFIFO|0777,0)) {
      sterr << "mknod " << pipe1_name << " problem \n" << errno << "\n"; 
      failexit();}
    if (mknod(pipe2_name,S_IFIFO|0777,0)) {
	  sterr <<"mknod " << pipe2_name << " problem \n"<< errno << "\n"; 
	  failexit(); }
}

int command2pli(const char *command)
{
int pli;
  if (! processtab.member(command)) {
    pli = processtab.addstr(command);
    processlists[pli] = NULL;
  } else {
    pli = processtab.posid;
  }
  return pli;
}


void open_subprocess_pipes(char *pipe1_name, char *pipe2_name, int r[], int w[])
{
	if((r[1] = open(pipe1_name,O_RDONLY/*|O_NDELAY*/,0)) == -1) {
	  sterr <<"open read " << pipe1_name << " problem \n"<< errno << "\n";
	  kill(getppid(),SIGKILL); failexit(); }

	if ((w[0] = open(pipe2_name,O_WRONLY,0)) == -1) {
	  sterr <<"open write " << pipe2_name << " problem \n"<<errno <<"\n"; 
	  kill(getppid(),SIGKILL); failexit(); }
	
        if (dup2(w[0],1) == -1 || dup2(r[1],0)== -1) {
	  sterr<< "[error] match_state: can't redirect i/o of subprocess "
	       << " int.err.\n";
	  kill(getppid(),SIGKILL); failexit(); }
}

void open_masterprocess_pipes(int noblock, char *pipe1_name, char *pipe2_name, int r[], int w[])
{
    if (noblock) {
      if((r[0] =open(pipe2_name,O_RDONLY | ((noblock)?O_NDELAY:0) ,0)) == -1) {
	sterr <<"open read " << pipe2_name << " problem \n"<< errno << "\n";
	failexit(); }
      if ((w[1] = open(pipe1_name,O_WRONLY,0)) == -1) {
	sterr <<"write " << pipe1_name << " problem \n"<< errno << "\n"; 
	failexit(); }
    } 
    else  /* !noblock */
      {
	if ((w[1] = open(pipe1_name,O_WRONLY,0)) == -1) {
	  sterr <<"write " << pipe1_name << " problem \n"<< errno << "\n"; 
	  failexit(); }
	if((r[0] = open(pipe2_name,O_RDONLY | ((noblock)?O_NDELAY:0),0)) == -1) {
	  sterr <<"read " << pipe2_name << " problem \n"<< errno << "\n"; 
	  failexit(); }
      }
}


struct processdata *newprocess(const char *command, 
			       const char *arg1,  
			       const char *arg2,
			       const char *arg3,
			       const char *arg4,
			       const char *arg5,
			       const char *arg6,
			       const char *arg7,
			       const char *arg8,
			       int maxcount, int noblock)
{ struct processdatalist  *pdl; /**pl,*/
  int pli;
  struct processdata *res;
  int r[2],w[2];
  char *pipename;
  char pipe1_name[STRLEN];
  char pipe2_name[STRLEN];
  FILE *tmp,*ttmp;

   pli = command2pli(command);
 
  pipe_names(pipe1_name,pipe2_name);

    NNEW(res ,struct processdata);
    res->counter = maxcount-1;
    res->nofreads = 0;
    res->actplist = & processlists[pli];

    // add a vagon
    NNEW(pdl, struct processdatalist);
    pdl->next = processlists[pli]; 
    pdl->pd = res;
    processlists[pli] = pdl;

    res->pid = fork() ;
    switch ( res->pid ) {
    case -1 :
        sterr<<"[error] get_process(match.c): sorry, can't create new process\n";
        sterr<<"\tprobably not enought memory\n";
        failexit();
    case 0  :                     /* here I will exec subprocess */
        fatal_in_forked_child();  // a fatal error exits the child directly (base/fatal.h)
        open_subprocess_pipes(pipe1_name,pipe2_name,r,w);
	execlp(command,command,arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,NULL);
        fprintf(stderr,"[error] match_state: can't execute %s\n\t int.err.\n",
		       command);
        kill(getppid(),SIGKILL); failexit();
    }

                                  /* here I will exec master process */

    open_masterprocess_pipes(noblock,pipe1_name,pipe2_name,r,w);

    ttmp = fdopen(w[1],"w");
    tmp = fdopen(r[0],"r");

    ungetc('#',tmp);
    if (ttmp == NULL || tmp==NULL) {
       sterr << "\n[error] get_process(match.c): " <<
	        "can't open pipe communications, int.error\n";
       failexit();
    }
    NNEW(res->pin ,ochstream(ttmp));
    pipename = addsuffix("pipe from ",command);
    ichstream *is;

    NNEW(is ,ichstream(tmp,pipename,noblock));   
    res->is = is;
    NNEW(res->s ,lstream(is,'#',noblock));
    res->noblocking = noblock;
    *res->pin << " ";
  return(res);
}

/*
 * Il y un bug ici dans la destruction de memoire
 */
void killprocess(struct processdata *ppd)
{
  int status;
  if (ppd) {
  kill(ppd->pid,SIGKILL);
  waitpid(ppd->pid,&status,0);
  /*
  if (ppd->pin != NULL)
    DELETE2(ppd->pin);
  DELETE2(ppd->is);
  DELETE2(ppd->s);
  CFRE(ppd);
  */
  }
} 


void freeprocess(struct processdata *pd)
{   struct processdatalist *pl;
    lexem lex, SSEND;
    SSEND.cridlex("END");
    if (pd) {
    if (pd->counter <= 0) killprocess(pd); 
    else {
      pd->s->fulex(lex);
      *pd->pin << ";\n"; pd->pin->flush();
      while(lex!=SSEND && lex.isnotendofstream()) {
	pd->s->ilex(lex);pd->s->fulex(lex);
      }
      pd->s->ilex(lex);
      NNEW(pl ,struct processdatalist);
      pl->pd = pd ; pl->next = * pd->actplist;
      * pd->actplist = pl;
    }
    }
}

/*
 * Il y un bug ici dans la destruction de memoire
 */
static void killprocess_for_all_processes(struct processdata *ppd)
{
  int status;
  if (ppd) {
  kill(ppd->pid,SIGKILL);
  waitpid(ppd->pid,&status,0);
  /*
  if (ppd->pin != NULL)
    DELETE2(ppd->pin);
  DELETE2(ppd->is);
  DELETE2(ppd->s);
  CFRE(ppd);
  */
  }
}

void kill_all_processus()
{
  struct processdatalist *pp,*ppl;
  int pind;
  //sterr << "enter kill_all_process\n";
  for(processtab.forinit(); processtab.forcond(); processtab.fornext()) {
    pind = processtab.addstr(processtab.foractval());
    ppl = processlists[pind];
    //    sterr << "pind = " << pind << " ppl = " << ppl << "\n";

    while(ppl!=NULL) {
      killprocess_for_all_processes(ppl->pd);
      pp = ppl;
      ppl = ppl->next;
      CFRE(pp);
    }
    processlists[pind] = NULL;
  }
  //sterr << "exit kill_all_process\n";
}

void kilproc(struct  processdata *pd)
{ killprocess(pd); }



struct processdata *newsubprocess(stateofexecution *stexec,
			       int maxcount, int noblock)
{ struct processdatalist  *pdl; /* *pl,*/
  int pli;
  struct processdata *res;
  int r[2],w[2];
  char *pipename;
  char pipe1_name[STRLEN];
  char pipe2_name[STRLEN];
  char command[STRLEN];
  FILE *tmp,*ttmp;
  int result;
  term resultterm;
  int ch;

  snprintf(command,STRLEN,"DKCON%d",pipe_index);

  pli = command2pli(command);

  pipe_names(pipe1_name, pipe2_name);

    NNEW(res ,struct processdata);
    res->counter = maxcount-1;
    res->nofreads = 0;
    res->actplist = & processlists[pli];

    // add a vagon
    NNEW(pdl, struct processdatalist);
    pdl->next = processlists[pli]; 
    pdl->pd = res;
    processlists[pli] = pdl;

    res->pid = fork() ;
    switch ( res->pid ) {
    case -1 :
        sterr<<"[error] get_process(match.c): sorry, can't create new sub-process\n";
        sterr<<"\tprobably not enought memory\n";
        failexit();
    case 0  :                     /* here I will exec subprocess */
        fatal_in_forked_child();  // a fatal error exits the child directly (base/fatal.h)
        open_subprocess_pipes(pipe1_name,pipe2_name,r,w);

	// ... execution

    batch = 1; quiet = 1; warnings= 0; dump= 0; trace= 0;
    // c'est pas une solution correcte !!!
    do {
  	result = stexec->nextsolution(resultterm);

	/*{ochstream ff("www"); resultterm.write(ff); ff.flush();} */

	if (result) {
	  stout << "## 0 *\n"; resultterm.write(stout); stout << "#\n"; } 
	else {
	  stout << "END#\n";  // no more solutions
//	  stout << "&END#\n";  // no more solutions
                  //^ ici, j'ai ajoute '&' pour lire plus facilement
         //sigsend(P_PID,getppid(),SIGUSR1); 

	  while(1) {;}  /**/exit(0); 
          }

	do { ch = getchar(); } while (ch != '.' && ch != ';');

	/* {ochstream gg("eee"); gg<<ch; gg.flush();} */
    } while (ch == '.');
    failexit();
    }

                                  /* here I will exec master process */
    open_masterprocess_pipes(noblock,pipe1_name,pipe2_name,r,w);

    ttmp = fdopen(w[1],"w");
    tmp = fdopen(r[0],"r");

    ungetc('#',tmp);
    if (ttmp == NULL || tmp==NULL) {
       sterr << "\n[error] get_process(match.c): " <<
	        "can't open pipe communications, int.error\n";
       failexit();
    }
    NNEW(res->pin ,ochstream(ttmp));
    pipename = addsuffix("pipe from ",command);
    ichstream *is;

    NNEW(is ,ichstream(tmp,pipename,noblock));   
    res->is = is;
    NNEW(res->s ,lstream(is,'#',noblock));
    res->noblocking = noblock;
    *res->pin << " ";
  return(res);
}


int isNextSolInSubProcess(struct processdata *pd)
{ lexem lex, SSEND;

  SSEND.cridlex("END");
  if (pd->noblocking && !pd->s->isready()) {
    return(0);
  }
  if (pd->pin != NULL) {
    *pd->pin << ".\n"; pd->pin->flush();
  }
  pd->s->fulex(lex);
  while(lex == '#') { 
    pd->s->ilex(lex); pd->s->fulex(lex);} 
  if (lex.isnum()) {                       // there is next solution
    do {pd->s->ilex(lex);pd->s->fulex(lex);
    } while (lex=='*');
    return(1);
//  } else if (lex == '&')  // 'E''N''D'
  } else if (lex == SSEND)  // 'E''N''D'
    return -1;
  else 
    return(0);
}


int nextsolsubprocess(struct processdata *pd,grammar *gr,int s,term &res)
{ lexem lex;
  int r;

  if ((r=isNextSolInSubProcess(pd)) == 1) { 

    lex.crtypelex(s);
    if (! gr->earleycall(pd->s,lex,procendofs)) failexit();

    res.popt();
    return(1);
  } else 
    return(r);
}


