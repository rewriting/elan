# One rule with 60 variables (more than MAXNOFVAR = 49).
vs = ["x%d" % i for i in range(60)]
with open("m.eln", "w") as f:
    f.write("module m\nimport global int; end\noperators global\n g(%s) : (%s) int;\nend\n"
            % (",".join(["@"] * 60), " ".join(["int"] * 60)))
    f.write("rules for int\n %s : int;\nglobal\n [] g(%s) => 0 end\nend\nend\n"
            % (", ".join(vs), ",".join(vs)))
