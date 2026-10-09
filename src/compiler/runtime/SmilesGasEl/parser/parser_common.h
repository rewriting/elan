/********************************************
 *
 * Fichier : parser_common.h
 * Auteur  : Régis Durand
 *           regis.durand@esial.uhp-nancy.fr
 *
 * Description :
 * Définition des fonctions communes aux
 * parsers Smiles-GasEl.
 *
 ********************************************/



/*
 * Pour obtenir un token 'entier' ou 'nilL', étant donné
 * qu'on retourne le code ASCII (< 256) du caractère pour
 * le reste des tokens, j'ai choisi de retourner 256 (soit 0x100)
 * pour 'nilL', et la valeur de l'entier parsé + 257 pour
 * entier (+ 0x101).
 *
 */


int       nextToken        (char **smiles);

#define   endParsing(t)    ((t) == '\0')
#define   anInteger(t)     ((t) >= 0x101)
#define   aOpBracket(t)    ((t) == '(')
#define   aClBracket(t)    ((t) == ')')
#define   aFreeElectron(t) ((t) == 'e')
#define   aNilL(t)         ((t) == 0x100)

#define   toInteger(t)     ((unsigned)(t) - 0x101u)
