#ifndef _minitools_h
#define _minitools_h

#define scale sizeof(long)
#define MAGICNUMBER ((long)0x12345678)
#define TRUEADR(adr) ((long*)(((char*)adr)-scale))
#define SIZE(adr)    (*(TRUEADR(adr)))
#define ADDSCALE(adr) ((char*)(((char*)adr)+scale))
#define SUBSCALE(adr) ((char*)(((char*)adr)-scale))

#endif
