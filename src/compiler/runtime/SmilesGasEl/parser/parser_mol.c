/********************************************
 *
 * Fichier : parser_mol.c
 * Auteur  : Régis Durand
 *           regis.durand@esial.uhp-nancy.fr
 *
 * Description :
 * Implantation du parser SMILES GasEl générant
 * un objet Molecule.
 *
 ********************************************/



/*
 * GRAMMAIRE SMILES GASEL :
 *
 * radical : 'e'
 *         | atom radical_list
 *
 * radical_list : /\
 *              | 'nilL'
 *              | radical
 *              | link radical
 *              | '(' radical_list ')' radical_list
 *
 * atom : symbol int_list
 *
 * int_list : /\
 *          | 'entier' int_list
 *
 * symbol : 'C'
 *        | 'c'
 *        | 'O'
 *        | 'o'
 *        | 'H'
 *        | 'e'
 *
 * link : '-'
 *      | '='
 *      | '#'
 *      | ':'
 *
 */


/*
 * On construit la molécule au fur et à mesure
 * de l'analyse, et on retourne également les
 * informations tirées de la grammaire.
 *
 */



#include <stdio.h>
#include <string.h>
#include "parser_common.h"
#include "parser_mol.h"



/* Génère une molécule à partir d'une chaine SMILES */
Molecule* smilesGasEl2molecule (char *smiles)
{
  /* On ne peut pas avoir plus d'atomes que de caractères dans la chaine */
  Molecule *mol = newMolecule (strlen (smiles));

  /* On lit le premier token */
  int curToken = nextToken (&smiles);

  /* On doit avoir un radical... */
  Atom *atom = NULL;
  if (! checkRadicalMol (mol, &curToken, &smiles, &atom))
    {
      Molecule_free (mol);
      return 0;
    }

  /* ...et seulement un radical */
  if (! endParsing (curToken))
    {
      Molecule_free (mol);
      return 0;
    }

  return mol;
}







/* RADICAL => e | ATOM RADICAL_LIST */
int checkRadicalMol (Molecule *mol, int *curToken, char **smiles, Atom **atom)
{
  float link;
  AtomList *linkedAtoms = NULL;

  /* On a soit un 'e'... */
  if (aFreeElectron (*curToken))
    {
      /* on ajoute cet électron à la molécule */
      *atom = Molecule_addAtom (mol, SYMBOL_e, NULL);
      *curToken = nextToken (smiles);
      return 1;
    }

  /* ...soit un ATOM et une RADICAL_LIST */
  if (! checkAtomMol (mol, curToken, smiles, atom, &link))
    return 0;
  if (! checkRadicalListMol (mol, curToken, smiles, link, &linkedAtoms))
    return 0;


  /* Si RADICAL_LIST n'est pas vide... */
  if (linkedAtoms)
    /* ...on relie l'atome avec ses voisins */
    Atom_addLinks (*atom, linkedAtoms);

  return 1;
}





/* RADICAL_LIST => /\ | nilL | RADICAL | LINK RADICAL | ( RADICAL_LIST ) RADICAL_LIST */
int checkRadicalListMol (Molecule *mol, int *curToken, char **smiles, float curLink, AtomList **atomList)
{
  Atom *atom = NULL;
  float link;

  /* On a soit un 'nilL'... */
  if (aNilL (*curToken))
    {
      /* on ne fait rien */
      *curToken = nextToken (smiles);
      return 1;
    }

  /* ...soit un LINK et un RADICAL... */
  if (checkLinkMol (mol, curToken, smiles, &link))
    {
      if (! checkRadicalMol (mol, curToken, smiles, &atom))
	return 0;
      /* on construit une nouvelle liste de liens */
      *atomList = newAtomList (atom, link);
      return 1;
    }

  /* ...soit une '(', une RADICAL_LIST, une ')', et une RADICAL_LIST... */
  if (aOpBracket (*curToken))
    {
      *curToken = nextToken (smiles);

      if (! checkRadicalListMol (mol, curToken, smiles, curLink, atomList))
	return 0;

      if (! aClBracket (*curToken))
	return 0;
      *curToken = nextToken (smiles);

      /* si une liste de lien existe déjà */
      if (*atomList)
	/* on continue de chercher à la suite de la liste */
	return checkRadicalListMol (mol, curToken, smiles, curLink, & ((*atomList)->next));
      else
	/* sinon on va chercher au meme emplacement */
	return checkRadicalListMol (mol, curToken, smiles, curLink, atomList);
    }

  /* ...soit un RADICAL... */
  if (checkRadicalMol (mol, curToken, smiles, &atom))
    /* on ajoute un nouveau lien */
    *atomList = newAtomList (atom, curLink);

  /* ...soit le mot vide */
  return 1;
}






