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

#include <stdio.h>
#include "../ruspma_obj.h"

char *ruspma_print_obj_name = "ruspma_print_obj";

ruspma_obj *ruspma_print_obj(ruspma_obj *term, ruspma_obj *parent, ruspma_obj *self)
{
  printf("%s\n", term->prim_vals.string);
  return parent;
}
