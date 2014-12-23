#include "hash_table.h"

long long unsigned int hash_of_board(const struct board* b)
{
  return 123457ull*((b->me+0x3710472ef29) ^ (b->opp-0xc0ffeeeee));
}


void hash_table_init(struct hash_table* ht)
{
  unsigned i;
  ht->buckets =  (struct hash_table_elem**)malloc(
    HASH_TABLE_BUCKETS*sizeof(struct hash_table_elem*));
  ht->unused = (struct hash_table_elem*)malloc(
    HASH_TABLE_MAX_ELEMS*sizeof(struct hash_table_elem));
  ht->elems_begin = ht->unused;
  for(i=0;i<HASH_TABLE_BUCKETS;i++){
    ht->buckets[i] = NULL;
  }
  for(i=0;i<HASH_TABLE_MAX_ELEMS-1;i++){
    ht->unused[i].next = ht->unused + i + 1;
  }
  ht->unused[HASH_TABLE_MAX_ELEMS-1].next = NULL;
}

int hash_table_add(struct hash_table* ht, const struct board* key,const struct hash_table_value* value)
{
  if(hash_table_find(ht,key)){
    return 0;
  }  
  struct hash_table_elem* elem = ht->unused;
  if(!elem){
    return 0;
  }
  ht->unused = ht->unused->next;
  unsigned long long hash = hash_of_board(key) % HASH_TABLE_BUCKETS;
  elem->b = *key;
  elem->value = *value;
  elem->next = ht->buckets[hash];
  ht->buckets[hash] = elem;
  return 1;
}

struct hash_table_value* hash_table_find(struct hash_table* ht, const struct board* key)
{
  unsigned long long hash = hash_of_board(key) % HASH_TABLE_BUCKETS;
  struct hash_table_elem* elem = ht->buckets[hash];
  while(elem){
    if(board_equals(&elem->b,key)){
      return &elem->value;
    }
    elem = elem->next;
  }
  return NULL;
}

void hash_table_init_free(struct hash_table* ht)
{
  free(ht->elems_begin);
  free(ht->buckets);
}

void hash_table_clear(struct hash_table* ht)
{
  int i;
  struct hash_table_elem *elem,*tmp;
  for(i=0;i<(int)HASH_TABLE_BUCKETS;i++){
    elem = ht->buckets[i];
    while(elem){
      tmp = elem->next;
      elem->next = ht->unused;
      ht->unused = elem;      
      elem = tmp;
    }
    ht->buckets[i] = NULL;
  }
}




