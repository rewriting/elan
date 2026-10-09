/********************************************
 *
 * Fichier : datatypes.h
 * Auteur  : Régis Durand
 *           regis.durand@esial.uhp-nancy.fr
 *
 * Description :
 * Définition des types de bases.
 *
 ********************************************/


#ifndef __DATATYPES_H__
#define __DATATYPES_H__



#include <stdlib.h>



/* ---------- IntList ------------------------- */

typedef struct _IntList
{
  unsigned val;
  struct _IntList *next;
} IntList;


IntList *newIntList (unsigned val);
void IntList_addVal (IntList *l, unsigned val); /* ajout APRES l */
void IntList_free (IntList *l);




/* ---------- Table --------------------------- */

void* newTable (size_t nbElts, size_t eltSize);
#define Table_free(tab)    (free(tab))




/* ---------- Matrix -------------------------- */

void* newMatrix (size_t nbRows, size_t nbCols, size_t eltSize);
#define Matrix_free(m)    (free(m))




#endif /* __DATATYPES_H__ */
