#ifndef _streval_h_
#define _streval_h_
//#include "../../elan/codes.h"
#include "codes.h"
#include "term.h"
#include "back.h"

#define EXTERN_FUN0(N) extern struct term *N(int code);
#define EXTERN_FUN1(N) extern struct term *N(int code, struct term *s);
#define EXTERN_FUN2(N) extern struct term *N(int code, struct term *s, struct term *t);
#define EXTERN_FUN3(N) extern struct term *N(int code, struct term *b, struct term *s, struct term *t);


EXTERN_FUN2(fun_180); 
EXTERN_FUN1(fun_181);
EXTERN_FUN1(fun_182);
EXTERN_FUN0(fun_183);
EXTERN_FUN0(fun_186);
EXTERN_FUN3(fun_184);
EXTERN_FUN2(fun_185);
EXTERN_FUN3(fun_187);
EXTERN_FUN2(fun_188);
EXTERN_FUN0(fun_189);
EXTERN_FUN1(fun_190);

extern struct term *str_eval(struct term *T);
extern struct term *str_eval2(struct term *s, struct term *t);

#endif
