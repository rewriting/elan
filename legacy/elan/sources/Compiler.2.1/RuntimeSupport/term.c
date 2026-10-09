#include "term.h"
#include "tools.h"
#include "builtin.h"
#include "back.h"

static void print_f_prefix(FILE *fich,struct term *t);
static struct cell_term *term_merge_sort(struct cell_term *p, int len);
static struct cell_term *merge_sorted_list(struct cell_term *p1,
					   struct cell_term *p2,
					   struct cell_term **ptr_last);
static struct cell_term *merge_shared_sorted_list(struct cell_term *p1,
						  struct cell_term *p2,
						  struct cell_term **ptr_last);
static struct cell_term *copy_list(struct cell_term *p, struct cell_term **ptr_last);
static void find_position(struct cell_term *list, struct term *elt,
			  struct cell_term **ptr_first,
			  struct cell_term **ptr_second);


void term_alloc(struct term **ptr_dest, int size_sname, unsigned int funsym) {
  struct term *dest;
  *ptr_dest=(struct term*) MALLOC(size_sname);
  dest=*ptr_dest;
  setSymb(dest,funsym);
  clearShared(dest);
#ifdef DEBUG
  cptTermAlloc++;
#endif
  if(symb_isAC(funsym)) {
    setAC(dest);
    term_first(dest)=(struct cell_term*)NULL;
    term_last(dest)=(struct cell_term*)NULL;
  }
}

struct term *term_build(int nbArg, int code, ...) {
  va_list    argv;
  struct term *res;
  int i;
  va_start(argv, code);
  TERM_ARITY_ALLOC(res,nbArg,code);
  for(i=0 ; i<nbArg ; i++) {
    res->sub[i] = (struct term *) va_arg(argv, struct term *);
  }
  va_end(argv);
  return res;
}

void fsym_init(int code, int a, char *n,
	       int sem, int dstrat,
	       struct term* (*semaction)(struct term *)) {
  fsymtab[code].arity=a;
  //strdup(fsymtab[code].name,n);
  fsymtab[code].name=n;
  fsymtab[code].semantic=sem;
  fsymtab[code].defstrat=dstrat;
  fsymtab[code].semact=semaction;
}



struct cell_term *cell_create()
{
  struct cell_term *cell;
  cell=(struct cell_term*) MALLOC(sizeof(struct cell_term));
#ifdef DEBUG
  setMult(cell,0);
#endif  
  setColor(cell,bicolor);
  cell_next(cell)=NULL;
  return cell;
}

#ifdef NOTMACRO
void cell_free(struct cell_term *cell)
{
  FREE(cell);
}

void cell_add_last(struct cell_term *cell, struct term *t) 
{
  if(term_first(t)==NULL) {
    term_first(t)=term_last(t)=cell;
  } else { 
    cell_next(term_last(t))=cell;
    term_last(t)=cell;
  }
}

void cell_delete(struct cell_term *last_cell,
		 struct cell_term *cell,
		 struct term *t) {
  if(cell==term_first(t)) {
    if(cell==term_last(t)) {
      term_first(t)=term_last(t)=NULL;
    } else {
      term_first(t)=cell_next(cell);
    }
  } else if(cell==term_last(t)) {
    cell_next(last_cell)=NULL;
    term_last(t)=last_cell;
  } else {
    cell_next(last_cell)=cell_next(cell);
  }
}


#endif

struct term *term_add_first(struct term *t, struct term *subterm)
{
  struct cell_term *cell;
  CELL_ALLOC(cell);
  cell_t(cell)=subterm;
  setMult(cell,1);
  addcounter(subterm);

  if(term_first(t)==NULL) {
    term_first(t)=term_last(t)=cell;
    cell_next(cell)=NULL;
  } else {
    cell_next(cell)=term_first(t);
    term_first(t)=cell;
  }
  return t;
}

struct term *term_add_last(struct term *t, struct term *subterm)
{
  struct cell_term *cell;
  CELL_ALLOC(cell);
  CELL_INIT(cell);
  cell_t(cell)=subterm;
  setMult(cell,1);
  addcounter(subterm);

  if(term_first(t)==NULL) {
    term_first(t)=term_last(t)=cell;
  } else {
    cell_next(term_last(t))=cell;
    term_last(t)=cell;
  }
  return t;
}

#ifdef NOTMACRO
int term_isAC(struct term *t)
{
  Verif_void(t,"term_isAC(t)");
  /*
  if( term_arity(t) == -1 )
    return 1;
  else
    return 0;
    */
  return isAC(t);
}
#endif

/*
 * output.c
 */
void term_printnl(FILE *fich,struct term *t)
{
  Verif_void(t,"term_printnl(fich,t)");
  term_print(fich,t);
  fprintf(fich,"\n");
}

void term_print(FILE *fich,struct term *t)
{
  Verif_void(t,"term_print(fich,t)");
  print_f_prefix(fich,t);
}


static void print_f_prefix(FILE *fich,struct term *t)
{
  if(isIntegerTagged(t)) {
      //fprintf(fich,"integer\n");
    fprintf(fich,"%d",getInt(t));
    return;
  } else if(isIdentifierTagged(t)) {
      //fprintf(fich,"identifier\n");
      fprintf(fich,"ident(%d)",getIdentifier(t));
      //fprintf(fich,"%d",getIdentifier(t));
      return; 
  }
  // PROBLEME SUR SUN
  
    else if(isStringTagged(t)) {
    fprintf(fich,"string '%s' ",getString(t));
    //fprintf(fich,"%s",getString(t));
    return;
  }
  

  //fprintf(fich,"symbol : %d\t%d\n",t,getSymb(t));
  fprintf(fich,"%s",term_name(t));
  //fprintf(fich,"[%d]",t->counter);
  
  if(term_isAC(t)) {
    fprintf(fich,"*");
    if(term_first(t) != NULL) {
      struct cell_term *cell;
        //if(isShared(t)) fprintf(fich,"+");
        //if(isReduced(t)) fprintf(fich,"[r]");
      fprintf(fich, "(");
      for(cell=term_first(t) ; cell!=NULL ; cell=cell_next(cell)) {
	/*
	 * print with multiplicity
	 */
	if(getMult(cell)>1) {
	  int i;
	  fprintf(fich,"[");
	  for(i=0 ; i<getMult(cell) ; i++) {
	    term_print(fich, cell_t(cell));
	    if(i!=getMult(cell)-1) {
	      fprintf(fich,",");
	    }
	  }
	  fprintf(fich,"]");
	} else {
	  term_print(fich, cell_t(cell));
	}
#ifdef COLOR
	// fprintf(fich,"{%d}",getColor(cell));
#endif
	if(cell != term_last(t)) {
	  fprintf(fich,",");
	}
      }
      fprintf(fich,")");

      /*
       * check last
       */
      for(cell=term_first(t) ; cell_next(cell)!=NULL ; cell=cell_next(cell)) {
	/* do nothing */
      }
      if(cell!=term_last(t)) {
	fprintf(fich,"\nError\n");
	exit(1);
      }

    }
  } else {
    int i;
    int arity=term_arity(t);
    if(arity>0) {
      fprintf(fich, "(");
      for(i=0 ; i<arity ; i++) {
	term_print(fich, t->sub[i]);
	if(i!=arity-1)
	  fprintf(fich,",");
      }
      fprintf(fich,")");
    }
  }
}

