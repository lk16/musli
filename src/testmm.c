#include "testmm.h"

int dummy_heur(const struct board* b){
  int me_move_count = board_count_moves(b);
  int opp_move_count = board_count_opponent_moves(b);
  if(me_move_count==0 && opp_move_count==0){
    return 1000 * board_get_disc_diff(b);
  }
  int res = me_move_count - opp_move_count;
  
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
  if(!ht || depth_remaining < 4){
    struct board copy = *b;
    return test_alphabeta_no_ht(&copy,depth_remaining,beta);
  }
  int g = MIN_HEURISTIC;
  
  
  int hash_table_heur = test_alphabeta_get_hashed_data(b,ht,depth_remaining,beta);
  if(hash_table_heur != HASHED_VALUE_NO_EXACT_RESULT){
    return hash_table_heur;
  }
  
  
  
  struct board children[32],*child_end,*it;
  child_end = board_get_children(b,children);
  sort_children(children,child_end);
  for(it=children;it!=child_end && g < beta;it++){
    int cur = -test_alphabeta_ht(it,ht,depth_remaining-1,1-beta);
    g = (cur > g ? cur : g);
  }
  
  
  struct board_ht_data* hashed_data = board_ht_hash(ht,b);
  test_alphabeta_update_hashed_data(b,g,beta,hashed_data,depth_remaining);
  
  return g;
}

int test_alphabeta_get_hashed_data(
  const struct board* b,
  struct board_ht* ht,
  int depth_remaining,
  int beta
){
  struct board_ht_data* hashed_data;
  if((hashed_data = board_ht_find(ht,b,depth_remaining))){
    if(hashed_data->low >= beta){
      return hashed_data->low;
    }
    if(hashed_data->up <= beta-1){
      return hashed_data->up;
    }
  }  
  return HASHED_VALUE_NO_EXACT_RESULT;
}


void test_alphabeta_update_hashed_data(
  const struct board* b,
  int g,
  int beta,
  struct board_ht_data* hashed_data,
  int depth_remaining
){
  if(hashed_data->depth == depth_remaining){
    if(!board_equals(b,&hashed_data->b)){
      hashed_data->b = *b;
      hashed_data->low = MIN_HEURISTIC;
      hashed_data->up = MAX_HEURISTIC;
    }
    hashed_data->depth = depth_remaining;
    if(g <= beta-1){
      hashed_data->up = g;
    }
    if(g >= beta){
      hashed_data->low = g;
    }
  }
}


int test_alphabeta_no_ht(
  struct board* b,
  int depth_remaining,
  int beta
){
   uint64_t moves = board_get_moves(b);
  
  if(moves == 0ull){
    int heur;
    struct board copy = *b;
    board_switch_turn(&copy);
    if(board_get_moves(&copy) == 0ull){
      heur = -1000 * board_get_disc_diff(&copy);    
    }
    else{
      heur = -test_alphabeta_no_ht(&copy,depth_remaining,1-beta);
    }
    return heur;
  }
  if(depth_remaining == 0){
    return dummy_heur(b);
  }
  else{
    int move,cur,g = MIN_HEURISTIC;
    uint64_t undo_data = board_get_moves(b);
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

int test_mtdf_ht(const struct board* b,struct board_ht* ht,int depth_remaining,int f){
  
  int g = f;
  
  if(depth_remaining>2){
    g = test_mtdf_ht(b,ht,depth_remaining-2,g);
  }
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
 

  
  struct board_ht* ht = board_ht_new((16*1024*1024)-1,board_hash,board_equals);
  unsigned i;
#if 0
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
#endif
  
  struct bot_stats stats;
  bot_stats_init(&stats);
  int heur;
  
  board_init(&b);
  //srand(0);
  //board_do_random_moves(&b,48);
  board_print(&b,stdout,0);
  
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


