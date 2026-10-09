/********************************************
 *
 * Fichier : huxu.c
 * Auteur  : Régis Durand
 *           regis.durand@esial.uhp-nancy.fr
 *
 * Description :
 * Implantation des fonctions utiles pour
 * l'algorithme HU & XU (1995).
 *
 ********************************************/




#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../../datatypes/datatypes.h"
#include "../../datatypes/molecule.h"
#include "../../parser/parser_mol.h"
#include "huxu.h"



static float*    setRadiiTable      (Molecule *m);
static float*    setCvTable         (Molecule *m);
static float*    setAdjacencyMatrix (Molecule *m);
static IntList** setLayerMatrix     (Molecule *m);

static float*    setCvmMatrix       (IntList** layer, float* cv, unsigned size);
static float*    setBondMatrix      (float* adj, IntList** layer, unsigned size);
static double*   setSVector         (float *cvm, float* bond, unsigned size);
static double*   setEaMatrix        (float* adj, double* sVector, float* radii, unsigned size);
static double*   setEaStarMatrix    (double* eaMatrix, unsigned size);
static double    setEaid            (double* eaStarMatrix, unsigned size);



int HUandXUCompare (char *s1, char *s2)
{
  /* on exécute l'algorithme sur s1 */
  double eaid1 = HUandXU (s1);
  /* puis sur s2 */
  double eaid2 = HUandXU (s2);

  /* si les 2 molécules ont été reconnues */
  if (eaid1 && eaid2)
    /* on compare à 10^(-8) près */
    return ((unsigned) (100000000. * eaid1) ==
	    (unsigned) (100000000. * eaid2));
  else
    /* sinon on considère les 2 molécules
     * comme différentes */
    return 0;
}





double HUandXU (char *s)
{
  /* On parse la chaine pour en faire une molécule */
  Molecule *mol = smilesGasEl2molecule (s);

  if (mol)
    {
      /* on exécute l'algorithme sur la molécule */
      unsigned i, j;

      /* cf. commentaires des fonctions */
      float    *radiiTable      = setRadiiTable (mol);
      float    *cvTable         = setCvTable (mol);
      float    *adjacencyMatrix = setAdjacencyMatrix (mol);
      IntList **layerMatrix     = setLayerMatrix (mol);

      float    *cvmMatrix       = setCvmMatrix (layerMatrix, cvTable, mol->count);
      float    *bondMatrix      = setBondMatrix (adjacencyMatrix, layerMatrix, mol->count);
      double   *sVector         = setSVector (cvmMatrix, bondMatrix, mol->count);
      double   *eaMatrix        = setEaMatrix (adjacencyMatrix, sVector, radiiTable, mol->count);
      double   *eaStarMatrix    = setEaStarMatrix (eaMatrix, mol->count);

      double result = setEaid (eaStarMatrix, mol->count);

      /* On libère toute la mémoire allouée */
      Molecule_free (mol);
      Table_free (radiiTable);
      Table_free (cvTable);
      Matrix_free (adjacencyMatrix);

      for (i = 0u; i < mol->count; i++)
	for (j = 0u; j < mol->count; j++)
	  if (layerMatrix [mol->count* i + j])
	    IntList_free (layerMatrix [mol->count* i + j]);
      Matrix_free (layerMatrix);

      Matrix_free (cvmMatrix);
      Matrix_free (bondMatrix);
      Table_free (sVector);
      Matrix_free (eaMatrix);
      Matrix_free (eaStarMatrix);


      return result;
    }

  return 0.;
}





static float* setRadiiTable (Molecule *m)
{
  float *table = radiis ();
  unsigned i;
  float *radiiTable = (float*) newMatrix (m->count, m->count, sizeof (float));

  /* pour chaque atome */
  for (i = 0u; i < m->count; i++)
    /* selon son symbole, la table radiis() nous fournit le nombre */
    radiiTable[i] = table [(int) m->atoms[i]->symbol];

  return radiiTable;
}



