/********************************************
 *
 * Fichier : molecule.h
 * Auteur  : Régis Durand
 *           regis.durand@esial.uhp-nancy.fr
 *
 * Description :
 * Définition du type Molecule.
 *
 ********************************************/


#ifndef __MOLECULE_H__
#define __MOLECULE_H__


#include "datatypes.h"


typedef struct _AtomList AtomList;


/* Index des tableaux radiis et cvs */
/* Servent aussi de symboles de reconnaissance des atomes */
#define SYMBOL_C     0
#define SYMBOL_O     1
#define SYMBOL_H     2
#define SYMBOL_e     3

float* radiis (); /* radiis des atomes C, O, H, e */
float* cvs ();    /* connectivités des atomes C, O, H, e */


/* prévu pour le coloriage de graphe (Atom.color) */
/* ou le marquae des atomes */
#define VISITED      1
#define NOT_VISITED  0





typedef struct _Atom
{
  char symbol;
  char color;
  unsigned number;
  AtomList *atoms;
  IntList *marks;
} Atom;



struct _AtomList
{
  Atom *atom;
  float link;
  AtomList *next;
};



typedef struct _Molecule
{
  Atom **atoms;
  unsigned size;
  unsigned count;
} Molecule;


Molecule* newMolecule          (unsigned maxSize);
void      Molecule_free        (Molecule *mol);

Atom*     Molecule_addAtom     (Molecule *mol, char symbol, IntList *marks);


Atom*     Atom_addLinks        (Atom *atom, AtomList *atoms);



AtomList* newAtomList          (Atom *atom, float link);


#endif /* __MOLECULE_H__ */
