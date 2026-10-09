/********************************************
 *
 * Fichier : parser_common.c
 * Auteur  : Régis Durand
 *           regis.durand@esial.uhp-nancy.fr
 *
 * Description :
 * Implantation des fonctions communes aux
 * parsers Smiles-GasEl.
 *
 ********************************************/



#include <ctype.h>
#include "parser_common.h"



/*
 * Pour obtenir un token 'entier' ou 'nilL', étant donné
 * qu'on retourne le code ASCII (< 256) du caractère pour
 * le reste des tokens, j'ai choisi de retourner 256 (soit 0x100)
 * pour 'nilL', et la valeur de l'entier parsé + 257 pour
 * entier (+ 0x101).
 *
 */


int nextToken (char **smiles)
{
  int val;
  char *tmp;

  while (isspace (* (*smiles)))
    (*smiles) ++;

  switch (* (*smiles))
    {
    case '\0':
      return * (*smiles);

    case '(':
    case ')':
    case '-':
    case '=':
    case '#':
    case ':':
    case '%':
    case 'H':
    case 'C':
    case 'c':
    case 'O':
    case 'o':
    case 'e':
      return * (*smiles)++;

    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      val = * (*smiles) - '0';
      while (isdigit (* (++ *smiles)))
	val = 10 * val + * (*smiles) - '0';
      return (val + 0x101);

    case 'n':
      tmp = *smiles;
      if (* ++tmp == 'i')
	if (* ++tmp == 'l')
	  if (* ++tmp == 'L')
	    {
	      *smiles = ++tmp;
	      return 0x100;
	    }
      return -1;

    default:
      return -1;
    }
}
