#include "hash_table.h"

struct board_ht* board_ht_new(unsigned int capacity, board_ht_hashfun_t hash, board_ht_key_equals_t key_equals)
{
  struct board_ht* res = malloc(sizeof(*res));
  res->capacity = capacity;
  res->data = malloc(capacity*sizeof(*res->data));
  memset(res->data,0,capacity*sizeof(*res->data));
  res->key_equals = key_equals;
  res->hash = hash;
  return res;
}

void board_ht_free(struct board_ht* ht)
{
  free(ht->data);
  free(ht);
}

struct board_ht_data* board_ht_hash(struct board_ht* ht,const struct board* b)
{
  return ht->data + (ht->hash(b) % ht->capacity);
}

struct board_ht_data* board_ht_find(struct board_ht* ht,const struct board* b,int depth)
{
  struct board_ht_data* data = board_ht_hash(ht,b);
  if(ht->key_equals(b,&data->b) && data->depth==depth){
    return data;
  }
  return NULL;
}