static float* setCvTable (Molecule *m)
{
  float *table = cvs ();
  unsigned i;
  float *cvTable = (float*) newMatrix (m->count, m->count, sizeof (float));

  /* pour chaque atome */
  for (i = 0u; i < m->count; i++)
    {
      AtomList *atoms;
      /* on a une connectivité de base fournie par cvs()[symbol] */
      cvTable[i] = table [(int) m->atoms[i]->symbol];
      /* et on ajoute à ce nombre les liens avec les atomes voisins */
      for (atoms = m->atoms[i]->atoms; atoms; atoms = atoms->next)
	cvTable[i] += atoms->link;
    }

  return cvTable;
}




static float* setAdjacencyMatrix (Molecule *m)
{
  unsigned i;
  float *adj = (float*) newMatrix (m->count, m->count, sizeof (float));

  /* pour chaque atome i , un lien i->j de n
   * se traduit par adj (i,j) = n */
  for (i = 0u; i < m->count; i++)
    {
      AtomList *atoms;
      for (atoms = m->atoms[i]->atoms; atoms; atoms = atoms->next)
	adj[m->count* i + atoms->atom->number] = atoms->link;
    }

  return adj;
}



static void fillLayer (Molecule *m, IntList **layer, unsigned row, unsigned col, IntList *toSee, unsigned size);


static IntList** setLayerMatrix (Molecule *m)
{
  unsigned i, j;
  IntList **layer = (IntList**) newMatrix (m->count, m->count, sizeof (IntList*));

  /* Pour chaque atome i... */
  for (i = 0u; i < m->count; i++)
    {
      /* layer (i, 0) = {i} */
      layer [m->count* i /* + 0 */] = newIntList (m->atoms[i]->number);
      /* Cet atome est marqué */
      m->atoms[i]->color = VISITED;

      /* on remplit les diverses couches */
      fillLayer (m, layer, i, 1u, layer[m->count* i /* + 0 */], m->count);

      /* on efface les marques */
      for (j = 0u; j < m->count; j++)
	m->atoms[j]->color = NOT_VISITED;
    }

  return layer;
}

static void fillLayer (Molecule *m, IntList **layer, unsigned row, unsigned col, IntList *toSee, unsigned size)
{
  AtomList *tmpList;

  /* Pour chaque atome de la liste que l'on vient de remplir */
  for (; toSee; toSee = toSee->next)
    {
      /* on regarde ses voisins */
      for (tmpList = m->atoms[toSee->val]->atoms; tmpList; tmpList = tmpList->next)
	{
	  /* s'il n'a pas été marqué */
	  if (m->atoms[tmpList->atom->number]->color == NOT_VISITED)
	    {
	      /* on l'ajoute à la nouvelle liste */
	      if (layer[size* row + col])
		IntList_addVal (layer[size* row + col], m->atoms[tmpList->atom->number]->number);
	      else
		layer[size* row + col] = newIntList (m->atoms[tmpList->atom->number]->number);
	      /* et on le marque */
	      m->atoms[tmpList->atom->number]->color = VISITED;
	    }
	}
    }

  /* si cette liste n'est pas vide */
  if (col+1u < size && layer[size* row + col])
    /* on crée la prochaine couche récursivement */
    fillLayer (m, layer, row, col+1u, layer[size* row + col], size);
}








static float* setCvmMatrix (IntList** layer, float* cv, unsigned size)
{
  unsigned i, j;
  IntList *list;
  float* cvmMatrix = (float*) newMatrix (size, size, sizeof (float));

  /* Pour chaque case de cvmMatrix */
  for (i = 0u; i < size; i++)
    {
      for (j = 0u; j < size; j++)
	{
	  float *resf = cvmMatrix+size* i + j;
	  /* on fait la somme des connectivités des atomes de
           * layer (i, j) */
	  for (list = layer [size* i + j]; list; list = list->next)
	    *resf += cv [list->val];
	}
    }

  return cvmMatrix;
}





