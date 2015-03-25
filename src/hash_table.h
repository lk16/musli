#pragma once

#include <malloc.h>
#include <string.h>

#include "board.h"

struct board_ht_data{
  struct board b;
  int up,low,depth;  
};

typedef unsigned(*board_ht_hashfun_t)(const struct board*);
typedef int(*board_ht_key_equals_t)(const struct board*,const struct board*);


struct board_ht{
  unsigned capacity;
  struct board_ht_data* data;
  board_ht_hashfun_t hash;
  board_ht_key_equals_t key_equals;
};


struct board_ht* board_ht_new(unsigned size,board_ht_hashfun_t hash,board_ht_key_equals_t key_equals);
void board_ht_free(struct board_ht* ht);


struct board_ht_data* board_ht_hash(struct board_ht* ht,const struct board* b);
struct board_ht_data* board_ht_find(struct board_ht* ht,const struct board* b,int depth);
void board_ht_clear(struct board_ht*ht);