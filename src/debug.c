#include <assert.h>
#include <stdio.h>

#include "board.h"
#include "macros.h"
#include "bot_moves.h"

void debug_bot_moves_do_move_normally(bot_moves* bot,const board* b, board* res)
{
  bot_stats_start(&bot->stats);
  
  board children[32];
  int child_count = board_get_children(b,children) - children;

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
  }
  *res = children[best_id];
}

int main(){
  FILE* devnull = fopen("/dev/null","w");
  bot_moves bot;
  bot_moves_init(&bot);
  bot_moves_set_level(&bot,13,18);
  bot.out = devnull;
  board b,res;
  board_init(&b);
  debug_bot_moves_do_move_normally(&bot,&b,&res);
  return 0;  
}
