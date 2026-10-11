package tables

import (
	"os"
	"regexp"
	"strconv"
	"testing"
)

// Every RW define of the generated commtokens.h (#define RWword
// (-pos+BOFIDENT)) is the position of word in NewTabOfIdent.
func TestTabOfIdentPositions(t *testing.T) {
	src, err := os.ReadFile("testdata/commtokens.h")
	if err != nil {
		t.Fatal(err)
	}
	d, _ := newD()
	tab := NewTabOfIdent(d)
	m := regexp.MustCompile(`#define RW(\w+) \(-(\d+)\+BOFIDENT\)`).FindAllStringSubmatch(string(src), -1)
	if len(m) != 80 {
		t.Fatalf("%d RW defines", len(m))
	}
	for _, x := range m {
		pos, _ := strconv.Atoi(x[2])
		if tab.Index(x[1]) != pos {
			t.Errorf("%s at %d, commtokens.h says %d", x[1], tab.Index(x[1]), pos)
		}
	}
	if tab.Len() != 80 || len(reservedWords) != 90 || tab.Size() != MaxNofIdent {
		t.Fatalf("Len %d, %d reserved words", tab.Len(), len(reservedWords))
	}
}

// mrwt: the positions of the reserved words of the three grammars, a word in
// two grammars counted twice.
func TestMRWTCounts(t *testing.T) {
	d, _ := newD()
	tab, m := NewTabOfIdent(d), NewMRWT(d)
	twice := 0
	tab.Each(func(i int, s string) {
		if !m.Member(i) {
			t.Errorf("%s (%d) not in mrwt", s, i)
			return
		}
		switch m.Count(m.Posid) {
		case 2:
			twice++
		case 1:
		default:
			t.Errorf("%s counted %d", s, m.Count(m.Posid))
		}
	})
	if twice != 10 || m.Len() != 80 {
		t.Fatalf("%d words counted twice, %d in mrwt", twice, m.Len())
	}
}
