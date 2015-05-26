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
#include "ruspma_obj.h"
#include "ruspma_symbol.h"
#include "hashmaps/obj_hashmap/ruspma_hashmap.h"

/* ruspma_obj *send_message(ruspma_obj *receiver, ruspma_obj *message) */
/* { */
/*   /\* Runs some code to adapt receiver, useful for reading */
/*      type information then adapting based off it *\/ */
/*   ruspma_obj *prep; */
/*   if((prep = get_hashmap(receiver->map, &prepare)) != NULL && prep != &none) { */
/*     if(prep->hidden_type == RUSPMA_OBJ_C_FUNTION) */
/*       prep->prim_vals.c_funct->c_funct(receiver, message, prep); */
/*     else */
/*       send_message(receiver, &prepare); */
/*   } */
/*   /\* Gets the value from the hashmap, if it is not there deals with it */
/*      without fear of an unintentioned infinite loop *\/ */
/*   ruspma_obj *response; */
/*   if((response = get_hashmap(receiver->map, message)) == NULL) { */
/*     if(message == &object_not_found) */
/*       return &undefined; */
/*     send_message(receiver, &object_not_found); */
/*   } */
/*   /\* Evaluates the response, this allows things to actually happen, */
/*      setup to deal with function pointers, and is recursive to allow */
/*      homoiconic code *\/ */
/*   ruspma_obj *eval_obj; */
/*   if((eval_obj = get_hashmap(receiver->map, &evaluate)) == NULL) */
/*     return &undefined; */
/*   if(eval_obj->hidden_type == RUSPMA_OBJ_C_FUNTION) */
/*     return eval_obj->prim_vals.c_funct->c_funct(receiver, response, eval_obj); */
/*   return send_message(eval_obj, response); */
/* } */
