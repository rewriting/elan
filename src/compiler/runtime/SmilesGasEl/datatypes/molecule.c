/********************************************
 *
 * Fichier : molecule.c
 * Auteur  : Régis Durand
 *           regis.durand@esial.uhp-nancy.fr
 *
 * Description :
 * Implantation du type Molecule.
 *
 ********************************************/


#include <stdio.h>
#include <stdlib.h>
#include "molecule.h"

/* Tableau des radiis pour C O H e */
float* radiis ()
{
  static float table[] = {.771f, .74f, .37f, .72f};
  return table;
}

/* Tableau des connectivités pour C O H e */
float* cvs ()
{
  static float table[] = {0.f, 4.f, 0.f, 6.f};
  return table;
}



Molecule* newMolecule (unsigned maxSize)
{
  Molecule *res = (Molecule*) malloc (sizeof (Molecule));
  res->atoms = (Atom**) newTable (maxSize, sizeof (Atom*));
  res->size  = maxSize;
  res->count = 0;
  return res;
}


static void Atom_free (Atom *atom);

void Molecule_free (Molecule *mol)
{
  unsigned i;

  for (i = 0u; i < mol->count; i++)
    if (mol->atoms[i])
      Atom_free (mol->atoms [i]);
  Table_free (mol->atoms);
  free (mol);
}


static void AtomList_free (AtomList *atomList);

static void Atom_free (Atom *atom)
{
  if (atom->marks)
    IntList_free (atom->marks);
  if (atom->atoms)
    AtomList_free (atom->atoms);
  free (atom);
}

static void AtomList_free (AtomList *atomList)
{
  if (atomList->next)
    AtomList_free (atomList->next);
  free (atomList);
}




Atom* Molecule_addAtom (Molecule *mol, char symbol, IntList *marks)
{
  Atom *res = (Atom*) malloc (sizeof (Atom));
  res->symbol = symbol;
  res->color  = NOT_VISITED;
  res->number = mol->count;
  res->marks  = marks;
  res->atoms  = NULL;
  return (mol->atoms [mol->count ++] = res);
}

Atom* Atom_addLinks (Atom *atom, AtomList *atoms)
{
  AtomList *tmpList;

  if (! atom->atoms)     /* si atom n'a encore aucun lien */
    atom->atoms = atoms; /* ses liens deviennent atoms */
  else
    { /* sinon on se place à la fin des liens existants */
      for (tmpList = atom->atoms; tmpList->next; tmpList = tmpList->next)
	;
      tmpList->next = atoms; /* et on ajoute les nouveaux liens */
    }

  /* on crée les liens B->A pour chaque lien A->B enregistré */
  while (atoms) /* pour chaque atome de la liste */
    {
      /* on détermine l'atome courant */
      Atom *linkedAtom = atoms->atom;
      /* on sauvegarde sa liste de liens*/
      tmpList = linkedAtom->atoms;
      /* on enregistre le lien B->A */
      linkedAtom->atoms = newAtomList (atom, atoms->link);
      /* et on colle à la suite les liens sauvegardés */
      linkedAtom->atoms->next = tmpList;
      /* puis on change d'atome B */
      atoms = atoms->next;
    }

  return atom;
}



AtomList* newAtomList (Atom *atom, float link)
{
  AtomList *res = (AtomList*) malloc (sizeof (AtomList));
  res->atom = atom;
  res->link = link;
  res->next = NULL;
  return res;
}
