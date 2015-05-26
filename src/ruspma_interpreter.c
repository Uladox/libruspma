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

#include "ruspma_symbol.h"
#include "ruspma_c_funct.h"
#include "c_functs/ruspma_eval.h"
#include "hashmaps/obj_hashmap/ruspma_hashmap.h"

ruspma_obj default_interpreter = {
  .hidden_type = RUSPMA_OBJ_INTERPRETER };

void setup_default_interpreter(void)
{
  default_interpreter.map = new_hashmap();
  add_to_hashmap(default_interpreter.map, &evaluate,
		 new_c_funct(&default_eval,
			     ruspma_src_funct_location,
			     default_eval_name));
}
