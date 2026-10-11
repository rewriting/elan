package tables

import (
	"fmt"

	"elan/front/diag"
)

// NoInt is NOINT, the free slot of a MiTab.
const NoInt = -1

// MiTab is mitab: an open-addressing table of positive ints with occurrence
// counts (the macro table of mlstream, mrwt), probing by +211.
type MiTab struct {
	d     *diag.D
	n     []int
	nofoc []int
	nin   int
	// Posid is the position of the last number looked up (mitab::posid).
	Posid int
}

// NewMiTab is mitab(size, init...): the C varargs list ends at the first
// value <= 0 (NOINT in the generated tabofident.cc), so the values after it
// are ignored.
func NewMiTab(d *diag.D, size int, init ...int) *MiTab {
	m := &MiTab{d: d, n: make([]int, size), nofoc: make([]int, size)}
	for i := range m.n {
		m.n[i] = NoInt
	}
	for _, v := range init {
		if v <= 0 {
			break
		}
		m.AddN(v)
	}
	return m
}

// Member is mitab::member; it sets Posid. (A negative n is an out-of-bounds
// index in the C++; it panics here.)
func (m *MiTab) Member(n int) bool {
	m.Posid = n % len(m.n)
	for m.n[m.Posid] != NoInt {
		if m.n[m.Posid] == n {
			return true
		}
		m.Posid = (m.Posid + 211) % len(m.n)
	}
	return false
}

// AddN is mitab::addn: adds n or counts one more occurrence; its position.
func (m *MiTab) AddN(n int) int {
	if n == NoInt {
		m.d.Errs("[addn] ERROR trying to add NOINT value into mitab\n int. err.\n")
		m.d.Fail()
	}
	if !m.Member(n) {
		if m.nin+1 >= len(m.n) {
			m.d.Errs(fmt.Sprintf("[addn] ERROR, mi tab overflowed over %d\n int.err.\n", len(m.n)))
			m.d.Fail()
		}
		m.nin++
		m.n[m.Posid], m.nofoc[m.Posid] = n, 0
	}
	m.nofoc[m.Posid]++
	return m.Posid
}

// RemoveNonP is mitab::removenonp: one occurrence less at position p; the
// slot is freed at zero; below zero is a fatal error.
func (m *MiTab) RemoveNonP(p int) {
	m.nofoc[p]--
	if m.nofoc[p] < 0 {
		m.d.Errs("[removen] trying to remove non ex. number\nint.err.\n")
		m.d.Fail()
	}
	if m.nofoc[p] != 0 {
		return
	}
	m.n[p] = NoInt
	m.nin--
}

// RemoveN is mitab::removen: RemoveNonP at the position of n (when n is
// not a member, at the free slot where it would go: the error above).
func (m *MiTab) RemoveN(n int) {
	m.Member(n)
	m.RemoveNonP(m.Posid)
}

// Count is the occurrence count at position p.
func (m *MiTab) Count(p int) int { return m.nofoc[p] }

// Len is the number of numbers in the table.
func (m *MiTab) Len() int { return m.nin }