/*
 * output.c
 */
void term_printREFln(FILE *fich,struct term *t)
{
  Verif_void(t,"term_printlnREF(fich,t)");
  term_printREF(fich,t);
  fprintf(fich,"\n");
}

void term_printREF(FILE *fich,struct term *t)
{
  int i;
  Verif_void(t,"term_printREF(fich,t)");
  if(isIntegerTagged(t)) {
    fprintf(fich,"INT(%d)",getInt(t));
    return;
  } else if(isIdentifierTagged(t)) {
    fprintf(fich,"IDENT(%d)",getIdentifier(t));
    return; 
  }
  // PROBLEME SUR SUN
  else if(isStringTagged(t)) {
    // A MODIFIER !!!
    fprintf(fich,"STRING(%s)",getString(t));
    return;
  }

  if(term_isAC(t)) {
    if(term_first(t) != NULL) {
      struct cell_term *cell;
      for(cell=term_first(t) ; cell!=NULL ; cell=cell_next(cell)) {
	for(i=0 ; i<getMult(cell) ; i++) {
          if( !((cell_next(cell)==NULL) && (i==getMult(cell)-1))) {
            fprintf(fich,"FSYM(");
          }
          term_printREF(fich, cell_t(cell));
          if( !((cell_next(cell)==NULL) && (i==getMult(cell)-1))) {
            fprintf(fich,".");
          }
        }
      }
      
      for(cell=term_first(t) ; cell!=NULL ; cell=cell_next(cell)) {
	for(i=0 ; i<getMult(cell) ; i++) {
          if( !((cell_next(cell)==NULL) && (i==getMult(cell)-1))) {
            fprintf(fich,".nil,%d)",getSymb(t));
          }
          
        }
      }
    } else {
      fprintf(stderr,"term_printREF: error in AC term\n");
      exit(1);
    }
  } else {
    int i;
    int arity=term_arity(t);
    fprintf(fich,"FSYM(");
    if(arity>0) {
      for(i=0 ; i<arity ; i++) {
	term_printREF(fich, t->sub[i]);
	fprintf(fich,".");
      }
    }
    fprintf(fich,"nil,");
    fprintf(fich,"%d)",getSymb(t));
  }
  
}

struct term *term_unflatten(struct term *t);
static struct term *cell_term_unflatten(unsigned int funsym, struct cell_term *cell, int multiplicity) 
{
  struct term *res;
  if(cell_next(cell)==NULL && multiplicity<=1) {
      /* si c'est le dernier element de la liste */
    return term_unflatten(cell_t(cell));
  } else {
    TERM_ALLOC(res,term2,funsym);
    res->sub[0] = term_unflatten(cell_t(cell));
    multiplicity--;
    if(multiplicity<=0) {
      cell=cell_next(cell);
      res->sub[1] = cell_term_unflatten(funsym,cell,getMult(cell));
    } else {
      res->sub[1] = cell_term_unflatten(funsym,cell,multiplicity);
    }
    return res;
  }
}

struct term *term_unflatten(struct term *t) {
  if(isTagged(t)) {
    return t;
  }
       
  if(term_isAC(t)) {
    struct cell_term *cell=term_first(t);
    return cell_term_unflatten(getSymb(t), cell, getMult(cell));
  } else {
    struct term *res = t;
    int arity=term_arity(t);
    int i;
    if(arity>0) {
      if(arity==1) {
        TERM_ALLOC(res,term1,getSymb(t));
      } if(arity==2) {
        TERM_ALLOC(res,term2,getSymb(t));
      } else {
        TERM_ARITY_ALLOC(res,arity,getSymb(t));
      }
      for(i=0 ; i<arity ; i++) {
        res->sub[i] = term_unflatten(t->sub[i]);
      }
      return res;
    } else {
      return t;
    }
  }
}
  

/*
 ************************************************************
 *
 * flatten
 *
 ************************************************************
 */

/*
 * t and subterm are two onf terms
 */

//#define AFFICHAGE

#ifdef COLOR
struct term *intern_term_add_onf_term(int isAC,
				      struct term *t,
				      struct term *subterm,
				      int color)
#else
struct term *intern_term_add_onf_term(int isAC,
				      struct term *t,
				      struct term *subterm)
