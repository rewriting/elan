/*

    CPL - The Nancy ChoicePoint Library

    Copyright (C) 1996-2003  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
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
    Marian Vittek		e-mail: vittek@guma.ii.fmph.uniba.sk

*/

#include "choice.h"

#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

volatile char    *backTrail = NULL;
long    backTraili = 0;

long    *backCTrail = NULL;
long    backCTraili = 0;

jmp_buf *jmpbuf_stack = NULL;
Pile    *tab_pile = NULL;
int     stack_ptr=-1;

static int backTrail_size    = 0;
static int backCTrail_size   = 0;
static int jmpbuf_stack_size = 0;

/* --- Variables globales ------------------------------------- */

static char *bp_main;
static char *s, *d;
static int global_size;

#ifdef __i386__
inline void * __memcpy_by4(void * to, const void * from, size_t n) {
  register void *tmp = (void *)to;
  register int dummy1,dummy2;
  __asm__ __volatile__ (
    "\n1:\tmovl (%2),%0\n\t"
    "addl $4,%2\n\t"
    "movl %0,(%1)\n\t"
    "addl $4,%1\n\t"
    "decl %3\n\t"
    "jnz 1b"
    :"=r" (dummy1), "=r" (tmp), "=r" (from), "=r" (dummy2) 
    :"1" (tmp), "2" (from), "3" (n/4)
    :"memory");
  return (to);
}

inline void * __memcpy_by8(void * to, const void * from, size_t n) {
  register void *tmp = (void *)to;
  register int tmp1,tmp2,dummy;
  __asm__ __volatile__ (
    "\n1:\tmovl (%3),%0\n\t"
    "movl 4(%3),%1\n\t"
    "addl $8,%3\n\t"
    "movl %0,(%2)\n\t"
    "movl %1,4(%2)\n\t"
    "addl $8,%2\n\t"
    "decl %4\n\t"
    "jnz 1b"
    :"=r" (tmp1), "=r" (tmp2), "=r" (tmp), "=r" (from), "=r" (dummy) 
    :                           "2" (tmp), "3" (from),  "4" (n/8)
    :"memory");
  return (to);
}
#define memcpy(d,s,n) __memcpy_by8(d,s,n)

#else

#include <string.h>

#endif


#define AT_MALLOC_PROTECT(x)    func_malloc_protect(x)
#define AT_AMALLOC(x)           func_malloc(x)
#define AT_REALLOC_PROTECT(x,y) func_realloc_protect(x,y)
#define AT_REALLOC(x,y)         func_realloc(x,y)

static void* (*func_malloc_protect)();
static void* (*func_malloc)();
static void* (*func_realloc_protect)();
static void* (*func_realloc)();

void CPL_init_malloc_protect(void* (*f)()) {
  func_malloc_protect = f;
}
void CPL_init_malloc(void* (*f)()) {
  func_malloc = f;
}
void CPL_init_realloc_protect(void* (*f)()) {
  func_realloc_protect = f;
}
void CPL_init_realloc(void* (*f)()) {
  func_realloc = f;
}




/* --- Le Choice_Point() -------------------------------------- */

#define scale sizeof(long)
#define memoround(n) if((n)%scale) (n)+=(scale-((n)%scale));
#define compute_size(old_size,size) ((size>=old_size)?(old_size+size+1):(2*old_size))

char *get_sp(long dummy) {
  return (char*)(&dummy);
}

/*
void check_size_char(char *name, void **ptr_stack, int stack_index, int *stack_size, int size) {
  if( stack_index+size >= (*stack_size) ) {
    (*stack_size) = compute_size((*stack_size),size);
    (*ptr_stack) = (void*) AT_REALLOC_PROTECT((*ptr_stack), (*stack_size)*sizeof(char));
    printf("resized '%s' to %d bytes\n",name,(*stack_size));
  }
}

void check_size_long(char *name, long **ptr_stack, int stack_index, int *stack_size, int size) {
  if( stack_index+size >= (*stack_size) ) {
    (*stack_size) = compute_size((*stack_size),size);
    (*ptr_stack) = (long*) AT_REALLOC((*ptr_stack), (*stack_size)*sizeof(long));
    printf("resized '%s' to %d entries\n",name,(*stack_size));
  }
}
*/

void increment_stack_ptr() {
  if( (stack_ptr+1) >= jmpbuf_stack_size) {
    jmpbuf_stack_size = jmpbuf_stack_size * 2;
    tab_pile     = (Pile*)    AT_REALLOC(tab_pile,jmpbuf_stack_size*sizeof(Pile));
    jmpbuf_stack = (jmp_buf*) AT_REALLOC_PROTECT(jmpbuf_stack, jmpbuf_stack_size*sizeof(jmp_buf));
#ifdef DEBUG
    printf("resized 'tab_pile' and 'jmpbuf_stack' to %d entries\n", jmpbuf_stack_size);
#endif
  }
}

void increment_backTrail(int size) {
  if( backTraili+size >= backTrail_size ) {
    backTrail_size = compute_size(backTrail_size,size);
    backTrail = (char*) AT_REALLOC_PROTECT(backTrail, backTrail_size*sizeof(char));
#ifdef DEBUG
    printf("resized 'backTrail' to %d bytes\n",backTrail_size);
#endif
  }
}

void increment_backCTrail(int size) {
  if( backCTraili+size >= backCTrail_size ) {
    backCTrail_size = compute_size(backCTrail_size,size);
    backCTrail = (long*) AT_REALLOC(backCTrail, backCTrail_size*sizeof(long));
#ifdef DEBUG
    printf("resized 'backCTrail' to %d entries\n",backCTrail_size);
#endif
  }
}

