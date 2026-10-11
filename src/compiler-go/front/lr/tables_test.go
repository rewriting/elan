package lr

import "testing"

// Sanity of the converted MACC tables (plan S8a, finding 1).
func TestConvertedTables(t *testing.T) {
	for _, c := range []struct {
		tab                    *Table
		name                   string
		states, actions, gotos int
	}{
		{ModGram, "modgram", 646, 2728, 1141},
		{LdModGram, "ldmodgram", 90, 186, 82},
		{PpexGram, "ppexgram", 50, 203, 24},
	} {
		if c.tab.Name != c.name || len(c.tab.States) != c.states {
			t.Fatalf("%s: %d states", c.tab.Name, len(c.tab.States))
		}
		na, ng, nd := 0, 0, 0
		for i, s := range c.tab.States {
			na += len(s.Pr)
			ng += len(s.Ak)
			for k, a := range s.Pr {
				if a.Sym == Default {
					nd++
					if k != 0 {
						t.Errorf("%s state %d: Default entry at %d", c.name, i, k)
					}
				}
				if a.Kind == SRErr || a.Kind == RRErr {
					t.Errorf("%s state %d: conflict entry", c.name, i)
				}
				if a.Kind == Shift && (a.Num < 0 || int(a.Num) >= c.states) {
					t.Errorf("%s state %d: shift to %d", c.name, i, a.Num)
				}
			}
			for _, g := range s.Ak {
				if g.To < 0 || int(g.To) >= c.states {
					t.Errorf("%s state %d: goto %d", c.name, i, g.To)
				}
			}
		}
		if na != c.actions || ng != c.gotos {
			t.Errorf("%s: %d actions, %d gotos", c.name, na, ng)
		}
		t.Logf("%s: %d states, %d actions, %d gotos, %d Default entries", c.name, len(c.tab.States), na, ng, nd)
	}
}
