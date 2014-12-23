#include "pvs_helper.h"

void pvs_helper_init(struct pvs_helper* bot,int(*heur_func)(const struct board*),
                     const char* name)
{
  bot_stats_init(&bot->stats);
  bot->out = stdout;
  hash_table_init(&bot->table);
  bot->heuristic = heur_func;
  bot->name = name;
}




void pvs_helper_do_move_normally(struct pvs_helper* bot,const struct board* b, struct board* res)
{
  bot_stats_start(&bot->stats);
  
  struct board children[32];
  int child_count = board_get_children(b,children) - children;
  
  fprintf(bot->out,"bot_%s searching at depth %d\n",bot->name,bot->depth);

  bot->moves_left = bot->depth;
  
  int best_heur,best_id=0;
  
  hash_table_clear(&bot->table);
  
  best_heur = MIN_HEURISTIC;
  for(int id=0;id<child_count;++id){
    bot->inspected = children[id];
    bot->moves_left--;
    int cur_heur = -pvs_helper_pvs_sorted(bot,MIN_HEURISTIC,-best_heur);
    bot->moves_left++;
    if(cur_heur > best_heur){
      best_heur = cur_heur;
      best_id = id;
    }
    fprintf(bot->out,"move %d/%d: %d\n",id+1,child_count,best_heur);    
  }
  
  *res = children[best_id];
  
  bot_stats_stop(&bot->stats);
  fprintf(bot->out,"%llu nodes in %lf seconds: %llu nodes/sec\n",
    bot->stats.nodes,
    bot_stats_get_seconds(&bot->stats),
    bot_stats_get_nodes_per_second(&bot->stats)
  );  
}

void pvs_helper_do_move_perfectly(struct pvs_helper* bot,const struct board* b, struct board* res)
{
  bot_stats_start(&bot->stats);
  
  struct board children[32];
  int child_count = board_get_children(b,children) - children;
  fprintf(bot->out,"bot_%s searching perfectly at depth %d\n",
          bot->name,
          uint64_count(~(b->me | b->opp))
  );
    
  uint64_t empty_fields = ~(bot->inspected.me | bot->inspected.opp);
  bot->moves_left = 64 - uint64_count(empty_fields);
  
  int best_id = 0;
  int best_heur = MIN_PERFECT_HEURISTIC;
  for(int id=0;id<child_count;++id){
    bot->inspected = children[id];
    int cur_heur = -pvs_helper_pvs_exact(bot,MIN_PERFECT_HEURISTIC,-best_heur);
    if(cur_heur > best_heur){
      best_heur = cur_heur;
      best_id = id;
    }
    fprintf(bot->out,"move %d/%d: %d\n",id+1,child_count,best_heur);
  }
  
  *res = children[best_id];
  
  bot_stats_stop(&bot->stats);

  fprintf(bot->out,"%llu nodes in %lf seconds: %llu nodes/sec\n",
          bot->stats.nodes,
          bot_stats_get_seconds(&bot->stats),
          bot_stats_get_nodes_per_second(&bot->stats)
  );
}

void pvs_helper_set_level(struct pvs_helper* bot, int depth, int perfect_depth)
{
  bot->depth = depth;
  bot->perfect_depth = perfect_depth;
}




void pvs_helper_do_move(struct pvs_helper* bot,const struct board* b,struct board* res)
{
  if(board_count_moves(b) == 1){
    pvs_helper_do_move_one_possibility(bot,b,res);
    return;
  }
  if(uint64_count(~(b->me | b->opp)) > bot->perfect_depth){
    pvs_helper_do_move_normally(bot,b,res);
    return;
  }
  
  pvs_helper_do_move_perfectly(bot,b,res);
}

