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

#include "ruspma_hashmap_structs.h"
#include "../MurmurHash2.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// The maximun extries to bins before a rehash
#define DEFAULT_ENTRIES_TO_BINS 5
#define DEFAULT_SEED 37

static unsigned int Hashmap_Primes[29] =
  {
    8 + 3, 16 + 3, 32 + 5, 64 + 3, 128 + 3, 256 + 27, 512 + 9, 1024 + 9, 2048 + 5, 4096 + 3,
    8192 + 27, 16384 + 43, 32768 + 3, 65536 + 45, 131072 + 29, 262144 + 3, 524288 + 21, 1048576 + 7,
    2097152 + 17, 4194304 + 15, 8388608 + 9, 16777216 + 43, 33554432 + 35, 67108864 + 15,
    134217728 + 29, 268435456 + 3, 536870912 + 11, 1073741824 + 85, 0
  };

static void rehash(ruspma_hashmap *);

/* Copies prim_vals.string of key pointer into the bin */
static ruspma_bin *new_bin(ruspma_obj *key, ruspma_obj *storage)
{
  ruspma_bin *new = malloc(sizeof(ruspma_bin));
  new->key = key;
  new->storage = storage;
  new->next = NULL;
}

/* If key exists saved in some place the hashmap can uses safely (such as a bin before
   a rehash) then use this method so there is no extra computation */
static ruspma_bin *copy_bin(ruspma_obj *key, ruspma_obj *storage)
{
  ruspma_bin *new = malloc(sizeof(ruspma_bin));
  new->key = key;
  new->storage = storage;
  new->next = NULL;
}

/* frees bin without freeing key, used in rehashing */
static void bin_free(ruspma_bin *bin)
{
  free(bin);
}

/* creates a new hashmap */
ruspma_hashmap *ruspma_new_hashmap(void)
{
  ruspma_hashmap *new = malloc(sizeof(ruspma_hashmap));
  new->bin_num = Hashmap_Primes[0];
  new->bins = malloc(sizeof(ruspma_bin *) * Hashmap_Primes[0]);

  new->entries_num = 0;
  new->Primes_pointer = Hashmap_Primes;
  ruspma_bin **bin_ptr = new->bins;
  int i;
  for(i = 0; i != Hashmap_Primes[0]; ++i) {
    *bin_ptr = NULL;
    ++bin_ptr;
  }
  return new;
}

/* Avoid uneeded rehashings by setting up the hashmap at creation
   with a point in the sequence */
ruspma_hashmap *ruspma_new_hashmap_seq(int sequence)
{
  ruspma_hashmap *new = malloc(sizeof(ruspma_hashmap));
  new->bin_num = Hashmap_Primes[sequence];
  new->bins = malloc(sizeof(ruspma_bin *) * Hashmap_Primes[sequence]);
  new->entries_num = 0;
  new->Primes_pointer = Hashmap_Primes + sequence;
  ruspma_bin **bin_ptr = new->bins;
  int i;
  for(i = 0; i != Hashmap_Primes[sequence]; ++i) {
    *bin_ptr = NULL;
    ++bin_ptr;
  }
  return new;
}

/* Final freeing of hashmap */
void ruspma_free_hashmap(ruspma_hashmap *hashmap)
{
  ruspma_bin **current_top_bin = hashmap->bins;
  unsigned int bin_num = hashmap->bin_num;
  unsigned int i;
  for(i = 0; i != bin_num; ++i) {
    ruspma_bin *current_bin = *current_top_bin;
    while(current_bin != NULL) {
      ruspma_bin *current_bin_holder = current_bin;
      current_bin = current_bin->next;
      bin_free(current_bin_holder);
    }
    ++current_top_bin;
  }
  free(hashmap->bins);
  free(hashmap);
}

void ruspma_print_hashmap(ruspma_hashmap *hashmap)
{
  ruspma_bin **current_top_bin = hashmap->bins;
  unsigned int bin_num = hashmap->bin_num;
  unsigned int i;
  for(i = 0; i != bin_num; ++i) {
    ruspma_bin *current_bin = *current_top_bin;
    while(current_bin != NULL) {
      ruspma_bin *current_bin_holder = current_bin;
      current_bin = current_bin->next;
      printf("%s ", current_bin_holder->storage->prim_vals.string);
    }
    ++current_top_bin;
    printf("\n");
  }
}
/* checks to see if bin uses key*/
static int check_key_bin(ruspma_bin *bin, ruspma_obj *key)
{
  if(strcmp(bin->key->prim_vals.string, key->prim_vals.string) == 0)
    return 1;
  else
    return 0;
}

