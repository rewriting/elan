/*
  
    REM - Reduce ELAN Machine

    Copyright (C) 2000-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
			     Nancy, France.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr

*/
#include "bgraph.h"
#include "bitset.h"
#include "tools.h"

BG *BG_create(int size, int necessary_link)
{
  BG *res;
  res=(BG*) IMALLOC(sizeof(BG));
  res->size=size;
  res->bs_tab=(bitSet**) IMALLOC(size*sizeof(bitSet*));
  if(necessary_link != 0) {
    int i;
    res->link_tab=(LINK**) IMALLOC(size*sizeof(LINK*));
    for(i=0 ; i<size ; i++) {
      res->bs_tab[i]=NULL;
      res->link_tab[i]=NULL;
    }
  } else {
    res->link_tab=(LINK**) NULL;
  }
  return res;
}


void BG_print(BG *bg)
{
  int i;
  Verif_void(bg,"BG_print(bg)");

  //printf("BG_size=%d\n",BG_size(bg));
  for(i=0 ; i<BG_size(bg) ; i++)
    {
      bitSet_print(BG_get(bg,i));
      printf(" . ");
    }
  printf("\n");
  printf("\n  link=");
  for(i=0 ; i<BG_size(bg) ; i++)
    {
      printf("[ ");
      LINK_print(BG_link_get(bg,i));
      printf(" ] . ");
    }
}


/*
 * returns 0 if there is no solution
 */
int BG_cbg2bg(int *liste_pattern, BG *cbg, BG *bg) {
  int i;
  bitSet *mask;
  for(i=0 ; i<BG_size(bg) ; i++) {
      /*
       * ici, on copie un champ de bits
       */
      //printf("liste_pattern[%d] = %d\n",i,liste_pattern[i]);
    BG_set(bg,i,BG_get(cbg,liste_pattern[i]));

/*
 * [pem: Apr 23 99]
 * verification supplementaire : il doit y avoir
 * au moins une arete par sommet
 */
    mask = BG_get(cbg,liste_pattern[i]);
    if(bitSet_isnull(mask)) {
        //printf("BG number %d has no solution\n",liste_pattern[i]);
      return 0;
    }
    
    
  }
  return 1;
}

/*
 * sol : derniere solution trouvee ou {-1,...,-1} pour initialiser
 * indice : derniere valeur de i retournee (taille du bg-1)
 *          ou 0 pour initialiser
 * retourne -1 lorsqu'il n'y a plus de solution
 */
int BG_solve_one(BG *bg, int choice[],multiplicityType multiplicity[], 
		 int maxChoice, int indice) {
  bitSet *mask;
  //int maxChoice = bitSet_size(BG_get(bg,0));
  int BGSize = BG_size(bg);

  /*
  for(i=0 ; i<BGSize ; i++) {
    int find=0;
    for(j=0 ; !find && j<maxChoice; j++) {
      find |= bitSet_get(BG_get(bg,i),j);
    }
    if(!find) {
      return -1;
    }
  }
  */

  /*
   * chaque case de choice[] contient le numero du choix
   * ie : le ieme bit du mask associe au pattern
   */
  while(indice>=0) {
    if( indice>=BGSize ) {
        /*
          printf("Solution : ");
          for(j=0 ; j<BG_size(bg) ; j++)
          printf("%d ",choice[j]);
          printf("\n");
        */
      indice--;
      return indice;
    } else {
      mask=BG_get(bg,indice);
      if(choice[indice]>=0)
        multiplicity[choice[indice]]++;

      choice[indice]++;

      while( (choice[indice] <  maxChoice ) &&
             (multiplicity[choice[indice]]<=0 ||
              !bitSet_get(mask,choice[indice])) ) {
        choice[indice]++;
      }
      
        /*
	  printf("choice=");
	  for(j=0;j<BG_size(bg);j++)
          printf("[%d] ",choice[j]);
	  printf("\n");
	  printf("mult=");
	  for(j=0;j<bitSet_size(mask);j++)
          printf(" %d  ",multiplicity[j]);
	  printf("\n");
        */
	  
      if(choice[indice] <  maxChoice) {
        multiplicity[choice[indice]]--;
        indice++;
      } else {
        choice[indice] = -1;
        indice--;
      }
    }
  }
  return indice;
}

/*
 * sol : derniere solution trouvee ou {-1,...,-1} pour initialiser
 * indice : derniere valeur de i retournee (taille du bg-1)
 *          ou 0 pour initialiser
 * retourne -1 lorsqu'il n'y a plus de solution
 */
int BG_greedy_solve_one(BG *bg, int choice[], int multiplicity[],
		 int maxChoice, int indice)
{
  bitSet *mask;
  int BGSize = BG_size(bg);

  /*
   * chaque case de choice[] contient le numero du choix
   * ie : le ieme bit du mask associe au pattern
   */
  while(indice>=0) {
    if( indice>=BGSize ) {
      indice--;
      return indice;
    } else {
      mask=BG_get(bg,indice);
      if(choice[indice]>=0) {
	multiplicity[choice[indice]]++;
      }
      choice[indice]++;
      while( (choice[indice] <  maxChoice ) &&
	     (multiplicity[choice[indice]]<=0 ||
	      !bitSet_get(mask,choice[indice])) ) {
	choice[indice]++;
      }
      if(choice[indice] <  maxChoice) {
	multiplicity[choice[indice]]--;
	indice++;
      } else {
	choice[indice] = -1;
	indice--;
      }
    }
  } 
  return indice;
}

