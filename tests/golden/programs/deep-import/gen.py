# A chain of 40 modules m -> m1 -> ... -> m39 -> int: deeper than the 2004
# limit MAXINCLDEEP = 30.
N = 40
names = ["m"] + ["m%d" % i for i in range(1, N)]
for i, name in enumerate(names):
    imp = names[i + 1] if i + 1 < N else "int"
    with open(name + ".eln", "w") as f:
        f.write("module %s\nimport global %s; end\nend\n" % (name, imp))
