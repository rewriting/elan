void variable_extract_298_repeat1_dk_249(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern, int base_id_pattern) {
  switch(id_pattern) {
            /* (++(var0,COMMIT,N(,)(var8,var1))) */
          case 0:
            break;
            /* (******(var6,var7,K()(var16),MN,N(,)(var8,var9),N(,)(var10,var11),A()(var12))) */
          case 1:
            break;
            /* (******(var6,var7,K()(var16),MNA,N(,)(var8,var9),N(,)(var10,var11),A()(var12))) */
          case 2:
            /* (******(var6,var7,K()(var16),MNNA,N(,)(var8,var9),N(,)(var10,var11),A()(var12))) */
          case 3:
            break;
            /* (******(var6,var7,K()(var8),var9,N(,)(var10,var11),N(,)(var12,var13),A()(var14))) */
          case 4:
            break;
          default:
            fprintf(stderr,"variable_extract_298_repeat1_dk_249: bad pattern number\n");
            exit(0);
          }
        }

void init_pattern_list_298_repeat1_dk_249() {
          int pattern_tab[max_nb_pattern_under_298_repeat1_dk_249];
          no_pattern_298_repeat1_dk_249_niv_0=0;
          pattern_list_298_repeat1_dk_249=MS_pattern_list_create(nb_pattern_298_repeat1_dk_249_niv_0);
          /* &(var14,(******(var5,var6,K()(var7),var8,N(,)(var9,var10),N(,)(var11,var12),A()(var13)))) */
          pattern_tab[0]=4;
          MS_pattern_list_init(pattern_list_298_repeat1_dk_249,no_pattern_298_repeat1_dk_249_niv_0++,1,pattern_tab);
          /* &(var9,(******(var3,var4,K()(var16),MNNA,N(,)(var17,var5),N(,)(var6,var7),A()(var8)))) */
          pattern_tab[0]=3;
          MS_pattern_list_init(pattern_list_298_repeat1_dk_249,no_pattern_298_repeat1_dk_249_niv_0++,1,pattern_tab);
          /* &(var10,(******(var3,var4,K()(var16),MNNA,N(,)(var5,var6),N(,)(var7,var8),A()(var9)))) */
          pattern_tab[0]=3;
          MS_pattern_list_init(pattern_list_298_repeat1_dk_249,no_pattern_298_repeat1_dk_249_niv_0++,1,pattern_tab);
          /* &(var10,(******(var3,var4,K()(var16),MNA,N(,)(var5,var6),N(,)(var7,var8),A()(var9)))) */
          pattern_tab[0]=2;
          MS_pattern_list_init(pattern_list_298_repeat1_dk_249,no_pattern_298_repeat1_dk_249_niv_0++,1,pattern_tab);
          /* &(var14,(******(var5,var6,K()(var7),var8,N(,)(var9,var10),N(,)(var11,var12),A()(var13)))) */
          pattern_tab[0]=4;
          MS_pattern_list_init(pattern_list_298_repeat1_dk_249,no_pattern_298_repeat1_dk_249_niv_0++,1,pattern_tab);
          /* &(var15,(******(var6,var7,K()(var8),var9,N(,)(var10,var11),N(,)(var12,var13),A()(var14)))) */
          pattern_tab[0]=4;
          MS_pattern_list_init(pattern_list_298_repeat1_dk_249,no_pattern_298_repeat1_dk_249_niv_0++,1,pattern_tab);
        }


        if( bitSet_get(mask,2) || bitSet_get(mask,3) || bitSet_get(mask,4) || bitSet_get(mask,5) || bitSet_get(mask,12) || bitSet_get(mask,13) || bitSet_get(mask,14) || bitSet_get(mask,15) || bitSet_get(mask,16) ) {
          indice = MS_init(&(ms[0]), match_subterm_298_repeat1_dk_249, no_pattern_298_repeat1_dk_249_niv_0, pattern_list_298_repeat1_dk_249, nb_pattern_298_repeat1_dk_249_niv_1,v9, necessary_link, max_nb_pattern_under_298_repeat1_dk_249);
        }
        if( bitSet_get(mask,6) ) {
          indice = MS_init(&(ms[1]), match_subterm_298_repeat1_dk_249, no_pattern_298_repeat1_dk_249_niv_0, pattern_list_298_repeat1_dk_249, nb_pattern_298_repeat1_dk_249_niv_1,v8, necessary_link, max_nb_pattern_under_298_repeat1_dk_249);
        }
        if( bitSet_get(mask,1) ) {
          indice = MS_init(&(ms[2]), match_subterm_298_repeat1_dk_249, no_pattern_298_repeat1_dk_249_niv_0, pattern_list_298_repeat1_dk_249, nb_pattern_298_repeat1_dk_249_niv_1,v3, necessary_link, max_nb_pattern_under_298_repeat1_dk_249);
        }
        if( bitSet_get(mask,0) ) {
          indice = MS_init(&(ms[3]), match_subterm_298_repeat1_dk_249, no_pattern_298_repeat1_dk_249_niv_0, pattern_list_298_repeat1_dk_249, nb_pattern_298_repeat1_dk_249_niv_1,v2, necessary_link, max_nb_pattern_under_298_repeat1_dk_249);
        }


        if(bitSet_get(mask,0)) {
          if(setChoicePoint()) {
            /* AC matching failed, try next rule */
            goto myend54;
          }
          if(ACPattern && MS_reinit(ms[3],v2,0)>0) {
            while(1) {
              if(MS_solve_rule(ms[3])<0) {
                fail();
              }
              /* choicePoint AC matching */
              if(!setChoicePoint()) {
                break;
              }
            }
          } else {
            fail();
          }
/* lhs: <><><><>(||(var2,(++(var0,COMMIT,N(,)(var8,var1)))),var3,var4,var5,var6) */

          if(bitSet_get(mask,1)) {
          if(setChoicePoint()) {
            /* AC matching failed, try next rule */
            goto myend55;
          }
          if(ACPattern && MS_reinit(ms[2],v3,0)>0) {
            while(1) {
              if(MS_solve_rule(ms[2])<0) {
                fail();
              }
              /* choicePoint AC matching */
              if(!setChoicePoint()) {
                break;
              }
            }
          } else {
            fail();
          }
/* lhs: <><><><>(var0,||(var3,(++(var1,COMMIT,N(,)(var8,var2)))),var4,var5,var6) */
          
        if(bitSet_get(mask,2)) {
          if(setChoicePoint()) {
            /* AC matching failed, try next rule */
            goto myend56;
          }
          if(ACPattern && MS_reinit(ms[0],v9,2)>0) {
            while(1) {
              if(MS_solve_rule(ms[0])<0) {
                fail();
              }
              /* choicePoint AC matching */
              if(!setChoicePoint()) {
                break;
              }
            }
          } else {
            fail();
          }
/* lhs: <><><><>(var0,var1,###(var2,var3,var4,var5),&(var13,(******(var6,var7,K()(var16),MN,N(,)(var8,var9),N(,)(var10,var11),A()(var12)))),var14) */

          
        if(bitSet_get(mask,3)) {
          if(setChoicePoint()) {
            /* AC matching failed, try next rule */
            goto myend57;
          }
          if(ACPattern && MS_reinit(ms[0],v9,3)>0) {
            while(1) {
              if(MS_solve_rule(ms[0])<0) {
                fail();
              }
              /* choicePoint AC matching */
              if(!setChoicePoint()) {
                break;
              }
            }
          } else {
            fail();
          }
/* lhs: <><><><>(var0,var1,###(var2,var3,var4,var5),&(var13,(******(var6,var7,K()(var16),MNA,N(,)(var8,var9),N(,)(var10,var11),A()(var12)))),var14) */

          if(bitSet_get(mask,4)) {
          if(setChoicePoint()) {
            /* AC matching failed, try next rule */
            goto myend58;
          }
          if(ACPattern && MS_reinit(ms[0],v9,4)>0) {
            while(1) {
              if(MS_solve_rule(ms[0])<0) {
                fail();
              }
              /* choicePoint AC matching */
              if(!setChoicePoint()) {
                break;
              }
            }
          } else {
            fail();
          }
/* lhs: <><><><>(var0,var1,###(var2,var3,var4,var5),&(var13,(******(var6,var7,K()(var16),MNNA,N(,)(var8,var9),N(,)(var10,var11),A()(var12)))),var14) */

          
        if(bitSet_get(mask,5)) {
          if(setChoicePoint()) {
            /* AC matching failed, try next rule */
            goto myend59;
          }
          if(ACPattern && MS_reinit(ms[0],v9,5)>0) {
            while(1) {
              if(MS_solve_rule(ms[0])<0) {
                fail();
              }
              /* choicePoint AC matching */
              if(!setChoicePoint()) {
                break;
              }
            }
          } else {
            fail();
          }
/* lhs: <><><><>(var0,var1,###(var2,var3,var4,var5),&(var15,(******(var6,var7,K()(var8),var9,N(,)(var10,var11),N(,)(var12,var13),A()(var14)))),var16) */

          
        if(bitSet_get(mask,6)) {
          if(setChoicePoint()) {
            /* AC matching failed, try next rule */
            goto myend60;
          }
          if(ACPattern && MS_reinit(ms[1],v8,5)>0) {
            while(1) {
              if(MS_solve_rule(ms[1])<0) {
                fail();
              }
              /* choicePoint AC matching */
              if(!setChoicePoint()) {
                break;
              }
            }
          } else {
            fail();
          }
/* lhs: <><><><>(var0,var1,###(var2,var3,var4,&(var14,(******(var5,var6,K()(var7),var8,N(,)(var9,var10),N(,)(var11,var12),A()(var13))))),var15,var16) */

          
        if(bitSet_get(mask,7)) {
          struct term *tmp, *sv[14];
          struct term *substitution[1];
/* lhs: <><><><>(var0,var1,###(var2,var3,var4,var5),var6,var7) */

          if(bitSet_get(mask,8)) {
          struct term *tmp, *sv[14];
          struct term *substitution[1];
/* lhs: <><><><>(var0,var1,###(var2,var3,var4,var5),var6,var7) */

          if(bitSet_get(mask,9)) {
          struct term *tmp, *sv[16];
          struct term *substitution[1];
/* lhs: <><><><>(var0,var1,###(var2,var3,var4,var5),var6,var7) */

          if(bitSet_get(mask,10)) {
          struct term *tmp, *sv[19];
          struct term *substitution[1];
/* lhs: <><><><>(var0,var1,var2,var3,var4) */

          if(bitSet_get(mask,11)) {
          struct term *tmp, *sv[15];
          struct term *substitution[1];
/* lhs: <><><><>(var0,var1,var2,var3,var4) */

          if(bitSet_get(mask,12)) {
          if(setChoicePoint()) {
            /* AC matching failed, try next rule */
            goto myend66;
          }
          if(ACPattern && MS_reinit(ms[0],v9,4)>0) {
            while(1) {
              if(MS_solve_rule(ms[0])<0) {
                fail();
              }
              /* choicePoint AC matching */
              if(!setChoicePoint()) {
                break;
              }
            }
          } else {
            fail();
          }
/* lhs: <><><><>(var0,var1,var2,&(var9,(******(var3,var4,K()(var16),MNNA,N(,)(var17,var5),N(,)(var6,var7),A()(var8)))),var10) */

          
        if(bitSet_get(mask,13)) {
          if(setChoicePoint()) {
            /* AC matching failed, try next rule */
            goto myend67;
          }
          if(ACPattern && MS_reinit(ms[0],v9,4)>0) {
            while(1) {
              if(MS_solve_rule(ms[0])<0) {
                fail();
              }
              /* choicePoint AC matching */
              if(!setChoicePoint()) {
                break;
              }
            }
          } else {
            fail();
          }
/* lhs: <><><><>(var0,var1,var2,&(var10,(******(var3,var4,K()(var16),MNNA,N(,)(var5,var6),N(,)(var7,var8),A()(var9)))),var11) */


          if(bitSet_get(mask,14)) {
          if(setChoicePoint()) {
            /* AC matching failed, try next rule */
            goto myend68;
          }
          if(ACPattern && MS_reinit(ms[0],v9,3)>0) {
            while(1) {
              if(MS_solve_rule(ms[0])<0) {
                fail();
              }
              /* choicePoint AC matching */
              if(!setChoicePoint()) {
                break;
              }
            }
          } else {
            fail();
          }
/* lhs: <><><><>(var0,var1,var2,&(var10,(******(var3,var4,K()(var16),MNA,N(,)(var5,var6),N(,)(var7,var8),A()(var9)))),var11) */


          if(bitSet_get(mask,15)) {
          if(setChoicePoint()) {
            /* AC matching failed, try next rule */
            goto myend69;
          }
          if(ACPattern && MS_reinit(ms[0],v9,2)>0) {
            while(1) {
              if(MS_solve_rule(ms[0])<0) {
                fail();
              }
              /* choicePoint AC matching */
              if(!setChoicePoint()) {
                break;
              }
            }
          } else {
            fail();
          }
/* lhs: <><><><>(var0,var1,var2,&(var9,(******(var3,var4,K()(var16),MN,N(,)(var17,var5),N(,)(var6,var7),A()(var8)))),var10) */


          if(bitSet_get(mask,16)) {
          if(setChoicePoint()) {
            /* AC matching failed, try next rule */
            goto myend70;
          }
          if(ACPattern && MS_reinit(ms[0],v9,2)>0) {
            while(1) {
              if(MS_solve_rule(ms[0])<0) {
                fail();
              }
              /* choicePoint AC matching */
              if(!setChoicePoint()) {
                break;
              }
            }
          } else {
            fail();
          }
/* lhs: <><><><>(var0,var1,var2,&(var10,(******(var3,var4,K()(var16),MN,N(,)(var5,var6),N(,)(var7,var8),A()(var9)))),var11) */
