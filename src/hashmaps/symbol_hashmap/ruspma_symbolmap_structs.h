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

#ifndef RUSPMA_SYMBOLMAP_STRUCTS
#define RUSPMA_SYMBOLMAP_STRUCTS

#include "../../ruspma_obj.h"
// The maximun extries to bins before a rehash

typedef struct ruspma_symbolbin
{
  char *key;
  ruspma_obj *storage;
  struct ruspma_symbolbin *next;
}ruspma_symbolbin;

typedef struct ruspma_symbolmap
{
  unsigned int symbolbin_num;
  int entries_num;
  int *Primes_pointer;
  ruspma_symbolbin **symbolbins;
}ruspma_symbolmap;
#endif
