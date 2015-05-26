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

#ifndef RUSPMA_C_FUNCT_H
#define RUSPMA_C_FUNCT_H

extern char *ruspma_src_funct_location;

typedef struct ruspma_obj ruspma_obj;

typedef struct ruspma_c_funct
{
  struct ruspma_obj* (*c_funct)(struct ruspma_obj *,
				struct ruspma_obj *,
				struct ruspma_obj *);
  char *location;
  char *name;
}ruspma_c_funct;

ruspma_obj *ruspma_new_c_funct(ruspma_obj* (*c_funct)(struct ruspma_obj *,
						      struct ruspma_obj *,
						      struct ruspma_obj *),
			       char *location, char *name);

#endif
