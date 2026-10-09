Lemma elan_641:(A_Plus(A_Opp(A_Plus(A_Opp(A_Plus y3 y2))(A_Opp y1)))(A_Opp(A_Plus(A_Opp A_Zero)(A_Opp y3))))=(A_Plus(A_Opp(A_Plus(A_Opp A_Zero)(A_Opp y3)))(A_Opp(A_Plus(A_Opp y1)(A_Opp(A_Plus y2 y3))))).
 Proof.
Time Reflexivity Orelse Erp ac_poly li;Reflexivity.
 Show Size.
Qed.
Lemma elan_642:(z1:A)(z2:A)z1=z2->(A_Plus z1(A_Opp(A_Plus(A_Opp y1)(A_Opp(A_Plus y2 y3)))))=(A_Plus z2(A_Opp(A_Plus(A_Opp y1)(A_Opp(A_Plus y2 y3))))).
 Proof.
Intros;Rewrite H;Reflexivity.
Qed.
Lemma elan_643:(z1:A)(z2:A)z1=z2->(A_Opp z1)=(A_Opp z2).
 Proof.
Intros;Rewrite H;Reflexivity.
Qed.
Lemma elan_644:(z1:A)(z2:A)z1=z2->(A_Plus z1(A_Opp y3))=(A_Plus z2(A_Opp y3)).
 Proof.
Intros;Rewrite H;Reflexivity.
Qed.
ErpLemma ac_poly li elan_16350_13560:(A_Plus A_Zero(A_Opp y3))=(A_Plus(A_Opp y3)A_Zero).
Lemma elan_645:(z1:A)(z2:A)z1=z2->(A_Plus y3(A_Opp(A_Plus(A_Opp y1)(A_Opp z1))))=(A_Plus y3(A_Opp(A_Plus(A_Opp y1)(A_Opp z2)))).
 Proof.
Intros;Rewrite H;Reflexivity.
Qed.
ErpLemma ac_poly li elan_1450_1540:(A_Plus y2 y3)=(A_Plus y3 y2).
Lemma elan_646:(z1:A)(z2:A)z1=z2->(A_Plus y3(A_Opp(A_Plus(A_Opp y1)z1)))=(A_Plus y3(A_Opp(A_Plus(A_Opp y1)z2))).
 Proof.
Intros;Rewrite H;Reflexivity.
Qed.
Lemma elan_647:(z1:A)(z2:A)z1=z2->(A_Plus y3(A_Opp z1))=(A_Plus y3(A_Opp z2)).
 Proof.
Intros;Rewrite H;Reflexivity.
Qed.
ErpLemma ac_poly li elan_133135340_135133340:(A_Plus(A_Opp y1)(A_Plus(A_Opp y3)(A_Opp y2)))=(A_Plus(A_Opp y3)(A_Plus(A_Opp y1)(A_Opp y2))).
Lemma elan_648:(z1:A)(z2:A)z1=z2->(A_Plus y3 z1)=(A_Plus y3 z2).
 Proof.
Intros;Rewrite H;Reflexivity.
Qed.
Lemma elan_649:(z1:A)(z2:A)z1=z2->(A_Plus z1(A_Opp(A_Plus(A_Opp y1)(A_Opp y2))))=(A_Plus z2(A_Opp(A_Plus(A_Opp y1)(A_Opp y2)))).
 Proof.
Intros;Rewrite H;Reflexivity.
Qed.
Lemma elan_650:(z1:A)(z2:A)z1=z2->(A_Plus y3(A_Opp z1))=(A_Plus y3(A_Opp z2)).
 Proof.
Intros;Rewrite H;Reflexivity.
Qed.
ErpLemma ac_poly li elan_133340_134330:(A_Plus(A_Opp y1)(A_Opp y2))=(A_Plus(A_Opp y2)(A_Opp y1)).
Lemma elan_651:(z1:A)(z2:A)z1=z2->(A_Plus y3 z1)=(A_Plus y3 z2).
 Proof.
Intros;Rewrite H;Reflexivity.
Qed.
ErpLemma ac_poly li elan_13343330_13333340:(A_Plus(A_Opp(A_Opp y2))(A_Opp(A_Opp y1)))=(A_Plus(A_Opp(A_Opp y1))(A_Opp(A_Opp y2))).
Lemma elan_652:(z1:A)(z2:A)z1=z2->(A_Plus z1(A_Opp(A_Opp y2)))=(A_Plus z2(A_Opp(A_Opp y2))).
 Proof.
Intros;Rewrite H;Reflexivity.
Qed.
Lemma elan_653:(z1:A)(z2:A)z1=z2->(A_Plus y1 z1)=(A_Plus y1 z2).
 Proof.