int pvs_helper_pvs_sorted(struct pvs_helper* bot,int alpha, int beta)
{
  bot->stats.nodes++;
  
  if(bot->moves_left < pvs_helper_max_sort_depth){
    return pvs_helper_pvs_unsorted(bot,alpha,beta);
  }
  
  if(bot->moves_left == 0){
    return bot->heuristic(&bot->inspected);
  }
  
  {
    const struct hash_table_value* hv;
    hv = hash_table_find(&bot->table,&bot->inspected);
    if(hv){
      return hv->heur;
    }
  }
  
  
  uint64_t valid_moves = board_get_moves(&bot->inspected);
  
  if(valid_moves == 0ull){
    int heur;
    board_switch_turn(&bot->inspected);
    if(board_get_moves(&bot->inspected) == 0ull){
      heur = -1000 * board_get_disc_diff(&bot->inspected);    
    }
    else{
      heur = -pvs_helper_pvs_sorted(bot,-beta,-alpha);
    }
    board_switch_turn(&bot->inspected);
    return heur;
  }
    
  struct board_with_heur children[32]; 
  int child_count = board_get_children_with_heur(&bot->inspected,children) - children;
  {
    int moves_left = 4;
    struct board inspected;
    generic_swap(moves_left,bot->moves_left);
    for(int i=0;i<child_count;i++){
      inspected = children[i].b;
      generic_swap(inspected,bot->inspected);
      children[i].heur = -pvs_helper_pvs_unsorted(bot,MIN_HEURISTIC,MAX_HEURISTIC);
      generic_swap(inspected,bot->inspected);
    }
    qsort(children,child_count,sizeof(struct board_with_heur),board_with_heur_compare);
    generic_swap(moves_left,bot->moves_left);
  }
  
  
  int score;
    
  for(int i=child_count-1;i>=0;i--){
    generic_swap(bot->inspected,children[i].b);
    bot->moves_left--;
    if(i==0){
      score = -pvs_helper_pvs_sorted(bot,-beta,-alpha);
    }
    else{
      score = -pvs_helper_pvs_null_window(bot,-alpha-1);
      if((alpha < score) && (score < beta)){
        score = -pvs_helper_pvs_sorted(bot,-beta,-score);
      } 
    }
    
    bot->moves_left++;
    generic_swap(bot->inspected,children[i].b);
    
    if(score >= beta){
      alpha = beta;
      break;
    }
    if(score >= alpha){
      alpha = score;
    }
  }
  
  {
    struct hash_table_value hv;
    hv.heur = alpha;
    hash_table_add(&bot->table,&bot->inspected,&hv);
  }
  return alpha;
}

void pvs_helper_do_move_one_possibility(struct pvs_helper* bot, const struct board* b, struct board* res)
{
  struct board children[32];
  board_get_children(b,children);
  *res = children[0];
  fprintf(bot->out,"%s","only one valid move found, evaluation skipped.\n");
}

int pvs_helper_pvs_exact(struct pvs_helper* bot,int alpha, int beta)
{
  bot->stats.nodes++;
  
  uint64_t valid_moves = board_get_moves(&bot->inspected);

  if(valid_moves == 0ull){
    int heur;
    board_switch_turn(&bot->inspected);
    if(board_get_moves(&bot->inspected) == 0ull){
      heur = -board_get_disc_diff(&bot->inspected);    
    }
    else{
      heur = -pvs_helper_pvs_exact(bot,-beta,-alpha);
    }
    board_switch_turn(&bot->inspected);
    return heur;
  }

  int loop = 0;
  while(valid_moves != 0ull){
    
    int move = uint64_find_first(valid_moves);
    
    uint64_t undo_data = board_do_move(&bot->inspected,move);
    
    int score;
    if(loop==0){
      score = -pvs_helper_pvs_exact(bot,-beta,-alpha);
    }
    else{
      score = -pvs_helper_pvs_exact_null_window(bot,-alpha-1);
      if((alpha < score) && (score < beta)){
        score = -pvs_helper_pvs_exact(bot,-beta,-score);
      }
    }
    board_undo_move(&bot->inspected,move,undo_data);
    
    if(score >= beta){
      return beta;
    }
    if(score >= alpha){
      alpha = score;
    }
    valid_moves &= uint64_reset[move];
    loop++;
  }
  return alpha;
}

