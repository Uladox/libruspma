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

#ifndef LIB_RUSPMA_H
#define LIB_RUSPMA_H

typedef struct ruspma_hashmap ruspma_hashmap;
typedef struct ruspma_c_funct ruspma_c_funct;

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

typedef struct ruspma_c_funct
{
  struct ruspma_obj* (*c_funct)(struct ruspma_obj *,
				struct ruspma_obj *,
				struct ruspma_obj *);
  char *location;
  char *name;
}ruspma_c_funct;

typedef struct ruspma_bin
{
  ruspma_obj *key;
  ruspma_obj *storage;
  struct ruspma_bin *next;
}ruspma_bin;

typedef struct ruspma_hashmap
{
  unsigned int bin_num;
  int entries_num;
  int *Primes_pointer;
  ruspma_bin **bins;
}ruspma_hashmap;

extern ruspma_obj ruspma_undefined;
extern ruspma_obj ruspma_evaluate;
extern ruspma_obj ruspma_object_not_found;
extern ruspma_obj ruspma_prepare;
extern ruspma_obj ruspma_none;
extern ruspma_obj ruspma_print_ruspma_obj;

extern void ruspma_setup_symbols();

#endif
