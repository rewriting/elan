# A strategy with 20 nested repeat* (the 2004 limit MAXINCLSTRAT = 30 allows
# 14: two stack entries per level).
s = "r"
for i in range(20):
    s = "repeat*(%s)" % s
with open("m.eln", "w") as f:
    f.write("""module m
import global int; end
operators global f(@) : (int) int; end
stratop global st : <int> bs; end
rules for int x : int;
local
 [r] f(x) => x end
end
strategies for int
implicit
 [] st => %s end
end
end
""" % s)
