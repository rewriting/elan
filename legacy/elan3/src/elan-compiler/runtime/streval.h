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
#ifndef _streval_h_
#define _streval_h_
//#include "../../elan/codes.h"
#include "codes.h"
#include "termCommon.h"
//#include "back.h"

#define EXTERN_FUN0(N) extern Gterm *N(int code);
#define EXTERN_FUN1(N) extern Gterm *N(int code, Gterm *s);
#define EXTERN_FUN2(N) extern Gterm *N(int code, Gterm *s, Gterm *t);
#define EXTERN_FUN3(N) extern Gterm *N(int code, Gterm *b, Gterm *s, Gterm *t);


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

EXTERN_FUN1(fun_205);
EXTERN_FUN1(fun_206);
EXTERN_FUN1(fun_207);

extern Gterm *str_eval(Gterm *T);
extern Gterm *str_eval2(Gterm *s, Gterm *t);

#endif
