/********************************************
 *
 * Fichier : usmiles.c
 * Auteur  : Régis Durand
 *           regis.durand@esial.uhp-nancy.fr
 *
 * Description :
 * Implantation de unique smiles version GasEl.
 *
 ********************************************/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../parser/parser_str.h"
#include "usmiles.h"



int USmilesGasElCompare (char *s1, char *s2)
{
  int result;
  char *res1 = (char*) malloc (strlen (s1) * 2u);
  char *res2 = (char*) malloc (strlen (s2) * 2u);

  /* si les 2 chaines sont valides */
  if (USmilesGasEl (res1, s1) && USmilesGasEl (res2, s2))
    /* les molécules sont égales si res1 et res2 correspondent */
    result = (! strcmp (res1, res2));
  else
    /*
     * Sinon on considère qu'elles sont différentes.
     * Ainsi si le parser ne prend pas en charge ces
     * chaines, et on ne gene pas le fonctionnement du
     * reste du programme.
     */
    result = 0;

  free (res1);
  free (res2);

  return result;
}



int USmilesGasEl (char *res, char *smilesGasEl)
{
  FILE *tmpfile;
  const char *progname = "./aauniq \"";
  const char *target = "\" > tmpfile";
  char *cmdline;

  /* Si on arrive pas à parser la chaine SMILES GasEl */
  if (! smilesGasEl2smiles (res, smilesGasEl))
    /* on arrete là */
    return 0;

  /* on crée la ligne de commande './aauniq <smiles> > tmpfile' */
  cmdline = (char*) malloc (strlen (progname) + strlen (res) + strlen (target) + 1u);

  strcpy (cmdline, progname);
  strcat (cmdline, res);
  strcat (cmdline, target);

  /* on exécute */
  system (cmdline);

  free (cmdline);

  /*
   * Et on va chercher dans tmpfile la chaine
   * résultat de aauniq.
   */
  tmpfile = fopen ("tmpfile","rt");
  fscanf (tmpfile, "%s", res);
  fclose (tmpfile);

  /*
   * Si il y a eu une erreur, le résultat
   * présent dans res est SMILES.
   * Ainsi, on sait que tout s'est bien
   * passé si res est différent de SMILES!
   */
  return (strcmp (res, "SMILES"));
}
