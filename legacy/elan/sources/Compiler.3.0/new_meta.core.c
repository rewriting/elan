#include "new_meta.h"

struct term* fun_235(struct term *v1,struct term *v2,struct term *v3,struct term *v4 ) {
  struct term *v5,*v6,*v7,*v8,*v9,*v10;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_235_%s_(",fsymtab[235].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(",");
    term_print(stdout,v3);
    printf(",");
    term_print(stdout,v4);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label2:
    switch(getSymb(v2)) {
    default:
    label3:
      switch(getInt(v3)) {
      default:
      label4:
        switch(getInt(v4)) {
        default:
        label5:
          bitSet32_set(mask32,0);
        }
      }
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[3];
    struct term *substitution[1];
    /* lhs: meta_apply(,,,)(var0,var1,var2,var3) */
    /* allDetEvaluation: det */
    /* where var4 := meta_apply(,,,)(var0,.(var1,nil),var2,var3) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term2,code_219);
    sv[2]->sub[0] = v2;
    sv[2]->sub[1] = con_218;
    // this=var2        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    sv[1] = fun_236( v1,sv[2],v3,v4 );
    tmp = sv[0] = sv[1];
    /* rhs: var4 */
    // this=var4        underAC=false        Instantiated=false
    res = sv[0] ;
    goto end;
    myend0:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term4, 235);
  res->sub[0] = v1;
  res->sub[1] = v2;
  res->sub[2] = v3;
  res->sub[3] = v4;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_223(struct term *v1 ) {
  struct term *v2,*v3,*v4,*v5,*v6;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_223_%s_(",fsymtab[223].name);
    term_print(stdout,v1);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_219: /* . */
    v4=v1->sub[0];
    switch(getSymb(v4)) {
    default:
    label10:
      v5=v1->sub[1];
      switch(getSymb(v5)) {
      default:
      label11:
        bitSet32_set(mask32,1);
      }
    }
    break;
  case code_218: /* nil */
    bitSet32_set(mask32,0);
    break;
  /* matching is not complete: jumpNode is null */
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[2];
    struct term *substitution[1];
    /* lhs: size_of_Foo_list()(nil) */
    /* allDetEvaluation: det */
    /* rhs: [](0) */
    TERM_ALLOC(sv[1],term1,code_200);
    sv[1]->sub[0] = (setIntegerTag(0));
    res = sv[1] ;
    goto end;
    myend1:;
    restoreGlobalIndent();
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[3];
    struct term *substitution[1];
    /* lhs: size_of_Foo_list()(.(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: plus(,)([](1),size_of_Foo_list()(var1)) */
    TERM_ALLOC(sv[1],term1,code_200);
    sv[1]->sub[0] = (setIntegerTag(1));
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_223( v5 );
    sv[2] = fun_201( sv[1],sv[0] );
    res = sv[2] ;
    goto end;
    myend2:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term1, 223);
  res->sub[0] = v1;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_216(struct term *v1 ) {
  struct term *v2,*v3,*v4,*v5;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_216_%s_(",fsymtab[216].name);
    term_print(stdout,v1);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_200: /* [] */
    v4=v1->sub[0];
    switch(getInt(v4)) {
    default:
    label15:
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
    /* rhs: itob_builtinInt()(var0) */
    // this=var0        underAC=false        Instantiated=false
    sv[0] = fun_26( v4 );
    res = sv[0] ;
    goto end;
    myend3:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term1, 216);
  res->sub[0] = v1;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_227(struct term *v1,struct term *v2,struct term *v3 ) {
  struct term *v4,*v5,*v6,*v7,*v8;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_227_%s_(",fsymtab[227].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(",");
    term_print(stdout,v3);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label18:
    switch(getSymb(v2)) {
    default:
    label19:
      switch(getSymb(v3)) {
      default:
      label20:
        bitSet32_set(mask32,0);
      }
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[5];
    struct term *substitution[1];
    /* lhs: [<-](var0,var1,var2) */
    /* allDetEvaluation: det */
    /* where var3 := valueOf()(var1) */
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_217( v2 );
    tmp = sv[0] = sv[1];
    /* where var4 := valueOf()(var2) */
    // this=var2        underAC=false        Instantiated=false
    sv[3] = fun_217( v3 );
    tmp = sv[2] = sv[3];
    /* rhs: intern[<-](var0,var3,var4) */
    // this=var0        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    // this=var4        underAC=false        Instantiated=false
    sv[4] = fun_153( v1,sv[0],sv[2] );
    res = sv[4] ;
    goto end;
    myend4:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term3, 227);
  res->sub[0] = v1;
  res->sub[1] = v2;
  res->sub[2] = v3;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_213(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_213_%s_(",fsymtab[213].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_200: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label24:
      switch(getSymb(v2)) {
      case code_200: /* [] */
        v7=v2->sub[0];
        switch(getInt(v7)) {
        default:
        label26:
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
    /* rhs: lesseq_builtinInt(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_11( v5,v7 );
    res = sv[0] ;
    goto end;
    myend5:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term2, 213);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_207(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_207_%s_(",fsymtab[207].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_200: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label30:
      switch(getSymb(v2)) {
      case code_200: /* [] */
        v7=v2->sub[0];
        switch(getInt(v7)) {
        default:
        label32:
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
    /* where var2 := or(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_29( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_200);
    sv[2]->sub[0] = sv[0];
    res = sv[2] ;
    goto end;
    myend6:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term2, 207);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_215(struct term *v1 ) {
  struct term *v2,*v3,*v4;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_215_%s_(",fsymtab[215].name);
    term_print(stdout,v1);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label35:
    bitSet32_set(mask32,0);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[3];
    struct term *substitution[1];
    /* lhs: btoi_int()(var0) */
    /* allDetEvaluation: det */
    /* where var1 := btoi_builtinInt()(var0) */
    // this=var0        underAC=false        Instantiated=false
    sv[1] = fun_25( v1 );
    tmp = sv[0] = sv[1];
    /* rhs: [](var1) */
    // this=var1        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_200);
    sv[2]->sub[0] = sv[0];
    res = sv[2] ;
    goto end;
    myend7:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term1, 215);
  res->sub[0] = v1;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_201(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_201_%s_(",fsymtab[201].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_200: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label39:
      switch(getSymb(v2)) {
      case code_200: /* [] */
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
    /* lhs: plus(,)([](var0),[](var1)) */
    /* allDetEvaluation: det */
    /* where var2 := plus(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_3( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_200);
    sv[2]->sub[0] = sv[0];
    res = sv[2] ;
    goto end;
    myend8:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term2, 201);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_212(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_212_%s_(",fsymtab[212].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_200: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label45:
      switch(getSymb(v2)) {
      case code_200: /* [] */
        v7=v2->sub[0];
        switch(getInt(v7)) {
        default:
        label47:
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
    /* rhs: greatereq_builtinInt(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_13( v5,v7 );
    res = sv[0] ;
    goto end;
    myend9:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term2, 212);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_232(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_232_%s_(",fsymtab[232].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label50:
    switch(getSymb(v2)) {
    default:
    label51:
      bitSet32_set(mask32,0);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[5];
    struct term *substitution[1];
    /* lhs: set_of(,)(var0,var1) */
    /* allDetEvaluation: nonDet */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend10;
    }
    /* where var2 := (WHERE0:list[Foo]/Meta_apply[Foo]) meta_apply(,,,)(var0,.(var1,nil),0,0) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term2,code_219);
    sv[2]->sub[0] = v2;
    sv[2]->sub[1] = con_218;
    sv[4] = fun_236( v1,sv[2],(setIntegerTag(0)),(setIntegerTag(0)) );
    sv[0] = str_464( sv[4] );
    /* rhs: var2 */
    // this=var2        underAC=false        Instantiated=false
    res = sv[0] ;
    CUTCLOSE(); /* Wheres */
    goto end;
    myend10:;
    restoreGlobalIndent();
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term2, 232);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_217(struct term *v1 ) {
  struct term *v2,*v3,*v4,*v5;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_217_%s_(",fsymtab[217].name);
    term_print(stdout,v1);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_200: /* [] */
    v4=v1->sub[0];
    switch(getInt(v4)) {
    default:
    label55:
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
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v4 ;
    goto end;
    myend11:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term1, 217);
  res->sub[0] = v1;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_203(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_203_%s_(",fsymtab[203].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_200: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label59:
      switch(getSymb(v2)) {
      case code_200: /* [] */
        v7=v2->sub[0];
        switch(getInt(v7)) {
        default:
        label61:
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
    /* where var2 := time(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_5( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_200);
    sv[2]->sub[0] = sv[0];
    res = sv[2] ;
    goto end;
    myend12:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term2, 203);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_210(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_210_%s_(",fsymtab[210].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_200: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label65:
      switch(getSymb(v2)) {
      case code_200: /* [] */
        v7=v2->sub[0];
        switch(getInt(v7)) {
        default:
        label67:
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
    /* rhs: neq_builtinInt(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_9( v5,v7 );
    res = sv[0] ;
    goto end;
    myend13:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term2, 210);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_214(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_214_%s_(",fsymtab[214].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_200: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label71:
      switch(getSymb(v2)) {
      case code_200: /* [] */
        v7=v2->sub[0];
        switch(getInt(v7)) {
        default:
        label73:
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
    /* rhs: less_builtinInt(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_10( v5,v7 );
    res = sv[0] ;
    goto end;
    myend14:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term2, 214);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_228(struct term *v1,struct term *v2,struct term *v3 ) {
  struct term *v4,*v5,*v6,*v7,*v8;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_228_%s_(",fsymtab[228].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(",");
    term_print(stdout,v3);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label76:
    switch(getSymb(v2)) {
    default:
    label77:
      switch(getSymb(v3)) {
      default:
      label78:
        bitSet32_set(mask32,0);
      }
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[5];
    struct term *substitution[1];
    /* lhs: substr(,,)(var0,var1,var2) */
    /* allDetEvaluation: det */
    /* where var3 := valueOf()(var1) */
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_217( v2 );
    tmp = sv[0] = sv[1];
    /* where var4 := valueOf()(var2) */
    // this=var2        underAC=false        Instantiated=false
    sv[3] = fun_217( v3 );
    tmp = sv[2] = sv[3];
    /* rhs: internsubstr(,,)(var0,var3,var4) */
    // this=var0        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    // this=var4        underAC=false        Instantiated=false
    sv[4] = fun_154( v1,sv[0],sv[2] );
    res = sv[4] ;
    goto end;
    myend15:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term3, 228);
  res->sub[0] = v1;
  res->sub[1] = v2;
  res->sub[2] = v3;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_205(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_205_%s_(",fsymtab[205].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_200: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label82:
      switch(getSymb(v2)) {
      case code_200: /* [] */
        v7=v2->sub[0];
        switch(getInt(v7)) {
        default:
        label84:
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
    /* where var2 := and(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_28( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_200);
    sv[2]->sub[0] = sv[0];
    res = sv[2] ;
    goto end;
    myend16:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term2, 205);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_209(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_209_%s_(",fsymtab[209].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_200: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label88:
      switch(getSymb(v2)) {
      case code_200: /* [] */
        v7=v2->sub[0];
        switch(getInt(v7)) {
        default:
        label90:
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
    /* rhs: eq_builtinInt(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_8( v5,v7 );
    res = sv[0] ;
    goto end;
    myend17:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term2, 209);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_234(struct term *v1,struct term *v2,struct term *v3,struct term *v4 ) {
  struct term *v5,*v6,*v7,*v8,*v9,*v10;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_234_%s_(",fsymtab[234].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(",");
    term_print(stdout,v3);
    printf(",");
    term_print(stdout,v4);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label93:
    switch(getSymb(v2)) {
    default:
    label94:
      switch(getInt(v3)) {
      default:
      label95:
        switch(getInt(v4)) {
        default:
        label96:
          bitSet32_set(mask32,0);
        }
      }
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[3];
    struct term *substitution[1];
    /* lhs: set_of(,,,)(var0,var1,var2,var3) */
    /* allDetEvaluation: nonDet */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend18;
    }
    /* where var4 := (WHERE2:list[Foo]/Meta_apply[Foo]) meta_apply(,,,)(var0,.(var1,nil),var2,var3) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term2,code_219);
    sv[2]->sub[0] = v2;
    sv[2]->sub[1] = con_218;
    // this=var2        underAC=false        Instantiated=false
    // this=var3        underAC=false        Instantiated=false
    sv[1] = fun_236( v1,sv[2],v3,v4 );
    sv[0] = str_466( sv[1] );
    /* rhs: var4 */
    // this=var4        underAC=false        Instantiated=false
    res = sv[0] ;
    CUTCLOSE(); /* Wheres */
    goto end;
    myend18:;
    restoreGlobalIndent();
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term4, 234);
  res->sub[0] = v1;
  res->sub[1] = v2;
  res->sub[2] = v3;
  res->sub[3] = v4;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_220(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8,*v9;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_220_%s_(",fsymtab[220].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_219: /* . */
    v5=v1->sub[0];
    switch(getSymb(v5)) {
    default:
    label102:
      v6=v1->sub[1];
      switch(getSymb(v6)) {
      default:
      label103:
        switch(getSymb(v2)) {
        default:
        label104:
          bitSet32_set(mask32,1);
        }
      }
    }
    break;
  case code_218: /* nil */
    switch(getSymb(v2)) {
    default:
    label100:
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
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v2 ;
    goto end;
    myend19:;
    restoreGlobalIndent();
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[2];
    struct term *substitution[1];
    /* lhs: @(.(var0,var1),var2) */
    /* allDetEvaluation: det */
    /* rhs: .(var0,@(var1,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[0] = fun_220( v6,v2 );
    TERM_ALLOC(sv[1],term2,code_219);
    sv[1]->sub[0] = v5;
    sv[1]->sub[1] = sv[0];
    res = sv[1] ;
    goto end;
    myend20:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term2, 220);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_211(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_211_%s_(",fsymtab[211].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_200: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label108:
      switch(getSymb(v2)) {
      case code_200: /* [] */
        v7=v2->sub[0];
        switch(getInt(v7)) {
        default:
        label110:
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
    /* rhs: greater_builtinInt(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[0] = fun_12( v5,v7 );
    res = sv[0] ;
    goto end;
    myend21:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term2, 211);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_202(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_202_%s_(",fsymtab[202].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_200: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label114:
      switch(getSymb(v2)) {
      case code_200: /* [] */
        v7=v2->sub[0];
        switch(getInt(v7)) {
        default:
        label116:
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
    /* where var2 := minus(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_4( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_200);
    sv[2]->sub[0] = sv[0];
    res = sv[2] ;
    goto end;
    myend22:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term2, 202);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_226(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_226_%s_(",fsymtab[226].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label119:
    switch(getSymb(v2)) {
    default:
    label120:
      bitSet32_set(mask32,0);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[4];
    struct term *substitution[1];
    /* lhs: [](var0,var1) */
    /* allDetEvaluation: det */
    /* where var2 := valueOf()(var1) */
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_217( v2 );
    tmp = sv[0] = sv[1];
    /* rhs: [](intern[](var0,var2)) */
    // this=var0        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[2] = fun_152( v1,sv[0] );
    TERM_ALLOC(sv[3],term1,code_200);
    sv[3]->sub[0] = sv[2];
    res = sv[3] ;
    goto end;
    myend23:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term2, 226);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_222(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8,*v9,*v10,*v11,*v12;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_222_%s_(",fsymtab[222].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_200: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    case 1: /* 1 */
      switch(getSymb(v2)) {
      case code_219: /* . */
        v7=v2->sub[0];
        switch(getSymb(v7)) {
        default:
        label126:
          v8=v2->sub[1];
          switch(getSymb(v8)) {
          default:
          label127:
            bitSet32_set(mask32,0);
            bitSet32_set(mask32,1);
          }
        }
        break;
      /* matching is not complete: jumpNode is null */
      }
      break;
    default:
      goto label123;
    }
    break;
  default:
  label123:
    switch(getSymb(v2)) {
    case code_219: /* . */
      v7=v2->sub[0];
      switch(getSymb(v7)) {
      default:
      label130:
        v8=v2->sub[1];
        switch(getSymb(v8)) {
        default:
        label131:
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
    /* lhs: -thelem()([](1),.(var0,var1)) */
    /* allDetEvaluation: det */
    /* rhs: var0 */
    // this=var0        underAC=false        Instantiated=false
    res = v7 ;
    goto end;
    myend24:;
    restoreGlobalIndent();
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[3];
    struct term *substitution[1];
    /* lhs: -thelem()(var0,.(var1,var2)) */
    /* allDetEvaluation: det */
    /* where var3 := minus(,)(var0,[](1)) */
    // this=var0        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_200);
    sv[2]->sub[0] = (setIntegerTag(1));
    sv[1] = fun_202( v1,sv[2] );
    tmp = sv[0] = sv[1];
    /* rhs: -thelem()(var3,var2) */
    // this=var3        underAC=false        Instantiated=false
    // this=var2        underAC=false        Instantiated=false
    sv[2] = fun_222( sv[0],v8 );
    res = sv[2] ;
    goto end;
    myend25:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term2, 222);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_204(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_204_%s_(",fsymtab[204].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_200: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label135:
      switch(getSymb(v2)) {
      case code_200: /* [] */
        v7=v2->sub[0];
        switch(getInt(v7)) {
        default:
        label137:
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
    /* where var2 := mod(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_27( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_200);
    sv[2]->sub[0] = sv[0];
    res = sv[2] ;
    goto end;
    myend26:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term2, 204);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_224(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,2);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_224_%s_(",fsymtab[224].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_200: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    case 0: /* 0 */
      switch(getSymb(v2)) {
      default:
      label142:
        bitSet32_set(mask32,0);
        bitSet32_set(mask32,1);
      }
      break;
    default:
      goto label140;
    }
    break;
  default:
  label140:
    switch(getSymb(v2)) {
    default:
    label144:
      bitSet32_set(mask32,1);
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[1];
    struct term *substitution[1];
    /* lhs: ccat(,)([](0),var0) */
    /* allDetEvaluation: det */
    /* rhs: nil */
    res = con_218 ;
    goto end;
    myend27:;
    restoreGlobalIndent();
  }
  if(bitSet32_get(mask32,1)) {
    struct term *tmp, *sv[3];
    struct term *substitution[1];
    /* lhs: ccat(,)(var0,var1) */
    /* allDetEvaluation: det */
    /* if greater_int(,)(var0,[](0)) */
    // this=var0        underAC=false        Instantiated=false
    setShared(v1);
    TERM_ALLOC(sv[1],term1,code_200);
    sv[1]->sub[0] = (setIntegerTag(0));
    sv[0] = fun_211( v1,sv[1] );
    if( sv[0] != con_1 ) {
      goto myend28;
    }
    /* rhs: @(var1,ccat(,)(minus(,)(var0,[](1)),var1)) */
    // this=var1        underAC=false        Instantiated=false
    setShared(v2);
    // this=var0        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_200);
    sv[2]->sub[0] = (setIntegerTag(1));
    sv[1] = fun_202( v1,sv[2] );
    // this=var1        underAC=false        Instantiated=false
    sv[2] = fun_224( sv[1],v2 );
    sv[1] = fun_220( v2,sv[2] );
    res = sv[1] ;
    goto end;
    myend28:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term2, 224);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_208(struct term *v1 ) {
  struct term *v2,*v3,*v4,*v5;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_208_%s_(",fsymtab[208].name);
    term_print(stdout,v1);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_200: /* [] */
    v4=v1->sub[0];
    switch(getInt(v4)) {
    default:
    label148:
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
    /* where var1 := umin_builtinInt()(var0) */
    // this=var0        underAC=false        Instantiated=false
    sv[1] = fun_20( v4 );
    tmp = sv[0] = sv[1];
    /* rhs: [](var1) */
    // this=var1        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_200);
    sv[2]->sub[0] = sv[0];
    res = sv[2] ;
    goto end;
    myend29:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term1, 208);
  res->sub[0] = v1;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_233(struct term *v1,struct term *v2,struct term *v3 ) {
  struct term *v4,*v5,*v6,*v7,*v8;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_233_%s_(",fsymtab[233].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(",");
    term_print(stdout,v3);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label151:
    switch(getSymb(v2)) {
    default:
    label152:
      switch(getInt(v3)) {
      default:
      label153:
        bitSet32_set(mask32,0);
      }
    }
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[4];
    struct term *substitution[1];
    /* lhs: set_of(,,)(var0,var1,var2) */
    /* allDetEvaluation: nonDet */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend30;
    }
    /* where var3 := (WHERE1:list[Foo]/Meta_apply[Foo]) meta_apply(,,,)(var0,.(var1,nil),0,var2) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term2,code_219);
    sv[2]->sub[0] = v2;
    sv[2]->sub[1] = con_218;
    // this=var2        underAC=false        Instantiated=false
    sv[3] = fun_236( v1,sv[2],(setIntegerTag(0)),v3 );
    sv[0] = str_465( sv[3] );
    /* rhs: var3 */
    // this=var3        underAC=false        Instantiated=false
    res = sv[0] ;
    CUTCLOSE(); /* Wheres */
    goto end;
    myend30:;
    restoreGlobalIndent();
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_ALLOC(res,term3, 233);
  res->sub[0] = v1;
  res->sub[1] = v2;
  res->sub[2] = v3;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_229(struct term *v1 ) {
  struct term *v2,*v3,*v4;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_229_%s_(",fsymtab[229].name);
    term_print(stdout,v1);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  default:
  label156:
    bitSet32_set(mask32,0);
  }
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[3];
    struct term *substitution[1];
    /* lhs: string()(var0) */
    /* allDetEvaluation: det */
    /* where var1 := valueOf()(var0) */
    // this=var0        underAC=false        Instantiated=false
    sv[1] = fun_217( v1 );
    tmp = sv[0] = sv[1];
    /* rhs: internstring()(var1) */
    // this=var1        underAC=false        Instantiated=false
    sv[2] = fun_158( sv[0] );
    res = sv[2] ;
    goto end;
    myend31:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term1, 229);
  res->sub[0] = v1;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* fun_206(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6,*v7,*v8;
  struct term *res;
  declareIndentLevel()
  match_state **ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_206_%s_(",fsymtab[206].name);
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
  case code_200: /* [] */
    v5=v1->sub[0];
    switch(getInt(v5)) {
    default:
    label160:
      switch(getSymb(v2)) {
      case code_200: /* [] */
        v7=v2->sub[0];
        switch(getInt(v7)) {
        default:
        label162:
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
    /* where var2 := div(,)(var0,var1) */
    // this=var0        underAC=false        Instantiated=false
    // this=var1        underAC=false        Instantiated=false
    sv[1] = fun_6( v5,v7 );
    tmp = sv[0] = sv[1];
    /* rhs: [](var2) */
    // this=var2        underAC=false        Instantiated=false
    TERM_ALLOC(sv[2],term1,code_200);
    sv[2]->sub[0] = sv[0];
    res = sv[2] ;
    goto end;
    myend32:;
    restoreGlobalIndent();
  }
match_fail:
  TERM_ALLOC(res,term2, 206);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_303( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  declareIndentLevel()
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("str_303(");
    term_print(stdout,v0);
    printf(")\n");
  }
  /* Meta */
  res=term_metaApply(v0);
end:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite(str_303)[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_465( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  declareIndentLevel()
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("str_465(");
    term_print(stdout,v0);
    printf(")\n");
  }
  /* Meta */
  res=term_metaApply(v0);
end:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite(str_465)[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_305( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  declareIndentLevel()
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("str_305(");
    term_print(stdout,v0);
    printf(")\n");
  }
  /* Meta */
  res=term_metaApply(v0);
end:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite(str_305)[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_464( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  declareIndentLevel()
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("str_464(");
    term_print(stdout,v0);
    printf(")\n");
  }
  /* Meta */
  res=term_metaApply(v0);
end:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite(str_464)[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_104( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  declareIndentLevel()
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("str_104(");
    term_print(stdout,v0);
    printf(")\n");
  }
  {
    /* dk[nonDet](M1:list[Foo]/new_meta!GL,M2:list[Foo]/new_meta!GL,M3:list[Foo]/new_meta!GL,M4:list[Foo]/new_meta!GL,M5:list[Foo]/new_meta!GL,M6:list[Foo]/new_meta!GL,M7:list[Foo]/new_meta!GL) */
    struct term *v1,*v2,*v3;
    bitSet_GC_create(mask,7);
    bitSet_init_clear(mask);
    switch(getSymb(v0)) {
    case code_244: /* dolist() */
      v2=v0->sub[0];
      switch(getSymb(v2)) {
      default:
      label165:
        bitSet_set(mask,0);
        bitSet_set(mask,1);
        bitSet_set(mask,2);
        bitSet_set(mask,3);
        bitSet_set(mask,4);
        bitSet_set(mask,5);
        bitSet_set(mask,6);
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      struct term *tmp, *sv[2];
      struct term *substitution[1];
      /* lhs: dolist()(var0) */
      /* allDetEvaluation: det */
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend33;
      }
      /* rhs: set_of(,)((call:Foo(491)),var0) */
      sv[1] = fun_144( (setIntegerTag(491)) );
      sv[0] = fun_141( sv[1] );
      // this=var0        underAC=false        Instantiated=false
      sv[1] = fun_232( sv[0],v2 );
      res = sv[1] ;
      rewrite_step++;
      goto stratLab8;
      myend33:;
      restoreGlobalIndent();
    }
    if(bitSet_get(mask,1)) {
      struct term *tmp, *sv[5];
      struct term *substitution[1];
      /* lhs: dolist()(var0) */
      /* allDetEvaluation: nonDet */
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend34;
      }
      /* where var1 := (WHERE3:list[Foo]/new_meta) meta_apply(,,,)((call:Foo(491)),var0,1,2) */
      sv[2] = fun_144( (setIntegerTag(491)) );
      sv[1] = fun_141( sv[2] );
      // this=var0        underAC=false        Instantiated=false
      sv[4] = fun_235( sv[1],v2,(setIntegerTag(1)),(setIntegerTag(2)) );
      sv[0] = str_303( sv[4] );
      /* rhs: var1 */
      // this=var1        underAC=false        Instantiated=false
      res = sv[0] ;
      rewrite_step++;
      goto stratLab8;
      myend34:;
      restoreGlobalIndent();
    }
    if(bitSet_get(mask,2)) {
      struct term *tmp, *sv[5];
      struct term *substitution[1];
      /* lhs: dolist()(var0) */
      /* allDetEvaluation: nonDet */
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend35;
      }
      /* where var1 := (WHERE4:list[Foo]/new_meta) meta_apply(,,,)((call:Foo(491)),var0,4,5) */
      sv[2] = fun_144( (setIntegerTag(491)) );
      sv[1] = fun_141( sv[2] );
      // this=var0        underAC=false        Instantiated=false
      sv[4] = fun_235( sv[1],v2,(setIntegerTag(4)),(setIntegerTag(5)) );
      sv[0] = str_304( sv[4] );
      /* rhs: var1 */
      // this=var1        underAC=false        Instantiated=false
      res = sv[0] ;
      rewrite_step++;
      goto stratLab8;
      myend35:;
      restoreGlobalIndent();
    }
    if(bitSet_get(mask,3)) {
      struct term *tmp, *sv[5];
      struct term *substitution[1];
      /* lhs: dolist()(var0) */
      /* allDetEvaluation: nonDet */
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend36;
      }
      /* where var1 := (WHERE5:list[Foo]/new_meta) meta_apply(,,,)((call:Foo(491)),var0,0,99) */
      sv[2] = fun_144( (setIntegerTag(491)) );
      sv[1] = fun_141( sv[2] );
      // this=var0        underAC=false        Instantiated=false
      sv[4] = fun_235( sv[1],v2,(setIntegerTag(0)),(setIntegerTag(99)) );
      sv[0] = str_305( sv[4] );
      /* rhs: var1 */
      // this=var1        underAC=false        Instantiated=false
      res = sv[0] ;
      rewrite_step++;
      goto stratLab8;
      myend36:;
      restoreGlobalIndent();
    }
    if(bitSet_get(mask,4)) {
      struct term *tmp, *sv[5];
      struct term *substitution[1];
      /* lhs: dolist()(var0) */
      /* allDetEvaluation: nonDet */
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend37;
      }
      /* where var1 := (WHERE6:list[Foo]/new_meta) meta_apply(,,,)((call:Foo(491)),var0,3,umin_builtinInt()(1)) */
      sv[2] = fun_144( (setIntegerTag(491)) );
      sv[1] = fun_141( sv[2] );
      // this=var0        underAC=false        Instantiated=false
      sv[4] = fun_20( (setIntegerTag(1)) );
      sv[3] = fun_235( sv[1],v2,(setIntegerTag(3)),sv[4] );
      sv[0] = str_306( sv[3] );
      /* rhs: var1 */
      // this=var1        underAC=false        Instantiated=false
      res = sv[0] ;
      rewrite_step++;
      goto stratLab8;
      myend37:;
      restoreGlobalIndent();
    }
    if(bitSet_get(mask,5)) {
      struct term *tmp, *sv[5];
      struct term *substitution[1];
      /* lhs: dolist()(var0) */
      /* allDetEvaluation: nonDet */
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend38;
      }
      /* where var1 := (WHERE7:list[Foo]/new_meta) meta_apply(,,,)((call:Foo(491)),var0,2,0) */
      sv[2] = fun_144( (setIntegerTag(491)) );
      sv[1] = fun_141( sv[2] );
      // this=var0        underAC=false        Instantiated=false
      sv[4] = fun_235( sv[1],v2,(setIntegerTag(2)),(setIntegerTag(0)) );
      sv[0] = str_307( sv[4] );
      /* rhs: var1 */
      // this=var1        underAC=false        Instantiated=false
      res = sv[0] ;
      rewrite_step++;
      goto stratLab8;
      myend38:;
      restoreGlobalIndent();
    }
    if(bitSet_get(mask,6)) {
      struct term *tmp, *sv[3];
      struct term *substitution[1];
      /* lhs: dolist()(var0) */
      /* allDetEvaluation: det */
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend39;
      }
      /* where var1 := set_of(,)((call:Foo(491)),var0) */
      sv[2] = fun_144( (setIntegerTag(491)) );
      sv[1] = fun_141( sv[2] );
      // this=var0        underAC=false        Instantiated=false
      sv[2] = fun_232( sv[1],v2 );
      tmp = sv[0] = sv[2];
      /* rhs: var1 */
      // this=var1        underAC=false        Instantiated=false
      res = sv[0] ;
      rewrite_step++;
      goto stratLab8;
      myend39:;
      restoreGlobalIndent();
    }
  }
  fail();
  stratLab8:;
  v0=res;
end:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite(str_104)[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_491( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  declareIndentLevel()
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("str_491(");
    term_print(stdout,v0);
    printf(")\n");
  }
  {
    /* dk[nonDet](R1:Foo/new_meta!GL,R2:Foo/new_meta!GL,R3:Foo/new_meta!GL) */
    struct term *v1,*v2;
    bitSet_GC_create(mask,6);
    bitSet_init_clear(mask);
    switch(getSymb(v0)) {
    case code_237: /* a */
      bitSet_set(mask,0);
      bitSet_set(mask,1);
      bitSet_set(mask,2);
      bitSet_set(mask,3);
      bitSet_set(mask,4);
      bitSet_set(mask,5);
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      struct term *tmp, *sv[1];
      struct term *substitution[1];
      /* lhs: a */
      /* allDetEvaluation: det */
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend40;
      }
      /* rhs: a */
      res = con_237 ;
      rewrite_step++;
      goto stratLab22;
      myend40:;
      restoreGlobalIndent();
    }
    if(bitSet_get(mask,1)) {
      struct term *tmp, *sv[1];
      struct term *substitution[1];
      /* lhs: a */
      /* allDetEvaluation: det */
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend41;
      }
      /* rhs: b */
      res = con_238 ;
      rewrite_step++;
      goto stratLab22;
      myend41:;
      restoreGlobalIndent();
    }
    if(bitSet_get(mask,2)) {
      struct term *tmp, *sv[1];
      struct term *substitution[1];
      /* lhs: a */
      /* allDetEvaluation: det */
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend42;
      }
      /* rhs: c */
      res = con_239 ;
      rewrite_step++;
      goto stratLab22;
      myend42:;
      restoreGlobalIndent();
    }
    if(bitSet_get(mask,3)) {
      struct term *tmp, *sv[1];
      struct term *substitution[1];
      /* lhs: a */
      /* allDetEvaluation: det */
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend43;
      }
      /* rhs: d */
      res = con_240 ;
      rewrite_step++;
      goto stratLab22;
      myend43:;
      restoreGlobalIndent();
    }
    if(bitSet_get(mask,4)) {
      struct term *tmp, *sv[1];
      struct term *substitution[1];
      /* lhs: a */
      /* allDetEvaluation: det */
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend44;
      }
      /* rhs: e */
      res = con_241 ;
      rewrite_step++;
      goto stratLab22;
      myend44:;
      restoreGlobalIndent();
    }
    if(bitSet_get(mask,5)) {
      struct term *tmp, *sv[1];
      struct term *substitution[1];
      /* lhs: a */
      /* allDetEvaluation: det */
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend45;
      }
      /* rhs: f */
      res = con_242 ;
      rewrite_step++;
      goto stratLab22;
      myend45:;
      restoreGlobalIndent();
    }
  }
  fail();
  stratLab22:;
  v0=res;
end:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite(str_491)[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_307( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  declareIndentLevel()
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("str_307(");
    term_print(stdout,v0);
    printf(")\n");
  }
  /* Meta */
  res=term_metaApply(v0);
end:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite(str_307)[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_449( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  declareIndentLevel()
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("str_449(");
    term_print(stdout,v0);
    printf(")\n");
  }
  {
    /* dk[nonDet](R:Foo/new_meta!GL) */
    struct term *v1,*v2,*v3;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(getSymb(v0)) {
    case code_243: /* do() */
      v2=v0->sub[0];
      switch(getSymb(v2)) {
      default:
      label170:
        bitSet_set(mask,0);
      }
      break;
    /* matching is not complete: jumpNode is null */
    }
    if(bitSet_get(mask,0)) {
      struct term *tmp, *sv[1];
      struct term *substitution[1];
      /* lhs: do()(var0) */
      /* allDetEvaluation: nonDet */
      if(setChoicePoint()) {
        /* local evaluations failed, try next rule */
        goto myend46;
      }
      /* where var1 := (RRR:Foo/new_meta) var0 */
      // this=var0        underAC=false        Instantiated=false
      sv[0] = str_491( v2 );
      /* rhs: var1 */
      // this=var1        underAC=false        Instantiated=false
      res = sv[0] ;
      rewrite_step++;
      goto stratLab15;
      myend46:;
      restoreGlobalIndent();
    }
  }
  fail();
  stratLab15:;
  v0=res;
end:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite(str_449)[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_492( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  declareIndentLevel()
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("str_492(");
    term_print(stdout,v0);
    printf(")\n");
  }
  /* cons[nonDet](iterate[multiDet](one[semiDet](extractrule2:Foo/list[Foo]!LO)),one[semiDet](extractrule1:Foo/list[Foo]!LO)) */
  /* iterate[multiDet](one[semiDet](extractrule2:Foo/list[Foo]!LO)) */
  while(1) {
    if(!setChoicePoint()) {
      break;
    }
    /* Apply the strategy */
    {
      /* one[semiDet](extractrule2:Foo/list[Foo]!LO) */
      struct term *v1,*v2,*v3,*v4,*v5;
      bitSet_GC_create(mask,1);
      bitSet_init_clear(mask);
      switch(getSymb(v0)) {
      case code_221: /* elem() */
        v2=v0->sub[0];
        switch(getSymb(v2)) {
        case code_219: /* . */
          v3=v2->sub[0];
          switch(getSymb(v3)) {
          default:
          label174:
            v4=v2->sub[1];
            switch(getSymb(v4)) {
            default:
            label175:
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
        /* lhs: elem()(.(var0,var1)) */
        /* allDetEvaluation: det */
        /* rhs: elem()(var1) */
        // this=var1        underAC=false        Instantiated=false
        TERM_ALLOC(sv[0],term1,code_221);
        sv[0]->sub[0] = v4;
        res = sv[0] ;
        rewrite_step++;
        goto stratLab24;
        myend47:;
        restoreGlobalIndent();
      }
    }
    fail();
    stratLab24:;
    v0=res;
  }
  {
    /* one[semiDet](extractrule1:Foo/list[Foo]!LO) */
    struct term *v1,*v2,*v3,*v4,*v5;
    bitSet_GC_create(mask,1);
    bitSet_init_clear(mask);
    switch(getSymb(v0)) {
    case code_221: /* elem() */
      v2=v0->sub[0];
      switch(getSymb(v2)) {
      case code_219: /* . */
        v3=v2->sub[0];
        switch(getSymb(v3)) {
        default:
        label179:
          v4=v2->sub[1];
          switch(getSymb(v4)) {
          default:
          label180:
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
      /* lhs: elem()(.(var0,var1)) */
      /* allDetEvaluation: det */
      /* rhs: var0 */
      // this=var0        underAC=false        Instantiated=false
      res = v3 ;
      rewrite_step++;
      goto stratLab27;
      myend48:;
      restoreGlobalIndent();
    }
  }
  fail();
  stratLab27:;
  v0=res;
end:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite(str_492)[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_306( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  declareIndentLevel()
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("str_306(");
    term_print(stdout,v0);
    printf(")\n");
  }
  /* Meta */
  res=term_metaApply(v0);
end:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite(str_306)[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_304( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  declareIndentLevel()
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("str_304(");
    term_print(stdout,v0);
    printf(")\n");
  }
  /* Meta */
  res=term_metaApply(v0);
end:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite(str_304)[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}

struct term* str_466( struct term *arg0 ) {
  struct term *v0=arg0;
  bitSet *mask;
  struct term *res=v0;
  declareIndentLevel()
  addindent();
  saveGlobalIndent();
  if(traceLevel>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("str_466(");
    term_print(stdout,v0);
    printf(")\n");
  }
  /* Meta */
  res=term_metaApply(v0);
end:
  if(traceLevel>=1) {
    doindent(indentlevel);
    printf("rewrite(str_466)[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  subindent();
  restoreGlobalIndent();
  return res;
  fail:
  // printf("fail\n");
  fail();
}
