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

#ifndef RUSPMA_DEFAULT_SYMBOLS_H
#define RUSPMA_DEFAULT_SYMBOLS_H

#include "ruspma_obj.h"

void ruspma_setup_default_symbols();

extern ruspma_obj ruspma_undefined;
extern ruspma_obj ruspma_evaluate;
extern ruspma_obj ruspma_object_not_found;
extern ruspma_obj ruspma_prepare;
extern ruspma_obj ruspma_none;
extern ruspma_obj ruspma_print_ruspma_obj;

#endif
