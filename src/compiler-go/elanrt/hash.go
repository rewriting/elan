package elanrt

// builtinHashTerm (code 210, common/builtinHashTerm.eln; S7e task 7): REM's
// hashTerm (src/compiler/runtime/termBase.c), 16 bits: an int or an
// identifier, the low bits of REM's tagged word ((n << 1) | 1, (i << 4) |
// 2); a term, Bob Jenkins' hash (doobs_hfunction) of its .ref symbol code,
// its arity and the hashes of its arguments (of the distinct elements of
// an AC term, in REM's order). Not REM's: strings (REM hashes the string
// term's two words, its length and its address: a value of its memory)
// and arrays (REM reads the arity of the symbol table's entry 204) get a
// value of elanc-go's, the same for equal strings; for 12 arguments or
// more REM reads its arguments from a wrong address (`GgetArgument(k+t,
// i)`), elanc-go hashes them as REM hashes the first 11.

// hashMask16 is REM's HASHMASK.
const hashMask16 = 0xFFFF

// HashTermInj is REM's hashTerm(t) for t of a sort whose only
// constructor is the injection of code inj (0: none) of a builtin sort:
// REM builds the injection's node over a builtin value (elanc-go keeps
// the value alone, native or boxed: S7e task 13); a stuck term is
// itself.
func HashTermInj(t Term, inj int32) int32 {
	if inj == 0 || !isValue(t) {
		return HashTerm(t)
	}
	return doobs(inj, []int32{HashTerm(t)})
}

// isValue tells whether t is a builtin value (an int, a string, a bool,
// an identifier).
func isValue(t Term) bool {
	switch t.Sym {
	case SymInt, SymString, SymTrue, SymFalse, SymIdent:
		return true
	}
	return false
}

// HashTerm is REM's hashTerm(t).
func HashTerm(t Term) int32 {
	switch t.Sym {
	case SymInt:
		return int32((uint64(int64(t.Int))<<1 | 1) & hashMask16)
	case SymIdent:
		return int32((uint64(uint32(t.Int))<<4 | 2) & hashMask16)
	case SymString:
		return int32(t.hash() & hashMask16)
	case SymArray:
		return doobs(Syms[t.Sym].Code, nil)
	}
	args := t.Args
	if Syms[t.Sym].AC {
		// the distinct elements (REM's termac: element, multiplicity)
		var e []Term
		for i, x := range args {
			if i == 0 || !Eq(x, args[i-1]) {
				e = append(e, x)
			}
		}
		args = e
	}
	inj := Syms[t.Sym].ArgInj
	h := make([]int32, len(args))
	for i, x := range args {
		switch {
		case inj == nil:
			h[i] = HashTerm(x)
		case Syms[t.Sym].AC:
			h[i] = HashTermInj(x, inj[0])
		default:
			h[i] = HashTermInj(x, inj[i])
		}
	}
	return doobs(Syms[t.Sym].Code, h)
}

// hsum4 and hshift are REM's HSUM4 and HSHIFT: int results, added to the
// unsigned longs a, b, c (sign-extended).
func hsum4(h0, h1, h2, h3 int32) uint64 {
	return uint64(int64(int32(uint32(h0) + uint32(h1)<<8 + uint32(h2)<<16 + uint32(h3)<<24)))
}

func hshift(h int32, n uint) uint64 { return uint64(int64(int32(uint32(h) << n))) }

// doobs is REM's doobs_hfunction on a symbol code and the hashes of its
// arguments, masked (16 bits).
func doobs(code int32, h []int32) int32 {
	a, b := uint64(0x9e3779b9), uint64(0x9e3779b9)
	c := uint64(int64(code))
	n := len(h)
	k := 0
	for n >= 12 {
		a += hsum4(h[k], h[k+1], h[k+2], h[k+3])
		b += hsum4(h[k+4], h[k+5], h[k+6], h[k+7])
		c += hsum4(h[k+8], h[k+9], h[k+10], h[k+11])
		a, b, c = jenkinsMix(a, b, c)
		k += 12
		n -= 12
	}
	c += uint64(len(h))
	// (REM's switch falls through from len down to 1)
	add := func(x *uint64, i int, shift uint) {
		if n > i {
			*x += hshift(h[k+i], shift)
		}
	}
	add(&c, 10, 24)
	add(&c, 9, 16)
	add(&c, 8, 8)
	add(&b, 7, 24)
	add(&b, 6, 16)
	add(&b, 5, 8)
	add(&b, 4, 0)
	add(&a, 3, 24)
	add(&a, 2, 16)
	add(&a, 1, 8)
	add(&a, 0, 0)
	_, _, c = jenkinsMix(a, b, c)
	return int32(c & hashMask16)
}

// jenkinsMix is REM's mix(a,b,c) on unsigned longs.
func jenkinsMix(a, b, c uint64) (uint64, uint64, uint64) {
	a -= b
	a -= c
	a ^= c >> 13
	b -= c
	b -= a
	b ^= a << 8
	c -= a
	c -= b
	c ^= b >> 13
	a -= b
	a -= c
	a ^= c >> 12
	b -= c
	b -= a
	b ^= a << 16
	c -= a
	c -= b
	c ^= b >> 5
	a -= b
	a -= c
	a ^= c >> 3
	b -= c
	b -= a
	b ^= a << 10
	c -= a
	c -= b
	c ^= b >> 15
	return a, b, c
}
