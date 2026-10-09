/********************************************
 *
 * Fichier : parser_mol.h
 * Auteur  : Régis Durand
 *           regis.durand@esial.uhp-nancy.fr
 * Date    : 14 aout 2003
 *
 * Description :
 * Définition du parser SMILES GasEl générant
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



#ifndef __PARSER_MOL_H__
#define __PARSER_MOL_H__


#include "../datatypes/molecule.h"


/* Génère une molécule à partir d'une chaine SMILES */
Molecule* smilesGasEl2molecule (char *smiles);

int       checkRadicalMol     (Molecule *mol, int *curToken, char **smiles, Atom**);
int       checkRadicalListMol (Molecule *mol, int *curToken, char **smiles, float curLink, AtomList**);
int       checkAtomMol        (Molecule *mol, int *curToken, char **smiles, Atom**, float*);
int       checkIntListMol     (Molecule *mol, int *curToken, char **smiles, IntList**);
int       checkSymbolMol      (Molecule *mol, int *curToken, char **smiles, char*, float*);
int       checkLinkMol        (Molecule *mol, int *curToken, char **smiles, float*);


#endif /* __PARSER_MOL_H__ */
