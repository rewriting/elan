#include "ans.h"

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
  case code_301: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label3:
      switch(getSymb(v2)) {
      case code_301: /* [] */
        v7=v2->sub[0];
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
    struct term *substitution[1];
    /* lhs: plus(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend0;
    }
    /* where var2 := plus(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_3( v5,v7 );
    tmp = sv[0] = sv[1];
    CUTCLOSE(); /* Wheres */
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_301);
    sv[2]->sub[0] = sv[0];
    res = sv[2] ;
    goto end;
    myend0:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term2, 302);
  res->sub[0] = v1;
  res->sub[1] = v2;
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
  case code_301: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label9:
      switch(getSymb(v2)) {
      case code_301: /* [] */
        v7=v2->sub[0];
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
    struct term *substitution[1];
    /* lhs: minus(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend1;
    }
    /* where var2 := minus(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_4( v5,v7 );
    tmp = sv[0] = sv[1];
    CUTCLOSE(); /* Wheres */
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_301);
    sv[2]->sub[0] = sv[0];
    res = sv[2] ;
    goto end;
    myend1:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term2, 303);
  res->sub[0] = v1;
  res->sub[1] = v2;
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
  case code_301: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label15:
      switch(getSymb(v2)) {
      case code_301: /* [] */
        v7=v2->sub[0];
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
    struct term *substitution[1];
    /* lhs: time(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend2;
    }
    /* where var2 := time(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_5( v5,v7 );
    tmp = sv[0] = sv[1];
    CUTCLOSE(); /* Wheres */
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_301);
    sv[2]->sub[0] = sv[0];
    res = sv[2] ;
    goto end;
    myend2:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term2, 304);
  res->sub[0] = v1;
  res->sub[1] = v2;
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
  case code_301: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label21:
      switch(getSymb(v2)) {
      case code_301: /* [] */
        v7=v2->sub[0];
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
    struct term *substitution[1];
    /* lhs: mod(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend3;
    }
    /* where var2 := mod(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_27( v5,v7 );
    tmp = sv[0] = sv[1];
    CUTCLOSE(); /* Wheres */
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_301);
    sv[2]->sub[0] = sv[0];
    res = sv[2] ;
    goto end;
    myend3:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term2, 305);
  res->sub[0] = v1;
  res->sub[1] = v2;
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
  case code_301: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label27:
      switch(getSymb(v2)) {
      case code_301: /* [] */
        v7=v2->sub[0];
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
    struct term *substitution[1];
    /* lhs: and(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend4;
    }
    /* where var2 := and(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_28( v5,v7 );
    tmp = sv[0] = sv[1];
    CUTCLOSE(); /* Wheres */
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_301);
    sv[2]->sub[0] = sv[0];
    res = sv[2] ;
    goto end;
    myend4:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term2, 306);
  res->sub[0] = v1;
  res->sub[1] = v2;
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
  case code_301: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label33:
      switch(getSymb(v2)) {
      case code_301: /* [] */
        v7=v2->sub[0];
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
    struct term *substitution[1];
    /* lhs: div(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend5;
    }
    /* where var2 := div(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_6( v5,v7 );
    tmp = sv[0] = sv[1];
    CUTCLOSE(); /* Wheres */
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_301);
    sv[2]->sub[0] = sv[0];
    res = sv[2] ;
    goto end;
    myend5:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term2, 307);
  res->sub[0] = v1;
  res->sub[1] = v2;
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

struct term* fun_308(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_301: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label39:
      switch(getSymb(v2)) {
      case code_301: /* [] */
        v7=v2->sub[0];
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
    struct term *substitution[1];
    /* lhs: or(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend6;
    }
    /* where var2 := or(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_29( v5,v7 );
    tmp = sv[0] = sv[1];
    CUTCLOSE(); /* Wheres */
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_301);
    sv[2]->sub[0] = sv[0];
    res = sv[2] ;
    goto end;
    myend6:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term2, 308);
  res->sub[0] = v1;
  res->sub[1] = v2;
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

