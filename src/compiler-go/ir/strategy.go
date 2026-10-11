package ir

import "strings"

// Analyses of the strategies (elanc-rs's ir.rs, S6b): determinism (at most
// one result), totality (at least one result when they terminate) and tail
// calls. They decide which strategies the emission turns into loops
// (constant stack, spec G4); a loop is only used where it gives the same
// results, in the same order, as the recursive version.

// StrategyDet tells whether s gives at most one result, given the
// determinism of the rules of each label, their number and the determinism
// of the named strategies (used by the lowering during its fixpoint and by
// [IR.Det]):
//
//   - id, fail, one(...), ONE(...): yes;
//   - dc(l1, ..., ln): all labels deterministic (dc keeps every result of
//     the first rule with one; a deterministic rule has at most one);
//   - dk(l1, ..., ln): deterministic rules, one rule at most in all;
//   - DC(a1, ..., an): every alternative deterministic; DK: one alternative
//     at most, deterministic;
//   - repeat*(s): s deterministic; iterate*(s): s contains fail (iterate*
//     gives the term itself and every result after it);
//   - a call: the called strategy;
//   - META: no (meta_apply(s, t) gives every result of s).
func StrategyDet(s *Strat, rulesDet func(LabelID) bool, nRules func(LabelID) int, stratDet func(StratID) bool) bool {
	seq := func(v []Strat) bool {
		for i := range v {
			if !StrategyDet(&v[i], rulesDet, nRules, stratDet) {
				return false
			}
		}
		return true
	}
	switch s.Kind {
	case StId, StFail:
		return true
	case StRules:
		switch s.Choice {
		case One:
			return true
		case Dc:
			for _, l := range s.Labels {
				if !rulesDet(l) {
					return false
				}
			}
			return true
		}
		n := 0
		for _, l := range s.Labels {
			if !rulesDet(l) {
				return false
			}
			n += nRules(l)
		}
		return n <= 1
	case StStrats:
		switch s.Choice {
		case One:
			return true
		case Dc:
			for _, a := range s.Alts {
				if !seq(a) {
					return false
				}
			}
			return true
		}
		return len(s.Alts) <= 1 && (len(s.Alts) == 0 || seq(s.Alts[0]))
	case StRepeat:
		return seq(s.Body)
	case StIterate:
		for _, b := range s.Body {
			if b.Kind == StFail {
				return true
			}
		}
		return false
	case StCall:
		return stratDet(s.Call)
	}
	return false
}

// Det tells whether s gives at most one result ([StrategyDet]).
func (ir *IR) Det(s *Strat) bool {
	return StrategyDet(s,
		func(l LabelID) bool { return ir.Labels[l].RulesDet },
		func(l LabelID) int { return len(ir.Labels[l].Rules) },
		func(c StratID) bool { return ir.Strategies[c].Det })
}

// SeqDet tells whether the sequence gives at most one result: all its
// elements do.
func (ir *IR) SeqDet(seq []Strat) bool {
	for i := range seq {
		if !ir.Det(&seq[i]) {
			return false
		}
	}
	return true
}

// TotalStrategies tells, per named strategy, whether it gives at least
// one result whenever it terminates: id, repeat*, iterate*, a choice with
// such an alternative, a call of such a strategy, sequences of these
// (least fixpoint over the calls).
func (ir *IR) TotalStrategies() []bool {
	tot := make([]bool, len(ir.Strategies))
	var total func(s *Strat) bool
	seq := func(v []Strat) bool {
		for i := range v {
			if !total(&v[i]) {
				return false
			}
		}
		return true
	}
	total = func(s *Strat) bool {
		switch s.Kind {
		case StId, StRepeat, StIterate:
			return true
		case StStrats:
			for _, a := range s.Alts {
				if seq(a) {
					return true
				}
			}
			return false
		case StCall:
			return tot[s.Call]
		}
		return false
	}
	for changed := true; changed; {
		changed = false
		for i := range ir.Strategies {
			if !tot[i] && seq(ir.Strategies[i].Body) {
				tot[i], changed = true, true
			}
		}
	}
	return tot
}

