specification propositional

    Props A:0 B:0 C:0 D:0 P:0 Q:0 E:0

end of specification

// True
// Hvide |- (A |=> (B |=> C)) |=> (A |=> B) |=> A |=> C 	end

// True
// (A or B) && C && (D && P) |-  (A or B) or ^(Q) or (A && B)	end

// False
//// (A or B) && C && (D && P) |-  ^(Q)or (A && B)		end

//// A or B |- A && B						end

// True
// (A |=> (B |=> C)) && (A && B) && ^(C) |- FALSE		end

// True
// (A |=> (B |=> C)) && ^((A && B) |=> C) |- FALSE		end

// False
//// (A |=> B) && (C && D) && P |-  (A or B) or ^(Q)		end

// False
//// (A |=> B) && (C && D) && P |-  (A or B) or ^(Q)or (A && B)	end

// False
//// (A |=> B) && (^(B) |=> ^(C)) |- FALSE			end

// True
//A && B |- A or B						end

// True
// C or (D or A) |- (C or D) or A				end

// True
// (P |=> Q) |- ^(Q) |=> ^(P)					end

// True
// P |=> Q |- ^(Q) |=> ^(P)					end

// True
// (^(B) |=> ^(A)) |- (C |=> A) |=> (C |=> B)			end
