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

#ifndef RUSPMA_SYMBOL_CONSTANTS_H
#define RUSPMA_SYMBOL_CONSTANTS_H

#include "hashmaps/symbol_hashmap/ruspma_symbolmap.h"

extern ruspma_symbolmap *ruspma_global_symbol_map;
extern ruspma_symbolmap *ruspma_global_keyword_map;
extern ruspma_hashmap *ruspma_symbol_message_map;
extern ruspma_hashmap *ruspma_keyword_message_map;
void ruspma_setup_symbol_constants();

#endif