/* ATOM => SYMBOL INT_LIST */
int checkAtomMol (Molecule *mol, int *curToken, char **smiles, Atom **atom, float *link)
{
  unsigned i;
  char symbol;
  IntList *intList = NULL, *tmpList;

  /* On a un SYMBOL... */
  if (! checkSymbolMol (mol, curToken, smiles, &symbol, link))
    return 0;

  /* ...et une INT_LIST */
  if (! checkIntListMol (mol, curToken, smiles, &intList))
    return 0;

  /* on ajoute l'atome à la molécule */
  *atom = Molecule_addAtom (mol, symbol, intList);

  /* 
   * Pour chaque élément de la liste d'entiers, on cherche
   * s'il n'existe pas le meme entier autre part : liaison coupée.
   * Exemple : C1 C C C C C1 => on va relier les deux atomes C1
   */ 
  for (; intList; intList = intList->next)
    {
      /* pour tous les atomes excepté celui que l'on vient d'ajouter */
      for (i = 0u; i < mol->count - 1u; i++)
	{
	  /* on regarde sa liste d'entiers */
	  for (tmpList = mol->atoms[i]->marks; tmpList; tmpList = tmpList->next)
	    {
	      /* si on en trouve 1 identique */
	      if(intList->val == tmpList->val)
		{
		  /* on ajoute le lien et on sort */
		  Atom_addLinks (*atom, newAtomList (mol->atoms[i], *link));
		  break;
		}
	    }

	  /* si on a trouvé = boucle inachevée = (tmpList != NULL) */
	  if (tmpList)
	    break; /* on quitte ce niveau également */
	}
    }


  return 1;
}






/* INT_LIST => /\ | integer INT_LIST */
int checkIntListMol (Molecule *mol, int *curToken, char **smiles, IntList **intList)
{
  /* On a soit un entier et une INT_LIST... */
  if (anInteger (*curToken))
    {
      /* on ajoute cet entier à la liste */
      *intList = newIntList (toInteger (*curToken));
      *curToken = nextToken (smiles);
      /* on continue avec le suivant */
      return checkIntListMol (mol, curToken, smiles, & ((*intList)->next));
    }

  /* ...soit le mot vide */
  return 1;
}





/* SYMBOL => C | c | O | o | H  */
int checkSymbolMol (Molecule *mol, int *curToken, char **smiles, char *symbol, float *link)
{
  (void)mol;
  switch (*curToken)
    {
    case 'H':
      *symbol = SYMBOL_H;
      *link = 1.f;
      break;
    case 'C':
      *symbol = SYMBOL_C;
      *link = 1.f;
      break;
    case 'c':
      *symbol = SYMBOL_C;
      *link = 1.5f;
      break;
    case 'O':
      *symbol = SYMBOL_O;
      *link = 1.f;
      break;
    case 'o':
      *symbol = SYMBOL_O;
      *link = 1.5f;
      break;

    default:
      return 0;
    }

  *curToken = nextToken (smiles);
  return 1;
}






/* LINK => - | = | # | : */
int checkLinkMol (Molecule *mol, int *curToken, char **smiles, float *link)
{
  (void)mol;
  switch (*curToken)
    {
    case '-':
      *link = 1.f;
      break;
    case '=':
      *link = 2.f;
      break;
    case '#':
      *link = 3.f;
      break;
    case ':':
      *link = 1.5f;
      break;

    default:
      return 0;
    }

  *curToken = nextToken (smiles);
  return 1;
}