/* Returns a bin given a key, else makes bin and returns that */
static void search_add_top_bin(ruspma_hashmap *hashmap, ruspma_bin *top_bin, ruspma_obj *key,
				ruspma_obj *storage)
{
  if(check_key_bin(top_bin, key))
    top_bin->storage = storage;
  else {
    ruspma_bin *next_bin = top_bin;
    if(top_bin->next != NULL) {
      next_bin = top_bin->next;
      int loop = 1;
      while(loop) {
	if(check_key_bin(next_bin, key)) {
	  next_bin->storage = storage;
	  loop = 0;
	} else if(next_bin->next == NULL) {
	  next_bin->next = new_bin(key, storage);
	  ++(hashmap->entries_num);
	  loop = 0;
	} else
	  next_bin = next_bin->next;
      }
    } else
      top_bin->next = new_bin(key, storage);
  }
}


/* You should never need to use this so long name is okay... I hate myself for this
   Returns bin if it exists, else makes new bin and returns it */
static ruspma_bin *search_add_top_bin_old_key(ruspma_hashmap *hashmap,
					      ruspma_bin *top_bin, ruspma_obj *key, ruspma_obj *storage)
{
  if(check_key_bin(top_bin, key))
    top_bin->storage = storage;
  else {
    ruspma_bin *next_bin = top_bin;
    if(top_bin->next != NULL) {
      next_bin = top_bin->next;
      int loop = 1;
      while(loop) {
	if(check_key_bin(next_bin, key)) {
	  next_bin->storage = storage;
	  loop = 0;
	} else if(next_bin->next == NULL) {
	  next_bin->next = copy_bin(key, storage);
	  ++(hashmap->entries_num);
	  loop = 0;
	} else
	  next_bin = next_bin->next;
      }
    } else {
      top_bin->next = new_bin(key, storage);
    }
  }
}

/* Adds an element to the hashmap */
void ruspma_add_hashmap(ruspma_hashmap *hashmap,
		    ruspma_obj *key, ruspma_obj *storage)
{
  if(hashmap->entries_num / hashmap->bin_num >= DEFAULT_ENTRIES_TO_BINS)
    rehash(hashmap);
  unsigned int row = MurmurHash2(&key, sizeof(ruspma_obj *), DEFAULT_SEED) % hashmap->bin_num;
  //printf("%i, %i\n", row, hashmap->bin_num);
  //getchar();
  ruspma_bin *top_bin = hashmap->bins[row];
  if(top_bin == NULL) {
    hashmap->bins[row] = new_bin(key, storage);
    ++(hashmap->entries_num);
    return;
  }

  search_add_top_bin(hashmap, top_bin, key, storage);
}

/* Adds an element to the hashmap without allocating a new key. Used in rehashes */
static void reruspma_add_hashmap(ruspma_hashmap *hashmap, 
			    ruspma_obj *key, ruspma_obj *storage)
{
  if(hashmap->entries_num / hashmap->bin_num >= DEFAULT_ENTRIES_TO_BINS)
    rehash(hashmap);
  unsigned int row = MurmurHash2(&key, sizeof(ruspma_obj *), DEFAULT_SEED) % hashmap->bin_num;
  ruspma_bin *top_bin = hashmap->bins[row];
  if(top_bin == NULL) {
    hashmap->bins[row] = copy_bin(key, storage);
    ++(hashmap->entries_num);
    return;
  }

  search_add_top_bin_old_key(hashmap, top_bin, key, storage);
}

/* Gets an element from a hashmap */
ruspma_obj *ruspma_get_hashmap(ruspma_hashmap *hashmap, ruspma_obj *key)
{
  unsigned int row = MurmurHash2(&key, sizeof(ruspma_obj *), DEFAULT_SEED) % hashmap->bin_num;
  
  ruspma_bin *top_bin = hashmap->bins[row];
  if(top_bin == NULL)
    return NULL;
  if(check_key_bin(top_bin, key))
    return top_bin->storage;

  ruspma_bin *next_bin = top_bin->next;
  while(next_bin != NULL) {
    if(check_key_bin(next_bin, key))
      return next_bin->storage;
    next_bin = next_bin->next;
  }
  getchar();

    return NULL;
}

/* Rehashes the hashmap */
static void rehash(ruspma_hashmap *hashmap)
{
  unsigned int old_bin_num = hashmap->bin_num;
  ruspma_bin **old_bins = hashmap->bins;

  hashmap->entries_num = 0;
  ++(hashmap->Primes_pointer);
  unsigned int new_bin_num = *(hashmap->Primes_pointer);
  hashmap->bin_num = new_bin_num;
  ruspma_bin **rehashed_bins = malloc(sizeof(ruspma_bin *) * new_bin_num);
  hashmap->bins = rehashed_bins;

  unsigned int j;
  for(j = 0; j != new_bin_num; ++j) {
    *rehashed_bins = NULL;
    ++rehashed_bins;
  }

  ruspma_bin **current_top_bin = old_bins;
  unsigned int i;
  for(i = 0; i != old_bin_num; ++i) {
    ruspma_bin *current_bin = *current_top_bin;
    while(current_bin != NULL) {
      reruspma_add_hashmap(hashmap,
			   current_bin->key, 
			   current_bin->storage);
      ruspma_bin *current_bin_holder = current_bin;
      current_bin = current_bin->next;
      bin_free(current_bin_holder);
    }
    ++current_top_bin;
  }
  free(old_bins);
}
