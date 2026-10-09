/********************************************
 *
 * Fichier : parser_str.h
 * Auteur  : Régis Durand
 *           regis.durand@esial.uhp-nancy.fr
 *
 * Description :
 * Définition du parser SMILES version
 * GasEl générant du SMILES.
 *
 ********************************************/


#ifndef __PARSER_STR_H__
#define __PARSER_STR_H__

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
 * Pour obtenir un token 'entier' ou 'nilL', étant donné
 * qu'on retourne le code ASCII (< 256) du caractère pour
 * le reste des tokens, j'ai choisi de retourner 256 (soit 0x100)
 * pour 'nilL', et la valeur de l'entier parsé + 257 pour
 * entier (+ 0x101).
 *
 */

int smilesGasEl2smiles (char *smiles, char *smilesGasEl);

int       checkRadicalStr     (char **res, int *curToken, char **smiles);
int       checkRadicalListStr (char **res, int *curToken, char **smiles);
int       checkAtomStr        (char **res, int *curToken, char **smiles);
int       checkIntListStr     (char **res, int *curToken, char **smiles);
int       checkSymbolStr      (char **res, int *curToken, char **smiles);
int       checkLinkStr        (char **res, int *curToken, char **smiles);




#endif /* __PARSER_STR_H__ */
