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
 *	typedefs
 */

/*
 *	enumerations
 */
/* typedef enum {FALSE, TRUE} BOOL;  */

typedef int BOOL; 
#define FALSE 0
#define TRUE 1


typedef enum {VARIABLE, AC_NORMAL, AC_COMPRESSED,
		FUNCTION, CONSTANT, BUILTIN} TERM_TYPE;

/*
 *	terms
 */
typedef struct term_struct {
  TERM_TYPE type;		/* type of term */
  unsigned long sym;             /* index to symbol table */
  union {
    struct {
      int list_len;				/* length of arg list */
      struct term_list_struct *arg_list;	/* pointer to argument list */
      struct term_list_struct *arg_tail;	/* tail of argument list */
    } f;	/* for AC_NORMAL & FUNCTION */
    struct {
      int arg_count;			/* total number of AC args */
      struct ac_list_struct *ac_list;	/* pointer to AC list */
      struct ac_list_struct *ac_tail;	/* tail of AC list */
    } a;	/* for AC_COMPRESSED */
    struct {
      int var_nr;		/* variable number */
    } v;	/* for VARIABLE */
  } rest;
} TERM;

/*
 *	lists of terms
 */
typedef struct term_list_struct {
  struct term_struct *arg;			/* pointer to term */
  struct term_list_struct *next_arg;		/* pointer to rest */
} TERM_LIST;

/*
 *	AC lists
 */
typedef struct ac_list_struct {
  struct term_struct *arg;		/* pointer to term */
  int mult;				/* multiplicity */
  struct ac_list_struct *next_ac;	/* pointer to rest */
} AC_LIST;

