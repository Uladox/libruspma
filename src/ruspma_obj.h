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

#ifndef RUSPMA_OBJ
#define RUSPMA_OBJ

#include "ruspma_c_funct.h"

typedef struct ruspma_hashmap ruspma_hashmap;

enum { RUSPMA_OBJ_NONE, RUSPMA_OBJ_SYMBOL, RUSPMA_OBJ_KEYWORD,
       RUSPMA_OBJ_C_FUNTION, RUSPMA_OBJ_THREAD, RUSPMA_OBJ_PIPE,
       RUSPMA_OBJ_USER, RUSPMA_OBJ_INTERPRETER
};

typedef struct ruspma_obj
{
  int hidden_type;
  int references;
  union
  {
    ruspma_c_funct *c_funct;
    char *string;
  } prim_vals;
  ruspma_hashmap *map;
}ruspma_obj;

//ruspma_obj *send_message(ruspma_obj *receiver, ruspma_obj *message);

#endif
