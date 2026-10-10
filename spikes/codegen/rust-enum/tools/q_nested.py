# Emits the Rust code of the propc constants q1, q2, q3: the right-hand side
# of each rule printed as one nested expression (no A-normal form needed:
# no runtime context is threaded through the calls). Rust evaluates call
# arguments left to right, so terms are built in the same (post-)order as
# the A-normal form of the first version.
# Usage: python3 q_nested.py propc.eln > q.rs
import re, sys
src = open(sys.argv[1]).read()
src = re.sub(r'//[^\n]*', '', src)

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

def count(t):
    return (1 if t[1] else 0) + sum(count(a) for a in t[1])

def emit(t, ind):
    name, args = t
    if not args:
        return 'c_%s()' % name
    inner = [emit(a, ind + 1) for a in args]
    one = '%s(%s)' % (name, ', '.join('&' + a for a in inner))
    if len(one) + 4 * ind <= 90:
        return one
    pad = '    ' * (ind + 1)
    return '%s(\n%s\n%s)' % (name, ',\n'.join(pad + '&' + a for a in inner), '    ' * ind)

for q in ('q1', 'q2', 'q3'):
    m = re.search(r'\[\]\s*' + q + r'\s*=>(.*?)\bend\b', src, re.S)
    term, _ = parse(tokens(m.group(1)), 0)
    print('// [] %s => <rhs, %d subterms>' % (q, count(term)))
    print('fn %s() -> P {' % q)
    print('    step();')
    print('    ' + emit(term, 1))
    print('}\n')