#endif
{
  struct cell_term *first;
  struct cell_term *second;
  struct cell_term *cell;
  struct cell_term *c;

#ifdef AFFICHAGE
  printf("term_add_onf_term(");
  term_print(stdout,t);
  printf(",");
  term_print(stdout,subterm);
  printf(")\n");
#endif
  
  // Ajout d un sous-terme vide 
  if(term_isAC(subterm) && term_first(subterm)==NULL) { 
    return t; 
  } 

  if(getSymb(t) == getSymb(subterm)) {
    // il faut applatir et faire un merge list
    if(term_first(t)==NULL) {
      if(isShared(subterm)) {
	term_first(t)=copy_list(term_first(subterm),&second);
	term_last(t)=second;
      } else {
	term_first(t)=term_first(subterm);
	term_last(t)=term_last(subterm);
      }
      return t;
    }
    
#ifdef COLOR
    // A optimiser
    // il faut colorier les sous-termes
    for(cell=term_first(subterm) ; cell !=NULL ; cell=cell_next(cell)) {
      setColor(cell,color);
    }
#endif

    /*
     * Memorisation de la fin du sous-terme pour
     * accelerer la maj de term_last(t)
     */
    //c = term_last(subterm);
    if(isAC) {
      if(isShared(subterm)) {
	term_first(t)=merge_shared_sorted_list(term_first(t),term_first(subterm),&second);
      } else {
	term_first(t)=merge_sorted_list(term_first(t),term_first(subterm),&second);
      }
    } else {
      printf("List-matching case not implemented\n");
      exit(1);
      // merge two lists
      /*
      if(isShared(subterm)) {
	first=copy_list(term_first(subterm));
      } else {
	first=term_first(subterm);
      }
      if(first==NULL) {
	term_first(t)=first;
      } else {
	cell_next(term_last(t)) = first;
      }
      // C'est bizarre ici !!!
      if(term_last(subterm)!=NULL) {
	term_last(t)=term_last(subterm);
      }
      */
    }
    term_last(t)=second;
    goto fin;
  }

  //printf("find_position\n");

  // Il n'y a pas d'applatissement
  if(isAC) {
    if(term_first(t)==NULL) {
      CELL_ALLOC(cell);
      cell_t(cell)=subterm;
      setMult(cell,1);
      setShared(subterm);
      // il faut colorier la nouvelle cellule
      setColor(cell,color);
      term_first(t) = term_last(t) = cell;
      goto fin;
    }
    find_position(term_first(t),subterm,&first,&second);
  } else {
    // To add at the end of the list
    first=term_first(t);
    second=NULL;
  }
  
#ifdef AFFICHAGE
  printf("first=%d\n",first);
  printf("first=");
  if(first==NULL)
    printf("NULL\n");
  else
    term_printnl(stdout,cell_t(first));

  printf("second=");
  if(second==NULL)
    printf("NULL\n");
  else
    term_printnl(stdout,cell_t(second));
#endif

  if(first!=NULL && first==second) {
    // il faut modifier la multiplicite
    // et mettre a jour la couleur
    setMult(first,1+getMult(first));
    setColor(first,bicolor);
    // Optimisation: Creation du partage
    /*
      if(subterm != cell_t(first)) {
      printf("p1=%x\tp2=%x\n",subterm,cell_t(first));
      cell_t(first) = subterm;
      setShared(subterm);
    }
    */
    goto fin;
  }

  // sinon : il faut inserer la cellule

  CELL_ALLOC(cell);
  cell_t(cell)=subterm;
  setMult(cell,1);
  // il faut colorier la nouvelle cellule
  setColor(cell,color);
  addcounter(subterm);
  //  printf("first=%x\tsecond=%x\n",first,second);
  cell_insert(t,cell,first,second);
fin:
#ifdef AFFICHAGE
  printf("result = ");
  term_printnl(stdout,t);
  printf("\n");
#endif
  return t;
}

struct term *intern_term_add_onf_term_old(int isAC,
				      struct term *t,
				      struct term *subterm)
{
  struct cell_term *first;
  struct cell_term *second;
  struct cell_term *cell;
  struct cell_term *last;

#ifdef AFFICHAGE
  printf("term_add_onf_term(");
  term_print(stdout,t);
  printf(",");
  term_print(stdout,subterm);
  printf(")\n");
#endif

  // Ajout d un sous-terme vide 
  if(term_isAC(subterm) && term_first(subterm)==NULL) { 
    return t; 
  } 

  if(getSymb(t) == getSymb(subterm)) {
    // il faut applatir et faire un merge list
    struct cell_term *subterm_first_cell=NULL;
    struct cell_term *c;
    
    if(isShared(subterm)) {
      // le sous terme est partage
      struct cell_term *copy_cell;
      struct cell_term *last_cell;
      last_cell=NULL;
      for(c=term_first(subterm) ; c!=NULL ; c=cell_next(c)) {
	printf("ALLOC\n");
	CELL_ALLOC(copy_cell);
	CELL_INIT(copy_cell);
	cell_t(copy_cell)=cell_t(c);
	setShared(cell_t(c));
	setMult(copy_cell, getMult(c));
	if(last_cell!=NULL)
	  cell_next(last_cell)=copy_cell;
	else
	  subterm_first_cell=copy_cell;
	last_cell=copy_cell;
      }
      //printf("onf noshared %d\n",noshared++);
    } else {
      subterm_first_cell=term_first(subterm);
      //printf("onf shared   %d\n",shared++);
    }
    
    /*
     * Memorisation de la fin du sous-terme pour
     * accelerer la maj de term_last(t)
     */
    //c = term_last(subterm);
    if(isAC) {
      term_first(t)=merge_sorted_list(term_first(t),subterm_first_cell,&last);
    } else {
      // merge two lists
      if(term_first(t)==NULL) {
	term_first(t)=subterm_first_cell;
      } else {
	cell_next(term_last(t)) = subterm_first_cell;
      }
    }
    // A OPTIMISER
    for(c=term_first(t) ; cell_next(c)!=NULL ; c=cell_next(c)) {
      //for( ; cell_next(c)!=NULL ; c=cell_next(c)) {
      /* do nothing */;
    }
    term_last(t)=c; // maj de la fin de liste
    goto fin;
  }

  //printf("find_position\n");

  // Il n'y a pas d'applatissement
  if(isAC) {
    find_position(term_first(t),subterm,&first,&second);
  } else {
    // To add at the end of the list
    first=term_first(t);
    second=NULL;
  }

  
#ifdef AFFICHAGE
  printf("first=%d\n",first);
  printf("first=");
  if(first==NULL)
    printf("NULL\n");
  else
    term_printnl(stdout,cell_t(first));

  printf("second=");
  if(second==NULL)
    printf("NULL\n");
  else
    term_printnl(stdout,cell_t(second));
#endif

  if(first!=NULL && first==second)
    // il faut modifier la multiplicite
    {
      setMult(first,1+getMult(first));
      goto fin;
    }

