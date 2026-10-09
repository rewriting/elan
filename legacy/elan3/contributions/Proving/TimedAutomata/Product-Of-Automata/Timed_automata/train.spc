specification train
  Clocks
    X Y Z
    nil
  States
    Loin Pres Sur Apres Haut t2 Bas t3 u0 u1 u2
    nil
  Labels
    app raise lower up down in out exit
    nil
  Automata
    (
    States
      Loin Pres Sur Apres
      nil
    Labels
      app in out exit
      nil
    Invariant
      Loin : true
      Pres : X<=5 ^ true
      Sur : X<=5 ^ true
      Apres : X<=5 ^ true
      nil
    Transitions
      Loin, app  :        true, X nil, Pres .
      Pres, in   : X>2  ^ true,   nil, Sur .
      Sur, out   :        true,   nil, Apres .
      Apres, exit :        true,   nil, Loin .
      nil
    ) .
    (
    States
      Haut t2 Bas t3
      nil
    Labels
      lower down raise up
      nil
    Invariant
      Haut : true
      t2 : Y<=1 ^ true
      Bas : true
      t3 : Y<=2 ^ true
      nil
    Transitions
      Haut, lower :        true, Y nil, t2 .
      t2, down  :         true,   nil, Bas .
      Bas, raise :        true, Y nil, t3 .
      t3, up    : Y>=1 ^ true,   nil, Haut .
      nil
    ) .
    (
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