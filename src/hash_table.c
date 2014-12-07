#include "hash_table.h"

long long unsigned int hash_of_board(const board* b)
{
  return 123457ull*((b->me+0x3710472ef29) ^ (b->opp-0xc0ffeeeee));
}


void hash_table_init(hash_table* ht)
{
  int i;
  ht->buckets = (hash_table_elem**)malloc(HASH_TABLE_BUCKETS*sizeof(void*));
  ht->unused = (hash_table_elem*)malloc(HASH_TABLE_MAX_ELEMS*sizeof(void*));
  ht->unused_start = ht->unused;
  for(i=0;i<(int)HASH_TABLE_BUCKETS;i++){
    ht->buckets[i] = NULL;
  }
  for(i=0;i<(int)HASH_TABLE_MAX_ELEMS;i++){
    ht->unused[i].next = &(ht->unused[i+1]);
  }
  ht->unused[HASH_TABLE_MAX_ELEMS-1].next = NULL;
}

int hash_table_add(hash_table* ht, const board* key,const hash_table_value* value)
{
  if(hash_table_find(ht,key)){
    return 0;
  }  
  hash_table_elem* elem = ht->unused;
  ht->unused = ht->unused->next;
  if(!elem){
    return 0;
  }
  unsigned long long hash = hash_of_board(key) % HASH_TABLE_BUCKETS;
  elem->b = *key;
  elem->value = *value;
  elem->next = ht->buckets[hash];
  ht->buckets[hash] = elem;
  return 1;
}

hash_table_value* hash_table_find(hash_table* ht, const board* key)
{
  unsigned long long hash = hash_of_board(key) % HASH_TABLE_BUCKETS;
  hash_table_elem* elem = ht->buckets[hash];
  while(elem){
    if(board_equals(&elem->b,key)){
      return &elem->value;
    }
  }
  return NULL;
}

void hash_table_init_clear(hash_table* ht)
{
  free(ht->unused_start);
  free(ht->buckets);
}

void hash_table_clear(hash_table* ht)
{
  int i;
  hash_table_elem *elem,*tmp;
  for(i=0;i<(int)HASH_TABLE_BUCKETS;i++){
    elem = ht->buckets[i];
    while(elem){
      tmp = elem->next;
      elem->next = ht->unused;
      ht->unused = elem;      
      elem = tmp;
    }
  }
}




