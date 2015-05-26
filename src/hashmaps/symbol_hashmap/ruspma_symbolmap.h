/*    This file is part of Ruspma.
 *
 *    Ruspma is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    Ruspma is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with Ruspma.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RUSPMA_SYMBOLMAP_H
#define RUSPMA_SYMBOLMAP_H

#include "ruspma_symbolmap_structs.h"

ruspma_symbolmap *ruspma_new_symbolmap_seq(int);

ruspma_symbolmap *ruspma_new_symbolmap(void);

void ruspma_free_symbolmap(ruspma_symbolmap *symbolmap);

void ruspma_print_symbolmap(ruspma_symbolmap *symbolmap);

void ruspma_add_symbolmap(ruspma_symbolmap *symbolmap,
			     char *key, ruspma_obj *storage);

ruspma_obj *ruspma_get_symbolmap(ruspma_symbolmap *symbolmap, char *key);

#endif
