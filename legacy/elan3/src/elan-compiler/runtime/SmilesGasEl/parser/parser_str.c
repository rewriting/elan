/********************************************
 *
 * Fichier : parser_str.c
 * Auteur  : Régis Durand
 *           regis.durand@esial.uhp-nancy.fr
 *
 * Description :
 * Implantation du parser SMILES version
 * GasEl générant du SMILES.
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
 * Pour obtenir un token 'entier' ou 'nilL', étant donné
 * qu'on retourne le code ASCII (< 256) du caractère pour
 * le reste des tokens, j'ai choisi de retourner 256 (soit 0x100)
 * pour 'nilL', et la valeur de l'entier parsé + 257 pour
 * entier (+ 0x101).
 *
 */



#include <stdio.h>
#include <string.h>
#include "parser_common.h"
#include "parser_str.h"


static void cleanParenthesis (char *str);

/* Génère une chaine SMILES à partir d'une chaine SMILES à la GasEl */
int smilesGasEl2smiles (char *smiles, char *smilesGasEl)
{

  char *tmp = smiles;

  /* On lit le premier token */
  int curToken = nextToken (&smilesGasEl);

  /* On doit avoir un radical... */
  if (! checkRadicalStr (&tmp, &curToken, &smilesGasEl))
    return 0;
  *tmp = '\0';

  /* ...et seulement un radical */
  if (! endParsing (curToken))
    return 0;

  /*
   * Comme on supprime les nilL, on peut avoir "()"
   * à certains endroits de la chaine...
   * On les supprime.
   *
   */
  cleanParenthesis (smiles);
  return 1;
}


static void cleanParenthesis (char *str)
{
  for (; *str; str++) /* pour toute la chaine */
    if (*str == '(' && *(str+1) == ')') /* si on a () */
      strcpy (str, str+2); /* on écrase () */
}






/* RADICAL => e | ATOM RADICAL_LIST */
int checkRadicalStr (char **res, int *curToken, char **smiles)
{
  /* On a soit un 'e'... */
  if (aFreeElectron (*curToken))
    {
      *(*res)++ = 'F';
      *curToken = nextToken (smiles);
      return 1;
    }

  /* ...soit un ATOM et une RADICAL_LIST */
  if (! checkAtomStr (res, curToken, smiles))
    return 0;
  if (! checkRadicalListStr (res, curToken, smiles))
    return 0;

  return 1;
}





/* RADICAL_LIST => /\ | nilL | RADICAL | LINK RADICAL | ( RADICAL_LIST ) RADICAL_LIST */
int checkRadicalListStr (char **res, int *curToken, char **smiles)
{
  /* On a soit un 'nilL'... */
  if (aNilL (*curToken))
    {
      *curToken = nextToken (smiles);
      return 1;
    }

  /* ...soit un LINK et un RADICAL... */
  if (checkLinkStr (res, curToken, smiles))
    return checkRadicalStr (res, curToken, smiles);

  /* ...soit une '(', une RADICAL_LIST, une ')', et une RADICAL_LIST... */
  if (aOpBracket (*curToken))
    {
      *(*res)++ = '(';
      *curToken = nextToken (smiles);

      if (! checkRadicalListStr (res, curToken, smiles))
	return 0;

      if (! aClBracket (*curToken))
	return 0;

      *(*res)++ = ')';
      *curToken = nextToken (smiles);

      return checkRadicalListStr (res, curToken, smiles);
    }

  /* ...soit un RADICAL... */
  checkRadicalStr (res, curToken, smiles);

  /* ...soit le mot vide */
  return 1;
}






/* ATOM => SYMBOL INT_LIST */
int checkAtomStr (char **res, int *curToken, char **smiles)
{
  /* On a un SYMBOL... */
  if (! checkSymbolStr (res, curToken, smiles))
    return 0;

  /* ...et une INT_LIST */
  if (! checkIntListStr (res, curToken, smiles))
    return 0;

  return 1;
}






/* INT_LIST => /\ | integer INT_LIST */
int checkIntListStr (char **res, int *curToken, char **smiles)
{
  /* On a soit un entier et une INT_LIST... */
  if (anInteger (*curToken))
    {
      int nb = toInteger (*curToken);
      if (nb < 10u)
	nb = sprintf (*res, "%u", nb);
      else
	nb = sprintf (*res, "%%%u", nb);

      *res += nb;
      *curToken = nextToken (smiles);
      return checkIntListStr (res, curToken, smiles);
    }

  /* ...soit le mot vide */
  return 1;
}





/* SYMBOL => C | c | O | o | H  */
int checkSymbolStr (char **res, int *curToken, char **smiles)
{
  switch (*curToken)
    {
    case 'H':
    case 'C':
    case 'c':
    case 'O':
    case 'o':
      *(*res)++ = (char)(*curToken);
      *curToken = nextToken (smiles);
      return 1;

    default:
      return 0;
    }

  return 1;
}






/* LINK => - | = | # | : */
int checkLinkStr (char **res, int *curToken, char **smiles)
{
  switch (*curToken)
    {
    case '-':
    case '=':
    case '#':
    case ':':
      *(*res)++ = (char)(*curToken);
      *curToken = nextToken (smiles);
      return 1;

    default:
      return 0;
    }

  return 1;
}
