#include "firewall.h"

void printTerm(char* s, Gterm* t) {
  printf("%s ",s);
  termOut(stdout,t);
  printf("\n");
}

Gterm *makeFirewall(char *protocol,
                    int x1_int,int x2_int,int x3_int,int x4_int,
                    int port_source_int,
                    int y1_int,int y2_int,int y3_int,int y4_int,
                    int port_dest_int){ 
  
  Gterm *ip_source,*ip_dest,*header,*proto;
  Gterm *x1,*x2,*x3,*x4;
  Gterm *y1,*y2,*y3,*y4;
  Gterm *port_source,*port_dest;
  int ip_index,h_index,tcp_index,udp_index;
  int int2Octet_index, int2Port_index;
  
  //trouver les index des symboles
  ip_index = getSymbolIndex("ip(...)");
  h_index = getSymbolIndex("h(,,,,)");
  tcp_index = getSymbolIndex("tcp");
  udp_index = getSymbolIndex("udp");

  int2Octet_index = getSymbolIndex("int2Octet()");
  int2Port_index = getSymbolIndex("int2Port()");
  
  /**********************************************
   * construction de l'arbre a partir des index *
   **********************************************/
  
  //construction du protocole
  if (!strcmp(protocol,"tcp")){
    GmakeAppl0(proto,tcp_index);
  } else if (!strcmp(protocol,"udp")) {
    GmakeAppl0(proto,udp_index);
  }
    //construction de l'adresse IP source
  GmakeAppl1(x1,int2Octet_index,GsetIntegerTag(x1_int));
  GmakeAppl1(x2,int2Octet_index,GsetIntegerTag(x2_int));
  GmakeAppl1(x3,int2Octet_index,GsetIntegerTag(x3_int));
  GmakeAppl1(x4,int2Octet_index,GsetIntegerTag(x4_int));
  GmakeAppl4(ip_source,ip_index,x1,x2,x3,x4);
  
  //construction de l'adresse IP destination
  GmakeAppl1(y1,int2Octet_index,GsetIntegerTag(y1_int));
  GmakeAppl1(y2,int2Octet_index,GsetIntegerTag(y2_int));
  GmakeAppl1(y3,int2Octet_index,GsetIntegerTag(y3_int));
  GmakeAppl1(y4,int2Octet_index,GsetIntegerTag(y4_int));
  GmakeAppl4(ip_dest,ip_index,y1,y2,y3,y4);

  //greffe des elements pour le header
  GmakeAppl1(port_source,int2Port_index,GsetIntegerTag(port_source_int));
  GmakeAppl1(port_dest,int2Port_index,GsetIntegerTag(port_dest_int));
  
  GmakeAppl5(header,h_index,proto,ip_source,port_source,ip_dest,port_dest);
  
  return header;
}

int main(int argc,char **argv){
  long bp;
  Gterm *query;
  Gterm *res;
  
  initElanLib(&bp);
  
  if (!setChoicePoint()){
      // eval(h(tcp,ip(140.12.10.1),20,ip(192.168.45.12),80))
      //query = makeFirewall("tcp",140,12,10,1,20,192,168,45,12,80);

      // eval(h(tcp,ip(140.192.37.30),20,ip(192.168.45.12),21))
    query = makeFirewall("tcp",140,192,37,30,20,192,168,45,12,21);

    printTerm("query",query);
  
    res = funTab[getSymbolIndex("eval()")](query);

    printTerm("res",res);
    
    printf("\nresult action = ");termOut(stdout,term_unflatten(res));printf("\n");
    fail();
  }
  exit(0);
}
