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

#include "ruspma_symbolmap_structs.h"
#include "../MurmurHash2.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DEFAULT_ENTRIES_TO_SYMBOLBINS 5
#define DEFAULT_SYMBOL_SEED 37

unsigned int Symbolmap_Primes[24] =
  {
    256 + 27, 512 + 9, 1024 + 9, 2048 + 5, 4096 + 3,
    8192 + 27, 16384 + 43, 32768 + 3, 65536 + 45, 131072 + 29, 262144 + 3, 524288 + 21, 1048576 + 7,
    2097152 + 17, 4194304 + 15, 8388608 + 9, 16777216 + 43, 33554432 + 35, 67108864 + 15,
    134217728 + 29, 268435456 + 3, 536870912 + 11, 1073741824 + 85, 0
  };

static void rehash(ruspma_symbolmap *);
 
/* Copies value of key pointer into the symbolbin */
static ruspma_symbolbin *new_symbolbin(char *key, ruspma_obj *storage)
{
  ruspma_symbolbin *new = malloc(sizeof(ruspma_symbolbin));
  new->key = key;
  new->storage = storage;
  new->next = NULL;
}

/* If key exists saved in some place the symbolmap can uses safely (such as a symbolbin before
   a rehash) then use this method so there is no extra computation */
static ruspma_symbolbin *copy_symbolbin(char *key, ruspma_obj *storage)
{
  ruspma_symbolbin *new = malloc(sizeof(ruspma_symbolbin));
  new->key = key;
  new->storage = storage;
  new->next = NULL;
}

/* frees symbolbin without freeing key, used in rehashing */
static void symbolbin_free(ruspma_symbolbin *symbolbin)
{
  free(symbolbin);
}

/* creates a new symbolmap */
ruspma_symbolmap *ruspma_new_symbolmap(void)
{
  ruspma_symbolmap *new = malloc(sizeof(ruspma_symbolmap));
  new->symbolbin_num = Symbolmap_Primes[0];
  new->symbolbins = malloc(sizeof(ruspma_symbolbin *) * Symbolmap_Primes[0]);
  new->entries_num = 0;
  new->Primes_pointer = Symbolmap_Primes;
  ruspma_symbolbin **symbolbin_ptr = new->symbolbins;
  int i;
  for(i = 0; i != Symbolmap_Primes[0]; ++i) {
    *symbolbin_ptr = NULL;
    ++symbolbin_ptr;
  }
  return new;
}

/* Avoid uneeded rehashings by setting up the symbolmap at creation
   with a point in the sequence */
ruspma_symbolmap *ruspma_new_symbolmap_seq(int sequence)
{
  ruspma_symbolmap *new = malloc(sizeof(ruspma_symbolmap));
  new->symbolbin_num = Symbolmap_Primes[sequence];
  new->symbolbins = malloc(sizeof(ruspma_symbolbin *) * Symbolmap_Primes[sequence]);
  new->entries_num = 0;
  new->Primes_pointer = Symbolmap_Primes + sequence;
  ruspma_symbolbin **symbolbin_ptr = new->symbolbins;
  int i;
  for(i = 0; i != Symbolmap_Primes[sequence]; ++i) {
    *symbolbin_ptr = NULL;
    ++symbolbin_ptr;
  }
  return new;
}

/* Final freeing of symbolmap */
void ruspma_free_symbolmap(ruspma_symbolmap *symbolmap)
{
  ruspma_symbolbin **current_top_symbolbin = symbolmap->symbolbins;
  unsigned int symbolbin_num = symbolmap->symbolbin_num;
  unsigned int i;
  for(i = 0; i != symbolbin_num; ++i) {
    ruspma_symbolbin *current_symbolbin = *current_top_symbolbin;
    while(current_symbolbin != NULL) {
      ruspma_symbolbin *current_symbolbin_holder = current_symbolbin;
      current_symbolbin = current_symbolbin->next;
      symbolbin_free(current_symbolbin_holder);
    }
    ++current_top_symbolbin;
  }
  free(symbolmap->symbolbins);
  free(symbolmap);
}

void ruspma_print_symbolmap(ruspma_symbolmap *symbolmap)
{
  ruspma_symbolbin **current_top_symbolbin = symbolmap->symbolbins;
  unsigned int symbolbin_num = symbolmap->symbolbin_num;
  unsigned int i;
  for(i = 0; i != symbolbin_num; ++i) {
    ruspma_symbolbin *current_symbolbin = *current_top_symbolbin;
    while(current_symbolbin != NULL) {
      ruspma_symbolbin *current_symbolbin_holder = current_symbolbin;
      current_symbolbin = current_symbolbin->next;
      printf("%s ", current_symbolbin_holder->storage->prim_vals.string);
    }
    ++current_top_symbolbin;
    printf("\n");
  }
}
/* checks to see if symbolbin uses key*/
static int check_key_symbolbin(ruspma_symbolbin *symbolbin, char *key)
{
  if(strcmp(symbolbin->key, key) == 0)
    return 1;
  else
    return 0;
}

/* Returns a symbolbin given a key, else makes symbolbin and returns that */
static void search_add_top_symbolbin(ruspma_symbolmap *symbolmap,
				     ruspma_symbolbin *top_symbolbin,
				     char *key, ruspma_obj *storage)
{
  if(check_key_symbolbin(top_symbolbin, key))
    top_symbolbin->storage = storage;
  else {
    ruspma_symbolbin *next_symbolbin = top_symbolbin;
    if(top_symbolbin->next != NULL) {
      next_symbolbin = top_symbolbin->next;
      int loop = 1;
      while(loop) {
	if(check_key_symbolbin(next_symbolbin, key)) {
	  next_symbolbin->storage = storage;
	  loop = 0;
	} else if(next_symbolbin->next == NULL) {
	  next_symbolbin->next = new_symbolbin(key, storage);
	  ++(symbolmap->entries_num);
	  loop = 0;
	} else
	  next_symbolbin = next_symbolbin->next;
      }
    } else
      top_symbolbin->next = new_symbolbin(key, storage);
  }
}