int _localSetChoicePoint() {
  increment_stack_ptr();
  stack_ptr++;
  tab_pile[stack_ptr].backCTraili = backCTraili;
  tab_pile[stack_ptr].size = -1;
  return setjmp(jmpbuf_stack[stack_ptr]);
}

int setChoicePoint() {
  increment_stack_ptr();
  stack_ptr++;
  /* pose du point de choix */
#if defined __GNUC__ && __i386__
  asm("mov %%esp,%0" : "=g" (s));
#else
  s = get_sp(0);
#endif
  global_size = bp_main - s;
    //check_size_char("backTrail",&backTrail, backTraili, &backTrail_size, global_size);
  tab_pile[stack_ptr].data_index  = backTraili;
  tab_pile[stack_ptr].size        = global_size;
  tab_pile[stack_ptr].backCTraili = backCTraili;
  increment_backTrail(global_size);
  d = (char*)backTrail + backTraili;
  backTraili += global_size;
#ifdef __sparc__
  asm(" ta      0x3   ! ST_FLUSH_WINDOWS");
#endif

    //printf("backTrail = %x\tdata_index=%d\n",backTrail,tab_pile[stack_ptr].data_index);
    //printf("dest      = %x\ts=%x\tsize=%d\n",d,s,size); 
  
  memcpy(d, s, global_size);
  return setjmp(jmpbuf_stack[stack_ptr]);
    /*
      if(setjmp(stack[stack_ptr])) {
        // mise a 0 du Trail pour le GC
          //for(s=tab_pile[stack_ptr+1].data ;
            //s<tab_pile[stack_ptr+1].data+tab_pile[stack_ptr+1].size ; )
              //s++=0;
              return 1;
              }
              return 0;
    */
}

/* --- Le Fail() ---------------------------------------------------- */

static void makeLongJump() {
  memcpy(d, s, global_size);
  backTraili -= global_size;
  backCTraili = tab_pile[stack_ptr].backCTraili;
  longjmp(jmpbuf_stack[stack_ptr--], 1);
  printf("[fail] Error\n");
}

void globalFail() {
  register volatile char *sp;
    /* suppression du point de choix */
  s = (char*)backTrail + tab_pile[stack_ptr].data_index;
  global_size = tab_pile[stack_ptr].size;
  d = bp_main - global_size;
#if DEBUG
//  printf("dest      = %x\ts=%x\tsize=%d\n",d,s,global_size);
#endif
    /* mise a niveau du stack pointer */
#if defined __GNUC__ && __i386__
  asm("mov %%esp,%0" : "=r" (sp));
#else
  sp=get_sp(0);
#endif
  if(sp > d) { // [pem: Oct 20 00] was: (sp >= d)
    alloca(sp - d);
  }
    
#ifdef __sparc__
  asm(" ta      0x3   ! ST_FLUSH_WINDOWS");
#endif
  
  makeLongJump();
  exit(0);
}

/* --- Initialisation ----------------------------------------------- */


void choice_init(long *bp) {
  bp_main = (char*)bp;

  backTrail_size    = INITIAL_BACKTRAIL_SIZE;
  backCTrail_size   = INITIAL_BACKCTRAIL_SIZE;
  jmpbuf_stack_size = INITIAL_JMPBUF_STACK_SIZE;

  backTrail    = (char*)    AT_MALLOC_PROTECT(backTrail_size*sizeof(char));
  backCTrail   = (long*)    AT_AMALLOC(backCTrail_size*sizeof(long));
  tab_pile     = (Pile*)    AT_AMALLOC(jmpbuf_stack_size*sizeof(Pile));
  jmpbuf_stack = (jmp_buf*) AT_MALLOC_PROTECT(jmpbuf_stack_size*sizeof(jmp_buf));
  
  backTraili  = 0;
  backCTraili = 0;
  stack_ptr=-1;

    //memset(backTrail,0,backTrail_size*sizeof(char));
    //memset(jmpbuf_stack,0,jmpbuf_stack_size*sizeof(jmp_buf));
}

void longjmperror() {
  fprintf(stderr,"longjmperror\n");
  exit(1);
}

/* --- CutOpen et CutClose ------------------------------------------- */
#define ALLOCMARK -1

inline void cCutMark(char *ra) {
    //check_size_long("backCTrail",&backCTrail, backCTraili, &backCTrail_size,2);
  increment_backCTrail(2);
  backCTraili += 2;
  backCTrail[backCTraili-2] = stack_ptr;
  backCTrail[backCTraili-1] = backTraili;
}

inline char *cCut() {
    //fprintf(stdout,"cCut\n");
  while(backCTrail[backCTraili-2] == ALLOCMARK) {
    backCTraili -= 2;
  }
  stack_ptr    = backCTrail[backCTraili-2];
  backTraili   = backCTrail[backCTraili-1];
  backCTraili -= 2;
  return 0;
}

inline int allocStablePointer(int size) {
    //check_size_long("backCTrail",&backCTrail, backCTraili, &backCTrail_size, 2);
  increment_backCTrail(2);
  backCTraili += 2;
  backCTrail[backCTraili-2] = ALLOCMARK;
  backCTrail[backCTraili-1] = backTraili;
    //check_size_char("backTrail",&backTrail, backTraili, &backTrail_size, size);
  increment_backTrail(size);
  backTraili += size;
    //return (char*) &(backTrail[backTraili-size]);
  return (backTraili-size);
}

inline char **getStablePointer(long index) {
    // printf("getStablePointer(%d) = %x\n",index,&(backTrail[index]));
  return (char**) &(backTrail[index]);
}
