#include "testmm.h"

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

int test_negamax(const struct board* b,int depth_remaining,int is_max){
  if(depth_remaining==0){
    return (is_max ? 1 : -1) * dummy_heur(b);
  }
  int best = INT_MAX;
  struct board children[32];
  struct board *it,*child_end = board_get_children(b,children);
  for(it=children;it!=child_end;it++){
    int cur = -test_negamax(it,depth_remaining-1,!is_max);
    if(cur < best){
      best = cur;
    }
  }
  return best;
}

int test_alphabeta(const struct board* b,int depth_remaining,int alpha,int beta){
  if(depth_remaining==0){
    return -dummy_heur(b);
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


int arg_run_minimax_tests(struct game_config* gc,const struct parse_state* ps){
  (void)ps;
  (void)gc;
  struct board b;
  board_init(&b);
  //board_do_random_moves(&b,10);

  int depth;
  for(depth=1;depth<7;depth++){
    printf("minimax at depth %d:\t%d\n",depth,test_minimax(&b,depth,1));
    printf("negamax at depth %d:\t%d\n",depth,-test_negamax(&b,depth,1));
    printf("alphabeta at depth %d:\t%d\n\n",depth,-test_alphabeta(&b,depth,-64000,64000));
  }
  exit(0); // ugly
  return 1;
}


