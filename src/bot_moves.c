#include "bot_moves.h"

void bot_moves_init(bot_moves* bot)
{
  bot_stats_init(&bot->stats);
  bot->out = stdout;
  bot->name = "moves";
}


int bot_moves_heuristic(const board* b)
{
  int res = board_count_moves(b);
  int opp_move_count = board_count_opponent_moves(b);
  if(res==0 && opp_move_count==0){
    return 1000 * board_get_disc_diff(b);
  }
  res -= opp_move_count;
  
  res += 3 *(
      uint64_count(b->me & 0x8100000000000081) 
      - uint64_count(b->opp & 0x8100000000000081)
  );
  return res;
}

void bot_moves_do_move_normally(bot_moves* bot,const board* b, board* res)
{
  bot_stats_start(&bot->stats);
  
  board children[32];
  int child_count = board_get_children(b,children) - children;
  
  if(board_only_similar_siblings(children,child_count)){
    fprintf(bot->out,"%s sees that all moves are similar\n",bot->name);
    *res = children[0];
    return;
  }
  fprintf(bot->out,"bot_%s searching at depth %d\n",bot->name,bot->depth);

  /*if(get_search_depth() > NORMAL_MOVE_SORT_DEPTH){
    
    moves_left = NORMAL_MOVE_SORT_DEPTH;
    
    int heurs[32];
    for(int i=0;i<child_count;i++){
      inspected = children[i];
      moves_left--;
      heurs[i] = -pvs_unsorted(MIN_HEURISTIC,MAX_HEURISTIC);
      moves_left++;
    }
    ugly_sort<board>(children,heurs,child_count);
    
  } */

  bot->moves_left = bot->depth;
  

  
  int best_heur,best_id=0;
  
  best_heur = MIN_HEURISTIC;
  for(int id=0;id<child_count;++id){
    bot->inspected = children[id];
    bot->moves_left--;
    int cur_heur = -bot_moves_pvs_sorted(bot,MIN_HEURISTIC,-best_heur);
    bot->moves_left++;
    if(cur_heur > best_heur){
      best_heur = cur_heur;
      best_id = id;
    }
    fprintf(bot->out,"move %d/%d: %d\n",id+1,child_count,best_heur);    
  }
  
  *res = children[best_id];
  
  /*if(get_use_book()){
    int move = b->get_move_index(res);
    book_t::value v(move,get_search_depth(),best_heur);
    
    if(book->add(b,&v)){
      output() << "board was added to book\n";
    }
  }*/
  
  bot_stats_stop(&bot->stats);
  fprintf(bot->out,"%llu nodes in %lf seconds: %llu nodes/sec\n",
    bot->stats.nodes,
    bot_stats_get_seconds(&bot->stats),
    bot_stats_get_nodes_per_second(&bot->stats)
  );  
}

