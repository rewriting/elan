specification Eng_train
  Clocks
    X Y Z
    nil
  States
    Far Near In After Up t1 Down t2 u0 u1 u2
    nil
  Labels
    app raise lower up down in out exit
    nil
  Automata
    (
    Clock
      X
    States
      Far Near In After
      nil
    Labels
      app in out exit
      nil
    Invariant
      Far   :        true
      Near  : X<=5 ^ true
      In    : X<=5 ^ true
      After : X<=5 ^ true
      nil
    Transitions
      Far  , app  :        true, X nil, Near  .
      Near , in   : X>2  ^ true,   nil, In    .
      In   , out  :        true,   nil, After .
      After, exit :        true,   nil, Far   .
      nil
    ) .
    (
    Clock
      Y
    States
      Up t1 Down t2
      nil
    Labels
      lower down raise up
      nil
    Invariant
      Up   : Y<=1 ^ true
      t1   : Y<=1 ^ true
      Down :        true
      t2   : Y<=2 ^ true
      nil
    Transitions
      Up  , lower :        true, Y nil, t1   .
      t1  , down  :        true,   nil, Down .
      Down, raise :        true, Y nil, t2   .
      t2  , up    : Y>=1 ^ true,   nil, Up   .
      nil
    ) .
    (
    Clock
      Z
    States
      u0 u1 u2
      nil
    Labels
      app lower raise exit
      nil
    Invariant
      u0 : true
      u1 : Z<=1 ^ true
      u2 : Z<=1 ^ true
      nil
    Transitions
      u0, app   :               true, Z nil, u1 .
      u1, lower : Z>=1 ^ Z<=1 ^ true,   nil, u0 .
      u0, exit  :               true, Z nil, u2 .
      u2, raise :               true,   nil, u0 .
      nil
    ) .
    nil
end