  // sinon : il faut inserer la cellule

  CELL_ALLOC(cell);
  cell_t(cell)=subterm;
  setMult(cell,1);
  addcounter(subterm);
  cell_insert(t,cell,first,second);
fin:
#ifdef AFFICHAGE
  printf("result = ");
  term_printnl(stdout,t);
  printf("\n");
#endif
  return t;
}



/*
 * insert une cellule dans un terme
 */
void cell_insert(struct term *t, struct cell_term *cell,
		 struct cell_term *first, struct cell_term *second)
{
  if(first==NULL && second==NULL) {
    term_first(t)=term_last(t)=cell;
    cell_next(cell)=NULL;
  } else if(first==NULL) {
    cell_next(cell)=term_first(t);
    term_first(t)=cell;
  } else if(second==NULL) {
    cell_next(term_last(t))=cell;
    term_last(t)=cell;
    cell_next(cell)=NULL;
  } else {
    cell_next(cell)=second;
    cell_next(first)=cell;
  }
}

/*
 * cherche une position ou inserer une cellule
 */
static void find_position(struct cell_term *list, struct term *elt,
			  struct cell_term **ptr_first,
			  struct cell_term **ptr_second)
{
  register struct cell_term *cell;
  struct cell_term *last_cell=NULL;
  register int comp;

  //printf("list=%d\n",list);
  for(cell=list ; cell!=NULL ; cell=cell_next(cell) ) {
    comp=term_cmp(cell_t(cell), elt);
    if(comp<=0) {
      *ptr_first = (comp!=0)?last_cell:cell;
      *ptr_second=cell;
      return;
    } else {
      last_cell=cell;
    }
  }
  *ptr_first=last_cell;
  //cell=NULL;
  //*ptr_second=cell;
  *ptr_second=NULL;
  //printf("*ptr_first=%d\n",*ptr_first);
  //printf("*ptr_second=%d\n",*ptr_second);
  return;
}

/*
 *	Sort argument lists in descending order
 */
static struct cell_term *term_merge_sort(struct cell_term *p1, int len)
{
  struct cell_term *p2, *q, *last;
  int i, l;

  if(len <= 1)
    return(p1);
  l = len / 2;
  for(q = p1, i = l - 1 ; i > 0 ; i--)
    q = cell_next(q);
  p2 = cell_next(q);
  cell_next(q) = NULL;
  p1 = term_merge_sort(p1, l);
  p2 = term_merge_sort(p2, len - l);
  return merge_sorted_list(p1,p2,&last);
}

/*
 *	merge two sorted lists in descending order
 *      fait la fusion des termes identiques
 */

static struct cell_term *merge_sorted_list(register struct cell_term *p1,
					   register struct cell_term *p2,
					   struct cell_term **ptr_last)
{
  struct cell_term *q, *base;
  int comp;

  if(p1 == NULL) { base=(q=p2); goto fin; }
  if(p2 == NULL) { base=(q=p1); goto fin; }

  if( (comp=term_cmp(cell_t(p1), cell_t(p2))) ) {
    if(comp > 0) {
      q = p1;
      p1 = cell_next(p1);
      if(p1 == NULL) { cell_next(q) = p2; base=q; goto fin; }
    } else {
      // comp < 0 
      q = p2;
      p2 = cell_next(p2);
      if(p2 == NULL) { cell_next(q) = p1; base=q; goto fin; }
    }
  } else {
    // on fusionne les 2 cellules
    q = p1;
    setMult(p1, getMult(p1)+getMult(p2));
    setColor(p1,bicolor);
    // Optimisation: Creation du partage
    //printf("p1=%x\tp2=%x\n",cell_t(p1),cell_t(p2));
    //cell_t(p2) = cell_t(p1);
    //setShared(cell_t(p2));

    p1 = cell_next(p1);
    p2 = cell_next(p2);
    if(p1 == NULL) { cell_next(q) = p2; base=q; goto fin; }
    if(p2 == NULL) { cell_next(q) = p1; base=q; goto fin; }
  }

  base = q;

  for(;;) {
    if( (comp = term_cmp(cell_t(p1), cell_t(p2))) ) {
      if( comp > 0) {
	q = (cell_next(q) = p1);
	p1 = cell_next(p1);
	if(p1 == NULL){ cell_next(q) = p2; break; }
      } else {
	// comp < 0
	q = (cell_next(q) = p2);
	p2 = cell_next(p2);
	if(p2 == NULL){ cell_next(q) = p1; break; }
      }
    } else {
      // on fusionne les 2 cellules
      q = (cell_next(q) = p1);
      setMult(p1, getMult(p1)+getMult(p2));
      setColor(p1,bicolor);
      // Optimisation: Creation du partage
      //      printf("p1=%x\tp2=%x\n",cell_t(p1),cell_t(p2));
      //cell_t(p1) = cell_t(p2);
      //setShared(cell_t(p1));

      p1 = cell_next(p1);
      p2 = cell_next(p2);
      if(p1 == NULL){ cell_next(q) = p2; break; }
      if(p2 == NULL){ cell_next(q) = p1; break; }
    }
  }
 fin:
  for( ; cell_next(q) != NULL ; q = cell_next(q));
  *ptr_last=q;
  return(base);
}


static struct cell_term *copy_list(struct cell_term *p, struct cell_term **ptr_last) {
  struct cell_term *c;
  struct cell_term *copy_cell;
  struct cell_term *last_cell=NULL;
  struct cell_term *subterm_first_cell=NULL;
  for(c=p ; c!=NULL ; c=cell_next(c)) {
    CELL_ALLOC(copy_cell);
    CELL_INIT(copy_cell);
    cell_t(copy_cell)=cell_t(c);
    setShared(cell_t(c));
    setMult(copy_cell, getMult(c));
    setColor(copy_cell,getColor(c));
    if(last_cell!=NULL)
      cell_next(last_cell)=copy_cell;
    else
      subterm_first_cell=copy_cell;
    last_cell=copy_cell;
  }
  *ptr_last = last_cell;
  return subterm_first_cell;
}

/*
 * cells are copied before being sorted
 */