struct term* fun_309(struct term *v1 ) {
  struct term *v2,*v3,*v4,*v5;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_301: /* [] */
    v4=v1->sub[0];
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
    struct term *substitution[1];
    /* lhs: umin()([](var0)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend7;
    }
    /* where var1 := umin_builtinInt()(var0) */
    // this=var0        underAC=false        Instantiated=false
    sv[1] = fun_20( v4 );
    tmp = sv[0] = sv[1];
    CUTCLOSE(); /* Wheres */
    /* rhs: [](var1) */
    // this=var1        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_301);
    sv[2]->sub[0] = sv[0];
    res = sv[2] ;
    goto end;
    myend7:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term1, 309);
  res->sub[0] = v1;
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
  case code_301: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label49:
      switch(getSymb(v2)) {
      case code_301: /* [] */
        v7=v2->sub[0];
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
    struct term *substitution[1];
    /* lhs: eq_int(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend8;
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: eq_builtinInt(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_8( v5,v7 );
    res = sv[0] ;
    goto end;
    myend8:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term2, 310);
  res->sub[0] = v1;
  res->sub[1] = v2;
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
  case code_301: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label55:
      switch(getSymb(v2)) {
      case code_301: /* [] */
        v7=v2->sub[0];
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
    struct term *substitution[1];
    /* lhs: neq_int(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend9;
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: neq_builtinInt(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_9( v5,v7 );
    res = sv[0] ;
    goto end;
    myend9:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term2, 311);
  res->sub[0] = v1;
  res->sub[1] = v2;
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
  case code_301: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label61:
      switch(getSymb(v2)) {
      case code_301: /* [] */
        v7=v2->sub[0];
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
    struct term *substitution[1];
    /* lhs: greater_int(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend10;
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: greater_builtinInt(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_12( v5,v7 );
    res = sv[0] ;
    goto end;
    myend10:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term2, 312);
  res->sub[0] = v1;
  res->sub[1] = v2;
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
  case code_301: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label67:
      switch(getSymb(v2)) {
      case code_301: /* [] */
        v7=v2->sub[0];
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
    struct term *substitution[1];
    /* lhs: greatereq_int(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend11;
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: greatereq_builtinInt(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_13( v5,v7 );
    res = sv[0] ;
    goto end;
    myend11:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term2, 313);
  res->sub[0] = v1;
  res->sub[1] = v2;
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
  case code_301: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label73:
      switch(getSymb(v2)) {
      case code_301: /* [] */
        v7=v2->sub[0];
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
    struct term *substitution[1];
    /* lhs: lesseq_int(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend12;
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: lesseq_builtinInt(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_11( v5,v7 );
    res = sv[0] ;
    goto end;
    myend12:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term2, 314);
  res->sub[0] = v1;
  res->sub[1] = v2;
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

struct term* fun_315(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_301: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label79:
      switch(getSymb(v2)) {
      case code_301: /* [] */
        v7=v2->sub[0];
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
    struct term *substitution[1];
    /* lhs: less_int(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend13;
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: less_builtinInt(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_10( v5,v7 );
    res = sv[0] ;
    goto end;
    myend13:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term2, 315);
  res->sub[0] = v1;
  res->sub[1] = v2;
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
    struct term *substitution[1];
    /* lhs: btoi_int()(var0) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend14;
    }
    /* where var1 := btoi_builtinInt()(var0) */
    // this=var0        underAC=false        Instantiated=false
    sv[1] = fun_25( v1 );
    tmp = sv[0] = sv[1];
    CUTCLOSE(); /* Wheres */
    /* rhs: [](var1) */
    // this=var1        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_301);
    sv[2]->sub[0] = sv[0];
    res = sv[2] ;
    goto end;
    myend14:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term1, 316);
  res->sub[0] = v1;
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
  case code_301: /* [] */
    v4=v1->sub[0];
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
    struct term *substitution[1];
    /* lhs: itob_int()([](var0)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend15;
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: itob_builtinInt()(var0) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_26( v4 );
    res = sv[0] ;
    goto end;
    myend15:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term1, 317);
  res->sub[0] = v1;
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

struct term* fun_318(struct term *v1 ) {
  struct term *v2,*v3,*v4,*v5;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_301: /* [] */
    v4=v1->sub[0];
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
    struct term *substitution[1];
    /* lhs: valueOf()([](var0)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend16;
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v4 ;
    goto end;
    myend16:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term1, 318);
  res->sub[0] = v1;
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
  struct term *v3,*v4,*v5,*v6,*v7,*v8,*v9;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_320: /* , */
    v5=v1->sub[0];
    switch(getSymb(v5)) {
    default:
    label98:
      v6=v1->sub[1];
      switch(getSymb(v6)) {
      default:
      label99:
        switch(getSymb(v2)) {
        default:
        label100:
          bitSet32_set(mask32,1);
        }
      }
    }
    break;
  case code_319: /* nil */
    switch(getSymb(v2)) {
    default:
    label96:
      bitSet32_set(mask32,0);
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    struct term *substitution[1];
    /* lhs: @(nil,var0) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend17;
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend17:;
    CUTCLOSE(); /* Wheres */
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[2];
    struct term *substitution[1];
    /* lhs: @(,(var0,var1),var2) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend18;
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: ,(var0,@(var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_321( v6,v2 );
    TERM_ALLOC(sv[1],term2,code_320);
    sv[1]->sub[0] = v5;
    sv[1]->sub[1] = sv[0];
    res = sv[1] ;
    goto end;
    myend18:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term2, 321);
  res->sub[0] = v1;
  res->sub[1] = v2;
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

struct term* fun_323(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_301: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    case 1: /* 1 */
      switch(getSymb(v2)) {
      case code_320: /* , */
        v7=v2->sub[0];
        switch(getSymb(v7)) {
        default:
        label106:
          v8=v2->sub[1];
          switch(getSymb(v8)) {
          default:
          label107:
            bitSet32_set(mask32,0);
            bitSet32_set(mask32,1);
          }
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    default:
      goto label103;
    }
    break;
  default:
  label103:
    switch(getSymb(v2)) {
    case code_320: /* , */
      v7=v2->sub[0];
      switch(getSymb(v7)) {
      default:
      label110:
        v8=v2->sub[1];
        switch(getSymb(v8)) {
        default:
        label111:
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
    struct term *substitution[1];
    /* lhs: -thelem()([](1),,(var0,var1)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend19;
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v7 ;
    goto end;
    myend19:;
    CUTCLOSE(); /* Wheres */
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[3];
    struct term *substitution[1];
    /* lhs: -thelem()(var0,,(var1,var2)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend20;
    }
    /* where var3 := minus(,)(var0,[](1)) */
    // this=var0        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_301);
    sv[2]->sub[0] = (setIntegerTag(1));
    sv[1] = fun_303( v1,sv[2] );
    tmp = sv[0] = sv[1];
    CUTCLOSE(); /* Wheres */
    /* rhs: -thelem()(var3,var2) */
    // this=var3        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[2] = fun_323( sv[0],v8 );
    res = sv[2] ;
    goto end;
    myend20:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term2, 323);
  res->sub[0] = v1;
  res->sub[1] = v2;
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
  struct term *v2,*v3,*v4,*v5,*v6;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_320: /* , */
    v4=v1->sub[0];
    switch(getSymb(v4)) {
    default:
    label116:
      v5=v1->sub[1];
      switch(getSymb(v5)) {
      default:
      label117:
        bitSet32_set(mask32,1);
      }
    }
    break;
  case code_319: /* nil */
    bitSet32_set(mask32,0);
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[2];
    struct term *substitution[1];
    /* lhs: size_of_identifier_list()(nil) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend21;
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: [](0) */
    TERM_ALLOC(sv[1],term1,code_301);
    sv[1]->sub[0] = (setIntegerTag(0));
    res = sv[1] ;
    goto end;
    myend21:;
    CUTCLOSE(); /* Wheres */
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[3];
    struct term *substitution[1];
    /* lhs: size_of_identifier_list()(,(var0,var1)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend22;
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: plus(,)([](1),size_of_identifier_list()(var1)) */
    TERM_ALLOC(sv[1],term1,code_301);
    sv[1]->sub[0] = (setIntegerTag(1));
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_324( v5 );
    sv[2] = fun_302( sv[1],sv[0] );
    res = sv[2] ;
    goto end;
    myend22:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term1, 324);
  res->sub[0] = v1;
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

struct term* fun_325(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_301: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    case 0: /* 0 */
      switch(getSymb(v2)) {
      default:
      label122:
        bitSet32_set(mask32,0);
        bitSet32_set(mask32,1);
      }
      break;
    default:
      goto label120;
    }
    break;
  default:
  label120:
    switch(getSymb(v2)) {
    default:
    label124:
      bitSet32_set(mask32,1);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    struct term *substitution[1];
    /* lhs: ccat(,)([](0),var0) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend23;
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: nil */
    res = con_319 ;
    goto end;
    myend23:;
    CUTCLOSE(); /* Wheres */
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[3];
    struct term *substitution[1];
    /* lhs: ccat(,)(var0,var1) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend24;
    }
    /* if greater_int(,)(var0,[](0)) */
    // this=var0        underAC=false        Instantiated=false
    TERM_ALLOC(sv[1],term1,code_301);
    sv[1]->sub[0] = (setIntegerTag(0));
    sv[0] = fun_312( v1,sv[1] );
    if( sv[0] != con_1 ) {
      fail();
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: @(var1,ccat(,)(minus(,)(var0,[](1)),var1)) */
    // this=var1        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_301);
    sv[2]->sub[0] = (setIntegerTag(1));
    sv[1] = fun_303( v1,sv[2] );
    // this=var1        underAC=false        Instantiated=false
    sv[2] = fun_325( sv[1],v2 );
    sv[1] = fun_321( v2,sv[2] );
    res = sv[1] ;
    goto end;
    myend24:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term2, 325);
  res->sub[0] = v1;
  res->sub[1] = v2;
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

struct term* fun_327( ) {
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
    struct term *tmp, *sv[5];
    struct term *substitution[1];
    /* lhs: Vars */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend25;
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: ,((88),,((89),,((90),nil))) */
    TERM_ALLOC(sv[1],term1,code_300);
    sv[1]->sub[0] = (setIdentifierTag(88));
    TERM_ALLOC(sv[2],term1,code_300);
    sv[2]->sub[0] = (setIdentifierTag(89));
    TERM_ALLOC(sv[3],term1,code_300);
    sv[3]->sub[0] = (setIdentifierTag(90));
    TERM_ALLOC(sv[4],term2,code_320);
    sv[4]->sub[0] = sv[3];
    sv[4]->sub[1] = con_319;
    TERM_ALLOC(sv[0],term2,code_320);
    sv[0]->sub[0] = sv[2];
    sv[0]->sub[1] = sv[4];
    TERM_ALLOC(sv[2],term2,code_320);
    sv[2]->sub[0] = sv[1];
    sv[2]->sub[1] = sv[0];
    res = sv[2] ;
    goto end;
    myend25:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term1,code_327);
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
int match_subterm_333(struct term *v0,int no_arg_subject, int *mask, BG *cbg);
void variable_extract_333(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern, int base_id_pattern);
static int **pattern_list_333;
static int no_pattern_333_niv_0;
static int nb_pattern_333_niv_0 = 1;
static int nb_pattern_333_niv_1 = 1;
#define max_nb_pattern_under_333 1

struct term* fun_333(struct term *v0 ) {
  struct term *v1,*v2;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  if(getArity(((struct termac*)v0))==1 && getMult(((struct termac*)v0),0)==1) {
    res=getSubterm(((struct termac*)v0),0);
    goto end_no_rewrite;
  }
  /* Begin syntactical matching */
  bitSet32_set(mask32,0);
  /* End syntactical matching */
  ms=(match_state**) MALLOC(1*sizeof(match_state*));
  if( bitSet32_get(mask32,0) ) {
    int necessary_link;
    ACPattern=1;
    /* Begin AC matching */
    necessary_link=0;
    indice = MS_init(&(ms[0]), match_subterm_333, no_pattern_333_niv_0, pattern_list_333, nb_pattern_333_niv_1, (struct termac*)v0, necessary_link, max_nb_pattern_under_333);
    /* End AC matching */
  }
  if(bitSet32_get(mask32,0)) {
    if(ACPattern)
      CUTOPEN(); /* AC matching */
    if(setChoicePoint()) {
      /* AC matching failed, try next rule */
      goto myend26;
    }
    if(ACPattern && MS_reinit(ms[0], (struct termac*)v0,0)>0 && MS_solve_rule(ms[0])>=0) {
      // do nothing 
    } else {
      fail();
    }
    if(1) {
      int nb_variable=1;
      int nb_variable_ac=1;
      int i;
      struct term *tmp, *sv[1];
      struct term *substitution[1];
      /* lhs: +(var0,([](0))) */
      /* To protect the term */
      substitution_build((struct termac*)v0,ms[0],nb_variable,substitution,nb_variable_ac,variable_extract_333,0);
      /* allDetEvaluation: det */
      if(ACPattern) { CUTCLOSE(); } /* AC matching */
      /* rhs: var0 */
      // this=var0        underAC=true        Instantiated=false
      sv[0]=substitution[0];
      /* --- re-normalisation */
      tmp = term_removeTopSymbol(sv[0]);
      if(tmp == NULL)
        sv[0]=fun_333( sv[0] );
      else
        sv[0]=tmp;
      res = sv[0] ;
      goto end;
    } else {
      fail();
    }
    myend26:;
    if(ACPattern)
      CUTCLOSE(); /* AC matching */
  }
match_fail:
  res=v0;
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

int match_subterm_333(struct term *v0,int no_arg_subject,int *mask,BG *cbg) {
  struct term *v1,*v2,*v3,*v4;
  int nb_bit=0;
  switch(getSymb(v0)) {
  case code_332: /*  */
    v2=v0->sub[0];
    switch(getSymb(v2)) {
    case code_301: /* [] */
      v3=v2->sub[0];
      switch(getInt(v3)) {
      case 0: /* 0 */
        mask[nb_bit++]=0;
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  return nb_bit;
}

void variable_extract_333(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern, int base_id_pattern) {
  switch(id_pattern) {
    /* ([](0)) */
  case 0:
    break;
  default:
    fprintf(stderr,"variable_extract_333: bad pattern number\n");
    exit(0);
  }
}

void init_pattern_list_333() {
  int pattern_tab[max_nb_pattern_under_333];
  no_pattern_333_niv_0=0;
  pattern_list_333=MS_pattern_list_create(nb_pattern_333_niv_0);
pattern_tab[0]=0;
MS_pattern_list_init(pattern_list_333,no_pattern_333_niv_0++,1,pattern_tab);
}

void delete_pattern_list_333() {
  MS_pattern_list_free(pattern_list_333,no_pattern_333_niv_0);
}
int match_subterm_334(struct term *v0,int no_arg_subject, int *mask, BG *cbg);
void variable_extract_334(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern, int base_id_pattern);
static int **pattern_list_334;
static int no_pattern_334_niv_0;
static int nb_pattern_334_niv_0 = 3;
static int nb_pattern_334_niv_1 = 3;
#define max_nb_pattern_under_334 2

struct term* fun_334(struct term *v0 ) {
  struct term *v1,*v2;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,3);
  bitSet32_init_clear(mask32);
  if(getArity(((struct termac*)v0))==1 && getMult(((struct termac*)v0),0)==1) {
    res=getSubterm(((struct termac*)v0),0);
    goto end_no_rewrite;
  }
  /* Begin syntactical matching */
  bitSet32_set(mask32,0);
  bitSet32_set(mask32,1);
  bitSet32_set(mask32,2);
  /* End syntactical matching */
  ms=(match_state**) MALLOC(1*sizeof(match_state*));
  if( bitSet32_get(mask32,0) || bitSet32_get(mask32,1) || bitSet32_get(mask32,2) ) {
    int necessary_link;
    ACPattern=1;
    /* Begin AC matching */
    necessary_link=0;
    indice = MS_init(&(ms[0]), match_subterm_334, no_pattern_334_niv_0, pattern_list_334, nb_pattern_334_niv_1, (struct termac*)v0, necessary_link, max_nb_pattern_under_334);
    /* End AC matching */
  }
  if(bitSet32_get(mask32,0)) {
    if(ACPattern)
      CUTOPEN(); /* AC matching */
    if(setChoicePoint()) {
      /* AC matching failed, try next rule */
      goto myend27;
    }
    if(ACPattern && MS_reinit(ms[0], (struct termac*)v0,0)>0 && MS_solve_rule(ms[0])>=0) {
      // do nothing 
    } else {
      fail();
    }
    if(1) {
      int nb_variable=1;
      int nb_variable_ac=1;
      int i;
      struct term *tmp, *sv[3];
      struct term *substitution[1];
      /* lhs: *(var0,([](0))) */
      /* To protect the term */
      substitution_build_without_context((struct termac*)v0,ms[0],nb_variable,substitution,nb_variable_ac,variable_extract_334,0);
      /* allDetEvaluation: det */
      if(ACPattern) { CUTCLOSE(); } /* AC matching */
      /* rhs: ([](0)) */
      TERM_ALLOC(sv[2],term1,code_301);
      sv[2]->sub[0] = (setIntegerTag(0));
      TERM_ALLOC(sv[1],term1,code_332);
      sv[1]->sub[0] = sv[2];
      res = sv[1] ;
      goto end;
    } else {
      fail();
    }
    myend27:;
    if(ACPattern)
      CUTCLOSE(); /* AC matching */
  }
  if(bitSet32_get(mask32,1)) {
    if(ACPattern)
      CUTOPEN(); /* AC matching */
    if(setChoicePoint()) {
      /* AC matching failed, try next rule */
      goto myend28;
    }
    if(ACPattern && MS_reinit(ms[0], (struct termac*)v0,1)>0 && MS_solve_rule(ms[0])>=0) {
      // do nothing 
    } else {
      fail();
    }
    if(1) {
      int nb_variable=1;
      int nb_variable_ac=1;
      int i;
      struct term *tmp, *sv[1];
      struct term *substitution[1];
      /* lhs: *(var0,([](1))) */
      /* To protect the term */
      substitution_build((struct termac*)v0,ms[0],nb_variable,substitution,nb_variable_ac,variable_extract_334,1);
      /* allDetEvaluation: det */
      if(ACPattern) { CUTCLOSE(); } /* AC matching */
      /* rhs: var0 */
      // this=var0        underAC=true        Instantiated=false
      sv[0]=substitution[0];
      /* --- re-normalisation */
      tmp = term_removeTopSymbol(sv[0]);
      if(tmp == NULL)
        sv[0]=fun_334( sv[0] );
      else
        sv[0]=tmp;
      res = sv[0] ;
      goto end;
    } else {
      fail();
    }
    myend28:;
    if(ACPattern)
      CUTCLOSE(); /* AC matching */
  }
  if(bitSet32_get(mask32,2)) {
    if(ACPattern)
      CUTOPEN(); /* AC matching */
    if(setChoicePoint()) {
      /* AC matching failed, try next rule */
      goto myend29;
    }
    if(ACPattern && MS_reinit(ms[0], (struct termac*)v0,2)>0 && MS_solve_rule(ms[0])>=0) {
      // do nothing 
    } else {
      fail();
    }
    if(1) {
      int nb_variable=3;
      int nb_variable_ac=1;
      int i;
      struct term *tmp, *sv[7];
      struct term *substitution[3];
      /* lhs: *(var3$,(var0),(var1)) */
      /* To protect the term */
      substitution_build((struct termac*)v0,ms[0],nb_variable,substitution,nb_variable_ac,variable_extract_334,2);
      /* allDetEvaluation: det */
      /* where var2 := time(,)(var0,var1) */
      // this=var0        underAC=true        Instantiated=false
      sv[1]=substitution[1];
      // this=var1        underAC=true        Instantiated=false
      sv[2]=substitution[2];
      sv[4] = fun_304( sv[1],sv[2] );
      tmp = sv[3] = sv[4];
      if(ACPattern) { CUTCLOSE(); } /* AC matching */
      /* rhs: *(var3$,(var2)) */
      // this=var3$        underAC=true        Instantiated=false
      sv[0]=substitution[0];
      // this=var2        underAC=false        Instantiated=false
      TERM_ALLOC(sv[5],term1,code_332);
      sv[5]->sub[0] = sv[3];
      sv[6] = NULL;
      sv[6] = (struct term*)term_add_onf_term(sv[6],code_334,sv[0]);
      sv[6] = (struct term*)term_add_onf_term(sv[6],code_334,sv[5]);
      sv[6] = fun_334( sv[6] );
      res = sv[6] ;
      goto end;
    } else {
      fail();
    }
    myend29:;
    if(ACPattern)
      CUTCLOSE(); /* AC matching */
  }
match_fail:
  res=v0;
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

int match_subterm_334(struct term *v0,int no_arg_subject,int *mask,BG *cbg) {
  struct term *v1,*v2,*v3,*v4;
  int nb_bit=0;
  switch(getSymb(v0)) {
  case code_332: /*  */
    v2=v0->sub[0];
    switch(getSymb(v2)) {
    case code_301: /* [] */
      v3=v2->sub[0];
      switch(getInt(v3)) {
      case 1: /* 1 */
        mask[nb_bit++]=1;
        mask[nb_bit++]=2;
        break;
      case 0: /* 0 */
        mask[nb_bit++]=0;
        mask[nb_bit++]=2;
        break;
      default:
        goto label137;
      }
      break;
    default:
    label137:
      mask[nb_bit++]=2;
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  return nb_bit;
}

void variable_extract_334(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern, int base_id_pattern) {
  switch(id_pattern) {
    /* ([](0)) */
  case 0:
    break;
    /* ([](1)) */
  case 1:
    break;
    /* (var0) */
  case 2:
    extract_substitution[*indice]=v0->sub[0];
    (*indice)++;
    break;
  default:
    fprintf(stderr,"variable_extract_334: bad pattern number\n");
    exit(0);
  }
}

void init_pattern_list_334() {
  int pattern_tab[max_nb_pattern_under_334];
  no_pattern_334_niv_0=0;
  pattern_list_334=MS_pattern_list_create(nb_pattern_334_niv_0);
pattern_tab[0]=0;
MS_pattern_list_init(pattern_list_334,no_pattern_334_niv_0++,1,pattern_tab);
pattern_tab[0]=1;
MS_pattern_list_init(pattern_list_334,no_pattern_334_niv_0++,1,pattern_tab);
pattern_tab[0]=2;
pattern_tab[1]=2;
MS_pattern_list_init(pattern_list_334,no_pattern_334_niv_0++,2,pattern_tab);
}

void delete_pattern_list_334() {
  MS_pattern_list_free(pattern_list_334,no_pattern_334_niv_0);
}

struct term* fun_335(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11;
  struct term *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,5);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_334: /* * */
    switch(getSymb(v2)) {
    default:
    label152:
      bitSet32_set(mask32,4);
    }
    break;
  case code_333: /* + */
    switch(getSymb(v2)) {
    default:
    label150:
      bitSet32_set(mask32,3);
    }
    break;
  case code_332: /*  */
    v7=v1->sub[0];
    switch(getSymb(v7)) {
    default:
    label147:
      switch(getSymb(v2)) {
      default:
      label148:
        bitSet32_set(mask32,2);
      }
    }
    break;
  case code_331: /*  */
    v9=v1->sub[0];
    switch(getSymb(v9)) {
    default:
    label144:
      switch(getSymb(v2)) {
      default:
      label145:
        bitSet32_set(mask32,0);
        bitSet32_set(mask32,1);
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  ms=(match_state**) MALLOC(1*sizeof(match_state*));
  if( bitSet32_get(mask32,3) || bitSet32_get(mask32,4) ) {
    int necessary_link;
    ACPattern=1;
  }
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[3];
    struct term *substitution[1];
    /* lhs: deriv(,)((var0),var1) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend30;
    }
    /* if eq_variable(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_18( v9,v2 );
    if( sv[0] != con_1 ) {
      fail();
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: ([](1)) */
    TERM_ALLOC(sv[2],term1,code_301);
    sv[2]->sub[0] = (setIntegerTag(1));
    TERM_ALLOC(sv[1],term1,code_332);
    sv[1]->sub[0] = sv[2];
    res = sv[1] ;
    goto end;
    myend30:;
    CUTCLOSE(); /* Wheres */
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[3];
    struct term *substitution[1];
    /* lhs: deriv(,)((var0),var1) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend31;
    }
    /* if neq_variable(,)(var1,var0) */
    // this=var1        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_19( v2,v9 );
    if( sv[0] != con_1 ) {
      fail();
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: ([](0)) */
    TERM_ALLOC(sv[2],term1,code_301);
    sv[2]->sub[0] = (setIntegerTag(0));
    TERM_ALLOC(sv[1],term1,code_332);
    sv[1]->sub[0] = sv[2];
    res = sv[1] ;
    goto end;
    myend31:;
    CUTCLOSE(); /* Wheres */
  }
  if(bitSet32_get(mask32,2)) {
    struct term *tmp, *sv[2];
    struct term *substitution[1];
    /* lhs: deriv(,)((var0),var1) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend32;
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: ([](0)) */
    TERM_ALLOC(sv[1],term1,code_301);
    sv[1]->sub[0] = (setIntegerTag(0));
    TERM_ALLOC(sv[0],term1,code_332);
    sv[0]->sub[0] = sv[1];
    res = sv[0] ;
    goto end;
    myend32:;
    CUTCLOSE(); /* Wheres */
  }
  if(bitSet32_get(mask32,3)) {
    if(ACPattern)
      CUTOPEN(); /* AC matching */
    if(setChoicePoint()) {
      /* AC matching failed, try next rule */
      goto myend33;
    }
    if(1) {
      int nb_variable=3;
      int nb_variable_ac=2;
      int i;
      struct term *tmp, *sv[5];
      struct term *substitution[2];
      /* lhs: deriv(,)(+(var0,var1),var2) */
      for(i=0 ; i<sizeof(substitution)/sizeof(struct term*) ; i++) {
        substitution[i]=v1;
      }
      sv[0]=substitution[0];
      sv[1]=substitution[1];
      {
        multiplicityType *E,*sol;
        int nb_arg_subject;
        int no_arg_subject;
        int indice=0;
        int i;
        struct termac *tac=(struct termac*)sv[0];
        E=(multiplicityType*)AMALLOC((1+getArity(tac))*sizeof(multiplicityType));
        sol=(multiplicityType*)AMALLOC((1+getArity(tac))*sizeof(multiplicityType));
        for(i=0 ; i<getArity(tac) ; i++) {
          E[i]=getMult(tac,i) - (getMult(tac,i)%1);
          sol[i]=0;
        }
        sol[getArity(tac)]=0; /* total */
        while(i=next_minimal_extract(getArity(tac),E,sol,1)) {
          if(!setChoicePoint()) {
            break;
          }
        }
        if(i) {
          struct termac *list_x = NULL;
          struct termac *list_y = NULL;
          extract_xy_from_pe((struct termac*)tac,E,sol,1,&list_x,&list_y);
          sv[0] = (struct term*)list_x;
          sv[1] = (struct term*)list_y;
          // printf("list_x = sv[0] = ");           internal_term_println(stdout,sv[0],resultMode);
          // printf("list_y = sv[1] = ");           internal_term_println(stdout,sv[1],resultMode);
          if(getArity(((struct termac*)sv[1]))==0) {
            /* Avoid an empty context */
            fail();
          }
        } else {
          /*  There is no more solution */
          fail();
        }
      }
      /* allDetEvaluation: det */
      if(ACPattern) { CUTCLOSE(); } /* AC matching */
      /* rhs: +(deriv(,)(var0,var2),deriv(,)(var1,var2)) */
      // this=var0        underAC=true        Instantiated=true
      tmp = term_removeTopSymbol(sv[0]);
      if(tmp != NULL)
        sv[0]=tmp;
      // this=var2        underAC=false        Instantiated=false
      sv[2] = fun_335( sv[0],v2 );
      // this=var1        underAC=true        Instantiated=true
      tmp = term_removeTopSymbol(sv[1]);
      if(tmp != NULL)
        sv[1]=tmp;
      // this=var2        underAC=false        Instantiated=false
      sv[3] = fun_335( sv[1],v2 );
      sv[4] = NULL;
      sv[4] = (struct term*)term_add_onf_term(sv[4],code_333,sv[2]);
      sv[4] = (struct term*)term_add_onf_term(sv[4],code_333,sv[3]);
      sv[4] = fun_333( sv[4] );
      res = sv[4] ;
      goto end;
    } else {
      fail();
    }
    myend33:;
    if(ACPattern)
      CUTCLOSE(); /* AC matching */
  }
  if(bitSet32_get(mask32,4)) {
    if(ACPattern)
      CUTOPEN(); /* AC matching */
    if(setChoicePoint()) {
      /* AC matching failed, try next rule */
      goto myend34;
    }
    if(1) {
      int nb_variable=3;
      int nb_variable_ac=2;
      int i;
      struct term *tmp, *sv[5];
      struct term *substitution[2];
      /* lhs: deriv(,)(*(var0,var1),var2) */
      for(i=0 ; i<sizeof(substitution)/sizeof(struct term*) ; i++) {
        substitution[i]=v1;
      }
      sv[0]=substitution[0];
      sv[1]=substitution[1];
      {
        multiplicityType *E,*sol;
        int nb_arg_subject;
        int no_arg_subject;
        int indice=0;
        int i;
        struct termac *tac=(struct termac*)sv[0];
        E=(multiplicityType*)AMALLOC((1+getArity(tac))*sizeof(multiplicityType));
        sol=(multiplicityType*)AMALLOC((1+getArity(tac))*sizeof(multiplicityType));
        for(i=0 ; i<getArity(tac) ; i++) {
          E[i]=getMult(tac,i) - (getMult(tac,i)%1);
          sol[i]=0;
        }
        sol[getArity(tac)]=0; /* total */
        while(i=next_minimal_extract(getArity(tac),E,sol,1)) {
          if(!setChoicePoint()) {
            break;
          }
        }
        if(i) {
          struct termac *list_x = NULL;
          struct termac *list_y = NULL;
          extract_xy_from_pe((struct termac*)tac,E,sol,1,&list_x,&list_y);
          sv[0] = (struct term*)list_x;
          sv[1] = (struct term*)list_y;
          // printf("list_x = sv[0] = ");           internal_term_println(stdout,sv[0],resultMode);
          // printf("list_y = sv[1] = ");           internal_term_println(stdout,sv[1],resultMode);
          if(getArity(((struct termac*)sv[1]))==0) {
            /* Avoid an empty context */
            fail();
          }
        } else {
          /*  There is no more solution */
          fail();
        }
      }
      /* allDetEvaluation: det */
      if(ACPattern) { CUTCLOSE(); } /* AC matching */
      /* rhs: +(*(var0,deriv(,)(var1,var2)),*(var1,deriv(,)(var0,var2))) */
      // this=var0        underAC=true        Instantiated=true
      // this=var1        underAC=true        Instantiated=true
      tmp = term_removeTopSymbol(sv[1]);
      if(tmp != NULL)
        sv[1]=tmp;
      // this=var2        underAC=false        Instantiated=false
      sv[2] = fun_335( sv[1],v2 );
      sv[3] = NULL;
      sv[3] = (struct term*)term_add_onf_term(sv[3],code_334,sv[0]);
      sv[3] = (struct term*)term_add_onf_term(sv[3],code_334,sv[2]);
      sv[3] = fun_334( sv[3] );
      // this=var1        underAC=true        Instantiated=true
      // this=var0        underAC=true        Instantiated=true
      tmp = term_removeTopSymbol(sv[0]);
      if(tmp != NULL)
        sv[0]=tmp;
      // this=var2        underAC=false        Instantiated=false
      sv[2] = fun_335( sv[0],v2 );
      sv[4] = NULL;
      sv[4] = (struct term*)term_add_onf_term(sv[4],code_334,sv[1]);
      sv[4] = (struct term*)term_add_onf_term(sv[4],code_334,sv[2]);
      sv[4] = fun_334( sv[4] );
      sv[2] = NULL;
      sv[2] = (struct term*)term_add_onf_term(sv[2],code_333,sv[3]);
      sv[2] = (struct term*)term_add_onf_term(sv[2],code_333,sv[4]);
      sv[2] = fun_333( sv[2] );
      res = sv[2] ;
      goto end;
    } else {
      fail();
    }
    myend34:;
    if(ACPattern)
      CUTCLOSE(); /* AC matching */
  }
match_fail:
  TERM_ALLOC(res,term2, 335);
  res->sub[0] = v1;
  res->sub[1] = v2;
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

struct term* str_26( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  /* cons[nonDet](iterate[multiDet](one[semiDet](extractrule2:identifier/list[identifier]!LO)),one[semiDet](extractrule1:identifier/list[identifier]!LO)) */
  /* iterate[multiDet](one[semiDet](extractrule2:identifier/list[identifier]!LO)) */
  while(1) {
    if(!setChoicePoint()) {
      break;
    }
    /* Apply the strategy */
    {
      /* one[semiDet](extractrule2:identifier/list[identifier]!LO) */
      struct term *v1,*v2,*v3,*v4,*v5;
      bitSet_GC_create(mask,1);
      bitSet_init_clear(mask);
      switch(getSymb(v0)) {
      case code_322: /* elem() */
        v2=v0->sub[0];
        switch(getSymb(v2)) {
        case code_320: /* , */
          v3=v2->sub[0];
          switch(getSymb(v3)) {
          default:
          label156:
            v4=v2->sub[1];
            switch(getSymb(v4)) {
            default:
            label157:
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
        struct term *substitution[1];
        long tmp_step;
        /* lhs: elem()(,(var0,var1)) */
        /* allDetEvaluation: det */
        CUTOPEN(); /* Wheres */
        if(setChoicePoint()) {
          /* local evaluations failed, try next rule */
          goto myend35;
        }
        CUTCLOSE(); /* Wheres */
        /* rhs: elem()(var1) */
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[0],term1,code_322);
        sv[0]->sub[0] = v4;
        res = sv[0] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab10;
        myend35:;
        CUTCLOSE(); /* Wheres */
      }
    }
    fail();
    stratLab10:;
    v0=res;
  }
  {
    /* one[semiDet](extractrule1:identifier/list[identifier]!LO) */
    struct term *v1,*v2,*v3,*v4,*v5;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(getSymb(v0)) {
    case code_322: /* elem() */
      v2=v0->sub[0];
      switch(getSymb(v2)) {
      case code_320: /* , */
        v3=v2->sub[0];
        switch(getSymb(v3)) {
        default:
        label161:
          v4=v2->sub[1];
          switch(getSymb(v4)) {
          default:
          label162:
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
      struct term *substitution[1];
      long tmp_step;
      /* lhs: elem()(,(var0,var1)) */
      /* allDetEvaluation: det */
      CUTOPEN(); /* Wheres */
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend36;
      }
      CUTCLOSE(); /* Wheres */
      /* rhs: var0 */
      // this=var0        underAC=false        Instantiated=false
      res = v3 ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab13;
      myend36:;
      CUTCLOSE(); /* Wheres */
    }
  }
  fail();
  stratLab13:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}
int match_subterm_413_tsome1_dk_334(struct term *v0,int no_arg_subject, int *mask, BG *cbg);
void variable_extract_413_tsome1_dk_334(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern, int base_id_pattern);
static int **pattern_list_413_tsome1_dk_334;
static int no_pattern_413_tsome1_dk_334_niv_0;
static int nb_pattern_413_tsome1_dk_334_niv_0 = 2;
static int nb_pattern_413_tsome1_dk_334_niv_1 = 1;
#define max_nb_pattern_under_413_tsome1_dk_334 1

int match_subterm_413_tsome1_dk_334(struct term *v0,int no_arg_subject,int *mask,BG *cbg) {
            struct term *v1,*v2;
            int nb_bit=0;
            switch(getSymb(v0)) {
            case code_333: /* + */
              /* AC case: Not tested */
              mask[nb_bit++]=0;
              break;
            /* matching is not complete: jumpNode is null */
            }
            return nb_bit;
}
          
void variable_extract_413_tsome1_dk_334(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern, int base_id_pattern) {
            switch(id_pattern) {
              /* +(var0,var1) */
            case 0:
              {
                /* Not tested */
                int nb_variable=2;
                int nb_variable_ac=2;
                struct term *substitution[2];
                LINK *link;
                match_state *msbg;
                int i;
                //link=BG_link_get(ms->cbg,base_id_pattern+id_pattern);
                if(1 || link==NULL) {
                  for(i=0 ; i<nb_variable ; i++) {
                    extract_substitution[*indice]=v0;
                    (*indice)++;
                  }
                } else {
                  msbg=LINK_get(link,no_arg_subject);
                  substitution_build((struct termac*)v0,msbg,nb_variable,substitution,nb_variable_ac,variable_extract_413_tsome1_dk_334,base_id_pattern);
                  for(i=0 ; i <nb_variable ; i++) {
                    extract_substitution[*indice]=substitution[i];
                    (*indice)++;
                  }
                }
              }
              break;
            default:
              fprintf(stderr,"variable_extract_413_tsome1_dk_334: bad pattern number\n");
              exit(0);
            }
          }
          
void init_pattern_list_413_tsome1_dk_334() {
            int pattern_tab[max_nb_pattern_under_413_tsome1_dk_334];
            no_pattern_413_tsome1_dk_334_niv_0=0;
            pattern_list_413_tsome1_dk_334=MS_pattern_list_create(nb_pattern_413_tsome1_dk_334_niv_0);
          pattern_tab[0]=0;
          MS_pattern_list_init(pattern_list_413_tsome1_dk_334,no_pattern_413_tsome1_dk_334_niv_0++,1,pattern_tab);
          no_pattern_413_tsome1_dk_334_niv_0++;
          }
          
void delete_pattern_list_413_tsome1_dk_334() {
            MS_pattern_list_free(pattern_list_413_tsome1_dk_334,no_pattern_413_tsome1_dk_334_niv_0);
          }

struct term* str_413( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  {
    /* compilation of tsome(dk[nonDet](expand:poly/poly5[Vars]!GL)) */
    int i,j,mult,arity;
    struct term *newTerm;
    int localWasr=0;
    arg0=res;
    arity = genericGetArity(arg0);
    if(arity!=0) {
      genericTermAlloc(newTerm,arity,getSymb(arg0));
    }
    for(i=0 ; i<arity ; i++) {
      v0 = genericGetSubterm(arg0,i);
      mult = genericGetMult(arg0,i);
      printf("v0 = "); internal_term_println(stdout,v0,resultMode);
      if(setChoicePoint()) {
        printf("copy v0 = "); internal_term_println(stdout,v0,resultMode);
        for(j=0 ; j<mult ; j++) {
          genericSetSubterm(newTerm,i,v0);
        }
      } else {

          /* begin dk[nonDet](expand:poly/poly5[Vars]!GL) */
        {
          /* dk[nonDet](expand:poly/poly5[Vars]!GL) */
          struct term *v1,*v2;
          match_state **ms=NULL;
          int necessary_link;
          int indice=-1;
          int ACPattern=0;
          bitSet_GC_create(mask,2);
          bitSet_init_clear(mask);
          switch(getSymb(v0)) {
          case code_334: /* * */
            bitSet_set(mask,0);
            bitSet_set(mask,1);
            break;
          /* matching is not complete: jumpNode is null */
          }
          ms=(match_state**) MALLOC(1*sizeof(match_state*));
          if( bitSet_get(mask,0) || bitSet_get(mask,1) ) {
            int necessary_link;
            ACPattern=1;
            /* Begin AC matching */
            necessary_link=0;
            indice = MS_init(&(ms[0]), match_subterm_413_tsome1_dk_334, no_pattern_413_tsome1_dk_334_niv_0, pattern_list_413_tsome1_dk_334, nb_pattern_413_tsome1_dk_334_niv_1, (struct termac*)v0, necessary_link, max_nb_pattern_under_413_tsome1_dk_334);
            /* End AC matching */
          }
          if(bitSet_get(mask,1)) {
            if(setChoicePoint()) {
              /* AC matching failed, try next rule */
              printf("chp 1\n");
              goto myend38;
            }
            if(ACPattern && MS_reinit(ms[0], (struct termac*)v0,0)>0) {
              while(1) {
                if(MS_solve_rule(ms[0])<0) {
                  fail();
                }
                /* choicePoint AC matching */
                if(!setChoicePoint()) {
                  break;
                } else {
                  printf("chp 2\n");
                }
              }
            } else {
              fail();
            }
            if(1) {
              int nb_variable=3;
              int nb_variable_ac=1;
              int i;
              struct term *tmp, *sv[6];
              struct term *substitution[3];
              /* lhs: *(var2,+(var0,var1)) */
              
              /* To protect the term */
              substitution_build((struct termac*)v0,ms[0],nb_variable,substitution,nb_variable_ac,variable_extract_413_tsome1_dk_334,0);
              sv[1]=substitution[1];
              sv[2]=substitution[2];
              {
                multiplicityType *E,*sol;
                int nb_arg_subject;
                int no_arg_subject;
                int indice=0;
                int i;
                struct termac *tac=(struct termac*)sv[1];
                E=(multiplicityType*)AMALLOC((1+getArity(tac))*sizeof(multiplicityType));
                sol=(multiplicityType*)AMALLOC((1+getArity(tac))*sizeof(multiplicityType));
                for(i=0 ; i<getArity(tac) ; i++) {
                  E[i]=getMult(tac,i) - (getMult(tac,i)%1);
                  sol[i]=0;
                }
                sol[getArity(tac)]=0; /* total */
                while(i=next_minimal_extract(getArity(tac),E,sol,1)) {

                  {
                    int i1;
                    printf("E   = ");
                    for(i1=0 ; i1<getArity(tac) ; i1++) {
                      printf("%d ",E[i1]);
                    }
                    printf("\nsol = ");
                    for(i1=0 ; i1<getArity(tac) ; i1++) {
                      printf("%d ",sol[i1]);
                    }
                    printf("\n");

                  }

                  
                  if(!setChoicePoint()) {
                    break;
                  } else {
                    printf("chp 3\n");
                  }
                }
                if(i) {
                  struct termac *list_x = NULL;
                  struct termac *list_y = NULL;
                  extract_xy_from_pe((struct termac*)tac,E,sol,1,&list_x,&list_y);
                  sv[1] = (struct term*)list_x;
                  sv[2] = (struct term*)list_y;
                  if(getArity(((struct termac*)sv[2]))==0) {
                    /* Avoid an empty context */
                    fail();
                  }
                  printf("list_x = sv[1] = ");                   internal_term_println(stdout,sv[1],resultMode);
                  printf("list_y = sv[2] = ");                   internal_term_println(stdout,sv[2],resultMode);

                } else {
                  /*  There is no more solution */
                  fail();
                }
              }
              /* allDetEvaluation: det */
              /* rhs: +(*(var0,var2),*(var1,var2)) */
              // this=var0        underAC=true        Instantiated=true
              tmp = term_removeTopSymbol(sv[1]);
              if(tmp != NULL)
                sv[1]=tmp;
              // this=var2        underAC=true        Instantiated=false
              sv[0]=substitution[0];
                printf("list_z = sv[0] = ");                   internal_term_println(stdout,sv[0],resultMode);
              /* --- re-normalisation */
              tmp = term_removeTopSymbol(sv[0]);
              if(tmp == NULL)
                sv[0]=fun_334( sv[0] );
              else
                sv[0]=tmp;
              sv[3] = NULL;
              sv[3] = (struct term*)term_add_onf_term(sv[3],code_334,sv[1]);
              sv[3] = (struct term*)term_add_onf_term(sv[3],code_334,sv[0]);
              sv[3] = fun_334( sv[3] );
              // this=var1        underAC=true        Instantiated=true
              tmp = term_removeTopSymbol(sv[2]);
              if(tmp != NULL)
                sv[2]=tmp;
              // this=var2        underAC=true        Instantiated=true
              sv[4] = NULL;
              sv[4] = (struct term*)term_add_onf_term(sv[4],code_334,sv[2]);
              sv[4] = (struct term*)term_add_onf_term(sv[4],code_334,sv[0]);
              sv[4] = fun_334( sv[4] );
              sv[5] = NULL;
              sv[5] = (struct term*)term_add_onf_term(sv[5],code_333,sv[3]);
              sv[5] = (struct term*)term_add_onf_term(sv[5],code_333,sv[4]);
              sv[5] = fun_333( sv[5] );
              res = sv[5] ;
            rewrite_step++;
              goto stratLab32;
            } else {
              fail();
            }
            myend38:;
          }
        }
        fail();
        stratLab32:;
        v0=res;
        /* end   dk[nonDet](expand:poly/poly5[Vars]!GL) */

        
        localWasr=1;
        printf("res = "); internal_term_println(stdout,res,resultMode);
        for(j=0 ; j<mult ; j++) {
          genericSetSubterm(newTerm,i,res);
        }
      } // choice
    } // for
    if(localWasr) {
      res = specialApply(newTerm);
    } else {
      fail();
    }
  }
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_240( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  {
    /* ONE[det](tone[det](repeat[det](one[semiDet](factorize:poly/poly5[Vars]!GL)))) */
    CUTOPEN(); /* DC v2 */
    {
      /* compilation of tone(repeat[det](one[semiDet](factorize:poly/poly5[Vars]!GL))) */
      int i,j,mult,arity;
      struct term *newTerm;
      arg0=res;
      arity = genericGetArity(arg0);
      for(i=0 ; i<arity ; i++) {
        v0 = genericGetSubterm(arg0,i);
        mult = genericGetMult(arg0,i);
        //printf("v0 = "); internal_term_println(stdout,v0,resultMode);
        if(setChoicePoint()) {
          /* la strategie a echoue: on essaie le fils suivant */
        } else {
          /* begin repeat[det](one[semiDet](factorize:poly/poly5[Vars]!GL)) */
          {
            /* repeat[det](one[semiDet](factorize:poly/poly5[Vars]!GL)) */
            struct term **lastTerm=(struct term **) allocStable(sizeof(struct term*));
            *lastTerm=v0;
            if(setChoicePoint()!=0) {
              res = *lastTerm;
              v0 = *lastTerm;
              /* End of repeat */
            } else {
              while(1) {
              /* Apply the strategy */
                {
                  /* one[semiDet](factorize:poly/poly5[Vars]!GL) */
                  struct term *v1,*v2;
                  match_state **ms=NULL;
                  int necessary_link;
                  int indice=-1;
                  int ACPattern=0;
                  bitSet_GC_create(mask,2);
                  bitSet_init_clear(mask);
                  switch(getSymb(v0)) {
                  case code_333: /* + */
                    bitSet_set(mask,0);
                    bitSet_set(mask,1);
                    break;
                  /* matching is not complete: jumpNode is null */
                  }
                  ms=(match_state**) MALLOC(1*sizeof(match_state*));
                  if( bitSet_get(mask,0) || bitSet_get(mask,1) ) {
                    int necessary_link;
                    ACPattern=1;
                  }
                  if(bitSet_get(mask,0)) {
                    if(ACPattern)
                      CUTOPEN(); /* AC matching */
                    if(setChoicePoint()) {
                      /* AC matching failed, try next rule */
                      goto myend39;
                    }
                    if(1) {
                      int nb_variable=2;
                      int nb_variable_ac=2;
                      int i;
                      struct term *tmp, *sv[19];
                      struct term *substitution[6];
                      /* lhs: +(var4,var6) */
                      for(i=0 ; i<sizeof(substitution)/sizeof(struct term*) ; i++) {
                        substitution[i]=v0;
                      }
                      sv[0]=substitution[0];
                      sv[1]=substitution[1];
                      {
                        multiplicityType *E,*sol;
                        int nb_arg_subject;
                        int no_arg_subject;
                        int indice=0;
                        int i;
                        struct termac *tac=(struct termac*)sv[0];
                        E=(multiplicityType*)AMALLOC((1+getArity(tac))*sizeof(multiplicityType));
                        sol=(multiplicityType*)AMALLOC((1+getArity(tac))*sizeof(multiplicityType));
                        for(i=0 ; i<getArity(tac) ; i++) {
                          E[i]=getMult(tac,i) - (getMult(tac,i)%1);
                          sol[i]=0;
                        }
                        sol[getArity(tac)]=0; /* total */
                        while(i=next_minimal_extract(getArity(tac),E,sol,1)) {
                          if(!setChoicePoint()) {
                            break;
                          }
                        }
                        if(i) {
                          struct termac *list_x = NULL;
                          struct termac *list_y = NULL;
                          extract_xy_from_pe((struct termac*)tac,E,sol,1,&list_x,&list_y);
                          sv[0] = (struct term*)list_x;
                          sv[1] = (struct term*)list_y;
                          // printf("list_x = sv[0] = ");                           internal_term_println(stdout,sv[0],resultMode);
                          // printf("list_y = sv[1] = ");                           internal_term_println(stdout,sv[1],resultMode);
                          if(getArity(((struct termac*)sv[1]))==0) {
                            /* Avoid an empty context */
                            fail();
                          }
                        } else {
                          /*  There is no more solution */
                          fail();
                        }
                      }
                      /* allDetEvaluation: nonDet */
                      /* where *(var1,(var0)) := var6 */
                      // this=var6        underAC=true        Instantiated=true
                      /* --- re-normalisation */
                      tmp = term_removeTopSymbol(sv[1]);
                      if(tmp == NULL)
                        sv[1]=fun_333( sv[1] );
                      else
                        sv[1]=tmp;
                      tmp = sv[7] = sv[1];
                      {
                        match_state *ACmatch;
                        /* TODO: modifier la taille du tableau */
                        TERM *assignment[2];
                        int i;
                        for(i=0 ; i<2 ; i++) assignment[i]=NULL;
                        /* pattern number 2 */
                        sv[7] = (struct term*) EkerTerm[2];
                        /* TODO: modifier le nb de variables */
                        tmp = (struct term*)toEkerForm(tmp);
                        ac_sort((TERM*)tmp);
                        /*
                        printf("sv  = "); eker_print_term((TERM*)sv[7]);
                        printf("\n");
                        printf("tmp  = "); eker_print_term((TERM*)tmp);
                        printf("\n");
                        printf("\n");
                        */
                        ACmatch = build_match((TERM*)sv[7],(TERM*)tmp,2);
                        while(1) {
                          if(!extract_match(ACmatch, assignment)) {
                            destroy_match(ACmatch);
                            fail();
                          }
                          /* choicePoint Eker AC matching */
                          if(!setChoicePoint()) {
                            break;
                          }
                        }
                        substitution[2]=fromEkerForm(assignment[0]);
                        destroy_term(assignment[0]);
                        substitution[3]=fromEkerForm(assignment[1]);
                        destroy_term(assignment[1]);
                        sv[6]=substitution[2];
                        sv[4]=substitution[3];
                      }
                      /* where *(var5,(var2)) := var4 */
                      // this=var4        underAC=true        Instantiated=true
                      /* --- re-normalisation */
                      tmp = term_removeTopSymbol(sv[0]);
                      if(tmp == NULL)
                        sv[0]=fun_333( sv[0] );
                      else
                        sv[0]=tmp;
                      tmp = sv[13] = sv[0];
                      {
                        match_state *ACmatch;
                        /* TODO: modifier la taille du tableau */
                        TERM *assignment[2];
                        int i;
                        for(i=0 ; i<2 ; i++) assignment[i]=NULL;
                        /* pattern number 3 */
                        sv[13] = (struct term*) EkerTerm[3];
                        /* TODO: modifier le nb de variables */
                        tmp = (struct term*)toEkerForm(tmp);
                        ac_sort((TERM*)tmp);
                        /*
                        printf("sv  = "); eker_print_term((TERM*)sv[13]);
                        printf("\n");
                        printf("tmp  = "); eker_print_term((TERM*)tmp);
                        printf("\n");
                        printf("\n");
                        */
                        ACmatch = build_match((TERM*)sv[13],(TERM*)tmp,2);
                        while(1) {
                          if(!extract_match(ACmatch, assignment)) {
                            destroy_match(ACmatch);
                            fail();
                          }
                          /* choicePoint Eker AC matching */
                          if(!setChoicePoint()) {
                            break;
                          }
                        }
                        substitution[4]=fromEkerForm(assignment[0]);
                        destroy_term(assignment[0]);
                        substitution[5]=fromEkerForm(assignment[1]);
                        destroy_term(assignment[1]);
                        sv[12]=substitution[4];
                        sv[10]=substitution[5];
                      }
                      /* if eq_variable(,)(var1,var5) */
                      // this=var1        underAC=true        Instantiated=true
                      /* --- re-normalisation */
                      tmp = term_removeTopSymbol(sv[6]);
                      if(tmp == NULL)
                        sv[6]=fun_334( sv[6] );
                      else
                        sv[6]=tmp;
                      // this=var5        underAC=true        Instantiated=true
                      /* --- re-normalisation */
                      tmp = term_removeTopSymbol(sv[12]);
                      if(tmp == NULL)
                        sv[12]=fun_334( sv[12] );
                      else
                        sv[12]=tmp;
                      sv[14] = fun_18( sv[6],sv[12] );
                      if( sv[14] != con_1 ) {
                        fail();
                      }
                      /* where var3 := plus(,)(var0,var2) */
                      // this=var0        underAC=true        Instantiated=true
                      // this=var2        underAC=true        Instantiated=true
                      sv[16] = fun_302( sv[4],sv[10] );
                      tmp = sv[15] = sv[16];
                      if(ACPattern) { CUTCLOSE(); } /* AC matching */
                      /* rhs: *(var1,(var3)) */
                      // this=var1        underAC=true        Instantiated=true
                      // this=var3        underAC=false        Instantiated=false
                      term_alloc(&sv[17],sizeof(struct term1),code_332);
                      sv[17]->sub[0] = sv[15];
                      sv[18] = NULL;
                      sv[18] = (struct term*)term_add_onf_term(sv[18],code_334,sv[6]);
                      sv[18] = (struct term*)term_add_onf_term(sv[18],code_334,sv[17]);
                      sv[18] = fun_334( sv[18] );
                      res = sv[18] ;
                    rewrite_step++;
                      goto stratLab19;
                    } else {
                      fail();
                    }
                    myend39:;
                    if(ACPattern)
                      CUTCLOSE(); /* AC matching */
                  }
                  if(bitSet_get(mask,1)) {
                    if(ACPattern)
                      CUTOPEN(); /* AC matching */
                    if(setChoicePoint()) {
                      /* AC matching failed, try next rule */
                      goto myend40;
                    }
                    if(1) {
                      int nb_variable=2;
                      int nb_variable_ac=2;
                      int i;
                      struct term *tmp, *sv[15];
                      struct term *substitution[6];
                      /* lhs: +(var3,var5) */
                      for(i=0 ; i<sizeof(substitution)/sizeof(struct term*) ; i++) {
                        substitution[i]=v0;
                      }
                      sv[0]=substitution[0];
                      sv[1]=substitution[1];
                      {
                        multiplicityType *E,*sol;
                        int nb_arg_subject;
                        int no_arg_subject;
                        int indice=0;
                        int i;
                        struct termac *tac=(struct termac*)sv[0];
                        E=(multiplicityType*)AMALLOC((1+getArity(tac))*sizeof(multiplicityType));
                        sol=(multiplicityType*)AMALLOC((1+getArity(tac))*sizeof(multiplicityType));
                        for(i=0 ; i<getArity(tac) ; i++) {
                          E[i]=getMult(tac,i) - (getMult(tac,i)%1);
                          sol[i]=0;
                        }
                        sol[getArity(tac)]=0; /* total */
                        while(i=next_minimal_extract(getArity(tac),E,sol,1)) {
                          if(!setChoicePoint()) {
                            break;
                          }
                        }
                        if(i) {
                          struct termac *list_x = NULL;
                          struct termac *list_y = NULL;
                          extract_xy_from_pe((struct termac*)tac,E,sol,1,&list_x,&list_y);
                          sv[0] = (struct term*)list_x;
                          sv[1] = (struct term*)list_y;
                          // printf("list_x = sv[0] = ");                           internal_term_println(stdout,sv[0],resultMode);
                          // printf("list_y = sv[1] = ");                           internal_term_println(stdout,sv[1],resultMode);
                          if(getArity(((struct termac*)sv[1]))==0) {
                            /* Avoid an empty context */
                            fail();
                          }
                        } else {
                          /*  There is no more solution */
                          fail();
                        }
                      }
                      /* allDetEvaluation: nonDet */
                      /* where *(var0,var1) := var5 */
                      // this=var5        underAC=true        Instantiated=true
                      /* --- re-normalisation */
                      tmp = term_removeTopSymbol(sv[1]);
                      if(tmp == NULL)
                        sv[1]=fun_333( sv[1] );
                      else
                        sv[1]=tmp;
                      tmp = sv[6] = sv[1];
                      {
                        match_state *ACmatch;
                        /* TODO: modifier la taille du tableau */
                        TERM *assignment[2];
                        int i;
                        for(i=0 ; i<2 ; i++) assignment[i]=NULL;
                        /* pattern number 4 */
                        sv[6] = (struct term*) EkerTerm[4];
                        /* TODO: modifier le nb de variables */
                        tmp = (struct term*)toEkerForm(tmp);
                        ac_sort((TERM*)tmp);
                        /*
                        printf("sv  = "); eker_print_term((TERM*)sv[6]);
                        printf("\n");
                        printf("tmp  = "); eker_print_term((TERM*)tmp);
                        printf("\n");
                        printf("\n");
                        */
                        ACmatch = build_match((TERM*)sv[6],(TERM*)tmp,2);
                        while(1) {
                          if(!extract_match(ACmatch, assignment)) {
                            destroy_match(ACmatch);
                            fail();
                          }
                          /* choicePoint Eker AC matching */
                          if(!setChoicePoint()) {
                            break;
                          }
                        }
                        substitution[2]=fromEkerForm(assignment[0]);
                        destroy_term(assignment[0]);
                        substitution[3]=fromEkerForm(assignment[1]);
                        destroy_term(assignment[1]);
                        sv[5]=substitution[2];
                        sv[4]=substitution[3];
                      }
                      /* where *(var4,var2) := var3 */
                      // this=var3        underAC=true        Instantiated=true
                      /* --- re-normalisation */
                      tmp = term_removeTopSymbol(sv[0]);
                      if(tmp == NULL)
                        sv[0]=fun_333( sv[0] );
                      else
                        sv[0]=tmp;
                      tmp = sv[11] = sv[0];
                      {
                        match_state *ACmatch;
                        /* TODO: modifier la taille du tableau */
                        TERM *assignment[2];
                        int i;
                        for(i=0 ; i<2 ; i++) assignment[i]=NULL;
                        /* pattern number 5 */
                        sv[11] = (struct term*) EkerTerm[5];
                        /* TODO: modifier le nb de variables */
                        tmp = (struct term*)toEkerForm(tmp);
                        ac_sort((TERM*)tmp);
                        /*
                        printf("sv  = "); eker_print_term((TERM*)sv[11]);
                        printf("\n");
                        printf("tmp  = "); eker_print_term((TERM*)tmp);
                        printf("\n");
                        printf("\n");
                        */
                        ACmatch = build_match((TERM*)sv[11],(TERM*)tmp,2);
                        while(1) {
                          if(!extract_match(ACmatch, assignment)) {
                            destroy_match(ACmatch);
                            fail();
                          }
                          /* choicePoint Eker AC matching */
                          if(!setChoicePoint()) {
                            break;
                          }
                        }
                        substitution[4]=fromEkerForm(assignment[0]);
                        destroy_term(assignment[0]);
                        substitution[5]=fromEkerForm(assignment[1]);
                        destroy_term(assignment[1]);
                        sv[10]=substitution[4];
                        sv[9]=substitution[5];
                      }
                      /* if eq_variable(,)(var0,var4) */
                      // this=var0        underAC=true        Instantiated=true
                      /* --- re-normalisation */
                      tmp = term_removeTopSymbol(sv[5]);
                      if(tmp == NULL)
                        sv[5]=fun_334( sv[5] );
                      else
                        sv[5]=tmp;
                      // this=var4        underAC=true        Instantiated=true
                      /* --- re-normalisation */
                      tmp = term_removeTopSymbol(sv[10]);
                      if(tmp == NULL)
                        sv[10]=fun_334( sv[10] );
                      else
                        sv[10]=tmp;
                      sv[12] = fun_18( sv[5],sv[10] );
                      if( sv[12] != con_1 ) {
                        fail();
                      }
                      if(ACPattern) { CUTCLOSE(); } /* AC matching */
                      /* rhs: *(var0,+(var1,var2)) */
                      // this=var0        underAC=true        Instantiated=true
                      // this=var1        underAC=true        Instantiated=true
                      /* --- re-normalisation */
                      tmp = term_removeTopSymbol(sv[4]);
                      if(tmp == NULL)
                        sv[4]=fun_334( sv[4] );
                      else
                        sv[4]=tmp;
                      // this=var2        underAC=true        Instantiated=true
                      /* --- re-normalisation */
                      tmp = term_removeTopSymbol(sv[9]);
                      if(tmp == NULL)
                        sv[9]=fun_334( sv[9] );
                      else
                        sv[9]=tmp;
                      sv[13] = NULL;
                      sv[13] = (struct term*)term_add_onf_term(sv[13],code_333,sv[4]);
                      sv[13] = (struct term*)term_add_onf_term(sv[13],code_333,sv[9]);
                      sv[13] = fun_333( sv[13] );
                      sv[14] = NULL;
                      sv[14] = (struct term*)term_add_onf_term(sv[14],code_334,sv[5]);
                      sv[14] = (struct term*)term_add_onf_term(sv[14],code_334,sv[13]);
                      sv[14] = fun_334( sv[14] );
                      res = sv[14] ;
                    rewrite_step++;
                      goto stratLab19;
                    } else {
                      fail();
                    }
                    myend40:;
                    if(ACPattern)
                      CUTCLOSE(); /* AC matching */
                  }
                }
                fail();
                stratLab19:;
                v0=res;
                *lastTerm = res;
              }
            }
          }
          /* end   repeat[det](one[semiDet](factorize:poly/poly5[Vars]!GL)) */
          //printf("res = "); internal_term_println(stdout,res,resultMode);
          genericCopyTermAllocExcept(newTerm,i,arg0);
          genericSetSubterm(newTerm,i,res);
          res = specialApply(newTerm);
          break;
        }
      }
      if(i==arity) {
        fail();
      }
    }
    /* La strategie a donne un resultat */
    goto stratLab22;
  stratLab22:;
    CUTCLOSE(); /* DC v2 */
  }
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_35( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  /* cons[semiDet](call(tdexpand:poly/poly5[Vars])[semiDet],call(tdfactorize:poly/poly5[Vars])[semiDet]) */
  res = str_14(v0);
  v0 = res;
  res = str_341(v0);
  v0 = res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_14( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  /* cons[semiDet](ONE[semiDet](dk[nonDet](topexpand:poly/poly5[Vars]!GL),id),ONE[semiDet](call(sexpand:poly/poly5[Vars])[nonDet],id)) */
  {
    /* ONE[semiDet](dk[nonDet](topexpand:poly/poly5[Vars]!GL),id) */
    CUTOPEN(); /* DC v2 */
    if(!setChoicePoint()) {
    /* Si la strategie suivante echoue, on passe a la suivante */
      {
        /* dk[nonDet](topexpand:poly/poly5[Vars]!GL) */
        struct term *v1,*v2;
        match_state **ms=NULL;
        int necessary_link;
        int indice=-1;
        int ACPattern=0;
        bitSet_GC_create(mask,2);
        bitSet_init_clear(mask);
        switch(getSymb(v0)) {
        case code_333: /* + */
          bitSet_set(mask,0);
          bitSet_set(mask,1);
          break;
        /* matching is not complete: jumpNode is null */
        }
        ms=(match_state**) MALLOC(1*sizeof(match_state*));
        if( bitSet_get(mask,0) || bitSet_get(mask,1) ) {
          int necessary_link;
          ACPattern=1;
        }
        if(bitSet_get(mask,0)) {
          if(setChoicePoint()) {
            /* AC matching failed, try next rule */
            goto myend41;
          }
          if(1) {
            int nb_variable=2;
            int nb_variable_ac=2;
            int i;
            struct term *tmp, *sv[18];
            struct term *substitution[6];
            /* lhs: +(var0,var5) */
            for(i=0 ; i<sizeof(substitution)/sizeof(struct term*) ; i++) {
              substitution[i]=v0;
            }
            sv[0]=substitution[0];
            sv[1]=substitution[1];
            {
              multiplicityType *E,*sol;
              int nb_arg_subject;
              int no_arg_subject;
              int indice=0;
              int i;
              struct termac *tac=(struct termac*)sv[0];
              E=(multiplicityType*)AMALLOC((1+getArity(tac))*sizeof(multiplicityType));
              sol=(multiplicityType*)AMALLOC((1+getArity(tac))*sizeof(multiplicityType));
              for(i=0 ; i<getArity(tac) ; i++) {
                E[i]=getMult(tac,i) - (getMult(tac,i)%1);
                sol[i]=0;
              }
              sol[getArity(tac)]=0; /* total */
              while(i=next_minimal_extract(getArity(tac),E,sol,1)) {
                if(!setChoicePoint()) {
                  break;
                }
              }
              if(i) {
                struct termac *list_x = NULL;
                struct termac *list_y = NULL;
                extract_xy_from_pe((struct termac*)tac,E,sol,1,&list_x,&list_y);
                sv[0] = (struct term*)list_x;
                sv[1] = (struct term*)list_y;
                // printf("list_x = sv[0] = ");                 internal_term_println(stdout,sv[0],resultMode);
                // printf("list_y = sv[1] = ");                 internal_term_println(stdout,sv[1],resultMode);
                if(getArity(((struct termac*)sv[1]))==0) {
                  /* Avoid an empty context */
                  fail();
                }
              } else {
                /*  There is no more solution */
                fail();
              }
            }
            /* allDetEvaluation: nonDet */
            /* where *(+(var1,var2),+(var3,var4)) := var5 */
            // this=var5        underAC=true        Instantiated=true
            /* --- re-normalisation */
            tmp = term_removeTopSymbol(sv[1]);
            if(tmp == NULL)
              sv[1]=fun_333( sv[1] );
            else
              sv[1]=tmp;
            tmp = sv[12] = sv[1];
            {
              match_state *ACmatch;
              /* TODO: modifier la taille du tableau */
              TERM *assignment[4];
              int i;
              for(i=0 ; i<4 ; i++) assignment[i]=NULL;
              /* pattern number 6 */
              sv[12] = (struct term*) EkerTerm[6];
              /* TODO: modifier le nb de variables */
              tmp = (struct term*)toEkerForm(tmp);
              ac_sort((TERM*)tmp);
              /*
              printf("sv  = "); eker_print_term((TERM*)sv[12]);
              printf("\n");
              printf("tmp  = "); eker_print_term((TERM*)tmp);
              printf("\n");
              printf("\n");
              */
              ACmatch = build_match((TERM*)sv[12],(TERM*)tmp,4);
              while(1) {
                if(!extract_match(ACmatch, assignment)) {
                  destroy_match(ACmatch);
                  fail();
                }
                /* choicePoint Eker AC matching */
                if(!setChoicePoint()) {
                  break;
                }
              }
              substitution[2]=fromEkerForm(assignment[0]);
              destroy_term(assignment[0]);
              substitution[3]=fromEkerForm(assignment[1]);
              destroy_term(assignment[1]);
              substitution[4]=fromEkerForm(assignment[2]);
              destroy_term(assignment[2]);
              substitution[5]=fromEkerForm(assignment[3]);
              destroy_term(assignment[3]);
              sv[10]=substitution[2];
              sv[9]=substitution[3];
              sv[7]=substitution[4];
              sv[6]=substitution[5];
            }
            /* rhs: +(var0,*(var1,var3),*(var1,var4),*(var2,var3),*(var2,var4)) */
            // this=var0        underAC=true        Instantiated=true
            // this=var1        underAC=true        Instantiated=true
            tmp = term_removeTopSymbol(sv[10]);
            if(tmp != NULL)
              sv[10]=tmp;
            // this=var3        underAC=true        Instantiated=true
            tmp = term_removeTopSymbol(sv[7]);
            if(tmp != NULL)
              sv[7]=tmp;
            sv[13] = NULL;
            sv[13] = (struct term*)term_add_onf_term(sv[13],code_334,sv[10]);
            sv[13] = (struct term*)term_add_onf_term(sv[13],code_334,sv[7]);
            sv[13] = fun_334( sv[13] );
            // this=var1        underAC=true        Instantiated=true
            // this=var4        underAC=true        Instantiated=true
            tmp = term_removeTopSymbol(sv[6]);
            if(tmp != NULL)
              sv[6]=tmp;
            sv[14] = NULL;
            sv[14] = (struct term*)term_add_onf_term(sv[14],code_334,sv[10]);
            sv[14] = (struct term*)term_add_onf_term(sv[14],code_334,sv[6]);
            sv[14] = fun_334( sv[14] );
            // this=var2        underAC=true        Instantiated=true
            tmp = term_removeTopSymbol(sv[9]);
            if(tmp != NULL)
              sv[9]=tmp;
            // this=var3        underAC=true        Instantiated=true
            sv[15] = NULL;
            sv[15] = (struct term*)term_add_onf_term(sv[15],code_334,sv[9]);
            sv[15] = (struct term*)term_add_onf_term(sv[15],code_334,sv[7]);
            sv[15] = fun_334( sv[15] );
            // this=var2        underAC=true        Instantiated=true
            // this=var4        underAC=true        Instantiated=true
            sv[16] = NULL;
            sv[16] = (struct term*)term_add_onf_term(sv[16],code_334,sv[9]);
            sv[16] = (struct term*)term_add_onf_term(sv[16],code_334,sv[6]);
            sv[16] = fun_334( sv[16] );
            sv[17] = NULL;
            sv[17] = (struct term*)term_add_onf_term(sv[17],code_333,sv[0]);
            sv[17] = (struct term*)term_add_onf_term(sv[17],code_333,sv[13]);
            sv[17] = (struct term*)term_add_onf_term(sv[17],code_333,sv[14]);
            sv[17] = (struct term*)term_add_onf_term(sv[17],code_333,sv[15]);
            sv[17] = (struct term*)term_add_onf_term(sv[17],code_333,sv[16]);
            sv[17] = fun_333( sv[17] );
            res = sv[17] ;
          rewrite_step++;
            goto stratLab2;
          } else {
            fail();
          }
          myend41:;
        }
        if(bitSet_get(mask,1)) {
          if(setChoicePoint()) {
            /* AC matching failed, try next rule */
            goto myend42;
          }
          if(1) {
            int nb_variable=2;
            int nb_variable_ac=2;
            int i;
            struct term *tmp, *sv[13];
            struct term *substitution[5];
            /* lhs: +(var0,var4) */
            for(i=0 ; i<sizeof(substitution)/sizeof(struct term*) ; i++) {
              substitution[i]=v0;
            }
            sv[0]=substitution[0];
            sv[1]=substitution[1];
            {
              multiplicityType *E,*sol;
              int nb_arg_subject;
              int no_arg_subject;
              int indice=0;
              int i;
              struct termac *tac=(struct termac*)sv[0];
              E=(multiplicityType*)AMALLOC((1+getArity(tac))*sizeof(multiplicityType));
              sol=(multiplicityType*)AMALLOC((1+getArity(tac))*sizeof(multiplicityType));
              for(i=0 ; i<getArity(tac) ; i++) {
                E[i]=getMult(tac,i) - (getMult(tac,i)%1);
                sol[i]=0;
              }
              sol[getArity(tac)]=0; /* total */
              while(i=next_minimal_extract(getArity(tac),E,sol,1)) {
                if(!setChoicePoint()) {
                  break;
                }
              }
              if(i) {
                struct termac *list_x = NULL;
                struct termac *list_y = NULL;
                extract_xy_from_pe((struct termac*)tac,E,sol,1,&list_x,&list_y);
                sv[0] = (struct term*)list_x;
                sv[1] = (struct term*)list_y;
                // printf("list_x = sv[0] = ");                 internal_term_println(stdout,sv[0],resultMode);
                // printf("list_y = sv[1] = ");                 internal_term_println(stdout,sv[1],resultMode);
                if(getArity(((struct termac*)sv[1]))==0) {
                  /* Avoid an empty context */
                  fail();
                }
              } else {
                /*  There is no more solution */
                fail();
              }
            }
            /* allDetEvaluation: nonDet */
            /* where *(var3,+(var1,var2)) := var4 */
            // this=var4        underAC=true        Instantiated=true
            /* --- re-normalisation */
            tmp = term_removeTopSymbol(sv[1]);
            if(tmp == NULL)
              sv[1]=fun_333( sv[1] );
            else
              sv[1]=tmp;
            tmp = sv[9] = sv[1];
            {
              match_state *ACmatch;
              /* TODO: modifier la taille du tableau */
              TERM *assignment[3];
              int i;
              for(i=0 ; i<3 ; i++) assignment[i]=NULL;
              /* pattern number 7 */
              sv[9] = (struct term*) EkerTerm[7];
              /* TODO: modifier le nb de variables */
              tmp = (struct term*)toEkerForm(tmp);
              ac_sort((TERM*)tmp);
              /*
              printf("sv  = "); eker_print_term((TERM*)sv[9]);
              printf("\n");
              printf("tmp  = "); eker_print_term((TERM*)tmp);
              printf("\n");
              printf("\n");
              */
              ACmatch = build_match((TERM*)sv[9],(TERM*)tmp,3);
              while(1) {
                if(!extract_match(ACmatch, assignment)) {
                  destroy_match(ACmatch);
                  fail();
                }
                /* choicePoint Eker AC matching */
                if(!setChoicePoint()) {
                  break;
                }
              }
              substitution[2]=fromEkerForm(assignment[0]);
              destroy_term(assignment[0]);
              substitution[3]=fromEkerForm(assignment[1]);
              destroy_term(assignment[1]);
              substitution[4]=fromEkerForm(assignment[2]);
              destroy_term(assignment[2]);
              sv[8]=substitution[2];
              sv[6]=substitution[3];
              sv[5]=substitution[4];
            }
            /* rhs: +(var0,*(var1,var3),*(var2,var3)) */
            // this=var0        underAC=true        Instantiated=true
            // this=var1        underAC=true        Instantiated=true
            tmp = term_removeTopSymbol(sv[6]);
            if(tmp != NULL)
              sv[6]=tmp;
            // this=var3        underAC=true        Instantiated=true
            /* --- re-normalisation */
            tmp = term_removeTopSymbol(sv[8]);
            if(tmp == NULL)
              sv[8]=fun_334( sv[8] );
            else
              sv[8]=tmp;
            sv[10] = NULL;
            sv[10] = (struct term*)term_add_onf_term(sv[10],code_334,sv[6]);
            sv[10] = (struct term*)term_add_onf_term(sv[10],code_334,sv[8]);
            sv[10] = fun_334( sv[10] );
            // this=var2        underAC=true        Instantiated=true
            tmp = term_removeTopSymbol(sv[5]);
            if(tmp != NULL)
              sv[5]=tmp;
            // this=var3        underAC=true        Instantiated=true
            sv[11] = NULL;
            sv[11] = (struct term*)term_add_onf_term(sv[11],code_334,sv[5]);
            sv[11] = (struct term*)term_add_onf_term(sv[11],code_334,sv[8]);
            sv[11] = fun_334( sv[11] );
            sv[12] = NULL;
            sv[12] = (struct term*)term_add_onf_term(sv[12],code_333,sv[0]);
            sv[12] = (struct term*)term_add_onf_term(sv[12],code_333,sv[10]);
            sv[12] = (struct term*)term_add_onf_term(sv[12],code_333,sv[11]);
            sv[12] = fun_333( sv[12] );
            res = sv[12] ;
          rewrite_step++;
            goto stratLab2;
          } else {
            fail();
          }
          myend42:;
        }
      }
      fail();
      stratLab2:;
      v0=res;
      /* La strategie a donne un resultat */
      goto stratLab4;
    }
    /* On vient d'un fail, on essai la strategie suivante */
    /* id */
    res=v0;
    /* La strategie a donne un resultat */
    goto stratLab4;
  stratLab4:;
    CUTCLOSE(); /* DC v2 */
  }
  {
    /* ONE[semiDet](call(sexpand:poly/poly5[Vars])[nonDet],id) */
    CUTOPEN(); /* DC v2 */
    if(!setChoicePoint()) {
    /* Si la strategie suivante echoue, on passe a la suivante */
      res = str_413(v0);
      v0 = res;
      /* La strategie a donne un resultat */
      goto stratLab7;
    }
    /* On vient d'un fail, on essai la strategie suivante */
    /* id */
    res=v0;
    /* La strategie a donne un resultat */
    goto stratLab7;
  stratLab7:;
    CUTCLOSE(); /* DC v2 */
  }
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_341( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  /* cons[semiDet](ONE[semiDet](dk[nonDet](topfactorize:poly/poly5[Vars]!GL),id),ONE[det](call(sfactorize:poly/poly5[Vars])[det],id)) */
  {
    /* ONE[semiDet](dk[nonDet](topfactorize:poly/poly5[Vars]!GL),id) */
    CUTOPEN(); /* DC v2 */
    if(!setChoicePoint()) {
    /* Si la strategie suivante echoue, on passe a la suivante */
      {
        /* dk[nonDet](topfactorize:poly/poly5[Vars]!GL) */
        struct term *v1,*v2;
        match_state **ms=NULL;
        int necessary_link;
        int indice=-1;
        int ACPattern=0;
        bitSet_GC_create(mask,2);
        bitSet_init_clear(mask);
        switch(getSymb(v0)) {
        case code_333: /* + */
          bitSet_set(mask,0);
          bitSet_set(mask,1);
          break;
        /* matching is not complete: jumpNode is null */
        }
        ms=(match_state**) MALLOC(1*sizeof(match_state*));
        if( bitSet_get(mask,0) || bitSet_get(mask,1) ) {
          int necessary_link;
          ACPattern=1;
        }
        if(bitSet_get(mask,0)) {
          if(setChoicePoint()) {
            /* AC matching failed, try next rule */
            goto myend43;
          }
          if(1) {
            int nb_variable=1;
            int nb_variable_ac=1;
            int i;
            struct term *tmp, *sv[25];
            struct term *substitution[8];
            /* lhs: +(var8) */
            for(i=0 ; i<sizeof(substitution)/sizeof(struct term*) ; i++) {
              substitution[i]=v0;
            }
            /* allDetEvaluation: nonDet */
            /* where +(var0,var5,var7) := var8 */
            // this=var8        underAC=true        Instantiated=false
            sv[0]=substitution[0];
            /* --- re-normalisation */
            tmp = term_removeTopSymbol(sv[0]);
            if(tmp == NULL)
              sv[0]=fun_333( sv[0] );
            else
              sv[0]=tmp;
            tmp = sv[7] = sv[0];
            {
              match_state *ACmatch;
              /* TODO: modifier la taille du tableau */
              TERM *assignment[3];
              int i;
              for(i=0 ; i<3 ; i++) assignment[i]=NULL;
              /* pattern number 8 */
              sv[7] = (struct term*) EkerTerm[8];
              /* TODO: modifier le nb de variables */
              tmp = (struct term*)toEkerForm(tmp);
              ac_sort((TERM*)tmp);
              /*
              printf("sv  = "); eker_print_term((TERM*)sv[7]);
              printf("\n");
              printf("tmp  = "); eker_print_term((TERM*)tmp);
              printf("\n");
              printf("\n");
              */
              ACmatch = build_match((TERM*)sv[7],(TERM*)tmp,3);
              while(1) {
                if(!extract_match(ACmatch, assignment)) {
                  destroy_match(ACmatch);
                  fail();
                }
                /* choicePoint Eker AC matching */
                if(!setChoicePoint()) {
                  break;
                }
              }
              substitution[1]=fromEkerForm(assignment[0]);
              destroy_term(assignment[0]);
              substitution[2]=fromEkerForm(assignment[1]);
              destroy_term(assignment[1]);
              substitution[3]=fromEkerForm(assignment[2]);
              destroy_term(assignment[2]);
              sv[6]=substitution[1];
              sv[5]=substitution[2];
              sv[4]=substitution[3];
            }
            /* where *(var2,(var1)) := var7 */
            // this=var7        underAC=true        Instantiated=true
            /* --- re-normalisation */
            tmp = term_removeTopSymbol(sv[4]);
            if(tmp == NULL)
              sv[4]=fun_333( sv[4] );
            else
              sv[4]=tmp;
            tmp = sv[13] = sv[4];
            {
              match_state *ACmatch;
              /* TODO: modifier la taille du tableau */
              TERM *assignment[2];
              int i;
              for(i=0 ; i<2 ; i++) assignment[i]=NULL;
              /* pattern number 9 */
              sv[13] = (struct term*) EkerTerm[9];
              /* TODO: modifier le nb de variables */
              tmp = (struct term*)toEkerForm(tmp);
              ac_sort((TERM*)tmp);
              /*
              printf("sv  = "); eker_print_term((TERM*)sv[13]);
              printf("\n");
              printf("tmp  = "); eker_print_term((TERM*)tmp);
              printf("\n");
              printf("\n");
              */
              ACmatch = build_match((TERM*)sv[13],(TERM*)tmp,2);
              while(1) {
                if(!extract_match(ACmatch, assignment)) {
                  destroy_match(ACmatch);
                  fail();
                }
                /* choicePoint Eker AC matching */
                if(!setChoicePoint()) {
                  break;
                }
              }
              substitution[4]=fromEkerForm(assignment[0]);
              destroy_term(assignment[0]);
              substitution[5]=fromEkerForm(assignment[1]);
              destroy_term(assignment[1]);
              sv[12]=substitution[4];
              sv[10]=substitution[5];
            }
            /* where *(var6,(var3)) := var5 */
            // this=var5        underAC=true        Instantiated=true
            /* --- re-normalisation */
            tmp = term_removeTopSymbol(sv[5]);
            if(tmp == NULL)
              sv[5]=fun_333( sv[5] );
            else
              sv[5]=tmp;
            tmp = sv[19] = sv[5];
            {
              match_state *ACmatch;
              /* TODO: modifier la taille du tableau */
              TERM *assignment[2];
              int i;
              for(i=0 ; i<2 ; i++) assignment[i]=NULL;
              /* pattern number 10 */
              sv[19] = (struct term*) EkerTerm[10];
              /* TODO: modifier le nb de variables */
              tmp = (struct term*)toEkerForm(tmp);
              ac_sort((TERM*)tmp);
              /*
              printf("sv  = "); eker_print_term((TERM*)sv[19]);
              printf("\n");
              printf("tmp  = "); eker_print_term((TERM*)tmp);
              printf("\n");
              printf("\n");
              */
              ACmatch = build_match((TERM*)sv[19],(TERM*)tmp,2);
              while(1) {
                if(!extract_match(ACmatch, assignment)) {
                  destroy_match(ACmatch);
                  fail();
                }
                /* choicePoint Eker AC matching */
                if(!setChoicePoint()) {
                  break;
                }
              }
              substitution[6]=fromEkerForm(assignment[0]);
              destroy_term(assignment[0]);
              substitution[7]=fromEkerForm(assignment[1]);
              destroy_term(assignment[1]);
              sv[18]=substitution[6];
              sv[16]=substitution[7];
            }
            /* if eq_variable(,)(var2,var6) */
            // this=var2        underAC=true        Instantiated=true
            /* --- re-normalisation */
            tmp = term_removeTopSymbol(sv[12]);
            if(tmp == NULL)
              sv[12]=fun_334( sv[12] );
            else
              sv[12]=tmp;
            // this=var6        underAC=true        Instantiated=true
            /* --- re-normalisation */
            tmp = term_removeTopSymbol(sv[18]);
            if(tmp == NULL)
              sv[18]=fun_334( sv[18] );
            else
              sv[18]=tmp;
            sv[20] = fun_18( sv[12],sv[18] );
            if( sv[20] != con_1 ) {
              fail();
            }
            /* where var4 := plus(,)(var1,var3) */
            // this=var1        underAC=true        Instantiated=true
            // this=var3        underAC=true        Instantiated=true
            sv[22] = fun_302( sv[10],sv[16] );
            tmp = sv[21] = sv[22];
            /* rhs: +(var0,*(var2,(var4))) */
            // this=var0        underAC=true        Instantiated=true
            // this=var2        underAC=true        Instantiated=true
            // this=var4        underAC=false        Instantiated=false
            term_alloc(&sv[23],sizeof(struct term1),code_332);
            sv[23]->sub[0] = sv[21];
            sv[24] = NULL;
            sv[24] = (struct term*)term_add_onf_term(sv[24],code_334,sv[12]);
            sv[24] = (struct term*)term_add_onf_term(sv[24],code_334,sv[23]);
            sv[24] = fun_334( sv[24] );
            sv[23] = NULL;
            sv[23] = (struct term*)term_add_onf_term(sv[23],code_333,sv[6]);
            sv[23] = (struct term*)term_add_onf_term(sv[23],code_333,sv[24]);
            sv[23] = fun_333( sv[23] );
            res = sv[23] ;
          rewrite_step++;
            goto stratLab24;
          } else {
            fail();
          }
          myend43:;
        }
        if(bitSet_get(mask,1)) {
          if(setChoicePoint()) {
            /* AC matching failed, try next rule */
            goto myend44;
          }
          if(1) {
            int nb_variable=1;
            int nb_variable_ac=1;
            int i;
            struct term *tmp, *sv[21];
            struct term *substitution[8];
            /* lhs: +(var7) */
            for(i=0 ; i<sizeof(substitution)/sizeof(struct term*) ; i++) {
              substitution[i]=v0;
            }
            /* allDetEvaluation: nonDet */
            /* where +(var0,var4,var6) := var7 */
            // this=var7        underAC=true        Instantiated=false
            sv[0]=substitution[0];
            /* --- re-normalisation */
            tmp = term_removeTopSymbol(sv[0]);
            if(tmp == NULL)
              sv[0]=fun_333( sv[0] );
            else
              sv[0]=tmp;
            tmp = sv[7] = sv[0];
            {
              match_state *ACmatch;
              /* TODO: modifier la taille du tableau */
              TERM *assignment[3];
              int i;
              for(i=0 ; i<3 ; i++) assignment[i]=NULL;
              /* pattern number 11 */
              sv[7] = (struct term*) EkerTerm[11];
              /* TODO: modifier le nb de variables */
              tmp = (struct term*)toEkerForm(tmp);
              ac_sort((TERM*)tmp);
              /*
              printf("sv  = "); eker_print_term((TERM*)sv[7]);
              printf("\n");
              printf("tmp  = "); eker_print_term((TERM*)tmp);
              printf("\n");
              printf("\n");
              */
              ACmatch = build_match((TERM*)sv[7],(TERM*)tmp,3);
              while(1) {
                if(!extract_match(ACmatch, assignment)) {
                  destroy_match(ACmatch);
                  fail();
                }
                /* choicePoint Eker AC matching */
                if(!setChoicePoint()) {
                  break;
                }
              }
              substitution[1]=fromEkerForm(assignment[0]);
              destroy_term(assignment[0]);
              substitution[2]=fromEkerForm(assignment[1]);
              destroy_term(assignment[1]);
              substitution[3]=fromEkerForm(assignment[2]);
              destroy_term(assignment[2]);
              sv[6]=substitution[1];
              sv[5]=substitution[2];
              sv[4]=substitution[3];
            }
            /* where *(var1,var3) := var6 */
            // this=var6        underAC=true        Instantiated=true
            /* --- re-normalisation */
            tmp = term_removeTopSymbol(sv[4]);
            if(tmp == NULL)
              sv[4]=fun_333( sv[4] );
            else
              sv[4]=tmp;
            tmp = sv[12] = sv[4];
            {
              match_state *ACmatch;
              /* TODO: modifier la taille du tableau */
              TERM *assignment[2];
              int i;
              for(i=0 ; i<2 ; i++) assignment[i]=NULL;
              /* pattern number 12 */
              sv[12] = (struct term*) EkerTerm[12];
              /* TODO: modifier le nb de variables */
              tmp = (struct term*)toEkerForm(tmp);
              ac_sort((TERM*)tmp);
              /*
              printf("sv  = "); eker_print_term((TERM*)sv[12]);
              printf("\n");
              printf("tmp  = "); eker_print_term((TERM*)tmp);
              printf("\n");
              printf("\n");
              */
              ACmatch = build_match((TERM*)sv[12],(TERM*)tmp,2);
              while(1) {
                if(!extract_match(ACmatch, assignment)) {
                  destroy_match(ACmatch);
                  fail();
                }
                /* choicePoint Eker AC matching */
                if(!setChoicePoint()) {
                  break;
                }
              }
              substitution[4]=fromEkerForm(assignment[0]);
              destroy_term(assignment[0]);
              substitution[5]=fromEkerForm(assignment[1]);
              destroy_term(assignment[1]);
              sv[11]=substitution[4];
              sv[10]=substitution[5];
            }
            /* where *(var5,var2) := var4 */
            // this=var4        underAC=true        Instantiated=true
            /* --- re-normalisation */
            tmp = term_removeTopSymbol(sv[5]);
            if(tmp == NULL)
              sv[5]=fun_333( sv[5] );
            else
              sv[5]=tmp;
            tmp = sv[17] = sv[5];
            {
              match_state *ACmatch;
              /* TODO: modifier la taille du tableau */
              TERM *assignment[2];
              int i;
              for(i=0 ; i<2 ; i++) assignment[i]=NULL;
              /* pattern number 13 */
              sv[17] = (struct term*) EkerTerm[13];
              /* TODO: modifier le nb de variables */
              tmp = (struct term*)toEkerForm(tmp);
              ac_sort((TERM*)tmp);
              /*
              printf("sv  = "); eker_print_term((TERM*)sv[17]);
              printf("\n");
              printf("tmp  = "); eker_print_term((TERM*)tmp);
              printf("\n");
              printf("\n");
              */
              ACmatch = build_match((TERM*)sv[17],(TERM*)tmp,2);
              while(1) {
                if(!extract_match(ACmatch, assignment)) {
                  destroy_match(ACmatch);
                  fail();
                }
                /* choicePoint Eker AC matching */
                if(!setChoicePoint()) {
                  break;
                }
              }
              substitution[6]=fromEkerForm(assignment[0]);
              destroy_term(assignment[0]);
              substitution[7]=fromEkerForm(assignment[1]);
              destroy_term(assignment[1]);
              sv[16]=substitution[6];
              sv[15]=substitution[7];
            }
            /* if eq_variable(,)(var1,var5) */
            // this=var1        underAC=true        Instantiated=true
            /* --- re-normalisation */
            tmp = term_removeTopSymbol(sv[11]);
            if(tmp == NULL)
              sv[11]=fun_334( sv[11] );
            else
              sv[11]=tmp;
            // this=var5        underAC=true        Instantiated=true
            /* --- re-normalisation */
            tmp = term_removeTopSymbol(sv[16]);
            if(tmp == NULL)
              sv[16]=fun_334( sv[16] );
            else
              sv[16]=tmp;
            sv[18] = fun_18( sv[11],sv[16] );
            if( sv[18] != con_1 ) {
              fail();
            }
            /* rhs: +(var0,*(var1,+(var2,var3))) */
            // this=var0        underAC=true        Instantiated=true
            // this=var1        underAC=true        Instantiated=true
            // this=var2        underAC=true        Instantiated=true
            /* --- re-normalisation */
            tmp = term_removeTopSymbol(sv[15]);
            if(tmp == NULL)
              sv[15]=fun_334( sv[15] );
            else
              sv[15]=tmp;
            // this=var3        underAC=true        Instantiated=true
            /* --- re-normalisation */
            tmp = term_removeTopSymbol(sv[10]);
            if(tmp == NULL)
              sv[10]=fun_334( sv[10] );
            else
              sv[10]=tmp;
            sv[19] = NULL;
            sv[19] = (struct term*)term_add_onf_term(sv[19],code_333,sv[15]);
            sv[19] = (struct term*)term_add_onf_term(sv[19],code_333,sv[10]);
            sv[19] = fun_333( sv[19] );
            sv[20] = NULL;
            sv[20] = (struct term*)term_add_onf_term(sv[20],code_334,sv[11]);
            sv[20] = (struct term*)term_add_onf_term(sv[20],code_334,sv[19]);
            sv[20] = fun_334( sv[20] );
            sv[19] = NULL;
            sv[19] = (struct term*)term_add_onf_term(sv[19],code_333,sv[6]);
            sv[19] = (struct term*)term_add_onf_term(sv[19],code_333,sv[20]);
            sv[19] = fun_333( sv[19] );
            res = sv[19] ;
          rewrite_step++;
            goto stratLab24;
          } else {
            fail();
          }
          myend44:;
        }
      }
      fail();
      stratLab24:;
      v0=res;
      /* La strategie a donne un resultat */
      goto stratLab26;
    }
    /* On vient d'un fail, on essai la strategie suivante */
    /* id */
    res=v0;
    /* La strategie a donne un resultat */
    goto stratLab26;
  stratLab26:;
    CUTCLOSE(); /* DC v2 */
  }
  {
    /* ONE[det](call(sfactorize:poly/poly5[Vars])[det],id) */
    CUTOPEN(); /* DC v2 */
    if(!setChoicePoint()) {
    /* Si la strategie suivante echoue, on passe a la suivante */
      res = str_240(v0);
      v0 = res;
      /* La strategie a donne un resultat */
      goto stratLab29;
    }
    /* On vient d'un fail, on essai la strategie suivante */
    /* id */
    res=v0;
    /* La strategie a donne un resultat */
    goto stratLab29;
  stratLab29:;
    CUTCLOSE(); /* DC v2 */
  }
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}