void bot_moves_do_move_perfectly(bot_moves* bot,const board* b, board* res)
{
  bot_stats_start(&bot->stats);
  
  board children[32];
  int child_count = board_get_children(b,children) - children;
  fprintf(bot->out,"bot_%s searching perfectly at depth %d\n",
          bot->name,
          uint64_count(~(b->me | b->opp))
  );
  /*if(moves_left > PERFECT_MOVE_SORT_DEPTH){
    
    moves_left = PERFECT_MOVE_SORT_DEPTH;
    
    int heurs[32];
    for(int i=0;i<child_count;i++){
      inspected = children[i];
      heurs[i] = -pvs_sorted(MIN_HEURISTIC,MAX_HEURISTIC);
    }
    ugly_sort<board>(children,heurs,child_count);
    
  }*/
  
  uint64_t empty_fields = ~(bot->inspected.me | bot->inspected.opp);
  bot->moves_left = 64 - uint64_count(empty_fields);
  
  int best_id = 0;
  int best_heur = MIN_PERFECT_HEURISTIC;
  for(int id=0;id<child_count;++id){
    bot->inspected = children[id];
    int cur_heur = -bot_moves_pvs_exact(bot,MIN_PERFECT_HEURISTIC,-best_heur);
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

void bot_moves_set_level(bot_moves* bot, int depth, int perfect_depth)
{
  bot->depth = depth;
  bot->perfect_depth = perfect_depth;
}




void bot_moves_do_move(bot_moves* bot,const board* b,board* res)
{
  if(board_count_moves(b) == 1){
    bot_moves_do_move_one_possibility(bot,b,res);
    return;
  }
  
  /*if(do_move_book(b,res)){
    return;
  }*/
  
  if(uint64_count(~(b->me | b->opp)) > bot->perfect_depth){
    bot_moves_do_move_normally(bot,b,res);
    return;
  }
  
  bot_moves_do_move_perfectly(bot,b,res);
}

int bot_moves_pvs_sorted(bot_moves* bot,int alpha, int beta)
{
  bot->stats.nodes++;
  
  if(bot->moves_left < bot_moves_max_sort_depth){
    return bot_moves_pvs_unsorted(bot,alpha,beta);
  }
  
  if(bot->moves_left == 0){
    return bot_moves_heuristic(&bot->inspected);
  }
  
  /*if(get_use_book()){
    book_t::value bv = book->lookup(&inspected,moves_left);
    if(bv.best_move != book_t::NOT_FOUND){
      return min(max(bv.heur,alpha),beta);
    }
  }*/
  
  uint64_t valid_moves = board_get_moves(&bot->inspected);
  
  if(valid_moves == 0ull){
    int heur;
    board_switch_turn(&bot->inspected);
    if(board_get_moves(&bot->inspected) == 0ull){
      heur = -1000 * board_get_disc_diff(&bot->inspected);    
    }
    else{
      heur = -bot_moves_pvs_sorted(bot,-beta,-alpha);
    }
    board_switch_turn(&bot->inspected);
    return heur;
  }
    
  board children[32]; 
  int child_count = board_get_children(&bot->inspected,children) - children;
  
  // this branch is almost never taken
  /*if(board::only_similar_siblings(children,child_count)){
    std::swap<board>(inspected,children[0]);
    moves_left--;
    int score = -pvs_sorted(-beta,-alpha);
    moves_left++;
    std::swap<board>(inspected,children[0]);
    return score;
  }
  
  
  do_sorting(children,child_count);
  */
  
  {
    board tmp = bot->inspected;
    bot->inspected = children[0];
    children[0] = tmp;
  }
    
  bot->moves_left--;
  int score = -bot_moves_pvs_sorted(bot,-beta,-alpha);
  bot->moves_left++;
  {
    board tmp = bot->inspected;
    bot->inspected = children[0];
    children[0] = tmp;
  }
  
  if(score >= beta){
    return beta;
  }
  if(score >= alpha){
    alpha = score;
  }
      
    
    
  for(int i=1;i<child_count;i++){
    
    {
      board tmp = bot->inspected;
      bot->inspected = children[i];
      children[i] = tmp;
    }
    bot->moves_left--;
    score = -bot_moves_pvs_null_window(bot,-alpha-1);
    if((alpha < score) && (score < beta)){
      score = -bot_moves_pvs_sorted(bot,-beta,-score);
    } 
    bot->moves_left++;
    {
      board tmp = bot->inspected;
      bot->inspected = children[i];
      children[i] = tmp;
    } 
    
    if(score >= beta){
      return beta;
    }
    if(score >= alpha){
      alpha = score;
    }
  }
  return alpha;
}

void bot_moves_do_move_one_possibility(bot_moves* bot, const board* b, board* res)
{
 board children[32];
  board_get_children(b,children);
  *res = children[0];
  fprintf(bot->out,"%s","only one valid move found, evaluation skipped.\n");
}

int bot_moves_pvs_exact(bot_moves* bot,int alpha, int beta)
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
      heur = -bot_moves_pvs_exact(bot,-beta,-alpha);
    }
    board_switch_turn(&bot->inspected);
    return heur;
  }

  int move = uint64_find_first(valid_moves);
  
  uint64_t undo_data = board_do_move(&bot->inspected,move);
  int score = -bot_moves_pvs_exact(bot,-beta,-alpha);
  board_undo_move(&bot->inspected,move,undo_data);
  
  if(score >= beta){
    return beta;
  }
  if(score >= alpha){
    alpha = score;
  }
  valid_moves &= uint64_reset[move];
  
  while(valid_moves != 0ull){
    
    move = uint64_find_first(valid_moves);
    
    undo_data = board_do_move(&bot->inspected,move);
    
    score = -bot_moves_pvs_exact_null_window(bot,-alpha-1);
    if((alpha < score) && (score < beta)){
      score = -bot_moves_pvs_exact(bot,-beta,-score);
    }
    
    board_undo_move(&bot->inspected,move,undo_data);
    
    if(score >= beta){
      return beta;
    }
    if(score >= alpha){
      alpha = score;
    }
    valid_moves &= uint64_reset[move];
  }
  return alpha;
}

