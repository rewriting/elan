# Emits the Rust code of the propc constants q1, q2, q3: the right-hand side
# of each rule, flattened to A-normal form (one `let` per subterm), as the
# compiler back-end would. Usage: python3 anf_q.py propc.eln > q.rs
import re, sys
src = open(sys.argv[1]).read()
src = re.sub(r'//[^\n]*', '', src)
FN = {'and': 'and', 'xor': 'xor', 'or': 'or', 'iff': 'iff', 'not': 'not', 'implies': 'implies'}

def tokens(s):
    return re.findall(r'[A-Za-z_][A-Za-z0-9_]*|[(),]', s)

def parse(toks, i):
    name = toks[i]; i += 1
    args = []
    if i < len(toks) and toks[i] == '(':
        i += 1
        while True:
            a, i = parse(toks, i); args.append(a)
            if toks[i] == ',': i += 1; continue
            assert toks[i] == ')'; i += 1; break
    return (name, args), i

for q in ('q1', 'q2', 'q3'):
    m = re.search(r'\[\]\s*' + q + r'\s*=>(.*?)\bend\b', src, re.S)
    term, _ = parse(tokens(m.group(1)), 0)
    lines = []
    consts = set()
    cnt = [0]
    def emit(t):
        name, args = t
        if not args:
            consts.add(name)
            return 'c_' + name
        vs = [emit(a) for a in args]
        v = 't%d' % cnt[0]; cnt[0] += 1
        lines.append('    let %s = %s(rt, %s);' % (v, FN[name], ', '.join(vs)))
        return v
    top = emit(term)
    print('// [] %s => <rhs, %d subterms>' % (q, cnt[0]))
    print('fn %s(rt: &mut Rt) -> Term {' % q)
    print('    rt.steps += 1;')
    for c in sorted(consts, key=lambda c: int(c[1:])):
        print('    let c_%s = rt.c(%s);' % (c, c.upper()))
    print('\n'.join(lines))
    print('    %s' % top)
    print('}\n')