int pvs_helper_pvs_exact_null_window(struct pvs_helper* bot,int alpha)
{
  bot->stats.nodes++;
  
  uint64_t valid_moves = board_get_moves(&bot->inspected);

  if(valid_moves == 0ull){
    int heur;
    board_switch_turn(&bot->inspected);
    if(board_get_moves(&bot->inspected) == 0ull){
      heur = -board_get_disc_diff(&bot->inspected);    
    }
    else{
      heur = -pvs_helper_pvs_exact_null_window(bot,-(alpha+1));
    }
    board_switch_turn(&bot->inspected);
    return heur;
  }

  
  while(valid_moves != 0ull){
    
    int move = uint64_find_first(valid_moves);
    
    uint64_t undo_data = board_do_move(&bot->inspected,move);
    int score = -pvs_helper_pvs_exact_null_window(bot,-alpha-1);
    board_undo_move(&bot->inspected,move,undo_data);
    
    if(score > alpha){
      return alpha+1;
    }
    valid_moves &= uint64_reset[move];
  }
  return alpha;
}

int pvs_helper_pvs_unsorted(struct pvs_helper* bot, int alpha, int beta)
{
  bot->stats.nodes++;
  
  if(bot->moves_left == 0){
    return bot->heuristic(&bot->inspected);
  }
  
  uint64_t valid_moves = board_get_moves(&bot->inspected);

  if(valid_moves == 0ull){
    int heur;
    board_switch_turn(&bot->inspected);
    if(board_get_moves(&bot->inspected) == 0ull){
      heur = -1000 * board_get_disc_diff(&bot->inspected);    
    }
    else{
      heur = -pvs_helper_pvs_unsorted(bot,-beta,-alpha);
    }
    board_switch_turn(&bot->inspected);
    return heur;
  }


  int loop = 0;
  while(valid_moves != 0ull){
    int move = uint64_find_first(valid_moves);
    uint64_t undo_data = board_do_move(&bot->inspected,move);
    
    bot->moves_left--;
    int score;
    if(loop==0){
      score = -pvs_helper_pvs_unsorted(bot,-beta,-alpha);
    }
    else{
      score = -pvs_helper_pvs_null_window(bot,-alpha-1);
      if((alpha < score) && (score < beta)){
        score = -pvs_helper_pvs_unsorted(bot,-beta,-score);
      }
    }
    bot->moves_left++;
    
    board_undo_move(&bot->inspected,move,undo_data);
    
    if(score >= beta){
      return beta;
    }
    if(score >= alpha){
      alpha = score;
    }
    valid_moves &= uint64_reset[move];
    loop++;
  }
  return alpha;
}

int pvs_helper_pvs_null_window(struct pvs_helper* bot, int alpha)
{
  bot->stats.nodes++;  
 
  if(bot->moves_left == 0){
    return bot->heuristic(&bot->inspected);
  }
  
  uint64_t valid_moves = board_get_moves(&bot->inspected);

  if(valid_moves == 0ull){
    int heur;
    board_switch_turn(&bot->inspected);
    if(board_get_moves(&bot->inspected) == 0ull){
      heur = -1000 * board_get_disc_diff(&bot->inspected);    
    }
    else{
      heur = -pvs_helper_pvs_null_window(bot,-(alpha+1));
    }
    board_switch_turn(&bot->inspected);
    return heur;
  }
  
  while(valid_moves != 0ull){
    int move = uint64_find_first(valid_moves);
    uint64_t undo_data = board_do_move(&bot->inspected,move);
    bot->moves_left--;
    int score = -pvs_helper_pvs_null_window(bot,-(alpha+1));
    bot->moves_left++;
    board_undo_move(&bot->inspected,move,undo_data);
    
    if(score > alpha){
      return alpha+1;
    }
    valid_moves &= uint64_reset[move];
  }
  return alpha;
  
}
