# 1600 mixfix operators of two fresh identifiers each: more than MAXNOFIDENT =
# 3000 identifiers (the table of identifiers overflows).
with open("m.eln", "w") as f:
    f.write("module m\nimport global int; end\noperators global\n")
    f.writelines(" c%d @ d%d : (int) int;\n" % (i, i) for i in range(1600))
    f.write("end\nend\n")