static float* setBondMatrix (float* adj, IntList** layer, unsigned size)
{
  unsigned i, j;
  IntList *list1, *list2;
  float* bondMatrix = (float*) newMatrix (size, size, sizeof (float));

  /* Pour chaque case de bondMatrix */
  for (i = 0u; i < size; i++)
    {
      for (j = 0u; j < size - 1u; j++)
	{
	  float *resf = bondMatrix+size* i + j;
	  /* on fait la somme des liens entre layer(i,j+1) et de layer(i,j) */
	  for (list1 = layer [size* i + j]; list1; list1 = list1->next)
	    for (list2 = layer [size* i + j+1]; list2; list2 = list2->next)
		  *resf += adj [size* list1->val + list2->val];
	}
    }

  return bondMatrix;
}






static double* setSVector (float *cvm, float* bond, unsigned size)
{
  unsigned i, j;
  double* sVector = (double*) newTable (size, sizeof (double));

  for (i = 0u; i < size; i++)
    {
      double factor = 0.1;
      double* resf = sVector+ i;
      *resf = (double) cvm [size* i /* + 0 */];
      for (j = 0u; j < size; j++)
	{
	  *resf += ((double) (cvm [size* i + j+1])) * ((double) bond [size* i + j]) * factor;
	  factor *= 0.1;
	}
    }

  return sVector;
}






static double* setEaMatrix (float* adj, double* sVector, float *radii, unsigned size)
{
  unsigned i, j;
  double* eaMatrix = (double*) newMatrix (size, size, sizeof (double));

  for (i = 0u; i < size; i++)
    for (j = 0u; j < size; j++)
      if (i == j)
	eaMatrix [size* i + j] = sqrt ((double) radii [i]) / 6.;
      else
	eaMatrix [size* i + j] = sqrt ((double) adj [size* i + j]) * (sqrt (sVector[i]/sVector[j]) + sqrt (sVector[j]/sVector[i])) / 6.;

  return eaMatrix;
}





static void ea_multiply (double* res, double* m1, double* m2, unsigned size);
static void ea_add (double* res, double* m1, double* m2, unsigned size);

static double* setEaStarMatrix (double* eaMatrix, unsigned size)
{
  unsigned i;
  double* tmpMatrix     = (double*) newMatrix (size, size, sizeof (double));
  double* eaPowerMatrix = (double*) newMatrix (size, size, sizeof (double));
  double* eaStarMatrix  = (double*) newMatrix (size, size, sizeof (double));



  for (i = 0u; i < size; i++)
      eaStarMatrix [size* i + i] = 1.;
  memcpy (eaPowerMatrix, eaMatrix, size * size * sizeof (double));
  ea_add (eaStarMatrix, eaStarMatrix, eaPowerMatrix, size);
  
  for (i = 2u; i < size; i++)
    {
      ea_multiply (tmpMatrix, eaPowerMatrix, eaMatrix, size);
      memcpy (eaPowerMatrix, tmpMatrix, size * size * sizeof (double));
      ea_add (eaStarMatrix, eaStarMatrix, tmpMatrix, size);
    }



  Matrix_free (tmpMatrix);
  Matrix_free (eaPowerMatrix);



  return eaStarMatrix;
}



static void ea_multiply (double* res, double* m1, double* m2, unsigned size)
{
  unsigned i, j, k;

  for (i = 0u; i < size; i++)
    {
      for (j = 0u; j < size; j++)
	{
	  double* resf = res +size* i + j;
	  *resf = 0.;
	  for (k = 0u; k < size; k++)
	      *resf += m1 [size* i + k] * m2 [size* k + j];
	}
    }
}

static void ea_add (double* res, double* m1, double* m2, unsigned size)
{
  unsigned i, j;

  for (i = 0u; i < size; i++)
    for (j = 0u; j < size; j++)
      res [size* i + j] = m1 [size* i + j] + m2 [size* i + j];
}







static double setEaid (double* eaStarMatrix, unsigned size)
{
  unsigned i;
  double eaid = 0.;

  for (i = 0u; i < size; i++)
    eaid += eaStarMatrix [size* i + i];

  return eaid;
}
