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
/*
		(c) 	Steven Eker, 1992
			INRIA-Lorraine & CRIN
        		615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
*/
/*
 *	Types for current state of matching problem
 */

/*
 *	Main match object
 */
typedef struct match_object_struct {
  TERM *orig_pat;		/* Original pattern */
  TERM *orig_sub;		/* Original subject */
  int total_var;		/* Total number of variables */
  FREE_PROBLEM *free_prob;		/* pointer to free problem structure */
  struct binding_struct *bind_tab;	/* table of variable bindings */
  struct pure_list_struct *pure_stack;	/* stack of pure AC equations */
  PURE_PROBLEM *pure_prob;	/* pointer to pure problem structure */
  BOOL first;			/* first match extracted ? */
} MATCH_OBJECT;

/*
 *	Variable binding
 */
typedef struct binding_struct {
  TERM *value;			/* term bound variable is bound to */
  int n_times;			/* number of times bound to this term */
} BINDING;

/*
 *	Linked list of pure AC equations
 */
typedef struct pure_list_struct {
  int ac_sym;                                   /* AC top symbol */
  struct ac_list_struct *unused;                /* unmatchable subjects */
  struct var_list_struct *variables;            /* variables under top sym */
  struct pure_list_struct *next_pure;           /* next_pure equation */
} PURE_LIST;

