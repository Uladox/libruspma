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

#include <stdlib.h>
#include <string.h>
#include "ruspma_symbol.h"
#include "hashmaps/symbol_hashmap/ruspma_symbolmap.h"
#include "hashmaps/obj_hashmap/ruspma_hashmap.h"
#include "ruspma_default_symbols.h"
#include "ruspma_symbol_constants.h"

void ruspma_setup_symbols()
{
  ruspma_setup_symbol_constants();
  ruspma_setup_default_symbols();
}

ruspma_obj *new_symbol(char *name)
{
  ruspma_obj *symbol_obj;
  if((symbol_obj = ruspma_get_symbolmap(ruspma_global_symbol_map, name)) == NULL) {
    symbol_obj = malloc(sizeof(ruspma_obj));
    symbol_obj->hidden_type = RUSPMA_OBJ_SYMBOL;
    symbol_obj->prim_vals.string = name;
    symbol_obj->map = ruspma_symbol_message_map;
  }
  return symbol_obj;
}

ruspma_obj *new_symbol_new_string(char *name)
{
  ruspma_obj *symbol_obj;
  if((symbol_obj = ruspma_get_symbolmap(ruspma_global_symbol_map, name)) == NULL) {
    symbol_obj = malloc(sizeof(ruspma_obj));
    symbol_obj->hidden_type = RUSPMA_OBJ_SYMBOL;
    symbol_obj->prim_vals.string =
      strcpy(malloc(strlen(name) + 1), name);
    symbol_obj->map = ruspma_symbol_message_map;
  }
  return symbol_obj;
}
