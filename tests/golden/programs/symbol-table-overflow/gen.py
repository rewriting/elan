# 2100 constants: more than MAXNFSYM = 2000 function symbols.
with open("m.eln", "w") as f:
    f.write("module m\nimport global int; end\noperators global\n")
    f.writelines(" c%d : int;\n" % i for i in range(2100))
    f.write("end\nend\n")