int bot_moves_pvs_exact_null_window(bot_moves* bot,int alpha)
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
      heur = -bot_moves_pvs_exact_null_window(bot,-(alpha+1));
    }
    board_switch_turn(&bot->inspected);
    return heur;
  }

  int move = uint64_find_first(valid_moves);
  
  uint64_t undo_data = board_do_move(&bot->inspected,move);
  int score = -bot_moves_pvs_exact_null_window(bot,-(alpha+1));
  board_undo_move(&bot->inspected,move,undo_data);
  
  if(score > alpha){
    return alpha+1;
  }
  valid_moves &= uint64_reset[move];
  
  while(valid_moves != 0ull){
    
    move = uint64_find_first(valid_moves);
    
    undo_data = board_do_move(&bot->inspected,move);
    score = -bot_moves_pvs_exact_null_window(bot,-alpha-1);
    board_undo_move(&bot->inspected,move,undo_data);
    
    if(score > alpha){
      return alpha+1;
    }
    valid_moves &= uint64_reset[move];
  }
  return alpha;
}

int bot_moves_pvs_unsorted(bot_moves* bot, int alpha, int beta)
{
  bot->stats.nodes++;
  
  if(bot->moves_left == 0){
    return bot_moves_heuristic(&bot->inspected);
  }
  
  uint64_t valid_moves = board_get_moves(&bot->inspected);

  if(valid_moves == 0ull){
    int heur;
    board_switch_turn(&bot->inspected);
    if(board_get_moves(&bot->inspected) == 0ull){
      heur = -1000 * board_get_disc_diff(&bot->inspected);    
    }
    else{
      heur = -bot_moves_pvs_unsorted(bot,-beta,-alpha);
    }
    board_switch_turn(&bot->inspected);
    return heur;
  }

  int move = uint64_find_first(valid_moves);
  uint64_t undo_data = board_do_move(&bot->inspected,move);
  bot->moves_left--;
  int score = -bot_moves_pvs_unsorted(bot,-beta,-alpha);
  bot->moves_left++;
  board_undo_move(&bot->inspected,move,undo_data);

  if(score >= beta){
    return beta;
  }
  if(score >= alpha){
    alpha = score;
  }
  valid_moves &= uint64_reset[move];
  
  while(valid_moves != 0ull){
    move = uint64_find_first(valid_moves);
    undo_data = board_do_move(&bot->inspected,move);
    
    bot->moves_left--;
    score = -bot_moves_pvs_null_window(bot,-alpha-1);
    if((alpha < score) && (score < beta)){
      score = -bot_moves_pvs_unsorted(bot,-beta,-score);
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
  }
  return alpha;
}

int bot_moves_pvs_null_window(bot_moves* bot, int alpha)
{
  bot->stats.nodes++;  
 
  if(bot->moves_left == 0){
    return bot_moves_heuristic(&bot->inspected);
  }
  
  uint64_t valid_moves = board_get_moves(&bot->inspected);

  if(valid_moves == 0ull){
    int heur;
    board_switch_turn(&bot->inspected);
    if(board_get_moves(&bot->inspected) == 0ull){
      heur = -1000 * board_get_disc_diff(&bot->inspected);    
    }
    else{
      heur = -bot_moves_pvs_null_window(bot,-(alpha+1));
    }
    board_switch_turn(&bot->inspected);
    return heur;
  }
  
  while(valid_moves != 0ull){
    int move = uint64_find_first(valid_moves);
    uint64_t undo_data = board_do_move(&bot->inspected,move);
    bot->moves_left--;
    int score = -bot_moves_pvs_null_window(bot,-(alpha+1));
    bot->moves_left++;
    board_undo_move(&bot->inspected,move,undo_data);
    
    if(score > alpha){
      return alpha+1;
    }
    valid_moves &= uint64_reset[move];
  }
  return alpha;
  
}
