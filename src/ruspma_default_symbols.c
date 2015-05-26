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

#include "ruspma_symbol_constants.h"
#include "c_functs/ruspma_symbol_functs.h"
#include "c_functs/ruspma_eval.h"

#define src_symbol(var_name, string_val)		    \
  ruspma_obj var_name = { .hidden_type = RUSPMA_OBJ_SYMBOL, \
			  .prim_vals.string = string_val };

#define src_symbol_map(var_name)					\
  var_name.map = ruspma_symbol_message_map;				\
  ruspma_add_symbolmap(ruspma_global_symbol_map,			\
		       var_name.prim_vals.string,			\
		       &var_name);

#define src_symbol_message(var_name, fnct_ptr, fnct_name)		\
  ruspma_add_hashmap(ruspma_symbol_message_map, var_name,		\
		     ruspma_new_c_funct(fnct_ptr,			\
					ruspma_src_funct_location,	\
					fnct_name));
//@editsrc src_symbol {
src_symbol(ruspma_undefined, "undefined")
src_symbol(ruspma_evaluate, "evaluate")
src_symbol(ruspma_object_not_found, "not-found")
src_symbol(ruspma_prepare, "prepare")
src_symbol(ruspma_none, "none")
src_symbol(ruspma_obj_print, "print")
//@editsrc src_symbol }

void ruspma_setup_default_symbols(void)
{
  src_symbol_message(&ruspma_obj_print, &ruspma_print_obj,
		     ruspma_print_obj_name)
  src_symbol_message(&ruspma_evaluate, &ruspma_default_eval,
		     ruspma_default_eval_name)

  src_symbol_map(ruspma_undefined)
  src_symbol_map(ruspma_evaluate)
  src_symbol_map(ruspma_object_not_found)
  src_symbol_map(ruspma_prepare)
  src_symbol_map(ruspma_none)
  src_symbol_map(ruspma_obj_print)
}
