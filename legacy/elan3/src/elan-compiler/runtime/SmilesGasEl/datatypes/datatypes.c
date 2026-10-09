/********************************************
 *
 * Fichier : datatypes.c
 * Auteur  : Régis Durand
 *           regis.durand@esial.uhp-nancy.fr
 *
 * Description :
 * Implantation des types de bases.
 *
 ********************************************/


#include <string.h>
#include "datatypes.h"


IntList *newIntList (unsigned val)
{
  IntList *res = (IntList*) malloc (sizeof (IntList));
  res->val = val;
  res->next = 0;
  return res;
}


/* Ajoute une IntList après l */
void IntList_addVal (IntList *l, unsigned val)
{
  IntList *tmp = newIntList (val);
  tmp->next = l->next;
  l->next = tmp;
}


void IntList_free (IntList *l)
{
  if (l->next)
      IntList_free (l->next);
  free (l);
}





void* newTable (size_t nbElts, size_t eltSize)
{
  size_t tableSize = nbElts * eltSize;
  void *res = malloc (tableSize);
  memset (res, 0, tableSize);
  return res;
}




void* newMatrix (size_t nbRows, size_t nbCols, size_t eltSize)
{
  size_t matrixSize = nbRows * nbCols * eltSize;
  void *res = malloc (matrixSize);
  memset (res, 0, matrixSize);
  return res;
}
