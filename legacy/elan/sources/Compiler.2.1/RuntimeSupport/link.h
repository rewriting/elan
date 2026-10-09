#ifndef _link_h
#define _link_h
#include "term.h"
//#include "match_state.h" CA NE MARCHE PAS AVEC !!!

/*
 * un link (LINK *) est un tableau de match_state*
 */

typedef struct LINK
{
  int size;
  struct match_state **ms_tab;
} LINK;

#ifdef NOTMACRO
extern LINK *LINK_create(int size);
extern void LINK_delete(LINK *link);
extern void LINK_set(LINK *link, int pos, struct match_state *ms);
extern struct match_state *LINK_get(LINK *link, int pos);
extern void LINK_clear(LINK *link, int pos);
extern void LINK_print(LINK *link);
extern int LINK_size(LINK *link);
extern void LINK_set_size(LINK *link, int size);

#else

extern LINK *LINK_create(int size);

extern void LINK_delete(LINK *link);
#define LINK_set(link,pos,ms) (link)->ms_tab[pos]=(ms);
#define LINK_get(link,pos) ((link)->ms_tab[pos])
#define LINK_clear(link,pos) (link)->ms_tab[pos]=0;

extern void LINK_print(LINK *link);

#define LINK_size(link) ((link)->size)
#define LINK_set_size(link,s) (link)->size=(s);

#endif
#endif