// TailCalls are the calls of strategy me to itself in tail position, when
// me is deterministic: its function can then be a loop (spec G4) where
// such a call on r replaces the term by r and starts again, the stack
// staying flat (`s => first(step ; s, id)` run 10^6 times). A call is in
// tail position when the result of the body is then the result of the
// call: it ends the body, or the last sequence of a DC/ONE alternative (or
// of a DK of one alternative) in tail position, what comes before it in
// its sequence giving at most one result; in an alternative that is not
// the last one, only if me always has a result (total,
// [IR.TotalStrategies]): else its failure would let the next alternatives
// run. The nodes are returned by address (the emission recognises them
// while it walks the same body).
//
// The loop gives the same results in the same order as the interpreter,
// but it may terminate where the interpreter does not: its one.K stops the
// search after the single result, so the alternatives that would run after
// it (and could loop for ever without a result) are never started. With
// `me => first(dc(dec) ; me, iterate*(spin ; fail))` and
// `spin => dc(inc) ; spin` on 3, elanc-go and elanc-rs print 0 and exit;
// `elan -b` prints 0, then searches the iterate* of the outer calls for
// ever (docs/followups.md).
func (ir *IR) TailCalls(me StratID, total []bool) map[*Strat]bool {
	out := map[*Strat]bool{}
	if ir.Strategies[me].Det {
		ir.tailSeq(me, total[me], ir.Strategies[me].Body, out)
	}
	return out
}

func (ir *IR) tailSeq(me StratID, meTotal bool, seq []Strat, out map[*Strat]bool) {
	if len(seq) == 0 || !ir.SeqDet(seq[:len(seq)-1]) {
		return
	}
	last := &seq[len(seq)-1]
	switch {
	case last.Kind == StCall && last.Call == me:
		out[last] = true
	case last.Kind == StStrats && (last.Choice == Dc || last.Choice == One):
		for i, a := range last.Alts {
			if i == len(last.Alts)-1 || meTotal {
				ir.tailSeq(me, meTotal, a, out)
			}
		}
	case last.Kind == StStrats && last.Choice == Dk && len(last.Alts) == 1:
		ir.tailSeq(me, meTotal, last.Alts[0], out)
	}
}

// BaseName is a strategy or label name without its sort and module
// (`go:nat/m` gives go).
func BaseName(name string) string {
	base, _, _ := strings.Cut(name, ":")
	return base
}

// SeqText is the ELAN text of a sequence (comments).
func (ir *IR) SeqText(seq []Strat) string {
	if len(seq) == 0 {
		return "id"
	}
	var parts []string
	for i := range seq {
		s := &seq[i]
		switch s.Kind {
		case StId:
			parts = append(parts, "id")
		case StFail:
			parts = append(parts, "fail")
		case StRules:
			var ls []string
			for _, l := range s.Labels {
				ls = append(ls, BaseName(ir.Labels[l].Name))
			}
			parts = append(parts, s.Choice.String()+"("+strings.Join(ls, ", ")+")")
		case StStrats:
			var as []string
			for _, a := range s.Alts {
				as = append(as, ir.SeqText(a))
			}
			parts = append(parts, strings.ToUpper(s.Choice.String())+"("+strings.Join(as, ", ")+")")
		case StRepeat:
			parts = append(parts, "repeat*("+ir.SeqText(s.Body)+")")
		case StIterate:
			parts = append(parts, "iterate*("+ir.SeqText(s.Body)+")")
		case StCall:
			parts = append(parts, BaseName(ir.Strategies[s.Call].Name))
		case StMeta:
			parts = append(parts, "META")
		}
	}
	return strings.Join(parts, " ; ")
}
