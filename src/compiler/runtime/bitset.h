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

#ifdef BITSETMASK
#include "bitset_tab_mask.h"
#endif

#ifdef BITSETINT
#include "bitset_tab_int.h"
#endif

#ifdef BITSET32
#include "bitset_32.h"
#else

#define bitSet32 bitSet
#define bitSet32_create bitSet_create
#define bitSet32_stack_create(dest,size) bitSet *dest; bitSet_stack_create(dest,size)
#define bitSet32_get bitSet_get
#define bitSet32_delete bitSet_delete
#define bitSet32_stack_delete bitSet_stack_delete
#define bitSet32_init_clear bitSet_init_clear
#define bitSet32_set bitSet_set

#endif
