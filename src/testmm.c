#include "testmm.h"

struct lame_ht* lame_ht_new(unsigned(*hash)(const struct board*))
{
  struct lame_ht* res = malloc(sizeof(*res));
  memset(res->data,0,LAME_HT_SIZE*sizeof(*res->data));
  res->hash = hash;
  return res;
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

void lame_ht_insert(struct lame_ht* ht, const struct board* b, unsigned int low, unsigned int high)
{
  struct lame_ht_data* ptr = ht->data + (ht->hash(b) % LAME_HT_SIZE);
  ptr->b = *b;
  ptr->low = low;
  ptr->high = high;
}





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



int test_minimax(const struct board* b,int depth_remaining,int is_max){
  if(depth_remaining==0){
    return (is_max ? 1 : -1) * dummy_heur(b);
  }
  int best;
  struct board children[32];
  struct board *it,*child_end = board_get_children(b,children);
  if(is_max){
    best = INT_MIN;
    for(it=children;it!=child_end;it++){
      int cur = test_minimax(it,depth_remaining-1,!is_max);
      if(cur > best){
        best = cur;
      }
    }
  }
  else{
    best = INT_MAX;
    for(it=children;it!=child_end;it++){
      int cur = test_minimax(it,depth_remaining-1,!is_max);
      if(cur < best){
        best = cur;
      }
    }
  }
  return best;
}

int test_negamax(const struct board* b,int depth_remaining){
  if(depth_remaining==0){
    return dummy_heur(b);
  }
  int best = INT_MIN;
  struct board children[32];
  struct board *it,*child_end = board_get_children(b,children);
  for(it=children;it!=child_end;it++){
    int cur = -test_negamax(it,depth_remaining-1);
    if(cur > best){
      best = cur;
    }
  }
  return best;
}

int test_alphabeta(const struct board* b,int depth_remaining,int alpha,int beta){
  if(depth_remaining==0){
    return dummy_heur(b);
  }
  struct board children[32];
  struct board *it,*child_end = board_get_children(b,children);
  for(it=children;it!=child_end;it++){
    int cur = -test_alphabeta(it,depth_remaining-1,-beta,-alpha);
    if(cur > alpha){
      alpha = cur;
    }
    if(alpha >= beta){
      break;
    }
  }
  return alpha;
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

int test_mtdf(const struct board* b,int depth_remaining,int f){
  int g = f;
  int high = 64000;
  int low = -64000;
  int beta;
  while(low < high){
    if(g == low){
      beta = g + 1;
    }
    else{
      beta = g;
    }
    g = test_pvs(b,depth_remaining,beta-1,beta);
    if(g < beta){
      high = g;
    }
    else{
      low = g;
    }
  }
  return g;  
}

int test_pvs_ht(const struct board* b,struct lame_ht* ht,int depth_remaining,int alpha,int beta){
  if(depth_remaining==0){
    return dummy_heur(b);
  }
  struct board children[32];
  struct board *it,*child_end = board_get_children(b,children);
  for(it=children;it!=child_end;it++){
    int score;
    if(it==children){
      score = -test_pvs_ht(it,ht,depth_remaining-1,-beta,-alpha);
    }
    else{ 
      score = -test_pvs_ht(it,ht,depth_remaining-1,-alpha-1,-alpha);
      if((alpha < score) && (score < beta)  ){
        score = -test_pvs_ht(it,ht,depth_remaining-1,-beta,-score);
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


int test_mtdf_ht(const struct board* b,struct lame_ht* ht,int depth_remaining,int f){
  int g = f;
  int high = 64000;
  int low = -64000;
  int beta;
  while(low < high){
    if(g == low){
      beta = g + 1;
    }
    else{
      beta = g;
    }
    g = test_pvs_ht(b,ht,depth_remaining,beta-1,beta);
    if(g < beta){
      high = g;
    }
    else{
      low = g;
    }
  }
  return g;  
}

unsigned int lame_board_hash(const struct board* b)
{
  return b->me + (b->opp*37);
}



int arg_run_minimax_tests(struct game_config* gc,const struct parse_state* ps){
  (void)ps;
  (void)gc;
  struct board b;
  board_init(&b);
  board_do_random_moves(&b,10);
  
  enum{
    MINIMAX_MAX_DEPTH = 6,
    NEGAMAX_MAX_DEPTH = 6,
    ALPHA_BETA_MAX_DEPTH = 8,
    PVS_MAX_DEPTH = 9,
    MTDF_MAX_DEPTH = 11
  };
  
  struct lame_ht* ht = lame_ht_new(lame_board_hash);
  
  int depth;
  for(depth=1;depth<60;depth++){
    if(depth > MTDF_MAX_DEPTH){ continue; }
    printf("%s","\n");
    printf("mtdf_ht at depth %d\t%d\n",depth,test_mtdf_ht(&b,ht,depth,0));    
    printf("mtdf at depth %d:\t%d\n",depth,test_mtdf(&b,depth,0));
    if(depth > PVS_MAX_DEPTH){ continue; }
    printf("pvs at depth %d:\t\t%d\n",depth,test_pvs(&b,depth,-64000,64000));
    if(depth > ALPHA_BETA_MAX_DEPTH){ continue; }
    printf("alphabeta at depth %d:\t%d\n",depth,test_alphabeta(&b,depth,-64000,64000));
    if(depth > NEGAMAX_MAX_DEPTH){ continue; }
    printf("negamax at depth %d:\t%d\n",depth,test_negamax(&b,depth));
    if(depth > MINIMAX_MAX_DEPTH){ continue; }
    printf("minimax at depth %d:\t%d\n",depth,test_minimax(&b,depth,1));
  }
  printf("%s","\n");
  
  lame_ht_free(ht);
  
  exit(0); // ugly
  return 1;
}