/* You should never need to use this so long name is okay... I hate myself for this
   Returns symbolbin if it exists, else makes new symbolbin and returns it */
static ruspma_symbolbin *search_add_top_symbolbin_old_key(ruspma_symbolmap *symbolmap,
							  ruspma_symbolbin *top_symbolbin,
							  char *key, ruspma_obj *storage)
{
  if(check_key_symbolbin(top_symbolbin, key))
    top_symbolbin->storage = storage;
  else {
    ruspma_symbolbin *next_symbolbin = top_symbolbin;
    if(top_symbolbin->next != NULL) {
      next_symbolbin = top_symbolbin->next;
      int loop = 1;
      while(loop) {
	if(check_key_symbolbin(next_symbolbin, key)) {
	  next_symbolbin->storage = storage;
	  loop = 0;
	} else if(next_symbolbin->next == NULL) {
	  next_symbolbin->next = copy_symbolbin(key, storage);
	  ++(symbolmap->entries_num);
	  loop = 0;
	} else
	  next_symbolbin = next_symbolbin->next;
      }
    } else {
      top_symbolbin->next = new_symbolbin(key, storage);
    }
  }
}

/* Adds an element to the symbolmap */
void ruspma_add_symbolmap(ruspma_symbolmap *symbolmap,
		      char *key, ruspma_obj *storage)
{
  if(symbolmap->entries_num / symbolmap->symbolbin_num >= DEFAULT_ENTRIES_TO_SYMBOLBINS)
    rehash(symbolmap);
  unsigned int row = MurmurHash2(key, strlen(key), DEFAULT_SYMBOL_SEED) % symbolmap->symbolbin_num;
  ruspma_symbolbin *top_symbolbin = symbolmap->symbolbins[row];
  if(top_symbolbin == NULL) {
    symbolmap->symbolbins[row] = new_symbolbin(key, storage);
    ++(symbolmap->entries_num);
    return;
  }

  search_add_top_symbolbin(symbolmap, top_symbolbin, key, storage);
}

/* Adds an element to the symbolmap without allocating a new key. Used in rehashes */
static void readd_to_symbolmap(ruspma_symbolmap *symbolmap, 
			       char *key, ruspma_obj *storage)
{
  if(symbolmap->entries_num / symbolmap->symbolbin_num >= DEFAULT_ENTRIES_TO_SYMBOLBINS)
    rehash(symbolmap);
  unsigned int row = MurmurHash2(key, strlen(key), DEFAULT_SYMBOL_SEED) % symbolmap->symbolbin_num;
  ruspma_symbolbin *top_symbolbin = symbolmap->symbolbins[row];
  if(top_symbolbin == NULL) {
    symbolmap->symbolbins[row] = copy_symbolbin(key, storage);
    ++(symbolmap->entries_num);
    return;
  }

  search_add_top_symbolbin_old_key(symbolmap, top_symbolbin, key, storage);
}

/* Gets an element from a symbolmap */
ruspma_obj *ruspma_get_symbolmap(ruspma_symbolmap *symbolmap, char *key)
{
  unsigned int row = MurmurHash2(key, strlen(key), DEFAULT_SYMBOL_SEED) % symbolmap->symbolbin_num;

  ruspma_symbolbin *top_symbolbin = symbolmap->symbolbins[row];
  if(top_symbolbin == NULL)
    return NULL;
  if(check_key_symbolbin(top_symbolbin, key))
    return top_symbolbin->storage;

  ruspma_symbolbin *next_symbolbin = top_symbolbin->next;
  while(next_symbolbin != NULL) {
    if(check_key_symbolbin(next_symbolbin, key))
      return next_symbolbin->storage;
    next_symbolbin = next_symbolbin->next;
  }
  getchar();

    return NULL;
}

/* Rehashes the symbolmap */
static void rehash(ruspma_symbolmap *symbolmap)
{
  unsigned int old_symbolbin_num = symbolmap->symbolbin_num;
  ruspma_symbolbin **old_symbolbins = symbolmap->symbolbins;

  symbolmap->entries_num = 0;
  ++(symbolmap->Primes_pointer);
  unsigned int new_symbolbin_num = *(symbolmap->Primes_pointer);
  symbolmap->symbolbin_num = new_symbolbin_num;
  ruspma_symbolbin **rehashed_symbolbins = malloc(sizeof(ruspma_symbolbin *) * new_symbolbin_num);
  symbolmap->symbolbins = rehashed_symbolbins;

  unsigned int j;
  for(j = 0; j != new_symbolbin_num; ++j) {
    *rehashed_symbolbins = NULL;
    ++rehashed_symbolbins;
  }

  ruspma_symbolbin **current_top_symbolbin = old_symbolbins;
  unsigned int i;
  for(i = 0; i != old_symbolbin_num; ++i) {
    ruspma_symbolbin *current_symbolbin = *current_top_symbolbin;
    while(current_symbolbin != NULL) {
      readd_to_symbolmap(symbolmap,
		       current_symbolbin->key, 
		       current_symbolbin->storage);
      ruspma_symbolbin *current_symbolbin_holder = current_symbolbin;
      current_symbolbin = current_symbolbin->next;
      symbolbin_free(current_symbolbin_holder);
    }
    ++current_top_symbolbin;
  }
  free(old_symbolbins);
}
