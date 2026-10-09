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

#ifdef NOTMACRO
extern void BG_delete(BG *bg)
{
  int i;
  Verif_void(bg,"BG_delete(bg)");
  Verif_void(bg->bs_tab,"BG_delete(bg->bs_tab)");
  //Verif_void(bg->link_tab,"BG_delete(bg->link_tab)");
  if(bg->link_tab != NULL)
    {
      for(i=0 ; i<bg->size ; i++)
	{
	  bitSet_delete(bg->bs_tab[i]);
	  if(bg->link_tab[i]!=NULL)
	    LINK_delete(bg->link_tab[i]);
	}
      IFREE(bg->bs_tab);
      IFREE(bg->link_tab);
    }
  else
    {
      for(i=0 ; i<bg->size ; i++) {
	bitSet_delete(bg->bs_tab[i]);
      }
      IFREE(bg->bs_tab);
    }
  IFREE(bg);
}

void BG_set(BG *bg, int no_pattern, bitSet *bs)
{
  Verif_void(bg,"BG_set(bg)");
  Verif_void(bs,"BG_set(bs)");
#ifdef DEBUG
  if(no_pattern<0 || no_pattern> BG_size(bg))
    {
      fprintf(stderr,"BG_set error : no_pattern<0 || no_pattern>size\n");
      assert(0);
    }
#endif
  bg->bs_tab[no_pattern]=bs;
}

bitSet *BG_get(BG *bg, int no_pattern)
{
  Verif_void(bg,"BG_get(bg)");
#ifdef DEBUG
  if(no_pattern<0 || no_pattern> BG_size(bg))
    {
      fprintf(stderr,"BG_get error : no_pattern<0 || no_pattern>size\n");
      fprintf(stderr,"size=%d\tbit=%d\n",BG_size(bg),no_pattern);
      assert(0);
    }
#endif
  return bg->bs_tab[no_pattern];
}

void BG_clear(BG *bg, int no_pattern)
{
  Verif_void(bg,"BG_clear(bg)");
#ifdef DEBUG
  if(no_pattern<0 || no_pattern> BG_size(bg))
    {
      fprintf(stderr,"BG_clear error : no_pattern<0 || no_pattern>size\n");
      assert(0);
    }
#endif
  bg->bs_tab[no_pattern]=0;
}
#endif

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

#ifdef NOTMACRO
int BG_size(BG *bg)
{
  Verif_void(bg,"BG_size(bg)");
  return bg->size;
}


void BG_set_size(BG *bg, int size)
{
  Verif_void(bg,"BG_set_size(bg)");
  bg->size=size;
}
#endif

/*
 * returns 0 if there is no solution
 */
int BG_cbg2bg(int *liste_pattern, BG *cbg, BG *bg) {
  int i;
  int j,find;
  bitSet *mask;
  for(i=0 ; i<BG_size(bg) ; i++) {
      /*
       * ici, on copie un champ de bits
       */
      //printf("liste_pattern[%d] = %d\n",i,liste_pattern[i]);
#ifdef BGSHARE
    BG_set(bg,i,BG_get(cbg,liste_pattern[i]));
#else
    BG_set(bg,i,bitSet_copy(BG_get(cbg,liste_pattern[i])));
#endif

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

#ifdef NOTMACRO
int BG_link_size(BG *bg)
{
  Verif_void(bg,"BG_link_size(bg)");
  return bg->size;
}

void BG_link_set(BG *bg, int no_pattern, LINK *link)
{
  Verif_void(bg,"BG_link_set(bg)");
  Verif_void(link,"BG_link_set(link)");
#ifdef DEBUG
  if(no_pattern<0 || no_pattern> BG_link_size(bg))
    {
      fprintf(stderr,"BG_link_set error : no_pattern<0 || no_pattern>size\n");
      assert(0);
    }
#endif
  bg->link_tab[no_pattern]=link;
}


LINK *BG_link_get(BG *bg, int no_pattern)
{
  Verif_void(bg,"BG_link_get(bg)");
  //Verif_void(bg->link_tab,"BG_link_get(bg->link_tab)");
#ifdef DEBUG
  if(no_pattern<0 || no_pattern> BG_link_size(bg))
    {
      fprintf(stderr,"BG_link_get error : no_pattern<0 || no_pattern>size\n");
      assert(0);
    }
#endif
  if(bg->link_tab == NULL)
    return NULL;
  else
    return bg->link_tab[no_pattern];
}
#endif
