#include "ans.h"

struct term* fun_301(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_300: /* [] */
    v5= getFreeSubterm(v1,0);
    switch(getInt(v5)) {
    default:
    label3:
      switch(getSymb(v2)) {
      case code_300: /* [] */
        v7= getFreeSubterm(v2,0);
        switch(getInt(v7)) {
        default:
        label5:
          bitSet32_set(mask32,0);
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[3];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: plus(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    /* where var2 := plus(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_3( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_300);
    setFreeSubterm(sv[2],0,sv[0]);
    res = sv[2] ;
    goto end;
    myend0:;
  }
match_fail:
  TERM_ALLOC(res,term2, 301);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_302(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_300: /* [] */
    v5= getFreeSubterm(v1,0);
    switch(getInt(v5)) {
    default:
    label9:
      switch(getSymb(v2)) {
      case code_300: /* [] */
        v7= getFreeSubterm(v2,0);
        switch(getInt(v7)) {
        default:
        label11:
          bitSet32_set(mask32,0);
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[3];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: minus(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    /* where var2 := minus(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_4( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_300);
    setFreeSubterm(sv[2],0,sv[0]);
    res = sv[2] ;
    goto end;
    myend1:;
  }
match_fail:
  TERM_ALLOC(res,term2, 302);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_303(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_300: /* [] */
    v5= getFreeSubterm(v1,0);
    switch(getInt(v5)) {
    default:
    label15:
      switch(getSymb(v2)) {
      case code_300: /* [] */
        v7= getFreeSubterm(v2,0);
        switch(getInt(v7)) {
        default:
        label17:
          bitSet32_set(mask32,0);
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[3];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: time(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    /* where var2 := time(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_5( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_300);
    setFreeSubterm(sv[2],0,sv[0]);
    res = sv[2] ;
    goto end;
    myend2:;
  }
match_fail:
  TERM_ALLOC(res,term2, 303);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_304(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_300: /* [] */
    v5= getFreeSubterm(v1,0);
    switch(getInt(v5)) {
    default:
    label21:
      switch(getSymb(v2)) {
      case code_300: /* [] */
        v7= getFreeSubterm(v2,0);
        switch(getInt(v7)) {
        default:
        label23:
          bitSet32_set(mask32,0);
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[3];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: mod(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    /* where var2 := mod(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_27( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_300);
    setFreeSubterm(sv[2],0,sv[0]);
    res = sv[2] ;
    goto end;
    myend3:;
  }
match_fail:
  TERM_ALLOC(res,term2, 304);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_305(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_300: /* [] */
    v5= getFreeSubterm(v1,0);
    switch(getInt(v5)) {
    default:
    label27:
      switch(getSymb(v2)) {
      case code_300: /* [] */
        v7= getFreeSubterm(v2,0);
        switch(getInt(v7)) {
        default:
        label29:
          bitSet32_set(mask32,0);
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[3];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: and(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    /* where var2 := and(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_28( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_300);
    setFreeSubterm(sv[2],0,sv[0]);
    res = sv[2] ;
    goto end;
    myend4:;
  }
match_fail:
  TERM_ALLOC(res,term2, 305);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_306(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_300: /* [] */
    v5= getFreeSubterm(v1,0);
    switch(getInt(v5)) {
    default:
    label33:
      switch(getSymb(v2)) {
      case code_300: /* [] */
        v7= getFreeSubterm(v2,0);
        switch(getInt(v7)) {
        default:
        label35:
          bitSet32_set(mask32,0);
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[3];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: div(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    /* where var2 := div(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_6( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_300);
    setFreeSubterm(sv[2],0,sv[0]);
    res = sv[2] ;
    goto end;
    myend5:;
  }
match_fail:
  TERM_ALLOC(res,term2, 306);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_307(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_300: /* [] */
    v5= getFreeSubterm(v1,0);
    switch(getInt(v5)) {
    default:
    label39:
      switch(getSymb(v2)) {
      case code_300: /* [] */
        v7= getFreeSubterm(v2,0);
        switch(getInt(v7)) {
        default:
        label41:
          bitSet32_set(mask32,0);
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[3];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: or(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    /* where var2 := or(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_29( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_300);
    setFreeSubterm(sv[2],0,sv[0]);
    res = sv[2] ;
    goto end;
    myend6:;
  }
match_fail:
  TERM_ALLOC(res,term2, 307);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_308(struct term *v1 ) {
  struct term *v2,*v3,*v4,*v5;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_300: /* [] */
    v4= getFreeSubterm(v1,0);
    switch(getInt(v4)) {
    default:
    label45:
      bitSet32_set(mask32,0);
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[3];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: umin()([](var0)) */
    /* allDetEvaluation: det */
    /* where var1 := umin_builtinInt()(var0) */
    // this=var0        underAC=false        Instantiated=false
    sv[1] = fun_20( v4 );
    tmp = sv[0] = sv[1];
    /* rhs: [](var1) */
    // this=var1        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_300);
    setFreeSubterm(sv[2],0,sv[0]);
    res = sv[2] ;
    goto end;
    myend7:;
  }
match_fail:
  TERM_ALLOC(res,term1, 308);
  setFreeSubterm(res,0,v1);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_309(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_300: /* [] */
    v5= getFreeSubterm(v1,0);
    switch(getInt(v5)) {
    default:
    label49:
      switch(getSymb(v2)) {
      case code_300: /* [] */
        v7= getFreeSubterm(v2,0);
        switch(getInt(v7)) {
        default:
        label51:
          bitSet32_set(mask32,0);
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: eq_int(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    /* rhs: eq_builtinInt(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_8( v5,v7 );
    res = sv[0] ;
    goto end;
    myend8:;
  }
match_fail:
  TERM_ALLOC(res,term2, 309);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_310(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_300: /* [] */
    v5= getFreeSubterm(v1,0);
    switch(getInt(v5)) {
    default:
    label55:
      switch(getSymb(v2)) {
      case code_300: /* [] */
        v7= getFreeSubterm(v2,0);
        switch(getInt(v7)) {
        default:
        label57:
          bitSet32_set(mask32,0);
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: neq_int(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    /* rhs: neq_builtinInt(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_9( v5,v7 );
    res = sv[0] ;
    goto end;
    myend9:;
  }
match_fail:
  TERM_ALLOC(res,term2, 310);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_311(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_300: /* [] */
    v5= getFreeSubterm(v1,0);
    switch(getInt(v5)) {
    default:
    label61:
      switch(getSymb(v2)) {
      case code_300: /* [] */
        v7= getFreeSubterm(v2,0);
        switch(getInt(v7)) {
        default:
        label63:
          bitSet32_set(mask32,0);
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: greater_int(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    /* rhs: greater_builtinInt(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_12( v5,v7 );
    res = sv[0] ;
    goto end;
    myend10:;
  }
match_fail:
  TERM_ALLOC(res,term2, 311);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_312(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_300: /* [] */
    v5= getFreeSubterm(v1,0);
    switch(getInt(v5)) {
    default:
    label67:
      switch(getSymb(v2)) {
      case code_300: /* [] */
        v7= getFreeSubterm(v2,0);
        switch(getInt(v7)) {
        default:
        label69:
          bitSet32_set(mask32,0);
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: greatereq_int(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    /* rhs: greatereq_builtinInt(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_13( v5,v7 );
    res = sv[0] ;
    goto end;
    myend11:;
  }
match_fail:
  TERM_ALLOC(res,term2, 312);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_313(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_300: /* [] */
    v5= getFreeSubterm(v1,0);
    switch(getInt(v5)) {
    default:
    label73:
      switch(getSymb(v2)) {
      case code_300: /* [] */
        v7= getFreeSubterm(v2,0);
        switch(getInt(v7)) {
        default:
        label75:
          bitSet32_set(mask32,0);
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: lesseq_int(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    /* rhs: lesseq_builtinInt(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_11( v5,v7 );
    res = sv[0] ;
    goto end;
    myend12:;
  }
match_fail:
  TERM_ALLOC(res,term2, 313);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_314(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_300: /* [] */
    v5= getFreeSubterm(v1,0);
    switch(getInt(v5)) {
    default:
    label79:
      switch(getSymb(v2)) {
      case code_300: /* [] */
        v7= getFreeSubterm(v2,0);
        switch(getInt(v7)) {
        default:
        label81:
          bitSet32_set(mask32,0);
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: less_int(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    /* rhs: less_builtinInt(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_10( v5,v7 );
    res = sv[0] ;
    goto end;
    myend13:;
  }
match_fail:
  TERM_ALLOC(res,term2, 314);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_315(struct term *v1 ) {
  struct term *v2,*v3,*v4;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label84:
    bitSet32_set(mask32,0);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[3];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: btoi_int()(var0) */
    /* allDetEvaluation: det */
    /* where var1 := btoi_builtinInt()(var0) */
    // this=var0        underAC=false        Instantiated=false
    sv[1] = fun_25( v1 );
    tmp = sv[0] = sv[1];
    /* rhs: [](var1) */
    // this=var1        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_300);
    setFreeSubterm(sv[2],0,sv[0]);
    res = sv[2] ;
    goto end;
    myend14:;
  }
match_fail:
  TERM_ALLOC(res,term1, 315);
  setFreeSubterm(res,0,v1);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_316(struct term *v1 ) {
  struct term *v2,*v3,*v4,*v5;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_300: /* [] */
    v4= getFreeSubterm(v1,0);
    switch(getInt(v4)) {
    default:
    label88:
      bitSet32_set(mask32,0);
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: itob_int()([](var0)) */
    /* allDetEvaluation: det */
    /* rhs: itob_builtinInt()(var0) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_26( v4 );
    res = sv[0] ;
    goto end;
    myend15:;
  }
match_fail:
  TERM_ALLOC(res,term1, 316);
  setFreeSubterm(res,0,v1);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_317(struct term *v1 ) {
  struct term *v2,*v3,*v4,*v5;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_300: /* [] */
    v4= getFreeSubterm(v1,0);
    switch(getInt(v4)) {
    default:
    label92:
      bitSet32_set(mask32,0);
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: valueOf()([](var0)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v4 ;
    goto end;
    myend16:;
  }
match_fail:
  TERM_ALLOC(res,term1, 317);
  setFreeSubterm(res,0,v1);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_318(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label95:
    switch(getSymb(v2)) {
    default:
    label96:
      bitSet32_set(mask32,0);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[4];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: new(,)(var0,var1) */
    /* allDetEvaluation: det */
    /* rhs: [](new(,)(valueOf()(var0),.([,](emptyKeyHashTable,var1),nil_HT))) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_317( v1 );
    // this=var1        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term2,code_326);
    setFreeSubterm(sv[2],0,con_327);
    setFreeSubterm(sv[2],1,v2);
    TERM_ALLOC(sv[3],term2,code_329);
    setFreeSubterm(sv[3],0,sv[2]);
    setFreeSubterm(sv[3],1,con_328);
    sv[1] = fun_200( sv[0],sv[3] );
    TERM_ALLOC(sv[0],term1,code_325);
    setFreeSubterm(sv[0],0,sv[1]);
    res = sv[0] ;
    goto end;
    myend17:;
  }
match_fail:
  TERM_ALLOC(res,term2, 318);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_319(struct term *v1 ) {
  struct term *v2,*v3,*v4;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label99:
    bitSet32_set(mask32,0);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[3];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: .cleanAll(var0) */
    /* allDetEvaluation: det */
    /* rhs: .cleanIndex()(var0,minus(,)(size()(var0),[](1))) */
    // this=var0        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_323( v1 );
    TERM_ALLOC(sv[2],term1,code_300);
    setFreeSubterm(sv[2],0,(setIntegerTag(1)));
    sv[1] = fun_302( sv[0],sv[2] );
    sv[0] = fun_334( v1,sv[1] );
    res = sv[0] ;
    goto end;
    myend18:;
  }
match_fail:
  TERM_ALLOC(res,term1, 319);
  setFreeSubterm(res,0,v1);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_320(struct term *v1,struct term *v2,struct term *v3 ) {
  struct term *v4,*v5,*v6,*v7,*v8,*v9;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_325: /* [] */
    v6= getFreeSubterm(v1,0);
    switch(getSymb(v6)) {
    default:
    label103:
      switch(getSymb(v2)) {
      default:
      label104:
        switch(getSymb(v3)) {
        default:
        label105:
          bitSet32_set(mask32,0);
        }
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[8];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: .add(,)([](var0),var1,var2) */
    /* allDetEvaluation: det */
    /* where var3 := mod(,)(builtinHashTerm()(var1),length()(var0)) */
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_210( v2 );
    // this=var0        underAC=false        Instantiated=false
    sv[2] = fun_203( v6 );
    sv[3] = fun_27( sv[1],sv[2] );
    tmp = sv[0] = sv[3];
    /* where var5 := [](var0,var3) */
    // this=var0        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    sv[2] = fun_201( v6,sv[0] );
    tmp = sv[1] = sv[2];
    /* where var4 := update(,,)([,](var1,var2),nil_HT,var5) */
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[5],term2,code_326);
    setFreeSubterm(sv[5],0,v2);
    setFreeSubterm(sv[5],1,v3);
    // this=var5        underAC=false        Instantiated=false
    sv[7] = fun_333( sv[5],con_328,sv[1] );
    tmp = sv[4] = sv[7];
    /* rhs: []([]<-(var0,var3,var4)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    // this=var4        underAC=false        Instantiated=false
    sv[5] = fun_202( v6,sv[0],sv[4] );
    TERM_ALLOC(sv[6],term1,code_325);
    setFreeSubterm(sv[6],0,sv[5]);
    res = sv[6] ;
    goto end;
    myend19:;
  }
match_fail:
  TERM_ALLOC(res,term3, 320);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  setFreeSubterm(res,2,v3);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_321(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_325: /* [] */
    v5= getFreeSubterm(v1,0);
    switch(getSymb(v5)) {
    default:
    label109:
      switch(getSymb(v2)) {
      default:
      label110:
        bitSet32_set(mask32,0);
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[8];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: .get()([](var0),var1) */
    /* allDetEvaluation: semiDet */
    CUTOPEN(); /* Wheres */
    if(localSetChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend20;
    }
    /* where var3 := mod(,)(builtinHashTerm()(var1),length()(var0)) */
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_210( v2 );
    // this=var0        underAC=false        Instantiated=false
    sv[2] = fun_203( v5 );
    sv[3] = fun_27( sv[1],sv[2] );
    tmp = sv[0] = sv[3];
    /* where var4 := [](var0,var3) */
    // this=var0        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    sv[2] = fun_201( v5,sv[0] );
    tmp = sv[1] = sv[2];
    /* where [,](var5,var2) := find(,)(var1,var4) */
    // this=var1        underAC=false        Instantiated=false
    // this=var4        underAC=false        Instantiated=false
    sv[7] = fun_331( v2,sv[1] );
    tmp = sv[6] = sv[7];
    if(code_326 != getSymb(sv[6])) {
      fail();
    } else {
      sv[5] = getFreeSubterm(sv[6],0);
      sv[4] = getFreeSubterm(sv[6],1);
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: var2 */
    // this=var2        underAC=false        Instantiated=false
    res = sv[4] ;
    goto end;
    myend20:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term2, 321);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_322(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_325: /* [] */
    v5= getFreeSubterm(v1,0);
    switch(getSymb(v5)) {
    default:
    label114:
      switch(getSymb(v2)) {
      default:
      label115:
        bitSet32_set(mask32,0);
        bitSet32_set(mask32,1);
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[10];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: .containsKey()([](var0),var1) */
    /* allDetEvaluation: semiDet */
    CUTOPEN(); /* Wheres */
    if(localSetChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend21;
    }
    /* where var2 := mod(,)(builtinHashTerm()(var1),length()(var0)) */
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_210( v2 );
    // this=var0        underAC=false        Instantiated=false
    sv[2] = fun_203( v5 );
    sv[3] = fun_27( sv[1],sv[2] );
    tmp = sv[0] = sv[3];
    /* where var3 := [](var0,var2) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[2] = fun_201( v5,sv[0] );
    tmp = sv[1] = sv[2];
    /* where [,](var4,var5) := find(,)(var1,var3) */
    // this=var1        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    sv[7] = fun_331( v2,sv[1] );
    tmp = sv[6] = sv[7];
    if(code_326 != getSymb(sv[6])) {
      fail();
    } else {
      sv[5] = getFreeSubterm(sv[6],0);
      sv[4] = getFreeSubterm(sv[6],1);
    }
    /* if eq_identifier(,)(var1,var4) */
    // this=var1        underAC=false        Instantiated=false
    // this=var4        underAC=false        Instantiated=false
    sv[8] = fun_18( v2,sv[5] );
    if( sv[8] != con_1 ) {
      fail();
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: true */
    res = con_1 ;
    goto end;
    myend21:;
    CUTCLOSE(); /* Wheres */
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: .containsKey()([](var0),var1) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend22:;
  }
match_fail:
  TERM_ALLOC(res,term2, 322);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_323(struct term *v1 ) {
  struct term *v2,*v3,*v4,*v5;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_325: /* [] */
    v4= getFreeSubterm(v1,0);
    switch(getSymb(v4)) {
    default:
    label119:
      bitSet32_set(mask32,0);
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[2];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: size()([](var0)) */
    /* allDetEvaluation: det */
    /* rhs: [](length()(var0)) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_203( v4 );
    TERM_ALLOC(sv[1],term1,code_300);
    setFreeSubterm(sv[1],0,sv[0]);
    res = sv[1] ;
    goto end;
    myend23:;
  }
match_fail:
  TERM_ALLOC(res,term1, 323);
  setFreeSubterm(res,0,v1);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_324(struct term *v1 ) {
  struct term *v2,*v3,*v4;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label122:
    bitSet32_set(mask32,0);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[2];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: hashTerm()(var0) */
    /* allDetEvaluation: det */
    /* rhs: [](builtinHashTerm()(var0)) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_210( v1 );
    TERM_ALLOC(sv[1],term1,code_300);
    setFreeSubterm(sv[1],0,sv[0]);
    res = sv[1] ;
    goto end;
    myend24:;
  }
match_fail:
  TERM_ALLOC(res,term1, 324);
  setFreeSubterm(res,0,v1);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_331(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,3);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label125:
    switch(getSymb(v2)) {
    case code_329: /* . */
      v6= getFreeSubterm(v2,0);
      switch(getSymb(v6)) {
      case code_326: /* [,] */
        v7= getFreeSubterm(v6,0);
        switch(getSymb(v7)) {
        default:
        label128:
          v8= getFreeSubterm(v6,1);
          switch(getSymb(v8)) {
          default:
          label129:
            v9= getFreeSubterm(v2,1);
            switch(getSymb(v9)) {
            case code_328: /* nil_HT */
              bitSet32_set(mask32,0);
              bitSet32_set(mask32,1);
              bitSet32_set(mask32,2);
              break;
            default:
            label130:
              bitSet32_set(mask32,0);
              bitSet32_set(mask32,2);
            }
          }
        }
        break;
      default:
      label127:
        v9= getFreeSubterm(v2,1);
        switch(getSymb(v9)) {
        case code_328: /* nil_HT */
          bitSet32_set(mask32,1);
          bitSet32_set(mask32,2);
          break;
        default:
        label132:
          bitSet32_set(mask32,2);
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[2];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: find(,)(var0,.([,](var1,var2),var3)) */
    /* allDetEvaluation: det */
    /* if eq_identifier(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_18( v1,v7 );
    if( sv[0] != con_1 ) {
      goto myend25;
    }
    /* rhs: [,](var0,var2) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[1],term2,code_326);
    setFreeSubterm(sv[1],0,v1);
    setFreeSubterm(sv[1],1,v8);
    res = sv[1] ;
    goto end;
    myend25:;
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: find(,)(var0,.(var1,nil_HT)) */
    /* allDetEvaluation: det */
    /* rhs: var1 */
    // this=var1        underAC=false        Instantiated=false
    res = v6 ;
    goto end;
    myend26:;
  }
  if(bitSet32_get(mask32,2)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: find(,)(var0,.(var1,var2)) */
    /* allDetEvaluation: det */
    /* rhs: find(,)(var0,var2) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_331( v1,v9 );
    res = sv[0] ;
    goto end;
    myend27:;
  }
match_fail:
  TERM_ALLOC(res,term2, 331);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_332(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8,*v9;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_329: /* . */
    v5= getFreeSubterm(v1,0);
    switch(getSymb(v5)) {
    default:
    label140:
      v6= getFreeSubterm(v1,1);
      switch(getSymb(v6)) {
      default:
      label141:
        switch(getSymb(v2)) {
        default:
        label142:
          bitSet32_set(mask32,1);
        }
      }
    }
    break;
  case code_328: /* nil_HT */
    switch(getSymb(v2)) {
    default:
    label138:
      bitSet32_set(mask32,0);
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: append(,)(nil_HT,var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend28:;
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[2];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: append(,)(.(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: .(var0,append(,)(var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_332( v6,v2 );
    TERM_ALLOC(sv[1],term2,code_329);
    setFreeSubterm(sv[1],0,v5);
    setFreeSubterm(sv[1],1,sv[0]);
    res = sv[1] ;
    goto end;
    myend29:;
  }
match_fail:
  TERM_ALLOC(res,term2, 332);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_333(struct term *v1,struct term *v2,struct term *v3 ) {
  struct term *v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,3);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_326: /* [,] */
    v6= getFreeSubterm(v1,0);
    switch(getSymb(v6)) {
    default:
    label146:
      v7= getFreeSubterm(v1,1);
      switch(getSymb(v7)) {
      default:
      label147:
        switch(getSymb(v2)) {
        default:
        label148:
          switch(getSymb(v3)) {
          case code_329: /* . */
            v10= getFreeSubterm(v3,0);
            switch(getSymb(v10)) {
            case code_326: /* [,] */
              v11= getFreeSubterm(v10,0);
              switch(getSymb(v11)) {
              default:
              label153:
                v12= getFreeSubterm(v10,1);
                switch(getSymb(v12)) {
                default:
                label154:
                  v13= getFreeSubterm(v3,1);
                  switch(getSymb(v13)) {
                  case code_328: /* nil_HT */
                    bitSet32_set(mask32,0);
                    bitSet32_set(mask32,1);
                    bitSet32_set(mask32,2);
                    break;
                  default:
                  label155:
                    bitSet32_set(mask32,1);
                    bitSet32_set(mask32,2);
                  }
                }
              }
              break;
            default:
            label150:
              v13= getFreeSubterm(v3,1);
              switch(getSymb(v13)) {
              case code_328: /* nil_HT */
                bitSet32_set(mask32,0);
                bitSet32_set(mask32,2);
                break;
              default:
              label151:
                bitSet32_set(mask32,2);
              }
            }
            break;
          /* matching is not complete: jumpNode is null */
          }
        }
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[3];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: update(,,)([,](var0,var1),var2,.(var3,nil_HT)) */
    /* allDetEvaluation: det */
    /* rhs: append(,)(.([,](var0,var1),var2),.(var3,nil_HT)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    TERM_ALLOC(sv[0],term2,code_326);
    setFreeSubterm(sv[0],0,v6);
    setFreeSubterm(sv[0],1,v7);
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[1],term2,code_329);
    setFreeSubterm(sv[1],0,sv[0]);
    setFreeSubterm(sv[1],1,v2);
    // this=var3        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term2,code_329);
    setFreeSubterm(sv[2],0,v10);
    setFreeSubterm(sv[2],1,con_328);
    sv[0] = fun_332( sv[1],sv[2] );
    res = sv[0] ;
    goto end;
    myend30:;
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[3];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: update(,,)([,](var0,var1),var2,.([,](var5,var3),var4)) */
    /* allDetEvaluation: det */
    /* if eq_identifier(,)(var0,var5) */
    // this=var0        underAC=false        Instantiated=false
    // this=var5        underAC=false        Instantiated=false
    sv[0] = fun_18( v6,v11 );
    if( sv[0] != con_1 ) {
      goto myend31;
    }
    /* rhs: append(,)(.([,](var0,var1),var2),var4) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    TERM_ALLOC(sv[1],term2,code_326);
    setFreeSubterm(sv[1],0,v6);
    setFreeSubterm(sv[1],1,v7);
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term2,code_329);
    setFreeSubterm(sv[2],0,sv[1]);
    setFreeSubterm(sv[2],1,v2);
    // this=var4        underAC=false        Instantiated=false
    sv[1] = fun_332( sv[2],v13 );
    res = sv[1] ;
    goto end;
    myend31:;
  }
  if(bitSet32_get(mask32,2)) {
    struct term *tmp, *sv[3];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: update(,,)([,](var0,var1),var2,.(var3,var4)) */
    /* allDetEvaluation: det */
    /* rhs: update(,,)([,](var0,var1),.(var3,var2),var4) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    TERM_ALLOC(sv[0],term2,code_326);
    setFreeSubterm(sv[0],0,v6);
    setFreeSubterm(sv[0],1,v7);
    // this=var3        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[1],term2,code_329);
    setFreeSubterm(sv[1],0,v10);
    setFreeSubterm(sv[1],1,v2);
    // this=var4        underAC=false        Instantiated=false
    sv[2] = fun_333( sv[0],sv[1],v13 );
    res = sv[2] ;
    goto end;
    myend32:;
  }
match_fail:
  TERM_ALLOC(res,term3, 333);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  setFreeSubterm(res,2,v3);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_334(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_325: /* [] */
    v5= getFreeSubterm(v1,0);
    switch(getSymb(v5)) {
    default:
    label161:
      switch(getSymb(v2)) {
      case code_300: /* [] */
        v7= getFreeSubterm(v2,0);
        switch(getInt(v7)) {
        case 0: /* 0 */
          bitSet32_set(mask32,0);
          bitSet32_set(mask32,1);
          break;
        default:
          goto label162;
        }
        break;
      default:
      label162:
        bitSet32_set(mask32,1);
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[6];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: .cleanIndex()([](var0),[](0)) */
    /* allDetEvaluation: det */
    /* where var2 := find(,)(emptyKeyHashTable,[](var0,0)) */
    // this=var0        underAC=false        Instantiated=false
    sv[3] = fun_201( v5,(setIntegerTag(0)) );
    sv[2] = fun_331( con_327,sv[3] );
    tmp = sv[0] = sv[2];
    /* where var1 := []<-(var0,0,.(var2,nil_HT)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[5],term2,code_329);
    setFreeSubterm(sv[5],0,sv[0]);
    setFreeSubterm(sv[5],1,con_328);
    sv[4] = fun_202( v5,(setIntegerTag(0)),sv[5] );
    tmp = sv[1] = sv[4];
    /* rhs: [](var1) */
    // this=var1        underAC=false        Instantiated=false
    TERM_ALLOC(sv[3],term1,code_325);
    setFreeSubterm(sv[3],0,sv[1]);
    res = sv[3] ;
    goto end;
    myend33:;
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[7];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: .cleanIndex()([](var0),var1) */
    /* allDetEvaluation: det */
    /* where var3 := find(,)(emptyKeyHashTable,[](var0,valueOf()(var1))) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[2] = fun_317( v2 );
    sv[3] = fun_201( v5,sv[2] );
    sv[2] = fun_331( con_327,sv[3] );
    tmp = sv[0] = sv[2];
    /* where var2 := []<-(var0,valueOf()(var1),.(var3,nil_HT)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[3] = fun_317( v2 );
    // this=var3        underAC=false        Instantiated=false
    TERM_ALLOC(sv[5],term2,code_329);
    setFreeSubterm(sv[5],0,sv[0]);
    setFreeSubterm(sv[5],1,con_328);
    sv[4] = fun_202( v5,sv[3],sv[5] );
    tmp = sv[1] = sv[4];
    /* rhs: .cleanIndex()([](var2),minus(,)(var1,[](1))) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[3],term1,code_325);
    setFreeSubterm(sv[3],0,sv[1]);
    // this=var1        underAC=false        Instantiated=false
    TERM_ALLOC(sv[6],term1,code_300);
    setFreeSubterm(sv[6],0,(setIntegerTag(1)));
    sv[5] = fun_302( v2,sv[6] );
    sv[6] = fun_334( sv[3],sv[5] );
    res = sv[6] ;
    goto end;
    myend34:;
  }
match_fail:
  TERM_ALLOC(res,term2, 334);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_337(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8,*v9;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_336: /* , */
    v5= getFreeSubterm(v1,0);
    switch(getSymb(v5)) {
    default:
    label170:
      v6= getFreeSubterm(v1,1);
      switch(getSymb(v6)) {
      default:
      label171:
        switch(getSymb(v2)) {
        default:
        label172:
          bitSet32_set(mask32,1);
        }
      }
    }
    break;
  case code_335: /* nil */
    switch(getSymb(v2)) {
    default:
    label168:
      bitSet32_set(mask32,0);
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: @(nil,var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend35:;
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[2];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: @(,(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: ,(var0,@(var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_337( v6,v2 );
    TERM_ALLOC(sv[1],term2,code_336);
    setFreeSubterm(sv[1],0,v5);
    setFreeSubterm(sv[1],1,sv[0]);
    res = sv[1] ;
    goto end;
    myend36:;
  }
match_fail:
  TERM_ALLOC(res,term2, 337);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_339(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_300: /* [] */
    v5= getFreeSubterm(v1,0);
    switch(getInt(v5)) {
    case 1: /* 1 */
      switch(getSymb(v2)) {
      case code_336: /* , */
        v7= getFreeSubterm(v2,0);
        switch(getSymb(v7)) {
        default:
        label178:
          v8= getFreeSubterm(v2,1);
          switch(getSymb(v8)) {
          default:
          label179:
            bitSet32_set(mask32,0);
            bitSet32_set(mask32,1);
          }
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    default:
      goto label175;
    }
    break;
  default:
  label175:
    switch(getSymb(v2)) {
    case code_336: /* , */
      v7= getFreeSubterm(v2,0);
      switch(getSymb(v7)) {
      default:
      label182:
        v8= getFreeSubterm(v2,1);
        switch(getSymb(v8)) {
        default:
        label183:
          bitSet32_set(mask32,1);
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: -thelem()([](1),,(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v7 ;
    goto end;
    myend37:;
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[3];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: -thelem()(var0,,(var1,var2)) */
    /* allDetEvaluation: det */
    /* where var3 := minus(,)(var0,[](1)) */
    // this=var0        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_300);
    setFreeSubterm(sv[2],0,(setIntegerTag(1)));
    sv[1] = fun_302( v1,sv[2] );
    tmp = sv[0] = sv[1];
    /* rhs: -thelem()(var3,var2) */
    // this=var3        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[2] = fun_339( sv[0],v8 );
    res = sv[2] ;
    goto end;
    myend38:;
  }
match_fail:
  TERM_ALLOC(res,term2, 339);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_340(struct term *v1 ) {
  struct term *v2,*v3,*v4,*v5,*v6;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_336: /* , */
    v4= getFreeSubterm(v1,0);
    switch(getSymb(v4)) {
    default:
    label188:
      v5= getFreeSubterm(v1,1);
      switch(getSymb(v5)) {
      default:
      label189:
        bitSet32_set(mask32,1);
      }
    }
    break;
  case code_335: /* nil */
    bitSet32_set(mask32,0);
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[2];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: size_of_rccst_list()(nil) */
    /* allDetEvaluation: det */
    /* rhs: [](0) */
    TERM_ALLOC(sv[1],term1,code_300);
    setFreeSubterm(sv[1],0,(setIntegerTag(0)));
    res = sv[1] ;
    goto end;
    myend39:;
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[3];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: size_of_rccst_list()(,(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: plus(,)([](1),size_of_rccst_list()(var1)) */
    TERM_ALLOC(sv[1],term1,code_300);
    setFreeSubterm(sv[1],0,(setIntegerTag(1)));
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_340( v5 );
    sv[2] = fun_301( sv[1],sv[0] );
    res = sv[2] ;
    goto end;
    myend40:;
  }
match_fail:
  TERM_ALLOC(res,term1, 340);
  setFreeSubterm(res,0,v1);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_341(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_300: /* [] */
    v5= getFreeSubterm(v1,0);
    switch(getInt(v5)) {
    case 0: /* 0 */
      switch(getSymb(v2)) {
      default:
      label194:
        bitSet32_set(mask32,0);
        bitSet32_set(mask32,1);
      }
      break;
    default:
      goto label192;
    }
    break;
  default:
  label192:
    switch(getSymb(v2)) {
    default:
    label196:
      bitSet32_set(mask32,1);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: ccat(,)([](0),var0) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_335 ;
    goto end;
    myend41:;
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[3];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: ccat(,)(var0,var1) */
    /* allDetEvaluation: det */
    /* if greater_int(,)(var0,[](0)) */
    // this=var0        underAC=false        Instantiated=false
    TERM_ALLOC(sv[1],term1,code_300);
    setFreeSubterm(sv[1],0,(setIntegerTag(0)));
    sv[0] = fun_311( v1,sv[1] );
    if( sv[0] != con_1 ) {
      goto myend42;
    }
    /* rhs: @(var1,ccat(,)(minus(,)(var0,[](1)),var1)) */
    // this=var1        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_300);
    setFreeSubterm(sv[2],0,(setIntegerTag(1)));
    sv[1] = fun_302( v1,sv[2] );
    // this=var1        underAC=false        Instantiated=false
    sv[2] = fun_341( sv[1],v2 );
    sv[1] = fun_337( v2,sv[2] );
    res = sv[1] ;
    goto end;
    myend42:;
  }
match_fail:
  TERM_ALLOC(res,term2, 341);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_343(struct term *v1,struct term *v2,struct term *v3 ) {
  struct term *v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label199:
    switch(getSymb(v2)) {
    default:
    label200:
      switch(getSymb(v3)) {
      default:
      label201:
        bitSet32_set(mask32,0);
      }
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[6];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: .updateHashTable(,)(var0,var1,var2) */
    /* allDetEvaluation: det */
    /* where var4 := .get()(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_321( v1,v2 );
    tmp = sv[0] = sv[1];
    /* where var5 := ,(var2,var4) */
    // this=var2        underAC=false        Instantiated=false
    // this=var4        underAC=false        Instantiated=false
    TERM_ALLOC(sv[3],term2,code_336);
    setFreeSubterm(sv[3],0,v3);
    setFreeSubterm(sv[3],1,sv[0]);
    tmp = sv[2] = sv[3];
    /* where var3 := .add(,)(var0,var1,var5) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var5        underAC=false        Instantiated=false
    sv[5] = fun_320( v1,v2,sv[2] );
    tmp = sv[4] = sv[5];
    /* rhs: var3 */
    // this=var3        underAC=false        Instantiated=false
    res = sv[4] ;
    goto end;
    myend43:;
  }
match_fail:
  TERM_ALLOC(res,term3, 343);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  setFreeSubterm(res,2,v3);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_352(struct term *v1 ) {
  struct term *v2,*v3,*v4;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label204:
    bitSet32_set(mask32,0);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: ()(var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v1 ;
    goto end;
    myend44:;
  }
match_fail:
  TERM_ALLOC(res,term1, 352);
  setFreeSubterm(res,0,v1);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_356(struct term *v1 ) {
  struct term *v2,*v3,*v4,*v5,*v6;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_355: /* (;;) */
    v4= getFreeSubterm(v1,0);
    switch(getSymb(v4)) {
    default:
    label208:
      v5= getFreeSubterm(v1,1);
      switch(getSymb(v5)) {
      default:
      label209:
        bitSet32_set(mask32,0);
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: getLabelCcst()((;;)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var1 */
    // this=var1        underAC=false        Instantiated=false
    res = v5 ;
    goto end;
    myend45:;
  }
match_fail:
  TERM_ALLOC(res,term1, 356);
  setFreeSubterm(res,0,v1);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_357(struct term *v1 ) {
  struct term *v2,*v3,*v4,*v5;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_358: /* (;) */
    v4= getFreeSubterm(v1,0);
    switch(getSymb(v4)) {
    default:
    label213:
      bitSet32_set(mask32,0);
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: getCcst()((;)(var0)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v4 ;
    goto end;
    myend46:;
  }
match_fail:
  TERM_ALLOC(res,term1, 357);
  setFreeSubterm(res,0,v1);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_360(struct term *v1 ) {
  struct term *v2,*v3,*v4,*v5,*v6;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_359: /* | */
    v4= getFreeSubterm(v1,0);
    switch(getSymb(v4)) {
    default:
    label217:
      v5= getFreeSubterm(v1,1);
      switch(getSymb(v5)) {
      default:
      label218:
        bitSet32_set(mask32,0);
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: getRccst()(|(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v4 ;
    goto end;
    myend47:;
  }
match_fail:
  TERM_ALLOC(res,term1, 360);
  setFreeSubterm(res,0,v1);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_361(struct term *v1 ) {
  struct term *v2,*v3,*v4,*v5,*v6;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_359: /* | */
    v4= getFreeSubterm(v1,0);
    switch(getSymb(v4)) {
    default:
    label222:
      v5= getFreeSubterm(v1,1);
      switch(getSymb(v5)) {
      default:
      label223:
        bitSet32_set(mask32,0);
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: getHashTable()(|(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var1 */
    // this=var1        underAC=false        Instantiated=false
    res = v5 ;
    goto end;
    myend48:;
  }
match_fail:
  TERM_ALLOC(res,term1, 361);
  setFreeSubterm(res,0,v1);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_362(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label226:
    switch(getSymb(v2)) {
    default:
    label227:
      bitSet32_set(mask32,0);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[2];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: buildRccst(,)(var0,var1) */
    /* allDetEvaluation: det */
    /* rhs: |((;)(var0),var1) */
    // this=var0        underAC=false        Instantiated=false
    TERM_ALLOC(sv[0],term1,code_358);
    setFreeSubterm(sv[0],0,v1);
    // this=var1        underAC=false        Instantiated=false
    TERM_ALLOC(sv[1],term2,code_359);
    setFreeSubterm(sv[1],0,sv[0]);
    setFreeSubterm(sv[1],1,v2);
    res = sv[1] ;
    goto end;
    myend49:;
  }
match_fail:
  TERM_ALLOC(res,term2, 362);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_364( ) {
  struct term *v1,*v2;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  bitSet32_set(mask32,0);
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[7];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: go */
    /* allDetEvaluation: det */
    /* where var0 := new(,)([](100),nil) */
    TERM_ALLOC(sv[2],term1,code_300);
    setFreeSubterm(sv[2],0,(setIntegerTag(100)));
    sv[3] = fun_318( sv[2],con_335 );
    tmp = sv[0] = sv[3];
    /* rhs: |((;)(#((Knuth),.(tau,.(tau,0)))),var0) */
    TERM_ALLOC(sv[2],term1,code_347);
    setFreeSubterm(sv[2],0,con_365);
    TERM_ALLOC(sv[6],term2,code_351);
    setFreeSubterm(sv[6],0,con_344);
    setFreeSubterm(sv[6],1,con_348);
    TERM_ALLOC(sv[4],term2,code_351);
    setFreeSubterm(sv[4],0,con_344);
    setFreeSubterm(sv[4],1,sv[6]);
    TERM_ALLOC(sv[1],term2,code_349);
    setFreeSubterm(sv[1],0,sv[2]);
    setFreeSubterm(sv[1],1,sv[4]);
    TERM_ALLOC(sv[2],term1,code_358);
    setFreeSubterm(sv[2],0,sv[1]);
    // this=var0        underAC=false        Instantiated=false
    TERM_ALLOC(sv[1],term2,code_359);
    setFreeSubterm(sv[1],0,sv[2]);
    setFreeSubterm(sv[1],1,sv[0]);
    res = sv[1] ;
    goto end;
    myend50:;
  }
match_fail:
  TERM_ALLOC(res,term1,code_364);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_414(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,3);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label232:
    switch(getSymb(v2)) {
    case code_346: /* ' */
      v6= getFreeSubterm(v2,0);
      switch(getSymb(v6)) {
      default:
      label237:
        bitSet32_set(mask32,2);
      }
      break;
    case code_345: /*  */
      v7= getFreeSubterm(v2,0);
      switch(getSymb(v7)) {
      default:
      label235:
        bitSet32_set(mask32,1);
      }
      break;
    case code_344: /* tau */
      bitSet32_set(mask32,0);
      break;
    /* matching is not complete: jumpNode is null */
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrict(,)(var0,tau) */
    /* allDetEvaluation: det */
    /* rhs: true */
    res = con_1 ;
    goto end;
    myend51:;
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrict(,)(var0,(var1)) */
    /* allDetEvaluation: det */
    /* rhs: doesNotRestrictName(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_415( v1,v7 );
    res = sv[0] ;
    goto end;
    myend52:;
  }
  if(bitSet32_get(mask32,2)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrict(,)(var0,'(var1)) */
    /* allDetEvaluation: det */
    /* rhs: doesNotRestrictName(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_415( v1,v6 );
    res = sv[0] ;
    goto end;
    myend53:;
  }
match_fail:
  TERM_ALLOC(res,term2, 414);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_415(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,17);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_391: /* L */
    switch(getSymb(v2)) {
    case code_400: /* c2r2 */
      bitSet32_set(mask32,15);
      bitSet32_set(mask32,16);
      break;
    case code_399: /* c2r1 */
      bitSet32_set(mask32,14);
      bitSet32_set(mask32,16);
      break;
    case code_398: /* c2r0 */
      bitSet32_set(mask32,13);
      bitSet32_set(mask32,16);
      break;
    case code_403: /* c2w2 */
      bitSet32_set(mask32,12);
      bitSet32_set(mask32,16);
      break;
    case code_402: /* c2w1 */
      bitSet32_set(mask32,11);
      bitSet32_set(mask32,16);
      break;
    case code_401: /* c2w0 */
      bitSet32_set(mask32,10);
      bitSet32_set(mask32,16);
      break;
    case code_394: /* c1r2 */
      bitSet32_set(mask32,9);
      bitSet32_set(mask32,16);
      break;
    case code_393: /* c1r1 */
      bitSet32_set(mask32,8);
      bitSet32_set(mask32,16);
      break;
    case code_392: /* c1r0 */
      bitSet32_set(mask32,7);
      bitSet32_set(mask32,16);
      break;
    case code_397: /* c1w2 */
      bitSet32_set(mask32,6);
      bitSet32_set(mask32,16);
      break;
    case code_396: /* c1w1 */
      bitSet32_set(mask32,5);
      bitSet32_set(mask32,16);
      break;
    case code_395: /* c1w0 */
      bitSet32_set(mask32,4);
      bitSet32_set(mask32,16);
      break;
    case code_409: /* kr2 */
      bitSet32_set(mask32,3);
      bitSet32_set(mask32,16);
      break;
    case code_408: /* kr1 */
      bitSet32_set(mask32,2);
      bitSet32_set(mask32,16);
      break;
    case code_411: /* kw2 */
      bitSet32_set(mask32,1);
      bitSet32_set(mask32,16);
      break;
    case code_410: /* kw1 */
      bitSet32_set(mask32,0);
      bitSet32_set(mask32,16);
      break;
    default:
    label241:
      bitSet32_set(mask32,16);
    }
    break;
  default:
  label240:
    switch(getSymb(v2)) {
    default:
    label258:
      bitSet32_set(mask32,16);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrictName(,)(L,kw1) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend54:;
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrictName(,)(L,kw2) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend55:;
  }
  if(bitSet32_get(mask32,2)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrictName(,)(L,kr1) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend56:;
  }
  if(bitSet32_get(mask32,3)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrictName(,)(L,kr2) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend57:;
  }
  if(bitSet32_get(mask32,4)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrictName(,)(L,c1w0) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend58:;
  }
  if(bitSet32_get(mask32,5)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrictName(,)(L,c1w1) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend59:;
  }
  if(bitSet32_get(mask32,6)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrictName(,)(L,c1w2) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend60:;
  }
  if(bitSet32_get(mask32,7)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrictName(,)(L,c1r0) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend61:;
  }
  if(bitSet32_get(mask32,8)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrictName(,)(L,c1r1) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend62:;
  }
  if(bitSet32_get(mask32,9)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrictName(,)(L,c1r2) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend63:;
  }
  if(bitSet32_get(mask32,10)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrictName(,)(L,c2w0) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend64:;
  }
  if(bitSet32_get(mask32,11)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrictName(,)(L,c2w1) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend65:;
  }
  if(bitSet32_get(mask32,12)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrictName(,)(L,c2w2) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend66:;
  }
  if(bitSet32_get(mask32,13)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrictName(,)(L,c2r0) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend67:;
  }
  if(bitSet32_get(mask32,14)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrictName(,)(L,c2r1) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend68:;
  }
  if(bitSet32_get(mask32,15)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrictName(,)(L,c2r2) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend69:;
  }
  if(bitSet32_get(mask32,16)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: doesNotRestrictName(,)(var0,var1) */
    /* allDetEvaluation: det */
    /* rhs: true */
    res = con_1 ;
    goto end;
    myend70:;
  }
match_fail:
  TERM_ALLOC(res,term2, 415);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_416(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_346: /* ' */
    v5= getFreeSubterm(v1,0);
    switch(getSymb(v5)) {
    default:
    label267:
      switch(getSymb(v2)) {
      case code_345: /*  */
        v7= getFreeSubterm(v2,0);
        switch(getSymb(v7)) {
        default:
        label269:
          bitSet32_set(mask32,1);
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
    break;
  case code_345: /*  */
    v8= getFreeSubterm(v1,0);
    switch(getSymb(v8)) {
    default:
    label263:
      switch(getSymb(v2)) {
      case code_346: /* ' */
        v10= getFreeSubterm(v2,0);
        switch(getSymb(v10)) {
        default:
        label265:
          bitSet32_set(mask32,0);
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[2];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: isInverseLabel(,)((var0),'(var1)) */
    /* allDetEvaluation: det */
    /* if eq_identifier(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_18( v8,v10 );
    if( sv[0] != con_1 ) {
      goto myend71;
    }
    /* rhs: true */
    res = con_1 ;
    goto end;
    myend71:;
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[2];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: isInverseLabel(,)('(var0),(var1)) */
    /* allDetEvaluation: det */
    /* if eq_identifier(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_18( v5,v7 );
    if( sv[0] != con_1 ) {
      goto myend72;
    }
    /* rhs: true */
    res = con_1 ;
    goto end;
    myend72:;
  }
match_fail:
  TERM_ALLOC(res,term2, 416);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_417(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,3);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label272:
    switch(getSymb(v2)) {
    case code_346: /* ' */
      v6= getFreeSubterm(v2,0);
      switch(getSymb(v6)) {
      default:
      label277:
        bitSet32_set(mask32,2);
      }
      break;
    case code_345: /*  */
      v7= getFreeSubterm(v2,0);
      switch(getSymb(v7)) {
      default:
      label275:
        bitSet32_set(mask32,1);
      }
      break;
    case code_344: /* tau */
      bitSet32_set(mask32,0);
      break;
    /* matching is not complete: jumpNode is null */
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: applyon(var0,tau) */
    /* allDetEvaluation: det */
    /* rhs: tau */
    res = con_344 ;
    goto end;
    myend73:;
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[2];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: applyon(var0,(var1)) */
    /* allDetEvaluation: det */
    /* rhs: (applon(var0,var1)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_418( v1,v7 );
    TERM_ALLOC(sv[1],term1,code_345);
    setFreeSubterm(sv[1],0,sv[0]);
    res = sv[1] ;
    goto end;
    myend74:;
  }
  if(bitSet32_get(mask32,2)) {
    struct term *tmp, *sv[2];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: applyon(var0,'(var1)) */
    /* allDetEvaluation: det */
    /* rhs: '(applon(var0,var1)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_418( v1,v6 );
    TERM_ALLOC(sv[1],term1,code_346);
    setFreeSubterm(sv[1],0,sv[0]);
    res = sv[1] ;
    goto end;
    myend75:;
  }
match_fail:
  TERM_ALLOC(res,term2, 417);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_418(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label280:
    switch(getSymb(v2)) {
    default:
    label281:
      bitSet32_set(mask32,0);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    multiplicityType *E,*sol;
    struct term *substitution[1];
    /* lhs: applon(var0,var1) */
    /* allDetEvaluation: det */
    /* rhs: var1 */
    // this=var1        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend76:;
  }
match_fail:
  TERM_ALLOC(res,term2, 418);
  setFreeSubterm(res,0,v1);
  setFreeSubterm(res,1,v2);
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  bitSet32_stack_delete(mask32);
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_472( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  {
    /* DC[nonDet](dk[nonDet](checkMemo:rccst/memo3!GL),cons[nonDet](dk[nonDet](act:rccst/memo3!GL,plus1:rccst/memo3!GL,plus2:rccst/memo3!GL,par1:rccst/memo3!GL,par2:rccst/memo3!GL,par3:rccst/memo3!GL,res:rccst/memo3!GL,rel:rccst/memo3!GL,proc:rccst/memo3!GL),dk[semiDet](updateMemo:rccst/memo3!GL))) */
    int *wasr=(int*) allocStable(sizeof(int));
    *wasr=0;
    if(!setChoicePoint()) {
      /* Si la strategie suivante echoue, on passe a la suivante */
      {
        /* dk[nonDet](checkMemo:rccst/memo3!GL) */
        struct term *v1,*v2,*v3,*v4;
        bitSet_GC_create(mask,1);
        bitSet_init_clear(mask);
        switch(getSymb(v0)) {
        case code_359: /* | */
          v2= getFreeSubterm(v0,0);
          switch(getSymb(v2)) {
          default:
          label284:
            v3= getFreeSubterm(v0,1);
            switch(getSymb(v3)) {
            default:
            label285:
              bitSet_set(mask,0);
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
        if(bitSet_get(mask,0)) {
          struct term *tmp, *sv[6];
          multiplicityType *E,*sol;
          struct term *substitution[1];
          /* lhs: |(var0,var1) */
          long tmp_step;
          /* allDetEvaluation: nonDet */
          if(localSetChoicePoint()) {
            /* local evaluations failed, try next rule */
            goto myend77;
          }
          /* where var3 := .get()(var1,var0) */
          // this=var1        underAC=false        Instantiated=false
          // this=var0        underAC=false        Instantiated=false
          sv[1] = fun_321( v3,v2 );
          tmp = sv[0] = sv[1];
          /* if neq_identifier(,)(var3,nil) */
          // this=var3        underAC=false        Instantiated=false
          sv[3] = fun_19( sv[0],con_335 );
          if( sv[3] != con_1 ) {
            fail();
          }
          /* where var2 := (listExtract:rccst/list[rccst]) elem()(var3) */
          // this=var3        underAC=false        Instantiated=false
          TERM_ALLOC(sv[4],term1,code_338);
          setFreeSubterm(sv[4],0,sv[0]);
          sv[2] = strTab[494]( sv[4] );
          /* rhs: |(var2,var1) */
          // this=var2        underAC=false        Instantiated=false
          // this=var1        underAC=false        Instantiated=false
          TERM_ALLOC(sv[5],term2,code_359);
          setFreeSubterm(sv[5],0,sv[2]);
          setFreeSubterm(sv[5],1,v3);
          res = sv[5] ;
          rewrite_step++;
          rewrite_label_step++;
          goto stratLab4;
          myend77:;
        }
      }
      fail();
      stratLab4:;
      v0=res;
      /* La strategie a donne un resultat */
      if(*wasr==0) {
        *wasr=1;
      }
      goto stratLab18;
    }
    /* On vient d'un fail */
    if(*wasr!=0) {
      /* Si on a un resultat on propage le fail */
      fail();
    }
    /* Sinon on essai la strategie suivante */
    /* cons[nonDet](dk[nonDet](act:rccst/memo3!GL,plus1:rccst/memo3!GL,plus2:rccst/memo3!GL,par1:rccst/memo3!GL,par2:rccst/memo3!GL,par3:rccst/memo3!GL,res:rccst/memo3!GL,rel:rccst/memo3!GL,proc:rccst/memo3!GL),dk[semiDet](updateMemo:rccst/memo3!GL)) */
    {
      /* dk[nonDet](act:rccst/memo3!GL,plus1:rccst/memo3!GL,plus2:rccst/memo3!GL,par1:rccst/memo3!GL,par2:rccst/memo3!GL,par3:rccst/memo3!GL,res:rccst/memo3!GL,rel:rccst/memo3!GL,proc:rccst/memo3!GL) */
      struct term *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16,*v17,*v18,*v19,*v20,*v21,*v22,*v23,*v24,*v25,*v26,*v27,*v28,*v29,*v30,*v31,*v32,*v33,*v34,*v35,*v36,*v37,*v38,*v39,*v40,*v41,*v42,*v43,*v44,*v45,*v46;
      bitSet_GC_create(mask,34);
      bitSet_init_clear(mask);
      switch(getSymb(v0)) {
      case code_359: /* | */
        v2= getFreeSubterm(v0,0);
        switch(getSymb(v2)) {
        case code_358: /* (;) */
          v3= getFreeSubterm(v2,0);
          switch(getSymb(v3)) {
          case code_347: /*  */
            v4= getFreeSubterm(v3,0);
            switch(getSymb(v4)) {
            case code_390: /* KBig */
              v5= getFreeSubterm(v0,1);
              switch(getSymb(v5)) {
              default:
              label361:
                bitSet_set(mask,33);
              }
              break;
            case code_389: /* P27 */
              v6= getFreeSubterm(v0,1);
              switch(getSymb(v6)) {
              default:
              label359:
                bitSet_set(mask,32);
              }
              break;
            case code_388: /* P26 */
              v7= getFreeSubterm(v0,1);
              switch(getSymb(v7)) {
              default:
              label357:
                bitSet_set(mask,31);
              }
              break;
            case code_387: /* P25 */
              v8= getFreeSubterm(v0,1);
              switch(getSymb(v8)) {
              default:
              label355:
                bitSet_set(mask,30);
              }
              break;
            case code_386: /* P24 */
              v9= getFreeSubterm(v0,1);
              switch(getSymb(v9)) {
              default:
              label353:
                bitSet_set(mask,29);
              }
              break;
            case code_385: /* P23 */
              v10= getFreeSubterm(v0,1);
              switch(getSymb(v10)) {
              default:
              label351:
                bitSet_set(mask,28);
              }
              break;
            case code_384: /* P22 */
              v11= getFreeSubterm(v0,1);
              switch(getSymb(v11)) {
              default:
              label349:
                bitSet_set(mask,27);
              }
              break;
            case code_383: /* P21 */
              v12= getFreeSubterm(v0,1);
              switch(getSymb(v12)) {
              default:
              label347:
                bitSet_set(mask,26);
              }
              break;
            case code_382: /* P2 */
              v13= getFreeSubterm(v0,1);
              switch(getSymb(v13)) {
              default:
              label345:
                bitSet_set(mask,25);
              }
              break;
            case code_381: /* P17 */
              v14= getFreeSubterm(v0,1);
              switch(getSymb(v14)) {
              default:
              label343:
                bitSet_set(mask,24);
              }
              break;
            case code_380: /* P16 */
              v15= getFreeSubterm(v0,1);
              switch(getSymb(v15)) {
              default:
              label341:
                bitSet_set(mask,23);
              }
              break;
            case code_379: /* P15 */
              v16= getFreeSubterm(v0,1);
              switch(getSymb(v16)) {
              default:
              label339:
                bitSet_set(mask,22);
              }
              break;
            case code_378: /* P14 */
              v17= getFreeSubterm(v0,1);
              switch(getSymb(v17)) {
              default:
              label337:
                bitSet_set(mask,21);
              }
              break;
            case code_377: /* P13 */
              v18= getFreeSubterm(v0,1);
              switch(getSymb(v18)) {
              default:
              label335:
                bitSet_set(mask,20);
              }
              break;
            case code_376: /* P12 */
              v19= getFreeSubterm(v0,1);
              switch(getSymb(v19)) {
              default:
              label333:
                bitSet_set(mask,19);
              }
              break;
            case code_375: /* P11 */
              v20= getFreeSubterm(v0,1);
              switch(getSymb(v20)) {
              default:
              label331:
                bitSet_set(mask,18);
              }
              break;
            case code_374: /* P1 */
              v21= getFreeSubterm(v0,1);
              switch(getSymb(v21)) {
              default:
              label329:
                bitSet_set(mask,17);
              }
              break;
            case code_373: /* C2_2 */
              v22= getFreeSubterm(v0,1);
              switch(getSymb(v22)) {
              default:
              label327:
                bitSet_set(mask,16);
              }
              break;
            case code_372: /* C2_1 */
              v23= getFreeSubterm(v0,1);
              switch(getSymb(v23)) {
              default:
              label325:
                bitSet_set(mask,15);
              }
              break;
            case code_371: /* C2_0 */
              v24= getFreeSubterm(v0,1);
              switch(getSymb(v24)) {
              default:
              label323:
                bitSet_set(mask,14);
              }
              break;
            case code_370: /* C1_2 */
              v25= getFreeSubterm(v0,1);
              switch(getSymb(v25)) {
              default:
              label321:
                bitSet_set(mask,13);
              }
              break;
            case code_369: /* C1_1 */
              v26= getFreeSubterm(v0,1);
              switch(getSymb(v26)) {
              default:
              label319:
                bitSet_set(mask,12);
              }
              break;
            case code_368: /* C1_0 */
              v27= getFreeSubterm(v0,1);
              switch(getSymb(v27)) {
              default:
              label317:
                bitSet_set(mask,11);
              }
              break;
            case code_367: /* K2 */
              v28= getFreeSubterm(v0,1);
              switch(getSymb(v28)) {
              default:
              label315:
                bitSet_set(mask,10);
              }
              break;
            case code_366: /* K1 */
              v29= getFreeSubterm(v0,1);
              switch(getSymb(v29)) {
              default:
              label313:
                bitSet_set(mask,9);
              }
              break;
            case code_365: /* Knuth */
              v30= getFreeSubterm(v0,1);
              switch(getSymb(v30)) {
              default:
              label311:
                bitSet_set(mask,8);
              }
              break;
            /* matching is not complete: jumpNode is null */
            }
            break;
          case code_354: /* [] */
            v31= getFreeSubterm(v3,0);
            switch(getSymb(v31)) {
            default:
            label306:
              v32= getFreeSubterm(v3,1);
              switch(getSymb(v32)) {
              default:
              label307:
                v33= getFreeSubterm(v0,1);
                switch(getSymb(v33)) {
                default:
                label308:
                  bitSet_set(mask,7);
                }
              }
            }
            break;
          case code_353: /* \\ */
            v34= getFreeSubterm(v3,0);
            switch(getSymb(v34)) {
            default:
            label302:
              v35= getFreeSubterm(v3,1);
              switch(getSymb(v35)) {
              default:
              label303:
                v36= getFreeSubterm(v0,1);
                switch(getSymb(v36)) {
                default:
                label304:
                  bitSet_set(mask,6);
                }
              }
            }
            break;
          case code_349: /* # */
            v37= getFreeSubterm(v3,0);
            switch(getSymb(v37)) {
            default:
            label298:
              v38= getFreeSubterm(v3,1);
              switch(getSymb(v38)) {
              default:
              label299:
                v39= getFreeSubterm(v0,1);
                switch(getSymb(v39)) {
                default:
                label300:
                  bitSet_set(mask,3);
                  bitSet_set(mask,4);
                  bitSet_set(mask,5);
                }
              }
            }
            break;
          case code_350: /* + */
            v40= getFreeSubterm(v3,0);
            switch(getSymb(v40)) {
            default:
            label294:
              v41= getFreeSubterm(v3,1);
              switch(getSymb(v41)) {
              default:
              label295:
                v42= getFreeSubterm(v0,1);
                switch(getSymb(v42)) {
                default:
                label296:
                  bitSet_set(mask,1);
                  bitSet_set(mask,2);
                }
              }
            }
            break;
          case code_351: /* . */
            v43= getFreeSubterm(v3,0);
            switch(getSymb(v43)) {
            default:
            label290:
              v44= getFreeSubterm(v3,1);
              switch(getSymb(v44)) {
              default:
              label291:
                v45= getFreeSubterm(v0,1);
                switch(getSymb(v45)) {
                default:
                label292:
                  bitSet_set(mask,0);
                }
              }
            }
            break;
          /* matching is not complete: jumpNode is null */
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
      if(bitSet_get(mask,0)) {
        struct term *tmp, *sv[3];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)(.(var0,var1)),var2) */
        long tmp_step;
        /* allDetEvaluation: det */
        if(setChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend78;
        }
        /* rhs: |(-->((;)(.(var0,var1)),(;;)(var0,var1)),var2) */
        // this=var0        underAC=false        Instantiated=false
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[0],term2,code_351);
        setFreeSubterm(sv[0],0,v43);
        setFreeSubterm(sv[0],1,v44);
        TERM_ALLOC(sv[1],term1,code_358);
        setFreeSubterm(sv[1],0,sv[0]);
        // this=var0        underAC=false        Instantiated=false
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[0],term2,code_355);
        setFreeSubterm(sv[0],0,v43);
        setFreeSubterm(sv[0],1,v44);
        TERM_ALLOC(sv[2],term2,code_363);
        setFreeSubterm(sv[2],0,sv[1]);
        setFreeSubterm(sv[2],1,sv[0]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[0],term2,code_359);
        setFreeSubterm(sv[0],0,sv[2]);
        setFreeSubterm(sv[0],1,v45);
        res = sv[0] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend78:;
      }
      if(bitSet_get(mask,1)) {
        struct term *tmp, *sv[6];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)(+(var0,var1)),var2) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend79;
        }
        /* where |(var3,var4) := (calc_str:rccst/memo3) |((;)(var0),var2) */
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,v40);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v42);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)(+(var0,var1)),var3),var4) */
        // this=var0        underAC=false        Instantiated=false
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_350);
        setFreeSubterm(sv[3],0,v40);
        setFreeSubterm(sv[3],1,v41);
        TERM_ALLOC(sv[5],term1,code_358);
        setFreeSubterm(sv[5],0,sv[3]);
        // this=var3        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_363);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[1]);
        // this=var4        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_359);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[0]);
        res = sv[5] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend79:;
      }
      if(bitSet_get(mask,2)) {
        struct term *tmp, *sv[6];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)(+(var0,var1)),var2) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend80;
        }
        /* where |(var3,var4) := (calc_str:rccst/memo3) |((;)(var1),var2) */
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,v41);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v42);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)(+(var0,var1)),var3),var4) */
        // this=var0        underAC=false        Instantiated=false
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_350);
        setFreeSubterm(sv[3],0,v40);
        setFreeSubterm(sv[3],1,v41);
        TERM_ALLOC(sv[5],term1,code_358);
        setFreeSubterm(sv[5],0,sv[3]);
        // this=var3        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_363);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[1]);
        // this=var4        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_359);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[0]);
        res = sv[5] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend80:;
      }
      if(bitSet_get(mask,3)) {
        struct term *tmp, *sv[9];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)(#(var0,var1)),var2) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend81;
        }
        /* where |((;;)(var3,var4),var5) := (calc_str:rccst/memo3) |((;)(var0),var2) */
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term1,code_358);
        setFreeSubterm(sv[5],0,v37);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[6],term2,code_359);
        setFreeSubterm(sv[6],0,sv[5]);
        setFreeSubterm(sv[6],1,v39);
        sv[4] = strTab[472]( sv[6] );
        if(code_359 != getSymb(sv[4])) {
          fail();
        } else {
          sv[3] = getFreeSubterm(sv[4],0);
          if(code_355 != getSymb(sv[3])) {
            fail();
          } else {
            sv[2] = getFreeSubterm(sv[3],0);
            sv[1] = getFreeSubterm(sv[3],1);
          }
          sv[0] = getFreeSubterm(sv[4],1);
        }
        /* rhs: |(-->((;)(#(var0,var1)),(;;)(var3,#(var4,var1))),var5) */
        // this=var0        underAC=false        Instantiated=false
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_349);
        setFreeSubterm(sv[5],0,v37);
        setFreeSubterm(sv[5],1,v38);
        TERM_ALLOC(sv[7],term1,code_358);
        setFreeSubterm(sv[7],0,sv[5]);
        // this=var3        underAC=false        Instantiated=false
        // this=var4        underAC=false        Instantiated=false
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_349);
        setFreeSubterm(sv[5],0,sv[1]);
        setFreeSubterm(sv[5],1,v38);
        TERM_ALLOC(sv[8],term2,code_355);
        setFreeSubterm(sv[8],0,sv[2]);
        setFreeSubterm(sv[8],1,sv[5]);
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[7]);
        setFreeSubterm(sv[5],1,sv[8]);
        // this=var5        underAC=false        Instantiated=false
        TERM_ALLOC(sv[7],term2,code_359);
        setFreeSubterm(sv[7],0,sv[5]);
        setFreeSubterm(sv[7],1,sv[0]);
        res = sv[7] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend81:;
      }
      if(bitSet_get(mask,4)) {
        struct term *tmp, *sv[9];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)(#(var0,var1)),var2) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend82;
        }
        /* where |((;;)(var3,var4),var5) := (calc_str:rccst/memo3) |((;)(var1),var2) */
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term1,code_358);
        setFreeSubterm(sv[5],0,v38);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[6],term2,code_359);
        setFreeSubterm(sv[6],0,sv[5]);
        setFreeSubterm(sv[6],1,v39);
        sv[4] = strTab[472]( sv[6] );
        if(code_359 != getSymb(sv[4])) {
          fail();
        } else {
          sv[3] = getFreeSubterm(sv[4],0);
          if(code_355 != getSymb(sv[3])) {
            fail();
          } else {
            sv[2] = getFreeSubterm(sv[3],0);
            sv[1] = getFreeSubterm(sv[3],1);
          }
          sv[0] = getFreeSubterm(sv[4],1);
        }
        /* rhs: |(-->((;)(#(var0,var1)),(;;)(var3,#(var0,var4))),var5) */
        // this=var0        underAC=false        Instantiated=false
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_349);
        setFreeSubterm(sv[5],0,v37);
        setFreeSubterm(sv[5],1,v38);
        TERM_ALLOC(sv[7],term1,code_358);
        setFreeSubterm(sv[7],0,sv[5]);
        // this=var3        underAC=false        Instantiated=false
        // this=var0        underAC=false        Instantiated=false
        // this=var4        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_349);
        setFreeSubterm(sv[5],0,v37);
        setFreeSubterm(sv[5],1,sv[1]);
        TERM_ALLOC(sv[8],term2,code_355);
        setFreeSubterm(sv[8],0,sv[2]);
        setFreeSubterm(sv[8],1,sv[5]);
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[7]);
        setFreeSubterm(sv[5],1,sv[8]);
        // this=var5        underAC=false        Instantiated=false
        TERM_ALLOC(sv[7],term2,code_359);
        setFreeSubterm(sv[7],0,sv[5]);
        setFreeSubterm(sv[7],1,sv[0]);
        res = sv[7] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend82:;
      }
      if(bitSet_get(mask,5)) {
        struct term *tmp, *sv[17];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)(#(var0,var1)),var2) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend83;
        }
        /* where |((;;)(var6,var3),var7) := (calc_str:rccst/memo3) |((;)(var0),var2) */
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term1,code_358);
        setFreeSubterm(sv[5],0,v37);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[6],term2,code_359);
        setFreeSubterm(sv[6],0,sv[5]);
        setFreeSubterm(sv[6],1,v39);
        sv[4] = strTab[472]( sv[6] );
        if(code_359 != getSymb(sv[4])) {
          fail();
        } else {
          sv[3] = getFreeSubterm(sv[4],0);
          if(code_355 != getSymb(sv[3])) {
            fail();
          } else {
            sv[2] = getFreeSubterm(sv[3],0);
            sv[1] = getFreeSubterm(sv[3],1);
          }
          sv[0] = getFreeSubterm(sv[4],1);
        }
        /* where |((;;)(var8,var4),var5) := (calc_str:rccst/memo3) |((;)(var1),var7) */
        // this=var1        underAC=false        Instantiated=false
        term_alloc(&sv[11],sizeof(struct term1),code_358);
        setFreeSubterm(sv[11],0,v38);
        // this=var7        underAC=false        Instantiated=false
        term_alloc(&sv[12],sizeof(struct term2),code_359);
        setFreeSubterm(sv[12],0,sv[11]);
        setFreeSubterm(sv[12],1,sv[0]);
        sv[10] = strTab[472]( sv[12] );
        if(code_359 != getSymb(sv[10])) {
          fail();
        } else {
          sv[9] = getFreeSubterm(sv[10],0);
          if(code_355 != getSymb(sv[9])) {
            fail();
          } else {
            sv[8] = getFreeSubterm(sv[9],0);
            sv[7] = getFreeSubterm(sv[9],1);
          }
          sv[5] = getFreeSubterm(sv[10],1);
        }
        /* if isInverseLabel(,)(var6,var8) */
        // this=var6        underAC=false        Instantiated=false
        // this=var8        underAC=false        Instantiated=false
        sv[11] = fun_416( sv[2],sv[8] );
        if( sv[11] != con_1 ) {
          fail();
        }
        /* rhs: |(-->((;)(#(var0,var1)),(;;)(tau,#(var3,var4))),var5) */
        // this=var0        underAC=false        Instantiated=false
        // this=var1        underAC=false        Instantiated=false
        term_alloc(&sv[13],sizeof(struct term2),code_349);
        setFreeSubterm(sv[13],0,v37);
        setFreeSubterm(sv[13],1,v38);
        term_alloc(&sv[14],sizeof(struct term1),code_358);
        setFreeSubterm(sv[14],0,sv[13]);
        // this=var3        underAC=false        Instantiated=false
        // this=var4        underAC=false        Instantiated=false
        term_alloc(&sv[15],sizeof(struct term2),code_349);
        setFreeSubterm(sv[15],0,sv[1]);
        setFreeSubterm(sv[15],1,sv[7]);
        term_alloc(&sv[16],sizeof(struct term2),code_355);
        setFreeSubterm(sv[16],0,con_344);
        setFreeSubterm(sv[16],1,sv[15]);
        term_alloc(&sv[13],sizeof(struct term2),code_363);
        setFreeSubterm(sv[13],0,sv[14]);
        setFreeSubterm(sv[13],1,sv[16]);
        // this=var5        underAC=false        Instantiated=false
        term_alloc(&sv[14],sizeof(struct term2),code_359);
        setFreeSubterm(sv[14],0,sv[13]);
        setFreeSubterm(sv[14],1,sv[5]);
        res = sv[14] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend83:;
      }
      if(bitSet_get(mask,6)) {
        struct term *tmp, *sv[10];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)(\\(var0,var1)),var2) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend84;
        }
        /* where |((;;)(var3,var4),var5) := (calc_str:rccst/memo3) |((;)(var0),var2) */
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term1,code_358);
        setFreeSubterm(sv[5],0,v34);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[6],term2,code_359);
        setFreeSubterm(sv[6],0,sv[5]);
        setFreeSubterm(sv[6],1,v36);
        sv[4] = strTab[472]( sv[6] );
        if(code_359 != getSymb(sv[4])) {
          fail();
        } else {
          sv[3] = getFreeSubterm(sv[4],0);
          if(code_355 != getSymb(sv[3])) {
            fail();
          } else {
            sv[2] = getFreeSubterm(sv[3],0);
            sv[1] = getFreeSubterm(sv[3],1);
          }
          sv[0] = getFreeSubterm(sv[4],1);
        }
        /* if doesNotRestrict(,)(var1,var3) */
        // this=var1        underAC=false        Instantiated=false
        // this=var3        underAC=false        Instantiated=false
        sv[5] = fun_414( v35,sv[2] );
        if( sv[5] != con_1 ) {
          fail();
        }
        /* rhs: |(-->((;)(\\(var0,var1)),(;;)(var3,\\(var4,var1))),var5) */
        // this=var0        underAC=false        Instantiated=false
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[7],term2,code_353);
        setFreeSubterm(sv[7],0,v34);
        setFreeSubterm(sv[7],1,v35);
        TERM_ALLOC(sv[8],term1,code_358);
        setFreeSubterm(sv[8],0,sv[7]);
        // this=var3        underAC=false        Instantiated=false
        // this=var4        underAC=false        Instantiated=false
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[7],term2,code_353);
        setFreeSubterm(sv[7],0,sv[1]);
        setFreeSubterm(sv[7],1,v35);
        TERM_ALLOC(sv[9],term2,code_355);
        setFreeSubterm(sv[9],0,sv[2]);
        setFreeSubterm(sv[9],1,sv[7]);
        TERM_ALLOC(sv[7],term2,code_363);
        setFreeSubterm(sv[7],0,sv[8]);
        setFreeSubterm(sv[7],1,sv[9]);
        // this=var5        underAC=false        Instantiated=false
        TERM_ALLOC(sv[8],term2,code_359);
        setFreeSubterm(sv[8],0,sv[7]);
        setFreeSubterm(sv[8],1,sv[0]);
        res = sv[8] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend84:;
      }
      if(bitSet_get(mask,7)) {
        struct term *tmp, *sv[11];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)([](var0,var1)),var2) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend85;
        }
        /* where |((;;)(var6,var4),var5) := (calc_str:rccst/memo3) |((;)(var0),var2) */
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term1,code_358);
        setFreeSubterm(sv[5],0,v31);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[6],term2,code_359);
        setFreeSubterm(sv[6],0,sv[5]);
        setFreeSubterm(sv[6],1,v33);
        sv[4] = strTab[472]( sv[6] );
        if(code_359 != getSymb(sv[4])) {
          fail();
        } else {
          sv[3] = getFreeSubterm(sv[4],0);
          if(code_355 != getSymb(sv[3])) {
            fail();
          } else {
            sv[2] = getFreeSubterm(sv[3],0);
            sv[1] = getFreeSubterm(sv[3],1);
          }
          sv[0] = getFreeSubterm(sv[4],1);
        }
        /* where var3 := applyon(var1,var6) */
        // this=var1        underAC=false        Instantiated=false
        // this=var6        underAC=false        Instantiated=false
        sv[7] = fun_417( v32,sv[2] );
        tmp = sv[5] = sv[7];
        /* rhs: |(-->((;)([](var0,var1)),(;;)(var3,[](var4,var1))),var5) */
        // this=var0        underAC=false        Instantiated=false
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[8],term2,code_354);
        setFreeSubterm(sv[8],0,v31);
        setFreeSubterm(sv[8],1,v32);
        TERM_ALLOC(sv[9],term1,code_358);
        setFreeSubterm(sv[9],0,sv[8]);
        // this=var3        underAC=false        Instantiated=false
        // this=var4        underAC=false        Instantiated=false
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[8],term2,code_354);
        setFreeSubterm(sv[8],0,sv[1]);
        setFreeSubterm(sv[8],1,v32);
        term_alloc(&sv[10],sizeof(struct term2),code_355);
        setFreeSubterm(sv[10],0,sv[5]);
        setFreeSubterm(sv[10],1,sv[8]);
        TERM_ALLOC(sv[8],term2,code_363);
        setFreeSubterm(sv[8],0,sv[9]);
        setFreeSubterm(sv[8],1,sv[10]);
        // this=var5        underAC=false        Instantiated=false
        TERM_ALLOC(sv[9],term2,code_359);
        setFreeSubterm(sv[9],0,sv[8]);
        setFreeSubterm(sv[9],1,sv[0]);
        res = sv[9] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend85:;
      }
      if(bitSet_get(mask,8)) {
        struct term *tmp, *sv[9];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((Knuth)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend86;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(\\(()(#((P1),#((P2),#((K1),#((C1_0),(C2_0)))))),L)),var0) */
        TERM_ALLOC(sv[4],term1,code_347);
        setFreeSubterm(sv[4],0,con_374);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_382);
        TERM_ALLOC(sv[6],term1,code_347);
        setFreeSubterm(sv[6],0,con_366);
        TERM_ALLOC(sv[7],term1,code_347);
        setFreeSubterm(sv[7],0,con_368);
        TERM_ALLOC(sv[8],term1,code_347);
        setFreeSubterm(sv[8],0,con_371);
        TERM_ALLOC(sv[3],term2,code_349);
        setFreeSubterm(sv[3],0,sv[7]);
        setFreeSubterm(sv[3],1,sv[8]);
        TERM_ALLOC(sv[7],term2,code_349);
        setFreeSubterm(sv[7],0,sv[6]);
        setFreeSubterm(sv[7],1,sv[3]);
        TERM_ALLOC(sv[3],term2,code_349);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[7]);
        TERM_ALLOC(sv[5],term2,code_349);
        setFreeSubterm(sv[5],0,sv[4]);
        setFreeSubterm(sv[5],1,sv[3]);
        sv[3] = fun_352( sv[5] );
        TERM_ALLOC(sv[5],term2,code_353);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,con_391);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v30);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((Knuth)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_365);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[0]);
        res = sv[3] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend86:;
      }
      if(bitSet_get(mask,9)) {
        struct term *tmp, *sv[8];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((K1)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend87;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(+(.((kw1),(K1)),+(.((kw2),(K2)),.('(kr1),(K1))))),var0) */
        TERM_ALLOC(sv[4],term1,code_345);
        setFreeSubterm(sv[4],0,con_410);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_366);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[5]);
        TERM_ALLOC(sv[5],term1,code_345);
        setFreeSubterm(sv[5],0,con_411);
        TERM_ALLOC(sv[6],term1,code_347);
        setFreeSubterm(sv[6],0,con_367);
        TERM_ALLOC(sv[4],term2,code_351);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[6],term1,code_346);
        setFreeSubterm(sv[6],0,con_408);
        TERM_ALLOC(sv[7],term1,code_347);
        setFreeSubterm(sv[7],0,con_366);
        TERM_ALLOC(sv[5],term2,code_351);
        setFreeSubterm(sv[5],0,sv[6]);
        setFreeSubterm(sv[5],1,sv[7]);
        TERM_ALLOC(sv[6],term2,code_350);
        setFreeSubterm(sv[6],0,sv[4]);
        setFreeSubterm(sv[6],1,sv[5]);
        TERM_ALLOC(sv[4],term2,code_350);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[4]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v29);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((K1)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_366);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[0]);
        res = sv[3] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend87:;
      }
      if(bitSet_get(mask,10)) {
        struct term *tmp, *sv[8];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((K2)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend88;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(+(.((kw1),(K1)),+(.((kw2),(K2)),.('(kr2),(K2))))),var0) */
        TERM_ALLOC(sv[4],term1,code_345);
        setFreeSubterm(sv[4],0,con_410);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_366);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[5]);
        TERM_ALLOC(sv[5],term1,code_345);
        setFreeSubterm(sv[5],0,con_411);
        TERM_ALLOC(sv[6],term1,code_347);
        setFreeSubterm(sv[6],0,con_367);
        TERM_ALLOC(sv[4],term2,code_351);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[6],term1,code_346);
        setFreeSubterm(sv[6],0,con_409);
        TERM_ALLOC(sv[7],term1,code_347);
        setFreeSubterm(sv[7],0,con_367);
        TERM_ALLOC(sv[5],term2,code_351);
        setFreeSubterm(sv[5],0,sv[6]);
        setFreeSubterm(sv[5],1,sv[7]);
        TERM_ALLOC(sv[6],term2,code_350);
        setFreeSubterm(sv[6],0,sv[4]);
        setFreeSubterm(sv[6],1,sv[5]);
        TERM_ALLOC(sv[4],term2,code_350);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[4]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v28);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((K2)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_367);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[0]);
        res = sv[3] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend88:;
      }
      if(bitSet_get(mask,11)) {
        struct term *tmp, *sv[9];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((C1_0)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend89;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(+(.((c1w0),(C1_0)),+(.((c1w1),(C1_1)),+(.((c1w2),(C1_2)),.('(c1r0),(C1_0)))))),var0) */
        TERM_ALLOC(sv[4],term1,code_345);
        setFreeSubterm(sv[4],0,con_395);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_368);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[5]);
        TERM_ALLOC(sv[5],term1,code_345);
        setFreeSubterm(sv[5],0,con_396);
        TERM_ALLOC(sv[6],term1,code_347);
        setFreeSubterm(sv[6],0,con_369);
        TERM_ALLOC(sv[4],term2,code_351);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[6],term1,code_345);
        setFreeSubterm(sv[6],0,con_397);
        TERM_ALLOC(sv[7],term1,code_347);
        setFreeSubterm(sv[7],0,con_370);
        TERM_ALLOC(sv[5],term2,code_351);
        setFreeSubterm(sv[5],0,sv[6]);
        setFreeSubterm(sv[5],1,sv[7]);
        TERM_ALLOC(sv[7],term1,code_346);
        setFreeSubterm(sv[7],0,con_392);
        TERM_ALLOC(sv[8],term1,code_347);
        setFreeSubterm(sv[8],0,con_368);
        TERM_ALLOC(sv[6],term2,code_351);
        setFreeSubterm(sv[6],0,sv[7]);
        setFreeSubterm(sv[6],1,sv[8]);
        TERM_ALLOC(sv[7],term2,code_350);
        setFreeSubterm(sv[7],0,sv[5]);
        setFreeSubterm(sv[7],1,sv[6]);
        TERM_ALLOC(sv[5],term2,code_350);
        setFreeSubterm(sv[5],0,sv[4]);
        setFreeSubterm(sv[5],1,sv[7]);
        TERM_ALLOC(sv[4],term2,code_350);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,sv[5]);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[4]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v27);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((C1_0)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_368);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[0]);
        res = sv[3] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend89:;
      }
      if(bitSet_get(mask,12)) {
        struct term *tmp, *sv[9];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((C1_1)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend90;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(+(.((c1w0),(C1_0)),+(.((c1w1),(C1_1)),+(.((c1w2),(C1_2)),.('(c1r1),(C1_1)))))),var0) */
        TERM_ALLOC(sv[4],term1,code_345);
        setFreeSubterm(sv[4],0,con_395);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_368);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[5]);
        TERM_ALLOC(sv[5],term1,code_345);
        setFreeSubterm(sv[5],0,con_396);
        TERM_ALLOC(sv[6],term1,code_347);
        setFreeSubterm(sv[6],0,con_369);
        TERM_ALLOC(sv[4],term2,code_351);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[6],term1,code_345);
        setFreeSubterm(sv[6],0,con_397);
        TERM_ALLOC(sv[7],term1,code_347);
        setFreeSubterm(sv[7],0,con_370);
        TERM_ALLOC(sv[5],term2,code_351);
        setFreeSubterm(sv[5],0,sv[6]);
        setFreeSubterm(sv[5],1,sv[7]);
        TERM_ALLOC(sv[7],term1,code_346);
        setFreeSubterm(sv[7],0,con_393);
        TERM_ALLOC(sv[8],term1,code_347);
        setFreeSubterm(sv[8],0,con_369);
        TERM_ALLOC(sv[6],term2,code_351);
        setFreeSubterm(sv[6],0,sv[7]);
        setFreeSubterm(sv[6],1,sv[8]);
        TERM_ALLOC(sv[7],term2,code_350);
        setFreeSubterm(sv[7],0,sv[5]);
        setFreeSubterm(sv[7],1,sv[6]);
        TERM_ALLOC(sv[5],term2,code_350);
        setFreeSubterm(sv[5],0,sv[4]);
        setFreeSubterm(sv[5],1,sv[7]);
        TERM_ALLOC(sv[4],term2,code_350);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,sv[5]);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[4]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v26);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((C1_1)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_369);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[0]);
        res = sv[3] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend90:;
      }
      if(bitSet_get(mask,13)) {
        struct term *tmp, *sv[9];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((C1_2)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend91;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(+(.((c1w0),(C1_0)),+(.((c1w1),(C1_1)),+(.((c1w2),(C1_2)),.('(c1r2),(C1_2)))))),var0) */
        TERM_ALLOC(sv[4],term1,code_345);
        setFreeSubterm(sv[4],0,con_395);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_368);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[5]);
        TERM_ALLOC(sv[5],term1,code_345);
        setFreeSubterm(sv[5],0,con_396);
        TERM_ALLOC(sv[6],term1,code_347);
        setFreeSubterm(sv[6],0,con_369);
        TERM_ALLOC(sv[4],term2,code_351);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[6],term1,code_345);
        setFreeSubterm(sv[6],0,con_397);
        TERM_ALLOC(sv[7],term1,code_347);
        setFreeSubterm(sv[7],0,con_370);
        TERM_ALLOC(sv[5],term2,code_351);
        setFreeSubterm(sv[5],0,sv[6]);
        setFreeSubterm(sv[5],1,sv[7]);
        TERM_ALLOC(sv[7],term1,code_346);
        setFreeSubterm(sv[7],0,con_394);
        TERM_ALLOC(sv[8],term1,code_347);
        setFreeSubterm(sv[8],0,con_370);
        TERM_ALLOC(sv[6],term2,code_351);
        setFreeSubterm(sv[6],0,sv[7]);
        setFreeSubterm(sv[6],1,sv[8]);
        TERM_ALLOC(sv[7],term2,code_350);
        setFreeSubterm(sv[7],0,sv[5]);
        setFreeSubterm(sv[7],1,sv[6]);
        TERM_ALLOC(sv[5],term2,code_350);
        setFreeSubterm(sv[5],0,sv[4]);
        setFreeSubterm(sv[5],1,sv[7]);
        TERM_ALLOC(sv[4],term2,code_350);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,sv[5]);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[4]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v25);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((C1_2)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_370);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[0]);
        res = sv[3] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend91:;
      }
      if(bitSet_get(mask,14)) {
        struct term *tmp, *sv[9];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((C2_0)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend92;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(+(.((c2w0),(C2_0)),+(.((c2w1),(C2_1)),+(.((c2w2),(C2_2)),.('(c2r0),(C2_0)))))),var0) */
        TERM_ALLOC(sv[4],term1,code_345);
        setFreeSubterm(sv[4],0,con_401);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_371);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[5]);
        TERM_ALLOC(sv[5],term1,code_345);
        setFreeSubterm(sv[5],0,con_402);
        TERM_ALLOC(sv[6],term1,code_347);
        setFreeSubterm(sv[6],0,con_372);
        TERM_ALLOC(sv[4],term2,code_351);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[6],term1,code_345);
        setFreeSubterm(sv[6],0,con_403);
        TERM_ALLOC(sv[7],term1,code_347);
        setFreeSubterm(sv[7],0,con_373);
        TERM_ALLOC(sv[5],term2,code_351);
        setFreeSubterm(sv[5],0,sv[6]);
        setFreeSubterm(sv[5],1,sv[7]);
        TERM_ALLOC(sv[7],term1,code_346);
        setFreeSubterm(sv[7],0,con_398);
        TERM_ALLOC(sv[8],term1,code_347);
        setFreeSubterm(sv[8],0,con_371);
        TERM_ALLOC(sv[6],term2,code_351);
        setFreeSubterm(sv[6],0,sv[7]);
        setFreeSubterm(sv[6],1,sv[8]);
        TERM_ALLOC(sv[7],term2,code_350);
        setFreeSubterm(sv[7],0,sv[5]);
        setFreeSubterm(sv[7],1,sv[6]);
        TERM_ALLOC(sv[5],term2,code_350);
        setFreeSubterm(sv[5],0,sv[4]);
        setFreeSubterm(sv[5],1,sv[7]);
        TERM_ALLOC(sv[4],term2,code_350);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,sv[5]);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[4]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v24);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((C2_0)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_371);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[0]);
        res = sv[3] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend92:;
      }
      if(bitSet_get(mask,15)) {
        struct term *tmp, *sv[9];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((C2_1)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend93;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(+(.((c2w0),(C2_0)),+(.((c2w1),(C2_1)),+(.((c2w2),(C2_2)),.('(c2r1),(C2_1)))))),var0) */
        TERM_ALLOC(sv[4],term1,code_345);
        setFreeSubterm(sv[4],0,con_401);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_371);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[5]);
        TERM_ALLOC(sv[5],term1,code_345);
        setFreeSubterm(sv[5],0,con_402);
        TERM_ALLOC(sv[6],term1,code_347);
        setFreeSubterm(sv[6],0,con_372);
        TERM_ALLOC(sv[4],term2,code_351);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[6],term1,code_345);
        setFreeSubterm(sv[6],0,con_403);
        TERM_ALLOC(sv[7],term1,code_347);
        setFreeSubterm(sv[7],0,con_373);
        TERM_ALLOC(sv[5],term2,code_351);
        setFreeSubterm(sv[5],0,sv[6]);
        setFreeSubterm(sv[5],1,sv[7]);
        TERM_ALLOC(sv[7],term1,code_346);
        setFreeSubterm(sv[7],0,con_399);
        TERM_ALLOC(sv[8],term1,code_347);
        setFreeSubterm(sv[8],0,con_372);
        TERM_ALLOC(sv[6],term2,code_351);
        setFreeSubterm(sv[6],0,sv[7]);
        setFreeSubterm(sv[6],1,sv[8]);
        TERM_ALLOC(sv[7],term2,code_350);
        setFreeSubterm(sv[7],0,sv[5]);
        setFreeSubterm(sv[7],1,sv[6]);
        TERM_ALLOC(sv[5],term2,code_350);
        setFreeSubterm(sv[5],0,sv[4]);
        setFreeSubterm(sv[5],1,sv[7]);
        TERM_ALLOC(sv[4],term2,code_350);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,sv[5]);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[4]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v23);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((C2_1)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_372);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[0]);
        res = sv[3] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend93:;
      }
      if(bitSet_get(mask,16)) {
        struct term *tmp, *sv[9];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((C2_2)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend94;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(+(.((c2w0),(C2_0)),+(.((c2w1),(C2_1)),+(.((c2w2),(C2_2)),.('(c2r2),(C2_2)))))),var0) */
        TERM_ALLOC(sv[4],term1,code_345);
        setFreeSubterm(sv[4],0,con_401);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_371);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[5]);
        TERM_ALLOC(sv[5],term1,code_345);
        setFreeSubterm(sv[5],0,con_402);
        TERM_ALLOC(sv[6],term1,code_347);
        setFreeSubterm(sv[6],0,con_372);
        TERM_ALLOC(sv[4],term2,code_351);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[6],term1,code_345);
        setFreeSubterm(sv[6],0,con_403);
        TERM_ALLOC(sv[7],term1,code_347);
        setFreeSubterm(sv[7],0,con_373);
        TERM_ALLOC(sv[5],term2,code_351);
        setFreeSubterm(sv[5],0,sv[6]);
        setFreeSubterm(sv[5],1,sv[7]);
        TERM_ALLOC(sv[7],term1,code_346);
        setFreeSubterm(sv[7],0,con_400);
        TERM_ALLOC(sv[8],term1,code_347);
        setFreeSubterm(sv[8],0,con_373);
        TERM_ALLOC(sv[6],term2,code_351);
        setFreeSubterm(sv[6],0,sv[7]);
        setFreeSubterm(sv[6],1,sv[8]);
        TERM_ALLOC(sv[7],term2,code_350);
        setFreeSubterm(sv[7],0,sv[5]);
        setFreeSubterm(sv[7],1,sv[6]);
        TERM_ALLOC(sv[5],term2,code_350);
        setFreeSubterm(sv[5],0,sv[4]);
        setFreeSubterm(sv[5],1,sv[7]);
        TERM_ALLOC(sv[4],term2,code_350);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,sv[5]);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[4]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v22);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((C2_2)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_373);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[0]);
        res = sv[3] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend94:;
      }
      if(bitSet_get(mask,17)) {
        struct term *tmp, *sv[7];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((P1)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend95;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(+(.(tau,(P11)),.(tau,0))),var0) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_375);
        TERM_ALLOC(sv[4],term2,code_351);
        setFreeSubterm(sv[4],0,con_344);
        setFreeSubterm(sv[4],1,sv[5]);
        TERM_ALLOC(sv[6],term2,code_351);
        setFreeSubterm(sv[6],0,con_344);
        setFreeSubterm(sv[6],1,con_348);
        TERM_ALLOC(sv[3],term2,code_350);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[6]);
        TERM_ALLOC(sv[4],term1,code_358);
        setFreeSubterm(sv[4],0,sv[3]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,v21);
        sv[2] = strTab[472]( sv[3] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((P1)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_374);
        TERM_ALLOC(sv[4],term1,code_358);
        setFreeSubterm(sv[4],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[4]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[0]);
        res = sv[4] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend95:;
      }
      if(bitSet_get(mask,18)) {
        struct term *tmp, *sv[7];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((P11)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend96;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(.('(c1w1),.((req1),(P12)))),var0) */
        TERM_ALLOC(sv[4],term1,code_346);
        setFreeSubterm(sv[4],0,con_396);
        TERM_ALLOC(sv[5],term1,code_345);
        setFreeSubterm(sv[5],0,con_412);
        TERM_ALLOC(sv[6],term1,code_347);
        setFreeSubterm(sv[6],0,con_376);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[6]);
        TERM_ALLOC(sv[5],term2,code_351);
        setFreeSubterm(sv[5],0,sv[4]);
        setFreeSubterm(sv[5],1,sv[3]);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v20);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((P11)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_375);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[0]);
        res = sv[3] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend96:;
      }
      if(bitSet_get(mask,19)) {
        struct term *tmp, *sv[7];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((P12)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend97;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(+(.((kr1),(P14)),.((kr2),(P13)))),var0) */
        TERM_ALLOC(sv[4],term1,code_345);
        setFreeSubterm(sv[4],0,con_408);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_378);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[5]);
        TERM_ALLOC(sv[5],term1,code_345);
        setFreeSubterm(sv[5],0,con_409);
        TERM_ALLOC(sv[6],term1,code_347);
        setFreeSubterm(sv[6],0,con_377);
        TERM_ALLOC(sv[4],term2,code_351);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[5],term2,code_350);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[4]);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v19);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((P12)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_376);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[0]);
        res = sv[3] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend97:;
      }
      if(bitSet_get(mask,20)) {
        struct term *tmp, *sv[8];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((P13)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend98;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(+(.((c2r0),(P14)),+(.((c2r1),(P12)),.((c2r2),(P12))))),var0) */
        TERM_ALLOC(sv[4],term1,code_345);
        setFreeSubterm(sv[4],0,con_398);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_378);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[5]);
        TERM_ALLOC(sv[5],term1,code_345);
        setFreeSubterm(sv[5],0,con_399);
        TERM_ALLOC(sv[6],term1,code_347);
        setFreeSubterm(sv[6],0,con_376);
        TERM_ALLOC(sv[4],term2,code_351);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[6],term1,code_345);
        setFreeSubterm(sv[6],0,con_400);
        TERM_ALLOC(sv[7],term1,code_347);
        setFreeSubterm(sv[7],0,con_376);
        TERM_ALLOC(sv[5],term2,code_351);
        setFreeSubterm(sv[5],0,sv[6]);
        setFreeSubterm(sv[5],1,sv[7]);
        TERM_ALLOC(sv[6],term2,code_350);
        setFreeSubterm(sv[6],0,sv[4]);
        setFreeSubterm(sv[6],1,sv[5]);
        TERM_ALLOC(sv[4],term2,code_350);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[4]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v18);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((P13)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_377);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[0]);
        res = sv[3] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend98:;
      }
      if(bitSet_get(mask,21)) {
        struct term *tmp, *sv[6];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((P14)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend99;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(.('(c1w2),(P15))),var0) */
        TERM_ALLOC(sv[4],term1,code_346);
        setFreeSubterm(sv[4],0,con_397);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_379);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[5]);
        TERM_ALLOC(sv[4],term1,code_358);
        setFreeSubterm(sv[4],0,sv[3]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,v17);
        sv[2] = strTab[472]( sv[3] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((P14)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_378);
        TERM_ALLOC(sv[4],term1,code_358);
        setFreeSubterm(sv[4],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[4]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[0]);
        res = sv[4] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend99:;
      }
      if(bitSet_get(mask,22)) {
        struct term *tmp, *sv[8];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((P15)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend100;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(+(.((c2r0),(P16)),+(.((c2r1),(P16)),.((c2r2),(P17))))),var0) */
        TERM_ALLOC(sv[4],term1,code_345);
        setFreeSubterm(sv[4],0,con_398);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_380);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[5]);
        TERM_ALLOC(sv[5],term1,code_345);
        setFreeSubterm(sv[5],0,con_399);
        TERM_ALLOC(sv[6],term1,code_347);
        setFreeSubterm(sv[6],0,con_380);
        TERM_ALLOC(sv[4],term2,code_351);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[6],term1,code_345);
        setFreeSubterm(sv[6],0,con_400);
        TERM_ALLOC(sv[7],term1,code_347);
        setFreeSubterm(sv[7],0,con_381);
        TERM_ALLOC(sv[5],term2,code_351);
        setFreeSubterm(sv[5],0,sv[6]);
        setFreeSubterm(sv[5],1,sv[7]);
        TERM_ALLOC(sv[6],term2,code_350);
        setFreeSubterm(sv[6],0,sv[4]);
        setFreeSubterm(sv[6],1,sv[5]);
        TERM_ALLOC(sv[4],term2,code_350);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[4]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v16);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((P15)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_379);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[0]);
        res = sv[3] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend100:;
      }
      if(bitSet_get(mask,23)) {
        struct term *tmp, *sv[10];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((P16)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend101;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(.('(kw1),.((enter1),.((exit1),.('(kw2),.('(c1w0),(P1))))))),var0) */
        TERM_ALLOC(sv[4],term1,code_346);
        setFreeSubterm(sv[4],0,con_410);
        TERM_ALLOC(sv[5],term1,code_345);
        setFreeSubterm(sv[5],0,con_404);
        TERM_ALLOC(sv[6],term1,code_345);
        setFreeSubterm(sv[6],0,con_406);
        TERM_ALLOC(sv[7],term1,code_346);
        setFreeSubterm(sv[7],0,con_411);
        TERM_ALLOC(sv[8],term1,code_346);
        setFreeSubterm(sv[8],0,con_395);
        TERM_ALLOC(sv[9],term1,code_347);
        setFreeSubterm(sv[9],0,con_374);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[8]);
        setFreeSubterm(sv[3],1,sv[9]);
        TERM_ALLOC(sv[8],term2,code_351);
        setFreeSubterm(sv[8],0,sv[7]);
        setFreeSubterm(sv[8],1,sv[3]);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[6]);
        setFreeSubterm(sv[3],1,sv[8]);
        TERM_ALLOC(sv[6],term2,code_351);
        setFreeSubterm(sv[6],0,sv[5]);
        setFreeSubterm(sv[6],1,sv[3]);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[6]);
        TERM_ALLOC(sv[4],term1,code_358);
        setFreeSubterm(sv[4],0,sv[3]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,v15);
        sv[2] = strTab[472]( sv[3] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((P16)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_380);
        TERM_ALLOC(sv[4],term1,code_358);
        setFreeSubterm(sv[4],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[4]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[0]);
        res = sv[4] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend101:;
      }
      if(bitSet_get(mask,24)) {
        struct term *tmp, *sv[6];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((P17)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend102;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(.('(c1w1),(P12))),var0) */
        TERM_ALLOC(sv[4],term1,code_346);
        setFreeSubterm(sv[4],0,con_396);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_376);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[5]);
        TERM_ALLOC(sv[4],term1,code_358);
        setFreeSubterm(sv[4],0,sv[3]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,v14);
        sv[2] = strTab[472]( sv[3] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((P17)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_381);
        TERM_ALLOC(sv[4],term1,code_358);
        setFreeSubterm(sv[4],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[4]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[0]);
        res = sv[4] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend102:;
      }
      if(bitSet_get(mask,25)) {
        struct term *tmp, *sv[7];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((P2)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend103;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(+(.(tau,(P21)),.(tau,0))),var0) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_383);
        TERM_ALLOC(sv[4],term2,code_351);
        setFreeSubterm(sv[4],0,con_344);
        setFreeSubterm(sv[4],1,sv[5]);
        TERM_ALLOC(sv[6],term2,code_351);
        setFreeSubterm(sv[6],0,con_344);
        setFreeSubterm(sv[6],1,con_348);
        TERM_ALLOC(sv[3],term2,code_350);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[6]);
        TERM_ALLOC(sv[4],term1,code_358);
        setFreeSubterm(sv[4],0,sv[3]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,v13);
        sv[2] = strTab[472]( sv[3] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((P2)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_382);
        TERM_ALLOC(sv[4],term1,code_358);
        setFreeSubterm(sv[4],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[4]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[0]);
        res = sv[4] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend103:;
      }
      if(bitSet_get(mask,26)) {
        struct term *tmp, *sv[7];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((P21)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend104;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(.('(c2w1),.((req2),(P22)))),var0) */
        TERM_ALLOC(sv[4],term1,code_346);
        setFreeSubterm(sv[4],0,con_402);
        TERM_ALLOC(sv[5],term1,code_345);
        setFreeSubterm(sv[5],0,con_413);
        TERM_ALLOC(sv[6],term1,code_347);
        setFreeSubterm(sv[6],0,con_384);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[6]);
        TERM_ALLOC(sv[5],term2,code_351);
        setFreeSubterm(sv[5],0,sv[4]);
        setFreeSubterm(sv[5],1,sv[3]);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v12);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((P21)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_383);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[0]);
        res = sv[3] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend104:;
      }
      if(bitSet_get(mask,27)) {
        struct term *tmp, *sv[7];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((P22)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend105;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(+(.((kr2),(P24)),.((kr1),(P23)))),var0) */
        TERM_ALLOC(sv[4],term1,code_345);
        setFreeSubterm(sv[4],0,con_409);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_386);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[5]);
        TERM_ALLOC(sv[5],term1,code_345);
        setFreeSubterm(sv[5],0,con_408);
        TERM_ALLOC(sv[6],term1,code_347);
        setFreeSubterm(sv[6],0,con_385);
        TERM_ALLOC(sv[4],term2,code_351);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[5],term2,code_350);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[4]);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v11);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((P22)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_384);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[0]);
        res = sv[3] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend105:;
      }
      if(bitSet_get(mask,28)) {
        struct term *tmp, *sv[8];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((P23)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend106;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(+(.((c1r0),(P24)),+(.((c1r1),(P22)),.((c1r2),(P22))))),var0) */
        TERM_ALLOC(sv[4],term1,code_345);
        setFreeSubterm(sv[4],0,con_392);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_386);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[5]);
        TERM_ALLOC(sv[5],term1,code_345);
        setFreeSubterm(sv[5],0,con_393);
        TERM_ALLOC(sv[6],term1,code_347);
        setFreeSubterm(sv[6],0,con_384);
        TERM_ALLOC(sv[4],term2,code_351);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[6],term1,code_345);
        setFreeSubterm(sv[6],0,con_394);
        TERM_ALLOC(sv[7],term1,code_347);
        setFreeSubterm(sv[7],0,con_384);
        TERM_ALLOC(sv[5],term2,code_351);
        setFreeSubterm(sv[5],0,sv[6]);
        setFreeSubterm(sv[5],1,sv[7]);
        TERM_ALLOC(sv[6],term2,code_350);
        setFreeSubterm(sv[6],0,sv[4]);
        setFreeSubterm(sv[6],1,sv[5]);
        TERM_ALLOC(sv[4],term2,code_350);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[4]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v10);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((P23)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_385);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[0]);
        res = sv[3] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend106:;
      }
      if(bitSet_get(mask,29)) {
        struct term *tmp, *sv[6];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((P24)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend107;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(.('(c2w2),(P25))),var0) */
        TERM_ALLOC(sv[4],term1,code_346);
        setFreeSubterm(sv[4],0,con_403);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_387);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[5]);
        TERM_ALLOC(sv[4],term1,code_358);
        setFreeSubterm(sv[4],0,sv[3]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,v9);
        sv[2] = strTab[472]( sv[3] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((P24)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_386);
        TERM_ALLOC(sv[4],term1,code_358);
        setFreeSubterm(sv[4],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[4]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[0]);
        res = sv[4] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend107:;
      }
      if(bitSet_get(mask,30)) {
        struct term *tmp, *sv[8];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((P25)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend108;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(+(.((c1r0),(P26)),+(.((c1r1),(P26)),.((c1r2),(P27))))),var0) */
        TERM_ALLOC(sv[4],term1,code_345);
        setFreeSubterm(sv[4],0,con_392);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_388);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[5]);
        TERM_ALLOC(sv[5],term1,code_345);
        setFreeSubterm(sv[5],0,con_393);
        TERM_ALLOC(sv[6],term1,code_347);
        setFreeSubterm(sv[6],0,con_388);
        TERM_ALLOC(sv[4],term2,code_351);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[6],term1,code_345);
        setFreeSubterm(sv[6],0,con_394);
        TERM_ALLOC(sv[7],term1,code_347);
        setFreeSubterm(sv[7],0,con_389);
        TERM_ALLOC(sv[5],term2,code_351);
        setFreeSubterm(sv[5],0,sv[6]);
        setFreeSubterm(sv[5],1,sv[7]);
        TERM_ALLOC(sv[6],term2,code_350);
        setFreeSubterm(sv[6],0,sv[4]);
        setFreeSubterm(sv[6],1,sv[5]);
        TERM_ALLOC(sv[4],term2,code_350);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,sv[6]);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[4]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v8);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((P25)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_387);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[0]);
        res = sv[3] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend108:;
      }
      if(bitSet_get(mask,31)) {
        struct term *tmp, *sv[10];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((P26)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend109;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(.('(kw2),.((enter2),.((exit2),.('(kw1),.('(c2w0),(P2))))))),var0) */
        TERM_ALLOC(sv[4],term1,code_346);
        setFreeSubterm(sv[4],0,con_411);
        TERM_ALLOC(sv[5],term1,code_345);
        setFreeSubterm(sv[5],0,con_405);
        TERM_ALLOC(sv[6],term1,code_345);
        setFreeSubterm(sv[6],0,con_407);
        TERM_ALLOC(sv[7],term1,code_346);
        setFreeSubterm(sv[7],0,con_410);
        TERM_ALLOC(sv[8],term1,code_346);
        setFreeSubterm(sv[8],0,con_401);
        TERM_ALLOC(sv[9],term1,code_347);
        setFreeSubterm(sv[9],0,con_382);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[8]);
        setFreeSubterm(sv[3],1,sv[9]);
        TERM_ALLOC(sv[8],term2,code_351);
        setFreeSubterm(sv[8],0,sv[7]);
        setFreeSubterm(sv[8],1,sv[3]);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[6]);
        setFreeSubterm(sv[3],1,sv[8]);
        TERM_ALLOC(sv[6],term2,code_351);
        setFreeSubterm(sv[6],0,sv[5]);
        setFreeSubterm(sv[6],1,sv[3]);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[6]);
        TERM_ALLOC(sv[4],term1,code_358);
        setFreeSubterm(sv[4],0,sv[3]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,v7);
        sv[2] = strTab[472]( sv[3] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((P26)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_388);
        TERM_ALLOC(sv[4],term1,code_358);
        setFreeSubterm(sv[4],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[4]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[0]);
        res = sv[4] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend109:;
      }
      if(bitSet_get(mask,32)) {
        struct term *tmp, *sv[6];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((P27)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend110;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(.('(c2w1),(P22))),var0) */
        TERM_ALLOC(sv[4],term1,code_346);
        setFreeSubterm(sv[4],0,con_402);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_384);
        TERM_ALLOC(sv[3],term2,code_351);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,sv[5]);
        TERM_ALLOC(sv[4],term1,code_358);
        setFreeSubterm(sv[4],0,sv[3]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[4]);
        setFreeSubterm(sv[3],1,v6);
        sv[2] = strTab[472]( sv[3] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((P27)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_389);
        TERM_ALLOC(sv[4],term1,code_358);
        setFreeSubterm(sv[4],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[4]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[5]);
        setFreeSubterm(sv[4],1,sv[0]);
        res = sv[4] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend110:;
      }
      if(bitSet_get(mask,33)) {
        struct term *tmp, *sv[7];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |((;)((KBig)),var0) */
        long tmp_step;
        /* allDetEvaluation: nonDet */
        if(localSetChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend111;
        }
        /* where |(var1,var2) := (calc_str:rccst/memo3) |((;)(#((Knuth),#((Knuth),(Knuth)))),var0) */
        TERM_ALLOC(sv[4],term1,code_347);
        setFreeSubterm(sv[4],0,con_365);
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_365);
        TERM_ALLOC(sv[6],term1,code_347);
        setFreeSubterm(sv[6],0,con_365);
        TERM_ALLOC(sv[3],term2,code_349);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[6]);
        TERM_ALLOC(sv[5],term2,code_349);
        setFreeSubterm(sv[5],0,sv[4]);
        setFreeSubterm(sv[5],1,sv[3]);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var0        underAC=false        Instantiated=false
        TERM_ALLOC(sv[4],term2,code_359);
        setFreeSubterm(sv[4],0,sv[3]);
        setFreeSubterm(sv[4],1,v5);
        sv[2] = strTab[472]( sv[4] );
        if(code_359 != getSymb(sv[2])) {
          fail();
        } else {
          sv[1] = getFreeSubterm(sv[2],0);
          sv[0] = getFreeSubterm(sv[2],1);
        }
        /* rhs: |(-->((;)((KBig)),var1),var2) */
        TERM_ALLOC(sv[5],term1,code_347);
        setFreeSubterm(sv[5],0,con_390);
        TERM_ALLOC(sv[3],term1,code_358);
        setFreeSubterm(sv[3],0,sv[5]);
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[5],term2,code_363);
        setFreeSubterm(sv[5],0,sv[3]);
        setFreeSubterm(sv[5],1,sv[1]);
        // this=var2        underAC=false        Instantiated=false
        TERM_ALLOC(sv[3],term2,code_359);
        setFreeSubterm(sv[3],0,sv[5]);
        setFreeSubterm(sv[3],1,sv[0]);
        res = sv[3] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab14;
        myend111:;
      }
    }
    fail();
    stratLab14:;
    v0=res;
    {
      /* dk[semiDet](updateMemo:rccst/memo3!GL) */
      struct term *v1,*v2,*v3,*v4,*v5,*v6;
      bitSet_GC_create(mask,1);
      bitSet_init_clear(mask);
      switch(getSymb(v0)) {
      case code_359: /* | */
        v2= getFreeSubterm(v0,0);
        switch(getSymb(v2)) {
        case code_363: /* --> */
          v3= getFreeSubterm(v2,0);
          switch(getSymb(v3)) {
          default:
          label365:
            v4= getFreeSubterm(v2,1);
            switch(getSymb(v4)) {
            default:
            label366:
              v5= getFreeSubterm(v0,1);
              switch(getSymb(v5)) {
              default:
              label367:
                bitSet_set(mask,0);
              }
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
      if(bitSet_get(mask,0)) {
        struct term *tmp, *sv[3];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: |(-->(var0,var1),var2) */
        long tmp_step;
        /* allDetEvaluation: det */
        /* where var3 := .updateHashTable(,)(var2,var0,var1) */
        // this=var2        underAC=false        Instantiated=false
        // this=var0        underAC=false        Instantiated=false
        // this=var1        underAC=false        Instantiated=false
        sv[1] = fun_343( v5,v3,v4 );
        tmp = sv[0] = sv[1];
        /* rhs: |(var1,var3) */
        // this=var1        underAC=false        Instantiated=false
        // this=var3        underAC=false        Instantiated=false
        TERM_ALLOC(sv[2],term2,code_359);
        setFreeSubterm(sv[2],0,v4);
        setFreeSubterm(sv[2],1,sv[0]);
        res = sv[2] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab16;
        myend112:;
      }
    }
    fail();
    stratLab16:;
    v0=res;
    goto stratLab18;
  stratLab18:;
  }
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_494( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  /* cons[nonDet](iterate[multiDet](one[semiDet](extractrule2:rccst/list[rccst]!LO)),one[semiDet](extractrule1:rccst/list[rccst]!LO)) */
  /* iterate[multiDet](one[semiDet](extractrule2:rccst/list[rccst]!LO)) */
  while(1) {
    if(!setChoicePoint()) {
      break;
    }
    /* Apply the strategy */
    {
      /* one[semiDet](extractrule2:rccst/list[rccst]!LO) */
      struct term *v1,*v2,*v3,*v4,*v5;
      bitSet_GC_create(mask,1);
      bitSet_init_clear(mask);
      switch(getSymb(v0)) {
      case code_338: /* elem() */
        v2= getFreeSubterm(v0,0);
        switch(getSymb(v2)) {
        case code_336: /* , */
          v3= getFreeSubterm(v2,0);
          switch(getSymb(v3)) {
          default:
          label371:
            v4= getFreeSubterm(v2,1);
            switch(getSymb(v4)) {
            default:
            label372:
              bitSet_set(mask,0);
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
      if(bitSet_get(mask,0)) {
        struct term *tmp, *sv[1];
        multiplicityType *E,*sol;
        struct term *substitution[1];
        /* lhs: elem()(,(var0,var1)) */
        long tmp_step;
        /* allDetEvaluation: det */
        /* rhs: elem()(var1) */
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[0],term1,code_338);
        setFreeSubterm(sv[0],0,v4);
        res = sv[0] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab20;
        myend113:;
      }
    }
    fail();
    stratLab20:;
    v0=res;
  }
  {
    /* one[semiDet](extractrule1:rccst/list[rccst]!LO) */
    struct term *v1,*v2,*v3,*v4,*v5;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(getSymb(v0)) {
    case code_338: /* elem() */
      v2= getFreeSubterm(v0,0);
      switch(getSymb(v2)) {
      case code_336: /* , */
        v3= getFreeSubterm(v2,0);
        switch(getSymb(v3)) {
        default:
        label376:
          v4= getFreeSubterm(v2,1);
          switch(getSymb(v4)) {
          default:
          label377:
            bitSet_set(mask,0);
          }
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      struct term *tmp, *sv[1];
      multiplicityType *E,*sol;
      struct term *substitution[1];
      /* lhs: elem()(,(var0,var1)) */
      long tmp_step;
      /* allDetEvaluation: det */
      /* rhs: var0 */
      // this=var0        underAC=false        Instantiated=false
      res = v3 ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab23;
      myend114:;
    }
  }
  fail();
  stratLab23:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_127( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  {
    /* dk[semiDet](start_rule:rccst/memo3!GL) */
    struct term *v1,*v2;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(getSymb(v0)) {
    default:
    label379:
      bitSet_set(mask,0);
    }
    if(bitSet_get(mask,0)) {
      struct term *tmp, *sv[1];
      multiplicityType *E,*sol;
      struct term *substitution[1];
      /* lhs: var0 */
      long tmp_step;
      /* allDetEvaluation: det */
      /* rhs: var0 */
      // this=var0        underAC=false        Instantiated=false
      res = v0 ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab2;
      myend115:;
    }
  }
  fail();
  stratLab2:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}
