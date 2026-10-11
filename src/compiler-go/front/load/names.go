package load

// Module names (msemact.cc mkmodname1-5, divideonargs, int_arg): a name is
// built on a stack of strings (struct chlist) by the actions of the import
// lists of modgram and ldmodgram.

// mkmodname1 pushes an identifier (or a number).
func mkmodname1(st []string, s string) []string { return append(st, s) }

// mkmodname applies f to the stack; f reports an internal error by false
// (the C++'s `[semact.mkmodnameN] int.err`, fatal).
func (L *loader) mkmodname(st []string, f func([]string) ([]string, bool, string)) []string {
	st, ok, which := f(st)
	if !ok {
		L.d.Errs("[semact.", which, "] int.err\n")
		L.d.Fail()
	}
	return st
}

// mkmodname2: name[args].
func mkmodname2(st []string) ([]string, bool, string) {
	if len(st) < 2 {
		return st, false, "mkmodname2"
	}
	n := len(st)
	st[n-2] = st[n-2] + "[" + st[n-1] + "]"
	return st[:n-1], true, ""
}

// mkmodname3: a,b.
func mkmodname3(st []string) ([]string, bool, string) {
	if len(st) < 2 {
		return st, false, "mkmodname3"
	}
	n := len(st)
	st[n-2] = st[n-2] + "," + st[n-1]
	return st[:n-1], true, ""
}

// mkmodname4: <X> is <X->X>.
func mkmodname4(st []string) ([]string, bool, string) {
	if len(st) < 1 {
		return st, false, "mkmodname4"
	}
	n := len(st)
	st[n-1] = "<" + st[n-1] + "->" + st[n-1] + ">"
	return st, true, ""
}

// mkmodname5: <X->Y>.
func mkmodname5(st []string) ([]string, bool, string) {
	if len(st) < 2 {
		return st, false, "mkmodname5"
	}
	n := len(st)
	st[n-2] = "<" + st[n-2] + "->" + st[n-1] + ">"
	return st[:n-1], true, ""
}

// divideonargs: the module name and its arguments, split at the '[' and the
// ',' of bracket depth 1; a final ']' is removed.
func divideonargs(cname string) (string, []string) {
	p := []byte(cname)
	var starts []int
	c := 0
	for i := 0; i < len(p); i++ {
		if p[i] == '[' {
			c++
		} else if p[i] == ']' {
			c--
		}
		if (p[i] == ',' && c == 1) || (p[i] == '[' && c == 1) {
			p[i] = 0
			starts = append(starts, i+1)
		}
	}
	if len(p) > 0 && p[len(p)-1] == ']' {
		p[len(p)-1] = 0
	}
	cstr := func(i int) string { // the C string at i
		j := i
		for j < len(p) && p[j] != 0 {
			j++
		}
		return string(p[i:j])
	}
	var args []string
	for _, s := range starts {
		args = append(args, cstr(s))
	}
	return cstr(0), args
}

// intArg is int_arg: only digits (the empty string too).
func intArg(s string) bool {
	for i := 0; i < len(s); i++ {
		if s[i] < '0' || s[i] > '9' {
			return false
		}
	}
	return true
}

// atoi of a string of digits, wrapping as a C int.
func atoi(s string) int32 {
	var u uint32
	for i := 0; i < len(s); i++ {
		u = u*10 + uint32(s[i]-'0')
	}
	return int32(u)
}