static struct cell_term *merge_shared_sorted_list(register struct cell_term *p1,
						  register struct cell_term *p2,
						  struct cell_term **ptr_last)
{
  struct cell_term *q, *base;
  int comp;

  *ptr_last=NULL;
  if(p1 == NULL) { base=(q=copy_list(p2,ptr_last)); goto fin; }
  if(p2 == NULL) { base=(q=p1); goto fin; }

  if( (comp=term_cmp(cell_t(p1), cell_t(p2))) ) {
    if(comp > 0) {
      q = p1;
      p1 = cell_next(p1);
      if(p1 == NULL) { cell_next(q) = copy_list(p2,ptr_last); base=q ; goto fin; }
    } else {
      // comp < 0 
      struct cell_term *copy_cell;
      CELL_ALLOC(copy_cell);
      CELL_INIT(copy_cell);
      cell_t(copy_cell)=cell_t(p2);
      setShared(cell_t(p2));
      setMult(copy_cell, getMult(p2));
      setColor(copy_cell,getColor(p2));
      q = copy_cell;
      p2 = cell_next(p2);
      if(p2 == NULL) { cell_next(q) = p1; base=q; goto fin; }
    }
  } else {
    // on fusionne les 2 cellules
    q = p1;
    setMult(p1, getMult(p1)+getMult(p2));
    setColor(p1,bicolor);
    // Optimisation: Creation du partage
    //cell_t(p1) = cell_t(p2);
    //setShared(cell_t(p1));

    p1 = cell_next(p1);
    p2 = cell_next(p2);
    if(p1 == NULL) { cell_next(q) = copy_list(p2,ptr_last); base=q; goto fin; }
    if(p2 == NULL) { cell_next(q) = p1; base=q; goto fin; }
  }

  base = q;

  for(;;) {
    if( (comp = term_cmp(cell_t(p1), cell_t(p2))) ) {
      if( comp > 0) {
	q = (cell_next(q) = p1);
	p1 = cell_next(p1);
	if(p1 == NULL){ cell_next(q) = copy_list(p2,ptr_last); goto fin; }
      } else {
	// comp < 0
	struct cell_term *copy_cell;
	CELL_ALLOC(copy_cell);
	CELL_INIT(copy_cell);
	cell_t(copy_cell)=cell_t(p2);
	setShared(cell_t(p2));
	setMult(copy_cell, getMult(p2));
	setColor(copy_cell,getColor(p2));
	q = (cell_next(q) = copy_cell);
	p2 = cell_next(p2);
	if(p2 == NULL){ cell_next(q) = p1; goto fin; }
      }
    } else {
      // on fusionne les 2 cellules
      q = (cell_next(q) = p1);
      setMult(p1, getMult(p1)+getMult(p2));
      setColor(p1,bicolor);
      // Optimisation: Creation du partage
      /*
      if(cell_t(p1)!=cell_t(p2)) {
	printf("p1=%x\tp2=%x\n",cell_t(p1),cell_t(p2));
	if(cell_t(p1) < cell_t(p2)) {
	  cell_t(p1) = cell_t(p2);
	  setShared(cell_t(p1));
	} else {
	  cell_t(p2) = cell_t(p1);
	  setShared(cell_t(p2));
	}
      }
      */
      p1 = cell_next(p1);
      p2 = cell_next(p2);
      if(p1 == NULL){ cell_next(q) = copy_list(p2,ptr_last); goto fin; }
      if(p2 == NULL){ cell_next(q) = p1; goto fin; }
    }
  }

 fin:
  /*
   * maj de *ptr_last
   */
  for( ; cell_next(q) != NULL ; q = cell_next(q));
  *ptr_last=q;

  /*  
  for( ; cell_next(q) != NULL ; q = cell_next(q)); 
  if(*ptr_last!= NULL && *ptr_last != q) { 
    printf("warning\n"); 
    printf("q=%x\t*ptr_last=%x\n",q,*ptr_last); 
    *ptr_last=q; 
  } 
  */

  return(base);
}


/*
 *	Compare flattened/sorted terms using lexicographic order for
 *	argument list of free function symbols and multiset order for
 *	argument lists of AC function symbols.
 */

int term_cmp(register struct term *t1, register struct term *t2)
{
  register int r, arity1;

start:
  if(t1==t2) {
    return (0);
  }

  if(isTagged(t1)) {
    if(isIntegerTagged(t1)) {
      if(getInt(t1) != getInt(t2)) {
	return(getInt(t1) - getInt(t2));
      } else {
	printf("error in term_cmp\n");
	exit(1);
      }
    } else if(isIdentifierTagged(t1)) {
      return (getIdentifier(t1) - getIdentifier(t2));
    } else if(isStringTagged(t1)) {
      return strcmp(getString(t1),getString(t2));
    }
  } else if(getSymb(t1) != getSymb(t2)) {
    return(getSymb(t1) - getSymb(t2));
  }

  if((arity1=term_arity(t1))==0) {
    return(0);
  }

  /*
  printf("t1 = "); term_printnl(stdout,t);
  printf("t2 = "); term_printnl(stdout,t2);
  printf("------------------------------\n");
  */

  if(!term_isAC(t1)) {
    /* lexicographic ordering on subterms */
    register struct term **tt1=&(t1->sub[0]);
    register struct term **tt2=&(t2->sub[0]);

    for( arity1-- ; arity1 ; arity1--, tt1++, tt2++) {
      if((r = term_cmp(*tt1,*tt2))) {
	return(r);
      }
      /*
       * on peut creer du partage ici
       */
    }
    /* last rec. opt. */
    t1=*tt1;
    t2=*tt2;
    goto start;
  } else {
    /* multiset ordering on subterms */
    register struct cell_term *p1, *p2;
    for(p1 = term_first(t1), p2 = term_first(t2); ;
	p1 = cell_next(p1) , p2 = cell_next(p2)) {
      if(p1 == NULL) return(p2 == NULL ? 0 : (-1));
      if(p2 == NULL) return(1);
      if((r = term_cmp(cell_t(p1), cell_t(p2))))
	return(r);
      /*
       * on peut creer du partage ici
       */
      cell_t(p2)=cell_t(p1);

      if(getMult(p1) < getMult(p2)) return(-1);
      if(getMult(p1) > getMult(p2)) return(1);
    }
  }
  return(0);
}


