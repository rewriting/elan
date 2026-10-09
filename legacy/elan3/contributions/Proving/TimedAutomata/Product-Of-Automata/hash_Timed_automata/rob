specification robots
  Clocks
    X Y Z B
    nil
  States
    D_Wait D_Pick D_Turn_R D_Put D_Turn_L
    G_Inspect G_Pick G_Turn_R G_Wait G_Put G_Turn_L
    S_Empty S_Busy S_Ready
    B_Mov B_Inspect B_On_G B_On_S B_On_D
    nil
  Labels
    s_ready s_empty s_busy
    d_pick d_turn_r d_put d_turn_l
    middle g_pick g_turn_r g_put g_turn_l
    b_mov 
    nil
//**
  Automata
    (
    Clock
      X
    States
      D_Wait D_Pick D_Turn_R D_Put D_Turn_L
      nil
    Labels
      d_pick d_turn_r d_put d_turn_l s_ready
      nil
    Invariant
      D_Wait    : true
      D_Pick    : X<=2^true
      D_Turn_R  : X<=6^true
      D_Put     : X<=2^true
      D_Turn_L  : X<=6^true
      nil
    Transitions
      D_Wait,   s_ready  :           true, X nil, D_Pick.
      D_Pick,   d_pick   : X>=1^X<=2^true, X nil, D_Turn_R.
      D_Turn_R, d_turn_r : X>=5^X<=6^true, X nil, D_Put.
      D_Put,    d_put    : X>=1^X<=2^true, X nil, D_Turn_L.
      D_Turn_L, d_turn_l : X>=5^X<=6^true, X nil, D_Wait.  
      nil
    ) .
    (
    Clock
      Y
    States
      G_Inspect G_Pick G_Turn_R G_Wait G_Put G_Turn_L
      nil
    Labels
      s_empty middle g_pick g_turn_r g_put g_turn_l
      nil
    Invariant
      G_Inspect : true
      G_Pick    : Y<=8^true
      G_Turn_R  : Y<=10^true
      G_Wait    : true
      G_Put     : Y<=2^true
      G_Turn_L  : Y<=10^true
      nil
    Transitions
      G_Inspect, middle  :           true,  Y nil, G_Pick.
      G_Pick,    g_pick  : Y>=3^Y<=8^true,  Y nil, G_Turn_R. 
      G_Turn_R,  g_turn_r: Y>=6^Y<=10^true, Y nil, G_Wait.
      G_Wait,    s_empty :           true,  Y nil, G_Put.
      G_Put,     g_put   : Y>=1^Y<=2^true,  Y nil, G_Turn_L.
      G_Turn_L,  g_turn_l: Y>=8^Y<=10^true, Y nil, G_Inspect.          
      nil
    ) .
    (
    Clock
      Z
    States
      S_Empty S_Busy S_Ready
      nil
    Labels
      s_empty s_busy s_ready g_put d_pick
      nil
    Invariant
      S_Empty : true
      S_Busy  : Z<=10^true
      S_Ready : true
      nil
    Transitions
      S_Empty, s_empty :           true,    nil, S_Empty.
      S_Empty, g_put   :           true,  Z nil, S_Busy.
      S_Busy,  s_busy  : Z>=8^Z<=10^true, Z nil, S_Ready.
      S_Ready, s_ready :           true,    nil, S_Ready.
      S_Ready, d_pick  :           true,  Z nil, S_Empty.
      nil
    ) .
    (
    Clock
      B
    States
      B_Mov B_Inspect B_On_G B_On_S B_On_D
      nil
    Labels
      b_mov g_pick g_put middle d_pick d_put
      nil
    Invariant
      B_Mov     : B<=134^true
      B_Inspect : B<=21^true
      B_On_G    : true
      B_On_S    : true
      B_On_D    : true
      nil
    Transitions
      B_Mov    , b_mov  : B>=133^B<=134^true, B nil, B_Inspect.
      B_Inspect, middle :             true,     nil, B_Inspect.
      B_Inspect, g_pick :             true,     nil, B_On_G.
      B_On_G   , g_put  :             true,     nil, B_On_S.
      B_On_S   , d_pick :             true,     nil, B_On_D.
      B_On_D   , d_put  :             true,   B nil, B_Mov.
      nil
    ) .
    nil
end