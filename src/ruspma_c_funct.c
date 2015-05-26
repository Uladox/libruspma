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
#include "hashmaps/obj_hashmap/ruspma_hashmap.h"
#include "ruspma_obj.h"

char *ruspma_src_funct_location = "src";

ruspma_obj *ruspma_new_c_funct(ruspma_obj* (*c_funct)(struct ruspma_obj *,
						      struct ruspma_obj *,
						      struct ruspma_obj *),
			       char *location, char *name)
{
  ruspma_obj *c_funct_obj = malloc(sizeof(ruspma_obj));
  c_funct_obj->hidden_type = RUSPMA_OBJ_C_FUNTION;
  c_funct_obj->map = ruspma_new_hashmap();
  c_funct_obj->prim_vals.c_funct = malloc(sizeof(ruspma_c_funct));
  c_funct_obj->prim_vals.c_funct->c_funct = c_funct;
  c_funct_obj->prim_vals.c_funct->name = name;
  c_funct_obj->prim_vals.c_funct->location = location;
  return c_funct_obj;
}
