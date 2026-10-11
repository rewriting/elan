package elanrt

import "testing"

func TestPercentFor(t *testing.T) {
	for _, c := range []struct {
		live uint64
		want int
	}{
		{0, GCPercent},             // nothing measured yet
		{1 << 20, 800},             // capped: Go's minimum 4 MiB x 8 = MinHeap
		{4 << 20, 700},             // goal 8 x 4 MiB = MinHeap
		{MinHeap / 5, GCPercent},   // goal 5 x live = MinHeap
		{MinHeap, GCPercent},       // large: GCPercent
		{100 * MinHeap, GCPercent}, // large: GCPercent
	} {
		if got := percentFor(c.live); got != c.want {
			t.Errorf("percentFor(%d) = %d, want %d", c.live, got, c.want)
		}
	}
}