long term_notDestructEqual(struct term *t1,struct term *t2)
{
  int arity;
  
  if(t1==t2) return(1);

  //printf("t1 = "); term_printnl(stdout,t1);
  //printf("t2 = "); term_printnl(stdout,t2);

  if(isTagged(t1) || isTagged(t2)) {
      /*
       * [pem: Jun 23 99] : ne sert a rien car teste precedemment
       */
    if(isIntegerTagged(t1) || isIntegerTagged(t2) ||
       isIdentifierTagged(t1) || isIdentifierTagged(t2) ) {
      return t1==t2;
    } else
      
    if(isStringTagged(t1) || isStringTagged(t2)) {
      if(isStringTagged(t1) && isStringTagged(t2)) {
	return strcmp(getString(t1),getString(t2));
      } else {
	printf("error in term_notDestructEqual\n");
	exit(1);
      }
    } 
  } else if(getSymb(t1) != getSymb(t2)) {
    return (0);
  }

  if ((arity=term_arity(t1))==0) return(1);

  if(!term_isAC(t1)) {
    register int i;
    for(i=0 ; i<arity ; i++) {
      if(!term_notDestructEqual(t1->sub[i], t2->sub[i]))
	return (0);
    }
    return (1);
  } else {
    register struct cell_term *p, *p2;
    for(p = term_first(t1), p2 = term_first(t2); ;
	p = cell_next(p), p2 = cell_next(p2)) {
      if(p == NULL) return(p2 == NULL ? 1 : 0);
      if(p2 == NULL) return(0);
      if(!term_notDestructEqual(cell_t(p), cell_t(p2))) return (0);
      if(getMult(p) != getMult(p2)) return (0);
    }  
    return (1);
  }
}

/*
 * Replace t2 by t3 in t1
 */

#ifdef __cplusplus
typedef struct term* (*funTabType)(...);
#else
typedef struct term* (*funTabType)();
#endif
extern funTabType funTab[];
extern funTabType strTab[];

#define funTabCall0
#define funTabCall1 arg[0]
#define funTabCall2  funTabCall1,arg[1]
#define funTabCall3  funTabCall2,arg[2]
#define funTabCall4  funTabCall3,arg[3]
#define funTabCall5  funTabCall4,arg[4]
#define funTabCall6  funTabCall5,arg[5]
#define funTabCall7  funTabCall6,arg[6]
#define funTabCall8  funTabCall7,arg[7]
#define funTabCall9  funTabCall8,arg[8]
#define funTabCall10 funTabCall9,arg[9]
#define funTabCall11 funTabCall10,arg[10]
#define funTabCall12 funTabCall11,arg[11]
#define funTabCall13 funTabCall12,arg[12]
#define funTabCall14 funTabCall13,arg[13]
#define funTabCall15 funTabCall14,arg[14]
#define funTabCall16 funTabCall15,arg[15]

struct term* specialApply(struct term *res) {
  int fsym = getSymb(res);
  struct term **arg=res->sub;

  if(funTab[fsym]==NULL) return res;

    //  printf("specialApply : arity = %d\n",term_arity(res));
  switch(term_arity(res)) {
      // pour le cas AC
  case -1: return funTab[fsym](res);
  case 0:  return funTab[fsym](funTabCall0);
  case 1:  return funTab[fsym](funTabCall1);
  case 2:  return funTab[fsym](funTabCall2);
  case 3:  return funTab[fsym](funTabCall3);
  case 4:  return funTab[fsym](funTabCall4);
  case 5:  return funTab[fsym](funTabCall5);
  case 6:  return funTab[fsym](funTabCall6);
  case 7:  return funTab[fsym](funTabCall7);
  case 8:  return funTab[fsym](funTabCall8);
  case 9:  return funTab[fsym](funTabCall9);
  case 10: return funTab[fsym](funTabCall10);
  case 11: return funTab[fsym](funTabCall11);
  case 12: return funTab[fsym](funTabCall12);
  case 13: return funTab[fsym](funTabCall13);
  case 14: return funTab[fsym](funTabCall14);
  case 15: return funTab[fsym](funTabCall15);
  case 16: return funTab[fsym](funTabCall16);
  default:
    printf("increase maxArity=16 in term.c::specialApply\n");
    exit(1);
  }
}

struct term* normalise(struct term *t) {
  int code, arity, i;
  struct cell_term *p;
  
  if(isIntegerTagged(t) || isIdentifierTagged(t) || isStringTagged(t)) {
    return t;
  }

  code  = getSymb(t);
  arity = term_arity(t);
    //printf("Normalise\tcode=%d\tarity=%d\n",code,arity);
    //term_println(stdout,t);
  
  if(!term_isAC(t)) {
    for(i=0 ; i<arity ; i++) {
      t->sub[i]=normalise(t->sub[i]);
    }
  } else {
    for(p = term_first(t) ; p!=NULL ; p = cell_next(p)) {
      cell_t(p)=normalise(cell_t(p));
    }
  }
  t=specialApply(t);
    //term_println(stdout,t);
    //printf("end normalise\n");
  return t;
}


/*
 * Replace t2 by t3 in t1
 */
static struct term *term_rec_replace(int isShared,
				     struct term *t1,
				     struct term *t2,
				     struct term *t3)
{
  int i,arity;
  struct cell_term *p;
  struct term *res;
  int renormalise=0;

  if(term_notDestructEqual(t1,t2)) {
    setShared(t3);
    return t3;
  }
  //if(isTagged(t1)) return t1;
  if(isIntegerTagged(t1) || isIdentifierTagged(t1) || isStringTagged(t1)) {
    return t1;
  }

  arity=term_arity(t1);
  if(arity==0) return t1;
/*  
  printf("replace( ");
  term_print(stdout,t2);
  printf(" by ");
  term_print(stdout,t3);
  printf(" in ");
  isShared|=isShared(t1);
  if(isShared) printf("[*]");
  term_print(stdout,t1);
  printf(" )\n");
*/
  
