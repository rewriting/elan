# 101 imports of any[X]: more than MAXANYS = 100. Until S3b the 101st
# overflowed the table silently (exit status 0, or a corrupted neighbour).
N = 101
with open("m.eln", "w") as f:
    f.write("module m\nimport global int %s; end\nsort %s; end\nend\n"
            % (" ".join("any[s%d]" % i for i in range(N)), " ".join("s%d" % i for i in range(N))))
