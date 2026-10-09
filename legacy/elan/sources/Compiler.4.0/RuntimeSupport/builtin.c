#include "builtin.h"

#ifdef ATERM
AFun AFUN_IDENTIFIER;
AFun AFUN_ARRAY;
AFun AFUN_STRING;
#endif

struct TabFile* tabfile;

void Ginit_builtin() {
  tabfile = (struct TabFile*) malloc(sizeof(struct TabFile));
  tabfile->size = 3;
  tabfile->files[0] = stdin;
  tabfile->files[1] = stdout;
  tabfile->files[2] = stderr;

#ifdef ATERM
  AFUN_IDENTIFIER=ATmakeAFun("afun_identifier",1,ATfalse);
  ATprotectAFun(AFUN_IDENTIFIER);

  Gfsym_init(249,2,"String(,)","builtinString",249,0, NULL);
  AFUN_STRING = fsymtab[249].afun;
    //AFUN_STRING=ATmakeAFun("afun_string",2,ATfalse);
  ATprotectAFun(AFUN_STRING);

  Gfsym_init(204,2,"Array(,)","builtinArray",204,0, NULL);
  AFUN_ARRAY = fsymtab[204].afun;
  ATprotectAFun(AFUN_ARRAY);
#endif
}

#ifdef ATERM 
int GgetIdentifier(Gterm *n){
  if(GisIdentifierTagged(n)) {
    return ATgetInt((ATermInt)ATgetArgument(n,0));
  } else {
    printf("error in GgetIdentifier\n");
    exit(1);
  }
}
#endif