  isShared|=isShared(t1);
  if(!term_isAC(t1)) {
    struct term *tmp;
    /*
     * Il y a un probleme dans le marquage du partage qui n'est pas recursif
     * Faut-il supprimer cette optimisation ou faire un marquage recursif ?
     * Pour le moment on supprime l'optimisation
     */
    if(1 || isShared) {
      // Il faut dupliquer le symbole de tete
      res=(struct term*)MALLOC(sizeof(struct term)+((arity-2)*sizeof(struct term*)));
      setSymb(res,getSymb(t1));
    } else {
      res=t1;
    }
    for(i=0 ; i<arity ; i++) {
      tmp = term_rec_replace(isShared,t1->sub[i],t2,t3);
      //      if(tmp != res->sub[i]) {
      res->sub[i] = tmp;
      if(tmp != t1->sub[i]) {
	//printf("syntactic renormalise\n");
	renormalise=1;
      }
    }
  } else {
    struct term *tmp;
    //printf("not yet implemented\n");
    //exit(0);
    TERM_ALLOC(res,term2,getSymb(t1));
    for(p = term_first(t1) ; p!=NULL ; p = cell_next(p)) {
      tmp = term_rec_replace(isShared,cell_t(p),t2,t3);
      // Attention a la multiplicite et a la couleur
      for(i=0 ; i<getMult(p) ; i++)
	term_add_onf_term(res,tmp);
      if(tmp != cell_t(p)) {
	//printf("AC renormalise\n");
	renormalise=1;
      }
    }
  }
  
  /*
   * re-normalisation
   */
  if(renormalise) {
      /*
        printf("start renormalise\n");
        printf("arity = %d\tfsym = %d\n",arity,getSymb(res));
        printf("term = "); term_printnl(stdout,res);
      */
    res=specialApply(res);
      /*
        printf("term = "); term_printnl(stdout,res);;
        printf("end renormalise\n");
      */
  }

  //printf("res = "); term_printnl(stdout,res);
  return res;
}

struct term *term_replace(struct term *t1,struct term *t2,struct term *t3)
{
  /*
   * Replace t2 by t3 in t1
   */
  if(term_occur(t1,t2)) {
    return term_rec_replace(0,t1,t2,t3);
  } else {
    return t1;
  }
}


/*
 * t2 occurs in t1
 */
int term_occur(struct term *t1,struct term *t2)
{
  int i,arity;
  struct cell_term *p;

  //term_print(stdout,t2); printf(" occurs in "); term_printnl(stdout,t1);
  //printf("isAC: %d\tarity = %d\n",term_isAC(t1),arity);

  if(term_notDestructEqual(t1,t2)) return 1;
  //if(isTagged(t1)) return 0;
  if(isIntegerTagged(t1) || isIdentifierTagged(t1) || isStringTagged(t1)) {
    return 0;
  }

  arity=term_arity(t1);
  if (arity==0) return 0;

  if(!term_isAC(t1)) {
    for(i=0 ; i<arity ; i++) {
      if(term_occur(t1->sub[i],t2)) return 1;
    }
  } else {
    for(p = term_first(t1) ; p!=NULL ; p = cell_next(p)) {
      if(term_occur(cell_t(p),t2)) return 1;
    }
  }
  return 0;
}

struct term *term_copyTopSymbol(struct term *t)
{
  struct term *res;
  struct cell_term *cell;
  struct cell_term *copy_cell;

  TERM_ALLOC(res,term2,getSymb(t));
  for(cell=term_first(t) ; cell!=NULL ; cell=cell_next(cell)) {
    CELL_ALLOC(copy_cell);
    CELL_INIT(copy_cell);
    cell_t(copy_cell)=cell_t(cell);
    addcounter(cell_t(cell));
    setMult(copy_cell, getMult(cell));
    setColor(copy_cell,getColor(cell));
    cell_add_last(cell,res);
    }
  return res;
}

/*
 * transform F(t) into t
 * returns NULL if the term is ok
 */
struct term *term_removeTopSymbol(struct term *t)
{
  Verif_void(t,"term_removeTopSymbol(t)");
  if(!isAC(t)) {
    /*
     * pour fonctionner avec l'ACMatcher qui peut retourner
     * des termes syntaxiques
     */
    return t;
  } else {
    if( term_first(t) == term_last(t) ) {
      if(term_first(t) == NULL) {
	// le terme vide F() n'est pas modifie
	return t;
      } else if(getMult(term_first(t))==1) {
	return cell_t(term_first(t));
      } else {
	return (struct term*) NULL;
      }
    } else {
      return (struct term*) NULL;
    }
  }
}

struct term *term_removePossibleTopSymbol(struct term *t)
{
  Verif_void(t,"term_removePossibleTopSymbol(t)");
  if( term_first(t) == term_last(t) ) {
    if(term_first(t) == NULL) {
      // le terme est vide: F()
      fprintf(stderr,"term_removePossibleTopSymbol: empty term\n");
      exit(1);
    } else if(getMult(term_first(t))==1) {
      return cell_t(term_first(t));
    } else {
      return t;
    }
  } else {
    return t;
  }
}



struct term *term_metaApply(struct term *t) {

  struct term *strategy;
  struct term *list;
  struct term *mainTerm;
  struct term *nil;
  struct term *strategyNumber;
  int index;
  int start, end, nbSol;
  int all=0;
  int *counter=(int*) allocStable(sizeof(int));

  struct term *applyRes, *newRes;
  struct term ***ptr_oldRes=(struct term***)allocStable(sizeof(struct term **));
  struct term **listRes=(struct term**)allocStable(sizeof(struct term *));
  *counter=0;