Intros;Rewrite H;Reflexivity.
Qed.
Lemma elan_654:(A_Plus(A_Opp(A_Plus(A_Opp(A_Plus y3 y2))(A_Opp y1)))(A_Opp(A_Plus(A_Opp A_Zero)(A_Opp y3))))=(A_Plus y3(A_Plus y3(A_Plus y1 y2))).
 Proof.
 Rewrite elan_641.
 Time Exact (trans_equal A(A_Plus(A_Opp(A_Plus(A_Opp A_Zero)(A_Opp y3)))(A_Opp(A_Plus(A_Opp y1)(A_Opp(A_Plus y2 y3)))))(A_Plus y3(A_Opp(A_Plus(A_Opp y1)(A_Opp(A_Plus y2 y3)))))(A_Plus y3(A_Plus y3(A_Plus y1 y2)))(elan_642(A_Opp(A_Plus(A_Opp A_Zero)(A_Opp y3)))y3(trans_equal A(A_Opp(A_Plus(A_Opp A_Zero)(A_Opp y3)))(A_Opp(A_Opp y3))y3(elan_643(A_Plus(A_Opp A_Zero)(A_Opp y3))(A_Opp y3)(trans_equal A(A_Plus(A_Opp A_Zero)(A_Opp y3))(A_Plus A_Zero(A_Opp y3))(A_Opp y3)(elan_644(A_Opp A_Zero)A_Zero A_Opp_Null)(trans_equal A(A_Plus A_Zero(A_Opp y3))(A_Plus(A_Opp y3)A_Zero)(A_Opp y3)elan_16350_13560(A_neutral(A_Opp y3)))))(A_Opp_Opp y3)))(trans_equal A(A_Plus y3(A_Opp(A_Plus(A_Opp y1)(A_Opp(A_Plus y2 y3)))))(A_Plus y3(A_Opp(A_Plus(A_Opp y1)(A_Opp(A_Plus y3 y2)))))(A_Plus y3(A_Plus y3(A_Plus y1 y2)))(elan_645(A_Plus y2 y3)(A_Plus y3 y2)elan_1450_1540)(trans_equal A(A_Plus y3(A_Opp(A_Plus(A_Opp y1)(A_Opp(A_Plus y3 y2)))))(A_Plus y3(A_Opp(A_Plus(A_Opp y1)(A_Plus(A_Opp y3)(A_Opp y2)))))(A_Plus y3(A_Plus y3(A_Plus y1 y2)))(elan_646(A_Opp(A_Plus y3 y2))(A_Plus(A_Opp y3)(A_Opp y2))(A_Opp_Plus y3 y2))(trans_equal A(A_Plus y3(A_Opp(A_Plus(A_Opp y1)(A_Plus(A_Opp y3)(A_Opp y2)))))(A_Plus y3(A_Opp(A_Plus(A_Opp y3)(A_Plus(A_Opp y1)(A_Opp y2)))))(A_Plus y3(A_Plus y3(A_Plus y1 y2)))(elan_647(A_Plus(A_Opp y1)(A_Plus(A_Opp y3)(A_Opp y2)))(A_Plus(A_Opp y3)(A_Plus(A_Opp y1)(A_Opp y2)))elan_133135340_135133340)(elan_648(A_Opp(A_Plus(A_Opp y3)(A_Plus(A_Opp y1)(A_Opp y2))))(A_Plus y3(A_Plus y1 y2))(trans_equal A(A_Opp(A_Plus(A_Opp y3)(A_Plus(A_Opp y1)(A_Opp y2))))(A_Plus(A_Opp(A_Opp y3))(A_Opp(A_Plus(A_Opp y1)(A_Opp y2))))(A_Plus y3(A_Plus y1 y2))(A_Opp_Plus(A_Opp y3)(A_Plus(A_Opp y1)(A_Opp y2)))(trans_equal A(A_Plus(A_Opp(A_Opp y3))(A_Opp(A_Plus(A_Opp y1)(A_Opp y2))))(A_Plus y3(A_Opp(A_Plus(A_Opp y1)(A_Opp y2))))(A_Plus y3(A_Plus y1 y2))(elan_649(A_Opp(A_Opp y3))y3(A_Opp_Opp y3))(trans_equal A(A_Plus y3(A_Opp(A_Plus(A_Opp y1)(A_Opp y2))))(A_Plus y3(A_Opp(A_Plus(A_Opp y2)(A_Opp y1))))(A_Plus y3(A_Plus y1 y2))(elan_650(A_Plus(A_Opp y1)(A_Opp y2))(A_Plus(A_Opp y2)(A_Opp y1))elan_133340_134330)(elan_651(A_Opp(A_Plus(A_Opp y2)(A_Opp y1)))(A_Plus y1 y2)(trans_equal A(A_Opp(A_Plus(A_Opp y2)(A_Opp y1)))(A_Plus(A_Opp(A_Opp y2))(A_Opp(A_Opp y1)))(A_Plus y1 y2)(A_Opp_Plus(A_Opp y2)(A_Opp y1))(trans_equal A(A_Plus(A_Opp(A_Opp y2))(A_Opp(A_Opp y1)))(A_Plus(A_Opp(A_Opp y1))(A_Opp(A_Opp y2)))(A_Plus y1 y2)elan_13343330_13333340(trans_equal A(A_Plus(A_Opp(A_Opp y1))(A_Opp(A_Opp y2)))(A_Plus y1(A_Opp(A_Opp y2)))(A_Plus y1 y2)(elan_652(A_Opp(A_Opp y1))y1(A_Opp_Opp y1))(elan_653(A_Opp(A_Opp y2))y2(A_Opp_Opp y2)))))))))))))).
 Show Size. 
 Qed.
Lemma elan_655:(A_Plus y3(A_Plus y3(A_Plus y1 y2)))=(A_Plus(A_Plus(A_Plus y1 y2)y3)y3).
 Proof.
Time Reflexivity Orelse Erp ac_poly li;Reflexivity.
 Show Size.
Qed.
Lemma main_theorem:(A_Plus(A_Opp(A_Plus(A_Opp(A_Plus y3 y2))(A_Opp y1)))(A_Opp(A_Plus(A_Opp A_Zero)(A_Opp y3))))=(A_Plus(A_Plus(A_Plus y1 y2)y3)y3).
 Proof.
 Rewrite elan_654.
 Apply elan_655.
 Qed.
