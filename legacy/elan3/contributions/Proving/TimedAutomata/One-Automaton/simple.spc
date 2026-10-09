specification simple
  Clocks
    X Y
    nil
  States
    s0 s1 s2
    nil
  Labels
    a b
    nil
  Invariant
    s0 : true
    s1 : X<5 ^ true
    s2 : true
    nil
  Transitions
    s0, a : X< 2 ^ true, Y nil, s1 .
    s0, b : X>=1 ^ true,   nil, s2 .
    s1, a : Y< 1 ^ true,   nil, s2 .
    s1, b :        true, Y nil, s1 .
    s2, a : Y> 2 ^ true,   nil, s0 .
    nil
end
