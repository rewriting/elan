
struct term *str_s1_term_ac4(arg)
  struct term *arg;
{ 
  unsigned long ok;
  struct term *v1 = arg;
  /* genRulesApp-begin */
  named_tried++;
  /*genStratAppBody*/
  { 
    ok=0xffffffff;
    /*genlinmatch*/
    /*genreclinmatch*/
    ok &= 03;
    switch(v1->fs){
      /*BL3*/
    case 202: /* 'b' */ 
      /*genreclinmatch*/
      ok &= 03;
      /*end-genreclinmatch*/
      break;
    default:
      /*genreclinmatch*/
      freeterm(v1);
      number_of_fail++;
      fail();
      return(NULL);
      /*end-genreclinmatch*/
    }
    /*end-genreclinmatch*/
    if (ok) {
      if (ok & 01) {
	nofsreductions++;
	v1 = con205;
	goto slab0;
      r1fin:;
      }
      if (ok & 02) {
	nofsreductions++;
	v1 = con206;
	goto slab0;
      r2fin:;
      }
    }
    freeterm(v1);
    number_of_fail++;
    fail();
    return(NULL);
    /*end-genlinmatch*/
  }
  /*end-genStratAppBody*/
  /* genRulesApp-end */
slab0:;
  return(v1);
}
