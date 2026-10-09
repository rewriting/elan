        if(bitSet_get(mask,7)) {
          struct term *tmp, *sv[30];
          struct term *substitution[1];
          /* lhs: <><><><><>(var0,var1,var2,var3,(var4),var5) */
          /* allDetEvaluation: nonDet */
          if(setChoicePoint()) {
            /* local evaluations failed, try next rule */
            goto myend257;
          }
          /* where var9 := (listExtract:message/list[message]) elem()(var4) */
          setShared(v12);
          TERM_ALLOC(sv[1],term1,code_259);
          sv[1]->sub[0] = v12;
          sv[0] = str_390( sv[1] );
          /* where *****(var10,var6,JK(,)(var17,var7),var18,JK(,)(var11,var12),RESP) := var9 */
          tmp = sv[12] = sv[0];
          if(code_352 != getSymb(sv[12])) {
            fail();
          } else {
            sv[11] = sv[12]->sub[0];
            sv[10] = sv[12]->sub[1];
            sv[9] = sv[12]->sub[2];
            if(code_345 != getSymb(sv[9])) {
              fail();
            } else {
              sv[8] = sv[9]->sub[0];
              sv[7] = sv[9]->sub[1];
            }
            sv[6] = sv[12]->sub[3];
            sv[5] = sv[12]->sub[4];
            if(code_345 != getSymb(sv[5])) {
              fail();
            } else {
              sv[4] = sv[5]->sub[0];
              sv[3] = sv[5]->sub[1];
            }
            sv[2] = sv[12]->sub[5];
            if(code_340 != getSymb(sv[2])) {
              fail();
            }
          }
          /* if eq_SWC(,)(var6,var17) */
          setShared(sv[10]);
          sv[13] = fun_18( sv[10],sv[8] );
          if( sv[13] != con_1 ) {
            fail();
          }
          /* if eq_SWC(,)(var7,var18) */
          setShared(sv[7]);
          sv[14] = fun_18( sv[7],sv[6] );
          if( sv[14] != con_1 ) {
            fail();
          }
          /* where var13 := (listExtract:node/list[node]) elem()(var0) */
          setShared(v2);
          term_alloc(&sv[16],sizeof(struct term1),code_221);
          sv[16]->sub[0] = v2;
          sv[15] = str_252( sv[16] );

            /* where ++(var12,WAIT,var14) := var13 */
          tmp = sv[20] = sv[15];
          if(code_348 != getSymb(sv[20])) {
            fail();
          } else {
            sv[19] = sv[20]->sub[0];
            sv[18] = sv[20]->sub[1];
            if(code_343 != getSymb(sv[18])) {
              fail();
            }
            sv[17] = sv[20]->sub[2];
          }

            /* if eq_SWC(,)(var6,var12) */
          sv[21] = fun_18( sv[10],sv[3] );
          if( sv[21] != con_1 ) {
            fail();
          }

            /* where var8 := elim(,)(++(var6,WAIT,var14),var0) */
          term_alloc(&sv[24],sizeof(struct term3),code_348);
          sv[24]->sub[0] = sv[10];
          sv[24]->sub[1] = con_343;
          sv[24]->sub[2] = sv[17];
          sv[23] = fun_232( sv[24],v2 );
          tmp = sv[22] = sv[23];
          /* where var15 := elim(,)(*****(s,var6,JK(,)(var6,var7),var7,JK(,)(var7,var6),RESP),var4) */
          term_alloc(&sv[26],sizeof(struct term2),code_345);
          sv[26]->sub[0] = sv[10];
          sv[26]->sub[1] = sv[7];
          term_alloc(&sv[27],sizeof(struct term2),code_345);
          sv[27]->sub[0] = sv[7];
          sv[27]->sub[1] = sv[10];
          term_alloc(&sv[29],sizeof(struct term6),code_352);
          sv[29]->sub[0] = con_337;
          sv[29]->sub[1] = sv[10];
          sv[29]->sub[2] = sv[26];
          sv[29]->sub[3] = sv[7];
          sv[29]->sub[4] = sv[27];
          sv[29]->sub[5] = con_340;
          sv[25] = fun_270( sv[29],v12 );
          tmp = sv[24] = sv[25];
          /* rhs: <><><><><>(.(++(var6,COMMIT,JK(,)(var6,var7)),var8),var1,var2,var3,(var4),plus(,)(var5,[](1))) */
          term_alloc(&sv[27],sizeof(struct term2),code_345);
          sv[27]->sub[0] = sv[10];
          sv[27]->sub[1] = sv[7];
          term_alloc(&sv[28],sizeof(struct term3),code_348);
          sv[28]->sub[0] = sv[10];
          sv[28]->sub[1] = con_344;
          sv[28]->sub[2] = sv[27];
          term_alloc(&sv[26],sizeof(struct term2),code_219);
          sv[26]->sub[0] = sv[28];
          sv[26]->sub[1] = sv[22];
          term_alloc(&sv[27],sizeof(struct term1),code_354);
          sv[27]->sub[0] = v12;
          term_alloc(&sv[29],sizeof(struct term1),code_200);
          sv[29]->sub[0] = (setIntegerTag(1));
          sv[28] = fun_201( v13,sv[29] );
          term_alloc(&sv[29],sizeof(struct term6),code_351);
          sv[29]->sub[0] = sv[26];
          sv[29]->sub[1] = v3;
          sv[29]->sub[2] = v4;
          sv[29]->sub[3] = v9;
          sv[29]->sub[4] = sv[27];
          sv[29]->sub[5] = sv[28];
          res = sv[29] ;
          rewrite_step++;
          goto stratLab41;
          myend257:;
          restoreGlobalIndent();
        }
      }
      fail();
      stratLab41:;
      v0=res;
      if(*wasr==0) {
        *wasr=1;
      }
    }
  }
end:
  if(trace>=1) {
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
