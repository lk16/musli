#include "mtdf_helper.h"


void mtdf_helper_init(struct mtdf_helper* m,int(*heur_func)(const struct board*),
                     const char* name)
{
  bot_stats_init(&m->stats);
  m->out = stdout;
  m->hashtable = board_ht_new((1024*1024*16)-1,board_hash,board_equals);
  m->heuristic = heur_func;
  m->name = name;
}

int mtdf_helper_alphabeta_main(struct mtdf_helper* m, int f)
{
  int g = f;
  m->moves_left = m->depth;
  
  /*if(depth_remaining>2){
    g = test_mtdf_ht(b,ht,depth_remaining-2,g);
  }*/

  int high = 64000;
  int low = -64000;
  int beta;
  while(low < high){
    beta = g + ((g==low) ? 1 : 0);
    g = mtdf_helper_alphabeta_ht(m,beta);
    if(g < beta){
      high = g;
    }
    else{
      low = g;
    }
  }
  return g;
}

int mtdf_helper_alphabeta_exact_main(struct mtdf_helper* m, int f)
{
  int g = f;
  
  /*if(depth_remaining>2){
    g = test_mtdf_ht(b,ht,depth_remaining-2,g);
  }*/

  int high = 64;
  int low = -64;
  int beta;
  while(low < high){
    beta = g + ((g==low) ? 1 : 0);
    g = mtdf_helper_alphabeta_exact_ht(m,beta);
    if(g < beta){
      high = g;
    }
    else{
      low = g;
    }
  }
  return g;
}

void mtdf_helper_do_move_normally(struct mtdf_helper* m,const struct board* b, struct board* res)
{
  bot_stats_start(&m->stats);
  
  struct board children[32];
  int child_count = board_get_children(b,children) - children;
  
  fprintf(m->out,"bot_%s searching at depth %d\n",m->name,m->depth);

  m->moves_left = m->depth;
  
  int best_heur,best_id=0;
  
  best_heur = MIN_HEURISTIC;
  for(int id=0;id<child_count;++id){
    m->inspected = children[id];
    m->moves_left--;
    int cur_heur = mtdf_helper_alphabeta_main(m,-best_heur);
    m->moves_left++;
    if(cur_heur > best_heur){
      best_heur = cur_heur;
      best_id = id;
    }
    fprintf(m->out,"move %d/%d: %d\n",id+1,child_count,best_heur);    
  }
  
  *res = children[best_id];
  
  bot_stats_stop(&m->stats);
  fprintf(m->out,"%llu nodes in %lf seconds: %llu nodes/sec\n",
    m->stats.nodes,
    bot_stats_get_seconds(&m->stats),
    bot_stats_get_nodes_per_second(&m->stats)
  );  
}

void mtdf_helper_do_move_perfectly(struct mtdf_helper* m,const struct board* b, struct board* res)
{
  bot_stats_start(&m->stats);
  
  struct board children[32];
  int child_count = board_get_children(b,children) - children;
  fprintf(m->out,"bot_%s searching perfectly at depth %d\n",
          m->name,
          uint64_count(~(b->me | b->opp))
  );
    
  uint64_t empty_fields = ~(m->inspected.me | m->inspected.opp);
  m->moves_left = 64 - uint64_count(empty_fields);
  
  int best_id = 0;
  int best_heur = MIN_PERFECT_HEURISTIC;
  for(int id=0;id<child_count;++id){
    m->inspected = children[id];
    int cur_heur = mtdf_helper_alphabeta_exact_main(m,-best_heur);
    if(cur_heur > best_heur){
      best_heur = cur_heur;
      best_id = id;
    }
    fprintf(m->out,"move %d/%d: %d\n",id+1,child_count,best_heur);
  }
  
  *res = children[best_id];
  
  bot_stats_stop(&m->stats);

  fprintf(m->out,"%llu nodes in %lf seconds: %llu nodes/sec\n",
          m->stats.nodes,
          bot_stats_get_seconds(&m->stats),
          bot_stats_get_nodes_per_second(&m->stats)
  );
}

void mtdf_helper_set_level(struct mtdf_helper* m, int depth, int perfect_depth)
{
  m->depth = depth;
  m->perfect_depth = perfect_depth;
}




void mtdf_helper_do_move(struct mtdf_helper* m,const struct board* b,struct board* res)
{
  if(board_count_moves(b) == 1){
    mtdf_helper_do_move_one_possibility(m,b,res);
    return;
  }
  if(uint64_count(~(b->me | b->opp)) > m->perfect_depth){
    mtdf_helper_do_move_normally(m,b,res);
    return;
  }  
  mtdf_helper_do_move_perfectly(m,b,res);
}

void mtdf_helper_do_move_one_possibility(struct mtdf_helper* bot, const struct board* b, struct board* res)
{
  struct board children[32];
  board_get_children(b,children);
  *res = children[0];
  fprintf(bot->out,"%s","only one valid move found, evaluation skipped.\n");
}




int mtdf_helper_alphabeta_ht(
  struct mtdf_helper* m,
  int beta
){
  const struct board* b = &m->inspected;
  struct board_ht* ht = m->hashtable;
  int depth_remaining = m->moves_left;
  
  if(!ht || depth_remaining < MTDF_MIN_HASHTABLE_DEPTH){
    return mtdf_helper_alphabeta_no_ht(m,beta);
  }
  int g = MIN_HEURISTIC;
  
  
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
  //sort_children(children,child_end);
  for(it=children;it!=child_end && g < beta;it++){
    m->moves_left--;
    int cur = -mtdf_helper_alphabeta_ht(m,1-beta);
    m->moves_left++;
    g = (cur > g ? cur : g);
  }
  
  
  hashed_data = board_ht_hash(ht,b);
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
  return g;
}

int mtdf_helper_alphabeta_no_ht(
  struct mtdf_helper* m,
  int beta
){
  struct board* b = &m->inspected;
  int depth_remaining = m->moves_left;
  
  if(depth_remaining == 0){
    return mtdf_helper_heuristic(b);
  }
  else{
    int move,cur,g = MIN_HEURISTIC;
    uint64_t undo_data,moves = board_get_moves(b);
    while(moves!=0ull && g < beta){ 
      
      move = uint64_find_first(moves);
      
      undo_data = board_do_move(b,move);
      m->moves_left--;
      cur = -mtdf_helper_alphabeta_no_ht(m,1-beta);
      m->moves_left++;
      board_undo_move(b,move,undo_data);
      
      moves &= uint64_reset[move];
      
      g = (cur > g ? cur : g);
    }
    return g;
  }
}  


