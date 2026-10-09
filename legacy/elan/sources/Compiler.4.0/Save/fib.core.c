#include "fib.h"

Gterm* fun_301(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /* [] */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label3:
      switch(GgetSymb(v2)) {
      case code_300: /* [] */
        v7= GgetArgument(v2,0);
        switch(GgetInt(v7)) {
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
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
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
    GmakeAppl1(sv[2],code_300    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend0:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  GmakeAppl2(res,code_301  ,v1  ,v2  );
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

Gterm* fun_302(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /* [] */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label9:
      switch(GgetSymb(v2)) {
      case code_300: /* [] */
        v7= GgetArgument(v2,0);
        switch(GgetInt(v7)) {
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
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
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
    GmakeAppl1(sv[2],code_300    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend1:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  GmakeAppl2(res,code_302  ,v1  ,v2  );
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

Gterm* fun_303(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /* [] */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label15:
      switch(GgetSymb(v2)) {
      case code_300: /* [] */
        v7= GgetArgument(v2,0);
        switch(GgetInt(v7)) {
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
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
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
    GmakeAppl1(sv[2],code_300    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend2:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  GmakeAppl2(res,code_303  ,v1  ,v2  );
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

Gterm* fun_304(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /* [] */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label21:
      switch(GgetSymb(v2)) {
      case code_300: /* [] */
        v7= GgetArgument(v2,0);
        switch(GgetInt(v7)) {
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
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
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
    GmakeAppl1(sv[2],code_300    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend3:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  GmakeAppl2(res,code_304  ,v1  ,v2  );
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

Gterm* fun_305(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /* [] */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label27:
      switch(GgetSymb(v2)) {
      case code_300: /* [] */
        v7= GgetArgument(v2,0);
        switch(GgetInt(v7)) {
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
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
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
    GmakeAppl1(sv[2],code_300    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend4:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  GmakeAppl2(res,code_305  ,v1  ,v2  );
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

Gterm* fun_306(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /* [] */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label33:
      switch(GgetSymb(v2)) {
      case code_300: /* [] */
        v7= GgetArgument(v2,0);
        switch(GgetInt(v7)) {
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
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
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
    GmakeAppl1(sv[2],code_300    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend5:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  GmakeAppl2(res,code_306  ,v1  ,v2  );
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

Gterm* fun_307(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /* [] */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label39:
      switch(GgetSymb(v2)) {
      case code_300: /* [] */
        v7= GgetArgument(v2,0);
        switch(GgetInt(v7)) {
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
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
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
    GmakeAppl1(sv[2],code_300    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend6:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  GmakeAppl2(res,code_307  ,v1  ,v2  );
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

Gterm* fun_308(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /* [] */
    v4= GgetArgument(v1,0);
    switch(GgetInt(v4)) {
    default:
    label45:
      bitSet32_set(mask32,0);
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
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
    GmakeAppl1(sv[2],code_300    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend7:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  GmakeAppl1(res,code_308  ,v1  );
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

Gterm* fun_309(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /* [] */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label49:
      switch(GgetSymb(v2)) {
      case code_300: /* [] */
        v7= GgetArgument(v2,0);
        switch(GgetInt(v7)) {
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
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
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
  GmakeAppl2(res,code_309  ,v1  ,v2  );
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

Gterm* fun_310(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /* [] */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label55:
      switch(GgetSymb(v2)) {
      case code_300: /* [] */
        v7= GgetArgument(v2,0);
        switch(GgetInt(v7)) {
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
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
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
  GmakeAppl2(res,code_310  ,v1  ,v2  );
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

Gterm* fun_311(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /* [] */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label61:
      switch(GgetSymb(v2)) {
      case code_300: /* [] */
        v7= GgetArgument(v2,0);
        switch(GgetInt(v7)) {
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
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
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
  GmakeAppl2(res,code_311  ,v1  ,v2  );
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

Gterm* fun_312(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /* [] */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label67:
      switch(GgetSymb(v2)) {
      case code_300: /* [] */
        v7= GgetArgument(v2,0);
        switch(GgetInt(v7)) {
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
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
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
  GmakeAppl2(res,code_312  ,v1  ,v2  );
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

Gterm* fun_313(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /* [] */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label73:
      switch(GgetSymb(v2)) {
      case code_300: /* [] */
        v7= GgetArgument(v2,0);
        switch(GgetInt(v7)) {
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
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
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
  GmakeAppl2(res,code_313  ,v1  ,v2  );
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

Gterm* fun_314(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /* [] */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label79:
      switch(GgetSymb(v2)) {
      case code_300: /* [] */
        v7= GgetArgument(v2,0);
        switch(GgetInt(v7)) {
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
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
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
  GmakeAppl2(res,code_314  ,v1  ,v2  );
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

Gterm* fun_315(Gterm *v1 ) {
  Gterm *v2,*v3,*v4;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  default:
  label84:
    bitSet32_set(mask32,0);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
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
    GmakeAppl1(sv[2],code_300    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend14:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  GmakeAppl1(res,code_315  ,v1  );
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

Gterm* fun_316(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /* [] */
    v4= GgetArgument(v1,0);
    switch(GgetInt(v4)) {
    default:
    label88:
      bitSet32_set(mask32,0);
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
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
  GmakeAppl1(res,code_316  ,v1  );
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

Gterm* fun_317(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /* [] */
    v4= GgetArgument(v1,0);
    switch(GgetInt(v4)) {
    default:
    label92:
      bitSet32_set(mask32,0);
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
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
  GmakeAppl1(res,code_317  ,v1  );
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

Gterm* fun_318(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,3);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /* [] */
    v4= GgetArgument(v1,0);
    switch(GgetInt(v4)) {
    case 1: /* 1 */
      bitSet32_set(mask32,0);
      bitSet32_set(mask32,1);
      bitSet32_set(mask32,2);
      break;
    default:
      goto label95;
    }
    break;
  default:
  label95:
    bitSet32_set(mask32,0);
    bitSet32_set(mask32,2);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: fib()(var0) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend17;
    }
    /* if eq_int(,)(var0,[](0)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[1],code_300    ,(GsetIntegerTag(0))    );
    sv[0] = fun_309( v1,sv[1] );
    if( sv[0] != con_1 ) {
      fail();
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: [](1) */
    GmakeAppl1(sv[2],code_300    ,(GsetIntegerTag(1))    );
    res = sv[2] ;
    goto end;
    myend17:;
    CUTCLOSE(); /* Wheres */
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: fib()([](1)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend18;
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: [](1) */
    GmakeAppl1(sv[1],code_300    ,(GsetIntegerTag(1))    );
    res = sv[1] ;
    goto end;
    myend18:;
    CUTCLOSE(); /* Wheres */
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[4];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: fib()(var0) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend19;
    }
    /* if greater_int(,)(var0,[](1)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[1],code_300    ,(GsetIntegerTag(1))    );
    sv[0] = fun_311( v1,sv[1] );
    if( sv[0] != con_1 ) {
      fail();
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: plus(,)(fib()(minus(,)(var0,[](1))),fib()(minus(,)(var0,[](2)))) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_300    ,(GsetIntegerTag(1))    );
    sv[1] = fun_302( v1,sv[2] );
    sv[2] = fun_318( sv[1] );
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[3],code_300    ,(GsetIntegerTag(2))    );
    sv[1] = fun_302( v1,sv[3] );
    sv[3] = fun_318( sv[1] );
    sv[1] = fun_301( sv[2],sv[3] );
    res = sv[1] ;
    goto end;
    myend19:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  GmakeAppl1(res,code_318  ,v1  );
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

Gterm* fun_319(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /* [] */
    v4= GgetArgument(v1,0);
    switch(GgetInt(v4)) {
    case 1: /* 1 */
      bitSet32_set(mask32,0);
      bitSet32_set(mask32,1);
      break;
    default:
      goto label100;
    }
    break;
  default:
  label100:
    bitSet32_set(mask32,1);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: fac()([](1)) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend20;
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: [](1) */
    GmakeAppl1(sv[1],code_300    ,(GsetIntegerTag(1))    );
    res = sv[1] ;
    goto end;
    myend20:;
    CUTCLOSE(); /* Wheres */
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: fac()(var0) */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend21;
    }
    /* if greater_int(,)(var0,[](1)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[1],code_300    ,(GsetIntegerTag(1))    );
    sv[0] = fun_311( v1,sv[1] );
    if( sv[0] != con_1 ) {
      fail();
    }
    CUTCLOSE(); /* Wheres */
    /* rhs: time(,)(var0,fac()(minus(,)(var0,[](1)))) */
    // this=var0        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_300    ,(GsetIntegerTag(1))    );
    sv[1] = fun_302( v1,sv[2] );
    sv[2] = fun_319( sv[1] );
    sv[1] = fun_303( v1,sv[2] );
    res = sv[1] ;
    goto end;
    myend21:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  GmakeAppl1(res,code_319  ,v1  );
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