  //printf("------------------------------------------------------------\n");
  //printf("symb = %d\n",getSymb(t));
  switch(getSymb(t)) {
  case 128:
    /*
    printf("t   : "); term_printnl(stdout,t);
    */
    strategy=t->sub[0];
    list=t->sub[1];
    start = getInt(t->sub[2]);
    nbSol = getInt(t->sub[3]);

    strategyNumber=strategy->sub[0]->sub[0];
    mainTerm=list->sub[0];
    nil=list->sub[1];
    /* 
    printf("list     : "); term_printnl(stdout,list);
    printf("nil      : "); term_printnl(stdout,nil);
    printf("mainTerm : "); term_printnl(stdout,mainTerm);
    printf("number   : "); term_printnl(stdout,strategyNumber);
    */
    if(isIntegerTagged(strategyNumber)) {
      index=getInt(strategyNumber);
    } else {
      fprintf(stderr,"term_metaApply: internal error\n");
      exit(1);
    }
    //printf("index = %d\n",index);
    //printf("start = %d\tnbSol = %d\n",start,nbSol);
    
    if(nbSol==0) {
      all=1;
    }
    end = start+nbSol;

    //*ptr_oldRes=&oldRes;
    *listRes=NULL;
    *ptr_oldRes=&(*listRes);
    *counter=0;
    if(setChoicePoint()==0){
      CUTOPEN();
      //printf("**********\n");
      
      applyRes = strTab[index](mainTerm);
      (*counter)++; 
      //printf("build=%d\ti=%d\tend=%d\n",*counter>start,*counter,end);

      if(*counter>start) {
	if(*counter<=end || all) {
	  // Construire la solution
	  //term_print(stdout,applyRes);

	  TERM_ALLOC(newRes,term2,getSymb(list));
	  newRes->sub[0]=applyRes;
	  (**ptr_oldRes)=newRes;
	  *ptr_oldRes=&(newRes->sub[1]);

	  //printf("\nres=%d\tnewRes=%d\n",*listRes,newRes);

	} else {
	  // C'est fini
	  //printf("C'est fini\n");
	  //(**ptr_oldRes)=nil;
	  CUTCLOSE();
	}
      }
      fail();
    } else {
      (**ptr_oldRes)=nil;
      //printf("\n");
      //printf("**********\n");
    }
    
    break;
  case 129:
    break;
  case 130:
    break;
  }
  
  //printf("RESULT = "); term_printnl(stdout,*listRes);
  //*listRes=t;
  return *listRes;
}


listTerm *listTermCreate(struct term *term) {
  listTerm *res;
  res=(listTerm*) MALLOC(sizeof(listTerm));
  res->term=term;
  res->next=NULL;
  res->last=res;
  return res;
}

listTerm *addTermListTerm(listTerm *list , struct term *term) {
  /* insertion en queue */
  listTerm *res;
  res=listTermCreate(term);
  if(list==NULL)
    return res;
  list->last->next=res;
  list->last=res;
  return list;
}

static struct term *internNull=NULL;
struct term *asfNull() {
  if(internNull!=NULL)
    return internNull;
  TERM_ALLOC(internNull,term2,193);
  return internNull;
}

struct term *asfCons(struct term *t1,struct term *t2) {
  struct term *res=NULL;

  printf("t1 = "); term_printnl(stdout,t1);
  printf("t2 = "); term_printnl(stdout,t2);

  if(term_isAC(t1)) {
    printf("t1 is AC\n");
    res = term_add_list_term(t1,t2);

  } else {
    printf("t1 is not AC\n");

    TERM_ALLOC(res,term2,193);
    res = term_add_list_term(res,t1);

    printf("res = "); term_printnl(stdout,res);

    res = term_add_list_term(res,t2);
  }
  return res;
}

struct term *asfHead(struct term *t) {
  if(term_first(t)==NULL) {
    printf("asfHead: empty list\n");
    exit(1);
  }
  return cell_t(term_first(t));
 }

struct term *asfTail(struct term *t) {
  struct term *res;
  if(term_first(t)==NULL) {
    printf("asfTail: empty list\n");
    exit(1);
  } else if(term_first(t)==term_last(t)) {
    //printf("asfTail: single element\n");
    //term_printnl(stdout,t);
    return asfNull();
  }
  TERM_ALLOC(res,term2,getSymb(t));
  term_first(res)=cell_next(term_first(t));
  term_last(res)=term_last(t);
  return res;
}

struct term *asfPrefix(struct term *t) {
  struct term *res;
  struct cell_term *cell;
  struct cell_term *copy_cell;

  if(term_first(t)==NULL || term_first(t)==term_last(t)) {
    printf("asfPrefix: empty list or single element\n");
    term_printnl(stdout,t);
    return asfNull();
  }

  TERM_ALLOC(res,term2,getSymb(t));
  for(cell=term_first(t) ; cell!=term_last(t) ; cell=cell_next(cell))
    {
      CELL_ALLOC(copy_cell);
      CELL_INIT(copy_cell);
      cell_t(copy_cell)=cell_t(cell);
      setShared(cell_t(cell));
      setMult(copy_cell, getMult(cell));
      cell_add_last(cell,res);
    }
  return res;
}

struct term *asfLast(struct term *t) {
  struct term *res=NULL;
  if(term_first(t)==NULL || term_last(t)==NULL) {
    printf("asfLast: empty list\n");
    exit(1);
  }
  res = cell_t(term_last(res));
  setShared(res);
  return res;
}

struct term *asfNotEmptyList(struct term *t) {
  if(term_first(t)==NULL && term_last(t)==NULL) {
    return bool2term(0);
  } else {
    return bool2term(1);
  }
}

struct term *asfIsSingleElement(struct term *t) {
  if(term_first(t)!=NULL && term_first(t)==term_last(t)) {
    return bool2term(1);
  } else {
    return bool2term(0);
  }
}


#ifdef COLOR

/*
void setColor(struct term *t, int c) {
  struct cell_term *cell;
  if(!isAC(t)) { 
    t->color=c;
  }

  for(cell=term_first(t) ; cell!=NULL ; cell=cell_next(cell)) {
    cell_t(cell)->color=c;
  }
}
*/

int isMonoColor(struct term *t) {
  struct cell_term *cell;
  int res;
  int color;

  if(!isAC(t)) {
    fprintf(stderr,"isMonoColor: not an AC term\n");
    exit(1);
  }
  
  if(term_first(t)==NULL) { 
    fprintf(stderr,"isMonoColor: no subterm\n");
    exit(1);
  }

  //printf("isMono: "); term_printnl(stdout,t);

  color=getColor(term_first(t));
  res = (color!=bicolor);

  for(cell=term_first(t) ; res && cell!=NULL ; cell=cell_next(cell)) {
    res = res && (color == getColor(cell));
  }
  return res;
}

#endif
