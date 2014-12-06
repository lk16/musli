#pragma once

#include <stdlib.h>
#include <malloc.h>

#include "board.h"

#define HASH_TABLE_MAX_ELEMS (1ull << 14)
#define HASH_TABLE_BUCKETS (1ull << 12)

typedef struct hash_table_value{
  int heur;
} hash_table_value;


typedef struct hash_table_elem{
  board b;
  hash_table_value value;
  struct hash_table_elem* next;
} hash_table_elem;

typedef struct hash_table{
  hash_table_elem** buckets;
  hash_table_elem *unused,*unused_start;
} hash_table;


unsigned long long hash_of_board(const board* b);

void hash_table_init(hash_table* ht);
void hash_table_init_clear(hash_table* ht);

void hash_table_add(hash_table* ht,const board* key,const hash_table_value* value);
void hash_table_clear(hash_table* ht);

hash_table_value* hash_table_find(hash_table* ht,const board* key);