#pragma once

#include <stdlib.h>
#include <malloc.h>

#include "board.h"

#define HASH_TABLE_MAX_ELEMS (1024*1024)
#define HASH_TABLE_BUCKETS (1024*1024)

struct hash_table_value{
  int heur;
};


struct hash_table_elem{
  struct board b;
  struct hash_table_value value;
  struct hash_table_elem* next;
};

struct hash_table{
  struct hash_table_elem** buckets;
  struct hash_table_elem *unused,*unused_start;
};


unsigned long long hash_of_board(const struct board* b);

void hash_table_init(struct hash_table* ht);
void hash_table_init_free(struct hash_table* ht);

int hash_table_add(struct hash_table* ht,const struct board* key,const struct hash_table_value* value);
void hash_table_clear(struct hash_table* ht);

struct hash_table_value* hash_table_find(struct hash_table* ht,const struct board* key);