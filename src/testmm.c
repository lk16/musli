#include "testmm.h"

/*
struct lame_ht* lame_ht_new(unsigned(*hash)(const struct board*))
{
  struct lame_ht* res = malloc(sizeof(struct lame_ht));
  res->hash = hash;
  lame_ht_clear(res);
  return res;
}

void lame_ht_clear(struct lame_ht* ht)
{
  int i;
  for(i=0;i<LAME_HT_SIZE;i++){
    ht->data[i].b.me = ht->data[i].b.opp = 0ull;
    ht->data[i].up = 64000;
    ht->data[i].low = -64000;
  }
}


void lame_ht_free(struct lame_ht* ht)
{
  free(ht);
}


struct lame_ht_data* lame_ht_find(struct lame_ht* ht, const struct board* b)
{
  struct lame_ht_data* res = ht->data + (ht->hash(b) % LAME_HT_SIZE);
  if(res->b.me == b->me && res->b.opp == b->opp){
    return res;
  }
  return NULL;
}

struct lame_ht_data* lame_ht_insert(struct lame_ht* ht, const struct board* b)
{
  struct lame_ht_data* ptr = ht->data + (ht->hash(b) % LAME_HT_SIZE);
  if(ptr->b.me != b->me || ptr->b.opp != b->opp){
    ptr->b = *b;
    ptr->low = -64000; 
    ptr->up = 64000;
  }
  return ptr;
}*/

int dummy_heur(const struct board* b){
  int res = 0;
  int me_move_count = board_count_moves(b);
  int opp_move_count = board_count_opponent_moves(b);
  if(me_move_count==0 && opp_move_count==0){
    return 1000 * board_get_disc_diff(b);
  }
  res += me_move_count;
  res -= opp_move_count;
  
  res += 3 *(
      uint64_count(b->me & 0x8100000000000081) 
      - uint64_count(b->opp & 0x8100000000000081)
  );
  return res;
}

void sort_children(struct board* children,struct board* child_end)
{
  int h[32],size,i;
  size = child_end - children;
  for(i=0;i<size;i++){
    h[i] = -test_mtdf_ht(children+i,NULL,2,0);
  }
  /*int loop;
  do{
    loop = 0;
    for(i=0;i<size-1;i++){
      if(h[i] < h[i+1]){
        generic_swap(h[i],h[i+1]);
        generic_swap(children[i],children[i+1]);
        loop = 1;
      }
    }
  }while(loop);*/
  
  int min_index,min_value,j;
  const int k = 3;
  for(i=1;i<k;i++){
    min_index = i;
    min_value = h[i];
    for(j=i+1;j<size;j++){
      if(h[j] < min_value){
        min_index = j;
        min_value = h[j];
      }
    }
    generic_swap(h[i],h[min_index]);
  }
  
  
}



int test_pvs(const struct board* b,int depth_remaining,int alpha,int beta){
  if(depth_remaining==0){
    return dummy_heur(b);
  }
  struct board children[32];
  struct board *it,*child_end = board_get_children(b,children);
  for(it=children;it!=child_end;it++){
    int score;
    if(it==children){
      score = -test_pvs(it,depth_remaining-1,-beta,-alpha);
    }
    else{ 
      score = -test_pvs(it,depth_remaining-1,-alpha-1,-alpha);
      if((alpha < score) && (score < beta)  ){
        score = -test_pvs(it,depth_remaining-1,-beta,-score);
      }
    }
    if(score >= beta){
      alpha = beta;
      break;
    }
    if(score > alpha){
      alpha = score;
    }
  
  }
  return alpha;
}

