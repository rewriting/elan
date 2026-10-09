# 10010 string literals: more than MAXNOFSTRING = 10000 string constants (a
# limit of the lexem encoding, lex/lexem.h).
with open("m.eln", "w") as f:
    f.write("module m\nimport global int string; end\noperators global\n g(@) : (string) int;\nend\n")
    f.write("rules for int\nglobal\n")
    f.writelines(' [] g("x") => %d end\n' % i for i in range(10010))
    f.write("end\nend\n")
