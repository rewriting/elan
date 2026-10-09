#include "ans_completion.h"

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
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label3:
      switch(GgetSymb(v2)) {
      case code_301: /*  */
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
    /* lhs: plus(,)((var0),(var1)) */
    /* allDetEvaluation: det */
    /* where var2 := plus(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_3( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: (var2) */
    // this=var2        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend0:;
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
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label9:
      switch(GgetSymb(v2)) {
      case code_301: /*  */
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
    /* lhs: minus(,)((var0),(var1)) */
    /* allDetEvaluation: det */
    /* where var2 := minus(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_4( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: (var2) */
    // this=var2        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend1:;
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
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label15:
      switch(GgetSymb(v2)) {
      case code_301: /*  */
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
    /* lhs: time(,)((var0),(var1)) */
    /* allDetEvaluation: det */
    /* where var2 := time(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_5( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: (var2) */
    // this=var2        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend2:;
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
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label21:
      switch(GgetSymb(v2)) {
      case code_301: /*  */
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
    /* lhs: mod(,)((var0),(var1)) */
    /* allDetEvaluation: det */
    /* where var2 := mod(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_27( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: (var2) */
    // this=var2        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend3:;
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
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label27:
      switch(GgetSymb(v2)) {
      case code_301: /*  */
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
    /* lhs: and(,)((var0),(var1)) */
    /* allDetEvaluation: det */
    /* where var2 := and(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_28( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: (var2) */
    // this=var2        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend4:;
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
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label33:
      switch(GgetSymb(v2)) {
      case code_301: /*  */
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
    /* lhs: div(,)((var0),(var1)) */
    /* allDetEvaluation: det */
    /* where var2 := div(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_6( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: (var2) */
    // this=var2        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend5:;
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

Gterm* fun_308(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label39:
      switch(GgetSymb(v2)) {
      case code_301: /*  */
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
    /* lhs: or(,)((var0),(var1)) */
    /* allDetEvaluation: det */
    /* where var2 := or(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_29( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: (var2) */
    // this=var2        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend6:;
  }
match_fail:
  GmakeAppl2(res,code_308  ,v1  ,v2  );
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

Gterm* fun_309(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_301: /*  */
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
    /* lhs: umin()((var0)) */
    /* allDetEvaluation: det */
    /* where var1 := umin_builtinInt()(var0) */
    // this=var0        underAC=false        Instantiated=false
    sv[1] = fun_20( v4 );
    tmp = sv[0] = sv[1];
    /* rhs: (var1) */
    // this=var1        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend7:;
  }
match_fail:
  GmakeAppl1(res,code_309  ,v1  );
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
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label49:
      switch(GgetSymb(v2)) {
      case code_301: /*  */
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
    /* lhs: eq_int(,)((var0),(var1)) */
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
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label55:
      switch(GgetSymb(v2)) {
      case code_301: /*  */
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
    /* lhs: neq_int(,)((var0),(var1)) */
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
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label61:
      switch(GgetSymb(v2)) {
      case code_301: /*  */
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
    /* lhs: greater_int(,)((var0),(var1)) */
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
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label67:
      switch(GgetSymb(v2)) {
      case code_301: /*  */
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
    /* lhs: greatereq_int(,)((var0),(var1)) */
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
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label73:
      switch(GgetSymb(v2)) {
      case code_301: /*  */
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
    /* lhs: lesseq_int(,)((var0),(var1)) */
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

Gterm* fun_315(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    default:
    label79:
      switch(GgetSymb(v2)) {
      case code_301: /*  */
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
    /* lhs: less_int(,)((var0),(var1)) */
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
  GmakeAppl2(res,code_315  ,v1  ,v2  );
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
    /* where var1 := btoi_builtinInt()(var0) */
    // this=var0        underAC=false        Instantiated=false
    sv[1] = fun_25( v1 );
    tmp = sv[0] = sv[1];
    /* rhs: (var1) */
    // this=var1        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend14:;
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
  case code_301: /*  */
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
    /* lhs: itob_int()((var0)) */
    /* allDetEvaluation: det */
    /* rhs: itob_builtinInt()(var0) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_26( v4 );
    res = sv[0] ;
    goto end;
    myend15:;
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
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_301: /*  */
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
    /* lhs: valueOf()((var0)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v4 ;
    goto end;
    myend16:;
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

Gterm* fun_321(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_320: /* cons_identifier(,) */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label98:
      v6= GgetArgument(v1,1);
      switch(GgetSymb(v6)) {
      default:
      label99:
        switch(GgetSymb(v2)) {
        default:
        label100:
          bitSet32_set(mask32,1);
        }
      }
    }
    break;
  case code_319: /* nil */
    switch(GgetSymb(v2)) {
    default:
    label96:
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
    /* lhs: @(nil,var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend17:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: @(cons_identifier(,)(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: cons_identifier(,)(var0,@(var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_321( v6,v2 );
    GmakeAppl2(sv[1],code_320    ,v5    ,sv[0]    );
    res = sv[1] ;
    goto end;
    myend18:;
  }
match_fail:
  GmakeAppl2(res,code_321  ,v1  ,v2  );
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

Gterm* fun_323(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    case 1: /* 1 */
      switch(GgetSymb(v2)) {
      case code_320: /* cons_identifier(,) */
        v7= GgetArgument(v2,0);
        switch(GgetSymb(v7)) {
        default:
        label106:
          v8= GgetArgument(v2,1);
          switch(GgetSymb(v8)) {
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
    switch(GgetSymb(v2)) {
    case code_320: /* cons_identifier(,) */
      v7= GgetArgument(v2,0);
      switch(GgetSymb(v7)) {
      default:
      label110:
        v8= GgetArgument(v2,1);
        switch(GgetSymb(v8)) {
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
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thelem()((1),cons_identifier(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v7 ;
    goto end;
    myend19:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thelem()(var0,cons_identifier(,)(var1,var2)) */
    /* allDetEvaluation: det */
    /* where var3 := minus(,)(var0,(1)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_303( v1,sv[2] );
    tmp = sv[0] = sv[1];
    /* rhs: -thelem()(var3,var2) */
    // this=var3        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[2] = fun_323( sv[0],v8 );
    res = sv[2] ;
    goto end;
    myend20:;
  }
match_fail:
  GmakeAppl2(res,code_323  ,v1  ,v2  );
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

Gterm* fun_324(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_320: /* cons_identifier(,) */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label116:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
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
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: size_of_identifier_list()(nil) */
    /* allDetEvaluation: det */
    /* rhs: (0) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(0))    );
    res = sv[1] ;
    goto end;
    myend21:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: size_of_identifier_list()(cons_identifier(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: plus(,)((1),size_of_identifier_list()(var1)) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(1))    );
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_324( v5 );
    sv[2] = fun_302( sv[1],sv[0] );
    res = sv[2] ;
    goto end;
    myend22:;
  }
match_fail:
  GmakeAppl1(res,code_324  ,v1  );
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

Gterm* fun_325(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    case 0: /* 0 */
      switch(GgetSymb(v2)) {
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
    switch(GgetSymb(v2)) {
    default:
    label124:
      bitSet32_set(mask32,1);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: ccat(,)((0),var0) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_319 ;
    goto end;
    myend23:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: ccat(,)(var0,var1) */
    /* allDetEvaluation: det */
    /* if greater_int(,)(var0,(0)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(0))    );
    sv[0] = fun_312( v1,sv[1] );
    if( sv[0] != con_1 ) {
      goto myend24;
    }
    /* rhs: @(var1,ccat(,)(minus(,)(var0,(1)),var1)) */
    // this=var1        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_303( v1,sv[2] );
    // this=var1        underAC=false        Instantiated=false
    sv[2] = fun_325( sv[1],v2 );
    sv[1] = fun_321( v2,sv[2] );
    res = sv[1] ;
    goto end;
    myend24:;
  }
match_fail:
  GmakeAppl2(res,code_325  ,v1  ,v2  );
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

Gterm* fun_328(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_327: /* [,] */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label128:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label129:
        bitSet32_set(mask32,0);
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
    /* lhs: 1-th()([,](var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v4 ;
    goto end;
    myend25:;
  }
match_fail:
  GmakeAppl1(res,code_328  ,v1  );
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

Gterm* fun_329(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_327: /* [,] */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label133:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label134:
        bitSet32_set(mask32,0);
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
    /* lhs: 2-th()([,](var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var1 */
    // this=var1        underAC=false        Instantiated=false
    res = v5 ;
    goto end;
    myend26:;
  }
match_fail:
  GmakeAppl1(res,code_329  ,v1  );
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

Gterm* fun_330(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_327: /* [,] */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label138:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label139:
        bitSet32_set(mask32,0);
        bitSet32_set(mask32,1);
      }
    }
    break;
  default:
  label137:
    bitSet32_set(mask32,1);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: ispair()([,](var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: true */
    res = con_1 ;
    goto end;
    myend27:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: ispair()(var0) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend28:;
  }
match_fail:
  GmakeAppl1(res,code_330  ,v1  );
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

Gterm* fun_333(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_332: /* cons_pair[identifier,int](,) */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label146:
      v6= GgetArgument(v1,1);
      switch(GgetSymb(v6)) {
      default:
      label147:
        switch(GgetSymb(v2)) {
        default:
        label148:
          bitSet32_set(mask32,1);
        }
      }
    }
    break;
  case code_331: /* nil */
    switch(GgetSymb(v2)) {
    default:
    label144:
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
    /* lhs: @(nil,var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend29:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: @(cons_pair[identifier,int](,)(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: cons_pair[identifier,int](,)(var0,@(var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_333( v6,v2 );
    GmakeAppl2(sv[1],code_332    ,v5    ,sv[0]    );
    res = sv[1] ;
    goto end;
    myend30:;
  }
match_fail:
  GmakeAppl2(res,code_333  ,v1  ,v2  );
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

Gterm* fun_335(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    case 1: /* 1 */
      switch(GgetSymb(v2)) {
      case code_332: /* cons_pair[identifier,int](,) */
        v7= GgetArgument(v2,0);
        switch(GgetSymb(v7)) {
        default:
        label154:
          v8= GgetArgument(v2,1);
          switch(GgetSymb(v8)) {
          default:
          label155:
            bitSet32_set(mask32,0);
            bitSet32_set(mask32,1);
          }
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    default:
      goto label151;
    }
    break;
  default:
  label151:
    switch(GgetSymb(v2)) {
    case code_332: /* cons_pair[identifier,int](,) */
      v7= GgetArgument(v2,0);
      switch(GgetSymb(v7)) {
      default:
      label158:
        v8= GgetArgument(v2,1);
        switch(GgetSymb(v8)) {
        default:
        label159:
          bitSet32_set(mask32,1);
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thelem()((1),cons_pair[identifier,int](,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v7 ;
    goto end;
    myend31:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thelem()(var0,cons_pair[identifier,int](,)(var1,var2)) */
    /* allDetEvaluation: det */
    /* where var3 := minus(,)(var0,(1)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_303( v1,sv[2] );
    tmp = sv[0] = sv[1];
    /* rhs: -thelem()(var3,var2) */
    // this=var3        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[2] = fun_335( sv[0],v8 );
    res = sv[2] ;
    goto end;
    myend32:;
  }
match_fail:
  GmakeAppl2(res,code_335  ,v1  ,v2  );
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

Gterm* fun_336(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_332: /* cons_pair[identifier,int](,) */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label164:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label165:
        bitSet32_set(mask32,1);
      }
    }
    break;
  case code_331: /* nil */
    bitSet32_set(mask32,0);
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: size_of_pair[identifier,int]_list()(nil) */
    /* allDetEvaluation: det */
    /* rhs: (0) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(0))    );
    res = sv[1] ;
    goto end;
    myend33:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: size_of_pair[identifier,int]_list()(cons_pair[identifier,int](,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: plus(,)((1),size_of_pair[identifier,int]_list()(var1)) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(1))    );
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_336( v5 );
    sv[2] = fun_302( sv[1],sv[0] );
    res = sv[2] ;
    goto end;
    myend34:;
  }
match_fail:
  GmakeAppl1(res,code_336  ,v1  );
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

Gterm* fun_337(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    case 0: /* 0 */
      switch(GgetSymb(v2)) {
      default:
      label170:
        bitSet32_set(mask32,0);
        bitSet32_set(mask32,1);
      }
      break;
    default:
      goto label168;
    }
    break;
  default:
  label168:
    switch(GgetSymb(v2)) {
    default:
    label172:
      bitSet32_set(mask32,1);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: ccat(,)((0),var0) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_331 ;
    goto end;
    myend35:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: ccat(,)(var0,var1) */
    /* allDetEvaluation: det */
    /* if greater_int(,)(var0,(0)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(0))    );
    sv[0] = fun_312( v1,sv[1] );
    if( sv[0] != con_1 ) {
      goto myend36;
    }
    /* rhs: @(var1,ccat(,)(minus(,)(var0,(1)),var1)) */
    // this=var1        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_303( v1,sv[2] );
    // this=var1        underAC=false        Instantiated=false
    sv[2] = fun_337( sv[1],v2 );
    sv[1] = fun_333( v2,sv[2] );
    res = sv[1] ;
    goto end;
    myend36:;
  }
match_fail:
  GmakeAppl2(res,code_337  ,v1  ,v2  );
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

Gterm* fun_339( ) {
  Gterm *v1,*v2;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  bitSet32_set(mask32,0);
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[5];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: Vars */
    /* allDetEvaluation: det */
    /* rhs: cons_identifier(,)((120),cons_identifier(,)((121),cons_identifier(,)((122),nil))) */
    GmakeAppl1(sv[1],code_300    ,(GsetIdentifierTag(120))    );
    GmakeAppl1(sv[2],code_300    ,(GsetIdentifierTag(121))    );
    GmakeAppl1(sv[3],code_300    ,(GsetIdentifierTag(122))    );
    GmakeAppl2(sv[4],code_320    ,sv[3]    ,con_319    );
    GmakeAppl2(sv[0],code_320    ,sv[2]    ,sv[4]    );
    GmakeAppl2(sv[2],code_320    ,sv[1]    ,sv[0]    );
    res = sv[2] ;
    goto end;
    myend37:;
  }
match_fail:
  GmakeAppl0(res,code_339);
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

Gterm* fun_340( ) {
  Gterm *v1,*v2;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  bitSet32_set(mask32,0);
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[13];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: Ops */
    /* allDetEvaluation: det */
    /* rhs: cons_pair[identifier,int](,)([,]((102),(2)),cons_pair[identifier,int](,)([,]((150),(0)),cons_pair[identifier,int](,)([,]((151),(0)),cons_pair[identifier,int](,)([,]((152),(0)),cons_pair[identifier,int](,)([,]((153),(0)),cons_pair[identifier,int](,)([,]((154),(0)),cons_pair[identifier,int](,)([,]((365),(1)),cons_pair[identifier,int](,)([,]((155),(1)),cons_pair[identifier,int](,)([,]((156),(1)),cons_pair[identifier,int](,)([,]((368),(1)),cons_pair[identifier,int](,)([,]((369),(1)),nil))))))))))) */
    GmakeAppl1(sv[1],code_300    ,(GsetIdentifierTag(102))    );
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(2))    );
    GmakeAppl2(sv[0],code_327    ,sv[1]    ,sv[2]    );
    GmakeAppl1(sv[2],code_300    ,(GsetIdentifierTag(150))    );
    GmakeAppl1(sv[3],code_301    ,(GsetIntegerTag(0))    );
    GmakeAppl2(sv[1],code_327    ,sv[2]    ,sv[3]    );
    GmakeAppl1(sv[3],code_300    ,(GsetIdentifierTag(151))    );
    GmakeAppl1(sv[4],code_301    ,(GsetIntegerTag(0))    );
    GmakeAppl2(sv[2],code_327    ,sv[3]    ,sv[4]    );
    GmakeAppl1(sv[4],code_300    ,(GsetIdentifierTag(152))    );
    GmakeAppl1(sv[5],code_301    ,(GsetIntegerTag(0))    );
    GmakeAppl2(sv[3],code_327    ,sv[4]    ,sv[5]    );
    GmakeAppl1(sv[5],code_300    ,(GsetIdentifierTag(153))    );
    GmakeAppl1(sv[6],code_301    ,(GsetIntegerTag(0))    );
    GmakeAppl2(sv[4],code_327    ,sv[5]    ,sv[6]    );
    GmakeAppl1(sv[6],code_300    ,(GsetIdentifierTag(154))    );
    GmakeAppl1(sv[7],code_301    ,(GsetIntegerTag(0))    );
    GmakeAppl2(sv[5],code_327    ,sv[6]    ,sv[7]    );
    GmakeAppl1(sv[7],code_300    ,(GsetIdentifierTag(365))    );
    GmakeAppl1(sv[8],code_301    ,(GsetIntegerTag(1))    );
    GmakeAppl2(sv[6],code_327    ,sv[7]    ,sv[8]    );
    GmakeAppl1(sv[8],code_300    ,(GsetIdentifierTag(155))    );
    GmakeAppl1(sv[9],code_301    ,(GsetIntegerTag(1))    );
    GmakeAppl2(sv[7],code_327    ,sv[8]    ,sv[9]    );
    GmakeAppl1(sv[9],code_300    ,(GsetIdentifierTag(156))    );
    GmakeAppl1(sv[10],code_301    ,(GsetIntegerTag(1))    );
    GmakeAppl2(sv[8],code_327    ,sv[9]    ,sv[10]    );
    GmakeAppl1(sv[10],code_300    ,(GsetIdentifierTag(368))    );
    GmakeAppl1(sv[11],code_301    ,(GsetIntegerTag(1))    );
    GmakeAppl2(sv[9],code_327    ,sv[10]    ,sv[11]    );
    GmakeAppl1(sv[11],code_300    ,(GsetIdentifierTag(369))    );
    GmakeAppl1(sv[12],code_301    ,(GsetIntegerTag(1))    );
    GmakeAppl2(sv[10],code_327    ,sv[11]    ,sv[12]    );
    GmakeAppl2(sv[12],code_332    ,sv[10]    ,con_331    );
    GmakeAppl2(sv[10],code_332    ,sv[9]    ,sv[12]    );
    GmakeAppl2(sv[9],code_332    ,sv[8]    ,sv[10]    );
    GmakeAppl2(sv[8],code_332    ,sv[7]    ,sv[9]    );
    GmakeAppl2(sv[7],code_332    ,sv[6]    ,sv[8]    );
    GmakeAppl2(sv[6],code_332    ,sv[5]    ,sv[7]    );
    GmakeAppl2(sv[5],code_332    ,sv[4]    ,sv[6]    );
    GmakeAppl2(sv[4],code_332    ,sv[3]    ,sv[5]    );
    GmakeAppl2(sv[3],code_332    ,sv[2]    ,sv[4]    );
    GmakeAppl2(sv[2],code_332    ,sv[1]    ,sv[3]    );
    GmakeAppl2(sv[1],code_332    ,sv[0]    ,sv[2]    );
    res = sv[1] ;
    goto end;
    myend38:;
  }
match_fail:
  GmakeAppl0(res,code_340);
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

Gterm* fun_341( ) {
  Gterm *v1,*v2;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  bitSet32_set(mask32,0);
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[13];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: Prec */
    /* allDetEvaluation: det */
    /* rhs: cons_pair[identifier,int](,)([,]((150),(1)),cons_pair[identifier,int](,)([,]((151),(2)),cons_pair[identifier,int](,)([,]((152),(3)),cons_pair[identifier,int](,)([,]((153),(4)),cons_pair[identifier,int](,)([,]((154),(5)),cons_pair[identifier,int](,)([,]((102),(6)),cons_pair[identifier,int](,)([,]((365),(7)),cons_pair[identifier,int](,)([,]((155),(8)),cons_pair[identifier,int](,)([,]((156),(9)),cons_pair[identifier,int](,)([,]((368),(10)),cons_pair[identifier,int](,)([,]((369),(11)),nil))))))))))) */
    GmakeAppl1(sv[1],code_300    ,(GsetIdentifierTag(150))    );
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    GmakeAppl2(sv[0],code_327    ,sv[1]    ,sv[2]    );
    GmakeAppl1(sv[2],code_300    ,(GsetIdentifierTag(151))    );
    GmakeAppl1(sv[3],code_301    ,(GsetIntegerTag(2))    );
    GmakeAppl2(sv[1],code_327    ,sv[2]    ,sv[3]    );
    GmakeAppl1(sv[3],code_300    ,(GsetIdentifierTag(152))    );
    GmakeAppl1(sv[4],code_301    ,(GsetIntegerTag(3))    );
    GmakeAppl2(sv[2],code_327    ,sv[3]    ,sv[4]    );
    GmakeAppl1(sv[4],code_300    ,(GsetIdentifierTag(153))    );
    GmakeAppl1(sv[5],code_301    ,(GsetIntegerTag(4))    );
    GmakeAppl2(sv[3],code_327    ,sv[4]    ,sv[5]    );
    GmakeAppl1(sv[5],code_300    ,(GsetIdentifierTag(154))    );
    GmakeAppl1(sv[6],code_301    ,(GsetIntegerTag(5))    );
    GmakeAppl2(sv[4],code_327    ,sv[5]    ,sv[6]    );
    GmakeAppl1(sv[6],code_300    ,(GsetIdentifierTag(102))    );
    GmakeAppl1(sv[7],code_301    ,(GsetIntegerTag(6))    );
    GmakeAppl2(sv[5],code_327    ,sv[6]    ,sv[7]    );
    GmakeAppl1(sv[7],code_300    ,(GsetIdentifierTag(365))    );
    GmakeAppl1(sv[8],code_301    ,(GsetIntegerTag(7))    );
    GmakeAppl2(sv[6],code_327    ,sv[7]    ,sv[8]    );
    GmakeAppl1(sv[8],code_300    ,(GsetIdentifierTag(155))    );
    GmakeAppl1(sv[9],code_301    ,(GsetIntegerTag(8))    );
    GmakeAppl2(sv[7],code_327    ,sv[8]    ,sv[9]    );
    GmakeAppl1(sv[9],code_300    ,(GsetIdentifierTag(156))    );
    GmakeAppl1(sv[10],code_301    ,(GsetIntegerTag(9))    );
    GmakeAppl2(sv[8],code_327    ,sv[9]    ,sv[10]    );
    GmakeAppl1(sv[10],code_300    ,(GsetIdentifierTag(368))    );
    GmakeAppl1(sv[11],code_301    ,(GsetIntegerTag(10))    );
    GmakeAppl2(sv[9],code_327    ,sv[10]    ,sv[11]    );
    GmakeAppl1(sv[11],code_300    ,(GsetIdentifierTag(369))    );
    GmakeAppl1(sv[12],code_301    ,(GsetIntegerTag(11))    );
    GmakeAppl2(sv[10],code_327    ,sv[11]    ,sv[12]    );
    GmakeAppl2(sv[12],code_332    ,sv[10]    ,con_331    );
    GmakeAppl2(sv[10],code_332    ,sv[9]    ,sv[12]    );
    GmakeAppl2(sv[9],code_332    ,sv[8]    ,sv[10]    );
    GmakeAppl2(sv[8],code_332    ,sv[7]    ,sv[9]    );
    GmakeAppl2(sv[7],code_332    ,sv[6]    ,sv[8]    );
    GmakeAppl2(sv[6],code_332    ,sv[5]    ,sv[7]    );
    GmakeAppl2(sv[5],code_332    ,sv[4]    ,sv[6]    );
    GmakeAppl2(sv[4],code_332    ,sv[3]    ,sv[5]    );
    GmakeAppl2(sv[3],code_332    ,sv[2]    ,sv[4]    );
    GmakeAppl2(sv[2],code_332    ,sv[1]    ,sv[3]    );
    GmakeAppl2(sv[1],code_332    ,sv[0]    ,sv[2]    );
    res = sv[1] ;
    goto end;
    myend39:;
  }
match_fail:
  GmakeAppl0(res,code_341);
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

Gterm* fun_347(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_346: /* cons_int(,) */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label184:
      v6= GgetArgument(v1,1);
      switch(GgetSymb(v6)) {
      default:
      label185:
        switch(GgetSymb(v2)) {
        default:
        label186:
          bitSet32_set(mask32,1);
        }
      }
    }
    break;
  case code_345: /* nil */
    switch(GgetSymb(v2)) {
    default:
    label182:
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
    /* lhs: @(nil,var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend40:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: @(cons_int(,)(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: cons_int(,)(var0,@(var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_347( v6,v2 );
    GmakeAppl2(sv[1],code_346    ,v5    ,sv[0]    );
    res = sv[1] ;
    goto end;
    myend41:;
  }
match_fail:
  GmakeAppl2(res,code_347  ,v1  ,v2  );
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

Gterm* fun_349(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    case 1: /* 1 */
      switch(GgetSymb(v2)) {
      case code_346: /* cons_int(,) */
        v7= GgetArgument(v2,0);
        switch(GgetSymb(v7)) {
        default:
        label192:
          v8= GgetArgument(v2,1);
          switch(GgetSymb(v8)) {
          default:
          label193:
            bitSet32_set(mask32,0);
            bitSet32_set(mask32,1);
          }
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    default:
      goto label189;
    }
    break;
  default:
  label189:
    switch(GgetSymb(v2)) {
    case code_346: /* cons_int(,) */
      v7= GgetArgument(v2,0);
      switch(GgetSymb(v7)) {
      default:
      label196:
        v8= GgetArgument(v2,1);
        switch(GgetSymb(v8)) {
        default:
        label197:
          bitSet32_set(mask32,1);
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thelem()((1),cons_int(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v7 ;
    goto end;
    myend42:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thelem()(var0,cons_int(,)(var1,var2)) */
    /* allDetEvaluation: det */
    /* where var3 := minus(,)(var0,(1)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_303( v1,sv[2] );
    tmp = sv[0] = sv[1];
    /* rhs: -thelem()(var3,var2) */
    // this=var3        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[2] = fun_349( sv[0],v8 );
    res = sv[2] ;
    goto end;
    myend43:;
  }
match_fail:
  GmakeAppl2(res,code_349  ,v1  ,v2  );
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

Gterm* fun_350(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_346: /* cons_int(,) */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label202:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label203:
        bitSet32_set(mask32,1);
      }
    }
    break;
  case code_345: /* nil */
    bitSet32_set(mask32,0);
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: size_of_int_list()(nil) */
    /* allDetEvaluation: det */
    /* rhs: (0) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(0))    );
    res = sv[1] ;
    goto end;
    myend44:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: size_of_int_list()(cons_int(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: plus(,)((1),size_of_int_list()(var1)) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(1))    );
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_350( v5 );
    sv[2] = fun_302( sv[1],sv[0] );
    res = sv[2] ;
    goto end;
    myend45:;
  }
match_fail:
  GmakeAppl1(res,code_350  ,v1  );
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

Gterm* fun_351(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    case 0: /* 0 */
      switch(GgetSymb(v2)) {
      default:
      label208:
        bitSet32_set(mask32,0);
        bitSet32_set(mask32,1);
      }
      break;
    default:
      goto label206;
    }
    break;
  default:
  label206:
    switch(GgetSymb(v2)) {
    default:
    label210:
      bitSet32_set(mask32,1);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: ccat(,)((0),var0) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_345 ;
    goto end;
    myend46:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: ccat(,)(var0,var1) */
    /* allDetEvaluation: det */
    /* if greater_int(,)(var0,(0)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(0))    );
    sv[0] = fun_312( v1,sv[1] );
    if( sv[0] != con_1 ) {
      goto myend47;
    }
    /* rhs: @(var1,ccat(,)(minus(,)(var0,(1)),var1)) */
    // this=var1        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_303( v1,sv[2] );
    // this=var1        underAC=false        Instantiated=false
    sv[2] = fun_351( sv[1],v2 );
    sv[1] = fun_347( v2,sv[2] );
    res = sv[1] ;
    goto end;
    myend47:;
  }
match_fail:
  GmakeAppl2(res,code_351  ,v1  ,v2  );
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

Gterm* fun_355(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_354: /* cons_variable(,) */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label216:
      v6= GgetArgument(v1,1);
      switch(GgetSymb(v6)) {
      default:
      label217:
        switch(GgetSymb(v2)) {
        default:
        label218:
          bitSet32_set(mask32,1);
        }
      }
    }
    break;
  case code_353: /* nil */
    switch(GgetSymb(v2)) {
    default:
    label214:
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
    /* lhs: @(nil,var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend48:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: @(cons_variable(,)(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: cons_variable(,)(var0,@(var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_355( v6,v2 );
    GmakeAppl2(sv[1],code_354    ,v5    ,sv[0]    );
    res = sv[1] ;
    goto end;
    myend49:;
  }
match_fail:
  GmakeAppl2(res,code_355  ,v1  ,v2  );
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

Gterm* fun_357(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    case 1: /* 1 */
      switch(GgetSymb(v2)) {
      case code_354: /* cons_variable(,) */
        v7= GgetArgument(v2,0);
        switch(GgetSymb(v7)) {
        default:
        label224:
          v8= GgetArgument(v2,1);
          switch(GgetSymb(v8)) {
          default:
          label225:
            bitSet32_set(mask32,0);
            bitSet32_set(mask32,1);
          }
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    default:
      goto label221;
    }
    break;
  default:
  label221:
    switch(GgetSymb(v2)) {
    case code_354: /* cons_variable(,) */
      v7= GgetArgument(v2,0);
      switch(GgetSymb(v7)) {
      default:
      label228:
        v8= GgetArgument(v2,1);
        switch(GgetSymb(v8)) {
        default:
        label229:
          bitSet32_set(mask32,1);
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thelem()((1),cons_variable(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v7 ;
    goto end;
    myend50:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thelem()(var0,cons_variable(,)(var1,var2)) */
    /* allDetEvaluation: det */
    /* where var3 := minus(,)(var0,(1)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_303( v1,sv[2] );
    tmp = sv[0] = sv[1];
    /* rhs: -thelem()(var3,var2) */
    // this=var3        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[2] = fun_357( sv[0],v8 );
    res = sv[2] ;
    goto end;
    myend51:;
  }
match_fail:
  GmakeAppl2(res,code_357  ,v1  ,v2  );
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

Gterm* fun_358(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_354: /* cons_variable(,) */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label234:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label235:
        bitSet32_set(mask32,1);
      }
    }
    break;
  case code_353: /* nil */
    bitSet32_set(mask32,0);
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: size_of_variable_list()(nil) */
    /* allDetEvaluation: det */
    /* rhs: (0) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(0))    );
    res = sv[1] ;
    goto end;
    myend52:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: size_of_variable_list()(cons_variable(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: plus(,)((1),size_of_variable_list()(var1)) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(1))    );
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_358( v5 );
    sv[2] = fun_302( sv[1],sv[0] );
    res = sv[2] ;
    goto end;
    myend53:;
  }
match_fail:
  GmakeAppl1(res,code_358  ,v1  );
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

Gterm* fun_359(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    case 0: /* 0 */
      switch(GgetSymb(v2)) {
      default:
      label240:
        bitSet32_set(mask32,0);
        bitSet32_set(mask32,1);
      }
      break;
    default:
      goto label238;
    }
    break;
  default:
  label238:
    switch(GgetSymb(v2)) {
    default:
    label242:
      bitSet32_set(mask32,1);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: ccat(,)((0),var0) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_353 ;
    goto end;
    myend54:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: ccat(,)(var0,var1) */
    /* allDetEvaluation: det */
    /* if greater_int(,)(var0,(0)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(0))    );
    sv[0] = fun_312( v1,sv[1] );
    if( sv[0] != con_1 ) {
      goto myend55;
    }
    /* rhs: @(var1,ccat(,)(minus(,)(var0,(1)),var1)) */
    // this=var1        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_303( v1,sv[2] );
    // this=var1        underAC=false        Instantiated=false
    sv[2] = fun_359( sv[1],v2 );
    sv[1] = fun_355( v2,sv[2] );
    res = sv[1] ;
    goto end;
    myend55:;
  }
match_fail:
  GmakeAppl2(res,code_359  ,v1  ,v2  );
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

Gterm* fun_363(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,12);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_362: /*  */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label264:
      bitSet32_set(mask32,11);
    }
    break;
  case code_380: /* i5() */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label262:
      bitSet32_set(mask32,10);
    }
    break;
  case code_379: /* i4() */
    v6= GgetArgument(v1,0);
    switch(GgetSymb(v6)) {
    default:
    label260:
      bitSet32_set(mask32,9);
    }
    break;
  case code_378: /* i3() */
    v7= GgetArgument(v1,0);
    switch(GgetSymb(v7)) {
    default:
    label258:
      bitSet32_set(mask32,8);
    }
    break;
  case code_377: /* i2() */
    v8= GgetArgument(v1,0);
    switch(GgetSymb(v8)) {
    default:
    label256:
      bitSet32_set(mask32,7);
    }
    break;
  case code_376: /* i1() */
    v9= GgetArgument(v1,0);
    switch(GgetSymb(v9)) {
    default:
    label254:
      bitSet32_set(mask32,6);
    }
    break;
  case code_374: /* e5 */
    bitSet32_set(mask32,5);
    break;
  case code_373: /* e4 */
    bitSet32_set(mask32,4);
    break;
  case code_372: /* e3 */
    bitSet32_set(mask32,3);
    break;
  case code_371: /* e2 */
    bitSet32_set(mask32,2);
    break;
  case code_370: /* e1 */
    bitSet32_set(mask32,1);
    break;
  case code_375: /* f(,) */
    v10= GgetArgument(v1,0);
    switch(GgetSymb(v10)) {
    default:
    label246:
      v11= GgetArgument(v1,1);
      switch(GgetSymb(v11)) {
      default:
      label247:
        bitSet32_set(mask32,0);
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: head()(f(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: (102) */
    GmakeAppl1(sv[1],code_300    ,(GsetIdentifierTag(102))    );
    res = sv[1] ;
    goto end;
    myend56:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: head()(e1) */
    /* allDetEvaluation: det */
    /* rhs: (150) */
    GmakeAppl1(sv[1],code_300    ,(GsetIdentifierTag(150))    );
    res = sv[1] ;
    goto end;
    myend57:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: head()(e2) */
    /* allDetEvaluation: det */
    /* rhs: (151) */
    GmakeAppl1(sv[1],code_300    ,(GsetIdentifierTag(151))    );
    res = sv[1] ;
    goto end;
    myend58:;
  }
  if(bitSet32_get(mask32,3)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: head()(e3) */
    /* allDetEvaluation: det */
    /* rhs: (152) */
    GmakeAppl1(sv[1],code_300    ,(GsetIdentifierTag(152))    );
    res = sv[1] ;
    goto end;
    myend59:;
  }
  if(bitSet32_get(mask32,4)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: head()(e4) */
    /* allDetEvaluation: det */
    /* rhs: (153) */
    GmakeAppl1(sv[1],code_300    ,(GsetIdentifierTag(153))    );
    res = sv[1] ;
    goto end;
    myend60:;
  }
  if(bitSet32_get(mask32,5)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: head()(e5) */
    /* allDetEvaluation: det */
    /* rhs: (154) */
    GmakeAppl1(sv[1],code_300    ,(GsetIdentifierTag(154))    );
    res = sv[1] ;
    goto end;
    myend61:;
  }
  if(bitSet32_get(mask32,6)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: head()(i1()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: (365) */
    GmakeAppl1(sv[1],code_300    ,(GsetIdentifierTag(365))    );
    res = sv[1] ;
    goto end;
    myend62:;
  }
  if(bitSet32_get(mask32,7)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: head()(i2()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: (155) */
    GmakeAppl1(sv[1],code_300    ,(GsetIdentifierTag(155))    );
    res = sv[1] ;
    goto end;
    myend63:;
  }
  if(bitSet32_get(mask32,8)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: head()(i3()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: (156) */
    GmakeAppl1(sv[1],code_300    ,(GsetIdentifierTag(156))    );
    res = sv[1] ;
    goto end;
    myend64:;
  }
  if(bitSet32_get(mask32,9)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: head()(i4()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: (368) */
    GmakeAppl1(sv[1],code_300    ,(GsetIdentifierTag(368))    );
    res = sv[1] ;
    goto end;
    myend65:;
  }
  if(bitSet32_get(mask32,10)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: head()(i5()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: (369) */
    GmakeAppl1(sv[1],code_300    ,(GsetIdentifierTag(369))    );
    res = sv[1] ;
    goto end;
    myend66:;
  }
  if(bitSet32_get(mask32,11)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: head()((var0)) */
    /* allDetEvaluation: det */
    /* rhs: (329) */
    GmakeAppl1(sv[1],code_300    ,(GsetIdentifierTag(329))    );
    res = sv[1] ;
    goto end;
    myend67:;
  }
match_fail:
  GmakeAppl1(res,code_363  ,v1  );
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

Gterm* fun_364(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16,*v17;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,7);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    case 2: /* 2 */
      switch(GgetSymb(v2)) {
      case code_375: /* f(,) */
        v7= GgetArgument(v2,0);
        switch(GgetSymb(v7)) {
        default:
        label274:
          v8= GgetArgument(v2,1);
          switch(GgetSymb(v8)) {
          default:
          label275:
            bitSet32_set(mask32,1);
          }
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    case 1: /* 1 */
      switch(GgetSymb(v2)) {
      case code_380: /* i5() */
        v10= GgetArgument(v2,0);
        switch(GgetSymb(v10)) {
        default:
        label285:
          bitSet32_set(mask32,6);
        }
        break;
      case code_379: /* i4() */
        v11= GgetArgument(v2,0);
        switch(GgetSymb(v11)) {
        default:
        label283:
          bitSet32_set(mask32,5);
        }
        break;
      case code_378: /* i3() */
        v12= GgetArgument(v2,0);
        switch(GgetSymb(v12)) {
        default:
        label281:
          bitSet32_set(mask32,4);
        }
        break;
      case code_377: /* i2() */
        v13= GgetArgument(v2,0);
        switch(GgetSymb(v13)) {
        default:
        label279:
          bitSet32_set(mask32,3);
        }
        break;
      case code_376: /* i1() */
        v14= GgetArgument(v2,0);
        switch(GgetSymb(v14)) {
        default:
        label277:
          bitSet32_set(mask32,2);
        }
        break;
      case code_375: /* f(,) */
        v15= GgetArgument(v2,0);
        switch(GgetSymb(v15)) {
        default:
        label270:
          v16= GgetArgument(v2,1);
          switch(GgetSymb(v16)) {
          default:
          label271:
            bitSet32_set(mask32,0);
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
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thsubterm()((1),f(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v15 ;
    goto end;
    myend68:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thsubterm()((2),f(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var1 */
    // this=var1        underAC=false        Instantiated=false
    res = v8 ;
    goto end;
    myend69:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thsubterm()((1),i1()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v14 ;
    goto end;
    myend70:;
  }
  if(bitSet32_get(mask32,3)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thsubterm()((1),i2()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v13 ;
    goto end;
    myend71:;
  }
  if(bitSet32_get(mask32,4)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thsubterm()((1),i3()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v12 ;
    goto end;
    myend72:;
  }
  if(bitSet32_get(mask32,5)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thsubterm()((1),i4()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v11 ;
    goto end;
    myend73:;
  }
  if(bitSet32_get(mask32,6)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thsubterm()((1),i5()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v10 ;
    goto end;
    myend74:;
  }
match_fail:
  GmakeAppl2(res,code_364  ,v1  ,v2  );
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

Gterm* fun_365(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_362: /*  */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label289:
      bitSet32_set(mask32,0);
      bitSet32_set(mask32,1);
    }
    break;
  default:
  label288:
    bitSet32_set(mask32,1);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: isvar()((var0)) */
    /* allDetEvaluation: det */
    /* rhs: true */
    res = con_1 ;
    goto end;
    myend75:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: isvar()(var0) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend76:;
  }
match_fail:
  GmakeAppl1(res,code_365  ,v1  );
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

Gterm* fun_367(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  default:
  label293:
    switch(GgetSymb(v2)) {
    case code_346: /* cons_int(,) */
      v6= GgetArgument(v2,0);
      switch(GgetSymb(v6)) {
      default:
      label296:
        v7= GgetArgument(v2,1);
        switch(GgetSymb(v7)) {
        default:
        label297:
          bitSet32_set(mask32,1);
        }
      }
      break;
    case code_345: /* nil */
      bitSet32_set(mask32,0);
      break;
    /* matching is not complete: jumpNode is null */
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: at(var0,nil) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v1 ;
    goto end;
    myend77:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: at(var0,cons_int(,)(var1,var2)) */
    /* allDetEvaluation: det */
    /* rhs: at(-thsubterm()(var1,var0),var2) */
    // this=var1        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_364( v6,v1 );
    // this=var2        underAC=false        Instantiated=false
    sv[1] = fun_367( sv[0],v7 );
    res = sv[1] ;
    goto end;
    myend78:;
  }
match_fail:
  GmakeAppl2(res,code_367  ,v1  ,v2  );
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

Gterm* fun_368(Gterm *v1,Gterm *v2,Gterm *v3 ) {
  Gterm *v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16,*v17,*v18,*v19,*v20,*v21,*v22,*v23,*v24,*v25,*v26,*v27,*v28,*v29,*v30,*v31,*v32,*v33,*v34,*v35,*v36,*v37,*v38,*v39,*v40,*v41,*v42,*v43,*v44,*v45,*v46;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,8);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_380: /* i5() */
    v6= GgetArgument(v1,0);
    switch(GgetSymb(v6)) {
    default:
    label339:
      switch(GgetSymb(v2)) {
      default:
      label340:
        switch(GgetSymb(v3)) {
        case code_345: /* nil */
          bitSet32_set(mask32,7);
          break;
        case code_346: /* cons_int(,) */
          v9= GgetArgument(v3,0);
          switch(GgetSymb(v9)) {
          case code_301: /*  */
            v10= GgetArgument(v9,0);
            switch(GgetInt(v10)) {
            case 1: /* 1 */
              v11= GgetArgument(v3,1);
              switch(GgetSymb(v11)) {
              default:
              label344:
                bitSet32_set(mask32,6);
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
      }
    }
    break;
  case code_379: /* i4() */
    v12= GgetArgument(v1,0);
    switch(GgetSymb(v12)) {
    default:
    label332:
      switch(GgetSymb(v2)) {
      default:
      label333:
        switch(GgetSymb(v3)) {
        case code_345: /* nil */
          bitSet32_set(mask32,7);
          break;
        case code_346: /* cons_int(,) */
          v15= GgetArgument(v3,0);
          switch(GgetSymb(v15)) {
          case code_301: /*  */
            v16= GgetArgument(v15,0);
            switch(GgetInt(v16)) {
            case 1: /* 1 */
              v17= GgetArgument(v3,1);
              switch(GgetSymb(v17)) {
              default:
              label337:
                bitSet32_set(mask32,5);
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
      }
    }
    break;
  case code_378: /* i3() */
    v18= GgetArgument(v1,0);
    switch(GgetSymb(v18)) {
    default:
    label325:
      switch(GgetSymb(v2)) {
      default:
      label326:
        switch(GgetSymb(v3)) {
        case code_345: /* nil */
          bitSet32_set(mask32,7);
          break;
        case code_346: /* cons_int(,) */
          v21= GgetArgument(v3,0);
          switch(GgetSymb(v21)) {
          case code_301: /*  */
            v22= GgetArgument(v21,0);
            switch(GgetInt(v22)) {
            case 1: /* 1 */
              v23= GgetArgument(v3,1);
              switch(GgetSymb(v23)) {
              default:
              label330:
                bitSet32_set(mask32,4);
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
      }
    }
    break;
  case code_377: /* i2() */
    v24= GgetArgument(v1,0);
    switch(GgetSymb(v24)) {
    default:
    label318:
      switch(GgetSymb(v2)) {
      default:
      label319:
        switch(GgetSymb(v3)) {
        case code_345: /* nil */
          bitSet32_set(mask32,7);
          break;
        case code_346: /* cons_int(,) */
          v27= GgetArgument(v3,0);
          switch(GgetSymb(v27)) {
          case code_301: /*  */
            v28= GgetArgument(v27,0);
            switch(GgetInt(v28)) {
            case 1: /* 1 */
              v29= GgetArgument(v3,1);
              switch(GgetSymb(v29)) {
              default:
              label323:
                bitSet32_set(mask32,3);
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
      }
    }
    break;
  case code_376: /* i1() */
    v30= GgetArgument(v1,0);
    switch(GgetSymb(v30)) {
    default:
    label311:
      switch(GgetSymb(v2)) {
      default:
      label312:
        switch(GgetSymb(v3)) {
        case code_345: /* nil */
          bitSet32_set(mask32,7);
          break;
        case code_346: /* cons_int(,) */
          v33= GgetArgument(v3,0);
          switch(GgetSymb(v33)) {
          case code_301: /*  */
            v34= GgetArgument(v33,0);
            switch(GgetInt(v34)) {
            case 1: /* 1 */
              v35= GgetArgument(v3,1);
              switch(GgetSymb(v35)) {
              default:
              label316:
                bitSet32_set(mask32,2);
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
      }
    }
    break;
  case code_375: /* f(,) */
    v36= GgetArgument(v1,0);
    switch(GgetSymb(v36)) {
    default:
    label301:
      v37= GgetArgument(v1,1);
      switch(GgetSymb(v37)) {
      default:
      label302:
        switch(GgetSymb(v2)) {
        default:
        label303:
          switch(GgetSymb(v3)) {
          case code_345: /* nil */
            bitSet32_set(mask32,7);
            break;
          case code_346: /* cons_int(,) */
            v40= GgetArgument(v3,0);
            switch(GgetSymb(v40)) {
            case code_301: /*  */
              v41= GgetArgument(v40,0);
              switch(GgetInt(v41)) {
              case 2: /* 2 */
                v42= GgetArgument(v3,1);
                switch(GgetSymb(v42)) {
                default:
                label309:
                  bitSet32_set(mask32,1);
                }
                break;
              case 1: /* 1 */
                v43= GgetArgument(v3,1);
                switch(GgetSymb(v43)) {
                default:
                label307:
                  bitSet32_set(mask32,0);
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
        }
      }
    }
    break;
  default:
  label300:
    switch(GgetSymb(v2)) {
    default:
    label346:
      switch(GgetSymb(v3)) {
      case code_345: /* nil */
        bitSet32_set(mask32,7);
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: []at(f(,)(var0,var1),var2,cons_int(,)((1),var3)) */
    /* allDetEvaluation: det */
    /* rhs: f(,)([]at(var0,var2,var3),var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    sv[0] = fun_368( v36,v2,v43 );
    // this=var1        underAC=false        Instantiated=false
    GmakeAppl2(sv[1],code_375    ,sv[0]    ,v37    );
    res = sv[1] ;
    goto end;
    myend79:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: []at(f(,)(var0,var1),var2,cons_int(,)((2),var3)) */
    /* allDetEvaluation: det */
    /* rhs: f(,)(var0,[]at(var1,var2,var3)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    sv[0] = fun_368( v37,v2,v42 );
    GmakeAppl2(sv[1],code_375    ,v36    ,sv[0]    );
    res = sv[1] ;
    goto end;
    myend80:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: []at(i1()(var0),var1,cons_int(,)((1),var2)) */
    /* allDetEvaluation: det */
    /* rhs: i1()([]at(var0,var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_368( v30,v2,v35 );
    GmakeAppl1(sv[1],code_376    ,sv[0]    );
    res = sv[1] ;
    goto end;
    myend81:;
  }
  if(bitSet32_get(mask32,3)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: []at(i2()(var0),var1,cons_int(,)((1),var2)) */
    /* allDetEvaluation: det */
    /* rhs: i2()([]at(var0,var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_368( v24,v2,v29 );
    GmakeAppl1(sv[1],code_377    ,sv[0]    );
    res = sv[1] ;
    goto end;
    myend82:;
  }
  if(bitSet32_get(mask32,4)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: []at(i3()(var0),var1,cons_int(,)((1),var2)) */
    /* allDetEvaluation: det */
    /* rhs: i3()([]at(var0,var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_368( v18,v2,v23 );
    GmakeAppl1(sv[1],code_378    ,sv[0]    );
    res = sv[1] ;
    goto end;
    myend83:;
  }
  if(bitSet32_get(mask32,5)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: []at(i4()(var0),var1,cons_int(,)((1),var2)) */
    /* allDetEvaluation: det */
    /* rhs: i4()([]at(var0,var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_368( v12,v2,v17 );
    GmakeAppl1(sv[1],code_379    ,sv[0]    );
    res = sv[1] ;
    goto end;
    myend84:;
  }
  if(bitSet32_get(mask32,6)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: []at(i5()(var0),var1,cons_int(,)((1),var2)) */
    /* allDetEvaluation: det */
    /* rhs: i5()([]at(var0,var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_368( v6,v2,v11 );
    GmakeAppl1(sv[1],code_380    ,sv[0]    );
    res = sv[1] ;
    goto end;
    myend85:;
  }
  if(bitSet32_get(mask32,7)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: []at(var0,var1,nil) */
    /* allDetEvaluation: det */
    /* rhs: var1 */
    // this=var1        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend86:;
  }
match_fail:
  GmakeAppl3(res,code_368  ,v1  ,v2  ,v3  );
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

Gterm* fun_369(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_354: /* cons_variable(,) */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label359:
      v6= GgetArgument(v1,1);
      switch(GgetSymb(v6)) {
      default:
      label360:
        switch(GgetSymb(v2)) {
        default:
        label361:
          bitSet32_set(mask32,1);
        }
      }
    }
    break;
  case code_353: /* nil */
    switch(GgetSymb(v2)) {
    default:
    label357:
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
    /* lhs: rename_subst(,)(nil,var0) */
    /* allDetEvaluation: det */
    /* rhs: identity */
    res = con_342 ;
    goto end;
    myend87:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: rename_subst(,)(cons_variable(,)(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: o(->(var0,(var()(var2))),rename_subst(,)(var1,plus(,)(var2,(1)))) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    GmakeAppl1(sv[0],code_361    ,v2    );
    GmakeAppl1(sv[1],code_362    ,sv[0]    );
    GmakeAppl2(sv[0],code_343    ,v5    ,sv[1]    );
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_302( v2,sv[2] );
    sv[2] = fun_369( v6,sv[1] );
    GmakeAppl2(sv[1],code_344    ,sv[0]    ,sv[2]    );
    res = sv[1] ;
    goto end;
    myend88:;
  }
match_fail:
  GmakeAppl2(res,code_369  ,v1  ,v2  );
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

Gterm* fun_384( ) {
  Gterm *v1,*v2;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  bitSet32_set(mask32,0);
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[5];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: varlist_Vars */
    /* allDetEvaluation: det */
    /* rhs: cons_variable(,)(x,cons_variable(,)(y,cons_variable(,)(z,nil))) */
    GmakeAppl2(sv[4],code_354    ,con_383    ,con_353    );
    GmakeAppl2(sv[2],code_354    ,con_382    ,sv[4]    );
    GmakeAppl2(sv[1],code_354    ,con_381    ,sv[2]    );
    res = sv[1] ;
    goto end;
    myend89:;
  }
match_fail:
  GmakeAppl0(res,code_384);
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

Gterm* fun_385( ) {
  Gterm *v1,*v2;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  bitSet32_set(mask32,0);
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: varnum_Vars */
    /* allDetEvaluation: det */
    /* rhs: (3) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(3))    );
    res = sv[1] ;
    goto end;
    myend90:;
  }
match_fail:
  GmakeAppl0(res,code_385);
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

Gterm* fun_387(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  default:
  label368:
    switch(GgetSymb(v2)) {
    default:
    label369:
      bitSet32_set(mask32,0);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: >sig(var0,var1) */
    /* allDetEvaluation: det */
    /* rhs: greater_int(,)(precedence()(var0),precedence()(var1)) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_389( v1 );
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_389( v2 );
    sv[2] = fun_312( sv[0],sv[1] );
    res = sv[2] ;
    goto end;
    myend91:;
  }
match_fail:
  GmakeAppl2(res,code_387  ,v1  ,v2  );
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

Gterm* fun_388(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  default:
  label372:
    switch(GgetSymb(v2)) {
    default:
    label373:
      bitSet32_set(mask32,0);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: ==sig(var0,var1) */
    /* allDetEvaluation: det */
    /* rhs: eq_int(,)(precedence()(var0),precedence()(var1)) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_389( v1 );
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_389( v2 );
    sv[2] = fun_310( sv[0],sv[1] );
    res = sv[2] ;
    goto end;
    myend92:;
  }
match_fail:
  GmakeAppl2(res,code_388  ,v1  ,v2  );
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

Gterm* fun_389(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,12);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_300: /*  */
    v4= GgetArgument(v1,0);
    switch(GgetIdentifier(v4)) {
    case 369: /* 369 */
      bitSet32_set(mask32,10);
      bitSet32_set(mask32,11);
      break;
    case 368: /* 368 */
      bitSet32_set(mask32,9);
      bitSet32_set(mask32,11);
      break;
    case 156: /* 156 */
      bitSet32_set(mask32,8);
      bitSet32_set(mask32,11);
      break;
    case 155: /* 155 */
      bitSet32_set(mask32,7);
      bitSet32_set(mask32,11);
      break;
    case 365: /* 365 */
      bitSet32_set(mask32,6);
      bitSet32_set(mask32,11);
      break;
    case 102: /* 102 */
      bitSet32_set(mask32,5);
      bitSet32_set(mask32,11);
      break;
    case 154: /* 154 */
      bitSet32_set(mask32,4);
      bitSet32_set(mask32,11);
      break;
    case 153: /* 153 */
      bitSet32_set(mask32,3);
      bitSet32_set(mask32,11);
      break;
    case 152: /* 152 */
      bitSet32_set(mask32,2);
      bitSet32_set(mask32,11);
      break;
    case 151: /* 151 */
      bitSet32_set(mask32,1);
      bitSet32_set(mask32,11);
      break;
    case 150: /* 150 */
      bitSet32_set(mask32,0);
      bitSet32_set(mask32,11);
      break;
    default:
      goto label376;
    }
    break;
  default:
  label376:
    bitSet32_set(mask32,11);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: precedence()((150)) */
    /* allDetEvaluation: det */
    /* rhs: (1) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(1))    );
    res = sv[1] ;
    goto end;
    myend93:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: precedence()((151)) */
    /* allDetEvaluation: det */
    /* rhs: (2) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(2))    );
    res = sv[1] ;
    goto end;
    myend94:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: precedence()((152)) */
    /* allDetEvaluation: det */
    /* rhs: (3) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(3))    );
    res = sv[1] ;
    goto end;
    myend95:;
  }
  if(bitSet32_get(mask32,3)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: precedence()((153)) */
    /* allDetEvaluation: det */
    /* rhs: (4) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(4))    );
    res = sv[1] ;
    goto end;
    myend96:;
  }
  if(bitSet32_get(mask32,4)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: precedence()((154)) */
    /* allDetEvaluation: det */
    /* rhs: (5) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(5))    );
    res = sv[1] ;
    goto end;
    myend97:;
  }
  if(bitSet32_get(mask32,5)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: precedence()((102)) */
    /* allDetEvaluation: det */
    /* rhs: (6) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(6))    );
    res = sv[1] ;
    goto end;
    myend98:;
  }
  if(bitSet32_get(mask32,6)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: precedence()((365)) */
    /* allDetEvaluation: det */
    /* rhs: (7) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(7))    );
    res = sv[1] ;
    goto end;
    myend99:;
  }
  if(bitSet32_get(mask32,7)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: precedence()((155)) */
    /* allDetEvaluation: det */
    /* rhs: (8) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(8))    );
    res = sv[1] ;
    goto end;
    myend100:;
  }
  if(bitSet32_get(mask32,8)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: precedence()((156)) */
    /* allDetEvaluation: det */
    /* rhs: (9) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(9))    );
    res = sv[1] ;
    goto end;
    myend101:;
  }
  if(bitSet32_get(mask32,9)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: precedence()((368)) */
    /* allDetEvaluation: det */
    /* rhs: (10) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(10))    );
    res = sv[1] ;
    goto end;
    myend102:;
  }
  if(bitSet32_get(mask32,10)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: precedence()((369)) */
    /* allDetEvaluation: det */
    /* rhs: (11) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(11))    );
    res = sv[1] ;
    goto end;
    myend103:;
  }
  if(bitSet32_get(mask32,11)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: precedence()(var0) */
    /* allDetEvaluation: det */
    /* rhs: (0) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(0))    );
    res = sv[1] ;
    goto end;
    myend104:;
  }
match_fail:
  GmakeAppl1(res,code_389  ,v1  );
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

Gterm* fun_392(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_391: /* cons_term(,) */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label394:
      v6= GgetArgument(v1,1);
      switch(GgetSymb(v6)) {
      default:
      label395:
        switch(GgetSymb(v2)) {
        default:
        label396:
          bitSet32_set(mask32,1);
        }
      }
    }
    break;
  case code_390: /* nil */
    switch(GgetSymb(v2)) {
    default:
    label392:
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
    /* lhs: @(nil,var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend105:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: @(cons_term(,)(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: cons_term(,)(var0,@(var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_392( v6,v2 );
    GmakeAppl2(sv[1],code_391    ,v5    ,sv[0]    );
    res = sv[1] ;
    goto end;
    myend106:;
  }
match_fail:
  GmakeAppl2(res,code_392  ,v1  ,v2  );
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

Gterm* fun_394(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    case 1: /* 1 */
      switch(GgetSymb(v2)) {
      case code_391: /* cons_term(,) */
        v7= GgetArgument(v2,0);
        switch(GgetSymb(v7)) {
        default:
        label402:
          v8= GgetArgument(v2,1);
          switch(GgetSymb(v8)) {
          default:
          label403:
            bitSet32_set(mask32,0);
            bitSet32_set(mask32,1);
          }
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    default:
      goto label399;
    }
    break;
  default:
  label399:
    switch(GgetSymb(v2)) {
    case code_391: /* cons_term(,) */
      v7= GgetArgument(v2,0);
      switch(GgetSymb(v7)) {
      default:
      label406:
        v8= GgetArgument(v2,1);
        switch(GgetSymb(v8)) {
        default:
        label407:
          bitSet32_set(mask32,1);
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thelem()((1),cons_term(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v7 ;
    goto end;
    myend107:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thelem()(var0,cons_term(,)(var1,var2)) */
    /* allDetEvaluation: det */
    /* where var3 := minus(,)(var0,(1)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_303( v1,sv[2] );
    tmp = sv[0] = sv[1];
    /* rhs: -thelem()(var3,var2) */
    // this=var3        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[2] = fun_394( sv[0],v8 );
    res = sv[2] ;
    goto end;
    myend108:;
  }
match_fail:
  GmakeAppl2(res,code_394  ,v1  ,v2  );
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

Gterm* fun_395(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_391: /* cons_term(,) */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label412:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label413:
        bitSet32_set(mask32,1);
      }
    }
    break;
  case code_390: /* nil */
    bitSet32_set(mask32,0);
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: size_of_term_list()(nil) */
    /* allDetEvaluation: det */
    /* rhs: (0) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(0))    );
    res = sv[1] ;
    goto end;
    myend109:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: size_of_term_list()(cons_term(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: plus(,)((1),size_of_term_list()(var1)) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(1))    );
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_395( v5 );
    sv[2] = fun_302( sv[1],sv[0] );
    res = sv[2] ;
    goto end;
    myend110:;
  }
match_fail:
  GmakeAppl1(res,code_395  ,v1  );
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

Gterm* fun_396(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    case 0: /* 0 */
      switch(GgetSymb(v2)) {
      default:
      label418:
        bitSet32_set(mask32,0);
        bitSet32_set(mask32,1);
      }
      break;
    default:
      goto label416;
    }
    break;
  default:
  label416:
    switch(GgetSymb(v2)) {
    default:
    label420:
      bitSet32_set(mask32,1);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: ccat(,)((0),var0) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_390 ;
    goto end;
    myend111:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: ccat(,)(var0,var1) */
    /* allDetEvaluation: det */
    /* if greater_int(,)(var0,(0)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(0))    );
    sv[0] = fun_312( v1,sv[1] );
    if( sv[0] != con_1 ) {
      goto myend112;
    }
    /* rhs: @(var1,ccat(,)(minus(,)(var0,(1)),var1)) */
    // this=var1        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_303( v1,sv[2] );
    // this=var1        underAC=false        Instantiated=false
    sv[2] = fun_396( sv[1],v2 );
    sv[1] = fun_392( v2,sv[2] );
    res = sv[1] ;
    goto end;
    myend112:;
  }
match_fail:
  GmakeAppl2(res,code_396  ,v1  ,v2  );
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

Gterm* fun_398(Gterm *v1 ) {
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
  label423:
    bitSet32_set(mask32,0);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: identity()(var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v1 ;
    goto end;
    myend113:;
  }
match_fail:
  GmakeAppl1(res,code_398  ,v1  );
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

Gterm* fun_399(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,3);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_354: /* cons_variable(,) */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label429:
      v6= GgetArgument(v1,1);
      switch(GgetSymb(v6)) {
      default:
      label430:
        switch(GgetSymb(v2)) {
        case code_354: /* cons_variable(,) */
          v8= GgetArgument(v2,0);
          switch(GgetSymb(v8)) {
          default:
          label437:
            v9= GgetArgument(v2,1);
            switch(GgetSymb(v9)) {
            default:
            label438:
              bitSet32_set(mask32,2);
            }
          }
          break;
        case code_353: /* nil */
          bitSet32_set(mask32,1);
          break;
        /* matching is not complete: jumpNode is null */
        }
      }
    }
    break;
  case code_353: /* nil */
    switch(GgetSymb(v2)) {
    case code_354: /* cons_variable(,) */
      v8= GgetArgument(v2,0);
      switch(GgetSymb(v8)) {
      default:
      label434:
        v9= GgetArgument(v2,1);
        switch(GgetSymb(v9)) {
        default:
        label435:
          bitSet32_set(mask32,0);
          bitSet32_set(mask32,2);
        }
      }
      break;
    default:
    label427:
      bitSet32_set(mask32,0);
    }
    break;
  default:
  label426:
    switch(GgetSymb(v2)) {
    case code_354: /* cons_variable(,) */
      v8= GgetArgument(v2,0);
      switch(GgetSymb(v8)) {
      default:
      label440:
        v9= GgetArgument(v2,1);
        switch(GgetSymb(v9)) {
        default:
        label441:
          bitSet32_set(mask32,2);
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: @@(nil,var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend114:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: @@(cons_variable(,)(var0,var1),nil) */
    /* allDetEvaluation: det */
    /* rhs: cons_variable(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    GmakeAppl2(sv[0],code_354    ,v5    ,v6    );
    res = sv[0] ;
    goto end;
    myend115:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: @@(var0,cons_variable(,)(var1,var2)) */
    /* allDetEvaluation: det */
    /* rhs: @@(@@(var0,var1),var2) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_400( v1,v8 );
    // this=var2        underAC=false        Instantiated=false
    sv[1] = fun_399( sv[0],v9 );
    res = sv[1] ;
    goto end;
    myend116:;
  }
match_fail:
  GmakeAppl2(res,code_399  ,v1  ,v2  );
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

Gterm* fun_400(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,3);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_353: /* nil */
    switch(GgetSymb(v2)) {
    default:
    label445:
      bitSet32_set(mask32,0);
      bitSet32_set(mask32,1);
      bitSet32_set(mask32,2);
    }
    break;
  default:
  label444:
    switch(GgetSymb(v2)) {
    default:
    label447:
      bitSet32_set(mask32,1);
      bitSet32_set(mask32,2);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: @@(nil,var0) */
    /* allDetEvaluation: det */
    /* rhs: cons_variable(,)(var0,nil) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl2(sv[1],code_354    ,v2    ,con_353    );
    res = sv[1] ;
    goto end;
    myend117:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: @@(var0,var1) */
    /* allDetEvaluation: det */
    /* if (not())(occurs(,)(var1,var0)) */
    // this=var1        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_17( v2,v1 );
    sv[1] = fun_24( sv[0] );
    if( sv[1] != con_1 ) {
      goto myend118;
    }
    /* rhs: @(var0,cons_variable(,)(var1,nil)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    GmakeAppl2(sv[2],code_354    ,v2    ,con_353    );
    sv[0] = fun_355( v1,sv[2] );
    res = sv[0] ;
    goto end;
    myend118:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: @@(var0,var1) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v1 ;
    goto end;
    myend119:;
  }
match_fail:
  GmakeAppl2(res,code_400  ,v1  ,v2  );
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

Gterm* fun_401(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_354: /* cons_variable(,) */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label451:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label452:
        bitSet32_set(mask32,0);
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
    /* lhs: tete()(cons_variable(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v4 ;
    goto end;
    myend120:;
  }
match_fail:
  GmakeAppl1(res,code_401  ,v1  );
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

Gterm* fun_402(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_354: /* cons_variable(,) */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label456:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label457:
        bitSet32_set(mask32,0);
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
    /* lhs: queue()(cons_variable(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var1 */
    // this=var1        underAC=false        Instantiated=false
    res = v5 ;
    goto end;
    myend121:;
  }
match_fail:
  GmakeAppl1(res,code_402  ,v1  );
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

Gterm* fun_403(Gterm *v1 ) {
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
  label460:
    bitSet32_set(mask32,0);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: vtot()(var0) */
    /* allDetEvaluation: det */
    /* rhs: (var0) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[0],code_362    ,v1    );
    res = sv[0] ;
    goto end;
    myend122:;
  }
match_fail:
  GmakeAppl1(res,code_403  ,v1  );
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

Gterm* fun_404(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_362: /*  */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label464:
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
    /* lhs: term_to_variable()((var0)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v4 ;
    goto end;
    myend123:;
  }
match_fail:
  GmakeAppl1(res,code_404  ,v1  );
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

Gterm* fun_405(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,12);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_380: /* i5() */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label486:
      bitSet32_set(mask32,11);
    }
    break;
  case code_379: /* i4() */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label484:
      bitSet32_set(mask32,10);
    }
    break;
  case code_378: /* i3() */
    v6= GgetArgument(v1,0);
    switch(GgetSymb(v6)) {
    default:
    label482:
      bitSet32_set(mask32,9);
    }
    break;
  case code_377: /* i2() */
    v7= GgetArgument(v1,0);
    switch(GgetSymb(v7)) {
    default:
    label480:
      bitSet32_set(mask32,8);
    }
    break;
  case code_376: /* i1() */
    v8= GgetArgument(v1,0);
    switch(GgetSymb(v8)) {
    default:
    label478:
      bitSet32_set(mask32,7);
    }
    break;
  case code_374: /* e5 */
    bitSet32_set(mask32,6);
    break;
  case code_373: /* e4 */
    bitSet32_set(mask32,5);
    break;
  case code_372: /* e3 */
    bitSet32_set(mask32,4);
    break;
  case code_371: /* e2 */
    bitSet32_set(mask32,3);
    break;
  case code_370: /* e1 */
    bitSet32_set(mask32,2);
    break;
  case code_375: /* f(,) */
    v9= GgetArgument(v1,0);
    switch(GgetSymb(v9)) {
    default:
    label470:
      v10= GgetArgument(v1,1);
      switch(GgetSymb(v10)) {
      default:
      label471:
        bitSet32_set(mask32,1);
      }
    }
    break;
  case code_362: /*  */
    v11= GgetArgument(v1,0);
    switch(GgetSymb(v11)) {
    default:
    label468:
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
    /* lhs: list_subterm()((var0)) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_390 ;
    goto end;
    myend124:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_subterm()(f(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: cons_term(,)(var0,cons_term(,)(var1,nil)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    GmakeAppl2(sv[1],code_391    ,v10    ,con_390    );
    GmakeAppl2(sv[0],code_391    ,v9    ,sv[1]    );
    res = sv[0] ;
    goto end;
    myend125:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_subterm()(e1) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_390 ;
    goto end;
    myend126:;
  }
  if(bitSet32_get(mask32,3)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_subterm()(e2) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_390 ;
    goto end;
    myend127:;
  }
  if(bitSet32_get(mask32,4)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_subterm()(e3) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_390 ;
    goto end;
    myend128:;
  }
  if(bitSet32_get(mask32,5)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_subterm()(e4) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_390 ;
    goto end;
    myend129:;
  }
  if(bitSet32_get(mask32,6)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_subterm()(e5) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_390 ;
    goto end;
    myend130:;
  }
  if(bitSet32_get(mask32,7)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_subterm()(i1()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: cons_term(,)(var0,nil) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl2(sv[1],code_391    ,v8    ,con_390    );
    res = sv[1] ;
    goto end;
    myend131:;
  }
  if(bitSet32_get(mask32,8)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_subterm()(i2()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: cons_term(,)(var0,nil) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl2(sv[1],code_391    ,v7    ,con_390    );
    res = sv[1] ;
    goto end;
    myend132:;
  }
  if(bitSet32_get(mask32,9)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_subterm()(i3()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: cons_term(,)(var0,nil) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl2(sv[1],code_391    ,v6    ,con_390    );
    res = sv[1] ;
    goto end;
    myend133:;
  }
  if(bitSet32_get(mask32,10)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_subterm()(i4()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: cons_term(,)(var0,nil) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl2(sv[1],code_391    ,v5    ,con_390    );
    res = sv[1] ;
    goto end;
    myend134:;
  }
  if(bitSet32_get(mask32,11)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_subterm()(i5()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: cons_term(,)(var0,nil) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl2(sv[1],code_391    ,v4    ,con_390    );
    res = sv[1] ;
    goto end;
    myend135:;
  }
match_fail:
  GmakeAppl1(res,code_405  ,v1  );
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

Gterm* fun_406(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,12);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_380: /* i5() */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label508:
      bitSet32_set(mask32,11);
    }
    break;
  case code_379: /* i4() */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label506:
      bitSet32_set(mask32,10);
    }
    break;
  case code_378: /* i3() */
    v6= GgetArgument(v1,0);
    switch(GgetSymb(v6)) {
    default:
    label504:
      bitSet32_set(mask32,9);
    }
    break;
  case code_377: /* i2() */
    v7= GgetArgument(v1,0);
    switch(GgetSymb(v7)) {
    default:
    label502:
      bitSet32_set(mask32,8);
    }
    break;
  case code_376: /* i1() */
    v8= GgetArgument(v1,0);
    switch(GgetSymb(v8)) {
    default:
    label500:
      bitSet32_set(mask32,7);
    }
    break;
  case code_374: /* e5 */
    bitSet32_set(mask32,6);
    break;
  case code_373: /* e4 */
    bitSet32_set(mask32,5);
    break;
  case code_372: /* e3 */
    bitSet32_set(mask32,4);
    break;
  case code_371: /* e2 */
    bitSet32_set(mask32,3);
    break;
  case code_370: /* e1 */
    bitSet32_set(mask32,2);
    break;
  case code_375: /* f(,) */
    v9= GgetArgument(v1,0);
    switch(GgetSymb(v9)) {
    default:
    label492:
      v10= GgetArgument(v1,1);
      switch(GgetSymb(v10)) {
      default:
      label493:
        bitSet32_set(mask32,1);
      }
    }
    break;
  case code_362: /*  */
    v11= GgetArgument(v1,0);
    switch(GgetSymb(v11)) {
    default:
    label490:
      bitSet32_set(mask32,0);
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_variable()((var0)) */
    /* allDetEvaluation: det */
    /* rhs: cons_variable(,)(var0,nil) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl2(sv[1],code_354    ,v11    ,con_353    );
    res = sv[1] ;
    goto end;
    myend136:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_variable()(f(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: @@(@@(list_variable()(var0),list_variable()(var1)),nil) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_406( v9 );
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_406( v10 );
    sv[2] = fun_399( sv[0],sv[1] );
    sv[1] = fun_399( sv[2],con_353 );
    res = sv[1] ;
    goto end;
    myend137:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_variable()(e1) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_353 ;
    goto end;
    myend138:;
  }
  if(bitSet32_get(mask32,3)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_variable()(e2) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_353 ;
    goto end;
    myend139:;
  }
  if(bitSet32_get(mask32,4)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_variable()(e3) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_353 ;
    goto end;
    myend140:;
  }
  if(bitSet32_get(mask32,5)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_variable()(e4) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_353 ;
    goto end;
    myend141:;
  }
  if(bitSet32_get(mask32,6)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_variable()(e5) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_353 ;
    goto end;
    myend142:;
  }
  if(bitSet32_get(mask32,7)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_variable()(i1()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: @@(list_variable()(var0),nil) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_406( v8 );
    sv[2] = fun_399( sv[0],con_353 );
    res = sv[2] ;
    goto end;
    myend143:;
  }
  if(bitSet32_get(mask32,8)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_variable()(i2()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: @@(list_variable()(var0),nil) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_406( v7 );
    sv[2] = fun_399( sv[0],con_353 );
    res = sv[2] ;
    goto end;
    myend144:;
  }
  if(bitSet32_get(mask32,9)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_variable()(i3()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: @@(list_variable()(var0),nil) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_406( v6 );
    sv[2] = fun_399( sv[0],con_353 );
    res = sv[2] ;
    goto end;
    myend145:;
  }
  if(bitSet32_get(mask32,10)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_variable()(i4()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: @@(list_variable()(var0),nil) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_406( v5 );
    sv[2] = fun_399( sv[0],con_353 );
    res = sv[2] ;
    goto end;
    myend146:;
  }
  if(bitSet32_get(mask32,11)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_variable()(i5()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: @@(list_variable()(var0),nil) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_406( v4 );
    sv[2] = fun_399( sv[0],con_353 );
    res = sv[2] ;
    goto end;
    myend147:;
  }
match_fail:
  GmakeAppl1(res,code_406  ,v1  );
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

Gterm* fun_407(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    case 0: /* 0 */
      switch(GgetSymb(v2)) {
      default:
      label513:
        bitSet32_set(mask32,0);
        bitSet32_set(mask32,1);
      }
      break;
    default:
      goto label511;
    }
    break;
  default:
  label511:
    switch(GgetSymb(v2)) {
    default:
    label515:
      bitSet32_set(mask32,1);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: rename_subst(,)((0),var0) */
    /* allDetEvaluation: det */
    /* rhs: ->(var()((0)),(var()(var0))) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(0))    );
    GmakeAppl1(sv[0],code_361    ,sv[1]    );
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[1],code_361    ,v2    );
    GmakeAppl1(sv[2],code_362    ,sv[1]    );
    GmakeAppl2(sv[1],code_343    ,sv[0]    ,sv[2]    );
    res = sv[1] ;
    goto end;
    myend148:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: rename_subst(,)(var0,var1) */
    /* allDetEvaluation: det */
    /* rhs: o(->(var()(var0),(var()(plus(,)(var0,var1)))),rename_subst(,)(minus(,)(var0,(1)),var1)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[0],code_361    ,v1    );
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_302( v1,v2 );
    GmakeAppl1(sv[2],code_361    ,sv[1]    );
    GmakeAppl1(sv[1],code_362    ,sv[2]    );
    GmakeAppl2(sv[2],code_343    ,sv[0]    ,sv[1]    );
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(1))    );
    sv[0] = fun_303( v1,sv[1] );
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_407( sv[0],v2 );
    GmakeAppl2(sv[0],code_344    ,sv[2]    ,sv[1]    );
    res = sv[0] ;
    goto end;
    myend149:;
  }
match_fail:
  GmakeAppl2(res,code_407  ,v1  ,v2  );
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

Gterm* fun_408(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16,*v17,*v18,*v19,*v20,*v21,*v22,*v23,*v24,*v25,*v26,*v27;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,14);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_380: /* i5() */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label551:
      switch(GgetSymb(v2)) {
      default:
      label552:
        bitSet32_set(mask32,13);
      }
    }
    break;
  case code_379: /* i4() */
    v7= GgetArgument(v1,0);
    switch(GgetSymb(v7)) {
    default:
    label548:
      switch(GgetSymb(v2)) {
      default:
      label549:
        bitSet32_set(mask32,12);
      }
    }
    break;
  case code_378: /* i3() */
    v9= GgetArgument(v1,0);
    switch(GgetSymb(v9)) {
    default:
    label545:
      switch(GgetSymb(v2)) {
      default:
      label546:
        bitSet32_set(mask32,11);
      }
    }
    break;
  case code_377: /* i2() */
    v11= GgetArgument(v1,0);
    switch(GgetSymb(v11)) {
    default:
    label542:
      switch(GgetSymb(v2)) {
      default:
      label543:
        bitSet32_set(mask32,10);
      }
    }
    break;
  case code_376: /* i1() */
    v13= GgetArgument(v1,0);
    switch(GgetSymb(v13)) {
    default:
    label539:
      switch(GgetSymb(v2)) {
      default:
      label540:
        bitSet32_set(mask32,9);
      }
    }
    break;
  case code_374: /* e5 */
    switch(GgetSymb(v2)) {
    default:
    label537:
      bitSet32_set(mask32,8);
    }
    break;
  case code_373: /* e4 */
    switch(GgetSymb(v2)) {
    default:
    label535:
      bitSet32_set(mask32,7);
    }
    break;
  case code_372: /* e3 */
    switch(GgetSymb(v2)) {
    default:
    label533:
      bitSet32_set(mask32,6);
    }
    break;
  case code_371: /* e2 */
    switch(GgetSymb(v2)) {
    default:
    label531:
      bitSet32_set(mask32,5);
    }
    break;
  case code_370: /* e1 */
    switch(GgetSymb(v2)) {
    default:
    label529:
      bitSet32_set(mask32,4);
    }
    break;
  case code_375: /* f(,) */
    v20= GgetArgument(v1,0);
    switch(GgetSymb(v20)) {
    default:
    label525:
      v21= GgetArgument(v1,1);
      switch(GgetSymb(v21)) {
      default:
      label526:
        switch(GgetSymb(v2)) {
        default:
        label527:
          bitSet32_set(mask32,3);
        }
      }
    }
    break;
  case code_362: /*  */
    v23= GgetArgument(v1,0);
    switch(GgetSymb(v23)) {
    case code_361: /* var() */
      v24= GgetArgument(v23,0);
      switch(GgetSymb(v24)) {
      default:
      label520:
        switch(GgetSymb(v2)) {
        default:
        label521:
          bitSet32_set(mask32,0);
          bitSet32_set(mask32,1);
          bitSet32_set(mask32,2);
        }
      }
      break;
    default:
    label519:
      switch(GgetSymb(v2)) {
      default:
      label523:
        bitSet32_set(mask32,2);
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
    /* lhs: maxvar(,)((var()(var0)),var1) */
    /* allDetEvaluation: det */
    /* if greatereq_int(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_313( v24,v2 );
    if( sv[0] != con_1 ) {
      goto myend150;
    }
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v24 ;
    goto end;
    myend150:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: maxvar(,)((var()(var0)),var1) */
    /* allDetEvaluation: det */
    /* if greatereq_int(,)(var1,var0) */
    // this=var1        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_313( v2,v24 );
    if( sv[0] != con_1 ) {
      goto myend151;
    }
    /* rhs: var1 */
    // this=var1        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend151:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: maxvar(,)((var0),var1) */
    /* allDetEvaluation: det */
    /* rhs: var1 */
    // this=var1        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend152:;
  }
  if(bitSet32_get(mask32,3)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: maxvar(,)(f(,)(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: maxvar(,)(var0,maxvar(,)(var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_408( v21,v2 );
    sv[1] = fun_408( v20,sv[0] );
    res = sv[1] ;
    goto end;
    myend153:;
  }
  if(bitSet32_get(mask32,4)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: maxvar(,)(e1,var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend154:;
  }
  if(bitSet32_get(mask32,5)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: maxvar(,)(e2,var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend155:;
  }
  if(bitSet32_get(mask32,6)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: maxvar(,)(e3,var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend156:;
  }
  if(bitSet32_get(mask32,7)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: maxvar(,)(e4,var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend157:;
  }
  if(bitSet32_get(mask32,8)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: maxvar(,)(e5,var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend158:;
  }
  if(bitSet32_get(mask32,9)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: maxvar(,)(i1()(var0),var1) */
    /* allDetEvaluation: det */
    /* rhs: maxvar(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_408( v13,v2 );
    res = sv[0] ;
    goto end;
    myend159:;
  }
  if(bitSet32_get(mask32,10)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: maxvar(,)(i2()(var0),var1) */
    /* allDetEvaluation: det */
    /* rhs: maxvar(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_408( v11,v2 );
    res = sv[0] ;
    goto end;
    myend160:;
  }
  if(bitSet32_get(mask32,11)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: maxvar(,)(i3()(var0),var1) */
    /* allDetEvaluation: det */
    /* rhs: maxvar(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_408( v9,v2 );
    res = sv[0] ;
    goto end;
    myend161:;
  }
  if(bitSet32_get(mask32,12)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: maxvar(,)(i4()(var0),var1) */
    /* allDetEvaluation: det */
    /* rhs: maxvar(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_408( v7,v2 );
    res = sv[0] ;
    goto end;
    myend162:;
  }
  if(bitSet32_get(mask32,13)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: maxvar(,)(i5()(var0),var1) */
    /* allDetEvaluation: det */
    /* rhs: maxvar(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_408( v5,v2 );
    res = sv[0] ;
    goto end;
    myend163:;
  }
match_fail:
  GmakeAppl2(res,code_408  ,v1  ,v2  );
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

Gterm* fun_409(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16,*v17,*v18,*v19,*v20,*v21,*v22,*v23,*v24,*v25,*v26,*v27,*v28,*v29,*v30,*v31,*v32,*v33;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,15);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_380: /* i5() */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label595:
      switch(GgetSymb(v2)) {
      default:
      label596:
        bitSet32_set(mask32,14);
      }
    }
    break;
  case code_379: /* i4() */
    v7= GgetArgument(v1,0);
    switch(GgetSymb(v7)) {
    default:
    label592:
      switch(GgetSymb(v2)) {
      default:
      label593:
        bitSet32_set(mask32,13);
      }
    }
    break;
  case code_378: /* i3() */
    v9= GgetArgument(v1,0);
    switch(GgetSymb(v9)) {
    default:
    label589:
      switch(GgetSymb(v2)) {
      default:
      label590:
        bitSet32_set(mask32,12);
      }
    }
    break;
  case code_377: /* i2() */
    v11= GgetArgument(v1,0);
    switch(GgetSymb(v11)) {
    default:
    label586:
      switch(GgetSymb(v2)) {
      default:
      label587:
        bitSet32_set(mask32,11);
      }
    }
    break;
  case code_376: /* i1() */
    v13= GgetArgument(v1,0);
    switch(GgetSymb(v13)) {
    default:
    label583:
      switch(GgetSymb(v2)) {
      default:
      label584:
        bitSet32_set(mask32,10);
      }
    }
    break;
  case code_374: /* e5 */
    switch(GgetSymb(v2)) {
    default:
    label581:
      bitSet32_set(mask32,9);
    }
    break;
  case code_373: /* e4 */
    switch(GgetSymb(v2)) {
    default:
    label579:
      bitSet32_set(mask32,8);
    }
    break;
  case code_372: /* e3 */
    switch(GgetSymb(v2)) {
    default:
    label577:
      bitSet32_set(mask32,7);
    }
    break;
  case code_371: /* e2 */
    switch(GgetSymb(v2)) {
    default:
    label575:
      bitSet32_set(mask32,6);
    }
    break;
  case code_370: /* e1 */
    switch(GgetSymb(v2)) {
    default:
    label573:
      bitSet32_set(mask32,5);
    }
    break;
  case code_375: /* f(,) */
    v20= GgetArgument(v1,0);
    switch(GgetSymb(v20)) {
    default:
    label569:
      v21= GgetArgument(v1,1);
      switch(GgetSymb(v21)) {
      default:
      label570:
        switch(GgetSymb(v2)) {
        default:
        label571:
          bitSet32_set(mask32,4);
        }
      }
    }
    break;
  case code_362: /*  */
    v23= GgetArgument(v1,0);
    switch(GgetSymb(v23)) {
    default:
    label556:
      switch(GgetSymb(v2)) {
      case code_343: /* -> */
        v25= GgetArgument(v2,0);
        switch(GgetSymb(v25)) {
        default:
        label565:
          v26= GgetArgument(v2,1);
          switch(GgetSymb(v26)) {
          case code_362: /*  */
            v27= GgetArgument(v26,0);
            switch(GgetSymb(v27)) {
            default:
            label567:
              bitSet32_set(mask32,2);
            }
            break;
          /* matching is not complete: jumpNode is null */
          }
        }
        break;
      case code_344: /* o */
        v28= GgetArgument(v2,0);
        switch(GgetSymb(v28)) {
        case code_343: /* -> */
          v29= GgetArgument(v28,0);
          switch(GgetSymb(v29)) {
          default:
          label560:
            v30= GgetArgument(v28,1);
            switch(GgetSymb(v30)) {
            case code_362: /*  */
              v31= GgetArgument(v30,0);
              switch(GgetSymb(v31)) {
              default:
              label562:
                v32= GgetArgument(v2,1);
                switch(GgetSymb(v32)) {
                default:
                label563:
                  bitSet32_set(mask32,1);
                  bitSet32_set(mask32,3);
                }
              }
              break;
            /* matching is not complete: jumpNode is null */
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
        break;
      case code_342: /* identity */
        bitSet32_set(mask32,0);
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
    /* lhs: normalize(,)((var0),identity) */
    /* allDetEvaluation: det */
    /* rhs: (var0) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[0],code_362    ,v23    );
    res = sv[0] ;
    goto end;
    myend164:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize(,)((var0),o(->(var1,(var2)),var3)) */
    /* allDetEvaluation: det */
    /* if eq_list[int](,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_18( v23,v29 );
    if( sv[0] != con_1 ) {
      goto myend165;
    }
    /* rhs: (var2) */
    // this=var2        underAC=false        Instantiated=false
    GmakeAppl1(sv[1],code_362    ,v31    );
    res = sv[1] ;
    goto end;
    myend165:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize(,)((var0),->(var1,(var2))) */
    /* allDetEvaluation: det */
    /* if eq_list[int](,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_18( v23,v25 );
    if( sv[0] != con_1 ) {
      goto myend166;
    }
    /* rhs: (var2) */
    // this=var2        underAC=false        Instantiated=false
    GmakeAppl1(sv[1],code_362    ,v27    );
    res = sv[1] ;
    goto end;
    myend166:;
  }
  if(bitSet32_get(mask32,3)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize(,)((var0),o(->(var1,(var2)),var3)) */
    /* allDetEvaluation: det */
    /* if neq_list[int](,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_19( v23,v29 );
    if( sv[0] != con_1 ) {
      goto myend167;
    }
    /* rhs: normalize(,)((var0),var3) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[1],code_362    ,v23    );
    // this=var3        underAC=false        Instantiated=false
    sv[2] = fun_409( sv[1],v32 );
    res = sv[2] ;
    goto end;
    myend167:;
  }
  if(bitSet32_get(mask32,4)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize(,)(f(,)(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: f(,)(normalize(,)(var0,var2),normalize(,)(var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_409( v20,v2 );
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[1] = fun_409( v21,v2 );
    GmakeAppl2(sv[2],code_375    ,sv[0]    ,sv[1]    );
    res = sv[2] ;
    goto end;
    myend168:;
  }
  if(bitSet32_get(mask32,5)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize(,)(e1,var0) */
    /* allDetEvaluation: det */
    /* rhs: e1 */
    res = con_370 ;
    goto end;
    myend169:;
  }
  if(bitSet32_get(mask32,6)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize(,)(e2,var0) */
    /* allDetEvaluation: det */
    /* rhs: e2 */
    res = con_371 ;
    goto end;
    myend170:;
  }
  if(bitSet32_get(mask32,7)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize(,)(e3,var0) */
    /* allDetEvaluation: det */
    /* rhs: e3 */
    res = con_372 ;
    goto end;
    myend171:;
  }
  if(bitSet32_get(mask32,8)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize(,)(e4,var0) */
    /* allDetEvaluation: det */
    /* rhs: e4 */
    res = con_373 ;
    goto end;
    myend172:;
  }
  if(bitSet32_get(mask32,9)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize(,)(e5,var0) */
    /* allDetEvaluation: det */
    /* rhs: e5 */
    res = con_374 ;
    goto end;
    myend173:;
  }
  if(bitSet32_get(mask32,10)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize(,)(i1()(var0),var1) */
    /* allDetEvaluation: det */
    /* rhs: i1()(normalize(,)(var0,var1)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_409( v13,v2 );
    GmakeAppl1(sv[1],code_376    ,sv[0]    );
    res = sv[1] ;
    goto end;
    myend174:;
  }
  if(bitSet32_get(mask32,11)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize(,)(i2()(var0),var1) */
    /* allDetEvaluation: det */
    /* rhs: i2()(normalize(,)(var0,var1)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_409( v11,v2 );
    GmakeAppl1(sv[1],code_377    ,sv[0]    );
    res = sv[1] ;
    goto end;
    myend175:;
  }
  if(bitSet32_get(mask32,12)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize(,)(i3()(var0),var1) */
    /* allDetEvaluation: det */
    /* rhs: i3()(normalize(,)(var0,var1)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_409( v9,v2 );
    GmakeAppl1(sv[1],code_378    ,sv[0]    );
    res = sv[1] ;
    goto end;
    myend176:;
  }
  if(bitSet32_get(mask32,13)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize(,)(i4()(var0),var1) */
    /* allDetEvaluation: det */
    /* rhs: i4()(normalize(,)(var0,var1)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_409( v7,v2 );
    GmakeAppl1(sv[1],code_379    ,sv[0]    );
    res = sv[1] ;
    goto end;
    myend177:;
  }
  if(bitSet32_get(mask32,14)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize(,)(i5()(var0),var1) */
    /* allDetEvaluation: det */
    /* rhs: i5()(normalize(,)(var0,var1)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_409( v5,v2 );
    GmakeAppl1(sv[1],code_380    ,sv[0]    );
    res = sv[1] ;
    goto end;
    myend178:;
  }
match_fail:
  GmakeAppl2(res,code_409  ,v1  ,v2  );
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

Gterm* fun_410(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,12);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_380: /* i5() */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label618:
      bitSet32_set(mask32,11);
    }
    break;
  case code_379: /* i4() */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label616:
      bitSet32_set(mask32,10);
    }
    break;
  case code_378: /* i3() */
    v6= GgetArgument(v1,0);
    switch(GgetSymb(v6)) {
    default:
    label614:
      bitSet32_set(mask32,9);
    }
    break;
  case code_377: /* i2() */
    v7= GgetArgument(v1,0);
    switch(GgetSymb(v7)) {
    default:
    label612:
      bitSet32_set(mask32,8);
    }
    break;
  case code_376: /* i1() */
    v8= GgetArgument(v1,0);
    switch(GgetSymb(v8)) {
    default:
    label610:
      bitSet32_set(mask32,7);
    }
    break;
  case code_374: /* e5 */
    bitSet32_set(mask32,6);
    break;
  case code_373: /* e4 */
    bitSet32_set(mask32,5);
    break;
  case code_372: /* e3 */
    bitSet32_set(mask32,4);
    break;
  case code_371: /* e2 */
    bitSet32_set(mask32,3);
    break;
  case code_370: /* e1 */
    bitSet32_set(mask32,2);
    break;
  case code_375: /* f(,) */
    v9= GgetArgument(v1,0);
    switch(GgetSymb(v9)) {
    default:
    label602:
      v10= GgetArgument(v1,1);
      switch(GgetSymb(v10)) {
      default:
      label603:
        bitSet32_set(mask32,1);
      }
    }
    break;
  case code_362: /*  */
    v11= GgetArgument(v1,0);
    switch(GgetSymb(v11)) {
    default:
    label600:
      bitSet32_set(mask32,0);
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: taille()((var0)) */
    /* allDetEvaluation: det */
    /* rhs: (0) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(0))    );
    res = sv[1] ;
    goto end;
    myend179:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: taille()(f(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: plus(,)(plus(,)(taille()(var0),taille()(var1)),(1)) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_410( v9 );
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_410( v10 );
    sv[2] = fun_302( sv[0],sv[1] );
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(1))    );
    sv[0] = fun_302( sv[2],sv[1] );
    res = sv[0] ;
    goto end;
    myend180:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: taille()(e1) */
    /* allDetEvaluation: det */
    /* rhs: (1) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(1))    );
    res = sv[1] ;
    goto end;
    myend181:;
  }
  if(bitSet32_get(mask32,3)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: taille()(e2) */
    /* allDetEvaluation: det */
    /* rhs: (1) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(1))    );
    res = sv[1] ;
    goto end;
    myend182:;
  }
  if(bitSet32_get(mask32,4)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: taille()(e3) */
    /* allDetEvaluation: det */
    /* rhs: (1) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(1))    );
    res = sv[1] ;
    goto end;
    myend183:;
  }
  if(bitSet32_get(mask32,5)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: taille()(e4) */
    /* allDetEvaluation: det */
    /* rhs: (1) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(1))    );
    res = sv[1] ;
    goto end;
    myend184:;
  }
  if(bitSet32_get(mask32,6)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: taille()(e5) */
    /* allDetEvaluation: det */
    /* rhs: (1) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(1))    );
    res = sv[1] ;
    goto end;
    myend185:;
  }
  if(bitSet32_get(mask32,7)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: taille()(i1()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: plus(,)(taille()(var0),(1)) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_410( v8 );
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_302( sv[0],sv[2] );
    res = sv[1] ;
    goto end;
    myend186:;
  }
  if(bitSet32_get(mask32,8)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: taille()(i2()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: plus(,)(taille()(var0),(1)) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_410( v7 );
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_302( sv[0],sv[2] );
    res = sv[1] ;
    goto end;
    myend187:;
  }
  if(bitSet32_get(mask32,9)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: taille()(i3()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: plus(,)(taille()(var0),(1)) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_410( v6 );
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_302( sv[0],sv[2] );
    res = sv[1] ;
    goto end;
    myend188:;
  }
  if(bitSet32_get(mask32,10)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: taille()(i4()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: plus(,)(taille()(var0),(1)) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_410( v5 );
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_302( sv[0],sv[2] );
    res = sv[1] ;
    goto end;
    myend189:;
  }
  if(bitSet32_get(mask32,11)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: taille()(i5()(var0)) */
    /* allDetEvaluation: det */
    /* rhs: plus(,)(taille()(var0),(1)) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_410( v4 );
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_302( sv[0],sv[2] );
    res = sv[1] ;
    goto end;
    myend190:;
  }
match_fail:
  GmakeAppl1(res,code_410  ,v1  );
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

Gterm* fun_411(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,3);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  default:
  label621:
    switch(GgetSymb(v2)) {
    default:
    label622:
      bitSet32_set(mask32,0);
      bitSet32_set(mask32,1);
      bitSet32_set(mask32,2);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[5];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: >lpo(var0,var1) */
    /* allDetEvaluation: det */
    /* if (and)((not())(isvar()(var0)),(not())(isvar()(var1))) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_365( v1 );
    sv[1] = fun_24( sv[0] );
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_365( v2 );
    sv[2] = fun_24( sv[0] );
    sv[0] = fun_21( sv[1],sv[2] );
    if( sv[0] != con_1 ) {
      goto myend191;
    }
    /* if ==sig(head()(var0),head()(var1)) */
    // this=var0        underAC=false        Instantiated=false
    sv[1] = fun_363( v1 );
    // this=var1        underAC=false        Instantiated=false
    sv[2] = fun_363( v2 );
    sv[3] = fun_388( sv[1],sv[2] );
    if( sv[3] != con_1 ) {
      goto myend191;
    }
    /* where var2 := lpo3(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[2] = fun_417( v1,v2 );
    tmp = sv[1] = sv[2];
    /* if var2 */
    // this=var2        underAC=false        Instantiated=false
    if( sv[1] != con_1 ) {
      goto myend191;
    }
    /* rhs: true */
    res = con_1 ;
    goto end;
    myend191:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[5];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: >lpo(var0,var1) */
    /* allDetEvaluation: det */
    /* if (and)((not())(isvar()(var0)),(not())(isvar()(var1))) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_365( v1 );
    sv[1] = fun_24( sv[0] );
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_365( v2 );
    sv[2] = fun_24( sv[0] );
    sv[0] = fun_21( sv[1],sv[2] );
    if( sv[0] != con_1 ) {
      goto myend192;
    }
    /* if >sig(head()(var0),head()(var1)) */
    // this=var0        underAC=false        Instantiated=false
    sv[1] = fun_363( v1 );
    // this=var1        underAC=false        Instantiated=false
    sv[2] = fun_363( v2 );
    sv[3] = fun_387( sv[1],sv[2] );
    if( sv[3] != con_1 ) {
      goto myend192;
    }
    /* where var2 := lpo2(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[2] = fun_416( v1,v2 );
    tmp = sv[1] = sv[2];
    /* if var2 */
    // this=var2        underAC=false        Instantiated=false
    if( sv[1] != con_1 ) {
      goto myend192;
    }
    /* rhs: true */
    res = con_1 ;
    goto end;
    myend192:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: >lpo(var0,var1) */
    /* allDetEvaluation: det */
    /* rhs: lpo1(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_415( v1,v2 );
    res = sv[0] ;
    goto end;
    myend193:;
  }
match_fail:
  GmakeAppl2(res,code_411  ,v1  ,v2  );
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

Gterm* fun_412(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,4);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_390: /* nil */
    switch(GgetSymb(v2)) {
    default:
    label632:
      bitSet32_set(mask32,2);
    }
    break;
  case code_391: /* cons_term(,) */
    v6= GgetArgument(v1,0);
    switch(GgetSymb(v6)) {
    default:
    label626:
      v7= GgetArgument(v1,1);
      switch(GgetSymb(v7)) {
      default:
      label627:
        switch(GgetSymb(v2)) {
        case code_390: /* nil */
          bitSet32_set(mask32,3);
          break;
        case code_391: /* cons_term(,) */
          v9= GgetArgument(v2,0);
          switch(GgetSymb(v9)) {
          default:
          label629:
            v10= GgetArgument(v2,1);
            switch(GgetSymb(v10)) {
            default:
            label630:
              bitSet32_set(mask32,0);
              bitSet32_set(mask32,1);
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[4];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: >lex(cons_term(,)(var0,var1),cons_term(,)(var2,var3)) */
    /* allDetEvaluation: det */
    /* if eq_list[int](,)(var0,var2) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_18( v6,v9 );
    if( sv[0] != con_1 ) {
      goto myend194;
    }
    /* where var4 := >lex(var1,var3) */
    // this=var1        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    sv[2] = fun_412( v7,v10 );
    tmp = sv[1] = sv[2];
    /* if var4 */
    // this=var4        underAC=false        Instantiated=false
    if( sv[1] != con_1 ) {
      goto myend194;
    }
    /* rhs: true */
    res = con_1 ;
    goto end;
    myend194:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: >lex(cons_term(,)(var0,var1),cons_term(,)(var2,var3)) */
    /* allDetEvaluation: det */
    /* rhs: >lpo(var0,var2) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_411( v6,v9 );
    res = sv[0] ;
    goto end;
    myend195:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: >lex(nil,var0) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend196:;
  }
  if(bitSet32_get(mask32,3)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: >lex(cons_term(,)(var0,var1),nil) */
    /* allDetEvaluation: det */
    /* rhs: true */
    res = con_1 ;
    goto end;
    myend197:;
  }
match_fail:
  GmakeAppl2(res,code_412  ,v1  ,v2  );
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

Gterm* fun_413(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,4);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_390: /* nil */
    switch(GgetSymb(v2)) {
    default:
    label641:
      bitSet32_set(mask32,3);
    }
    break;
  case code_391: /* cons_term(,) */
    v6= GgetArgument(v1,0);
    switch(GgetSymb(v6)) {
    default:
    label637:
      v7= GgetArgument(v1,1);
      switch(GgetSymb(v7)) {
      default:
      label638:
        switch(GgetSymb(v2)) {
        default:
        label639:
          bitSet32_set(mask32,0);
          bitSet32_set(mask32,1);
          bitSet32_set(mask32,2);
        }
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: lpo(,)(cons_term(,)(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* if eq_list[int](,)(var0,var2) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_18( v6,v2 );
    if( sv[0] != con_1 ) {
      goto myend198;
    }
    /* rhs: true */
    res = con_1 ;
    goto end;
    myend198:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: lpo(,)(cons_term(,)(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* if lpo(,)(var1,var2) */
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_413( v7,v2 );
    if( sv[0] != con_1 ) {
      goto myend199;
    }
    /* rhs: true */
    res = con_1 ;
    goto end;
    myend199:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: lpo(,)(cons_term(,)(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: >lpo(var0,var2) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_411( v6,v2 );
    res = sv[0] ;
    goto end;
    myend200:;
  }
  if(bitSet32_get(mask32,3)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: lpo(,)(nil,var0) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend201:;
  }
match_fail:
  GmakeAppl2(res,code_413  ,v1  ,v2  );
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

Gterm* fun_414(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  default:
  label644:
    switch(GgetSymb(v2)) {
    case code_390: /* nil */
      bitSet32_set(mask32,1);
      break;
    case code_391: /* cons_term(,) */
      v6= GgetArgument(v2,0);
      switch(GgetSymb(v6)) {
      default:
      label646:
        v7= GgetArgument(v2,1);
        switch(GgetSymb(v7)) {
        default:
        label647:
          bitSet32_set(mask32,0);
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: lpo(,)(var0,cons_term(,)(var1,var2)) */
    /* allDetEvaluation: det */
    /* rhs: (and)(>lpo(var0,var1),lpo(,)(var0,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_411( v1,v6 );
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[1] = fun_414( v1,v7 );
    sv[2] = fun_21( sv[0],sv[1] );
    res = sv[2] ;
    goto end;
    myend202:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: lpo(,)(var0,nil) */
    /* allDetEvaluation: det */
    /* rhs: true */
    res = con_1 ;
    goto end;
    myend203:;
  }
match_fail:
  GmakeAppl2(res,code_414  ,v1  ,v2  );
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

Gterm* fun_415(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  default:
  label651:
    switch(GgetSymb(v2)) {
    default:
    label652:
      bitSet32_set(mask32,0);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: lpo1(,)(var0,var1) */
    /* allDetEvaluation: det */
    /* rhs: lpo(,)(list_subterm()(var0),var1) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_405( v1 );
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_413( sv[0],v2 );
    res = sv[1] ;
    goto end;
    myend204:;
  }
match_fail:
  GmakeAppl2(res,code_415  ,v1  ,v2  );
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

Gterm* fun_416(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  default:
  label655:
    switch(GgetSymb(v2)) {
    default:
    label656:
      bitSet32_set(mask32,0);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: lpo2(,)(var0,var1) */
    /* allDetEvaluation: det */
    /* rhs: lpo(,)(var0,list_subterm()(var1)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_405( v2 );
    sv[1] = fun_414( v1,sv[0] );
    res = sv[1] ;
    goto end;
    myend205:;
  }
match_fail:
  GmakeAppl2(res,code_416  ,v1  ,v2  );
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

Gterm* fun_417(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  default:
  label659:
    switch(GgetSymb(v2)) {
    default:
    label660:
      bitSet32_set(mask32,0);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: lpo3(,)(var0,var1) */
    /* allDetEvaluation: det */
    /* rhs: (and)(>lex(list_subterm()(var0),list_subterm()(var1)),lpo(,)(var0,list_subterm()(var1))) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_405( v1 );
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_405( v2 );
    sv[2] = fun_412( sv[0],sv[1] );
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_405( v2 );
    sv[1] = fun_414( v1,sv[0] );
    sv[0] = fun_21( sv[2],sv[1] );
    res = sv[0] ;
    goto end;
    myend206:;
  }
match_fail:
  GmakeAppl2(res,code_417  ,v1  ,v2  );
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

Gterm* fun_419(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_418: /* [,] */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label664:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label665:
        bitSet32_set(mask32,0);
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
    /* lhs: 1-th()([,](var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v4 ;
    goto end;
    myend207:;
  }
match_fail:
  GmakeAppl1(res,code_419  ,v1  );
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

Gterm* fun_420(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_418: /* [,] */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label669:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label670:
        bitSet32_set(mask32,0);
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
    /* lhs: 2-th()([,](var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var1 */
    // this=var1        underAC=false        Instantiated=false
    res = v5 ;
    goto end;
    myend208:;
  }
match_fail:
  GmakeAppl1(res,code_420  ,v1  );
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

Gterm* fun_421(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_418: /* [,] */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label674:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label675:
        bitSet32_set(mask32,0);
        bitSet32_set(mask32,1);
      }
    }
    break;
  default:
  label673:
    bitSet32_set(mask32,1);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: ispair()([,](var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: true */
    res = con_1 ;
    goto end;
    myend209:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: ispair()(var0) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend210:;
  }
match_fail:
  GmakeAppl1(res,code_421  ,v1  );
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

Gterm* fun_422(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,3);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_343: /* -> */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label686:
      v6= GgetArgument(v1,1);
      switch(GgetSymb(v6)) {
      default:
      label687:
        switch(GgetSymb(v2)) {
        default:
        label688:
          bitSet32_set(mask32,2);
        }
      }
    }
    break;
  case code_344: /* o */
    v8= GgetArgument(v1,0);
    switch(GgetSymb(v8)) {
    default:
    label682:
      v9= GgetArgument(v1,1);
      switch(GgetSymb(v9)) {
      default:
      label683:
        switch(GgetSymb(v2)) {
        default:
        label684:
          bitSet32_set(mask32,1);
        }
      }
    }
    break;
  case code_342: /* identity */
    switch(GgetSymb(v2)) {
    default:
    label680:
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
    /* lhs: apply(,)(identity,var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend211:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: apply(,)(o(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: apply(,)(var1,apply(,)(var0,var2)) */
    // this=var1        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_422( v8,v2 );
    sv[1] = fun_422( v9,sv[0] );
    res = sv[1] ;
    goto end;
    myend212:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: apply(,)(->(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: replace(,,)((var0),var1,var2) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[0],code_362    ,v5    );
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[1] = fun_16( sv[0],v6,v2 );
    res = sv[1] ;
    goto end;
    myend213:;
  }
match_fail:
  GmakeAppl2(res,code_422  ,v1  ,v2  );
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

Gterm* fun_427(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,3);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_426: /* (&) */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label699:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label700:
        bitSet32_set(mask32,2);
      }
    }
    break;
  case code_425: /*  */
    v6= GgetArgument(v1,0);
    switch(GgetSymb(v6)) {
    case code_423: /* = */
      v7= GgetArgument(v6,0);
      switch(GgetSymb(v7)) {
      case code_362: /*  */
        v8= GgetArgument(v7,0);
        switch(GgetSymb(v8)) {
        default:
        label696:
          v9= GgetArgument(v6,1);
          switch(GgetSymb(v9)) {
          default:
          label697:
            bitSet32_set(mask32,1);
          }
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    break;
  case code_424: /*  */
    v10= GgetArgument(v1,0);
    switch(GgetSymb(v10)) {
    case code_1: /* true */
      bitSet32_set(mask32,0);
      break;
    /* matching is not complete: jumpNode is null */
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: system_to_subst()((true)) */
    /* allDetEvaluation: det */
    /* rhs: identity */
    res = con_342 ;
    goto end;
    myend214:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: system_to_subst()((=((var0),var1))) */
    /* allDetEvaluation: det */
    /* rhs: ->(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    GmakeAppl2(sv[0],code_343    ,v8    ,v9    );
    res = sv[0] ;
    goto end;
    myend215:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: system_to_subst()((&)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: o(system_to_subst()(var0),system_to_subst()(var1)) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_427( v4 );
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_427( v5 );
    GmakeAppl2(sv[2],code_344    ,sv[0]    ,sv[1]    );
    res = sv[2] ;
    goto end;
    myend216:;
  }
match_fail:
  GmakeAppl1(res,code_427  ,v1  );
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

Gterm* fun_430(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_429: /* -> */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label707:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label708:
        bitSet32_set(mask32,1);
      }
    }
    break;
  case code_423: /* = */
    v6= GgetArgument(v1,0);
    switch(GgetSymb(v6)) {
    default:
    label704:
      v7= GgetArgument(v1,1);
      switch(GgetSymb(v7)) {
      default:
      label705:
        bitSet32_set(mask32,0);
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
    /* lhs: left()(=(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v6 ;
    goto end;
    myend217:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: left()(->(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v4 ;
    goto end;
    myend218:;
  }
match_fail:
  GmakeAppl1(res,code_430  ,v1  );
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

Gterm* fun_431(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_429: /* -> */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label715:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label716:
        bitSet32_set(mask32,1);
      }
    }
    break;
  case code_423: /* = */
    v6= GgetArgument(v1,0);
    switch(GgetSymb(v6)) {
    default:
    label712:
      v7= GgetArgument(v1,1);
      switch(GgetSymb(v7)) {
      default:
      label713:
        bitSet32_set(mask32,0);
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
    /* lhs: right()(=(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var1 */
    // this=var1        underAC=false        Instantiated=false
    res = v7 ;
    goto end;
    myend219:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: right()(->(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var1 */
    // this=var1        underAC=false        Instantiated=false
    res = v5 ;
    goto end;
    myend220:;
  }
match_fail:
  GmakeAppl1(res,code_431  ,v1  );
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

Gterm* fun_433(Gterm *v1 ) {
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
  label719:
    bitSet32_set(mask32,0);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[5];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: translate()(var0) */
    /* allDetEvaluation: det */
    /* where var2 := maxvar(,)((var0),(0)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[1],code_425    ,v1    );
    GmakeAppl1(sv[3],code_301    ,(GsetIntegerTag(0))    );
    sv[2] = fun_440( sv[1],sv[3] );
    tmp = sv[0] = sv[2];
    /* where var1 := rename_subst(,)(var2,plus(,)(var2,(1))) */
    // this=var2        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    GmakeAppl1(sv[4],code_301    ,(GsetIntegerTag(1))    );
    sv[3] = fun_302( sv[0],sv[4] );
    sv[4] = fun_407( sv[0],sv[3] );
    tmp = sv[1] = sv[4];
    /* rhs: normalize(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[3] = fun_436( v1,sv[1] );
    res = sv[3] ;
    goto end;
    myend221:;
  }
match_fail:
  GmakeAppl1(res,code_433  ,v1  );
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

Gterm* fun_434(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  default:
  label722:
    switch(GgetSymb(v2)) {
    default:
    label723:
      bitSet32_set(mask32,0);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[4];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: translate(,)(var0,var1) */
    /* allDetEvaluation: det */
    /* where var2 := rename_subst(,)(maxvar(,)((var0),(0)),plus(,)(var1,(1))) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[1],code_425    ,v1    );
    GmakeAppl1(sv[3],code_301    ,(GsetIntegerTag(0))    );
    sv[2] = fun_440( sv[1],sv[3] );
    // this=var1        underAC=false        Instantiated=false
    GmakeAppl1(sv[3],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_302( v2,sv[3] );
    sv[3] = fun_407( sv[2],sv[1] );
    tmp = sv[0] = sv[3];
    /* rhs: normalize(,)(var0,var2) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[1] = fun_436( v1,sv[0] );
    res = sv[1] ;
    goto end;
    myend222:;
  }
match_fail:
  GmakeAppl2(res,code_434  ,v1  ,v2  );
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

Gterm* fun_435(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  default:
  label726:
    switch(GgetSymb(v2)) {
    default:
    label727:
      bitSet32_set(mask32,0);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[5];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize(,)(var0,var1) */
    /* allDetEvaluation: det */
    /* where var3 := list_variable()((var0)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[1],code_425    ,v1    );
    sv[2] = fun_439( sv[1] );
    tmp = sv[0] = sv[2];
    /* where var2 := rename_subst(,)(var3,var1) */
    // this=var3        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[3] = fun_369( sv[0],v2 );
    tmp = sv[1] = sv[3];
    /* rhs: normalize(,)(var0,var2) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[4] = fun_436( v1,sv[1] );
    res = sv[4] ;
    goto end;
    myend223:;
  }
match_fail:
  GmakeAppl2(res,code_435  ,v1  ,v2  );
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

Gterm* fun_436(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_429: /* -> */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label735:
      v6= GgetArgument(v1,1);
      switch(GgetSymb(v6)) {
      default:
      label736:
        switch(GgetSymb(v2)) {
        default:
        label737:
          bitSet32_set(mask32,1);
        }
      }
    }
    break;
  case code_423: /* = */
    v8= GgetArgument(v1,0);
    switch(GgetSymb(v8)) {
    default:
    label731:
      v9= GgetArgument(v1,1);
      switch(GgetSymb(v9)) {
      default:
      label732:
        switch(GgetSymb(v2)) {
        default:
        label733:
          bitSet32_set(mask32,0);
        }
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
    /* lhs: normalize(,)(=(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: =(normalize(,)(var0,var2),normalize(,)(var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_409( v8,v2 );
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[1] = fun_409( v9,v2 );
    GmakeAppl2(sv[2],code_423    ,sv[0]    ,sv[1]    );
    res = sv[2] ;
    goto end;
    myend224:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize(,)(->(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: ->(normalize(,)(var0,var2),normalize(,)(var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_409( v5,v2 );
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[1] = fun_409( v6,v2 );
    GmakeAppl2(sv[2],code_429    ,sv[0]    ,sv[1]    );
    res = sv[2] ;
    goto end;
    myend225:;
  }
match_fail:
  GmakeAppl2(res,code_436  ,v1  ,v2  );
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

Gterm* fun_437(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_424: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label748:
      switch(GgetSymb(v2)) {
      default:
      label749:
        bitSet32_set(mask32,1);
      }
    }
    break;
  case code_426: /* (&) */
    v7= GgetArgument(v1,0);
    switch(GgetSymb(v7)) {
    default:
    label741:
      v8= GgetArgument(v1,1);
      switch(GgetSymb(v8)) {
      case code_425: /*  */
        v9= GgetArgument(v8,0);
        switch(GgetSymb(v9)) {
        case code_423: /* = */
          v10= GgetArgument(v9,0);
          switch(GgetSymb(v10)) {
          default:
          label744:
            v11= GgetArgument(v9,1);
            switch(GgetSymb(v11)) {
            default:
            label745:
              switch(GgetSymb(v2)) {
              default:
              label746:
                bitSet32_set(mask32,0);
              }
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
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
    Gterm *tmp, *sv[4];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize(,)((&)(var0,(=(var1,var2))),var3) */
    /* allDetEvaluation: det */
    /* rhs: (&)(normalize(,)(var0,var3),(=(normalize(,)(var1,var3),normalize(,)(var2,var3)))) */
    // this=var0        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    sv[0] = fun_437( v7,v2 );
    // this=var1        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    sv[1] = fun_409( v10,v2 );
    // this=var2        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    sv[2] = fun_409( v11,v2 );
    GmakeAppl2(sv[3],code_423    ,sv[1]    ,sv[2]    );
    GmakeAppl1(sv[1],code_425    ,sv[3]    );
    GmakeAppl2(sv[2],code_426    ,sv[0]    ,sv[1]    );
    res = sv[2] ;
    goto end;
    myend226:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize(,)((var0),var1) */
    /* allDetEvaluation: det */
    /* rhs: (var0) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[0],code_424    ,v5    );
    res = sv[0] ;
    goto end;
    myend227:;
  }
match_fail:
  GmakeAppl2(res,code_437  ,v1  ,v2  );
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

Gterm* fun_438(Gterm *v1 ) {
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
  label752:
    bitSet32_set(mask32,0);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize()(var0) */
    /* allDetEvaluation: det */
    /* rhs: normalize(,)(var0,(0)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(0))    );
    sv[0] = fun_435( v1,sv[1] );
    res = sv[0] ;
    goto end;
    myend228:;
  }
match_fail:
  GmakeAppl1(res,code_438  ,v1  );
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

Gterm* fun_439(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,5);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_424: /*  */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label771:
      bitSet32_set(mask32,4);
    }
    break;
  case code_426: /* (&) */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label763:
      v6= GgetArgument(v1,1);
      switch(GgetSymb(v6)) {
      case code_424: /*  */
        v7= GgetArgument(v6,0);
        switch(GgetSymb(v7)) {
        default:
        label769:
          bitSet32_set(mask32,3);
        }
        break;
      case code_425: /*  */
        v8= GgetArgument(v6,0);
        switch(GgetSymb(v8)) {
        case code_423: /* = */
          v9= GgetArgument(v8,0);
          switch(GgetSymb(v9)) {
          default:
          label766:
            v10= GgetArgument(v8,1);
            switch(GgetSymb(v10)) {
            default:
            label767:
              bitSet32_set(mask32,2);
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
    break;
  case code_425: /*  */
    v11= GgetArgument(v1,0);
    switch(GgetSymb(v11)) {
    case code_429: /* -> */
      v12= GgetArgument(v11,0);
      switch(GgetSymb(v12)) {
      default:
      label760:
        v13= GgetArgument(v11,1);
        switch(GgetSymb(v13)) {
        default:
        label761:
          bitSet32_set(mask32,1);
        }
      }
      break;
    case code_423: /* = */
      v14= GgetArgument(v11,0);
      switch(GgetSymb(v14)) {
      default:
      label757:
        v15= GgetArgument(v11,1);
        switch(GgetSymb(v15)) {
        default:
        label758:
          bitSet32_set(mask32,0);
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_variable()((=(var0,var1))) */
    /* allDetEvaluation: det */
    /* rhs: @@(list_variable()(var0),list_variable()(var1)) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_406( v14 );
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_406( v15 );
    sv[2] = fun_399( sv[0],sv[1] );
    res = sv[2] ;
    goto end;
    myend229:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_variable()((->(var0,var1))) */
    /* allDetEvaluation: det */
    /* rhs: list_variable()(var0) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_406( v12 );
    res = sv[0] ;
    goto end;
    myend230:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_variable()((&)(var0,(=(var1,var2)))) */
    /* allDetEvaluation: det */
    /* rhs: @@(@@(list_variable()(var0),list_variable()(var1)),list_variable()(var2)) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_439( v5 );
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_406( v9 );
    sv[2] = fun_399( sv[0],sv[1] );
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_406( v10 );
    sv[1] = fun_399( sv[2],sv[0] );
    res = sv[1] ;
    goto end;
    myend231:;
  }
  if(bitSet32_get(mask32,3)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_variable()((&)(var0,(var1))) */
    /* allDetEvaluation: det */
    /* rhs: list_variable()(var0) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_439( v5 );
    res = sv[0] ;
    goto end;
    myend232:;
  }
  if(bitSet32_get(mask32,4)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: list_variable()((var0)) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_353 ;
    goto end;
    myend233:;
  }
match_fail:
  GmakeAppl1(res,code_439  ,v1  );
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

Gterm* fun_440(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16,*v17,*v18,*v19,*v20,*v21,*v22;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,5);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_424: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label794:
      switch(GgetSymb(v2)) {
      default:
      label795:
        bitSet32_set(mask32,4);
      }
    }
    break;
  case code_426: /* (&) */
    v7= GgetArgument(v1,0);
    switch(GgetSymb(v7)) {
    default:
    label784:
      v8= GgetArgument(v1,1);
      switch(GgetSymb(v8)) {
      case code_424: /*  */
        v9= GgetArgument(v8,0);
        switch(GgetSymb(v9)) {
        default:
        label791:
          switch(GgetSymb(v2)) {
          default:
          label792:
            bitSet32_set(mask32,3);
          }
        }
        break;
      case code_425: /*  */
        v11= GgetArgument(v8,0);
        switch(GgetSymb(v11)) {
        case code_423: /* = */
          v12= GgetArgument(v11,0);
          switch(GgetSymb(v12)) {
          default:
          label787:
            v13= GgetArgument(v11,1);
            switch(GgetSymb(v13)) {
            default:
            label788:
              switch(GgetSymb(v2)) {
              default:
              label789:
                bitSet32_set(mask32,2);
              }
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
    break;
  case code_425: /*  */
    v15= GgetArgument(v1,0);
    switch(GgetSymb(v15)) {
    case code_423: /* = */
      v16= GgetArgument(v15,0);
      switch(GgetSymb(v16)) {
      default:
      label780:
        v17= GgetArgument(v15,1);
        switch(GgetSymb(v17)) {
        default:
        label781:
          switch(GgetSymb(v2)) {
          default:
          label782:
            bitSet32_set(mask32,1);
          }
        }
      }
      break;
    case code_429: /* -> */
      v19= GgetArgument(v15,0);
      switch(GgetSymb(v19)) {
      default:
      label776:
        v20= GgetArgument(v15,1);
        switch(GgetSymb(v20)) {
        default:
        label777:
          switch(GgetSymb(v2)) {
          default:
          label778:
            bitSet32_set(mask32,0);
          }
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: maxvar(,)((->(var0,var1)),var2) */
    /* allDetEvaluation: det */
    /* rhs: maxvar(,)(var0,var2) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_408( v19,v2 );
    res = sv[0] ;
    goto end;
    myend234:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: maxvar(,)((=(var0,var1)),var2) */
    /* allDetEvaluation: det */
    /* rhs: maxvar(,)(var0,maxvar(,)(var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_408( v17,v2 );
    sv[1] = fun_408( v16,sv[0] );
    res = sv[1] ;
    goto end;
    myend235:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: maxvar(,)((&)(var0,(=(var1,var2))),var3) */
    /* allDetEvaluation: det */
    /* rhs: maxvar(,)(var0,maxvar(,)(var1,maxvar(,)(var2,var3))) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    sv[0] = fun_408( v13,v2 );
    sv[1] = fun_408( v12,sv[0] );
    sv[0] = fun_440( v7,sv[1] );
    res = sv[0] ;
    goto end;
    myend236:;
  }
  if(bitSet32_get(mask32,3)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: maxvar(,)((&)(var0,(var1)),var2) */
    /* allDetEvaluation: det */
    /* rhs: maxvar(,)(var0,var2) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_440( v7,v2 );
    res = sv[0] ;
    goto end;
    myend237:;
  }
  if(bitSet32_get(mask32,4)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: maxvar(,)((var0),var1) */
    /* allDetEvaluation: det */
    /* rhs: var1 */
    // this=var1        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend238:;
  }
match_fail:
  GmakeAppl2(res,code_440  ,v1  ,v2  );
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

Gterm* fun_441(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,5);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_424: /*  */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label814:
      bitSet32_set(mask32,4);
    }
    break;
  case code_426: /* (&) */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label806:
      v6= GgetArgument(v1,1);
      switch(GgetSymb(v6)) {
      case code_424: /*  */
        v7= GgetArgument(v6,0);
        switch(GgetSymb(v7)) {
        default:
        label812:
          bitSet32_set(mask32,3);
        }
        break;
      case code_425: /*  */
        v8= GgetArgument(v6,0);
        switch(GgetSymb(v8)) {
        case code_423: /* = */
          v9= GgetArgument(v8,0);
          switch(GgetSymb(v9)) {
          default:
          label809:
            v10= GgetArgument(v8,1);
            switch(GgetSymb(v10)) {
            default:
            label810:
              bitSet32_set(mask32,2);
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
    }
    break;
  case code_425: /*  */
    v11= GgetArgument(v1,0);
    switch(GgetSymb(v11)) {
    case code_423: /* = */
      v12= GgetArgument(v11,0);
      switch(GgetSymb(v12)) {
      default:
      label803:
        v13= GgetArgument(v11,1);
        switch(GgetSymb(v13)) {
        default:
        label804:
          bitSet32_set(mask32,1);
        }
      }
      break;
    case code_429: /* -> */
      v14= GgetArgument(v11,0);
      switch(GgetSymb(v14)) {
      default:
      label800:
        v15= GgetArgument(v11,1);
        switch(GgetSymb(v15)) {
        default:
        label801:
          bitSet32_set(mask32,0);
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: taille()((->(var0,var1))) */
    /* allDetEvaluation: det */
    /* rhs: plus(,)(taille()(var0),taille()(var1)) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_410( v14 );
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_410( v15 );
    sv[2] = fun_302( sv[0],sv[1] );
    res = sv[2] ;
    goto end;
    myend239:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: taille()((=(var0,var1))) */
    /* allDetEvaluation: det */
    /* rhs: plus(,)(taille()(var0),taille()(var1)) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_410( v12 );
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_410( v13 );
    sv[2] = fun_302( sv[0],sv[1] );
    res = sv[2] ;
    goto end;
    myend240:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: taille()((&)(var0,(=(var1,var2)))) */
    /* allDetEvaluation: det */
    /* rhs: plus(,)(taille()(var0),taille()((=(var1,var2)))) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_441( v5 );
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    GmakeAppl2(sv[1],code_423    ,v9    ,v10    );
    GmakeAppl1(sv[2],code_425    ,sv[1]    );
    sv[1] = fun_441( sv[2] );
    sv[2] = fun_302( sv[0],sv[1] );
    res = sv[2] ;
    goto end;
    myend241:;
  }
  if(bitSet32_get(mask32,3)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: taille()((&)(var0,(var1))) */
    /* allDetEvaluation: det */
    /* rhs: taille()(var0) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_441( v5 );
    res = sv[0] ;
    goto end;
    myend242:;
  }
  if(bitSet32_get(mask32,4)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: taille()((var0)) */
    /* allDetEvaluation: det */
    /* rhs: (0) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(0))    );
    res = sv[1] ;
    goto end;
    myend243:;
  }
match_fail:
  GmakeAppl1(res,code_441  ,v1  );
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

Gterm* fun_446(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_445: /* cons_equation(,) */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label820:
      v6= GgetArgument(v1,1);
      switch(GgetSymb(v6)) {
      default:
      label821:
        switch(GgetSymb(v2)) {
        default:
        label822:
          bitSet32_set(mask32,1);
        }
      }
    }
    break;
  case code_444: /* nil */
    switch(GgetSymb(v2)) {
    default:
    label818:
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
    /* lhs: @(nil,var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend244:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: @(cons_equation(,)(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: cons_equation(,)(var0,@(var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_446( v6,v2 );
    GmakeAppl2(sv[1],code_445    ,v5    ,sv[0]    );
    res = sv[1] ;
    goto end;
    myend245:;
  }
match_fail:
  GmakeAppl2(res,code_446  ,v1  ,v2  );
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

Gterm* fun_448(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    case 1: /* 1 */
      switch(GgetSymb(v2)) {
      case code_445: /* cons_equation(,) */
        v7= GgetArgument(v2,0);
        switch(GgetSymb(v7)) {
        default:
        label828:
          v8= GgetArgument(v2,1);
          switch(GgetSymb(v8)) {
          default:
          label829:
            bitSet32_set(mask32,0);
            bitSet32_set(mask32,1);
          }
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    default:
      goto label825;
    }
    break;
  default:
  label825:
    switch(GgetSymb(v2)) {
    case code_445: /* cons_equation(,) */
      v7= GgetArgument(v2,0);
      switch(GgetSymb(v7)) {
      default:
      label832:
        v8= GgetArgument(v2,1);
        switch(GgetSymb(v8)) {
        default:
        label833:
          bitSet32_set(mask32,1);
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thelem()((1),cons_equation(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v7 ;
    goto end;
    myend246:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: -thelem()(var0,cons_equation(,)(var1,var2)) */
    /* allDetEvaluation: det */
    /* where var3 := minus(,)(var0,(1)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_303( v1,sv[2] );
    tmp = sv[0] = sv[1];
    /* rhs: -thelem()(var3,var2) */
    // this=var3        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[2] = fun_448( sv[0],v8 );
    res = sv[2] ;
    goto end;
    myend247:;
  }
match_fail:
  GmakeAppl2(res,code_448  ,v1  ,v2  );
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

Gterm* fun_449(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_445: /* cons_equation(,) */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label838:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label839:
        bitSet32_set(mask32,1);
      }
    }
    break;
  case code_444: /* nil */
    bitSet32_set(mask32,0);
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: size_of_equation_list()(nil) */
    /* allDetEvaluation: det */
    /* rhs: (0) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(0))    );
    res = sv[1] ;
    goto end;
    myend248:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: size_of_equation_list()(cons_equation(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: plus(,)((1),size_of_equation_list()(var1)) */
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(1))    );
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_449( v5 );
    sv[2] = fun_302( sv[1],sv[0] );
    res = sv[2] ;
    goto end;
    myend249:;
  }
match_fail:
  GmakeAppl1(res,code_449  ,v1  );
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

Gterm* fun_450(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_301: /*  */
    v5= GgetArgument(v1,0);
    switch(GgetInt(v5)) {
    case 0: /* 0 */
      switch(GgetSymb(v2)) {
      default:
      label844:
        bitSet32_set(mask32,0);
        bitSet32_set(mask32,1);
      }
      break;
    default:
      goto label842;
    }
    break;
  default:
  label842:
    switch(GgetSymb(v2)) {
    default:
    label846:
      bitSet32_set(mask32,1);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: ccat(,)((0),var0) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_444 ;
    goto end;
    myend250:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: ccat(,)(var0,var1) */
    /* allDetEvaluation: det */
    /* if greater_int(,)(var0,(0)) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[1],code_301    ,(GsetIntegerTag(0))    );
    sv[0] = fun_312( v1,sv[1] );
    if( sv[0] != con_1 ) {
      goto myend251;
    }
    /* rhs: @(var1,ccat(,)(minus(,)(var0,(1)),var1)) */
    // this=var1        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[2],code_301    ,(GsetIntegerTag(1))    );
    sv[1] = fun_303( v1,sv[2] );
    // this=var1        underAC=false        Instantiated=false
    sv[2] = fun_450( sv[1],v2 );
    sv[1] = fun_446( v2,sv[2] );
    res = sv[1] ;
    goto end;
    myend251:;
  }
match_fail:
  GmakeAppl2(res,code_450  ,v1  ,v2  );
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

Gterm* fun_452( ) {
  Gterm *v1,*v2;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  bitSet32_set(mask32,0);
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[13];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: System */
    /* allDetEvaluation: det */
    /* rhs: cons_equation(,)(=(f(,)(f(,)((x),(y)),(z)),f(,)((x),f(,)((y),(z)))),cons_equation(,)(=(f(,)(e1,(x)),(x)),cons_equation(,)(=(f(,)(e2,(x)),(x)),cons_equation(,)(=(f(,)(e3,(x)),(x)),cons_equation(,)(=(f(,)(e4,(x)),(x)),cons_equation(,)(=(f(,)(e5,(x)),(x)),cons_equation(,)(=(f(,)((x),i1()((x))),e1),cons_equation(,)(=(f(,)((x),i2()((x))),e2),cons_equation(,)(=(f(,)((x),i3()((x))),e3),cons_equation(,)(=(f(,)((x),i4()((x))),e4),cons_equation(,)(=(f(,)((x),i5()((x))),e5),nil))))))))))) */
    GmakeAppl1(sv[1],code_362    ,con_381    );
    GmakeAppl1(sv[2],code_362    ,con_382    );
    GmakeAppl2(sv[0],code_375    ,sv[1]    ,sv[2]    );
    GmakeAppl1(sv[2],code_362    ,con_383    );
    GmakeAppl2(sv[1],code_375    ,sv[0]    ,sv[2]    );
    GmakeAppl1(sv[2],code_362    ,con_381    );
    GmakeAppl1(sv[3],code_362    ,con_382    );
    GmakeAppl1(sv[4],code_362    ,con_383    );
    GmakeAppl2(sv[0],code_375    ,sv[3]    ,sv[4]    );
    GmakeAppl2(sv[3],code_375    ,sv[2]    ,sv[0]    );
    GmakeAppl2(sv[0],code_423    ,sv[1]    ,sv[3]    );
    GmakeAppl1(sv[3],code_362    ,con_381    );
    GmakeAppl2(sv[2],code_375    ,con_370    ,sv[3]    );
    GmakeAppl1(sv[3],code_362    ,con_381    );
    GmakeAppl2(sv[1],code_423    ,sv[2]    ,sv[3]    );
    GmakeAppl1(sv[4],code_362    ,con_381    );
    GmakeAppl2(sv[3],code_375    ,con_371    ,sv[4]    );
    GmakeAppl1(sv[4],code_362    ,con_381    );
    GmakeAppl2(sv[2],code_423    ,sv[3]    ,sv[4]    );
    GmakeAppl1(sv[5],code_362    ,con_381    );
    GmakeAppl2(sv[4],code_375    ,con_372    ,sv[5]    );
    GmakeAppl1(sv[5],code_362    ,con_381    );
    GmakeAppl2(sv[3],code_423    ,sv[4]    ,sv[5]    );
    GmakeAppl1(sv[6],code_362    ,con_381    );
    GmakeAppl2(sv[5],code_375    ,con_373    ,sv[6]    );
    GmakeAppl1(sv[6],code_362    ,con_381    );
    GmakeAppl2(sv[4],code_423    ,sv[5]    ,sv[6]    );
    GmakeAppl1(sv[7],code_362    ,con_381    );
    GmakeAppl2(sv[6],code_375    ,con_374    ,sv[7]    );
    GmakeAppl1(sv[7],code_362    ,con_381    );
    GmakeAppl2(sv[5],code_423    ,sv[6]    ,sv[7]    );
    GmakeAppl1(sv[7],code_362    ,con_381    );
    GmakeAppl1(sv[8],code_362    ,con_381    );
    GmakeAppl1(sv[6],code_376    ,sv[8]    );
    GmakeAppl2(sv[8],code_375    ,sv[7]    ,sv[6]    );
    GmakeAppl2(sv[7],code_423    ,sv[8]    ,con_370    );
    GmakeAppl1(sv[8],code_362    ,con_381    );
    GmakeAppl1(sv[9],code_362    ,con_381    );
    GmakeAppl1(sv[6],code_377    ,sv[9]    );
    GmakeAppl2(sv[9],code_375    ,sv[8]    ,sv[6]    );
    GmakeAppl2(sv[8],code_423    ,sv[9]    ,con_371    );
    GmakeAppl1(sv[9],code_362    ,con_381    );
    GmakeAppl1(sv[10],code_362    ,con_381    );
    GmakeAppl1(sv[6],code_378    ,sv[10]    );
    GmakeAppl2(sv[10],code_375    ,sv[9]    ,sv[6]    );
    GmakeAppl2(sv[9],code_423    ,sv[10]    ,con_372    );
    GmakeAppl1(sv[10],code_362    ,con_381    );
    GmakeAppl1(sv[11],code_362    ,con_381    );
    GmakeAppl1(sv[6],code_379    ,sv[11]    );
    GmakeAppl2(sv[11],code_375    ,sv[10]    ,sv[6]    );
    GmakeAppl2(sv[10],code_423    ,sv[11]    ,con_373    );
    GmakeAppl1(sv[11],code_362    ,con_381    );
    GmakeAppl1(sv[12],code_362    ,con_381    );
    GmakeAppl1(sv[6],code_380    ,sv[12]    );
    GmakeAppl2(sv[12],code_375    ,sv[11]    ,sv[6]    );
    GmakeAppl2(sv[11],code_423    ,sv[12]    ,con_374    );
    GmakeAppl2(sv[12],code_445    ,sv[11]    ,con_444    );
    GmakeAppl2(sv[6],code_445    ,sv[10]    ,sv[12]    );
    GmakeAppl2(sv[10],code_445    ,sv[9]    ,sv[6]    );
    GmakeAppl2(sv[6],code_445    ,sv[8]    ,sv[10]    );
    GmakeAppl2(sv[8],code_445    ,sv[7]    ,sv[6]    );
    GmakeAppl2(sv[6],code_445    ,sv[5]    ,sv[8]    );
    GmakeAppl2(sv[5],code_445    ,sv[4]    ,sv[6]    );
    GmakeAppl2(sv[4],code_445    ,sv[3]    ,sv[5]    );
    GmakeAppl2(sv[3],code_445    ,sv[2]    ,sv[4]    );
    GmakeAppl2(sv[2],code_445    ,sv[1]    ,sv[3]    );
    GmakeAppl2(sv[1],code_445    ,sv[0]    ,sv[2]    );
    res = sv[1] ;
    goto end;
    myend252:;
  }
match_fail:
  GmakeAppl0(res,code_452);
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

Gterm* fun_453(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,3);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_343: /* -> */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label858:
      v6= GgetArgument(v1,1);
      switch(GgetSymb(v6)) {
      default:
      label859:
        switch(GgetSymb(v2)) {
        default:
        label860:
          bitSet32_set(mask32,2);
        }
      }
    }
    break;
  case code_344: /* o */
    v8= GgetArgument(v1,0);
    switch(GgetSymb(v8)) {
    default:
    label854:
      v9= GgetArgument(v1,1);
      switch(GgetSymb(v9)) {
      default:
      label855:
        switch(GgetSymb(v2)) {
        default:
        label856:
          bitSet32_set(mask32,1);
        }
      }
    }
    break;
  case code_342: /* identity */
    switch(GgetSymb(v2)) {
    default:
    label852:
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
    /* lhs: apply(,)(identity,var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend253:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: apply(,)(o(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: apply(,)(var1,apply(,)(var0,var2)) */
    // this=var1        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_453( v8,v2 );
    sv[1] = fun_453( v9,sv[0] );
    res = sv[1] ;
    goto end;
    myend254:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: apply(,)(->(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: replace(,,)((var0),var1,var2) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[0],code_362    ,v5    );
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[1] = fun_16( sv[0],v6,v2 );
    res = sv[1] ;
    goto end;
    myend255:;
  }
match_fail:
  GmakeAppl2(res,code_453  ,v1  ,v2  );
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

Gterm* fun_454(Gterm *v1,Gterm *v2,Gterm *v3 ) {
  Gterm *v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16,*v17,*v18,*v19;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,3);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_425: /*  */
    v6= GgetArgument(v1,0);
    switch(GgetSymb(v6)) {
    case code_423: /* = */
      v7= GgetArgument(v6,0);
      switch(GgetSymb(v7)) {
      case code_362: /*  */
        v8= GgetArgument(v7,0);
        switch(GgetSymb(v8)) {
        default:
        label875:
          v9= GgetArgument(v6,1);
          switch(GgetSymb(v9)) {
          default:
          label876:
            switch(GgetSymb(v2)) {
            default:
            label877:
              switch(GgetSymb(v3)) {
              default:
              label878:
                bitSet32_set(mask32,2);
              }
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
  case code_426: /* (&) */
    v12= GgetArgument(v1,0);
    switch(GgetSymb(v12)) {
    default:
    label868:
      v13= GgetArgument(v1,1);
      switch(GgetSymb(v13)) {
      default:
      label869:
        switch(GgetSymb(v2)) {
        default:
        label870:
          switch(GgetSymb(v3)) {
          default:
          label871:
            bitSet32_set(mask32,1);
          }
        }
      }
    }
    break;
  case code_424: /*  */
    v16= GgetArgument(v1,0);
    switch(GgetSymb(v16)) {
    case code_1: /* true */
      switch(GgetSymb(v2)) {
      default:
      label865:
        switch(GgetSymb(v3)) {
        default:
        label866:
          bitSet32_set(mask32,0);
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: mergingClash(,,)((true),var0,var1) */
    /* allDetEvaluation: det */
    /* rhs: false */
    res = con_0 ;
    goto end;
    myend256:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: mergingClash(,,)((&)(var0,var1),var2,var3) */
    /* allDetEvaluation: det */
    /* rhs: (or)(mergingClash(,,)(var0,var2,var3),mergingClash(,,)(var1,var2,var3)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    sv[0] = fun_454( v12,v2,v3 );
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    sv[1] = fun_454( v13,v2,v3 );
    sv[2] = fun_22( sv[0],sv[1] );
    res = sv[2] ;
    goto end;
    myend257:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: mergingClash(,,)((=((var0),var1)),var2,var3) */
    /* allDetEvaluation: det */
    /* rhs: (and)(eq_list[int](,)(var0,var2),neq_list[int](,)(var1,var3)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_18( v8,v2 );
    // this=var1        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    sv[1] = fun_19( v9,v3 );
    sv[2] = fun_21( sv[0],sv[1] );
    res = sv[2] ;
    goto end;
    myend258:;
  }
match_fail:
  GmakeAppl3(res,code_454  ,v1  ,v2  ,v3  );
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

Gterm* fun_460(Gterm *v1 ) {
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
  label881:
    bitSet32_set(mask32,0);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: identity()(var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v1 ;
    goto end;
    myend259:;
  }
match_fail:
  GmakeAppl1(res,code_460  ,v1  );
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

Gterm* fun_463(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_461: /* ***** */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label885:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label886:
        v6= GgetArgument(v1,2);
        switch(GgetSymb(v6)) {
        default:
        label887:
          v7= GgetArgument(v1,3);
          switch(GgetSymb(v7)) {
          default:
          label888:
            v8= GgetArgument(v1,4);
            switch(GgetSymb(v8)) {
            default:
            label889:
              v9= GgetArgument(v1,5);
              switch(GgetSymb(v9)) {
              default:
              label890:
                bitSet32_set(mask32,0);
              }
            }
          }
        }
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
    /* lhs: CS_to_A()(*****(var0,var1,var2,var3,var4,var5)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v4 ;
    goto end;
    myend260:;
  }
match_fail:
  GmakeAppl1(res,code_463  ,v1  );
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

Gterm* fun_464(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_461: /* ***** */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label894:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label895:
        v6= GgetArgument(v1,2);
        switch(GgetSymb(v6)) {
        default:
        label896:
          v7= GgetArgument(v1,3);
          switch(GgetSymb(v7)) {
          default:
          label897:
            v8= GgetArgument(v1,4);
            switch(GgetSymb(v8)) {
            default:
            label898:
              v9= GgetArgument(v1,5);
              switch(GgetSymb(v9)) {
              default:
              label899:
                bitSet32_set(mask32,0);
              }
            }
          }
        }
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
    /* lhs: CS_to_N()(*****(var0,var1,var2,var3,var4,var5)) */
    /* allDetEvaluation: det */
    /* rhs: var1 */
    // this=var1        underAC=false        Instantiated=false
    res = v5 ;
    goto end;
    myend261:;
  }
match_fail:
  GmakeAppl1(res,code_464  ,v1  );
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

Gterm* fun_465(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_461: /* ***** */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label903:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label904:
        v6= GgetArgument(v1,2);
        switch(GgetSymb(v6)) {
        default:
        label905:
          v7= GgetArgument(v1,3);
          switch(GgetSymb(v7)) {
          default:
          label906:
            v8= GgetArgument(v1,4);
            switch(GgetSymb(v8)) {
            default:
            label907:
              v9= GgetArgument(v1,5);
              switch(GgetSymb(v9)) {
              default:
              label908:
                bitSet32_set(mask32,0);
              }
            }
          }
        }
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
    /* lhs: CS_to_C()(*****(var0,var1,var2,var3,var4,var5)) */
    /* allDetEvaluation: det */
    /* rhs: var2 */
    // this=var2        underAC=false        Instantiated=false
    res = v6 ;
    goto end;
    myend262:;
  }
match_fail:
  GmakeAppl1(res,code_465  ,v1  );
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

Gterm* fun_466(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_461: /* ***** */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label912:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label913:
        v6= GgetArgument(v1,2);
        switch(GgetSymb(v6)) {
        default:
        label914:
          v7= GgetArgument(v1,3);
          switch(GgetSymb(v7)) {
          default:
          label915:
            v8= GgetArgument(v1,4);
            switch(GgetSymb(v8)) {
            default:
            label916:
              v9= GgetArgument(v1,5);
              switch(GgetSymb(v9)) {
              default:
              label917:
                bitSet32_set(mask32,0);
              }
            }
          }
        }
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
    /* lhs: CS_to_T()(*****(var0,var1,var2,var3,var4,var5)) */
    /* allDetEvaluation: det */
    /* rhs: var3 */
    // this=var3        underAC=false        Instantiated=false
    res = v7 ;
    goto end;
    myend263:;
  }
match_fail:
  GmakeAppl1(res,code_466  ,v1  );
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

Gterm* fun_467(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_461: /* ***** */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label921:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label922:
        v6= GgetArgument(v1,2);
        switch(GgetSymb(v6)) {
        default:
        label923:
          v7= GgetArgument(v1,3);
          switch(GgetSymb(v7)) {
          default:
          label924:
            v8= GgetArgument(v1,4);
            switch(GgetSymb(v8)) {
            default:
            label925:
              v9= GgetArgument(v1,5);
              switch(GgetSymb(v9)) {
              default:
              label926:
                bitSet32_set(mask32,0);
              }
            }
          }
        }
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
    /* lhs: CS_to_S()(*****(var0,var1,var2,var3,var4,var5)) */
    /* allDetEvaluation: det */
    /* rhs: var4 */
    // this=var4        underAC=false        Instantiated=false
    res = v8 ;
    goto end;
    myend264:;
  }
match_fail:
  GmakeAppl1(res,code_467  ,v1  );
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

Gterm* fun_468(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_461: /* ***** */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label930:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label931:
        v6= GgetArgument(v1,2);
        switch(GgetSymb(v6)) {
        default:
        label932:
          v7= GgetArgument(v1,3);
          switch(GgetSymb(v7)) {
          default:
          label933:
            v8= GgetArgument(v1,4);
            switch(GgetSymb(v8)) {
            default:
            label934:
              v9= GgetArgument(v1,5);
              switch(GgetSymb(v9)) {
              default:
              label935:
                bitSet32_set(mask32,0);
              }
            }
          }
        }
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
    /* lhs: CS_to_E()(*****(var0,var1,var2,var3,var4,var5)) */
    /* allDetEvaluation: det */
    /* rhs: var5 */
    // this=var5        underAC=false        Instantiated=false
    res = v9 ;
    goto end;
    myend265:;
  }
match_fail:
  GmakeAppl1(res,code_468  ,v1  );
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

Gterm* fun_469(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_462: /* ||| */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label939:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label940:
        v6= GgetArgument(v1,2);
        switch(GgetSymb(v6)) {
        default:
        label941:
          v7= GgetArgument(v1,3);
          switch(GgetSymb(v7)) {
          default:
          label942:
            bitSet32_set(mask32,0);
          }
        }
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
    /* lhs: ICS_to_1()(|||(var0,var1,var2,var3)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v4 ;
    goto end;
    myend266:;
  }
match_fail:
  GmakeAppl1(res,code_469  ,v1  );
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

Gterm* fun_470(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_462: /* ||| */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label946:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label947:
        v6= GgetArgument(v1,2);
        switch(GgetSymb(v6)) {
        default:
        label948:
          v7= GgetArgument(v1,3);
          switch(GgetSymb(v7)) {
          default:
          label949:
            bitSet32_set(mask32,0);
          }
        }
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
    /* lhs: ICS_to_2()(|||(var0,var1,var2,var3)) */
    /* allDetEvaluation: det */
    /* rhs: var1 */
    // this=var1        underAC=false        Instantiated=false
    res = v5 ;
    goto end;
    myend267:;
  }
match_fail:
  GmakeAppl1(res,code_470  ,v1  );
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

Gterm* fun_471(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_462: /* ||| */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label953:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label954:
        v6= GgetArgument(v1,2);
        switch(GgetSymb(v6)) {
        default:
        label955:
          v7= GgetArgument(v1,3);
          switch(GgetSymb(v7)) {
          default:
          label956:
            bitSet32_set(mask32,0);
          }
        }
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
    /* lhs: ICS_to_3()(|||(var0,var1,var2,var3)) */
    /* allDetEvaluation: det */
    /* rhs: var2 */
    // this=var2        underAC=false        Instantiated=false
    res = v6 ;
    goto end;
    myend268:;
  }
match_fail:
  GmakeAppl1(res,code_471  ,v1  );
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

Gterm* fun_472(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_462: /* ||| */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label960:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label961:
        v6= GgetArgument(v1,2);
        switch(GgetSymb(v6)) {
        default:
        label962:
          v7= GgetArgument(v1,3);
          switch(GgetSymb(v7)) {
          default:
          label963:
            bitSet32_set(mask32,0);
          }
        }
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
    /* lhs: ICS_to_4()(|||(var0,var1,var2,var3)) */
    /* allDetEvaluation: det */
    /* rhs: var3 */
    // this=var3        underAC=false        Instantiated=false
    res = v7 ;
    goto end;
    myend269:;
  }
match_fail:
  GmakeAppl1(res,code_472  ,v1  );
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

Gterm* fun_473(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,3);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_343: /* -> */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label973:
      v6= GgetArgument(v1,1);
      switch(GgetSymb(v6)) {
      default:
      label974:
        switch(GgetSymb(v2)) {
        default:
        label975:
          bitSet32_set(mask32,2);
        }
      }
    }
    break;
  case code_344: /* o */
    v8= GgetArgument(v1,0);
    switch(GgetSymb(v8)) {
    default:
    label969:
      v9= GgetArgument(v1,1);
      switch(GgetSymb(v9)) {
      default:
      label970:
        switch(GgetSymb(v2)) {
        default:
        label971:
          bitSet32_set(mask32,1);
        }
      }
    }
    break;
  case code_342: /* identity */
    switch(GgetSymb(v2)) {
    default:
    label967:
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
    /* lhs: apply(,)(identity,var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend270:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: apply(,)(o(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: apply(,)(var1,apply(,)(var0,var2)) */
    // this=var1        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_473( v8,v2 );
    sv[1] = fun_473( v9,sv[0] );
    res = sv[1] ;
    goto end;
    myend271:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: apply(,)(->(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: replace(,,)((var0),var1,var2) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[0],code_362    ,v5    );
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[1] = fun_16( sv[0],v6,v2 );
    res = sv[1] ;
    goto end;
    myend272:;
  }
match_fail:
  GmakeAppl2(res,code_473  ,v1  ,v2  );
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

Gterm* fun_474(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,3);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_445: /* cons_equation(,) */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label981:
      v6= GgetArgument(v1,1);
      switch(GgetSymb(v6)) {
      default:
      label982:
        switch(GgetSymb(v2)) {
        case code_445: /* cons_equation(,) */
          v8= GgetArgument(v2,0);
          switch(GgetSymb(v8)) {
          default:
          label989:
            v9= GgetArgument(v2,1);
            switch(GgetSymb(v9)) {
            default:
            label990:
              bitSet32_set(mask32,2);
            }
          }
          break;
        case code_444: /* nil */
          bitSet32_set(mask32,1);
          break;
        /* matching is not complete: jumpNode is null */
        }
      }
    }
    break;
  case code_444: /* nil */
    switch(GgetSymb(v2)) {
    case code_445: /* cons_equation(,) */
      v8= GgetArgument(v2,0);
      switch(GgetSymb(v8)) {
      default:
      label986:
        v9= GgetArgument(v2,1);
        switch(GgetSymb(v9)) {
        default:
        label987:
          bitSet32_set(mask32,0);
          bitSet32_set(mask32,2);
        }
      }
      break;
    default:
    label979:
      bitSet32_set(mask32,0);
    }
    break;
  default:
  label978:
    switch(GgetSymb(v2)) {
    case code_445: /* cons_equation(,) */
      v8= GgetArgument(v2,0);
      switch(GgetSymb(v8)) {
      default:
      label992:
        v9= GgetArgument(v2,1);
        switch(GgetSymb(v9)) {
        default:
        label993:
          bitSet32_set(mask32,2);
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: @@(nil,var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend273:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: @@(cons_equation(,)(var0,var1),nil) */
    /* allDetEvaluation: det */
    /* rhs: cons_equation(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    GmakeAppl2(sv[0],code_445    ,v5    ,v6    );
    res = sv[0] ;
    goto end;
    myend274:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: @@(var0,cons_equation(,)(var1,var2)) */
    /* allDetEvaluation: det */
    /* rhs: @@(@@(var0,var1),var2) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_475( v1,v8 );
    // this=var2        underAC=false        Instantiated=false
    sv[1] = fun_474( sv[0],v9 );
    res = sv[1] ;
    goto end;
    myend275:;
  }
match_fail:
  GmakeAppl2(res,code_474  ,v1  ,v2  );
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

Gterm* fun_475(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,3);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_444: /* nil */
    switch(GgetSymb(v2)) {
    default:
    label997:
      bitSet32_set(mask32,0);
      bitSet32_set(mask32,1);
      bitSet32_set(mask32,2);
    }
    break;
  default:
  label996:
    switch(GgetSymb(v2)) {
    default:
    label999:
      bitSet32_set(mask32,1);
      bitSet32_set(mask32,2);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: @@(nil,var0) */
    /* allDetEvaluation: det */
    /* rhs: cons_equation(,)(var0,nil) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl2(sv[1],code_445    ,v2    ,con_444    );
    res = sv[1] ;
    goto end;
    myend276:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: @@(var0,var1) */
    /* allDetEvaluation: det */
    /* if (not())(occurs(,)(var1,var0)) */
    // this=var1        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_17( v2,v1 );
    sv[1] = fun_24( sv[0] );
    if( sv[1] != con_1 ) {
      goto myend277;
    }
    /* rhs: @(var0,cons_equation(,)(var1,nil)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    GmakeAppl2(sv[2],code_445    ,v2    ,con_444    );
    sv[0] = fun_446( v1,sv[2] );
    res = sv[0] ;
    goto end;
    myend277:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: @@(var0,var1) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v1 ;
    goto end;
    myend278:;
  }
match_fail:
  GmakeAppl2(res,code_475  ,v1  ,v2  );
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

Gterm* fun_476(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_445: /* cons_equation(,) */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label1003:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label1004:
        bitSet32_set(mask32,0);
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
    /* lhs: tete()(cons_equation(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v4 ;
    goto end;
    myend279:;
  }
match_fail:
  GmakeAppl1(res,code_476  ,v1  );
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

Gterm* fun_477(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_445: /* cons_equation(,) */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label1008:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label1009:
        bitSet32_set(mask32,0);
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
    /* lhs: queue()(cons_equation(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var1 */
    // this=var1        underAC=false        Instantiated=false
    res = v5 ;
    goto end;
    myend280:;
  }
match_fail:
  GmakeAppl1(res,code_477  ,v1  );
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

Gterm* fun_480(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,4);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_444: /* nil */
    switch(GgetSymb(v2)) {
    case code_445: /* cons_equation(,) */
      v6= GgetArgument(v2,0);
      switch(GgetSymb(v6)) {
      default:
      label1014:
        v7= GgetArgument(v2,1);
        switch(GgetSymb(v7)) {
        default:
        label1015:
          bitSet32_set(mask32,0);
          bitSet32_set(mask32,2);
          bitSet32_set(mask32,3);
        }
      }
      break;
    case code_444: /* nil */
      bitSet32_set(mask32,1);
      break;
    /* matching is not complete: jumpNode is null */
    }
    break;
  default:
  label1012:
    switch(GgetSymb(v2)) {
    case code_445: /* cons_equation(,) */
      v6= GgetArgument(v2,0);
      switch(GgetSymb(v6)) {
      default:
      label1020:
        v7= GgetArgument(v2,1);
        switch(GgetSymb(v7)) {
        default:
        label1021:
          bitSet32_set(mask32,2);
          bitSet32_set(mask32,3);
        }
      }
      break;
    case code_444: /* nil */
      bitSet32_set(mask32,1);
      break;
    /* matching is not complete: jumpNode is null */
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: append_taille(,)(nil,cons_equation(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: append_taille(,)(cons_equation(,)(var0,nil),var1) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl2(sv[1],code_445    ,v6    ,con_444    );
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_480( sv[1],v7 );
    res = sv[0] ;
    goto end;
    myend281:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: append_taille(,)(var0,nil) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v1 ;
    goto end;
    myend282:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: append_taille(,)(var0,cons_equation(,)(var1,var2)) */
    /* allDetEvaluation: det */
    /* if (not())(occurs(,)(var1,var0)) */
    // this=var1        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_17( v6,v1 );
    sv[1] = fun_24( sv[0] );
    if( sv[1] != con_1 ) {
      goto myend283;
    }
    /* rhs: append_taille(,)(append_taille(,)(var0,var1),var2) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_481( v1,v6 );
    // this=var2        underAC=false        Instantiated=false
    sv[2] = fun_480( sv[0],v7 );
    res = sv[2] ;
    goto end;
    myend283:;
  }
  if(bitSet32_get(mask32,3)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: append_taille(,)(var0,cons_equation(,)(var1,var2)) */
    /* allDetEvaluation: det */
    /* if occurs(,)(var1,var0) */
    // this=var1        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_17( v6,v1 );
    if( sv[0] != con_1 ) {
      goto myend284;
    }
    /* rhs: append_taille(,)(var0,var2) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[1] = fun_480( v1,v7 );
    res = sv[1] ;
    goto end;
    myend284:;
  }
match_fail:
  GmakeAppl2(res,code_480  ,v1  ,v2  );
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

Gterm* fun_481(Gterm *v1,Gterm *v2 ) {
  Gterm *v3,*v4,*v5,*v6,*v7,*v8,*v9;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,3);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_445: /* cons_equation(,) */
    v5= GgetArgument(v1,0);
    switch(GgetSymb(v5)) {
    default:
    label1027:
      v6= GgetArgument(v1,1);
      switch(GgetSymb(v6)) {
      default:
      label1028:
        switch(GgetSymb(v2)) {
        default:
        label1029:
          bitSet32_set(mask32,1);
          bitSet32_set(mask32,2);
        }
      }
    }
    break;
  case code_444: /* nil */
    switch(GgetSymb(v2)) {
    default:
    label1025:
      bitSet32_set(mask32,0);
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: append_taille(,)(nil,var0) */
    /* allDetEvaluation: det */
    /* rhs: cons_equation(,)(var0,nil) */
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl2(sv[1],code_445    ,v2    ,con_444    );
    res = sv[1] ;
    goto end;
    myend285:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: append_taille(,)(cons_equation(,)(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* if less_int(,)(taille()((var2)),taille()((var0))) */
    // this=var2        underAC=false        Instantiated=false
    GmakeAppl1(sv[0],code_425    ,v2    );
    sv[1] = fun_441( sv[0] );
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[0],code_425    ,v5    );
    sv[2] = fun_441( sv[0] );
    sv[0] = fun_315( sv[1],sv[2] );
    if( sv[0] != con_1 ) {
      goto myend286;
    }
    /* rhs: cons_equation(,)(var2,cons_equation(,)(var0,var1)) */
    // this=var2        underAC=false        Instantiated=false
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    GmakeAppl2(sv[1],code_445    ,v5    ,v6    );
    GmakeAppl2(sv[2],code_445    ,v2    ,sv[1]    );
    res = sv[2] ;
    goto end;
    myend286:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: append_taille(,)(cons_equation(,)(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* if greatereq_int(,)(taille()((var2)),taille()((var0))) */
    // this=var2        underAC=false        Instantiated=false
    GmakeAppl1(sv[0],code_425    ,v2    );
    sv[1] = fun_441( sv[0] );
    // this=var0        underAC=false        Instantiated=false
    GmakeAppl1(sv[0],code_425    ,v5    );
    sv[2] = fun_441( sv[0] );
    sv[0] = fun_313( sv[1],sv[2] );
    if( sv[0] != con_1 ) {
      goto myend287;
    }
    /* rhs: cons_equation(,)(var0,append_taille(,)(var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[1] = fun_481( v6,v2 );
    GmakeAppl2(sv[2],code_445    ,v5    ,sv[1]    );
    res = sv[2] ;
    goto end;
    myend287:;
  }
match_fail:
  GmakeAppl2(res,code_481  ,v1  ,v2  );
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

Gterm* fun_484(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_461: /* ***** */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label1033:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      case code_444: /* nil */
        v6= GgetArgument(v1,2);
        switch(GgetSymb(v6)) {
        case code_445: /* cons_equation(,) */
          v7= GgetArgument(v6,0);
          switch(GgetSymb(v7)) {
          default:
          label1036:
            v8= GgetArgument(v6,1);
            switch(GgetSymb(v8)) {
            case code_444: /* nil */
              v9= GgetArgument(v1,3);
              switch(GgetSymb(v9)) {
              default:
              label1038:
                v10= GgetArgument(v1,4);
                switch(GgetSymb(v10)) {
                default:
                label1039:
                  v11= GgetArgument(v1,5);
                  switch(GgetSymb(v11)) {
                  default:
                  label1040:
                    bitSet32_set(mask32,0);
                  }
                }
              }
              break;
            /* matching is not complete: jumpNode is null */
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
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
    Gterm *tmp, *sv[4];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: AC_to_N()(*****(var0,nil,cons_equation(,)(var1,nil),var2,var3,var4)) */
    /* allDetEvaluation: det */
    /* rhs: *****(nil,append_taille(,)(var0,var1),nil,var2,var3,var4) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_481( v4,v7 );
    // this=var2        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    // this=var4        underAC=false        Instantiated=false
    GmakeAppl6(sv[3],code_461    ,con_444    ,sv[1]    ,con_444    ,v9    ,v10    ,v11    );
    res = sv[3] ;
    goto end;
    myend288:;
  }
match_fail:
  GmakeAppl1(res,code_484  ,v1  );
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

Gterm* fun_485(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_461: /* ***** */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label1044:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label1045:
        v6= GgetArgument(v1,2);
        switch(GgetSymb(v6)) {
        default:
        label1046:
          v7= GgetArgument(v1,3);
          switch(GgetSymb(v7)) {
          default:
          label1047:
            v8= GgetArgument(v1,4);
            switch(GgetSymb(v8)) {
            case code_445: /* cons_equation(,) */
              v9= GgetArgument(v8,0);
              switch(GgetSymb(v9)) {
              default:
              label1049:
                v10= GgetArgument(v8,1);
                switch(GgetSymb(v10)) {
                default:
                label1050:
                  v11= GgetArgument(v1,5);
                  switch(GgetSymb(v11)) {
                  default:
                  label1051:
                    bitSet32_set(mask32,0);
                  }
                }
              }
              break;
            /* matching is not complete: jumpNode is null */
            }
          }
        }
      }
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: S_to_T()(*****(var0,var1,var2,var3,cons_equation(,)(var4,var5),var6)) */
    /* allDetEvaluation: det */
    /* rhs: *****(var0,var1,var2,append_taille(,)(var3,var4),var5,var6) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    // this=var4        underAC=false        Instantiated=false
    sv[0] = fun_481( v7,v9 );
    // this=var5        underAC=false        Instantiated=false
    // this=var6        underAC=false        Instantiated=false
    GmakeAppl6(sv[1],code_461    ,v4    ,v5    ,v6    ,sv[0]    ,v10    ,v11    );
    res = sv[1] ;
    goto end;
    myend289:;
  }
match_fail:
  GmakeAppl1(res,code_485  ,v1  );
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

Gterm* fun_486(Gterm *v1 ) {
  Gterm *v2,*v3,*v4,*v5,*v6;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  case code_444: /* nil */
    bitSet32_set(mask32,1);
    break;
  case code_445: /* cons_equation(,) */
    v4= GgetArgument(v1,0);
    switch(GgetSymb(v4)) {
    default:
    label1055:
      v5= GgetArgument(v1,1);
      switch(GgetSymb(v5)) {
      default:
      label1056:
        bitSet32_set(mask32,0);
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
    /* lhs: normalize()(cons_equation(,)(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: cons_equation(,)(normalize()(var0),normalize()(var1)) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_438( v4 );
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_486( v5 );
    GmakeAppl2(sv[2],code_445    ,sv[0]    ,sv[1]    );
    res = sv[2] ;
    goto end;
    myend290:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: normalize()(nil) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_444 ;
    goto end;
    myend291:;
  }
match_fail:
  GmakeAppl1(res,code_486  ,v1  );
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

Gterm* fun_487( ) {
  Gterm *v1,*v2;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  bitSet32_set(mask32,0);
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[13];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: eqlist */
    /* allDetEvaluation: det */
    /* rhs: cons_equation(,)(=(f(,)(f(,)((x),(y)),(z)),f(,)((x),f(,)((y),(z)))),cons_equation(,)(=(f(,)(e1,(x)),(x)),cons_equation(,)(=(f(,)(e2,(x)),(x)),cons_equation(,)(=(f(,)(e3,(x)),(x)),cons_equation(,)(=(f(,)(e4,(x)),(x)),cons_equation(,)(=(f(,)(e5,(x)),(x)),cons_equation(,)(=(f(,)((x),i1()((x))),e1),cons_equation(,)(=(f(,)((x),i2()((x))),e2),cons_equation(,)(=(f(,)((x),i3()((x))),e3),cons_equation(,)(=(f(,)((x),i4()((x))),e4),cons_equation(,)(=(f(,)((x),i5()((x))),e5),nil))))))))))) */
    GmakeAppl1(sv[1],code_362    ,con_381    );
    GmakeAppl1(sv[2],code_362    ,con_382    );
    GmakeAppl2(sv[0],code_375    ,sv[1]    ,sv[2]    );
    GmakeAppl1(sv[2],code_362    ,con_383    );
    GmakeAppl2(sv[1],code_375    ,sv[0]    ,sv[2]    );
    GmakeAppl1(sv[2],code_362    ,con_381    );
    GmakeAppl1(sv[3],code_362    ,con_382    );
    GmakeAppl1(sv[4],code_362    ,con_383    );
    GmakeAppl2(sv[0],code_375    ,sv[3]    ,sv[4]    );
    GmakeAppl2(sv[3],code_375    ,sv[2]    ,sv[0]    );
    GmakeAppl2(sv[0],code_423    ,sv[1]    ,sv[3]    );
    GmakeAppl1(sv[3],code_362    ,con_381    );
    GmakeAppl2(sv[2],code_375    ,con_370    ,sv[3]    );
    GmakeAppl1(sv[3],code_362    ,con_381    );
    GmakeAppl2(sv[1],code_423    ,sv[2]    ,sv[3]    );
    GmakeAppl1(sv[4],code_362    ,con_381    );
    GmakeAppl2(sv[3],code_375    ,con_371    ,sv[4]    );
    GmakeAppl1(sv[4],code_362    ,con_381    );
    GmakeAppl2(sv[2],code_423    ,sv[3]    ,sv[4]    );
    GmakeAppl1(sv[5],code_362    ,con_381    );
    GmakeAppl2(sv[4],code_375    ,con_372    ,sv[5]    );
    GmakeAppl1(sv[5],code_362    ,con_381    );
    GmakeAppl2(sv[3],code_423    ,sv[4]    ,sv[5]    );
    GmakeAppl1(sv[6],code_362    ,con_381    );
    GmakeAppl2(sv[5],code_375    ,con_373    ,sv[6]    );
    GmakeAppl1(sv[6],code_362    ,con_381    );
    GmakeAppl2(sv[4],code_423    ,sv[5]    ,sv[6]    );
    GmakeAppl1(sv[7],code_362    ,con_381    );
    GmakeAppl2(sv[6],code_375    ,con_374    ,sv[7]    );
    GmakeAppl1(sv[7],code_362    ,con_381    );
    GmakeAppl2(sv[5],code_423    ,sv[6]    ,sv[7]    );
    GmakeAppl1(sv[7],code_362    ,con_381    );
    GmakeAppl1(sv[8],code_362    ,con_381    );
    GmakeAppl1(sv[6],code_376    ,sv[8]    );
    GmakeAppl2(sv[8],code_375    ,sv[7]    ,sv[6]    );
    GmakeAppl2(sv[7],code_423    ,sv[8]    ,con_370    );
    GmakeAppl1(sv[8],code_362    ,con_381    );
    GmakeAppl1(sv[9],code_362    ,con_381    );
    GmakeAppl1(sv[6],code_377    ,sv[9]    );
    GmakeAppl2(sv[9],code_375    ,sv[8]    ,sv[6]    );
    GmakeAppl2(sv[8],code_423    ,sv[9]    ,con_371    );
    GmakeAppl1(sv[9],code_362    ,con_381    );
    GmakeAppl1(sv[10],code_362    ,con_381    );
    GmakeAppl1(sv[6],code_378    ,sv[10]    );
    GmakeAppl2(sv[10],code_375    ,sv[9]    ,sv[6]    );
    GmakeAppl2(sv[9],code_423    ,sv[10]    ,con_372    );
    GmakeAppl1(sv[10],code_362    ,con_381    );
    GmakeAppl1(sv[11],code_362    ,con_381    );
    GmakeAppl1(sv[6],code_379    ,sv[11]    );
    GmakeAppl2(sv[11],code_375    ,sv[10]    ,sv[6]    );
    GmakeAppl2(sv[10],code_423    ,sv[11]    ,con_373    );
    GmakeAppl1(sv[11],code_362    ,con_381    );
    GmakeAppl1(sv[12],code_362    ,con_381    );
    GmakeAppl1(sv[6],code_380    ,sv[12]    );
    GmakeAppl2(sv[12],code_375    ,sv[11]    ,sv[6]    );
    GmakeAppl2(sv[11],code_423    ,sv[12]    ,con_374    );
    GmakeAppl2(sv[12],code_445    ,sv[11]    ,con_444    );
    GmakeAppl2(sv[6],code_445    ,sv[10]    ,sv[12]    );
    GmakeAppl2(sv[10],code_445    ,sv[9]    ,sv[6]    );
    GmakeAppl2(sv[6],code_445    ,sv[8]    ,sv[10]    );
    GmakeAppl2(sv[8],code_445    ,sv[7]    ,sv[6]    );
    GmakeAppl2(sv[6],code_445    ,sv[5]    ,sv[8]    );
    GmakeAppl2(sv[5],code_445    ,sv[4]    ,sv[6]    );
    GmakeAppl2(sv[4],code_445    ,sv[3]    ,sv[5]    );
    GmakeAppl2(sv[3],code_445    ,sv[2]    ,sv[4]    );
    GmakeAppl2(sv[2],code_445    ,sv[1]    ,sv[3]    );
    GmakeAppl2(sv[1],code_445    ,sv[0]    ,sv[2]    );
    res = sv[1] ;
    goto end;
    myend292:;
  }
match_fail:
  GmakeAppl0(res,code_487);
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

Gterm* fun_495(Gterm *v1,Gterm *v2,Gterm *v3,Gterm *v4 ) {
  Gterm *v5,*v6,*v7,*v8,*v9,*v10;
  Gterm *res;
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,3);
  bitSet32_init_clear(mask32);
  /* Begin syntactical matching */
  switch(GgetSymb(v1)) {
  default:
  label1062:
    switch(GgetSymb(v2)) {
    default:
    label1063:
      switch(GgetSymb(v3)) {
      default:
      label1064:
        switch(GgetSymb(v4)) {
        default:
        label1065:
          bitSet32_set(mask32,0);
          bitSet32_set(mask32,1);
          bitSet32_set(mask32,2);
        }
      }
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[2];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: supemb(,,,)(var0,var1,var2,var3) */
    /* allDetEvaluation: det */
    /* if neq_list[int](,)(var2,nil) */
    // this=var2        underAC=false        Instantiated=false
    sv[1] = fun_19( v3,con_345 );
    if( sv[1] != con_1 ) {
      goto myend293;
    }
    /* rhs: true */
    res = con_1 ;
    goto end;
    myend293:;
  }
  if(bitSet32_get(mask32,1)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: supemb(,,,)(var0,var1,var2,var3) */
    /* allDetEvaluation: det */
    /* if neq_list[int](,)(left()(var0),left()(var1)) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_430( v1 );
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_430( v2 );
    sv[2] = fun_19( sv[0],sv[1] );
    if( sv[2] != con_1 ) {
      goto myend294;
    }
    /* rhs: true */
    res = con_1 ;
    goto end;
    myend294:;
  }
  if(bitSet32_get(mask32,2)) {
    Gterm *tmp, *sv[3];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: supemb(,,,)(var0,var1,var2,var3) */
    /* allDetEvaluation: det */
    /* rhs: >lpo(right()(var0),apply(,)(var3,right()(var1))) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_431( v1 );
    // this=var3        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_431( v2 );
    sv[2] = fun_453( v4,sv[1] );
    sv[1] = fun_411( sv[0],sv[2] );
    res = sv[1] ;
    goto end;
    myend295:;
  }
match_fail:
  GmakeAppl4(res,code_495  ,v1  ,v2  ,v3  ,v4  );
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

Gterm* fun_509(Gterm *v1 ) {
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
  label1068:
    bitSet32_set(mask32,0);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    Gterm *tmp, *sv[1];
    multiplicityType *E,*sol;
    Gterm *substitution[1];
    /* lhs: identity()(var0) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v1 ;
    goto end;
    myend296:;
  }
match_fail:
  GmakeAppl1(res,code_509  ,v1  );
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

Gterm* str_9( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](simplify_E:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_461: /* ***** */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1071:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        default:
        label1072:
          v4= GgetArgument(v0,2);
          switch(GgetSymb(v4)) {
          default:
          label1073:
            v5= GgetArgument(v0,3);
            switch(GgetSymb(v5)) {
            default:
            label1074:
              v6= GgetArgument(v0,4);
              switch(GgetSymb(v6)) {
              default:
              label1075:
                v7= GgetArgument(v0,5);
                switch(GgetSymb(v7)) {
                default:
                label1076:
                  bitSet_set(mask,0);
                }
              }
            }
          }
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[5];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,var2,var3,var4,var5) */
      /* allDetEvaluation: det */
      /* where var7 := (s_simplify_ics:internal_compute_state/ans_completion[Vars,Ops,Prec,System]) |||(nil,nil,@(var0,@(var1,@(var2,var3))),var5) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      sv[3] = fun_446( v4,v5 );
      sv[4] = fun_446( v3,sv[3] );
      sv[3] = fun_446( v2,sv[4] );
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl4(sv[4],code_462      ,con_444      ,con_444      ,sv[3]      ,v7      );
      sv[0] = strTab[39]( sv[4] );
      /* where var6 := ICS_to_1()(var7) */
      // this=var7        underAC=false        Instantiated=false
      sv[2] = fun_469( sv[0] );
      tmp = sv[1] = sv[2];
      /* rhs: *****(var0,var1,var2,var3,var4,var6) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var6        underAC=false        Instantiated=false
      GmakeAppl6(sv[3],code_461      ,v2      ,v3      ,v4      ,v5      ,v6      ,sv[1]      );
      res = sv[3] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab8;
      myend297:;
    }
  }
  fail();
  stratLab8:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_151( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](delete:list[equation]/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7;
    bitSet_GC_create(mask,3);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_488: /* delete() */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_445: /* cons_equation(,) */
        v3= GgetArgument(v2,0);
        switch(GgetSymb(v3)) {
        case code_423: /* = */
          v4= GgetArgument(v3,0);
          switch(GgetSymb(v4)) {
          default:
          label1082:
            v5= GgetArgument(v3,1);
            switch(GgetSymb(v5)) {
            default:
            label1083:
              v6= GgetArgument(v2,1);
              switch(GgetSymb(v6)) {
              default:
              label1084:
                bitSet_set(mask,1);
                bitSet_set(mask,2);
              }
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
        break;
      case code_444: /* nil */
        bitSet_set(mask,0);
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[1];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: delete()(nil) */
      /* allDetEvaluation: det */
      /* rhs: nil */
      res = con_444 ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab126;
      myend298:;
    }
    if(bitSet_get(mask,1)) {
      Gterm *tmp, *sv[3];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: delete()(cons_equation(,)(=(var0,var1),var2)) */
      /* allDetEvaluation: semiDet */
      CUTOPEN(); /* Wheres */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend299;
      }
      /* if eq_list[int](,)(var0,var1) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      sv[0] = fun_18( v4,v5 );
      if( sv[0] != con_1 ) {
        fail();
      }
      /* where var3 := (WHERE20:list[equation]/ans_completion[Vars,Ops,Prec,System]) delete()(var2) */
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl1(sv[2],code_488      ,v6      );
      sv[1] = strTab[152]( sv[2] );
      CUTCLOSE(); /* Wheres */
      /* rhs: var3 */
      // this=var3        underAC=false        Instantiated=false
      res = sv[1] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab126;
      myend299:;
      CUTCLOSE(); /* Wheres */
    }
    if(bitSet_get(mask,2)) {
      Gterm *tmp, *sv[5];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: delete()(cons_equation(,)(=(var0,var1),var2)) */
      /* allDetEvaluation: semiDet */
      CUTOPEN(); /* Wheres */
      /* if neq_list[int](,)(var0,var1) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      sv[0] = fun_19( v4,v5 );
      if( sv[0] != con_1 ) {
        fail();
      }
      /* where var3 := (WHERE21:list[equation]/ans_completion[Vars,Ops,Prec,System]) delete()(var2) */
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl1(sv[2],code_488      ,v6      );
      sv[1] = strTab[153]( sv[2] );
      CUTCLOSE(); /* Wheres */
      /* rhs: cons_equation(,)(=(var0,var1),var3) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl2(sv[3],code_423      ,v4      ,v5      );
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl2(sv[4],code_445      ,sv[3]      ,sv[1]      );
      res = sv[4] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab126;
      myend300:;
      CUTCLOSE(); /* Wheres */
    }
  }
  fail();
  stratLab126:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_24( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](collapse:equation/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_490: /* collapse(,) */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_429: /* -> */
        v3= GgetArgument(v2,0);
        switch(GgetSymb(v3)) {
        default:
        label1088:
          v4= GgetArgument(v2,1);
          switch(GgetSymb(v4)) {
          default:
          label1089:
            v5= GgetArgument(v0,1);
            switch(GgetSymb(v5)) {
            case code_429: /* -> */
              v6= GgetArgument(v5,0);
              switch(GgetSymb(v6)) {
              default:
              label1091:
                v7= GgetArgument(v5,1);
                switch(GgetSymb(v7)) {
                default:
                label1092:
                  bitSet_set(mask,0);
                }
              }
              break;
            /* matching is not complete: jumpNode is null */
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
      Gterm *tmp, *sv[13];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: collapse(,)(->(var0,var1),->(var2,var3)) */
      /* allDetEvaluation: nonDet */
      CUTOPEN(); /* Wheres */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend301;
      }
      /* where var5 := translate(,)(->(var2,var3),maxvar(,)(var0,(0))) */
      // this=var2        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl2(sv[1],code_429      ,v6      ,v7      );
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl1(sv[3],code_301      ,(GsetIntegerTag(0))      );
      sv[2] = fun_408( v3,sv[3] );
      sv[3] = fun_434( sv[1],sv[2] );
      tmp = sv[0] = sv[3];
      /* where var6 := (chooseOccurence:list[int]/termCommons[Ops,Vars]) nvocc()(var0) */
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl1(sv[2],code_366      ,v3      );
      sv[1] = strTab[242]( sv[2] );
      /* where var7 := (matchs:eqSystem/syntacticUnification[Ops,Vars]) (&)((true),(=(left()(var5),at(var0,var6)))) */
      GmakeAppl1(sv[6],code_424      ,con_1      );
      // this=var5        underAC=false        Instantiated=false
      sv[5] = fun_430( sv[0] );
      // this=var0        underAC=false        Instantiated=false
      // this=var6        underAC=false        Instantiated=false
      sv[7] = fun_367( v3,sv[1] );
      GmakeAppl2(sv[8],code_423      ,sv[5]      ,sv[7]      );
      GmakeAppl1(sv[5],code_425      ,sv[8]      );
      GmakeAppl2(sv[7],code_426      ,sv[6]      ,sv[5]      );
      sv[4] = strTab[173]( sv[7] );
      /* where var8 := system_to_subst()(var7) */
      // this=var7        underAC=false        Instantiated=false
      sv[6] = fun_427( sv[4] );
      tmp = sv[5] = sv[6];
      /* where var4 := []at(var0,apply(,)(var8,right()(var5)),var6) */
      // this=var0        underAC=false        Instantiated=false
      // this=var8        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      sv[9] = fun_431( sv[0] );
      sv[10] = fun_453( sv[5],sv[9] );
      // this=var6        underAC=false        Instantiated=false
      sv[9] = fun_368( v3,sv[10],sv[1] );
      tmp = sv[8] = sv[9];
      /* if supemb(,,,)(->(var0,var1),->(var2,var3),var6,var8) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl2(sv[10],code_429      ,v3      ,v4      );
      // this=var2        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl2(sv[11],code_429      ,v6      ,v7      );
      // this=var6        underAC=false        Instantiated=false
      // this=var8        underAC=false        Instantiated=false
      sv[12] = fun_495( sv[10],sv[11],sv[1],sv[5] );
      if( sv[12] != con_1 ) {
        fail();
      }
      CUTCLOSE(); /* Wheres */
      /* rhs: =(var4,var1) */
      // this=var4        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl2(sv[10],code_423      ,sv[8]      ,v4      );
      res = sv[10] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab30;
      myend301:;
      CUTCLOSE(); /* Wheres */
    }
  }
  fail();
  stratLab30:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_25( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](compose:equation/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_489: /* compose(,) */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_429: /* -> */
        v3= GgetArgument(v2,0);
        switch(GgetSymb(v3)) {
        default:
        label1096:
          v4= GgetArgument(v2,1);
          switch(GgetSymb(v4)) {
          default:
          label1097:
            v5= GgetArgument(v0,1);
            switch(GgetSymb(v5)) {
            case code_429: /* -> */
              v6= GgetArgument(v5,0);
              switch(GgetSymb(v6)) {
              default:
              label1099:
                v7= GgetArgument(v5,1);
                switch(GgetSymb(v7)) {
                default:
                label1100:
                  bitSet_set(mask,0);
                }
              }
              break;
            /* matching is not complete: jumpNode is null */
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
      Gterm *tmp, *sv[5];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: compose(,)(->(var0,var1),->(var2,var3)) */
      /* allDetEvaluation: nonDet */
      CUTOPEN(); /* Wheres */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend302;
      }
      /* where var5 := translate(,)(->(var2,var3),maxvar(,)(var0,(0))) */
      // this=var2        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl2(sv[1],code_429      ,v6      ,v7      );
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl1(sv[3],code_301      ,(GsetIntegerTag(0))      );
      sv[2] = fun_408( v3,sv[3] );
      sv[3] = fun_434( sv[1],sv[2] );
      tmp = sv[0] = sv[3];
      /* where var4 := (WHERE17:term/ans_completion[Vars,Ops,Prec,System]) rewriteStep(,)(var1,var5) */
      // this=var1        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_459      ,v4      ,sv[0]      );
      sv[1] = strTab[100]( sv[2] );
      CUTCLOSE(); /* Wheres */
      /* rhs: ->(var0,var4) */
      // this=var0        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      GmakeAppl2(sv[4],code_429      ,v3      ,sv[1]      );
      res = sv[4] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab32;
      myend302:;
      CUTCLOSE(); /* Wheres */
    }
  }
  fail();
  stratLab32:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_237( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](compose:equation/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_489: /* compose(,) */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_429: /* -> */
        v3= GgetArgument(v2,0);
        switch(GgetSymb(v3)) {
        default:
        label1104:
          v4= GgetArgument(v2,1);
          switch(GgetSymb(v4)) {
          default:
          label1105:
            v5= GgetArgument(v0,1);
            switch(GgetSymb(v5)) {
            case code_429: /* -> */
              v6= GgetArgument(v5,0);
              switch(GgetSymb(v6)) {
              default:
              label1107:
                v7= GgetArgument(v5,1);
                switch(GgetSymb(v7)) {
                default:
                label1108:
                  bitSet_set(mask,0);
                }
              }
              break;
            /* matching is not complete: jumpNode is null */
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
      Gterm *tmp, *sv[5];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: compose(,)(->(var0,var1),->(var2,var3)) */
      /* allDetEvaluation: nonDet */
      CUTOPEN(); /* Wheres */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend303;
      }
      /* where var5 := translate(,)(->(var2,var3),maxvar(,)(var0,(0))) */
      // this=var2        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl2(sv[1],code_429      ,v6      ,v7      );
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl1(sv[3],code_301      ,(GsetIntegerTag(0))      );
      sv[2] = fun_408( v3,sv[3] );
      sv[3] = fun_434( sv[1],sv[2] );
      tmp = sv[0] = sv[3];
      /* where var4 := (WHERE17:term/ans_completion[Vars,Ops,Prec,System]) rewriteStep(,)(var1,var5) */
      // this=var1        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_459      ,v4      ,sv[0]      );
      sv[1] = strTab[100]( sv[2] );
      CUTCLOSE(); /* Wheres */
      /* rhs: ->(var0,var4) */
      // this=var0        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      GmakeAppl2(sv[4],code_429      ,v3      ,sv[1]      );
      res = sv[4] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab184;
      myend303:;
      CUTCLOSE(); /* Wheres */
    }
  }
  fail();
  stratLab184:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_27( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* dk[nonDet](simplify:equation/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8;
    bitSet_GC_create(mask,2);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_491: /* simplify(,) */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_423: /* = */
        v3= GgetArgument(v2,0);
        switch(GgetSymb(v3)) {
        default:
        label1112:
          v4= GgetArgument(v2,1);
          switch(GgetSymb(v4)) {
          default:
          label1113:
            v5= GgetArgument(v0,1);
            switch(GgetSymb(v5)) {
            case code_429: /* -> */
              v6= GgetArgument(v5,0);
              switch(GgetSymb(v6)) {
              default:
              label1115:
                v7= GgetArgument(v5,1);
                switch(GgetSymb(v7)) {
                default:
                label1116:
                  bitSet_set(mask,0);
                  bitSet_set(mask,1);
                }
              }
              break;
            /* matching is not complete: jumpNode is null */
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
      Gterm *tmp, *sv[5];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: simplify(,)(=(var0,var1),->(var2,var3)) */
      /* allDetEvaluation: nonDet */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend304;
      }
      /* where var5 := translate(,)(->(var2,var3),maxvar(,)(var0,maxvar(,)(var1,(0)))) */
      // this=var2        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl2(sv[1],code_429      ,v6      ,v7      );
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl1(sv[3],code_301      ,(GsetIntegerTag(0))      );
      sv[2] = fun_408( v4,sv[3] );
      sv[3] = fun_408( v3,sv[2] );
      sv[2] = fun_434( sv[1],sv[3] );
      tmp = sv[0] = sv[2];
      /* where var4 := (WHERE18:term/ans_completion[Vars,Ops,Prec,System]) rewriteStep(,)(var1,var5) */
      // this=var1        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[3],code_459      ,v4      ,sv[0]      );
      sv[1] = strTab[101]( sv[3] );
      /* rhs: =(var0,var4) */
      // this=var0        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      GmakeAppl2(sv[4],code_423      ,v3      ,sv[1]      );
      res = sv[4] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab40;
      myend304:;
    }
    if(bitSet_get(mask,1)) {
      Gterm *tmp, *sv[5];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: simplify(,)(=(var0,var1),->(var2,var3)) */
      /* allDetEvaluation: nonDet */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend305;
      }
      /* where var5 := translate(,)(->(var2,var3),maxvar(,)(var0,maxvar(,)(var1,(0)))) */
      // this=var2        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl2(sv[1],code_429      ,v6      ,v7      );
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl1(sv[3],code_301      ,(GsetIntegerTag(0))      );
      sv[2] = fun_408( v4,sv[3] );
      sv[3] = fun_408( v3,sv[2] );
      sv[2] = fun_434( sv[1],sv[3] );
      tmp = sv[0] = sv[2];
      /* where var4 := (WHERE19:term/ans_completion[Vars,Ops,Prec,System]) rewriteStep(,)(var0,var5) */
      // this=var0        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[3],code_459      ,v3      ,sv[0]      );
      sv[1] = strTab[102]( sv[3] );
      /* rhs: =(var4,var1) */
      // this=var4        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl2(sv[4],code_423      ,sv[1]      ,v4      );
      res = sv[4] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab40;
      myend305:;
    }
  }
  fail();
  stratLab40:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_28( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* dk[nonDet](deduce:equation/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14;
    bitSet_GC_create(mask,2);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_493: /* internal_deduce(,) */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_429: /* -> */
        v3= GgetArgument(v2,0);
        switch(GgetSymb(v3)) {
        default:
        label1127:
          v4= GgetArgument(v2,1);
          switch(GgetSymb(v4)) {
          default:
          label1128:
            v5= GgetArgument(v0,1);
            switch(GgetSymb(v5)) {
            case code_429: /* -> */
              v6= GgetArgument(v5,0);
              switch(GgetSymb(v6)) {
              default:
              label1130:
                v7= GgetArgument(v5,1);
                switch(GgetSymb(v7)) {
                default:
                label1131:
                  bitSet_set(mask,1);
                }
              }
              break;
            /* matching is not complete: jumpNode is null */
            }
          }
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    case code_492: /* deduce(,) */
      v8= GgetArgument(v0,0);
      switch(GgetSymb(v8)) {
      case code_429: /* -> */
        v9= GgetArgument(v8,0);
        switch(GgetSymb(v9)) {
        default:
        label1120:
          v10= GgetArgument(v8,1);
          switch(GgetSymb(v10)) {
          default:
          label1121:
            v11= GgetArgument(v0,1);
            switch(GgetSymb(v11)) {
            case code_429: /* -> */
              v12= GgetArgument(v11,0);
              switch(GgetSymb(v12)) {
              default:
              label1123:
                v13= GgetArgument(v11,1);
                switch(GgetSymb(v13)) {
                default:
                label1124:
                  bitSet_set(mask,0);
                }
              }
              break;
            /* matching is not complete: jumpNode is null */
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
      Gterm *tmp, *sv[10];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: deduce(,)(->(var0,var1),->(var2,var3)) */
      /* allDetEvaluation: nonDet */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend306;
      }
      /* where var5 := (chooseOccurence:list[int]/termCommons[Ops,Vars]) nvocc()(var0) */
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl1(sv[1],code_366      ,v9      );
      sv[0] = strTab[242]( sv[1] );
      /* where var6 := (unifys:eqSystem/syntacticUnification[Ops,Vars]) (&)((true),(=(at(var0,var5),var2))) */
      GmakeAppl1(sv[4],code_424      ,con_1      );
      // this=var0        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      sv[3] = fun_367( v9,sv[0] );
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl2(sv[5],code_423      ,sv[3]      ,v12      );
      GmakeAppl1(sv[3],code_425      ,sv[5]      );
      GmakeAppl2(sv[5],code_426      ,sv[4]      ,sv[3]      );
      sv[2] = strTab[203]( sv[5] );
      /* where var7 := system_to_subst()(var6) */
      // this=var6        underAC=false        Instantiated=false
      sv[4] = fun_427( sv[2] );
      tmp = sv[3] = sv[4];
      /* where var4 := =([]at(apply(,)(var7,var0),apply(,)(var7,var3),var5),apply(,)(var7,var1)) */
      // this=var7        underAC=false        Instantiated=false
      // this=var0        underAC=false        Instantiated=false
      sv[7] = fun_453( sv[3],v9 );
      // this=var7        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      sv[8] = fun_453( sv[3],v13 );
      // this=var5        underAC=false        Instantiated=false
      sv[9] = fun_368( sv[7],sv[8],sv[0] );
      // this=var7        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      sv[7] = fun_453( sv[3],v10 );
      GmakeAppl2(sv[8],code_423      ,sv[9]      ,sv[7]      );
      tmp = sv[6] = sv[8];
      /* rhs: var4 */
      // this=var4        underAC=false        Instantiated=false
      res = sv[6] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab42;
      myend306:;
    }
    if(bitSet_get(mask,1)) {
      Gterm *tmp, *sv[11];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: internal_deduce(,)(->(var0,var1),->(var2,var3)) */
      /* allDetEvaluation: nonDet */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend307;
      }
      /* where var5 := (chooseOccurence:list[int]/termCommons[Ops,Vars]) nvocc()(var0) */
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl1(sv[1],code_366      ,v3      );
      sv[0] = strTab[242]( sv[1] );
      /* if neq_list[int](,)(var5,nil) */
      // this=var5        underAC=false        Instantiated=false
      sv[3] = fun_19( sv[0],con_345 );
      if( sv[3] != con_1 ) {
        fail();
      }
      /* where var6 := (unifys:eqSystem/syntacticUnification[Ops,Vars]) (&)((true),(=(at(var0,var5),var2))) */
      GmakeAppl1(sv[5],code_424      ,con_1      );
      // this=var0        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      sv[4] = fun_367( v3,sv[0] );
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl2(sv[6],code_423      ,sv[4]      ,v6      );
      GmakeAppl1(sv[4],code_425      ,sv[6]      );
      GmakeAppl2(sv[6],code_426      ,sv[5]      ,sv[4]      );
      sv[2] = strTab[203]( sv[6] );
      /* where var7 := system_to_subst()(var6) */
      // this=var6        underAC=false        Instantiated=false
      sv[5] = fun_427( sv[2] );
      tmp = sv[4] = sv[5];
      /* where var4 := =([]at(apply(,)(var7,var0),apply(,)(var7,var3),var5),apply(,)(var7,var1)) */
      // this=var7        underAC=false        Instantiated=false
      // this=var0        underAC=false        Instantiated=false
      sv[8] = fun_453( sv[4],v3 );
      // this=var7        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      sv[9] = fun_453( sv[4],v7 );
      // this=var5        underAC=false        Instantiated=false
      sv[10] = fun_368( sv[8],sv[9],sv[0] );
      // this=var7        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      sv[8] = fun_453( sv[4],v4 );
      GmakeAppl2(sv[9],code_423      ,sv[10]      ,sv[8]      );
      tmp = sv[7] = sv[9];
      /* rhs: var4 */
      // this=var4        underAC=false        Instantiated=false
      res = sv[7] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab42;
      myend307:;
    }
  }
  fail();
  stratLab42:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_29( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* dk[nonDet](deduce:equation/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14;
    bitSet_GC_create(mask,2);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_493: /* internal_deduce(,) */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_429: /* -> */
        v3= GgetArgument(v2,0);
        switch(GgetSymb(v3)) {
        default:
        label1142:
          v4= GgetArgument(v2,1);
          switch(GgetSymb(v4)) {
          default:
          label1143:
            v5= GgetArgument(v0,1);
            switch(GgetSymb(v5)) {
            case code_429: /* -> */
              v6= GgetArgument(v5,0);
              switch(GgetSymb(v6)) {
              default:
              label1145:
                v7= GgetArgument(v5,1);
                switch(GgetSymb(v7)) {
                default:
                label1146:
                  bitSet_set(mask,1);
                }
              }
              break;
            /* matching is not complete: jumpNode is null */
            }
          }
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    case code_492: /* deduce(,) */
      v8= GgetArgument(v0,0);
      switch(GgetSymb(v8)) {
      case code_429: /* -> */
        v9= GgetArgument(v8,0);
        switch(GgetSymb(v9)) {
        default:
        label1135:
          v10= GgetArgument(v8,1);
          switch(GgetSymb(v10)) {
          default:
          label1136:
            v11= GgetArgument(v0,1);
            switch(GgetSymb(v11)) {
            case code_429: /* -> */
              v12= GgetArgument(v11,0);
              switch(GgetSymb(v12)) {
              default:
              label1138:
                v13= GgetArgument(v11,1);
                switch(GgetSymb(v13)) {
                default:
                label1139:
                  bitSet_set(mask,0);
                }
              }
              break;
            /* matching is not complete: jumpNode is null */
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
      Gterm *tmp, *sv[10];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: deduce(,)(->(var0,var1),->(var2,var3)) */
      /* allDetEvaluation: nonDet */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend308;
      }
      /* where var5 := (chooseOccurence:list[int]/termCommons[Ops,Vars]) nvocc()(var0) */
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl1(sv[1],code_366      ,v9      );
      sv[0] = strTab[242]( sv[1] );
      /* where var6 := (unifys:eqSystem/syntacticUnification[Ops,Vars]) (&)((true),(=(at(var0,var5),var2))) */
      GmakeAppl1(sv[4],code_424      ,con_1      );
      // this=var0        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      sv[3] = fun_367( v9,sv[0] );
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl2(sv[5],code_423      ,sv[3]      ,v12      );
      GmakeAppl1(sv[3],code_425      ,sv[5]      );
      GmakeAppl2(sv[5],code_426      ,sv[4]      ,sv[3]      );
      sv[2] = strTab[203]( sv[5] );
      /* where var7 := system_to_subst()(var6) */
      // this=var6        underAC=false        Instantiated=false
      sv[4] = fun_427( sv[2] );
      tmp = sv[3] = sv[4];
      /* where var4 := =([]at(apply(,)(var7,var0),apply(,)(var7,var3),var5),apply(,)(var7,var1)) */
      // this=var7        underAC=false        Instantiated=false
      // this=var0        underAC=false        Instantiated=false
      sv[7] = fun_453( sv[3],v9 );
      // this=var7        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      sv[8] = fun_453( sv[3],v13 );
      // this=var5        underAC=false        Instantiated=false
      sv[9] = fun_368( sv[7],sv[8],sv[0] );
      // this=var7        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      sv[7] = fun_453( sv[3],v10 );
      GmakeAppl2(sv[8],code_423      ,sv[9]      ,sv[7]      );
      tmp = sv[6] = sv[8];
      /* rhs: var4 */
      // this=var4        underAC=false        Instantiated=false
      res = sv[6] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab44;
      myend308:;
    }
    if(bitSet_get(mask,1)) {
      Gterm *tmp, *sv[11];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: internal_deduce(,)(->(var0,var1),->(var2,var3)) */
      /* allDetEvaluation: nonDet */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend309;
      }
      /* where var5 := (chooseOccurence:list[int]/termCommons[Ops,Vars]) nvocc()(var0) */
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl1(sv[1],code_366      ,v3      );
      sv[0] = strTab[242]( sv[1] );
      /* if neq_list[int](,)(var5,nil) */
      // this=var5        underAC=false        Instantiated=false
      sv[3] = fun_19( sv[0],con_345 );
      if( sv[3] != con_1 ) {
        fail();
      }
      /* where var6 := (unifys:eqSystem/syntacticUnification[Ops,Vars]) (&)((true),(=(at(var0,var5),var2))) */
      GmakeAppl1(sv[5],code_424      ,con_1      );
      // this=var0        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      sv[4] = fun_367( v3,sv[0] );
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl2(sv[6],code_423      ,sv[4]      ,v6      );
      GmakeAppl1(sv[4],code_425      ,sv[6]      );
      GmakeAppl2(sv[6],code_426      ,sv[5]      ,sv[4]      );
      sv[2] = strTab[203]( sv[6] );
      /* where var7 := system_to_subst()(var6) */
      // this=var6        underAC=false        Instantiated=false
      sv[5] = fun_427( sv[2] );
      tmp = sv[4] = sv[5];
      /* where var4 := =([]at(apply(,)(var7,var0),apply(,)(var7,var3),var5),apply(,)(var7,var1)) */
      // this=var7        underAC=false        Instantiated=false
      // this=var0        underAC=false        Instantiated=false
      sv[8] = fun_453( sv[4],v3 );
      // this=var7        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      sv[9] = fun_453( sv[4],v7 );
      // this=var5        underAC=false        Instantiated=false
      sv[10] = fun_368( sv[8],sv[9],sv[0] );
      // this=var7        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      sv[8] = fun_453( sv[4],v4 );
      GmakeAppl2(sv[9],code_423      ,sv[10]      ,sv[8]      );
      tmp = sv[7] = sv[9];
      /* rhs: var4 */
      // this=var4        underAC=false        Instantiated=false
      res = sv[7] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab44;
      myend309:;
    }
  }
  fail();
  stratLab44:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_100( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* dk[nonDet](rewriteStep:term/rewriteStep[Vars,Ops,Prec]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_459: /* rewriteStep(,) */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1149:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        case code_429: /* -> */
          v4= GgetArgument(v3,0);
          switch(GgetSymb(v4)) {
          default:
          label1151:
            v5= GgetArgument(v3,1);
            switch(GgetSymb(v5)) {
            default:
            label1152:
              bitSet_set(mask,0);
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[8];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: rewriteStep(,)(var0,->(var1,var2)) */
      /* allDetEvaluation: nonDet */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend310;
      }
      /* where var4 := (chooseOccurence:list[int]/termCommons[Ops,Vars]) nvocc()(var0) */
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl1(sv[1],code_366      ,v2      );
      sv[0] = strTab[242]( sv[1] );
      /* where var5 := (matchs:eqSystem/syntacticUnification[Ops,Vars]) (&)((true),(=(var1,at(var0,var4)))) */
      GmakeAppl1(sv[4],code_424      ,con_1      );
      // this=var1        underAC=false        Instantiated=false
      // this=var0        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      sv[3] = fun_367( v2,sv[0] );
      GmakeAppl2(sv[5],code_423      ,v4      ,sv[3]      );
      GmakeAppl1(sv[3],code_425      ,sv[5]      );
      GmakeAppl2(sv[5],code_426      ,sv[4]      ,sv[3]      );
      sv[2] = strTab[173]( sv[5] );
      /* where var3 := system_to_subst()(var5) */
      // this=var5        underAC=false        Instantiated=false
      sv[4] = fun_427( sv[2] );
      tmp = sv[3] = sv[4];
      /* rhs: []at(var0,apply(,)(var3,var2),var4) */
      // this=var0        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      sv[6] = fun_453( sv[3],v5 );
      // this=var4        underAC=false        Instantiated=false
      sv[7] = fun_368( v2,sv[6],sv[0] );
      res = sv[7] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab111;
      myend310:;
    }
  }
  fail();
  stratLab111:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_101( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* dk[nonDet](rewriteStep:term/rewriteStep[Vars,Ops,Prec]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_459: /* rewriteStep(,) */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1155:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        case code_429: /* -> */
          v4= GgetArgument(v3,0);
          switch(GgetSymb(v4)) {
          default:
          label1157:
            v5= GgetArgument(v3,1);
            switch(GgetSymb(v5)) {
            default:
            label1158:
              bitSet_set(mask,0);
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[8];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: rewriteStep(,)(var0,->(var1,var2)) */
      /* allDetEvaluation: nonDet */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend311;
      }
      /* where var4 := (chooseOccurence:list[int]/termCommons[Ops,Vars]) nvocc()(var0) */
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl1(sv[1],code_366      ,v2      );
      sv[0] = strTab[242]( sv[1] );
      /* where var5 := (matchs:eqSystem/syntacticUnification[Ops,Vars]) (&)((true),(=(var1,at(var0,var4)))) */
      GmakeAppl1(sv[4],code_424      ,con_1      );
      // this=var1        underAC=false        Instantiated=false
      // this=var0        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      sv[3] = fun_367( v2,sv[0] );
      GmakeAppl2(sv[5],code_423      ,v4      ,sv[3]      );
      GmakeAppl1(sv[3],code_425      ,sv[5]      );
      GmakeAppl2(sv[5],code_426      ,sv[4]      ,sv[3]      );
      sv[2] = strTab[173]( sv[5] );
      /* where var3 := system_to_subst()(var5) */
      // this=var5        underAC=false        Instantiated=false
      sv[4] = fun_427( sv[2] );
      tmp = sv[3] = sv[4];
      /* rhs: []at(var0,apply(,)(var3,var2),var4) */
      // this=var0        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      sv[6] = fun_453( sv[3],v5 );
      // this=var4        underAC=false        Instantiated=false
      sv[7] = fun_368( v2,sv[6],sv[0] );
      res = sv[7] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab113;
      myend311:;
    }
  }
  fail();
  stratLab113:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_102( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* dk[nonDet](rewriteStep:term/rewriteStep[Vars,Ops,Prec]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_459: /* rewriteStep(,) */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1161:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        case code_429: /* -> */
          v4= GgetArgument(v3,0);
          switch(GgetSymb(v4)) {
          default:
          label1163:
            v5= GgetArgument(v3,1);
            switch(GgetSymb(v5)) {
            default:
            label1164:
              bitSet_set(mask,0);
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[8];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: rewriteStep(,)(var0,->(var1,var2)) */
      /* allDetEvaluation: nonDet */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend312;
      }
      /* where var4 := (chooseOccurence:list[int]/termCommons[Ops,Vars]) nvocc()(var0) */
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl1(sv[1],code_366      ,v2      );
      sv[0] = strTab[242]( sv[1] );
      /* where var5 := (matchs:eqSystem/syntacticUnification[Ops,Vars]) (&)((true),(=(var1,at(var0,var4)))) */
      GmakeAppl1(sv[4],code_424      ,con_1      );
      // this=var1        underAC=false        Instantiated=false
      // this=var0        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      sv[3] = fun_367( v2,sv[0] );
      GmakeAppl2(sv[5],code_423      ,v4      ,sv[3]      );
      GmakeAppl1(sv[3],code_425      ,sv[5]      );
      GmakeAppl2(sv[5],code_426      ,sv[4]      ,sv[3]      );
      sv[2] = strTab[173]( sv[5] );
      /* where var3 := system_to_subst()(var5) */
      // this=var5        underAC=false        Instantiated=false
      sv[4] = fun_427( sv[2] );
      tmp = sv[3] = sv[4];
      /* rhs: []at(var0,apply(,)(var3,var2),var4) */
      // this=var0        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      sv[6] = fun_453( sv[3],v5 );
      // this=var4        underAC=false        Instantiated=false
      sv[7] = fun_368( v2,sv[6],sv[0] );
      res = sv[7] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab115;
      myend312:;
    }
  }
  fail();
  stratLab115:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_10( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](delete_E:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_461: /* ***** */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1167:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        default:
        label1168:
          v4= GgetArgument(v0,2);
          switch(GgetSymb(v4)) {
          default:
          label1169:
            v5= GgetArgument(v0,3);
            switch(GgetSymb(v5)) {
            default:
            label1170:
              v6= GgetArgument(v0,4);
              switch(GgetSymb(v6)) {
              default:
              label1171:
                v7= GgetArgument(v0,5);
                switch(GgetSymb(v7)) {
                default:
                label1172:
                  bitSet_set(mask,0);
                }
              }
            }
          }
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[3];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,var2,var3,var4,var5) */
      /* allDetEvaluation: semiDet */
      CUTOPEN(); /* Wheres */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend313;
      }
      /* where var6 := (WHERE10:list[equation]/ans_completion[Vars,Ops,Prec,System]) delete()(var5) */
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl1(sv[1],code_488      ,v7      );
      sv[0] = strTab[151]( sv[1] );
      CUTCLOSE(); /* Wheres */
      /* rhs: *****(var0,var1,var2,var3,var4,var6) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var6        underAC=false        Instantiated=false
      GmakeAppl6(sv[2],code_461      ,v2      ,v3      ,v4      ,v5      ,v6      ,sv[0]      );
      res = sv[2] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab10;
      myend313:;
      CUTCLOSE(); /* Wheres */
    }
  }
  fail();
  stratLab10:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_152( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](delete:list[equation]/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7;
    bitSet_GC_create(mask,3);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_488: /* delete() */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_445: /* cons_equation(,) */
        v3= GgetArgument(v2,0);
        switch(GgetSymb(v3)) {
        case code_423: /* = */
          v4= GgetArgument(v3,0);
          switch(GgetSymb(v4)) {
          default:
          label1178:
            v5= GgetArgument(v3,1);
            switch(GgetSymb(v5)) {
            default:
            label1179:
              v6= GgetArgument(v2,1);
              switch(GgetSymb(v6)) {
              default:
              label1180:
                bitSet_set(mask,1);
                bitSet_set(mask,2);
              }
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
        break;
      case code_444: /* nil */
        bitSet_set(mask,0);
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[1];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: delete()(nil) */
      /* allDetEvaluation: det */
      /* rhs: nil */
      res = con_444 ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab128;
      myend314:;
    }
    if(bitSet_get(mask,1)) {
      Gterm *tmp, *sv[3];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: delete()(cons_equation(,)(=(var0,var1),var2)) */
      /* allDetEvaluation: semiDet */
      CUTOPEN(); /* Wheres */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend315;
      }
      /* if eq_list[int](,)(var0,var1) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      sv[0] = fun_18( v4,v5 );
      if( sv[0] != con_1 ) {
        fail();
      }
      /* where var3 := (WHERE20:list[equation]/ans_completion[Vars,Ops,Prec,System]) delete()(var2) */
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl1(sv[2],code_488      ,v6      );
      sv[1] = strTab[152]( sv[2] );
      CUTCLOSE(); /* Wheres */
      /* rhs: var3 */
      // this=var3        underAC=false        Instantiated=false
      res = sv[1] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab128;
      myend315:;
      CUTCLOSE(); /* Wheres */
    }
    if(bitSet_get(mask,2)) {
      Gterm *tmp, *sv[5];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: delete()(cons_equation(,)(=(var0,var1),var2)) */
      /* allDetEvaluation: semiDet */
      CUTOPEN(); /* Wheres */
      /* if neq_list[int](,)(var0,var1) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      sv[0] = fun_19( v4,v5 );
      if( sv[0] != con_1 ) {
        fail();
      }
      /* where var3 := (WHERE21:list[equation]/ans_completion[Vars,Ops,Prec,System]) delete()(var2) */
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl1(sv[2],code_488      ,v6      );
      sv[1] = strTab[153]( sv[2] );
      CUTCLOSE(); /* Wheres */
      /* rhs: cons_equation(,)(=(var0,var1),var3) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl2(sv[3],code_423      ,v4      ,v5      );
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl2(sv[4],code_445      ,sv[3]      ,sv[1]      );
      res = sv[4] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab128;
      myend316:;
      CUTCLOSE(); /* Wheres */
    }
  }
  fail();
  stratLab128:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_153( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](delete:list[equation]/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7;
    bitSet_GC_create(mask,3);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_488: /* delete() */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_445: /* cons_equation(,) */
        v3= GgetArgument(v2,0);
        switch(GgetSymb(v3)) {
        case code_423: /* = */
          v4= GgetArgument(v3,0);
          switch(GgetSymb(v4)) {
          default:
          label1186:
            v5= GgetArgument(v3,1);
            switch(GgetSymb(v5)) {
            default:
            label1187:
              v6= GgetArgument(v2,1);
              switch(GgetSymb(v6)) {
              default:
              label1188:
                bitSet_set(mask,1);
                bitSet_set(mask,2);
              }
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
        break;
      case code_444: /* nil */
        bitSet_set(mask,0);
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[1];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: delete()(nil) */
      /* allDetEvaluation: det */
      /* rhs: nil */
      res = con_444 ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab130;
      myend317:;
    }
    if(bitSet_get(mask,1)) {
      Gterm *tmp, *sv[3];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: delete()(cons_equation(,)(=(var0,var1),var2)) */
      /* allDetEvaluation: semiDet */
      CUTOPEN(); /* Wheres */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend318;
      }
      /* if eq_list[int](,)(var0,var1) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      sv[0] = fun_18( v4,v5 );
      if( sv[0] != con_1 ) {
        fail();
      }
      /* where var3 := (WHERE20:list[equation]/ans_completion[Vars,Ops,Prec,System]) delete()(var2) */
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl1(sv[2],code_488      ,v6      );
      sv[1] = strTab[152]( sv[2] );
      CUTCLOSE(); /* Wheres */
      /* rhs: var3 */
      // this=var3        underAC=false        Instantiated=false
      res = sv[1] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab130;
      myend318:;
      CUTCLOSE(); /* Wheres */
    }
    if(bitSet_get(mask,2)) {
      Gterm *tmp, *sv[5];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: delete()(cons_equation(,)(=(var0,var1),var2)) */
      /* allDetEvaluation: semiDet */
      CUTOPEN(); /* Wheres */
      /* if neq_list[int](,)(var0,var1) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      sv[0] = fun_19( v4,v5 );
      if( sv[0] != con_1 ) {
        fail();
      }
      /* where var3 := (WHERE21:list[equation]/ans_completion[Vars,Ops,Prec,System]) delete()(var2) */
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl1(sv[2],code_488      ,v6      );
      sv[1] = strTab[153]( sv[2] );
      CUTCLOSE(); /* Wheres */
      /* rhs: cons_equation(,)(=(var0,var1),var3) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl2(sv[3],code_423      ,v4      ,v5      );
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl2(sv[4],code_445      ,sv[3]      ,sv[1]      );
      res = sv[4] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab130;
      myend319:;
      CUTCLOSE(); /* Wheres */
    }
  }
  fail();
  stratLab130:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_11( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](orient_E:compute_state/ans_completion[Vars,Ops,Prec,System]!GL,orient_fail:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
    bitSet_GC_create(mask,3);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_461: /* ***** */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1191:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        default:
        label1192:
          v4= GgetArgument(v0,2);
          switch(GgetSymb(v4)) {
          default:
          label1193:
            v5= GgetArgument(v0,3);
            switch(GgetSymb(v5)) {
            default:
            label1194:
              v6= GgetArgument(v0,4);
              switch(GgetSymb(v6)) {
              case code_444: /* nil */
                v7= GgetArgument(v0,5);
                switch(GgetSymb(v7)) {
                case code_445: /* cons_equation(,) */
                  v8= GgetArgument(v7,0);
                  switch(GgetSymb(v8)) {
                  default:
                  label1198:
                    v9= GgetArgument(v7,1);
                    switch(GgetSymb(v9)) {
                    default:
                    label1199:
                      bitSet_set(mask,1);
                      bitSet_set(mask,2);
                    }
                  }
                  break;
                case code_444: /* nil */
                  bitSet_set(mask,0);
                  break;
                /* matching is not complete: jumpNode is null */
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
          }
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[3];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,var2,var3,nil,nil) */
      /* allDetEvaluation: det */
      /* rhs: *****(var0,var1,var2,var3,nil,nil) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl6(sv[2],code_461      ,v2      ,v3      ,v4      ,v5      ,con_444      ,con_444      );
      res = sv[2] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab13;
      myend320:;
    }
    if(bitSet_get(mask,1)) {
      Gterm *tmp, *sv[6];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,var2,var3,nil,cons_equation(,)(var4,var5)) */
      /* allDetEvaluation: semiDet */
      CUTOPEN(); /* Wheres */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend321;
      }
      /* where var6 := (s_choix_orient:list[equation]/ans_completion[Vars,Ops,Prec,System]) choix_orient(,)(nil,cons_equation(,)(var4,var5)) */
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_445      ,v8      ,v9      );
      GmakeAppl2(sv[3],code_494      ,con_444      ,sv[2]      );
      sv[0] = strTab[176]( sv[3] );
      /* if neq_list[int](,)(var6,nil) */
      // this=var6        underAC=false        Instantiated=false
      sv[2] = fun_19( sv[0],con_444 );
      if( sv[2] != con_1 ) {
        fail();
      }
      CUTCLOSE(); /* Wheres */
      /* rhs: *****(var0,var1,var2,var3,cons_equation(,)(normalize()(tete()(var6)),nil),queue()(var6)) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      // this=var6        underAC=false        Instantiated=false
      sv[1] = fun_476( sv[0] );
      sv[4] = fun_438( sv[1] );
      GmakeAppl2(sv[5],code_445      ,sv[4]      ,con_444      );
      // this=var6        underAC=false        Instantiated=false
      sv[1] = fun_477( sv[0] );
      GmakeAppl6(sv[4],code_461      ,v2      ,v3      ,v4      ,v5      ,sv[5]      ,sv[1]      );
      res = sv[4] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab13;
      myend321:;
      CUTCLOSE(); /* Wheres */
    }
    if(bitSet_get(mask,2)) {
      Gterm *tmp, *sv[7];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,var2,var3,nil,cons_equation(,)(var4,var5)) */
      /* allDetEvaluation: det */
      /* rhs: *****(nil,nil,nil,nil,nil,nil) */
      GmakeAppl6(sv[6],code_461      ,con_444      ,con_444      ,con_444      ,con_444      ,con_444      ,con_444      );
      res = sv[6] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab13;
      myend322:;
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

Gterm* str_12( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](deduce_NC:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_461: /* ***** */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1202:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        case code_445: /* cons_equation(,) */
          v4= GgetArgument(v3,0);
          switch(GgetSymb(v4)) {
          default:
          label1204:
            v5= GgetArgument(v3,1);
            switch(GgetSymb(v5)) {
            default:
            label1205:
              v6= GgetArgument(v0,2);
              switch(GgetSymb(v6)) {
              case code_445: /* cons_equation(,) */
                v7= GgetArgument(v6,0);
                switch(GgetSymb(v7)) {
                default:
                label1207:
                  v8= GgetArgument(v6,1);
                  switch(GgetSymb(v8)) {
                  case code_444: /* nil */
                    v9= GgetArgument(v0,3);
                    switch(GgetSymb(v9)) {
                    default:
                    label1209:
                      v10= GgetArgument(v0,4);
                      switch(GgetSymb(v10)) {
                      default:
                      label1210:
                        v11= GgetArgument(v0,5);
                        switch(GgetSymb(v11)) {
                        case code_444: /* nil */
                          bitSet_set(mask,0);
                          break;
                        /* matching is not complete: jumpNode is null */
                        }
                      }
                    }
                    break;
                  /* matching is not complete: jumpNode is null */
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[9];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,cons_equation(,)(var1,var2),cons_equation(,)(var3,nil),var4,var5,nil) */
      /* allDetEvaluation: det */
      /* where var7 := (s_deduce_list:internal_compute_state/ans_completion[Vars,Ops,Prec,System]) |||(nil,nil,cons_equation(,)(var1,nil),cons_equation(,)(var3,nil)) */
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl2(sv[4],code_445      ,v4      ,con_444      );
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl2(sv[5],code_445      ,v7      ,con_444      );
      GmakeAppl4(sv[3],code_462      ,con_444      ,con_444      ,sv[4]      ,sv[5]      );
      sv[0] = strTab[116]( sv[3] );
      /* where var8 := (s_deduce_list:internal_compute_state/ans_completion[Vars,Ops,Prec,System]) |||(ICS_to_1()(var7),nil,cons_equation(,)(var3,nil),cons_equation(,)(var1,nil)) */
      // this=var7        underAC=false        Instantiated=false
      sv[2] = fun_469( sv[0] );
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl2(sv[6],code_445      ,v7      ,con_444      );
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl2(sv[7],code_445      ,v4      ,con_444      );
      GmakeAppl4(sv[5],code_462      ,sv[2]      ,con_444      ,sv[6]      ,sv[7]      );
      sv[1] = strTab[116]( sv[5] );
      /* where var6 := ICS_to_1()(var8) */
      // this=var8        underAC=false        Instantiated=false
      sv[4] = fun_469( sv[1] );
      tmp = sv[2] = sv[4];
      /* rhs: *****(append_taille(,)(var0,var1),var2,cons_equation(,)(var3,nil),var4,var5,var6) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      sv[6] = fun_481( v2,v4 );
      // this=var2        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl2(sv[8],code_445      ,v7      ,con_444      );
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      // this=var6        underAC=false        Instantiated=false
      GmakeAppl6(sv[7],code_461      ,sv[6]      ,v5      ,sv[8]      ,v9      ,v10      ,sv[2]      );
      res = sv[7] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab15;
      myend323:;
    }
  }
  fail();
  stratLab15:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_13( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](internal_deduce:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_461: /* ***** */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1214:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        default:
        label1215:
          v4= GgetArgument(v0,2);
          switch(GgetSymb(v4)) {
          case code_445: /* cons_equation(,) */
            v5= GgetArgument(v4,0);
            switch(GgetSymb(v5)) {
            default:
            label1217:
              v6= GgetArgument(v4,1);
              switch(GgetSymb(v6)) {
              case code_444: /* nil */
                v7= GgetArgument(v0,3);
                switch(GgetSymb(v7)) {
                default:
                label1219:
                  v8= GgetArgument(v0,4);
                  switch(GgetSymb(v8)) {
                  default:
                  label1220:
                    v9= GgetArgument(v0,5);
                    switch(GgetSymb(v9)) {
                    case code_444: /* nil */
                      bitSet_set(mask,0);
                      break;
                    /* matching is not complete: jumpNode is null */
                    }
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
            break;
          /* matching is not complete: jumpNode is null */
          }
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[6];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,cons_equation(,)(var2,nil),var3,var4,nil) */
      /* allDetEvaluation: det */
      /* where var6 := (s_deduce_list:internal_compute_state/ans_completion[Vars,Ops,Prec,System]) |||(nil,nil,nil,cons_equation(,)(var2,nil)) */
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl2(sv[5],code_445      ,v5      ,con_444      );
      GmakeAppl4(sv[4],code_462      ,con_444      ,con_444      ,con_444      ,sv[5]      );
      sv[0] = strTab[116]( sv[4] );
      /* where var5 := ICS_to_1()(var6) */
      // this=var6        underAC=false        Instantiated=false
      sv[2] = fun_469( sv[0] );
      tmp = sv[1] = sv[2];
      /* rhs: *****(var0,var1,cons_equation(,)(var2,nil),var3,var4,var5) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl2(sv[5],code_445      ,v5      ,con_444      );
      // this=var3        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl6(sv[3],code_461      ,v2      ,v3      ,sv[5]      ,v7      ,v8      ,sv[1]      );
      res = sv[3] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab17;
      myend324:;
    }
  }
  fail();
  stratLab17:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_14( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](choix_C:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_461: /* ***** */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1224:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        default:
        label1225:
          v4= GgetArgument(v0,2);
          switch(GgetSymb(v4)) {
          default:
          label1226:
            v5= GgetArgument(v0,3);
            switch(GgetSymb(v5)) {
            case code_445: /* cons_equation(,) */
              v6= GgetArgument(v5,0);
              switch(GgetSymb(v6)) {
              default:
              label1228:
                v7= GgetArgument(v5,1);
                switch(GgetSymb(v7)) {
                default:
                label1229:
                  v8= GgetArgument(v0,4);
                  switch(GgetSymb(v8)) {
                  default:
                  label1230:
                    v9= GgetArgument(v0,5);
                    switch(GgetSymb(v9)) {
                    default:
                    label1231:
                      bitSet_set(mask,0);
                    }
                  }
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
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[6];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,var2,cons_equation(,)(var3,var4),var5,var6) */
      /* allDetEvaluation: det */
      /* rhs: *****(nil,append_taille(,)(var0,var1),cons_equation(,)(var3,nil),var4,nil,nil) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      sv[1] = fun_480( v2,v3 );
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl2(sv[3],code_445      ,v6      ,con_444      );
      // this=var4        underAC=false        Instantiated=false
      GmakeAppl6(sv[5],code_461      ,con_444      ,sv[1]      ,sv[3]      ,v7      ,con_444      ,con_444      );
      res = sv[5] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab19;
      myend325:;
    }
  }
  fail();
  stratLab19:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_15( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](delete_E:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_461: /* ***** */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1234:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        default:
        label1235:
          v4= GgetArgument(v0,2);
          switch(GgetSymb(v4)) {
          default:
          label1236:
            v5= GgetArgument(v0,3);
            switch(GgetSymb(v5)) {
            default:
            label1237:
              v6= GgetArgument(v0,4);
              switch(GgetSymb(v6)) {
              default:
              label1238:
                v7= GgetArgument(v0,5);
                switch(GgetSymb(v7)) {
                default:
                label1239:
                  bitSet_set(mask,0);
                }
              }
            }
          }
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[3];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,var2,var3,var4,var5) */
      /* allDetEvaluation: semiDet */
      CUTOPEN(); /* Wheres */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend326;
      }
      /* where var6 := (WHERE10:list[equation]/ans_completion[Vars,Ops,Prec,System]) delete()(var5) */
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl1(sv[1],code_488      ,v7      );
      sv[0] = strTab[151]( sv[1] );
      CUTCLOSE(); /* Wheres */
      /* rhs: *****(var0,var1,var2,var3,var4,var6) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var6        underAC=false        Instantiated=false
      GmakeAppl6(sv[2],code_461      ,v2      ,v3      ,v4      ,v5      ,v6      ,sv[0]      );
      res = sv[2] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab21;
      myend326:;
      CUTCLOSE(); /* Wheres */
    }
  }
  fail();
  stratLab21:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_16( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](orient_E:compute_state/ans_completion[Vars,Ops,Prec,System]!GL,orient_fail:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
    bitSet_GC_create(mask,3);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_461: /* ***** */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1242:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        default:
        label1243:
          v4= GgetArgument(v0,2);
          switch(GgetSymb(v4)) {
          default:
          label1244:
            v5= GgetArgument(v0,3);
            switch(GgetSymb(v5)) {
            default:
            label1245:
              v6= GgetArgument(v0,4);
              switch(GgetSymb(v6)) {
              case code_444: /* nil */
                v7= GgetArgument(v0,5);
                switch(GgetSymb(v7)) {
                case code_445: /* cons_equation(,) */
                  v8= GgetArgument(v7,0);
                  switch(GgetSymb(v8)) {
                  default:
                  label1249:
                    v9= GgetArgument(v7,1);
                    switch(GgetSymb(v9)) {
                    default:
                    label1250:
                      bitSet_set(mask,1);
                      bitSet_set(mask,2);
                    }
                  }
                  break;
                case code_444: /* nil */
                  bitSet_set(mask,0);
                  break;
                /* matching is not complete: jumpNode is null */
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
          }
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[3];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,var2,var3,nil,nil) */
      /* allDetEvaluation: det */
      /* rhs: *****(var0,var1,var2,var3,nil,nil) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl6(sv[2],code_461      ,v2      ,v3      ,v4      ,v5      ,con_444      ,con_444      );
      res = sv[2] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab24;
      myend327:;
    }
    if(bitSet_get(mask,1)) {
      Gterm *tmp, *sv[6];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,var2,var3,nil,cons_equation(,)(var4,var5)) */
      /* allDetEvaluation: semiDet */
      CUTOPEN(); /* Wheres */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend328;
      }
      /* where var6 := (s_choix_orient:list[equation]/ans_completion[Vars,Ops,Prec,System]) choix_orient(,)(nil,cons_equation(,)(var4,var5)) */
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_445      ,v8      ,v9      );
      GmakeAppl2(sv[3],code_494      ,con_444      ,sv[2]      );
      sv[0] = strTab[176]( sv[3] );
      /* if neq_list[int](,)(var6,nil) */
      // this=var6        underAC=false        Instantiated=false
      sv[2] = fun_19( sv[0],con_444 );
      if( sv[2] != con_1 ) {
        fail();
      }
      CUTCLOSE(); /* Wheres */
      /* rhs: *****(var0,var1,var2,var3,cons_equation(,)(normalize()(tete()(var6)),nil),queue()(var6)) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      // this=var6        underAC=false        Instantiated=false
      sv[1] = fun_476( sv[0] );
      sv[4] = fun_438( sv[1] );
      GmakeAppl2(sv[5],code_445      ,sv[4]      ,con_444      );
      // this=var6        underAC=false        Instantiated=false
      sv[1] = fun_477( sv[0] );
      GmakeAppl6(sv[4],code_461      ,v2      ,v3      ,v4      ,v5      ,sv[5]      ,sv[1]      );
      res = sv[4] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab24;
      myend328:;
      CUTCLOSE(); /* Wheres */
    }
    if(bitSet_get(mask,2)) {
      Gterm *tmp, *sv[7];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,var2,var3,nil,cons_equation(,)(var4,var5)) */
      /* allDetEvaluation: det */
      /* rhs: *****(nil,nil,nil,nil,nil,nil) */
      GmakeAppl6(sv[6],code_461      ,con_444      ,con_444      ,con_444      ,con_444      ,con_444      ,con_444      );
      res = sv[6] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab24;
      myend329:;
    }
  }
  fail();
  stratLab24:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_17( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](deduce_NC:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_461: /* ***** */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1253:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        case code_445: /* cons_equation(,) */
          v4= GgetArgument(v3,0);
          switch(GgetSymb(v4)) {
          default:
          label1255:
            v5= GgetArgument(v3,1);
            switch(GgetSymb(v5)) {
            default:
            label1256:
              v6= GgetArgument(v0,2);
              switch(GgetSymb(v6)) {
              case code_445: /* cons_equation(,) */
                v7= GgetArgument(v6,0);
                switch(GgetSymb(v7)) {
                default:
                label1258:
                  v8= GgetArgument(v6,1);
                  switch(GgetSymb(v8)) {
                  case code_444: /* nil */
                    v9= GgetArgument(v0,3);
                    switch(GgetSymb(v9)) {
                    default:
                    label1260:
                      v10= GgetArgument(v0,4);
                      switch(GgetSymb(v10)) {
                      default:
                      label1261:
                        v11= GgetArgument(v0,5);
                        switch(GgetSymb(v11)) {
                        case code_444: /* nil */
                          bitSet_set(mask,0);
                          break;
                        /* matching is not complete: jumpNode is null */
                        }
                      }
                    }
                    break;
                  /* matching is not complete: jumpNode is null */
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[9];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,cons_equation(,)(var1,var2),cons_equation(,)(var3,nil),var4,var5,nil) */
      /* allDetEvaluation: det */
      /* where var7 := (s_deduce_list:internal_compute_state/ans_completion[Vars,Ops,Prec,System]) |||(nil,nil,cons_equation(,)(var1,nil),cons_equation(,)(var3,nil)) */
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl2(sv[4],code_445      ,v4      ,con_444      );
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl2(sv[5],code_445      ,v7      ,con_444      );
      GmakeAppl4(sv[3],code_462      ,con_444      ,con_444      ,sv[4]      ,sv[5]      );
      sv[0] = strTab[116]( sv[3] );
      /* where var8 := (s_deduce_list:internal_compute_state/ans_completion[Vars,Ops,Prec,System]) |||(ICS_to_1()(var7),nil,cons_equation(,)(var3,nil),cons_equation(,)(var1,nil)) */
      // this=var7        underAC=false        Instantiated=false
      sv[2] = fun_469( sv[0] );
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl2(sv[6],code_445      ,v7      ,con_444      );
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl2(sv[7],code_445      ,v4      ,con_444      );
      GmakeAppl4(sv[5],code_462      ,sv[2]      ,con_444      ,sv[6]      ,sv[7]      );
      sv[1] = strTab[116]( sv[5] );
      /* where var6 := ICS_to_1()(var8) */
      // this=var8        underAC=false        Instantiated=false
      sv[4] = fun_469( sv[1] );
      tmp = sv[2] = sv[4];
      /* rhs: *****(append_taille(,)(var0,var1),var2,cons_equation(,)(var3,nil),var4,var5,var6) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      sv[6] = fun_481( v2,v4 );
      // this=var2        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl2(sv[8],code_445      ,v7      ,con_444      );
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      // this=var6        underAC=false        Instantiated=false
      GmakeAppl6(sv[7],code_461      ,sv[6]      ,v5      ,sv[8]      ,v9      ,v10      ,sv[2]      );
      res = sv[7] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab26;
      myend330:;
    }
  }
  fail();
  stratLab26:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_18( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](internal_deduce:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_461: /* ***** */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1265:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        default:
        label1266:
          v4= GgetArgument(v0,2);
          switch(GgetSymb(v4)) {
          case code_445: /* cons_equation(,) */
            v5= GgetArgument(v4,0);
            switch(GgetSymb(v5)) {
            default:
            label1268:
              v6= GgetArgument(v4,1);
              switch(GgetSymb(v6)) {
              case code_444: /* nil */
                v7= GgetArgument(v0,3);
                switch(GgetSymb(v7)) {
                default:
                label1270:
                  v8= GgetArgument(v0,4);
                  switch(GgetSymb(v8)) {
                  default:
                  label1271:
                    v9= GgetArgument(v0,5);
                    switch(GgetSymb(v9)) {
                    case code_444: /* nil */
                      bitSet_set(mask,0);
                      break;
                    /* matching is not complete: jumpNode is null */
                    }
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
            break;
          /* matching is not complete: jumpNode is null */
          }
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[6];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,cons_equation(,)(var2,nil),var3,var4,nil) */
      /* allDetEvaluation: det */
      /* where var6 := (s_deduce_list:internal_compute_state/ans_completion[Vars,Ops,Prec,System]) |||(nil,nil,nil,cons_equation(,)(var2,nil)) */
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl2(sv[5],code_445      ,v5      ,con_444      );
      GmakeAppl4(sv[4],code_462      ,con_444      ,con_444      ,con_444      ,sv[5]      );
      sv[0] = strTab[116]( sv[4] );
      /* where var5 := ICS_to_1()(var6) */
      // this=var6        underAC=false        Instantiated=false
      sv[2] = fun_469( sv[0] );
      tmp = sv[1] = sv[2];
      /* rhs: *****(var0,var1,cons_equation(,)(var2,nil),var3,var4,var5) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl2(sv[5],code_445      ,v5      ,con_444      );
      // this=var3        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl6(sv[3],code_461      ,v2      ,v3      ,sv[5]      ,v7      ,v8      ,sv[1]      );
      res = sv[3] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab28;
      myend331:;
    }
  }
  fail();
  stratLab28:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_242( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* dk[nonDet](topOccRule:list[int]/termCommons[Ops,Vars]!LO,inOccRule:list[int]/termCommons[Ops,Vars]!LO) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
    bitSet_GC_create(mask,8);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_366: /* nvocc() */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_380: /* i5() */
        v3= GgetArgument(v2,0);
        switch(GgetSymb(v3)) {
        default:
        label1288:
          bitSet_set(mask,0);
          bitSet_set(mask,7);
        }
        break;
      case code_379: /* i4() */
        v4= GgetArgument(v2,0);
        switch(GgetSymb(v4)) {
        default:
        label1286:
          bitSet_set(mask,0);
          bitSet_set(mask,6);
        }
        break;
      case code_378: /* i3() */
        v5= GgetArgument(v2,0);
        switch(GgetSymb(v5)) {
        default:
        label1284:
          bitSet_set(mask,0);
          bitSet_set(mask,5);
        }
        break;
      case code_377: /* i2() */
        v6= GgetArgument(v2,0);
        switch(GgetSymb(v6)) {
        default:
        label1282:
          bitSet_set(mask,0);
          bitSet_set(mask,4);
        }
        break;
      case code_376: /* i1() */
        v7= GgetArgument(v2,0);
        switch(GgetSymb(v7)) {
        default:
        label1280:
          bitSet_set(mask,0);
          bitSet_set(mask,3);
        }
        break;
      case code_375: /* f(,) */
        v8= GgetArgument(v2,0);
        switch(GgetSymb(v8)) {
        default:
        label1277:
          v9= GgetArgument(v2,1);
          switch(GgetSymb(v9)) {
          default:
          label1278:
            bitSet_set(mask,0);
            bitSet_set(mask,1);
            bitSet_set(mask,2);
          }
        }
        break;
      default:
      label1275:
        bitSet_set(mask,0);
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[2];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: nvocc()(var0) */
      /* allDetEvaluation: det */
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend332;
      }
      /* if (not())(isvar()(var0)) */
      // this=var0        underAC=false        Instantiated=false
      sv[0] = fun_365( v2 );
      sv[1] = fun_24( sv[0] );
      if( sv[1] != con_1 ) {
        fail();
      }
      /* rhs: nil */
      res = con_345 ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab187;
      myend332:;
    }
    if(bitSet_get(mask,1)) {
      Gterm *tmp, *sv[4];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: nvocc()(f(,)(var0,var1)) */
      /* allDetEvaluation: nonDet */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend333;
      }
      /* where var2 := (chooseOccurence:list[int]/termCommons[Ops,Vars]) nvocc()(var0) */
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl1(sv[1],code_366      ,v8      );
      sv[0] = strTab[242]( sv[1] );
      /* rhs: cons_int(,)((1),var2) */
      GmakeAppl1(sv[3],code_301      ,(GsetIntegerTag(1))      );
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_346      ,sv[3]      ,sv[0]      );
      res = sv[2] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab187;
      myend333:;
    }
    if(bitSet_get(mask,2)) {
      Gterm *tmp, *sv[4];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: nvocc()(f(,)(var0,var1)) */
      /* allDetEvaluation: nonDet */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend334;
      }
      /* where var2 := (chooseOccurence:list[int]/termCommons[Ops,Vars]) nvocc()(var1) */
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl1(sv[1],code_366      ,v9      );
      sv[0] = strTab[242]( sv[1] );
      /* rhs: cons_int(,)((2),var2) */
      GmakeAppl1(sv[3],code_301      ,(GsetIntegerTag(2))      );
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_346      ,sv[3]      ,sv[0]      );
      res = sv[2] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab187;
      myend334:;
    }
    if(bitSet_get(mask,3)) {
      Gterm *tmp, *sv[4];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: nvocc()(i1()(var0)) */
      /* allDetEvaluation: nonDet */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend335;
      }
      /* where var1 := (chooseOccurence:list[int]/termCommons[Ops,Vars]) nvocc()(var0) */
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl1(sv[1],code_366      ,v7      );
      sv[0] = strTab[242]( sv[1] );
      /* rhs: cons_int(,)((1),var1) */
      GmakeAppl1(sv[3],code_301      ,(GsetIntegerTag(1))      );
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_346      ,sv[3]      ,sv[0]      );
      res = sv[2] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab187;
      myend335:;
    }
    if(bitSet_get(mask,4)) {
      Gterm *tmp, *sv[4];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: nvocc()(i2()(var0)) */
      /* allDetEvaluation: nonDet */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend336;
      }
      /* where var1 := (chooseOccurence:list[int]/termCommons[Ops,Vars]) nvocc()(var0) */
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl1(sv[1],code_366      ,v6      );
      sv[0] = strTab[242]( sv[1] );
      /* rhs: cons_int(,)((1),var1) */
      GmakeAppl1(sv[3],code_301      ,(GsetIntegerTag(1))      );
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_346      ,sv[3]      ,sv[0]      );
      res = sv[2] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab187;
      myend336:;
    }
    if(bitSet_get(mask,5)) {
      Gterm *tmp, *sv[4];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: nvocc()(i3()(var0)) */
      /* allDetEvaluation: nonDet */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend337;
      }
      /* where var1 := (chooseOccurence:list[int]/termCommons[Ops,Vars]) nvocc()(var0) */
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl1(sv[1],code_366      ,v5      );
      sv[0] = strTab[242]( sv[1] );
      /* rhs: cons_int(,)((1),var1) */
      GmakeAppl1(sv[3],code_301      ,(GsetIntegerTag(1))      );
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_346      ,sv[3]      ,sv[0]      );
      res = sv[2] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab187;
      myend337:;
    }
    if(bitSet_get(mask,6)) {
      Gterm *tmp, *sv[4];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: nvocc()(i4()(var0)) */
      /* allDetEvaluation: nonDet */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend338;
      }
      /* where var1 := (chooseOccurence:list[int]/termCommons[Ops,Vars]) nvocc()(var0) */
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl1(sv[1],code_366      ,v4      );
      sv[0] = strTab[242]( sv[1] );
      /* rhs: cons_int(,)((1),var1) */
      GmakeAppl1(sv[3],code_301      ,(GsetIntegerTag(1))      );
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_346      ,sv[3]      ,sv[0]      );
      res = sv[2] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab187;
      myend338:;
    }
    if(bitSet_get(mask,7)) {
      Gterm *tmp, *sv[4];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: nvocc()(i5()(var0)) */
      /* allDetEvaluation: nonDet */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend339;
      }
      /* where var1 := (chooseOccurence:list[int]/termCommons[Ops,Vars]) nvocc()(var0) */
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl1(sv[1],code_366      ,v3      );
      sv[0] = strTab[242]( sv[1] );
      /* rhs: cons_int(,)((1),var1) */
      GmakeAppl1(sv[3],code_301      ,(GsetIntegerTag(1))      );
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_346      ,sv[3]      ,sv[0]      );
      res = sv[2] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab187;
      myend339:;
    }
  }
  fail();
  stratLab187:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_470( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* repeat[det](one[semiDet](Regle1bis:compute_state/ans_completion[Vars,Ops,Prec,System]!GL,Regle2bis:compute_state/ans_completion[Vars,Ops,Prec,System]!GL,Regle3bis:compute_state/ans_completion[Vars,Ops,Prec,System]!GL,Regle4bis:compute_state/ans_completion[Vars,Ops,Prec,System]!GL,Regle5bis:compute_state/ans_completion[Vars,Ops,Prec,System]!GL)) */
    long lastTerm_index;
    CUTOPEN(); /* Repeat */
    lastTerm_index = allocStable(sizeof(Gterm*));
    *((Gterm**)getStablePointer(lastTerm_index))=v0;
    if(localSetChoicePoint()!=0) {
      res = v0 = *((Gterm**)getStablePointer(lastTerm_index));
      /* End of repeat */
    } else {
      while(1) {
      /* Apply the strategy */
        {
          /* one[semiDet](Regle1bis:compute_state/ans_completion[Vars,Ops,Prec,System]!GL,Regle2bis:compute_state/ans_completion[Vars,Ops,Prec,System]!GL,Regle3bis:compute_state/ans_completion[Vars,Ops,Prec,System]!GL,Regle4bis:compute_state/ans_completion[Vars,Ops,Prec,System]!GL,Regle5bis:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
          Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16,*v17,*v18,*v19,*v20,*v21,*v22,*v23,*v24,*v25,*v26,*v27,*v28,*v29,*v30,*v31,*v32,*v33,*v34,*v35,*v36,*v37,*v38,*v39,*v40,*v41,*v42,*v43,*v44,*v45,*v46,*v47,*v48,*v49,*v50,*v51,*v52,*v53,*v54,*v55,*v56,*v57,*v58,*v59,*v60,*v61,*v62,*v63,*v64,*v65,*v66,*v67,*v68,*v69,*v70,*v71,*v72,*v73,*v74,*v75,*v76,*v77,*v78,*v79,*v80,*v81,*v82,*v83,*v84,*v85,*v86,*v87,*v88,*v89,*v90,*v91,*v92,*v93,*v94,*v95,*v96,*v97,*v98,*v99,*v100,*v101,*v102,*v103,*v104;
          bitSet_GC_create(mask,5);
          bitSet_init_clear(mask);
          switch(GgetSymb(v0)) {
          case code_461: /* ***** */
            v2= GgetArgument(v0,0);
            switch(GgetSymb(v2)) {
            default:
            label1291:
              v3= GgetArgument(v0,1);
              switch(GgetSymb(v3)) {
              case code_444: /* nil */
                v4= GgetArgument(v0,2);
                switch(GgetSymb(v4)) {
                case code_444: /* nil */
                  v5= GgetArgument(v0,3);
                  switch(GgetSymb(v5)) {
                  case code_445: /* cons_equation(,) */
                    v6= GgetArgument(v5,0);
                    switch(GgetSymb(v6)) {
                    default:
                    label1386:
                      v7= GgetArgument(v5,1);
                      switch(GgetSymb(v7)) {
                      default:
                      label1387:
                        v8= GgetArgument(v0,4);
                        switch(GgetSymb(v8)) {
                        case code_444: /* nil */
                          v9= GgetArgument(v0,5);
                          switch(GgetSymb(v9)) {
                          case code_444: /* nil */
                            bitSet_set(mask,4);
                            break;
                          case code_445: /* cons_equation(,) */
                            v10= GgetArgument(v9,0);
                            switch(GgetSymb(v10)) {
                            default:
                            label1394:
                              v11= GgetArgument(v9,1);
                              switch(GgetSymb(v11)) {
                              default:
                              label1395:
                                bitSet_set(mask,1);
                              }
                            }
                            break;
                          /* matching is not complete: jumpNode is null */
                          }
                          break;
                        case code_445: /* cons_equation(,) */
                          v12= GgetArgument(v8,0);
                          switch(GgetSymb(v12)) {
                          default:
                          label1389:
                            v13= GgetArgument(v8,1);
                            switch(GgetSymb(v13)) {
                            default:
                            label1390:
                              v14= GgetArgument(v0,5);
                              switch(GgetSymb(v14)) {
                              default:
                              label1391:
                                bitSet_set(mask,0);
                              }
                            }
                          }
                          break;
                        /* matching is not complete: jumpNode is null */
                        }
                      }
                    }
                    break;
                  default:
                  label1376:
                    v8= GgetArgument(v0,4);
                    switch(GgetSymb(v8)) {
                    case code_444: /* nil */
                      v9= GgetArgument(v0,5);
                      switch(GgetSymb(v9)) {
                      case code_445: /* cons_equation(,) */
                        v10= GgetArgument(v9,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1383:
                          v11= GgetArgument(v9,1);
                          switch(GgetSymb(v11)) {
                          default:
                          label1384:
                            bitSet_set(mask,1);
                          }
                        }
                        break;
                      /* matching is not complete: jumpNode is null */
                      }
                      break;
                    case code_445: /* cons_equation(,) */
                      v12= GgetArgument(v8,0);
                      switch(GgetSymb(v12)) {
                      default:
                      label1378:
                        v13= GgetArgument(v8,1);
                        switch(GgetSymb(v13)) {
                        default:
                        label1379:
                          v14= GgetArgument(v0,5);
                          switch(GgetSymb(v14)) {
                          default:
                          label1380:
                            bitSet_set(mask,0);
                          }
                        }
                      }
                      break;
                    /* matching is not complete: jumpNode is null */
                    }
                  }
                  break;
                case code_445: /* cons_equation(,) */
                  v22= GgetArgument(v4,0);
                  switch(GgetSymb(v22)) {
                  default:
                  label1341:
                    v23= GgetArgument(v4,1);
                    switch(GgetSymb(v23)) {
                    case code_444: /* nil */
                      v5= GgetArgument(v0,3);
                      v24= GgetArgument(v0,3);
                      switch(GgetSymb(v24)) {
                      default:
                      label1343:
                        v8= GgetArgument(v0,4);
                        v25= GgetArgument(v0,4);
                        switch(GgetSymb(v25)) {
                        case code_444: /* nil */
                          v9= GgetArgument(v0,5);
                          v26= GgetArgument(v0,5);
                          switch(GgetSymb(v26)) {
                          case code_444: /* nil */
                            bitSet_set(mask,3);
                            break;
                          case code_445: /* cons_equation(,) */
                            v10= GgetArgument(v9,0);
                            switch(GgetSymb(v10)) {
                            default:
                            label1350:
                              v11= GgetArgument(v9,1);
                              switch(GgetSymb(v11)) {
                              default:
                              label1351:
                                bitSet_set(mask,1);
                              }
                            }
                            break;
                          /* matching is not complete: jumpNode is null */
                          }
                          break;
                        case code_445: /* cons_equation(,) */
                          v12= GgetArgument(v8,0);
                          switch(GgetSymb(v12)) {
                          default:
                          label1345:
                            v13= GgetArgument(v8,1);
                            switch(GgetSymb(v13)) {
                            default:
                            label1346:
                              v14= GgetArgument(v0,5);
                              switch(GgetSymb(v14)) {
                              default:
                              label1347:
                                bitSet_set(mask,0);
                              }
                            }
                          }
                          break;
                        /* matching is not complete: jumpNode is null */
                        }
                      }
                      break;
                    default:
                      goto label1330;
                    }
                  }
                  break;
                default:
                label1330:
                  v5= GgetArgument(v0,3);
                  switch(GgetSymb(v5)) {
                  default:
                  label1331:
                    v8= GgetArgument(v0,4);
                    switch(GgetSymb(v8)) {
                    case code_444: /* nil */
                      v9= GgetArgument(v0,5);
                      switch(GgetSymb(v9)) {
                      case code_445: /* cons_equation(,) */
                        v10= GgetArgument(v9,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1338:
                          v11= GgetArgument(v9,1);
                          switch(GgetSymb(v11)) {
                          default:
                          label1339:
                            bitSet_set(mask,1);
                          }
                        }
                        break;
                      /* matching is not complete: jumpNode is null */
                      }
                      break;
                    case code_445: /* cons_equation(,) */
                      v12= GgetArgument(v8,0);
                      switch(GgetSymb(v12)) {
                      default:
                      label1333:
                        v13= GgetArgument(v8,1);
                        switch(GgetSymb(v13)) {
                        default:
                        label1334:
                          v14= GgetArgument(v0,5);
                          switch(GgetSymb(v14)) {
                          default:
                          label1335:
                            bitSet_set(mask,0);
                          }
                        }
                      }
                      break;
                    /* matching is not complete: jumpNode is null */
                    }
                  }
                }
                break;
              case code_445: /* cons_equation(,) */
                v40= GgetArgument(v3,0);
                switch(GgetSymb(v40)) {
                default:
                label1304:
                  v41= GgetArgument(v3,1);
                  switch(GgetSymb(v41)) {
                  default:
                  label1305:
                    v4= GgetArgument(v0,2);
                    v42= GgetArgument(v0,2);
                    switch(GgetSymb(v4)) {
                    case code_444: /* nil */
                      v5= GgetArgument(v0,3);
                      switch(GgetSymb(v5)) {
                      case code_445: /* cons_equation(,) */
                        v6= GgetArgument(v5,0);
                        switch(GgetSymb(v6)) {
                        default:
                        label1364:
                          v7= GgetArgument(v5,1);
                          switch(GgetSymb(v7)) {
                          default:
                          label1365:
                            v8= GgetArgument(v0,4);
                            switch(GgetSymb(v8)) {
                            case code_444: /* nil */
                              v9= GgetArgument(v0,5);
                              switch(GgetSymb(v9)) {
                              case code_444: /* nil */
                                bitSet_set(mask,4);
                                break;
                              case code_445: /* cons_equation(,) */
                                v10= GgetArgument(v9,0);
                                switch(GgetSymb(v10)) {
                                default:
                                label1372:
                                  v11= GgetArgument(v9,1);
                                  switch(GgetSymb(v11)) {
                                  default:
                                  label1373:
                                    bitSet_set(mask,1);
                                  }
                                }
                                break;
                              /* matching is not complete: jumpNode is null */
                              }
                              break;
                            case code_445: /* cons_equation(,) */
                              v12= GgetArgument(v8,0);
                              switch(GgetSymb(v12)) {
                              default:
                              label1367:
                                v13= GgetArgument(v8,1);
                                switch(GgetSymb(v13)) {
                                default:
                                label1368:
                                  v14= GgetArgument(v0,5);
                                  switch(GgetSymb(v14)) {
                                  default:
                                  label1369:
                                    bitSet_set(mask,0);
                                  }
                                }
                              }
                              break;
                            /* matching is not complete: jumpNode is null */
                            }
                          }
                        }
                        break;
                      default:
                      label1354:
                        v8= GgetArgument(v0,4);
                        switch(GgetSymb(v8)) {
                        case code_444: /* nil */
                          v9= GgetArgument(v0,5);
                          switch(GgetSymb(v9)) {
                          case code_445: /* cons_equation(,) */
                            v10= GgetArgument(v9,0);
                            switch(GgetSymb(v10)) {
                            default:
                            label1361:
                              v11= GgetArgument(v9,1);
                              switch(GgetSymb(v11)) {
                              default:
                              label1362:
                                bitSet_set(mask,1);
                              }
                            }
                            break;
                          /* matching is not complete: jumpNode is null */
                          }
                          break;
                        case code_445: /* cons_equation(,) */
                          v12= GgetArgument(v8,0);
                          switch(GgetSymb(v12)) {
                          default:
                          label1356:
                            v13= GgetArgument(v8,1);
                            switch(GgetSymb(v13)) {
                            default:
                            label1357:
                              v14= GgetArgument(v0,5);
                              switch(GgetSymb(v14)) {
                              default:
                              label1358:
                                bitSet_set(mask,0);
                              }
                            }
                          }
                          break;
                        /* matching is not complete: jumpNode is null */
                        }
                      }
                      break;
                    case code_445: /* cons_equation(,) */
                      v60= GgetArgument(v42,0);
                      switch(GgetSymb(v60)) {
                      default:
                      label1317:
                        v61= GgetArgument(v42,1);
                        switch(GgetSymb(v61)) {
                        case code_444: /* nil */
                          v5= GgetArgument(v0,3);
                          v62= GgetArgument(v0,3);
                          switch(GgetSymb(v62)) {
                          default:
                          label1319:
                            v8= GgetArgument(v0,4);
                            v63= GgetArgument(v0,4);
                            switch(GgetSymb(v63)) {
                            case code_444: /* nil */
                              v9= GgetArgument(v0,5);
                              v64= GgetArgument(v0,5);
                              switch(GgetSymb(v64)) {
                              case code_444: /* nil */
                                bitSet_set(mask,2);
                                break;
                              case code_445: /* cons_equation(,) */
                                v10= GgetArgument(v9,0);
                                switch(GgetSymb(v10)) {
                                default:
                                label1326:
                                  v11= GgetArgument(v9,1);
                                  switch(GgetSymb(v11)) {
                                  default:
                                  label1327:
                                    bitSet_set(mask,1);
                                  }
                                }
                                break;
                              /* matching is not complete: jumpNode is null */
                              }
                              break;
                            case code_445: /* cons_equation(,) */
                              v12= GgetArgument(v8,0);
                              switch(GgetSymb(v12)) {
                              default:
                              label1321:
                                v13= GgetArgument(v8,1);
                                switch(GgetSymb(v13)) {
                                default:
                                label1322:
                                  v14= GgetArgument(v0,5);
                                  switch(GgetSymb(v14)) {
                                  default:
                                  label1323:
                                    bitSet_set(mask,0);
                                  }
                                }
                              }
                              break;
                            /* matching is not complete: jumpNode is null */
                            }
                          }
                          break;
                        default:
                          goto label1306;
                        }
                      }
                      break;
                    default:
                    label1306:
                      v5= GgetArgument(v0,3);
                      switch(GgetSymb(v5)) {
                      default:
                      label1307:
                        v8= GgetArgument(v0,4);
                        switch(GgetSymb(v8)) {
                        case code_444: /* nil */
                          v9= GgetArgument(v0,5);
                          switch(GgetSymb(v9)) {
                          case code_445: /* cons_equation(,) */
                            v10= GgetArgument(v9,0);
                            switch(GgetSymb(v10)) {
                            default:
                            label1314:
                              v11= GgetArgument(v9,1);
                              switch(GgetSymb(v11)) {
                              default:
                              label1315:
                                bitSet_set(mask,1);
                              }
                            }
                            break;
                          /* matching is not complete: jumpNode is null */
                          }
                          break;
                        case code_445: /* cons_equation(,) */
                          v12= GgetArgument(v8,0);
                          switch(GgetSymb(v12)) {
                          default:
                          label1309:
                            v13= GgetArgument(v8,1);
                            switch(GgetSymb(v13)) {
                            default:
                            label1310:
                              v14= GgetArgument(v0,5);
                              switch(GgetSymb(v14)) {
                              default:
                              label1311:
                                bitSet_set(mask,0);
                              }
                            }
                          }
                          break;
                        /* matching is not complete: jumpNode is null */
                        }
                      }
                    }
                  }
                }
                break;
              default:
              label1292:
                v4= GgetArgument(v0,2);
                switch(GgetSymb(v4)) {
                case code_444: /* nil */
                  v5= GgetArgument(v0,3);
                  switch(GgetSymb(v5)) {
                  case code_445: /* cons_equation(,) */
                    v6= GgetArgument(v5,0);
                    switch(GgetSymb(v6)) {
                    default:
                    label1408:
                      v7= GgetArgument(v5,1);
                      switch(GgetSymb(v7)) {
                      default:
                      label1409:
                        v8= GgetArgument(v0,4);
                        switch(GgetSymb(v8)) {
                        case code_444: /* nil */
                          v9= GgetArgument(v0,5);
                          switch(GgetSymb(v9)) {
                          case code_444: /* nil */
                            bitSet_set(mask,4);
                            break;
                          case code_445: /* cons_equation(,) */
                            v10= GgetArgument(v9,0);
                            switch(GgetSymb(v10)) {
                            default:
                            label1416:
                              v11= GgetArgument(v9,1);
                              switch(GgetSymb(v11)) {
                              default:
                              label1417:
                                bitSet_set(mask,1);
                              }
                            }
                            break;
                          /* matching is not complete: jumpNode is null */
                          }
                          break;
                        case code_445: /* cons_equation(,) */
                          v12= GgetArgument(v8,0);
                          switch(GgetSymb(v12)) {
                          default:
                          label1411:
                            v13= GgetArgument(v8,1);
                            switch(GgetSymb(v13)) {
                            default:
                            label1412:
                              v14= GgetArgument(v0,5);
                              switch(GgetSymb(v14)) {
                              default:
                              label1413:
                                bitSet_set(mask,0);
                              }
                            }
                          }
                          break;
                        /* matching is not complete: jumpNode is null */
                        }
                      }
                    }
                    break;
                  default:
                  label1398:
                    v8= GgetArgument(v0,4);
                    switch(GgetSymb(v8)) {
                    case code_444: /* nil */
                      v9= GgetArgument(v0,5);
                      switch(GgetSymb(v9)) {
                      case code_445: /* cons_equation(,) */
                        v10= GgetArgument(v9,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1405:
                          v11= GgetArgument(v9,1);
                          switch(GgetSymb(v11)) {
                          default:
                          label1406:
                            bitSet_set(mask,1);
                          }
                        }
                        break;
                      /* matching is not complete: jumpNode is null */
                      }
                      break;
                    case code_445: /* cons_equation(,) */
                      v12= GgetArgument(v8,0);
                      switch(GgetSymb(v12)) {
                      default:
                      label1400:
                        v13= GgetArgument(v8,1);
                        switch(GgetSymb(v13)) {
                        default:
                        label1401:
                          v14= GgetArgument(v0,5);
                          switch(GgetSymb(v14)) {
                          default:
                          label1402:
                            bitSet_set(mask,0);
                          }
                        }
                      }
                      break;
                    /* matching is not complete: jumpNode is null */
                    }
                  }
                  break;
                default:
                label1293:
                  v5= GgetArgument(v0,3);
                  switch(GgetSymb(v5)) {
                  default:
                  label1294:
                    v8= GgetArgument(v0,4);
                    switch(GgetSymb(v8)) {
                    case code_444: /* nil */
                      v9= GgetArgument(v0,5);
                      switch(GgetSymb(v9)) {
                      case code_445: /* cons_equation(,) */
                        v10= GgetArgument(v9,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1301:
                          v11= GgetArgument(v9,1);
                          switch(GgetSymb(v11)) {
                          default:
                          label1302:
                            bitSet_set(mask,1);
                          }
                        }
                        break;
                      /* matching is not complete: jumpNode is null */
                      }
                      break;
                    case code_445: /* cons_equation(,) */
                      v12= GgetArgument(v8,0);
                      switch(GgetSymb(v12)) {
                      default:
                      label1296:
                        v13= GgetArgument(v8,1);
                        switch(GgetSymb(v13)) {
                        default:
                        label1297:
                          v14= GgetArgument(v0,5);
                          switch(GgetSymb(v14)) {
                          default:
                          label1298:
                            bitSet_set(mask,0);
                          }
                        }
                      }
                      break;
                    /* matching is not complete: jumpNode is null */
                    }
                  }
                }
              }
            }
            break;
          /* matching is not complete: jumpNode is null */
          }
          if(bitSet_get(mask,0)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: *****(var0,var1,var2,var3,cons_equation(,)(var4,var5),var6) */
            /* allDetEvaluation: det */
            /* where var7 := *****(var0,var1,var2,var3,cons_equation(,)(var4,var5),var6) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            // this=var4        underAC=false        Instantiated=false
            // this=var5        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_445            ,v12            ,v13            );
            // this=var6        underAC=false        Instantiated=false
            GmakeAppl6(sv[2],code_461            ,v2            ,v3            ,v4            ,v5            ,sv[1]            ,v14            );
            tmp = sv[0] = sv[2];
            /* rhs: S_to_T()(var7) */
            // this=var7        underAC=false        Instantiated=false
            sv[1] = fun_485( sv[0] );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab238;
            myend340:;
          }
          if(bitSet_get(mask,1)) {
            Gterm *tmp, *sv[4];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: *****(var0,var1,var2,var3,nil,cons_equation(,)(var4,var5)) */
            /* allDetEvaluation: semiDet */
            CUTOPEN(); /* Wheres */
            if(localSetChoicePoint()) {
              /* local evaluations failed, try next rule */
              goto myend341;
            }
            /* where var7 := (WHERE6:compute_state/ans_completion[Vars,Ops,Prec,System]) *****(var0,var1,var2,var3,nil,cons_equation(,)(var4,var5)) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            // this=var4        underAC=false        Instantiated=false
            // this=var5        underAC=false        Instantiated=false
            GmakeAppl2(sv[2],code_445            ,v10            ,v11            );
            GmakeAppl6(sv[3],code_461            ,v2            ,v3            ,v4            ,v5            ,con_444            ,sv[2]            );
            sv[0] = strTab[15]( sv[3] );
            /* where var6 := (WHERE7:compute_state/ans_completion[Vars,Ops,Prec,System]) var7 */
            // this=var7        underAC=false        Instantiated=false
            sv[1] = strTab[16]( sv[0] );
            CUTCLOSE(); /* Wheres */
            /* rhs: var6 */
            // this=var6        underAC=false        Instantiated=false
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab238;
            myend341:;
            CUTCLOSE(); /* Wheres */
          }
          if(bitSet_get(mask,2)) {
            Gterm *tmp, *sv[10];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: *****(var0,cons_equation(,)(var1,var2),cons_equation(,)(var3,nil),var4,nil,nil) */
            /* allDetEvaluation: semiDet */
            CUTOPEN(); /* Wheres */
            if(localSetChoicePoint()) {
              /* local evaluations failed, try next rule */
              goto myend342;
            }
            /* where var6 := (WHERE8:compute_state/ans_completion[Vars,Ops,Prec,System]) *****(var0,cons_equation(,)(var1,var2),cons_equation(,)(var3,nil),var4,nil,nil) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_445            ,v40            ,v41            );
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl2(sv[3],code_445            ,v60            ,con_444            );
            // this=var4        underAC=false        Instantiated=false
            GmakeAppl6(sv[5],code_461            ,v2            ,sv[1]            ,sv[3]            ,v62            ,con_444            ,con_444            );
            sv[0] = strTab[17]( sv[5] );
            /* where var5 := *****(@@(CS_to_A()(var6),CS_to_E()(var6)),CS_to_N()(var6),CS_to_C()(var6),CS_to_T()(var6),CS_to_S()(var6),nil) */
            // this=var6        underAC=false        Instantiated=false
            sv[2] = fun_463( sv[0] );
            // this=var6        underAC=false        Instantiated=false
            sv[3] = fun_468( sv[0] );
            sv[4] = fun_474( sv[2],sv[3] );
            // this=var6        underAC=false        Instantiated=false
            sv[2] = fun_464( sv[0] );
            // this=var6        underAC=false        Instantiated=false
            sv[3] = fun_465( sv[0] );
            // this=var6        underAC=false        Instantiated=false
            sv[6] = fun_466( sv[0] );
            // this=var6        underAC=false        Instantiated=false
            sv[7] = fun_467( sv[0] );
            GmakeAppl6(sv[9],code_461            ,sv[4]            ,sv[2]            ,sv[3]            ,sv[6]            ,sv[7]            ,con_444            );
            tmp = sv[1] = sv[9];
            CUTCLOSE(); /* Wheres */
            /* rhs: var5 */
            // this=var5        underAC=false        Instantiated=false
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab238;
            myend342:;
            CUTCLOSE(); /* Wheres */
          }
          if(bitSet_get(mask,3)) {
            Gterm *tmp, *sv[10];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: *****(var0,nil,cons_equation(,)(var1,nil),var2,nil,nil) */
            /* allDetEvaluation: semiDet */
            CUTOPEN(); /* Wheres */
            if(localSetChoicePoint()) {
              /* local evaluations failed, try next rule */
              goto myend343;
            }
            /* where var4 := (WHERE9:compute_state/ans_completion[Vars,Ops,Prec,System]) *****(var0,nil,cons_equation(,)(var1,nil),var2,nil,nil) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[3],code_445            ,v22            ,con_444            );
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl6(sv[5],code_461            ,v2            ,con_444            ,sv[3]            ,v24            ,con_444            ,con_444            );
            sv[0] = strTab[18]( sv[5] );
            /* where var3 := *****(@@(CS_to_A()(var4),CS_to_E()(var4)),CS_to_C()(var4),nil,CS_to_T()(var4),CS_to_S()(var4),nil) */
            // this=var4        underAC=false        Instantiated=false
            sv[2] = fun_463( sv[0] );
            // this=var4        underAC=false        Instantiated=false
            sv[3] = fun_468( sv[0] );
            sv[4] = fun_474( sv[2],sv[3] );
            // this=var4        underAC=false        Instantiated=false
            sv[2] = fun_465( sv[0] );
            // this=var4        underAC=false        Instantiated=false
            sv[6] = fun_466( sv[0] );
            // this=var4        underAC=false        Instantiated=false
            sv[7] = fun_467( sv[0] );
            GmakeAppl6(sv[9],code_461            ,sv[4]            ,sv[2]            ,con_444            ,sv[6]            ,sv[7]            ,con_444            );
            tmp = sv[1] = sv[9];
            CUTCLOSE(); /* Wheres */
            /* rhs: var3 */
            // this=var3        underAC=false        Instantiated=false
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab238;
            myend343:;
            CUTCLOSE(); /* Wheres */
          }
          if(bitSet_get(mask,4)) {
            Gterm *tmp, *sv[5];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: *****(var0,var1,nil,cons_equation(,)(var2,var3),nil,nil) */
            /* allDetEvaluation: det */
            /* where var4 := *****(var0,var1,cons_equation(,)(var2,nil),var3,nil,nil) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[2],code_445            ,v6            ,con_444            );
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl6(sv[4],code_461            ,v2            ,v3            ,sv[2]            ,v7            ,con_444            ,con_444            );
            tmp = sv[0] = sv[4];
            /* rhs: var4 */
            // this=var4        underAC=false        Instantiated=false
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab238;
            myend344:;
          }
        }
        fail();
        stratLab238:;
        v0=res;
        *((Gterm**)getStablePointer(lastTerm_index)) = res;
      }
    }
    CUTCLOSE(); /* Repeat */
  }
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_464( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* repeat[det](one[semiDet](Regle1:compute_state/ans_completion[Vars,Ops,Prec,System]!GL,Regle2:compute_state/ans_completion[Vars,Ops,Prec,System]!GL,Regle3:compute_state/ans_completion[Vars,Ops,Prec,System]!GL,Regle4:compute_state/ans_completion[Vars,Ops,Prec,System]!GL,Regle5:compute_state/ans_completion[Vars,Ops,Prec,System]!GL)) */
    long lastTerm_index;
    CUTOPEN(); /* Repeat */
    lastTerm_index = allocStable(sizeof(Gterm*));
    *((Gterm**)getStablePointer(lastTerm_index))=v0;
    if(localSetChoicePoint()!=0) {
      res = v0 = *((Gterm**)getStablePointer(lastTerm_index));
      /* End of repeat */
    } else {
      while(1) {
      /* Apply the strategy */
        {
          /* one[semiDet](Regle1:compute_state/ans_completion[Vars,Ops,Prec,System]!GL,Regle2:compute_state/ans_completion[Vars,Ops,Prec,System]!GL,Regle3:compute_state/ans_completion[Vars,Ops,Prec,System]!GL,Regle4:compute_state/ans_completion[Vars,Ops,Prec,System]!GL,Regle5:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
          Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16,*v17,*v18,*v19,*v20,*v21,*v22,*v23,*v24,*v25,*v26,*v27,*v28,*v29,*v30,*v31,*v32,*v33,*v34,*v35,*v36,*v37,*v38,*v39,*v40,*v41,*v42,*v43,*v44,*v45,*v46,*v47,*v48,*v49,*v50,*v51,*v52,*v53,*v54,*v55,*v56,*v57,*v58,*v59,*v60,*v61,*v62,*v63,*v64,*v65,*v66,*v67,*v68,*v69,*v70,*v71,*v72,*v73,*v74,*v75,*v76,*v77,*v78,*v79,*v80,*v81,*v82,*v83,*v84,*v85,*v86,*v87,*v88,*v89,*v90,*v91,*v92,*v93,*v94,*v95,*v96,*v97,*v98,*v99,*v100,*v101,*v102,*v103,*v104;
          bitSet_GC_create(mask,5);
          bitSet_init_clear(mask);
          switch(GgetSymb(v0)) {
          case code_461: /* ***** */
            v2= GgetArgument(v0,0);
            switch(GgetSymb(v2)) {
            default:
            label1421:
              v3= GgetArgument(v0,1);
              switch(GgetSymb(v3)) {
              case code_444: /* nil */
                v4= GgetArgument(v0,2);
                switch(GgetSymb(v4)) {
                case code_444: /* nil */
                  v5= GgetArgument(v0,3);
                  switch(GgetSymb(v5)) {
                  case code_445: /* cons_equation(,) */
                    v6= GgetArgument(v5,0);
                    switch(GgetSymb(v6)) {
                    default:
                    label1516:
                      v7= GgetArgument(v5,1);
                      switch(GgetSymb(v7)) {
                      default:
                      label1517:
                        v8= GgetArgument(v0,4);
                        switch(GgetSymb(v8)) {
                        case code_444: /* nil */
                          v9= GgetArgument(v0,5);
                          switch(GgetSymb(v9)) {
                          case code_444: /* nil */
                            bitSet_set(mask,4);
                            break;
                          case code_445: /* cons_equation(,) */
                            v10= GgetArgument(v9,0);
                            switch(GgetSymb(v10)) {
                            default:
                            label1524:
                              v11= GgetArgument(v9,1);
                              switch(GgetSymb(v11)) {
                              default:
                              label1525:
                                bitSet_set(mask,1);
                              }
                            }
                            break;
                          /* matching is not complete: jumpNode is null */
                          }
                          break;
                        case code_445: /* cons_equation(,) */
                          v12= GgetArgument(v8,0);
                          switch(GgetSymb(v12)) {
                          default:
                          label1519:
                            v13= GgetArgument(v8,1);
                            switch(GgetSymb(v13)) {
                            default:
                            label1520:
                              v14= GgetArgument(v0,5);
                              switch(GgetSymb(v14)) {
                              default:
                              label1521:
                                bitSet_set(mask,0);
                              }
                            }
                          }
                          break;
                        /* matching is not complete: jumpNode is null */
                        }
                      }
                    }
                    break;
                  default:
                  label1506:
                    v8= GgetArgument(v0,4);
                    switch(GgetSymb(v8)) {
                    case code_444: /* nil */
                      v9= GgetArgument(v0,5);
                      switch(GgetSymb(v9)) {
                      case code_445: /* cons_equation(,) */
                        v10= GgetArgument(v9,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1513:
                          v11= GgetArgument(v9,1);
                          switch(GgetSymb(v11)) {
                          default:
                          label1514:
                            bitSet_set(mask,1);
                          }
                        }
                        break;
                      /* matching is not complete: jumpNode is null */
                      }
                      break;
                    case code_445: /* cons_equation(,) */
                      v12= GgetArgument(v8,0);
                      switch(GgetSymb(v12)) {
                      default:
                      label1508:
                        v13= GgetArgument(v8,1);
                        switch(GgetSymb(v13)) {
                        default:
                        label1509:
                          v14= GgetArgument(v0,5);
                          switch(GgetSymb(v14)) {
                          default:
                          label1510:
                            bitSet_set(mask,0);
                          }
                        }
                      }
                      break;
                    /* matching is not complete: jumpNode is null */
                    }
                  }
                  break;
                case code_445: /* cons_equation(,) */
                  v22= GgetArgument(v4,0);
                  switch(GgetSymb(v22)) {
                  default:
                  label1471:
                    v23= GgetArgument(v4,1);
                    switch(GgetSymb(v23)) {
                    case code_444: /* nil */
                      v5= GgetArgument(v0,3);
                      v24= GgetArgument(v0,3);
                      switch(GgetSymb(v24)) {
                      default:
                      label1473:
                        v8= GgetArgument(v0,4);
                        v25= GgetArgument(v0,4);
                        switch(GgetSymb(v25)) {
                        case code_444: /* nil */
                          v9= GgetArgument(v0,5);
                          v26= GgetArgument(v0,5);
                          switch(GgetSymb(v26)) {
                          case code_444: /* nil */
                            bitSet_set(mask,3);
                            break;
                          case code_445: /* cons_equation(,) */
                            v10= GgetArgument(v9,0);
                            switch(GgetSymb(v10)) {
                            default:
                            label1480:
                              v11= GgetArgument(v9,1);
                              switch(GgetSymb(v11)) {
                              default:
                              label1481:
                                bitSet_set(mask,1);
                              }
                            }
                            break;
                          /* matching is not complete: jumpNode is null */
                          }
                          break;
                        case code_445: /* cons_equation(,) */
                          v12= GgetArgument(v8,0);
                          switch(GgetSymb(v12)) {
                          default:
                          label1475:
                            v13= GgetArgument(v8,1);
                            switch(GgetSymb(v13)) {
                            default:
                            label1476:
                              v14= GgetArgument(v0,5);
                              switch(GgetSymb(v14)) {
                              default:
                              label1477:
                                bitSet_set(mask,0);
                              }
                            }
                          }
                          break;
                        /* matching is not complete: jumpNode is null */
                        }
                      }
                      break;
                    default:
                      goto label1460;
                    }
                  }
                  break;
                default:
                label1460:
                  v5= GgetArgument(v0,3);
                  switch(GgetSymb(v5)) {
                  default:
                  label1461:
                    v8= GgetArgument(v0,4);
                    switch(GgetSymb(v8)) {
                    case code_444: /* nil */
                      v9= GgetArgument(v0,5);
                      switch(GgetSymb(v9)) {
                      case code_445: /* cons_equation(,) */
                        v10= GgetArgument(v9,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1468:
                          v11= GgetArgument(v9,1);
                          switch(GgetSymb(v11)) {
                          default:
                          label1469:
                            bitSet_set(mask,1);
                          }
                        }
                        break;
                      /* matching is not complete: jumpNode is null */
                      }
                      break;
                    case code_445: /* cons_equation(,) */
                      v12= GgetArgument(v8,0);
                      switch(GgetSymb(v12)) {
                      default:
                      label1463:
                        v13= GgetArgument(v8,1);
                        switch(GgetSymb(v13)) {
                        default:
                        label1464:
                          v14= GgetArgument(v0,5);
                          switch(GgetSymb(v14)) {
                          default:
                          label1465:
                            bitSet_set(mask,0);
                          }
                        }
                      }
                      break;
                    /* matching is not complete: jumpNode is null */
                    }
                  }
                }
                break;
              case code_445: /* cons_equation(,) */
                v40= GgetArgument(v3,0);
                switch(GgetSymb(v40)) {
                default:
                label1434:
                  v41= GgetArgument(v3,1);
                  switch(GgetSymb(v41)) {
                  default:
                  label1435:
                    v4= GgetArgument(v0,2);
                    v42= GgetArgument(v0,2);
                    switch(GgetSymb(v4)) {
                    case code_444: /* nil */
                      v5= GgetArgument(v0,3);
                      switch(GgetSymb(v5)) {
                      case code_445: /* cons_equation(,) */
                        v6= GgetArgument(v5,0);
                        switch(GgetSymb(v6)) {
                        default:
                        label1494:
                          v7= GgetArgument(v5,1);
                          switch(GgetSymb(v7)) {
                          default:
                          label1495:
                            v8= GgetArgument(v0,4);
                            switch(GgetSymb(v8)) {
                            case code_444: /* nil */
                              v9= GgetArgument(v0,5);
                              switch(GgetSymb(v9)) {
                              case code_444: /* nil */
                                bitSet_set(mask,4);
                                break;
                              case code_445: /* cons_equation(,) */
                                v10= GgetArgument(v9,0);
                                switch(GgetSymb(v10)) {
                                default:
                                label1502:
                                  v11= GgetArgument(v9,1);
                                  switch(GgetSymb(v11)) {
                                  default:
                                  label1503:
                                    bitSet_set(mask,1);
                                  }
                                }
                                break;
                              /* matching is not complete: jumpNode is null */
                              }
                              break;
                            case code_445: /* cons_equation(,) */
                              v12= GgetArgument(v8,0);
                              switch(GgetSymb(v12)) {
                              default:
                              label1497:
                                v13= GgetArgument(v8,1);
                                switch(GgetSymb(v13)) {
                                default:
                                label1498:
                                  v14= GgetArgument(v0,5);
                                  switch(GgetSymb(v14)) {
                                  default:
                                  label1499:
                                    bitSet_set(mask,0);
                                  }
                                }
                              }
                              break;
                            /* matching is not complete: jumpNode is null */
                            }
                          }
                        }
                        break;
                      default:
                      label1484:
                        v8= GgetArgument(v0,4);
                        switch(GgetSymb(v8)) {
                        case code_444: /* nil */
                          v9= GgetArgument(v0,5);
                          switch(GgetSymb(v9)) {
                          case code_445: /* cons_equation(,) */
                            v10= GgetArgument(v9,0);
                            switch(GgetSymb(v10)) {
                            default:
                            label1491:
                              v11= GgetArgument(v9,1);
                              switch(GgetSymb(v11)) {
                              default:
                              label1492:
                                bitSet_set(mask,1);
                              }
                            }
                            break;
                          /* matching is not complete: jumpNode is null */
                          }
                          break;
                        case code_445: /* cons_equation(,) */
                          v12= GgetArgument(v8,0);
                          switch(GgetSymb(v12)) {
                          default:
                          label1486:
                            v13= GgetArgument(v8,1);
                            switch(GgetSymb(v13)) {
                            default:
                            label1487:
                              v14= GgetArgument(v0,5);
                              switch(GgetSymb(v14)) {
                              default:
                              label1488:
                                bitSet_set(mask,0);
                              }
                            }
                          }
                          break;
                        /* matching is not complete: jumpNode is null */
                        }
                      }
                      break;
                    case code_445: /* cons_equation(,) */
                      v60= GgetArgument(v42,0);
                      switch(GgetSymb(v60)) {
                      default:
                      label1447:
                        v61= GgetArgument(v42,1);
                        switch(GgetSymb(v61)) {
                        case code_444: /* nil */
                          v5= GgetArgument(v0,3);
                          v62= GgetArgument(v0,3);
                          switch(GgetSymb(v62)) {
                          default:
                          label1449:
                            v8= GgetArgument(v0,4);
                            v63= GgetArgument(v0,4);
                            switch(GgetSymb(v63)) {
                            case code_444: /* nil */
                              v9= GgetArgument(v0,5);
                              v64= GgetArgument(v0,5);
                              switch(GgetSymb(v64)) {
                              case code_444: /* nil */
                                bitSet_set(mask,2);
                                break;
                              case code_445: /* cons_equation(,) */
                                v10= GgetArgument(v9,0);
                                switch(GgetSymb(v10)) {
                                default:
                                label1456:
                                  v11= GgetArgument(v9,1);
                                  switch(GgetSymb(v11)) {
                                  default:
                                  label1457:
                                    bitSet_set(mask,1);
                                  }
                                }
                                break;
                              /* matching is not complete: jumpNode is null */
                              }
                              break;
                            case code_445: /* cons_equation(,) */
                              v12= GgetArgument(v8,0);
                              switch(GgetSymb(v12)) {
                              default:
                              label1451:
                                v13= GgetArgument(v8,1);
                                switch(GgetSymb(v13)) {
                                default:
                                label1452:
                                  v14= GgetArgument(v0,5);
                                  switch(GgetSymb(v14)) {
                                  default:
                                  label1453:
                                    bitSet_set(mask,0);
                                  }
                                }
                              }
                              break;
                            /* matching is not complete: jumpNode is null */
                            }
                          }
                          break;
                        default:
                          goto label1436;
                        }
                      }
                      break;
                    default:
                    label1436:
                      v5= GgetArgument(v0,3);
                      switch(GgetSymb(v5)) {
                      default:
                      label1437:
                        v8= GgetArgument(v0,4);
                        switch(GgetSymb(v8)) {
                        case code_444: /* nil */
                          v9= GgetArgument(v0,5);
                          switch(GgetSymb(v9)) {
                          case code_445: /* cons_equation(,) */
                            v10= GgetArgument(v9,0);
                            switch(GgetSymb(v10)) {
                            default:
                            label1444:
                              v11= GgetArgument(v9,1);
                              switch(GgetSymb(v11)) {
                              default:
                              label1445:
                                bitSet_set(mask,1);
                              }
                            }
                            break;
                          /* matching is not complete: jumpNode is null */
                          }
                          break;
                        case code_445: /* cons_equation(,) */
                          v12= GgetArgument(v8,0);
                          switch(GgetSymb(v12)) {
                          default:
                          label1439:
                            v13= GgetArgument(v8,1);
                            switch(GgetSymb(v13)) {
                            default:
                            label1440:
                              v14= GgetArgument(v0,5);
                              switch(GgetSymb(v14)) {
                              default:
                              label1441:
                                bitSet_set(mask,0);
                              }
                            }
                          }
                          break;
                        /* matching is not complete: jumpNode is null */
                        }
                      }
                    }
                  }
                }
                break;
              default:
              label1422:
                v4= GgetArgument(v0,2);
                switch(GgetSymb(v4)) {
                case code_444: /* nil */
                  v5= GgetArgument(v0,3);
                  switch(GgetSymb(v5)) {
                  case code_445: /* cons_equation(,) */
                    v6= GgetArgument(v5,0);
                    switch(GgetSymb(v6)) {
                    default:
                    label1538:
                      v7= GgetArgument(v5,1);
                      switch(GgetSymb(v7)) {
                      default:
                      label1539:
                        v8= GgetArgument(v0,4);
                        switch(GgetSymb(v8)) {
                        case code_444: /* nil */
                          v9= GgetArgument(v0,5);
                          switch(GgetSymb(v9)) {
                          case code_444: /* nil */
                            bitSet_set(mask,4);
                            break;
                          case code_445: /* cons_equation(,) */
                            v10= GgetArgument(v9,0);
                            switch(GgetSymb(v10)) {
                            default:
                            label1546:
                              v11= GgetArgument(v9,1);
                              switch(GgetSymb(v11)) {
                              default:
                              label1547:
                                bitSet_set(mask,1);
                              }
                            }
                            break;
                          /* matching is not complete: jumpNode is null */
                          }
                          break;
                        case code_445: /* cons_equation(,) */
                          v12= GgetArgument(v8,0);
                          switch(GgetSymb(v12)) {
                          default:
                          label1541:
                            v13= GgetArgument(v8,1);
                            switch(GgetSymb(v13)) {
                            default:
                            label1542:
                              v14= GgetArgument(v0,5);
                              switch(GgetSymb(v14)) {
                              default:
                              label1543:
                                bitSet_set(mask,0);
                              }
                            }
                          }
                          break;
                        /* matching is not complete: jumpNode is null */
                        }
                      }
                    }
                    break;
                  default:
                  label1528:
                    v8= GgetArgument(v0,4);
                    switch(GgetSymb(v8)) {
                    case code_444: /* nil */
                      v9= GgetArgument(v0,5);
                      switch(GgetSymb(v9)) {
                      case code_445: /* cons_equation(,) */
                        v10= GgetArgument(v9,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1535:
                          v11= GgetArgument(v9,1);
                          switch(GgetSymb(v11)) {
                          default:
                          label1536:
                            bitSet_set(mask,1);
                          }
                        }
                        break;
                      /* matching is not complete: jumpNode is null */
                      }
                      break;
                    case code_445: /* cons_equation(,) */
                      v12= GgetArgument(v8,0);
                      switch(GgetSymb(v12)) {
                      default:
                      label1530:
                        v13= GgetArgument(v8,1);
                        switch(GgetSymb(v13)) {
                        default:
                        label1531:
                          v14= GgetArgument(v0,5);
                          switch(GgetSymb(v14)) {
                          default:
                          label1532:
                            bitSet_set(mask,0);
                          }
                        }
                      }
                      break;
                    /* matching is not complete: jumpNode is null */
                    }
                  }
                  break;
                default:
                label1423:
                  v5= GgetArgument(v0,3);
                  switch(GgetSymb(v5)) {
                  default:
                  label1424:
                    v8= GgetArgument(v0,4);
                    switch(GgetSymb(v8)) {
                    case code_444: /* nil */
                      v9= GgetArgument(v0,5);
                      switch(GgetSymb(v9)) {
                      case code_445: /* cons_equation(,) */
                        v10= GgetArgument(v9,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1431:
                          v11= GgetArgument(v9,1);
                          switch(GgetSymb(v11)) {
                          default:
                          label1432:
                            bitSet_set(mask,1);
                          }
                        }
                        break;
                      /* matching is not complete: jumpNode is null */
                      }
                      break;
                    case code_445: /* cons_equation(,) */
                      v12= GgetArgument(v8,0);
                      switch(GgetSymb(v12)) {
                      default:
                      label1426:
                        v13= GgetArgument(v8,1);
                        switch(GgetSymb(v13)) {
                        default:
                        label1427:
                          v14= GgetArgument(v0,5);
                          switch(GgetSymb(v14)) {
                          default:
                          label1428:
                            bitSet_set(mask,0);
                          }
                        }
                      }
                      break;
                    /* matching is not complete: jumpNode is null */
                    }
                  }
                }
              }
            }
            break;
          /* matching is not complete: jumpNode is null */
          }
          if(bitSet_get(mask,0)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: *****(var0,var1,var2,var3,cons_equation(,)(var4,var5),var6) */
            /* allDetEvaluation: semiDet */
            CUTOPEN(); /* Wheres */
            if(localSetChoicePoint()) {
              /* local evaluations failed, try next rule */
              goto myend345;
            }
            /* where var7 := (s_simplify_ANCT:compute_state/ans_completion[Vars,Ops,Prec,System]) *****(var0,var1,var2,var3,cons_equation(,)(var4,var5),var6) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            // this=var4        underAC=false        Instantiated=false
            // this=var5        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_445            ,v12            ,v13            );
            // this=var6        underAC=false        Instantiated=false
            GmakeAppl6(sv[2],code_461            ,v2            ,v3            ,v4            ,v5            ,sv[1]            ,v14            );
            sv[0] = strTab[269]( sv[2] );
            CUTCLOSE(); /* Wheres */
            /* rhs: S_to_T()(var7) */
            // this=var7        underAC=false        Instantiated=false
            sv[1] = fun_485( sv[0] );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab229;
            myend345:;
            CUTCLOSE(); /* Wheres */
          }
          if(bitSet_get(mask,1)) {
            Gterm *tmp, *sv[4];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: *****(var0,var1,var2,var3,nil,cons_equation(,)(var4,var5)) */
            /* allDetEvaluation: semiDet */
            CUTOPEN(); /* Wheres */
            if(localSetChoicePoint()) {
              /* local evaluations failed, try next rule */
              goto myend346;
            }
            /* where var7 := (WHERE0:compute_state/ans_completion[Vars,Ops,Prec,System]) *****(var0,var1,var2,var3,nil,cons_equation(,)(var4,var5)) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            // this=var4        underAC=false        Instantiated=false
            // this=var5        underAC=false        Instantiated=false
            GmakeAppl2(sv[2],code_445            ,v10            ,v11            );
            GmakeAppl6(sv[3],code_461            ,v2            ,v3            ,v4            ,v5            ,con_444            ,sv[2]            );
            sv[0] = strTab[9]( sv[3] );
            /* where var8 := (WHERE1:compute_state/ans_completion[Vars,Ops,Prec,System]) var7 */
            // this=var7        underAC=false        Instantiated=false
            sv[1] = strTab[10]( sv[0] );
            /* where var6 := (WHERE2:compute_state/ans_completion[Vars,Ops,Prec,System]) var8 */
            // this=var8        underAC=false        Instantiated=false
            sv[2] = strTab[11]( sv[1] );
            CUTCLOSE(); /* Wheres */
            /* rhs: var6 */
            // this=var6        underAC=false        Instantiated=false
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab229;
            myend346:;
            CUTCLOSE(); /* Wheres */
          }
          if(bitSet_get(mask,2)) {
            Gterm *tmp, *sv[6];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: *****(var0,cons_equation(,)(var1,var2),cons_equation(,)(var3,nil),var4,nil,nil) */
            /* allDetEvaluation: semiDet */
            CUTOPEN(); /* Wheres */
            if(localSetChoicePoint()) {
              /* local evaluations failed, try next rule */
              goto myend347;
            }
            /* where var5 := (WHERE3:compute_state/ans_completion[Vars,Ops,Prec,System]) *****(var0,cons_equation(,)(var1,var2),cons_equation(,)(var3,nil),var4,nil,nil) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_445            ,v40            ,v41            );
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl2(sv[3],code_445            ,v60            ,con_444            );
            // this=var4        underAC=false        Instantiated=false
            GmakeAppl6(sv[5],code_461            ,v2            ,sv[1]            ,sv[3]            ,v62            ,con_444            ,con_444            );
            sv[0] = strTab[12]( sv[5] );
            CUTCLOSE(); /* Wheres */
            /* rhs: var5 */
            // this=var5        underAC=false        Instantiated=false
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab229;
            myend347:;
            CUTCLOSE(); /* Wheres */
          }
          if(bitSet_get(mask,3)) {
            Gterm *tmp, *sv[6];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: *****(var0,nil,cons_equation(,)(var1,nil),var2,nil,nil) */
            /* allDetEvaluation: semiDet */
            CUTOPEN(); /* Wheres */
            if(localSetChoicePoint()) {
              /* local evaluations failed, try next rule */
              goto myend348;
            }
            /* where var4 := (WHERE4:compute_state/ans_completion[Vars,Ops,Prec,System]) *****(var0,nil,cons_equation(,)(var1,nil),var2,nil,nil) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[3],code_445            ,v22            ,con_444            );
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl6(sv[5],code_461            ,v2            ,con_444            ,sv[3]            ,v24            ,con_444            ,con_444            );
            sv[0] = strTab[13]( sv[5] );
            /* where var3 := AC_to_N()(var4) */
            // this=var4        underAC=false        Instantiated=false
            sv[2] = fun_484( sv[0] );
            tmp = sv[1] = sv[2];
            CUTCLOSE(); /* Wheres */
            /* rhs: var3 */
            // this=var3        underAC=false        Instantiated=false
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab229;
            myend348:;
            CUTCLOSE(); /* Wheres */
          }
          if(bitSet_get(mask,4)) {
            Gterm *tmp, *sv[6];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: *****(var0,var1,nil,cons_equation(,)(var2,var3),nil,nil) */
            /* allDetEvaluation: semiDet */
            CUTOPEN(); /* Wheres */
            /* where var4 := (WHERE5:compute_state/ans_completion[Vars,Ops,Prec,System]) *****(var0,var1,nil,cons_equation(,)(var2,var3),nil,nil) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl2(sv[2],code_445            ,v6            ,v7            );
            GmakeAppl6(sv[5],code_461            ,v2            ,v3            ,con_444            ,sv[2]            ,con_444            ,con_444            );
            sv[0] = strTab[14]( sv[5] );
            CUTCLOSE(); /* Wheres */
            /* rhs: var4 */
            // this=var4        underAC=false        Instantiated=false
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab229;
            myend349:;
            CUTCLOSE(); /* Wheres */
          }
        }
        fail();
        stratLab229:;
        v0=res;
        *((Gterm**)getStablePointer(lastTerm_index)) = res;
      }
    }
    CUTCLOSE(); /* Repeat */
  }
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_369( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* repeat[det](one[semiDet](new_var_elim:eqSystem/eqSystem[Ops,Vars]!GL,unfold:eqSystem/eqSystem[Ops,Vars]!GL,eqPass:eqSystem/eqSystem[Ops,Vars]!GL)) */
    long lastTerm_index;
    CUTOPEN(); /* Repeat */
    lastTerm_index = allocStable(sizeof(Gterm*));
    *((Gterm**)getStablePointer(lastTerm_index))=v0;
    if(localSetChoicePoint()!=0) {
      res = v0 = *((Gterm**)getStablePointer(lastTerm_index));
      /* End of repeat */
    } else {
      while(1) {
      /* Apply the strategy */
        {
          /* one[semiDet](new_var_elim:eqSystem/eqSystem[Ops,Vars]!GL,unfold:eqSystem/eqSystem[Ops,Vars]!GL,eqPass:eqSystem/eqSystem[Ops,Vars]!GL) */
          Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11;
          bitSet_GC_create(mask,3);
          bitSet_init_clear(mask);
          switch(GgetSymb(v0)) {
          case code_426: /* (&) */
            v2= GgetArgument(v0,0);
            switch(GgetSymb(v2)) {
            default:
            label1551:
              v3= GgetArgument(v0,1);
              switch(GgetSymb(v3)) {
              case code_425: /*  */
                v4= GgetArgument(v3,0);
                switch(GgetSymb(v4)) {
                case code_423: /* = */
                  v5= GgetArgument(v4,0);
                  switch(GgetSymb(v5)) {
                  case code_362: /*  */
                    v6= GgetArgument(v5,0);
                    switch(GgetSymb(v6)) {
                    case code_361: /* var() */
                      v7= GgetArgument(v6,0);
                      switch(GgetSymb(v7)) {
                      default:
                      label1556:
                        v8= GgetArgument(v4,1);
                        switch(GgetSymb(v8)) {
                        default:
                        label1557:
                          bitSet_set(mask,0);
                          bitSet_set(mask,2);
                        }
                      }
                      break;
                    default:
                      goto label1553;
                    }
                    break;
                  default:
                    goto label1553;
                  }
                  break;
                default:
                label1553:
                  bitSet_set(mask,2);
                }
                break;
              case code_426: /* (&) */
                v9= GgetArgument(v3,0);
                switch(GgetSymb(v9)) {
                default:
                label1559:
                  v10= GgetArgument(v3,1);
                  switch(GgetSymb(v10)) {
                  default:
                  label1560:
                    bitSet_set(mask,1);
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
            break;
          /* matching is not complete: jumpNode is null */
          }
          if(bitSet_get(mask,0)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=((var()(var1)),var2))) */
            /* allDetEvaluation: det */
            /* rhs: apply(,)(->(var()(var1),var2),var0) */
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl1(sv[0],code_361            ,v7            );
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_343            ,sv[0]            ,v8            );
            // this=var0        underAC=false        Instantiated=false
            sv[0] = fun_422( sv[1],v2 );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab214;
            myend350:;
          }
          if(bitSet_get(mask,1)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(&)(var1,var2)) */
            /* allDetEvaluation: det */
            /* rhs: (&)((&)(var0,var1),var2) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_426            ,v2            ,v9            );
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_426            ,sv[0]            ,v10            );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab214;
            myend351:;
          }
          if(bitSet_get(mask,2)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(var1)) */
            /* allDetEvaluation: det */
            /* rhs: (&)((var1),var0) */
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl1(sv[0],code_425            ,v4            );
            // this=var0        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_426            ,sv[0]            ,v2            );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab214;
            myend352:;
          }
        }
        fail();
        stratLab214:;
        v0=res;
        *((Gterm**)getStablePointer(lastTerm_index)) = res;
      }
    }
    CUTCLOSE(); /* Repeat */
  }
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_148( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  /* cons[nonDet](iterate[multiDet](one[semiDet](extractrule2:equation/list[equation]!LO)),one[semiDet](extractrule1:equation/list[equation]!LO)) */
  /* iterate[multiDet](one[semiDet](extractrule2:equation/list[equation]!LO)) */
  while(1) {
    if(!setChoicePoint()) {
      break;
    }
    /* Apply the strategy */
    {
      /* one[semiDet](extractrule2:equation/list[equation]!LO) */
      Gterm *v1,*v2,*v3,*v4,*v5;
      bitSet_GC_create(mask,1);
      bitSet_init_clear(mask);
      switch(GgetSymb(v0)) {
      case code_447: /* elem() */
        v2= GgetArgument(v0,0);
        switch(GgetSymb(v2)) {
        case code_445: /* cons_equation(,) */
          v3= GgetArgument(v2,0);
          switch(GgetSymb(v3)) {
          default:
          label1565:
            v4= GgetArgument(v2,1);
            switch(GgetSymb(v4)) {
            default:
            label1566:
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
        Gterm *tmp, *sv[1];
        multiplicityType *E,*sol;
        Gterm *substitution[1];
        /* lhs: elem()(cons_equation(,)(var0,var1)) */
        /* allDetEvaluation: det */
        /* rhs: elem()(var1) */
        // this=var1        underAC=false        Instantiated=false
        GmakeAppl1(sv[0],code_447        ,v4        );
        res = sv[0] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab120;
        myend353:;
      }
    }
    fail();
    stratLab120:;
    v0=res;
  }
  {
    /* one[semiDet](extractrule1:equation/list[equation]!LO) */
    Gterm *v1,*v2,*v3,*v4,*v5;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_447: /* elem() */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_445: /* cons_equation(,) */
        v3= GgetArgument(v2,0);
        switch(GgetSymb(v3)) {
        default:
        label1570:
          v4= GgetArgument(v2,1);
          switch(GgetSymb(v4)) {
          default:
          label1571:
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
      Gterm *tmp, *sv[1];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: elem()(cons_equation(,)(var0,var1)) */
      /* allDetEvaluation: det */
      /* rhs: var0 */
      // this=var0        underAC=false        Instantiated=false
      res = v3 ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab123;
      myend354:;
    }
  }
  fail();
  stratLab123:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_26( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  /* cons[nonDet](iterate[multiDet](one[semiDet](extractrule2:identifier/list[identifier]!LO)),one[semiDet](extractrule1:identifier/list[identifier]!LO)) */
  /* iterate[multiDet](one[semiDet](extractrule2:identifier/list[identifier]!LO)) */
  while(1) {
    if(!setChoicePoint()) {
      break;
    }
    /* Apply the strategy */
    {
      /* one[semiDet](extractrule2:identifier/list[identifier]!LO) */
      Gterm *v1,*v2,*v3,*v4,*v5;
      bitSet_GC_create(mask,1);
      bitSet_init_clear(mask);
      switch(GgetSymb(v0)) {
      case code_322: /* elem() */
        v2= GgetArgument(v0,0);
        switch(GgetSymb(v2)) {
        case code_320: /* cons_identifier(,) */
          v3= GgetArgument(v2,0);
          switch(GgetSymb(v3)) {
          default:
          label1575:
            v4= GgetArgument(v2,1);
            switch(GgetSymb(v4)) {
            default:
            label1576:
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
        Gterm *tmp, *sv[1];
        multiplicityType *E,*sol;
        Gterm *substitution[1];
        /* lhs: elem()(cons_identifier(,)(var0,var1)) */
        /* allDetEvaluation: det */
        /* rhs: elem()(var1) */
        // this=var1        underAC=false        Instantiated=false
        GmakeAppl1(sv[0],code_322        ,v4        );
        res = sv[0] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab34;
        myend355:;
      }
    }
    fail();
    stratLab34:;
    v0=res;
  }
  {
    /* one[semiDet](extractrule1:identifier/list[identifier]!LO) */
    Gterm *v1,*v2,*v3,*v4,*v5;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_322: /* elem() */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_320: /* cons_identifier(,) */
        v3= GgetArgument(v2,0);
        switch(GgetSymb(v3)) {
        default:
        label1580:
          v4= GgetArgument(v2,1);
          switch(GgetSymb(v4)) {
          default:
          label1581:
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
      Gterm *tmp, *sv[1];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: elem()(cons_identifier(,)(var0,var1)) */
      /* allDetEvaluation: det */
      /* rhs: var0 */
      // this=var0        underAC=false        Instantiated=false
      res = v3 ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab37;
      myend356:;
    }
  }
  fail();
  stratLab37:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_70( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  /* cons[nonDet](iterate[multiDet](one[semiDet](extractrule2:int/list[int]!LO)),one[semiDet](extractrule1:int/list[int]!LO)) */
  /* iterate[multiDet](one[semiDet](extractrule2:int/list[int]!LO)) */
  while(1) {
    if(!setChoicePoint()) {
      break;
    }
    /* Apply the strategy */
    {
      /* one[semiDet](extractrule2:int/list[int]!LO) */
      Gterm *v1,*v2,*v3,*v4,*v5;
      bitSet_GC_create(mask,1);
      bitSet_init_clear(mask);
      switch(GgetSymb(v0)) {
      case code_348: /* elem() */
        v2= GgetArgument(v0,0);
        switch(GgetSymb(v2)) {
        case code_346: /* cons_int(,) */
          v3= GgetArgument(v2,0);
          switch(GgetSymb(v3)) {
          default:
          label1585:
            v4= GgetArgument(v2,1);
            switch(GgetSymb(v4)) {
            default:
            label1586:
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
        Gterm *tmp, *sv[1];
        multiplicityType *E,*sol;
        Gterm *substitution[1];
        /* lhs: elem()(cons_int(,)(var0,var1)) */
        /* allDetEvaluation: det */
        /* rhs: elem()(var1) */
        // this=var1        underAC=false        Instantiated=false
        GmakeAppl1(sv[0],code_348        ,v4        );
        res = sv[0] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab74;
        myend357:;
      }
    }
    fail();
    stratLab74:;
    v0=res;
  }
  {
    /* one[semiDet](extractrule1:int/list[int]!LO) */
    Gterm *v1,*v2,*v3,*v4,*v5;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_348: /* elem() */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_346: /* cons_int(,) */
        v3= GgetArgument(v2,0);
        switch(GgetSymb(v3)) {
        default:
        label1590:
          v4= GgetArgument(v2,1);
          switch(GgetSymb(v4)) {
          default:
          label1591:
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
      Gterm *tmp, *sv[1];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: elem()(cons_int(,)(var0,var1)) */
      /* allDetEvaluation: det */
      /* rhs: var0 */
      // this=var0        underAC=false        Instantiated=false
      res = v3 ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab77;
      myend358:;
    }
  }
  fail();
  stratLab77:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_0( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  /* cons[nonDet](iterate[multiDet](one[semiDet](extractrule2:pair[identifier,int]/list[pair[identifier,int]]!LO)),one[semiDet](extractrule1:pair[identifier,int]/list[pair[identifier,int]]!LO)) */
  /* iterate[multiDet](one[semiDet](extractrule2:pair[identifier,int]/list[pair[identifier,int]]!LO)) */
  while(1) {
    if(!setChoicePoint()) {
      break;
    }
    /* Apply the strategy */
    {
      /* one[semiDet](extractrule2:pair[identifier,int]/list[pair[identifier,int]]!LO) */
      Gterm *v1,*v2,*v3,*v4,*v5;
      bitSet_GC_create(mask,1);
      bitSet_init_clear(mask);
      switch(GgetSymb(v0)) {
      case code_334: /* elem() */
        v2= GgetArgument(v0,0);
        switch(GgetSymb(v2)) {
        case code_332: /* cons_pair[identifier,int](,) */
          v3= GgetArgument(v2,0);
          switch(GgetSymb(v3)) {
          default:
          label1595:
            v4= GgetArgument(v2,1);
            switch(GgetSymb(v4)) {
            default:
            label1596:
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
        Gterm *tmp, *sv[1];
        multiplicityType *E,*sol;
        Gterm *substitution[1];
        /* lhs: elem()(cons_pair[identifier,int](,)(var0,var1)) */
        /* allDetEvaluation: det */
        /* rhs: elem()(var1) */
        // this=var1        underAC=false        Instantiated=false
        GmakeAppl1(sv[0],code_334        ,v4        );
        res = sv[0] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab2;
        myend359:;
      }
    }
    fail();
    stratLab2:;
    v0=res;
  }
  {
    /* one[semiDet](extractrule1:pair[identifier,int]/list[pair[identifier,int]]!LO) */
    Gterm *v1,*v2,*v3,*v4,*v5;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_334: /* elem() */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_332: /* cons_pair[identifier,int](,) */
        v3= GgetArgument(v2,0);
        switch(GgetSymb(v3)) {
        default:
        label1600:
          v4= GgetArgument(v2,1);
          switch(GgetSymb(v4)) {
          default:
          label1601:
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
      Gterm *tmp, *sv[1];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: elem()(cons_pair[identifier,int](,)(var0,var1)) */
      /* allDetEvaluation: det */
      /* rhs: var0 */
      // this=var0        underAC=false        Instantiated=false
      res = v3 ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab5;
      myend360:;
    }
  }
  fail();
  stratLab5:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_288( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  /* cons[nonDet](iterate[multiDet](one[semiDet](extractrule2:term/list[term]!LO)),one[semiDet](extractrule1:term/list[term]!LO)) */
  /* iterate[multiDet](one[semiDet](extractrule2:term/list[term]!LO)) */
  while(1) {
    if(!setChoicePoint()) {
      break;
    }
    /* Apply the strategy */
    {
      /* one[semiDet](extractrule2:term/list[term]!LO) */
      Gterm *v1,*v2,*v3,*v4,*v5;
      bitSet_GC_create(mask,1);
      bitSet_init_clear(mask);
      switch(GgetSymb(v0)) {
      case code_393: /* elem() */
        v2= GgetArgument(v0,0);
        switch(GgetSymb(v2)) {
        case code_391: /* cons_term(,) */
          v3= GgetArgument(v2,0);
          switch(GgetSymb(v3)) {
          default:
          label1605:
            v4= GgetArgument(v2,1);
            switch(GgetSymb(v4)) {
            default:
            label1606:
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
        Gterm *tmp, *sv[1];
        multiplicityType *E,*sol;
        Gterm *substitution[1];
        /* lhs: elem()(cons_term(,)(var0,var1)) */
        /* allDetEvaluation: det */
        /* rhs: elem()(var1) */
        // this=var1        underAC=false        Instantiated=false
        GmakeAppl1(sv[0],code_393        ,v4        );
        res = sv[0] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab206;
        myend361:;
      }
    }
    fail();
    stratLab206:;
    v0=res;
  }
  {
    /* one[semiDet](extractrule1:term/list[term]!LO) */
    Gterm *v1,*v2,*v3,*v4,*v5;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_393: /* elem() */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_391: /* cons_term(,) */
        v3= GgetArgument(v2,0);
        switch(GgetSymb(v3)) {
        default:
        label1610:
          v4= GgetArgument(v2,1);
          switch(GgetSymb(v4)) {
          default:
          label1611:
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
      Gterm *tmp, *sv[1];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: elem()(cons_term(,)(var0,var1)) */
      /* allDetEvaluation: det */
      /* rhs: var0 */
      // this=var0        underAC=false        Instantiated=false
      res = v3 ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab209;
      myend362:;
    }
  }
  fail();
  stratLab209:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_84( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  /* cons[nonDet](iterate[multiDet](one[semiDet](extractrule2:variable/list[variable]!LO)),one[semiDet](extractrule1:variable/list[variable]!LO)) */
  /* iterate[multiDet](one[semiDet](extractrule2:variable/list[variable]!LO)) */
  while(1) {
    if(!setChoicePoint()) {
      break;
    }
    /* Apply the strategy */
    {
      /* one[semiDet](extractrule2:variable/list[variable]!LO) */
      Gterm *v1,*v2,*v3,*v4,*v5;
      bitSet_GC_create(mask,1);
      bitSet_init_clear(mask);
      switch(GgetSymb(v0)) {
      case code_356: /* elem() */
        v2= GgetArgument(v0,0);
        switch(GgetSymb(v2)) {
        case code_354: /* cons_variable(,) */
          v3= GgetArgument(v2,0);
          switch(GgetSymb(v3)) {
          default:
          label1615:
            v4= GgetArgument(v2,1);
            switch(GgetSymb(v4)) {
            default:
            label1616:
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
        Gterm *tmp, *sv[1];
        multiplicityType *E,*sol;
        Gterm *substitution[1];
        /* lhs: elem()(cons_variable(,)(var0,var1)) */
        /* allDetEvaluation: det */
        /* rhs: elem()(var1) */
        // this=var1        underAC=false        Instantiated=false
        GmakeAppl1(sv[0],code_356        ,v4        );
        res = sv[0] ;
        rewrite_step++;
        rewrite_label_step++;
        goto stratLab80;
        myend363:;
      }
    }
    fail();
    stratLab80:;
    v0=res;
  }
  {
    /* one[semiDet](extractrule1:variable/list[variable]!LO) */
    Gterm *v1,*v2,*v3,*v4,*v5;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_356: /* elem() */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_354: /* cons_variable(,) */
        v3= GgetArgument(v2,0);
        switch(GgetSymb(v3)) {
        default:
        label1620:
          v4= GgetArgument(v2,1);
          switch(GgetSymb(v4)) {
          default:
          label1621:
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
      Gterm *tmp, *sv[1];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: elem()(cons_variable(,)(var0,var1)) */
      /* allDetEvaluation: det */
      /* rhs: var0 */
      // this=var0        underAC=false        Instantiated=false
      res = v3 ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab83;
      myend364:;
    }
  }
  fail();
  stratLab83:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_58( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  /* cons[semiDet](one[semiDet](trueadd:eqSystem/syntacticUnification[Ops,Vars]!GL),repeat[det](one[semiDet](unfold:eqSystem/eqSystem[Ops,Vars]!GL,delete:eqSystem/syntacticUnification[Ops,Vars]!GL,decompose:eqSystem/syntacticUnification[Ops,Vars]!LO,conflict:eqSystem/syntacticUnification[Ops,Vars]!GL,matchClash:eqSystem/syntacticUnification[Ops,Vars]!GL,mergePass1:eqSystem/syntacticUnification[Ops,Vars]!GL)),one[semiDet](truepass:eqSystem/syntacticUnification[Ops,Vars]!GL,truepropag:eqSystem/syntacticUnification[Ops,Vars]!GL,falsepropag:eqSystem/syntacticUnification[Ops,Vars]!GL),repeat[det](one[semiDet](unfold:eqSystem/eqSystem[Ops,Vars]!GL,mergeClash:eqSystem/syntacticUnification[Ops,Vars]!GL,mergePass2:eqSystem/syntacticUnification[Ops,Vars]!GL,mergePass3:eqSystem/syntacticUnification[Ops,Vars]!GL)),one[semiDet](trueelim:eqSystem/syntacticUnification[Ops,Vars]!GL,truepropag:eqSystem/syntacticUnification[Ops,Vars]!GL,falsepropag:eqSystem/syntacticUnification[Ops,Vars]!GL)) */
  {
    /* one[semiDet](trueadd:eqSystem/syntacticUnification[Ops,Vars]!GL) */
    Gterm *v1,*v2;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    default:
    label1623:
      bitSet_set(mask,0);
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[2];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: var0 */
      /* allDetEvaluation: det */
      /* rhs: (&)((true),var0) */
      GmakeAppl1(sv[1],code_424      ,con_1      );
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl2(sv[0],code_426      ,sv[1]      ,v0      );
      res = sv[0] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab49;
      myend365:;
    }
  }
  fail();
  stratLab49:;
  v0=res;
  {
    /* repeat[det](one[semiDet](unfold:eqSystem/eqSystem[Ops,Vars]!GL,delete:eqSystem/syntacticUnification[Ops,Vars]!GL,decompose:eqSystem/syntacticUnification[Ops,Vars]!LO,conflict:eqSystem/syntacticUnification[Ops,Vars]!GL,matchClash:eqSystem/syntacticUnification[Ops,Vars]!GL,mergePass1:eqSystem/syntacticUnification[Ops,Vars]!GL)) */
    long lastTerm_index;
    CUTOPEN(); /* Repeat */
    lastTerm_index = allocStable(sizeof(Gterm*));
    *((Gterm**)getStablePointer(lastTerm_index))=v0;
    if(localSetChoicePoint()!=0) {
      res = v0 = *((Gterm**)getStablePointer(lastTerm_index));
      /* End of repeat */
    } else {
      while(1) {
      /* Apply the strategy */
        {
          /* one[semiDet](unfold:eqSystem/eqSystem[Ops,Vars]!GL,delete:eqSystem/syntacticUnification[Ops,Vars]!GL,decompose:eqSystem/syntacticUnification[Ops,Vars]!LO,conflict:eqSystem/syntacticUnification[Ops,Vars]!GL,matchClash:eqSystem/syntacticUnification[Ops,Vars]!GL,mergePass1:eqSystem/syntacticUnification[Ops,Vars]!GL) */
          Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16,*v17,*v18,*v19,*v20,*v21,*v22,*v23,*v24,*v25,*v26,*v27,*v28,*v29,*v30,*v31,*v32,*v33,*v34,*v35,*v36,*v37,*v38,*v39,*v40,*v41,*v42,*v43,*v44,*v45,*v46,*v47,*v48,*v49,*v50;
          bitSet_GC_create(mask,17);
          bitSet_init_clear(mask);
          switch(GgetSymb(v0)) {
          case code_426: /* (&) */
            v2= GgetArgument(v0,0);
            switch(GgetSymb(v2)) {
            default:
            label1626:
              v3= GgetArgument(v0,1);
              switch(GgetSymb(v3)) {
              case code_424: /*  */
                v4= GgetArgument(v3,0);
                switch(GgetSymb(v4)) {
                case code_1: /* true */
                  bitSet_set(mask,2);
                  break;
                /* matching is not complete: jumpNode is null */
                }
                break;
              case code_425: /*  */
                v5= GgetArgument(v3,0);
                switch(GgetSymb(v5)) {
                case code_423: /* = */
                  v6= GgetArgument(v5,0);
                  switch(GgetSymb(v6)) {
                  case code_380: /* i5() */
                    v7= GgetArgument(v6,0);
                    switch(GgetSymb(v7)) {
                    default:
                    label1671:
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_380: /* i5() */
                        v9= GgetArgument(v8,0);
                        switch(GgetSymb(v9)) {
                        default:
                        label1673:
                          bitSet_set(mask,13);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1709:
                          bitSet_set(mask,14);
                          bitSet_set(mask,15);
                        }
                        break;
                      default:
                      label1672:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_379: /* i4() */
                    v11= GgetArgument(v6,0);
                    switch(GgetSymb(v11)) {
                    default:
                    label1667:
                      v12= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_379: /* i4() */
                        v13= GgetArgument(v12,0);
                        switch(GgetSymb(v13)) {
                        default:
                        label1669:
                          bitSet_set(mask,12);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1707:
                          bitSet_set(mask,14);
                          bitSet_set(mask,15);
                        }
                        break;
                      default:
                      label1668:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_378: /* i3() */
                    v15= GgetArgument(v6,0);
                    switch(GgetSymb(v15)) {
                    default:
                    label1663:
                      v16= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_378: /* i3() */
                        v17= GgetArgument(v16,0);
                        switch(GgetSymb(v17)) {
                        default:
                        label1665:
                          bitSet_set(mask,11);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1705:
                          bitSet_set(mask,14);
                          bitSet_set(mask,15);
                        }
                        break;
                      default:
                      label1664:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_377: /* i2() */
                    v19= GgetArgument(v6,0);
                    switch(GgetSymb(v19)) {
                    default:
                    label1659:
                      v20= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_377: /* i2() */
                        v21= GgetArgument(v20,0);
                        switch(GgetSymb(v21)) {
                        default:
                        label1661:
                          bitSet_set(mask,10);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1703:
                          bitSet_set(mask,14);
                          bitSet_set(mask,15);
                        }
                        break;
                      default:
                      label1660:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_376: /* i1() */
                    v23= GgetArgument(v6,0);
                    switch(GgetSymb(v23)) {
                    default:
                    label1655:
                      v24= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_376: /* i1() */
                        v25= GgetArgument(v24,0);
                        switch(GgetSymb(v25)) {
                        default:
                        label1657:
                          bitSet_set(mask,9);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1701:
                          bitSet_set(mask,14);
                          bitSet_set(mask,15);
                        }
                        break;
                      default:
                      label1656:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_374: /* e5 */
                    v27= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_374: /* e5 */
                      bitSet_set(mask,8);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label1699:
                        bitSet_set(mask,14);
                        bitSet_set(mask,15);
                      }
                      break;
                    default:
                    label1653:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_373: /* e4 */
                    v29= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_373: /* e4 */
                      bitSet_set(mask,7);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label1697:
                        bitSet_set(mask,14);
                        bitSet_set(mask,15);
                      }
                      break;
                    default:
                    label1651:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_372: /* e3 */
                    v31= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_372: /* e3 */
                      bitSet_set(mask,6);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label1695:
                        bitSet_set(mask,14);
                        bitSet_set(mask,15);
                      }
                      break;
                    default:
                    label1649:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_371: /* e2 */
                    v33= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_371: /* e2 */
                      bitSet_set(mask,5);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label1693:
                        bitSet_set(mask,14);
                        bitSet_set(mask,15);
                      }
                      break;
                    default:
                    label1647:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_370: /* e1 */
                    v35= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_370: /* e1 */
                      bitSet_set(mask,4);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label1691:
                        bitSet_set(mask,14);
                        bitSet_set(mask,15);
                      }
                      break;
                    default:
                    label1645:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_375: /* f(,) */
                    v37= GgetArgument(v6,0);
                    switch(GgetSymb(v37)) {
                    default:
                    label1639:
                      v38= GgetArgument(v6,1);
                      switch(GgetSymb(v38)) {
                      default:
                      label1640:
                        v39= GgetArgument(v5,1);
                        v8= GgetArgument(v5,1);
                        switch(GgetSymb(v8)) {
                        case code_375: /* f(,) */
                          v40= GgetArgument(v39,0);
                          switch(GgetSymb(v40)) {
                          default:
                          label1642:
                            v41= GgetArgument(v39,1);
                            switch(GgetSymb(v41)) {
                            default:
                            label1643:
                              bitSet_set(mask,3);
                              bitSet_set(mask,14);
                            }
                          }
                          break;
                        case code_362: /*  */
                          v10= GgetArgument(v8,0);
                          switch(GgetSymb(v10)) {
                          default:
                          label1689:
                            bitSet_set(mask,14);
                            bitSet_set(mask,15);
                          }
                          break;
                        default:
                        label1641:
                          bitSet_set(mask,14);
                        }
                      }
                    }
                    break;
                  case code_362: /*  */
                    v43= GgetArgument(v6,0);
                    switch(GgetSymb(v43)) {
                    default:
                    label1633:
                      v44= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v44)) {
                      case code_362: /*  */
                        v45= GgetArgument(v44,0);
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1635:
                          bitSet_set(mask,1);
                          bitSet_set(mask,14);
                          bitSet_set(mask,15);
                          bitSet_set(mask,16);
                        }
                        break;
                      default:
                      label1634:
                        bitSet_set(mask,14);
                        bitSet_set(mask,16);
                      }
                    }
                    break;
                  default:
                  label1632:
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label1711:
                        bitSet_set(mask,14);
                        bitSet_set(mask,15);
                      }
                      break;
                    default:
                    label1675:
                      bitSet_set(mask,14);
                    }
                  }
                  break;
                /* matching is not complete: jumpNode is null */
                }
                break;
              case code_426: /* (&) */
                v48= GgetArgument(v3,0);
                switch(GgetSymb(v48)) {
                default:
                label1628:
                  v49= GgetArgument(v3,1);
                  switch(GgetSymb(v49)) {
                  default:
                  label1629:
                    bitSet_set(mask,0);
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
            break;
          /* matching is not complete: jumpNode is null */
          }
          if(bitSet_get(mask,0)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(&)(var1,var2)) */
            /* allDetEvaluation: det */
            /* rhs: (&)((&)(var0,var1),var2) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_426            ,v2            ,v48            );
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_426            ,sv[0]            ,v49            );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab56;
            myend366:;
          }
          if(bitSet_get(mask,1)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=((var1),(var2)))) */
            /* allDetEvaluation: det */
            /* if eq_list[int](,)(var1,var2) */
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            sv[0] = fun_18( v43,v45 );
            if( sv[0] != con_1 ) {
              goto myend367;
            }
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab56;
            myend367:;
          }
          if(bitSet_get(mask,2)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(true)) */
            /* allDetEvaluation: det */
            /* if occurs(,)(true,var0) */
            // this=var0        underAC=false        Instantiated=false
            sv[1] = fun_17( con_1,v2 );
            if( sv[1] != con_1 ) {
              goto myend368;
            }
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab56;
            myend368:;
          }
          if(bitSet_get(mask,3)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(f(,)(var1,var2),f(,)(var3,var4)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)((&)(var0,(=(var1,var3))),(=(var2,var4))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v37            ,v40            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            // this=var2        underAC=false        Instantiated=false
            // this=var4        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_423            ,v38            ,v41            );
            GmakeAppl1(sv[2],code_425            ,sv[1]            );
            GmakeAppl2(sv[1],code_426            ,sv[0]            ,sv[2]            );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab56;
            myend369:;
          }
          if(bitSet_get(mask,4)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e1,e1))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab56;
            myend370:;
          }
          if(bitSet_get(mask,5)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e2,e2))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab56;
            myend371:;
          }
          if(bitSet_get(mask,6)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e3,e3))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab56;
            myend372:;
          }
          if(bitSet_get(mask,7)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e4,e4))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab56;
            myend373:;
          }
          if(bitSet_get(mask,8)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e5,e5))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab56;
            myend374:;
          }
          if(bitSet_get(mask,9)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i1()(var1),i1()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v23            ,v25            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab56;
            myend375:;
          }
          if(bitSet_get(mask,10)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i2()(var1),i2()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v19            ,v21            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab56;
            myend376:;
          }
          if(bitSet_get(mask,11)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i3()(var1),i3()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v15            ,v17            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab56;
            myend377:;
          }
          if(bitSet_get(mask,12)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i4()(var1),i4()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v11            ,v13            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab56;
            myend378:;
          }
          if(bitSet_get(mask,13)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i5()(var1),i5()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v7            ,v9            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab56;
            myend379:;
          }
          if(bitSet_get(mask,14)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(var1,var2))) */
            /* allDetEvaluation: det */
            /* if (and)((and)(neq_list[int](,)(head()(var1),head()(var2)),(not())(isvar()(var1))),(not())(isvar()(var2))) */
            // this=var1        underAC=false        Instantiated=false
            sv[0] = fun_363( v6 );
            // this=var2        underAC=false        Instantiated=false
            sv[1] = fun_363( v8 );
            sv[2] = fun_19( sv[0],sv[1] );
            // this=var1        underAC=false        Instantiated=false
            sv[0] = fun_365( v6 );
            sv[1] = fun_24( sv[0] );
            sv[0] = fun_21( sv[2],sv[1] );
            // this=var2        underAC=false        Instantiated=false
            sv[1] = fun_365( v8 );
            sv[2] = fun_24( sv[1] );
            sv[1] = fun_21( sv[0],sv[2] );
            if( sv[1] != con_1 ) {
              goto myend380;
            }
            /* rhs: (false) */
            GmakeAppl1(sv[2],code_424            ,con_0            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab56;
            myend380:;
          }
          if(bitSet_get(mask,15)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(var1,(var2)))) */
            /* allDetEvaluation: det */
            /* if (not())(isvar()(var1)) */
            // this=var1        underAC=false        Instantiated=false
            sv[0] = fun_365( v6 );
            sv[1] = fun_24( sv[0] );
            if( sv[1] != con_1 ) {
              goto myend381;
            }
            /* rhs: (false) */
            GmakeAppl1(sv[2],code_424            ,con_0            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab56;
            myend381:;
          }
          if(bitSet_get(mask,16)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=((var1),var2))) */
            /* allDetEvaluation: det */
            /* rhs: (&)((=((var1),var2)),var0) */
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl1(sv[0],code_362            ,v43            );
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_423            ,sv[0]            ,v44            );
            GmakeAppl1(sv[0],code_425            ,sv[1]            );
            // this=var0        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_426            ,sv[0]            ,v2            );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab56;
            myend382:;
          }
        }
        fail();
        stratLab56:;
        v0=res;
        *((Gterm**)getStablePointer(lastTerm_index)) = res;
      }
    }
    CUTCLOSE(); /* Repeat */
  }
  {
    /* one[semiDet](truepass:eqSystem/syntacticUnification[Ops,Vars]!GL,truepropag:eqSystem/syntacticUnification[Ops,Vars]!GL,falsepropag:eqSystem/syntacticUnification[Ops,Vars]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6;
    bitSet_GC_create(mask,3);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_424: /*  */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_0: /* false */
        bitSet_set(mask,2);
        break;
      case code_1: /* true */
        bitSet_set(mask,1);
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    case code_426: /* (&) */
      v3= GgetArgument(v0,0);
      switch(GgetSymb(v3)) {
      default:
      label1714:
        v4= GgetArgument(v0,1);
        switch(GgetSymb(v4)) {
        case code_424: /*  */
          v5= GgetArgument(v4,0);
          switch(GgetSymb(v5)) {
          case code_1: /* true */
            bitSet_set(mask,0);
            break;
          /* matching is not complete: jumpNode is null */
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[2];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: (&)(var0,(true)) */
      /* allDetEvaluation: det */
      /* rhs: (&)((true),var0) */
      GmakeAppl1(sv[1],code_424      ,con_1      );
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl2(sv[0],code_426      ,sv[1]      ,v3      );
      res = sv[0] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab61;
      myend383:;
    }
    if(bitSet_get(mask,1)) {
      Gterm *tmp, *sv[2];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: (true) */
      /* allDetEvaluation: det */
      /* rhs: (true) */
      GmakeAppl1(sv[1],code_424      ,con_1      );
      res = sv[1] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab61;
      myend384:;
    }
    if(bitSet_get(mask,2)) {
      Gterm *tmp, *sv[2];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: (false) */
      /* allDetEvaluation: det */
      /* rhs: (false) */
      GmakeAppl1(sv[1],code_424      ,con_0      );
      res = sv[1] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab61;
      myend385:;
    }
  }
  fail();
  stratLab61:;
  v0=res;
  {
    /* repeat[det](one[semiDet](unfold:eqSystem/eqSystem[Ops,Vars]!GL,mergeClash:eqSystem/syntacticUnification[Ops,Vars]!GL,mergePass2:eqSystem/syntacticUnification[Ops,Vars]!GL,mergePass3:eqSystem/syntacticUnification[Ops,Vars]!GL)) */
    long lastTerm_index;
    CUTOPEN(); /* Repeat */
    lastTerm_index = allocStable(sizeof(Gterm*));
    *((Gterm**)getStablePointer(lastTerm_index))=v0;
    if(localSetChoicePoint()!=0) {
      res = v0 = *((Gterm**)getStablePointer(lastTerm_index));
      /* End of repeat */
    } else {
      while(1) {
      /* Apply the strategy */
        {
          /* one[semiDet](unfold:eqSystem/eqSystem[Ops,Vars]!GL,mergeClash:eqSystem/syntacticUnification[Ops,Vars]!GL,mergePass2:eqSystem/syntacticUnification[Ops,Vars]!GL,mergePass3:eqSystem/syntacticUnification[Ops,Vars]!GL) */
          Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
          bitSet_GC_create(mask,4);
          bitSet_init_clear(mask);
          switch(GgetSymb(v0)) {
          case code_426: /* (&) */
            v2= GgetArgument(v0,0);
            switch(GgetSymb(v2)) {
            default:
            label1722:
              v3= GgetArgument(v0,1);
              switch(GgetSymb(v3)) {
              case code_425: /*  */
                v4= GgetArgument(v3,0);
                switch(GgetSymb(v4)) {
                case code_423: /* = */
                  v5= GgetArgument(v4,0);
                  switch(GgetSymb(v5)) {
                  case code_362: /*  */
                    v6= GgetArgument(v5,0);
                    switch(GgetSymb(v6)) {
                    default:
                    label1729:
                      v7= GgetArgument(v4,1);
                      switch(GgetSymb(v7)) {
                      default:
                      label1730:
                        bitSet_set(mask,1);
                        bitSet_set(mask,2);
                        bitSet_set(mask,3);
                      }
                    }
                    break;
                  default:
                    goto label1721;
                  }
                  break;
                default:
                  goto label1721;
                }
                break;
              case code_426: /* (&) */
                v8= GgetArgument(v3,0);
                switch(GgetSymb(v8)) {
                default:
                label1724:
                  v9= GgetArgument(v3,1);
                  switch(GgetSymb(v9)) {
                  default:
                  label1725:
                    bitSet_set(mask,0);
                    bitSet_set(mask,3);
                  }
                }
                break;
              default:
                goto label1721;
              }
            }
            break;
          default:
          label1721:
            bitSet_set(mask,3);
          }
          if(bitSet_get(mask,0)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(&)(var1,var2)) */
            /* allDetEvaluation: det */
            /* rhs: (&)((&)(var0,var1),var2) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_426            ,v2            ,v8            );
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_426            ,sv[0]            ,v9            );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab66;
            myend386:;
          }
          if(bitSet_get(mask,1)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=((var1),var2))) */
            /* allDetEvaluation: det */
            /* if mergingClash(,,)(var0,var1,var2) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            sv[0] = fun_454( v2,v6,v7 );
            if( sv[0] != con_1 ) {
              goto myend387;
            }
            /* rhs: (false) */
            GmakeAppl1(sv[2],code_424            ,con_0            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab66;
            myend387:;
          }
          if(bitSet_get(mask,2)) {
            Gterm *tmp, *sv[6];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=((var1),var2))) */
            /* allDetEvaluation: nonDet */
            CUTOPEN(); /* Wheres */
            if(localSetChoicePoint()) {
              /* local evaluations failed, try next rule */
              goto myend388;
            }
            /* where var4 := occurs(,)(var1,var0) */
            // this=var1        underAC=false        Instantiated=false
            // this=var0        underAC=false        Instantiated=false
            sv[1] = fun_17( v6,v2 );
            tmp = sv[0] = sv[1];
            if(!setChoicePoint()) {
              /* try branch 0
                  if (not())(var4)
                  where var3 := (&)((=((var1),var2)),var0)
               */
              /* if (not())(var4) */
              // this=var4        underAC=false        Instantiated=false
              sv[2] = fun_24( sv[0] );
              if( sv[2] != con_1 ) {
                fail();
              }
              /* where var3 := (&)((=((var1),var2)),var0) */
              // this=var1        underAC=false        Instantiated=false
              GmakeAppl1(sv[4],code_362              ,v6              );
              // this=var2        underAC=false        Instantiated=false
              GmakeAppl2(sv[5],code_423              ,sv[4]              ,v7              );
              GmakeAppl1(sv[4],code_425              ,sv[5]              );
              // this=var0        underAC=false        Instantiated=false
              GmakeAppl2(sv[5],code_426              ,sv[4]              ,v2              );
              tmp = sv[3] = sv[5];
            } else { 
              if(!setChoicePoint()) {
                /* try branch 1
                    if var4
                    where var3 := var0
                 */
                /* if var4 */
                // this=var4        underAC=false        Instantiated=false
                if( sv[0] != con_1 ) {
                  fail();
                }
                /* where var3 := var0 */
                // this=var0        underAC=false        Instantiated=false
                tmp = sv[3] = v2;
              } else { 
                fail();
              }
            }
            CUTCLOSE(); /* Wheres */
            /* rhs: var3 */
            // this=var3        underAC=false        Instantiated=false
            res = sv[3] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab66;
            myend388:;
            CUTCLOSE(); /* Wheres */
          }
          if(bitSet_get(mask,3)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: var0 */
            /* allDetEvaluation: det */
            /* if false */
            if( con_0 != con_1 ) {
              fail();
            }
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v0 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab66;
            myend389:;
          }
        }
        fail();
        stratLab66:;
        v0=res;
        *((Gterm**)getStablePointer(lastTerm_index)) = res;
      }
    }
    CUTCLOSE(); /* Repeat */
  }
  {
    /* one[semiDet](trueelim:eqSystem/syntacticUnification[Ops,Vars]!GL,truepropag:eqSystem/syntacticUnification[Ops,Vars]!GL,falsepropag:eqSystem/syntacticUnification[Ops,Vars]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6;
    bitSet_GC_create(mask,3);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_424: /*  */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_0: /* false */
        bitSet_set(mask,2);
        break;
      case code_1: /* true */
        bitSet_set(mask,1);
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    case code_426: /* (&) */
      v3= GgetArgument(v0,0);
      switch(GgetSymb(v3)) {
      default:
      label1734:
        v4= GgetArgument(v0,1);
        switch(GgetSymb(v4)) {
        case code_424: /*  */
          v5= GgetArgument(v4,0);
          switch(GgetSymb(v5)) {
          case code_1: /* true */
            bitSet_set(mask,0);
            break;
          /* matching is not complete: jumpNode is null */
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[1];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: (&)(var0,(true)) */
      /* allDetEvaluation: det */
      /* rhs: var0 */
      // this=var0        underAC=false        Instantiated=false
      res = v3 ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab71;
      myend390:;
    }
    if(bitSet_get(mask,1)) {
      Gterm *tmp, *sv[2];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: (true) */
      /* allDetEvaluation: det */
      /* rhs: (true) */
      GmakeAppl1(sv[1],code_424      ,con_1      );
      res = sv[1] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab71;
      myend391:;
    }
    if(bitSet_get(mask,2)) {
      Gterm *tmp, *sv[2];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: (false) */
      /* allDetEvaluation: det */
      /* rhs: (false) */
      GmakeAppl1(sv[1],code_424      ,con_0      );
      res = sv[1] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab71;
      myend392:;
    }
  }
  fail();
  stratLab71:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_173( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  /* cons[semiDet](repeat[det](one[semiDet](unfold:eqSystem/eqSystem[Ops,Vars]!GL,delete:eqSystem/syntacticUnification[Ops,Vars]!GL,decompose:eqSystem/syntacticUnification[Ops,Vars]!LO,conflict:eqSystem/syntacticUnification[Ops,Vars]!GL,matchClash:eqSystem/syntacticUnification[Ops,Vars]!GL,mergePass1:eqSystem/syntacticUnification[Ops,Vars]!GL)),one[semiDet](truepass:eqSystem/syntacticUnification[Ops,Vars]!GL,truepropag:eqSystem/syntacticUnification[Ops,Vars]!GL),repeat[det](one[semiDet](unfold:eqSystem/eqSystem[Ops,Vars]!GL,mergeClash:eqSystem/syntacticUnification[Ops,Vars]!GL,mergePass2:eqSystem/syntacticUnification[Ops,Vars]!GL,mergePass3:eqSystem/syntacticUnification[Ops,Vars]!GL)),one[semiDet](truepass:eqSystem/syntacticUnification[Ops,Vars]!GL,truepropag:eqSystem/syntacticUnification[Ops,Vars]!GL)) */
  {
    /* repeat[det](one[semiDet](unfold:eqSystem/eqSystem[Ops,Vars]!GL,delete:eqSystem/syntacticUnification[Ops,Vars]!GL,decompose:eqSystem/syntacticUnification[Ops,Vars]!LO,conflict:eqSystem/syntacticUnification[Ops,Vars]!GL,matchClash:eqSystem/syntacticUnification[Ops,Vars]!GL,mergePass1:eqSystem/syntacticUnification[Ops,Vars]!GL)) */
    long lastTerm_index;
    CUTOPEN(); /* Repeat */
    lastTerm_index = allocStable(sizeof(Gterm*));
    *((Gterm**)getStablePointer(lastTerm_index))=v0;
    if(localSetChoicePoint()!=0) {
      res = v0 = *((Gterm**)getStablePointer(lastTerm_index));
      /* End of repeat */
    } else {
      while(1) {
      /* Apply the strategy */
        {
          /* one[semiDet](unfold:eqSystem/eqSystem[Ops,Vars]!GL,delete:eqSystem/syntacticUnification[Ops,Vars]!GL,decompose:eqSystem/syntacticUnification[Ops,Vars]!LO,conflict:eqSystem/syntacticUnification[Ops,Vars]!GL,matchClash:eqSystem/syntacticUnification[Ops,Vars]!GL,mergePass1:eqSystem/syntacticUnification[Ops,Vars]!GL) */
          Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16,*v17,*v18,*v19,*v20,*v21,*v22,*v23,*v24,*v25,*v26,*v27,*v28,*v29,*v30,*v31,*v32,*v33,*v34,*v35,*v36,*v37,*v38,*v39,*v40,*v41,*v42,*v43,*v44,*v45,*v46,*v47,*v48,*v49,*v50;
          bitSet_GC_create(mask,17);
          bitSet_init_clear(mask);
          switch(GgetSymb(v0)) {
          case code_426: /* (&) */
            v2= GgetArgument(v0,0);
            switch(GgetSymb(v2)) {
            default:
            label1742:
              v3= GgetArgument(v0,1);
              switch(GgetSymb(v3)) {
              case code_424: /*  */
                v4= GgetArgument(v3,0);
                switch(GgetSymb(v4)) {
                case code_1: /* true */
                  bitSet_set(mask,2);
                  break;
                /* matching is not complete: jumpNode is null */
                }
                break;
              case code_425: /*  */
                v5= GgetArgument(v3,0);
                switch(GgetSymb(v5)) {
                case code_423: /* = */
                  v6= GgetArgument(v5,0);
                  switch(GgetSymb(v6)) {
                  case code_380: /* i5() */
                    v7= GgetArgument(v6,0);
                    switch(GgetSymb(v7)) {
                    default:
                    label1787:
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_380: /* i5() */
                        v9= GgetArgument(v8,0);
                        switch(GgetSymb(v9)) {
                        default:
                        label1789:
                          bitSet_set(mask,13);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1825:
                          bitSet_set(mask,14);
                          bitSet_set(mask,15);
                        }
                        break;
                      default:
                      label1788:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_379: /* i4() */
                    v11= GgetArgument(v6,0);
                    switch(GgetSymb(v11)) {
                    default:
                    label1783:
                      v12= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_379: /* i4() */
                        v13= GgetArgument(v12,0);
                        switch(GgetSymb(v13)) {
                        default:
                        label1785:
                          bitSet_set(mask,12);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1823:
                          bitSet_set(mask,14);
                          bitSet_set(mask,15);
                        }
                        break;
                      default:
                      label1784:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_378: /* i3() */
                    v15= GgetArgument(v6,0);
                    switch(GgetSymb(v15)) {
                    default:
                    label1779:
                      v16= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_378: /* i3() */
                        v17= GgetArgument(v16,0);
                        switch(GgetSymb(v17)) {
                        default:
                        label1781:
                          bitSet_set(mask,11);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1821:
                          bitSet_set(mask,14);
                          bitSet_set(mask,15);
                        }
                        break;
                      default:
                      label1780:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_377: /* i2() */
                    v19= GgetArgument(v6,0);
                    switch(GgetSymb(v19)) {
                    default:
                    label1775:
                      v20= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_377: /* i2() */
                        v21= GgetArgument(v20,0);
                        switch(GgetSymb(v21)) {
                        default:
                        label1777:
                          bitSet_set(mask,10);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1819:
                          bitSet_set(mask,14);
                          bitSet_set(mask,15);
                        }
                        break;
                      default:
                      label1776:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_376: /* i1() */
                    v23= GgetArgument(v6,0);
                    switch(GgetSymb(v23)) {
                    default:
                    label1771:
                      v24= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_376: /* i1() */
                        v25= GgetArgument(v24,0);
                        switch(GgetSymb(v25)) {
                        default:
                        label1773:
                          bitSet_set(mask,9);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1817:
                          bitSet_set(mask,14);
                          bitSet_set(mask,15);
                        }
                        break;
                      default:
                      label1772:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_374: /* e5 */
                    v27= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_374: /* e5 */
                      bitSet_set(mask,8);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label1815:
                        bitSet_set(mask,14);
                        bitSet_set(mask,15);
                      }
                      break;
                    default:
                    label1769:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_373: /* e4 */
                    v29= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_373: /* e4 */
                      bitSet_set(mask,7);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label1813:
                        bitSet_set(mask,14);
                        bitSet_set(mask,15);
                      }
                      break;
                    default:
                    label1767:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_372: /* e3 */
                    v31= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_372: /* e3 */
                      bitSet_set(mask,6);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label1811:
                        bitSet_set(mask,14);
                        bitSet_set(mask,15);
                      }
                      break;
                    default:
                    label1765:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_371: /* e2 */
                    v33= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_371: /* e2 */
                      bitSet_set(mask,5);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label1809:
                        bitSet_set(mask,14);
                        bitSet_set(mask,15);
                      }
                      break;
                    default:
                    label1763:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_370: /* e1 */
                    v35= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_370: /* e1 */
                      bitSet_set(mask,4);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label1807:
                        bitSet_set(mask,14);
                        bitSet_set(mask,15);
                      }
                      break;
                    default:
                    label1761:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_375: /* f(,) */
                    v37= GgetArgument(v6,0);
                    switch(GgetSymb(v37)) {
                    default:
                    label1755:
                      v38= GgetArgument(v6,1);
                      switch(GgetSymb(v38)) {
                      default:
                      label1756:
                        v39= GgetArgument(v5,1);
                        v8= GgetArgument(v5,1);
                        switch(GgetSymb(v8)) {
                        case code_375: /* f(,) */
                          v40= GgetArgument(v39,0);
                          switch(GgetSymb(v40)) {
                          default:
                          label1758:
                            v41= GgetArgument(v39,1);
                            switch(GgetSymb(v41)) {
                            default:
                            label1759:
                              bitSet_set(mask,3);
                              bitSet_set(mask,14);
                            }
                          }
                          break;
                        case code_362: /*  */
                          v10= GgetArgument(v8,0);
                          switch(GgetSymb(v10)) {
                          default:
                          label1805:
                            bitSet_set(mask,14);
                            bitSet_set(mask,15);
                          }
                          break;
                        default:
                        label1757:
                          bitSet_set(mask,14);
                        }
                      }
                    }
                    break;
                  case code_362: /*  */
                    v43= GgetArgument(v6,0);
                    switch(GgetSymb(v43)) {
                    default:
                    label1749:
                      v44= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v44)) {
                      case code_362: /*  */
                        v45= GgetArgument(v44,0);
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label1751:
                          bitSet_set(mask,1);
                          bitSet_set(mask,14);
                          bitSet_set(mask,15);
                          bitSet_set(mask,16);
                        }
                        break;
                      default:
                      label1750:
                        bitSet_set(mask,14);
                        bitSet_set(mask,16);
                      }
                    }
                    break;
                  default:
                  label1748:
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label1827:
                        bitSet_set(mask,14);
                        bitSet_set(mask,15);
                      }
                      break;
                    default:
                    label1791:
                      bitSet_set(mask,14);
                    }
                  }
                  break;
                /* matching is not complete: jumpNode is null */
                }
                break;
              case code_426: /* (&) */
                v48= GgetArgument(v3,0);
                switch(GgetSymb(v48)) {
                default:
                label1744:
                  v49= GgetArgument(v3,1);
                  switch(GgetSymb(v49)) {
                  default:
                  label1745:
                    bitSet_set(mask,0);
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
            break;
          /* matching is not complete: jumpNode is null */
          }
          if(bitSet_get(mask,0)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(&)(var1,var2)) */
            /* allDetEvaluation: det */
            /* rhs: (&)((&)(var0,var1),var2) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_426            ,v2            ,v48            );
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_426            ,sv[0]            ,v49            );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab142;
            myend393:;
          }
          if(bitSet_get(mask,1)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=((var1),(var2)))) */
            /* allDetEvaluation: det */
            /* if eq_list[int](,)(var1,var2) */
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            sv[0] = fun_18( v43,v45 );
            if( sv[0] != con_1 ) {
              goto myend394;
            }
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab142;
            myend394:;
          }
          if(bitSet_get(mask,2)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(true)) */
            /* allDetEvaluation: det */
            /* if occurs(,)(true,var0) */
            // this=var0        underAC=false        Instantiated=false
            sv[1] = fun_17( con_1,v2 );
            if( sv[1] != con_1 ) {
              goto myend395;
            }
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab142;
            myend395:;
          }
          if(bitSet_get(mask,3)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(f(,)(var1,var2),f(,)(var3,var4)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)((&)(var0,(=(var1,var3))),(=(var2,var4))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v37            ,v40            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            // this=var2        underAC=false        Instantiated=false
            // this=var4        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_423            ,v38            ,v41            );
            GmakeAppl1(sv[2],code_425            ,sv[1]            );
            GmakeAppl2(sv[1],code_426            ,sv[0]            ,sv[2]            );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab142;
            myend396:;
          }
          if(bitSet_get(mask,4)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e1,e1))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab142;
            myend397:;
          }
          if(bitSet_get(mask,5)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e2,e2))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab142;
            myend398:;
          }
          if(bitSet_get(mask,6)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e3,e3))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab142;
            myend399:;
          }
          if(bitSet_get(mask,7)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e4,e4))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab142;
            myend400:;
          }
          if(bitSet_get(mask,8)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e5,e5))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab142;
            myend401:;
          }
          if(bitSet_get(mask,9)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i1()(var1),i1()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v23            ,v25            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab142;
            myend402:;
          }
          if(bitSet_get(mask,10)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i2()(var1),i2()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v19            ,v21            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab142;
            myend403:;
          }
          if(bitSet_get(mask,11)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i3()(var1),i3()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v15            ,v17            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab142;
            myend404:;
          }
          if(bitSet_get(mask,12)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i4()(var1),i4()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v11            ,v13            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab142;
            myend405:;
          }
          if(bitSet_get(mask,13)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i5()(var1),i5()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v7            ,v9            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab142;
            myend406:;
          }
          if(bitSet_get(mask,14)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(var1,var2))) */
            /* allDetEvaluation: det */
            /* if (and)((and)(neq_list[int](,)(head()(var1),head()(var2)),(not())(isvar()(var1))),(not())(isvar()(var2))) */
            // this=var1        underAC=false        Instantiated=false
            sv[0] = fun_363( v6 );
            // this=var2        underAC=false        Instantiated=false
            sv[1] = fun_363( v8 );
            sv[2] = fun_19( sv[0],sv[1] );
            // this=var1        underAC=false        Instantiated=false
            sv[0] = fun_365( v6 );
            sv[1] = fun_24( sv[0] );
            sv[0] = fun_21( sv[2],sv[1] );
            // this=var2        underAC=false        Instantiated=false
            sv[1] = fun_365( v8 );
            sv[2] = fun_24( sv[1] );
            sv[1] = fun_21( sv[0],sv[2] );
            if( sv[1] != con_1 ) {
              goto myend407;
            }
            /* rhs: (false) */
            GmakeAppl1(sv[2],code_424            ,con_0            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab142;
            myend407:;
          }
          if(bitSet_get(mask,15)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(var1,(var2)))) */
            /* allDetEvaluation: det */
            /* if (not())(isvar()(var1)) */
            // this=var1        underAC=false        Instantiated=false
            sv[0] = fun_365( v6 );
            sv[1] = fun_24( sv[0] );
            if( sv[1] != con_1 ) {
              goto myend408;
            }
            /* rhs: (false) */
            GmakeAppl1(sv[2],code_424            ,con_0            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab142;
            myend408:;
          }
          if(bitSet_get(mask,16)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=((var1),var2))) */
            /* allDetEvaluation: det */
            /* rhs: (&)((=((var1),var2)),var0) */
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl1(sv[0],code_362            ,v43            );
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_423            ,sv[0]            ,v44            );
            GmakeAppl1(sv[0],code_425            ,sv[1]            );
            // this=var0        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_426            ,sv[0]            ,v2            );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab142;
            myend409:;
          }
        }
        fail();
        stratLab142:;
        v0=res;
        *((Gterm**)getStablePointer(lastTerm_index)) = res;
      }
    }
    CUTCLOSE(); /* Repeat */
  }
  {
    /* one[semiDet](truepass:eqSystem/syntacticUnification[Ops,Vars]!GL,truepropag:eqSystem/syntacticUnification[Ops,Vars]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6;
    bitSet_GC_create(mask,2);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_424: /*  */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_1: /* true */
        bitSet_set(mask,1);
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    case code_426: /* (&) */
      v3= GgetArgument(v0,0);
      switch(GgetSymb(v3)) {
      default:
      label1830:
        v4= GgetArgument(v0,1);
        switch(GgetSymb(v4)) {
        case code_424: /*  */
          v5= GgetArgument(v4,0);
          switch(GgetSymb(v5)) {
          case code_1: /* true */
            bitSet_set(mask,0);
            break;
          /* matching is not complete: jumpNode is null */
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[2];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: (&)(var0,(true)) */
      /* allDetEvaluation: det */
      /* rhs: (&)((true),var0) */
      GmakeAppl1(sv[1],code_424      ,con_1      );
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl2(sv[0],code_426      ,sv[1]      ,v3      );
      res = sv[0] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab146;
      myend410:;
    }
    if(bitSet_get(mask,1)) {
      Gterm *tmp, *sv[2];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: (true) */
      /* allDetEvaluation: det */
      /* rhs: (true) */
      GmakeAppl1(sv[1],code_424      ,con_1      );
      res = sv[1] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab146;
      myend411:;
    }
  }
  fail();
  stratLab146:;
  v0=res;
  {
    /* repeat[det](one[semiDet](unfold:eqSystem/eqSystem[Ops,Vars]!GL,mergeClash:eqSystem/syntacticUnification[Ops,Vars]!GL,mergePass2:eqSystem/syntacticUnification[Ops,Vars]!GL,mergePass3:eqSystem/syntacticUnification[Ops,Vars]!GL)) */
    long lastTerm_index;
    CUTOPEN(); /* Repeat */
    lastTerm_index = allocStable(sizeof(Gterm*));
    *((Gterm**)getStablePointer(lastTerm_index))=v0;
    if(localSetChoicePoint()!=0) {
      res = v0 = *((Gterm**)getStablePointer(lastTerm_index));
      /* End of repeat */
    } else {
      while(1) {
      /* Apply the strategy */
        {
          /* one[semiDet](unfold:eqSystem/eqSystem[Ops,Vars]!GL,mergeClash:eqSystem/syntacticUnification[Ops,Vars]!GL,mergePass2:eqSystem/syntacticUnification[Ops,Vars]!GL,mergePass3:eqSystem/syntacticUnification[Ops,Vars]!GL) */
          Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
          bitSet_GC_create(mask,4);
          bitSet_init_clear(mask);
          switch(GgetSymb(v0)) {
          case code_426: /* (&) */
            v2= GgetArgument(v0,0);
            switch(GgetSymb(v2)) {
            default:
            label1837:
              v3= GgetArgument(v0,1);
              switch(GgetSymb(v3)) {
              case code_425: /*  */
                v4= GgetArgument(v3,0);
                switch(GgetSymb(v4)) {
                case code_423: /* = */
                  v5= GgetArgument(v4,0);
                  switch(GgetSymb(v5)) {
                  case code_362: /*  */
                    v6= GgetArgument(v5,0);
                    switch(GgetSymb(v6)) {
                    default:
                    label1844:
                      v7= GgetArgument(v4,1);
                      switch(GgetSymb(v7)) {
                      default:
                      label1845:
                        bitSet_set(mask,1);
                        bitSet_set(mask,2);
                        bitSet_set(mask,3);
                      }
                    }
                    break;
                  default:
                    goto label1836;
                  }
                  break;
                default:
                  goto label1836;
                }
                break;
              case code_426: /* (&) */
                v8= GgetArgument(v3,0);
                switch(GgetSymb(v8)) {
                default:
                label1839:
                  v9= GgetArgument(v3,1);
                  switch(GgetSymb(v9)) {
                  default:
                  label1840:
                    bitSet_set(mask,0);
                    bitSet_set(mask,3);
                  }
                }
                break;
              default:
                goto label1836;
              }
            }
            break;
          default:
          label1836:
            bitSet_set(mask,3);
          }
          if(bitSet_get(mask,0)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(&)(var1,var2)) */
            /* allDetEvaluation: det */
            /* rhs: (&)((&)(var0,var1),var2) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_426            ,v2            ,v8            );
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_426            ,sv[0]            ,v9            );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab151;
            myend412:;
          }
          if(bitSet_get(mask,1)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=((var1),var2))) */
            /* allDetEvaluation: det */
            /* if mergingClash(,,)(var0,var1,var2) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            sv[0] = fun_454( v2,v6,v7 );
            if( sv[0] != con_1 ) {
              goto myend413;
            }
            /* rhs: (false) */
            GmakeAppl1(sv[2],code_424            ,con_0            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab151;
            myend413:;
          }
          if(bitSet_get(mask,2)) {
            Gterm *tmp, *sv[6];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=((var1),var2))) */
            /* allDetEvaluation: nonDet */
            CUTOPEN(); /* Wheres */
            if(localSetChoicePoint()) {
              /* local evaluations failed, try next rule */
              goto myend414;
            }
            /* where var4 := occurs(,)(var1,var0) */
            // this=var1        underAC=false        Instantiated=false
            // this=var0        underAC=false        Instantiated=false
            sv[1] = fun_17( v6,v2 );
            tmp = sv[0] = sv[1];
            if(!setChoicePoint()) {
              /* try branch 0
                  if (not())(var4)
                  where var3 := (&)((=((var1),var2)),var0)
               */
              /* if (not())(var4) */
              // this=var4        underAC=false        Instantiated=false
              sv[2] = fun_24( sv[0] );
              if( sv[2] != con_1 ) {
                fail();
              }
              /* where var3 := (&)((=((var1),var2)),var0) */
              // this=var1        underAC=false        Instantiated=false
              GmakeAppl1(sv[4],code_362              ,v6              );
              // this=var2        underAC=false        Instantiated=false
              GmakeAppl2(sv[5],code_423              ,sv[4]              ,v7              );
              GmakeAppl1(sv[4],code_425              ,sv[5]              );
              // this=var0        underAC=false        Instantiated=false
              GmakeAppl2(sv[5],code_426              ,sv[4]              ,v2              );
              tmp = sv[3] = sv[5];
            } else { 
              if(!setChoicePoint()) {
                /* try branch 1
                    if var4
                    where var3 := var0
                 */
                /* if var4 */
                // this=var4        underAC=false        Instantiated=false
                if( sv[0] != con_1 ) {
                  fail();
                }
                /* where var3 := var0 */
                // this=var0        underAC=false        Instantiated=false
                tmp = sv[3] = v2;
              } else { 
                fail();
              }
            }
            CUTCLOSE(); /* Wheres */
            /* rhs: var3 */
            // this=var3        underAC=false        Instantiated=false
            res = sv[3] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab151;
            myend414:;
            CUTCLOSE(); /* Wheres */
          }
          if(bitSet_get(mask,3)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: var0 */
            /* allDetEvaluation: det */
            /* if false */
            if( con_0 != con_1 ) {
              fail();
            }
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v0 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab151;
            myend415:;
          }
        }
        fail();
        stratLab151:;
        v0=res;
        *((Gterm**)getStablePointer(lastTerm_index)) = res;
      }
    }
    CUTCLOSE(); /* Repeat */
  }
  {
    /* one[semiDet](truepass:eqSystem/syntacticUnification[Ops,Vars]!GL,truepropag:eqSystem/syntacticUnification[Ops,Vars]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6;
    bitSet_GC_create(mask,2);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_424: /*  */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_1: /* true */
        bitSet_set(mask,1);
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    case code_426: /* (&) */
      v3= GgetArgument(v0,0);
      switch(GgetSymb(v3)) {
      default:
      label1849:
        v4= GgetArgument(v0,1);
        switch(GgetSymb(v4)) {
        case code_424: /*  */
          v5= GgetArgument(v4,0);
          switch(GgetSymb(v5)) {
          case code_1: /* true */
            bitSet_set(mask,0);
            break;
          /* matching is not complete: jumpNode is null */
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[2];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: (&)(var0,(true)) */
      /* allDetEvaluation: det */
      /* rhs: (&)((true),var0) */
      GmakeAppl1(sv[1],code_424      ,con_1      );
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl2(sv[0],code_426      ,sv[1]      ,v3      );
      res = sv[0] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab155;
      myend416:;
    }
    if(bitSet_get(mask,1)) {
      Gterm *tmp, *sv[2];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: (true) */
      /* allDetEvaluation: det */
      /* rhs: (true) */
      GmakeAppl1(sv[1],code_424      ,con_1      );
      res = sv[1] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab155;
      myend417:;
    }
  }
  fail();
  stratLab155:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_176( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](choix_orient1:list[equation]/ans_completion[Vars,Ops,Prec,System]!GL,choix_orient2:list[equation]/ans_completion[Vars,Ops,Prec,System]!GL,choix_orient3:list[equation]/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6;
    bitSet_GC_create(mask,3);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_494: /* choix_orient(,) */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1856:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        case code_444: /* nil */
          bitSet_set(mask,2);
          break;
        case code_445: /* cons_equation(,) */
          v4= GgetArgument(v3,0);
          switch(GgetSymb(v4)) {
          default:
          label1858:
            v5= GgetArgument(v3,1);
            switch(GgetSymb(v5)) {
            default:
            label1859:
              bitSet_set(mask,0);
              bitSet_set(mask,1);
            }
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[5];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: choix_orient(,)(var0,cons_equation(,)(var1,var2)) */
      /* allDetEvaluation: semiDet */
      CUTOPEN(); /* Wheres */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend418;
      }
      /* where var3 := (s_orient_eq:equation/equation[Vars,Ops,Prec]) orient()(var1) */
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl1(sv[1],code_432      ,v4      );
      sv[0] = strTab[405]( sv[1] );
      CUTCLOSE(); /* Wheres */
      /* rhs: cons_equation(,)(normalize()(var3),@(var0,var2)) */
      // this=var3        underAC=false        Instantiated=false
      sv[2] = fun_438( sv[0] );
      // this=var0        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      sv[3] = fun_446( v2,v5 );
      GmakeAppl2(sv[4],code_445      ,sv[2]      ,sv[3]      );
      res = sv[4] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab160;
      myend418:;
      CUTCLOSE(); /* Wheres */
    }
    if(bitSet_get(mask,1)) {
      Gterm *tmp, *sv[3];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: choix_orient(,)(var0,cons_equation(,)(var1,var2)) */
      /* allDetEvaluation: semiDet */
      CUTOPEN(); /* Wheres */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend419;
      }
      /* where var3 := (s_choix_orient:list[equation]/ans_completion[Vars,Ops,Prec,System]) choix_orient(,)(cons_equation(,)(var1,var0),var2) */
      // this=var1        underAC=false        Instantiated=false
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl2(sv[1],code_445      ,v4      ,v2      );
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_494      ,sv[1]      ,v5      );
      sv[0] = strTab[176]( sv[2] );
      CUTCLOSE(); /* Wheres */
      /* rhs: var3 */
      // this=var3        underAC=false        Instantiated=false
      res = sv[0] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab160;
      myend419:;
      CUTCLOSE(); /* Wheres */
    }
    if(bitSet_get(mask,2)) {
      Gterm *tmp, *sv[1];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: choix_orient(,)(var0,nil) */
      /* allDetEvaluation: det */
      /* rhs: nil */
      res = con_444 ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab160;
      myend420:;
    }
  }
  fail();
  stratLab160:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_224( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* repeat[det](one[semiDet](collapse_ics_ok:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL,collapse_ics_echec:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL,collapse_ics_stop:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL)) */
    long lastTerm_index;
    CUTOPEN(); /* Repeat */
    lastTerm_index = allocStable(sizeof(Gterm*));
    *((Gterm**)getStablePointer(lastTerm_index))=v0;
    if(localSetChoicePoint()!=0) {
      res = v0 = *((Gterm**)getStablePointer(lastTerm_index));
      /* End of repeat */
    } else {
      while(1) {
      /* Apply the strategy */
        {
          /* one[semiDet](collapse_ics_ok:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL,collapse_ics_echec:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL,collapse_ics_stop:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
          Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14;
          bitSet_GC_create(mask,3);
          bitSet_init_clear(mask);
          switch(GgetSymb(v0)) {
          case code_462: /* ||| */
            v2= GgetArgument(v0,0);
            switch(GgetSymb(v2)) {
            default:
            label1863:
              v3= GgetArgument(v0,1);
              switch(GgetSymb(v3)) {
              case code_444: /* nil */
                v4= GgetArgument(v0,2);
                switch(GgetSymb(v4)) {
                case code_445: /* cons_equation(,) */
                  v5= GgetArgument(v4,0);
                  switch(GgetSymb(v5)) {
                  default:
                  label1873:
                    v6= GgetArgument(v4,1);
                    switch(GgetSymb(v6)) {
                    case code_444: /* nil */
                      v7= GgetArgument(v0,3);
                      switch(GgetSymb(v7)) {
                      default:
                      label1875:
                        bitSet_set(mask,2);
                      }
                      break;
                    /* matching is not complete: jumpNode is null */
                    }
                  }
                  break;
                /* matching is not complete: jumpNode is null */
                }
                break;
              case code_445: /* cons_equation(,) */
                v8= GgetArgument(v3,0);
                switch(GgetSymb(v8)) {
                default:
                label1865:
                  v9= GgetArgument(v3,1);
                  switch(GgetSymb(v9)) {
                  default:
                  label1866:
                    v10= GgetArgument(v0,2);
                    switch(GgetSymb(v10)) {
                    case code_445: /* cons_equation(,) */
                      v11= GgetArgument(v10,0);
                      switch(GgetSymb(v11)) {
                      default:
                      label1868:
                        v12= GgetArgument(v10,1);
                        switch(GgetSymb(v12)) {
                        case code_444: /* nil */
                          v13= GgetArgument(v0,3);
                          switch(GgetSymb(v13)) {
                          default:
                          label1870:
                            bitSet_set(mask,0);
                            bitSet_set(mask,1);
                          }
                          break;
                        /* matching is not complete: jumpNode is null */
                        }
                      }
                      break;
                    /* matching is not complete: jumpNode is null */
                    }
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
            break;
          /* matching is not complete: jumpNode is null */
          }
          if(bitSet_get(mask,0)) {
            Gterm *tmp, *sv[5];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: |||(var0,cons_equation(,)(var1,var2),cons_equation(,)(var3,nil),var4) */
            /* allDetEvaluation: semiDet */
            CUTOPEN(); /* Wheres */
            if(localSetChoicePoint()) {
              /* local evaluations failed, try next rule */
              goto myend421;
            }
            /* where var5 := (WHERE11:equation/ans_completion[Vars,Ops,Prec,System]) collapse(,)(var1,var3) */
            // this=var1        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_490            ,v8            ,v11            );
            sv[0] = strTab[24]( sv[1] );
            CUTCLOSE(); /* Wheres */
            /* rhs: |||(var0,var2,cons_equation(,)(var3,nil),@@(var4,cons_equation(,)(var5,nil))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl2(sv[3],code_445            ,v11            ,con_444            );
            // this=var4        underAC=false        Instantiated=false
            // this=var5        underAC=false        Instantiated=false
            GmakeAppl2(sv[4],code_445            ,sv[0]            ,con_444            );
            sv[2] = fun_474( v13,sv[4] );
            GmakeAppl4(sv[4],code_462            ,v2            ,v9            ,sv[3]            ,sv[2]            );
            res = sv[4] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab181;
            myend421:;
            CUTCLOSE(); /* Wheres */
          }
          if(bitSet_get(mask,1)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: |||(var0,cons_equation(,)(var1,var2),cons_equation(,)(var3,nil),var4) */
            /* allDetEvaluation: det */
            /* rhs: |||(@(var0,cons_equation(,)(var1,nil)),var2,cons_equation(,)(var3,nil),var4) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_445            ,v8            ,con_444            );
            sv[0] = fun_446( v2,sv[1] );
            // this=var2        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl2(sv[2],code_445            ,v11            ,con_444            );
            // this=var4        underAC=false        Instantiated=false
            GmakeAppl4(sv[1],code_462            ,sv[0]            ,v9            ,sv[2]            ,v13            );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab181;
            myend422:;
          }
          if(bitSet_get(mask,2)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: |||(var0,nil,cons_equation(,)(var1,nil),var2) */
            /* allDetEvaluation: det */
            /* rhs: |||(var0,nil,nil,var2) */
            // this=var0        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl4(sv[2],code_462            ,v2            ,con_444            ,con_444            ,v7            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab181;
            myend423:;
          }
        }
        fail();
        stratLab181:;
        v0=res;
        *((Gterm**)getStablePointer(lastTerm_index)) = res;
      }
    }
    CUTCLOSE(); /* Repeat */
  }
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_221( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](commande:list[equation]/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2;
    bitSet_GC_create(mask,2);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_479: /* cp */
      bitSet_set(mask,1);
      break;
    case code_478: /* sat */
      bitSet_set(mask,0);
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[3];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: sat */
      /* allDetEvaluation: semiDet */
      CUTOPEN(); /* Wheres */
      if(localSetChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend424;
      }
      /* where var0 := (s_completion:list[equation]/ans_completion[Vars,Ops,Prec,System]) completion()(eqlist) */
      sv[1] = fun_487(  );
      GmakeAppl1(sv[2],code_482      ,sv[1]      );
      sv[0] = strTab[467]( sv[2] );
      CUTCLOSE(); /* Wheres */
      /* rhs: var0 */
      // this=var0        underAC=false        Instantiated=false
      res = sv[0] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab177;
      myend424:;
      CUTCLOSE(); /* Wheres */
    }
    if(bitSet_get(mask,1)) {
      Gterm *tmp, *sv[3];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: cp */
      /* allDetEvaluation: semiDet */
      CUTOPEN(); /* Wheres */
      /* where var0 := (s_cp:list[equation]/ans_completion[Vars,Ops,Prec,System]) cp()(eqlist) */
      sv[1] = fun_487(  );
      GmakeAppl1(sv[2],code_483      ,sv[1]      );
      sv[0] = strTab[96]( sv[2] );
      CUTCLOSE(); /* Wheres */
      /* rhs: var0 */
      // this=var0        underAC=false        Instantiated=false
      res = sv[0] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab177;
      myend425:;
      CUTCLOSE(); /* Wheres */
    }
  }
  fail();
  stratLab177:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_467( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](completion:list[equation]/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_482: /* completion() */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1881:
        bitSet_set(mask,0);
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[9];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: completion()(var0) */
      /* allDetEvaluation: det */
      /* where var2 := normalize()(var0) */
      // this=var0        underAC=false        Instantiated=false
      sv[1] = fun_486( v2 );
      tmp = sv[0] = sv[1];
      /* where var3 := (completion_kb:compute_state/ans_completion[Vars,Ops,Prec,System]) *****(nil,nil,nil,nil,nil,var2) */
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl6(sv[8],code_461      ,con_444      ,con_444      ,con_444      ,con_444      ,con_444      ,sv[0]      );
      sv[2] = strTab[464]( sv[8] );
      /* where var1 := @@(CS_to_A()(var3),CS_to_N()(var3)) */
      // this=var3        underAC=false        Instantiated=false
      sv[4] = fun_463( sv[2] );
      // this=var3        underAC=false        Instantiated=false
      sv[5] = fun_464( sv[2] );
      sv[6] = fun_474( sv[4],sv[5] );
      tmp = sv[3] = sv[6];
      /* rhs: var1 */
      // this=var1        underAC=false        Instantiated=false
      res = sv[3] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab232;
      myend426:;
    }
  }
  fail();
  stratLab232:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_420( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* repeat[det](one[semiDet](compose_ics_ok:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL,compose_ics_echec:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL,compose_ics_stop:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL)) */
    long lastTerm_index;
    CUTOPEN(); /* Repeat */
    lastTerm_index = allocStable(sizeof(Gterm*));
    *((Gterm**)getStablePointer(lastTerm_index))=v0;
    if(localSetChoicePoint()!=0) {
      res = v0 = *((Gterm**)getStablePointer(lastTerm_index));
      /* End of repeat */
    } else {
      while(1) {
      /* Apply the strategy */
        {
          /* one[semiDet](compose_ics_ok:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL,compose_ics_echec:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL,compose_ics_stop:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
          Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14;
          bitSet_GC_create(mask,3);
          bitSet_init_clear(mask);
          switch(GgetSymb(v0)) {
          case code_462: /* ||| */
            v2= GgetArgument(v0,0);
            switch(GgetSymb(v2)) {
            default:
            label1884:
              v3= GgetArgument(v0,1);
              switch(GgetSymb(v3)) {
              case code_444: /* nil */
                v4= GgetArgument(v0,2);
                switch(GgetSymb(v4)) {
                case code_445: /* cons_equation(,) */
                  v5= GgetArgument(v4,0);
                  switch(GgetSymb(v5)) {
                  default:
                  label1894:
                    v6= GgetArgument(v4,1);
                    switch(GgetSymb(v6)) {
                    case code_444: /* nil */
                      v7= GgetArgument(v0,3);
                      switch(GgetSymb(v7)) {
                      default:
                      label1896:
                        bitSet_set(mask,2);
                      }
                      break;
                    /* matching is not complete: jumpNode is null */
                    }
                  }
                  break;
                /* matching is not complete: jumpNode is null */
                }
                break;
              case code_445: /* cons_equation(,) */
                v8= GgetArgument(v3,0);
                switch(GgetSymb(v8)) {
                default:
                label1886:
                  v9= GgetArgument(v3,1);
                  switch(GgetSymb(v9)) {
                  default:
                  label1887:
                    v10= GgetArgument(v0,2);
                    switch(GgetSymb(v10)) {
                    case code_445: /* cons_equation(,) */
                      v11= GgetArgument(v10,0);
                      switch(GgetSymb(v11)) {
                      default:
                      label1889:
                        v12= GgetArgument(v10,1);
                        switch(GgetSymb(v12)) {
                        case code_444: /* nil */
                          v13= GgetArgument(v0,3);
                          switch(GgetSymb(v13)) {
                          default:
                          label1891:
                            bitSet_set(mask,0);
                            bitSet_set(mask,1);
                          }
                          break;
                        /* matching is not complete: jumpNode is null */
                        }
                      }
                      break;
                    /* matching is not complete: jumpNode is null */
                    }
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
            break;
          /* matching is not complete: jumpNode is null */
          }
          if(bitSet_get(mask,0)) {
            Gterm *tmp, *sv[5];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: |||(var0,cons_equation(,)(var1,var2),cons_equation(,)(var3,nil),var4) */
            /* allDetEvaluation: semiDet */
            CUTOPEN(); /* Wheres */
            if(localSetChoicePoint()) {
              /* local evaluations failed, try next rule */
              goto myend427;
            }
            /* where var5 := (WHERE12:equation/ans_completion[Vars,Ops,Prec,System]) compose(,)(var1,var3) */
            // this=var1        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_489            ,v8            ,v11            );
            sv[0] = strTab[25]( sv[1] );
            CUTCLOSE(); /* Wheres */
            /* rhs: |||(var0,var2,cons_equation(,)(var3,nil),append_taille(,)(var4,var5)) */
            // this=var0        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl2(sv[3],code_445            ,v11            ,con_444            );
            // this=var4        underAC=false        Instantiated=false
            // this=var5        underAC=false        Instantiated=false
            sv[2] = fun_481( v13,sv[0] );
            GmakeAppl4(sv[4],code_462            ,v2            ,v9            ,sv[3]            ,sv[2]            );
            res = sv[4] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab222;
            myend427:;
            CUTCLOSE(); /* Wheres */
          }
          if(bitSet_get(mask,1)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: |||(var0,cons_equation(,)(var1,var2),cons_equation(,)(var3,nil),var4) */
            /* allDetEvaluation: det */
            /* rhs: |||(@(var0,cons_equation(,)(var1,nil)),var2,cons_equation(,)(var3,nil),var4) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_445            ,v8            ,con_444            );
            sv[0] = fun_446( v2,sv[1] );
            // this=var2        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl2(sv[2],code_445            ,v11            ,con_444            );
            // this=var4        underAC=false        Instantiated=false
            GmakeAppl4(sv[1],code_462            ,sv[0]            ,v9            ,sv[2]            ,v13            );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab222;
            myend428:;
          }
          if(bitSet_get(mask,2)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: |||(var0,nil,cons_equation(,)(var1,nil),var2) */
            /* allDetEvaluation: det */
            /* rhs: |||(var0,nil,nil,var2) */
            // this=var0        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl4(sv[2],code_462            ,v2            ,con_444            ,con_444            ,v7            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab222;
            myend429:;
          }
        }
        fail();
        stratLab222:;
        v0=res;
        *((Gterm**)getStablePointer(lastTerm_index)) = res;
      }
    }
    CUTCLOSE(); /* Repeat */
  }
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_99( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* repeat[det](one[semiDet](compose_ics_ok_T:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL,compose_ics_echec:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL,compose_ics_stop:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL)) */
    long lastTerm_index;
    CUTOPEN(); /* Repeat */
    lastTerm_index = allocStable(sizeof(Gterm*));
    *((Gterm**)getStablePointer(lastTerm_index))=v0;
    if(localSetChoicePoint()!=0) {
      res = v0 = *((Gterm**)getStablePointer(lastTerm_index));
      /* End of repeat */
    } else {
      while(1) {
      /* Apply the strategy */
        {
          /* one[semiDet](compose_ics_ok_T:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL,compose_ics_echec:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL,compose_ics_stop:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
          Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14;
          bitSet_GC_create(mask,3);
          bitSet_init_clear(mask);
          switch(GgetSymb(v0)) {
          case code_462: /* ||| */
            v2= GgetArgument(v0,0);
            switch(GgetSymb(v2)) {
            default:
            label1899:
              v3= GgetArgument(v0,1);
              switch(GgetSymb(v3)) {
              case code_444: /* nil */
                v4= GgetArgument(v0,2);
                switch(GgetSymb(v4)) {
                case code_445: /* cons_equation(,) */
                  v5= GgetArgument(v4,0);
                  switch(GgetSymb(v5)) {
                  default:
                  label1910:
                    v6= GgetArgument(v4,1);
                    switch(GgetSymb(v6)) {
                    case code_444: /* nil */
                      v7= GgetArgument(v0,3);
                      switch(GgetSymb(v7)) {
                      default:
                      label1912:
                        bitSet_set(mask,2);
                      }
                      break;
                    /* matching is not complete: jumpNode is null */
                    }
                  }
                  break;
                /* matching is not complete: jumpNode is null */
                }
                break;
              case code_445: /* cons_equation(,) */
                v8= GgetArgument(v3,0);
                switch(GgetSymb(v8)) {
                default:
                label1901:
                  v9= GgetArgument(v3,1);
                  switch(GgetSymb(v9)) {
                  default:
                  label1902:
                    v10= GgetArgument(v0,2);
                    switch(GgetSymb(v10)) {
                    case code_445: /* cons_equation(,) */
                      v11= GgetArgument(v10,0);
                      switch(GgetSymb(v11)) {
                      default:
                      label1904:
                        v12= GgetArgument(v10,1);
                        switch(GgetSymb(v12)) {
                        case code_444: /* nil */
                          v13= GgetArgument(v0,3);
                          switch(GgetSymb(v13)) {
                          case code_444: /* nil */
                            bitSet_set(mask,0);
                            bitSet_set(mask,1);
                            break;
                          default:
                          label1906:
                            bitSet_set(mask,1);
                          }
                          break;
                        /* matching is not complete: jumpNode is null */
                        }
                      }
                      break;
                    /* matching is not complete: jumpNode is null */
                    }
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
            break;
          /* matching is not complete: jumpNode is null */
          }
          if(bitSet_get(mask,0)) {
            Gterm *tmp, *sv[6];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: |||(var0,cons_equation(,)(var1,var2),cons_equation(,)(var3,nil),nil) */
            /* allDetEvaluation: semiDet */
            CUTOPEN(); /* Wheres */
            if(localSetChoicePoint()) {
              /* local evaluations failed, try next rule */
              goto myend430;
            }
            /* where var4 := (WHERE13:equation/ans_completion[Vars,Ops,Prec,System]) compose(,)(var1,var3) */
            // this=var1        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_489            ,v8            ,v11            );
            sv[0] = strTab[237]( sv[1] );
            CUTCLOSE(); /* Wheres */
            /* rhs: |||(var0,cons_equation(,)(var4,var2),cons_equation(,)(var3,nil),nil) */
            // this=var0        underAC=false        Instantiated=false
            // this=var4        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[2],code_445            ,sv[0]            ,v9            );
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl2(sv[4],code_445            ,v11            ,con_444            );
            GmakeAppl4(sv[5],code_462            ,v2            ,sv[2]            ,sv[4]            ,con_444            );
            res = sv[5] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab108;
            myend430:;
            CUTCLOSE(); /* Wheres */
          }
          if(bitSet_get(mask,1)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: |||(var0,cons_equation(,)(var1,var2),cons_equation(,)(var3,nil),var4) */
            /* allDetEvaluation: det */
            /* rhs: |||(@(var0,cons_equation(,)(var1,nil)),var2,cons_equation(,)(var3,nil),var4) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_445            ,v8            ,con_444            );
            sv[0] = fun_446( v2,sv[1] );
            // this=var2        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl2(sv[2],code_445            ,v11            ,con_444            );
            // this=var4        underAC=false        Instantiated=false
            GmakeAppl4(sv[1],code_462            ,sv[0]            ,v9            ,sv[2]            ,v13            );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab108;
            myend431:;
          }
          if(bitSet_get(mask,2)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: |||(var0,nil,cons_equation(,)(var1,nil),var2) */
            /* allDetEvaluation: det */
            /* rhs: |||(var0,nil,nil,var2) */
            // this=var0        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl4(sv[2],code_462            ,v2            ,con_444            ,con_444            ,v7            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab108;
            myend432:;
          }
        }
        fail();
        stratLab108:;
        v0=res;
        *((Gterm**)getStablePointer(lastTerm_index)) = res;
      }
    }
    CUTCLOSE(); /* Repeat */
  }
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_96( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](cp:list[equation]/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_483: /* cp() */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1915:
        bitSet_set(mask,0);
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[9];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: cp()(var0) */
      /* allDetEvaluation: det */
      /* where var2 := normalize()(var0) */
      // this=var0        underAC=false        Instantiated=false
      sv[1] = fun_486( v2 );
      tmp = sv[0] = sv[1];
      /* where var3 := (completion_cp:compute_state/ans_completion[Vars,Ops,Prec,System]) *****(nil,nil,nil,nil,nil,var2) */
      // this=var2        underAC=false        Instantiated=false
      GmakeAppl6(sv[8],code_461      ,con_444      ,con_444      ,con_444      ,con_444      ,con_444      ,sv[0]      );
      sv[2] = strTab[470]( sv[8] );
      /* where var1 := @@(CS_to_A()(var3),CS_to_N()(var3)) */
      // this=var3        underAC=false        Instantiated=false
      sv[4] = fun_463( sv[2] );
      // this=var3        underAC=false        Instantiated=false
      sv[5] = fun_464( sv[2] );
      sv[6] = fun_474( sv[4],sv[5] );
      tmp = sv[3] = sv[6];
      /* rhs: var1 */
      // this=var1        underAC=false        Instantiated=false
      res = sv[3] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab104;
      myend433:;
    }
  }
  fail();
  stratLab104:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_116( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* repeat[det](one[semiDet](deduce_list:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL)) */
    long lastTerm_index;
    CUTOPEN(); /* Repeat */
    lastTerm_index = allocStable(sizeof(Gterm*));
    *((Gterm**)getStablePointer(lastTerm_index))=v0;
    if(localSetChoicePoint()!=0) {
      res = v0 = *((Gterm**)getStablePointer(lastTerm_index));
      /* End of repeat */
    } else {
      while(1) {
      /* Apply the strategy */
        {
          /* one[semiDet](deduce_list:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
          Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13;
          bitSet_GC_create(mask,2);
          bitSet_init_clear(mask);
          switch(GgetSymb(v0)) {
          case code_462: /* ||| */
            v2= GgetArgument(v0,0);
            switch(GgetSymb(v2)) {
            default:
            label1918:
              v3= GgetArgument(v0,1);
              switch(GgetSymb(v3)) {
              case code_444: /* nil */
                v4= GgetArgument(v0,2);
                switch(GgetSymb(v4)) {
                case code_445: /* cons_equation(,) */
                  v5= GgetArgument(v4,0);
                  switch(GgetSymb(v5)) {
                  default:
                  label1921:
                    v6= GgetArgument(v4,1);
                    switch(GgetSymb(v6)) {
                    case code_444: /* nil */
                      v7= GgetArgument(v0,3);
                      switch(GgetSymb(v7)) {
                      case code_445: /* cons_equation(,) */
                        v8= GgetArgument(v7,0);
                        switch(GgetSymb(v8)) {
                        default:
                        label1924:
                          v9= GgetArgument(v7,1);
                          switch(GgetSymb(v9)) {
                          case code_444: /* nil */
                            bitSet_set(mask,0);
                            break;
                          /* matching is not complete: jumpNode is null */
                          }
                        }
                        break;
                      /* matching is not complete: jumpNode is null */
                      }
                      break;
                    /* matching is not complete: jumpNode is null */
                    }
                  }
                  break;
                case code_444: /* nil */
                  v10= GgetArgument(v0,3);
                  switch(GgetSymb(v10)) {
                  case code_445: /* cons_equation(,) */
                    v11= GgetArgument(v10,0);
                    switch(GgetSymb(v11)) {
                    default:
                    label1928:
                      v12= GgetArgument(v10,1);
                      switch(GgetSymb(v12)) {
                      case code_444: /* nil */
                        bitSet_set(mask,1);
                        break;
                      /* matching is not complete: jumpNode is null */
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
            }
            break;
          /* matching is not complete: jumpNode is null */
          }
          if(bitSet_get(mask,0)) {
            Gterm *tmp, *sv[10];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: |||(var0,nil,cons_equation(,)(var1,nil),cons_equation(,)(var2,nil)) */
            /* allDetEvaluation: nonDet */
            CUTOPEN(); /* Wheres */
            if(localSetChoicePoint()) {
              /* local evaluations failed, try next rule */
              goto myend434;
            }
            /* where var4 := translate(,)(var2,maxvar(,)((var1),(0))) */
            // this=var2        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl1(sv[1],code_425            ,v5            );
            GmakeAppl1(sv[3],code_301            ,(GsetIntegerTag(0))            );
            sv[2] = fun_440( sv[1],sv[3] );
            sv[1] = fun_434( v8,sv[2] );
            tmp = sv[0] = sv[1];
            /* where var3 := (WHERE15:equation/ans_completion[Vars,Ops,Prec,System]) deduce(,)(var1,var4) */
            // this=var1        underAC=false        Instantiated=false
            // this=var4        underAC=false        Instantiated=false
            GmakeAppl2(sv[3],code_492            ,v5            ,sv[0]            );
            sv[2] = strTab[28]( sv[3] );
            /* if (not())(occurs(,)(var3,var0)) */
            // this=var3        underAC=false        Instantiated=false
            // this=var0        underAC=false        Instantiated=false
            sv[4] = fun_17( sv[2],v2 );
            sv[5] = fun_24( sv[4] );
            if( sv[5] != con_1 ) {
              fail();
            }
            CUTCLOSE(); /* Wheres */
            /* rhs: |||(cons_equation(,)(var3,var0),nil,cons_equation(,)(var1,nil),cons_equation(,)(var2,nil)) */
            // this=var3        underAC=false        Instantiated=false
            // this=var0        underAC=false        Instantiated=false
            GmakeAppl2(sv[4],code_445            ,sv[2]            ,v2            );
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[8],code_445            ,v5            ,con_444            );
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[9],code_445            ,v8            ,con_444            );
            GmakeAppl4(sv[7],code_462            ,sv[4]            ,con_444            ,sv[8]            ,sv[9]            );
            res = sv[7] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab117;
            myend434:;
            CUTCLOSE(); /* Wheres */
          }
          if(bitSet_get(mask,1)) {
            Gterm *tmp, *sv[10];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: |||(var0,nil,nil,cons_equation(,)(var1,nil)) */
            /* allDetEvaluation: nonDet */
            CUTOPEN(); /* Wheres */
            /* where var3 := translate(,)(var1,maxvar(,)((var1),(0))) */
            // this=var1        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl1(sv[1],code_425            ,v11            );
            GmakeAppl1(sv[3],code_301            ,(GsetIntegerTag(0))            );
            sv[2] = fun_440( sv[1],sv[3] );
            sv[1] = fun_434( v11,sv[2] );
            tmp = sv[0] = sv[1];
            /* where var2 := (WHERE16:equation/ans_completion[Vars,Ops,Prec,System]) internal_deduce(,)(var1,var3) */
            // this=var1        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl2(sv[3],code_493            ,v11            ,sv[0]            );
            sv[2] = strTab[29]( sv[3] );
            /* if (not())(occurs(,)(var2,var0)) */
            // this=var2        underAC=false        Instantiated=false
            // this=var0        underAC=false        Instantiated=false
            sv[4] = fun_17( sv[2],v2 );
            sv[5] = fun_24( sv[4] );
            if( sv[5] != con_1 ) {
              fail();
            }
            CUTCLOSE(); /* Wheres */
            /* rhs: |||(cons_equation(,)(var2,var0),nil,nil,cons_equation(,)(var1,nil)) */
            // this=var2        underAC=false        Instantiated=false
            // this=var0        underAC=false        Instantiated=false
            GmakeAppl2(sv[4],code_445            ,sv[2]            ,v2            );
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[9],code_445            ,v11            ,con_444            );
            GmakeAppl4(sv[8],code_462            ,sv[4]            ,con_444            ,con_444            ,sv[9]            );
            res = sv[8] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab117;
            myend435:;
            CUTCLOSE(); /* Wheres */
          }
        }
        fail();
        stratLab117:;
        v0=res;
        *((Gterm**)getStablePointer(lastTerm_index)) = res;
      }
    }
    CUTCLOSE(); /* Repeat */
  }
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_405( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* one[semiDet](orient_eq1:equation/equation[Vars,Ops,Prec]!GL,orient_eq2:equation/equation[Vars,Ops,Prec]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5;
    bitSet_GC_create(mask,2);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_432: /* orient() */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_423: /* = */
        v3= GgetArgument(v2,0);
        switch(GgetSymb(v3)) {
        default:
        label1933:
          v4= GgetArgument(v2,1);
          switch(GgetSymb(v4)) {
          default:
          label1934:
            bitSet_set(mask,0);
            bitSet_set(mask,1);
          }
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[2];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: orient()(=(var0,var1)) */
      /* allDetEvaluation: det */
      /* if >lpo(var0,var1) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      sv[0] = fun_411( v3,v4 );
      if( sv[0] != con_1 ) {
        goto myend436;
      }
      /* rhs: ->(var0,var1) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      GmakeAppl2(sv[1],code_429      ,v3      ,v4      );
      res = sv[1] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab218;
      myend436:;
    }
    if(bitSet_get(mask,1)) {
      Gterm *tmp, *sv[2];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: orient()(=(var0,var1)) */
      /* allDetEvaluation: det */
      /* if >lpo(var1,var0) */
      // this=var1        underAC=false        Instantiated=false
      // this=var0        underAC=false        Instantiated=false
      sv[0] = fun_411( v4,v3 );
      if( sv[0] != con_1 ) {
        fail();
      }
      /* rhs: ->(var1,var0) */
      // this=var1        underAC=false        Instantiated=false
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl2(sv[1],code_429      ,v4      ,v3      );
      res = sv[1] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab218;
      myend437:;
    }
  }
  fail();
  stratLab218:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_269( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  /* cons[semiDet](one[semiDet](compose_A:compute_state/ans_completion[Vars,Ops,Prec,System]!GL),one[semiDet](collapse_A:compute_state/ans_completion[Vars,Ops,Prec,System]!GL),one[semiDet](compose_N:compute_state/ans_completion[Vars,Ops,Prec,System]!GL),one[semiDet](collapse_N:compute_state/ans_completion[Vars,Ops,Prec,System]!GL),one[semiDet](compose_C:compute_state/ans_completion[Vars,Ops,Prec,System]!GL),one[semiDet](collapse_C:compute_state/ans_completion[Vars,Ops,Prec,System]!GL),one[semiDet](compose_T:compute_state/ans_completion[Vars,Ops,Prec,System]!GL),one[semiDet](collapse_T:compute_state/ans_completion[Vars,Ops,Prec,System]!GL)) */
  {
    /* one[semiDet](compose_A:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_461: /* ***** */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1937:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        default:
        label1938:
          v4= GgetArgument(v0,2);
          switch(GgetSymb(v4)) {
          default:
          label1939:
            v5= GgetArgument(v0,3);
            switch(GgetSymb(v5)) {
            default:
            label1940:
              v6= GgetArgument(v0,4);
              switch(GgetSymb(v6)) {
              case code_445: /* cons_equation(,) */
                v7= GgetArgument(v6,0);
                switch(GgetSymb(v7)) {
                default:
                label1942:
                  v8= GgetArgument(v6,1);
                  switch(GgetSymb(v8)) {
                  default:
                  label1943:
                    v9= GgetArgument(v0,5);
                    switch(GgetSymb(v9)) {
                    default:
                    label1944:
                      bitSet_set(mask,0);
                    }
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
          }
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[8];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,var2,var3,cons_equation(,)(var4,var5),var6) */
      /* allDetEvaluation: det */
      /* where var9 := (s_compose_ics:internal_compute_state/ans_completion[Vars,Ops,Prec,System]) |||(nil,var0,cons_equation(,)(var4,var5),var3) */
      // this=var0        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_445      ,v7      ,v8      );
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl4(sv[3],code_462      ,con_444      ,v2      ,sv[2]      ,v5      );
      sv[0] = strTab[420]( sv[3] );
      /* where var8 := ICS_to_4()(var9) */
      // this=var9        underAC=false        Instantiated=false
      sv[2] = fun_472( sv[0] );
      tmp = sv[1] = sv[2];
      /* where var7 := ICS_to_1()(var9) */
      // this=var9        underAC=false        Instantiated=false
      sv[5] = fun_469( sv[0] );
      tmp = sv[4] = sv[5];
      /* rhs: *****(var7,var1,var2,var8,cons_equation(,)(var4,var5),var6) */
      // this=var7        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      // this=var8        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[6],code_445      ,v7      ,v8      );
      // this=var6        underAC=false        Instantiated=false
      GmakeAppl6(sv[7],code_461      ,sv[4]      ,v3      ,v4      ,sv[1]      ,sv[6]      ,v9      );
      res = sv[7] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab189;
      myend438:;
    }
  }
  fail();
  stratLab189:;
  v0=res;
  {
    /* one[semiDet](collapse_A:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_461: /* ***** */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1947:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        default:
        label1948:
          v4= GgetArgument(v0,2);
          switch(GgetSymb(v4)) {
          default:
          label1949:
            v5= GgetArgument(v0,3);
            switch(GgetSymb(v5)) {
            default:
            label1950:
              v6= GgetArgument(v0,4);
              switch(GgetSymb(v6)) {
              case code_445: /* cons_equation(,) */
                v7= GgetArgument(v6,0);
                switch(GgetSymb(v7)) {
                default:
                label1952:
                  v8= GgetArgument(v6,1);
                  switch(GgetSymb(v8)) {
                  default:
                  label1953:
                    v9= GgetArgument(v0,5);
                    switch(GgetSymb(v9)) {
                    default:
                    label1954:
                      bitSet_set(mask,0);
                    }
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
          }
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[8];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,var2,var3,cons_equation(,)(var4,var5),var6) */
      /* allDetEvaluation: det */
      /* where var9 := (s_collapse_ics:internal_compute_state/ans_completion[Vars,Ops,Prec,System]) |||(nil,var0,cons_equation(,)(var4,var5),var6) */
      // this=var0        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_445      ,v7      ,v8      );
      // this=var6        underAC=false        Instantiated=false
      GmakeAppl4(sv[3],code_462      ,con_444      ,v2      ,sv[2]      ,v9      );
      sv[0] = strTab[224]( sv[3] );
      /* where var8 := ICS_to_4()(var9) */
      // this=var9        underAC=false        Instantiated=false
      sv[2] = fun_472( sv[0] );
      tmp = sv[1] = sv[2];
      /* where var7 := ICS_to_1()(var9) */
      // this=var9        underAC=false        Instantiated=false
      sv[5] = fun_469( sv[0] );
      tmp = sv[4] = sv[5];
      /* rhs: *****(var7,var1,var2,var3,cons_equation(,)(var4,var5),var8) */
      // this=var7        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[6],code_445      ,v7      ,v8      );
      // this=var8        underAC=false        Instantiated=false
      GmakeAppl6(sv[7],code_461      ,sv[4]      ,v3      ,v4      ,v5      ,sv[6]      ,sv[1]      );
      res = sv[7] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab191;
      myend439:;
    }
  }
  fail();
  stratLab191:;
  v0=res;
  {
    /* one[semiDet](compose_N:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_461: /* ***** */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1957:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        default:
        label1958:
          v4= GgetArgument(v0,2);
          switch(GgetSymb(v4)) {
          default:
          label1959:
            v5= GgetArgument(v0,3);
            switch(GgetSymb(v5)) {
            default:
            label1960:
              v6= GgetArgument(v0,4);
              switch(GgetSymb(v6)) {
              case code_445: /* cons_equation(,) */
                v7= GgetArgument(v6,0);
                switch(GgetSymb(v7)) {
                default:
                label1962:
                  v8= GgetArgument(v6,1);
                  switch(GgetSymb(v8)) {
                  default:
                  label1963:
                    v9= GgetArgument(v0,5);
                    switch(GgetSymb(v9)) {
                    default:
                    label1964:
                      bitSet_set(mask,0);
                    }
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
          }
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[8];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,var2,var3,cons_equation(,)(var4,var5),var6) */
      /* allDetEvaluation: det */
      /* where var9 := (s_compose_ics:internal_compute_state/ans_completion[Vars,Ops,Prec,System]) |||(nil,var1,cons_equation(,)(var4,var5),var3) */
      // this=var1        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_445      ,v7      ,v8      );
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl4(sv[3],code_462      ,con_444      ,v3      ,sv[2]      ,v5      );
      sv[0] = strTab[420]( sv[3] );
      /* where var8 := ICS_to_4()(var9) */
      // this=var9        underAC=false        Instantiated=false
      sv[2] = fun_472( sv[0] );
      tmp = sv[1] = sv[2];
      /* where var7 := ICS_to_1()(var9) */
      // this=var9        underAC=false        Instantiated=false
      sv[5] = fun_469( sv[0] );
      tmp = sv[4] = sv[5];
      /* rhs: *****(var0,var7,var2,var8,cons_equation(,)(var4,var5),var6) */
      // this=var0        underAC=false        Instantiated=false
      // this=var7        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      // this=var8        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[6],code_445      ,v7      ,v8      );
      // this=var6        underAC=false        Instantiated=false
      GmakeAppl6(sv[7],code_461      ,v2      ,sv[4]      ,v4      ,sv[1]      ,sv[6]      ,v9      );
      res = sv[7] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab193;
      myend440:;
    }
  }
  fail();
  stratLab193:;
  v0=res;
  {
    /* one[semiDet](collapse_N:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_461: /* ***** */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1967:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        default:
        label1968:
          v4= GgetArgument(v0,2);
          switch(GgetSymb(v4)) {
          default:
          label1969:
            v5= GgetArgument(v0,3);
            switch(GgetSymb(v5)) {
            default:
            label1970:
              v6= GgetArgument(v0,4);
              switch(GgetSymb(v6)) {
              case code_445: /* cons_equation(,) */
                v7= GgetArgument(v6,0);
                switch(GgetSymb(v7)) {
                default:
                label1972:
                  v8= GgetArgument(v6,1);
                  switch(GgetSymb(v8)) {
                  default:
                  label1973:
                    v9= GgetArgument(v0,5);
                    switch(GgetSymb(v9)) {
                    default:
                    label1974:
                      bitSet_set(mask,0);
                    }
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
          }
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[8];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,var2,var3,cons_equation(,)(var4,var5),var6) */
      /* allDetEvaluation: det */
      /* where var9 := (s_collapse_ics:internal_compute_state/ans_completion[Vars,Ops,Prec,System]) |||(nil,var1,cons_equation(,)(var4,var5),var6) */
      // this=var1        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_445      ,v7      ,v8      );
      // this=var6        underAC=false        Instantiated=false
      GmakeAppl4(sv[3],code_462      ,con_444      ,v3      ,sv[2]      ,v9      );
      sv[0] = strTab[224]( sv[3] );
      /* where var8 := ICS_to_4()(var9) */
      // this=var9        underAC=false        Instantiated=false
      sv[2] = fun_472( sv[0] );
      tmp = sv[1] = sv[2];
      /* where var7 := ICS_to_1()(var9) */
      // this=var9        underAC=false        Instantiated=false
      sv[5] = fun_469( sv[0] );
      tmp = sv[4] = sv[5];
      /* rhs: *****(var0,var7,var2,var3,cons_equation(,)(var4,var5),var8) */
      // this=var0        underAC=false        Instantiated=false
      // this=var7        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[6],code_445      ,v7      ,v8      );
      // this=var8        underAC=false        Instantiated=false
      GmakeAppl6(sv[7],code_461      ,v2      ,sv[4]      ,v4      ,v5      ,sv[6]      ,sv[1]      );
      res = sv[7] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab195;
      myend441:;
    }
  }
  fail();
  stratLab195:;
  v0=res;
  {
    /* one[semiDet](compose_C:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_461: /* ***** */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1977:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        default:
        label1978:
          v4= GgetArgument(v0,2);
          switch(GgetSymb(v4)) {
          default:
          label1979:
            v5= GgetArgument(v0,3);
            switch(GgetSymb(v5)) {
            default:
            label1980:
              v6= GgetArgument(v0,4);
              switch(GgetSymb(v6)) {
              case code_445: /* cons_equation(,) */
                v7= GgetArgument(v6,0);
                switch(GgetSymb(v7)) {
                default:
                label1982:
                  v8= GgetArgument(v6,1);
                  switch(GgetSymb(v8)) {
                  default:
                  label1983:
                    v9= GgetArgument(v0,5);
                    switch(GgetSymb(v9)) {
                    default:
                    label1984:
                      bitSet_set(mask,0);
                    }
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
          }
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[8];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,var2,var3,cons_equation(,)(var4,var5),var6) */
      /* allDetEvaluation: det */
      /* where var9 := (s_compose_ics:internal_compute_state/ans_completion[Vars,Ops,Prec,System]) |||(nil,var2,cons_equation(,)(var4,var5),var3) */
      // this=var2        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_445      ,v7      ,v8      );
      // this=var3        underAC=false        Instantiated=false
      GmakeAppl4(sv[3],code_462      ,con_444      ,v4      ,sv[2]      ,v5      );
      sv[0] = strTab[420]( sv[3] );
      /* where var8 := ICS_to_4()(var9) */
      // this=var9        underAC=false        Instantiated=false
      sv[2] = fun_472( sv[0] );
      tmp = sv[1] = sv[2];
      /* where var7 := ICS_to_1()(var9) */
      // this=var9        underAC=false        Instantiated=false
      sv[5] = fun_469( sv[0] );
      tmp = sv[4] = sv[5];
      /* rhs: *****(var0,var1,var7,var8,cons_equation(,)(var4,var5),var6) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      // this=var7        underAC=false        Instantiated=false
      // this=var8        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[6],code_445      ,v7      ,v8      );
      // this=var6        underAC=false        Instantiated=false
      GmakeAppl6(sv[7],code_461      ,v2      ,v3      ,sv[4]      ,sv[1]      ,sv[6]      ,v9      );
      res = sv[7] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab197;
      myend442:;
    }
  }
  fail();
  stratLab197:;
  v0=res;
  {
    /* one[semiDet](collapse_C:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_461: /* ***** */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1987:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        default:
        label1988:
          v4= GgetArgument(v0,2);
          switch(GgetSymb(v4)) {
          default:
          label1989:
            v5= GgetArgument(v0,3);
            switch(GgetSymb(v5)) {
            default:
            label1990:
              v6= GgetArgument(v0,4);
              switch(GgetSymb(v6)) {
              case code_445: /* cons_equation(,) */
                v7= GgetArgument(v6,0);
                switch(GgetSymb(v7)) {
                default:
                label1992:
                  v8= GgetArgument(v6,1);
                  switch(GgetSymb(v8)) {
                  default:
                  label1993:
                    v9= GgetArgument(v0,5);
                    switch(GgetSymb(v9)) {
                    default:
                    label1994:
                      bitSet_set(mask,0);
                    }
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
          }
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[8];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,var2,var3,cons_equation(,)(var4,var5),var6) */
      /* allDetEvaluation: det */
      /* where var9 := (s_collapse_ics:internal_compute_state/ans_completion[Vars,Ops,Prec,System]) |||(nil,var2,cons_equation(,)(var4,var5),var6) */
      // this=var2        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_445      ,v7      ,v8      );
      // this=var6        underAC=false        Instantiated=false
      GmakeAppl4(sv[3],code_462      ,con_444      ,v4      ,sv[2]      ,v9      );
      sv[0] = strTab[224]( sv[3] );
      /* where var8 := ICS_to_4()(var9) */
      // this=var9        underAC=false        Instantiated=false
      sv[2] = fun_472( sv[0] );
      tmp = sv[1] = sv[2];
      /* where var7 := ICS_to_1()(var9) */
      // this=var9        underAC=false        Instantiated=false
      sv[5] = fun_469( sv[0] );
      tmp = sv[4] = sv[5];
      /* rhs: *****(var0,var1,var7,var3,cons_equation(,)(var4,var5),var8) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      // this=var7        underAC=false        Instantiated=false
      // this=var3        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[6],code_445      ,v7      ,v8      );
      // this=var8        underAC=false        Instantiated=false
      GmakeAppl6(sv[7],code_461      ,v2      ,v3      ,sv[4]      ,v5      ,sv[6]      ,sv[1]      );
      res = sv[7] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab199;
      myend443:;
    }
  }
  fail();
  stratLab199:;
  v0=res;
  {
    /* one[semiDet](compose_T:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_461: /* ***** */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label1997:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        default:
        label1998:
          v4= GgetArgument(v0,2);
          switch(GgetSymb(v4)) {
          default:
          label1999:
            v5= GgetArgument(v0,3);
            switch(GgetSymb(v5)) {
            default:
            label2000:
              v6= GgetArgument(v0,4);
              switch(GgetSymb(v6)) {
              case code_445: /* cons_equation(,) */
                v7= GgetArgument(v6,0);
                switch(GgetSymb(v7)) {
                default:
                label2002:
                  v8= GgetArgument(v6,1);
                  switch(GgetSymb(v8)) {
                  default:
                  label2003:
                    v9= GgetArgument(v0,5);
                    switch(GgetSymb(v9)) {
                    default:
                    label2004:
                      bitSet_set(mask,0);
                    }
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
          }
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[6];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,var2,var3,cons_equation(,)(var4,var5),var6) */
      /* allDetEvaluation: det */
      /* where var8 := (s_compose_ics_T:internal_compute_state/ans_completion[Vars,Ops,Prec,System]) |||(nil,var3,cons_equation(,)(var4,var5),nil) */
      // this=var3        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_445      ,v7      ,v8      );
      GmakeAppl4(sv[4],code_462      ,con_444      ,v5      ,sv[2]      ,con_444      );
      sv[0] = strTab[99]( sv[4] );
      /* where var7 := ICS_to_1()(var8) */
      // this=var8        underAC=false        Instantiated=false
      sv[2] = fun_469( sv[0] );
      tmp = sv[1] = sv[2];
      /* rhs: *****(var0,var1,var2,var7,cons_equation(,)(var4,var5),var6) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      // this=var7        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[3],code_445      ,v7      ,v8      );
      // this=var6        underAC=false        Instantiated=false
      GmakeAppl6(sv[5],code_461      ,v2      ,v3      ,v4      ,sv[1]      ,sv[3]      ,v9      );
      res = sv[5] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab201;
      myend444:;
    }
  }
  fail();
  stratLab201:;
  v0=res;
  {
    /* one[semiDet](collapse_T:compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_461: /* ***** */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      default:
      label2007:
        v3= GgetArgument(v0,1);
        switch(GgetSymb(v3)) {
        default:
        label2008:
          v4= GgetArgument(v0,2);
          switch(GgetSymb(v4)) {
          default:
          label2009:
            v5= GgetArgument(v0,3);
            switch(GgetSymb(v5)) {
            default:
            label2010:
              v6= GgetArgument(v0,4);
              switch(GgetSymb(v6)) {
              case code_445: /* cons_equation(,) */
                v7= GgetArgument(v6,0);
                switch(GgetSymb(v7)) {
                default:
                label2012:
                  v8= GgetArgument(v6,1);
                  switch(GgetSymb(v8)) {
                  default:
                  label2013:
                    v9= GgetArgument(v0,5);
                    switch(GgetSymb(v9)) {
                    default:
                    label2014:
                      bitSet_set(mask,0);
                    }
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
          }
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[8];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: *****(var0,var1,var2,var3,cons_equation(,)(var4,var5),var6) */
      /* allDetEvaluation: det */
      /* where var9 := (s_collapse_ics:internal_compute_state/ans_completion[Vars,Ops,Prec,System]) |||(nil,var3,cons_equation(,)(var4,var5),var6) */
      // this=var3        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[2],code_445      ,v7      ,v8      );
      // this=var6        underAC=false        Instantiated=false
      GmakeAppl4(sv[3],code_462      ,con_444      ,v5      ,sv[2]      ,v9      );
      sv[0] = strTab[224]( sv[3] );
      /* where var8 := ICS_to_4()(var9) */
      // this=var9        underAC=false        Instantiated=false
      sv[2] = fun_472( sv[0] );
      tmp = sv[1] = sv[2];
      /* where var7 := ICS_to_1()(var9) */
      // this=var9        underAC=false        Instantiated=false
      sv[5] = fun_469( sv[0] );
      tmp = sv[4] = sv[5];
      /* rhs: *****(var0,var1,var2,var7,cons_equation(,)(var4,var5),var8) */
      // this=var0        underAC=false        Instantiated=false
      // this=var1        underAC=false        Instantiated=false
      // this=var2        underAC=false        Instantiated=false
      // this=var7        underAC=false        Instantiated=false
      // this=var4        underAC=false        Instantiated=false
      // this=var5        underAC=false        Instantiated=false
      GmakeAppl2(sv[6],code_445      ,v7      ,v8      );
      // this=var8        underAC=false        Instantiated=false
      GmakeAppl6(sv[7],code_461      ,v2      ,v3      ,v4      ,sv[4]      ,sv[6]      ,sv[1]      );
      res = sv[7] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab203;
      myend445:;
    }
  }
  fail();
  stratLab203:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_39( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* repeat[det](one[semiDet](simplify_ics:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL)) */
    long lastTerm_index;
    CUTOPEN(); /* Repeat */
    lastTerm_index = allocStable(sizeof(Gterm*));
    *((Gterm**)getStablePointer(lastTerm_index))=v0;
    if(localSetChoicePoint()!=0) {
      res = v0 = *((Gterm**)getStablePointer(lastTerm_index));
      /* End of repeat */
    } else {
      while(1) {
      /* Apply the strategy */
        {
          /* one[semiDet](simplify_ics:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
          Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8;
          bitSet_GC_create(mask,1);
          bitSet_init_clear(mask);
          switch(GgetSymb(v0)) {
          case code_462: /* ||| */
            v2= GgetArgument(v0,0);
            switch(GgetSymb(v2)) {
            default:
            label2017:
              v3= GgetArgument(v0,1);
              switch(GgetSymb(v3)) {
              case code_444: /* nil */
                v4= GgetArgument(v0,2);
                switch(GgetSymb(v4)) {
                default:
                label2019:
                  v5= GgetArgument(v0,3);
                  switch(GgetSymb(v5)) {
                  case code_445: /* cons_equation(,) */
                    v6= GgetArgument(v5,0);
                    switch(GgetSymb(v6)) {
                    default:
                    label2021:
                      v7= GgetArgument(v5,1);
                      switch(GgetSymb(v7)) {
                      default:
                      label2022:
                        bitSet_set(mask,0);
                      }
                    }
                    break;
                  /* matching is not complete: jumpNode is null */
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
            break;
          /* matching is not complete: jumpNode is null */
          }
          if(bitSet_get(mask,0)) {
            Gterm *tmp, *sv[7];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: |||(var0,nil,var1,cons_equation(,)(var2,var3)) */
            /* allDetEvaluation: det */
            /* where var5 := (s_simplify_list:internal_compute_state/ans_completion[Vars,Ops,Prec,System]) |||(nil,nil,var1,cons_equation(,)(var2,nil)) */
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[4],code_445            ,v6            ,con_444            );
            GmakeAppl4(sv[3],code_462            ,con_444            ,con_444            ,v4            ,sv[4]            );
            sv[0] = strTab[164]( sv[3] );
            /* where var4 := ICS_to_1()(var5) */
            // this=var5        underAC=false        Instantiated=false
            sv[2] = fun_469( sv[0] );
            tmp = sv[1] = sv[2];
            /* rhs: |||(@@(var0,var4),nil,var1,var3) */
            // this=var0        underAC=false        Instantiated=false
            // this=var4        underAC=false        Instantiated=false
            sv[4] = fun_474( v2,sv[1] );
            // this=var1        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl4(sv[6],code_462            ,sv[4]            ,con_444            ,v4            ,v7            );
            res = sv[6] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab46;
            myend446:;
          }
        }
        fail();
        stratLab46:;
        v0=res;
        *((Gterm**)getStablePointer(lastTerm_index)) = res;
      }
    }
    CUTCLOSE(); /* Repeat */
  }
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_164( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  {
    /* repeat[det](one[semiDet](simplify_list_ok:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL,simplify_list_echec:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL,simplify_list_stop:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL)) */
    long lastTerm_index;
    CUTOPEN(); /* Repeat */
    lastTerm_index = allocStable(sizeof(Gterm*));
    *((Gterm**)getStablePointer(lastTerm_index))=v0;
    if(localSetChoicePoint()!=0) {
      res = v0 = *((Gterm**)getStablePointer(lastTerm_index));
      /* End of repeat */
    } else {
      while(1) {
      /* Apply the strategy */
        {
          /* one[semiDet](simplify_list_ok:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL,simplify_list_echec:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL,simplify_list_stop:internal_compute_state/ans_completion[Vars,Ops,Prec,System]!GL) */
          Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16;
          bitSet_GC_create(mask,4);
          bitSet_init_clear(mask);
          switch(GgetSymb(v0)) {
          case code_462: /* ||| */
            v2= GgetArgument(v0,0);
            switch(GgetSymb(v2)) {
            case code_444: /* nil */
              v3= GgetArgument(v0,1);
              switch(GgetSymb(v3)) {
              default:
              label2026:
                v4= GgetArgument(v0,2);
                switch(GgetSymb(v4)) {
                case code_445: /* cons_equation(,) */
                  v5= GgetArgument(v4,0);
                  switch(GgetSymb(v5)) {
                  default:
                  label2028:
                    v6= GgetArgument(v4,1);
                    switch(GgetSymb(v6)) {
                    default:
                    label2029:
                      v7= GgetArgument(v0,3);
                      switch(GgetSymb(v7)) {
                      case code_445: /* cons_equation(,) */
                        v8= GgetArgument(v7,0);
                        switch(GgetSymb(v8)) {
                        case code_423: /* = */
                          v9= GgetArgument(v8,0);
                          switch(GgetSymb(v9)) {
                          default:
                          label2032:
                            v10= GgetArgument(v8,1);
                            switch(GgetSymb(v10)) {
                            default:
                            label2033:
                              v11= GgetArgument(v7,1);
                              switch(GgetSymb(v11)) {
                              case code_444: /* nil */
                                bitSet_set(mask,0);
                                bitSet_set(mask,1);
                                bitSet_set(mask,2);
                                break;
                              /* matching is not complete: jumpNode is null */
                              }
                            }
                          }
                          break;
                        default:
                        label2031:
                          v11= GgetArgument(v7,1);
                          switch(GgetSymb(v11)) {
                          case code_444: /* nil */
                            bitSet_set(mask,1);
                            bitSet_set(mask,2);
                            break;
                          /* matching is not complete: jumpNode is null */
                          }
                        }
                        break;
                      /* matching is not complete: jumpNode is null */
                      }
                    }
                  }
                  break;
                case code_444: /* nil */
                  v13= GgetArgument(v0,3);
                  switch(GgetSymb(v13)) {
                  case code_445: /* cons_equation(,) */
                    v14= GgetArgument(v13,0);
                    switch(GgetSymb(v14)) {
                    default:
                    label2039:
                      v15= GgetArgument(v13,1);
                      switch(GgetSymb(v15)) {
                      case code_444: /* nil */
                        bitSet_set(mask,3);
                        break;
                      /* matching is not complete: jumpNode is null */
                      }
                    }
                    break;
                  /* matching is not complete: jumpNode is null */
                  }
                  break;
                /* matching is not complete: jumpNode is null */
                }
              }
              break;
            /* matching is not complete: jumpNode is null */
            }
            break;
          /* matching is not complete: jumpNode is null */
          }
          if(bitSet_get(mask,0)) {
            Gterm *tmp, *sv[6];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: |||(nil,var0,cons_equation(,)(var1,var2),cons_equation(,)(=(var3,var4),nil)) */
            /* allDetEvaluation: det */
            /* if eq_list[int](,)(var3,var4) */
            // this=var3        underAC=false        Instantiated=false
            // this=var4        underAC=false        Instantiated=false
            sv[0] = fun_18( v9,v10 );
            if( sv[0] != con_1 ) {
              goto myend447;
            }
            /* rhs: |||(nil,nil,nil,nil) */
            GmakeAppl4(sv[5],code_462            ,con_444            ,con_444            ,con_444            ,con_444            );
            res = sv[5] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab134;
            myend447:;
          }
          if(bitSet_get(mask,1)) {
            Gterm *tmp, *sv[7];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: |||(nil,var0,cons_equation(,)(var1,var2),cons_equation(,)(var3,nil)) */
            /* allDetEvaluation: nonDet */
            CUTOPEN(); /* Wheres */
            if(localSetChoicePoint()) {
              /* local evaluations failed, try next rule */
              goto myend448;
            }
            /* where var4 := (WHERE14:equation/ans_completion[Vars,Ops,Prec,System]) simplify(,)(var3,var1) */
            // this=var3        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_491            ,v8            ,v5            );
            sv[0] = strTab[27]( sv[1] );
            CUTCLOSE(); /* Wheres */
            /* rhs: |||(nil,nil,@(cons_equation(,)(var1,var2),var0),cons_equation(,)(var4,nil)) */
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[4],code_445            ,v5            ,v6            );
            // this=var0        underAC=false        Instantiated=false
            sv[5] = fun_446( sv[4],v3 );
            // this=var4        underAC=false        Instantiated=false
            GmakeAppl2(sv[6],code_445            ,sv[0]            ,con_444            );
            GmakeAppl4(sv[4],code_462            ,con_444            ,con_444            ,sv[5]            ,sv[6]            );
            res = sv[4] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab134;
            myend448:;
            CUTCLOSE(); /* Wheres */
          }
          if(bitSet_get(mask,2)) {
            Gterm *tmp, *sv[4];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: |||(nil,var0,cons_equation(,)(var1,var2),cons_equation(,)(var3,nil)) */
            /* allDetEvaluation: det */
            /* rhs: |||(nil,cons_equation(,)(var1,var0),var2,cons_equation(,)(var3,nil)) */
            // this=var1        underAC=false        Instantiated=false
            // this=var0        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_445            ,v5            ,v3            );
            // this=var2        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl2(sv[3],code_445            ,v8            ,con_444            );
            GmakeAppl4(sv[2],code_462            ,con_444            ,sv[1]            ,v6            ,sv[3]            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab134;
            myend449:;
          }
          if(bitSet_get(mask,3)) {
            Gterm *tmp, *sv[5];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: |||(nil,var0,nil,cons_equation(,)(var1,nil)) */
            /* allDetEvaluation: det */
            /* rhs: |||(cons_equation(,)(var1,nil),nil,nil,nil) */
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_445            ,v14            ,con_444            );
            GmakeAppl4(sv[4],code_462            ,sv[1]            ,con_444            ,con_444            ,con_444            );
            res = sv[4] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab134;
            myend450:;
          }
        }
        fail();
        stratLab134:;
        v0=res;
        *((Gterm**)getStablePointer(lastTerm_index)) = res;
      }
    }
    CUTCLOSE(); /* Repeat */
  }
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_88( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  /* cons[semiDet](one[semiDet](trueadd:eqSystem/syntacticUnification[Ops,Vars]!GL),repeat[det](one[semiDet](unfold:eqSystem/eqSystem[Ops,Vars]!GL,delete:eqSystem/syntacticUnification[Ops,Vars]!GL,decompose:eqSystem/syntacticUnification[Ops,Vars]!LO,conflict:eqSystem/syntacticUnification[Ops,Vars]!GL,coalesce:eqSystem/syntacticUnification[Ops,Vars]!GL,occCheck1:eqSystem/syntacticUnification[Ops,Vars]!GL,occCheck2:eqSystem/syntacticUnification[Ops,Vars]!GL,eliminate1:eqSystem/syntacticUnification[Ops,Vars]!GL,eliminate2:eqSystem/syntacticUnification[Ops,Vars]!GL)),one[semiDet](trueelim:eqSystem/syntacticUnification[Ops,Vars]!GL,truepropag:eqSystem/syntacticUnification[Ops,Vars]!GL,falsepropag:eqSystem/syntacticUnification[Ops,Vars]!GL)) */
  {
    /* one[semiDet](trueadd:eqSystem/syntacticUnification[Ops,Vars]!GL) */
    Gterm *v1,*v2;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    default:
    label2042:
      bitSet_set(mask,0);
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[2];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: var0 */
      /* allDetEvaluation: det */
      /* rhs: (&)((true),var0) */
      GmakeAppl1(sv[1],code_424      ,con_1      );
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl2(sv[0],code_426      ,sv[1]      ,v0      );
      res = sv[0] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab86;
      myend451:;
    }
  }
  fail();
  stratLab86:;
  v0=res;
  {
    /* repeat[det](one[semiDet](unfold:eqSystem/eqSystem[Ops,Vars]!GL,delete:eqSystem/syntacticUnification[Ops,Vars]!GL,decompose:eqSystem/syntacticUnification[Ops,Vars]!LO,conflict:eqSystem/syntacticUnification[Ops,Vars]!GL,coalesce:eqSystem/syntacticUnification[Ops,Vars]!GL,occCheck1:eqSystem/syntacticUnification[Ops,Vars]!GL,occCheck2:eqSystem/syntacticUnification[Ops,Vars]!GL,eliminate1:eqSystem/syntacticUnification[Ops,Vars]!GL,eliminate2:eqSystem/syntacticUnification[Ops,Vars]!GL)) */
    long lastTerm_index;
    CUTOPEN(); /* Repeat */
    lastTerm_index = allocStable(sizeof(Gterm*));
    *((Gterm**)getStablePointer(lastTerm_index))=v0;
    if(localSetChoicePoint()!=0) {
      res = v0 = *((Gterm**)getStablePointer(lastTerm_index));
      /* End of repeat */
    } else {
      while(1) {
      /* Apply the strategy */
        {
          /* one[semiDet](unfold:eqSystem/eqSystem[Ops,Vars]!GL,delete:eqSystem/syntacticUnification[Ops,Vars]!GL,decompose:eqSystem/syntacticUnification[Ops,Vars]!LO,conflict:eqSystem/syntacticUnification[Ops,Vars]!GL,coalesce:eqSystem/syntacticUnification[Ops,Vars]!GL,occCheck1:eqSystem/syntacticUnification[Ops,Vars]!GL,occCheck2:eqSystem/syntacticUnification[Ops,Vars]!GL,eliminate1:eqSystem/syntacticUnification[Ops,Vars]!GL,eliminate2:eqSystem/syntacticUnification[Ops,Vars]!GL) */
          Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16,*v17,*v18,*v19,*v20,*v21,*v22,*v23,*v24,*v25,*v26,*v27,*v28,*v29,*v30,*v31,*v32,*v33,*v34,*v35,*v36,*v37,*v38,*v39,*v40,*v41,*v42,*v43,*v44,*v45,*v46,*v47,*v48,*v49,*v50;
          bitSet_GC_create(mask,20);
          bitSet_init_clear(mask);
          switch(GgetSymb(v0)) {
          case code_426: /* (&) */
            v2= GgetArgument(v0,0);
            switch(GgetSymb(v2)) {
            default:
            label2045:
              v3= GgetArgument(v0,1);
              switch(GgetSymb(v3)) {
              case code_424: /*  */
                v4= GgetArgument(v3,0);
                switch(GgetSymb(v4)) {
                case code_1: /* true */
                  bitSet_set(mask,2);
                  break;
                /* matching is not complete: jumpNode is null */
                }
                break;
              case code_425: /*  */
                v5= GgetArgument(v3,0);
                switch(GgetSymb(v5)) {
                case code_423: /* = */
                  v6= GgetArgument(v5,0);
                  switch(GgetSymb(v6)) {
                  case code_380: /* i5() */
                    v7= GgetArgument(v6,0);
                    switch(GgetSymb(v7)) {
                    default:
                    label2090:
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_380: /* i5() */
                        v9= GgetArgument(v8,0);
                        switch(GgetSymb(v9)) {
                        default:
                        label2092:
                          bitSet_set(mask,13);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label2128:
                          bitSet_set(mask,14);
                          bitSet_set(mask,17);
                          bitSet_set(mask,19);
                        }
                        break;
                      default:
                      label2091:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_379: /* i4() */
                    v11= GgetArgument(v6,0);
                    switch(GgetSymb(v11)) {
                    default:
                    label2086:
                      v12= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_379: /* i4() */
                        v13= GgetArgument(v12,0);
                        switch(GgetSymb(v13)) {
                        default:
                        label2088:
                          bitSet_set(mask,12);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label2126:
                          bitSet_set(mask,14);
                          bitSet_set(mask,17);
                          bitSet_set(mask,19);
                        }
                        break;
                      default:
                      label2087:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_378: /* i3() */
                    v15= GgetArgument(v6,0);
                    switch(GgetSymb(v15)) {
                    default:
                    label2082:
                      v16= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_378: /* i3() */
                        v17= GgetArgument(v16,0);
                        switch(GgetSymb(v17)) {
                        default:
                        label2084:
                          bitSet_set(mask,11);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label2124:
                          bitSet_set(mask,14);
                          bitSet_set(mask,17);
                          bitSet_set(mask,19);
                        }
                        break;
                      default:
                      label2083:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_377: /* i2() */
                    v19= GgetArgument(v6,0);
                    switch(GgetSymb(v19)) {
                    default:
                    label2078:
                      v20= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_377: /* i2() */
                        v21= GgetArgument(v20,0);
                        switch(GgetSymb(v21)) {
                        default:
                        label2080:
                          bitSet_set(mask,10);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label2122:
                          bitSet_set(mask,14);
                          bitSet_set(mask,17);
                          bitSet_set(mask,19);
                        }
                        break;
                      default:
                      label2079:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_376: /* i1() */
                    v23= GgetArgument(v6,0);
                    switch(GgetSymb(v23)) {
                    default:
                    label2074:
                      v24= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_376: /* i1() */
                        v25= GgetArgument(v24,0);
                        switch(GgetSymb(v25)) {
                        default:
                        label2076:
                          bitSet_set(mask,9);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label2120:
                          bitSet_set(mask,14);
                          bitSet_set(mask,17);
                          bitSet_set(mask,19);
                        }
                        break;
                      default:
                      label2075:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_374: /* e5 */
                    v27= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_374: /* e5 */
                      bitSet_set(mask,8);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label2118:
                        bitSet_set(mask,14);
                        bitSet_set(mask,17);
                        bitSet_set(mask,19);
                      }
                      break;
                    default:
                    label2072:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_373: /* e4 */
                    v29= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_373: /* e4 */
                      bitSet_set(mask,7);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label2116:
                        bitSet_set(mask,14);
                        bitSet_set(mask,17);
                        bitSet_set(mask,19);
                      }
                      break;
                    default:
                    label2070:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_372: /* e3 */
                    v31= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_372: /* e3 */
                      bitSet_set(mask,6);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label2114:
                        bitSet_set(mask,14);
                        bitSet_set(mask,17);
                        bitSet_set(mask,19);
                      }
                      break;
                    default:
                    label2068:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_371: /* e2 */
                    v33= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_371: /* e2 */
                      bitSet_set(mask,5);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label2112:
                        bitSet_set(mask,14);
                        bitSet_set(mask,17);
                        bitSet_set(mask,19);
                      }
                      break;
                    default:
                    label2066:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_370: /* e1 */
                    v35= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_370: /* e1 */
                      bitSet_set(mask,4);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label2110:
                        bitSet_set(mask,14);
                        bitSet_set(mask,17);
                        bitSet_set(mask,19);
                      }
                      break;
                    default:
                    label2064:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_375: /* f(,) */
                    v37= GgetArgument(v6,0);
                    switch(GgetSymb(v37)) {
                    default:
                    label2058:
                      v38= GgetArgument(v6,1);
                      switch(GgetSymb(v38)) {
                      default:
                      label2059:
                        v39= GgetArgument(v5,1);
                        v8= GgetArgument(v5,1);
                        switch(GgetSymb(v8)) {
                        case code_375: /* f(,) */
                          v40= GgetArgument(v39,0);
                          switch(GgetSymb(v40)) {
                          default:
                          label2061:
                            v41= GgetArgument(v39,1);
                            switch(GgetSymb(v41)) {
                            default:
                            label2062:
                              bitSet_set(mask,3);
                              bitSet_set(mask,14);
                            }
                          }
                          break;
                        case code_362: /*  */
                          v10= GgetArgument(v8,0);
                          switch(GgetSymb(v10)) {
                          default:
                          label2108:
                            bitSet_set(mask,14);
                            bitSet_set(mask,17);
                            bitSet_set(mask,19);
                          }
                          break;
                        default:
                        label2060:
                          bitSet_set(mask,14);
                        }
                      }
                    }
                    break;
                  case code_362: /*  */
                    v43= GgetArgument(v6,0);
                    switch(GgetSymb(v43)) {
                    default:
                    label2052:
                      v44= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_362: /*  */
                        v45= GgetArgument(v44,0);
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label2054:
                          bitSet_set(mask,1);
                          bitSet_set(mask,14);
                          bitSet_set(mask,15);
                          bitSet_set(mask,16);
                          bitSet_set(mask,17);
                          bitSet_set(mask,18);
                          bitSet_set(mask,19);
                        }
                        break;
                      default:
                      label2053:
                        bitSet_set(mask,14);
                        bitSet_set(mask,16);
                        bitSet_set(mask,18);
                      }
                    }
                    break;
                  default:
                  label2051:
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label2130:
                        bitSet_set(mask,14);
                        bitSet_set(mask,17);
                        bitSet_set(mask,19);
                      }
                      break;
                    default:
                    label2094:
                      bitSet_set(mask,14);
                    }
                  }
                  break;
                /* matching is not complete: jumpNode is null */
                }
                break;
              case code_426: /* (&) */
                v48= GgetArgument(v3,0);
                switch(GgetSymb(v48)) {
                default:
                label2047:
                  v49= GgetArgument(v3,1);
                  switch(GgetSymb(v49)) {
                  default:
                  label2048:
                    bitSet_set(mask,0);
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
            break;
          /* matching is not complete: jumpNode is null */
          }
          if(bitSet_get(mask,0)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(&)(var1,var2)) */
            /* allDetEvaluation: det */
            /* rhs: (&)((&)(var0,var1),var2) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_426            ,v2            ,v48            );
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_426            ,sv[0]            ,v49            );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend452:;
          }
          if(bitSet_get(mask,1)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=((var1),(var2)))) */
            /* allDetEvaluation: det */
            /* if eq_list[int](,)(var1,var2) */
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            sv[0] = fun_18( v43,v45 );
            if( sv[0] != con_1 ) {
              goto myend453;
            }
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend453:;
          }
          if(bitSet_get(mask,2)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(true)) */
            /* allDetEvaluation: det */
            /* if occurs(,)(true,var0) */
            // this=var0        underAC=false        Instantiated=false
            sv[1] = fun_17( con_1,v2 );
            if( sv[1] != con_1 ) {
              goto myend454;
            }
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend454:;
          }
          if(bitSet_get(mask,3)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(f(,)(var1,var2),f(,)(var3,var4)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)((&)(var0,(=(var1,var3))),(=(var2,var4))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v37            ,v40            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            // this=var2        underAC=false        Instantiated=false
            // this=var4        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_423            ,v38            ,v41            );
            GmakeAppl1(sv[2],code_425            ,sv[1]            );
            GmakeAppl2(sv[1],code_426            ,sv[0]            ,sv[2]            );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend455:;
          }
          if(bitSet_get(mask,4)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e1,e1))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend456:;
          }
          if(bitSet_get(mask,5)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e2,e2))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend457:;
          }
          if(bitSet_get(mask,6)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e3,e3))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend458:;
          }
          if(bitSet_get(mask,7)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e4,e4))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend459:;
          }
          if(bitSet_get(mask,8)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e5,e5))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend460:;
          }
          if(bitSet_get(mask,9)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i1()(var1),i1()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v23            ,v25            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend461:;
          }
          if(bitSet_get(mask,10)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i2()(var1),i2()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v19            ,v21            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend462:;
          }
          if(bitSet_get(mask,11)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i3()(var1),i3()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v15            ,v17            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend463:;
          }
          if(bitSet_get(mask,12)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i4()(var1),i4()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v11            ,v13            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend464:;
          }
          if(bitSet_get(mask,13)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i5()(var1),i5()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v7            ,v9            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend465:;
          }
          if(bitSet_get(mask,14)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(var1,var2))) */
            /* allDetEvaluation: det */
            /* if (and)((and)(neq_list[int](,)(head()(var1),head()(var2)),(not())(isvar()(var1))),(not())(isvar()(var2))) */
            // this=var1        underAC=false        Instantiated=false
            sv[0] = fun_363( v6 );
            // this=var2        underAC=false        Instantiated=false
            sv[1] = fun_363( v8 );
            sv[2] = fun_19( sv[0],sv[1] );
            // this=var1        underAC=false        Instantiated=false
            sv[0] = fun_365( v6 );
            sv[1] = fun_24( sv[0] );
            sv[0] = fun_21( sv[2],sv[1] );
            // this=var2        underAC=false        Instantiated=false
            sv[1] = fun_365( v8 );
            sv[2] = fun_24( sv[1] );
            sv[1] = fun_21( sv[0],sv[2] );
            if( sv[1] != con_1 ) {
              goto myend466;
            }
            /* rhs: (false) */
            GmakeAppl1(sv[2],code_424            ,con_0            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend466:;
          }
          if(bitSet_get(mask,15)) {
            Gterm *tmp, *sv[4];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=((var1),(var2)))) */
            /* allDetEvaluation: det */
            /* if neq_list[int](,)(var1,var2) */
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            sv[0] = fun_19( v43,v45 );
            if( sv[0] != con_1 ) {
              goto myend467;
            }
            /* rhs: (&)((=((var1),(var2))),apply(,)(->(var1,(var2)),var0)) */
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl1(sv[1],code_362            ,v43            );
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl1(sv[2],code_362            ,v45            );
            GmakeAppl2(sv[3],code_423            ,sv[1]            ,sv[2]            );
            GmakeAppl1(sv[1],code_425            ,sv[3]            );
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl1(sv[2],code_362            ,v45            );
            GmakeAppl2(sv[3],code_343            ,v43            ,sv[2]            );
            // this=var0        underAC=false        Instantiated=false
            sv[2] = fun_422( sv[3],v2 );
            GmakeAppl2(sv[3],code_426            ,sv[1]            ,sv[2]            );
            res = sv[3] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend467:;
          }
          if(bitSet_get(mask,16)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=((var1),var2))) */
            /* allDetEvaluation: det */
            /* if (and)(occurs(,)(var1,var2),(not())(isvar()(var2))) */
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            sv[0] = fun_17( v43,v44 );
            // this=var2        underAC=false        Instantiated=false
            sv[1] = fun_365( v44 );
            sv[2] = fun_24( sv[1] );
            sv[1] = fun_21( sv[0],sv[2] );
            if( sv[1] != con_1 ) {
              goto myend468;
            }
            /* rhs: (false) */
            GmakeAppl1(sv[2],code_424            ,con_0            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend468:;
          }
          if(bitSet_get(mask,17)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(var1,(var2)))) */
            /* allDetEvaluation: det */
            /* if (and)(occurs(,)(var2,var1),(not())(isvar()(var1))) */
            // this=var2        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            sv[0] = fun_17( v10,v6 );
            // this=var1        underAC=false        Instantiated=false
            sv[1] = fun_365( v6 );
            sv[2] = fun_24( sv[1] );
            sv[1] = fun_21( sv[0],sv[2] );
            if( sv[1] != con_1 ) {
              goto myend469;
            }
            /* rhs: (false) */
            GmakeAppl1(sv[2],code_424            ,con_0            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend469:;
          }
          if(bitSet_get(mask,18)) {
            Gterm *tmp, *sv[4];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=((var1),var2))) */
            /* allDetEvaluation: det */
            /* if (not())(occurs(,)(var1,var2)) */
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            sv[0] = fun_17( v43,v44 );
            sv[1] = fun_24( sv[0] );
            if( sv[1] != con_1 ) {
              goto myend470;
            }
            /* rhs: (&)((=((var1),var2)),apply(,)(->(var1,var2),var0)) */
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl1(sv[0],code_362            ,v43            );
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[2],code_423            ,sv[0]            ,v44            );
            GmakeAppl1(sv[0],code_425            ,sv[2]            );
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[2],code_343            ,v43            ,v44            );
            // this=var0        underAC=false        Instantiated=false
            sv[3] = fun_422( sv[2],v2 );
            GmakeAppl2(sv[2],code_426            ,sv[0]            ,sv[3]            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend470:;
          }
          if(bitSet_get(mask,19)) {
            Gterm *tmp, *sv[4];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(var1,(var2)))) */
            /* allDetEvaluation: det */
            /* if (not())(occurs(,)(var2,var1)) */
            // this=var2        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            sv[0] = fun_17( v10,v6 );
            sv[1] = fun_24( sv[0] );
            if( sv[1] != con_1 ) {
              fail();
            }
            /* rhs: (&)((=((var2),var1)),apply(,)(->(var2,var1),var0)) */
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl1(sv[0],code_362            ,v10            );
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[2],code_423            ,sv[0]            ,v6            );
            GmakeAppl1(sv[0],code_425            ,sv[2]            );
            // this=var2        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[2],code_343            ,v10            ,v6            );
            // this=var0        underAC=false        Instantiated=false
            sv[3] = fun_422( sv[2],v2 );
            GmakeAppl2(sv[2],code_426            ,sv[0]            ,sv[3]            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab96;
            myend471:;
          }
        }
        fail();
        stratLab96:;
        v0=res;
        *((Gterm**)getStablePointer(lastTerm_index)) = res;
      }
    }
    CUTCLOSE(); /* Repeat */
  }
  {
    /* one[semiDet](trueelim:eqSystem/syntacticUnification[Ops,Vars]!GL,truepropag:eqSystem/syntacticUnification[Ops,Vars]!GL,falsepropag:eqSystem/syntacticUnification[Ops,Vars]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6;
    bitSet_GC_create(mask,3);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_424: /*  */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_0: /* false */
        bitSet_set(mask,2);
        break;
      case code_1: /* true */
        bitSet_set(mask,1);
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    case code_426: /* (&) */
      v3= GgetArgument(v0,0);
      switch(GgetSymb(v3)) {
      default:
      label2133:
        v4= GgetArgument(v0,1);
        switch(GgetSymb(v4)) {
        case code_424: /*  */
          v5= GgetArgument(v4,0);
          switch(GgetSymb(v5)) {
          case code_1: /* true */
            bitSet_set(mask,0);
            break;
          /* matching is not complete: jumpNode is null */
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[1];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: (&)(var0,(true)) */
      /* allDetEvaluation: det */
      /* rhs: var0 */
      // this=var0        underAC=false        Instantiated=false
      res = v3 ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab101;
      myend472:;
    }
    if(bitSet_get(mask,1)) {
      Gterm *tmp, *sv[2];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: (true) */
      /* allDetEvaluation: det */
      /* rhs: (true) */
      GmakeAppl1(sv[1],code_424      ,con_1      );
      res = sv[1] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab101;
      myend473:;
    }
    if(bitSet_get(mask,2)) {
      Gterm *tmp, *sv[2];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: (false) */
      /* allDetEvaluation: det */
      /* rhs: (false) */
      GmakeAppl1(sv[1],code_424      ,con_0      );
      res = sv[1] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab101;
      myend474:;
    }
  }
  fail();
  stratLab101:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}

Gterm* str_203( Gterm *arg0 ) {
  Gterm *v0=arg0;
  bitSet *mask;
  Gterm *res=v0;
  /* cons[semiDet](repeat[det](one[semiDet](unfold:eqSystem/eqSystem[Ops,Vars]!GL,delete:eqSystem/syntacticUnification[Ops,Vars]!GL,decompose:eqSystem/syntacticUnification[Ops,Vars]!LO,conflict:eqSystem/syntacticUnification[Ops,Vars]!GL,coalesce:eqSystem/syntacticUnification[Ops,Vars]!GL,occCheck1:eqSystem/syntacticUnification[Ops,Vars]!GL,occCheck2:eqSystem/syntacticUnification[Ops,Vars]!GL,eliminate1:eqSystem/syntacticUnification[Ops,Vars]!GL,eliminate2:eqSystem/syntacticUnification[Ops,Vars]!GL)),one[semiDet](truepass:eqSystem/syntacticUnification[Ops,Vars]!GL,truepropag:eqSystem/syntacticUnification[Ops,Vars]!GL)) */
  {
    /* repeat[det](one[semiDet](unfold:eqSystem/eqSystem[Ops,Vars]!GL,delete:eqSystem/syntacticUnification[Ops,Vars]!GL,decompose:eqSystem/syntacticUnification[Ops,Vars]!LO,conflict:eqSystem/syntacticUnification[Ops,Vars]!GL,coalesce:eqSystem/syntacticUnification[Ops,Vars]!GL,occCheck1:eqSystem/syntacticUnification[Ops,Vars]!GL,occCheck2:eqSystem/syntacticUnification[Ops,Vars]!GL,eliminate1:eqSystem/syntacticUnification[Ops,Vars]!GL,eliminate2:eqSystem/syntacticUnification[Ops,Vars]!GL)) */
    long lastTerm_index;
    CUTOPEN(); /* Repeat */
    lastTerm_index = allocStable(sizeof(Gterm*));
    *((Gterm**)getStablePointer(lastTerm_index))=v0;
    if(localSetChoicePoint()!=0) {
      res = v0 = *((Gterm**)getStablePointer(lastTerm_index));
      /* End of repeat */
    } else {
      while(1) {
      /* Apply the strategy */
        {
          /* one[semiDet](unfold:eqSystem/eqSystem[Ops,Vars]!GL,delete:eqSystem/syntacticUnification[Ops,Vars]!GL,decompose:eqSystem/syntacticUnification[Ops,Vars]!LO,conflict:eqSystem/syntacticUnification[Ops,Vars]!GL,coalesce:eqSystem/syntacticUnification[Ops,Vars]!GL,occCheck1:eqSystem/syntacticUnification[Ops,Vars]!GL,occCheck2:eqSystem/syntacticUnification[Ops,Vars]!GL,eliminate1:eqSystem/syntacticUnification[Ops,Vars]!GL,eliminate2:eqSystem/syntacticUnification[Ops,Vars]!GL) */
          Gterm *v1,*v2,*v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12,*v13,*v14,*v15,*v16,*v17,*v18,*v19,*v20,*v21,*v22,*v23,*v24,*v25,*v26,*v27,*v28,*v29,*v30,*v31,*v32,*v33,*v34,*v35,*v36,*v37,*v38,*v39,*v40,*v41,*v42,*v43,*v44,*v45,*v46,*v47,*v48,*v49,*v50;
          bitSet_GC_create(mask,20);
          bitSet_init_clear(mask);
          switch(GgetSymb(v0)) {
          case code_426: /* (&) */
            v2= GgetArgument(v0,0);
            switch(GgetSymb(v2)) {
            default:
            label2141:
              v3= GgetArgument(v0,1);
              switch(GgetSymb(v3)) {
              case code_424: /*  */
                v4= GgetArgument(v3,0);
                switch(GgetSymb(v4)) {
                case code_1: /* true */
                  bitSet_set(mask,2);
                  break;
                /* matching is not complete: jumpNode is null */
                }
                break;
              case code_425: /*  */
                v5= GgetArgument(v3,0);
                switch(GgetSymb(v5)) {
                case code_423: /* = */
                  v6= GgetArgument(v5,0);
                  switch(GgetSymb(v6)) {
                  case code_380: /* i5() */
                    v7= GgetArgument(v6,0);
                    switch(GgetSymb(v7)) {
                    default:
                    label2186:
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_380: /* i5() */
                        v9= GgetArgument(v8,0);
                        switch(GgetSymb(v9)) {
                        default:
                        label2188:
                          bitSet_set(mask,13);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label2224:
                          bitSet_set(mask,14);
                          bitSet_set(mask,17);
                          bitSet_set(mask,19);
                        }
                        break;
                      default:
                      label2187:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_379: /* i4() */
                    v11= GgetArgument(v6,0);
                    switch(GgetSymb(v11)) {
                    default:
                    label2182:
                      v12= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_379: /* i4() */
                        v13= GgetArgument(v12,0);
                        switch(GgetSymb(v13)) {
                        default:
                        label2184:
                          bitSet_set(mask,12);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label2222:
                          bitSet_set(mask,14);
                          bitSet_set(mask,17);
                          bitSet_set(mask,19);
                        }
                        break;
                      default:
                      label2183:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_378: /* i3() */
                    v15= GgetArgument(v6,0);
                    switch(GgetSymb(v15)) {
                    default:
                    label2178:
                      v16= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_378: /* i3() */
                        v17= GgetArgument(v16,0);
                        switch(GgetSymb(v17)) {
                        default:
                        label2180:
                          bitSet_set(mask,11);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label2220:
                          bitSet_set(mask,14);
                          bitSet_set(mask,17);
                          bitSet_set(mask,19);
                        }
                        break;
                      default:
                      label2179:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_377: /* i2() */
                    v19= GgetArgument(v6,0);
                    switch(GgetSymb(v19)) {
                    default:
                    label2174:
                      v20= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_377: /* i2() */
                        v21= GgetArgument(v20,0);
                        switch(GgetSymb(v21)) {
                        default:
                        label2176:
                          bitSet_set(mask,10);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label2218:
                          bitSet_set(mask,14);
                          bitSet_set(mask,17);
                          bitSet_set(mask,19);
                        }
                        break;
                      default:
                      label2175:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_376: /* i1() */
                    v23= GgetArgument(v6,0);
                    switch(GgetSymb(v23)) {
                    default:
                    label2170:
                      v24= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_376: /* i1() */
                        v25= GgetArgument(v24,0);
                        switch(GgetSymb(v25)) {
                        default:
                        label2172:
                          bitSet_set(mask,9);
                          bitSet_set(mask,14);
                        }
                        break;
                      case code_362: /*  */
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label2216:
                          bitSet_set(mask,14);
                          bitSet_set(mask,17);
                          bitSet_set(mask,19);
                        }
                        break;
                      default:
                      label2171:
                        bitSet_set(mask,14);
                      }
                    }
                    break;
                  case code_374: /* e5 */
                    v27= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_374: /* e5 */
                      bitSet_set(mask,8);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label2214:
                        bitSet_set(mask,14);
                        bitSet_set(mask,17);
                        bitSet_set(mask,19);
                      }
                      break;
                    default:
                    label2168:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_373: /* e4 */
                    v29= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_373: /* e4 */
                      bitSet_set(mask,7);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label2212:
                        bitSet_set(mask,14);
                        bitSet_set(mask,17);
                        bitSet_set(mask,19);
                      }
                      break;
                    default:
                    label2166:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_372: /* e3 */
                    v31= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_372: /* e3 */
                      bitSet_set(mask,6);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label2210:
                        bitSet_set(mask,14);
                        bitSet_set(mask,17);
                        bitSet_set(mask,19);
                      }
                      break;
                    default:
                    label2164:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_371: /* e2 */
                    v33= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_371: /* e2 */
                      bitSet_set(mask,5);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label2208:
                        bitSet_set(mask,14);
                        bitSet_set(mask,17);
                        bitSet_set(mask,19);
                      }
                      break;
                    default:
                    label2162:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_370: /* e1 */
                    v35= GgetArgument(v5,1);
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_370: /* e1 */
                      bitSet_set(mask,4);
                      bitSet_set(mask,14);
                      break;
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label2206:
                        bitSet_set(mask,14);
                        bitSet_set(mask,17);
                        bitSet_set(mask,19);
                      }
                      break;
                    default:
                    label2160:
                      bitSet_set(mask,14);
                    }
                    break;
                  case code_375: /* f(,) */
                    v37= GgetArgument(v6,0);
                    switch(GgetSymb(v37)) {
                    default:
                    label2154:
                      v38= GgetArgument(v6,1);
                      switch(GgetSymb(v38)) {
                      default:
                      label2155:
                        v39= GgetArgument(v5,1);
                        v8= GgetArgument(v5,1);
                        switch(GgetSymb(v8)) {
                        case code_375: /* f(,) */
                          v40= GgetArgument(v39,0);
                          switch(GgetSymb(v40)) {
                          default:
                          label2157:
                            v41= GgetArgument(v39,1);
                            switch(GgetSymb(v41)) {
                            default:
                            label2158:
                              bitSet_set(mask,3);
                              bitSet_set(mask,14);
                            }
                          }
                          break;
                        case code_362: /*  */
                          v10= GgetArgument(v8,0);
                          switch(GgetSymb(v10)) {
                          default:
                          label2204:
                            bitSet_set(mask,14);
                            bitSet_set(mask,17);
                            bitSet_set(mask,19);
                          }
                          break;
                        default:
                        label2156:
                          bitSet_set(mask,14);
                        }
                      }
                    }
                    break;
                  case code_362: /*  */
                    v43= GgetArgument(v6,0);
                    switch(GgetSymb(v43)) {
                    default:
                    label2148:
                      v44= GgetArgument(v5,1);
                      v8= GgetArgument(v5,1);
                      switch(GgetSymb(v8)) {
                      case code_362: /*  */
                        v45= GgetArgument(v44,0);
                        v10= GgetArgument(v8,0);
                        switch(GgetSymb(v10)) {
                        default:
                        label2150:
                          bitSet_set(mask,1);
                          bitSet_set(mask,14);
                          bitSet_set(mask,15);
                          bitSet_set(mask,16);
                          bitSet_set(mask,17);
                          bitSet_set(mask,18);
                          bitSet_set(mask,19);
                        }
                        break;
                      default:
                      label2149:
                        bitSet_set(mask,14);
                        bitSet_set(mask,16);
                        bitSet_set(mask,18);
                      }
                    }
                    break;
                  default:
                  label2147:
                    v8= GgetArgument(v5,1);
                    switch(GgetSymb(v8)) {
                    case code_362: /*  */
                      v10= GgetArgument(v8,0);
                      switch(GgetSymb(v10)) {
                      default:
                      label2226:
                        bitSet_set(mask,14);
                        bitSet_set(mask,17);
                        bitSet_set(mask,19);
                      }
                      break;
                    default:
                    label2190:
                      bitSet_set(mask,14);
                    }
                  }
                  break;
                /* matching is not complete: jumpNode is null */
                }
                break;
              case code_426: /* (&) */
                v48= GgetArgument(v3,0);
                switch(GgetSymb(v48)) {
                default:
                label2143:
                  v49= GgetArgument(v3,1);
                  switch(GgetSymb(v49)) {
                  default:
                  label2144:
                    bitSet_set(mask,0);
                  }
                }
                break;
              /* matching is not complete: jumpNode is null */
              }
            }
            break;
          /* matching is not complete: jumpNode is null */
          }
          if(bitSet_get(mask,0)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(&)(var1,var2)) */
            /* allDetEvaluation: det */
            /* rhs: (&)((&)(var0,var1),var2) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_426            ,v2            ,v48            );
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_426            ,sv[0]            ,v49            );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend475:;
          }
          if(bitSet_get(mask,1)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=((var1),(var2)))) */
            /* allDetEvaluation: det */
            /* if eq_list[int](,)(var1,var2) */
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            sv[0] = fun_18( v43,v45 );
            if( sv[0] != con_1 ) {
              goto myend476;
            }
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend476:;
          }
          if(bitSet_get(mask,2)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(true)) */
            /* allDetEvaluation: det */
            /* if occurs(,)(true,var0) */
            // this=var0        underAC=false        Instantiated=false
            sv[1] = fun_17( con_1,v2 );
            if( sv[1] != con_1 ) {
              goto myend477;
            }
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend477:;
          }
          if(bitSet_get(mask,3)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(f(,)(var1,var2),f(,)(var3,var4)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)((&)(var0,(=(var1,var3))),(=(var2,var4))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var3        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v37            ,v40            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            // this=var2        underAC=false        Instantiated=false
            // this=var4        underAC=false        Instantiated=false
            GmakeAppl2(sv[1],code_423            ,v38            ,v41            );
            GmakeAppl1(sv[2],code_425            ,sv[1]            );
            GmakeAppl2(sv[1],code_426            ,sv[0]            ,sv[2]            );
            res = sv[1] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend478:;
          }
          if(bitSet_get(mask,4)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e1,e1))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend479:;
          }
          if(bitSet_get(mask,5)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e2,e2))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend480:;
          }
          if(bitSet_get(mask,6)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e3,e3))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend481:;
          }
          if(bitSet_get(mask,7)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e4,e4))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend482:;
          }
          if(bitSet_get(mask,8)) {
            Gterm *tmp, *sv[1];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(e5,e5))) */
            /* allDetEvaluation: det */
            /* rhs: var0 */
            // this=var0        underAC=false        Instantiated=false
            res = v2 ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend483:;
          }
          if(bitSet_get(mask,9)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i1()(var1),i1()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v23            ,v25            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend484:;
          }
          if(bitSet_get(mask,10)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i2()(var1),i2()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v19            ,v21            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend485:;
          }
          if(bitSet_get(mask,11)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i3()(var1),i3()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v15            ,v17            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend486:;
          }
          if(bitSet_get(mask,12)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i4()(var1),i4()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v11            ,v13            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend487:;
          }
          if(bitSet_get(mask,13)) {
            Gterm *tmp, *sv[2];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(i5()(var1),i5()(var2)))) */
            /* allDetEvaluation: det */
            /* rhs: (&)(var0,(=(var1,var2))) */
            // this=var0        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[0],code_423            ,v7            ,v9            );
            GmakeAppl1(sv[1],code_425            ,sv[0]            );
            GmakeAppl2(sv[0],code_426            ,v2            ,sv[1]            );
            res = sv[0] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend488:;
          }
          if(bitSet_get(mask,14)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(var1,var2))) */
            /* allDetEvaluation: det */
            /* if (and)((and)(neq_list[int](,)(head()(var1),head()(var2)),(not())(isvar()(var1))),(not())(isvar()(var2))) */
            // this=var1        underAC=false        Instantiated=false
            sv[0] = fun_363( v6 );
            // this=var2        underAC=false        Instantiated=false
            sv[1] = fun_363( v8 );
            sv[2] = fun_19( sv[0],sv[1] );
            // this=var1        underAC=false        Instantiated=false
            sv[0] = fun_365( v6 );
            sv[1] = fun_24( sv[0] );
            sv[0] = fun_21( sv[2],sv[1] );
            // this=var2        underAC=false        Instantiated=false
            sv[1] = fun_365( v8 );
            sv[2] = fun_24( sv[1] );
            sv[1] = fun_21( sv[0],sv[2] );
            if( sv[1] != con_1 ) {
              goto myend489;
            }
            /* rhs: (false) */
            GmakeAppl1(sv[2],code_424            ,con_0            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend489:;
          }
          if(bitSet_get(mask,15)) {
            Gterm *tmp, *sv[4];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=((var1),(var2)))) */
            /* allDetEvaluation: det */
            /* if neq_list[int](,)(var1,var2) */
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            sv[0] = fun_19( v43,v45 );
            if( sv[0] != con_1 ) {
              goto myend490;
            }
            /* rhs: (&)((=((var1),(var2))),apply(,)(->(var1,(var2)),var0)) */
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl1(sv[1],code_362            ,v43            );
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl1(sv[2],code_362            ,v45            );
            GmakeAppl2(sv[3],code_423            ,sv[1]            ,sv[2]            );
            GmakeAppl1(sv[1],code_425            ,sv[3]            );
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl1(sv[2],code_362            ,v45            );
            GmakeAppl2(sv[3],code_343            ,v43            ,sv[2]            );
            // this=var0        underAC=false        Instantiated=false
            sv[2] = fun_422( sv[3],v2 );
            GmakeAppl2(sv[3],code_426            ,sv[1]            ,sv[2]            );
            res = sv[3] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend490:;
          }
          if(bitSet_get(mask,16)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=((var1),var2))) */
            /* allDetEvaluation: det */
            /* if (and)(occurs(,)(var1,var2),(not())(isvar()(var2))) */
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            sv[0] = fun_17( v43,v44 );
            // this=var2        underAC=false        Instantiated=false
            sv[1] = fun_365( v44 );
            sv[2] = fun_24( sv[1] );
            sv[1] = fun_21( sv[0],sv[2] );
            if( sv[1] != con_1 ) {
              goto myend491;
            }
            /* rhs: (false) */
            GmakeAppl1(sv[2],code_424            ,con_0            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend491:;
          }
          if(bitSet_get(mask,17)) {
            Gterm *tmp, *sv[3];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(var1,(var2)))) */
            /* allDetEvaluation: det */
            /* if (and)(occurs(,)(var2,var1),(not())(isvar()(var1))) */
            // this=var2        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            sv[0] = fun_17( v10,v6 );
            // this=var1        underAC=false        Instantiated=false
            sv[1] = fun_365( v6 );
            sv[2] = fun_24( sv[1] );
            sv[1] = fun_21( sv[0],sv[2] );
            if( sv[1] != con_1 ) {
              goto myend492;
            }
            /* rhs: (false) */
            GmakeAppl1(sv[2],code_424            ,con_0            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend492:;
          }
          if(bitSet_get(mask,18)) {
            Gterm *tmp, *sv[4];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=((var1),var2))) */
            /* allDetEvaluation: det */
            /* if (not())(occurs(,)(var1,var2)) */
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            sv[0] = fun_17( v43,v44 );
            sv[1] = fun_24( sv[0] );
            if( sv[1] != con_1 ) {
              goto myend493;
            }
            /* rhs: (&)((=((var1),var2)),apply(,)(->(var1,var2),var0)) */
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl1(sv[0],code_362            ,v43            );
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[2],code_423            ,sv[0]            ,v44            );
            GmakeAppl1(sv[0],code_425            ,sv[2]            );
            // this=var1        underAC=false        Instantiated=false
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl2(sv[2],code_343            ,v43            ,v44            );
            // this=var0        underAC=false        Instantiated=false
            sv[3] = fun_422( sv[2],v2 );
            GmakeAppl2(sv[2],code_426            ,sv[0]            ,sv[3]            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend493:;
          }
          if(bitSet_get(mask,19)) {
            Gterm *tmp, *sv[4];
            multiplicityType *E,*sol;
            Gterm *substitution[1];
            /* lhs: (&)(var0,(=(var1,(var2)))) */
            /* allDetEvaluation: det */
            /* if (not())(occurs(,)(var2,var1)) */
            // this=var2        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            sv[0] = fun_17( v10,v6 );
            sv[1] = fun_24( sv[0] );
            if( sv[1] != con_1 ) {
              fail();
            }
            /* rhs: (&)((=((var2),var1)),apply(,)(->(var2,var1),var0)) */
            // this=var2        underAC=false        Instantiated=false
            GmakeAppl1(sv[0],code_362            ,v10            );
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[2],code_423            ,sv[0]            ,v6            );
            GmakeAppl1(sv[0],code_425            ,sv[2]            );
            // this=var2        underAC=false        Instantiated=false
            // this=var1        underAC=false        Instantiated=false
            GmakeAppl2(sv[2],code_343            ,v10            ,v6            );
            // this=var0        underAC=false        Instantiated=false
            sv[3] = fun_422( sv[2],v2 );
            GmakeAppl2(sv[2],code_426            ,sv[0]            ,sv[3]            );
            res = sv[2] ;
            rewrite_step++;
            rewrite_label_step++;
            goto stratLab170;
            myend494:;
          }
        }
        fail();
        stratLab170:;
        v0=res;
        *((Gterm**)getStablePointer(lastTerm_index)) = res;
      }
    }
    CUTCLOSE(); /* Repeat */
  }
  {
    /* one[semiDet](truepass:eqSystem/syntacticUnification[Ops,Vars]!GL,truepropag:eqSystem/syntacticUnification[Ops,Vars]!GL) */
    Gterm *v1,*v2,*v3,*v4,*v5,*v6;
    bitSet_GC_create(mask,2);
    bitSet_init_clear(mask);
    switch(GgetSymb(v0)) {
    case code_424: /*  */
      v2= GgetArgument(v0,0);
      switch(GgetSymb(v2)) {
      case code_1: /* true */
        bitSet_set(mask,1);
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    case code_426: /* (&) */
      v3= GgetArgument(v0,0);
      switch(GgetSymb(v3)) {
      default:
      label2229:
        v4= GgetArgument(v0,1);
        switch(GgetSymb(v4)) {
        case code_424: /*  */
          v5= GgetArgument(v4,0);
          switch(GgetSymb(v5)) {
          case code_1: /* true */
            bitSet_set(mask,0);
            break;
          /* matching is not complete: jumpNode is null */
          }
          break;
        /* matching is not complete: jumpNode is null */
        }
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      Gterm *tmp, *sv[2];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: (&)(var0,(true)) */
      /* allDetEvaluation: det */
      /* rhs: (&)((true),var0) */
      GmakeAppl1(sv[1],code_424      ,con_1      );
      // this=var0        underAC=false        Instantiated=false
      GmakeAppl2(sv[0],code_426      ,sv[1]      ,v3      );
      res = sv[0] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab174;
      myend495:;
    }
    if(bitSet_get(mask,1)) {
      Gterm *tmp, *sv[2];
      multiplicityType *E,*sol;
      Gterm *substitution[1];
      /* lhs: (true) */
      /* allDetEvaluation: det */
      /* rhs: (true) */
      GmakeAppl1(sv[1],code_424      ,con_1      );
      res = sv[1] ;
      rewrite_step++;
      rewrite_label_step++;
      goto stratLab174;
      myend496:;
    }
  }
  fail();
  stratLab174:;
  v0=res;
end:
  return res;
  fail:
  // printf("fail\n");
  fail();
}