int test_alphabeta_ht(
  const struct board* b,
  struct board_ht* ht,
  int depth_remaining,
  int beta
){
  if(!ht || depth_remaining < 6){
    struct board copy = *b;
    return test_alphabeta_no_ht(&copy,depth_remaining,beta);
  }
  int g = -64000;
  
  
  struct board_ht_data* hashed_data;
  if((hashed_data = board_ht_find(ht,b,depth_remaining))){
    if(hashed_data->low >= beta){
      return hashed_data->low;
    }
    if(hashed_data->up <= beta-1){
      return hashed_data->up;
    }
  }
  struct board children[32],*child_end,*it;
  child_end = board_get_children(b,children);
  sort_children(children,child_end);
  for(it=children;it!=child_end && g < beta;it++){
    int cur = -test_alphabeta_ht(it,ht,depth_remaining-1,1-beta);
    g = (cur > g ? cur : g);
  }
  
  
  hashed_data = board_ht_hash(ht,b);
  if(hashed_data->depth <= depth_remaining){
    if(!board_equals(b,&hashed_data->b)){
      hashed_data->b = *b;
      hashed_data->low = -64000;
      hashed_data->up = 64000;
    }
    hashed_data->depth = depth_remaining;
    if(g <= beta-1){
      hashed_data->up = g;
    }
    if(g >= beta){
      hashed_data->low = g;
    }
    
  }
  return g;
}

int test_alphabeta_no_ht(
  struct board* b,
  int depth_remaining,
  int beta
){
  if(depth_remaining == 0){
    return dummy_heur(b);
  }
  else{
    int move,cur,g = -64000;
    uint64_t undo_data,moves = board_get_moves(b);
    while(moves!=0ull && g < beta){ 
      
      move = uint64_find_first(moves);
      
      undo_data = board_do_move(b,move);
      cur = -test_alphabeta_no_ht(b,depth_remaining-1,1-beta);
      board_undo_move(b,move,undo_data);
      
      moves &= uint64_reset[move];
      
      g = (cur > g ? cur : g);
    }
    return g;
  }
}  

#define USE_HT 1

int test_mtdf_ht(const struct board* b,struct board_ht* ht,int depth_remaining,int f){
  
  int g = f;
  
  /*if(depth_remaining<0){
    return g;
  }
  g = test_mtdf_ht(b,ht,depth_remaining-2,g);
  */
  int high = 64000;
  int low = -64000;
  int beta;
  while(low < high){
    beta = g + ((g==low) ? 1 : 0);
    g = test_alphabeta_ht(b,ht,depth_remaining,beta);
    if(g < beta){
      high = g;
    }
    else{
      low = g;
    }
  }
  return g;
  
}




int arg_run_minimax_tests(struct game_config* gc,const struct parse_state* ps){
  (void)ps;
  (void)gc;
  struct board b;
 

  
  struct board_ht* ht = board_ht_new((1024*1024)-1,board_hash,board_equals);
  unsigned i;
  for(unsigned depth=7;depth<=8;depth++){
    for(i=0;i<100;i++){
      board_init(&b);
      board_do_random_moves(&b,10);
      int pvs,mtdf_ht;
      pvs = test_pvs(&b,depth,-64000,64000);
      mtdf_ht = test_mtdf_ht(&b,ht,depth,0);
      if(pvs != mtdf_ht){
        printf("%s","\nALERT! WE HAVE BUGS!\n");
        printf("pvs=%d mtdf_ht=%d\n",pvs,mtdf_ht);
        exit(1);
      }
      else{
        printf("%s",".");
        fflush(stdout);
      }
    }
    printf("%s","\n");
  }
  
  struct bot_stats stats;
  bot_stats_init(&stats);
  int heur;
  
  board_init(&b);
  srand(0);
  //board_do_random_moves(&b,25);
  
  for(i=0;i<60;i++){
    bot_stats_start(&stats);
    heur = test_mtdf_ht(&b,ht,i,0);
    bot_stats_stop(&stats);
    printf("depth %d \t%lf seconds\theur = %d\n",
      i,
      bot_stats_get_seconds(&stats),
      heur
    );
  }
  
  
  printf("%s","\n");
  board_ht_free(ht);
  
  exit(0); // ugly
  
  return 1;
}


