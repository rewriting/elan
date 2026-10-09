#ifndef _bgraph_h
#define _bgraph_h
#include "bitset.h"
#include "link.h"
#include "tools.h"

/*
 * un Bipartite Graph (BG *) est un tableau de bitSet* (1 par pattern)
 * c'est aussi un tableau de tableaux de term* (1 par sujet)
 */

/*
 * size         : nombre de patterns sous AC
 * bs_tab       : tableau de bitSet. Chaque bitSet represente tous les
                  sous-termes du sujet
 * sub_tab_size : nombre de substitutions. Il y en a autant que de patterns
                  (regles de reecriture) concernes
 * sub_tab      : tableau de substitutions qui ont toutes la meme taille
 * link_tab     : tableau de links, de taille size
 *                chaque link a la taille du sujet
 */

typedef struct BG
{
  int size;
  bitSet **bs_tab;
  LINK **link_tab;
} BG;

#ifdef NOTMACRO
extern void BG_delete(BG *bg);
extern void BG_set(BG *bg, int no_pattern, bitSet *bs);
extern bitSet *BG_get(BG *bg, int no_pattern);
extern void BG_clear(BG *bg, int no_pattern);
extern int BG_size(BG *bg);
extern void BG_set_size(BG *bg, int size);
extern int BG_link_size(BG *bg);
extern void BG_link_set(BG *bg, int no_pattern, LINK *link);
extern LINK *BG_link_get(BG *bg, int no_pattern);

#else

#define BG_delete(bg) {\
  int i;\
  if(bg->link_tab != NULL) {\
    for(i=0 ; i<bg->size ; i++) {\
	  bitSet_delete(bg->bs_tab[i]);\
	  if(bg->link_tab[i]!=NULL) LINK_delete(bg->link_tab[i]); }\
      IFREE(bg->bs_tab); IFREE(bg->link_tab); }\
  else {\
    for(i=0 ; i<bg->size ; i++) bitSet_delete(bg->bs_tab[i]);\
      IFREE(bg->bs_tab); }\
  IFREE(bg); }

#define BG_set(bg,no_pattern,bs) bg->bs_tab[no_pattern]=bs;

#define BG_get(bg,no_pattern) bg->bs_tab[no_pattern]

#define BG_clear(bg,no_pattern) bg->bs_tab[no_pattern]=0;

#define BG_size(bg) ((bg)->size)
#define BG_set_size(bg,s) ((bg)->size)=(s);


#define BG_link_size(bg) BG_size(bg)
#define BG_link_set(bg,no_pattern,link) bg->link_tab[no_pattern]=link;
#define BG_link_get(bg,no_pattern) ((bg->link_tab==NULL)?NULL:(bg->link_tab[no_pattern]))
//extern LINK *BG_link_get(BG *bg, int no_pattern);

#endif

extern BG *BG_create(int size, int necessary_link);
extern void BG_print(BG *bg);
extern int BG_cbg2bg(int *liste_pattern, BG *cbg, BG *bg);
extern int BG_solve_one(BG *bg, int choice[], multiplicityType multiplicity[],
			int maxChoice, int indice);
extern int BG_greedy_solve_one(BG *bg, int choice[], int multiplicity[],
			int maxChoice, int indice);

#endif








